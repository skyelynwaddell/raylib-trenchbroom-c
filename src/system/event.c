#include "event.h"

Event events[255];


/*
print
Will function as the engines print statement
and will print to the ingame console as well
*/
void print(const char *string)
{
    printf("%s\n", string);
}


/*
event_create
Creates an event and adds it to
the event array
*/
void event_create(enum EVENT_TYPE event_type)
{
    switch(event_type)
    {
        case MAP_START:

        break;
    }
}