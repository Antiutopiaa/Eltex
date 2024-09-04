#include <bits/types/sigset_t.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

volatile sig_atomic_t paused = 1;

void signal_handler(int signum)
{
    paused = 0;
}

int main(void)
{
    char buffer[MSG_SIZE];
    unsigned int prio;
    
    // init attr for signal
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    // starting signal handler
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MSG_SIZE + sizeof(pid_t);
    attr.mq_curmsgs = 0;

    struct msg_handshake handshake;
    handshake.pid = getpid();
    strcpy(handshake.msg, "Hi!\0");

    // create & open file desc
    mqd_t fd = mq_open(NAME_MQ, O_CREAT | O_RDWR, 0644, &attr);   
    if (fd == (mqd_t)-1)
    {
        perror("mq_open");
        mq_unlink(NAME_MQ);
        exit(EXIT_FAILURE);
    }
    
    // send pid & "Hi!" to client
    if (mq_send(fd, (const char *)&handshake,\
                sizeof(struct msg_handshake), 1) == -1)
    {
        perror("mq_send");
        mq_close(fd);
        mq_unlink(NAME_MQ);
        exit(EXIT_FAILURE);
    }

    while (paused == 1)
    {
        pause();
    }
    
    if (mq_receive(fd, buffer, MSG_SIZE + sizeof(pid_t), &prio) == -1)
    {
        perror("mq_receive");
        mq_close(fd);
        mq_unlink(NAME_MQ);
        exit(EXIT_FAILURE);
    }

    printf("message from client: %s\n", buffer);

    mq_close(fd);
    mq_unlink(NAME_MQ);

    exit(EXIT_SUCCESS);
}
