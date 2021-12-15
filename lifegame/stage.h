#pragma once

#include "util.h"
#include "config.h"

#define MAX_LINE_SIZE 100

typedef struct lifegame_stage
{
    int height;
    int width;
    int cell[MAX_LINE_SIZE][MAX_LINE_SIZE];
    int nutrition[MAX_LINE_SIZE][MAX_LINE_SIZE];
} Stage;

Stage *new_stage(const int height, const int width);
void step_stage(Stage *stage);
void echo_stage(Stage *stage);
void debug_stage(Stage *stage);
