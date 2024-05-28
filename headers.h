#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>


#define MAX_PIZZAS 50
#define MAX_CUSTOMERS 100
#define MAX_CHEFS 100
#define MAX_LIMITEDINGREDIENTS 50
#define MAX_INGREDIENTS 100


struct customer
{
     int id;
     int arrivaltime;
     int number_pizzas;
     int pizzas[MAX_PIZZAS];
     int numpizzas_completed;
     sem_t customerlock;
};

typedef struct customer Customer;

struct chef
{
    int id;
    int arrivaltime;
    int exittime;
    int assignedto;
    int pizzatype;
    sem_t cheflock;
    int pizzanumber;
    pthread_mutex_t chefmutex;
    sem_t signallock;
};

struct pizza
{
    int id;
    int preparationtime;
    int num_limitedingredients;
    int ID[MAX_LIMITEDINGREDIENTS];

};


struct order{
    int id;
    int pizzatype;
    int number;
};
typedef struct order Order;


typedef struct chef Chef;
typedef struct pizza Pizza;

Customer customer[MAX_CUSTOMERS];
Chef     chef[MAX_CHEFS];
Pizza    pizza[MAX_PIZZAS];
int amount_ingredients[MAX_INGREDIENTS];

void *function_chef(void *arg);
void *function_customer(void *arg);

int number_chefs;
int num_pizzavarieties;
int num_limitedingredients;
int num_customers;
int num_ovens;
int timetaken_pickupspot;

sem_t n_ovens;;

pthread_t Custom[MAX_CUSTOMERS];
pthread_t chef_thread[MAX_CHEFS];
pthread_mutex_t ingredients[MAX_INGREDIENTS];

int start_time;
int current_time;






