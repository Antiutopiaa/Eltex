#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>
#include <signal.h>

#include <semaphore.h>

// shared memory POSIX
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <ncurses.h>

#include "header.h"

char msg4send[SIZE_MSG];

int fd;
char self_name[NAME_SIZE];
unsigned int local_num_users;
unsigned int local_curr_msg;
void *start_of_segment;
sem_t *sem;

struct shared_data *buffer;

pthread_t win_user_pthid;
pthread_t win_input_pthid;
pthread_t recive_msg_pthid;

void sigint_handler(int signum)
{
    (void)signum;
    endwin();
    // thread exit
    pthread_cancel(win_user_pthid);
    pthread_cancel(win_input_pthid);
    pthread_cancel(recive_msg_pthid);
   
    pthread_join(win_user_pthid, NULL);
    pthread_join(win_input_pthid, NULL);
    pthread_join(recive_msg_pthid, NULL);

    close(fd);
    //munmap(start_of_segment, sizeof(struct shared_data));

    sem_close(sem);
    
    exit(EXIT_SUCCESS);
}

int connect_to_segment(void)
{
   sem = sem_open(SEM_NAME, O_RDWR);
   if (sem == SEM_FAILED)
   {
       perror("sem_open");
       return -1;
   }

    fd = shm_open(SHM_NAME, O_RDWR, 0644);
    if (fd == -1)
    {
        perror("shm_open");
        sem_close(sem);
        return -1;
    }

    start_of_segment = mmap(0, sizeof(struct shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (start_of_segment == MAP_FAILED)
    {
        perror("mmap");
        sem_close(sem);
        return -1;
    }

    buffer = start_of_segment;

    return 0; 
}

void *recive_msg(void *arg)
{
    (void)arg;
    int flag = 1;
    while (flag)
    {
        if (local_curr_msg < buffer->num_msg)
        {
            sem_wait(sem);
            win_chat_f();
            local_curr_msg++;
            sem_post(sem);
        }
        usleep(50000); // 0.05 second
    }

    return NULL;
}

void send_msg(void)
{
    sem_wait(sem);

    snprintf(buffer->messages[buffer->num_msg], SIZE_MSG, "%s: %s", self_name, msg4send);

    buffer->num_msg += 1;
        
    sem_post(sem);
}

void replace_n_on_null(char *string, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (string[i] == '\n')
        {
            string[i] = '\0';
            return;
        }
    }
}

void init_user(void)
{
    // get self name (< 16 symbols)
    printf("Enter ur name: ");
    fgets(self_name, sizeof(self_name), stdin);
    replace_n_on_null(self_name, sizeof(self_name));

    sem_wait(sem);
    
    snprintf(buffer->users[buffer->num_users], NAME_SIZE, "%s", self_name);
    local_curr_msg = buffer->num_msg;
    buffer->num_users += 1;
    sem_post(sem);

    local_num_users = 0;
    win_chat_f();
    usleep(600000);
}

int main(void)
{
    buffer = NULL;
    
    signal(SIGINT, sigint_handler);
    
    if (connect_to_segment())
    {
        exit(EXIT_FAILURE);
    }

    init_user();

    pseudographics();

    pthread_t win_user_pthid;
    pthread_t win_input_pthid;
    pthread_t recive_msg_pthid;

    if (pthread_create(&win_user_pthid, NULL, win_user_f, NULL) != 0)
    {
        endwin();
        perror("pthread_create user_win");
        kill(getpid(), SIGINT);
    }
    if (pthread_create(&recive_msg_pthid, NULL, recive_msg, NULL) != 0)
    {
        endwin();
        perror("pthread_create recive_msg");
        kill(getpid(), SIGINT);
    }
    if (pthread_create(&win_input_pthid, NULL, win_input_f, NULL) != 0)
    {
        endwin();
        perror("pthread_create win_input");
        kill(getpid(), SIGINT);
    }

    pause();

    exit(EXIT_SUCCESS);
}
