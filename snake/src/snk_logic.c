#include "snk.h"

static void __regen_fruit(snake *snk) {
		snk->fruit_pos[0] = rand() % (snk->f_width - 2) + 1;
		snk->fruit_pos[1] = rand() % (snk->f_height - 2) + 1;
}
 
void snake_grow_up(snake *snk) {
	int count;
	++snk->snake_size;

	snk->snake_arr = codl_realloc_check(snk->snake_arr, (size_t)snk->snake_size * sizeof(int*));
	snk->snake_arr[snk->snake_size - 1] = codl_malloc_check(2 * (int)sizeof(int));

	for(count = snk->snake_size - 1; count > 0; --count) {
		snk->snake_arr[count][0] = snk->snake_arr[count - 1][0];
		snk->snake_arr[count][1] = snk->snake_arr[count - 1][1];
	}

	snk->snake_arr[0][0] = snk->fruit_pos[0];
	snk->snake_arr[0][1] = snk->fruit_pos[1];

	for(count = 0; count < snk->snake_size; ++count) {
		if((snk->fruit_pos[0] == snk->snake_arr[count][0]) &&
				(snk->fruit_pos[1] == snk->snake_arr[count][1])) {
			__regen_fruit(snk);
			count = 0;
		}
	}

	snk->fruit_eaten = 1;
}

static void __add_snake_head_pos(snake *snk, int x_add, int y_add) {
	snk->snake_arr[0][0] = snk->snake_arr[1][0] + x_add;
	snk->snake_arr[0][1] = snk->snake_arr[1][1] + y_add;
}

static int __check_next_position(snake *snk, int x_add, int y_add) {
	int count;

	for(count = 0; count < snk->snake_size; ++count) {
		if((snk->snake_arr[0][0] + x_add == snk->snake_arr[count][0]) && 
				(snk->snake_arr[0][1] + y_add == snk->snake_arr[count][1])) {
			return(0);
		}
	}

	if(!snk->field_arr[snk->snake_arr[0][0] + x_add][snk->snake_arr[0][1] + y_add]) {
		if((snk->snake_arr[0][0] + x_add == snk->fruit_pos[0]) && (snk->snake_arr[0][1] + y_add == snk->fruit_pos[1]))
			snake_grow_up(snk);

		return(1);
	}

	return(0);
}

int test_snake_move(snake *snk, int dir) {
	switch(dir) {
		case 1: /* Rigth */
			return(__check_next_position(snk, 1, 0));
		case 2: /* Left  */
			return(__check_next_position(snk, -1, 0));
		case 3: /* Up    */
			return(__check_next_position(snk, 0, -1));
		case 4: /* Down  */
			return(__check_next_position(snk, 0, 1));
	}

	return(0);
}

int snake_move(snake *snk, codl_window *snk_win) {
	int count;
	
	if(game_counter(snk)) {
		if(!test_snake_move(snk, snk->direction)) return(0);
		if(snk->fruit_eaten) {
	        	snk->fruit_eaten = 0;
			draw_snake(snk, snk_win);
        		codl_display();

			return(1);
		}

		for(count = snk->snake_size - 1; count > 0; --count) {
			snk->snake_arr[count][0] = snk->snake_arr[count - 1][0];
			snk->snake_arr[count][1] = snk->snake_arr[count - 1][1];
		}

		switch(snk->direction) {
			case 1:
				__add_snake_head_pos(snk, 1, 0);
				codl_memcpy(snk->snake_head, 2, " :", 2);
				break;
			case 2:
				__add_snake_head_pos(snk, -1, 0);
				codl_memcpy(snk->snake_head, 2, ": ", 2);
				break;
			case 3:
				__add_snake_head_pos(snk, 0, -1);
				codl_memcpy(snk->snake_head, 2, "''", 2);
				break;
			case 4:
				__add_snake_head_pos(snk, 0, 1);
				codl_memcpy(snk->snake_head, 2, "..", 2);
				break;
		}
	
		draw_snake(snk, snk_win);
		codl_display();
		snk->wait_move = 0;
	}

	return(1);
}

int snake_movement(snake *snk, unsigned int ch) {
	switch(ch) {
		case CODL_KEY_RIGHT:
			if((snk->direction != 2) && !snk->wait_move) {
				snk->direction = 1;
				snk->wait_move = 1;
			}

			break;
		case CODL_KEY_LEFT:
			if((snk->direction != 1) && !snk->wait_move) {
				snk->direction = 2;
				snk->wait_move = 1;
			}

			break;
		case CODL_KEY_UP:
			if((snk->direction != 4) && !snk->wait_move) {
				snk->direction = 3;
				snk->wait_move = 1;
			}

			break;
		case CODL_KEY_DOWN:
			if((snk->direction != 3) && !snk->wait_move) {
				snk->direction = 4;
				snk->wait_move = 1;
			}

			break;
		case CODL_KEY_LC_Q:
			return(0);
	}
	

	return(1);
}

void end_snake(snake *snk) {
	int count;
	for(count = 0; count < snk->snake_size; ++count) {
		free(snk->snake_arr[count]);
	}

	free(snk->snake_arr);

	for(count = 0; count < snk->f_width; ++count) {
		free(snk->field_arr[count]);
	}

	free(snk->field_arr);
}

