#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

int x = 0, y = 0;

int do_something()
{
	sleep(1);
   printf ("\nThis is process(thread) %d. \n", getpid());
	x = 7;
	printf("x + y = %d\n", x + y);
	_exit(0);
}

int main ( void ) 
{
	const int STACK_SIZE = 65536;
	char *stack, *stackTop;
   
   pid_t pid, fpid;
	int cret;

	stack = malloc(STACK_SIZE);
	stackTop = stack + STACK_SIZE;
   

	cret = clone(do_something,stackTop,CLONE_VM|CLONE_FILES,NULL);
	if (cret == -1) {
		perror("clone");
		exit(0);
	}
   
   printf ("\nThis is process (thread) %d.\n", getpid());
   y = 1;
   printf("x + y = %d\n", x + y);
}
