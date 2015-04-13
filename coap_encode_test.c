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
    coap_opt o1, o2;
    char tok[] = "PEPE";
    char o1data[] = "o1data";
    char o2data[] = "Arriba Cachipurriana";
    char payload[] = "{this is the payload}";
    TR(coap_msg_init(&m));
    TR(coap_msg_settype(&m, coap_typ_CON));
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

} /* main */
