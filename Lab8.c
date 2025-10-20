#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_array(int** arr, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 1; j < cols+1; j ++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

void remove_all_null_elements(int* arr, int *B) {
    int size = *B;
    int new_size = 0;
    for (int i = 1; i < size+1; i++) {
        if (arr[i] != 0) {
            arr[new_size++] = arr[i];
        }
        *B = new_size;
    }
}

int main() {
    srand(time(NULL));

    int A, B;
    printf("Input the number of rows and cols >> ");
    scanf("%d %d", &A, &B);

    while ((A < 1) || (B < 1)) {
        printf("The number of rows and columns must be greater than 0!\n");
        printf("Input the number of rows and cols >> ");
        scanf("%d %d", &A, &B);
    }

    int **arr = (int **) malloc(A * sizeof(int *));

    if (arr == NULL) {
        printf("Memory not allocated!");
        return 1;
    }

    int min, max;
    printf("Input the minimum and maximum values for random fill >> ");
    scanf("%d %d", &min, &max);
    while (min >= max) {
        printf("The min value must be less than the max value!\n");
        printf("Input the minimum and maximum values for random fill >> ");
        scanf("%d %d", &min, &max);
    }

    for (int i = 0; i < A; i++) {
        int *temp_i = (int *) malloc((B + 1) * sizeof(int));
        if (temp_i == NULL) {
            printf("Memory not allocated!");
            return 1;
        }
        arr[i] = temp_i;

        arr[i][0] = B;
        for (int j = 1; j < B+1; j++) {
            arr[i][j] = rand() % (max - min + 1) + min;
        }
    }

    print_array(arr, A, B);
    printf("\n-----------------------------\n");
    for (int i = 0; i < A; i++) {
        remove_all_null_elements(arr[i], &B);
        print_array(arr, A, B);
        printf("\n-----------------------------\n");
    }





}
