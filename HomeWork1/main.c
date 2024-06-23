#include <stdio.h>

// первое и второе решение работают в обе стороны 
// но варианты подхода разные и во втором принято решение
// укоротить int до char'а для улучшения читаемости вывода

void first(unsigned int a){
    if (a > 1)
        first(a >> 1);
    printf("%d", a & 1);
}

void second(char a){
    for (int i = 7; i >= 0; i--){
        printf("%d", (a >> i) & 1);
    }
}

int third(unsigned int a){
    int count = 0;
    while (a >= 1) {
        if ((a & 1) != 0)
            count++;
        a = a >> 1;
    }
    
    return count;
}

int fourth(unsigned int a, unsigned char b){
    a = a & 0xff00ffff;
    int c = b << 16;
    a = a | c;
    
    return a;   
}

int main(){
    unsigned int a;
    printf("##1##\n");
    printf("enter a positive number(unsignet int): ");
    scanf("%u", &a);
    printf("binary representation of a number %u -> ", a);
    first(a);
    printf("\n\n");
    
    char b;
    printf("##2##\n");
    printf("enter a negative number(-128:127): ");
    scanf("%i", &b);
    printf("binary representation of a number %i -> ", b);
    second(b);
    printf("\n\n");

    unsigned char c;
    printf("##3##\n");
    printf("enter a positive number(unsignet int): ");
    scanf("%u", &c);
    printf("number of units in a number %u -> %d\n\n", c, third(c));

    
    unsigned int d;
    unsigned char e;
    printf("##4##\n");
    printf("enter a positive number (0x0:0xFFFFFFFF)\
 and a number (0x0:0xFF) to replace the third byte with: ");
    scanf("%x %x", &d, &e);
    printf("final value after replacement 0x%X\n", fourth(d, e)); 

    return 0;
}
