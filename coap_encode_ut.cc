/* coap_encode_ut.cc -- google test program for coap_encode.c
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Mon Apr 20 11:46:49 EEST 2015
 * Disclaimer: (C) 2015 LUIS COLORADO.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "fprintbuf.h"
#include "coap.h"

static char   token[] = { 0, 1, 2, 3, 4, 5, };
static char   shrtMessage[] = "SMESSAGE";
static char   medmMessage[] =
    "MEDIUM LENGTH MESSAGE (< 269 chars)";
static char   longMessage[] = 
    "Lorem ipsum dolor sit amet, consectetur adipiscing "
    "elit, sed do eiusmod tempor incididunt ut labore et "
    "dolore magna aliqua. Ut enim ad minim veniam, quis "
    "nostrud exercitation ullamco laboris nisi ut aliquip "
    "ex ea commodo consequat. Duis aute irure dolor in "
    "reprehenderit in voluptate velit esse cillum dolore "
    "eu fugiat nulla pariatur. Excepteur sint occaecat "
    "cupidatat non proident, sunt in culpa qui officia "
    "deserunt mollit anim id est laborum."
    " (This is a long option to demons"
    "trate the use of payload data)";

struct CoapEncode: ::testing::Test {

    coap_msg            msg;
    uint8_t             buffer[100];

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
}; /* CoapEncode */

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

TEST_F(CoapEncode, EncodeWithCodeAndPayload)
{
    uint8_t payload[] = { 1, 1, 2, 3, 5, 8, 13, 21, };

    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 2);
    msg.c_pldlen = sizeof payload;
    msg.c_plddat = payload;

    EXPECT_EQ(COAP_OK,
            coap_encode(&msg, buffer, sizeof buffer));

    EXPECT_EQ(buffer, msg.c_pktdat);

    EXPECT_EQ(13, msg.c_pktlen);

    EXPECT_EQ(0x40, msg.c_pktdat[0]);
    EXPECT_EQ(0x02, msg.c_pktdat[1]);
    EXPECT_EQ(0x00, msg.c_pktdat[2]);
    EXPECT_EQ(0x00, msg.c_pktdat[3]);

    EXPECT_EQ(0xff, msg.c_pktdat[4]);

    /* payload */
    EXPECT_EQ(0x01, msg.c_pktdat[5]); 
    EXPECT_EQ(0x01, msg.c_pktdat[6]);
    EXPECT_EQ(0x02, msg.c_pktdat[7]);
    EXPECT_EQ(0x03, msg.c_pktdat[8]);
    EXPECT_EQ(0x05, msg.c_pktdat[9]);
    EXPECT_EQ(0x08, msg.c_pktdat[10]);
    EXPECT_EQ(0x0d, msg.c_pktdat[11]);
    EXPECT_EQ(0x15, msg.c_pktdat[12]);

    EXPECT_EQ(0xA5, msg.c_pktdat[13]);
}

TEST_F(CoapEncode, EncodeWithCodeVSOptionsAndPayload)
{
    uint8_t payload[] = { 1, 1, 2, 3, 5, 8, 13, 21, };
    coap_opt opt;
    int i;
    uint8_t optdat[] = { 0x01, 0x23, 0x45, 0x67 };

    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 2);
    msg.c_pldlen = sizeof payload;
    msg.c_plddat = payload;
    opt.o_typ = 3;
    opt.o_len = sizeof optdat;
    opt.o_val = optdat;
    LIST_APPEND(&msg.c_optslst, &opt.o_nod);
    

    EXPECT_EQ(COAP_OK,
            coap_encode(&msg, buffer, sizeof buffer));

    EXPECT_EQ(buffer, msg.c_pktdat);

    EXPECT_EQ(4 + 1 + sizeof optdat + 1 + sizeof payload,
            msg.c_pktlen);

    EXPECT_EQ(0x40, msg.c_pktdat[0]);
    EXPECT_EQ(0x02, msg.c_pktdat[1]);
    EXPECT_EQ(0x00, msg.c_pktdat[2]);
    EXPECT_EQ(0x00, msg.c_pktdat[3]);

    /* option, very short name and length */
    EXPECT_EQ(0x30 + sizeof optdat, msg.c_pktdat[4]); 
    for (i = 0; i < sizeof optdat; ++i)
        EXPECT_EQ(optdat[i], msg.c_pktdat[5 + i]);


    EXPECT_EQ(0xff, msg.c_pktdat[5 + sizeof optdat]);

    /* payload */
    for (i = 0; i < sizeof payload; ++i)
    EXPECT_EQ(payload[i], msg.c_pktdat[6 + sizeof optdat + i]); 

    EXPECT_EQ(0xA5, msg.c_pktdat[6 + sizeof optdat + i]);
}

TEST_F(CoapEncode, EncodeWithCodeSOptionsAndPayload)
{
    uint8_t payload[] = { 1, 1, 2, 3, 5, 8, 13, 21, 33, 54, 87, };
    coap_opt opt;
    uint8_t optdat[] = 
        "This is a long option to demons"
        "trate the use of payload data";

    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 2);
    msg.c_pldlen = sizeof payload;
    msg.c_plddat = payload;
    opt.o_typ = 3;
    opt.o_len = sizeof optdat;
    opt.o_val = optdat;
    LIST_APPEND(&msg.c_optslst, &opt.o_nod);
    
    EXPECT_EQ(COAP_OK,
            coap_encode(&msg, buffer, sizeof buffer));

    EXPECT_EQ(buffer, msg.c_pktdat);

    EXPECT_EQ(4 + 2 + sizeof optdat + 1 + sizeof payload,
            msg.c_pktlen);

    EXPECT_EQ(0x40, msg.c_pktdat[0]);
    EXPECT_EQ(0x02, msg.c_pktdat[1]);
    EXPECT_EQ(0x00, msg.c_pktdat[2]);
    EXPECT_EQ(0x00, msg.c_pktdat[3]);

    /* option, very short name and length */
    EXPECT_EQ(0x3d, msg.c_pktdat[4]); 
    EXPECT_EQ(sizeof optdat - 13, msg.c_pktdat[5]); 

    EXPECT_STREQ((const char *)optdat,
            (const char *)msg.c_pktdat + 6);

    EXPECT_EQ(0xff, msg.c_pktdat[6 + sizeof optdat]);

    /* payload */
    for (int i = 0; i < sizeof payload; ++i)
        EXPECT_EQ(payload[i], msg.c_pktdat[7 + sizeof optdat + i]); 

    EXPECT_EQ(0xA5, msg.c_pktdat[msg.c_pktlen]);
}

TEST_F(CoapEncode, EncodeWithCodeLOptionsAndSPayloadAndShortBuffer)
{
    uint8_t payload[] = { 1, 1, 2, 3, 5, 8, 13, 21, 33, 54, 87, };
    coap_opt opt;
    uint8_t optdat[] = 
        "Lorem ipsum dolor sit amet, consectetur adipiscing "
        "elit, sed do eiusmod tempor incididunt ut labore et "
        "dolore magna aliqua. Ut enim ad minim veniam, quis "
        "nostrud exercitation ullamco laboris nisi ut aliquip "
        "ex ea commodo consequat. Duis aute irure dolor in "
        "reprehenderit in voluptate velit esse cillum dolore "
        "eu fugiat nulla pariatur. Excepteur sint occaecat "
        "cupidatat non proident, sunt in culpa qui officia "
        "deserunt mollit anim id est laborum."
        " (This is a long option to demons"
        "trate the use of payload data)";

    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 2);
    msg.c_pldlen = sizeof payload;
    msg.c_plddat = payload;
    opt.o_typ = 3;
    opt.o_len = sizeof optdat;
    opt.o_val = optdat;
    LIST_APPEND(&msg.c_optslst, &opt.o_nod);
    
    EXPECT_EQ(COAP_INVALID_LENGTH,
            coap_encode(&msg, buffer, sizeof buffer));

}

TEST_F(CoapEncode, EncodeWithCodeLOptionsAndLPayload)
{
    uint8_t payload[] = { 1, 1, 2, 3, 5, 8, 13, 21, 33, 54, 87, };
    coap_opt opt;
    uint8_t optdat[] = 
        "Lorem ipsum dolor sit amet, consectetur adipiscing "
        "elit, sed do eiusmod tempor incididunt ut labore et "
        "dolore magna aliqua. Ut enim ad minim veniam, quis "
        "nostrud exercitation ullamco laboris nisi ut aliquip "
        "ex ea commodo consequat. Duis aute irure dolor in "
        "reprehenderit in voluptate velit esse cillum dolore "
        "eu fugiat nulla pariatur. Excepteur sint occaecat "
        "cupidatat non proident, sunt in culpa qui officia "
        "deserunt mollit anim id est laborum."
        " (This is a long option to demons"
        "trate the use of payload data)";
    uint8_t new_buff[1500];
    memset(new_buff, 0xA5, sizeof new_buff);

    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 2);
    msg.c_pldlen = sizeof payload;
    msg.c_plddat = payload;
    opt.o_typ = 3;
    opt.o_len = sizeof optdat;
    opt.o_val = optdat;
    LIST_APPEND(&msg.c_optslst, &opt.o_nod);
    
    EXPECT_EQ(COAP_OK,
            coap_encode(&msg, new_buff, sizeof new_buff));

    EXPECT_EQ(new_buff, msg.c_pktdat);

    EXPECT_EQ(4 + /* header length */
            3 + sizeof optdat + /* opt length */
            1 + /* end of options */
            sizeof payload /* payload */,
            msg.c_pktlen);

    EXPECT_EQ(0x40,
            msg.c_pktdat[0]);
    EXPECT_EQ(0x02,
            msg.c_pktdat[1]);
    EXPECT_EQ(0x00,
            msg.c_pktdat[2]);
    EXPECT_EQ(0x00,
            msg.c_pktdat[3]);

    /* option, very short name and very long length */
    EXPECT_EQ(0x3e,
            msg.c_pktdat[4]); 
    EXPECT_EQ((sizeof optdat - 13 - 256) >> 8,
            msg.c_pktdat[5]); 
    EXPECT_EQ((sizeof optdat - 13 - 256) &  0xff,
            msg.c_pktdat[6]); 

    EXPECT_STREQ((const char *)optdat,
            (const char *)msg.c_pktdat + 7);

    EXPECT_EQ(0xff,
            msg.c_pktdat[7 + sizeof optdat]);

    /* payload */
    for (int i = 0; i < sizeof payload; ++i)
        EXPECT_EQ(payload[i], msg.c_pktdat[8 + sizeof optdat + i]); 

    EXPECT_EQ(0xA5, msg.c_pktdat[msg.c_pktlen]);
}
