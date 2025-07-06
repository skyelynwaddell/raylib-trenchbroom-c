#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include "raylib.h"
#include "./defs.h"

// Global variables

extern int paused; // Game pause state

void pause_toggle(); // Toggle pause state

#endif // GLOBAL_H