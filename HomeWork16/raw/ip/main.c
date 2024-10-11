#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/in.h>

int main(void)
{
    int                 ret, fd, flag = 1;
    struct sockaddr_in  addr;
    char                packet[64 + sizeof(struct udphdr) + sizeof(struct iphdr)];
    char                packet_rcv[128];
    char *              user_data = packet + sizeof(struct udphdr) + sizeof(struct iphdr);
    socklen_t           addr_len = sizeof(addr);
    struct iphdr *      ip_header = (struct iphdr *)packet;
    struct udphdr *     udp_header = (struct udphdr *)(packet + sizeof(struct iphdr));
    
    


    fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    ret = setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag));

    memset(&addr, 0, addr_len);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7777);
    inet_aton("127.0.0.1", &addr.sin_addr);
    
    memset(packet_rcv, 0, sizeof(packet_rcv));
    memset(packet, 0, sizeof(packet));
    udp_header->source  = htons(9999);
    udp_header->dest    = htons(7777);
    udp_header->len     = htons(sizeof(struct udphdr) + 64);
    udp_header->check   = 0;
    
    ip_header->ihl      = 5;
    ip_header->version  = 4;
    ip_header->tos      = 0;
    ip_header->frag_off = 0;
    ip_header->ttl      = 255;
    ip_header->protocol = IPPROTO_UDP;
    ip_header->saddr    = inet_addr("127.0.0.5");
    ip_header->daddr    = inet_addr("127.0.0.1");

    strncpy(user_data, "hello from client udphdr + iphdr", 64);

    ret = sendto(fd, packet, sizeof(packet), 0, (struct sockaddr *)&addr, addr_len);
    if (ret < 0)
    {
        perror("sendto");
        close(fd);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        ret = recvfrom(fd, packet_rcv, sizeof(packet_rcv), 0, (struct sockaddr *)&addr, &addr_len);
        if (ret < 0)
        {
            perror("recvfrom");
            continue;
        }
        
        ip_header  = (struct iphdr *)(packet_rcv);
        udp_header = (struct udphdr *)(packet_rcv + 20);
        if (udp_header->dest == htons(9999))
            if (ip_header->daddr == inet_addr("127.0.0.5"))
            {
                break;
            }
    }
    
    user_data = packet_rcv + 28;
    
    printf("recive: %s\n", user_data);
    printf("from\n\t%s:%d\n", inet_ntoa(*(struct in_addr *)(&ip_header->saddr)), ntohs(udp_header->source));

    close(fd);

    exit(EXIT_SUCCESS);
}
