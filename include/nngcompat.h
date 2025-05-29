#ifndef NNGCOMPAT_H
#define NNGCOMPAT_H

#include "lugh.h"
#include "security.h"

// NNG integration for LughOS
// This file provides compatibility between NNG and our bare metal environment

// NNG message operations - keep same name structure as original NNG
typedef struct nng_msg {
    void   *body;
    size_t  body_len;
    void   *header;
    size_t  header_len;
    uint32_t flags;
    uint32_t checksum;
} nng_msg_t;

// Socket type - keep same name structure as original NNG
typedef struct nng_socket {
    int id;
    int protocol;
    uint32_t flags;
} nng_socket_t;

// NNG compatibility functions
void nng_init(void);
void nng_shutdown(void);
int nng_msg_alloc(nng_msg_t **msgp, size_t size);
int nng_msg_free(nng_msg_t *msg);
int nng_msg_append(nng_msg_t *msg, const void *data, size_t size);
int nng_msg_len(const nng_msg_t *msg);
void *nng_msg_body(const nng_msg_t *msg);
int nng_socket_create(nng_socket_t *sock, int protocol);
int nng_socket_close(nng_socket_t *sock);
int nng_send(nng_socket_t *sock, nng_msg_t *msg, int flags);
int nng_recv(nng_socket_t *sock, nng_msg_t **msgp, int flags);

// Error codes
#define NNG_OK        0
#define NNG_ENOMEM    1
#define NNG_EINVAL    2
#define NNG_ECLOSED   3
#define NNG_ETIMEDOUT 4
#define NNG_ENOTSUP   5

// Protocol types - simplified for our implementation
#define NNG_PROTO_PAIR1    1
#define NNG_PROTO_PUB0     2
#define NNG_PROTO_SUB0     3
#define NNG_PROTO_REQ0     4
#define NNG_PROTO_REP0     5

// Add function to transform LughOS messages to NNG messages
int lugh_message_to_nng(const message_t* lugh_msg, nng_msg_t** nng_msg);

// Add function to transform NNG messages to LughOS messages
int nng_message_to_lugh(const nng_msg_t* nng_msg, message_t* lugh_msg);

// Message validation and security
int validate_message(const message_t *msg);

#endif // NNGCOMPAT_H
