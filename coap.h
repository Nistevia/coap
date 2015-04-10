/* coap.h -- definitions for CoAP protocol.
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Thu Apr  9 13:21:14 EEST 2015
 * Disclaimer: (C) LUIS COLORADO.  All rights reserved.
 */
#ifndef _COAP_H
#define _COAP_H

#include <stdint.h>
#include <string.h>

#define COAP_HDR_LEN        4

#define COAP_VERS_OFFSET    0
#define COAP_VERS_MASK      0xc0
#define COAP_VERS_SHFT      6
#define COAP_VERS_VALUE     0x1
#define COAP_VERS_OK(c)     (((c) & COAP_VERS_MASK) \
        ==(COAP_VERS_VALUE << COAP_VERS_SHFT))

#define COAP_TYPE_OFFSET    0
#define COAP_TYPE_MASK      0x30
#define COAP_TYPE_SHFT      4

#define COAP_TKL_OFFSET     0
#define COAP_TKL_MASK       0x0f
#define COAP_TKL_SHFT       0
#define COAP_TKL_NBR        9

#define COAP_CODE_OFFSET    1
#define COAP_MSGID_OFFSET   2
#define COAP_TOKEN_OFFSET   4

#define COAP_OK                 0
#define COAP_INVALID_LENGTH     -1
#define COAP_INVALID_VERSION    -2
#define COAP_

typedef enum coap_err_e {
    COAP_OK=0,
    COAP_INVALID_LENGTH,
    COAP_INVALID_VERSION,
    COAP_INVALID_TKL,
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
    uint8_t             c_typ;
    uint16_t            c_len;
    void               *c_val;
    struct coap_opt_s  *c_nxt;
} coap_opt;

typedef struct coap_msg_s {
    uint8_t            *c_pktdat; /* packet data (raw) */
    uint8_t             c_vers; /* version (in host format) */
    coap_typ            c_typ; /* packet type. */
    unsigned            c_tkl; /* token length */
    uint8_t            *c_tok; /* token content */
    unsigned            c_code; /* packet code */
    unsigned            c_msgid; /* message id */
} coap_msg;

coap_err
coap_msg_init(
        coap_msg *tgt)
{
    memcpy(tgt, 0, sizeof *tgt);
    return COAP_OK;
} /* coap_msg_init */


coap_parse(
        const uint8_t *buff,
        size_t bufsz,
        coap_msg *tgt);

uint8_t *
coap_code(
        const coap_msg *msg,
        uint8_t        *buff,
        size_t          bufsz);

#endif /* _COAP_H */
