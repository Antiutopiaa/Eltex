#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>

int main(void)
{
    int                 fd, ret;
    char                buf[64];
    struct ip_mreq      multi;
    struct sockaddr_in  multi_addr, sender;
    socklen_t           multi_addr_len;

    multi_addr_len = sizeof(multi_addr);

    memset(&multi_addr, 0, multi_addr_len);
    multi_addr.sin_family = AF_INET;
    multi_addr.sin_port = htons(8080);
    inet_aton("224.1.1.80", &multi_addr.sin_addr);

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    ret = bind(fd, (struct sockaddr *)&multi_addr, multi_addr_len);
    if (ret < 0)
    {
        perror("bind");
        close(fd);
        exit(EXIT_FAILURE);
    }

    multi.imr_multiaddr.s_addr = inet_addr("224.1.1.80");
    multi.imr_interface.s_addr = htonl(INADDR_ANY);

    ret = setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multi, sizeof(multi));
    if (ret < 0)
    {
        perror("setsockopt");
        close(fd);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 5; i++)
    {
        ret = recvfrom(fd, (void *)buf, sizeof(buf), 0, (struct sockaddr *)&sender, &multi_addr_len);
        if (ret < 0)
        {
            perror("recvfrom");
            close(fd);
            exit(EXIT_FAILURE);
        }

        buf[sizeof(buf) - 1] = '\0';
        printf("%s\n", buf);

    }

    close(fd);

    exit(EXIT_SUCCESS);
}
