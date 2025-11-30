#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <windows.h>

#define MAX_SENTENCE_LEN 2048
#define MAX_WORD_LEN 256
#define MAX_COMMAND_LEN 64

bool is_end_sentence_char(char c) {
    return c == '.' || c == '!' || c == '?';
}

int count_numbers_in_text(const char *text) {
    if (text == NULL) return -1;
    int counter = 0;
    const char *p = text;

    while (*p != '\0') {
        while (*p && (isspace((unsigned char) *p) || *p == ',' || is_end_sentence_char(*p))) p++;
        if (!*p) break;

        const char *s = p;
        while (*p && !(isspace((unsigned char) *p) || *p == ',' || is_end_sentence_char(*p))) p++;
        const char *e = p;

        bool all_digits = true;
        for (const char *q = s; q < e; q++) {
            if (!isdigit((unsigned char) *q)) {
                all_digits = false;
                break;
            }
        }
        if (all_digits && e > s) counter++;
    }
    return counter;
}

bool starts_with_seq_case_insensitive(const char *p, const char *seq, int len_word, int len_seq) {
    if (len_word < len_seq) return false;

    for (int i = 0; i <= len_word - len_seq; i++) {
        bool match = true;
        for (int j = 0; j < len_seq; j++) {
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
    if (!text || !sequence) return NULL;

    const size_t len_text = strlen(text);
    char *result = malloc(len_text + 1);
    if (!result) return NULL;

    char *dst = result;
    const char *src = text;

    while (*src) {
        if (isspace((unsigned char) *src) || *src == ',' || is_end_sentence_char(*src)) {
            *dst++ = *src++;
            continue;
        }

        const char *tok_start = src;
        const char *p = src;
        while (*p && !(isspace((unsigned char) *p) || *p == ',' || is_end_sentence_char(*p))) p++;
        const char *tok_end = p;

        bool is_word = true;
        for (const char *q = tok_start; q < tok_end; q++) {
            if (!isalpha((unsigned char) *q)) {
                is_word = false;
                break;
            }
        }

        int tok_len = tok_end - tok_start;
        bool contains = false;

        if (is_word) {
            contains = starts_with_seq_case_insensitive(tok_start, sequence, tok_len, strlen(sequence));
        }

        if (!contains) {
            while (tok_start < tok_end)
                *dst++ = *tok_start++;
        }

        src = tok_end;
    }

    *dst = '\0';

    char *norm = malloc(strlen(result) + 1);
    if (!norm) {
        free(result);
        return NULL;
    }

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

    char *shrunk = realloc(norm, strlen(norm) + 1);
    if (!shrunk) return norm;

    return shrunk;
}


char **create_arr_by_first_words(const char *text) {
    if (!text) return NULL;

    size_t capacity = 8;
    size_t count = 0;

    char **arr = malloc(capacity * sizeof(char *));
    if (!arr) return NULL;

    const char *p = text;

    while (*p) {
        while (*p && isspace((unsigned char) *p)) p++;
        if (!*p) break;

        const char *sentence_start = p;
        const char *sentence_end = p;

        while (*sentence_end && !is_end_sentence_char(*sentence_end))
            sentence_end++;

        const char *q = sentence_start;
        char *word = NULL;

        while (q < sentence_end) {
            while (q < sentence_end && (isspace((unsigned char) *q) || *q == ',')) q++;
            if (q >= sentence_end) break;

            const char *w0 = q;
            while (q < sentence_end && !(isspace((unsigned char) *q) || *q == ',')) q++;
            const char *w1 = q;

            bool all_alpha = true;
            for (const char *t = w0; t < w1; t++) {
                if (!isalpha((unsigned char) *t)) {
                    all_alpha = false;
                    break;
                }
            }

            if (all_alpha) {
                int len = w1 - w0;

                word = malloc(len + 1);
                if (!word) {
                    for (size_t i = 0; i < count; i++) free(arr[i]);
                    free(arr);
                    return NULL;
                }

                memcpy(word, w0, len);
                word[len] = '\0';
                break;
            }
        }

        if (word) {
            if (count + 1 >= capacity) {
                capacity *= 2;
                char **tmp = realloc(arr, capacity * sizeof(char *));
                if (!tmp) {
                    for (size_t i = 0; i < count; i++) free(arr[i]);
                    free(arr);
                    return NULL;
                }
                arr = tmp;
            }
            arr[count++] = word;
        }

        p = (*sentence_end ? sentence_end + 1 : sentence_end);
    }

    arr[count] = NULL;
    return arr;
}

typedef struct {
    char text[MAX_SENTENCE_LEN];
    char command[MAX_COMMAND_LEN];
    char param[MAX_WORD_LEN];
} ParsedLine;

int parseLine(const char *line, ParsedLine *parsed) {
    strcpy(parsed->param, "");
    const int r = sscanf(line, "\"%[^\"]\" -%s \"%[^\"]\"", parsed->text, parsed->command, parsed->param);
    if (r < 2) return -1;
    return 0;
}

int main(int argc, char *argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    if (argc < 3) {
        printf("Формат: main.exe \"input.txt\" \"output.txt\"\n");
        return 1;
    }

    FILE *fin = fopen(argv[1], "r");
    FILE *fout = fopen(argv[2], "w");

    if (!fin || !fout) {
        printf("Ошибка открытия файлов.\n");
        return 1;
    }

    char line[MAX_SENTENCE_LEN];
    ParsedLine parsed;

    while (fgets(line, sizeof(line), fin)) {
        line[strcspn(line, "\n")] = 0;

        if (parseLine(line, &parsed) != 0) {
            printf("Ошибка разбора: %s\n", line);
            return 1;
        }

        if (strcmp(parsed.command, "info") == 0) {
            int count = count_numbers_in_text(parsed.text);
            fprintf(fout, "%d\n", count);
        } else if (strcmp(parsed.command, "create") == 0) {
            char **arr = create_arr_by_first_words(parsed.text);
            if (!arr) {
                printf("Ошибка выделения памяти.\n");
                return 1;
            }

            for (int i = 0; arr[i]; i++) {
                fprintf(fout, "%s ", arr[i]);
                free(arr[i]);
            }
            fprintf(fout, "\n");
            free(arr);
        } else if (strcmp(parsed.command, "delete") == 0) {
            if (parsed.param[0] == '\0') {
                printf("Для команды delete нужен параметр строки.\n");
                return 1;
            }

            char *res = remove_words_by_sequence(parsed.text, parsed.param);
            if (!res) {
                printf("Ошибка выделения памяти.\n");
                return 1;
            }

            fprintf(fout, "%s\n", res);
            free(res);
        } else {
            printf("Неизвестная команда: %s\n", parsed.command);
            return 1;
        }
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
