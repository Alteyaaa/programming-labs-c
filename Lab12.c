#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>

typedef enum {
    CONCERT,
    CONFERENCE,
    EXHIBITION
} EventType;

#define SCHEDULED 0x01
#define CANCELLED 0x02
#define TICKET_REQUIRED 0x04
#define ONLINE 0x08
#define FULLY_BOOKED 0x10

#define MAX_LENGTH 50

typedef struct {
    char bandName[MAX_LENGTH];
    int duration;
} ConcertInfo;

typedef struct {
    char topic[MAX_LENGTH];
    int speakers;
} ConferenceInfo;

typedef struct {
    char exhibitName[MAX_LENGTH];
    int halls;
} ExhibitionInfo;

typedef union {
    ConcertInfo concert;
    ConferenceInfo conference;
    ExhibitionInfo exhibition;
} EventDetails;

typedef union {
    unsigned char raw;
    struct {
        unsigned scheduled      : 1;
        unsigned cancelled      : 1;
        unsigned ticketRequired : 1;
        unsigned online         : 1;
        unsigned fullyBooked    : 1;
        unsigned reserved       : 3;
    } bits;
} EventFlags;

typedef struct {
    EventType type;
    EventDetails details;
    EventFlags flags;
} Event;

Event *createEvent(EventType type, EventFlags flags, EventDetails details) {
    Event *newEvent = (Event *) malloc(sizeof(Event));
    if (newEvent) {
        newEvent->type = type;
        newEvent->flags = flags;
        newEvent->details = details;
    }
    return newEvent;
}

Event **addEvent(Event **events, int *count, Event *newEvent) {
    events = (Event **) realloc(events, (*count + 1) * sizeof(Event *));
    if (events) {
        events[*count] = newEvent;
        (*count)++;
    }
    return events;
}

Event **deleteEvent(Event **events, int *count, int index) {
    if (index >= 0 && index < *count) {
        free(events[index]);
        for (int i = index; i < *count - 1; i++) {
            events[i] = events[i + 1];
        }
        (*count)--;
        events = (Event **) realloc(events, (*count) * sizeof(Event *));
    }
    return events;
}

void updateEvent(Event *event, EventType newType, EventFlags newFlags, EventDetails newDetails) {
    event->type = newType;
    event->flags = newFlags;
    event->details = newDetails;
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
            case 1: f.raw |= SCHEDULED; break;
            case 2: f.raw |= CANCELLED; break;
            case 3: f.raw |= TICKET_REQUIRED; break;
            case 4: f.raw |= ONLINE; break;
            case 5: f.raw |= FULLY_BOOKED; break;
            default: printf("Неверный выбор.\n"); break;
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

int main() {
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
                return 0;

            default:
                printf("Неверный выбор. Повторите попытку.\n");
        }
    }
}
