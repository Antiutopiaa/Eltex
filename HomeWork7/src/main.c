#include <stdio.h>
#include <stdlib.h>
#include "libcalc.h"

void getNum(int *a, int *b) {
    printf("\nEnter first number: ");
    scanf("%d", a);
    printf("Enter second number: ");
    scanf("%d", b);

}

int main(void) {
    int s;
    int a, b;

    while (1){
        printf("######\n");
        printf("1 Addition\n");
        printf("2 Subtraction\n");
        printf("3 Multiplication\n");
        printf("4 Division\n");
        printf("5 Exit\n");
        printf("Enter task number -> ");
        scanf("%d", &s);
        if (s < 5 && s >=1)
            getNum(&a, &b);
        switch (s) {
            case 1:
                add(a, b);
                getchar();
                break;
            case 2:
                sub(a, b);
                getchar();
                break;
            case 3:
                mul(a, b);
                getchar();
                break;
            case 4:
                divL(a, b);
                getchar();
                break;
            case 5:
                printf("Exit...\n");
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
