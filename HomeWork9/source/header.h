#ifndef HEADER_H
#define HEADER_H

#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
//#include <signal.h>
//#include <stdbool.h>


void printMenu(WINDOW *win, char *path, int *highlight, int offset);
//void updateSize(); TODO :)
//void ex(int);      TODO :)
void moveMenu(char *path, int *highlight);
void return_back(char *path);

#endif
