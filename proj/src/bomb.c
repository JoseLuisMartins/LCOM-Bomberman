/** @file */

#include "bomb.h"
#include <stdio.h>

static Bitmap *bomb=NULL;
static Bitmap *bomb1=NULL;
static Bitmap *bomb2=NULL;


struct bomb {
	int x,y; //pos in map array
	int range;
	float counter;
	BOMB_STATE bombState;

	int xpos,ypos; 				//pos in pixels
	int kicked_bomb;			//flag that indicates this bomb has been kicked
	DIRECTION d; 				//direcao do deslocamento da bomba
	int x_endpos, y_endpos;		//posicao final depois do deslocamento


	int bitmaps_until_refresh;
	int animation_index;
};

void initializeBombBmp(){
	bomb = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2bomb.bmp");
	bomb1= loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2bomb1.bmp");
	bomb2= loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2bomb2.bmp");
}

void destroyBombBmp(){
	deleteBitmap(bomb);
	deleteBitmap(bomb1);
	deleteBitmap(bomb2);
}


bomb_t* create_bomb(int x,int y,int bomb_range){
	bomb_t *b =(bomb_t*) malloc(sizeof(bomb_t));

	b->x=x;
	b->y=y;

	b->range = bomb_range;


	b->counter=0;
	b->bombState = DEPLOYED;

	b->xpos=174+50*x;
	b->ypos=18+50*y;

	b->kicked_bomb=0;
	b->bitmaps_until_refresh=0;
	b->animation_index=0;

	return b;
}



void update_bomb(bomb_t* b){
	b->counter += (1/60.0);

	if(b->counter < 3 && b->kicked_bomb==1){
		switch (b->d) {
		case UP:
			if((b->ypos-5) >= b->y_endpos){
				b->ypos -=5;
				if(b->ypos==b->y_endpos)
					b->kicked_bomb=0;
			}
			break;
		case DOWN:
			if((b->ypos+5) <= b->y_endpos){
				b->ypos +=5;
				if(b->ypos==b->y_endpos)
					b->kicked_bomb=0;
			}
			break;
		case LEFT:
			if((b->xpos-5) >= b->x_endpos){
				b->xpos -=5;
				if(b->xpos==b->x_endpos)
					b->kicked_bomb=0;
			}
			break;
		case RIGHT:
			if((b->xpos+5) <= b->x_endpos){
				b->xpos +=5;
				if(b->xpos==b->x_endpos)
					b->kicked_bomb=0;
			}
			break;
		}
	}else if (b->counter >= 3 && b->counter < 5)
		b->bombState = EXPLODING;
	else if (b->counter >= 5)
		b->bombState = DONE;

	int x_pos = b->xpos - 174;
	int y_pos = b->ypos - 18;

	if(x_pos % 50 > 25)
		b->x = x_pos/50 + 1;
	else
		b->x = x_pos/50;

	if(y_pos % 50 > 25)
		b->y = y_pos/50 + 1;
	else
		b->y = y_pos/50;


}


void move_bomb(bomb_t* b,DIRECTION dir,int x_end,int y_end){
	asm ("movl %1, %%eax;  movl %%eax, %0;"
		:"=r"(b->d)        /* output */
		:"r"(dir)         /* input */
		:"%eax"         /* clobbered register */
		);

	asm ("movl %1, %%eax;  movl %%eax, %0;"
		:"=r"(b->x_endpos)        /* output */
		:"r"(x_end)         /* input */
		:"%eax"         /* clobbered register */
		);


	asm ("movl %1, %%eax;  movl %%eax, %0;"
		:"=r"(b->y_endpos)        /* output */
		:"r"(y_end)         /* input */
		:"%eax"         /* clobbered register */
		);

	asm ("movl %1, %%eax;  movl %%eax, %0;"
		:"=r"(b->kicked_bomb)        /* output */
		:"r"(1)         /* input */
		:"%eax"         /* clobbered register */
		);
}



int Bomb_getKicked(bomb_t* b){
	return b->kicked_bomb;
}

BOMB_STATE Bomb_getState(bomb_t* b){
	return b->bombState;
}

int Bomb_getX(bomb_t* b) {
	return b->x;
}


int Bomb_getY(bomb_t* b) {
	return b->y;
}


int Bomb_getRange(bomb_t* b) {
	return b->range;
}


void delete_bomb(bomb_t* b){
	free(b);
}

void draw_Bomb(bomb_t* b){

	switch ((b->animation_index) % (ANIMATION_ARRAY_SIZE)) {
	case 0:
		drawBitmap(bomb,b->xpos,b->ypos,ALIGN_LEFT, SECOND_BUFFER);
		break;
	case 1:
		drawBitmap(bomb1,b->xpos,b->ypos,ALIGN_LEFT, SECOND_BUFFER);
		break;
	case 2:
		drawBitmap(bomb2,b->xpos,b->ypos,ALIGN_LEFT, SECOND_BUFFER);
		break;
	}

	if(	b->bitmaps_until_refresh==7){
		b->animation_index++;
		b->bitmaps_until_refresh=0;
	}
	b->bitmaps_until_refresh++;
}
