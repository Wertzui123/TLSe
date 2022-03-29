#include <sys/types.h>
#include <stdio.h>
#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <sys/socket.h>
#endif

#include "tlse.c"

void error(char *msg) {
    perror(msg);
    exit(0);
}

int send_pending(int client_sock, struct TLSContext *context) {
    unsigned int out_buffer_len = 0;
    const unsigned char *out_buffer = tls_get_write_buffer(context, &out_buffer_len);
    unsigned int out_buffer_index = 0;
    int send_res = 0;
    while ((out_buffer) && (out_buffer_len > 0)) {
        int res = send(client_sock, (char *)&out_buffer[out_buffer_index], out_buffer_len, 0);
        if (res <= 0) {
            send_res = res;
            break;
        }
        out_buffer_len -= res;
        out_buffer_index += res;
    }
    tls_buffer_clear(context);
    return send_res;
}

int validate_certificate(struct TLSContext *context, struct TLSCertificate **certificate_chain, int len) {
    int i;
    if (certificate_chain) {
        for (i = 0; i < len; i++) {
            struct TLSCertificate *certificate = certificate_chain[i];
            // check certificate ...
        }
    }
    //return certificate_expired;
    //return certificate_revoked;
    //return certificate_unknown;
    return no_error;
}


int connect_socket(const char *host, int port) {
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    #ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    #else
        signal(SIGPIPE, SIG_IGN);
    #endif
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    serv_addr.sin_port = htons(port);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    return sockfd;
}

int connect_tls(int sockfd, struct TLSContext* context){
    int res = tls_client_connect(context);
    send_pending(sockfd, context);
    unsigned char client_message[0xFFFF];
    for(;;){
        int read_size = recv(sockfd, client_message, sizeof(client_message) , 0);
        tls_consume_stream(context, client_message, read_size, validate_certificate);
        send_pending(sockfd, context);
        if(tls_established(context)){
            break;
        }
    }
    return res;
}

int read_tls(int sockfd, struct TLSContext* context, void* buffer, int len){
    unsigned char client_message[0xFFFF];
    int read_res;
    int read_size;
    int read = 0;
    for(;;){
        if(tls_established(context)){
            unsigned char read_buffer[len];
            read_res = tls_read(context, read_buffer, sizeof(read_buffer));
            if (read_res > 0){
                memcpy(buffer + read, read_buffer, read_res);
                read += read_res;
            }
        }
        if(read >= len){
            break;
        }
        read_size = recv(sockfd, client_message, sizeof(client_message), 0);
        if(read_size <= 0){
            break;
        }
        tls_consume_stream(context, client_message, read_size, validate_certificate);
        send_pending(sockfd, context);
    }
    return read;
}

int write_tls(int sockfd, struct TLSContext* context, const char* data, int len){
    int write_res = tls_write(context, data, len);
    if(write_res > 0){
        send_pending(sockfd, context);
    }
    return write_res;
}