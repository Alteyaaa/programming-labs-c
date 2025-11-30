#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>

#include "11.h"
#include "12.h"

// Вывод круга
void printCircle(const Circle *c) {
    printf("(%.2f, %.2f) [%.2f]\n", c->x, c->y, c->radius);
}

// Функция ввода круга с проверкой
Circle inputCircle() {
    float x, y, r;
    printf("Введите x, y, r круга >> ");
    scanf("%f %f %f", &x, &y, &r);
    while (r <= 0) {
        printf("Некорректный ввод! Радиус должен быть > 0\n");
        printf("Введите корректный радиус круга >> ");
        scanf("%f", &r);
    }
    return createCircle(x, y, r);
}

void lab11() {
    SetConsoleOutputCP(CP_UTF8);
    while (true) {
        int command;
        printf("\nВыберите операцию (0 - выход):\n");
        printf("1) Создать круг\n");
        printf("2) Проверить точку в круге\n");
        printf("3) Пересечение двух кругов\n");
        printf("4) Пересечение круга с прямоугольником\n");
        printf("5) Площадь пересечения двух кругов\n");
        printf("6) Описанный круг вокруг треугольника\n");
        printf("7) Минимальный охватывающий круг\n");
        printf("8) Вывод круга\n");

        scanf("%d", &command);

        switch (command) {
            case 0:
                return;

            case 1: {
                Circle c = inputCircle();
                printCircle(&c);
                break;
            }

            case 2: {
                Circle c = inputCircle();
                float px, py;
                printf("Введите координаты проверяемой точки >> ");
                scanf("%f %f", &px, &py);
                printf("Точка %s круг\n", inCircle(&c, px, py) ? "входит в" : "не входит в");
                break;
            }

            case 3: {
                Circle c1 = inputCircle();
                Circle c2 = inputCircle();
                printf("Круги %s\n", intersectionCirсles(&c1, &c2) ? "пересекаются" : "не пересекаются");
                break;
            }

            case 4: {
                Circle c = inputCircle();
                float x, y, width, height;
                printf("Введите параметры прямоугольной области (x, y, ширина, высота) >> ");
                scanf("%f %f %f %f", &x, &y, &width, &height);
                while (width <= 0 || height <= 0) {
                    printf("Некорректный ввод! Высота и ширина должны быть > 0");
                    printf("Введите ширину и высоту прямоугольной области >> ");
                    scanf("%f %f", &width, &height);
                }
                printf("Круг %s с прямоугольной областью\n",
                       intersectionCircleRectangle(&c, x, y, width, height) ? "пересекается" : "не перескается");
                break;
            }

            case 5: {
                Circle c1 = inputCircle();
                Circle c2 = inputCircle();
                printf("Площадь пересечения двух кругов равна %.2f\n", squareIntersectionCircles(&c1, &c2));
                break;
            }

            case 6: {
                float x1, y1, x2, y2, x3, y3;
                printf("Введите вершины треугольника x1 y1 x2 y2 x3 y3: ");
                scanf("%f %f %f %f %f %f", &x1, &y1, &x2, &y2, &x3, &y3);
                bool error;
                Circle c = circumscribedCircle(x1, y1, x2, y2, x3, y3, &error);
                if (error)
                    printf("Треугольник вырожденный, описанный круг построить невозможно.\n");
                else
                    printf(
                        "Вокруг данного треугольника можно описать круг с центром в точке (%.2f, %.2f) и радиусом %.2f\n",
                        c.x, c.y, c.radius);
                break;
            }

            case 7: {
                int n;
                printf("Введите количество кругов >> ");
                scanf("%d", &n);
                Circle arr[100];
                for (int i = 0; i < n; i++) {
                    printf("Круг %d:\n", i + 1);
                    arr[i] = inputCircle();
                }
                Circle c = minimalEnclosingCircle(arr, n);
                printf("Минимальный охватывающий круг имеет центр в точке (%.2f, %.2f) и радиус %.2f\n", c.x, c.y,
                       c.radius);
                break;
            }

            case 8: {
                Circle c = inputCircle();
                printCircle(&c);
                break;
            }

            default:
                printf("Некорректный выбор пункта меню\n");
        }
    }
}

void printEvent(Event *event) {
    printf("Тип события: ");
    switch (event->type) {
        case CONCERT:
            printf("Концерт\n");
            printf("Группа: %s\n", event->details.concert.bandName);
            printf("Продолжительность: %d минут\n", event->details.concert.duration);
            break;
        case CONFERENCE:
            printf("Конференция\n");
            printf("Тема: %s\n", event->details.conference.topic);
            printf("Докладчиков: %d\n", event->details.conference.speakers);
            break;
        case EXHIBITION:
            printf("Выставка\n");
            printf("Название: %s\n", event->details.exhibition.exhibitName);
            printf("Залы: %d\n", event->details.exhibition.halls);
            break;
    }
    printf("Состояние: ");
    if (event->flags.raw & SCHEDULED)
        printf("Запланировано ");
    if (event->flags.raw & CANCELLED)
        printf("Отменено ");
    if (event->flags.raw & TICKET_REQUIRED)
        printf("Требует билета ");
    if (event->flags.raw & ONLINE)
        printf("Онлайн ");
    if (event->flags.raw & FULLY_BOOKED)
        printf("Полностью забронировано ");
    printf("\n");
}

void printAllEvents(Event **events, int count) {
    for (int i = 0; i < count; i++) {
        printf("Событие %d:\n", i + 1);
        printEvent(events[i]);
        printf("\n");
    }
}

void printEventsByFlags(Event **events, int count, EventFlags mask) {
    for (int i = 0; i < count; i++) {
        if ((events[i]->flags.raw & mask.raw) == mask.raw) {
            printEvent(events[i]);
            printf("\n");
        }
    }
}

void inputString(char *str) {
    getchar();
    fgets(str, MAX_LENGTH, stdin);
    str[strcspn(str, "\n")] = 0;
}

EventFlags inputFlags() {
    EventFlags f;
    f.raw = 0;

    int choice;
    printf("\nВыберите флаг (0 — закончить ввод):\n");
    printf("1 — Запланировано\n");
    printf("2 — Отменено\n");
    printf("3 — Требует билета\n");
    printf("4 — Онлайн\n");
    printf("5 — Полностью забронировано\n");
    printf("Ваш выбор: ");
    scanf("%d", &choice);

    switch (choice) {
        case 0: return f;
        case 1: f.raw |= SCHEDULED;
            break;
        case 2: f.raw |= CANCELLED;
            break;
        case 3: f.raw |= TICKET_REQUIRED;
            break;
        case 4: f.raw |= ONLINE;
            break;
        case 5: f.raw |= FULLY_BOOKED;
            break;
        default: printf("Неверный выбор.\n");
            break;
    }
}

void inputEventDetails(EventDetails *details, EventType type) {
    switch (type) {
        case CONCERT:
            printf("Введите название группы: ");
            inputString(details->concert.bandName);
            printf("Введите продолжительность концерта (в минутах): ");
            scanf("%d", &details->concert.duration);
            break;
        case CONFERENCE:
            printf("Введите тему конференции: ");
            inputString(details->conference.topic);
            printf("Введите количество докладчиков: ");
            scanf("%d", &details->conference.speakers);
            break;
        case EXHIBITION:
            printf("Введите название выставки: ");
            inputString(details->exhibition.exhibitName);
            printf("Введите количество залов: ");
            scanf("%d", &details->exhibition.halls);
            break;
    }
}

void lab12() {
    SetConsoleOutputCP(CP_UTF8);

    int count = 0;
    Event **events = NULL;
    int choice;
    Event *newEvent;
    EventDetails details;
    EventFlags flags;
    int eventIndex;

    while (true) {
        printf("Выберите операцию:\n");
        printf("1. Создать событие\n");
        printf("2. Удалить событие из массива\n");
        printf("3. Обновить событие\n");
        printf("4. Вывести все события\n");
        printf("5. Вывести события по заданной комбинации флагов\n");
        printf("6. Выход\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                printf("Выберите тип события (0 - Концерт, 1 - Конференция, 2 - Выставка): ");
                int type;
                scanf("%d", &type);

                if (type < 0 || type > 2) {
                    printf("Неверный тип события.\n");
                    break;
                }

                printf("Выберите флаги состояния события:\n");
                flags = inputFlags();


                inputEventDetails(&details, (EventType) type);

                newEvent = createEvent((EventType) type, flags, details);
                events = addEvent(events, &count, newEvent);
                printf("Событие создано и добавлено в массив.\n");
                break;
            }

            case 2:
                if (count == 0) {
                    printf("Нет событий для обновления.\n");
                    break;
                }

                printf("Введите индекс события для удаления (1-%d): ", count);
                scanf("%d", &eventIndex);
                eventIndex--;

                if (eventIndex >= 0 && eventIndex < count) {
                    events = deleteEvent(events, &count, eventIndex);
                    printf("Событие удалено.\n");
                } else {
                    printf("Неверный индекс.\n");
                }
                break;

            case 3:
                if (count == 0) {
                    printf("Нет событий для обновления.\n");
                    break;
                }

                printf("Введите индекс события для обновления (1-%d): ", count);
                scanf("%d", &eventIndex);
                eventIndex--;

                if (eventIndex >= 0 && eventIndex < count) {
                    printf("Введите новые данные для события.\n");
                    printf("Выберите тип события (0 - Концерт, 1 - Конференция, 2 - Выставка): ");
                    int type;
                    scanf("%d", &type);

                    if (type < 0 || type > 2) {
                        printf("Неверный тип события.\n");
                        break;
                    }

                    printf("Выберите новые флаги состояния:\n");
                    flags = inputFlags();

                    inputEventDetails(&details, (EventType) type);

                    updateEvent(events[eventIndex], (EventType) type, flags, details);
                    printf("Событие обновлено.\n");
                } else {
                    printf("Неверный индекс.\n");
                }
                break;

            case 4:
                printf("Все события:\n");
                printAllEvents(events, count);
                break;

            case 5: {
                EventFlags mask;
                printf("Выберите маску флагов для фильтрации:\n");
                mask = inputFlags();

                printf("События с указанной комбинацией флагов:\n");
                printEventsByFlags(events, count, mask);
                break;
            }

            case 6:
                printf("Выход из программы.\n");
                for (int i = 0; i < count; i++) {
                    free(events[i]);
                }
                free(events);
                return;

            default:
                printf("Неверный выбор. Повторите попытку.\n");
        }
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    int choice;

    while (true) {
        printf("Выберите лабораторную работу:\n");
        printf("1. Лабораторная работа 11\n");
        printf("2. Лабораторная работа 12\n");
        printf("3. Выход\n");
        printf("Введите номер: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                lab11();
                break;
            case 2:
                lab12();
                break;
            case 3:
                printf("Выход из программы.\n");
                return 0;
            default:
                printf("Некорректный выбор. Пожалуйста, попробуйте снова.\n");
        }
    }
}
