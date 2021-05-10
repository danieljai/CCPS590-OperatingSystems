#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER;

void *functionCount1();
void *functionCount2();
int  count = 0;

#define COUNT_DONE  10
#define COUNT_HALT1  3
#define COUNT_HALT2  6

int main()
{
   pthread_t thread1, thread2;

   pthread_create( &thread1, NULL, &functionCount1, NULL);
   pthread_create( &thread2, NULL, &functionCount2, NULL);

   pthread_join( thread1, NULL);
   pthread_join( thread2, NULL);

   printf("Final count: %d\n", count);

   exit(0);
}

void *functionCount1()
{
   for(;;)
   {
      // Lock mutex and then wait for signal to relase mutex
      pthread_mutex_lock( &count_mutex );

      pthread_cond_wait( &condition_var, &count_mutex );

      count++;
      printf("Counter value functionOdd: %d\n",count);

      pthread_mutex_unlock( &count_mutex );

      if(count >= COUNT_DONE) return(NULL);
    }
}

void *functionCount2()
{
    for(;;)
    {
       pthread_mutex_lock( &count_mutex );

       if( count % 2 == 1 )
       {
			
          pthread_cond_signal( &condition_var );
       }
       else
       {
            if(count >= COUNT_DONE) {
                pthread_mutex_unlock( &count_mutex );
                return(NULL);
            }

          count++;
          printf("Counter value functionEven: %d\n",count);
       }

       pthread_mutex_unlock( &count_mutex );
    }

}
