#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define PORT 5500

int main() {
    int sockfd = 0;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    memset((void *) &servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    int err = inet_pton(AF_INET, "172.17.0.1", &(servaddr.sin_addr));
    if (err < 0) {
        perror("inet_pton");
        exit(1);
    }
    int servfd = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if (servfd < 0) {
        perror("connect");
        exit(0);
    }
    char buff[1000];
    int client_int = 4;
    while (1) {
        printf("Enter Address: ");
        scanf("%d", &client_int);
        send(sockfd, &client_int, sizeof(int), 0);

        if (!recv(sockfd, &buff, sizeof(buff), 0)) {
            printf("Connection Closed!!");
            break;
        }
        printf("Address: ");
        printf("%s\n", buff);

        memset((void *) &buff, 0, sizeof(buff));
    }
    close(sockfd); close(servfd);
}
