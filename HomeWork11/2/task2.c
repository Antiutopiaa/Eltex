#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 6 // num threads

long shared_resource = 0;

pthread_mutex_t mutex_point = PTHREAD_MUTEX_INITIALIZER;

/* pthread_function adds one to a global variable
 * there are no arguments*/

void* pthread_function(void* arg) {
    
    pthread_mutex_lock(&mutex_point);
    for (int i = 0; i < 10000000; i++) {
        //pthread_mutex_lock(&mutex_point);
        shared_resource++;
        //pthread_mutex_unlock(&mutex_point);
    }
    pthread_mutex_unlock(&mutex_point);
    return NULL;
}

int main(void) {
    int i, i1;
    pthread_t pthread[N];
    int *return_value;

    for (i = 0; i < N; i++) {
        pthread_create(&pthread[i], NULL, pthread_function, NULL);
    }

    for (i1 = 0; i1 < N; i1++) {
        pthread_join(pthread[i1], (void **)&return_value);
    }

    printf("Shared resource = %ld\n", shared_resource);

    exit(EXIT_SUCCESS);
}

