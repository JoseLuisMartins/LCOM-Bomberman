/** @file */


#include <stdio.h>
#include <stdlib.h>
#include <minix/drivers.h>
#include "vbe.h"
#include "i8042.h"
#include "mouse.h"
#include "Player.h"
#include "Bomberman.h"
#include "menu.h"
#include "utilities.h"



int main(){

	sef_startup();
	vg_init(MODE_64K_COLORS);

	GAME_STATE game_state = MENU;
	initializeGamePieces();

	while(1){
		if(game_state==MENU)
			game_state = main_menu();
		else if(game_state == SINGLEPLAYER)
			game_state = Singleplayer();
		else if (game_state == BEST_OF_1)
			game_state = Multiplayer(BEST_OF_1);
		else if (game_state == BEST_OF_3)
			game_state = Multiplayer(BEST_OF_3);
		else if (game_state == BEST_OF_5)
			game_state = Multiplayer(BEST_OF_5);
		else if(game_state == MULTIPLAYERSELECT)
			game_state = multiplayer_select();
		else {
			vg_exit();
			destroyGamePieces();
			return 0;
		}
	}
}
