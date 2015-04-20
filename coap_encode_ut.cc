/* coap_encode_ut.cc -- google test program for coap_encode.c
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Mon Apr 20 11:46:49 EEST 2015
 * Disclaimer: (C) 2015 LUIS COLORADO.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "fprintbuf.h"
#include "coap.h"

struct CoapEncode: ::testing::Test {

    coap_msg            msg;
    uint8_t             buffer[100];
    const size_t        bufsz = sizeof buffer;

    CoapEncode() {
        msg.c_pktdat    = 0;
        msg.c_pktlen    = 0;
        msg.c_typ       = coap_typ_CON;
        msg.c_tokdat    = 0;
        msg.c_toklen    = 0;
        msg.c_code      = 0;
        msg.c_msgid     = 0;
        msg.c_plddat    = 0;
        msg.c_pldlen    = 0;
        LIST_INIT(&msg.c_optslst);
        msg.c_optssz    = 0;
        memset(buffer, 0xA5, sizeof buffer);
    }
};

TEST_F(CoapEncode, EncodeWithEmptyCONMsg)
{
    EXPECT_EQ(COAP_OK, coap_encode(&msg, buffer, sizeof buffer));
    EXPECT_EQ(buffer, msg.c_pktdat);

    EXPECT_EQ(4, msg.c_pktlen);

    EXPECT_EQ(0x40, msg.c_pktdat[0]);
    EXPECT_EQ(0x00, msg.c_pktdat[1]);
    EXPECT_EQ(0x00, msg.c_pktdat[2]);
    EXPECT_EQ(0x00, msg.c_pktdat[3]);

    EXPECT_EQ(0xA5, msg.c_pktdat[4]);
}

TEST_F(CoapEncode, EncodeWithEmptyNONMsg)
{
    msg.c_typ = coap_typ_NON;

    EXPECT_EQ(COAP_OK, coap_encode(&msg, buffer, sizeof buffer));
    EXPECT_EQ(buffer, msg.c_pktdat);

    EXPECT_EQ(4, msg.c_pktlen);

    EXPECT_EQ(0x50, msg.c_pktdat[0]);
    EXPECT_EQ(0x00, msg.c_pktdat[1]);
    EXPECT_EQ(0x00, msg.c_pktdat[2]);
    EXPECT_EQ(0x00, msg.c_pktdat[3]);

    EXPECT_EQ(0xA5, msg.c_pktdat[4]);
}

TEST_F(CoapEncode, EncodeWithEmptyACKMsg)
{
    msg.c_typ = coap_typ_ACK;

    EXPECT_EQ(COAP_OK, coap_encode(&msg, buffer, sizeof buffer));
    EXPECT_EQ(buffer, msg.c_pktdat);

    EXPECT_EQ(4, msg.c_pktlen);

    EXPECT_EQ(0x60, msg.c_pktdat[0]);
    EXPECT_EQ(0x00, msg.c_pktdat[1]);
    EXPECT_EQ(0x00, msg.c_pktdat[2]);
    EXPECT_EQ(0x00, msg.c_pktdat[3]);

    EXPECT_EQ(0xA5, msg.c_pktdat[4]);
}

TEST_F(CoapEncode, EncodeWithEmptyRSTMsg)
{
    msg.c_typ = coap_typ_RST;

    EXPECT_EQ(COAP_OK, coap_encode(&msg, buffer, sizeof buffer));
    EXPECT_EQ(buffer, msg.c_pktdat);

    EXPECT_EQ(4, msg.c_pktlen);

    EXPECT_EQ(0x70, msg.c_pktdat[0]);
    EXPECT_EQ(0x00, msg.c_pktdat[1]);
    EXPECT_EQ(0x00, msg.c_pktdat[2]);
    EXPECT_EQ(0x00, msg.c_pktdat[3]);

    EXPECT_EQ(0xA5, msg.c_pktdat[4]);
}

TEST_F(CoapEncode, EncodeWithMsgId)
{
    msg.c_msgid = 0x1234;

    EXPECT_EQ(COAP_OK, coap_encode(&msg, buffer, sizeof buffer));
    EXPECT_EQ(buffer, msg.c_pktdat);

    EXPECT_EQ(4, msg.c_pktlen);

    EXPECT_EQ(0x40, msg.c_pktdat[0]);
    EXPECT_EQ(0x00, msg.c_pktdat[1]);
    EXPECT_EQ(0x12, msg.c_pktdat[2]);
    EXPECT_EQ(0x34, msg.c_pktdat[3]);

    EXPECT_EQ(0xA5, msg.c_pktdat[4]);
}

TEST_F(CoapEncode, EncodeWithCode)
{
    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 1);

    EXPECT_EQ(COAP_OK, coap_encode(&msg, buffer, sizeof buffer));
    EXPECT_EQ(buffer, msg.c_pktdat);

    EXPECT_EQ(4, msg.c_pktlen);

    EXPECT_EQ(0x40, msg.c_pktdat[0]);
    EXPECT_EQ(0x01, msg.c_pktdat[1]);
    EXPECT_EQ(0x00, msg.c_pktdat[2]);
    EXPECT_EQ(0x00, msg.c_pktdat[3]);

    EXPECT_EQ(0xA5, msg.c_pktdat[4]);
}

TEST_F(CoapEncode, EncodeWithCodeAndInvalidToken)
{
    uint8_t tok[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 1);
    msg.c_toklen = sizeof tok;
    msg.c_tokdat = tok;

    EXPECT_EQ(COAP_INVALID_PARAMETER,
            coap_encode(&msg, buffer, sizeof buffer));
}

TEST_F(CoapEncode, EncodeWithCodeAndToken)
{
    uint8_t tok[6] = { 6, 5, 4, 3, 2, 1 };

    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 1);
    msg.c_toklen = sizeof tok;
    msg.c_tokdat = tok;

    EXPECT_EQ(COAP_OK,
            coap_encode(&msg, buffer, sizeof buffer));

    EXPECT_EQ(buffer, msg.c_pktdat);

    EXPECT_EQ(10, msg.c_pktlen);

    EXPECT_EQ(0x46, msg.c_pktdat[0]);
    EXPECT_EQ(0x01, msg.c_pktdat[1]);
    EXPECT_EQ(0x00, msg.c_pktdat[2]);
    EXPECT_EQ(0x00, msg.c_pktdat[3]);

    /* token */
    EXPECT_EQ(0x06, msg.c_pktdat[4]); 
    EXPECT_EQ(0x05, msg.c_pktdat[5]);
    EXPECT_EQ(0x04, msg.c_pktdat[6]);
    EXPECT_EQ(0x03, msg.c_pktdat[7]);
    EXPECT_EQ(0x02, msg.c_pktdat[8]);
    EXPECT_EQ(0x01, msg.c_pktdat[9]);

    EXPECT_EQ(0xA5, msg.c_pktdat[10]);
}
