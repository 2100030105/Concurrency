# RealLife Simulation using Threads and locks
## Assumptions
- I assumed that the maximum number of studnets is 1000
- The student who arrives first will get the washing machine first and if two students are arriving at the same time then we will give the washing machine to the student who had lower index.
- We are calclating the wasting time in the following cases:
    - Waiting time for washing and then getting their clothes washed
    - Waiting for washing upto their patience time


 ## Procedure
 - I created a user defined data type for students which contains student id ,their arival time,their patience time and washing time
 - Iam creating a thread for each student 
 -Each thread executes independently of the thread
 -The variables studentsleft and totaltimewasted which are accessed by all the threads are protected by mutex locks and binary semaphores
 -every student thread sleeps up to the student arrival
 -After the arrival i used sem_timedwait function to check whether any washing machine is available for this student in a specific time slot upto patience time
 -The return values of the above function are used to find whether the student got washing machine or not
 -if return value is -1  and errno is set to timedout i print student left without washing
 -if return value is !=-1 then we print student starts washing
 -student thread sleeps for the washing amout of time
 - current time at every instant is calculated using time(NULL) function
 
