#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

#define INFO_COMMAND "-info"
#define CREATE_COMMAND "-create"
#define DELETE_COMMAND "-delete"

bool is_end_sentence_char(char c) {
    return c == '.' || c == '!' || c == '?';
}

// _
int count_numbers_in_text(const char *text) {
    if (text == NULL) return -1;
    int counter = 0;
    const char *p = text;
    while (*p != '\0') {
        while (*p && (isspace((unsigned char) *p) || *p == ',' || is_end_sentence_char(*p))) ++p;
        if (!*p) break;
        const char *s = p;
        while (*p && !(isspace((unsigned char) *p) || *p == ',' || is_end_sentence_char(*p))) ++p;
        const char *e = p;
        bool all_digits = true;
        for (const char *q = s; q < e; ++q) {
            if (!isdigit((unsigned char) *q)) {
                all_digits = false;
                break;
            }
        }
        if (all_digits && e > s) ++counter;
    }
    return counter;
}

// Сравнение подстроки без учёта регистра
bool starts_with_seq_case_insensitive(const char *p, const char *seq, int len_word, int len_seq) {
    if (len_word < len_seq) return false;
    for (int i = 0; i <= len_word - len_seq; ++i) {
        bool match = true;
        for (int j = 0; j < len_seq; ++j) {
            if (tolower((unsigned char) p[i + j]) != tolower((unsigned char) seq[j])) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}

char *remove_words_by_sequence(const char *text, const char *sequence) {
    if (text == NULL || sequence == NULL) return NULL;
    size_t seq_len = strlen(sequence);

    size_t buf_size = strlen(text) + 1;
    char *result = malloc(buf_size);
    if (result == NULL) return NULL;

    const char *src = text;
    char *dst = result;

    while (*src != '\0') {
        // Если текущий символ - разделитель (пробел, таб, запятая, или .,!,?), копируем и двигаем дальше
        if (isspace((unsigned char) *src) || *src == ',' || is_end_sentence_char(*src)) {
            *dst++ = *src++;
            continue;
        }

        // Иначе, начало токена: находим конец токена
        const char *tok_start = src;
        const char *p = src;
        while (*p != '\0' && !(isspace((unsigned char) *p) || *p == ',' || is_end_sentence_char(*p))) ++p;
        const char *tok_end = p;

        bool is_word = true;
        for (const char *q = tok_start; q < tok_end; ++q) {
            if (!isalpha((unsigned char) *q)) {
                is_word = false;
                break;
            }
        }

        int tok_len = (int) (tok_end - tok_start);
        // Проверяем, содержит ли токен последовательность
        bool contains = false;
        if (is_word) {
            contains = starts_with_seq_case_insensitive(tok_start, sequence, tok_len, (int) seq_len);
        }

        if (!contains) {
            for (const char *q = tok_start; q < tok_end; ++q) {
                *dst++ = *q;
            }
        }
        src = tok_end;
    }

    *dst = '\0';

    // нормализация пробелов
    {
        char *norm = malloc(strlen(result) + 1);
        if (norm) {
            char *w = norm;
            const char *r = result;
            while (*r && isspace((unsigned char) *r)) r++;
            bool sp = false;
            while (*r) {
                if (isspace((unsigned char) *r)) {
                    if (!sp) {
                        *w++ = ' ';
                        sp = true;
                    }
                } else {
                    *w++ = *r;
                    sp = false;
                }
                r++;
            }
            if (w > norm && isspace((unsigned char) w[-1])) w--;
            *w = '\0';
            free(result);
            result = norm;
        }
    }

    size_t final_len = strlen(result);
    char *shrunk = realloc(result, final_len + 1);
    return shrunk ? shrunk : result;
}

char **create_arr_by_first_words(const char *text) {
    if (text == NULL) return NULL;

    size_t capacity = 8;
    size_t count = 0;
    char **arr = malloc(capacity * sizeof(char *));
    if (!arr) return NULL;

    const char *p = text;
    while (*p != '\0') {
        // Пропустить начальные пробелы перед предложением
        while (*p != '\0' && isspace((unsigned char) *p)) ++p;
        if (*p == '\0') break;

        const char *sentence_start = p;
        const char *sentence_end = sentence_start;
        while (*sentence_end != '\0' && !is_end_sentence_char(*sentence_end)) ++sentence_end;

        // Текущий p - начало предполагаемого предложения
        const char *q = sentence_start;
        char *word = NULL;
        while (q < sentence_end) {
            while (q < sentence_end && (isspace((unsigned char) *q) || *q == ',')) ++q;
            if (q >= sentence_end) break;
            const char *t0 = q;
            while (q < sentence_end && !(isspace((unsigned char) *q) || *q == ',')) ++q;
            const char *t1 = q;

            bool all_alpha = true;
            for (const char *k = t0; k < t1; ++k) {
                if (!isalpha((unsigned char) *k)) {
                    all_alpha = false;
                    break;
                }
            }
            if (all_alpha && t1 > t0) {
                int len = (int) (t1 - t0);
                word = malloc(len + 1);
                if (!word) {
                    // очистка при ошибке
                    for (size_t i = 0; i < count; ++i) free(arr[i]);
                    free(arr);
                    return NULL;
                }
                memcpy(word, t0, len);
                word[len] = '\0';
                break;
            }
        }

        if (word) {
            if (count + 1 >= capacity) {
                size_t newcap = capacity * 2;
                char **tmp = realloc(arr, newcap * sizeof(char *));
                if (!tmp) {
                    free(word);
                    for (size_t i = 0; i < count; ++i) free(arr[i]);
                    free(arr);
                    return NULL;
                }
                arr = tmp;
                capacity = newcap;
            }
            arr[count++] = word;
        }

        // Если sentence_end указывает на символ конца предложения, пропускаем его и идём дальше.
        if (*sentence_end == '\0') {
            p = sentence_end;
        } else {
            p = sentence_end + 1; // пропускаем . ! ? и продолжаем
        }
    }

    // NULL-терминатор
    // обеспечить место для одного указателя
    if (count + 1 >= capacity) {
        char **tmp = realloc(arr, (count + 1) * sizeof(char *));
        if (tmp) arr = tmp;
    }
    arr[count] = NULL;
    return arr;
}

void print_array(char **arr) {
    if (arr == NULL) {
        printf("(пусто)\n");
        return;
    }
    for (int i = 0; arr[i] != NULL; ++i) {
        printf("[%d] -> %s\n", i, arr[i]);
    }
}

void free_string_array(char **arr) {
    if (arr == NULL) return;
    for (int i = 0; arr[i] != NULL; ++i) free(arr[i]);
    free(arr);
}

int main(int argc, char *argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    // for (int i = 0; i < argc; ++i) {
    //     printf("[%d] >> %s\n", i, argv[i]);
    // }
    srand(time(NULL));

    if (argc < 3) {
        printf("Использование: %s \"<текст>\" <команда> [параметры...]\n", argv[0]);
        printf(
            "Команды: -info (подсчитать числа), -create (создать массив первых слов), -delete <последовательность>\n");
        return 1;
    }

    const char *text = argv[1];
    const char *command = argv[2];

    if (strcmp(command, INFO_COMMAND) == 0) {
        int count = count_numbers_in_text(text);
        if (count < 0) {
            printf("Ошибка при подсчёте чисел\n");
            return 1;
        }
        printf("Общее количество чисел в тексте: %d\n", count);
    } else if (strcmp(command, CREATE_COMMAND) == 0) {
        printf("Массив первых слов предложений:\n");
        char **arr = create_arr_by_first_words(text);
        if (arr == NULL) {
            printf("Ошибка: не удалось создать массив\n");
            return 1;
        }
        print_array(arr);
        free_string_array(arr);
    } else if (strcmp(command, DELETE_COMMAND) == 0) {
        if (argc < 4) {
            printf("Ошибка: для команды -delete требуется указать последовательность символов\n");
            return 1;
        }
        const char *sequence = argv[3];
        printf("Удаление слов, содержащих последовательность (без учёта регистра): \"%s\"\n", sequence);
        printf("Исходная строка: %s\n", text);
        char *res = remove_words_by_sequence(text, sequence);
        if (res == NULL) {
            printf("Ошибка при удалении\n");
            return 1;
        }
        printf("Новая строка: %s\n", res);
        free(res);
    } else {
        printf("Неизвестная команда: %s\n", command);
        printf("Доступные команды: -info, -create, -delete\n");
        return 1;
    }
}

// "tytcat 234catt werweCAT. $33cat333 caffcat ert345erdfgcat dfcATdfg" -delete cat
//
// "34 $jdf sdfsd sd sdf. dsf 34 345? er$33 fde34 rer##. er33 4ffed 34334 rerrr 43" -create
//
// "234 hjjd dhjhh 23hd 34. 2334 wek 233 33 jjrr3jj? 233$ 2323 23 222" -info
