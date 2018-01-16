#include <stdio.h>
#include <stdint.h>
//#include <ncurses.h>
#include <termios.h>
#include <unistd.h>
#include "config.h"

struct {
	int fig;
	int x;
	int y;
} game_fig = {
	.fig = 1,
	.x = 2,
	.y = FIELD_H + 2
};

static void clrscr (){
	printf ("\033[2J");
//	printf ("\033[10;10H");
}

static void setFieldPos (int x, int y){
	printf ("\033[%d;%dH", x, y);
}

void printScore (int score){
	setFieldPos (2, FIELD_H + 2);
	printf ("Score: %05d", score);
	fflush (stdout);
}

static void printField (){

  printf("\e[?25l");
  fflush(stdout);

  uint8_t i;
  uint8_t j;
  putchar ('\n');
  // Печать вершины
  for (i = 0; i < FIELD_W; i++)
    putchar ('=');
  putchar ('\n');
  // Печать поля
  for (i = 0; i < FIELD_H - 2; i++){
    putchar('|');
    for (j = 0; j < FIELD_W - 2; j++)
      putchar (' ');
    printf ("|\n");
  }
  // Печать нижней части
  for (i = 0; i< FIELD_W; i++)
    putchar ('=');
  putchar ('\n');
}


char * figures [] = {
"\
o   \n\
o   \n\
o   \n\
o   \n",
"\
    \n\
    \n\
    \n\
oooo\n",
"\
    \n\
    \n\
oo  \n\
oo  \n",
"\
    \n\
    \n\
 o  \n\
ooo \n",
"\
    \n\
o   \n\
oo  \n\
o   \n",
"\
    \n\
    \n\
ooo \n\
 o  \n",
"\
    \n\
  o \n\
 oo \n\
  o \n",
"\
    \n\
    \n\
 o  \n\
ooo \n",
"\
    \n\
o   \n\
o   \n\
oo  \n",
"\
    \n\
    \n\
ooo \n\
o   \n",
"\
    \n\
oo  \n\
 o  \n\
 o  \n",
"\
    \n\
    \n\
  o \n\
ooo \n",
"\
    \n\
o   \n\
o   \n\
oo  \n",
NULL
};

#define FIG_CNT (13)

static void hide_figure (int fig, int x_pos, int y_pos) {
	int ch = 0;
	int x = x_pos;
	int y = y_pos;
	//setFieldPos (x, y);
	while (*(figures [fig] + ch)){
		if (*(figures [fig] + ch) == '\n'){
			x++;
			y = y_pos;
			setFieldPos (x, y);
			//ch++;
		}
		if (*(figures [fig] + ch) == 'o'){
			y++;
			setFieldPos (x, y);
			putchar (' ');
		}
		//x++;
		ch++;
	}
}

static void show_figure (int fig, int x_pos, int y_pos) {
	int ch = 0;
	int x = x_pos;
	int y = y_pos;
	//setFieldPos (x, y);
	while (*(figures [fig] + ch)){
		if (*(figures [fig] + ch) == '\n'){
			x++;
			y = y_pos;
			setFieldPos (x, y);
			//ch++;
		}
		if (*(figures [fig] + ch) == 'o'){
			y++;
			setFieldPos (x, y);
			putchar ('o');
		}
		//x++;
		ch++;
	}
}

static int get_figure (){
	static num;
	//int fignum = rand () % FIG_CNT;
	int fignum;

	if (num >= FIG_CNT) {

		num = 0;
	}
	fignum = num++;
	show_figure (fignum, 1, FIELD_W / 2);
	
	//setFieldPos (1, FIELD_W/2);
	
	//printf ("%s", figures [fignum]);
	return fignum;
}

int game_init (int with, int heght){
	//noecho();
    struct termios TermConf;
    tcgetattr(STDIN_FILENO, &TermConf);
    TermConf.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &TermConf);

	clrscr ();
	printField ();
	printScore (0);
}

int game_delay = 1000000;


typedef enum {
	GAME_NEXT_FIG = 0,
	GAME_IDLE,
	GAME_PAUSE,
	GAME_FAIL
} game_state_t;

void game_tick (void){
	static game_state_t state;
	static int i;
	static int cntr;
	//static int fig;
	int res = 0;
	char c;
	
	//printf (" %d\n", state);
	/*
	res = fread  ( &c, 1, 1, stdin);
	if (res > 0) {
		putchar (c);
		printf ("Res: %d\n", res);
		printf ("FIG_CNT: %d\n", FIG_CNT);
	}
	fflush (stdout);
*/
	switch (state){
		case GAME_NEXT_FIG:

/*			setFieldPos (10, 10);
			putchar ('+');
			setFieldPos (20, 10);
			putchar ('+');
			setFieldPos (30, 10);
			putchar ('+');
			setFieldPos (40, 40);
			putchar ('+');			*/
			game_fig.fig = get_figure ();
			state = GAME_IDLE;
	//		printf ("GAME_NEXT_FIG \n");
			break;
		case GAME_IDLE:
			cntr++;
			if (cntr >= 2){
				hide_figure (game_fig.fig, 1, FIELD_W / 2);
				state = GAME_NEXT_FIG;
				cntr = 0;
			}
			break;
		default:
			state = GAME_NEXT_FIG;
			break;
	}
	fflush (stdout);
}


void game_sleep (void){
	usleep (game_delay);
}
