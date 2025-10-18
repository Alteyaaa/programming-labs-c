#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void random_fill(int arr[], size_t size, int min, int max) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % (max - min + 1) + min;
    }
}

bool is_power_of_two(int element) {
    if (element <= 1) return false;
    while (element > 1) {
        if (element % 2 != 0) return false;
        element /= 2;
    }
    return true;
}

size_t count_the_powers_of_two(int arr_a[], size_t size) {
    size_t j = 0;
    for (int i = 0; i < size; i++) {
        if (is_power_of_two(arr_a[i])) {
            j++;
        }
    }
    return j;
}

void fill_powers_of_two(int arr_a[], int arr_b[], size_t size) {
    int j = 0;
    for (int i = 0; i < size; i++) {
        if (is_power_of_two(arr_a[i])) {
            arr_b[j++] = arr_a[i];
        }
    }
}

void reverse(int arr_b[], size_t size) {
    int temp;
    for (int i = 0; i < size / 2; i++) {
        temp = arr_b[i];
        arr_b[i] = arr_b[size - i - 1];
        arr_b[size - i - 1] = temp;
    }
}

void print_array(int arr[], size_t size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
}

int main() {
    srand(time(NULL));

    size_t size_a;
    printf("Input the size of array >> ");
    scanf("%zu", &size_a);

    int *arr_a;
    arr_a = malloc(size_a * sizeof(int));
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

    random_fill(arr_a, size_a, min, max);

    printf("A array\n");
    print_array(arr_a, size_a);

    size_t size_b = count_the_powers_of_two(arr_a, size_a);

    int *arr_b;
    arr_b = malloc(size_b * sizeof(int));
    if (arr_b == NULL) {
        printf("Memory not allocated!");
        free(arr_a);
        exit(1);
    }

    fill_powers_of_two(arr_a, arr_b, size_a);

    printf("\nB array\n");
    print_array(arr_b, size_b);

    reverse(arr_b, size_b);

    printf("\nB array (reverse)\n");
    print_array(arr_b, size_b);

    free(arr_a);
    free(arr_b);

    arr_a = NULL;
    arr_b = NULL;
}
