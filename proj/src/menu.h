#ifndef __MENU_H
#define __MENU_H


/** @defgroup menu menu
 * @{
 *
 * Functions for using menu
 */



#include "timer.h"
#include "mouse.h"
#include "keyboard.h"
#include "utilities.h"
#include "Bitmap.h"
#include "i8042.h"
#include "rtc.h"



/**
 * @brief Shows main menu\n
 * Displays Singleplayer, Multiplayer and Exit options
 *
 * @return Game's state
 */
GAME_STATE main_menu();



/**
 * Shows multiplayer menu\n
 * Displays Quick Game, Best of 3 and Best of 5 options
 *
 * @return Game's state
 */
GAME_STATE multiplayer_select();



/**
 * Displays all information related to current time and date
 */
void displayDateAndTime();



/**
 * Shows pause menu\n
 * Pauses the game, enabling the player to continue playing or quitting
 *
 * @return Returns 0 to continue playing, 1 to quit
 */
int pause_menu();



/**
 * @brief Handles pause\n
 * Call pause_menu(), but subscribes and unsubscribes timer and keyboard
 *
 * @return Returns 1 to continue playing, 0 to quit
 */
int pause();



/**
 * @brief Shows settings menu\n
 * Displays avatar's and map's options
 *
 * @param[out] setting defined by user
 *
 * @return Returns 0 on success, 1 if canceled
 */
int settings_menu(Settings *setting);



/**
 * Initializes bitmaps necessary to draw a menu
 */
void initializeMenuBmp();



/**
 * Frees memory used by menu's bitmaps
 */
void destroyMenuBmp();



/**
 * @brief Shows end game menu for multiplayer mode\n
 * Displays winner and loser
 *
 * @param winner player's winner bitmap
 */
void endGame_menu_multiplayer(Bitmap *winner);
/**
 * @brief Shows end game menu for multiplayer mode\n
 *
 * @param winner player's winner bitmap
 * @param game_time time of token to the player to reach the portal
 * @param win flag that indicates if the player ended the game because he reached the portal or if he died with a bomb
 *
 */
void endGame_menu_singleplayer(Bitmap *winner,time_info_t *game_time,int win);


/**@} */

#endif
