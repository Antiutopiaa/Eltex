#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MSGLEN 64

int main(void)
{
    int                 ret;
    int                 fd;
    char                buf[MSGLEN];
    struct sockaddr_in  server, client;
    socklen_t           server_len = sizeof(server);
    socklen_t           client_len = sizeof(client);

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // create server point
    memset(&server, 0, server_len);
    server.sin_family = AF_INET;
    server.sin_port = htons(7777);
    ret = inet_aton("127.0.0.1", &server.sin_addr);
    if (ret < 0)
    {
        perror("inet_aton");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // create client point
    memset(&client, 0, client_len);

    ret = bind(fd, (struct sockaddr *)&server, server_len);
    if (ret < 0)
    {
        perror("bind");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("server is ready to connect...\n");
    printf("\twait on %s:%d\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));

    ret = recvfrom(fd, (void *)buf, sizeof(buf), 0, (struct sockaddr *)&client, &client_len);
    if (ret < 0)
    {
        perror("recvfrom");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("client say: %s\n", buf);
    printf("client info:\n\taddress: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    buf[0] = '<';
    buf[1] = '3';

    ret = sendto(fd, (void *)buf, sizeof(buf), 0, (struct sockaddr *)&client, client_len);
    if (ret < 0)
    {
        perror("sendto");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
    exit(EXIT_SUCCESS);
}
