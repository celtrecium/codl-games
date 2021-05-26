#ifndef tetrisinterface_h_INCLUDED
#define tetrisinterface_h_INCLUDED

#include <stdio.h>
#include <time.h>
#include <codl.h>
#include "tetrisbase.h"

/* void create_shadow_frame_1(unsigned char **arr, unsigned char bg); */
void game_interrupt(unsigned int num);
void draw_tetris(tetris_window *wt, codl_window *wi, int tetris_bg, int tetris_fg);
void setup_window(codl_window **win, codl_window **tet_win, codl_window *p_win, char bg, tetris_window *wt, int x_pos, int y_pos);
int menu_function(codl_window *win, int colour_bg_act, int colour_fg_act, int colour_bg, int colour_fg, char *list);

#endif /* tetrisinterface_h_INCLUDED */
