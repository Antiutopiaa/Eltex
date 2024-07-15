#include "header.h"

void display(struct abonent *base, int index){
    if (index < 1) {
        perror("Directory is empty.");
        return;
    }

    for (int i = 0; i < index; i++){
        printf("\tabonent #%d\n" , i + 1);
        printf ("name: %s\n", base[i].name);
        printf("second name: %s\n", base[i].second_name);
        printf("phone number: %s\n", base[i].tel);
        printf("\n");
    }
}
