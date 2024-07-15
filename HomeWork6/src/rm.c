#include "header.h"

void rm(struct abonent **base, int *index){
    if (*index < 1) {
        perror("Directory is empty.");
        return;
    }
    
    int i;
    printf("To delete an entry, enter the number in the book: ");
    scanf("%d", &i);
    i--;
    if (i < 0 || i >= *index){
        perror("Invalid value! Try again...");
        getchar();
        return;
    }
    (*index)--;

    for (; i < *index; i++){
        strcpy((*base)[i].name, (*base)[i + 1].name);
        strcpy((*base)[i].second_name, (*base)[i + 1].second_name);
        strcpy((*base)[i].tel, (*base)[i + 1].tel);
    }
    int new_size = (*index) * sizeof(struct abonent);
    *base = realloc((*base), new_size);
}
