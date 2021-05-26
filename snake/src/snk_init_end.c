#include "snk.h"

void game_interrupt(unsigned int num) {
	struct timespec interrupt = {0, 0};
	struct timespec ret;
	interrupt.tv_nsec = num;
	nanosleep(&interrupt, &ret);

	return;
}

int game_counter(snake *snk) {
	++snk->counter;
	if(!(snk->counter % snk->speed)) {
		snk->counter = 0;
		return(1);
	}

	return(0);
}

void setup_snake(snake *snk, int f_width, int f_height, int speed) {
	int count;
	const int stdsize = 5;

	snk->snake_arr = codl_malloc_check(15 * (int)sizeof(int*));
	for(count = 0; count < stdsize; ++count) {
		snk->snake_arr[count] = codl_malloc_check(2 * (int)sizeof(int));
		snk->snake_arr[count][0] = f_width / 2;
		snk->snake_arr[count][1] = f_height / 2 + count;
	}

	snk->field_arr = codl_malloc_check((size_t)f_width * sizeof(char*));
	for(count = 0; count < f_width; ++count) {
		snk->field_arr[count] = codl_malloc_check((size_t)f_height * sizeof(char));
		codl_memset(snk->field_arr[count], (size_t)f_height, 0, (size_t)f_height);
		snk->field_arr[count][0] = 1;
		snk->field_arr[count][f_height - 1] = 1;
	}

	codl_memset(snk->field_arr[0], (size_t)f_height, 1, (size_t)f_height);
	codl_memset(snk->field_arr[f_width - 1], (size_t)f_height, 1, (size_t)f_height);

	snk->snake_size   = stdsize;
	snk->f_width      = f_width;
	snk->f_height     = f_height;
	snk->score        = 0;
	snk->fruit_pos[0] = 5;
	snk->fruit_pos[1] = 5;
	snk->fruit_eaten  = 0;
	snk->direction    = 1;
	snk->counter      = 0;
	snk->speed        = speed;
	snk->wait_move    = 1;
	codl_memset(snk->snake_head, 3, 0, 3);
}

void init_snk(snake *snk, codl_window **main_win, codl_window **f_win, codl_window **snk_win, int width, int height) {
	srand((unsigned int)time(NULL));

	codl_cursor_mode(CODL_HIDE);
	/* codl_noecho(); */

	setup_snake(snk, width, height, 120);

	codl_initialize();
	*main_win = codl_create_window(codl_get_term(), 1, (codl_get_term()->width / 2) - width,
			(codl_get_term()->height / 2) - height / 2, width * 2, height);
	
	*f_win = codl_create_window(*main_win, 2, 0, 0, (*main_win)->width, (*main_win)->height);
	*snk_win = codl_create_window(*f_win, 3, 0, 0, (*f_win)->width, (*f_win)->height);
	
	codl_set_alpha(*snk_win, CODL_ENABLE);
	
	draw_field(snk, *f_win);
}

void game_loop(snake *snk, codl_window *snk_win, codl_window *main_win) {
	unsigned int key;

	for(;;) {
		game_interrupt(1000000);
		
		key = codl_get_key();

		if(codl_resize_term()) {
			codl_set_window_position(main_win, 
				(codl_get_term()->width  / 2) - main_win->width / 2, 
				(codl_get_term()->height / 2) - main_win->height / 2);
			codl_display();
		}

		if(key) {
			if(!snake_movement(snk, key)) break;
		}
		
		if(!snake_move(snk, snk_win)) break;
	}

}

void end_snk(snake *snk) {
	end_snake(snk);
	codl_end();
	codl_clear();
	codl_echo();
	codl_cursor_mode(CODL_SHOW);	
}
