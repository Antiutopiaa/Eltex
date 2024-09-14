#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <stdlib.h>
#include <unistd.h>

#define KEYGEN "keygen", 1
#define END() do \
    { \
    shmdt(start_of_segment); \
    shmctl(shmid, IPC_RMID, NULL); \
    } while (0)

int main(void)
{
    int *sig;

    key_t key = ftok(KEYGEN);
    
    // int sig + char[2][32]
    int shmid = shmget(key, sizeof(char) * 2 * 32 + sizeof(int), 0644 | IPC_CREAT);
    if (shmid == -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    void *start_of_segment = shmat(shmid, NULL, 0);

    if (start_of_segment == ((void *)-1))
    {
        perror("shmat");
        END();
        exit(EXIT_FAILURE);
    }

    sig = (int *)start_of_segment;
    char (*message)[32] = (char (*)[32])(start_of_segment + sizeof(int));

    *sig = 0;

    strcpy(message[0], "Hi!");
    *sig = 1;

    while(*sig != 2)
    {
        sleep(1);
    }

    printf("client: %s\n", message[1]);

    END();
    exit(EXIT_SUCCESS);
}
