#include <stdio.h>
#include<stdlib.h>
#include <pthread.h>

int flag = 0; 


struct input {
    int *array;
    int size;
	int value;
	int chunkSize;
	int threadNumber;
};



void* linearThreadSearch(void* args) { 


   int num = ((struct input*)args)->threadNumber;
   int blockSize=((struct input*)args)->chunkSize;
   int *array=((struct input*)args)->array;
   int value=((struct input*)args)->value;
   for (int i = num * blockSize; i < ((num + 1) * blockSize); i++){
      if (array[i] == value){
		 int *index=malloc(sizeof(int));
         *index=i;
		 pthread_exit(index);
	  }
   }
	int *exit=malloc(sizeof(int));
	*exit=-1;
	pthread_exit(exit);
}

int search(int *array, int size, int value, int chunkSize) {
struct input *parameters=(struct input *) malloc(sizeof(struct input));

   void *status;
   pthread_t thread[size/chunkSize];
   for (int i = 0; i < size; i+=chunkSize) { 
	parameters->array=array;
	parameters->size=size;
	parameters->value=value;
	parameters->chunkSize=chunkSize;
	parameters->threadNumber=i;
      pthread_create(&thread[i], NULL, linearThreadSearch, (void*)parameters);
   }
   for (int i = 0; i < size; i+=chunkSize) {
      pthread_join(thread[i], &status); 
	  if (*(int*) status !=-1){
	   return i * chunkSize + status;        
   }
   }
   
  
}



