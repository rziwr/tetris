#include <stdio.h>
#include "game.h"

int main (void){
	printf ("Hello world\n");
	game_init (11, 22);

	while (1){
		game_tick ();
		game_sleep ();
	}
}
