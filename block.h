#ifndef	BLOCK_H
#define	BLOCK_H

#define	I	0
#define	J	1
#define	L	2
#define	O	3	
#define	S	4
#define	T	5
#define	Z	6
#define	RIGHT	0
#define	DOWN	1
#define	LEFT	2
#define	UP		3

typedef struct{
	int line;
	int col;
	int type;
	int dir;
}Block;

Block create_block();
Block create_predict(Block, bool[][10]);
void set_color_scheme(ALLEGRO_COLOR*);
void draw_block_bitmap(ALLEGRO_BITMAP*, Block, ALLEGRO_DISPLAY*);
void move_block(Block*, bool[][10], int, int);

#endif
