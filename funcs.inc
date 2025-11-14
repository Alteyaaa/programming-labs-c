#define ARRAY_TYPE int
#define ELEM(array, i) array[i+1]
#define LEN(array) array[0]


void print_array(ARRAY_TYPE **arr_ptr, int rows) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < LEN(arr_ptr[i]); j++) {
            printf("%d ", ELEM(arr_ptr[i], j));
        }
        printf("\n");
    }
}

ARRAY_TYPE *remove_zeros(ARRAY_TYPE *arr, bool *error) {
    int size = LEN(arr); // arr - указатель на подмассив (первый элемент строки), *arr - то же самое, что и arr[0]
    int new_size = 0;
    for (int i = 0; i < size; i++) {
        if (ELEM(arr, i) != 0) {
            ELEM(arr, new_size++) = ELEM(arr, i);
        }
    }

    if (new_size == size) {
        return arr;
    }

    ARRAY_TYPE *temp = realloc(arr, (new_size + 1) * sizeof(ARRAY_TYPE));
    // Теперь arr указывает на освобожденное место в памяти, а данные лежат по указателю temp
    if (temp == NULL) {
        *error = true;
        return arr;
    }
    arr = temp;
    LEN(arr) = new_size;
    return arr;
}

ARRAY_TYPE *add_after_negative(ARRAY_TYPE *arr, int min, int max, bool *error) {
    int size = LEN(arr);
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (ELEM(arr, i) < 0) {
            count++;
        }
    }
    if (count == 0) {
        return arr;
    }

    int new_size = size + count;

    ARRAY_TYPE *temp = realloc(arr, (new_size + 1) * sizeof(ARRAY_TYPE));
    if (temp == NULL) {
        *error = true;
        return arr;
    }

    arr = temp;
    LEN(arr) = new_size;
    temp = NULL;

    int insert_ind = new_size - 1; // ячейка для вставки элемента
    int take_ind = size - 1; // ячейка для взятия элемента

    // Запись в новую область памяти элементов с конца, чтобы избежать перезаписи значений
    while (take_ind >= 0) {
        if (ELEM(arr, take_ind) < 0) {
            ELEM(arr, insert_ind--) = rand() % (max - min + 1) + min;
            ELEM(arr, insert_ind--) = ELEM(arr, take_ind--);
        } else {
            ELEM(arr, insert_ind--) = ELEM(arr, take_ind--);
        }
    }
    return arr;
}

void rearrange_positive_negative(ARRAY_TYPE *arr) {
    int size = LEN(arr);
    ARRAY_TYPE temp[size];

    int index = 0;

#ifdef POSITIVE_NEGATIVE

    for (int i = 0; i < size; i++) {
        if (ELEM(arr, i) >= 0) {
            temp[index++] = ELEM(arr, i);
        }
    }

    for (int i = 0; i < size; i++) {
        if (ELEM(arr, i) < 0) {
            temp[index++] = ELEM(arr, i);
        }
    }

#else

    for (int i = 0; i < size; i++) {
        if (ELEM(arr, i) < 0) {
            temp[index++] = ELEM(arr, i);
        }
    }

    for (int i = 0; i < size; i++) {
        if (ELEM(arr, i) >= 0) {
            temp[index++] = ELEM(arr, i);
        }
    }

#endif

    for (int i = 0; i < size; i++) {
        ELEM(arr, i) = temp[i];
    }
}

void replace_negative_with_average(ARRAY_TYPE *arr) {
    int size = LEN(arr);

    if (size == 0) {
        return;
    }
#ifdef REPLACE_WITH_AVERAGE
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += ELEM(arr, i);
    }
    int average = sum / size;

    for (int i = 0; i < size; i++) {
        if (ELEM(arr, i) < 0) {
            ELEM(arr, i) = average;
        }
    }
#else
    ARRAY_TYPE max_val = ELEM(arr, 0);
    for (int i = 1; i < size; i++) {
        if (ELEM(arr, i) > max_val) {
            max_val = ELEM(arr, i);
        }
    }
    for (int i = 0; i < size; i++) {
        if (ELEM(arr, i) < 0) {
            ELEM(arr, i) = max_val;
        }
    }
#endif
}
