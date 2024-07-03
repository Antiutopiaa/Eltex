#include <stdio.h>
#include <stdlib.h>

struct abonent{
    char name[10];
    char second_name[10];
    char tel[10];
};

void add(struct abonent base[100]){
    int i;
    for (i = 0; i < 100; i++){ //search for free entries
        if (base[i].name[0] == '\0')
            break;
    }

    if (i == 100){
        printf("error: Phone book is full!\n");
        return;
    }

    printf("Enter name: ");
    getchar();
    fgets(base[i].name, 10, stdin);
    printf("Enter second name: ");
    fgets(base[i].second_name, 10, stdin);
    printf("Enter phone number: ");
    fgets(base[i].tel, 10, stdin);
}

void rm(struct abonent base[100]){
    int i;
    printf("To delete an entry, enter the number in the book: ");
    scanf("%d", &i);
    i--;
    base[i].name[0] = '\0';
    base[i].second_name[0] = '\0';
    base[i].tel[0] = '\0';
}

void search(struct abonent base[100]){
    printf("To search, enter a name: ");
    char nameS[10] = "\0";
    getchar();
    fgets(nameS, 10, stdin);
    for (int i = 0; i < 100; i++){
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

void display(struct abonent base[100]){
    for (int i = 99; i >=0; i--){
        printf("\tabonent #%d\n" , i + 1);
        printf ("name: %s\n", base[i].name);
        printf("second name: %s\n", base[i].second_name);
        printf("phone number: %s\n", base[i].tel);
        printf("\n");
    }
}

int main(){
    
    struct abonent base[100];

    for (int i = 0; i < 100; i++){
        base[i].name[0] = '\0';
        base[i].second_name[0] = '\0';
        base[i].tel[0] = '\0'; 
    }


    int a;
    char c;
    while (1){
        printf("######\n");
        printf("1 add\n");
        printf("2 remove\n");
        printf("3 search by name\n");
        printf("4 display all entries\n");
        printf("5 exit\n");
        printf("enter task number -> ");
        scanf("%d", &a);
        switch (a) {
            case 1:
                add(base);
                break;
            case 2:
                rm(base);
                getchar();
                break;
            case 3:
                search(base);
                break;
            case 4:
                display(base);
                getchar();
                break;
            case 5:
                printf("Exit...\n");
                return 0;
            default:
                system("clear");
                printf("error: invalid input!\n");
                getchar();
                break;
        }

        printf("Press Enter to continue...");
        getchar();
        system("clear");
    }

    return 0;
}
