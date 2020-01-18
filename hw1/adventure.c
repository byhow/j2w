#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_SIZE 120
#define MAX_FILE_SIZE 100

char DUNGEON_MAP[4][1000];
// N, S, E, W
char NAME_MAP[120][1200];
int loaded = 0;
int current = -255;

void loadDungeon(char* fileName) {
    FILE* fp;
    char buf[120];
    fp = fopen(fileName,"r");
    if (fp == NULL) {
        printf("Fail to open the file %s \n", fileName);
        return;
    }
    int init_room = -1;
    while(fgets(buf, 255, (FILE*)fp)) {
        if (buf[0] == '\n') { continue; }
        char* cur = strtok(buf, " ");
        int room_num = atoi(cur);
        if (init_room == -1) {
            init_room = room_num;
        }
        if (current == -255) { current = room_num; }
        cur = strtok(NULL, "+");
        strcpy(NAME_MAP[room_num], cur);
        strcpy(DUNGEON_MAP[room_num], strtok(NULL, "\n"));
    }
    printf("%s\n", NAME_MAP[init_room]);
    fclose(fp);
}

int north() {
    char cpy[strlen(DUNGEON_MAP[current])];
    strcpy(cpy, DUNGEON_MAP[current]);
    char* north_str = strtok(cpy, " ");
    int north = atoi(north_str);
    int south = atoi(strtok(NULL, " "));
    int east = atoi(strtok(NULL, " "));
    int west = atoi(strtok(NULL, " "));
    if (north != -1) {
        current = north;
        printf("%s\n", NAME_MAP[current]);
    } else {
        printf("Not accessible\n");
    }
    return 0;
}

int east() {
    // if failed to move to the room
    char cpy[strlen(DUNGEON_MAP[current])];
    strcpy(cpy, DUNGEON_MAP[current]);
    char* north_str = strtok(cpy, " ");
    int north = atoi(north_str);
    int south = atoi(strtok(NULL, " "));
    int east = atoi(strtok(NULL, " "));
    int west = atoi(strtok(NULL, " "));
    if (east != -1) {
        current = east;
        printf("%s\n", NAME_MAP[current]);
    } else {
        printf("Not accessible\n");
    }
    return 0;
}

int west() {
    char cpy[strlen(DUNGEON_MAP[current])];
    strcpy(cpy, DUNGEON_MAP[current]);
    char* north_str = strtok(cpy, " ");
    int north = atoi(north_str);
    int south = atoi(strtok(NULL, " "));
    int east = atoi(strtok(NULL, " "));
    int west = atoi(strtok(NULL, " "));
    if (west != -1) {
        current = west;
        printf("%s\n", NAME_MAP[current]);
    } else {
        printf("Not accessible\n");
    }
    return 0;
}

int south() {
    char cpy[strlen(DUNGEON_MAP[current])];
    strcpy(cpy, DUNGEON_MAP[current]);
    char* north_str = strtok(cpy, " ");
    int north = atoi(north_str);
    int south = atoi(strtok(NULL, " "));
    int east = atoi(strtok(NULL, " "));
    int west = atoi(strtok(NULL, " "));
    if (south != -1) {
        current = south;
        printf("%s\n", NAME_MAP[current]);
    } else {
        printf("Not accessible\n");
    }
    return 0;
}


void adventure() {
    char buf[120];
    char endWord[] = "quit";
    int ctr = 0;
    int i;
    
    printf("$ ");
    while (fgets(buf, sizeof buf, stdin)) {
        buf[strcspn(buf, "\n")] = '\0';
        if (!strcmp(buf, "quit")) {
            break;
        } else if (strlen(buf) > 10 && !strncmp(buf, "loaddungeon", 11)) {
            // char cpy[strlen(buf)];
            // strcpy(cpy, buf);
            if (!loaded) {
                // the file itself can be empty
                loadDungeon(buf + 12); 
                loaded = 1;           
            } else {
                printf("Dungeon already loaded\n");
            }

        } else if (loaded) {
            if (!strcmp(buf, "north")) {
                north();
            } else if (!strcmp(buf, "south")) {
                south();
            } else if (!strcmp(buf, "east")) {
                east();
            } else if (!strcmp(buf, "west")) {
                west();
            } else {
                if (strlen(buf)) {
                    printf("INVALID COMMAND!\n");
                }
            }
        } else {
            printf("INVALID COMMAND!\n");
        }
        
        printf("$ ");

    }
    
}


int main() {
    adventure();
    return 0;
}