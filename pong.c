// updated 12-17-18 at 12:15 pm

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define DELAY 50000

typedef struct{int x, y, score;}player_obj;
typedef struct{int x, y; bool horizontal, vertical;}ball_obj;

int max_x, max_y;
int end;
int cont;
bool sleep_sig = FALSE, sleep_sig2 = FALSE;

int main(void){

    initscr();
    start_color();
    noecho();
    cbreak();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);

    getmaxyx(stdscr, max_y, max_x);

    clear();

    int color_flag = TRUE;
    cont = 1;
    while(TRUE){
        if(color_flag) attron(COLOR_PAIR(3));
        else attron(COLOR_PAIR(2));
        mvprintw(max_y/2-11, max_x/2-36,"    IIIIIIIIIII                                                           ");
        mvprintw(max_y/2-10, max_x/2-36,"   IIIIIIIIIIIIII                                                         ");
        mvprintw(max_y/2-9, max_x/2-36, "  IIIII       IIIIII                                                      ");
        mvprintw(max_y/2-8, max_x/2-36, "  IIIII        IIIIII                                                     ");
        mvprintw(max_y/2-7, max_x/2-36, "  IIIII        IIIIII                                                     ");
        mvprintw(max_y/2-6, max_x/2-36, "  IIIII        IIIIII                                                     ");
        mvprintw(max_y/2-5, max_x/2-36, "  IIIII       IIIIII   IIIIIIIII      IIIII                               ");
        mvprintw(max_y/2-4, max_x/2-36, "  IIIIIIIIIIIIIIII   IIIIIIIIIIIII    IIIIIIIIIIIII       IIIIIIIIII      ");
        mvprintw(max_y/2-3, max_x/2-36, "  IIIIIIIIIIIII     IIIII   IIIIIII   IIIIIIIIIIIIII     IIIIIIIIIIII     ");
        mvprintw(max_y/2-2, max_x/2-36, "  IIIIII           IIIIII   IIIIIIII  IIIII     IIIII   IIIII   IIIIII    ");
        mvprintw(max_y/2-1, max_x/2-36, "  IIIIII           IIIIII   IIIIIIII  IIIII     IIIII  IIIIII   IIIIII    ");
        mvprintw(max_y/2, max_x/2-36,   "  IIIIII           IIIIII   IIIIIIII  IIIII     IIIII  IIIIII   IIIIII    ");
        mvprintw(max_y/2+1, max_x/2-36, "  IIIIII            IIIII   IIIIIII   IIIII     IIIII   IIIII   IIIIII    ");
        mvprintw(max_y/2+2, max_x/2-36, " IIIIIIII            IIIIIIIIIIIII    IIIII     IIIII    IIIIIIIIIIIII    ");
        mvprintw(max_y/2+3, max_x/2-36, "IIIIIIIIII             IIIIIIIII     IIIIIII   IIIIIII    IIIIIIIIIIII    ");
        mvprintw(max_y/2+4, max_x/2-36, "                                                                IIIIII    ");
        mvprintw(max_y/2+5, max_x/2-36, "                                                                 IIIII    ");
        mvprintw(max_y/2+6, max_x/2-36, "               Written by Cullen Drissell              IIIIII     IIII    ");
        mvprintw(max_y/2+7, max_x/2-36, "                                                        IIII      IIII    ");
        mvprintw(max_y/2+8, max_x/2-36, "                Press Any Key to Continue                IIIIIIIIIIII     ");
        mvprintw(max_y/2+9, max_x/2-36, "                                                          IIIIIIIIII      ");
        refresh();
        usleep(600000);
        if(color_flag){
            attroff(COLOR_PAIR(3));
            color_flag = !color_flag;
        }
        else{
            attroff(COLOR_PAIR(2));
            color_flag = !color_flag;
        }
        if (getch() != ERR) {
            clear();
            break;
        }
    }

    while(TRUE){
        attron(COLOR_PAIR(1));
        mvprintw(max_y/2-5, max_x/2-6,  "Instructions");
        attron(COLOR_PAIR(3));
        mvprintw(max_y/2-3, max_x/2-28, "Player 1: the 'w' and 's' keys move your paddle up and down");
        attroff(COLOR_PAIR(3));
        attron(COLOR_PAIR(2));
        mvprintw(max_y/2-2, max_x/2-28, "Player 2: the up and down keys move your paddle up and down");
        attroff(COLOR_PAIR(2));
        attron(COLOR_PAIR(1));
        mvprintw(max_y/2, max_x/2-17,   "Press the 'p' key to pause the game");
        mvprintw(max_y/2+1, max_x/2-30, "Press either the backspace/delete or 'esc' key to exit the game");
        mvprintw(max_y/2+3, max_x/2-14, "First player to 7 points wins");
        mvprintw(max_y/2+5, max_x/2-11, "Press any key to begin");
        refresh();
        usleep(DELAY);
        clear();
        if (getch() != ERR) break;
    }

    player_obj player1 = {1, max_y/2, 0};
    player_obj player2 = {max_x-2, max_y/2, 0};
    ball_obj ball = {max_x/2, max_y/2, FALSE, FALSE};

    if ((max_y/2) % 2 == 0){
        player1.y++;
        player2.y++;
    }

    mvprintw(max_y/2,max_x/2, "3");
    refresh();
	usleep(600000);
    clear();
    mvprintw(max_y/2,max_x/2, "2");
    refresh();
    usleep(600000);
    clear();
    mvprintw(max_y/2,max_x/2, "1");
    refresh();
    usleep(600000);
    clear();
    mvprintw(max_y/2,max_x/2-1, "play");
    refresh();
    usleep(600000);
    clear();

    for(nodelay(stdscr, TRUE); !end ;usleep(DELAY)){

        if (ball.y == max_y-2 || ball.y == 1){
            ball.vertical = !ball.vertical;
        }
        if (ball.x >= max_x-3){
            if (ball.y == player2.y){
                ball.horizontal = !ball.horizontal;
            }
            else if(ball.y == player2.y-2 || (ball.y == player2.y-1 && ball.vertical == FALSE)){
                ball.horizontal = !ball.horizontal;
            }
            else if(ball.y == player2.y+2 || (ball.y == player2.y+1 && ball.vertical == TRUE)){
                ball.horizontal = !ball.horizontal;
            }
            else if (ball.y == player2.y-2 || (ball.y == player2.y-1 && ball.vertical == TRUE)){
                ball.horizontal = FALSE;
                ball.vertical = FALSE;
            }
            else if (ball.y == player2.y+2 || (ball.y == player2.y+1 && ball.vertical == FALSE)){
                ball.horizontal = FALSE;
                ball.vertical = TRUE;
            }
            else {
                player1.score++;
                ball.x = max_x/2;
                ball.y = max_y/2;
                ball.horizontal = false;
                sleep_sig = TRUE;
            }
        }
        if (ball.x <= 2){
            if (ball.y == player1.y){
                ball.horizontal = !ball.horizontal;
            }
            else if(ball.y == player1.y-2 || (ball.y == player1.y-1 && ball.vertical == FALSE)){
                ball.horizontal = !ball.horizontal;
            }
            else if(ball.y == player1.y+2 || (ball.y == player1.y+1 && ball.vertical == TRUE)){
                ball.horizontal = !ball.horizontal;
            }
            else if (ball.y == player1.y-2 || (ball.y == player1.y-1 && ball.vertical == TRUE)){
                ball.horizontal = TRUE;
                ball.vertical = FALSE;
            }
            else if (ball.y == player1.y+2 || (ball.y == player1.y+1 && ball.vertical == FALSE)){
                ball.horizontal = TRUE;
                ball.vertical = TRUE;
            }
            else {
                player2.score++;
                ball.x = max_x/2;
                ball.y = max_y/2;
                ball.horizontal = true;
                sleep_sig = TRUE;
            }
        }

        if (player1.y <= 1) player1.y = max_y-4;
        else if (player1.y >= max_y-2) player1.y = 3;

        if (player2.y <= 1) player2.y = max_y-4;
        else if (player2.y >= max_y-2) player2.y = 3;

        ball.x=ball.horizontal ? ball.x+2 : ball.x-2;
        ball.y=ball.vertical ? ball.y+1 : ball.y-1;

        switch(getch()){
            case KEY_BACKSPACE:
            case 127:
            case 0x1B:
                end = TRUE;
                break;
            case 'w':
            case 'W':
                player1.y-=2;
                break;
            case 's':
            case 'S':
                player1.y+=2;
                break;
            case KEY_UP:
                player2.y-=2;
                break;
            case KEY_DOWN:
                player2.y+=2;
                break;
            case 'p':
            case 'P':
                mvprintw(max_y/2,max_x/2-2,"Paused");
                refresh();
                getchar();
                break;
        }

        clear();

        attron(COLOR_PAIR(1));

        //prints scores and vertical line
        mvprintw(1,max_x/2-12,"p1 score: %i | p2 score: %i",player1.score,player2.score);
        mvvline(0,max_x/2,ACS_VLINE,max_y);
        wborder(stdscr, 0,0,0,0,0,0,0,0);

        //updates ball
        mvprintw(ball.y,ball.x,"o");

        if (sleep_sig2 == TRUE){
            getch();
            sleep_sig2 = FALSE;
        }

        //prints paddles for players
        for(int i =- 2; i < 3; i++){
            attron(COLOR_PAIR(3));
            mvprintw(player1.y+i,player1.x,"|");
            attroff(COLOR_PAIR(3));
            attron(COLOR_PAIR(2));
            mvprintw(player2.y+i,player2.x,"|");
            attroff(COLOR_PAIR(2));
        }

        attron(COLOR_PAIR(1));

        //Checks if game is over
        if (player1.score == 7){
            clear();
            mvprintw(max_y/2, (max_x/2)-7, "Player 1 Wins!");
            refresh();
            sleep(2);
            break;
        }
        else if(player2.score == 7){
            clear();
            mvprintw(max_y/2, (max_x/2)-7, "Player 2 Wins!");
            refresh();
            sleep(2);
            break;
        }

        if (sleep_sig == TRUE){
            sleep_sig2 = TRUE;
            sleep_sig = FALSE;
        }
    }
    clear();
    mvprintw(max_y/2, (max_x/2)-5, "Game Over");
	refresh();
	sleep(2);
    attroff(COLOR_PAIR(1));
	endwin();
	exit(1);
}