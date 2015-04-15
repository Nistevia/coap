# Makefile -- makefile of library coap.
# Author: Luis Colorado <luis.colorado@ericsson.com>
# Date: Fri Apr 10 21:16:48 EEST 2015
# Disclaimer: (C) 2015 LUIS COLORADO.  All rights reserved.

libcoap_base=libcoap.so
libcoap_MAJOR=1
libcoap_MINOR=0
libcoap_objs=coap_encode.so coap_parse.so coap_misc.so fprintbuf.so

libcoap_SONAME=${libcoap_base}.${libcoap_MAJOR}
libcoap_lib=${libcoap_SONAME}.${libcoap_MINOR}
${libcoap_lib}: ${libcoap_objs}
	$(CC) -o ${libcoap_lib} -shared -Wl,-soname=${libcoap_SONAME} ${libcoap_objs} ${libcoap_libs}
	ln -s ${libcoap_lib} ${libcoap_SONAME}
	ln -s ${libcoap_SONAME} ${libcoap_base}

.SUFFIXES: .c .so .o

%.so:%.c
	$(CC) $(CFLAGS) -o $@ -fPIC -c $<

targets=${libcoap_lib} coap_encode_test

all: $(targets)
clean:
	$(RM) $(targets) ${foreach i, $(targets), $($(i)_objs)}

libcoap_objs=coap_encode_test.o coap_encode.o coap_parse.o coap_misc.o fprintbuf.o

${libcoap_soname}: ${libcoap_objs}
	$(CC) $(LDFLAGS) $(LDLIBS) -o $@ $(coap_encode_test_objs) $(coap_encode_test_libs)

coap_encode_test_objs = coap_encode_test.o ${libcoap_lib}
coap_encode_test: ${coap_encode_test_objs}
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ ${coap_encode_test_objs} ${coap_encode_tst_libs}

coap_encode_test.o: coapP.h coap.h lists.h fprintbuf.h
coap_parse.o: coapP.h coap.h lists.h fprintbuf.h
coap_encode.o: coapP.h coap.h lists.h fprintbuf.h
coap_misc.o: coapP.h coap.h lists.h fprintbuf.h
