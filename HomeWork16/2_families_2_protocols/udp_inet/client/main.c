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
    struct sockaddr_in  server;
    socklen_t           server_len = sizeof(server);

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // create server point
    memset(&server, 0, server_len);
    server.sin_family = AF_INET;
    server.sin_port = htons(2056);
    ret = inet_aton("127.0.0.1", &server.sin_addr);
    if (ret < 0)
    {
        perror("inet_aton");
        close(fd);
        exit(EXIT_FAILURE);
    }

    ret = connect(fd, (struct sockaddr *)&server, server_len);
    if (ret < 0)
    {
        perror("connect");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("enter message: ");
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';

    ret = send(fd, (void *)buf, sizeof(buf), 0);
    if (ret < 0)
    {
        perror("send");
        close(fd);
        exit(EXIT_FAILURE);
    }

    ret = recv(fd, (void *)buf, sizeof(buf), 0);
    if (ret < 0)
    {
        perror("recv");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("server say: %s\n", buf);
    printf("server info:\n\taddress: %s:%d\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));

    close(fd);

    exit(EXIT_SUCCESS);
}
