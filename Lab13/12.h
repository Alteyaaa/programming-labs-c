#ifndef LAB12_H
#define LAB12_H

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
        unsigned scheduled: 1;
        unsigned cancelled: 1;
        unsigned ticketRequired: 1;
        unsigned online: 1;
        unsigned fullyBooked: 1;
        unsigned reserved: 3;
    } bits;
} EventFlags;

typedef struct {
    EventType type;
    EventDetails details;
    EventFlags flags;
} Event;

Event *createEvent(EventType type, EventFlags flags, EventDetails details);

Event **addEvent(Event **events, int *count, Event *newEvent);

Event **deleteEvent(Event **events, int *count, int index);

void updateEvent(Event *event, EventType newType, EventFlags newFlags, EventDetails newDetails);
#endif

