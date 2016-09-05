#ifndef __PLAYER_H
#define __PLAYER_H

/** @defgroup player player
 * @{
 *
 * Functions for using the player
 */

#include "map.h"
#include "Bitmap.h"
#include "keyboard.h"
#include "sprite.h"
#include "bomb.h"
#include "utilities.h"

#define DEFAULT_RANGE		1
#define MAX_NUM_BOMBS		20

#define UP_INDEX			0
#define LEFT_INDEX			1
#define DOWN_INDEX			2
#define RIGHT_INDEX			3
#define BOMB_INDEX			4

typedef enum { ALIVE, DEAD } PLAYER_STATE;


struct player;
typedef struct player player_t;

/** @brief  Creates a player with specified coordinates and color
 *
 * @param x horizontal position in map array
 * @param y vertical position in map array
 * @param player_color of the player sprite
 *
 * @return Pointer to the player created
 */
player_t* create_player(int x,int y,int player_color);
/**
 * @brief Updates the player status
 *
 * calls 3 functions to do: \n
 *  -the handling of the current player power ups\n
 *	-the handling of the player position\n
 *	-the handling of the map where the player is located
 *
 *	@param player The player
 *	@param teclas_player Array that indicates which keys the player is pressing
 *	@param map Map where the player is located
 */
void update_player(player_t *player, int *teclas_player, map_t *map);
/**
 * @brief Draws the player
 *
 * @param player The player
 * @param d The direction in which the player is moving
 */
void draw_player(player_t *player,DIRECTION d);

/**
 * @brief Destroys the player
 *
 * @param player The player
 */
void destroy_player(player_t *player);

/**
 * @brief Return the x coordinate of the player in the map array
 *
 * @param player The player
 * @return x coordinate of the player in the map array
 */
int Player_getX(player_t *player);

/**
 * @brief Return the y coordinate of the player in the map array
 *
 * @param player The player
 * @return y coordinate of the player in the map array
 */
int Player_getY(player_t *player);
/**
 * @brief Returns an array that contains the current player bombs
 *
 * @param player The player
 * @return array that contains the current player bombs
 */
bomb_t** Player_getBombs(player_t *player);
/**
 * @brief Returns an integer with the maximum number of bombs that the player can place at the moment
 *
 * @param player The player
 */
int Player_MaxBombs(player_t * player);
/**
 * @brief Returns the current number of bombs placed by the player
 *
 * @param player The player
 * @return current number of bombs placed by the player
 */
int Player_PlacedBombs(player_t *player);
/**
 * @brief Decreases the placed bombs from the player by one
 *
 * @param player The player
 */
void Player_decreaseBombs(player_t *player);
/**
 * @brief Checks if the player was hit by a bomb
 *
 * @param player The player
 * @param map Map where the player is located
 * @return 1 when the player was hit, 0 when the player was not
 */
int check_player_hit(player_t *player,map_t *map);
/**
 * @brief Checks if the player is on a portal
 *
 * @param player The player
 * @param map Map where the player is located
 * @return 1 when the player is on a portal, 0 when the player is not
 */
int check_player_on_portal(player_t *player,map_t *map);
/**
 * @brief move the player in the map and does all the collision handling
 *
 * @param player The player
 * @param map Map where the player is located
 * @param d The direction in which the player is moving
 */
void move_player_handler(player_t *player,map_t* map,DIRECTION d);
/**
 * @brief update,create and remove player bombs
 *
 * @param player The player
 * @param map Map where the player is located
 * @param place_bomb flag activated when the player wants to place a bomb on the map
 */
void bombs_player_handler(player_t *player,map_t* map,int place_bomb);
/**
 * @brief Function responsible for all the power ups handling
 *\n
 * responsible for the activation,desactivation and updating of the power ups
 *
 * @param player The player
 * @param map Map where the player is located
 * @param d The direction in which the player is moving
 */
void power_ups_player_handler(player_t *player, map_t* map,DIRECTION d);
/**
 * @brief Returns the player Bitmap
 *
 * @param player The player
 * @return player bitmap
 */
Bitmap* Player_getBitmap(player_t *player);
/**
 * @brief Returns whether the player has the velocity boost power up or not
 *
 * @param player The player
 * @return 1 when the player has the velocity boost power up and 0 in the other case
 */
int Player_getVelocity_boost(player_t *player);
/**
 * @brief Returns whether the player has the range boost power up or not
 *
 * @param player The player
 * @return 1 when the player has the range boost power up and 0 in the other case
 */
int Player_getrange_boost(player_t *player);
/**
 * @brief Returns whether the player has the kick boost power up or not
 *
 * @param player The player
 * @return 1 when the player has the kick boost power up and 0 in the other case
 */
int Player_getKickBoost(player_t *player);
/**
 * @brief Updates the player position in the map array
 *
 * @param player The player
 */
void updatePlayerPosition(player_t *player);
/**
 * @brief Returns the range counter
 *
 * @param player The player
 * @return integer that indicates the remaining time to the end of the  range boost power up
 */
double Player_getRangeCounter(player_t *player);
/**
 * @brief Returns the Velocity counter
 *
 * @param player The player
 * @return integer that indicates the remaining time to the end of the velocity boost power up
 */
double Player_getVelocityCounter(player_t *player);
/**
 * @brief Returns the Kick counter
 *
 * @param player The player
 * @return integer that indicates the remaining time to the end of the kick power up
 */
double Player_getKickCounter(player_t *player);
/**
 * @brief Returns the player win Bitmap
 *
 * @param player The player
 * @return bitmap with the player celebrating
 */
Bitmap* Player_getWinnerBmp(player_t *player);
/**
 * @brief Returns the number of victories of a player
 *
 * @param player The player
 * @return integer with the number of the victories
 */
int Player_getNumVictories(player_t *player);
/**
 * @brief Increments the player victories by one
 *
 * @param player The player
 */
void Player_incVictories(player_t *player);
/**
 * @brief Resets \n
 * -The player position \n
 * -The player power ups \n
 * -The player bombs
 *
 * @param player The player
 * @param x The new position x coordinate of the player
 * @param y The new position y coordinate of the player
 */
void resetPlayer(player_t *player, int x, int y);
/**
 * @brief Draws all the player bombs
 *
 * @param player The player
 */
void draw_PlayerBombs(player_t *player);
/**
 * @brief Updates the player state to DEAD
 *
 * @param player The player
 */
void playerDead(player_t *player);
/**
 * @brief Returns the player state
 *
 * @param player The player
 * @return returns the player state ALIVE or DEAD
 */
PLAYER_STATE Player_getState(player_t *player);

/**@} */
#endif
