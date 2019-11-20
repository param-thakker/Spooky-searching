#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

char *mode() {
	return "multiprocess";
}

int _search(int *array, int size, int value, int chunkSize) {
	int numProcs = size / chunkSize;
	if(size % chunkSize != 0) {
		numProcs++;
	}
	
    // printf("search called for array of size %d, searching for %d, chunk size %d\n", size, value, chunkSize);
    pid_t processes[numProcs];

    int i;
    for(i = 0; i < size; i += chunkSize) {
        pid_t pid = fork();
        if(pid == 0) {
            // printf("\tin child process (%d), searching indices %d-%d\n", getpid(), i, i + chunkSize - 1);

            int j;
            for(j = i; j < i + chunkSize; j++) {
				if(array[j] == value) {
					// printf("\tfound value, returning relative index %d\n", j - i);
					exit(j - i);
				}
            }

			// printf("\tvalue not found, returning 255\n");
            exit(255);
        } else {
            processes[i / chunkSize] = pid;
        }
    }

    int status;
    for(i = 0; i < numProcs; i++) {
        waitpid(processes[i], &status, 0);
        status = WEXITSTATUS(status);
        // printf("process #%d returned %d\n", processes[i], status);

        if(status != 255) {
            return i * chunkSize + status;
        }
    }
}