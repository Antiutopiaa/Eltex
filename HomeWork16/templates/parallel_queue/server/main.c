#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>
#include <mqueue.h>
#include <sys/stat.h>

#define MSGLEN 64
#define BACKLOG 5
#define PROC_NUM 2

#define mq_del() do { mq_close(mq_tasks); mq_close(mq_resolved_task);\
                        mq_unlink("/tasks"); mq_unlink("/resolved_task"); } while (0)

struct msg
{
    struct sockaddr_in  client;
    char                buf[MSGLEN];
};

int     sfd;                        // server socket fd
mqd_t   mq_tasks, mq_resolved_task;

// task processing
void *data_proc(void *arg)
{
    int         ret;
    struct msg  task;
    char        id = *(char *)arg;

    printf("service data proc #%d is up\n", id);
    
    while (1)
    {

        ret = mq_receive(mq_tasks, (char *)&task, sizeof(task), NULL);
        if (ret < 0)
        {
            perror("mq_receive");
            continue;
        }

        task.buf[0] = '<';
        task.buf[1] = '3';
        task.buf[2] = '0' + id;

        sleep(2);
        
        ret = mq_send(mq_resolved_task, (char *)&task, sizeof(task), 2);
        if (ret < 0)
        {
            perror("mq_send");
            continue;
        }

    }

    exit(EXIT_SUCCESS);
}

void *sending_solve(void *arg)
{
    (void)arg;
    int         ret;
    struct msg  solve;

    printf("service sending data is up\n");
    
    while (1)
    {
        ret = mq_receive(mq_resolved_task, (char *)&solve, sizeof(solve), NULL);
        if (ret < 0)
        {
            perror("mq_receive");
            continue;
        }

        ret = sendto(sfd, (void *)solve.buf, sizeof(solve.buf), 0, (struct sockaddr *)&solve.client, sizeof(struct sockaddr_in));
        if (ret < 0)
        {
            perror("sendto");
            continue;
        }
    }

    return NULL;
}

int main(void)
{
    int                 ret;
    struct msg          task;
    struct mq_attr      tattr, sattr;
    struct sockaddr_in  server;
    socklen_t           server_len = sizeof(server), client_len = server_len;
    pthread_t           proc_servers[PROC_NUM], sending_tid;

    // get passive fd socket
    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, server_len);
    server.sin_family = AF_INET;
    server.sin_port = htons(2056);
    inet_aton("127.0.0.1", &server.sin_addr);

    ret = bind(sfd, (struct sockaddr *)&server, server_len);
    if (ret < 0)
    {
        perror("bind");
        close(sfd);
        exit(EXIT_FAILURE);
    }

    // create queue
    tattr.mq_flags = 0;
    tattr.mq_curmsgs = 0;
    tattr.mq_maxmsg = 10;
    tattr.mq_msgsize = sizeof(struct msg);
    
    mq_tasks = mq_open("/tasks", O_CREAT | O_RDWR, 0600, &tattr);
    if (mq_tasks < 0)
    {
        perror("mq_open 1");
        mq_unlink("/task");
        close(sfd);
        exit(EXIT_FAILURE);
    }

    sattr.mq_flags = 0;
    sattr.mq_curmsgs = 0;
    sattr.mq_maxmsg = 10;
    sattr.mq_msgsize = sizeof(struct msg);

    mq_resolved_task = mq_open("/resolved_task", O_CREAT | O_RDWR, 0600, &sattr);
    if (mq_resolved_task < 0)
    {
        perror("mq_open 2");
        mq_del();
        close(sfd);
        exit(EXIT_FAILURE);
    }

    // create threads sevices proc
    for (char i = 0; i < PROC_NUM; i++)
    {
        char *id = malloc(sizeof(short));
        *id = i;
        ret = pthread_create(&proc_servers[(int)i], NULL, data_proc, (void *)id);
        if (ret < 0)
        {
            perror("pthread_create");
            close(sfd);
            mq_del();
            exit(EXIT_FAILURE);
        }
    }

    // create thread service sending
    ret = pthread_create(&sending_tid, NULL, sending_solve, NULL);
    if (ret < 0)
    {
        perror("pthread_create");
        close(sfd);
        mq_del();
        exit(EXIT_FAILURE);
    }

    printf("listening server pid: %d\n", getpid());
    printf("server is ready to connect...\n");
    printf("\tlistening %s:%d\n", inet_ntoa(server.sin_addr), htons(server.sin_port));
    
    while (1) // listening service
    {
        ret = recvfrom(sfd, (void *)task.buf, sizeof(task.buf), 0, (struct sockaddr *)&task.client, &client_len);
        if (ret < 0)
        {
            perror("recvfrom");
            continue;
        }

        ret = mq_send(mq_tasks, (char *)&task, sizeof(task), 1);
        if (ret < 0)
        {
            perror("mq_send");
            continue;
        }
    }

    exit(EXIT_SUCCESS);
}
