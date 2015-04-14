/* coap_parse.c -- implementation for packet decoding.
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Fri Apr 10 09:54:34 EEST 2015
 * Disclaimer: (C) LUIS COLORADO.  All rights reserved.
 */

#include <assert.h>
#include "coapP.h"

char *
coap_typ2str(coap_typ typ)
{
    static char *msg[] = {
        "CON", "NON", "ACK", "RST",
    };
    static char inv[] = "INVALID";

    return typ >= sizeof msg/sizeof msg[0] ? inv : msg[typ];
} /* tgt2typ */

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
coap_msg_settype(
        coap_msg       *tgt,
        coap_typ        typ)
{
    DEB(D("BEGIN(typ=%s)\n"), coap_typ2str(typ));

    tgt->c_typ = typ;

    DEB(D("RETURN ==> COAP_OK\n"));

    return COAP_OK;
} /* coap_msg_settype */

coap_err
coap_msg_setcode(
        coap_msg       *tgt,
        uint8_t        cod)
{
    DEB(D("BEGIN(cod=0x%02x)\n"), cod);

    tgt->c_code = cod;

    DEB(D("RETURN ==> COAP_OK\n"));

    return COAP_OK;
} /* coap_msg_setcode */

coap_err
coap_msg_setpayload(
        coap_msg       *tgt,
        uint8_t        *payload,
        size_t          payloadlen)
{
    DEB(D("BEGIN(payloadlen=%d,payload=...)\n"), payloadlen);

    tgt->c_plddat = payload;
    tgt->c_pldlen = payloadlen;

    DEB(D("RETURN ==> COAP_OK\n"));

    return COAP_OK;
} /* coap_msg_setpayload */

coap_err
coap_msg_settoken(
        coap_msg       *tgt,
        uint8_t        *tokdat,
        size_t          toklen)
{
    DEB(D("BEGIN(toklen=%d,tokdat=...)\n"), toklen);
    if (toklen > COAP_TKL_MAX) {
        DEB(D("RETURN COAP_INVALID_PARAMETER"
                    "(toklen(%d) > %d);\n"),
                toklen, COAP_TKL_MAX);
        return COAP_INVALID_PARAMETER;
    } /* if */

    tgt->c_tokdat = tokdat;
    tgt->c_toklen = toklen;

    DEB(D("RETURN ==> COAP_OK\n"));

    return COAP_OK;
} /* coap_msg_settoken */

coap_err
coap_msg_setmsgid(
        coap_msg       *tgt,
        uint16_t        msgid)
{
    DEB(D("BEGIN(msgid=0x%04x)\n"),
            msgid);

    tgt->c_msgid = msgid;

    DEB(D("RETURN ==> COAP_OK\n"));
    return COAP_OK;
} /* coap_msg_setmsgid */

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
