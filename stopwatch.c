#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include "stopwatch.h"

//Global module variables
int row, col;

void stopwatch_resize() {
    erase();
    endwin();
    refresh();
    getmaxyx(stdscr, row, col); //Gets window width and height and stores it in row and col vars
}

void stopwatch_start(int time_in_seconds) {
    size_t hours, minutes, seconds;

    signal(SIGWINCH, stopwatch_resize);

    initscr();
    curs_set(0); //hides cursor
    noecho(); //prevents user input from printing to the screen

    getmaxyx(stdscr, row, col); //Gets window width and height and stores it in row and col vars

    hours = 0;
    minutes = 0;
    seconds = 0;

    //TODO: bad practice to save getch() as char, should be int
    // I need to figure out what x would be as an int
    //char ch;
    //while((ch = getch()) != 'x') {
    while (hours < 24) {
        ++seconds;

        if (seconds == 60) {
            seconds = 0;
            ++minutes;
        }

        if (minutes == 60) {
            minutes = 0;
            ++hours;
        }


        refresh();

        if (hours != 0) {
            mvprintw(row/2, (col-4)/2, "%02d:%02d:%02d", hours, minutes, seconds);
            //mvprintw(row/2 + 1, (col-8)/2, "Press 'x' to exit");
        } else if (minutes != 0) {
            mvprintw(row/2, (col-2)/2, "%02d:%02d", minutes, seconds);
            //mvprintw(row/2 + 1, (col-8)/2, "Press 'x' to exit");
        } else {
            mvprintw(row/2, (col-1)/2, "%02d", seconds);
            //mvprintw(row/2 + 1, (col-8)/2, "Press 'x' to exit");
        }

        //after ++seconds so if time_in_seconds == 0 this will never hit
        if (time_in_seconds == (hours * 3600) + (minutes * 60) + seconds) {
            for (size_t j = 5; j > 0; j--) {
                beep();
                flash();

                if (j != 1) {
                    sleep(1);
                }

                ++seconds;

                if (seconds == 60) {
                    seconds = 0;
                    ++minutes;
                }

                if (minutes == 60) {
                    minutes = 0;
                    ++hours;
                }
            }

            //seconds += 4; //To compensate for j = 5
        }

        sleep(1);
    }

    //TODO: add logging
    // echo();
    // getch();
    // save getch to log
    //
    endwin();

    exit(0);
}

