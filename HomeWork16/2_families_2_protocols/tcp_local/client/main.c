#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#define SOCKPATH "/tmp/local_tcp_socket"
#define BACKLOGS 5
#define LENMSG 64

// AF_LOCAL tcp client


int main(void)
{
    int ret;
    int fd;
    struct sockaddr_un addr;

    char buf[LENMSG];

    fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    strncpy(addr.sun_path, SOCKPATH, sizeof(addr.sun_path));
    addr.sun_family = AF_LOCAL;

    ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0)
    {
        perror("connect");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    printf("enter ur message:\n");
    fgets(buf, sizeof(buf), stdin);

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

    shutdown(fd, SHUT_RDWR);
    close(fd);

    exit(EXIT_SUCCESS);
}
