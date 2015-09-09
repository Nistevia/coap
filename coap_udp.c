/* $Id$
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: mié sep  9 18:15:21 EEST 2015
 * Copyright: (C) 2015 LUIS COLORADO.  All rights reserved.
 */

#include <avl.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "coap_debug.h"

#define COAP_BUFSIZE        1536 /* 1.5Kb */
#define COAP_SAP_CONT      (1 << 0)

struct coap_udp_sap {
    int                 cus_flags;
    int                 cus_sock;
    int                 cus_sock_flags;
    struct sockaddr_in  cus_remote;
    socklen_t           cus_remsz;
    unsigned char       cus_buffer[COAP_BUFSIZE];
    size_t              cus_bufsz;
    AVL_TREE            cus_metreg;
    AVL_TREE            cus_respreg;
};

void *coap_udp_input_thread(void *p)
{
    struct coap_udp_sap *sap = p;

    while (sap->cus_flags & COAP_SAP_CONT) {
        int res;
        sap->cus_remsz = sizeof sap->cus_remote;
        res = recvfrom(
                sap->cus_sock,
                sap->cus_buffer, sizeof sap->cus_buffer,
                sap->cus_sock_flags,
                (struct sockaddr *)&sap->cus_remote, &sap->cus_remsz);
        if (res < 0) {
            ERR(D("recvfrom: %s (errno = %d)\n"),
                    strerror(errno), errno);
            break;
        } /* if */
        sap->cus_remsz = res;
        LOG_PACKET(
                sap->cus_buffer, sap->cus_bufsz,
                sap->cus_remote, sap->cus_remsz);
    } /* while */
    return sap;
} /* coap_udp_input_thread */
