#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

void custom_call(char *comm);
char** split_comm(char input[1024]);
char* add_path(char*);

int input_validation(char* comm)
{
    if (access(add_path(comm), X_OK) == 0)
    {
        return 1;
    }
    else 
    {
        fprintf(stderr, "%s: command not found\n", comm);
        return 0;
    }
}

void pipe_construction(char *comm)
{
    char first_comm[1028] = "";
    char second_comm[1028] = "";
    char **first_comm_arg;// = malloc(sizeof(char*) * 1024);
    char **second_comm_arg;// = malloc(sizeof(char*) * 1024);
    
    pid_t child1, child2;    
    int fd[2];
    int wstatus;
    char flag = 1;

    for (int i = 0, j = 0; i < strlen(comm); i++) // split
    {
        if (flag)
        {
            first_comm[i] = comm[i];
            if (comm[i + 1] == '|')
            {
                first_comm[i + 1] = '\0';
                flag = 0;
                i++;
            }
        } 
        else
        {
            second_comm[j] = comm[i];
            j++;
        }
        second_comm[j] = '\0';
    }
    
    first_comm_arg = split_comm(first_comm);
    second_comm_arg = split_comm(second_comm);

    if (input_validation(first_comm_arg[0]) == 0 || input_validation(second_comm_arg[0]) == 0)
    {
        return; 
    }

    if (pipe(fd))
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }


    child1 = fork();

    if (child1 == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (child1 == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
     
        execv(add_path(first_comm_arg[0]), first_comm_arg);
    }
    if (child1)
    {
        waitpid(child1, &wstatus,  0);
    }
    

        child2 = fork();

    if (child2 == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (child2 == 0)
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        
        execv(add_path(second_comm_arg[0]), second_comm_arg);
    }
    
    if (child2)
    {
        close(fd[0]);
        close(fd[1]);
        waitpid(child2, &wstatus, 0);
    }
    
}

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

char **split_comm(char input[1024]) {
    char **words = NULL;

    size_t count = 0;

    char *token = strtok(input, " ");
    while (token != NULL) {
        words = realloc(words, 128 * (count + 2));
        if (words == NULL) {
            perror("memory allocation error 1");
            return NULL;
        }
        words[count] = strdup(token);
        if (words[count] == NULL) {
            perror("memory allocation error 2");
            return NULL;
        }
        count++;
        token = strtok(NULL, " ");
    }
    words[count] = NULL;
    
    return words;
}

void custom_call(char *comm) {
    char **com_arg = split_comm(comm);
    
    if (input_validation(com_arg[0])) {
        pid_t pid = fork();
    
        if (pid == 0){
            execv(add_path(com_arg[0]), com_arg);
        }
        else {
            int wstatus;
            waitpid(pid, &wstatus, 0);
        }
    }
}

int main(void)
{
    char comm[1024];
    char *username = getenv("USER");

    if (username == NULL)
    {
        username = getenv("LOGNAME");
    }
 
    while (1)
    {
        printf("%s@gash$ ", username);
        fgets(comm, sizeof(comm), stdin);
            
        comm[strcspn(comm, "\n")] = '\0';
        
        if (strcmp(comm, "") != 0)
        {
            if (strcmp(comm, "exit") == 0)
            {
                break;
            }
            
            else
            {
                if (!strchr(comm, '|'))
                {
                    custom_call(comm);
                } else
                {
                    pipe_construction(comm);
                }
            }
        }
    }

    exit(EXIT_SUCCESS);
}
