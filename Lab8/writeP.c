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

void cleanup(int);

int main (void) 
{
	int *pint, *ppint, i;

	/* force any signal from 1-20 sent to this process to call handler 
	cleanup, which will detach shared mem before exiting */
	for (i = 0; i < 20; i++)  
		signal(i, cleanup);  

	shmid = shmget(SHMKEY, 128, 0666|IPC_CREAT);
	pint = (int*) shmat(shmid, 0, 0);
	ppint = pint;

	for(i = 0; i < 10; i++)  
		*ppint++ = i; /* load sh. mem with ints 0-10 */
	*pint = -1;  /* load -1 into start of shmem */

	pause(); /* wait here until a signal occurs--eg, kill from reader */
	
	return 0;
}

void cleanup(int x) 
{ 
	shmctl(shmid, IPC_RMID, 0);  /* detach sh mem */
	exit(0);
}
