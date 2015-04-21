/* coap_misc_ut.cc -- Unit Tests for coap_misc.c
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: mar abr 21 21:28:20 EEST 2015
 * Disclaimer: (C) 2015 LUIS COLORADO.  All rights reserved.
 */

#include <gtest/gtest.h>
#include "coap.h"
#include <cstdio>
#include "fprintbuf.h"

uint8_t buf[] = "TEST";

TEST(Misc, a) {
    EXPECT_LT(0, fprintbuf(stdout, sizeof buf, buf, "TEST (%d bytes)", sizeof buf));
}
