#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{

    int file_desc;
    char *message = "Hi!";

    if (mkfifo("pipe_server", 0644) == -1)
    {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    file_desc = open("pipe_server", O_WRONLY);
    write(file_desc, message, strlen(message));
    close(file_desc);

    exit(EXIT_SUCCESS);
}
