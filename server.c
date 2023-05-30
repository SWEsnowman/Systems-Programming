#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>


int initserver(int type, const struct sockaddr *addr,socklen_t alen, int qlen){
    int fd;
    int err = 0;
    if ((fd = socket(addr->sa_family, type, 0)) < 0){
        return(-1);
    }
    //printf("socket created\n");     
    if (bind(fd, addr, alen) < 0){
        goto errout;
    }
    //printf("bind successful\n");
    if (type == SOCK_STREAM || type == SOCK_SEQPACKET) {
        if (listen(fd, qlen) < 0)
            goto errout;
        }
       // printf("listen successful\n");
    return(fd);

errout:
err = errno;
close(fd);
errno = err;
return(-1);
}

int main(int argc, char *argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int server_socket, client_socket, bytes_recvd;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    struct timeval tv;
    int port;
    float sum = 0;
    port = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if ((server_socket = initserver(SOCK_STREAM, (struct sockaddr *) &serv_addr, addrlen, 10)) == -1){
        return EXIT_FAILURE;
    }
    if ((client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &addrlen)) == -1){
        close(server_socket);
        fprintf(stderr, "Error: Failed to accept. %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }
    //printf("connection accepted\n");
    while(1){
        float num;
        bytes_recvd = recv(client_socket, &num, sizeof(num), 0);
        //printf("Data received from client\n");
        if(num == -1){
            break;
        }
        sum = sum + num;
        write(client_socket, &sum, sizeof(sum));
        //printf("Data sent to client\n");
    }
    close(server_socket);
    return 0;
}