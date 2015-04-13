# Makefile -- makefile of library coap.
# Author: Luis Colorado <luis.colorado@ericsson.com>
# Date: Fri Apr 10 21:16:48 EEST 2015
# Disclaimer: (C) 2015 LUIS COLORADO.  All rights reserved.
targets=coap_encode_test

all: $(targets)
clean:
	$(RM) $(targets)

coap_encode_test_objs=coap_encode.o coap_encode_test.o coap_misc.o fprintbuf.o

coap_encode_test: $(coap_encode_test_objs)
	$(CC) $(LDFLAGS) $(LDLIBS) -o $@ $(coap_encode_test_objs) $(coap_encode_test_libs)

coap_parse.o: coapP.h coap.h
coap_encode.o: coapP.h coap.h
coap_misc.o: coapP.h coap.h
