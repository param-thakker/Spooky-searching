#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "multitest.h"

int *genArray(int size) {
	int *array = malloc(sizeof(int) * size);

	int i;
	for(i = 0; i < size; i++) {
		array[i] = i + 1;
	}

	for(i = size - 1; i > 0; i--) {
		int j = rand() % i;
		int temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}

	return array;
}

int simpleSearch(int *array, int size, int value) {
	int i;
	for(i = 0; i < size; i++) {
		if(array[i] == value) {
			return i;
		}
	}
}

void calculateStats(double *times, int numTests) {
	double min = times[0];
	double max = times[0];
	double avg;
	double std;
	
	int i;
	for(i = 1; i < numTests; i++) {
		if(times[i] < min) {
			min = times[i];
		} else if(times[i] > max) {
			max = times[i];
		}
	}
	
	double sum = 0;
	for(i = 0; i < numTests; i++) {
		sum += times[i];
	}
	
	avg = sum / numTests;
	
	sum = 0;
	for(i = 0; i < numTests; i++) {
		times[i] = (times[i] - avg) * (times[i] - avg);
		sum += times[i];
	}
	
	std = sqrt(sum / numTests);
	
	printf("--STATS--\n");
	printf("min: %f ms\nmax: %f ms\navg: %f ms\nstandard deviation: %f ms\n", min, max, avg, std);
	printf("\n");
}

void testA() {
	int minSize = 250;
	int maxSize = 10000;
	int step = 250;
	int numTests = (maxSize - minSize) / step + 1;
	int chunkSize = 250;

	double times[numTests];
	
	struct timespec start, end;

	printf("%s test A\n", (mode() == PROCESS ? "Multiprocess" : "Multithreaded"));

	int size;
	for(size = minSize; size <= maxSize; size += step) {
		//printf("Search #%d (%d elements)\n", size / minSize, size);
	
		int *array = genArray(size);
		
		clock_gettime(CLOCK_REALTIME, &start);
		int index = search(array, size, 49, chunkSize);
		clock_gettime(CLOCK_REALTIME, &end);
		
		int simpleIndex = simpleSearch(array, size, 49);
		if(index != simpleIndex) {
			printf("ERROR: incorrect index (expected %d but found %d)\n", simpleIndex, index);
		}
		
		double time = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;
		times[size / step - 1] = time;
	}
	
	calculateStats(times, numTests);
}

void testB(){
	int numTests;
	int chunkSize = 250;
	int size = 250;
   	int *array = genArray(size);
	
	struct timespec start, end;
	
 	printf("%s test B\n", (mode() == PROCESS ? "Multiprocess" : "Multithreaded"));
	
	if (mode() == PROCESS) {
		numTests = 100;
   		double times[numTests];
	
		int i;
		for (i = 0; i < numTests; i++) {
			printf("Search #%d\n", i);
			
			clock_gettime(CLOCK_REALTIME, &start);
			int index = search(array, size, 49, chunkSize);
			clock_gettime(CLOCK_REALTIME, &end);
			
			int simpleIndex = simpleSearch(array, size, 49);
			if(index != simpleIndex) {
				printf("ERROR: incorrect index (expected %d but found %d)\n", simpleIndex, index);
			}
			
			int randIndex = rand() % size;
			int temp = array[index];
			array[index] = array[randIndex];
			array[randIndex] = temp;
				
			double time = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;
			times[i] = time;
		}
		
		calculateStats(times, numTests);
	} else if (mode() == THREAD) {
		int minSize = 250;
		int maxSize = 5000;
		int step = 250;
		numTests = (maxSize - minSize) / step + 1;
		double times[numTests];
		
		int size;
		for(size = minSize; size <= maxSize; size += step) {
			//printf("Search #%d (%d elements)\n", size / minSize, size);
			
			int *array = genArray(size);
			
			double sum = 0;
			int i;
			for(i = 0; i < 100; i++) {				
				clock_gettime(CLOCK_REALTIME, &start);
				int index = search(array, size, 49, chunkSize);
				clock_gettime(CLOCK_REALTIME, &end);
				
				int simpleIndex = simpleSearch(array, size, 49);
				if(index != simpleIndex) {
					printf("ERROR: incorrect index (expected %d but found %d)\n", simpleIndex, index);
				}

				double time = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;
				sum += time;
				
				int randIndex = rand() % size;
				int temp = array[index];
				array[index] = array[randIndex];
				array[randIndex] = temp;
			}
			
			double avgTime = sum / 100;
			times[size / step - 1] = avgTime;
			
			printf("%d,%f\n", size, avgTime);
		}
		
		calculateStats(times, numTests);
	}
}

void testC() {
	int size = 250;
	int minChunkSize = 1;
	int maxChunkSize = 250;
	int numTests = (maxChunkSize - minChunkSize) + 1;
	int *array = genArray(size);

	double times[numTests];
	
	struct timespec start, end;

	printf("%s test C\n", (mode() == PROCESS ? "Multiprocess" : "Multithreaded"));

	int chunkSize;
	for(chunkSize = minChunkSize; chunkSize <= maxChunkSize; chunkSize++) {
		//printf("Search #%d (%d %s)\n", chunkSize - minChunkSize + 1, (int) ceil((double) size / (double) chunkSize), mode() == PROCESS ? "processes" : "threads");
		
		double sum = 0;
		int i;
		for(i = 0; i < 100; i++) {
			clock_gettime(CLOCK_REALTIME, &start);
			int index = search(array, size, 49, chunkSize);
			clock_gettime(CLOCK_REALTIME, &end);
			
			int simpleIndex = simpleSearch(array, size, 49);
			if(index != simpleIndex) {
				printf("ERROR: incorrect index (expected %d but found %d)\n", simpleIndex, index);
			}

			double time = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;
			sum += time;
				
			int randIndex = rand() % size;
			int temp = array[index];
			array[index] = array[randIndex];
			array[randIndex] = temp;
		}
		
		double avgTime = sum / 100;
		times[chunkSize - minChunkSize] = avgTime;
		
		printf("%d,%f\n", chunkSize, avgTime);
	}
	
	calculateStats(times, numTests);
}

int main() {
	srand(time(0));

//	testA();
//	testB();
	testC();

	return 0;
}