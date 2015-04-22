/* coap.h -- definitions for CoAP protocol.
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Thu Apr  9 13:21:14 EEST 2015
 * Disclaimer: (C) LUIS COLORADO.  All rights reserved.
 */
#ifndef _COAP_H
#define _COAP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
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
#define COAP_CODE(M, m)          ((((M) & 0x7) << 5) | ((m) & 0x1f))
#define COAP_CODE_MAJOR(c)       (((c) >> 5) & 0x07)
#define COAP_CODE_MINOR(c)       ((c) & 0x1f)

#define COAP_METHD               (0)
#define COAP_RSPOK               (2)
#define COAP_CLERR               (4)
#define COAP_SVERR               (5)

#define COAP_METHD_EMPTY                 (COAP_CODE(COAP_METHD, 0))
#define COAP_METHD_GET                   (COAP_CODE(COAP_METHD, 1))
#define COAP_METHD_POST                  (COAP_CODE(COAP_METHD, 2))
#define COAP_METHD_PUT                   (COAP_CODE(COAP_METHD, 3))
#define COAP_METHD_DELETE                (COAP_CODE(COAP_METHD, 4))

#define COAP_RSPOK_CREATED               (COAP_CODE(COAP_RSPOK, 1))
#define COAP_RSPOK_DELETED               (COAP_CODE(COAP_RSPOK, 2))
#define COAP_RSPOK_VALID                 (COAP_CODE(COAP_RSPOK, 3))
#define COAP_RSPOK_CHANGED               (COAP_CODE(COAP_RSPOK, 4))
#define COAP_RSPOK_CONTENT               (COAP_CODE(COAP_RSPOK, 5))

#define COAP_CLERR_BAD_REQUEST           (COAP_CODE(COAP_CLERR, 0))
#define COAP_CLERR_UAUTHORIZED           (COAP_CODE(COAP_CLERR, 1))
#define COAP_CLERR_BAD_OPTION            (COAP_CODE(COAP_CLERR, 2))
#define COAP_CLERR_FORBIDDEN             (COAP_CODE(COAP_CLERR, 3))
#define COAP_CLERR_NOT_FOUND             (COAP_CODE(COAP_CLERR, 4))
#define COAP_CLERR_METHOD_NOT_ALLOWED    (COAP_CODE(COAP_CLERR, 5))
#define COAP_CLERR_NOT_ACCEPTABLE        (COAP_CODE(COAP_CLERR, 6))
#define COAP_CLERR_PRECONDITION_FAILED   (COAP_CODE(COAP_CLERR,12))
#define COAP_CLERR_REQ_ENTITY_TOO_LARGE  (COAP_CODE(COAP_CLERR,13))
#define COAP_CLERR_UNSUPP_CONTENT_FORMAT (COAP_CODE(COAP_CLERR,15))

#define COAP_SVERR_INTERNAL_SERVER_ERROR (COAP_CODE(5, 0))
#define COAP_SVERR_NOT_IMPLEMENTED       (COAP_CODE(5, 1))
#define COAP_SVERR_BAD_GATEWAY           (COAP_CODE(5, 2))
#define COAP_SVERR_SERVICE_UAVAIL        (COAP_CODE(5, 3))
#define COAP_SVERR_GW_TIMEOUT            (COAP_CODE(5, 4))
#define COAP_SVERR_PROXY_NOT_SUPP        (COAP_CODE(5, 5))

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
    COAP_OK=0,              /* function return ok */
    COAP_INVALID_LENGTH,    /* invalid length on parsing. */
    COAP_INVALID_VERSION,   /* invalid version on parsing */
    COAP_INVALID_TKL,       /* invalid token length */
    COAP_INVALID_FORMAT,    /* invalid format */
    COAP_INVALID_OptDLT,    /* invalid OptionDelta in parsing */ COAP_INVALID_OptLEN,    /* invalid OptionLength in parsing */
    COAP_INVALID_PARAMETER, /* invalid PARAMETER on packet construction */
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
    LNODE_T             o_nod;
} coap_opt;

typedef struct coap_msg_s {
    const uint8_t      *c_pktdat;   /* packet data (raw) */
    size_t              c_pktlen;   /* packet buffer length */

    coap_typ            c_typ;      /* packet type. */

    const uint8_t      *c_tokdat;   /* token content */
    size_t              c_toklen;   /* token length */

    unsigned            c_code;     /* packet code */
    unsigned            c_msgid;    /* message id */

    const uint8_t      *c_plddat;   /* payload */
    size_t              c_pldlen;   /* payload size */

    LNODE_T             c_optslst;  /* options list */
    size_t              c_optssz;  /* number of options */

} coap_msg;

char *
coap_typ2str(
        coap_typ        typ);

coap_err
coap_msg_init(
        coap_msg       *tgt);

coap_err
coap_msg_settype(
        coap_msg       *tgt,
        coap_typ        typ);

coap_err
coap_msg_setcode(
        coap_msg       *tgt,
        uint8_t        cod);

coap_err
coap_msg_setpayload(
        coap_msg       *tgt,
        const uint8_t  *payload,
        size_t          payloadlen);

coap_err
coap_msg_settoken(
        coap_msg       *tgt,
        const uint8_t  *tokdat,
        size_t          toklen);

coap_err
coap_msg_setmsgid(
        coap_msg       *tgt,
        uint16_t        msgid);

coap_err
coap_msg_addopt(
        coap_msg       *tgt,
        coap_opt       *opt);

coap_opt
*coap_msg_fstopt(
        coap_msg       *tgt);

coap_opt
*coap_msg_lstopt(
        coap_msg       *tgt);

coap_opt
*coap_msg_nxtopt(
        coap_opt       *opt);

coap_opt
*coap_msg_prvopt(
        coap_opt       *opt);

coap_err
coap_parse(
        const uint8_t  *buff,
        size_t          bufsz,
        coap_msg       *tgt);

coap_err
coap_encode_unsigned(
        uint32_t        val,
        uint8_t        *buff,
        size_t          buffsz);

coap_err
coap_encode(
        coap_msg       *msg,
        uint8_t        *buff,
        size_t          bufsz);

coap_err
coap_parse_unsigned(
        const uint8_t  *buff,
        size_t          buffsz,
        uint32_t       *valref);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* _COAP_H */
