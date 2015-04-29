/* coap_mock.hh -- mock object for coap interface.
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Mon Apr 20 11:12:59 EEST 2015
 * Disclaimer: (C) 2015 LUIS COLORADO.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdint.h>

extern "C" {
#include "coap.h"
};

class CoapMock: ::testing::Mock {
    static CoapMock instance;
    CoapMock(){}
public:
    MOCK_METHOD1(coap_typ2str, char *(coap_typ));
    MOCK_METHOD1(coap_msg_init, coap_err(coap_msg *tgt));
    MOCK_METHOD2(coap_msg_settype, coap_err(coap_msg*, coap_typ));
    MOCK_METHOD2(coap_msg_setcode, coap_err(coap_msg*, uint8_t));
    MOCK_METHOD3(coap_msg_setpayload, coap_err(coap_msg*,
                        const uint8_t*, size_t));
    MOCK_METHOD3(coap_msg_settoken, coap_err(coap_msg*,
                        const uint8_t*, size_t));
    MOCK_METHOD2(coap_msg_setmsgid, coap_err(coap_msg*, uint16_t));
    MOCK_METHOD2(coap_msg_addopt, coap_err(coap_msg *, coap_opt *));
    MOCK_METHOD1(coap_msg_fstopt, coap_opt(coap_msg *));
    MOCK_METHOD1(coap_msg_lstopt, coap_opt(coap_msg *));
    MOCK_METHOD1(coap_msg_nxtopt, coap_opt(coap_opt *));
    MOCK_METHOD1(coap_msg_prvopt, coap_opt(coap_opt *));
    MOCK_METHOD3(coap_parse, coap_err(const uint8_t*, size_t, coap_msg*));
    MOCK_METHOD3(coap_encode, coap_err(coap_msg*, uint8_t*, size_t));
    MOCK_METHOD3(coap_encode_unsigned, coap_err(uint32_t, uint8_t*, size_t));
    MOCK_METHOD3(coap_parse_unsigned, coap_err(const uint8_t*, size_t, uint32_t));

    static CoapMock& getInstance()
    {
        return instance;
    }
};

