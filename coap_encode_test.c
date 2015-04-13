/* coap_encode_test.c -- testing program for coap_encoder.
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Mon Apr 13 14:33:30 EEST 2015
 * Disclaimer: (C) LUIS COLORADO.  All rights reserved.
 */

#include "fprintbuf.h"
#include "coapP.h"

uint8_t buffer[1500];

int main()
{
    coap_msg m;
    coap_opt o;
    char tok[] = "PEPE\x00\x01\x02\x03";
    coap_msg_init(&m);
    coap_msg_settype(&m, coap_typ_RST);
    coap_msg_setmsgid(&m, 0x1234);
    coap_msg_settoken(&m, tok, sizeof tok - 1);
    o.o_typ = 3;
    o.o_len = sizeof tok - 1;
    o.o_val = tok;
    LIST_INIT(&o.o_nod);
    coap_msg_addopt(&m, &o);
    coap_encode(&m, buffer, sizeof buffer);
    fprintbuf(stderr, m.c_pktlen, m.c_pktdat,
            D("CODED PACKET (%d bytes)"),
            m.c_pktlen);

} /* main */
