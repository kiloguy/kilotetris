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

Block create_predict(Block c, bool m[][10]){
	bool hit = false;

	do{
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				if(form[c.type][c.dir][i * 4 + j] && (m[c.line + i + 1][c.col + j] || c.line + i + 1 >= 20))
					hit = true;
			}
		}
		if(!hit)
			c.line++;
	}while(!hit);

	return c;
}

void draw_block_bitmap(ALLEGRO_BITMAP* bitmap, Block c, ALLEGRO_DISPLAY* md){
	al_set_target_bitmap(bitmap);
	al_clear_to_color(al_map_rgba(0, 0, 0, 0));
	if(c.type != NON){
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				if(form[c.type][c.dir][i * 4 + j])
					al_draw_filled_rectangle(j * 20, i * 20, j * 20 + 20, i * 20 + 20, color_scheme[c.type]);
			}
		}
	}
	al_set_target_backbuffer(md);
}

void move_block(Block* c, bool m[][10], int m_line, int m_col){
	/* move block by m_line and m_col, can be positive or negative. If hit, won't move. */
	bool hit = false;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(form[c->type][c->dir][i * 4 + j] && (c->line + i + m_line >= 20 || c->col + j + m_col >= 10 || c->col + j + m_col < 0 || m[c->line + i + m_line][c->col + j + m_col])){
				hit = true;
				break;
			}
		}
		if(hit)
			break;
	}
	if(!hit){
		c->line += m_line;
		c->col += m_col;
	}
}
