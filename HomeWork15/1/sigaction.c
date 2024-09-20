#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <signal.h>

void sig_handler(int sig_num, siginfo_t *info, void *args)
{
    (void)info;
    (void)args;

    printf("handler signal #%d, from %d\n", sig_num, info->si_pid);
}

int main(void)
{
    struct sigaction act;
    sigset_t set;
    int ret;

    printf("my pid: %d\n", getpid());

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);

    act.sa_sigaction = sig_handler;
    act.sa_mask = set;
    act.sa_flags = 0;

    ret = sigaction(SIGUSR1, &act, NULL);
    if (ret == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        sleep(1);
    }

    exit(EXIT_SUCCESS);
}
