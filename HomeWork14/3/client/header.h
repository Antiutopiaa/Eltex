#define SEM_NAME "/server_sem"
#define SHM_NAME "/server_shm"

// these value match the values from the server
#define USERS_MAX 10 
#define MSG_MAX 125
#define SIZE_MSG 64
#define NAME_SIZE 16

struct shared_data
{
    unsigned int num_users;
    unsigned int num_msg;
    char users[USERS_MAX][NAME_SIZE];
    char messages[MSG_MAX][SIZE_MSG];
};

void sigint_handler(int signum);

// logic

int connect_to_segment(void);
void init_user(void);

// messages manager
void *recive_msg(void *arg);
void send_msg(void);

// other
void replace_n_on_null(char *string, int size);


// pseudographics
void win_chat_f(void);
void* win_user_f(void *arg);
void* win_input_f(void *arg);

void pseudographics(void);
