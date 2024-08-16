#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define N 5

/*pthread_function is called by a thread and prints its number*/
/*takes the thread number as an argument                      */
void* pthread_function (void* arg) {
    int* num = (int *)arg;
    printf("Pthread num: %d\n", *num);

    return NULL;
}


int main(void) {
    int i, i1;
    pthread_t pthread[N];
    int *return_value;
    int args_value[N];

    for (i = 0; i < N; i++) {
        args_value[i] = i;
        pthread_create(&pthread[i], NULL, pthread_function, (void *)&(args_value[i]));
    }

    for (i1 = 0; i1 < N; i1++) {
        pthread_join(pthread[i1], (void **)&return_value);
    }

    exit(EXIT_SUCCESS);
}
