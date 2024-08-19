#include <complex.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
    char choice;
    pid_t pid, pid2;
    int wstatus;
    short flag = 1;

while (flag) {
    printf("###\tTasks\n");
    printf("1 - task 1\n");
    printf("2 - task 2\n");
    printf("3 - task 3\n");
    printf("0 - exit\n");
    printf("Enter task: ");

    scanf("%c", &choice);
    
    system("clear");

    switch (choice) {
        case '0':
            exit(EXIT_SUCCESS);
            break;

        case '1':
            pid = fork();

            if (pid == -1) {
                perror("error: fork");
                exit(EXIT_FAILURE);
            } else if (pid) {
                waitpid(pid, &wstatus, 0);
            } else {
                execl("./1/task1", "task1", NULL);
            }
            break;

        case '2':
            pid = fork(); // fork server

            if (pid == -1) {
                perror("error: fork");
                exit(EXIT_FAILURE);
            } else if (pid) {

                pid2 = fork(); // fork client
                
                if (pid2 == -1) 
                {
                    perror("error: fork");
                    exit(EXIT_FAILURE);
                } 
                else if (pid2 == 0)
                {
                    execl("./2/client", "client", NULL);
                }
                waitpid(pid2, &wstatus, 0);
                waitpid(pid, &wstatus, 0);
            } else {
                execl("./2/server", "server", NULL);
            }
            break;

        case '3':
            pid = fork();
            
            if (pid == -1) {
                perror("error: fork");
                exit(EXIT_FAILURE);
            } else if (pid) {
                waitpid(pid, &wstatus, 0);
            } else {
                execl("./3/task3", "task3", NULL);
            }
            break;
    }

    choice = getchar();
    printf("\nPress enter...\n");
    choice = getchar();
    
    system("clear");
}
}
