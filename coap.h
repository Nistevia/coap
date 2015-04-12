/* coap.h -- definitions for CoAP protocol.
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Thu Apr  9 13:21:14 EEST 2015
 * Disclaimer: (C) LUIS COLORADO.  All rights reserved.
 */
#ifndef _COAP_H
#define _COAP_H

#include <stdlib.h>
#include <stdint.h>
#include "lists.h"

#define COAP_HDR_LEN             4

#define COAP_VERS_OFFS           0
#define COAP_VERS_MASK           0xc0
#define COAP_VERS_SHFT           6
#define COAP_VERS_VALUE          0x1
#define COAP_VERS_OK(c)     (((c) & COAP_VERS_MASK) \
        ==(COAP_VERS_VALUE << COAP_VERS_SHFT))

#define COAP_TYPE_OFFS           0
#define COAP_TYPE_MASK           0x30
#define COAP_TYPE_SHFT           4

#define COAP_TKL_OFFS            0
#define COAP_TKL_MASK            0x0f
#define COAP_TKL_SHFT            0
#define COAP_TKL_MAX             8

#define COAP_CODE_OFFS           1
#define COAP_CODE_MASK           0xff
#define COAP_CODE_SHFT           0
#define COAP_CODE_SZ             1

#define COAP_MSGID_OFFS          (COAP_CODE_OFFS + COAP_CODE_SZ)
#define COAP_MSGID_SZ            2

#define COAP_TOKEN_OFFS          (COAP_MSGID_OFFS + COAP_MSGID_SZ)
#define COAP_END_OF_OPTIONS      0xff

#define COAP_OptDLT_OFFS            0
#define COAP_OptDLT_MASK            0xf0
#define COAP_OptDLT_SHFT            4
#define COAP_OptLEN_OFFS            0
#define COAP_OptLEN_MASK            0x0f
#define COAP_OptLEN_SHFT            0

typedef enum coap_err_e {
    COAP_OK=0,
    COAP_INVALID_LENGTH,
    COAP_INVALID_VERSION,
    COAP_INVALID_TKL,
    COAP_INVALID_FORMAT,
    COAP_INVALID_OptDLT,
    COAP_INVALID_OptLEN,
    /* insert error codes before this line */
    COAP_ERR_NBR /* must be the last one */
} coap_err;

typedef enum coap_typ_e {
    /* we state explicitly the values to make them match the RFC
     * values as stated in RFC-7252. */
    coap_typ_CON=0,
    coap_typ_NON=1,
    coap_typ_ACK=2,
    coap_typ_RST=3,
} coap_typ;

typedef struct coap_opt_s {
    uint16_t            o_typ;
    uint16_t            o_len;
    const uint8_t      *o_val;
    struct coap_opt_s  *o_nxt;
    LNODE_T             o_nod;
} coap_opt;

typedef struct coap_msg_s {
    const uint8_t      *c_pktdat; /* packet data (raw) */
    uint8_t             c_vers; /* version (in host format) */
    coap_typ            c_typ; /* packet type. */
    unsigned            c_tkl; /* token length */
    const uint8_t      *c_tok; /* token content */
    unsigned            c_code; /* packet code */
    unsigned            c_msgid; /* message id */
    const uint8_t      *c_pld; /* payload */
    size_t              c_pldsz; /* payload size */
    LNODE_T             c_opts; /* options list */
} coap_msg;


coap_err
coap_msg_init(
        coap_msg *tgt);

coap_err
coap_parse(
        const uint8_t  *buff,
        size_t          bufsz,
        coap_msg       *tgt);

coap_err
coap_encode(
        const coap_msg *msg,
        uint8_t        *buff,
        size_t         *bsz_io);

#endif /* _COAP_H */
