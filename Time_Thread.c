
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <time.h>

#define PHILOS 5
#define DELAY 5000
#define FOOD 50
void *philosopher (void *id);
void grab_chopstick (int,int,char *);
void down_chopsticks (int,int);
int food_on_table ();
long time_clock[5]={0};
pthread_mutex_t chopstick[PHILOS];
pthread_t philo[PHILOS];
pthread_mutex_t food_lock;
int sleep_time = 0;
int count1 =0;
//clock_t start1,end1,total1;
    //start1=clock();
int main (int argn,char **argv)

{   
    printf("Thoi gian ngu: %d\n",sleep_time);
    int i;
    struct timespec start,end;

    if (argn == 2)
        sleep_time = atoi (argv[1]);

    pthread_mutex_init (&food_lock, NULL);
    for (i = 0; i < PHILOS; i++)
        pthread_mutex_init (&chopstick[i], NULL);
    for (i = 0; i < PHILOS; i++)
        pthread_create (&philo[i], NULL, philosopher, (void*)i);
    for (i = 0; i < PHILOS; i++)
    {
        clock_gettime(CLOCK_THREAD_CPUTIME_ID,&start);
        pthread_join (philo[i], NULL);
        clock_gettime(CLOCK_THREAD_CPUTIME_ID,&end);
        time_clock[i] = end.tv_nsec - start.tv_nsec;
        }
        for(i = 0; i<PHILOS;i++)
        printf("Thread %d: %ld nanoseconds\n",i,time_clock[i]);
    printf("Thoi gian ngu: %d\n",sleep_time);   
    printf("so thuc an ban dau:: %d\n",FOOD);
    //printf("so luong thuc an da dung: %d\n",count1);
    printf("Thuc an da het => KET THUC \n");
    
    
    
    return 0;
}

void *philosopher (void *num)
{
    int id;
    int i, left_chopstick, right_chopstick, f;

    id = (long)num;
    printf ("Thang thu %d da san sang an.\n", id);
    right_chopstick = id;
    left_chopstick = id + 1;

    /* Wrap around the chopsticks. */
    if (left_chopstick == PHILOS)
        left_chopstick = 0;

    while (f = food_on_table ()) {

      
        if (id == 1){
        grab_chopstick (id, left_chopstick, "trai");
        grab_chopstick (id, right_chopstick, "phai");
         printf ("Thang thu %d: dang an.\n", id);
        usleep (DELAY * (FOOD - f + 1));
        down_chopsticks ( right_chopstick,left_chopstick);
        count1++;}
            //sleep (sleep_time);
        else{
        grab_chopstick (id, right_chopstick, "phai");
        grab_chopstick (id, left_chopstick, "trai");}

        printf ("Nguoi thu %d: dang an.\n", id);
        usleep (DELAY * (FOOD - f + 1));
        down_chopsticks (left_chopstick, right_chopstick);
        count1++;
    }
    
    printf ("Thang thu %d da an xong.\n", id);
    
    return (NULL);
}

int
food_on_table ()
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

void
grab_chopstick (int phil,int c,char *hand)
{
    pthread_mutex_lock (&chopstick[c]);
    printf ("Thang thu %d: co dua %s %d\n", phil, hand, c);
}

void
down_chopsticks (int c1,
                 int c2)
{
    pthread_mutex_unlock (&chopstick[c1]);
    pthread_mutex_unlock (&chopstick[c2]);
   
}

