#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int PORT = 5500;
int BACKLOG = 5;

int main(int argc, char *argv[])
{
    if (argc != 4) {
        printf("Incorrect number of arguments!!\n");
        printf("Usage:\n");
        printf("$%s [server_address] [port_number] [file_name.txt]", argv[0]);
        exit(0);
    }
    char IP[20];
    char port[5];
    char filename[1000];

    strcpy(IP, argv[1]);
    strcpy(port, argv[2]);
    strcpy(filename, argv[3]);

    printf("IP: %s\nPORT: %s\nFile: %s\n", IP, port, filename);

    char c;

    PORT = atoi(port);

    int err = 0;
    int sockfd;

    struct sockaddr_in myaddr;
    memset((void *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(PORT);
    myaddr.sin_addr.s_addr = INADDR_ANY;
    err = inet_pton(AF_INET, IP, &(myaddr.sin_addr));

    if (err <= 0) {
        perror("inet_pton");
        exit(0);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(0);
    }
    err = bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr));
    if (err < 0) {
        perror("bind");
        exit(0);
    }
    err = listen(sockfd, 5);
    if (err < 0) {
        perror("listen");
        exit(0);
    }

    struct sockaddr_in cliaddr;
    socklen_t len;
    while (1) {
        int new_fd = 0;
        new_fd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);
        printf("Connection Successful!!\n");
        int id = fork();
        if (id == 0) {
            FILE *file = fopen(filename, "r");
            while (fread(&c, sizeof(c), 1, file) != 0) {
                printf("%c", c);
                send(new_fd, &c, 1, 0);
            }
            c = EOF;
            send(new_fd, &c, 1, 0);
            close(new_fd);
            exit(0);
        }
    }
    pid_t wpid;
    int status = 0;
    while ((wpid = wait(&status)) > 0)
        ;
    close(sockfd);
}