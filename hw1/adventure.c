#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_SIZE 120
#define MAX_FILE_SIZE 100

int *DUNGEON_MAP[1000];
const char *NAME_MAP[1200];
int loaded = 0;

char* parseDescription(char* cpy) {
    int strLen = strlen(cpy);
    // get start index and end index, change 
    // end index to \0
    int start = 0, end = 0;
    for (int i = 0; i < strLen; i++) {
        if (cpy[i] == '+') {
            if (!start) { 
                start = i;
                continue;
            } else {
                end = i;
                break;
            }
        }

    }
    cpy[end] = '\0';
    return cpy + start + 1;
}


void parseRoomMapByLine(char* buf, int current_room) {
    char cpy[strlen(buf)];
    strcpy(cpy, buf);
    int last = 0, i = 0, len = strlen(buf);
    for (; i < len; i++) {
        if (cpy[i] == '+') {
            last = i;
        }
    }
    char* map_arr = cpy + last + 1;
    int connect_rooms[120];
    char delim[] = " ";
    char *ptr = strtok(map_arr, delim);
    int square_map[4];
    int index = 0;
    while(ptr != NULL) {
        int room = atoi(ptr);
        ptr = strtok(NULL, delim);
        square_map[index++] = room;
    }

    DUNGEON_MAP[current_room] = square_map;
    printf("%d\n", DUNGEON_MAP[current_room][0]);
}

void loadDungeon(char* fileName) {
    // read the content to map
    printf("Reading file...\n");
    FILE* fp;
    char buf[120];
    fp = fopen(fileName,"r");
    if (fp == NULL) {
        printf("Fail to open the file %s \n", fileName);
        return;
    }
    
    while(fgets(buf, 255, (FILE*)fp)) {
        char cpy[strlen(buf)];
        strcpy(cpy, buf);
        char *ptr = strtok(cpy, "+");
        int room_num = atoi(ptr);
        parseRoomMapByLine(cpy, room_num);
        // cpy is changed after getting the description
        char* lineStr = parseDescription(cpy);
        NAME_MAP[room_num] = lineStr;
        printf("#");
    }
    
    fclose(fp);
    printf("\nFinished reading...\n");
}

int north() {
    printf("turning north\n");
    return 0;
}

int east() {
    // if failed to move to the room
    printf("turning east\n");
    return 0;
}

int west() {
    printf("turning west\n");

    return 0;
}

int south() {
    printf("turning south\n");

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
                loadDungeon(buf + 12); 
                loaded += 1;           
            } else {
                printf("Dungeon already loaded\n");
            }

        } else if (!strcmp(buf, "north")) {
            north();
        } else if (!strcmp(buf, "south")) {
            south();
        } else if (!strcmp(buf, "east")) {
            east();
        } else if (!strcmp(buf, "west")) {
            west();
        } else
        {
            printf("INVALID COMMAND!\n");
        }
        
        printf("$ ");

    }
    
}


int main() {
    adventure();
    return 0;
}