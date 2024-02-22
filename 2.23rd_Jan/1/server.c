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
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(0);
    }
    err = bind(sockfd, (struct sockaddr *) &myaddr, sizeof(myaddr));
    if (err < 0) {
        perror("bind");
        exit(0);
    }
    err = listen(sockfd, 5);
    if (err < 0) {
        perror("listen");
        exit(0);
    }

    struct sockaddr_in cliaddr; socklen_t len;
    int new_fd = accept(sockfd, (struct sockaddr *) &cliaddr, &len);
    if (new_fd < 0) {
        perror("accept");
        exit(0);
    }
    printf("Connection Successful!!\n");
    char message[1000];
    while(1) {
        printf("Enter message: ");
        // scanf("%s\n", message);
        fgets(message, 1000, stdin);
        if (message[0] == 'q') {
            break;
        }
        send(new_fd, &message, 1000, 0);
        memset((void *) &message, 0, sizeof(message));
    }
    // recv(sockfd, &str, 10, 0);

    close(sockfd); close(new_fd);
}