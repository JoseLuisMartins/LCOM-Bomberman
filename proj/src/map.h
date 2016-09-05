#ifndef __MAP_H
#define __MAP_H


/** @defgroup map map
 * @{
 *
 * Functions for using maps
 */


#include <stdio.h>
#include <stdlib.h>
#include "Bitmap.h"
#include "video_gr.h"
#include "bomb.h"
#include "utilities.h"

#define BLOCO_DESTRUTIVEL 	0
#define BLOCO_INDESTRUTIVEL 1
#define CAMINHO 			2
#define BOMBA 				3
#define BOMB_EXPLOSION		4
#define CAM_HOR_EXPLODE 	5
#define CAM_VER_EXPLODE 	6
#define DES_HOR_EXPLODE 	7
#define DES_VER_EXPLODE 	8
#define BOMBA_POWER_UP 		9
#define RANGE_POWER_UP		10
#define VELOCITY_POWER_UP 	11
#define KICK_POWER_UP		12
#define PORTAL				13


struct map;
typedef struct map map_t;



/**
 * @brief Creates a map according to specified parameters
 *
 * @param selection	equal to 1 if map is randomized, different from 1 if map is edited
 * @param gs game state, to check if the map is for multiplayer or singleplayer
 *
 * @return Pointer to map
 */
map_t* create_map(int selection,GAME_STATE gs);




/**
 * @brief Checks if number is even (or odd)
 *
 * @param x number to check
 *
 * @return 1 if even, 0 otherwise
 */
int even(int x);



/**
 * @brief Updated map according to bomb\n
 * Depending on bomb state, the content of the map will be updated
 *
 * @param m	map that will be updated
 * @param b	bomb responsible for the map update
 *
 * @return Returns 0 if map is unchanged, 1 otherwise
 */
int update_map(map_t* m, bomb_t* b);



/**
 * @brief Draws map\n
 * Draws bitmaps associated to map
 *
 * @param m	pointer to map
 */
void draw_map(map_t* m);



/**
 * @brief Deletes map\n
 * Frees memory occupied by the map data
 *
 * @param m pointer to map that will be deleted
 */
void delete_map(map_t* m);



/**
 * @brief Checks if it's OK to move, to desired position
 *
 * @param map pointer to map
 * @param x	horizontal position on map's array
 * @param y	vertical position on map's array
 * @param xpos	horizontal position on buffer
 * @param ypos	vertical position on buffer
 * @param d	direction of the movement
 *
 * @return Returns 0 if movement is possible, 1 otherwise
 */
int check_collision(map_t* map, int x, int y,int xpos,int ypos, DIRECTION d);



/**
 * @brief Retrieves map's content on certain position
 *
 * @param m pointer to map
 * @param x	horizontal position on map's array
 * @param y	vertical position on map's array
 *
 * @return Map's content
 */
int map_getContent(map_t* m, int x, int y);



/**
 * @brief Sets map's content on certain position
 *
 * @param m	pointer to map
 * @param x	horizontal position on map's array
 * @param y	vertical position on map's array
 * @param content new content of map
 */
void map_setContent(map_t* m, int x, int y, int content);



/*
 * @param m pointer to map
 *
 * @return Returns draw (data-member of map)
 */
int map_getDraw(map_t* m);



/**
 * @brief Sets map's draw to 1
 *
 * @param m pointer to map
 */
void map_setDraw(map_t* m);



/**
 * @brief Randomizes power up, to drop on map
 *
 * @return Returns power up
 */
int powerUpSelector();



/**
 * @param m	pointer to map
 *
 * @return Map's bitmap associated to destructible block
 */
Bitmap* getBlocoDestrutivel(map_t* m);



/**
 * @brief Places randomly a portal on map (singleplayer mode)
 *
 * @param m	pointer to map
 */
void place_portal(map_t* m);



/**
 * Initializes bitmaps necessary to draw a map
 */
void initializeMapBmp();



/**
 * Frees memory used by map's bitmaps
 */
void destroyMapBmp();


/**@} */

#endif
