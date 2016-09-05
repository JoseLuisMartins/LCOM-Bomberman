#ifndef __BOMBERMAN_H
#define __BOMBERMAN_H


/** @defgroup Bomberman Bomberman
 * @{
 *
 * Functions for using Bomberman
 */


#include "Player.h"
#include "map.h"
#include "timer.h"
#include "utilities.h"
#include "rtc.h"


/**
 * @brief Runs singleplayer mode
 *
 * @return State of the game
 */
GAME_STATE Singleplayer();



/**
 * @brief Runs multiplayer mode
 *
 * @param BestOf number of games a player has to win
 *
 * @return State of the game
 */
GAME_STATE Multiplayer(int BestOf);



/**
 * @brief Retrieves direction from array info
 *
 * @param teclas_player	array which contains info on which keys were pressed
 *
 * @return Direction
 */
DIRECTION get_key_direction(int teclas_player[]);



/**
 * Draws side bar for multiplayer, according to players' info
 *
 * @param player1 pointer to the first player
 * @param player2 pointer to the second player
 *
 */
void draw_side_bar_multiplayer(player_t *player1, player_t *player2);



/**
 * Draws counters on side bar for multiplayer, according to players' info
 *
 * @param player1 pointer to the first player
 * @param player2 pointer to the second player
 */
void draw_side_bar_multiplayer_counter(player_t* player1, player_t *player2);



/**
 * Draws side bar for singleplayer, according to player's info
 *
 * @param player pointer to the player
 */
void draw_side_bar_singleplayer(player_t *player);



/**
 * Draws counters on side bar for singleplayer, according to player's info
 *
 * @param player pointer to the player
 */
void draw_side_bar_singleplayer_counter(player_t* player);



/**
 * Initializes bitmaps necessary to draw the side bar
 */
void initializeSideBarBmp();



/**
 * Frees memory used by side bar's bitmaps
 */
void destroySideBarBmp();



/**
 * @brief Enables player to edit its own map
 *
 * @param map pointer to the map that will be edited
 * @param gs game state, so the map can be edited accordingly
 *
 * @return Returns gs if successful, MENU if player cancels map editing
 */
GAME_STATE editMap(map_t *map, GAME_STATE gs);



/**
 * @brief Displays the time of the day
 *
 * @param time pointer to the info that will be displayed
 */
void displayTime(time_info_t *time);



/**
 * Initializes all game pieces
 */
void initializeGamePieces();



/**
 * Destroys all game pieces
 */
void destroyGamePieces();


/**@} */

#endif
