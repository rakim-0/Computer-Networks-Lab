#include <stdlib.h>
#include <stdio.h>

struct Person {
    int id;
    char name[50];
};

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

    printf("ID\tName\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%s\n", people[i].id, people[i].name);
    }
    return 0;
}

int main() {
    readFile();
}