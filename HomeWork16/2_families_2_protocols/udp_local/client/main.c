#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#define SOCKPATH "/tmp/local_udp_socket"
#define SOCKPATH_CLIENT "/tmp/local_udp_socket_client"

#define LENMSG 64

// AF_LOCAL udp client


int main(void)
{
    int                 ret;
    int                 fd;
    struct sockaddr_un  server, client;
    socklen_t           server_len, client_len;
    char                buf[LENMSG];

    fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    // create client point
    client_len = sizeof(client);
    memset(&client, 0, sizeof(client));
    client.sun_family = AF_LOCAL;
    strncpy(client.sun_path, SOCKPATH_CLIENT, sizeof(client.sun_path));

    // create server point
    server_len = sizeof(server);
    memset(&server, 0, server_len);
    server.sun_family = AF_LOCAL;
    strncpy(server.sun_path, SOCKPATH, sizeof(server.sun_path) - 1);
    
    unlink(SOCKPATH_CLIENT);

    // bind client
    ret = bind(fd, (struct sockaddr *)&client, client_len);
    if (ret < 0)
    {
        perror("bind");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // get message from user
    printf("enter ur message:\n");
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';

    ret = sendto(fd, (void *)buf, strlen(buf), 0, (struct sockaddr *)&server, server_len);
    if (ret < 0)
    {
        perror("sendto");
        close(fd);
        exit(EXIT_FAILURE);
    }

    ret = recvfrom(fd, (void *)buf, sizeof(buf), 0, (struct sockaddr *)&server, &server_len);
    if (ret < 0)
    {
        perror("recvfrom");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("server say: %s\n", buf);
    printf("server info:\n\taddress: %s\n", server.sun_path);

    close(fd);
    unlink(SOCKPATH_CLIENT);

    exit(EXIT_SUCCESS);
}
