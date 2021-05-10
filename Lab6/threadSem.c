#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>

/*
Purpose: Use 2 threads to increment myglobal exactly 40 times in total.
Compile: using -pthread option
 */

int myglobal = 0;
sem_t mutex;


 void *thread_function(void *arg) 
 {
	int i,j;
	
	for (i = 0; i < 20; i++) 
	{
		sem_wait(&mutex);
		j = myglobal;
		j = j + 1;
		printf(".");
		fflush(stdout);
		sleep(0.2);
		myglobal = j;
		sem_post(&mutex);
	}
	
	return NULL;
}

int main(void) 
{
	sem_init(&mutex, 1, 1);
	pthread_t mythread;
	int i;
	

	if (pthread_create(&mythread, NULL, thread_function, NULL)) {
		printf("error creating thread.");
		abort();
	}

	for (i = 0; i < 20; i++) 
	{
		sem_wait(&mutex);
		myglobal = myglobal + 1;
		printf("o");
		fflush(stdout);
		sleep(0.21);
		sem_post(&mutex);
	}

	if (pthread_join(mythread, NULL)) {
		printf("error joining thread.");
		abort();
	}

	printf("\nmyglobal equals %d\n", myglobal);
	sem_destroy(&mutex);

	exit(0);
}
 

