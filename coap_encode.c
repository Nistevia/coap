/* coap_encode.c -- implementation for packet encoding.
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Fri Apr 10 09:54:34 EEST 2015
 * Disclaimer: (C) LUIS COLORADO.  All rights reserved.
 */

#include <assert.h>
#include <string.h>

#include "coapP.h"

static size_t
code_unsigned(uint32_t val, uint8_t *buff, size_t bufsz)
{
    size_t res = bufsz;
    DEB(D("BEGIN\n"));
    /* no need to CHK, as it is done outside */
    buff += bufsz;
    while(bufsz--) {
        *--buff = val & 0xff;
        val >>= 8;
    } /* while */
    DEB(D("RETURN ==> %d\n"), res);
    return res;
} /* code_unsigned */

/* ENCODING MACRO FOR THE nibble fields of the option
 * processing */
#define ENCODE(x) do{ \
        if (x < 13) { \
            *aux |= (x << COAP_Opt##x##_SHFT) & COAP_Opt##x##_MASK;\
        } else { \
            x -= 13; \
            if (x < 256) {\
                *aux |= (13 << COAP_Opt##x##_SHFT) & COAP_Opt##x##_MASK;\
                CHK(1); \
                *buff = x; \
                ACT(1); \
                msg->c_pktlen++;\
            } else {\
                x -= 256;\
                *aux |= (14 << COAP_Opt##x##_SHFT) & COAP_Opt##x##_MASK;\
                CHK(2);\
                code_unsigned(x, buff, 2);\
                ACT(2);\
                msg->c_pktlen += 2;\
            } /* if */\
        } /* if */\
    } while(0)

coap_err
coap_encode(
        coap_msg       *msg,
        uint8_t        *buff,
        size_t          bufsz)
{
    coap_opt *opt;
    size_t optname = 0;

    DEB(D("BEGIN\n"));
    CHK(COAP_HDR_LEN); /* check we have at least space for header */

    msg->c_pktdat = buff;
    msg->c_pktlen = 0;

    /* version */
    buff[COAP_VERS_OFFS]  = 0;
    buff[COAP_VERS_OFFS] |= COAP_VERS_VALUE << COAP_VERS_SHFT;
    /* type */
    buff[COAP_TYPE_OFFS] |= (msg->c_typ << COAP_TYPE_SHFT) & COAP_TYPE_MASK;
    /* token length */
    if (msg->c_toklen > COAP_TKL_MAX) {
        DEB(D("RETURN COAP_INVALID_PARAMETER: "
                    "c_toklen(%d) > COAP_TKL_MAX(%d)\n"),
                msg->c_toklen, COAP_TKL_MAX);
        return COAP_INVALID_PARAMETER;
    } /* if */
    buff[COAP_TKL_OFFS]  |= msg->c_toklen & COAP_TKL_MASK; /* 0 shift */
    /* Code */
    buff[COAP_CODE_OFFS]  = msg->c_code; /* full byte */
    /* MsgId */
    code_unsigned(msg->c_msgid, buff + COAP_MSGID_OFFS, COAP_MSGID_SZ);

    ACT(COAP_HDR_LEN); /* actualize */
    msg->c_pktlen += COAP_HDR_LEN;

    /* TOKEN */
    if (msg->c_toklen) CHK(msg->c_toklen);
    memcpy(buff, msg->c_tokdat, msg->c_toklen);
    if (msg->c_toklen) ACT(msg->c_toklen);
    msg->c_pktlen += msg->c_toklen;

    /* Options */
    LIST_FOREACH_ELEMENT(opt, &msg->c_optslst, coap_opt, o_nod) {
        size_t DLT = opt->o_typ - optname;
        size_t LEN = opt->o_len;
        uint8_t *aux = buff;
        PRB(opt->o_len, opt->o_val,
                D("OPTION %d, DLT=%d, LEN=%d"),
                opt->o_typ, DLT, LEN);
        if (DLT < 0) {
            DEB(D("RETURN ==> COAP_INVALID_PARAMETER(Delta < 0)\n"));
            return COAP_INVALID_PARAMETER;
        } /* if */
        CHK(1);
        *aux = 0;
        ACT(1);
        msg->c_pktlen++;

        /* SEE ABOVE FOR THE DEFINITION OF THESE MACROS */
        ENCODE(DLT);
        ENCODE(LEN);

        if (opt->o_len) {
            if (opt->o_len) CHK(opt->o_len);
            memcpy(buff, opt->o_val, opt->o_len);
            if (opt->o_len) ACT(opt->o_len);
            msg->c_pktlen += opt->o_len;
        } /* if */

        optname = opt->o_typ;
    } /* LIST_FOREACH_ELEMENT */

    if (msg->c_pldlen) {
        CHK(1 + msg->c_pldlen);
        *buff = COAP_END_OF_OPTIONS;
        ACT(1);
        memcpy(buff, msg->c_plddat, msg->c_pldlen);
        if (msg->c_pldlen) ACT(msg->c_pldlen);
        msg->c_pktlen += 1 + msg->c_pldlen;
    } /* if */

    return COAP_OK;
} /* coap_encode */
