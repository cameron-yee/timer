#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>

//Global module variables
int row, col;

typedef struct Tea {
    char *key;
    int minutes;
    int seconds;
} tea;

void init_tea(tea teas[], char *key, int minutes, int seconds, size_t position); 
void start_timer(int time_in_seconds, size_t max_unit, int is_tea);

void init_tea(tea teas[], char *key, int minutes, int seconds, size_t position) {
    teas[position].key = key;
    teas[position].minutes = minutes;
    teas[position].seconds = seconds;
}

void resize() {
    erase();
    endwin();
    refresh();
    getmaxyx(stdscr, row, col); //Gets window width and height and stores it in row and col vars
}


//Max unit: 3 for hour, 2 for minute, 1 for second
void start_timer(int time_in_seconds, size_t max_unit, int is_tea) {
    signal(SIGWINCH, resize);

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


int main(int argc, char* argv[]) {
    int hours;   // argv[1]
    int minutes; // argv[2], argv[1];
    int seconds; // argv[3], argv[2], argv[1];
    int time_in_seconds;

    tea teas[4];
    char *tea_type;

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

            char gs[] = "green";
            char bs[] = "black";
            char gl[] = "greenlong";
            char bl[] = "blacklong";

            init_tea(teas, gs, 1, 30, 0);
            init_tea(teas, bs, 3, 0, 1);
            init_tea(teas, gl, 2, 0, 2);
            init_tea(teas, bl, 5, 0, 3);

            for (size_t i = 0; i < sizeof(teas)/sizeof(teas[0]); i++) {
                if (strcmp(argv[1], teas[i].key) == 0) {
                    minutes = teas[i].minutes;
                    seconds = teas[i].seconds;
                    time_in_seconds = (minutes * 60) + seconds;

                    start_timer(time_in_seconds, 2, 1);
                }
            }

            printf("Not a tea.\n");
            exit(1);
        }

        minutes = 0;
        hours = 0;

        start_timer(seconds, 1, 0);
    }


    if (argc == 3) {
        seconds = atoi(argv[2]);
        minutes = atoi(argv[1]);
        hours = 0;

        time_in_seconds = (minutes * 60) + seconds;

        start_timer(time_in_seconds, 2, 0);
    }

    if (argc == 4) {
        seconds = atoi(argv[3]);
        minutes = atoi(argv[2]);
        hours = atoi(argv[1]);

        time_in_seconds = (hours * 3600) + (minutes * 60) + seconds;

        start_timer(time_in_seconds, 3, 0);
    }

    return 0;
}
