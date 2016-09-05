#ifndef __BOMB_H
#define __BOMB_H


/** @defgroup bomb bomb
 * @{
 *
 * Functions for using bombs
 */


#include "Bitmap.h"
#include "video_gr.h"
#include "utilities.h"

typedef enum { DEPLOYED, EXPLODING, DONE } BOMB_STATE;

struct bomb;
typedef struct bomb bomb_t;



/**
 * @brief Creates a bomb\n
 * Creates a bomb with specified coordinates and range
 *
 * @param x horizontal position
 * @param y vertical position
 * @param bomb_range explosion radius
 *
 * @return Pointer to bomb
 */
bomb_t* create_bomb(int x,int y,int bomb_range);



/**
 * @brief Updates bomb\n
 * Updates the bomb according to its member-data
 *
 * @param b pointer to bomb that will be updated
 */
void update_bomb(bomb_t* b);



/**
 * @brief Deletes bomb\n
 * Frees memory occupied by bomb's data
 *
 * @param b pointer to bomb that will be deleted
 */
void delete_bomb(bomb_t* b);



/**
 * @param b pointer to bomb
 *
 * @return Bomb's state
 */
BOMB_STATE Bomb_getState(bomb_t* b);



/**
 * @param b pointer to bomb
 *
 * @return Bomb's horizontal position
 */
int Bomb_getX(bomb_t* b);



/**
 * @param b pointer to bomb
 *
 * @return Bomb's vertical position
 */
int Bomb_getY(bomb_t* b);



/**
 * @param b	pointer to bomb
 *
 * @return Bomb's explosion range
 */
int Bomb_getRange(bomb_t* b);



/**
 * @brief Draws bomb\n
 * Draws bitmap associated to bomb
 *
 * @param b pointer to bomb
 */
void draw_Bomb(bomb_t* b);



/**
 * Initializes bitmaps necessary to draw a bomb
 */
void initializeBombBmp();



/**
 * Frees memory used by bomb's bitmaps
 */
void destroyBombBmp();



/**
 * @brief Moves bomb to specified parameters
 *
 * @param b pointer to bomb
 * @param dir bomb's direction
 * @param x_end	horizontal position
 * @param y_end vertical position
 */
void move_bomb(bomb_t* b,DIRECTION dir,int x_end,int y_end);


/**@} */

#endif
