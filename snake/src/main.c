#include "snk.h"
 
int main(void) {
	const int SNK_F_WIDTH  = 30;
	const int SNK_F_HEIGHT = 20;

	/* Declaration of windows and snake strutures */
	codl_window *main_win;
	codl_window *field_win;
	codl_window *snake_win;
	snake main_snk;
	
	/* Initialization */
	init_snk(&main_snk, &main_win, &field_win, &snake_win, 
			SNK_F_WIDTH, SNK_F_HEIGHT);

	/* Game loop */
	game_loop(&main_snk, snake_win, main_win);

	/* Game end */
	end_snk(&main_snk);

	return(0);
}
