#include <stdio.h>
#include <string.h>

int GLOBAL_DUNGEON_MAP[10];

int north() {
    return 0;
}


int loadDungeon(char* fileName) {
    // read the content to map
    printf("reading file...");
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
            char cpy[strlen(buf)];
            strcpy(buf, cpy);
            loadDungeon("dfile.txt");
        }

        printf("$ ");

    }

    // do {
        
    //     fgets(str, 50, stdin);
    //     char cpy[strlen(str)];
    //     strcpy(str, cpy);
        // if (len > 10 && strncmp(str, "loaddungeon", 10 == 0)) {
        //     printf("%s", cpy+10);
        //     printf("\n");
        // } else if (strcmp(str, "north") == 0) {
        //     printf("north");
        // } 
        // str[0] = '\0';
    //     printf("%s", str);
    //     printf("%d", strcmp("quit", endWord));
    // } while (strcmp(cpy, endWord));
    
}


int main() {
    adventure();
    return 0;
}