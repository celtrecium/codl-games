#ifndef SI_ENGINE_H
#define SI_ENGINE_H

#define _POSIX_C_SOURCE 199309L

#include <codl.h>
#include <time.h>

typedef enum SENG_SWITCH {
	SENG_DISABLE,
	SENG_ENABLE
} SENG_SWITCH;

typedef struct game_object {
	int pos_x;
	int pos_y;
	int hitbox_width;
	int hitbox_height;
	int health;
	int damage;
	int dead;
	SENG_SWITCH collision;
	SENG_SWITCH self_destroy;
	int direction;
	codl_window *texture;
	struct game_object **enemy_objs;
	int val_enemys;
	int speed;
	int update;
} game_object;

typedef struct obj_list {
	game_object **list;
	int size;
} obj_list;

void game_interrupt(unsigned int num);
int  seng_term_resize_check(void);
int  seng_init(int width, int height);
int  seng_end();
int  seng_create_object(game_object *gobj, int pos_x, int pos_y, int width, int height,
		int health, int dmg, SENG_SWITCH cllsn, SENG_SWITCH slfdstr, int speed);
int  seng_destroy_object(game_object *gobj);
int  seng_set_obj_position(game_object *gobj, int x_add, int y_add);
int  seng_obj_move(game_object *gobj);
int  seng_render();
int  seng_load_texture(game_object *gobj, char *texture_file_name);
int  seng_load_background(char *filename);
int  seng_set_obj_enemy(game_object *gobj, game_object *enemy);
int  seng_move(game_object *gobj);
int  seng_game_counter(void);
int  seng_get_game_counter(void);
codl_window *seng_get_window(void);

#endif /* SI_ENGINE_H */
