/* coap_misc_test.cc -- testing framework for coap_misc.c
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Mon Apr 20 11:17:54 EEST 2015
 * Disclaimer: (C) 2015 LUIS COLORADO.  All rights reserved.
 */

#include <gtest/gtest.h>
#include "coap.h"

TEST(CoapMisc, CoapTyp2Str) {
    EXPECT_STREQ("CON", coap_typ2str(coap_typ_CON));
    EXPECT_STREQ("NON", coap_typ2str(coap_typ_NON));
    EXPECT_STREQ("ACK", coap_typ2str(coap_typ_ACK));
    EXPECT_STREQ("RST", coap_typ2str(coap_typ_RST));
    //EXPECT_STREQ("INVALID", coap_typ2str(123456));
}
