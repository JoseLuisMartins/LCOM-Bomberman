#ifndef __SPRITE_H
#define __SPRITE_H

#include "i8042.h"
#include "i8254.h"
#include "timer.h"
#include "keyboard.h"
#include "video_gr.h"

typedef struct {
double x, y; // current position
int width, height; // dimensions
double xspeed, yspeed; // current speed
char *map; // the pixmap
} Sprite;


typedef struct {
	int xmax;
	int ymax;
} Limits;

Sprite *create_sprite(char *pic[],int xi,int yi,int vx,int vy);
void destroy_sprite(Sprite *sp);
int remove_sprite(Sprite* s);
int animate_sprite(Sprite *sp, Limits lim);
static int check_collision(Sprite *sp, char *base);
static int draw_sprite(Sprite *sp, char *base);





#endif
