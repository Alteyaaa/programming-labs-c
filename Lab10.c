#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define POSITIVE_NEGATIVE
#define REPLACE_WITH_AVERAGE

#include "funcs.inc"


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

    ARRAY_TYPE **arr = malloc(A * sizeof(ARRAY_TYPE *));

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
        ARRAY_TYPE *temp_i = malloc((B + 1) * sizeof(ARRAY_TYPE));
        if (temp_i == NULL) {
            printf("Memory not allocated!");
            for (int k = 0; k < i; k++) {
                free(arr[k]);
            }
            free(arr);
            return 1;
        }
        arr[i] = temp_i;

        LEN(arr[i]) = B;
        for (int j = 0; j < B; j++) {
            ELEM(arr[i], j) = rand() % (max - min + 1) + min;
        }
        temp_i = NULL;
    }

    printf("Unmodified array:\n");
    print_array(arr, A);

    bool error = false;

    for (int i = 0; i < A; i++) {
        switch (i % 4) {
            case 0:
                arr[i] = remove_zeros(arr[i], &error);
                break;
            case 1:
                arr[i] = add_after_negative(arr[i], min, max, &error);
                break;
            case 2:
                rearrange_positive_negative(arr[i]);
                break;
            case 3:
                replace_negative_with_average(arr[i]);
                break;
        }
    }

    if (error) {
        printf("Error: failed to allocate memory\n");
        for (int i = 0; i < A; i++) {
            free(arr[i]);
            arr[i] = NULL;
        }
        free(arr);
        arr = NULL;
        return 1;
    }

    printf("Modified array:\n");
    print_array(arr, A);

    for (int i = 0; i < A; i++) {
        free(arr[i]);
        arr[i] = NULL;
    }
    free(arr);
    arr = NULL;
}
