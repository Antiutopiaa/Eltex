#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MSGLEN 64
#define BACKLOG 5

int main(void)
{
    int                 ret;
    int                 sfd, cfd;
    char                buf[MSGLEN];
    struct sockaddr_in  client, server;
    socklen_t           client_len = sizeof(client);
    socklen_t           server_len = sizeof(server);

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    // create server point
    memset(&server, 0, server_len);
    server.sin_family = AF_INET;
    server.sin_port = htons(2056);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);
    
    // clear client point before connect
    memset(&client, 0, client_len);

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

    printf("server is ready to connect...\n");
    printf("\tlistening %s:%d\n", inet_ntoa(server.sin_addr), htons(server.sin_port));

    cfd = accept(sfd, (struct sockaddr *)&client, &client_len);
    if (cfd < 0)
    {
        perror("accept");
        close(sfd);
        exit(EXIT_FAILURE);
    }

    ret = recv(cfd, (void *)buf, sizeof(buf), 0);
    if (ret < 0)
    {
        perror("recv");
        close(sfd);
        close(cfd);
        exit(EXIT_FAILURE);
    }

    printf("client say: %s\n", buf);
    printf("client info\n\taddress: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    
    buf[0] = '<';
    buf[1] = '3';

    ret = send(cfd, (void *)buf, sizeof(buf), 0);
    if (ret < 0)
    {
        perror("send");
        close(sfd);
        close(cfd);
        exit(EXIT_FAILURE);
    }

    close(cfd);
    close(sfd);

    exit(EXIT_SUCCESS);
}
