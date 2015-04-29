# Makefile -- makefile of library coap.
# Author: Luis Colorado <luis.colorado@ericsson.com>
# Date: Fri Apr 10 21:16:48 EEST 2015
# Disclaimer: (C) 2015 LUIS COLORADO.  All rights reserved.

coaplib_MAJOR=1
coaplib_MINOR=0.2

targets= $(coaplib) $(coapslib) ut

coaplib=libcoap.so
coaplib_soname=$(coaplib).$(coaplib_MAJOR)
coaplib_fullname=$(coaplib_soname).$(coaplib_MINOR)
coaplib_objs=coap_encode.so coap_parse.so coap_misc.so fprintbuf.so

coapslib=libcoap.a
coapslib_objs=$(coaplib_objs:.so=.o)

ut_objs=coap_ut.o $(coaplib_fullname)
ut_libs=

toclean += $(targets) $(coaplib_soname) $(coaplib_fullname) \
		   $(coaplib_objs) $(coapslib_objs) $(ut_objs)
toclean += $(coaplib_objs:.so=_ut) $(coaplib_objs:.so=_ut.o)

prefix=/usr


objs=$(foreach i, $(targets), $($(i)_objs))

CXXFLAGS += -g0 -O3 -pthread
UTFLAGS += -lgmock_main -lgmock -lgtest

.PHONY: all clean install uts

all: $(targets)
clean:; $(RM) $(toclean)
install:
	mkdir -p $(prefix)/lib
	install $(coaplib_fullname) $(prefix)/lib
	ln -sf $(coaplib_fullname) $(prefix)/lib/$(coaplib_soname)
	ln -sf $(coaplib_soname) $(prefix)/lib/$(coaplib)
	install coap.h coapP.h $(prefix)/include

$(coaplib): $(coaplib_soname)
	ln -sf $< $@
$(coaplib_soname): $(coaplib_fullname)
	ln -sf $< $@
$(coaplib_fullname): $(coaplib_objs)
	$(CC) $(LDFLAGS) -o $@ -shared -Wl,-soname=$(coaplib_soname) $(coaplib_objs)

$(coapslib):$(coapslib)($(coapslib_objs))

%.so:%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $< -o $@
%.so:%.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -fPIC -c $< -o $@
%.o:%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
%.o:%.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

ut: $(ut_objs)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(ut_objs) $(ut_libs) $(UTFLAGS)

uts:
	$(MAKE) $(coaplib_objs:.so=_ut)

%_ut: %_ut.o %.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $*_ut.o $*.o $(UTFLAGS)
	-$@

# DO NOT DELETE

coap_encode.o: /usr/include/assert.h /usr/include/features.h
coap_encode.o: /usr/include/string.h /usr/include/xlocale.h coapP.h coap.h
coap_encode.o: /usr/include/stdlib.h /usr/include/alloca.h
coap_encode.o: /usr/include/stdint.h /usr/include/pthread.h
coap_encode.o: /usr/include/endian.h /usr/include/sched.h /usr/include/time.h
coap_encode.o: /usr/include/netinet/in.h lists.h
coap_encode_test.o: fprintbuf.h /usr/include/stdio.h /usr/include/features.h
coap_encode_test.o: /usr/include/libio.h /usr/include/_G_config.h
coap_encode_test.o: /usr/include/wchar.h /usr/include/stdint.h coapP.h coap.h
coap_encode_test.o: /usr/include/stdlib.h /usr/include/alloca.h
coap_encode_test.o: /usr/include/pthread.h /usr/include/endian.h
coap_encode_test.o: /usr/include/sched.h /usr/include/time.h
coap_encode_test.o: /usr/include/netinet/in.h lists.h
coap_misc.o: /usr/include/assert.h /usr/include/features.h coapP.h coap.h
coap_misc.o: /usr/include/stdlib.h /usr/include/alloca.h
coap_misc.o: /usr/include/stdint.h /usr/include/pthread.h
coap_misc.o: /usr/include/endian.h /usr/include/sched.h /usr/include/time.h
coap_misc.o: /usr/include/netinet/in.h lists.h
coap_parse.o: /usr/include/assert.h /usr/include/features.h coapP.h coap.h
coap_parse.o: /usr/include/stdlib.h /usr/include/alloca.h
coap_parse.o: /usr/include/stdint.h /usr/include/pthread.h
coap_parse.o: /usr/include/endian.h /usr/include/sched.h /usr/include/time.h
coap_parse.o: /usr/include/netinet/in.h lists.h
fprintbuf.o: /usr/include/ctype.h /usr/include/features.h
fprintbuf.o: /usr/include/endian.h /usr/include/xlocale.h
fprintbuf.o: /usr/include/stdio.h /usr/include/libio.h
fprintbuf.o: /usr/include/_G_config.h /usr/include/wchar.h fprintbuf.h
fprintbuf.o: /usr/include/stdint.h
coap_encode_ut.o: /usr/include/gtest/gtest.h
coap_encode_ut.o: /usr/include/gtest/internal/gtest-internal.h
coap_encode_ut.o: /usr/include/gtest/internal/gtest-port.h
coap_encode_ut.o: /usr/include/ctype.h /usr/include/features.h
coap_encode_ut.o: /usr/include/endian.h /usr/include/xlocale.h
coap_encode_ut.o: /usr/include/stdlib.h /usr/include/alloca.h
coap_encode_ut.o: /usr/include/stdio.h /usr/include/libio.h
coap_encode_ut.o: /usr/include/_G_config.h /usr/include/wchar.h
coap_encode_ut.o: /usr/include/string.h /usr/include/unistd.h
coap_encode_ut.o: /usr/include/getopt.h /usr/include/strings.h
coap_encode_ut.o: /usr/include/regex.h /usr/include/gtest/gtest-message.h
coap_encode_ut.o: /usr/include/gtest/internal/gtest-string.h
coap_encode_ut.o: /usr/include/gtest/internal/gtest-filepath.h
coap_encode_ut.o: /usr/include/gtest/internal/gtest-type-util.h
coap_encode_ut.o: /usr/include/gtest/gtest-death-test.h
coap_encode_ut.o: /usr/include/gtest/internal/gtest-death-test-internal.h
coap_encode_ut.o: /usr/include/gtest/gtest-param-test.h
coap_encode_ut.o: /usr/include/gtest/internal/gtest-param-util.h
coap_encode_ut.o: /usr/include/gtest/internal/gtest-linked_ptr.h
coap_encode_ut.o: /usr/include/assert.h /usr/include/gtest/gtest-printers.h
coap_encode_ut.o: /usr/include/gtest/internal/gtest-param-util-generated.h
coap_encode_ut.o: /usr/include/gtest/gtest_prod.h
coap_encode_ut.o: /usr/include/gtest/gtest-test-part.h
coap_encode_ut.o: /usr/include/gtest/gtest-typed-test.h
coap_encode_ut.o: /usr/include/gtest/gtest_pred_impl.h
coap_encode_ut.o: /usr/include/gmock/gmock.h
coap_encode_ut.o: /usr/include/gmock/gmock-actions.h /usr/include/errno.h
coap_encode_ut.o: /usr/include/gmock/internal/gmock-internal-utils.h
coap_encode_ut.o: /usr/include/gmock/internal/gmock-generated-internal-utils.h
coap_encode_ut.o: /usr/include/gmock/internal/gmock-port.h
coap_encode_ut.o: /usr/include/gmock/gmock-cardinalities.h
coap_encode_ut.o: /usr/include/limits.h
coap_encode_ut.o: /usr/include/gmock/gmock-generated-actions.h
coap_encode_ut.o: /usr/include/gmock/gmock-generated-function-mockers.h
coap_encode_ut.o: /usr/include/gmock/gmock-spec-builders.h
coap_encode_ut.o: /usr/include/gmock/gmock-matchers.h /usr/include/math.h
coap_encode_ut.o: /usr/include/gmock/gmock-generated-nice-strict.h
coap_encode_ut.o: /usr/include/gmock/gmock-generated-matchers.h
coap_encode_ut.o: /usr/include/gmock/gmock-more-actions.h
coap_encode_ut.o: /usr/include/gmock/gmock-more-matchers.h fprintbuf.h
coap_encode_ut.o: /usr/include/stdint.h coap.h /usr/include/pthread.h
coap_encode_ut.o: /usr/include/sched.h /usr/include/time.h
coap_encode_ut.o: /usr/include/netinet/in.h lists.h
coap_misc_ut.o: /usr/include/gtest/gtest.h
coap_misc_ut.o: /usr/include/gtest/internal/gtest-internal.h
coap_misc_ut.o: /usr/include/gtest/internal/gtest-port.h /usr/include/ctype.h
coap_misc_ut.o: /usr/include/features.h /usr/include/endian.h
coap_misc_ut.o: /usr/include/xlocale.h /usr/include/stdlib.h
coap_misc_ut.o: /usr/include/alloca.h /usr/include/stdio.h
coap_misc_ut.o: /usr/include/libio.h /usr/include/_G_config.h
coap_misc_ut.o: /usr/include/wchar.h /usr/include/string.h
coap_misc_ut.o: /usr/include/unistd.h /usr/include/getopt.h
coap_misc_ut.o: /usr/include/strings.h /usr/include/regex.h
coap_misc_ut.o: /usr/include/gtest/gtest-message.h
coap_misc_ut.o: /usr/include/gtest/internal/gtest-string.h
coap_misc_ut.o: /usr/include/gtest/internal/gtest-filepath.h
coap_misc_ut.o: /usr/include/gtest/internal/gtest-type-util.h
coap_misc_ut.o: /usr/include/gtest/gtest-death-test.h
coap_misc_ut.o: /usr/include/gtest/internal/gtest-death-test-internal.h
coap_misc_ut.o: /usr/include/gtest/gtest-param-test.h
coap_misc_ut.o: /usr/include/gtest/internal/gtest-param-util.h
coap_misc_ut.o: /usr/include/gtest/internal/gtest-linked_ptr.h
coap_misc_ut.o: /usr/include/assert.h /usr/include/gtest/gtest-printers.h
coap_misc_ut.o: /usr/include/gtest/internal/gtest-param-util-generated.h
coap_misc_ut.o: /usr/include/gtest/gtest_prod.h
coap_misc_ut.o: /usr/include/gtest/gtest-test-part.h
coap_misc_ut.o: /usr/include/gtest/gtest-typed-test.h
coap_misc_ut.o: /usr/include/gtest/gtest_pred_impl.h coap.h
coap_misc_ut.o: /usr/include/stdint.h /usr/include/pthread.h
coap_misc_ut.o: /usr/include/sched.h /usr/include/time.h
coap_misc_ut.o: /usr/include/netinet/in.h lists.h
coap_mock.o: coap_mock.hh /usr/include/gtest/gtest.h
coap_mock.o: /usr/include/gtest/internal/gtest-internal.h
coap_mock.o: /usr/include/gtest/internal/gtest-port.h /usr/include/ctype.h
coap_mock.o: /usr/include/features.h /usr/include/endian.h
coap_mock.o: /usr/include/xlocale.h /usr/include/stdlib.h
coap_mock.o: /usr/include/alloca.h /usr/include/stdio.h /usr/include/libio.h
coap_mock.o: /usr/include/_G_config.h /usr/include/wchar.h
coap_mock.o: /usr/include/string.h /usr/include/unistd.h
coap_mock.o: /usr/include/getopt.h /usr/include/strings.h
coap_mock.o: /usr/include/regex.h /usr/include/gtest/gtest-message.h
coap_mock.o: /usr/include/gtest/internal/gtest-string.h
coap_mock.o: /usr/include/gtest/internal/gtest-filepath.h
coap_mock.o: /usr/include/gtest/internal/gtest-type-util.h
coap_mock.o: /usr/include/gtest/gtest-death-test.h
coap_mock.o: /usr/include/gtest/internal/gtest-death-test-internal.h
coap_mock.o: /usr/include/gtest/gtest-param-test.h
coap_mock.o: /usr/include/gtest/internal/gtest-param-util.h
coap_mock.o: /usr/include/gtest/internal/gtest-linked_ptr.h
coap_mock.o: /usr/include/assert.h /usr/include/gtest/gtest-printers.h
coap_mock.o: /usr/include/gtest/internal/gtest-param-util-generated.h
coap_mock.o: /usr/include/gtest/gtest_prod.h
coap_mock.o: /usr/include/gtest/gtest-test-part.h
coap_mock.o: /usr/include/gtest/gtest-typed-test.h
coap_mock.o: /usr/include/gtest/gtest_pred_impl.h /usr/include/gmock/gmock.h
coap_mock.o: /usr/include/gmock/gmock-actions.h /usr/include/errno.h
coap_mock.o: /usr/include/gmock/internal/gmock-internal-utils.h
coap_mock.o: /usr/include/gmock/internal/gmock-generated-internal-utils.h
coap_mock.o: /usr/include/gmock/internal/gmock-port.h
coap_mock.o: /usr/include/gmock/gmock-cardinalities.h /usr/include/limits.h
coap_mock.o: /usr/include/gmock/gmock-generated-actions.h
coap_mock.o: /usr/include/gmock/gmock-generated-function-mockers.h
coap_mock.o: /usr/include/gmock/gmock-spec-builders.h
coap_mock.o: /usr/include/gmock/gmock-matchers.h /usr/include/math.h
coap_mock.o: /usr/include/gmock/gmock-generated-nice-strict.h
coap_mock.o: /usr/include/gmock/gmock-generated-matchers.h
coap_mock.o: /usr/include/gmock/gmock-more-actions.h
coap_mock.o: /usr/include/gmock/gmock-more-matchers.h /usr/include/stdint.h
coap_mock.o: coap.h /usr/include/pthread.h /usr/include/sched.h
coap_mock.o: /usr/include/time.h /usr/include/netinet/in.h lists.h
coap_parse_ut.o: /usr/include/gtest/gtest.h
coap_parse_ut.o: /usr/include/gtest/internal/gtest-internal.h
coap_parse_ut.o: /usr/include/gtest/internal/gtest-port.h
coap_parse_ut.o: /usr/include/ctype.h /usr/include/features.h
coap_parse_ut.o: /usr/include/endian.h /usr/include/xlocale.h
coap_parse_ut.o: /usr/include/stdlib.h /usr/include/alloca.h
coap_parse_ut.o: /usr/include/stdio.h /usr/include/libio.h
coap_parse_ut.o: /usr/include/_G_config.h /usr/include/wchar.h
coap_parse_ut.o: /usr/include/string.h /usr/include/unistd.h
coap_parse_ut.o: /usr/include/getopt.h /usr/include/strings.h
coap_parse_ut.o: /usr/include/regex.h /usr/include/gtest/gtest-message.h
coap_parse_ut.o: /usr/include/gtest/internal/gtest-string.h
coap_parse_ut.o: /usr/include/gtest/internal/gtest-filepath.h
coap_parse_ut.o: /usr/include/gtest/internal/gtest-type-util.h
coap_parse_ut.o: /usr/include/gtest/gtest-death-test.h
coap_parse_ut.o: /usr/include/gtest/internal/gtest-death-test-internal.h
coap_parse_ut.o: /usr/include/gtest/gtest-param-test.h
coap_parse_ut.o: /usr/include/gtest/internal/gtest-param-util.h
coap_parse_ut.o: /usr/include/gtest/internal/gtest-linked_ptr.h
coap_parse_ut.o: /usr/include/assert.h /usr/include/gtest/gtest-printers.h
coap_parse_ut.o: /usr/include/gtest/internal/gtest-param-util-generated.h
coap_parse_ut.o: /usr/include/gtest/gtest_prod.h
coap_parse_ut.o: /usr/include/gtest/gtest-test-part.h
coap_parse_ut.o: /usr/include/gtest/gtest-typed-test.h
coap_parse_ut.o: /usr/include/gtest/gtest_pred_impl.h /usr/include/stdint.h
coap_parse_ut.o: coap_mock.hh /usr/include/gmock/gmock.h
coap_parse_ut.o: /usr/include/gmock/gmock-actions.h /usr/include/errno.h
coap_parse_ut.o: /usr/include/gmock/internal/gmock-internal-utils.h
coap_parse_ut.o: /usr/include/gmock/internal/gmock-generated-internal-utils.h
coap_parse_ut.o: /usr/include/gmock/internal/gmock-port.h
coap_parse_ut.o: /usr/include/gmock/gmock-cardinalities.h
coap_parse_ut.o: /usr/include/limits.h
coap_parse_ut.o: /usr/include/gmock/gmock-generated-actions.h
coap_parse_ut.o: /usr/include/gmock/gmock-generated-function-mockers.h
coap_parse_ut.o: /usr/include/gmock/gmock-spec-builders.h
coap_parse_ut.o: /usr/include/gmock/gmock-matchers.h /usr/include/math.h
coap_parse_ut.o: /usr/include/gmock/gmock-generated-nice-strict.h
coap_parse_ut.o: /usr/include/gmock/gmock-generated-matchers.h
coap_parse_ut.o: /usr/include/gmock/gmock-more-actions.h
coap_parse_ut.o: /usr/include/gmock/gmock-more-matchers.h coap.h
coap_parse_ut.o: /usr/include/pthread.h /usr/include/sched.h
coap_parse_ut.o: /usr/include/time.h /usr/include/netinet/in.h lists.h
coap_sap_ut.o: /usr/include/gtest/gtest.h
coap_sap_ut.o: /usr/include/gtest/internal/gtest-internal.h
coap_sap_ut.o: /usr/include/gtest/internal/gtest-port.h /usr/include/ctype.h
coap_sap_ut.o: /usr/include/features.h /usr/include/endian.h
coap_sap_ut.o: /usr/include/xlocale.h /usr/include/stdlib.h
coap_sap_ut.o: /usr/include/alloca.h /usr/include/stdio.h
coap_sap_ut.o: /usr/include/libio.h /usr/include/_G_config.h
coap_sap_ut.o: /usr/include/wchar.h /usr/include/string.h
coap_sap_ut.o: /usr/include/unistd.h /usr/include/getopt.h
coap_sap_ut.o: /usr/include/strings.h /usr/include/regex.h
coap_sap_ut.o: /usr/include/gtest/gtest-message.h
coap_sap_ut.o: /usr/include/gtest/internal/gtest-string.h
coap_sap_ut.o: /usr/include/gtest/internal/gtest-filepath.h
coap_sap_ut.o: /usr/include/gtest/internal/gtest-type-util.h
coap_sap_ut.o: /usr/include/gtest/gtest-death-test.h
coap_sap_ut.o: /usr/include/gtest/internal/gtest-death-test-internal.h
coap_sap_ut.o: /usr/include/gtest/gtest-param-test.h
coap_sap_ut.o: /usr/include/gtest/internal/gtest-param-util.h
coap_sap_ut.o: /usr/include/gtest/internal/gtest-linked_ptr.h
coap_sap_ut.o: /usr/include/assert.h /usr/include/gtest/gtest-printers.h
coap_sap_ut.o: /usr/include/gtest/internal/gtest-param-util-generated.h
coap_sap_ut.o: /usr/include/gtest/gtest_prod.h
coap_sap_ut.o: /usr/include/gtest/gtest-test-part.h
coap_sap_ut.o: /usr/include/gtest/gtest-typed-test.h
coap_sap_ut.o: /usr/include/gtest/gtest_pred_impl.h
coap_sap_ut.o: /usr/include/gmock/gmock.h /usr/include/gmock/gmock-actions.h
coap_sap_ut.o: /usr/include/errno.h
coap_sap_ut.o: /usr/include/gmock/internal/gmock-internal-utils.h
coap_sap_ut.o: /usr/include/gmock/internal/gmock-generated-internal-utils.h
coap_sap_ut.o: /usr/include/gmock/internal/gmock-port.h
coap_sap_ut.o: /usr/include/gmock/gmock-cardinalities.h /usr/include/limits.h
coap_sap_ut.o: /usr/include/gmock/gmock-generated-actions.h
coap_sap_ut.o: /usr/include/gmock/gmock-generated-function-mockers.h
coap_sap_ut.o: /usr/include/gmock/gmock-spec-builders.h
coap_sap_ut.o: /usr/include/gmock/gmock-matchers.h /usr/include/math.h
coap_sap_ut.o: /usr/include/gmock/gmock-generated-nice-strict.h
coap_sap_ut.o: /usr/include/gmock/gmock-generated-matchers.h
coap_sap_ut.o: /usr/include/gmock/gmock-more-actions.h
coap_sap_ut.o: /usr/include/gmock/gmock-more-matchers.h coap.h
coap_sap_ut.o: /usr/include/stdint.h /usr/include/pthread.h
coap_sap_ut.o: /usr/include/sched.h /usr/include/time.h
coap_sap_ut.o: /usr/include/netinet/in.h lists.h
coap_ut.o: coap_sap_ut.cc /usr/include/gtest/gtest.h
coap_ut.o: /usr/include/gtest/internal/gtest-internal.h
coap_ut.o: /usr/include/gtest/internal/gtest-port.h /usr/include/ctype.h
coap_ut.o: /usr/include/features.h /usr/include/endian.h
coap_ut.o: /usr/include/xlocale.h /usr/include/stdlib.h /usr/include/alloca.h
coap_ut.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
coap_ut.o: /usr/include/wchar.h /usr/include/string.h /usr/include/unistd.h
coap_ut.o: /usr/include/getopt.h /usr/include/strings.h /usr/include/regex.h
coap_ut.o: /usr/include/gtest/gtest-message.h
coap_ut.o: /usr/include/gtest/internal/gtest-string.h
coap_ut.o: /usr/include/gtest/internal/gtest-filepath.h
coap_ut.o: /usr/include/gtest/internal/gtest-type-util.h
coap_ut.o: /usr/include/gtest/gtest-death-test.h
coap_ut.o: /usr/include/gtest/internal/gtest-death-test-internal.h
coap_ut.o: /usr/include/gtest/gtest-param-test.h
coap_ut.o: /usr/include/gtest/internal/gtest-param-util.h
coap_ut.o: /usr/include/gtest/internal/gtest-linked_ptr.h
coap_ut.o: /usr/include/assert.h /usr/include/gtest/gtest-printers.h
coap_ut.o: /usr/include/gtest/internal/gtest-param-util-generated.h
coap_ut.o: /usr/include/gtest/gtest_prod.h
coap_ut.o: /usr/include/gtest/gtest-test-part.h
coap_ut.o: /usr/include/gtest/gtest-typed-test.h
coap_ut.o: /usr/include/gtest/gtest_pred_impl.h /usr/include/gmock/gmock.h
coap_ut.o: /usr/include/gmock/gmock-actions.h /usr/include/errno.h
coap_ut.o: /usr/include/gmock/internal/gmock-internal-utils.h
coap_ut.o: /usr/include/gmock/internal/gmock-generated-internal-utils.h
coap_ut.o: /usr/include/gmock/internal/gmock-port.h
coap_ut.o: /usr/include/gmock/gmock-cardinalities.h /usr/include/limits.h
coap_ut.o: /usr/include/gmock/gmock-generated-actions.h
coap_ut.o: /usr/include/gmock/gmock-generated-function-mockers.h
coap_ut.o: /usr/include/gmock/gmock-spec-builders.h
coap_ut.o: /usr/include/gmock/gmock-matchers.h /usr/include/math.h
coap_ut.o: /usr/include/gmock/gmock-generated-nice-strict.h
coap_ut.o: /usr/include/gmock/gmock-generated-matchers.h
coap_ut.o: /usr/include/gmock/gmock-more-actions.h
coap_ut.o: /usr/include/gmock/gmock-more-matchers.h coap.h
coap_ut.o: /usr/include/stdint.h /usr/include/pthread.h /usr/include/sched.h
coap_ut.o: /usr/include/time.h /usr/include/netinet/in.h lists.h
coap_ut.o: coap_misc_ut.cc coap_encode_ut.cc fprintbuf.h coap_parse_ut.cc
coap_ut.o: coap_mock.hh fprintbuf_ut.cc
fprintbuf_ut.o: /usr/include/gtest/gtest.h
fprintbuf_ut.o: /usr/include/gtest/internal/gtest-internal.h
fprintbuf_ut.o: /usr/include/gtest/internal/gtest-port.h /usr/include/ctype.h
fprintbuf_ut.o: /usr/include/features.h /usr/include/endian.h
fprintbuf_ut.o: /usr/include/xlocale.h /usr/include/stdlib.h
fprintbuf_ut.o: /usr/include/alloca.h /usr/include/stdio.h
fprintbuf_ut.o: /usr/include/libio.h /usr/include/_G_config.h
fprintbuf_ut.o: /usr/include/wchar.h /usr/include/string.h
fprintbuf_ut.o: /usr/include/unistd.h /usr/include/getopt.h
fprintbuf_ut.o: /usr/include/strings.h /usr/include/regex.h
fprintbuf_ut.o: /usr/include/gtest/gtest-message.h
fprintbuf_ut.o: /usr/include/gtest/internal/gtest-string.h
fprintbuf_ut.o: /usr/include/gtest/internal/gtest-filepath.h
fprintbuf_ut.o: /usr/include/gtest/internal/gtest-type-util.h
fprintbuf_ut.o: /usr/include/gtest/gtest-death-test.h
fprintbuf_ut.o: /usr/include/gtest/internal/gtest-death-test-internal.h
fprintbuf_ut.o: /usr/include/gtest/gtest-param-test.h
fprintbuf_ut.o: /usr/include/gtest/internal/gtest-param-util.h
fprintbuf_ut.o: /usr/include/gtest/internal/gtest-linked_ptr.h
fprintbuf_ut.o: /usr/include/assert.h /usr/include/gtest/gtest-printers.h
fprintbuf_ut.o: /usr/include/gtest/internal/gtest-param-util-generated.h
fprintbuf_ut.o: /usr/include/gtest/gtest_prod.h
fprintbuf_ut.o: /usr/include/gtest/gtest-test-part.h
fprintbuf_ut.o: /usr/include/gtest/gtest-typed-test.h
fprintbuf_ut.o: /usr/include/gtest/gtest_pred_impl.h coap.h
fprintbuf_ut.o: /usr/include/stdint.h /usr/include/pthread.h
fprintbuf_ut.o: /usr/include/sched.h /usr/include/time.h
fprintbuf_ut.o: /usr/include/netinet/in.h lists.h fprintbuf.h
