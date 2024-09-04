#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>
#include <stdlib.h>

#include "header.h"

#define FTOK_FILE "keygen"
#define MSG_SIZE 256

extern long num_listen_chanel;

extern char list_of_users[20][32];
extern int num_users;
extern char name[32];

// Pointer to the user window
WINDOW* win_user;

// Mutex for synchronizing access to ncurses functions
pthread_mutex_t ncurses_mutex;

/**
 * @brief Updates and displays the list of online users in the user window.
 *
 * This function performs the following steps:
 * 1. Locks the mutex to ensure thread safety while updating the user window.
 * 2. Clears the user window.
 * 3. Iterates through the list of users and displays each user.
 * 4. Highlights the user's name if it matches the current user's name.
 * 5. Draws a box around the user window and displays the title "users online".
 * 6. Refreshes the user window to show the updated content.
 *
 * @return void
 */
void win_user_f(void)
{

    pthread_mutex_lock(&ncurses_mutex);

    werase(win_user);

    for (int i = 0; i < num_users; i++)
    {
        if (strcmp(list_of_users[i], name) == 0)
        {
            wattron(win_user, COLOR_PAIR(2));
            mvwprintw(win_user, i + 1, 1, "%s", list_of_users[i]);
            wattroff(win_user, COLOR_PAIR(2));
            continue;
        }
        mvwprintw(win_user, i + 1, 1, "%s", list_of_users[i]);
    }


    box(win_user, 0, 0);
    mvwprintw(win_user, 0, 1, "users online");
    wrefresh(win_user);

    pthread_mutex_unlock(&ncurses_mutex);
}

/**
 * @brief Handles the chat window, receiving and displaying messages.
 *
 * This function runs in a separate thread and performs the following steps:
 * 1. Retrieves the maximum dimensions of the chat window.
 * 2. Enters an infinite loop to continuously receive messages.
 * 3. Locks the mutex for thread safety while updating the chat window.
 * 4. Clears the previous messages and displays new ones.
 * 5. Highlights messages from the server.
 * 6. Draws a box around the chat window and displays the title "chat".
 * 7. Refreshes the chat window to show the updated content.
 *
 * @param arg Pointer to the chat window.
 * @return void* Pointer to NULL (thread return value).
 */
void* win_chat_f(void *arg)
{
    WINDOW* win_chat = (WINDOW *)arg;

    int y, x;
    getmaxyx(win_chat, y, x);
       
    struct msgbuf buffer;

    while (1)
    {
        buffer = get_message();
        pthread_mutex_lock(&ncurses_mutex);

        for (int iy = 1; iy < y - 1; iy++)
        {
            char line_buffer[x + 1];

            mvwinnstr(win_chat, iy, 0, line_buffer, x); 

            mvwaddstr(win_chat, iy - 1, 0, line_buffer);
        }



        for (int ix = 0; ix < x; ix++) {
            mvwaddch(win_chat, y - 2, ix, ' ');
        }

        if (strcmp(buffer.mname, "server") == 0)
        {
            wattron(win_chat, COLOR_PAIR(2));
            mvwprintw(win_chat, y - 2, 1, "%s: %s", buffer.mname, buffer.mtext);
            wattroff(win_chat, COLOR_PAIR(2));
        } else
        {
            mvwprintw(win_chat, y - 2, 1, "%s: %s", buffer.mname, buffer.mtext);
        }
        
        box(win_chat, 0, 0);
        mvwprintw(win_chat, 0, (x - 4)/2 - 1, "chat");
        wrefresh(win_chat);

        pthread_mutex_unlock(&ncurses_mutex);
    }
    return NULL;
}

/**
 * @brief Handles user input from the input window.
 *
 * This function runs in a separate thread and performs the following steps:
 * 1. Prepares the input window for user input.
 * 2. Locks the mutex for thread safety while updating the input window.
 * 3. Prompts the user for input and waits for their response.
 * 4. Checks for the exit command ("!q") to terminate the process.
 * 5. Sends the input message to the message queue.
 *
 * @param arg Pointer to the input window.
 * @return void* Pointer to NULL (thread return value).
 */
void* win_input_f(void *arg)
{
    WINDOW* win_input = (WINDOW *)arg;
    
    char input_mtext[MSG_SIZE];
    char copy_mtext[MSG_SIZE];

    while (1)
    {
        // подготовка поля для ввода
        pthread_mutex_lock(&ncurses_mutex);

        werase(win_input);
        box(win_input, 0, 0);

        mvwprintw(win_input, 1, 1, "->");
        wrefresh(win_input);

        pthread_mutex_unlock(&ncurses_mutex);

        echo();
        wgetstr(win_input, input_mtext);
        noecho();

        if (strcmp(input_mtext, "!q") == 0)
        {
            kill(getpid(), SIGINT);
            pthread_exit(NULL);
        }
        strcpy(copy_mtext, input_mtext);
        send_message(copy_mtext);
    
    }
    return NULL;
}

/**
 * @brief Initializes the ncurses environment and creates windows for chat and input.
 *
 * This function sets up the ncurses library for graphical user interface (GUI) in the terminal.
 * It performs the following steps:
 * 1. Initializes the ncurses screen with initscr().
 * 2. Enables cbreak mode to allow immediate input without waiting for Enter.
 * 3. Disables echoing of typed characters.
 * 4. Initializes color support with start_color().
 * 5. Initializes color pairs for different message types.
 * 6. Checks the maximum screen size and exits if it is too small.
 * 7. Creates windows for chat, input, and user list.
 * 8. Starts separate threads for handling chat and input.
 *
 * @return void
 */
void pseudographics(void)
{

    initscr();
    cbreak();
    noecho();
    start_color();

    pthread_t win_chat_id, win_input_id;


    int maxY, maxX;

    init_pair(1, COLOR_WHITE, COLOR_BLACK); // default mode
    init_pair(2, COLOR_GREEN, COLOR_BLACK); // mode for server's msg or selfname online

    attron(COLOR_PAIR(1));

    getmaxyx(stdscr, maxY, maxX);

    if (maxY < 30 || maxX < 53)
    {
        endwin();
        printf("error: screen too small. minimum size (53;30). ur (%d;%d)", maxY, maxX);
        kill(getpid(), SIGINT);
    }

    WINDOW *win_chat = newwin(maxY - 4, maxX * 0.75, 0, 0);
    WINDOW *win_input = newwin(4, maxX, maxY - 4, 0);
    win_user = newwin(maxY - 4, maxX * 0.25 + 1, 0, maxX * 0.75);

    werase(stdscr);
    refresh();

    pthread_create(&win_chat_id, NULL, win_chat_f, (void *)win_chat);
    pthread_create(&win_input_id, NULL, win_input_f, (void *)win_input);

}
