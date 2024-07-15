#include "header.h"

int main(){
    
    struct abonent *base = NULL;
    int index = 0;

    int a;
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
                add(&base, &index);
                break;
            case 2:
                rm(&base, &index);
                getchar();
                break;
            case 3:
                search(base, index);
                break;
            case 4:
                display(base, index);
                getchar();
                break;
            case 5:
                printf("Exit...\n");
                free(base);
                return 0;
            default:
                system("clear");
                perror("error: invalid input!\n");
                getchar();
                break;
        }

        printf("Press Enter to continue...");
        getchar();
        system("clear");
    }

    return 0;
}
