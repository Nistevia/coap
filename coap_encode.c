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
    CHK(bufsz);
    buff += bufsz;
    while(bufsz--) {
        *--buff = val & 0xff;
        val >>= 8;
    } /* while */
    DEB(D("RETURN %d;\n"), res);
    return res;
} /* code_unsigned */

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

    CHK(COAP_HDR_LEN);

    buff[COAP_VERS_OFFS]  = 0;
    buff[COAP_VERS_OFFS] |= COAP_VERS_VALUE << COAP_VERS_SHFT;
    buff[COAP_TYPE_OFFS] |= (msg->c_typ << COAP_TYPE_SHFT) & COAP_TYPE_MASK;
    buff[COAP_TKL_OFFS]  |= msg->c_toklen & COAP_TKL_MASK; /* 0 shift */
    buff[COAP_CODE_OFFS]  = msg->c_code; /* full byte */
    code_unsigned(msg->c_msgid, buff + COAP_MSGID_OFFS, COAP_MSGID_SZ);
    ACT(COAP_TOKEN_OFFS);
    CHK(msg->c_toklen);
    memcpy(buff, msg->c_tokdat, msg->c_toklen);
    ACT(msg->c_toklen);

    LIST_FOREACH_ELEMENT(opt, &msg->c_optslst, coap_opt, o_nod) {
        size_t OptDLT, OptLEN;

    } /* LIST_FOREACH_ELEMENT */

    return COAP_OK;
} /* coap_encode */
