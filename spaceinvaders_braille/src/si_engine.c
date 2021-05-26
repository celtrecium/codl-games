#include "si_engine.h"

static obj_list intern_list;
static codl_window *game_window;
static int game_counter = 0;
static int __seng_check_next_pos(game_object *gobj, int x_add, int y_add);

void game_interrupt(unsigned int num) {
	struct timespec interrupt = {0, 0};
	struct timespec ret;
	interrupt.tv_nsec = num;
	nanosleep(&interrupt, &ret);

	return;
}

int seng_term_resize_check(void) {
	if(codl_resize_term()) {
		codl_set_window_position(game_window, 
			(codl_get_term()->width  / 2) - game_window->width / 2, 
			(codl_get_term()->height / 2) - game_window->height / 2);
		codl_display();

		return(1);
	}

	return(0);
}

int seng_init(int width, int height) {
	if(!codl_initialize()) return(0);
	codl_cursor_mode(CODL_HIDE);
	codl_noecho();

	game_window = codl_create_window(codl_get_term(), 0, codl_get_term()->width / 2 - width / 2, 
			codl_get_term()->height / 2 - height / 2, width, height);

	codl_set_colour(game_window, 6, 256);
	codl_rectangle(game_window, 0, 0, width, height, " ");

	intern_list.size = 0;

	return(1);
}

int seng_end() {
	int count = 0;
	
	if(intern_list.list) {
		for(count = 0; count < intern_list.size; ++count) {
			if(intern_list.list[count]->enemy_objs) {
				free(intern_list.list[count]->enemy_objs);
			}
		}

		if(intern_list.size) free(intern_list.list);
	}

	codl_cursor_mode(CODL_SHOW);
	codl_echo();
	codl_end();
	codl_clear();

	return(1);
}

int seng_create_object(game_object *gobj, int pos_x, int pos_y, int width, int height,
		int health, int dmg, SENG_SWITCH cllsn, SENG_SWITCH slfdstr, int speed) {
	gobj->pos_x         = pos_x;
	gobj->pos_y         = pos_y;
	gobj->hitbox_width  = width;
	gobj->hitbox_height = height;
	gobj->health        = health;
	gobj->damage        = dmg;
	gobj->collision     = cllsn;
	gobj->self_destroy  = slfdstr;
	gobj->direction     = 0;
	gobj->val_enemys    = 0;
	gobj->enemy_objs    = NULL;
	gobj->speed         = speed;
	gobj->update        = 0;
	gobj->dead          = 0;

	gobj->texture = codl_create_window(game_window, intern_list.size + 1, pos_x, pos_y, width, height);
	codl_set_colour(gobj->texture, 14, 256);
	codl_rectangle(gobj->texture, 0, 0, width, height, " ");
	codl_set_alpha(gobj->texture, CODL_ENABLE);

	++intern_list.size;
	if(!intern_list.list) {
		intern_list.list = codl_malloc_check((int)sizeof(game_object*));
		intern_list.list[intern_list.size - 1] = gobj;
	} else {
		intern_list.list = codl_realloc_check(intern_list.list, (size_t)intern_list.size * sizeof(game_object*));
		intern_list.list[intern_list.size - 1] = gobj;
	}

	return(1);
}

int seng_destroy_object(game_object *gobj) {
	int count = 0;
	int curr_obj = 0;

	if(gobj->dead) return(0);

	codl_destroy_window(gobj->texture);
	
	if(gobj->enemy_objs) {
		free(gobj->enemy_objs);
	}

	for(count = 0; count < intern_list.size; ++count) {
		if(intern_list.list[count] == gobj) {
			curr_obj = count;
			break;
		}
	}

	for(count = curr_obj; count < intern_list.size - 1; ++count) {
		intern_list.list[count] = intern_list.list[count + 1];
	}

	--intern_list.size;
	if(intern_list.size) {
		intern_list.list = codl_realloc_check(intern_list.list, (size_t)intern_list.size * sizeof(game_object*));
	} else {
		free(intern_list.list);
	}

	return(1);
}

static int __seng_check_next_pos(game_object *gobj, int x_add, int y_add) {
	int count = 0;
	int count_1 = 0;
	game_object *ptr;

	if(gobj->dead) return(0);

	for(count = 0; count < intern_list.size; ++count) {
		ptr = intern_list.list[count];

		if(((gobj->pos_x + gobj->hitbox_width + x_add > ptr->pos_x) && (gobj->pos_x + x_add < ptr->pos_x + ptr->hitbox_width)) && 
				((gobj->pos_y + gobj->hitbox_height + y_add > ptr->pos_y) && 
				 (gobj->pos_y + y_add < ptr->pos_y + ptr->hitbox_height)) && (ptr != gobj) && !ptr->dead) {

			for(count_1 = 0; count_1 < gobj->val_enemys; ++count_1) {
				if(gobj->enemy_objs[count_1] == ptr) {
					ptr->health -= gobj->damage;

					if(ptr->health <= 0) {
						codl_set_window_visibility(ptr->texture, CODL_DISABLE);
						ptr->dead = 1;
					}

					if(gobj->self_destroy) {
						codl_set_window_visibility(gobj->texture, CODL_DISABLE);
						gobj->health = 0;
						gobj->dead = 1;
					}

					return(0);
				}
			}
			
			if(ptr->collision && gobj->collision) return(0);
		}
	}

	return(1);
}

int seng_set_obj_position(game_object *gobj, int x_add, int y_add) {
	if(__seng_check_next_pos(gobj, x_add, y_add)) {
		gobj->pos_x += x_add;
		gobj->pos_y += y_add;

		codl_set_window_position(gobj->texture,
			gobj->pos_x, gobj->pos_y);
	}

	return(1);
}

int seng_obj_move(game_object *gobj) {
	if(gobj->dead) return(1); 

	switch(gobj->direction) {
		case 1: /* RIGHT */
			seng_set_obj_position(gobj, 1, 0);
			break;
		case 2: /* LEFT */
			seng_set_obj_position(gobj, -1, 0);
			break;
		case 3: /* UP */
			seng_set_obj_position(gobj, 0, -1);
			break;
		case 4: /* DOWN */
			seng_set_obj_position(gobj, 0, 1);
			break;
	}

	return(1);
}

int seng_render() {
	if(!codl_display()) return(0);

	return(1);
}

int seng_load_texture(game_object *gobj, char *texture_file_name) {
	codl_window_clear(gobj->texture);
	if(!codl_load_buffer_from_file(gobj->texture, texture_file_name, 0, 0)) return(0);

	return(1);
}

int seng_load_background(char *filename) {
	if(!codl_load_buffer_from_file(game_window, filename, 0, 0)) return(0);

	return(1);
}

int seng_set_obj_enemy(game_object *gobj, game_object *enemy) {
	++gobj->val_enemys;

	if(!gobj->enemy_objs) {
		gobj->enemy_objs = codl_malloc_check((int)sizeof(game_object*));
	} else {
		gobj->enemy_objs = codl_realloc_check(gobj->enemy_objs, (size_t)gobj->val_enemys * sizeof(game_object*));
	}

	gobj->enemy_objs[gobj->val_enemys - 1] = enemy;

	return(1);
}

int seng_move(game_object *gobj) {
	if(!(game_counter % gobj->speed)) {
		seng_obj_move(gobj);

		return(1);
	}

	return(0);
}



int seng_game_counter(void) {
	++game_counter;
	
	if(game_counter == 2000000000) game_counter = 0;
	
	return(game_counter);
}

int seng_get_game_counter(void) {
	return(game_counter);
}

codl_window *seng_get_window(void) {
	return(game_window);
}

