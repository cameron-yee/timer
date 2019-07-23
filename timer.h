#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>

#ifndef TIMER_H
#define TIMER_H

typedef struct Tea {
    char *key;
    int minutes;
    int seconds;
} tea;

void timer_resize();
void timer_init_tea(tea teas[], char *key, int minutes, int seconds, size_t position);
void timer_start(int time_in_seconds, size_t max_unit, int is_tea);

#endif
