/* coap_encode_ut.cc -- google test program for coap_encode.c
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Mon Apr 20 11:46:49 EEST 2015
 * Disclaimer: (C) 2015 LUIS COLORADO.  All rights reserved.
 */

#include <gtest/gtest.h>
#include "coap.h"

TEST(CoapEncodeUt, CoapEncode1) {

    coap_msg msg;
    uint8_t buffer[100];

    msg.c_pktdat = 0;
    msg.c_pktlen = 0;
    msg.c_typ = coap_typ_CON;
    msg.c_tokdat = 0;
    msg.c_toklen = 0;
    msg.c_code = 0;
    msg.c_msgid = 0;
    msg.c_plddat = 0;
    msg.c_pldlen = 0;
    LIST_INIT(&msg.c_optslst);
    msg.c_optssz = 0;

    EXPECT_EQ(COAP_OK, coap_encode(&msg, buffer, sizeof buffer));

} /* CoapEncodeUt/CoapEncode1 */
