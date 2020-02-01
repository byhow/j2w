#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main (int argc, char* argv[]) {
	int num = 0;
	if (argc >= 2) {
		num = atoi(argv[1]);
	}
	int i = 0;
	while(i <= num) {
		// printf("Printing at %d\n",i);
		++i;
        sleep(1);
	}
	return 0;
}