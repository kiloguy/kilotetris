#include	<stdio.h>
#include	<allegro5/allegro.h>
#include	<allegro5/allegro_primitives.h>
#include	<allegro5/allegro_font.h>
#include	<allegro5/allegro_ttf.h>
#include	"block.h"

ALLEGRO_COLOR	black;
ALLEGRO_COLOR	color_scheme[7];
extern int form[][4][16];

int main(){
	if(!al_init()){
		fprintf(stderr, "Allegro init failed.\n");
		return -1;
	}
	if(!al_init_primitives_addon()){
		fprintf(stderr, "Allegro init primitives addon failed.\n");
		return -1;
	}
	if(!al_install_keyboard()){
		fprintf(stderr, "Allegro keyboard init failed.\n");
		return -1;
	}
	if(!al_init_font_addon() || !al_init_ttf_addon()){
		fprintf(stderr, "Allegro font addon init failed.\n");
		return -1;
	}


	bool map[20][10];
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 10; j++)
			map[i][j] = false;
	}

	black = al_map_rgb(0, 0, 0);
	set_color_scheme(color_scheme);
	ALLEGRO_COLOR map_color[20][10];
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 10; j++)
			map_color[i][j] = black;
	}

	Block cur, next;
	ALLEGRO_BITMAP* cur_bitmap = al_create_bitmap(80, 80);

	ALLEGRO_DISPLAY* dis = al_create_display(360, 400);

	ALLEGRO_TIMER* flip = al_create_timer(1.0 / 60.0);
	ALLEGRO_TIMER* step = al_create_timer(1.0);

	ALLEGRO_EVENT_QUEUE* eq = al_create_event_queue();
	al_register_event_source(eq, al_get_display_event_source(dis));
	al_register_event_source(eq, al_get_timer_event_source(flip));
	al_register_event_source(eq, al_get_timer_event_source(step));
	al_register_event_source(eq, al_get_keyboard_event_source());

	al_start_timer(flip);
	al_start_timer(step);

	ALLEGRO_FONT* iosevka = al_load_ttf_font("iosevka-cc-semibold.ttf", 64, 0);
	int score = 0;
	char scoreString[1024] = "0";

	cur = create_block();
	next = create_block();
	draw_block_bitmap(cur_bitmap, cur, dis);

	while(true){
		ALLEGRO_EVENT e;
		bool hasEvent = al_get_next_event(eq, &e);

		if(hasEvent){
			if(e.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				break;
			else if(e.type == ALLEGRO_EVENT_TIMER && e.timer.source == flip){
				al_clear_to_color(black);

				/* draw background line */
				for(int i = 0; i < 11; i++)
					al_draw_line(20 * i, 0, 20 * i, 400, al_map_rgb(60, 60, 60), 2);
				for(int j = 0; j < 21; j++)
					al_draw_line(0, 20 * j, 200, 20 * j, al_map_rgb(60, 60, 60), 2);
				
				/* draw current block */
				al_draw_bitmap(cur_bitmap, cur.col * 20, cur.line * 20, 0);

				/* draw map */
				for(int i = 0; i < 20; i++){
					for(int j = 0; j < 10; j++){
						if(map[i][j])
							al_draw_filled_rectangle(20 * j, 20 * i, 20 * j + 20, 20 * i + 20, map_color[i][j]);
					}
				}
				/* draw score */
				sprintf(scoreString, "%d", score);
				al_draw_textf(iosevka, al_map_rgb(255, 255, 255), 280 - al_get_text_width(iosevka, scoreString) / 2, 300, 0, "%d", score);

				al_flip_display();
			}
			else if(e.type == ALLEGRO_EVENT_TIMER && e.timer.source == step){
				bool hit = false;
				for(int i = 0; i < 4; i++){
					for(int j = 0; j < 4; j++){
						if(form[cur.type][cur.dir][i * 4 + j] && (cur.line + i + 1 >= 20 || map[cur.line + i + 1][cur.col + j]))
							hit = true;
					}
				}
				if(!hit)
					cur.line++;
				else{
					/* update map */
					for(int i = 0; i < 4; i++){
						for(int j = 0; j < 4; j++){
							if(form[cur.type][cur.dir][i * 4 + j]){
								map[cur.line + i][cur.col + j] = true;
								map_color[cur.line + i][cur.col + j] = color_scheme[cur.type];
							}
						}
					}
					for(int i = 0; i < 20; i++){
						bool hole = false;
						for(int j = 0; j < 10; j++){
							if(!map[i][j])
								hole = true;
						}
						/* clear line */
						if(!hole){
							for(int k = i; k > 0; k--){
								for(int l = 0; l < 10; l++){
									map[k][l] = map[k - 1][l];
									map_color[k][l] = map_color[k - 1][l];
								}
							}
							for(int l = 0; l < 10; l++){
								map[0][l] = false;
								map_color[0][l] = black;
							}
							score++;
						}
					}
					/* create new dropping block */
					cur = next;
					draw_block_bitmap(cur_bitmap, cur, dis);
					next = create_block();
					/* check if hit the top */
					hit = false;
					for(int i = 0; i < 4; i++){
						for(int j = 0; j < 4; j++){
							if(form[cur.type][cur.dir][i * 4 + j] && map[cur.line + i][cur.col + j])
								hit = true;
						}
					}
					if(hit)
						break;	// game over
				}
			}
			else if(e.type == ALLEGRO_EVENT_KEY_DOWN){
				bool hit = false;
				int hitCheck[16];
				int count = 0;
				int correct_line = 0;
				int correct_col = 0;
				switch(e.keyboard.keycode){
					case ALLEGRO_KEY_W:
					case ALLEGRO_KEY_UP:
						cur.dir = (cur.dir + 1) % 4;
						for(int i = 0; i < 4; i++){
							for(int j = 0; j < 4; j++){
								if(form[cur.type][cur.dir][i * 4 + j] && (cur.col + j < 0 || cur.col + j >= 10 || cur.line + i >= 20 || map[cur.line + i][cur.col + j])){
									hit = true;
									hitCheck[count++] = i * 4 + j;
								}
							}
						}
						if(hit){
							/* hit! check correction */
							printf("hit\n");
							if(cur.type != I){
								for(int i = 0; i < count; i++){
									if(hitCheck[i] == 0 || hitCheck[i] == 4 || hitCheck[i] == 8)
										correct_col = 1;
									else if(hitCheck[i] == 2 || hitCheck[i] == 6 || hitCheck[i] == 10)
										correct_col = -1;
									else if(hitCheck[i] == 0 || hitCheck[i] == 1 || hitCheck[i] == 2)
										correct_line = 1;
									else if(hitCheck[i] == 8 || hitCheck[i] == 9 || hitCheck[i] == 10)
										correct_line = -1;
								}
							}
							else{
								for(int i = 0; i < count; i++){
									if(hitCheck[i] == 0 || hitCheck[i] == 4 || hitCheck[i] == 8 || hitCheck[i] == 12)
										correct_col = 1;
									else if(hitCheck[i] == 3 || hitCheck[i] == 7 || hitCheck[i] == 11 || hitCheck[i] == 15)
										correct_col = -1;
									else if(hitCheck[i] == 0 || hitCheck[i] == 1 || hitCheck[i] == 2 || hitCheck[i] == 3)
										correct_line = 1;
									else if(hitCheck[i] == 12 || hitCheck[i] == 13 || hitCheck[i] == 14 || hitCheck[i] == 15)
										correct_line = -1;
								}
							}
							/* test after apply correction */
							hit = false;
							for(int i = 0; i < 4; i++){
								for(int j = 0; j < 4; j++){
									if(form[cur.type][cur.dir][i * 4 + j] && (cur.col + correct_col + j < 0 || cur.col + correct_col + j >= 10 || cur.line + correct_line + i >= 20 || map[cur.line + correct_line + i][cur.col + correct_col + j])){
										hit = true;
									}
								}
							}
							/* result: still hit -> cancel rotate dir *cur.col + correct_col + j */
							if(hit){
								cur.dir = (cur.dir + 4 - 1) % 4;
								printf("can't correct\n");
							}
							/* result: won't hit after correcting -> apply correction */
							else{
								cur.col += correct_col;
								cur.line += correct_line;
								printf("correct!\n");
							}
						}
						draw_block_bitmap(cur_bitmap, cur, dis);
						break;
					case ALLEGRO_KEY_A:
					case ALLEGRO_KEY_LEFT:
						for(int i = 0; i < 4; i++){
							for(int j = 0; j < 4; j++){
								if(form[cur.type][cur.dir][i * 4 + j] && (cur.col + j - 1 < 0 || map[cur.line + i][cur.col + j - 1]))
									hit = true;
							}
						}
						if(!hit)
							cur.col--;
						break;
					case ALLEGRO_KEY_D:
					case ALLEGRO_KEY_RIGHT:
						for(int i = 0; i < 4; i++){
							for(int j = 0; j < 4; j++){
								if(form[cur.type][cur.dir][i * 4 + j] && (cur.col + j + 1 >= 10 || map[cur.line + i][cur.col + j + 1]))
									hit = true;
							}
						}
						if(!hit)
							cur.col++;
						break;
					case ALLEGRO_KEY_S:
					case ALLEGRO_KEY_DOWN:
						for(int i = 0; i < 4; i++){
							for(int j = 0; j < 4; j++){
								if(form[cur.type][cur.dir][i * 4 + j] && (cur.line + i + 1 >= 20 || map[cur.line + i + 1][cur.col + j]))
									hit = true;
							}
						}
						if(!hit)
							cur.line++;
						break;
					case ALLEGRO_KEY_SPACE:
						do{
							for(int i = 0; i < 4; i++){
								for(int j = 0; j < 4; j++){
									if(form[cur.type][cur.dir][i * 4 + j] && (map[cur.line + i + 1][cur.col + j] || cur.line + i + 1 >= 20))
										hit = true;
								}
							}
							if(!hit)
								cur.line++;
						}while(!hit);
						break;
				}
			}
		}
	}

	al_destroy_timer(flip);
	al_destroy_timer(step);
	al_destroy_event_queue(eq);
	al_destroy_bitmap(cur_bitmap);
	al_destroy_display(dis);
	al_destroy_font(iosevka);
	al_shutdown_font_addon();
	al_shutdown_ttf_addon();
	al_shutdown_primitives_addon();
	al_uninstall_keyboard();

	return 0;
}
