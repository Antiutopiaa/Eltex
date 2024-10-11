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
    int                 flag = 1;
    int                 fd, ret;
    char                buf[64];
    int                 broad_len;
    struct sockaddr_in  broad;

    broad_len   = sizeof(broad);

    memset(&broad, 0, broad_len);
    broad.sin_family = AF_INET;
    broad.sin_port = htons(8080);
    inet_aton("255.255.255.255", &broad.sin_addr);

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    ret = setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag));
    if (ret < 0)
    {
        perror("setsockopt");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("sleep 10 seconds\n");
    sleep(10);
    printf("start...\n");
    
    for (int i = 0; i < 5; i++)
    {
        snprintf(buf, sizeof(buf), "Hello from broad #%d", i);

        sleep(1);
        ret = sendto(fd, (void *)buf, sizeof(buf), 0, (struct sockaddr *)&broad, broad_len);
        if (ret < 0)
        {
            perror("sendto");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}
