// updated 12-17-18 at 12:15 pm

#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DELAY 50000
#define LEFT -1
#define RIGHT 1
#define UP -1
#define DOWN 1
#define X 1
#define Y 0

typedef struct{int x, y, dir, x_or_y; bool alive;}player_object;
typedef struct{double x, y; bool start, alive;}laser_object;
typedef struct{double x, y; bool life;}enemy_object;

int max_x = 0, max_y = 0;
time_t start_time = 0;
int t = 0;
int score = 0;
int damage = 200;
double speed = 0.1;
int title_flag = TRUE;

int color_flag;
void title(void){
	while (TRUE){
		if(color_flag) attron(COLOR_PAIR(3));
        else attron(COLOR_PAIR(2));
		mvprintw(max_y/2-7, max_x/2-43, "   IIIIIIIIIIIII      IIIIIIIII         IIIIIIII        IIIIIIIIIIIII    IIIIIIIIIIIIII");
		mvprintw(max_y/2-6, max_x/2-43, "  IIIIIIIIIIIIII    IIIIIIIIIIIII      IIIIIIIIII      IIIIIIIIIIIIII    IIIIIIIIIIIII ");
		mvprintw(max_y/2-5, max_x/2-43, " IIIII    IIIIII   IIIII   IIIIIII    IIIIIIIIIIII    IIIIII    IIIII    IIIII         ");
		mvprintw(max_y/2-4, max_x/2-43, "IIIIII            IIIIII   IIIIIII   IIII     IIIII  IIIIII    IIIIII    IIIIIIIIIIIIII");
		mvprintw(max_y/2-3, max_x/2-43, "IIIIIIIIIIIIIIII  IIIIIIIIIIIIIII   IIII  II  IIIII  IIIII    IIIIIII  IIIIIIIIIIIIIII ");
		mvprintw(max_y/2-2, max_x/2-43, "IIIIIIIIIIIIIIII  IIIIIIIIIIIIII   IIII  III  IIIII  IIIII             IIIII           ");
		mvprintw(max_y/2-1, max_x/2-43, "          IIIIII  IIIIII          IIII  IIII  IIIII  IIIII     IIIIII  IIIII           ");
		mvprintw(max_y/2, max_x/2-43,   "IIIIII    IIIII   IIIIII         IIII  IIIII  IIIII  IIIII    IIIII    IIIIIIIIIIIIIIII");
		mvprintw(max_y/2+1, max_x/2-43, "IIIIIIIIIIIIII    IIIIII        IIII  IIIIII  IIIII  IIIIIIIIIIIII     IIIIIIIIIIIIIII ");
		mvprintw(max_y/2+2, max_x/2-43, "IIIIIIIIIIII      IIIIII       IIII           IIIII  IIIIIIIIIIII      IIIIIIIIIIIIII  ");
		mvprintw(max_y/2+5, max_x/2-43, "                         Written by Cullen Drissell                                    ");
		mvprintw(max_y/2+7, max_x/2-43, "                                   Press Any Key to Continue                           ");
		refresh();
		usleep(60000);
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
}

void instruction(void){
	attron(COLOR_PAIR(3));
	while(TRUE){

		mvprintw(max_y/2-8, max_x/2-6,  "instructions");
		mvprintw(max_y/2-6, max_x/2-31, "navigate your ship around space with the w,a,s,d or arrow keys");
		mvprintw(max_y/2-5, max_x/2-19, "press the space bar to fire your laser");
		mvprintw(max_y/2-3, max_x/2-17, "press the 'p' key to pause the game");
		mvprintw(max_y/2-1, max_x/2-30, "press either the backspace/delete or 'esc' key to exit the game");
		mvprintw(max_y/2+1, max_x/2-9, "there are 10 rounds");
		mvprintw(max_y/2+3, max_x/2-28, "make sure enemy ships do not hit the bottom of the screen");
		mvprintw(max_y/2+4, max_x/2-22, "this will result in a decrease to your health");
		mvprintw(max_y/2+6, max_x/2-17, "if your health reaches 0, game over");
		mvprintw(max_y/2+8, max_x/2-11, "press any key to begin");
		refresh();
		usleep(DELAY);
		clear();
		if (getch() != ERR){
			clear();
			break;
		}
	}
	attroff(COLOR_PAIR(3));
}

void stars(void){
	attron(COLOR_PAIR(4));
	for (int x = 0; x <= max_x+1; x++){
		for (int y = 0; y <= max_y+1; y++){
			if (x*y%2 != 0 && rand()%400 <= 2){
				mvprintw(y,x,".");
				refresh();
			}
		}
	}
	attroff(COLOR_PAIR(4));
}

void gameOver(void){
	clear();
	time_t st = 0;
	st = time(NULL);
	time_t end = st+2;
	while (st <= end){
		st = time(NULL);
		clear();
		stars();
		attron(COLOR_PAIR(3));
		mvprintw(max_y/2, (max_x/2)-6, "game exited");
		refresh();
		attroff(COLOR_PAIR(3));
		if(t == end) break;
		usleep(DELAY);
	}
	endwin();
	exit(1);
}

void gameOverLose(void){
	clear();
	int x;
	while(TRUE){
		clear();
		stars();
		attron(COLOR_PAIR(3));
		mvprintw(max_y/2-3, (max_x/2)-4, "game over");
		mvprintw(max_y/2-1, (max_x/2)-8 , "final score: %d", score);
		mvprintw(max_y/2, (max_x/2)-15, "total elapsed time: %d seconds", t);
		mvprintw(max_y/2+2, (max_x/2)-23, "press the delete/backspace or esc key to exit");
		refresh();
		usleep(DELAY);
		if ((x = getch()) != ERR){
			if (x == 0x1B || x == 127 || x == KEY_BACKSPACE) break;
		}
	}
	attroff(COLOR_PAIR(3));
	endwin();
	exit(1);
}

void gameOverWin(void){
	clear();
	int x;
	while(TRUE){
		clear();
		stars();
		attron(COLOR_PAIR(3));
		mvprintw(max_y/2-3, (max_x/2)-8, "congrats! you won");
		mvprintw(max_y/2-1, (max_x/2)-8 , "final score: %d", score);
		mvprintw(max_y/2, (max_x/2)-15, "total elapsed time: %d seconds", t);
		mvprintw(max_y/2+2, (max_x/2)-23, "press the delete/backspace or esc key to exit");
		refresh();
		usleep(DELAY);
		if ((x = getch()) != ERR){
			if (x == 0x1B || x == 127 || x == KEY_BACKSPACE) break;
		}
	}
	attroff(COLOR_PAIR(3));
	clear();
	endwin();
	exit(1);
}

int main(int argc, char* argv[]){

	start_time = time(NULL);

	for (int x = 1; x <= 10; x++){

		int round_num = x;
		int next_x = 0, next_y = 0;
		int num_lasers = 0;
		int active_lasers = 0;
		int num_enemies = 0;
		int reload_flag;
		int star_flag;
		int enemy_count = 0;
		int speed_check = TRUE;

		initscr();
		if (has_colors() == TRUE) start_color();
		cbreak();
		noecho();
		curs_set(FALSE);
		keypad(stdscr, TRUE);
		nodelay(stdscr, TRUE);
		srand(time(NULL));
		getmaxyx(stdscr, max_y, max_x);

		init_pair(1, COLOR_GREEN, COLOR_BLACK);
    	init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
    	init_pair(3, COLOR_CYAN, COLOR_BLACK);
		init_pair(4, COLOR_WHITE, COLOR_BLACK);

		if(title_flag == TRUE){
			title();
			instruction();
			title_flag = FALSE;
		}

		num_enemies = round_num;
		num_lasers = 5;
		active_lasers = num_lasers;

		player_object player;
		laser_object laser[num_lasers];
		enemy_object enemy[num_enemies];

		player.x = max_x/2;
		player.y = max_y/2+10;
		player.alive = TRUE;
		player.dir = 0;
		player.x_or_y = -1;

		int place = max_x/(num_enemies+1);

		for (int i = 1; i <= num_enemies; i++){
			enemy[i].x = (i*place);
			enemy[i].y = 0;
			enemy[i].life = TRUE;
		}

		for (int i = 1; i <= num_lasers; i++){
			laser[i].x = player.x;
			laser[i].y = player.y;
			laser[i].start = FALSE;
			laser[i].alive = FALSE;
		}

		for(nodelay(stdscr, TRUE); TRUE ; usleep(DELAY)){

			int c;

			if ((c = getch()) != ERR){

				switch(c){
					case 0x1B: case 127: case KEY_BACKSPACE:
						gameOver();

					case KEY_LEFT: case 'a': case 'A':
						player.dir = LEFT;
						player.x_or_y = X;
						break;

					case KEY_RIGHT: case 'd': case 'D':
						player.dir = RIGHT;
						player.x_or_y = X;
						break;

					case KEY_UP: case 'w': case 'W':
						player.dir = UP;
						player.x_or_y = Y;
						break;

					case KEY_DOWN: case 's': case 'S':
						player.dir = DOWN;
						player.x_or_y = Y;
						break;

					case 'p': case 'P':
						while(getch() == ERR){
							clear();
							stars();
							attron(COLOR_PAIR(3));
							mvprintw(max_y/2-1,max_x/2-2,"paused");
							mvprintw(max_y/2+1,max_x/2-11,"press any key to resume");
							refresh();
							usleep(DELAY);
						}
						clear();
						break;

					case ' ':
						if (active_lasers == 5){
							laser[1].start = TRUE;
							break;
						}
						else if (active_lasers == 4){
							laser[2].start = TRUE;
							break;
						}
						else if (active_lasers == 3){
							laser[3].start = TRUE;
							break;
						}
						else if (active_lasers == 2){
							laser[4].start = TRUE;
							break;
						}
						else if (active_lasers == 1){
							laser[5].start = TRUE;
							break;
						}
						else break;
				}
			}

			// updates player's position
			if (player.x_or_y == X){
				next_x = player.x + player.dir;
				if (next_x >= max_x-1 || next_x < 1){
					player.dir *= -1;
				}
				else {
					player.x += 2*player.dir;
				}
			}
			else {
				next_y = player.y + player.dir;
				if (next_y >= max_y-1 || next_y < 1){
					player.dir *= -1;
				}
				else {
					player.y += player.dir;
				}
			}

			// updates enemy's positions
			for(int i = 1; i <= num_enemies; i++){
				if (enemy[i].life == TRUE){
					if(round_num % 2 == 0 && speed_check == TRUE){
						speed += 0.05;
						speed_check = FALSE;
					}
					enemy[i].y+=speed;
				}
			}

			// initializes a laser if it is shot
			for (int i = num_lasers; i >= 1; i--){
				if (laser[i].start == TRUE){
					laser[i].alive = TRUE;
					laser[i].x = player.x;
					laser[i].y = player.y;
					active_lasers--;
					laser[i].start = FALSE;
					break;
				}
			}

			// updates laser(s) position(s)
			for(int i = 1; i <= num_lasers; i++){
				if (laser[i].alive == TRUE ){
					laser[i].y-=2;
					if (laser[i].y <= 0){
						laser[i].alive = FALSE;
					}
				}
			}

			// reloads lasers if none left
			if (active_lasers == 0 &&
				laser[3].alive == FALSE &&
				laser[2].alive == FALSE &&
				laser[1].alive == FALSE) {
				active_lasers = num_lasers;
			}

			// checks if laser has hit enemy
			for (int i = 1; i <= num_enemies; i++){
				for (int j = 1; j <= num_lasers; j++){
					if (laser[j].x <= enemy[i].x+2 &&
					laser[j].x >= enemy[i].x-2 &&
					laser[j].y >= enemy[i].y-2 &&
					laser[j].y <= enemy[i].y &&
					enemy[i].life == TRUE){
					laser[j].alive = FALSE;
					enemy[i].life = FALSE;
					enemy_count++;
					score += 50;
					}
				}
			}

			// eliminates enemy if dead
			for(int i = 1; i <= num_enemies; i++){
				if (enemy[i].y > max_y){
					if (enemy[i].life == TRUE) damage -= 25;;
					enemy[i].life = FALSE;
					enemy_count++;
				}
			}

			clear();

			// prints player
			if (player.alive == TRUE){
				attron(COLOR_PAIR(3));
				mvprintw(player.y,player.x,"A");
				mvprintw(player.y+1,player.x-1,"A");
				mvprintw(player.y+1,player.x,"W");
				mvprintw(player.y+1,player.x+1,"A");
				attroff(COLOR_PAIR(3));
			}

			// prints laser(s)
			for (int i = 1; i <= num_lasers; i++){
				attron(COLOR_PAIR(1));
				if (laser[i].alive == TRUE) mvprintw(laser[i].y,laser[i].x,"|");
				attroff(COLOR_PAIR(1));
			}

			//prints enemy/enemies
			for (int i = 1; i <= num_enemies; i++){
				if (enemy[i].life == TRUE) {
					attron(COLOR_PAIR(2));
					mvprintw((int)enemy[i].y,(int)enemy[i].x,"V");
					mvprintw((int)enemy[i].y-1,(int)enemy[i].x-1,"W");
					mvprintw((int)enemy[i].y-1,(int)enemy[i].x+1,"W");
					mvprintw((int)enemy[i].y-2,(int)enemy[i].x,"W");
					mvprintw((int)enemy[i].y-2,(int)enemy[i].x-1,"W");
					mvprintw((int)enemy[i].y-2,(int)enemy[i].x+1,"W");
					mvprintw((int)enemy[i].y-2,(int)enemy[i].x+2,"W");
					mvprintw((int)enemy[i].y-2,(int)enemy[i].x-2,"W");
					attroff(COLOR_PAIR(2));
				}
			}

			// updates game information (time, score, damage, ammo)
			t = time(NULL)-start_time;
			attron(COLOR_PAIR(3));
			mvprintw(max_y-5, 2, "Time: %d", t);
			mvprintw(max_y-4, 2, "Score: %d", score);
			mvprintw(max_y-3,2, "Damage: %d", damage);
			mvprintw(max_y-2,2, "Ammo:");
			attroff(COLOR_PAIR(3));
			attron(COLOR_PAIR(1));
			for (int i = 1; i <= active_lasers; i++){
				mvprintw(max_y-2, 7+i, "|");
			}
			attroff(COLOR_PAIR(1));


			// prints stars
			stars();

			attron(COLOR_PAIR(3));
			mvprintw(1,2,"Round %d", round_num);
			attroff(COLOR_PAIR(3));

			refresh();
			clear();

			if (enemy_count == num_enemies) break;

		}

		if (damage <= 0){
			gameOverLose();
		}
		else if(round_num == 9) {
			time_t st = 0;
			st = time(NULL);
			time_t end = st+3;
			while (st <= end){
				st = time(NULL);
				clear();
				stars();
				attron(COLOR_PAIR(3));
				mvprintw(max_y/2-2, max_x/2-5, "final round");
				mvprintw(max_y/2, max_x/2-10, "current score: %d", score);
				mvprintw(max_y/2+1, max_x/2-13, "elapsed time: %d seconds", t);
				mvprintw(max_y/2+2, max_x/2-9, "health left: %d", damage);
				refresh();
				usleep(DELAY);
			}
			attroff(COLOR_PAIR(3));
		}
		else if(round_num == 10)break;
		else {
			time_t st = 0;
			st = time(NULL);
			time_t end = st+3;
			while (st <= end){
				st = time(NULL);
				clear();
				stars();
				attron(COLOR_PAIR(3));
				mvprintw(max_y/2-2, max_x/2-5, "round %d", round_num+1);
				mvprintw(max_y/2, max_x/2-10, "current score: %d", score);
				mvprintw(max_y/2+1, max_x/2-13, "elapsed time: %d seconds", t);
				mvprintw(max_y/2+2, max_x/2-9, "health left: %d", damage);
				refresh();
				usleep(DELAY);
			}
			attroff(COLOR_PAIR(3));
		}
	}
	gameOverWin();
}
