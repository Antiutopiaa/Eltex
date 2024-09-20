#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <signal.h>

int main(void)
{
    sigset_t set_block;

    sigemptyset(&set_block);
    sigaddset(&set_block, SIGINT);

    if (sigprocmask(SIG_BLOCK, &set_block, NULL) == -1)
    {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }
    
    printf("my pid: %d\n", getpid());
    printf("signal INT has been blocked\nfor terminate send SIGUSR1(2)\n");

    while (1)
    {
        sleep(1);
    }

    exit(EXIT_SUCCESS);
}
