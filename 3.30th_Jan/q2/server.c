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
#define BACKLOG 5



struct Person people[10];

int readFile() {
    FILE *file;
    file = fopen("dir.txt", "r");  // Replace "data.txt" with the actual file name

    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    
    int count = 0;

    while (fscanf(file, "%d %s", &people[count].id, people[count].name) == 2) {
        count++;
        if (count == 10) {
            fprintf(stderr, "Array size exceeded. Increase MAX_LINES if needed.\n");
            break;
        }
    }

    fclose(file);

    // printf("ID\tName\n");
    // for (int i = 0; i < count; i++) {
    //     printf("%d %s\n", people[i].id, people[i].name);
    // }
    return 0;
}

void getName(int id, char message[1000]) {
    for (int i = 0; i < 10; i++) {
        if (people[i].id == id) {
            strcpy(message, people[i].name);
            return;
        }
    }
    strcpy(message, "Address not found");
    return;
}

void getID(char message[1000], int* id) {
    for (int i = 0; i < 10; i++) {
        if (strcmp(people[i].name, message) == 0) {
            *id = people[i].id;
            return;
        }
    }
    *id = -1;
    return;
}

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
    readFile();
    int client_int = 0;
    char message[1000];

    struct Req request;
    struct Reply reply;

    while(1) {
        reply.eof = 3;
        request.eof = 3;
        if (!recv(new_fd, &request, sizeof(request), 0)) {
            printf("Connection Closed!!");
            break;
        }
        if (request.type == 'I') {
            getName(request.id, reply.name);
            // printf("Address fetched: %s\n", reply.name);
            send(new_fd, &reply, sizeof(reply), 0);
        }
        else if (request.type == 'N') {
            getID(request.name, &reply.id);
            send(new_fd, &reply, sizeof(reply), 0);
        }

        memset((void *) &message, 0, sizeof(message));
        memset((void *) &request, 0, sizeof(request));
        memset((void *) &reply, 0, sizeof(reply));
    }
    close(sockfd); 
    close(new_fd);
}
