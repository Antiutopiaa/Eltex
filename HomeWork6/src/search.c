#include "header.h"

void search(struct abonent *base, int index){
    if (index < 1) {
        perror("Directory is empty.");
        return;
    }


    printf("To search, enter a name: ");
    char nameS[10] = "\0";
    getchar();
    fgets(nameS, 10, stdin);
    for (int i = 0; i <= index; i++){
        if (base[i].name[0] == nameS[0])
            for (int j = 0; j < 10; j++){
                if (base[i].name[j] != nameS[j])
                    break;
                if (base[i].name[j] == '\0' && nameS[j] == '\0'){
                    printf("\tabonent #%d\n" , i + 1);
                    printf("name: %s\n", base[i].name);
                    printf("second name: %s\n", base[i].second_name);
                    printf("phone number: %s\n", base[i].tel);
                    printf("\n");
                    break;
                }
            }
    }
}

