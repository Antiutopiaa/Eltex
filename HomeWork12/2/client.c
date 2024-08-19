#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
    int file_desc;
    char buffer[50];

    file_desc = open("pipe_server", O_RDONLY);
    
    if (file_desc == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    read(file_desc, buffer, sizeof(buffer));
    printf("message from pipe: %s\n", buffer);
    unlink("pipe_server");

    exit(EXIT_SUCCESS);
}
