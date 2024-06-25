#include <stdio.h>

#define N 5

void printMatrix(int m[N][N]){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++)
            printf("%d ", m[i][j]);
        printf("\n");    
    }

}

void fourth() {
    int m[N][N];
    int i = 0, j = 0, b = 0, c = 0;

    for (int q = 0; q < N; q++)
        for (int w = 0; w < N; w++)
            m[q][w] = 0;

    while (1) {
        for (; j < N - b -1; j++) {    //->
            c++;
            m[i][j] = c;
        }

        if ((i == (N - 1) / 2)  &&  (j == (N - 1) / 2) && (c == N * N -1)){
            m[i][j] = c + 1;
            break;
        }
        
        for (; i < N - b -1; i++) {     // V
            c++;
            m[i][j] = c;
        }

        for (; j > b; j--) {   // <-
            c++;
            m[i][j] = c;
        }
        
        for (; i > 1 + b; i--) {   // ^
            c++;
            m[i][j] = c;
        }
        b++;
    }
    
    printMatrix(m);
    

}

int main() {
    fourth();

    return 0;
}
