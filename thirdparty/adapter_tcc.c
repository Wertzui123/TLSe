#include "tlse.h"

#ifndef __TINYC__
#error("Cannot compile this file without TCC")
#endif
#if defined(__TINYC__) && !defined(TINYC_TLSE_COMPILE)
#error("TCC is currently not supported for TLSe")
#endif

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int send_pending(int client_sock, struct TLSContext *context)
{
    printf("TCC is currently not supported for TLSe.\n");
    exit(1);
}

int validate_certificate(struct TLSContext *context, struct TLSCertificate **certificate_chain, int len)
{
    printf("TCC is currently not supported for TLSe.\n");
    exit(1);
}

int connect_socket(const char *host, int port)
{
    printf("TCC is currently not supported for TLSe.\n");
    exit(1);
}

int connect_tls(int sockfd, struct TLSContext *context)
{
    printf("TCC is currently not supported for TLSe.\n");
    exit(1);
}

int read_tls(int sockfd, struct TLSContext *context, void *buffer, int len)
{
    printf("TCC is currently not supported for TLSe.\n");
    exit(1);
}

int write_tls(int sockfd, struct TLSContext *context, const char *data, int len)
{
    printf("TCC is currently not supported for TLSe.\n");
    exit(1);
}

int tls_established(struct TLSContext *context)
{
    printf("TCC is currently not supported for TLSe.\n");
    exit(1);
}

struct TLSContext *tls_create_context(unsigned char is_server, unsigned short version)
{
    printf("TCC is currently not supported for TLSe.\n");
    exit(1);
}

void tls_close_notify(struct TLSContext *context)
{
    printf("TCC is currently not supported for TLSe.\n");
    exit(1);
}