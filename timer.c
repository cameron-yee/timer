#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include "timer.h"

//Global module variables
int row, col;

void timer_init_tea(tea teas[], char *key, int minutes, int seconds, size_t position) {
    teas[position].key = key;
    teas[position].minutes = minutes;
    teas[position].seconds = seconds;
}

void timer_resize() {
    erase();
    endwin();
    refresh();
    getmaxyx(stdscr, row, col); //Gets window width and height and stores it in row and col vars
}

//Max unit: 3 for hour, 2 for minute, 1 for second
void timer_start(int time_in_seconds, size_t max_unit, int is_tea) {
    signal(SIGWINCH, timer_resize);

    initscr();
    curs_set(0); //hides cursor
    noecho(); //prevents user input from printing to the screen

    getmaxyx(stdscr, row, col); //Gets window width and height and stores it in row and col vars

    for (int i = time_in_seconds; i >= 0; i--) {
        refresh();

        if (max_unit == 3) {
            mvprintw(row/2, (col-4)/2, "%02d:%02d:%02d", i/3600, i%3600/60, i%3600%60);
        } else if (max_unit == 2) {
            mvprintw(row/2, (col-2)/2, "%02d:%02d", i/60, i%60);
        } else if (max_unit == 1) {
            mvprintw(row/2, (col-1)/2, "%02d", i);
        }

        sleep(1);
    }

    refresh();
    if (is_tea == 0) {
        mvprintw(row/2, (col-5)/2, "%s\n",  "Time is up.");
    } else {
        mvprintw(row/2, (col-9)/2, "%s\n",  "Your tea is ready.");
    }

    mvprintw(row/2 + 2, (col-11)/2, "%s\n",  "Press any key to exit");

    for (size_t j = 5; j > 0; j--) {
        beep();
        flash();
        sleep(1);
    }

    getch();

    endwin();

    exit(0);
}
