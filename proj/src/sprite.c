/** @file */
#include "sprite.h"
#include <stdlib.h>


#define COLOR_BLACK			0



static Bitmap *upWhite[ANIMATION_ARRAY_SIZE];
static Bitmap *downWhite[ANIMATION_ARRAY_SIZE];
static Bitmap *leftWhite[ANIMATION_ARRAY_SIZE];
static Bitmap *rightWhite[ANIMATION_ARRAY_SIZE];
static Bitmap *upBlue[ANIMATION_ARRAY_SIZE];
static Bitmap *downBlue[ANIMATION_ARRAY_SIZE];
static Bitmap *leftBlue[ANIMATION_ARRAY_SIZE];
static Bitmap *rightBlue[ANIMATION_ARRAY_SIZE];
static Bitmap *player_win_white;
static Bitmap *player_win_blue;

Sprite *create_sprite(int xi,int yi,int player_color) {
	//allocate space for the "object"
	Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
	if( sp == NULL )
		return NULL;

	int i;
	if(player_color == PLAYER_WHITE){

		for (i = 0; i < 3; i++) {
			sp->up[i] = upWhite[i];
			sp->down[i] = downWhite[i];
			sp->left[i] = leftWhite[i];
			sp->right[i] = rightWhite[i];
		}
		sp->player_win = player_win_white;

	} else if(player_color == PLAYER_BLUE) {

		for (i = 0; i < 3; i++) {
			sp->up[i] = upBlue[i];
			sp->down[i] = downBlue[i];
			sp->left[i] = leftBlue[i];
			sp->right[i] = rightBlue[i];
		}
		sp->player_win = player_win_blue;
	}

	sp->x = xi;
	sp->y = yi;
	sp->xspeed = 5;
	sp->yspeed = 5;

	return sp;
}

void destroy_sprite(Sprite *sp) {
	if( sp == NULL )
		return;

	free(sp);
	sp = NULL;
}




void initializeSpriteBmp() {

	// white avatar
	upWhite[0] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/playerUp.bmp");
	upWhite[1] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/playerUp1.bmp");
	upWhite[2] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/playerUp2.bmp");

	downWhite[0] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/playerDown.bmp");
	downWhite[1] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/playerDown1.bmp");
	downWhite[2] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/playerDown2.bmp");

	leftWhite[0] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/playerLeft.bmp");
	leftWhite[1] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/playerLeft1.bmp");
	leftWhite[2] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/playerLeft2.bmp");

	rightWhite[0] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/playerRight.bmp");
	rightWhite[1] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/playerRight1.bmp");
	rightWhite[2] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/playerRight2.bmp");

	player_win_white = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/playerWin.bmp");


	// blue avatar
	upBlue[0] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2playerUp.bmp");
	upBlue[1] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2playerUp1.bmp");
	upBlue[2] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2playerUp2.bmp");

	downBlue[0] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2playerDown.bmp");
	downBlue[1] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2playerDown1.bmp");
	downBlue[2] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2playerDown2.bmp");

	leftBlue[0] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2playerLeft.bmp");
	leftBlue[1] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2playerLeft1.bmp");
	leftBlue[2] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2playerLeft2.bmp");

	rightBlue[0] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2playerRight.bmp");
	rightBlue[1] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2playerRight1.bmp");
	rightBlue[2] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2playerRight2.bmp");

	player_win_blue = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2playerWin.bmp");
}




void destroySpriteBmp() {

	int i;
	for (i = 0; i < 3; i++) {
		deleteBitmap(upWhite[i]);
		deleteBitmap(downWhite[i]);
		deleteBitmap(leftWhite[i]);
		deleteBitmap(rightWhite[i]);

		deleteBitmap(upBlue[i]);
		deleteBitmap(downBlue[i]);
		deleteBitmap(leftBlue[i]);
		deleteBitmap(rightBlue[i]);
	}

	deleteBitmap(player_win_white);
	deleteBitmap(player_win_blue);
}
