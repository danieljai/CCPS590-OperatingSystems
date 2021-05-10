// From: http://beej.us/guide/bgipc/output/html/multipage/mq.html
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf {
    long mtype;
    char mtext[200];
};

int main(int argc, char *argv[])
{
    struct my_msgbuf buf;
    int msqid;
	char *ptr;
	long key2;
    key_t key;


	if (argc == 1) {
		printf("Terminated. Missing key integer.\n");
		exit(1);
	}

	key2 = strtol(argv[1],&ptr,10);

	if (key2==0) {
		printf("Terminated! Argument needs to be an integer\n");
		exit(1);
	}

    if ((msqid = msgget(key2, 0644 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
    
    printf("Enter lines of text, ^D to quit:\n");

    buf.mtype = 1; /* we don't really care in this case */

    while(fgets(buf.mtext, sizeof buf.mtext, stdin) != NULL) 
	{
        int len = strlen(buf.mtext);

        /* ditch newline at end, if it exists */
        if (buf.mtext[len-1] == '\n') 
			buf.mtext[len-1] = '\0';

        if (msgsnd(msqid, &buf, len+1, 0) == -1) /* +1 for '\0' */
            perror("msgsnd");
    }

    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }

    return 0;
}

