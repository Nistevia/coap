/* coap.c -- implementation for packet coding/decoding.
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Fri Apr 10 09:54:34 EEST 2015
 * Disclaimer: (C) LUIS COLORADO.  All rights reserved.
 */

#include "coap.h"

#define CHK(N) do{\
        if (bufsz < (N)) {\
            return COAP_INVALID_LENGTH;\
        }\
    }while(0)

#define ACT(N) do{\
        size_t n=(N);\
        buff+=n;\
        bufsz-=n;\
    }while(0)

static uint32_t
parse_unsigned(uint8_t *buff, size_t bufsz)
{
    uint32_t res = 0;
    while (bufsz--) {
        res <<= 8;
        res |= *buff++;
    } /* while */
    return res;
} /* parse_unsigned */
   
coap_err
coap_msg_init(
        coap_msg *tgt)
{
    static coap_msg src = {0};

    *tgt = src;

    return COAP_OK;
} /* coap_msg_init */

coap_err
coap_parse(
        const uint8_t  *buff,
        size_t          bufsz,
        coap_msg       *tgt)
{
    uint32_t opt_name = 0;

#define VALUEOF(T) ((buff[COAP_##T##_OFFS] & COAP_##T##_MASK) \
        >> COAP_##T##_SHFT)

    /* check we have enough buffer to parse */
    CHK(COAP_HDR_LEN);

    coap_msg_init(tgt); /* initialize structure */
    tgt->c_pktdat = buff; /* pointer to packet data */

    /* check the packet version */
    if (VALUEOF(VERS) != COAP_VERS_VALUE)
        return COAP_INVALID_VERSION;
    tgt->c_vers = COAP_VERS_VALUE;

    /* packet type */
    tgt->c_typ  = VALUEOF(TYPE);

    /* TKL */
    if ((tgt->c_tkl = VALUEOF(TKL)) > COAP_TYPE_MAX)
        return COAP_INVALID_TKL;

    /* Code */
    tgt->c_code = VALUEOF(CODE);

    /* Message ID */
    tgt->c_msgid = parse_unsigned(buff + COAP_MSGID_OFFS, COAP_MSGID_SZ);

    ACT(COAP_HDR_LEN);
    CHK(tgt->c_tkl); /* check for space for the token */

    /* token */
    tgt->c_tok = buff;
    ACT(tgt->c_tkl);

    while(buffsz) { /* process options */
        size_t OptDLT, OptLEN;
        coap_opt *opt;
        /* no need to CHK(1); as we got into the loop */
        if (buff[0] == COAP_END_OF_OPTIONS) {
            ACT(1); /* move the pointer */
            /* it's invalid to have payload data of length 0, so... */
            CHK(1); /* check at least one byte of payload */
            tgt->c_pld = buff;
            tgt->c_pldsz = bufsz;
            break;
        } /* if */
        /* buff[0] != COAP_END_OF_OPTIONS */
        OptDLT = VALUEOF(OptDLT);
        OptLEN = VALUEOF(OptLEN);
        ACT(1); /* advance */

#define SWITCH(var)  do{\
        switch(var) { \
        case 15: \
            return COAP_INVALID_##v; \
        case 14: \
            /* two bytes of data */ \
            CHK(2); \
            var = 269 + parse_unsigned(buff, 2); \
            ACT(2); \
        case 13: /* one byte of delta */ \
            CHK(1); \
            var = 13 + parse_unsigned(buff, 1); \
            ACT(1); \
            break; \
        } /* switch */\
    } while(0)

        SWITCH(OptDLT); /* process Delta */
        SWITCH(OptLEN); /* process Length */
        CHK(OptLEN);
        assert(opt = malloc(sizeof(coap_opt)));
        opt_name += OptDLT;
        opt->o_typ = opt_name;
        opt->o_len = OptLEN;
        opt->o_val = buff;

        ACT(len);
    } /* while */

} /* coap_parse */
