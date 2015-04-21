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

static char   shortOption[] = "8 BYTES";
static char   mediumOption[] =
    "MEDIUM LENGTH MESSAGE (13 <= length < 269 chars)";
static char   longOption[] = 
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
static char thePayload[] =
    "<<<This is the payload>>>";

struct CoapEncode: ::testing::Test {

    coap_msg            msg;
    uint8_t             buffer[100];
    uint8_t             longBuffer[1500];

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
        memset(longBuffer, 0xA5, sizeof longBuffer);
    } /* CoapEncode() */

}; /* CoapEncode */

TEST_F(CoapEncode, EncodeWithEmptyCONMsg)
{
    EXPECT_EQ(COAP_OK,
            coap_encode(&msg,
                buffer,
                sizeof buffer));
    EXPECT_EQ(buffer, msg.c_pktdat);
    const uint8_t *p = msg.c_pktdat;

    EXPECT_EQ(4, msg.c_pktlen);

    EXPECT_EQ(0x40, *p++);
    EXPECT_EQ(0x00, *p++);
    EXPECT_EQ(0x00, *p++);
    EXPECT_EQ(0x00, *p++);

    /* extra token data at end of buffer to check */
    EXPECT_EQ(0xA5, *p++);
}

TEST_F(CoapEncode, EncodeWithEmptyNONMsg)
{
    msg.c_typ = coap_typ_NON;

    EXPECT_EQ(COAP_OK,
            coap_encode(&msg,
                buffer,
                sizeof buffer));
    EXPECT_EQ(buffer, msg.c_pktdat);
    const uint8_t *p = msg.c_pktdat;

    EXPECT_EQ(4, msg.c_pktlen);

    EXPECT_EQ(0x50, *p++);
    EXPECT_EQ(0x00, *p++);
    EXPECT_EQ(0x00, *p++);
    EXPECT_EQ(0x00, *p++);

    /* extra token data at end of buffer to check */
    EXPECT_EQ(0xA5, *p++);
}

TEST_F(CoapEncode, EncodeWithEmptyACKMsg)
{
    msg.c_typ = coap_typ_ACK;

    EXPECT_EQ(COAP_OK,
            coap_encode(&msg,
                buffer,
                sizeof buffer));
    EXPECT_EQ(buffer, msg.c_pktdat);
    const uint8_t *p = msg.c_pktdat;

    EXPECT_EQ(4, msg.c_pktlen);

    EXPECT_EQ(0x60, *p++);
    EXPECT_EQ(0x00, *p++);
    EXPECT_EQ(0x00, *p++);
    EXPECT_EQ(0x00, *p++);

    /* extra token data at end of buffer to check */
    EXPECT_EQ(0xA5, *p++);
}

TEST_F(CoapEncode, EncodeWithEmptyRSTMsg)
{
    msg.c_typ = coap_typ_RST;

    EXPECT_EQ(COAP_OK,
            coap_encode(&msg,
                buffer,
                sizeof buffer));
    EXPECT_EQ(buffer, msg.c_pktdat);
    const uint8_t *p = msg.c_pktdat;

    EXPECT_EQ(4, msg.c_pktlen);

    EXPECT_EQ(0x70, *p++);
    EXPECT_EQ(0x00, *p++);
    EXPECT_EQ(0x00, *p++);
    EXPECT_EQ(0x00, *p++);

    /* extra token data at end of buffer to check */
    EXPECT_EQ(0xA5, *p++);
}

TEST_F(CoapEncode, EncodeWithMsgId)
{
    msg.c_msgid = 0x1234;

    EXPECT_EQ(COAP_OK,
            coap_encode(&msg,
                buffer,
                sizeof buffer));
    EXPECT_EQ(buffer, msg.c_pktdat);
    const uint8_t *p = msg.c_pktdat;

    EXPECT_EQ(4, msg.c_pktlen);

    EXPECT_EQ(0x40, *p++);
    EXPECT_EQ(0x00, *p++);
    EXPECT_EQ(0x12, *p++);
    EXPECT_EQ(0x34, *p++);

    /* extra token data at end of buffer to check */
    EXPECT_EQ(0xA5, *p++);
}

TEST_F(CoapEncode, EncodeWithCode)
{
    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 1);

    EXPECT_EQ(COAP_OK,
            coap_encode(&msg,
                buffer,
                sizeof buffer));
    EXPECT_EQ(buffer, msg.c_pktdat);
    const uint8_t *p = msg.c_pktdat;

    EXPECT_EQ(4, msg.c_pktlen);

    EXPECT_EQ(0x40, *p++);
    EXPECT_EQ(0x01, *p++);
    EXPECT_EQ(0x00, *p++);
    EXPECT_EQ(0x00, *p++);

    /* extra token data at end of buffer to check */
    EXPECT_EQ(0xA5, *p++);
}

TEST_F(CoapEncode, EncodeWithCodeAndInvalidToken)
{
    /* token too long */
    const uint8_t tok[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 1);
    msg.c_toklen = sizeof tok;
    msg.c_tokdat = tok;

    EXPECT_EQ(COAP_INVALID_PARAMETER,
            coap_encode(&msg,
                buffer,
                sizeof buffer));
}

TEST_F(CoapEncode, EncodeWithCodeAndValidToken)
{
    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 1);
    msg.c_tokdat = (const uint8_t *)token;
    msg.c_toklen = sizeof token;

    EXPECT_EQ(COAP_OK,
            coap_encode(&msg, buffer, sizeof buffer));

    EXPECT_EQ(buffer, msg.c_pktdat);
    const uint8_t *p = msg.c_pktdat;

    EXPECT_EQ(4 + sizeof token, msg.c_pktlen);

    EXPECT_EQ(0x46, *p++);
    EXPECT_EQ(0x01, *p++);
    EXPECT_EQ(0x00, *p++);
    EXPECT_EQ(0x00, *p++);

    /* token */
    for (int i = 0; i < sizeof token; ++i)
        EXPECT_EQ(token[i], *p++);

    /* extra token data at end of buffer to check */
    EXPECT_EQ(0xA5, *p++);
}

TEST_F(CoapEncode, EncodeWithCodeAndPayload)
{
    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 2);
    msg.c_pldlen = sizeof thePayload;
    msg.c_plddat = (const uint8_t *)thePayload;

    EXPECT_EQ(COAP_OK,
            coap_encode(&msg,
                buffer,
                sizeof buffer));

    EXPECT_EQ(buffer, msg.c_pktdat);
    const uint8_t *p = msg.c_pktdat;

    EXPECT_EQ(4 /* header length */
            + 1 /* end of options */
            + sizeof thePayload /* payload size */,
            msg.c_pktlen);

    EXPECT_EQ(0x40, *p++);
    EXPECT_EQ(0x02, *p++);
    EXPECT_EQ(0x00, *p++);
    EXPECT_EQ(0x00, *p++);

    /* end of options */
    EXPECT_EQ(0xff, *p++);

    /* payload */
    for (int i = 0; i < sizeof thePayload; ++i) {
        EXPECT_EQ(thePayload[i], *p++);
    } /* for */

    /* extra token data at end of buffer to check */
    EXPECT_EQ(0xA5, *p++);
}

TEST_F(CoapEncode, EncodeWithCodeAndZeroLengthPayload)
{
    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 2);
    msg.c_pldlen = 0;
    msg.c_plddat = (const uint8_t *)thePayload;

    EXPECT_EQ(COAP_OK,
            coap_encode(&msg,
                buffer,
                sizeof buffer));

    EXPECT_EQ(buffer, msg.c_pktdat);
    const uint8_t *p = msg.c_pktdat;

    EXPECT_EQ(4 /* header length */,
            msg.c_pktlen);

    EXPECT_EQ(0x40, *p++);
    EXPECT_EQ(0x02, *p++);
    EXPECT_EQ(0x00, *p++);
    EXPECT_EQ(0x00, *p++);

    /* extra token data at end of buffer to check */
    EXPECT_EQ(0xA5, *p++);
}

TEST_F(CoapEncode, EncodeWithCodeAndLength1Payload)
{
    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 2);
    const size_t PAYLOAD_SIZE = 1;
    msg.c_pldlen = PAYLOAD_SIZE;
    msg.c_plddat = (const uint8_t *)thePayload;

    EXPECT_EQ(COAP_OK,
            coap_encode(&msg,
                buffer,
                sizeof buffer));

    EXPECT_EQ(buffer, msg.c_pktdat);
    const uint8_t *p = msg.c_pktdat;

    EXPECT_EQ(4 /* header length */
            + 1 /* end of options */
            + PAYLOAD_SIZE,
            msg.c_pktlen);

    EXPECT_EQ(0x40, *p++);
    EXPECT_EQ(0x02, *p++);
    EXPECT_EQ(0x00, *p++);
    EXPECT_EQ(0x00, *p++);

    /* end of options */
    EXPECT_EQ(0xff, *p++);

    /* payload */
    EXPECT_EQ(thePayload[0], *p++);

    /* extra token data at end of buffer to check */
    EXPECT_EQ(0xA5, *p++);
}

TEST_F(CoapEncode, EncodeWithCodeShortOptionAndPayload)
{
    coap_opt opt;

    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 2);
    msg.c_pldlen = sizeof thePayload;
    msg.c_plddat = (const uint8_t *)thePayload;
    opt.o_typ = 3;
    opt.o_len = sizeof shortOption;
    opt.o_val = (const uint8_t *)shortOption;
    LIST_APPEND(&msg.c_optslst, &opt.o_nod);
    

    EXPECT_EQ(COAP_OK,
            coap_encode(&msg,
                buffer,
                sizeof buffer));

    EXPECT_EQ(buffer, msg.c_pktdat);
    const uint8_t *p = msg.c_pktdat;

    EXPECT_EQ(4 /* header length */
            + 1 /* option header */
            + sizeof shortOption /* the option */
            + 1 /* end of options */
            + sizeof thePayload /* the payload */,
            msg.c_pktlen);

    EXPECT_EQ(0x40, *p++);
    EXPECT_EQ(0x02, *p++);
    EXPECT_EQ(0x00, *p++);
    EXPECT_EQ(0x00, *p++);

    /* option, very short name and length */
    EXPECT_EQ(0x30 + sizeof shortOption, *p++); 
    for (int i = 0; i < sizeof shortOption; ++i)
        EXPECT_EQ(shortOption[i], *p++);

    /* end of options */
    EXPECT_EQ(0xff, *p++);

    /* payload */
    for (int i = 0; i < sizeof thePayload; ++i)
        EXPECT_EQ(thePayload[i], *p++); 

    /* extra token data at end of buffer to check */
    EXPECT_EQ(0xA5, *p++);
}

TEST_F(CoapEncode, EncodeWithCodeShortOptionMediumOptionAndPayload)
{
    coap_opt opt1, opt2;

    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 2);
    msg.c_pldlen = sizeof thePayload;
    msg.c_plddat = (const uint8_t *)thePayload;
    opt1.o_typ = 3;
    opt1.o_len = sizeof shortOption;
    opt1.o_val = (const uint8_t *)shortOption;
    LIST_APPEND(&msg.c_optslst, &opt1.o_nod);
    opt2.o_typ = 3;
    opt2.o_len = sizeof mediumOption;
    opt2.o_val = (const uint8_t *)mediumOption;
    LIST_APPEND(&msg.c_optslst, &opt2.o_nod);
    
    EXPECT_EQ(COAP_OK,
            coap_encode(&msg, buffer, sizeof buffer));

    EXPECT_EQ(buffer, msg.c_pktdat);
    const uint8_t *p = msg.c_pktdat;

    EXPECT_EQ(4 /* coap header */
            + 1 /* shortOption length and tag */
            + sizeof shortOption
            + 2 /* mediumOption header length */
            + sizeof mediumOption
            + 1 /* end of options */
            + sizeof thePayload,
            msg.c_pktlen);
    p = msg.c_pktdat;

    EXPECT_EQ(0x40, *p++);
    EXPECT_EQ(0x02, *p++);
    EXPECT_EQ(0x00, *p++);
    EXPECT_EQ(0x00, *p++);

    /* first option */
    EXPECT_EQ(0x30 + sizeof shortOption, *p++) << "short option header"; 
    for (int i = 0; i < sizeof shortOption; ++i)
        EXPECT_EQ(shortOption[i], *p++) << "short option value"; 

    /* second, mediumOption */
    EXPECT_EQ(0x0d, *p++) << "medium option header";
    EXPECT_EQ((sizeof mediumOption - 13), *p++) << "medium option length";
    for (int i = 0; i < sizeof mediumOption; ++i)
        EXPECT_EQ(mediumOption[i], *p++) << "medium option value";

    /* end of options */
    EXPECT_EQ(0xff, *p++) << "END OF OPTIONS";

    /* payload */
    for (int i = 0; i < sizeof thePayload; ++i)
        EXPECT_EQ(thePayload[i], *p++); 

    /* extra token data at end of buffer to check */
    EXPECT_EQ(0xA5, *p++);
}

TEST_F(CoapEncode, EncodeWithCodeShortOptionMediumOptionLongOptionAndPayload)
{
    coap_opt opt1, opt2, opt3;
    const uint16_t LONG_TAG = 3000;

    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 2);
    msg.c_pldlen = sizeof thePayload;
    msg.c_plddat = (const uint8_t *)thePayload;
    opt1.o_typ = 3;
    opt1.o_len = sizeof shortOption;
    opt1.o_val = (const uint8_t *)shortOption;
    LIST_APPEND(&msg.c_optslst, &opt1.o_nod);
    opt2.o_typ = 3;
    opt2.o_len = sizeof mediumOption;
    opt2.o_val = (const uint8_t *)mediumOption;
    LIST_APPEND(&msg.c_optslst, &opt2.o_nod);
    opt3.o_typ = LONG_TAG;
    opt3.o_len = sizeof longOption;
    opt3.o_val = (const uint8_t *)longOption;
    LIST_APPEND(&msg.c_optslst, &opt3.o_nod);
    
    EXPECT_EQ(COAP_OK,
            coap_encode(&msg,
                longBuffer,
                sizeof longBuffer));

    EXPECT_EQ(longBuffer, msg.c_pktdat);
    const uint8_t *p = msg.c_pktdat;

    EXPECT_EQ(4 /* coap header */
            + 1 /* shortOption length and tag */
            + sizeof shortOption
            + 2 /* mediumOption header and tag and length */
            + sizeof mediumOption
            + 5 /* longOption header and tag and length */
            + sizeof longOption
            + 1 /* end of options */
            + sizeof thePayload,
            msg.c_pktlen);
    p = msg.c_pktdat;

    EXPECT_EQ(0x40, *p++);
    EXPECT_EQ(0x02, *p++);
    EXPECT_EQ(0x00, *p++);
    EXPECT_EQ(0x00, *p++);

    /* first option */
    EXPECT_EQ(0x30 + sizeof shortOption, *p++) << "short option header"; 
    for (int i = 0; i < sizeof shortOption; ++i)
        EXPECT_EQ(shortOption[i], *p++) << "short option data"; 

    /* second, mediumOption */
    EXPECT_EQ(0x0D, *p++) << "medium option header";
    EXPECT_EQ((sizeof mediumOption - 13), *p++) << "medium option length";
    for (int i = 0; i < sizeof mediumOption; ++i)
        EXPECT_EQ(mediumOption[i], *p++) << "medium option data";

    /* third, longOption */
    EXPECT_EQ(0xEE, *p++) << "large option header";
    /* -3 because first option was 3 */
    EXPECT_EQ((LONG_TAG - 3 - 13 - 256) >>   8, *p++) << "tag, high byte";
    EXPECT_EQ((LONG_TAG - 3 - 13 - 256) & 0xff, *p++) << "tag, low byte";
    EXPECT_EQ((sizeof longOption - 13 - 256) >>   8, *p++) << "long option length, high byte";
    EXPECT_EQ((sizeof longOption - 13 - 256) & 0xff, *p++) << "long option length, high byte";
    for (int i = 0; i < sizeof longOption; ++i)
        EXPECT_EQ(longOption[i], *p++) << "long option data";

    /* end of options */
    EXPECT_EQ(0xff, *p++) << "END OF OPTIONS";

    /* payload */
    for (int i = 0; i < sizeof thePayload; ++i)
        EXPECT_EQ(thePayload[i], *p++) << "payload data"; 

    /* extra token data at end of buffer to check */
    EXPECT_EQ(0xA5, *p++);
}

TEST_F(CoapEncode, EncodeWithShortBuffer)
{
    coap_opt opt1, opt2, opt3;
    const uint16_t LONG_TAG = 3000;

    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 2);
    msg.c_pldlen = sizeof thePayload;
    msg.c_plddat = (const uint8_t *)thePayload;
    opt1.o_typ = 3;
    opt1.o_len = sizeof shortOption;
    opt1.o_val = (const uint8_t *)shortOption;
    LIST_APPEND(&msg.c_optslst, &opt1.o_nod);
    opt2.o_typ = 3;
    opt2.o_len = sizeof mediumOption;
    opt2.o_val = (const uint8_t *)mediumOption;
    LIST_APPEND(&msg.c_optslst, &opt2.o_nod);
    opt3.o_typ = LONG_TAG;
    opt3.o_len = sizeof longOption;
    opt3.o_val = (const uint8_t *)longOption;
    LIST_APPEND(&msg.c_optslst, &opt3.o_nod);
    
    EXPECT_EQ(COAP_INVALID_LENGTH,
            coap_encode(&msg,
                buffer,
                sizeof buffer));
}

TEST_F(CoapEncode, EncodeWithInvalidOption)
{
    coap_opt opt1, opt2, opt3;
    const uint16_t LONG_TAG = 3000;

    msg.c_typ = coap_typ_CON;
    msg.c_code = COAP_CODE(0, 2);
    msg.c_pldlen = sizeof thePayload;
    msg.c_plddat = (const uint8_t *)thePayload;
    opt1.o_typ = 3;
    opt1.o_len = sizeof shortOption;
    opt1.o_val = (const uint8_t *)shortOption;
    LIST_APPEND(&msg.c_optslst, &opt1.o_nod);
    opt2.o_typ = 2; /* this is invalid, opts must be in ascending order */
    opt2.o_len = sizeof mediumOption;
    opt2.o_val = (const uint8_t *)mediumOption;
    LIST_APPEND(&msg.c_optslst, &opt2.o_nod);
    
    EXPECT_EQ(COAP_INVALID_PARAMETER,
            coap_encode(&msg,
                longBuffer,
                sizeof longBuffer));
}

struct CoapEncodeUnsigned: ::testing::Test {
    uint8_t buffer[10];
    uint8_t *p;
    CoapEncodeUnsigned() {
        memset(buffer, 0xA5, sizeof buffer);
        p = buffer;
    } /* CoapEncodeUnsigned() */
}; /* CoapEncodeUnsigned */

TEST_F(CoapEncodeUnsigned, EncodeWithBuffer6Enough)
{
    EXPECT_EQ(COAP_OK,
            coap_encode_unsigned(0x76543210, buffer, 6));
    EXPECT_EQ(0, *p++);
    EXPECT_EQ(0, *p++);
    EXPECT_EQ(0x76, *p++);
    EXPECT_EQ(0x54, *p++);
    EXPECT_EQ(0x32, *p++);
    EXPECT_EQ(0x10, *p++);
    EXPECT_EQ(0xA5, *p++);
}

TEST_F(CoapEncodeUnsigned, EncodeWithBuffer4Enough)
{
    EXPECT_EQ(COAP_OK,
            coap_encode_unsigned(0x76543210, buffer, 4));
    EXPECT_EQ(0x76, *p++);
    EXPECT_EQ(0x54, *p++);
    EXPECT_EQ(0x32, *p++);
    EXPECT_EQ(0x10, *p++);
    EXPECT_EQ(0xA5, *p++);
}

TEST_F(CoapEncodeUnsigned, EncodeWithBuffer3Fail)
{
    EXPECT_EQ(COAP_INVALID_LENGTH,
            coap_encode_unsigned(0x76543210, buffer, 3));
    EXPECT_NE(0xA5,*p++);
    EXPECT_NE(0xA5,*p++);
    EXPECT_NE(0xA5,*p++);
    EXPECT_EQ(0xA5, *p++);
}

