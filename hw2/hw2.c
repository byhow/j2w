#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXARGV 80
#define MAXROLL 25

#define PRESET -1
#define BUILT 0
#define GEN 1
#define IOREDIR 2
#define BG 3


static const char QUIT[] = "quit";

struct cmd
{
    int mode;
    char* argv[MAXARGV];
    char* infile;
    char* outfile;
    int bg;
};


struct cmd* parseLine(char* commandStr) {
    struct cmd* command = malloc(sizeof(struct cmd));
    
    if (strlen(commandStr)) {        
        if (strlen(commandStr) > 3 && !strncmp(commandStr, QUIT, 4)) {
            command -> mode = BUILT;
        } else {
            command -> mode = PRESET;
            printf("It is not quit command\n");
        }

    } else {
        printf("empty command");
    }
    return command;
}

int execCommand() {
    return 0;
}

int main(){
    char commandStr[MAXARGV];
    printf("prompt > ");
    while(fgets(commandStr, MAXARGV, stdin)) {    
        struct cmd* commands = parseLine(commandStr);
        if (!(commands -> mode)) {
            free(commands);
            exit(0);
        } else {
            printf("other commands\n");
        }
        printf("prompt > ");
    }

	return 0;
}
