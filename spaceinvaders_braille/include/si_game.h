#ifndef SI_GAME_H
#define SI_GAME_H

#include "si_engine.h"

#define RIGHT 67
#define LEFT  68
#define UP    65
#define DOWN  66

typedef struct invaders {
	game_object *invaders;
	game_object bullet;
	int offset;
	int offset_counter;
	int direction;
	int counter_moves_fwd;
	int vertical_val;
	int horizontal_val;
	int t_width;
	int t_height;
} invaders;

typedef struct player_struct {
	game_object player;
	game_object bullet;
} player_struct;

typedef struct shields_struct {
	game_object *shields;
	int value;
} shields_struct;

int init_invaders(invaders *invstrct, player_struct *plr, int vert_val, int hor_val,
		int t_width, int t_height, int invoffs, char *bllt_texture, char *textures[]);
int init_player(player_struct *plrs, int t_width, int t_height, int f_width, int f_height, char *bllt_texture, char *texture);
int init_shields(shields_struct *shlds, player_struct *plrs, invaders *invs, int f_width, int f_height,
		int num, int t_width, int t_heigth, char *texture);
int move_invaders(invaders *invs, int f_height);
int spawn_invader_bullet(invaders *invs);
int control_player(player_struct *plrs, unsigned int ch);
void set_bullet_dead(game_object *gobj);

#endif /* SI_GAME_H */
