
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/msg.h>

#define MSG_SIZE 256
#define MAX_USERS 5 
#define FTOK_FILE "keygen"


volatile sig_atomic_t paused = 1;
volatile int global_flag = 1;
char list_users[MAX_USERS][32];
pthread_t wait_new_users_id;
pthread_t common_channel_logic_id;
pthread_t input_t;

pthread_mutex_t global_mutex;

struct msgbuf
{
    long mtype;
    char mname[32];
    char mtext[MSG_SIZE];
};

void sigint_handler(int signum)
{
    global_flag = 0;
    pthread_cancel(wait_new_users_id);
    pthread_cancel(common_channel_logic_id);
    //pthread_cancel(input_t);
    paused = 0;
}

void init_actions(int *msgid)
{
    key_t key = ftok(FTOK_FILE, 1);
    if (key == -1)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    
    *msgid = msgget(key, 0644 | IPC_CREAT);
    if (*msgid == -1)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MAX_USERS; i++)
    {
        strcpy(list_users[i], "");
    }
}

void* wait_new_users(void *arg)
{
    int i;
    int msgid = *(int *)arg;
    struct msgbuf buffer;

    while (global_flag)
    {
        if (msgrcv(msgid, (void *)&buffer, sizeof(struct msgbuf), 2, 0) == -1)
        {
            perror("msgrcv");
        }
        // TODO: check for duplicate name

        // generate & send data to connect
        for (i = 0; i < MAX_USERS; i++) // search for free cells
        {
            if (strcmp(list_users[i], "") == 0)
            {
                // cell found. recording and data exchange
                strcpy(list_users[i], buffer.mname);
                snprintf(buffer.mtext, sizeof(buffer.mtext), "%d", i + 4);
                strcpy(buffer.mname, "server");
                buffer.mtype = 1;

                break;
            }
        }
        msgsnd(msgid, (void *)&buffer, sizeof(struct msgbuf), 0);
        printf("%s: %s\n", buffer.mname, buffer.mtext);
 
        // sending notification
        buffer.mtype = 3;
        strcpy(buffer.mname, "new_user");
        snprintf(buffer.mtext, sizeof(buffer.mtext), "user %s joined the chat", list_users[i]);
        msgsnd(msgid, (void *)&buffer, sizeof(struct msgbuf), 0);
    }

    return NULL;
}

void* common_channel_logic(void *arg)
{
    int msgid = *(int *)arg;
    struct msgbuf buffer;

    while(global_flag)
    {
        if (msgrcv(msgid, (void *)&buffer, sizeof(struct msgbuf), 3, 0) == -1)
        {
            perror("msgrcv");
        }
        printf("rcv ch%ld: %s: %s\n", buffer.mtype, buffer.mname, buffer.mtext);
        
        
        for (int i = 0; i < MAX_USERS; i++)
        {
            buffer.mtype = i + 4;
            if (msgsnd(msgid, (void *)&buffer, sizeof(struct msgbuf), IPC_NOWAIT) == -1)
            {
                perror("msgsnd");
            }
            printf("snd ch%ld: %s: %s\n", buffer.mtype, buffer.mname, buffer.mtext);
        }
    }

    return NULL;
}

void* input(void *arg)
{
    int msgid = *(int *)arg;
    struct msgbuf scan_buffer;
    strcpy(scan_buffer.mname, "server");
    scan_buffer.mtype = 3;

    while (global_flag)
    {
        fgets(scan_buffer.mtext, sizeof(scan_buffer.mtext), stdin);
        if (msgsnd(msgid, (void *)&scan_buffer, sizeof(struct msgbuf), 0) == -1)
        {
            perror("msgsnd");
        }
    }

    return NULL;
}

int main()
{
    signal(SIGINT, sigint_handler);

    int msgid;
    init_actions(&msgid);

    pthread_create(&wait_new_users_id, NULL, wait_new_users, (void *)&msgid);
    pthread_create(&common_channel_logic_id, NULL, common_channel_logic, (void *)&msgid);
    //pthread_create(&input_t, NULL, input, (void *)&msgid);

    while (paused == 1)
    {
        pause();
    }

    msgctl(msgid, IPC_RMID, NULL);

    exit(EXIT_SUCCESS);
}

