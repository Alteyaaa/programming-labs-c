#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <windows.h>

#define MAX_CIRCLES 50
#define MAX_FILENAME_LEN 128

typedef struct {
    float x;
    float y;
    float radius;
} Circle;

Circle createCircle(const float x, const float y, const float r) {
    Circle circle;
    circle.x = x;
    circle.y = y;
    circle.radius = r;
    return circle;
}

bool inCircle(const Circle *circle, const float x, const float y) {
    float x0 = circle->x;
    float y0 = circle->y;
    float r = circle->radius;
    return (pow(x - x0, 2) + pow(y - y0, 2)) <= pow(r, 2);
}

bool intersectionCircles(const Circle *circle1, const Circle *circle2) {
    float dx = circle1->x - circle2->x;
    float dy = circle1->y - circle2->y;
    float distanceBetweenCenters = sqrtf(dx * dx + dy * dy);
    float sumRadiuses = circle1->radius + circle2->radius;
    return distanceBetweenCenters <= sumRadiuses;
}

bool intersectionCircleRectangle(const Circle *circle, const float x, const float y, const float width,
                                 const float height) {
    float closestX = fmaxf(x, fminf(circle->x, x + width));
    float closestY = fmaxf(y, fminf(circle->y, y + height));
    float distanceX = circle->x - closestX;
    float distanceY = circle->y - closestY;
    return (distanceX * distanceX + distanceY * distanceY) <= (circle->radius * circle->radius);
}

float squareIntersectionCircles(const Circle *circle1, const Circle *circle2) {
    float dx = circle1->x - circle2->x;
    float dy = circle1->y - circle2->y;
    float d = sqrtf(dx * dx + dy * dy);
    float r1 = circle1->radius;
    float r2 = circle2->radius;

    if (d >= r1 + r2) return 0.0f;
    if (d <= fabsf(r1 - r2)) {
        float rmin = fminf(r1, r2);
        return M_PI * rmin * rmin;
    }

    float term1 = (r1 * r1 + d * d - r2 * r2) / (2 * r1 * d);
    float term2 = (r2 * r2 + d * d - r1 * r1) / (2 * r2 * d);
    term1 = fmaxf(-1.0f, fminf(1.0f, term1));
    term2 = fmaxf(-1.0f, fminf(1.0f, term2));

    float angle1 = 2 * acosf(term1);
    float angle2 = 2 * acosf(term2);
    float area1 = 0.5f * r1 * r1 * (angle1 - sinf(angle1));
    float area2 = 0.5f * r2 * r2 * (angle2 - sinf(angle2));
    return area1 + area2;
}

Circle circumscribedCircle(const float x1, const float y1,
                           const float x2, const float y2,
                           const float x3, const float y3, bool *error) {
    float A = x2 - x1;
    float B = y2 - y1;
    float C = x3 - x1;
    float D = y3 - y1;
    float E = A * (x1 + x2) + B * (y1 + y2);
    float F = C * (x1 + x3) + D * (y1 + y3);
    float G = 2 * (A * (y3 - y2) - B * (x3 - x2));
    if (fabs(G) < 1e-6) {
        *error = true;
        return createCircle(0, 0, 0);
    }
    float cx = (D * E - B * F) / G;
    float cy = (A * F - C * E) / G;
    float r = hypot(cx - x1, cy - y1);
    *error = false;
    return createCircle(cx, cy, r);
}

Circle minimalEnclosingCircle(Circle *circles, int n) {
    if (n <= 0) return createCircle(0, 0, 0);
    float minX = circles[0].x - circles[0].radius;
    float maxX = circles[0].x + circles[0].radius;
    float minY = circles[0].y - circles[0].radius;
    float maxY = circles[0].y + circles[0].radius;
    for (int i = 1; i < n; i++) {
        if (circles[i].x - circles[i].radius < minX) minX = circles[i].x - circles[i].radius;
        if (circles[i].x + circles[i].radius > maxX) maxX = circles[i].x + circles[i].radius;
        if (circles[i].y - circles[i].radius < minY) minY = circles[i].y - circles[i].radius;
        if (circles[i].y + circles[i].radius > maxY) maxY = circles[i].y + circles[i].radius;
    }
    float cx = (minX + maxX) / 2;
    float cy = (minY + maxY) / 2;
    float maxR = 0;
    for (int i = 0; i < n; i++) {
        float dist = hypot(cx - circles[i].x, cy - circles[i].y) + circles[i].radius;
        if (dist > maxR) maxR = dist;
    }
    return createCircle(cx, cy, maxR);
}

// Сохранение в файл
void save(char *format, FILE *outputFile, ...) {
    int d;
    float f;
    Circle c;
    bool b;
    va_list args;
    va_start(args, outputFile);

    for (char *p = format; *p; p++) {
        switch (*p) {
            case 'd': // int
                d = va_arg(args, int);
                fwrite(&d, sizeof(int), 1, outputFile);
                break;
            case 'f': // float
                f = (float) va_arg(args, double);
                fwrite(&f, sizeof(float), 1, outputFile);
                break;
            case 'c': // Circle
                c = va_arg(args, Circle);
                fwrite(&c, sizeof(Circle), 1, outputFile);
                break;
            case 'b': // bool
                b = (bool) va_arg(args, int);
                fwrite(&b, sizeof(bool), 1, outputFile);
                break;
        }
    }
    va_end(args);
}

// Загрузка из файла
void load(char *format, FILE *inputFile, ...) {
    int *d;
    float *f;
    Circle *c;
    bool *b;
    va_list args;
    va_start(args, inputFile);

    for (char *p = format; *p; p++) {
        switch (*p) {
            case 'd': // int
                d = va_arg(args, int*);
                fread(d, sizeof(int), 1, inputFile);
                break;
            case 'f': // float
                f = va_arg(args, float*);
                fread(f, sizeof(float), 1, inputFile);
                break;
            case 'c': // Circle
                c = va_arg(args, Circle*);
                fread(c, sizeof(Circle), 1, inputFile);
                break;
            case 'b': // bool
                b = va_arg(args, bool*);
                fread(b, sizeof(bool), 1, inputFile);
                break;
        }
    }
    va_end(args);
}

// Режим 1: Запись операций в файл
void saveOperations(FILE *outputFile) {
    int choice;
    Circle c1, c2;
    float x, y, r, width, height;
    float x1, y1, x2, y2, x3, y3;
    int n;

    printf("\n=== РЕЖИМ ЗАПИСИ ОПЕРАЦИЙ ===\n");
    printf("1. Проверить точку в круге\n");
    printf("2. Пересечение двух кругов\n");
    printf("3. Пересечение круга с прямоугольником\n");
    printf("4. Площадь пересечения двух кругов\n");
    printf("5. Описанный круг вокруг треугольника\n");
    printf("6. Минимальный охватывающий круг\n");
    printf("7. Выход\n");

    while (1) {
        printf("\nВыберите операцию: ");
        scanf("%d", &choice);

        if (choice == 7) {
            fclose(outputFile);
            printf("Запись завершена.\n");
            return;
        }

        switch (choice) {
            case 1:
                printf("Введите круг (x y r): ");
                scanf("%f %f %f", &x, &y, &r);
                c1 = createCircle(x, y, r);
                printf("Введите точку (x y): ");
                scanf("%f %f", &x, &y);
                save("dcff", outputFile, choice, c1, x, y);
                break;

            case 2:
                printf("Введите первый круг (x y r): ");
                scanf("%f %f %f", &x, &y, &r);
                c1 = createCircle(x, y, r);
                printf("Введите второй круг (x y r): ");
                scanf("%f %f %f", &x, &y, &r);
                c2 = createCircle(x, y, r);
                save("dcc", outputFile, choice, c1, c2);
                break;

            case 3:
                printf("Введите круг (x y r): ");
                scanf("%f %f %f", &x, &y, &r);
                c1 = createCircle(x, y, r);
                printf("Введите прямоугольник (x y width height): ");
                scanf("%f %f %f %f", &x, &y, &width, &height);
                save("dcffff", outputFile, choice, c1, x, y, width, height);
                break;

            case 4:
                printf("Введите первый круг (x y r): ");
                scanf("%f %f %f", &x, &y, &r);
                c1 = createCircle(x, y, r);
                printf("Введите второй круг (x y r): ");
                scanf("%f %f %f", &x, &y, &r);
                c2 = createCircle(x, y, r);
                save("dcc", outputFile, choice, c1, c2);
                break;

            case 5:
                printf("Введите треугольник (x1 y1 x2 y2 x3 y3): ");
                scanf("%f %f %f %f %f %f", &x1, &y1, &x2, &y2, &x3, &y3);
                save("dffffff", outputFile, choice, x1, y1, x2, y2, x3, y3);
                break;

            case 6:
                printf("Введите количество кругов: ");
                scanf("%d", &n);
                save("dd", outputFile, choice, n);
                for (int i = 0; i < n; i++) {
                    printf("Круг %d (x y r): ", i + 1);
                    scanf("%f %f %f", &x, &y, &r);
                    c1 = createCircle(x, y, r);
                    save("c", outputFile, c1);
                }
                break;

            default:
                printf("Неверный выбор!\n");
        }
        printf("Операция записана.\n");
    }
}

// Режим 2: Обработка операций
void executeOperations(FILE *inputFile, FILE *outputFile) {
    int choice;
    Circle c1, c2, resultCircle;
    float x, y, width, height;
    float x1, y1, x2, y2, x3, y3;
    int n;
    bool error, resultBool;
    float resultFloat;

    printf("\n=== ОБРАБОТКА ОПЕРАЦИЙ ===\n");

    while (fread(&choice, sizeof(int), 1, inputFile) == 1) {
        // читаем, пока есть что читать
        save("d", outputFile, choice);

        switch (choice) {
            case 1:
                load("cff", inputFile, &c1, &x, &y);
                resultBool = inCircle(&c1, x, y);
                save("cffb", outputFile, c1, x, y, resultBool);
                break;

            case 2:
                load("cc", inputFile, &c1, &c2);
                resultBool = intersectionCircles(&c1, &c2);
                save("ccb", outputFile, c1, c2, resultBool);
                break;

            case 3:
                load("cffff", inputFile, &c1, &x, &y, &width, &height);
                resultBool = intersectionCircleRectangle(&c1, x, y, width, height);
                save("cffffb", outputFile, c1, x, y, width, height, resultBool);
                break;

            case 4:
                load("cc", inputFile, &c1, &c2);
                resultFloat = squareIntersectionCircles(&c1, &c2);
                save("ccf", outputFile, c1, c2, resultFloat);
                break;

            case 5:
                load("ffffff", inputFile, &x1, &y1, &x2, &y2, &x3, &y3);
                resultCircle = circumscribedCircle(x1, y1, x2, y2, x3, y3, &error);
                save("ffffffcb", outputFile, x1, y1, x2, y2, x3, y3, resultCircle, error);
                break;

            case 6:
                load("d", inputFile, &n);
                save("d", outputFile, n);
                Circle circles[MAX_CIRCLES];
                for (int i = 0; i < n; i++) {
                    load("c", inputFile, &circles[i]);
                    save("c", outputFile, circles[i]);
                }
                resultCircle = minimalEnclosingCircle(circles, n);
                save("c", outputFile, resultCircle);
                break;
        }
    }

    fclose(inputFile);
    fclose(outputFile);
    printf("Обработка завершена.\n");
}

// Режим 3: Вывод результатов
void printOperations(FILE *inputFile) {
    int choice;
    Circle c1, c2, resultCircle;
    float x, y, width, height, resultFloat;
    float x1, y1, x2, y2, x3, y3;
    int n;
    bool resultBool, error;

    printf("\n=== РЕЗУЛЬТАТЫ ОПЕРАЦИЙ ===\n");

    while (fread(&choice, sizeof(int), 1, inputFile) == 1) {
        printf("\nОперация %d: ", choice);

        switch (choice) {
            case 1:
                load("cffb", inputFile, &c1, &x, &y, &resultBool);
                printf("Точка в круге\n");
                printf("Круг: (%.2f, %.2f) радиус %.2f\n", c1.x, c1.y, c1.radius);
                printf("Точка: (%.2f, %.2f)\n", x, y);
                printf("Результат: %s\n", resultBool ? "точка в круге" : "точка вне круга");
                break;

            case 2:
                load("ccb", inputFile, &c1, &c2, &resultBool);
                printf("Пересечение кругов\n");
                printf("Круг 1: (%.2f, %.2f) радиус %.2f\n", c1.x, c1.y, c1.radius);
                printf("Круг 2: (%.2f, %.2f) радиус %.2f\n", c2.x, c2.y, c2.radius);
                printf("Результат: %s\n", resultBool ? "пересекаются" : "не пересекаются");
                break;

            case 3:
                load("cffffb", inputFile, &c1, &x, &y, &width, &height, &resultBool);
                printf("Пересечение круга с прямоугольником\n");
                printf("Круг: (%.2f, %.2f) радиус %.2f\n", c1.x, c1.y, c1.radius);
                printf("Прямоугольник: (%.2f, %.2f) размер %.2fx%.2f\n", x, y, width, height);
                printf("Результат: %s\n", resultBool ? "пересекается" : "не пересекается");
                break;

            case 4:
                load("ccf", inputFile, &c1, &c2, &resultFloat);
                printf("Площадь пересечения кругов\n");
                printf("Круг 1: (%.2f, %.2f) радиус %.2f\n", c1.x, c1.y, c1.radius);
                printf("Круг 2: (%.2f, %.2f) радиус %.2f\n", c2.x, c2.y, c2.radius);
                printf("Площадь пересечения: %.2f\n", resultFloat);
                break;

            case 5:
                load("ffffffcb", inputFile, &x1, &y1, &x2, &y2, &x3, &y3, &resultCircle, &error);
                printf("Описанный круг вокруг треугольника\n");
                printf("Треугольник: (%.2f,%.2f) (%.2f,%.2f) (%.2f,%.2f)\n", x1, y1, x2, y2, x3, y3);
                if (error) {
                    printf("Результат: треугольник вырожденный\n");
                } else {
                    printf("Описанный круг: (%.2f, %.2f) радиус %.2f\n",
                           resultCircle.x, resultCircle.y, resultCircle.radius);
                }
                break;

            case 6:
                load("d", inputFile, &n);
                printf("Минимальный охватывающий круг для %d кругов\n", n);
                Circle circles[MAX_CIRCLES];
                for (int i = 0; i < n; i++) {
                    load("c", inputFile, &circles[i]);
                    printf("Круг %d: (%.2f, %.2f) радиус %.2f\n",
                           i + 1, circles[i].x, circles[i].y, circles[i].radius);
                }
                load("c", inputFile, &resultCircle);
                printf("Минимальный охватывающий круг: (%.2f, %.2f) радиус %.2f\n",
                       resultCircle.x, resultCircle.y, resultCircle.radius);
                break;
        }
        printf("---");
    }

    fclose(inputFile);
}


int main() {
    SetConsoleOutputCP(CP_UTF8);
    int choice;
    char input[MAX_FILENAME_LEN];
    char output[MAX_FILENAME_LEN];
    FILE *inputFile, *outputFile;

    printf("=== РАБОТА С БИНАРНЫМИ ФАЙЛАМИ ===\n");

    while (1) {
        printf("\nВыберите режим:\n");
        printf("1. Запись операций в файл\n");
        printf("2. Обработка операций и запись результатов\n");
        printf("3. Чтение и вывод результатов\n");
        printf("4. Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Введите имя файла для записи операций: ");
                scanf("%s", input);
                inputFile = fopen(input, "wb");
                if (inputFile) {
                    saveOperations(inputFile);
                } else {
                    printf("Ошибка открытия файла!\n");
                }
                break;

            case 2:
                printf("Введите имя файла с операциями: ");
                scanf("%s", input);
                printf("Введите имя файла для результатов: ");
                scanf("%s", output);
                inputFile = fopen(input, "rb");
                outputFile = fopen(output, "wb");
                if (inputFile && outputFile) {
                    executeOperations(inputFile, outputFile);
                } else {
                    printf("Ошибка открытия файлов!\n");
                }
                break;

            case 3:
                printf("Введите имя файла с результатами: ");
                scanf("%s", input);
                inputFile = fopen(input, "rb");
                if (inputFile) {
                    printOperations(inputFile);
                } else {
                    printf("Ошибка открытия файла!\n");
                }
                break;

            case 4:
                printf("Выход из программы.\n");
                return 0;

            default:
                printf("Неверный выбор!\n");
        }
    }
}
