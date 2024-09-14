#include <sys/ipc.h>
#include <sys/shm.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define KEYGEN "keygen", 1
#define END() do \
    { \
    shmdt(start_of_segment); \
    } while (0)

int main(void)
{
    int *sig;
    int shmid;
    void *start_of_segment;

    key_t key = ftok(KEYGEN);

    shmid = shmget(key, sizeof(char) * 2 * 32 + sizeof(int), 0644); // check server.c
    if (shmid == -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    start_of_segment = shmat(shmid, NULL, 0);
    if (start_of_segment == ((void *)-1))
    {
        perror("shmat");
        END();
        exit(EXIT_FAILURE);
    }

    sig = (int *)start_of_segment;
    char (*message)[32] = (char (*)[32])(start_of_segment + sizeof(int));

    while (*sig != 1)
    {
        pause();
    }
    
    printf("server: %s\n", message[0]);

    strcpy(message[1], "Hello!");
    *sig = 2;
    
    END();
    
    exit(EXIT_SUCCESS);
}
