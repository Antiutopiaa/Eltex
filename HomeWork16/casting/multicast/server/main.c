#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

int main(void)
{
    int                 fd, ret;
    char                buf[64];
    int                 multi_len;
    struct sockaddr_in  multi;

    multi_len   = sizeof(multi);

    memset(&multi, 0, multi_len);
    multi.sin_family = AF_INET;
    multi.sin_port = htons(8080);
    inet_aton("224.1.1.80", &multi.sin_addr);

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("sleep 10 seconds\n");
    sleep(10);
    printf("send to %s:%d\n", inet_ntoa(multi.sin_addr), ntohs(multi.sin_port));
    printf("start...\n");


    for (int i = 0; i < 5; i++)
    {
        snprintf(buf, sizeof(buf), "Hello from multi #%d", i);

        sleep(1);
        ret = sendto(fd, (void *)buf, sizeof(buf), 0, (struct sockaddr *)&multi, multi_len);
        if (ret < 0)
        {
            perror("sendto");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}
