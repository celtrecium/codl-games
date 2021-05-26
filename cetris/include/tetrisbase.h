#ifndef TETRISBASE_H
#define TETRISBASE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "codl.h"

typedef struct tetris_move {
	unsigned int t_right,
		t_left,
		t_up,
		t_down,
		t_quit;
} tetris_move;

typedef struct tetris_window {
	int field_width;
	int field_height;
	int screen_width;
	int screen_height;

	int square_width;
	int square_height;

	char *field;
	char *screen;
	int gameover;

	int curr_tetr;
	int curr_rot;
	int curr_x_pos;
	int curr_y_pos;
	int speed;
	int speed_count;
	int force_down;
	int rot_hold;
	int tetr_count;
	int score;
	int line_arr_size;
	int line_count;
	int *line;
	tetris_move tetr_movement;
	char tetr_exit;
} tetris_window;


void set_tetris_movement(tetris_window *wi, unsigned int right, unsigned int left, unsigned int up, unsigned int down, unsigned int quit);
void tetris_end(tetris_window *wi);
int  rotate_tetromino(int x_pos, int y_pos, int deg);
void tetris_force_down(tetris_window *tw);
void create_tetris_window(tetris_window *twin, int field_width, int field_height, int speed);
tetris_move tetris_create_movement(char t_right, char t_left, char t_up, char t_down, char t_quit);
void tetris_movement(unsigned int sym, tetris_window *win);
void tetris_display_buffer(tetris_window *wt);
void tetris_count(tetris_window *wt);
void tetris_burn_line(tetris_window *wt);
void tetris_clear_field(tetris_window *twin);

#endif /* TETRISBASE_H */
