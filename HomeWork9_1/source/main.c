#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void fileW(void) {
    int fd = open("file.txt", O_CREAT | O_WRONLY, 0666);
    if (fd == -1) {
        perror("Cannot open file to write! ");
        exit(-1);
    }
    else {
        char *message = "String from file\n\0";
        ssize_t countW = write(fd, message, (size_t)strlen(message));
        if (countW != strlen(message))
            perror("Incorrect write to file: the length of the line and the number of written characters are not equal!");
    }
    close(fd);
}

void fileR(void) {
   int fd = open("file.txt", O_RDONLY);
    if (fd == -1) {
        perror("Cannot open file to write! ");
        exit(-1);
    }
    else {
        char message[17];
        ssize_t countR = read(fd, message, 17);
        if (countR != 17) {
            perror("Incorrect reading from file: line length and number of characters read are not equal!");
            exit(-1);
        }
        else {
            printf("%.*s", (int)countR, message);
        }
    }
    close(fd);
}

int main() {
    fileW();
    fileR();
}
