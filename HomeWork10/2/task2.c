#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    printf("parrent: pid = %d, ppid = %d\n", getpid(), getppid());

    pid_t pid1 = fork();
    pid_t pid2;
    
    pid_t pid3, pid4;
    pid_t pid5;

    int wstatus;

    if (pid1 == 0) {
        printf("\tprocess 1: pid = %d, ppid = %d\n", getpid(), getppid());
        pid3 = fork();
       
        if (pid3 == 0) {
            printf("\t\tprocess 3: pid = %d, ppid = %d\n", getpid(), getppid());
            exit(EXIT_SUCCESS); // exit from 3
        }
        
        else {
            pid4 = fork();
        
            if (pid4 == 0) {
                printf("\t\tprocess 4: pid = %d, ppid = %d\n", getpid(), getppid());
                exit(EXIT_SUCCESS); // exit from 4
            }
        
            waitpid(pid3, &wstatus, 0);
            waitpid(pid4, &wstatus, 0);
            exit(EXIT_SUCCESS); // exit from 1
        }
    
    }
    else {
        pid2 = fork();
        if (pid2 == 0) {
            printf("\tprocess 2: pid = %d, ppid = %d\n", getpid(), getppid());
            pid5 = fork();    
            if (pid5 == 0) {
                printf("\t\tprocess 5: pid = %d, ppid = %d\n", getpid(), getppid());
                exit(EXIT_SUCCESS); // exit from 5
            }
            waitpid(pid5, &wstatus, 0);
            exit(EXIT_SUCCESS); // exit from 2
        }
    }
    waitpid(pid1, &wstatus, 0);
    waitpid(pid2, &wstatus, 0);

    exit(EXIT_SUCCESS);
}

