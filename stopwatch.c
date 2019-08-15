#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include "stopwatch.h"

//Global module variables
int row, col;
size_t hours, minutes, seconds;

void stopwatch_resize() {
    erase();
    endwin();
    refresh();
    getmaxyx(stdscr, row, col); //Gets window width and height and stores it in row and col vars
}

void stopwatch_log() {
    FILE *log;
    time_t t; 

    t = time(NULL);
    struct tm time = *localtime(&t);

    endwin();
    printf("%lu hours, %lu minutes, %lu seconds\n", hours, minutes, seconds);
    fflush(stdout);

    log = fopen("log.txt", "a+");
    fprintf(log, "%lu hours, %lu minutes, %lu seconds | %d/%d/%d %d:%d:%d\n", hours, minutes, seconds, time.tm_mon + 1, time.tm_mday, time.tm_year + 1900, time.tm_hour, time.tm_min, time.tm_sec);
    fclose(log);

    exit(0);
}

void stopwatch_start(int time_in_seconds) {
    //size_t hours, minutes, seconds;
    int paused;

    paused = 0;

    signal(SIGWINCH, stopwatch_resize);
    signal(SIGINT, stopwatch_log);

    initscr();
    curs_set(0); //hides cursor
    noecho(); //prevents user input from printing to the screen
    cbreak();

    getmaxyx(stdscr, row, col); //Gets window width and height and stores it in row and col vars

    hours = time_in_seconds / 3600;
    minutes = (time_in_seconds / 60) % 60;
    seconds = time_in_seconds % 60;

    while (hours < 24) {
        timeout(1000); //delay in milliseconds for getch
        ++seconds;

        int input = getch(); //replaces sleep(1) because it blocks for 1 second

        if (input != -1 && paused == 0) {
            paused = 1;
            refresh();
            if (hours != 0 || minutes != 0) {
                mvprintw(row/2 - 1, (col-2)/2, "PAUSED");
            } else {
                mvprintw(row/2 - 1, (col-5)/2, "PAUSED");
            }
            timeout(10000000000000);
            getch();
            paused = 0;
            erase();
        } else if (input != -1 && paused == 1) {
            paused = 0;
            erase();
        }

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
        } else if (minutes != 0) {
            mvprintw(row/2, (col-2)/2, "%02d:%02d", minutes, seconds);
        } else {
            mvprintw(row/2, (col-1)/2, "%02d", seconds);
        }

        //after ++seconds so if time_in_seconds == 0 this will never hit
        //if (time_in_seconds == (hours * 3600) + (minutes * 60) + seconds) {
        //    for (size_t j = 5; j > 0; j--) {
        //        beep();
        //        flash();

        //        if (j != 1) {
        //            sleep(1);
        //        }

        //        ++seconds;

        //        if (seconds == 60) {
        //            seconds = 0;
        //            ++minutes;
        //        }

        //        if (minutes == 60) {
        //            minutes = 0;
        //            ++hours;
        //        }
        //    }

        //    //seconds += 4; //To compensate for j = 5
        //}

        //sleep(1);
    }

    //TODO: add logging
    // echo();
    // getch();
    // save getch to log
    //
    endwin();

    exit(0);
}

