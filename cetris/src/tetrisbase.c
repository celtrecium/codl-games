#include "tetrisbase.h"

int test_tetromino_pos(int tetr, int rot, int pos_x, int pos_y, tetris_window *tw);

static char *tetrs[7] = { 
	"__X___X___X___X_",
	"__X__XX__X______",
	"_X___XX___X_____",
	"_____XX__XX_____",
	"_XX___X___X_____",
	"_XX__X___X______",
	"__X__XX___X_____"
};


void set_tetris_movement(tetris_window *wi, unsigned int right, unsigned int left, unsigned int up, unsigned int down, unsigned int quit) {
	wi->tetr_movement.t_right = right;
	wi->tetr_movement.t_left  = left;
	wi->tetr_movement.t_up    = up;
	wi->tetr_movement.t_down  = down;
	wi->tetr_movement.t_quit  = quit;
}

void tetris_end(tetris_window *wi) {
	free(wi->field);
	free(wi->screen);
	free(wi->line);
}



int rotate_tetromino(int x_pos, int y_pos, int deg) {
	switch(deg % 4) {
		case 0: 
			return(y_pos * 4 + x_pos); 
		case 1: 
			return(12 + y_pos - (x_pos * 4)); 
		case 2: 
			return(15 - (y_pos * 4) - x_pos); 
		case 3: 
			return(3 - y_pos + (x_pos * 4)); 
	}

	return(0);
}

int test_tetromino_pos(int tetr, int rot, int pos_x, int pos_y, tetris_window *tw) {
	int x = 0;
	int y = 0;
	int p;
	int f;

	for((void)(x = 0); x < 4; ++x) {
		for((void)(y = 0); y < 4; ++y) {
			p = rotate_tetromino(x, y, rot);
			f = (pos_y + y) * tw->field_width + (pos_x + x);

			if(pos_x + x >= 0 && pos_x + x < tw->field_width &&
			   pos_y + y >= 0 && pos_y + y < tw->field_height &&
			   tetrs[tetr][p] != '_' && tw->field[f] != 0)
				return(0);
		}
	}

	return(1);
}

void tetris_force_down(tetris_window *tw) {
	int temp_x;
	int temp_y;
	int line_1;
	
	 if(tw->force_down && !tw->tetr_exit && !tw->gameover) {
		tw->speed_count = 0;
		++tw->tetr_count;

		if(tw->tetr_count % 50 == 0) {
			if(tw->speed >= 10) { 
				--tw->speed;
			}

			if(test_tetromino_pos(tw->curr_tetr, tw->curr_rot, tw->curr_x_pos, tw->curr_y_pos + 1, tw)) {
	 	 	 	 ++tw->curr_y_pos;
	 	 	 } 
	 	 	 else {
				temp_x = 0;
				temp_y = 0;

				for(temp_x = 0; temp_x < 4; ++temp_x) {
					for(temp_y = 0; temp_y < 4; ++temp_y) {
						if(tetrs[tw->curr_tetr][rotate_tetromino(temp_x, temp_y, tw->curr_rot)] != '_') {
							tw->field[(tw->curr_y_pos + temp_y) * tw->field_width + (tw->curr_x_pos + temp_x)] =
							(char)tw->curr_tetr + 1;
						}
					}
				}

				for(temp_y = 0; temp_y < 4; ++temp_y) {
					if(tw->curr_y_pos + temp_y < tw->field_height - 1) {
						line_1 = 1;

						for(temp_x = 1; temp_x < tw->field_width - 1; ++temp_x) {
							line_1 &= (tw->field[(tw->curr_y_pos + temp_y) * tw->field_width + temp_x]) != 0;
						}

						if(line_1) {
							for(temp_x = 1; temp_x < tw->field_width - 1; ++temp_x) {
								tw->field[(tw->curr_y_pos + temp_y) * tw->field_width + temp_x] = 8;
							}

							++tw->line_arr_size;

							tw->line[tw->line_arr_size] = tw->curr_y_pos + temp_y;
						}
					}
				}

				tw->score += 25;

				if(tw->line_arr_size) {
					tw->score += (1 << tw->line_arr_size) * 100;
				}

				tw->curr_x_pos = tw->field_width / 2;
				tw->curr_y_pos = 0;
				tw->curr_rot = 0;
				tw->curr_tetr = rand() % 7;

				tw->gameover = !test_tetromino_pos(tw->curr_tetr, tw->curr_rot, tw->curr_x_pos, tw->curr_y_pos, tw);
			}
		}
	}
}


void create_tetris_window(tetris_window *twin, int field_width, int field_height, int speed) {
	int x = 0;
	int y = 0;
	int count = 0;

	twin->field_width   = field_width;
	twin->field_height  = field_height;
	twin->screen_width  = field_width;
	twin->screen_height = field_height;
	twin->square_width  = 4;
	twin->square_height = 2;
	twin->field 	    = NULL;
	twin->screen        = NULL;
	twin->gameover      = 0;
	twin->curr_tetr     = rand() % 7;
	twin->curr_rot      = 0;
	twin->curr_x_pos    = field_width / 2;
	twin->curr_y_pos    = 0;
	twin->speed         = speed;
	twin->speed_count   = 0;
	twin->force_down    = 0;
	twin->rot_hold      = 0;
	twin->tetr_count    = 0;
	twin->score         = 0;
	twin->line_arr_size = 0;
	twin->line_count    = 0;
	twin->line          = NULL;
	twin->tetr_exit     = 0;

	twin->field  = malloc((size_t)((twin->field_width  * twin->field_height)  * (int)sizeof(char)));
	twin->screen = malloc((size_t)((twin->screen_width * twin->screen_height) * (int)sizeof(char)));
	twin->line   = malloc((size_t)(twin->field_height  * (int)sizeof(int)));

	if(!twin->field || !twin->screen || !twin->line) {
		fputs("Memory allocation error!\n", stderr);

		exit(EXIT_FAILURE);
	}

	for((void)(x = 0); x < twin->field_width; ++x) {
		for((void)(y = 0); y < twin->field_height; ++y) {
			twin->field[y * twin->field_width + x] = 
                (x == 0 || x == twin->field_width - 1 || y == twin->field_height - 1) 
                ? 9 : 0;
		}
	}

	for((void)(count = 0); count < (twin->screen_width * twin->screen_height); ++count) {
		twin->screen[count] = ' ';
	}
}


void tetris_movement(unsigned int sym, tetris_window *win) {
	if(!win->tetr_exit && !win->gameover) { 
		win->rot_hold = 1;
		if(sym == win->tetr_movement.t_right) {
			win->curr_x_pos += 
				 (test_tetromino_pos(win->curr_tetr, win->curr_rot, win->curr_x_pos + 1, win->curr_y_pos, win)) 
				 ? 1 : 0;
		} 
		else if(sym == win->tetr_movement.t_left) {
			win->curr_x_pos -=
				(test_tetromino_pos(win->curr_tetr, win->curr_rot, win->curr_x_pos - 1, win->curr_y_pos, win)) 
				? 1 : 0;
		}
		else if(sym == win->tetr_movement.t_down) {
			win->curr_y_pos +=
				(test_tetromino_pos(win->curr_tetr, win->curr_rot, win->curr_x_pos, win->curr_y_pos + 1, win)) 
				? 1 : 0;
		}
		else if(sym == win->tetr_movement.t_up) {
			win->curr_rot   +=
				(win->rot_hold && test_tetromino_pos(win->curr_tetr, win->curr_rot + 1, win->curr_x_pos, win->curr_y_pos, win)) 
				 ? 1 : 0;
		    		 win->rot_hold = 0;
		} 
		else if(sym == win->tetr_movement.t_quit) {
		 	win->tetr_exit = 1;
		}
	}
}


void tetris_display_buffer(tetris_window *wt) {
	int temp_x = 0;
	int temp_y = 0;
	char sym_array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 9, 8 };

	for((void)(temp_x = 0); temp_x < wt->field_width; ++temp_x) {
		for((void)(temp_y = 0); temp_y < wt->field_height; ++temp_y) {
			wt->screen[temp_y * wt->field_width + temp_x] = sym_array[(int) wt->field[temp_y * wt->field_width + temp_x]];
		}
	}

	for((void)(temp_x = 0); temp_x < 4; ++temp_x) {
		for((void)(temp_y = 0); temp_y < 4; ++temp_y) {
			if(tetrs[wt->curr_tetr][rotate_tetromino(temp_x, temp_y, wt->curr_rot)] != '_') {
				wt->screen[(wt->curr_y_pos + temp_y) * wt->field_width + (wt->curr_x_pos + temp_x)] = (char)wt->curr_tetr + 1;
			}
		}
	}
}

void tetris_count(tetris_window *wt) {
	++wt->speed_count;

	if(wt->speed_count == INT_MAX)
		wt->speed_count = 0;

	wt->force_down = (wt->speed_count == wt->speed);
}

void tetris_burn_line(tetris_window *wt) {
	if(wt->line_arr_size) {
	 	int temp_x = 0;
	 	int temp_y = 0;
		int second_y = 0;

		for((void)(temp_x = 1); temp_x < wt->field_width - 1; ++temp_x) {
			for((void)(temp_y = 1); temp_y <= wt->line_arr_size; temp_y++) {
				for((void)(second_y = wt->line[temp_y]); second_y > 0; --second_y) {
					wt->field[second_y * wt->field_width + temp_x] = wt->field[(second_y - 1) * wt->field_width + temp_x];
				}

				wt->field[temp_x] = 0;
			}
   		}

	 	wt->line_count += wt->line_arr_size;
		wt->line_arr_size = 0;

		if(wt->speed > 0) {
			wt->speed -= (wt->line_count % 10) ? 0 : 1;
		}
	}
}


void tetris_clear_field(tetris_window *twin) {
	int x = 0;
	int y = 0;
	int count = 0;

	for((void)(x = 0); x < twin->field_width; ++x) {
		for((void)(y = 0); y < twin->field_height; ++y) {
			twin->field[y * twin->field_width + x] = 
                (x == 0 || x == twin->field_width - 1 || y == twin->field_height - 1) 
                ? 9 : 0;
		}
	}

	for((void)(count = 0); count < (twin->screen_width * twin->screen_height); ++count) {
		twin->screen[count] = ' ';
	}

	twin->gameover      = 0;
	twin->speed         += (twin->line_count / 10);
	twin->curr_tetr     = rand() % 7;
	twin->curr_rot      = 0;
	twin->curr_x_pos    = twin->field_width / 2;
	twin->curr_y_pos    = 0;
	twin->speed_count   = 0;
	twin->force_down    = 0;
	twin->rot_hold      = 0;
	twin->tetr_count    = 0;
	twin->score         = 0;
	twin->line_arr_size = 0;
	twin->line_count    = 0;
	twin->tetr_exit     = 0;
}
