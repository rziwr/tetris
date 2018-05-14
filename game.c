#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <termios.h>
#include "config.h"

static int game_stop = 0;

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
  uint8_t i;
  uint8_t j;
  putchar ('\n');
  // Печать вершины
//  for (i = 0; i < FIELD_W; i++)
//    putchar ('=');
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

static char field [FIELD_W][FIELD_H];

static const char * figures [] = {
"\
o   \n\
o   \n\
o   \n\
o   \n\0",
"\
    \n\
    \n\
    \n\
oooo\n\0",
"\
    \n\
    \n\
oo  \n\
oo  \n\0",
"\
    \n\
    \n\
 o  \n\
ooo \n\0",
"\
    \n\
o   \n\
oo  \n\
o   \n\0",
"\
    \n\
    \n\
ooo \n\
 o  \n\0",
"\
    \n\
  o \n\
 oo \n\
  o \n\0",
"\
    \n\
    \n\
 o  \n\
ooo \n\0",
"\
    \n\
o   \n\
o   \n\
oo  \n\0",
"\
    \n\
    \n\
ooo \n\
o   \n\0",
"\
    \n\
oo  \n\
 o  \n\
 o  \n\0",
"\
    \n\
    \n\
  o \n\
ooo \n\0",
"\
    \n\
o   \n\
o   \n\
oo  \n\0",
NULL
};

#define FIG_CNT (13)

static int falling_figure;
static int X, Y;

static void show_figure (int fig, int x_pos, int y_pos) {
	int ch = 0;
	int x = x_pos;
	int y = y_pos;
	//setFieldPos (x, y);
	while (*(figures [fig] + ch)){
		if (*(figures [fig] + ch) == '\n'){
			x++;
			y = y_pos;
			//setFieldPos (x, y);
			ch++;
		}
		if (*(figures [fig] + ch) == 'o'){
			setFieldPos (x, y);
			putchar ('o');
		}
		y++;
		ch++;
	}
}

static void hide_figure (int fig, int x_pos, int y_pos) {
	int ch = 0;
	int x = x_pos;
	int y = y_pos;
	//setFieldPos (x, y);
	while (*(figures [fig] + ch)){
		if (*(figures [fig] + ch) == '\n'){
			x++;
			y = y_pos;
			//setFieldPos (x, y);
			ch++;
		}
		if (*(figures [fig] + ch) == 'o'){
			setFieldPos (x, y);
			putchar (' ');
		}
		y++;
		ch++;
	}
}

static int get_figure (){
	int fignum;
	fignum = rand () % FIG_CNT;
	show_figure ( fignum, 3, FIELD_W / 2);

	X = 3;
	Y = FIELD_W / 2;	
	//setFieldPos (1, FIELD_W/2);
	
	//printf ("%s", figures [fignum]);
	return fignum;
}

int fig_falled (void){
	static int i;
	
	if (i++ >= 4){
		i = 0;
		return 1;
	} else return 0;
	
}

struct termios term_old, term_new;

int game_init (int with, int heght){	
	
	tcgetattr (0, &term_old);
	term_new = term_old;
	term_new.c_lflag &= ~(ICANON | ECHO | ISIG);
	term_new.c_cc[VMIN] = 0;
	term_new.c_cc[VTIME] = 0;	
	tcsetattr(0, TCSANOW, &term_new);	
	
	clrscr ();
	printField ();
	printScore (0);
	
	int i, j;
	for (i = 0; i < FIELD_W; i++)
		for (j = 0; j < FIELD_H; j++)
			field [i][j] = ' ';
}

int timeout = 1000000;

typedef enum {
	GAME_NEXT_FIG = 0,
	GAME_FIG_FALL,
	GAME_IDLE,
	GAME_PAUSE,
	GAME_FAIL
} game_state_t;
	
void game_tick (void){
	static game_state_t state;
	static int i;
	int res = 0;
	char c;
	//printf (" %d\n", i++);
	//res = fread ( &c, 1, 1, stdin);
	res = getchar ();
	if (res > 0) {
		if (res == 'q'){
			game_stop = 1;
			tcsetattr(0, TCSANOW, &term_old);
			clrscr ();
			setFieldPos (1, 1);
		}	
		//putchar (c);
		//printf ("Res: %d\n", res);
		//printf ("FIG_CNT: %d\n", FIG_CNT);
	}
	//fflush (stdout);

	switch (state){
		case GAME_NEXT_FIG:
			falling_figure = get_figure ();
			state = GAME_FIG_FALL;

			break;
		case GAME_IDLE:
			hide_figure (falling_figure, 1, FIELD_W / 2);
			state = GAME_NEXT_FIG;
			break;
			
		case GAME_FIG_FALL:
			hide_figure (falling_figure, X, Y);
			X++;
			show_figure (falling_figure, X, Y);
			if (fig_falled ())
				state = GAME_NEXT_FIG;
			break;
		default:
			state = GAME_NEXT_FIG;
			break;
	}
}

void game_sleep (void){
	usleep (timeout);
}

int game_is_stopped (void){
	return game_stop;
}
