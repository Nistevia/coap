# Makefile -- makefile of library coap.
# Author: Luis Colorado <luis.colorado@ericsson.com>
# Date: Fri Apr 10 21:16:48 EEST 2015
# Disclaimer: (C) 2015 LUIS COLORADO.  All rights reserved.

targets=coap_encode_test

objs=$(foreach i, $(targets), $($(i)_objs))

.PHONY: all clean

all: $(targets)
clean:
	$(RM) $(targets) ${foreach i, $(targets), $($(i)_objs)}

%.so:%.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

coap_encode_test_objs = coap_encode_test.o coap_encode.o coap_parse.o coap_misc.o fprintbuf.o

coap_encode_test: ${coap_encode_test_objs}
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ ${coap_encode_test_objs} ${coap_encode_tst_libs}

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
