#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define MAX_STUDENTS 1000

int students_left = 0;
int students;
int washing_machines;
int isrequired = 0;


sem_t wash_machines;
int studentsremained;

time_t start_time;

int totalsec_wasted;
pthread_mutex_t secondslock;


int seconds_completed = 0;

typedef struct student{
    
    int id;
    int arrival_time;
    int washing_time;
    int patience_time;
    struct timespec *currtime;

} student;

student  S[MAX_STUDENTS];
pthread_t S_thread[MAX_STUDENTS];
sem_t mu_studentsleft;
 struct timespec * h1;



struct timespec * SetTime(int time);
void * Schedule_WashingMachine(void* arg);
void yellow();
void white();
void reset();
void red();
void green();






