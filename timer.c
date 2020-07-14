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
    int input, i;
    //size_t additional_time;
    char sign, unit;

    //additional_time = 1;
    sign = '+';
    unit = 's';

    //timeout(1000); //delay in milliseconds for getch

    signal(SIGWINCH, timer_resize);

    initscr();
    curs_set(0); //hides cursor
    noecho(); //prevents user input from printing to the screen
    cbreak();

    getmaxyx(stdscr, row, col); //Gets window width and height and stores it in row and col vars

    for (i = time_in_seconds; i >= 0; i--) {
        size_t handled_input;
        timeout(1000); //delay in milliseconds for getch
        input = getch();

        handled_input = 0;

        switch (input) {
            case 115: {
                handled_input = 1;
                unit = 's';
                i++;
                break;
            }
        
            case 109: {
                handled_input = 1;
                unit = 'm';
                i++;
                break;
            }

            case 104: {
                handled_input = 1;
                unit = 'h';
                i++;
                break;
            }
        
            case 43: {
                handled_input = 1;
                //sign = '+';
                if (unit == 's') {
                    i = i + 1;
                }
                
                if (unit == 'm') {
                    i = i + 60;
                }
                
                if (unit == 'h') {
                    i = i + 3600;
                }
                
                i++;
                break;
            }

            case 45: {
                handled_input = 1;
                //sign = '-';
                
                if (unit == 's') {
                    i = i - 1;
                }
                
                if (unit == 'm') {
                    i = i - 60;
                }
                
                if (unit == 'h') {
                    i = i - 3600;
                }
                
                i++;

                if (i <= 0) {
                    i = 1;
                }
                break;
            }
        }

        if (input != -1 && handled_input == 0) {
            i++;
        };

        clear();
        refresh();

        if (i > 3600) {
            mvprintw(row/2, (col-4)/2, "%02d:%02d:%02d", i/3600, i%3600/60, i%3600%60);
            mvprintw(row/2 + 1, (col-4)/2, "+/-: %c1%c", sign, unit);
            //mvprintw(row/2 + 1, (col-4)/2, "+/-: %c%02dmin", sign, additional_time);
        } else if (i % 3600 > 60) {
            mvprintw(row/2, (col-2)/2, "%02d:%02d", i/60, i%60);
            mvprintw(row/2 + 1, (col-2)/2, "+/-: %c1%c", sign, unit);
            //mvprintw(row/2 + 1, (col-2)/2, "+/-: %c%02dmin", sign, additional_time);
        } else {
            mvprintw(row/2, (col-1)/2, "%02d", i);
            mvprintw(row/2 + 1, (col-1)/2, "+/-: %c1%c", sign, unit);
            //mvprintw(row/2 + 1, (col-1)/2, "+/-: %c%02dmin", sign, additional_time);
        }
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

    timeout(10000000000000);
    getch();

    endwin();

    exit(0);
}
