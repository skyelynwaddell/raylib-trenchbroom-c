#ifndef EVENT_H
#define EVENT_H

#include "raylib.h"
#include <stdio.h>
#include "global.h"

typedef struct {
    char name[64];
} Event;

enum EVENT_TYPE {
    MAP_START,
};

extern Event events[];

void event_create(enum EVENT_TYPE event_type);
void print(const char *string);

#endif // EVENT_H