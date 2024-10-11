#include <linux/if_ether.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/in.h>

#define DEST_IP "192.168.0.11"
#define DEST_MAC "08:8f:c3:41:ce:60"

uint16_t checksum(uint16_t *buffer, int length)
{
    unsigned long sum = 0;
    while (length > 1) {
        sum += *buffer++;
        length -= 2;
    }
    if (length > 0) {
        sum += *(unsigned char *)buffer;
    }
    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }
    return (uint16_t)(~sum);
}

int main(void)
{
    int                 ret, fd;
    struct sockaddr_ll  addr;
    char                packet[64 + sizeof(struct udphdr) + sizeof(struct iphdr) + sizeof(struct ethhdr)];
    char                packet_rcv[128];
    socklen_t           addr_len = sizeof(addr);
    struct ethhdr *     eth_header  = (struct ethhdr *)packet;
    struct iphdr *      ip_header   = (struct iphdr *)(packet + sizeof(struct ethhdr));
    struct udphdr *     udp_header  = (struct udphdr *)(packet + sizeof(struct ethhdr) + sizeof(struct iphdr));
    char *              user_data   = (packet + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr));

    fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, addr_len);
    addr.sll_family     = AF_PACKET;
    addr.sll_ifindex    = if_nametoindex("enp3s0");
    addr.sll_halen      = 6;
    sscanf(DEST_MAC, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
           &addr.sll_addr[0], &addr.sll_addr[1],
           &addr.sll_addr[2], &addr.sll_addr[3],
           &addr.sll_addr[4], &addr.sll_addr[5]);

    memset(packet_rcv, 0, sizeof(packet_rcv));
    memset(packet, 0, sizeof(packet));
    udp_header->source  = htons(9999);
    udp_header->dest    = htons(7777);
    udp_header->len     = htons(sizeof(struct udphdr) + 64);
    udp_header->check   = 0;

    ip_header->id       = htons(1478);
    ip_header->ihl      = 5;
    ip_header->tos      = 0;
    ip_header->ttl      = 255;
    ip_header->check    = 0;
    ip_header->saddr    = inet_addr("192.168.0.12");
    ip_header->daddr    = inet_addr(DEST_IP);
    ip_header->version  = 4;
    ip_header->tot_len  = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + 64);
    ip_header->protocol = IPPROTO_UDP;
    ip_header->frag_off = 0;

    memcpy(eth_header->h_dest, addr.sll_addr, 6);
    eth_header->h_proto = htons(ETH_P_IP);

    strncpy(user_data, "hello from client ethhdr", 64);

    udp_header->check = checksum((uint16_t *)udp_header, sizeof(struct udphdr) + 64);
    ip_header->check = checksum((uint16_t *)ip_header, sizeof(struct iphdr) + sizeof(struct udphdr) + 64);

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

        eth_header  = (struct ethhdr *)packet_rcv;
        ip_header   = (struct iphdr *)(packet_rcv + sizeof(struct ethhdr));
        udp_header  = (struct udphdr *)(packet_rcv + sizeof(struct ethhdr) + sizeof(struct iphdr));
        if (udp_header->dest == htons(9999) && ip_header->daddr == inet_addr("192.168.0.12"))
        {
            break;
        }
    }

    printf("recive: %s\n", user_data);

    close(fd);

    exit(EXIT_SUCCESS);
}
