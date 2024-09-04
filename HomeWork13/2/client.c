#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mqueue.h>

#define MSG_SIZE 256
#define NAME_MQ "/server_mq"


struct msg_handshake
{
    pid_t pid;
    char msg[MSG_SIZE];
};

int main(void)
{
    char buffer[MSG_SIZE];
    unsigned int prio;
    struct msg_handshake handshake;

    mqd_t fd = mq_open(NAME_MQ, O_RDWR);
    if (fd == (mqd_t)-1)
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }
    
    if (mq_receive(fd, (char *)&handshake, sizeof(handshake), &prio) == -1)
    {
        perror("mq_receive");
        mq_close(fd);
        exit(EXIT_FAILURE);  
    }
    kill(handshake.pid, SIGUSR1);
    
    printf("message from server: %s\n", handshake.msg);
    

    strcpy(buffer, "Hello!\0");
    if (mq_send(fd, buffer, MSG_SIZE, 1) == -1)
    {
        perror("mq_send");
        close(fd);
    }

    close(fd);
    exit(EXIT_SUCCESS);
}
