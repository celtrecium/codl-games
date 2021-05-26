#include "tetrisinterface.h"


void game_interrupt(unsigned int num) {
	struct timespec interrupt = {0, 0};
	struct timespec ret;
	interrupt.tv_nsec = num;

	nanosleep(&interrupt, &ret);

	return;
}


void draw_tetris(tetris_window *wt, codl_window *wi, int tetris_bg, int tetris_fg) {
	int temp_x = 0;
	int temp_y = 0;
	int pos;
	char numstring[10];

	codl_set_frame_colours(0, 0, 0, 0, 0, 0, 0, 0);
	
	for(temp_y = 0; temp_y < wt->field_height; ++temp_y) {
		for(temp_x = 0; temp_x < wt->field_width; ++temp_x) {
			pos = temp_y * wt->field_width + temp_x;
			if(wt->screen[pos] != 0) {
				codl_set_frame_colours(
						(wt->screen[pos] < 9) ? wt->screen[pos] + 8 : wt->screen[pos] - 8, 0,
						(wt->screen[pos] < 9) ? wt->screen[pos] + 8 : wt->screen[pos] - 8, 0,
						(wt->screen[pos] < 9) ? wt->screen[pos] + 8 : wt->screen[pos] - 8, 0,
						(wt->screen[pos] < 9) ? wt->screen[pos] + 8 : wt->screen[pos] - 8, 0
						);

				codl_set_colour(wi, wt->screen[pos], 256);

				codl_frame(wi, temp_x * wt->square_width, temp_y * wt->square_height,
						(temp_x * wt->square_width) + wt->square_width,
						(temp_y * wt->square_height) + wt->square_height);
			} else {
				codl_set_colour(wi, 0, 256);
				codl_rectangle(wi, temp_x * wt->square_width, temp_y * wt->square_height,
						(temp_x * wt->square_width) + wt->square_width,
						(temp_y * wt->square_height) + wt->square_height, " ");
			}
		}
	}

	codl_set_colour(wi->parent_win, tetris_bg, tetris_fg);
	codl_set_attribute(wi->parent_win, CODL_BOLD);

	codl_set_cursor_position(wi->parent_win, wi->width + 2, 3);
	codl_write(wi->parent_win, "S C O R E :");

	codl_set_cursor_position(wi->parent_win, wi->width + 2, 4);
	codl_write(wi->parent_win, codl_itoa(wt->score, numstring));

	codl_set_cursor_position(wi->parent_win, wi->width + 2, 6);
	codl_write(wi->parent_win, "L I N E S :");

	codl_set_cursor_position(wi->parent_win, wi->width + 2, 7);
	codl_write(wi->parent_win, codl_itoa(wt->line_count, numstring));
}


void setup_window(codl_window **win, codl_window **tet_win, codl_window *p_win, char bg, tetris_window *wt, int x_pos, int y_pos) {
	codl_set_frame_colours(
		(bg < 9) ? bg + 8 : bg - 8,
		0,
		(bg < 9) ? bg + 8 : bg - 8,
		0,
		(bg < 9) ? bg + 8 : bg - 8,
		0,
		(bg < 9) ? bg + 8 : bg - 8,
		0
	);
	
	*win = codl_create_window(p_win, 1, x_pos, y_pos,
				(wt->field_width * wt->square_width) + 15, (wt->field_height * wt->square_height) + 2);
	codl_set_colour(*win, bg, 256);
	codl_rectangle(*win, 0, 0, (*win)->width, (*win)->height, " ");
	codl_frame(*win, 0, 0, (*win)->width, (*win)->height);

	*tet_win = codl_create_window(*win, 2, 1, 1, wt->field_width * wt->square_width, wt->field_height * wt->square_height);
}


int menu_function(codl_window *win, int colour_bg_act, int colour_fg_act, int colour_bg, int colour_fg, char *list) {
	int count;
	int len = 0;
	int list_pos = 0;
	int redraw = 1;
	unsigned int key = 0;
	struct timespec interrupt = {0, 10000000};
	struct timespec ret;

	for((void)(count = 0); list[count]; ++count) {
		if((list[count] & 0xC0) == 0xC0) count += 2;
		if((list[count] & 0xE0) == 0xE0) count += 3;
		if((list[count] & 0xF0) == 0xF0) count += 4;

		if(list[count] == '\n') ++len;
	}


	if(win->height < len) {
		codl_resize_window(win, win->width, len);
	}

	codl_set_cursor_position(win, 0, 0);
	codl_write(win, list);

	for(;;) {
		nanosleep(&interrupt, &ret);

		key = codl_get_key();

		if(codl_resize_term()) {
			codl_set_window_position(win->parent_win, 
				(codl_get_term()->width  / 2) - (win->parent_win->width  / 2),
				(codl_get_term()->height / 2) - (win->parent_win->height / 2));
			codl_redraw();
			redraw = 1;
			codl_window_clear(codl_get_term());
		}

		if(redraw) {
			codl_set_colour(win, colour_bg, colour_fg);
			codl_replace_attributes(win, 0, 0, win->width, win->height);
			codl_set_colour(win, colour_bg_act, colour_fg_act);
			codl_replace_attributes(win, 0, list_pos, win->width, list_pos);

			codl_display();

			redraw = 0;
		}

		if(key) {
			codl_set_colour(win, colour_bg, colour_fg);
			codl_replace_attributes(win, 0, 0, win->width, win->height);
			codl_set_colour(win, colour_bg_act, colour_fg_act);
			codl_replace_attributes(win, 0, list_pos, win->width, list_pos);
			
			switch(key) {
				case CODL_KEY_DOWN:
					if(list_pos < len) ++list_pos;
					redraw = 1;

					break;
				case CODL_KEY_UP:
					if(list_pos > 0) --list_pos;
					redraw = 1;

					break;
				case CODL_KEY_RETURN:
					return(list_pos);
			}
		}
	}
}
