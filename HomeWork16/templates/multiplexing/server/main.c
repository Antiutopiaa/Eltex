#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <poll.h>

#define MSGLEN 64
#define BACKLOG 5
#define MAX_CLIENTS 10

int main(void)
{
    int                 ret;
    int                 tcp_sfd, udp_sfd, cfd;                                      // Server FD, Client FD
    char                buf[MSGLEN];
    struct sockaddr_in  tcp_addr, udp_addr, client_addr;
    socklen_t           tcp_len = sizeof(tcp_addr), udp_len = sizeof(udp_addr), client_len = sizeof(client_addr);
    struct pollfd       fds[2];

    // create tcp socket
    tcp_sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sfd < 0)
    {
        perror("tcp socket");
        exit(EXIT_FAILURE);
    }

    memset(&tcp_addr, 0, tcp_len);
    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_port = htons(2056);
    inet_aton("127.0.0.1", &tcp_addr.sin_addr);

    ret = bind(tcp_sfd, (struct sockaddr *)&tcp_addr, tcp_len);
    if (ret < 0)
    {
        perror("tcp bind");
        exit(EXIT_FAILURE);
    }

    ret = listen(tcp_sfd, BACKLOG);
    if (ret < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }


    // create upd socket
    udp_sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sfd < 0)
    {
        perror("udp socket");
        exit(EXIT_FAILURE);
    }

    memset(&udp_addr, 0, udp_len);
    udp_addr.sin_family = AF_INET;
    udp_addr.sin_port = htons(2020);
    inet_aton("127.0.0.1", &udp_addr.sin_addr);

    ret = bind(udp_sfd, (struct sockaddr *)&udp_addr, udp_len);
    if (ret < 0)
    {
        perror("udp bind");
        exit(EXIT_FAILURE);
    }

    // poll
    memset(fds, 0, sizeof(struct pollfd) * 2);
    fds[0].fd       = tcp_sfd;
    fds[0].events   = POLLIN;

    fds[1].fd       = udp_sfd;
    fds[1].events   = POLLIN;

    printf("listening server pid: %d\n", getpid());
    printf("server is ready to connect...\n");
    printf("\ttcp listening %s:%d\n", inet_ntoa(tcp_addr.sin_addr), htons(tcp_addr.sin_port));
    printf("\tudp listening %s:%d\n", inet_ntoa(udp_addr.sin_addr), ntohs(udp_addr.sin_port));
    
    while (1) // connection cycle
    {
        ret = poll(fds, 2, -1);
        if (ret < 0)
        {
            perror("poll");
            exit(EXIT_FAILURE);
            continue;
        }

        // tcp processing
        if (fds[0].revents & POLLIN)
        {
            cfd = accept(tcp_sfd, NULL, NULL);
            if (cfd < 0)
            {
                perror("accept");
                close(cfd);
                continue;
            }

            ret = recv(cfd, buf, MSGLEN, 0);
            if (ret < 0)
            {
                perror("recv");
                close(cfd);
                continue;
            }

            buf[0] = 't';
            buf[1] = 'c';
            buf[2] = 'p';

            ret = send(cfd, (void *)buf, ret, 0);
            if (ret < 0)
            {
                perror("send");
                close(cfd);
                continue;
            }

            close(cfd);
        }

        // udp processing
        if (fds[1].revents & POLLIN)
        {
            ret = recvfrom(udp_sfd, (void *)buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &client_len);
            if (ret < 0)
            {
                perror("recvfrom");
                continue;
            }

            buf[0] = 'u';
            buf[1] = 'd';
            buf[2] = 'p';

            ret = sendto(udp_sfd, (void *)buf, ret, 0, (struct sockaddr *)&client_addr, client_len);
            if (ret < 0)
            {
                perror("sendto");
                continue;
            }
        }
    
    }

    exit(EXIT_SUCCESS);
}
