#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* DUNGEON_MAP = NULL;
int* NAME_MAP = NULL;


char* getDescription(char* cpy) {
    int strLen = strlen(cpy);
    // get start index and end index, change 
    // end index to \0
    int start = 0, end = 0;
    for (int i = 0; i < strLen; i++) {
        if (cpy[i] == '+') {
            start = i;
            continue;
        } 
        
        if (start != 0 && cpy[i] == '+') {
            end = i;
            break;
        }
    }
    cpy[end] = '\0';
    printf("%s", cpy);
    return cpy + start;
}

int north() {
    return 0;
}


int loadDungeon(char* fileName) {
    // read the content to map
    printf("reading file...\n");
    FILE* fp;
    char buf[255];
    fp = fopen(fileName,"r");

    while(fgets(buf, 255, (FILE*)fp)) {
        // printf("%s", buf);
        char cpy[strlen(buf)];
        strcpy(cpy, buf);

        char* lineStr = getDescription(cpy);
        // printf("%s", lineStr);
    }

    
    fclose(fp);
    printf("finished reading...\n");
}


int east() {
    // if failed to move to the room
    return 0;
}

int west() {
    return 0;
}

int south() {
    return 0;
}


void adventure() {
    char buf[50];
    char endWord[] = "quit";
    int ctr = 0;
    int i;
    
    printf("$ ");
    while (fgets(buf, sizeof buf, stdin)) {
        buf[strcspn(buf, "\n")] = '\0';
        if (!strcmp(buf, "quit")) break;

        if (strlen(buf) > 10 && !strncmp(buf, "loaddungeon", 11)) {
            // char cpy[strlen(buf)];
            // strcpy(cpy, buf);
            // catch exception
            loadDungeon(buf + 12);
        }

        printf("$ ");

    }
    
}


int main() {
    adventure();
    return 0;
}