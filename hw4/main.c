#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HEAPSIZE 127
#define MAXARGV 80

// save word to the memory heap
static char heap[HEAPSIZE];
// save the blocks that are either free or allocated
static char blocklist[HEAPSIZE] = {0};
// current block
static int current_index=0;
// current size of the blocklist, not exceeding 126
static int current_size=0;

int custom_malloc(int addr) {
    printf("In Malloc: %d\n", addr);
    // blocklist
    int allocate_size=addr+1;
    unsigned char new_header=allocate_size<<1|1;
    int i=0;
    for(; i < HEAPSIZE; i++){
        if (blocklist[i]) {
            int block_num = i;
            int header_size = blocklist[i]>>1;
            int status = blocklist[i]&1;

            if (!status && (unsigned int)(blocklist[i]>>1) > header_size+1) {
                int next_block_size = header_size-allocate_size;
                int next_block_index = block_num + allocate_size - 1;
                int current_block_size = allocate_size;

                
            }
        }
    }
    return 0;
}


int custom_free(int addr) {
    printf("In Free: %d\n", addr);
    return 0;
}


int print_blocklist() { 
    // printf("In blocklist\n");
    int i=0;
    for (;i<HEAPSIZE;i++) {
        if (blocklist[i]) {
            int block_num = i+1;
            int heap=(int)(unsigned char)blocklist[i]>>1;
            int status=(int)(unsigned char)blocklist[i]&1;
            printf("%d, %d, %s\n", block_num, heap, status?"allocated":"free");
        }
    }

    return 0; 
}


int writemem(int addr, char* content) {
    printf("in write mem: addr:%d content:%s\n", addr, content);
    // heap
    return 0;
}


int printmem(int addr, int num) {
    printf("in printmem: addr:%d num:%d\n", addr, num);
    return 0;
}


int main(){
    char command_str[MAXARGV];
    // initialize heap
    unsigned char size_char = 126 & 0xFF;
    heap[0] = size_char << 1;
    blocklist[0] = size_char << 1;
    current_size++;
    current_index++;
    printf(">");
    
    while(fgets(command_str, MAXARGV, stdin)) {
        if (strlen(command_str)) {
            if (strlen(command_str) > 3 && !strncmp(command_str, "quit", 4)) {
                return 0;
            } else if (!strncmp(command_str, "malloc", 6)){
                char* line = strtok(command_str, " ");
                line = strtok(NULL, " ");
                int addr = atoi(line);
                custom_malloc(addr);
            } else if (!strncmp(command_str, "free", 4)) {
                char* line = strtok(command_str, " ");
                line = strtok(NULL, " ");
                int addr = atoi(line);
                custom_free(addr);
            } else if (!strncmp(command_str, "blocklist", 9)) {
                print_blocklist();
            } else if (!strncmp(command_str, "writemem", 8)) {
                char* argv = strtok(command_str, " ");
                char* argv2;
                argv = strtok(NULL, " ");
                argv2 = strtok(NULL, " ");
                int addr = atoi(argv);
                // strip newline char
                if (argv2[strlen(argv2)-1]=='\n') {
                    argv2[strlen(argv2)-1]='\0';
                }
                writemem(addr, argv2);
            } else if (!strncmp(command_str, "printmem", 8)) {
                char* argv = strtok(command_str, " ");
                char* argv2;
                argv = strtok(NULL, " ");
                argv2 = strtok(NULL, " ");
                int addr = atoi(argv);
                int num = atoi(argv2);
                printmem(addr, num);
            }
        }
        printf(">");
        
    }

	return 0;
}
