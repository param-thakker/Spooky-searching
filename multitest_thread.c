#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int mode() {
	return 2;
}

typedef struct _input {
	int *array;
	int size;
	int value;
	int chunkSize;
	int threadNumber;
} input;

void *linearThreadSearch(void *args) {
	int num = ((input *) args)->threadNumber;
	int blockSize = ((input *) args)->chunkSize;
	int *array = ((input *) args)->array;
	int size = ((input *) args)->size;
	int value = ((input *) args)->value;
	
	int i;
	for (i = num * blockSize; i < (num + 1) * blockSize && i < size; i++) {
		if (array[i] == value) {
			int *index = (int *) malloc(sizeof(int));
			*index = i;
			pthread_exit(index);
		}
	}

	int *exit = (int *) malloc(sizeof(int));
	*exit = -1;
	pthread_exit(exit);
}

int _search(int *array, int size, int value, int chunkSize) {
	int numThreads = size / chunkSize;
	if(size % chunkSize != 0) {
		numThreads++;
	}

	pthread_t thread[numThreads];
	
	int i;
	for (i = 0; i < numThreads; i++) {
		input *parameters = (input *) malloc(sizeof(input));
		
		parameters->array = array;
		parameters->size = size;
		parameters->value = value;
		parameters->chunkSize = chunkSize;
		parameters->threadNumber = i;
		pthread_create(&thread[i], NULL, linearThreadSearch, (void *) parameters);
	}

	void *status;
	int index;
	for (i = 0; i < numThreads; i++) {
		pthread_join(thread[i], &status);
		
		if (*((int *) status) != -1) {
			index = *((int *) status);
		}
	}
	
	return index;
}