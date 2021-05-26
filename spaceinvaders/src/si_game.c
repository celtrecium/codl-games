#include "si_game.h"

int init_invaders(invaders *invstrct, player_struct *plr, int vert_val, int hor_val,
		int t_width, int t_height, int invoffs, char *textures[]) {
	int count;
	int count_1;
	game_object *ptr;

	seng_create_object(&invstrct->bullet, 0, 0, 1, 1, 1, 1, SENG_DISABLE, SENG_ENABLE, 5);
	codl_set_window_visibility(invstrct->bullet.texture, CODL_DISABLE);
	invstrct->bullet.direction = 4;
	invstrct->bullet.dead = 1;

	invstrct->horizontal_val    = hor_val;
	invstrct->vertical_val      = vert_val;
	invstrct->t_width           = t_width;
	invstrct->t_height          = t_height;
	invstrct->offset            = invoffs;
	invstrct->offset_counter    = 0;
	invstrct->direction         = 1;
	invstrct->counter_moves_fwd = 0;

	invstrct->invaders = codl_malloc_check((size_t)(hor_val * vert_val) * sizeof(game_object));

	for(count = 0; count < vert_val; ++count) {
		for(count_1 = 0; count_1 < hor_val; ++count_1) {
			ptr = &invstrct->invaders[(count * hor_val) + count_1];
			seng_create_object(ptr, (count_1 * (t_width + 2)), (count * (t_height + 1)),
					t_width, t_height, 1, 1, SENG_ENABLE, SENG_ENABLE, 1);

			ptr->direction = 1;
			seng_load_texture(ptr, textures[count]);
			seng_set_obj_enemy(&plr->bullet, ptr);
			seng_set_obj_enemy(ptr, &plr->player);
			seng_set_obj_enemy(ptr, &plr->bullet);
		}
	}

	seng_set_obj_enemy(&invstrct->bullet, &plr->bullet);
	seng_set_obj_enemy(&invstrct->bullet, &plr->player);

	return(1);
}

int init_player(player_struct *plrs, int t_width, int t_height, int f_width, int f_height, char *texture) {
	seng_create_object(&plrs->player, (f_width - t_width) / 2, f_height - t_height,
			t_width, t_height, 1, 1, SENG_ENABLE, SENG_ENABLE, 1);
	seng_create_object(&plrs->bullet, 0, 0, 1, 1, 1, 1, SENG_DISABLE, SENG_ENABLE, 3);
	seng_load_texture(&plrs->player, texture);
	plrs->bullet.direction = 3;
	plrs->bullet.dead = 1;
	codl_set_window_visibility(plrs->bullet.texture, CODL_DISABLE);
	
	return(1);
}

int init_shields(shields_struct *shlds, player_struct *plrs, invaders *invs, int f_width, int f_height,
		int num, int t_width, int t_heigth, char *texture) {
	int count;

	shlds->shields = codl_malloc_check((size_t)num * sizeof(game_object));

	for(count = 0; count < num; ++count) {
		seng_create_object(&shlds->shields[count], count * ((f_width - (num * t_width)) / (num - 1) + t_width),
				f_height - 8, t_width, t_heigth, 5, 1, SENG_ENABLE, SENG_DISABLE, 1);	
		seng_load_texture(&shlds->shields[count], texture);
		seng_set_obj_enemy(&invs->bullet, &shlds->shields[count]);
		seng_set_obj_enemy(&plrs->bullet, &shlds->shields[count]);
	}

	return(1);
}

int move_invaders(invaders *invs, int f_height) {
	int count;
	const int invs_value = invs->horizontal_val * invs->vertical_val;
	++invs->offset_counter;

	for(count = 0; count < invs_value; ++count) {
		seng_obj_move(&invs->invaders[count]);
		if(invs->invaders[count].pos_y == f_height - 5) return(0);
	}

	if((invs->offset_counter >= invs->offset - 1) && (invs->direction == 1)) {
		for(count = 0; count < invs_value; ++count) {
			invs->invaders[count].direction = 4;
			seng_obj_move(&invs->invaders[count]);
			seng_render();
			invs->invaders[count].direction = 2;
			invs->direction = 2;
			invs->offset_counter = 0;
		}
			
	} else if((invs->offset_counter >= invs->offset - 1) && (invs->direction == 2)) {
		for(count = 0; count < invs_value; ++count) {
			invs->invaders[count].direction = 4;
			seng_obj_move(&invs->invaders[count]);
			seng_render();
			invs->invaders[count].direction = 1;
			invs->direction = 1;
			invs->offset_counter = 0;
		}
	}

	return(1);
}

int spawn_invader_bullet(invaders *invs) {
	int value;
	int count;
	const int invs_value = invs->horizontal_val * invs->vertical_val;

	if(!invs->bullet.dead) return(0);
	
	for(count = 0; count < invs_value; ++count) {
		if(!invs->invaders[count].dead) break;
		if(count == 29) return(0);
	}

	for(;;) {
		value = rand() % invs_value;
		if(!invs->invaders[value].dead) {
			invs->bullet.dead = 0;
			invs->bullet.health = 1;
			invs->bullet.pos_x = invs->invaders[value].pos_x + invs->invaders[value].hitbox_width / 2;
			invs->bullet.pos_y = invs->invaders[value].pos_y + invs->invaders[value].hitbox_height;
			codl_set_window_visibility(invs->bullet.texture, CODL_ENABLE);
			seng_render();

			return(1);
		}
	}

	return(0);
}

int control_player(player_struct *plrs, unsigned int ch) {
	switch(ch) {
		case CODL_KEY_RIGHT: /* RIGHT */
			if((plrs->player.pos_x + plrs->player.hitbox_width) < seng_get_window()->width) {
				seng_set_obj_position(&plrs->player, 1, 0);
			}

			break;

		case CODL_KEY_LEFT: /* LEFT */
			if(plrs->player.pos_x > 0) {
				seng_set_obj_position(&plrs->player, -1, 0);
			}

			break;

		case CODL_KEY_SPACE:
			if(plrs->bullet.dead) {
				plrs->bullet.direction  = 3;
				plrs->bullet.dead       = 0;
				plrs->bullet.health     = 1;
				plrs->bullet.pos_x = plrs->player.pos_x + plrs->player.hitbox_width / 2;
				plrs->bullet.pos_y = plrs->player.pos_y;
				codl_set_window_visibility(plrs->bullet.texture, CODL_ENABLE);
				seng_render();
			}

			break;

		case CODL_KEY_LC_Q:
		case CODL_KEY_UC_Q:
			return(0);
	}

	return(1);
}

void set_bullet_dead(game_object *gobj) {
	gobj->dead = 1;
	codl_set_window_visibility(gobj->texture, CODL_DISABLE);
}
