#include <stdio.h>

#define N 11

void printMatrix(int m[N][N]){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++)
            printf("%d\t", m[i][j]);
        printf("\n");    
    }

}

void first(){
    int matrix[N][N];
    int f = 66000;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++){
    //        f++;
            matrix[i][j] =0; //f;
        }
    
    for (int i = 0; i < N * N; i++){
        f++;
        *(&matrix[0][0] + i*sizeof(int)) = f;
    }
    
    printMatrix(matrix);
}

void sec(){
    int arr[N];
    int inv[N];
    for (int i = 0; i < N; i++){
        scanf("%d", &arr[i]);
        inv[N -i -1] = arr[i]; 
    }
    printf("input -> ");                                        
    for (int i = 0; i < N; i++)
        printf("%d ", arr[i]);
    printf("\noutput -> ");                                        
    for (int i = 0; i < N; i++)
        printf("%d ", inv[i]);

}

void third(){
    int matrix[N][N];

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++){
            if (i + j <= N -1)
                matrix[i][j] = 1;
            else
                matrix [i][j] = 0;
        }
    printMatrix(matrix);
}


void fourth() {
    int m[N][N];
    int i = 0, j = 0, b = 0, c = 0; // i, j - счетчики для циклов, b - смещение, c - число для записи в матрицу

    for (int q = 0; q < N; q++)
        for (int w = 0; w < N; w++)
            m[q][w] = 0;

    while (1) {
        for (; j < N - b -1; j++) {    // ->
            c++;
            m[i][j] = c;
        }

        if ((i == (N - 1) / 2)  &&  (j == (N - 1) / 2) && (c == N * N -1)){ // условие через костыли, но рабочее
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



int main(){
   fourth();

    return 0;
}
