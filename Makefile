# Makefile -- makefile of library coap.
# Author: Luis Colorado <luis.colorado@ericsson.com>
# Date: Fri Apr 10 21:16:48 EEST 2015
# Disclaimer: (C) 2015 LUIS COLORADO.  All rights reserved.

targets=coap_encode_test

objs=$(foreach i, $(targets), $($(i)_objs))

CXXFLAGS += -g3 -O0 -pthread
UTFLAGS = -lgmock_main -lgmock -lgtest

.PHONY: all clean ut-setup

all: $(targets)
	@echo all: TARGETS=$(targets) OBJS=$(objs)
clean:
	$(RM) $(targets) $(objs) $(objs:.o=_ut) $(foreach i, $(objs:.o=_ut), $($(i)_objs))

ut-setup:
	$(MAKE) $(objs:.o=_ut)

ut-run:
	$(MAKE) $(objs:.o=_utrun)

%_utrun:%_ut
	$<

%.so:%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $< -o $@
%.so:%.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -fPIC -c $< -o $@
%.o:%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
%.o:%.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
%_utrun: %_ut
	-$< $(UTRUNFLAGS)

coap_encode_test_objs=coap_encode_test.o coap_encode.o coap_parse.o coap_misc.o fprintbuf.o
coap_encode_test_libs=
coap_encode_test: $(coap_encode_test_objs)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(coap_encode_test_objs) $(coap_encode_test_libs)

coap_encode_test_ut:

coap_encode_ut_objs=coap_encode_ut.o coap_encode.o fprintbuf.o
coap_encode_ut_libs=
coap_encode_ut: $(coap_encode_ut_objs)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(coap_encode_ut_objs) $(coap_encode_ut_libs) $(UTFLAGS)

coap_parse_ut_objs=coap_parse_ut.o coap_parse.o coap_misc.o fprintbuf.o
coap_parse_ut_libs=
coap_parse_ut: $(coap_parse_ut_objs)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(coap_parse_ut_objs) $(coap_parse_ut_libs) $(UTFLAGS)

coap_misc_ut_objs=coap_misc_ut.o coap_misc.o fprintbuf.o
coap_misc_ut_libs=
coap_misc_ut: $(coap_misc_ut_objs)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(coap_misc_ut_objs) $(coap_misc_ut_libs) $(UTFLAGS)

fprintbuf_ut_objs=fprintbuf_ut.o fprintbuf.o
fprintbuf_ut_libs=
fprintbuf_ut: $(fprintbuf_ut_objs)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(fprintbuf_ut_objs) $(fprintbuf_ut_libs) $(UTFLAGS)


# DO NOT DELETE

coap_encode.o: /usr/include/assert.h /usr/include/features.h
coap_encode.o: /usr/include/string.h /usr/include/xlocale.h coapP.h coap.h
coap_encode.o: /usr/include/stdlib.h /usr/include/alloca.h
coap_encode.o: /usr/include/stdint.h lists.h
coap_encode_test.o: fprintbuf.h /usr/include/stdio.h /usr/include/features.h
coap_encode_test.o: /usr/include/libio.h /usr/include/_G_config.h
coap_encode_test.o: /usr/include/wchar.h /usr/include/stdint.h coapP.h coap.h
coap_encode_test.o: /usr/include/stdlib.h /usr/include/alloca.h lists.h
coap_misc.o: /usr/include/assert.h /usr/include/features.h coapP.h coap.h
coap_misc.o: /usr/include/stdlib.h /usr/include/alloca.h
coap_misc.o: /usr/include/stdint.h lists.h
coap_parse.o: /usr/include/assert.h /usr/include/features.h coapP.h coap.h
coap_parse.o: /usr/include/stdlib.h /usr/include/alloca.h
coap_parse.o: /usr/include/stdint.h lists.h
fprintbuf.o: /usr/include/ctype.h /usr/include/features.h
fprintbuf.o: /usr/include/endian.h /usr/include/xlocale.h
fprintbuf.o: /usr/include/stdio.h /usr/include/libio.h
fprintbuf.o: /usr/include/_G_config.h /usr/include/wchar.h fprintbuf.h
fprintbuf.o: /usr/include/stdint.h
