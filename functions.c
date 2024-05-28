#include "headers.h"

//check 41 line 
struct timespec * SetTime(int time)
{
     struct timespec * t = (struct timespec *) malloc(sizeof(struct timespec *));
     clock_gettime(CLOCK_REALTIME,t);
     t->tv_sec+=time;
     t->tv_nsec+=9000000;
    
     return t;
}


void * Schedule_WashingMachine(void* arg)
{
    student *st = (student *)arg;

    int ok;
    int c = st->arrival_time;
    sleep(c);
    white();
    printf("%d : student %d arrives\n",c,st->id+1);
    reset();
    struct timespec *c1 = SetTime(st->patience_time);
   
    ok = sem_timedwait(&wash_machines,c1);

    if(ok == 0)
    {
        //NOW this student is so lucky as he got the washing machine
         struct timespec *c2 = (struct timespec *) malloc(sizeof(struct timespec *));
         int c_time;
         clock_gettime(CLOCK_REALTIME,c2);
         c_time = c2->tv_sec-h1->tv_sec;
         pthread_mutex_lock(&secondslock);
         totalsec_wasted+=((int)(time(NULL)-start_time))-st->arrival_time;
         pthread_mutex_unlock(&secondslock);
         green();
         printf("%d : student %d starts washing\n",(int)(time(NULL)-start_time),st->id+1);
         reset();

         sleep(st->washing_time);

         
         yellow();
         printf("%d : student %d leaves after washing\n",(int)(time(NULL)-start_time),st->id+1);
         reset();

         sem_post(&wash_machines);


        //Now student completes his washing  

    }
    else
    {
        if(ok == -1 && errno == ETIMEDOUT)
        {
           pthread_mutex_lock(&secondslock);
           totalsec_wasted+=(c1->tv_sec-h1->tv_sec-st->arrival_time);
           pthread_mutex_unlock(&secondslock);
           red();
           printf("%ld : student %d leaves without washing\n",c1->tv_sec-h1->tv_sec,st->id+1);
           reset();
           sem_wait(&mu_studentsleft);
           students_left++;
           sem_post(&mu_studentsleft); 
        }
        else
        {
            printf("something went wrong in sem_timedwait");
        }
    }

}


void yellow()
{
    printf("\033[1;33m");
}


void green()
{
    printf("\033[0;32m");
}


void red()
{
    printf("\033[0;31m");
}


void white()
{
    printf("\033[0;37m");
}

void reset()
{
    printf("\033[0m");
}