#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

char *add_path(char *exe) {
    char *path = "/bin/";
    size_t size_exe = strlen(exe);
    size_t size_path = strlen(path);
    
    char *result = malloc(size_exe + size_path + 1);
    if (result == NULL) {
        perror("memory allocation error");
    }
    
    strcpy(result, path);
    strcat(result, exe);
    strcat(result, "\0");

    return result;
}

char **split_comm(char input[1024], size_t *count) {
    char **words = NULL;

    *count = 0;

    char *token = strtok(input, " ");
    while (token != NULL) {
        words = realloc(words, 128 * (*count + 1));
        if (words == NULL) {
            perror("memory allocation error 1");
            return NULL;
        }
        words[*count] = strdup(token);
        if (words[*count] == NULL) {
            perror("memory allocation error 2");
            return NULL;
        }
        (*count)++;
        token = strtok(NULL, " ");
    }
    
    
    return words;
}

void command_call(char *comm) {
    size_t count = sizeof(comm);
    char **com_arg = split_comm(comm, &count);
    
    if (access(add_path(com_arg[0]), X_OK) == 0) {
        pid_t pid = fork();
    
        if (pid == 0){  
            execlp(com_arg[0], com_arg[0], com_arg[1], com_arg[2], com_arg[3], com_arg[4], com_arg[5], com_arg[6], com_arg[7], NULL);
        }
        else {
            int wstatus;
            waitpid(pid, &wstatus, 0);
        }
    }
    else {
        fprintf(stderr, "%s: command not found\n", com_arg[0]);
    }
}

int main(void) {
    char comm[1024];

    char *username = getenv("USER");
    if (username == NULL) {
        username = getenv("LOGNAME");
    }
    while (1) {
        printf("%s@gash$ ", username);
        fgets(comm, sizeof(comm), stdin);
            
        comm[strcspn(comm, "\n")] = '\0';
        
        if (strcmp(comm, "") != 0){

            if (strcmp(comm, "exit") == 0) {
                break;
            }
            
            else {
                command_call(comm);       
            }
        }
    }

    exit(EXIT_SUCCESS);
}
