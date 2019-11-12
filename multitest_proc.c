#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int search(int *array, int size, int value, int chunkSize) {
    printf("search called for array of size %d, searching for %d, chunk size %d\n", size, value, chunkSize);

    int i;
    for(i = 0; i < size; i += chunkSize) {
        pid_t pid = fork();
        if(pid == 0) {
            printf("in child process, searching indices %d-%d\n", i, i + chunkSize - 1);
            
            int j;
            for(j = i; j < i + chunkSize; j++) {
				if(array[j] == value) {
					printf("found value, returning %d\n", j);
					exit(j);
				}
            }

			printf("value not found, returning -1\n");
            exit(-1);
        } else {
            printf("child PID: %d\n", pid);

			int status;
			wait(&status);
			printf("search returned %d\n", WEXITSTATUS(status));
        }
    }
}