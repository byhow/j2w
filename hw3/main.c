#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXARGV 80
#define BLOCKSIZE 4

static int lru = 0;
static const char QUIT[] = "quit";
static const char LRU_STR[] = "LRU";
static int tlb[4] = {0, 1, 2, 3};
static int fifoq[4] = {-1, -1, -1, -1};
static int used_num = 0;

static int main_memory[4][BLOCKSIZE] = {{-1, -1, -1, -1}, {-1, -1, -1, -1},
                                {-1, -1, -1, -1}, {-1, -1, -1, -1} };

static int disk_memory[8][BLOCKSIZE] = {{-1, -1, -1, -1}, {-1, -1, -1, -1},
                                {-1, -1, -1, -1}, {-1, -1, -1, -1},
                                {-1, -1, -1, -1}, {-1, -1, -1, -1},
                                {-1, -1, -1, -1}, {-1, -1, -1, -1} };

struct pageEntry {
    int valid;
    int dirty;
    int pageNum;
};

static struct pageEntry page_table[8] = { {0, 0, 0}, {0, 0, 1},
                                    {0, 0, 2}, {0, 0, 3}, {0, 0, 4},
                                    {0, 0, 5}, {0, 0, 6}, {0, 0, 7} };

int findFirstAvailable() {
    int page_entry_tmp[4] = {0, 0, 0, 0}; // number of pages in page table
    int i = 0;
    for (; i < 8; i++) {
        if (page_table[i].valid) {
            page_entry_tmp[page_table[i].pageNum] = 1;
        }
    }
    int ret_index = -1;
    int j = 0;
    for (; j < 4; j++) {
        if (!page_entry_tmp[j]) {
            return j;
        }
    }
    return ret_index;
}

int swapLRU() {
    int min_index = 0;
    int min = tlb[min_index];
    int i = 1;
    for (; i < 4; i++) {
        if (min > tlb[i]) {
            min = tlb[i];
            min_index = i;
        }
    }
    return min_index;
}

int swapFIFO() {
    // shift array
    int first = fifoq[0]; // 0
    int i = 0;
    for (; i < 3; i++) {
        fifoq[i] = fifoq[i+1]; // [1, 2, 3, 3]
    }
    fifoq[3] = first; // [1, 2, 3, 0]
    return first;
}

int findpTableEntry(int page_addr) {
    int i = 0;
    for (; i < 8; i++) {
        if (page_table[i].valid && page_table[i].pageNum == page_addr) {
            return i;
        }
    }
    return -1;
}

int execRead(int virtual_addr) {
    used_num++;
    
    // if valid
    if (page_table[virtual_addr>>2].valid) {
        // increment tlb or fifoq
        if (!lru) {
            int i = 0;
            for (; i < 4; i++) {
                if (fifoq[i] == -1) {
                    fifoq[i] = page_table[virtual_addr>>2].pageNum;
                    break;
                }
            } 
        } else {
            tlb[page_table[virtual_addr>>2].pageNum] = used_num;
        }
        printf("%d\n", main_memory[page_table[virtual_addr>>2].pageNum][virtual_addr%4]);
    } else {
        // page fault
        printf("A Page Fault Has Occurred\n");
        int free_block_addr = findFirstAvailable();
        if (free_block_addr < 0) {
            // need to swap out with FIFO or LRU
            int free_page;
            if (lru) {
                free_page = swapLRU();
            } else {
                free_page = swapFIFO();
            }
            int table_i = findpTableEntry(free_page);
            if (page_table[table_i].dirty) {
                disk_memory[table_i][0] = main_memory[free_page][0];
                disk_memory[table_i][1] = main_memory[free_page][1];
                disk_memory[table_i][2] = main_memory[free_page][2];
                disk_memory[table_i][3] = main_memory[free_page][3];
            } 

            page_table[virtual_addr>>2].valid = 1;
            page_table[virtual_addr>>2].pageNum = free_page;
            page_table[table_i].valid = 0;
            page_table[table_i].dirty = 0;
            page_table[table_i].pageNum = table_i;
            main_memory[free_page][0] = disk_memory[virtual_addr>>2][0];
            main_memory[free_page][1] = disk_memory[virtual_addr>>2][1];
            main_memory[free_page][2] = disk_memory[virtual_addr>>2][2];
            main_memory[free_page][3] = disk_memory[virtual_addr>>2][3];
            
            printf("%d\n", main_memory[free_page][virtual_addr%4]);
            if(lru) { tlb[free_page] = used_num; }

        } else {
            // use the checked block
            main_memory[free_block_addr][0] = disk_memory[virtual_addr>>2][0];
            main_memory[free_block_addr][1] = disk_memory[virtual_addr>>2][1];
            main_memory[free_block_addr][2] = disk_memory[virtual_addr>>2][2];
            main_memory[free_block_addr][3] = disk_memory[virtual_addr>>2][3];

            page_table[virtual_addr>>2].valid = 1;
            page_table[virtual_addr>>2].pageNum = free_block_addr;
            
            // increment tlb or fifoq
            if (!lru) {
                int i = 0;
                for (; i < 4; i++) {
                    if (fifoq[i] == -1) {
                        fifoq[i] = page_table[virtual_addr>>2].pageNum;
                        break;
                    }
                } 
            } else {
                tlb[free_block_addr] = used_num;
            }
            
            printf("%d\n", main_memory[free_block_addr][virtual_addr%4]);
        }
    }
}

int execWrite(int virtual_addr, int num) {
    used_num++;

    if (page_table[virtual_addr>>2].valid) {
        // increment tlb or fifoq
        if (!lru) {
            int i = 0;
            for (; i < 4; i++) {
                if (fifoq[i] == -1) {
                    fifoq[i] = page_table[virtual_addr>>2].pageNum;
                    break;
                }
            } 
        } else {
            tlb[page_table[virtual_addr>>2].pageNum] = used_num;
        }
        main_memory[page_table[virtual_addr>>2].pageNum][virtual_addr%4] = num;
        page_table[virtual_addr>>2].dirty = 1;

    } else {
        printf("A Page Fault Has Occurred\n");
        int free_block_addr = findFirstAvailable();

        if (free_block_addr < 0) {
            // need to swap out with FIFO or LRU
            int free_page;
            if (lru) {
                free_page = swapLRU();
            } else {
                free_page = swapFIFO();
            }
            
            int table_i = findpTableEntry(free_page);
            if (page_table[table_i].dirty) {
                disk_memory[table_i][0] = main_memory[free_page][0];
                disk_memory[table_i][1] = main_memory[free_page][1];
                disk_memory[table_i][2] = main_memory[free_page][2];
                disk_memory[table_i][3] = main_memory[free_page][3];
            } 
            // counter for lru should kick in
            tlb[free_page] = used_num;

            page_table[virtual_addr>>2].valid = 1;
            page_table[virtual_addr>>2].dirty = 1;
            page_table[virtual_addr>>2].pageNum = free_page;
            page_table[table_i].valid = 0;
            page_table[table_i].dirty = 0;
            page_table[table_i].pageNum = table_i;
            main_memory[free_page][0] = disk_memory[virtual_addr>>2][0];
            main_memory[free_page][1] = disk_memory[virtual_addr>>2][1];
            main_memory[free_page][2] = disk_memory[virtual_addr>>2][2];
            main_memory[free_page][3] = disk_memory[virtual_addr>>2][3];
            main_memory[free_page][virtual_addr%4] = num;
            
            
        } else {
            main_memory[free_block_addr][0] = disk_memory[virtual_addr>>2][0];
            main_memory[free_block_addr][1] = disk_memory[virtual_addr>>2][1];
            main_memory[free_block_addr][2] = disk_memory[virtual_addr>>2][2];
            main_memory[free_block_addr][3] = disk_memory[virtual_addr>>2][3];
            main_memory[free_block_addr][virtual_addr%4] = num;
            page_table[virtual_addr>>2].valid = 1;
            page_table[virtual_addr>>2].dirty = 1;
            page_table[virtual_addr>>2].pageNum = free_block_addr;
            // increment tlb or fifoq
            if (!lru) {
                int i = 0;
                for (; i < 4; i++) {
                    if (fifoq[i] == -1) {
                        fifoq[i] = page_table[virtual_addr>>2].pageNum;
                        break;
                    }
                } 
            } else {
                tlb[free_block_addr] = used_num;
            }

        }
    }

}

int showMain(int ppn) {
    if (ppn > 3) { printf("physical page number exceeds maximum\n"); };
    int i = 0;
    for (; i < 4; i++) {
        printf("%d:%d\n", ppn*BLOCKSIZE + i, main_memory[ppn][i]);
    }
}

int showDisk(int dpn) {
    if (dpn > 7) { printf("disk page number exceeds maximum\n"); };
    int i = 0;
    for (; i < 4; i++) {
        printf("%d:%d\n", dpn*BLOCKSIZE + i, disk_memory[dpn][i]);
    }
}

void showpTable() {
    int i = 0;
    for (; i < 8; i++) {
        printf("%d:%d:%d:%d\n", i, page_table[i].valid, page_table[i].dirty, page_table[i].pageNum);
    }
}

int main(int argc, char** argv){
    if (argc > 2) {
        printf("More than 2 arguments!\n");
        return -1;
    } else if (argc > 1 && strlen(argv[1]) > 2 && !strncmp(argv[1], LRU_STR, 3)) {
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

