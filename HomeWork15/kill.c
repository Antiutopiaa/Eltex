#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <signal.h>

int main(void)
{
    pid_t pid;

    printf("Enter pid of process to send SIGUSR1 signal: ");
    scanf("%d", &pid);

    while (1)
    {
        sleep(1);
        if (kill(pid, SIGUSR1) == -1)
        {
            perror("kill");
            exit(EXIT_FAILURE);
        }
    }
    
    exit(EXIT_SUCCESS);
}
