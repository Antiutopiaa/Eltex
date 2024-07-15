#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct abonent{
    char name[10];
    char second_name[10];
    char tel[10];
};

void add(struct abonent **base, int *index);
void rm(struct abonent **base, int *index);
void search(struct abonent *base, int index);
void display(struct abonent *base, int index);

#endif
