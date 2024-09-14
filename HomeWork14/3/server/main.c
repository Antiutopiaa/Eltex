#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <signal.h>

#include <semaphore.h>

// shared memory POSIX
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

//#define SEM_RCV_NAME "/server_sem_rcv"
#define SEM_NAME "/server_sem"
#define SHM_NAME "/server_shm"

#define SERVER_NAME "server" // self name for send notification

// config data
#define USERS_MAX 10 
#define MSG_MAX 125 // this value is allocated as the remainder to the end of second page
#define SIZE_MSG 64
#define NAME_SIZE 16

struct shared_data
{
    unsigned int num_users;             // number of users
    unsigned int num_msg;               // number of messages, current messages
    char users[USERS_MAX][NAME_SIZE];   // names of all users
    char messages[MSG_MAX][SIZE_MSG];   // all messages
};

unsigned int local_num_users;

void *start_of_segment;
sem_t *sem;
//sem_t *sem_rcv;

int fd;

pthread_t hello_notification_id;

void sigint_handler(int sig)
{
    (void)sig;
    munmap(start_of_segment, sizeof(struct shared_data));
    sem_close(sem);

    shm_unlink(SHM_NAME);
    sem_unlink(SEM_NAME);
    
    close(fd);

    pthread_cancel(hello_notification_id);
}

void* notification_of_new_users(void* arg)
{
    struct shared_data *buffer = (struct shared_data *)arg;
    int flag = 1;
    local_num_users = 0;
    while (flag)
    {
        if (local_num_users < buffer->num_users)
        {
            sem_wait(sem);
            
            snprintf(buffer->messages[buffer->num_msg], sizeof(buffer->messages[buffer->num_msg]), "user %s joined to chat\n", buffer->users[local_num_users]);
            
            buffer->num_msg += 1;
            
            sem_post(sem);
            
            local_num_users += 1;
        }
        usleep(500000);
    }
    
    return NULL;
}

int main()
{
    struct shared_data *buffer;

    signal(SIGINT, sigint_handler);
    printf("Wait for the server to initialize everything it needs...\n");

    sem = sem_open(SEM_NAME, O_CREAT | O_RDWR, 0644, 1);
    if (sem == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    /*sem_rcv = sem_open(SEM_RCV_NAME, O_CREAT | O_RDWR, 0644, 1);
    if (sem_rcv == SEM_FAILED)
    {
        perror("sem_open(rcv)");
        sem_close(sem);
    }*/

    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0644);
    if (fd == -1)
    {
        perror("shm_open");
        sem_close(sem);
        //sem_close(sem_rcv);
        sem_unlink(SEM_NAME);
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, sizeof(struct shared_data)) == -1)
    {
        perror("ftruncate");
        sem_close(sem);
        //sem_close(sem_rcv);
        close(fd);
        sem_unlink(SEM_NAME);
        shm_unlink(SHM_NAME);
        exit(EXIT_FAILURE);
    }

    start_of_segment = mmap(0, sizeof(struct shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (start_of_segment == MAP_FAILED)
    {
        perror("mmap");
        sem_close(sem);
        //sem_close(sem_rcv);
        close(fd);
        sem_unlink(SEM_NAME);
        shm_unlink(SHM_NAME);
        exit(EXIT_FAILURE);
    }

    buffer = start_of_segment;

    buffer->num_msg = 0;
    buffer->num_users = 0;

    if (pthread_create(&hello_notification_id, NULL, notification_of_new_users, (void *)buffer) != 0)
    {
        perror("pthread_create");
        sem_close(sem);
        //sem_close(sem_rcv);
        munmap(start_of_segment, sizeof(struct shared_data));
        close(fd);
        sem_unlink(SEM_NAME);
        shm_unlink(SHM_NAME);
        exit(EXIT_FAILURE);

    }

    printf("The server is ready for clients to connect!\n");
    printf("To exit the program, press Ctrl+C...\n");

    pause();

    exit(EXIT_SUCCESS);
}
