#include "header.h"
#include <string.h>

void return_back(char *path) {
    for (int i = strlen(path) - 1; i >= 0; i--) {
        if (i == 0) {
            path[0] = '/';
            path[1] = 0;
            break;
        }
        if (path[i] == '/') {
            path[i] = 0;
            break;
        }
    }
}

void moveMenu(char *path, int *highlight) {
    struct dirent **namelist;
    int count = scandir(path, &namelist, NULL, alphasort);
        if (count == -1) // error scandir
            return;
        if (strcmp(namelist[*highlight] -> d_name , ".") == 0) // open /.
            return;

        if (strcmp(namelist[*highlight] -> d_name ,"..") == 0) { // open /..
            return_back(path);
        } 
        else if (namelist[*highlight] -> d_type == DT_DIR) { // open dir
            if (strcmp(path, "/") != 0) strcat(path, "/");
            strcat(path, namelist[*highlight] -> d_name);
        }
        *highlight = 2;
}
