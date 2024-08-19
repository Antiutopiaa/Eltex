#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int fildes[2]; // 0 - read, 1 - write
    const int BUF_SIZE = 50;
    char buf[BUF_SIZE];
    int pipe_status;
    char *message = "Hi!";
    
    pipe_status = pipe(fildes); // create pipr & error handling
    if (pipe_status == -1)
    {
        perror("err: pipe\n");
        exit(EXIT_FAILURE);
    }

    printf("fd: [0] = %d, [1] = %d\n", fildes[0],fildes[1]);

    switch (fork())
    {
        case -1: // handling error
            perror("err: fork\n");
            close(fildes[0]);
            close(fildes[1]);
            exit(EXIT_FAILURE);

        case 0: // child
            close(fildes[1]);
            read(fildes[0], buf, BUF_SIZE);
            close(fildes[0]);
            printf("%s\n", buf);
            exit(EXIT_SUCCESS);
        
        default: // parrent
            close(fildes[0]);
            write(fildes[1], message, (ssize_t)strlen(message));
            close(fildes[1]);
            exit(EXIT_SUCCESS);
    }

    exit(EXIT_SUCCESS);
}
