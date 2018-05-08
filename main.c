#include <stdio.h>
#include "game.h"

int main (void){
	printf ("Hello world\n");
	game_init (11, 22);

	while (game_is_stopped () == 0){
		game_tick ();
		game_sleep ();
	}
}
