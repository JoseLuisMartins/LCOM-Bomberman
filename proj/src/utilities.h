#ifndef __UTILITIES_H
#define __UTILITIES_H

/** @file */

//used to choose what buffer setpixel should write
#define SECOND_BUFFER 				0
#define THIRD_BUFFER  				1
#define VIDEO_MEM 	  				2

#define ANIMATION_ARRAY_SIZE		3

#define PLAYER_WHITE 				1
#define PLAYER_BLUE					2

#define COLOR_YELLOW 				65024
#define COLOR_RED					38914

typedef enum {MENU, BEST_OF_1, BEST_OF_3, BEST_OF_5, MULTIPLAYER, SINGLEPLAYER, END, MULTIPLAYERSELECT} GAME_STATE;

typedef enum {
	UP = 0,
	LEFT = 1,
	DOWN = 2,
	RIGHT = 3,
	INVALID_DIRECTION = 4,
} DIRECTION;

typedef struct{
	int player_selection;
	int map_selection;
}Settings;

#endif
