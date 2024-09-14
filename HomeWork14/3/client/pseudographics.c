#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>
#include <stdlib.h>

#include "header.h"

// Pointer to the user window
WINDOW* win_user;

WINDOW* win_chat;
WINDOW* win_input;

extern sem_t *sem;

extern struct shared_data *buffer;

extern char msg4send[SIZE_MSG];

extern unsigned int local_num_users;
extern unsigned int local_curr_msg;
extern char self_name[NAME_SIZE];

// Mutex for synchronizing access to ncurses functions
pthread_mutex_t ncurses_mutex;

void *win_user_f(void *arg)
{
    (void)arg;
    while (1)
    {
        while (1)
        {
            if (local_num_users < buffer->num_users)
            {
                break;
            }
            sleep(1);
        }
        
        pthread_mutex_lock(&ncurses_mutex);
        sem_wait(sem);
        local_num_users = buffer->num_users;
        sem_post(sem);

        werase(win_user);

        for (unsigned int i = 0; i < local_num_users; i++)
        {
            if (strcmp(buffer->users[i], self_name) == 0)
            {
                wattron(win_user, COLOR_PAIR(2));
                mvwprintw(win_user, i + 1, 1, "%s", buffer->users[i]);
                wattroff(win_user, COLOR_PAIR(2));
                continue;
            }
            mvwprintw(win_user, i + 1, 1, "%s", buffer->users[i]);
        }

        box(win_user, 0, 0);
        mvwprintw(win_user, 0, 1, "users (%d/%d)", local_num_users, USERS_MAX);
        wrefresh(win_user);

        pthread_mutex_unlock(&ncurses_mutex);
    }
    return NULL;
}

void win_chat_f(void)
{
    int y, x;
    getmaxyx(win_chat, y, x);

    pthread_mutex_lock(&ncurses_mutex);

    werase(win_chat);

    for (int i = 0, a = y - 2; i < y - 3; i++, a--)
    {
        mvwprintw(win_chat, a, 1, "%s", buffer->messages[local_curr_msg - i]);
        if (local_curr_msg <= (unsigned int)i )
            break;
    }

    box(win_chat, 0, 0);
    mvwprintw(win_chat, 0, (x - 4)/2 - 1, "chat");
    wrefresh(win_chat);

    pthread_mutex_unlock(&ncurses_mutex);
}

void *win_input_f(void *arg)
{
    (void)arg;
    char input_mtext[SIZE_MSG];

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
        
        strcpy(msg4send, input_mtext);
        send_msg();
    }
    return NULL;
}

void pseudographics(void)
{

    initscr();
    cbreak();
    noecho();
    start_color();

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

    win_chat = newwin(maxY - 4, maxX * 0.75, 0, 0);
    win_input = newwin(4, maxX, maxY - 4, 0);
    win_user = newwin(maxY - 4, maxX * 0.25 + 1, 0, maxX * 0.75);

    werase(stdscr);
    refresh();

    return;
}
