#include "header.h"

extern int maxX, maxY;

void printMenu(WINDOW *win, char *path, int *highlight, int offset) {
    
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    werase(win);


    //box(win, 0, 0);
    //mvwprintw(win, 0, 1, "%s", path);

    struct dirent **namelist;
    int count = scandir(path, &namelist, NULL, alphasort);

    // highlight processing
    
    for (int i = 0 + offset; i < maxY - 2 + offset; i++) {
        
        if (*highlight < 0) // scroll up handling
            *highlight = count - 1;

        if (count > maxY && *highlight - offset >= maxY - 2 && *highlight <= count) // scroll down handling
            offset++;

        if (*highlight > count) {
            offset = 0;
            *highlight = (*highlight - 1) % count;
        }

        if (i < count) { //Limitation for output in menu
            if (namelist[i] -> d_type == DT_DIR) { //checking for directory type
                wattron(win, COLOR_PAIR(2));
            }
            if (i == *highlight) {
                wattron(win, A_REVERSE);
            }
            mvwprintw(win, i + 1 - offset, 1, "%s", namelist[i] -> d_name);
        }
        wattroff(win, COLOR_PAIR(2));
        wattroff(win, A_REVERSE);
    }
    box(win, 0, 0);
    mvwprintw(win, 0, 1, "%s", path);

    wrefresh(win);
}
