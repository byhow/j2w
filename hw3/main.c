#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXARGV 80
#define BLOCKSIZE 4

static int lru = 0;
static const char QUIT[] = "quit";
static const char LRU[] = "LRU";

static int main_memory[4][BLOCKSIZE] = {{-1, -1, -1, -1},
                                {-1, -1, -1, -1},
                                {-1, -1, -1, -1},
                                {-1, -1, -1, -1} };

static int disk_memory[8][BLOCKSIZE] = {{-1, -1, -1, -1},
                                {-1, -1, -1, -1},
                                {-1, -1, -1, -1},
                                {-1, -1, -1, -1},
                                {-1, -1, -1, -1},
                                {-1, -1, -1, -1},
                                {-1, -1, -1, -1},
                                {-1, -1, -1, -1} };

struct pageEntry {
    int valid;
    int dirty;
    int pageNum;
};

static struct pageEntry page_table[8] = { {0, 0, 0},
                                    {0, 0, 1},
                                    {0, 0, 2},
                                    {0, 0, 3},
                                    {0, 0, 4},
                                    {0, 0, 5},
                                    {0, 0, 6},
                                    {0, 0, 7} };

int findFirstAvailable() {
    int page_entry_tmp[4] = {0, 0, 0, 0}; // number of pages in page table
    for (int i = 0; i < 8; i++) {
        if (page_table[i][0]) {
            page_entry_tmp[page_table[i][2]] = 1;
        }
    }
    int ret_index = -1;
    for (int j = 0; j < 4; j++) {
        if (!page_entry_tmp[j]) {
            return j;
        }
    }
    return ret_index;
}

int swapLRU();
int swapFIFO();

int execRead(int virtual_addr) {
    printf("READ:Virtual Address is %d\n", virtual_addr);
}

int execWrite(int virtual_addr, int num) {
    printf("WRITE:Virtual Address is %d, and num is %d\n", virtual_addr, num);
}

int showMain(int ppn) {
    if (ppn > 3) { printf("physical page number exceeds maximum\n")};
    for (int i = 0; i < 4; i++) {
        printf("%d:%d\n", ppn*BLOCKSIZE + i, main_memory[ppn][i]);
    }
}

int showDisk(int dpn) {
    if (dpn > 7) { printf("disk page number exceeds maximum\n" };
    for (int i = 0; i < 8; i++) {
        printf("%d:%d\n", dpn*BLOCKSIZE + i, disk_memory[dpn][i]);
    }
}

void showpTable() {
    for (int i = 0; i < 8; i++) {
        printf("%d:%d:%d:%d\n", i, page_table[i].valid, page_table[i].dirty, page_table[i].pageNum);
    }
}

int main(int argc, char** argv){
    if (argc > 2) {
        printf("More than 2 arguments!\n");
        return -1;
    } else if (argc > 1 && strlen(argv[1]) > 2 && !strncmp(argv[1], LRU, 3)) {
        lru = 1;
    }

    char commandStr[MAXARGV];

    printf("> ");
    while(fgets(commandStr, MAXARGV, stdin)) {    
        if (strlen(commandStr)) {
            if (strlen(commandStr) > 3 && !strncmp(commandStr, QUIT, 4)) {
                return 0;
            } else if (!strncmp(commandStr, "read", 4)) {
                char* line = strtok(commandStr, " ");
                line = strtok(NULL, " ");
                int virtual_addr = atoi(line);
                execRead(virtual_addr);
            } else if (!strncmp(commandStr, "write", 5)) {
                char* argv = strtok(commandStr, " ");
                char* argv2;
                argv = strtok(NULL, " ");
                argv2 = strtok(NULL, " ");
                int virtual_addr = atoi(argv);
                int num = atoi(argv2);
                execWrite(virtual_addr, num);
            } else if (!strncmp(commandStr, "showmain", 8)) {
                char* line = strtok(commandStr, " ");
                line = strtok(NULL, " ");
                int ppn = atoi(line);
                showMain(ppn);
            } else if (!strncmp(commandStr, "showdisk", 8)) {
                char* line = strtok(commandStr, " ");
                line = strtok(NULL, " ");
                int dpn = atoi(line);
                showDisk(dpn);
            } else if (!strncmp(commandStr, "showptable", 10)) {
                showpTable();
            } else {
                printf("INVALID COMMAND!\n");
            }
        } 
        printf("> ");
    }

	return 0;
}

