#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define MSGLEN 64
#define BACKLOG 5

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

        ret = fork();
        if (ret < 0)
        {
            perror("fork");
            continue;
        } else if (ret == 0)
        {
            data_proc(cfd);
        }

        memset(&client, 0, sizeof(client));
    }

    exit(EXIT_SUCCESS);
}
