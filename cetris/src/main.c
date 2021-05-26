#define _C_POSIX_SOURCE 199309L

#include <stdio.h>
#include <time.h>
#include "tetrisbase.h"
#include "tetrisinterface.h"
#include "codl.h"
#include <signal.h>


static int game_menu(codl_window *tetris_title_win, codl_window *tetris_menu) {
	int choise;

	codl_window_clear(codl_get_term());
	codl_load_buffer_from_file(tetris_title_win, "title_img.cdl", 0, 0);

	codl_set_cursor_position(tetris_title_win, 0, 12);
	codl_set_colour(tetris_title_win, 256, 6);
	codl_set_attribute(tetris_title_win, 0);
	codl_write(tetris_title_win, "cetris v0.1");

	codl_set_attribute(tetris_menu, CODL_BOLD | CODL_DIM | CODL_ITALIC);
	choise = menu_function(tetris_menu, 256, 14, 256, 6, "   Start\n   Help\n   Quit");

	return(choise);
}


static int game_over(codl_window *g_win) {
	codl_window *gameover_win;
	const int seconds = 9;
	int count         = seconds;
	const int width   = 22;
	const int height  = 9;
	const int bg_col  = 0;
	time_t first_time = time(NULL);
	char count_str[4];
	unsigned int key  = 0;

	codl_set_frame_symbols("│", "│", "─", "─", "┌", "┐", "└", "┘");
	codl_set_frame_colours(7, 8, 7, 8, 7, 8, 7, 8);

	gameover_win = codl_create_window(codl_get_term(), 3, 
			(codl_get_term()->width / 2) - width / 2,
			(codl_get_term()->height / 2) - height / 2, width, height);

	codl_set_colour(gameover_win, bg_col, 0);
	codl_rectangle(gameover_win, 0, 0, gameover_win->width, gameover_win->height, " ");
	codl_frame(gameover_win, 0, 0, gameover_win->width, gameover_win->height);

	codl_set_colour(gameover_win, bg_col, CODL_RED);
	codl_set_attribute(gameover_win, CODL_BOLD | CODL_DIM);
	codl_set_cursor_position(gameover_win, 6, 2);
	codl_write(gameover_win, "GAME OVER!");

	codl_set_colour(gameover_win, bg_col, 14);

	while(count) {
		game_interrupt(1000000);
		if(codl_resize_term()) {
			codl_set_window_position(g_win, 
				(codl_get_term()->width  / 2) - g_win->width  / 2, 
				(codl_get_term()->height / 2) - g_win->height / 2);
			codl_set_window_position(gameover_win, 
				(codl_get_term()->width  / 2) - gameover_win->width  / 2, 
				(codl_get_term()->height / 2) - gameover_win->height / 2);
			codl_window_clear(codl_get_term());
			codl_redraw();
		}

		game_interrupt(1000000);

		codl_set_cursor_position(gameover_win, 11, 4);
		codl_write(gameover_win, codl_itoa(count, count_str));

		codl_set_cursor_position(gameover_win, 5, 6);
		codl_write(gameover_win, "Retry? [y/n]");

		count = seconds - (int)(time(NULL) - first_time);

		if((key = codl_get_key())) {
			switch(key) {
				case 'y':
					codl_destroy_window(gameover_win);
					return(0);
				case 'n':
					codl_destroy_window(gameover_win);
					return(1);
			}
		}

		codl_display();
	}

	codl_destroy_window(gameover_win);
	return(1);

}


static void game(void) {
	const char tetr_bg = 4;
	char tetris_quit   = 0;
	unsigned int key   = 0;

	codl_window *main_win;
	codl_window *tetris_window_1;

	tetris_window main_tetris;

	codl_set_frame_symbols("│", "│", "─", "─", "┌", "┐", "└", "┘");
	codl_window_clear(codl_get_term());

	create_tetris_window(&main_tetris, 12, 20, 5);
	set_tetris_movement(&main_tetris, CODL_KEY_RIGHT, CODL_KEY_LEFT, CODL_KEY_UP, CODL_KEY_DOWN, CODL_KEY_LC_Q);

	setup_window(&main_win, &tetris_window_1, codl_get_term(), tetr_bg, &main_tetris, 
			(codl_get_term()->width  / 2) - (((main_tetris.field_width  * main_tetris.square_width)  + 15) / 2), 
			(codl_get_term()->height / 2) - (((main_tetris.field_height * main_tetris.square_height) + 2)  / 2));

	while(!tetris_quit) {
		game_interrupt(1000000);

		key = codl_get_key();

		if(codl_resize_term()) {
			codl_set_window_position(main_win, 
				(codl_get_term()->width  / 2) - (((main_tetris.field_width  * main_tetris.square_width)  + 15) / 2), 
				(codl_get_term()->height / 2) - (((main_tetris.field_height * main_tetris.square_height) + 2)  / 2));
			codl_redraw();
		}

		codl_window_clear(codl_get_term());
		tetris_force_down(&main_tetris);
		tetris_count(&main_tetris);


		if(key) {
			tetris_movement(key, &main_tetris);
		}

		tetris_display_buffer(&main_tetris);
		tetris_burn_line(&main_tetris);
		draw_tetris(&main_tetris, tetris_window_1, tetr_bg, 7);

		if(main_tetris.gameover) {
			switch(game_over(main_win)) {
				case 1:
					tetris_quit = 1;
					break;
				case 0:
					codl_set_colour(main_win, tetr_bg, 256);
					codl_rectangle(main_win,
						tetris_window_1->width + 2, 3, 
					        main_win->width - 1, 7, " ");
					tetris_clear_field(&main_tetris);
					break;
			}
		}

		if(main_tetris.tetr_exit) {
			break;
		}

		codl_display();
	}

	codl_destroy_window(main_win);
	codl_destroy_window(tetris_window_1);
	tetris_end(&main_tetris);
}


static void help(codl_window *ttw) {
	codl_window *help_win;
	int bg_col = 0;
	const int strings_count = 5;
	const int width = 23;
	const int height = 12;
	int count = 0;
        
	char *strings[5] = {
		"← - Move figure left",
		"↓ - Move figure down",
		"↑ - Rotate figure",
		"→ - Move figure right",
		"Q - Exit tetris"
	};

	codl_set_frame_symbols("│", "│", "─", "─", "┌", "┐", "└", "┘");
	codl_set_frame_colours(7, 8, 7, 8, 7, 8, 7, 8);

	help_win = codl_create_window(codl_get_term(), 2, 
			(codl_get_term()->width / 2) - width / 2,
			(codl_get_term()->height / 2) - height / 2, width, height);

	codl_set_colour(help_win, bg_col, 0);
	codl_rectangle(help_win, 0, 0, help_win->width, help_win->height, " ");
	codl_frame(help_win, 0, 0, help_win->width, help_win->height);

	codl_set_attribute(help_win, CODL_BOLD | CODL_DIM);
	codl_set_colour(help_win, bg_col, 14);
	codl_set_cursor_position(help_win, 6, 1);
	codl_write(help_win, "Help screen");

	codl_set_attribute(help_win, 0);

	for(; count < strings_count; ++count) {
		codl_set_cursor_position(help_win, 1, 3 + count);
		codl_write(help_win, strings[count]);
	}

	codl_set_cursor_position(help_win, 1, 3 + strings_count + 2);
	codl_write(help_win, "Press any key to quit");

	for(;;) {
		if(codl_get_key())
			break;

		if(codl_resize_term()) {
			codl_set_window_position(help_win, 
				(codl_get_term()->width  / 2) - (help_win->width  / 2), 
				(codl_get_term()->height / 2) - (help_win->height / 2));
			codl_set_window_position(ttw, 
				(codl_get_term()->width  / 2) - (ttw->width  / 2), 
				(codl_get_term()->height / 2) - (ttw->height / 2));

			codl_window_clear(codl_get_term());

		}

		codl_display();
	}

	codl_destroy_window(help_win);
}


static void sign_handler(int signum) {
	switch(signum) {
		case SIGCONT:
			codl_noecho();
			codl_cursor_mode(CODL_HIDE);
			codl_redraw();
		break;
		
		case SIGINT:
		case SIGTSTP:
		break;
	}
}


int main(void) {
	char quit_var = 0;
	codl_window *tetris_title_win = NULL;
	codl_window *tetris_menu      = NULL;

	srand((unsigned int)time(NULL));
	signal(SIGINT, sign_handler);
	signal(SIGTSTP, sign_handler);
	signal(SIGCONT, sign_handler);

	codl_cursor_mode(CODL_HIDE);
	codl_noecho();
	codl_initialize();

	tetris_title_win = codl_create_window(codl_get_term(), 1, 
			(codl_get_term()->width / 2) - 14, (codl_get_term()->height / 2) - 7, 28, 17);
	tetris_menu = codl_create_window(tetris_title_win, 2, 7, 8, 11, 3);

	while(!quit_var) {
		switch(game_menu(tetris_title_win, tetris_menu)) {
			case 0:
				codl_destroy_window(tetris_title_win);
				codl_destroy_window(tetris_menu);

				game();
				quit_var = 1;

				break;
			case 1:
				help(tetris_title_win);
				break;
			case 2:
				quit_var = 1;
				codl_destroy_window(tetris_title_win);
				codl_destroy_window(tetris_menu);
				break;
		}
	}

	codl_cursor_mode(CODL_SHOW);
	codl_end();
	codl_echo();
	codl_clear();

	return(0);
} 
