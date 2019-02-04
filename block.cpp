#include	<random>
#include	<allegro5/allegro.h>
#include	<allegro5/allegro_primitives.h>
#include	"block.h"

extern ALLEGRO_COLOR color_scheme[];

int form[7][4][16] = {{{0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0}, {0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0}, {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0}, {0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}},
				 	  {{1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0}, {0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0}, {0,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0}, {0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0}},
				      {{0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0}, {0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0}, {0,0,0,0,1,1,1,0,1,0,0,0,0,0,0,0}, {1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0}},
				      {{0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0}, {0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0}, {0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0}, {0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0}},
					  {{0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0}, {0,1,0,0,0,1,1,0,0,0,1,0,0,0,0,0}, {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0}, {1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0}},
					  {{0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0}, {0,1,0,0,0,1,1,0,0,1,0,0,0,0,0,0}, {0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0}, {0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0}},
					  {{1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0}, {0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0}, {0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0}, {0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0}}};

void set_color_scheme(ALLEGRO_COLOR* s){
	s[I] = al_map_rgb(108, 214, 216);
	s[J] = al_map_rgb(108, 166, 216);
	s[L] = al_map_rgb(206, 153, 103);
	s[O] = al_map_rgb(232, 222, 116);
	s[S] = al_map_rgb(137, 232, 120);
	s[T] = al_map_rgb(213, 176, 232);
	s[Z] = al_map_rgb(234, 150, 150);
}

Block create_block(){
	std::random_device rd;
	std::default_random_engine gen = std::default_random_engine(rd());
	std::uniform_int_distribution<int> type_dis(0, 6);
	std::uniform_int_distribution<int> dir_dis(0, 3);
	Block ret{0, 3, type_dis(gen), dir_dis(gen)};
	return ret;
}

void draw_block_bitmap(ALLEGRO_BITMAP* bitmap, Block c, ALLEGRO_DISPLAY* md){
	al_set_target_bitmap(bitmap);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(form[c.type][c.dir][i * 4 + j])
				al_draw_filled_rectangle(j * 20, i * 20, j * 20 + 20, i * 20 + 20, color_scheme[c.type]);
		}
	}
	al_set_target_backbuffer(md);
}
