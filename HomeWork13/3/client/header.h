void sigint_handler(int signum);

/**
 * @brief Structure representing a message in the message queue.
 *
 * This structure is used to define the format of messages that are sent and received
 * through the message queue. It contains the following fields:
 *
 * @field mtype Message type, used to identify the type of message.
 * @field mname Name of the sender, limited to 32 characters.
 * @field mtext Message text, limited to 256 characters.
 */
struct msgbuf
{
    long mtype;        // Message type, used to identify the type of message
    char mname[32];    // Name of the sender, limited to 32 characters
    char mtext[256];   // Message text, limited to 256 characters
};


// logic
void connection_to_server(void);
struct msgbuf get_message(void);
void send_message(char message[256]);

// ncurses
void win_user_f(void);
void* win_chat_f(void *arg);
void* win_input_f(void *arg);
void pseudographics(void);
