#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/in.h>


int main(void)
{
    int                 ret, fd;
    char                buf[2048];
    struct sockaddr_in  addr;
    socklen_t           addr_len = sizeof(addr);
    struct iphdr        *ip_header;
    struct udphdr       *udp_header;

    memset(&addr, 0, addr_len);
    addr.sin_family =       AF_INET;
    addr.sin_port =         htons(0);
    addr.sin_addr.s_addr =  INADDR_ANY;

    fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    ret = bind(fd, (struct sockaddr *)&addr, addr_len);
    if (ret < 0)
    {
        perror("bind");
        close(fd);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        ret = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addr_len);
        if (ret < 0)
        {
            perror("recvfrom");
            continue;
        }

        ip_header = (struct iphdr *)buf;
        udp_header = (struct udphdr *)(buf + sizeof(struct iphdr));
        struct in_addr ip_addr;


        ip_addr.s_addr = ip_header->saddr;
        printf("source ip:port %s:%d\n", inet_ntoa(ip_addr), ntohs(udp_header->source));
        
        ip_addr.s_addr = ip_header->daddr;
        printf("destin ip:port %s:%d\n\n", inet_ntoa(ip_addr), ntohs(udp_header->dest));

    }
    close(fd);

    exit(EXIT_SUCCESS);
}
