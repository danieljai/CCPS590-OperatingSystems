#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

/*
Purpose: Use 2 threads to increment myglobal exactly 40 times in total.
Compile: using -pthread option
 */

int myglobal = 0;

 void *thread_function(void *arg) 
 {
	int i,j;
	
	for (i = 0; i < 20; i++) 
	{
		j = myglobal;
		j = j + 1;
		printf(".");
		fflush(stdout);
		sleep(0.2);
		myglobal = j;
	}
	
	return NULL;
}

int main(void) 
{
	pthread_t mythread;
	int i;

	if (pthread_create(&mythread, NULL, thread_function, NULL)) {
		printf("error creating thread.");
		abort();
	}

	for (i = 0; i < 20; i++) 
	{
		myglobal = myglobal + 1;
		printf("o");
		fflush(stdout);
		sleep(0.21);
	}

	if (pthread_join(mythread, NULL)) {
		printf("error joining thread.");
		abort();
	}

	printf("\nmyglobal equals %d\n", myglobal);

	exit(0);
}
 

