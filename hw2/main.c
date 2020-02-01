#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXARGV 80
#define MAXROLL 25

// command mode
// #define PRESET -1
#define BUILT 0
#define GEN 1
#define IOREDIR 2

// bg mode
#define BG 1

// stop chars
#define INPUT '<'
#define OUTPUT '>'
#define AMPERS '&'

// stop word
static const char QUIT[] = "quit";
static pid_t pid_arr[120];
static int counter = 0;

struct cmd
{
    int mode; // 0, 1, 2
    char* argv[MAXARGV]; // ["ls", "-l", "./../."]
    char* infile;
    char* outfile;
    int bg; // 0 or 1
    int pin; // 0, precedence
    int pout; // 0 
};


void wrapUp() { 
    int status;
    
    for (int i = 0; i < counter; i++) {
        kill(pid_arr[i], SIGINT);
        // printf("Killing processes\n");
    }

    for (int i = 0; i < counter; i++) {
        pid_t pw = wait(&status);
    }
}


void printCmd(struct cmd* command) {
    int i = 0;
    while (command -> argv[i] != NULL) {
        printf("### %d th term is %s \n", i, command -> argv[i++]);
    }
    printf("### input file is %s\n", command->infile);
    printf("### output file is %s\n", command->outfile);
    printf("### command mode is %d\n", command->mode);
    printf("### command bg is %d\n", command->bg);
    printf("### pin is %d\n", command->pin);
    printf("### pout is %d\n", command->pout);
}


int execCommand(struct cmd* command) {
    pid_t pid;
    int status;

    if (!command -> bg) {
        if ((pid=fork()) == 0) {
            if (command -> mode == 1) {
                if (execve(command->argv[0], command->argv, NULL)) {
                    exit(0);
                }
            } else if (command -> mode == 2) {
                if ((command -> pin + command -> pout)==1) {
                    if (command -> infile != NULL) {
                        // char input_str[255];
                        FILE* fp = freopen(command -> infile, "r", stdin);
                        // printf("$$input file\n"); 

                        if (fp == NULL) {
                            fprintf(stderr, "redirect input %s failed\n", command -> infile);
                        } 
                        
                        if (execve(command->argv[0], command->argv, NULL)) {
                            exit(0);
                        }
                        
                        
                    } else if (command -> outfile != NULL) {
                        FILE* fp = freopen(command -> outfile, "w+", stdout);

                        // printf("$$output file\n");

                        if (fp == NULL) {
                            fprintf(stderr, "redirect output %s failed\n", command -> outfile);
                        } 
                        if (execve(command->argv[0], command->argv, NULL)) {
                            exit(0);
                        }
                        
                    } 
                } else {
                    if (command -> pin > command -> pout) {
                        // > <
                        printf("input is after output");
                    } else {
                        // < >
                        char input_str[255];
                        FILE* fp = freopen(command -> infile, "r", stdin);
                        FILE* fpout = freopen(command -> outfile, "w+", stdout);
                        if (fp == NULL) {
                            fprintf(stderr, "redirect input %s failed\n", command -> infile);
                        } 
                        if (fpout == NULL) {
                            fprintf(stderr, "redirect output %s failed\n", command -> outfile);
                        }

                        if (execve(command->argv[0], command->argv, NULL)) {
                            exit(0);
                        }

                        fclose(fp);
                        fclose(fpout);
                    }
                }
            }
        

        } else {
            if (waitpid(pid, &status, 0) < 0) {
                // printf("PARENT: waitpid %d\n", pid);
            }
        }

    } else { 
        if ((pid=fork()==0)) {
            if (execve(command->argv[0], command->argv, NULL)) {
                exit(0);
            }
        } else {
            pid_arr[counter++] = pid;
        }
        // run process in background
        
    }

    return 0;
}

struct cmd* parseLine(char* commandStr) {
    struct cmd* command = malloc(sizeof(struct cmd));
    
    if (strlen(commandStr)) {        
        if (strlen(commandStr) > 3 && !strncmp(commandStr, QUIT, 4)) {
            command -> mode = BUILT;
        } else {
            // new line char
            commandStr[strlen(commandStr) - 1] = 0;
            // general command
            int i = 0;
            command -> mode = GEN;
            command -> argv[i] = strtok(commandStr, " ");
            command -> pin = 0;
            command -> pout = 0;
            
            while ((command -> argv)[i] != NULL) {
                if (command -> argv[i][0] == INPUT) {
                    // get the file for input
                    command -> mode = IOREDIR; 
                    command -> infile = strtok(NULL, " ");
                    command -> argv[++i] = command -> infile;
                    if (!command -> pout) {
                        command -> pin = 1;
                    } else {
                        command -> pin = 2;
                    }
                    command -> argv[--i] = 0;
                    
                } else if (command -> argv[i][0] == OUTPUT) {
                    // get the file for output
                    command -> mode = IOREDIR;
                    command -> outfile = strtok(NULL, " ");
                    command -> argv[++i] = command -> outfile;
                    if (!command -> pin) {
                        command -> pout = 1;
                    } else {
                        command -> pout = 2;
                    }
                    command -> argv[--i] = 0;

                } else if (command -> argv[i][0] == AMPERS) {
                    command -> bg = BG;
                    command -> argv[i][0] = 0;
                    continue;
                    // goes to child validate and stuff

                } else {
                    // do nothing yet
                }
                command -> argv[++i] = strtok(NULL, " "); 
            }
            // printCmd(command);

        }

    } else {
        // printf("empty command");
    }
    return command;
}


int main(){
    char commandStr[MAXARGV];
    printf("prompt > ");
    
    while(fgets(commandStr, MAXARGV, stdin)) {    
        struct cmd* commands = parseLine(commandStr);

        if (!(commands -> mode)) {
            wrapUp();
            free(commands);
            // clean zombie process
            exit(0);
        } else {
            execCommand(commands);
        }
        printf("prompt > ");
    }

	return 0;
}
