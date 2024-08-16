#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>

struct shop
{
    unsigned int balance; // balance 0-10000
    bool status; // 0 - unlock, 1 - lock
};

struct shop shops[5];

pthread_mutex_t mtx[5]; // mutex for shops
bool signal_of_customer_need[3] = {1, 1, 1}; // 1 - there is a need, 0 - there is no need


/*the buyer function simulates the life of the buyer 
 * and is used for the thread. 
 * when the buyer enters the shop, it locks with a mutex,
 * after which it falls asleep for 1 second
 *
 * it takes the ID, which is issued by the mine function when created, as an input*/ 

void* buyer(void* args)
{

    int *self_id_buyer = (int *)args;
    signal_of_customer_need[*self_id_buyer] = 1;

    srand(time(NULL));
    int need = rand() % 100000 + 1; // 1-10000 //
    short num_shop;

    while (need > 0)
    {
        for (int i = 0; i < 5; i++)
        {
            if (shops[i].status == 0)
            {
                num_shop = i;
                break;
            }

        }

        pthread_mutex_lock(&mtx[num_shop]);
        shops[num_shop].status = 1;

        printf("Customer #%d, went into store #%d, there were %d, need %d. Fell asleep\n", \
                *self_id_buyer, num_shop, shops[num_shop].balance, need);
    
        if (need >= shops[num_shop].balance)
        {
            need -= shops[num_shop].balance;
            shops[num_shop].balance = 0;
        }
        else 
        {
            shops[num_shop].balance -= need;
            need = 0;
        }
        pthread_mutex_unlock(&(mtx[num_shop]));            
        shops[num_shop].status = 0;

        sleep(2);
    }

    signal_of_customer_need[*self_id_buyer] = 0;
    //printf("buyer #%d: exit! need = %d\n", *self_id_buyer, need);

    return NULL;
}

/*the loader function simulates the life of a loader 
 * and is used for a thread.
 * when it starts working in a shop, the shop is locked using a mutex,
 * after which it falls asleep for 1 second.
 *
 * no arguments*/


void* loader(void* args)
{
    srand(time(NULL));
    while (signal_of_customer_need[0] != 0 || signal_of_customer_need[1] != 0 || signal_of_customer_need[2] != 0) 
    {
        int num_shop = rand() % 5;
        if (shops[num_shop].status == 0)
        {
        pthread_mutex_lock(&mtx[num_shop]);
        shops[num_shop].status = 1;
        printf("loader: went into shop #%d, there were %d, I put 5000. Fell asleep\n", num_shop, shops[num_shop].balance);
        shops[num_shop].balance += 5000;

        pthread_mutex_unlock(&mtx[num_shop]);
        shops[num_shop].status = 0;

        sleep(1);
        }

    }

    //printf("\n\nloader: exit!\n[0] = %d,[1] = %d, [2] = %d\n\n", signal_of_customer_need[0], signal_of_customer_need[1], signal_of_customer_need[2]);

    return NULL;
}

int main(void) 
{
    srand(time(NULL));
    pthread_t threads_buyers[3];
    pthread_t threads_loader;
    int id_buyers[3];
    int i, i1;
    int *return_value;
    
    // filling shops & init mutex
    for (int i = 0; i < 5; i++) 
    {
        shops[i].balance = rand() % 10001; // 0-10000
        pthread_mutex_init(&mtx[i], NULL);
    }

    // create threads buyers
    for (i = 0; i < 3; i++)
    {
        id_buyers[i] = i;
        if(pthread_create(&threads_buyers[i], NULL, buyer, (void *)&id_buyers[i]) != 0)
        {
            perror("buyer: cannot create thread!\n");
            exit(EXIT_FAILURE);
        }
        
    }

    if (pthread_create(&threads_loader, NULL, \
            loader, NULL) != 0) // create thread loader
    {
        perror("loader: cannot create thread!\n");
        exit(EXIT_FAILURE);
    }
    // waiting for threads to complete
    while (signal_of_customer_need[0] || signal_of_customer_need[1] || signal_of_customer_need[2]) 
    {
        sleep(2);
    }

    // join buyers
    for (i1 = 0; i1 < 3; i1++) 
    {
        pthread_join(threads_buyers[i1], (void **)&return_value);
    }

    // join loader
    pthread_join(threads_loader, (void **)&return_value);

    // destroy mutex
    for (i = 0; i < 5; i++) {
        pthread_mutex_destroy(&mtx[i]);
    }

    exit(EXIT_SUCCESS);

}
