/* coap_misc_ut.cc -- Unit Tests for coap_misc.c
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: mar abr 21 21:28:20 EEST 2015
 * Disclaimer: (C) 2015 LUIS COLORADO.  All rights reserved.
 */

#include <gtest/gtest.h>
#include "coap.h"

struct Misc: ::testing::Test {
    coap_msg            msg;
};

TEST_F(Misc, a) {
    coap_msg_init(&msg);
    EXPECT_EQ(0, msg.c_pktdat);
    EXPECT_EQ(0, msg.c_pktlen);
}
