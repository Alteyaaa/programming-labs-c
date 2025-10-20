#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void print_array(int **arr, int rows) {
    for (int i = 0; i < rows; i++) {
        for (int j = 1; j < arr[i][0] + 1; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

void remove_zeros(int **arr_ptr, bool *error) {
    int *arr = *arr_ptr;
    // arr_ptr - адрес указателя на текущий подмассив, *arr_ptr - указатель на подмассив, arr - то же самое, что и *arr_ptr
    int size = arr[0]; // arr - указатель на подмассив (первый элемент строки), *arr - то же самое, что и arr[0]
    int new_size = 0;
    for (int i = 1; i < size + 1; i++) {
        if (arr[i] != 0) {
            arr[++new_size] = arr[i];
        }
    }
    arr[0] = new_size;

    if (new_size == size) {
        return;
    }

    int *temp = realloc(arr, (new_size + 1) * sizeof(int));
    // Теперь arr указывает на освобожденное место в памяти, а данные лежат по указателю temp
    if (temp == NULL) {
        *error = true;
        return;
    }
    *arr_ptr = temp;
    temp = NULL;
}

void add_after_negative(int **arr_ptr, int min, int max, bool *error) {
    int *arr = *arr_ptr;
    int size = arr[0];
    int count = 0;
    for (int i = 1; i < size + 1; i++) {
        if (arr[i] < 0) {
            count++;
        }
    }
    if (count == 0) {
        return;
    }

    int new_size = size + count;

    int *temp = realloc(arr, (new_size + 1) * sizeof(int));
    if (temp == NULL) {
        *error = true;
        return;
    }
    *arr_ptr = temp;
    arr = *arr_ptr;

    arr[0] = new_size;

    int insert_ind = new_size; // ячейка для вставки элемента
    int take_ind = size; // ячейка для взятия элемента

    // Запись в новую область памяти элементов с конца, чтобы избежать перезаписи значений
    while (take_ind > 0) {
        if (arr[take_ind] < 0) {
            arr[insert_ind--] = rand() % (max - min + 1) + min;
            arr[insert_ind--] = arr[take_ind--];
        } else {
            arr[insert_ind--] = arr[take_ind--];
        }
    }
}

void rearrange_positive_negative(int *arr) {
    int size = arr[0];
    int temp[size];

    int index = 0;

    for (int i = 1; i <= size; i++) {
        if (arr[i] >= 0) {
            temp[index++] = arr[i];
        }
    }

    for (int i = 1; i <= size; i++) {
        if (arr[i] < 0) {
            temp[index++] = arr[i];
        }
    }


    for (int i = 1; i < size + 1; i++) {
        arr[i] = temp[i - 1];
    }
}

void replace_negative_with_average(int *arr) {
    int size = arr[0];

    if (size == 0) {
        return;
    }

    int sum = 0;
    for (int i = 1; i < size + 1; i++) {
        sum += arr[i];
    }
    int average = sum / size;

    for (int i = 1; i <= size; i++) {
        if (arr[i] < 0) {
            arr[i] = average;
        }
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

    int **arr = malloc(A * sizeof(int *));

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
        int *temp_i = malloc((B + 1) * sizeof(int));
        if (temp_i == NULL) {
            printf("Memory not allocated!");
            for (int k = 0; k < i; k++) {
                free(arr[k]);
            }
            free(arr);
            return 1;
        }
        arr[i] = temp_i;

        arr[i][0] = B;
        for (int j = 1; j < B + 1; j++) {
            arr[i][j] = rand() % (max - min + 1) + min;
        }
        temp_i = NULL;
    }

    bool error = false;

    for (int i = 0; i < A; i++) {
        switch (i % 4) {
            case 0:
                remove_zeros(&arr[i], &error);
                break;
            case 1:
                add_after_negative(&arr[i], min, max, &error);
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
