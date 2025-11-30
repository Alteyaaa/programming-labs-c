#include <windows.h>
#include "12.h"

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
