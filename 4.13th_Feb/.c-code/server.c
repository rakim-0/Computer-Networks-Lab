#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>

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

    char buff[1000];
    int child1 = fork();
    if (child1 == 0) {
        while (1) {
            fgets(buff, 1000, stdin);
            if (buff[0] == 'q') {
                break;
            }
            send(new_fd, &buff, 1000, 0);
            memset((void *) &buff, 0, sizeof(buff));
        }
        exit(0);
    }
    int flag = 1;
    int child2 = fork();
    if (child2 == 0) {
        while (flag) {
            if (!recv(new_fd, &buff, 1000, 0)) {
                printf("Connection Closed!!");
                flag = 0;
                break;
            }
            printf("%s", buff);
            memset((void *) &buff, 0, sizeof(buff));
        }
        if (flag == 0) {
            kill(child2, SIGQUIT);
            exit(0);
        }
    }
    pid_t wpid; int status = 0;
    while ((wpid = wait(&status)) > 0);
    close(sockfd); close(new_fd);
}