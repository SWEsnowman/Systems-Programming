#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int client_socket = -1, bytes_recvd;
    struct sockaddr_in serv_addr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    struct timeval tv;
    int port = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
        fprintf(stderr, "Error: Failed to create socket. %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }
    //printf("client socket created\n");
    if (connect(client_socket, (struct sockaddr *) &serv_addr, addrlen) == -1) {
        close(client_socket);
        fprintf(stderr, "Error: Failed to connect to server. %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }
    //printf("connection accepted\n");
    float sum;
    while(1){
        float num;
        printf("Entering -1 will close the connection\n");
        printf("Enter a number: ");
        scanf("%f", &num);
        if (num == -1){
            break;
        }
        if (send(client_socket, &num, sizeof(num), 0) == -1){
            fprintf(stderr, "Error: Failed to send data to server. %s.\n", strerror(errno));
            return EXIT_FAILURE;
        }
        //printf("\nData sent to server\n");
        if ((bytes_recvd = recv(client_socket, &sum, sizeof(sum), 0)) == -1){
            fprintf(stderr, "Error: Failed to receive data from server. %s.\n", strerror(errno));
            return EXIT_FAILURE;
        }
        //printf("Data received from server");
        printf("Sum of data: %f\n", sum);
    }
    int closes = -1;
    send(client_socket, &closes, sizeof(closes), 0);
    close(client_socket);
    printf("Final sum of data: %f\n", sum);
    return 0;
}

