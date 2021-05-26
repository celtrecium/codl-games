#include "si_main.h"

static void __sign_handler(int signum) {
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

void init_game(si_struct *si, int f_width, int f_height) {
	char *inv_textures[] = {"textures/inv1.cdl", "textures/inv2.cdl", "textures/inv3.cdl"};
	char *bgfilename     =  "textures/background.cdl";
	char *plrtexture     =  "textures/player.cdl";
	char *shldtexture    =  "textures/shield1.cdl";

	srand((unsigned int)time(NULL));

	signal(SIGINT, __sign_handler);
	signal(SIGTSTP, __sign_handler);
	signal(SIGCONT, __sign_handler);
	
	si->F_WIDTH  = f_width;
	si->F_HEIGHT = f_height;

	seng_init(f_width, f_height);
	seng_load_background(bgfilename);
	
	init_player(&si->plrs, 7, 2, f_width, f_height, plrtexture);
	init_invaders(&si->invs, &si->plrs, 3, 10, 5, 2, 13, inv_textures);
	init_shields(&si->shlds, &si->plrs, &si->invs, f_width, f_height, 4, 10, 3, shldtexture);

	seng_render();
}

int loop_game(si_struct *si) {
	int end = 0;
	int count;
	unsigned int key = 0;
	const int INVS_VAL = si->invs.horizontal_val * si->invs.vertical_val;

	for(;;) {
		game_interrupt(10000000);

		key = codl_get_key();

		seng_term_resize_check();
		if(!(seng_get_game_counter() % 60) && !si->plrs.player.dead) {
			if(!move_invaders(&si->invs, si->F_HEIGHT)) break;
			seng_render();
		}

		if(key) {
			if(!control_player(&si->plrs, key)) break;
			seng_render();
		}

		if((si->plrs.bullet.pos_y < 0) && !si->plrs.bullet.dead) {
			set_bullet_dead(&si->plrs.bullet);
			seng_render();
		}

		if((si->invs.bullet.pos_y > si->F_HEIGHT) && !si->invs.bullet.dead) {
			set_bullet_dead(&si->invs.bullet);
			seng_render();
		}
		
		if(seng_move(&si->plrs.bullet)) seng_render();
		if(seng_move(&si->invs.bullet)) seng_render();
		if(!(seng_get_game_counter() % 160)) spawn_invader_bullet(&si->invs);
		if(si->plrs.player.dead) return(0);

		if(!end) {
			for(count = 0; count < INVS_VAL; ++count) {
				if(!si->invs.invaders[count].dead) break;
				if(count == INVS_VAL - 1) {
					end = 1;
				}
			}
		} else return(1);

		seng_game_counter();
	}

	return(0);
}

void end_game(si_struct *si) {
	seng_end();
	free(si->shlds.shields);
	free(si->invs.invaders);
}
