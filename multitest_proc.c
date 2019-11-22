#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>

// Indicates that the program is currently running in process mode
int mode() {
	return 1;
}

int _search(int *array, int size, int value, int chunkSize) {
	int numProcs = size / chunkSize;
	if(size % chunkSize != 0) {
		numProcs++;
	}
	
	pid_t processes[numProcs];

	int i;
	for(i = 0; i < size; i += chunkSize) {
		pid_t pid = fork();
		if(pid == 0) {
			int j;
			for(j = i; j < i + chunkSize; j++) {
				if(array[j] == value) {
					exit(j - i);
				}
			}

			exit(255);
		} else {
			if(pid == -1) {
				perror("Error creating process");
			}
			
			processes[i / chunkSize] = pid;
		}
	}

	// Wait for each process, calculating the correct index in the process
	int status;
	int index;
	for(i = 0; i < numProcs; i++) {
		waitpid(processes[i], &status, 0);
		status = WEXITSTATUS(status);

		if(status != 255) {
			index = i * chunkSize + status;
		}
	}
	
	return index;
}