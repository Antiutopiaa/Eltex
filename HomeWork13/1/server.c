#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define N 512

struct msgbuf
{
    long mtype; // 1 from server, 2 from client
    char mtext[N];
};

int main(void)
{
    key_t key;
    int msgid;
    struct msgbuf buffer;

    // generate message for send
    strcpy(buffer.mtext, "Hi!\0");
    buffer.mtype = 1;

    key = ftok("keygen", 1);
    if (key == -1)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    
    // send message to client
    if (msgsnd(msgid, &buffer, N, 0) == -1)
    {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    if (msgrcv(msgid, &buffer, N, 2, 0) == -1)
    {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    printf("message from client: %s\n", buffer.mtext);
    
    msgctl(msgid, IPC_RMID, NULL);

    exit(EXIT_SUCCESS);
}
