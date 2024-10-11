#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>

#define SOCKPATH "/tmp/local_tcp_socket"
#define BACKLOG 5
#define LENMSG 64

// AF_LOCAL tcp

int main(void)
{
    int ret;
    int cfd; // fd for socket()
    int sfd; // fd for accept()
    struct sockaddr_un my_addr, peer_addr;
    socklen_t peer_addr_len;

    char buf[LENMSG];

    sfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (sfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    strncpy(my_addr.sun_path, SOCKPATH, sizeof(my_addr.sun_path));
    my_addr.sun_family = AF_LOCAL;

    ret = bind(sfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_un));
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
    
    printf("server is already to connect...\n");
    cfd = accept(sfd, (struct sockaddr *)&peer_addr,  &peer_addr_len);
    if (cfd < 0)
    {
        perror("accept");
        close(sfd);
        exit(EXIT_FAILURE);
    }

    ret = recv(cfd, (void *)buf, sizeof(buf), 0);
    if (ret < -1)
    {
        perror("recv");
        close(cfd);
        close(sfd);
        exit(EXIT_FAILURE);
    }

    printf("client say: %s\n", buf);

    buf[0] = '<';
    buf[1] = '3';

    ret = send(cfd, (void *)buf, sizeof(buf), 0);
    if (ret < 0)
    {
        perror("send");
        close(cfd);
        close(sfd);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        ret = recv(cfd, (void *)buf, sizeof(buf), 0);
        if (ret < 0)
        {
            perror("recv");
            /* idk what to do in this situation :)*/
        }

        if (ret == 0) // client send shutdown
        {
            break;
        }

    }
    
    close(cfd);
    close(sfd);
    unlink(SOCKPATH);

    exit(EXIT_SUCCESS);
}
