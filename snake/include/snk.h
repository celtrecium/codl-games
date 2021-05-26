#ifndef SNK_H
#define SNK_H

#define _POSIX_C_SOURCE 199309L

#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <codl.h>

#define RIGHT 67
#define LEFT  68
#define UP    65
#define DOWN  66

#define SNAKE_BODY_COLOUR 31
#define SNAKE_HEAD_COLOUR 24
#define SNAKE_EYES_COLOUR 38
#define SNAKE_BG_COLOUR   234
#define SNAKE_WALL_COLOUR 38
#define SNAKE_FOOD_COLOUR 1

typedef struct snake {
	int **snake_arr;
	char **field_arr;
	int snake_size;
	int f_width;
	int f_height;
	int score;
	int fruit_pos[2];
	int fruit_eaten;
	int direction;
	int counter;
	int speed;
	int wait_move;
	char snake_head[3];
	int prev_move;
} snake;

void game_interrupt(unsigned int num);
int  game_counter(snake *snk);
void snake_grow_up(snake *snk);
int  test_snake_move(snake *snk, int dir);
int  snake_move(snake *snk, codl_window *snk_win);
int  snake_movement(snake *snk, unsigned int ch);
void end_snake(snake *snk);
void draw_field(snake *snk, codl_window *f_win);
void draw_snake(snake *snk, codl_window *snk_win);
void init_snk(snake *snk, codl_window **main_win, codl_window **f_win, codl_window **snk_win, int width, int height);
void setup_snake(snake *snk, int f_width, int f_height, int speed);
void game_loop(snake *snk, codl_window *snk_win, codl_window *main_win);
void end_snk(snake *snk);

#endif
