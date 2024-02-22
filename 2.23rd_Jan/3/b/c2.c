#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "structures.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define PORT1 5500
#define PORT2 5503

#define BACKLOG 5

int main() {
    int err = 0;
    int sockfd; 
    
    struct sockaddr_in myaddr;
    memset((void *) &myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(PORT2);
    myaddr.sin_addr.s_addr = INADDR_ANY;
    err = inet_pton(AF_INET, "172.17.0.1", &(myaddr.sin_addr));

    if (err <= 0) {
        perror("inet_pton");
        exit(0);
    }
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(0);
    }
    err = bind(sockfd, (struct sockaddr *) &myaddr, sizeof(myaddr));
    if (err < 0) {
        perror("bind");
        exit(0);
    }
    
    struct fit message;
    struct sockaddr_storage their_addr;
    socklen_t addr_len;

    recvfrom(sockfd, &message, sizeof(message), 0, 
        (struct sockaddr *) &their_addr, &addr_len);
    printf("Message Received: ");
    printf("%d %c %f\n", message.i, message.c, message.f);
    close(sockfd);
}