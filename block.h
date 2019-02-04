#ifndef	BLOCK_H
#define	BLOCK_H

#define	I	0
#define	J	1
#define	L	2
#define	O	3	
#define	S	4
#define	T	5
#define	Z	6
#define	UP	0
#define	RIGHT	1
#define	DOWN	2
#define	LEFT	3

typedef struct{
	int line;
	int col;
	int type;
	int dir;
}Block;

Block create_block();
void set_color_scheme(ALLEGRO_COLOR*);
void draw_block_bitmap(ALLEGRO_BITMAP*, Block, ALLEGRO_DISPLAY*);

#endif
