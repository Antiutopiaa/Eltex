#include <stdio.h>
#include <stdlib.h>

void first() {
    int b;
    int a;
    
    printf("enter origin num(0x0 - 0xFFFFFFFF): ");
    scanf("%x", &a);
    
    printf("enter sub num(0x0 - 0xFF): ");
    scanf("%x", &b);

    char *ptr = (char *)&a;
    ptr++;
    ptr++;

    *ptr = b;
    printf("\n%X\n", a);
}

void second(){
    float x = 5.0;
    printf("x = %f, ", x);
    
    float y = 6.0;
    printf("y = %f\n", y);
    
    float *xp = &y; // there
    
    float *yp = &y;
    printf("Результат: %f\n", *xp + *yp);
}

void third(){
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int *ptr = arr;

    printf("array = { ");

    for (int i = 0; i < 10; i++){
        printf("%d ", *ptr);
        ptr++;
    }
    printf("}\n");
}

char* fourth(){
    char str[100];
    printf("enter str (<100 sim): ");
    getchar();
    fgets(str, 100, stdin);
    int sStr = 0;
    while (str[sStr] != '\0' && str[sStr] != '\n')
        sStr++;

    printf("enter substr (<10): ");
    char substr[10];
    fgets(substr, 10, stdin);
    int sSubStr = 0;
    while (substr[sSubStr] != '\0' && substr[sSubStr] != '\n')
        sSubStr++;

    
    char *ptr = str;
    ptr--;

    for (int i = 0; i < sStr; i++){
         ptr++;
        
        for (int j = 0; j < sSubStr; j++){
            if (*(ptr + j) != substr[j])
                break;
            if (j + 1 == sSubStr){
                printf("\n");   
                return ptr;
            }
        }

    }

    ptr = NULL;
    return ptr;


}

int main() {
    int a;
    char c;
    while (1){
        printf("######\n");
        printf("1 task\n");
        printf("2 task\n");
        printf("3 task\n");
        printf("4 task\n");
        printf("0 - exit\n");
        printf("enter task number -> ");
        scanf("%d", &a);
        switch (a) {
            case 1:
                first();
                break;
            case 2:
                second();
                break;
            case 3:
                third();
                break;
            case 4:
                printf("%p\n", (void *)fourth());
                break;
            case 0:
                printf("Exit...\n");
                return 0;
        }

        printf("Press Enter to continue...");
        getchar();
        getchar();
        //scanf("%c %c");
        //printf("111");
        system("clear");
    }

    
    return 0;
}
