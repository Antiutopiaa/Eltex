#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    
    int wstatus;

    pid_t pid1 = fork();

    if (pid1 == -1) {
        perror("fork");
    }
    
    if (pid1 == 0) {
        printf("Child: pid = %d, ppid = %d\n", getpid(), getppid());
    }
    else {
        printf("Parr: pid = %d, ppid = %d\n", getpid(), getppid());
        wait(&wstatus);

        printf("wait status = %d", wstatus);
    }

    //wait(&wstatus);

    exit(EXIT_SUCCESS);
}

