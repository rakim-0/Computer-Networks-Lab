#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int PORT = 5500;

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
    PORT = atoi(port);
    FILE *output = fopen(filename, "w");
    int sockfd = 0;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    memset((void *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    int err = inet_pton(AF_INET, IP, &(servaddr.sin_addr));
    if (err < 0) {
        perror("inet_pton");
        exit(1);
    }
    int servfd = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (servfd < 0) {
        perror("connect");
        exit(0);
    }
    char buff;
    while (1) {
        if (!recv(sockfd, &buff, sizeof(buff), 0)) {
            printf("\nFile Received Sucessfully!\nConnection Closed!!");
            break;
        }
        printf("%c", buff);
        fwrite(&buff, sizeof(buff), 1, output);
        if (buff == EOF) {
            break;
        }
        memset((void *)&buff, 0, sizeof(buff));
    }
    fclose(output);
    while (1)
        ;
    close(sockfd);
    close(servfd);
}