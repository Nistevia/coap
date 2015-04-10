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
        size_t n=(N);
        buff+=n;
        bufsz-=n;
    }while(0)

   
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

    CHK(COAP_HDR_LEN);

    coap_msg_init(tgt);
    tgt->c_pktdat = buff; /* pointer to packet data */

    /* check the packet version */
    if (!COAP_VERS_OK(buff[COAP_VERS_OFF])) return COAP_INVALID_VERSION;

    tgt->c_vers = (buff[COAP_VERS_OFF] & COAP_VERS_MASK) >> COAP_VERS_SHFT;
    tgt->c_typ  = (buff[COAP_TYPE_OFF] & COAP_TYPE_MASK) >> COAP_TYPE_SHFT;

    if (buff[0] & COAP_TKL_MASK >= COAP_TKL_NBR)
        return COAP_INVALID_TKL;

    tgt->c_tkl  = (buff[0] & COAP_TKL_MASK);
    tgt->c_typ  = buff[1];
    tgt->c_code = parse_unsigned(buff + COAP_MSGID_OFFSET

} /* coap_parse */
