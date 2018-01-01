#include <stdio.h>
#include <stdint.h>
#include "config.h"

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

static void show_figure (int fig, int x_pos, int y_pos) {
	int ch = 0;
	int x = x_pos;
	int y = y_pos;
	//setFieldPos (x, y);
	while (*(figures [fig] + ch)){
		if (*(figures [fig] + ch) == '\n'){
			y++;
			x = x_pos;
			setFieldPos (x, y);
			ch++;
		}
		if (*(figures [fig] + ch) == 'o'){
			setFieldPos (x, y);
			putchar ('o');
		}
		x++;
		ch++;
	}
}

static void get_figure (){
	int fignum = rand () % FIG_CNT;
	show_figure ( fignum, 1, FIELD_W / 2);

	//setFieldPos (1, FIELD_W/2);
	
	//printf ("%s", figures [fignum]);
}

int game_init (int with, int heght){
	clrscr ();
	printField ();
	printScore (0);
}

int timeout = 1000000;


typedef enum {
	GAME_NEXT_FIG = 0,
	GAME_IDLE,
	GAME_PAUSE,
	GAME_FAIL
} game_state_t;

void game_tick (void){
	static game_state_t state;
	static int i;
	int res = 0;
	char c;
	printf (" %d\n", i++);
	res = fread  ( &c, 1, 1, stdin);
	if (res > 0) {
		putchar (c);
		printf ("Res: %d\n", res);
		printf ("FIG_CNT: %d\n", FIG_CNT);
	}
	fflush (stdout);

	switch (state){
		case GAME_NEXT_FIG:
			get_figure ();
			state = GAME_IDLE;
			break;
		case GAME_IDLE:

			break;
		default:
			state = GAME_NEXT_FIG;
			break;
	}
}


void game_sleep (void){
	usleep (timeout);
}
