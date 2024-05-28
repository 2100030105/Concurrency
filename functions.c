#include "headers.h"




void *time_count()
{
    while(1)
    {
        if(time(NULL)-start_time > current_time)
        {
            current_time++;
        }
    }
}



void *pizzafunc(void *arg)
{

    Order *o = (Order *)arg;
    //The main goal of this function is to make this order acceptible by any chef
    //if this order can't be accepted by any chef just inform it

    int assigned = -1;
    int ing = 1;

    for(int i = 1 ; i <= pizza[o->pizzatype].num_limitedingredients ;i++)
            {
               pthread_mutex_lock(&ingredients[pizza[o->pizzatype].ID[i]]);
               if(amount_ingredients[pizza[o->pizzatype].ID[i]]  == 0)
               {
                  ing = -1;
               }
               pthread_mutex_unlock(&ingredients[pizza[o->pizzatype].ID[i]]);

            }

    if(ing == -1)
    {
        return NULL;
    }       


   while(1)
   {

    for(int j = 1 ; j <= number_chefs ; j++)
    {
        pthread_mutex_lock(&chef[j].chefmutex);
        if(chef[j].assignedto == -1 && chef[j].exittime > pizza[o->pizzatype].preparationtime + 3 + current_time)
        {
            chef[j].assignedto = o->id;
            chef[j].pizzatype = o->pizzatype;
            chef[j].pizzanumber = o->number;
            assigned = j;
        }

        pthread_mutex_unlock(&chef[j].chefmutex);

        if(assigned != -1)
        {
            break;
        }
    }

    if(assigned != -1)
    {
        break;
    }

   }

   

    if(assigned != -1)
    {
        printf("pizza %d in order %d assigned to chef %d\n",o->number,o->id,assigned);
        sem_post(&chef[assigned].cheflock);
    }


    if(assigned != -1)
    {
        sem_wait(&chef[assigned].signallock);
    }

    return NULL;



        
}





void *customer_func(void *arg)
{
    Customer *cus =(Customer *)arg;

    sleep(cus->arrivaltime);
    printf("Customer %d arrives at time %d\n",cus->id,cus->arrivaltime);
    printf("order %d placed by customer %d has pizzas",cus->id,cus->id);
    for(int j = 1 ;j <= cus->number_pizzas ; j++)
    {
      if(j == 1)
      printf("{%d",cus->pizzas[j]);
      else
      printf(",%d",cus->pizzas[j]);
    }

    printf("}\n");
    printf("order %d placed by customer %d awaits processing\n",cus->id,cus->id);

    pthread_t pizza_thread[cus->number_pizzas];

    Order o;





    for(int j =0 ; j < cus->number_pizzas ; j++)
    {
          //create thread for each pizza 
          //Each pizza finds its own way of searching for a thread

          o.id = cus->id;
          o.number = j+1;
          o.pizzatype = cus->pizzas[j+1];

          pthread_create(&pizza_thread[j],NULL,pizzafunc,&o);

    }

   
    for(int j = 0 ; j < cus->number_pizzas ; j++)
    {
        pthread_join(pizza_thread[j],NULL);
    }

    sem_wait(&cus->customerlock);

    if(cus->numpizzas_completed == cus->number_pizzas)
    {
        printf("Order %d placed by customer %d has been processed\n",cus->id,cus->id);
    }
    else
    {
        if(cus->numpizzas_completed == 0)
        {
            printf("order %d placed by customer %d completely rejected\n",cus->id,cus->id);
            usleep(1);
            printf("Customer %d is rejected\n",cus->id);

            return NULL;
        }
        else
        {
            printf("Order %d placed by customer %d partially processed and remaining couldn't be\n",cus->id,cus->id);
        }
    }


    sem_post(&cus->customerlock);

    printf("Customer %d exits the drive thru zone\n",cus->id);










     
}

void *chef_func(void *arg)
{

    Chef *cf =(Chef *)arg;

    sleep(cf->arrivaltime);

   
     


    printf("Chef %d arrives at time %d\n",cf->id,cf->arrivaltime);


    pthread_mutex_lock(&cf->chefmutex);
    cf->assignedto = -1;
    pthread_mutex_unlock(&cf->chefmutex);
    struct timespec exit;
    clock_gettime(CLOCK_REALTIME,&exit);
    exit.tv_sec+=cf->exittime;
    exit.tv_nsec+=9000000;
    int ans = 1;
    while(1)
    {
         ans =  sem_timedwait(&cf->cheflock,&exit);

       
        if(ans == -1 &&  errno == ETIMEDOUT)
        {
              break;
        }
        else
        {
            //Now the chef has to prepare the assigned pizza
            //pizza type and order is known to the chef before hand as he is informed by pizza thread
            int c = 1;
            for(int i = 1 ; i <= pizza[cf->pizzatype].num_limitedingredients ;i++)
            {
               pthread_mutex_lock(&ingredients[pizza[cf->pizzatype].ID[i]]);
               if(amount_ingredients[pizza[cf->pizzatype].ID[i]]  == 0)
               {
                  c = -1;
               }
               else
               {
                amount_ingredients[pizza[cf->pizzatype].ID[i]]--;
               }

               pthread_mutex_unlock(&ingredients[pizza[cf->pizzatype].ID[i]]);

            }

            if(c == -1)
            {
                //make this chef free
                //inform couldnt prepare the pizza to the pizza thread by any means

                printf("chef %d could not complete pizza %d for order %d due to ingredient shortage\n",cf->id,cf->pizzanumber,cf->assignedto);
                sem_post(&cf->signallock);   
            }
            else
            {

                sleep(3);
                //sleeping for 3seconds as this time is used for gathering ingredients
                int k = 1;
                printf("Chef %d is waiting for oven allocation for pizza %d for order %d\n",cf->id,cf->pizzanumber,cf->assignedto);
                k = sem_timedwait(&n_ovens,&exit);

                if(k == -1 && errno == ETIMEDOUT)
                {
                    //order can't be prepared so notify according to this
                    sem_post(&cf->signallock);
                    break;

                }
                else
                {
                    //All set just sleep for amount of preparation time
                    if(current_time+pizza[cf->pizzatype].preparationtime > cf->exittime)
                    {
                        //order can't be prepared in this case also
                        sem_post(&cf->signallock);
                        break;
                    }
                    else
                    {
                       printf("Chef %d has put the pizza %d for order %d in the oven at time %d\n",cf->id,cf->pizzanumber,cf->assignedto,current_time);
                       sleep(pizza[cf->pizzatype].preparationtime);
                       sem_post(&n_ovens);

                       printf("Chef %d has picked up the pizza %d for order %d from the oven at time %d\n",cf->id,cf->pizzanumber,cf->assignedto,current_time);

                       sleep(timetaken_pickupspot);

                       printf("Customer %d picks up their pizza %d\n",cf->assignedto,cf->pizzanumber);

                       pthred_mutex_lock(&customer[cf->assignedto].customerlock);
                       customer[cf->assignedto].numpizzas_completed++;
                       pthread_mutex_unlock(&customer[cf->assignedto].customerlock);

                       sem_post(&cf->signallock);

                       
                       pthread_mutex_lock(&cf->chefmutex);
                        cf->assignedto = -1;
                       pthread_mutex_unlock(&cf->chefmutex);

                       //pizza preparation completed so make this chef free now 
                    }


                }


            }

            

        }
    }
    printf("Chef %d exits at time %d\n",cf->id,cf->exittime);
   
}