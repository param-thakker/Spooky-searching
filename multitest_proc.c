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
            printf("in child process, pid = %d\n", getpid());
            exit(0);
        } else {
            printf("child PID: %d\n", pid);
        }
    }
}