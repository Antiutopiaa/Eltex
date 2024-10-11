#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define MSGLEN 64

int main(void)
{
    int                 ret;
    int                 fd;
    char                buf[MSGLEN];
    socklen_t           server_len;
    struct sockaddr_in  server;

    server_len = sizeof(server);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, server_len);
    server.sin_family = AF_INET;
    server.sin_port = htons(2056);
    inet_aton("127.0.0.1", &server.sin_addr);

    ret = connect(fd, (struct sockaddr *)&server, server_len);
    if (ret < 0)
    {
        perror("connect");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("server connected (%s:%d)\n\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));

    while (1)
    {
        printf("enter ur message: \n");
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = '\0';

        ret = strncmp(buf, "!quit", strlen("!quit"));
        if (ret == 0)
        {
            printf("exit...\n");
            close(fd);
            exit(EXIT_SUCCESS);
        }

        ret = send(fd, (void *)buf, sizeof(buf), 0);
        if (ret < 0)
        {
            perror("send");
            continue;
        }

        ret = recv(fd, (void *)buf, sizeof(buf), 0);
        if (ret < 0)
        {
            perror("recv");
            continue;
        }
        else if (ret == 0)
        {
            printf("connection closed\nexit...\n");
            close(fd);
            exit(EXIT_SUCCESS);
        }

        printf("server responded: %s\n", buf);

    }
    exit(EXIT_SUCCESS);
}
