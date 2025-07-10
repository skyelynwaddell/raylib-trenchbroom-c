#include "event.h"

Event events[255];

void print(const char *string)
{
    printf("%s\n", string);
}


void event_create(enum EVENT_TYPE event_type)
{
    switch(event_type)
    {
        case MAP_START:

        break;
    }
}