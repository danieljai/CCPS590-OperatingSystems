#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main ( void ) 
{
   int x = 0, y = 0;
   pid_t pid, fpid;
   
   fpid = fork ();
   pid  = getpid();
   
   if (fpid > 0) {
       printf ("\nThis is process (thread) %d.\n", pid);
       y = 1;
   }
   else if (fpid == 0){
       sleep(1);
       printf ("\nThis is process(thread) %d. \n", pid);
       x = 7;
   }
   else {
       printf ("fork failed\n");
       return (1);
   }
   
   printf("x + y = %d\n", x + y);
}
