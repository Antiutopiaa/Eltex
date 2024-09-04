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
    key_t key = ftok("./keygen", 1);
    int msgid = msgget(key, 0666);
    struct msgbuf buffer;
    
    if (msgid == -1)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    if (msgrcv(msgid, &buffer, N, 1, 0) == -1)
    {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    printf("message from server: %s\n", buffer.mtext);
    
    buffer.mtype = 2;
    strcpy(buffer.mtext, "client: Hello!");

    msgsnd(msgid, &buffer, N, 0);

    exit(EXIT_SUCCESS);
}
