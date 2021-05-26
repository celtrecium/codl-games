#include "si_main.h"

int main(void) {
	si_struct spaceinvaders;

	init_game(&spaceinvaders, 80, 25);
	loop_game(&spaceinvaders);
	end_game(&spaceinvaders);

	return(0);
}
