#include "functions.c"



int main(void)
{

   srand(time(NULL));
   scanf("%d %d",&students,&washing_machines);

   seconds_completed = 0;
   totalsec_wasted   = 0;
   
   studentsremained = students;
   for(int i = 0 ; i < students ;i++)
   {
      S[i].id = i;
      scanf("%d %d %d",&S[i].arrival_time,&S[i].washing_time,&S[i].patience_time);
   } 

   //Now create a thread independently for each student 
   //initalise semaphore and mutex lock

   sem_init(&wash_machines,0,washing_machines);
   sem_init(&mu_studentsleft,0,1);
   //sem_init(&secondslock,0,1);
   pthread_mutex_init(&secondslock,NULL);
   //initialising the semaphore with number of washing machines
    h1 = (struct timespec *) malloc(sizeof(struct timespec *));
    clock_gettime(CLOCK_REALTIME,h1);
   start_time = time(NULL);
   for(int i = 0 ; i < students ;i++)
   {
      pthread_create(&S_thread[i],NULL,Schedule_WashingMachine,&S[i]);
      usleep(1);
   }

   
    

   for(int i = 0 ; i < students ; i++)
   {
    pthread_join(S_thread[i],NULL);
   }
   

    printf("%d\n",students_left);
    printf("%d\n",totalsec_wasted);
   

   isrequired = 0;

   if(students_left*4 >= students)
   {
      isrequired = 1;
   }

   if(isrequired == 1)
   {
      printf("YES\n");
   }
   else
   {
    printf("NO\n");
   }

  return 0;




}