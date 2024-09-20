#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <signal.h>

int main(void)
{
    sigset_t set;
    int sig;
    int ret;
    unsigned int iteration = 0;

    printf("my pid: %d\n", getpid());
    printf("press CTRL+C for exit...\n");

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);

    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1)
    {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        iteration++;
        printf("iteration #%d\n", iteration);
        ret = sigwait(&set, &sig);
        if (ret != 0)
        {
            perror("sigwait");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}
