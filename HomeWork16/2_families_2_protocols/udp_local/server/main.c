#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>

#define SOCKPATH "/tmp/local_udp_socket"
#define LENMSG 64

// AF_LOCAL udp


int main(void)
{
    int                 ret;
    int                 fd;
    socklen_t           size_client;
    char                buf[LENMSG];
    struct sockaddr_un  server, client;

    fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof(server));
    server.sun_family = AF_LOCAL;
    strncpy(server.sun_path, SOCKPATH, sizeof(server.sun_path));

    memset(&client, 0, sizeof(client));

    unlink(SOCKPATH);

    ret = bind(fd, (struct sockaddr *)&server, sizeof(server));
    if (ret < 0)
    {
        perror("bind");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("server is ready to recive message...\n");

    size_client = sizeof(client);

    ret = recvfrom(fd, (void *)buf, sizeof(buf), 0, (struct sockaddr *)&client, &size_client);
    if (ret < 0)
    {
        perror("recvfrom");
        close(fd);
    }

    printf("client say: %s\n", buf);
    
    if (client.sun_path[0] == '\0')
    {
        perror("client address is empty!");
    } else
    {
        printf("client info:\n\taddress: %s\n", client.sun_path);
    }

    buf[0] = '<';
    buf[1] = '3';

    ret = sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *)&client, size_client);
    if (ret < 0)
    {
        perror("sendto");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
    unlink(SOCKPATH);

    exit(EXIT_SUCCESS);
}
