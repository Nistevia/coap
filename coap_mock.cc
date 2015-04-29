/* coap_mock.cc -- stub function implementations for C calls
 * to mock.
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Wed Apr 29 11:07:49 EEST 2015
 * Disclaimer: (C) 2015 LUIS COLORADO.  All rights reserved.
 */

#include "coap_mock.hh"

CoapMock CoapMock::instance;

coap_err coap_msg_init(coap_msg *tgt)
{
    return CoapMock::getInstance().coap_msg_init(tgt);
}

coap_err coap_msg_addopt(coap_msg *tgt, coap_opt *opt)
{
    return CoapMock::getInstance().coap_msg_addopt(tgt, opt);
}
