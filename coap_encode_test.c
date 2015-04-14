/* coap_encode_test.c -- testing program for coap_encoder.
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Mon Apr 13 14:33:30 EEST 2015
 * Disclaimer: (C) LUIS COLORADO.  All rights reserved.
 */

#define DEBUG   1

#include "fprintbuf.h"
#include "coapP.h"

uint8_t buffer[1500];

int main()
{
    coap_msg m;
    coap_opt o1={0}, o2={0}, *o;
    coap_msg m2;
    char tok[] = "PEPE";
    char o1data[] = "o1data";
    char o2data[] = "Arriba Cachipurriana";
    char payload[] = "{this is the payload}";

    TR(coap_msg_init(&m));
    TR(coap_msg_settype(&m, coap_typ_NON));
    TR(coap_msg_setcode(&m, COAP_CODE(7, 3)));
    TR(coap_msg_setmsgid(&m, 0x15fe));

    TR(coap_msg_settoken(&m, tok, sizeof tok - 1));
    TR(o1.o_typ = 6);
    TR(o1.o_len = sizeof o1data - 1);
    TR(o1.o_val = o1data);
    TR(coap_msg_addopt(&m, &o1));

    TR(o2.o_typ = 6);
#if 0
    TR(o2.o_len = sizeof o2data -1);
    TR(o2.o_val = o2data);
#endif
#if 0
    TR(coap_msg_addopt(&m, &o2));
    TR(coap_msg_setpayload(&m, payload, sizeof payload - 1));
#endif
    TR(coap_encode(&m, buffer, sizeof buffer));

    PRB(m.c_pktlen, m.c_pktdat,
            D("CODED PACKET (%ld bytes)"),
            m.c_pktlen);

    TR(coap_parse(m.c_pktdat, m.c_pktlen, &m2));
    DEB(D("c_typ = %s\n"), coap_typ2str(m2.c_typ));
    PRB(m2.c_toklen, m2.c_tokdat, "c_toklen(%ld), c_tokdat", m2.c_toklen);
    DEB(D("c_code = %d.%d\n"),
            COAP_CODE_MAJOR(m2.c_code),
            COAP_CODE_MINOR(m2.c_code));
    DEB(D("c_msgid = 0x%x\n"), m2.c_msgid);
    LIST_FOREACH_ELEMENT(o, &m2.c_optslst, coap_opt, o_nod) {
        PRB(o->o_len, o->o_val,
                D("  opt.o_typ = %d, o_len=%ld:"),
                o->o_typ, o->o_len);
    } /* LIST_FOREACH_ELEMENT */
    PRB(m.c_pldlen, m.c_plddat, D("PAYLOAD len=%ld:"), m2.c_pldlen);

} /* main */
