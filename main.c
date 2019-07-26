#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include "timer.h"
#include "stopwatch.h"

int main(int argc, char* argv[]) {
    int hours;   // argv[1]
    int minutes; // argv[2], argv[1];
    int seconds; // argv[3], argv[2], argv[1];
    int time_in_seconds;

    tea teas[4];
    char *tea_type;

    if (!strcmp(argv[1], "stopwatch")) {
        if (argc == 5) {
            hours = atoi(argv[2]);
            minutes = atoi(argv[3]);
            seconds = atoi(argv[4]);
            time_in_seconds = (hours * 3600) + (minutes * 60) + seconds;
            stopwatch_start(time_in_seconds);
        } else if (argc == 4) {
            minutes = atoi(argv[2]);
            seconds = atoi(argv[3]);
            time_in_seconds = (minutes * 60) + seconds;
            stopwatch_start(time_in_seconds);
        } else if (argc == 3) {
            seconds = atoi(argv[2]);
            time_in_seconds = seconds;
            stopwatch_start(time_in_seconds);
        } else {
            stopwatch_start(0);
        }
    }

    if (argc > 4) {
        printf("Too many arguments provided. Timer only takes one argument: time\n");
        exit(1);
    }

    if (argc < 2) {
        printf("Too few arguments provided. Timer takes one argument: time\n");
        exit(1);
    }

    if (argc == 2) {
        // returns 0 if argv[1] is not an int
        seconds = atoi(argv[1]);

        // Do tea timer stuff
        if (seconds == 0) {
            tea_type = argv[1];
            printf("%s", tea_type);

            char *gs = "green";
            char *bs = "black";
            char *gl = "greenlong";
            char *bl = "blacklong";

            timer_init_tea(teas, gs, 1, 30, 0);
            timer_init_tea(teas, bs, 3, 0, 1);
            timer_init_tea(teas, gl, 2, 0, 2);
            timer_init_tea(teas, bl, 5, 0, 3);

            for (size_t i = 0; i < sizeof(teas)/sizeof(teas[0]); i++) {
                if (strcmp(argv[1], teas[i].key) == 0) {
                    minutes = teas[i].minutes;
                    seconds = teas[i].seconds;
                    time_in_seconds = (minutes * 60) + seconds;

                    timer_start(time_in_seconds, 2, 1);
                }
            }

            printf("Not a tea.\n");
            exit(1);
        }

        minutes = 0;
        hours = 0;

        timer_start(seconds, 1, 0);
    }


    if (argc == 3) {
        seconds = atoi(argv[2]);
        minutes = atoi(argv[1]);
        hours = 0;

        time_in_seconds = (minutes * 60) + seconds;

        timer_start(time_in_seconds, 2, 0);
    }

    if (argc == 4) {
        seconds = atoi(argv[3]);
        minutes = atoi(argv[2]);
        hours = atoi(argv[1]);

        time_in_seconds = (hours * 3600) + (minutes * 60) + seconds;

        timer_start(time_in_seconds, 3, 0);
    }

    return 0;
}
