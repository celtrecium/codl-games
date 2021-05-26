#include "snk.h"

void draw_field(snake *snk, codl_window *f_win) {
	int count;
	int count_1;

	for(count = 0; count < snk->f_height; ++count) {
		for(count_1 = 0; count_1 < snk->f_width; ++count_1) {
			codl_set_cursor_position(f_win, count_1 * 2, count);
			codl_set_colour(f_win, snk->field_arr[count_1][count] ? SNAKE_WALL_COLOUR : SNAKE_BG_COLOUR, 256);
			codl_write(f_win, "  ");
		}
	}
}

void draw_snake(snake *snk, codl_window *snk_win) {
	int count;
	
	codl_set_colour(snk_win, SNAKE_BODY_COLOUR, 256);
	codl_window_clear(snk_win);

	for(count = 0; count < snk->snake_size; ++count) {
		codl_set_cursor_position(snk_win, snk->snake_arr[count][0] * 2, snk->snake_arr[count][1]);
		codl_write(snk_win, "  ");
	}
	
	codl_set_colour(snk_win, SNAKE_HEAD_COLOUR, SNAKE_EYES_COLOUR);
	codl_set_cursor_position(snk_win, snk->snake_arr[0][0] * 2, snk->snake_arr[0][1]);
	codl_write(snk_win, snk->snake_head);

	codl_set_colour(snk_win, SNAKE_FOOD_COLOUR, 256);
	codl_set_cursor_position(snk_win, snk->fruit_pos[0] * 2, snk->fruit_pos[1]);
	codl_write(snk_win, "  ");
}

