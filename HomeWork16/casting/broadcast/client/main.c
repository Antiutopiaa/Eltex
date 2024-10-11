#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/in.h>

int main(void)
{
    int                 ret, fd;
    char                buf[64];
    struct sockaddr_in  server, srv4print;
    socklen_t           server_len = sizeof(server);

    memset(&server, 0, server_len);
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    ret = bind(fd, (struct sockaddr *)&server, server_len);
    if (ret < 0)
    {
        perror("bind");
        close(fd);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 5; i++)
    {
        ret = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&srv4print, &server_len);
        if (ret < 0)
        {
            perror("recvfrom");
            close(fd);
            exit(EXIT_FAILURE);
        }

        buf[sizeof(buf) - 1] = '\0';
        printf("%s\n", buf);
    }

    exit(EXIT_SUCCESS);
}
