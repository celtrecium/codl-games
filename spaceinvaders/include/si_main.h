#ifndef SI_MAIN_H
#define SI_MAIN_H

#include "si_game.h"
#include <signal.h>

typedef struct si_struct {
	invaders invs;
	player_struct plrs;
	shields_struct shlds;
	int F_WIDTH;
	int F_HEIGHT;
} si_struct;

void init_game(si_struct *si, int f_width, int f_height);
int loop_game(si_struct *si);
void end_game(si_struct *si);

#endif /* SI_MAIN_H */
