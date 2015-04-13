/* coap_encode_test.c -- testing program for coap_encoder.
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Mon Apr 13 14:33:30 EEST 2015
 * Disclaimer: (C) LUIS COLORADO.  All rights reserved.
 */

#define DEBUG   1

#include "fprintbuf.h"
#include "coapP.h"

#define TR(arg) do{ printf(D("%s\n"), #arg); arg; } while(0)

uint8_t buffer[1500];

int main()
{
    coap_msg m;
    coap_opt o1, o2, *o;
    coap_msg m2;
    char tok[] = "PEPE";
    char o1data[] = "o1data";
    char o2data[] = "Arriba Cachipurriana";
    char payload[] = "{this is the payload}";
    TR(coap_msg_init(&m));
    TR(coap_msg_settype(&m, coap_typ_NON));
    TR(coap_msg_setcode(&m, 0xe0));
    TR(coap_msg_setmsgid(&m, 0x15fe));
    TR(coap_msg_settoken(&m, tok, sizeof tok - 1));
    TR(o1.o_typ = 6);
    TR(o1.o_len = sizeof o1data - 1);
    TR(o1.o_val = o1data);
    TR(LIST_INIT(&o1.o_nod));
    TR(coap_msg_addopt(&m, &o1));
    TR(o2.o_typ = 10);
    TR(o2.o_len = sizeof o2data -1);
    TR(o2.o_val = o2data);
    TR(coap_msg_addopt(&m, &o2));
    TR(coap_msg_setpayload(&m, payload, sizeof payload - 1));
    TR(coap_encode(&m, buffer, sizeof buffer));

    fprintbuf(stderr, m.c_pktlen, m.c_pktdat,
            D("CODED PACKET (%d bytes)"),
            m.c_pktlen);

    TR(coap_parse(m.c_pktdat, m.c_pktlen, &m2));
    DEB(D("c_typ = 0x%02x\n"), m2.c_typ);
    PRB(m2.c_toklen, m2.c_tokdat, "c_toklen(%d), c_tokdat", m2.c_toklen);
    DEB(D("c_code = 0x%x\n"), m2.c_code);
    DEB(D("c_msgid = 0x%x\n"), m2.c_msgid);
    LIST_FOREACH_ELEMENT(o, &m2.c_optslst, coap_opt, o_nod) {
        DEB(D("  opt.o_typ = 0x%02x\n"), o->o_typ);
        PRB(o->o_len, o->o_val, D("  opt.o_len=%d, opt.o_val:"), o->o_len);
    } /* LIST_FOREACH_ELEMENT */
    PRB(m.c_pldlen, m.c_plddat, D("c_pldlen=%d, c_plddat:"), m2.c_pldlen);

} /* main */
