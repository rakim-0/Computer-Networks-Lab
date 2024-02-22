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

int flag = 1;
void handler(int sig, siginfo_t *info, void *context) {
    flag = 0;
}
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
    int flag = 1;

    int id = fork();
    if (id == 0) {
        while (1) {
            fgets(buff, 1000, stdin);
            if (buff[0] == 'q') {
                break;
            }
            send(sockfd, &buff, 1000, 0);
            memset((void *) &buff, 0, sizeof(buff));
        }
        exit(0);
    }
    
    int id2 = fork();
    if (id2 == 0) {
        while (flag) {
            if (!recv(sockfd, &buff, 1000, 0)) {
                printf("Connection Closed!!");
                flag = 0;
                break;
            }
            printf("%s", buff);
            memset((void *) &buff, 0, sizeof(buff));
        }
        if (flag == 0) {
            kill(id, SIGQUIT);
            exit(0);
        }
    }
    pid_t wpid; int status = 0;
    while ((wpid = wait(&status)) > 0);
    close(sockfd); close(servfd);
}