#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include "multitest.h"

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

int main() {
    srand(time(0));

    int size = 100;
    int *array = genArray(size);

    int i;
    for(i = 0; i < size; i++) {
        printf("array[%d] = %d\n", i, array[i]);
    }

    return 0;
}