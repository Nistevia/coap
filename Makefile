# Makefile -- makefile of library coap.
# Author: Luis Colorado <luis.colorado@ericsson.com>
# Date: Fri Apr 10 21:16:48 EEST 2015
# Disclaimer: (C) 2015 LUIS COLORADO.  All rights reserved.
targets=coap_parse.o coap_encode.o

all: $(targets)
clean:
	$(RM) $(targets)

coap_parse.o: coapP.h coap.h
coap_encode.o: coapP.h coap.h
