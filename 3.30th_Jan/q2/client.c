#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include "struct.h"

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

    struct Req request;
    struct Reply reply;

    char type[10]; // request type: ID or name
    char name[10]; // Memory for storing name
    while (1) {
        reply.eof = 3;
        request.eof = 3;
        printf("Do you want to search using name or ID? [I/N]: ");
        // fgets(type, 10, stdin);
        scanf("%s", type);
        if (type[0] == 'I') {
            request.type = 'I';
            printf("Enter ID: ");
            scanf("%d", &request.id);

            send(sockfd, &request, sizeof(struct Req), 0);

            if (!recv(sockfd, &reply, sizeof(reply), 0)) {
                printf("Connection Closed!!");
                break;
            }
            printf("Name: ");
            printf("%s\n", reply.name);
        }
        
        else if (type[0] == 'N') {
            request.type = 'N';
            printf("Enter Name: ");
            scanf("%s", &request.name);
            send(sockfd, &request, sizeof(struct Req), 0);

            if (!recv(sockfd, &reply, sizeof(reply), 0)) {
                printf("Connection Closed!!");
                break;
            }
            printf("ID: %d\n", reply.id);
        }

        memset((void *) &request, 0, sizeof(request));
    }
    close(sockfd); close(servfd);
}
