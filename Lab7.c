#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

void random_fill(int arr[], size_t size, int min, int max) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % (max - min + 1) + min;
    }
}

bool is_power_of_two(int element, size_t size) {
    if (element <= 1) return false;
    while (element > 1) {
        if (element % 2 != 0) return false;
        element /= 2;
    }
    return true;
}

int fill_powers_of_two(int arr_a[], int arr_b[], size_t size) {
    int j = 0;
    for (int i = 0; i < size; i++) {
        if (is_power_of_two(arr_a[i], size)) {
            arr_b[j] = arr_a[i];
            j++;
        }
    }
    return j;
}

void print_array(int arr[], int size) {
    for (int i = 0; i < size; i ++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int size;
    printf("Input the size of array >> ");
    scanf("%d", &size);

    srand(time(NULL));

    int *arr_a = (int *) malloc(size * sizeof(int));
    if (arr_a == NULL) {
        printf("Memory not allocated!");
        exit(1);
    }
    int min, max;
    printf("Input the minimum and maximum values for random fill >> ");
    scanf("%d %d", &min, &max);
    while (min >= max) {
        printf("The min value must be less than the max value!");
        scanf("%d %d", &min, &max);
    }
    random_fill(arr_a, size, min, max);

    int *arr_b = (int *) malloc(size * sizeof(int));
    if (arr_b == NULL) {
        printf("Memory not allocated!");
        exit(1);
    }
    int size_b = fill_powers_of_two(arr_a, arr_b, size);

    print_array(arr_a, size);

    printf("--------------------------\n");

    print_array(arr_b, size_b);

    free(arr_a);
    free(arr_b);
}
