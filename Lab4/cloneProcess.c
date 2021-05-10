/*
  In this program we make a child process (heavyweight process) by using clone, but not setting 
  any flags (which is what fork does). The child would get a copy of parents memory, file descs, 
  etc, but any changes in child would be only for the child

  For making a child thread (lightweight process) see cloneThread.c
*/

#define _GNU_SOURCE  //for Ubuntu
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

int variable, fd;

int do_something() 
{
	variable = 42;
	close(fd);
	printf("I am %d and my parent is %d\n", getpid(), getppid());
	_exit(0);
}

int main(int argc, char *argv[]) 
{
	const int STACK_SIZE = 65536;  // note stack grows down
	char *stack, *stackTop, tempch;
	int cret;

	stack = malloc(STACK_SIZE);
	if (stack == NULL) {
		perror("malloc"); 
		exit(1);
	}
	stackTop = stack + STACK_SIZE;  /* Assume stack grows downward */

	variable = 9;
	fd = open("test.file", O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "To run this, you need a file named test.file in this directory!!\n");
		exit(2);
	}
	printf("The variable was: %d\n", variable);
   
	cret = clone(do_something, stackTop, 0, NULL);
	if (cret == -1)	{
		perror("clone"); 
		exit(0); 
	}
	
	// parent keeps going here
	printf("Part after clone has pid %d\n", getpid());
	sleep(1);

	printf("The variable is now: %d\n", variable);
	if (read(fd, &tempch, 1) < 1) {
		perror("File Read Error");
		exit(1);
	}
	
	printf("Read from the file: %s\n", &tempch);
	return 0;
}