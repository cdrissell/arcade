// updated 12-17-18 at 12:15 pm

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main(void){

    int max_x, max_y;
    int status;

    initscr();
    start_color();
    noecho();
    cbreak();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);

    while (TRUE){

        getmaxyx(stdscr, max_y, max_x);

        attron(COLOR_PAIR(2));
        mvprintw(max_y/2-2, max_x/2-3, "arcade");
        attroff(COLOR_PAIR(2));
        attron(COLOR_PAIR(1));
        mvprintw(max_y/2, max_x/2-11, "press 1 to play pong");
        attroff(COLOR_PAIR(1));
        attron(COLOR_PAIR(3));
        mvprintw(max_y/2+1, max_x/2-11, "press 2 to play space");
        attroff(COLOR_PAIR(3));
        attron(COLOR_PAIR(2));
        mvprintw(max_y/2+3, max_x/2-21, "press 'esc' or backspace/delete to exit");
        attroff(COLOR_PAIR(2));

        refresh();

        int f = fork();

        if (f == 0){
            while(TRUE){
                switch(getch()){
                    case '1':
                        execv("./pong", NULL);
                
                    case '2':
                        execv("./space", NULL);

                    case 0x1B: case 127: case KEY_BACKSPACE: 
                        endwin();
                        kill(f,SIGINT);
                }
            }
        }
        else{
            wait(&status); 
            endwin();
        }
    }
}