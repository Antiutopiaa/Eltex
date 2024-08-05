#include "header.h"

int maxX, maxY;

int main() {
    //init srceen
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    noecho();
    if (stdscr == NULL) {
        perror("err: cannot to init screen!");
        endwin();
        exit(1);
    }

    //get size srceen
    getmaxyx(stdscr, maxY, maxX);
    
    //init windows
    WINDOW *winl = newwin(maxY, maxX / 2, 0, 0);
    WINDOW *winr = newwin(maxY, maxX / 2, 0, maxX / 2);
    if (winl == NULL || winr == NULL) {
        perror("err: cannot to create windows!");
        delwin(winl);
        delwin(winr);
    }    

    //inital setting for windows
    keypad(stdscr, TRUE);
    //cbreak();

    wbkgd(winl, COLOR_PAIR(1));
    wbkgd(winr, COLOR_PAIR(1));
    
    //get path to display
    char lPath[1024], rPath[1024];
    getcwd(lPath, sizeof(lPath));
    strcpy(rPath, getenv("HOME"));

    bool cycleState = true; // state of cycle
    int c; // for getch()
    int highlightl = 0, highlightr = 0; // active menu item number
    short actW = 0; // 0 - left, 1 - right windows
    
    while (cycleState) {
        refresh();
        printMenu(winl, lPath, &highlightl, 0);
        printMenu(winr, rPath, &highlightr, 0);
        c = getch();
        switch (c) {
            case '\t':
                actW = (actW + 1) % 2;
                break;

                case KEY_UP:
                if (actW == 0)
                    highlightl--;  
                
                else
                    highlightr--;
                
                break;

            case KEY_DOWN:
                if (actW == 0) 
                    highlightl = (highlightl + 1);
                
                else 
                    highlightr = (highlightr + 1);
                
                break;
            
            case KEY_BACKSPACE:
                if (actW == 0)
                    return_back(lPath);
                else
                 return_back(rPath);
                break;
            
            case 'q':
                delwin(winl);
                delwin(winr);
                endwin();
                cycleState = false;   
                break;
            default:
                if (actW == 0)
                   moveMenu(lPath, &highlightl);
                else
                    moveMenu(rPath, &highlightr); 
                break;
        }   
    }

    return 0;
}
