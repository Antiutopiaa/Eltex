#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <ncurses.h>

#include "header.h"

#define FTOK_FILE "keygen"
#define MSG_SIZE 256

// User's name, limited to 32 characters
char name[32];

// Channel number for listening to messages
long num_listen_chanel;

// Flag indicating whether the program is paused (volatile to ensure correct behavior with signals)
volatile sig_atomic_t paused = 1;

// Message queue identifier
int msgid;

// Array to store the list of users, with a maximum of 20 users, each having a name of up to 32 characters
char list_of_users[20][32];

// Counter for the number of users currently in the list
int num_users;


/**
 * @brief Signal handler for SIGINT (interrupt signal).
 *
 * This function is called when the process receives the SIGINT signal (usually generated by pressing Ctrl+C).
 * It performs the following actions:
 * 1. Resets the paused state to 0.
 * 2. Ends the ncurses session by calling endwin().
 * 3. Exits the program with a success status.
 *
 * @param signum The signal number (not used in this function).
 * @return void
 */
void sigint_handler(int signum)
{
    paused = 0;
    endwin();
    exit(EXIT_SUCCESS);
}

/**
 * @brief Establishes a connection to the server and retrieves the channel number for listening.
 *
 * This function performs the following steps:
 * 1. Creates a key for accessing the message queue using ftok().
 * 2. Retrieves the message queue identifier using msgget().
 * 3. Prompts the user for their name and stores it in the msgbuf structure.
 * 4. Sends a message of type 2 containing the user's name to the message queue using msgsnd().
 * 5. Receives a message of type 1 from the message queue using msgrcv(), which contains the channel number for listening.
 * 6. Converts the received channel number from a string to an integer and stores it in the variable num_listen_chanel.
 * 7. Outputs the server's name and the channel number for listening to the console.
 *
 * @return void
 */
void connection_to_server()
{
    key_t key = ftok(FTOK_FILE, 1);
    if (key == -1)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    
    msgid = msgget(key, 0);
    if (msgid == -1)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    
    struct msgbuf buffer;

    printf("enter name: ");
    scanf("%s", buffer.mname);
    
    strcpy(name, buffer.mname);
    buffer.mtype = 2;

    if (msgsnd(msgid, (void *)&buffer, sizeof(struct msgbuf), 0) == -1)
    {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    if (msgrcv(msgid, (void *)&buffer, sizeof(struct msgbuf), 1, 0) == -1)
    {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    
    num_listen_chanel = atoi(buffer.mtext);
    printf("%s: chanel to connect: %ld\n", buffer.mname, num_listen_chanel);
    return;   
}


/**
 * @brief Receives a message from the message queue and processes it.
 *
 * This function performs the following steps:
 * 1. Receives a message from the message queue identified by msgid.
 * 2. If the message is of type "new_user", it extracts the new user's name from the message.
 * 3. Updates the global list of users with the new user's name.
 * 4. Calls the function win_user_f() to update the user interface.
 *
 * @return A msgbuf structure containing the received message.
 */
struct msgbuf get_message(void)
{
    struct msgbuf buffer;
    
    if (msgrcv(msgid, (void *)&buffer, sizeof(struct msgbuf), num_listen_chanel, 0) == -1)
    {
        perror("msgrcv");
        kill(getpid(), SIGINT);
    }
        

    // adding all new users to the list
    if (strcmp(buffer.mname, "new_user") == 0)
    {
        char copy_mtext[MSG_SIZE];
        char token[32];
        strcpy(buffer.mname, "server");

        strcpy(copy_mtext, buffer.mtext);
        strcpy(token, strtok(copy_mtext, " "));
        strcpy(token, strtok(NULL, " "));
        
        strcpy(list_of_users[num_users], token);
        num_users += 1;
        win_user_f();
    }
        
    return buffer;
}

/**
 * @brief Sends a message to the message queue.
 *
 * This function prepares a message buffer and sends it to the message queue
 * if the message is not just a newline character. It performs the following steps:
 * 1. Sets the message type to 3.
 * 2. Copies the sender's name into the message buffer.
 * 3. Copies the provided message into the message buffer.
 * 4. If the message is not a newline character, it sends the message to the message queue.
 * 5. If sending the message fails, it prints an error and terminates the process.
 *
 * @param message The message to be sent, limited to MSG_SIZE characters.
 * @return void
 */
 void send_message(char message[MSG_SIZE])
{
    struct msgbuf buffer;

    buffer.mtype = 3;
    strcpy(buffer.mname, name);
    strcpy(buffer.mtext, message);

    if (strcmp(buffer.mtext, "\n") != 0)
    {
        if (msgsnd(msgid, (void *)&buffer, sizeof(struct msgbuf), 0))
        {
            perror("msgsnd");
            kill(getpid(), SIGINT);
        }
    }
}

/**
 * @brief Entry point of the program.
 *
 * This function is the main entry point of the program. It performs the following steps:
 * 1. Initializes the number of users to 0.
 * 2. Sets up a signal handler for SIGINT (interrupt signal) using signal().
 * 3. Establishes a connection to the server by calling connection_to_server().
 * 4. Initializes the pseudographics by calling pseudographics().
 * 5. Enters a loop that pauses the program until a signal is received.
 * 6. Exits the program with a success status.
 *
 * @return int Exit status (0 for success, non-zero for failure).
 */
int main(void)
{
    num_users = 0;
    signal(SIGINT, sigint_handler);

    connection_to_server();
    pseudographics();   
    
    while (paused)
    {
        pause();
    }
    
    exit(EXIT_SUCCESS);
}
