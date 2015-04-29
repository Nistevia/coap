/* coap_sap_ut.cc -- Unit tests for CoAP Service Access Point.
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: mié abr 22 18:37:34 EEST 2015
 * Disclaimer: (C) 2015 LUIS COLORADO.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "coap.h"

struct CoapSAP: ::testing::Test {
    struct sockaddr_in sa;
    coap_sap           sap;

    CoapSAP() {
        sa.sin_family = AF_INET;
        sa.sin_port = htons(5683);
        sa.sin_addr.s_addr = INADDR_ANY;
    } /* CoapSAP() */

}; /* CoapSAP */

#if 0
TEST_F(CoapSAP, newCoapSAP) {
    EXPECT_EQ(COAP_OK, 
            new_coap_sap(&sa, &sap));
}
#endif
