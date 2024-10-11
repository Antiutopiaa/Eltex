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


#define MSGLEN 64
#define BACKLOG 5
#define POOL_SIZE 2

struct data_pool
{
    bool        busy; // false - server is ready to connect true - server is busy
    int         cfd;  // client fd
    pthread_t   thread_id;
};

struct data_pool pool_servers[POOL_SIZE];

// child process pool overflow
void data_proc(int fd)
{
    int     ret;
    char    buf[MSGLEN];

    while (1)
    {
        ret = recv(fd, (void *)buf, sizeof(buf), 0);
        // recv error
        if (ret < 0)
        {
            perror("recv");
            strncpy(buf, "server error\0", sizeof(buf));
        }
        // recv ok
        else if (ret > 0)
        {
            buf[0] = '<';
            buf[1] = '3';
            buf[2] = 'O'; // pool Overfow
        }
        // connection closed
        else if (ret == 0)
        {
            exit(EXIT_SUCCESS);
        }

        ret = send(fd, (void *)buf, sizeof(buf), 0);
        if (ret < 0)
        {
            perror("send");
        }
    }

    exit(EXIT_SUCCESS);
}

// child process in pool
void *data_proc_pool(void *arg)
{
    int     ret;
    int     fd;
    char    buf[MSGLEN];
    short   *id = (short *)arg;
    short   self_id = *(short *)arg;

    free(id);

    printf("pool. server #%d is ready to work\n", self_id);

    while (true)
    {
        // waiting for load
        while (pool_servers[self_id].busy != true)
            sleep(1);

        fd = pool_servers[self_id].cfd;

        while (true)
        {
            ret = recv(fd, (void *)buf, sizeof(buf), 0);
            // recv error
            if (ret < 0)
            {
                perror("recv");
                strncpy(buf, "server error\0", sizeof(buf));
            }
            // recv ok
            else if (ret > 0)
            {
                buf[0] = '<';
                buf[1] = '3';
                buf[2] = 'P'; // Pool
            }
            // connection closed
            else if (ret == 0)
            {
                break;
            }

            ret = send(fd, (void *)buf, sizeof(buf), 0);
            if (ret < 0)
            {
                perror("send");
            }
        }

        pool_servers[self_id].busy = false;

    }

    return NULL;
}

// thread for pool server busy handler
void balancer(int cfd)
{
    int ret;

    for (int i = 0; i < POOL_SIZE; i++)
    {
        if (pool_servers[i].busy == false)
        {
            pool_servers[i].cfd = cfd;
            pool_servers[i].busy = true;
            printf("load directed to pool. server #%d\n", i);
            return;
        }
    }

    // pool is busy
    ret = fork();

    // error
    if (ret == -1)
    {
        perror("fork");
        return;
    }

    // child
    else if (ret == 0)
    {
        data_proc(cfd);
    }

    // parrent
    printf("create new child outside pool. pid = %d\n", ret);
    return;
}

int main(void)
{
    int                 ret;
    int                 sfd;                                // server fd, passive
    int                 cfd;                                // client fd, active
    struct sockaddr_in  server;
    socklen_t           server_len = sizeof(server);
    struct sockaddr_in  client;
    socklen_t           client_len = sizeof(server);
    
    // get passive fd socket
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, server_len);
    server.sin_family = AF_INET;
    server.sin_port = htons(2056);
    inet_aton("127.0.0.1", &server.sin_addr);

    memset(&client, 0, sizeof(client));

    ret = bind(sfd, (struct sockaddr *)&server, server_len);
    if (ret < 0)
    {
        perror("bind");
        close(sfd);
        exit(EXIT_FAILURE);
    }

    ret = listen(sfd, BACKLOG);
    if (ret < 0)
    {
        perror("listen");
        close(sfd);
        exit(EXIT_FAILURE);
    }

    // create threads
    for (int i = 0; i < POOL_SIZE; i++)
    {
        pool_servers[i].busy = false;
        short *id = malloc(sizeof(short));
        *id = i;
        ret = pthread_create(&pool_servers[i].thread_id, NULL, data_proc_pool, (void *)id);
        if (ret < 0)
        {
            perror("pthread_create");
            close(sfd);
            exit(EXIT_FAILURE);
        }
    }

    printf("listening server pid: %d\n", getpid());
    printf("server is ready to connect...\n");
    printf("\tlistening %s:%d\n", inet_ntoa(server.sin_addr), htons(server.sin_port));

    while (1) // connection cycle
    {
        cfd = accept(sfd, (struct sockaddr *)&client, &client_len);
        if (cfd < 0)
        {
            perror("accept");
            continue;
        }

        balancer(cfd);
        memset(&client, 0, sizeof(client));
    }

    exit(EXIT_SUCCESS);
}
