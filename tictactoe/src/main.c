#define _POSIX_C_SOURCE 199309L

#include <codl.h>
#include <time.h>
#include <signal.h>

#define TTT_WIDTH  3
#define TTT_HEIGHT 3

#define RIGHT 67
#define LEFT  68
#define UP    65
#define DOWN  66

typedef struct cell_pos {
	int row;
	int column;
} cell_pos;

void game_interrupt(unsigned int num) {
	struct timespec interrupt = {0, 0};
	struct timespec ret;
	interrupt.tv_nsec = num;
	nanosleep(&interrupt, &ret);

	return;
}

static int select_cell(codl_window *xo_win, cell_pos *cp, codl_window *main_win) {
	unsigned int key = 0;

	codl_set_colour(xo_win, 1, 15);
	codl_replace_attributes(xo_win, cp->column * 4, cp->row * 2, cp->column * 4 + 2, cp->row * 2);
	codl_display();

	for(;;) {
		if(codl_resize_term()) {
			codl_set_window_position(main_win, 
				(codl_get_term()->width  / 2) - 7, 
				(codl_get_term()->height / 2) - 5);
			codl_display();
		}

		if((key = codl_get_key())) {
			codl_set_colour(xo_win, 256, 256);

			switch(key) {
				case CODL_KEY_LEFT:
					codl_replace_attributes(xo_win, 0, 0, xo_win->width, xo_win->height);
					if(cp->column) --cp->column;
					break;
				case CODL_KEY_RIGHT:
					codl_replace_attributes(xo_win, 0, 0, xo_win->width, xo_win->height);
					if(cp->column < TTT_WIDTH - 1) ++cp->column;
					break;
				case CODL_KEY_UP:
					codl_replace_attributes(xo_win, 0, 0, xo_win->width, xo_win->height);
					if(cp->row) --cp->row;
					break;
				case CODL_KEY_DOWN:
					codl_replace_attributes(xo_win, 0, 0, xo_win->width, xo_win->height);
					if(cp->row < TTT_HEIGHT - 1) ++cp->row;
					break;
				case '\n':
					codl_set_colour(xo_win, 1, 15);
					return(1);
				case 'q':
					return(0);
			}
			
			codl_set_colour(xo_win, 1, 15);
			codl_replace_attributes(xo_win, cp->column * 4, cp->row * 2, cp->column * 4 + 2, cp->row * 2);
			codl_display();
		}
		
		game_interrupt(5000000);
	}
}

static void draw_winner_line(codl_window *xo_win, int x0, int y0, int x1, int y1) {
	int count   = 0;
	int count_1 = 0;

	codl_set_colour(xo_win, 1, 256);
	for(count = y0; count <= y1; ++count) {
		for(count_1 = x0; count_1 <= x1; ++count_1) {
			codl_replace_attributes(xo_win, count_1 * 4, count * 2,
					count_1 * 4 + 3, count * 2);
		}
	}
}

static void draw_winner_line_d(codl_window *xo_win, int mode) {
	int count   = 0;

	codl_set_colour(xo_win, 1, 256);
	if(mode) {
		for(count = 0; count < TTT_WIDTH; ++count) {
			codl_replace_attributes(xo_win, count * 4, count * 2,
					count * 4 + 3, count * 2);
		}
	} else {
		for(count = TTT_WIDTH; count >= 0; --count) {
			codl_replace_attributes(xo_win, count * 4, (TTT_WIDTH - count - 1) * 2,
					count * 4 + 3, (TTT_WIDTH - count - 1) * 2);
		}
	}
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
	codl_window *main_win;
	codl_window *ttt_win;
	codl_window *xo_win;
	cell_pos c_p = {0, 0};
	int player   = 0;
	int cell_val;
	int moves = TTT_HEIGHT * TTT_WIDTH;
	int count;
	int count_1;
	int xo_arr[TTT_HEIGHT][TTT_WIDTH] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
	int quit = 0;
	int neutral = 0;

	signal(SIGINT, sign_handler);
	signal(SIGTSTP, sign_handler);
	signal(SIGCONT, sign_handler);

	codl_cursor_mode(CODL_HIDE);
	codl_noecho();
	codl_initialize();
	main_win = codl_create_window(codl_get_term(), 1, (codl_get_term()->width / 2 - 7),
			(codl_get_term()->height / 2) - 5, 13, 10);
	ttt_win  = codl_create_window(main_win, 2, 1, 1, 11, 5);
	xo_win   = codl_create_window(ttt_win, 3, 0, 0, 11, 5);
	codl_set_alpha(xo_win, CODL_ENABLE);

	codl_set_colour(ttt_win,  256, 15);
	codl_set_colour(main_win, 256, 14);
	codl_set_attribute(main_win, CODL_BOLD);
	codl_write(main_win, "  TicTacToe \n");
	codl_write(ttt_win,  "   │   │   \n");
	codl_write(ttt_win,  "───┼───┼───\n");
	codl_write(ttt_win,  "   │   │   \n");
	codl_write(ttt_win,  "───┼───┼───\n");
	codl_write(ttt_win,  "   │   │   \n");
	codl_set_colour(main_win, 256, 256);
	codl_set_attribute(main_win, CODL_NO_ATTRIBUTES);

	codl_set_attribute(xo_win, CODL_BOLD);
	for(count = 0; count < TTT_HEIGHT; ++count) {
		for(count_1 = 0; count_1 < TTT_WIDTH; ++count_1) {
			codl_set_cursor_position(xo_win, count_1 * 4, count * 2);
			codl_write(xo_win, "   ");
		}
	}

	codl_display();
	while(!quit) {
		if(select_cell(xo_win, &c_p, main_win)) {
			if(!xo_arr[c_p.row][c_p.column]) {
				--moves;
				player = !player;
				codl_set_cursor_position(xo_win, c_p.column * 4 + 1, c_p.row * 2);
				codl_write(xo_win, player ? "X" : "0");
				xo_arr[c_p.row][c_p.column] = player + 1;

				cell_val = xo_arr[0][0];
				
				/* check diagonal 1 */
				for(count = 1; (count < TTT_WIDTH) && !quit; ++count) {
					if((cell_val == xo_arr[count][count]) && xo_arr[count][count]) {
						if(count == TTT_WIDTH - 1) {
							draw_winner_line_d(xo_win, 1);
							
							quit = 1;
							break;
						}
					} else break;
				}

				/* check diagonal 2 */
				cell_val = xo_arr[0][TTT_HEIGHT - 1];
				for(count = 1; (count < TTT_WIDTH) && !quit; ++count) {
					if((cell_val == xo_arr[count][TTT_WIDTH - count - 1]) && xo_arr[count][TTT_WIDTH - count - 1]) {
						if(count == TTT_WIDTH - 1) {
							draw_winner_line_d(xo_win, 0);
							
							quit = 1;
							break;
						}
					} else break;
				}

			
				/*check horizontal */
				for(count = 0; (count < TTT_HEIGHT) && !quit; ++count) {
					cell_val = xo_arr[count][0];
					for(count_1 = 1; count_1 < TTT_WIDTH; ++count_1) {
						if((cell_val == xo_arr[count][count_1]) && xo_arr[count][count_1]) {
							if(count_1 == TTT_WIDTH - 1) {
								draw_winner_line(xo_win, 0, count, TTT_WIDTH, count);

								quit = 1;
								break;
							}
						} else break;
					}
				}

				/* check vertical */
				for(count = 0; (count < TTT_WIDTH) && !quit; ++count) {
					cell_val = xo_arr[0][count];
					for(count_1 = 1; count_1 < TTT_HEIGHT; ++count_1) {
						if((cell_val == xo_arr[count_1][count]) && xo_arr[count_1][count]) {
							if(count_1 == TTT_HEIGHT - 1) {
								draw_winner_line(xo_win, count, 0, count, TTT_HEIGHT);

								quit = 1;
								break;
							}
						} else break;
					}
				}	
				
				codl_display();

				if(!moves && !quit) {
					quit    = 1;
					neutral = 1;
				};
			}
		} else {
			quit    = 1;
			neutral = 1;
		}
	}

	codl_set_cursor_position(main_win, 0, ttt_win->height + 1);
	if(!neutral) {
		codl_write(main_win, "Player ");
		codl_write(main_win, player ? "X win!" : "0 win!");
	} else {
		codl_write(main_win, "  Neutral:)");
	}

	codl_display();
	codl_end();
	codl_echo();
	codl_cursor_mode(CODL_SHOW);

	return(0);
}
