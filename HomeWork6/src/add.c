#include "header.h"

void add(struct abonent **base, int *index) {
    (*index)++;
    if (*index <= 1)
        *base = (struct abonent*)malloc(sizeof(struct abonent));
    else {
        *base = (struct abonent*)realloc(*base, *index * sizeof(struct abonent));
    }

    int i = *index - 1;
    
    printf("Enter name: ");
    getchar();
    fgets((*base)[i].name, 10, stdin);
    printf("Enter second name: ");
    fgets((*base)[i].second_name, 10, stdin);
    printf("Enter phone number: ");
    fgets((*base)[i].tel, 10, stdin);


}

