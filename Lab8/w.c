#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

/* don't change 0666 to 666. The leading 0 of an integer indicates that the
 * integer is an octal number. Thus, 0666 is 110110110 in binary. If the
 * leading zero is removed, the integer becomes six hundred sixty six with
 * a binary representation 1111011010. 
*/

#define SHMKEY 75
int shmid;
int flag=0;

int arr[10];

void cleanup(int);

void handle(int signum) {
	flag=0;
}


int main (void) 
{
	int *pint, *ppint, i;
	struct sigaction sVal;



	for (i = 0; i < 20; i++)  
		signal(i, cleanup);  


	shmid = shmget(SHMKEY, sizeof(int), 0666|IPC_CREAT);
	pint = (int*) shmat(shmid, 0, 0);
	ppint = pint;

	for(i = 0; i < 10; i++)  
		pint[i] = i;
	pint[0] = -1;
	flag=1;

	while(flag==1) {
		signal(SIGUSR1,handle);
	}
	
	kill(pint[10],10);

	for(i = 1; i < 10; i++)  
		pint[i] = i+10;
	pint[0] = -1;
	flag=1;

	while(flag==1) {
		signal(SIGUSR1,handle);
	}

	cleanup(shmid);

	return 0;
}

void cleanup(int x) 
{ 
	shmctl(shmid, IPC_RMID, 0);  /* detach sh mem */
	exit(0);
}
