/* coap_misc_ut.cc -- Unit Tests for coap_misc.c
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: mar abr 21 21:28:20 EEST 2015
 * Disclaimer: (C) 2015 LUIS COLORADO.  All rights reserved.
 */

#include <gtest/gtest.h>
#include "coap.h"
#include <cstdio>
#include "fprintbuf.h"
#include <iostream>

TEST(Misc, zeroLengthBuffer) {
    size_t res;

    EXPECT_LT(0,
            res = fprintbuf(stdout,
                0, 0,
                "TEST (%d bytes)",
                0));
    std::cout << res << " bytes output" << std::endl;
}

TEST(Misc, lessThanARowBuffer) {
    size_t res;
    uint8_t buf[] = "TEST";

    EXPECT_LT(0,
            res = fprintbuf(stdout,
                sizeof buf, buf,
                "TEST (%d bytes)",
                sizeof buf));
    std::cout << res << " bytes output" << std::endl;
}

TEST(Misc, moreThanARowBuffer) {
    size_t res;
    uint8_t buf[] =
        "This is a long TEST BUFFER to show "
        "how it splits output in several "
        "lines";


    EXPECT_LT(0,
            res = fprintbuf(stdout,
                sizeof buf, buf,
                "TEST (%d bytes)",
                sizeof buf));
    std::cout << res << " bytes output" << std::endl;
}
