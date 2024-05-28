#include "functions.c"



int main(void)
{

    
    srand(time(NULL));
    start_time = time(NULL);

    scanf("%d%d%d%d%d%d",&number_chefs,&num_pizzavarieties,&num_limitedingredients,&num_customers,&num_ovens,&timetaken_pickupspot);
    int id;

  //  printf("%d %d %d %d %d %d",number_chefs,num_pizzavarieties,num_limitedingredients,num_customers,num_ovens,timetaken_pickupspot);
   

    
    
    for(int i = 1 ;i<=num_pizzavarieties;i++)
    {
        scanf("%d",&id); 
        pizza[id].id = id;
        scanf("%d%d",&pizza[id].preparationtime,&pizza[id].num_limitedingredients);
        for(int j = 1 ; j <= pizza[id].num_limitedingredients ; j++)
        {
            scanf("%d",&pizza[id].ID[j]);
        }

       // printf("%d ",id); 
       // printf("%d %d ",pizza[id].preparationtime,pizza[id].num_limitedingredients);
       // for(int j = 1 ; j <= pizza[id].num_limitedingredients ; j++)
       // {
       //     printf("%d ",pizza[id].ID[j]);
       // }






    }




    
   //printf("%d\n",num_limitedingredients);

    for(int i =1;i<=num_limitedingredients ;i++)
    {
          if(i!=num_limitedingredients)
          scanf("%d ",&amount_ingredients[i]);
          else
          scanf("%d",&amount_ingredients[i]);
          pthread_mutex_init(&ingredients[i],NULL);
    }

   // for(int i = 1 ; i<=num_limitedingredients ; i++)
   // {
   //     printf("%d ",amount_ingredients[i]);
   // }

    sem_init(&n_ovens,0,num_ovens);





    for(int i = 1; i <= number_chefs ; i++)
    {
        chef[i].id = i;
        sem_init(&chef[i].cheflock,0,0);
        pthread_mutex_init(&chef[i].chefmutex,NULL);
        sem_init(&chef[i].signallock,0,0);
        scanf("%d%d",&chef[i].arrivaltime,&chef[i].exittime);
        chef[i].assignedto = -2;



        
    }

   // for(int i = 1 ; i <= number_chefs ; i++)
   // {
   //     printf("%d %d\n",chef[i].arrivaltime,chef[i].exittime);
   // }


    for(int i = 1 ; i<=num_customers ;i++)
    {
        customer[i].id = i;
        sem_init(&customer[i].customerlock,0,1);
        scanf("%d%d",&customer[i].arrivaltime,&customer[i].number_pizzas);

        for(int j = 1 ;j <= customer[i].number_pizzas ;j++)
        {
            scanf("%d",&customer[i].pizzas[j]);
        }

    }

    //create thread for each customer and each chef

   // for(int i = 1 ; i <= number_chefs ; i++)
   // {
   //     printf("%d %d %d\n",chef[i].id,chef[i].arrivaltime,chef[i].exittime);
   // }

    printf("Simulation Started\n");

    //create a thread for updating time

    pthread_t time_thread;

    pthread_create(&time_thread,NULL,time_count,NULL);

     for(int i =1 ;i<=number_chefs;i++)
    {
        sem_init(&chef[i].cheflock,0,1);
        pthread_create(&chef_thread[i],NULL,chef_func,&chef[i]);
    }
    
    for(int i=1;i<=num_customers ;i++)
    {
        pthread_create(&Custom[i],NULL,customer_func,&customer[i]);
    }

    
   

    for(int i=1;i<=number_chefs;i++)
    {
        pthread_join(chef_thread[i],NULL);
    }

    for(int i=1;i<=num_customers ; i++)
    {
        pthread_join(Custom[i],NULL);
    }

    printf("Simulation Ended\n");

    

    






}