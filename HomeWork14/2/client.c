#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <signal.h>

#include <semaphore.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SEM_FILE "/server_sem"
#define SHM_FILE "/server_shm"

void *start_of_segment;
sem_t *sem;

struct shm_buff
{
    char server2client[32];
    char client2server[32];
};

void sigint_handler(int sig)
{
    (void)sig;
    munmap(start_of_segment, sizeof(struct shm_buff));
    sem_close(sem);

    shm_unlink(SHM_FILE);
    sem_unlink(SEM_FILE);
    exit(EXIT_SUCCESS);
}

int main(void)
{
    int fd;
    struct shm_buff *buffer;

    signal(SIGINT, sigint_handler);

    sem = sem_open(SEM_FILE, O_RDWR);
    if (sem == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    fd = shm_open(SHM_FILE, O_RDWR, 0644);
    if (fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, sizeof(struct shm_buff)) == -1)
    {
        perror("ftruncate");
        shm_unlink(SHM_FILE);
        exit(EXIT_FAILURE);
    }

    start_of_segment = mmap(0, sizeof(struct shm_buff), PROT_READ | PROT_WRITE, MAP_SHARED, fd , 0);
    if (start_of_segment == MAP_FAILED)
    {
        perror("mmap");
        shm_unlink(SHM_FILE);
        exit(EXIT_FAILURE);
    }
    buffer = start_of_segment;
    
    printf("server: %s\n", buffer->server2client);
    snprintf(buffer->client2server, 32, "Hello!");
    
    sem_post(sem);

    sem_close(sem);
    munmap(start_of_segment, sizeof(struct shm_buff));

    exit(EXIT_SUCCESS);
}
