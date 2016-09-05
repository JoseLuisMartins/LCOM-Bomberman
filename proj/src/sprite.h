#ifndef __SPRITE_H
#define __SPRITE_H

/** @defgroup sprite sprite
 * @{
 *
 * Functions related to use the sprite
 */


#include "i8042.h"
#include "i8254.h"
#include "timer.h"
#include "keyboard.h"
#include "video_gr.h"
#include "Bitmap.h"
#include "utilities.h"

typedef struct {
int x, y; // current position
double xspeed, yspeed; // current speed
Bitmap *up[ANIMATION_ARRAY_SIZE];
Bitmap *down[ANIMATION_ARRAY_SIZE];
Bitmap *left[ANIMATION_ARRAY_SIZE];
Bitmap *right[ANIMATION_ARRAY_SIZE];
Bitmap *player_win;
} Sprite;


typedef struct {
	int xmax;
	int ymax;
} Limits;


/**
 *@brief Creates a sprite in the position xi and yi passed buy argument and with the color player_color
 *
 *@param xi initial x coordenate of the sprite
 *@param yi initial y coordenate of the sprite
 *@param player_color The player color (Blue or White)
 *
 *@return Returns a pointer to the created sprite
 */
Sprite *create_sprite(int xi,int yi,int player_color);
/**
 * @brief Destroys the sprite received
 *
 * @param sp Pointer to the sprite that will be destroyed
 *
 */
void destroy_sprite(Sprite *sp);
/**
 * @brief Function that initializes all the player Bitmaps`s
 *
 */
void initializeSpriteBmp();
/**
 * @brief Function that destroys all the player Bitmaps`s
 *
 */
void destroySpriteBmp();



/** @} */
#endif
