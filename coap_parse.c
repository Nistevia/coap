/* coap_parse.c -- implementation for packet decoding.
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Fri Apr 10 09:54:34 EEST 2015
 * Disclaimer: (C) LUIS COLORADO.  All rights reserved.
 */

#include <assert.h>
#include "coapP.h"

coap_err
coap_parse_unsigned(
        const uint8_t  *buff,
        size_t          bufsz,
        uint32_t       *valref)
{
    uint32_t res = 0;
    DEB(D("BEGIN(bufsz=%ld)\n"), bufsz);
    while (bufsz--) {
        res <<= 8;
        res |= *buff++;
    } /* while */
    if (valref) *valref = res;
    DEB(D("RETURN ==> COAP_OK (res=0x%x,%d)\n"), res, res);
    return COAP_OK;
} /* coap_parse_unsigned */
   
coap_err
coap_msg_init(
        coap_msg       *tgt)
{
    static coap_msg src = {0};
    DEB(D("BEGIN\n"));

    *tgt = src;

    DEB(D("RETURN ==> COAP_OK\n"));
    return COAP_OK;
} /* coap_msg_init */

coap_err
coap_msg_addopt(
        coap_msg       *tgt,
        coap_opt       *opt)
{
    coap_opt   *last;
    uint16_t    lasttyp;

    DEB(D("BEGIN(opt=%d, len=%d)\n"), opt->o_typ, opt->o_len);
    last = LIST_ELEMENT_LAST(&tgt->c_optslst, coap_opt, o_nod);
    lasttyp = last ? last->o_typ : 0;
    if (opt->o_typ < lasttyp) {
        DEB(D("RETURN ==> COAP_INVALID_PARAMETER(o_typ=%d)\n"),
                opt->o_typ);
        return COAP_INVALID_PARAMETER;
    } /* if */

    LIST_APPEND(&tgt->c_optslst, &opt->o_nod);
    tgt->c_optssz++;

    DEB(D("RETURN ==> COAP_OK\n"));
    return COAP_OK;

} /* coap_msg_addopt */

coap_opt
*coap_msg_fstopt(
        coap_msg       *tgt)
{
    return LIST_ELEMENT_FIRST(&tgt->c_optslst, coap_opt, o_nod);
}

coap_opt
*coap_msg_lstopt(
        coap_msg       *tgt)
{
    return LIST_ELEMENT_LAST(&tgt->c_optslst, coap_opt, o_nod);
}

coap_opt
*coap_msg_nxtopt(
        coap_opt       *opt)
{
    return LIST_ELEMENT_NEXT(opt, coap_opt, o_nod);
}

coap_opt
*coap_msg_prvopt(
        coap_opt       *opt)
{
    return LIST_ELEMENT_PREV(opt, coap_opt, o_nod);
}

/* The next macro is to process the Options of a CoAP message.
 * It acts on the values taken from the OptDelta and OptLength
 * fields of the first byte from the option field. */
#define SWITCH(var)  do{\
        switch(var) { \
        case 15: \
            DEB(D("RETURN COAP_INVALID_"#var"\n")); \
            return COAP_INVALID_##var; \
        case 14: \
            /* two bytes of data */ \
            CHK(2); \
            coap_parse_unsigned(buff, 2, &var); \
            var += 256 + 13; \
            DEB(D(#var " <== 0x%x(%d)\n"), var, var); \
            ACT(2); \
            break; \
        case 13: /* one byte of delta */ \
            CHK(1); \
            coap_parse_unsigned(buff, 1, &var); \
            var += 13; \
            DEB(D(#var " <== 0x%x(%d)\n"), var, var); \
            ACT(1); \
            break; \
        } /* switch */\
    } while(0)

coap_err
coap_parse(
        const uint8_t  *buff,
        size_t          bufsz,
        coap_msg       *tgt)
{
    uint32_t opt_name = 0;
    static coap_msg initial = {0};

    DEB(D("BEGIN\n"));
    PRB(bufsz, buff, D("PACKET (size = %d bytes)"), bufsz);

#define VALUEOF(T) ((buff[COAP_##T##_OFFS] & COAP_##T##_MASK) \
        >> COAP_##T##_SHFT)

    /* check we have enough buffer to parse */
    CHK(COAP_HDR_LEN);

    *tgt = initial; /* initialize structure */
    tgt->c_pktdat = buff; /* pointer to packet data */
    tgt->c_pktlen = 0;

    /* check the packet version */
    if (VALUEOF(VERS) != COAP_VERS_VALUE) {
        DEB(D("==> COAP_INVALID_VERSION\n"));
        return COAP_INVALID_VERSION;
    } /* if */

    /* packet type */
    tgt->c_typ  = VALUEOF(TYPE);
    DEB(D("TYPE = %s\n"),
            coap_typ2str(tgt->c_typ));

    /* TKL */
    if ((tgt->c_toklen = VALUEOF(TKL)) > COAP_TKL_MAX) {
        DEB(D("==> COAP_INVALID_TKL(%lu)\n"), tgt->c_toklen);
        return COAP_INVALID_TKL;
    } /* if */

    /* Code */
    tgt->c_code = VALUEOF(CODE);
    DEB(D("CODE = COAP_CODE(%d, %d)\n"),
            COAP_CODE_MAJOR(tgt->c_code),
            COAP_CODE_MINOR(tgt->c_code));

    /* Message ID */

    coap_parse_unsigned(
            buff + COAP_MSGID_OFFS,
            COAP_MSGID_SZ,
            &tgt->c_msgid);
    DEB(D("MSGID = 0x%04x\n"),
            tgt->c_msgid);

    ACT(COAP_HDR_LEN); tgt->c_pktlen += COAP_HDR_LEN;
    if (tgt->c_toklen) CHK(tgt->c_toklen); /* check for space for the token */

    /* token */
    if (tgt->c_toklen) {
        tgt->c_tokdat = buff;
        PRB(tgt->c_toklen, tgt->c_tokdat,
                D("TOKEN: %d bytes"),
                tgt->c_toklen);
        ACT(tgt->c_toklen); 
        tgt->c_pktlen += tgt->c_toklen;
    } /* if */

    while(bufsz) { /* process options */
        uint32_t OptDLT, OptLEN;
        coap_opt *opt;

        CHK(1); /* no need to CHK(1); as we got into the loop */

        if (buff[0] == COAP_END_OF_OPTIONS) {
            DEB(D("END_OF_OPTIONS\n"));
            ACT(1); /* move the pointer */
            tgt->c_pktlen++;
            /* it's invalid to have payload data of length 0, so... */
            CHK(1); /* check at least one byte of payload */
            tgt->c_plddat = buff;
            tgt->c_pldlen = bufsz;
            break;
        } /* if */

        /* buff[0] != COAP_END_OF_OPTIONS */
        TR(OptDLT = VALUEOF(OptDLT));
        TR(OptLEN = VALUEOF(OptLEN));
        ACT(1); /* advance */
        tgt->c_pktlen++;

        /* see macro defined before this function */
        SWITCH(OptDLT); /* process Delta */
        SWITCH(OptLEN); /* process Length */

        if (OptLEN) CHK(OptLEN);

        /* space for the option struct */
        assert(opt = malloc(sizeof(coap_opt)));

        opt_name += OptDLT;
        opt->o_typ = opt_name;
        opt->o_len = OptLEN;
        opt->o_val = buff;

        PRB(opt->o_len, opt->o_val,
                D("OPT %d: len=%d\n"),
                opt->o_typ, opt->o_len);

        coap_msg_addopt(tgt, opt);

        if (OptLEN) {
            ACT(OptLEN);
            tgt->c_pktlen += OptLEN;
        } /* if */
    } /* while */

    DEB(D("RETURN ==> COAP_OK\n"));

    return COAP_OK;
} /* coap_parse */
