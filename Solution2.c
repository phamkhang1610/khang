       #include <pthread.h>
       #include <stdio.h>
       #include <unistd.h>
       #include <stdlib.h>
       #include <errno.h>
       #include <assert.h>
       #include <time.h>
       #include <stdint.h>
       #include <string.h>

       #define PHILOS 5
       #define DELAY 5000
       #define FOOD 500
 	
 	// Initialize global data   
    	void *philosopher (void *id);
    	void grab_chopstick (int, int, char *);
    	void down_chopsticks (int, int);
    	int food_on_table ();
    	long time_clock[5] = {0};
    	
    	// create arrays with 5 items contain chopticks and philo
    	pthread_mutex_t chopstick[PHILOS];
    	pthread_t philo[PHILOS];
    	pthread_mutex_t food_lock;
    	int sleep_seconds =0	;
    	
    	
    	int main (int argn, char **argv)
    	{
    	    clock_t start_time, end_time;
    	    start_time = clock();
    	    int i;
    	    struct timespec start, end;
            // atoi() function converts a pointed string into interger type if not right -> return 0	
    	    if (argn == 2) sleep_seconds = atoi (argv[1]);
    	    printf("sleep seconds = %d\n", sleep_seconds);
    	    
    
    	    pthread_mutex_init (&food_lock, NULL);
    	    
    	    for (i = 0; i < PHILOS; i++)
	    {
		pthread_mutex_init (&chopstick[i], NULL);
	}
		
    	    printf("Main thread consuming some CPU time...\n");
            

    	    for (i = 0; i < PHILOS; i++)
    	        pthread_create (&philo[i], NULL, philosopher, &i);
    	    for (i = 0; i < PHILOS; i++)
    	    {
    	    	clock_gettime(CLOCK_THREAD_CPUTIME_ID,&start);
		pthread_join (philo[i], NULL);
    	        clock_gettime(CLOCK_THREAD_CPUTIME_ID,&end);
    	        time_clock[i] = end.tv_nsec - start.tv_nsec;
		}
		
	    for (i=0; i<PHILOS; i++)
	    
	    	printf("Thread %d: %ld nanoseconds\n",i,  time_clock[i]);
    	    end_time = clock();
    	    printf("STOP DININNER\n");
    	    return 0;
    	    
    	}
    	
    	// philosopher is a function create a thread
    	void *philosopher (void *num)
    	{
    	    int id;
    	    int i, left_chopstick, right_chopstick, f;
    	    
    	    id = *(int*)num;

    	    printf ("Philosopher %d is done thinking and now ready to eat.\n", id);
    	    right_chopstick = id;
    	    left_chopstick = id + 1;
    	
    	    /* Wrap around the chopsticks. */
    	    if (left_chopstick == PHILOS)
    	        left_chopstick = 0;
    
    	    while (f = food_on_table ()) {
    	        if(id==PHILOS-1)
    	        {   sleep(sleep_seconds);
	    	        grab_chopstick (id, right_chopstick, "right");
	    	        grab_chopstick (id, left_chopstick, "left");
	    	        printf ("Philosopher %d: eating.\n", id);
	    	        usleep (DELAY * (FOOD - f + 1));
	    	        down_chopsticks (left_chopstick, right_chopstick);
    	        }
    	        else{
	    	        grab_chopstick (id, right_chopstick, "right");
	    	        grab_chopstick (id, left_chopstick, "left");
	    	        printf ("Philosopher %d: eating.\n", id);
	    	        usleep (DELAY * (FOOD - f + 1));
	    	        down_chopsticks (left_chopstick, right_chopstick);
    	            
    	        }
				printf ("Philosopher %d is done eating.\n", id);
				return (NULL);
    	    }
    	}
    	
    	//
    	int food_on_table ()
    	{
    	    static int food = FOOD;
    	    int myfood;
    	
    	    pthread_mutex_lock (&food_lock);
    	    if (food > 0) {
    	        food--;
    	    }
    	    myfood = food;
    	    pthread_mutex_unlock (&food_lock);
    	    return myfood;
    	}
    	
    	void grab_chopstick (int phil,int c,char *hand)
   	{
   	    pthread_mutex_lock (&chopstick[c]);
   	    printf ("Philosopher %d: got %s chopstick %d\n", phil, hand, c);
   	}
   	
   	void down_chopsticks (int c1, int c2)
   	{
   	    pthread_mutex_unlock (&chopstick[c1]);
   	    pthread_mutex_unlock (&chopstick[c2]);
    }