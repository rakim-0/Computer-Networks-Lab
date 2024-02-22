#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define PORT 5500
#define BACKLOG 5

int main() {
    int err = 0;
    int sockfd; 
    
    struct sockaddr_in myaddr;
    memset((void *) &myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(PORT);
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
    
    char message[1000];
    struct sockaddr_storage their_addr;
    socklen_t addr_len;

    sendto(sockfd, "hi!!", 5, 0, 
        (struct sockaddr *) &myaddr, sizeof(myaddr));

    while(1) {
        printf("Enter message: ");
        fgets(message, 1000, stdin);
        sendto(sockfd, message, 1000, 0, 
        (struct sockaddr *) &myaddr, sizeof(myaddr));
        if (message[0] == 'q') {
            break;
        }
        memset((void *) &message, 0, sizeof(message));
    }
    close(sockfd); 
}