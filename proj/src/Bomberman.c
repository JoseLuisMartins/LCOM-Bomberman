#include "Bomberman.h"
#include "Player.h"
#include "map.h"
#include "mouse.h"
#include "Bitmap.h"
#include "utilities.h"

GAME_STATE game_state=MENU;

static Bitmap *side_bar_multiplayer=NULL;
static Bitmap *side_bar_singleplayer=NULL;
static Bitmap *bomb_sb=NULL;
static Bitmap *range_sb=NULL;
static Bitmap *range_faded_sb=NULL;
static Bitmap *velocity_sb=NULL;
static Bitmap *velocity_faded_sb=NULL;
static Bitmap *kick_sb = NULL;
static Bitmap *kick_faded_sb = NULL;
static Bitmap *side_bar_win1 =NULL;
static Bitmap *side_bar_win2 =NULL;
static Bitmap *side_bar_create_map = NULL;
static Bitmap *side_bar_create_map1 = NULL;
static Bitmap* chamuscadito=NULL;


DIRECTION get_key_direction(int teclas_player[]){
	int i;
	int x=0;
	for (i = 0; i < 4; ++i) {
		if(teclas_player[i]){
			x=1;
			break;
		}
	}
	if(x){
		switch (i) {
		case UP_INDEX:
			return UP;
			break;
		case LEFT_INDEX:
			return LEFT;
			break;
		case DOWN_INDEX:
			return DOWN;
			break;
		case RIGHT_INDEX:
			return RIGHT;
			break;
		}
	}
	else
		return INVALID_DIRECTION;
}




void draw_side_bar_multiplayer(player_t *player1, player_t *player2){

	drawBitmap(side_bar_multiplayer,0,0,ALIGN_LEFT, SECOND_BUFFER);
	//player 2
	if (Player_getState(player1) == DEAD)
		drawBitmap(Player_getWinnerBmp(player2), 10,  50, ALIGN_LEFT, SECOND_BUFFER);
	else if (Player_getState(player2) == DEAD)
		drawBitmap(chamuscadito, 10,  50, ALIGN_LEFT, SECOND_BUFFER);
	else
		drawBitmap(Player_getBitmap(player2),10,50,ALIGN_LEFT,SECOND_BUFFER);

	//bomb
	drawBitmap(bomb_sb,10,100,ALIGN_LEFT, SECOND_BUFFER);
	//bomb range power up
	if(Player_getrange_boost(player2)==0)
		drawBitmap(range_faded_sb,10,150,ALIGN_LEFT, SECOND_BUFFER);
	else {
		drawBitmap(range_sb,10,150,ALIGN_LEFT, SECOND_BUFFER);
	}
	//velocity power up
	if(Player_getVelocity_boost(player2)==0)
		drawBitmap(velocity_faded_sb,10,200,ALIGN_LEFT, SECOND_BUFFER);
	else
		drawBitmap(velocity_sb,10,200,ALIGN_LEFT, SECOND_BUFFER);

	if (Player_getKickBoost(player2) == 0)
		drawBitmap(kick_faded_sb, 10, 250, ALIGN_LEFT, SECOND_BUFFER);
	else
		drawBitmap(kick_sb, 10, 250, ALIGN_LEFT, SECOND_BUFFER);


	//player 1
	if (Player_getState(player2) == DEAD)
		drawBitmap(Player_getWinnerBmp(player1), 10,  450, ALIGN_LEFT, SECOND_BUFFER);
	else if (Player_getState(player1) == DEAD)
		drawBitmap(chamuscadito, 10,  450, ALIGN_LEFT, SECOND_BUFFER);
	else
		drawBitmap(Player_getBitmap(player1), 10, 450,ALIGN_LEFT,SECOND_BUFFER);

	//bomb
	drawBitmap(bomb_sb, 10,500,ALIGN_LEFT, SECOND_BUFFER);
	//bomb range power up
	if(Player_getrange_boost(player1)==0)
		drawBitmap(range_faded_sb, 10,550,ALIGN_LEFT, SECOND_BUFFER);
	else
		drawBitmap(range_sb, 10,550,ALIGN_LEFT, SECOND_BUFFER);
	//velocity power up
	if(Player_getVelocity_boost(player1)==0)
		drawBitmap(velocity_faded_sb, 10,600,ALIGN_LEFT, SECOND_BUFFER);
	else
		drawBitmap(velocity_sb, 10,600,ALIGN_LEFT, SECOND_BUFFER);
	// kick power up
	if (Player_getKickBoost(player1) == 0)
		drawBitmap(kick_faded_sb, 10, 650, ALIGN_LEFT, SECOND_BUFFER);
	else
		drawBitmap(kick_sb, 10, 650, ALIGN_LEFT, SECOND_BUFFER);
}



void draw_side_bar_multiplayer_counter(player_t* player1, player_t *player2) {
	double number;

	draw_side_bar_number_int(Player_getNumVictories(player2), 105, 60);
	draw_side_bar_number_int(Player_getNumVictories(player1), 105, 460);

	draw_side_bar_number_int(Player_MaxBombs(player2) - Player_PlacedBombs(player2), 105, 110);
	draw_side_bar_number_int(Player_MaxBombs(player1) - Player_PlacedBombs(player1), 105, 510);

	if((number = Player_getRangeCounter(player2)) > 0) {
		if (number >= 10.0)
			draw_side_bar_number_double(number, 86, 160);
		else
			draw_side_bar_number_double(number, 94, 160);
	}
	if((number = Player_getVelocityCounter(player2)) > 0) {
		if (number >= 10.0)
			draw_side_bar_number_double(number, 86, 210);
		else
			draw_side_bar_number_double(number, 94, 210);
	}
	if ((number = Player_getKickCounter(player2)) > 0) {
		if (number >= 10.0)
			draw_side_bar_number_double(number, 86, 260);
		else
			draw_side_bar_number_double(number, 94, 260);
	}



	if((number = Player_getRangeCounter(player1)) > 0) {
		if (number >= 10.0)
			draw_side_bar_number_double(number, 86, 560);
		else
			draw_side_bar_number_double(number, 94, 560);
	}
	if((number = Player_getVelocityCounter(player1)) > 0) {
		if (number >= 10.0)
			draw_side_bar_number_double(number, 86, 610);
		else
			draw_side_bar_number_double(number, 94, 610);
	}
	if ((number = Player_getKickCounter(player1)) > 0) {
		if (number >= 10.0)
			draw_side_bar_number_double(number, 86, 660);
		else
			draw_side_bar_number_double(number, 94, 660);
	}
}



void draw_side_bar_singleplayer(player_t *player) {

	drawBitmap(side_bar_singleplayer,0,0,ALIGN_LEFT, SECOND_BUFFER);
	//player
	drawBitmap(Player_getBitmap(player),50,50,ALIGN_LEFT,SECOND_BUFFER);
	//bomb
	drawBitmap(bomb_sb,10,100,ALIGN_LEFT, SECOND_BUFFER);
	//bomb range power up
	if(Player_getrange_boost(player)==0)
		drawBitmap(range_faded_sb, 10,150,ALIGN_LEFT, SECOND_BUFFER);
	else {
		drawBitmap(range_sb, 10,150,ALIGN_LEFT, SECOND_BUFFER);
	}
	//velocity power up
	if(Player_getVelocity_boost(player)==0)
		drawBitmap(velocity_faded_sb, 10,200,ALIGN_LEFT, SECOND_BUFFER);
	else
		drawBitmap(velocity_sb, 10,200,ALIGN_LEFT, SECOND_BUFFER);

	//boost power up
	if (Player_getKickBoost(player) == 0)
		drawBitmap(kick_faded_sb, 10, 250, ALIGN_LEFT, SECOND_BUFFER);
	else
		drawBitmap(kick_sb, 10, 250, ALIGN_LEFT, SECOND_BUFFER);
}




void draw_side_bar_singleplayer_counter(player_t* player) {

	double number;

	draw_side_bar_number_int(Player_MaxBombs(player) - Player_PlacedBombs(player), 105, 110);

	if((number = Player_getRangeCounter(player)) > 0) {
		if (number >= 10.0)
			draw_side_bar_number_double(number, 86, 160);
		else
			draw_side_bar_number_double(number, 94, 160);
	}

	if((number = Player_getVelocityCounter(player)) > 0) {
		if (number >= 10.0)
			draw_side_bar_number_double(number, 86, 210);
		else
			draw_side_bar_number_double(number, 94, 210);
	}

	if ((number = Player_getKickCounter(player)) > 0) {
		if (number >= 10.0)
			draw_side_bar_number_double(number, 86, 260);
		else
			draw_side_bar_number_double(number, 94, 260);
	}
}





GAME_STATE Multiplayer(int BestOf)
{
	int continueGame = 1;
	int firstPlay = 1;
	static Settings setting;

	player_t *player1;
	player_t *player2;

	while (continueGame) {
		int valid = 1;
		int counter = 0;
		time_info_t time;

		if (firstPlay) {
			if (settings_menu(&setting))
				return MENU;

			player1 = create_player(16,14,(setting.player_selection%2)+1);
			player2 = create_player(0,0,setting.player_selection);
			firstPlay = 0;
		}
		else {
			resetPlayer(player1, 16, 14);
			resetPlayer(player2, 0, 0);
		}


		map_t *map = create_map(setting.map_selection, MULTIPLAYER);
		if (setting.map_selection == 2)
			if (editMap(map, MULTIPLAYER) == MENU)
			{
				delete_map(map);
				return MENU;
			}


		int irq_set_kbd = BIT(keyboard_subscribe_int());
		int irq_set_timer = BIT(timer_subscribe_int());


		int ipc_status;
		message msg;
		int r;
		int i;

		int teclas_player1[5]={0};
		int teclas_player2[5]={0};


		while(valid) {
			if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
				printf("driver_receive failed with: %d", r);
				continue;
			}

			if (is_ipc_notify(ipc_status)) {

				if(_ENDPOINT_P(msg.m_source)==HARDWARE) {
					if(msg.NOTIFY_ARG & irq_set_timer){
						//update da velocidade e desenho dos jogadores
						update_player(player1, teclas_player1, map);

						if (check_player_hit(player1, map)){
							valid = 0;
							Player_incVictories(player2);
							playerDead(player1);

							draw_side_bar_multiplayer(player1, player2);
							draw_map(map);
							swapThirdBuffer();
							draw_player(player1,get_key_direction(teclas_player1));
							draw_player(player2,get_key_direction(teclas_player2));
							draw_side_bar_multiplayer_counter(player1, player2);
							swapVideoMem();
							endGame_menu_multiplayer(Player_getWinnerBmp(player2));
						}

						update_player(player2, teclas_player2, map);

						if (check_player_hit(player2,map)){
							valid = 0;
							Player_incVictories(player1);
							playerDead(player2);

							draw_side_bar_multiplayer(player1, player2);
							draw_map(map);
							swapThirdBuffer();
							draw_player(player1,get_key_direction(teclas_player1));
							draw_player(player2,get_key_direction(teclas_player2));
							draw_side_bar_multiplayer_counter(player1, player2);
							swapVideoMem();
							endGame_menu_multiplayer(Player_getWinnerBmp(player1));
						}


						if (Player_getNumVictories(player1) == BestOf || Player_getNumVictories(player2) == BestOf) {
							valid = 0;
							continueGame = 0;
						}


						if (valid) {
							if(map_getDraw(map))
								draw_map(map);

							draw_PlayerBombs(player1);
							draw_PlayerBombs(player2);

							draw_side_bar_multiplayer(player1,player2);
							swapThirdBuffer();

							if (counter % 60 == 0)
								getCurrentTime(&time);
							displayTime(&time);
							counter++;

							draw_side_bar_multiplayer_counter(player1,player2);
							draw_player(player1,get_key_direction(teclas_player1));
							draw_player(player2,get_key_direction(teclas_player2));
							swapVideoMem();
						}
					}
					if (msg.NOTIFY_ARG & irq_set_kbd) {
						int result;
						unsigned long scan;
						result = keyboard_int_handler(&scan);
						switch (result) {
						case -1:
							vg_exit();
							return;
							break;
						case 1:
							break;
						case 0:
							//update array teclas
							if(scan== KEY_UP(KEY_ESC)) {
								if (pause()) {
									valid = 0;
									continueGame = 0;
								}
								map_setDraw(map);
							}
							else
								switch (scan) {
								case KEY_ARR_UP:teclas_player1[0]=1;//makecode
								break;
								case KEY_UP(KEY_ARR_UP):teclas_player1[0]=0;//breakcode
								break;
								case KEY_W:teclas_player2[0]=1;
								break;
								case KEY_UP(KEY_W):teclas_player2[0]=0;
								break;
								case KEY_ARR_LEFT:teclas_player1[1]=1;
								break;
								case KEY_UP(KEY_ARR_LEFT):teclas_player1[1]=0;
								break;
								case KEY_A:teclas_player2[1]=1;
								break;
								case KEY_UP(KEY_A):teclas_player2[1]=0;
								break;
								case KEY_ARR_DOWN:teclas_player1[2]=1;
								break;
								case KEY_UP(KEY_ARR_DOWN):teclas_player1[2]=0;
								break;
								case KEY_S:teclas_player2[2]=1;
								break;
								case KEY_UP(KEY_S):teclas_player2[2]=0;
								break;
								case KEY_ARR_RIGHT:teclas_player1[3]=1;
								break;
								case KEY_UP(KEY_ARR_RIGHT):teclas_player1[3]=0;
								break;
								case KEY_D:teclas_player2[3]=1;
								break;
								case KEY_UP(KEY_D):teclas_player2[3]=0;
								break;
								case KEY_ENTER:teclas_player1[4]=1;
								break;
								case KEY_UP(KEY_ENTER):teclas_player1[4]=0;
								break;
								case KEY_SPACE:teclas_player2[4]=1;
								break;
								case KEY_UP(KEY_SPACE):teclas_player2[4]=0;
								break;
								default:
									break;
								}
							break;
						}
					}
				}
			}
		}

		delete_map(map);
		timer_unsubscribe_int();
		keyboard_unsubscribe_int();
	}

	destroy_player(player1);
	destroy_player(player2);
	return MENU;
}




void initializeSideBarBmp() {

	side_bar_multiplayer = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/side_bar_multiplayer.bmp");
	side_bar_singleplayer = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/side_bar_singleplayer.bmp");
	side_bar_win1 = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/side_bar_win1.bmp");
	side_bar_win2 = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/side_bar_win1.bmp");
	bomb_sb=loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2bomb.bmp");
	range_sb=loadBitmap("home/lcom/lcom1516-t6g01/proj/res/rangePowerUp.bmp");
	range_faded_sb=loadBitmap("home/lcom/lcom1516-t6g01/proj/res/rangePowerUpFaded.bmp");
	velocity_sb=loadBitmap("home/lcom/lcom1516-t6g01/proj/res/velocityPowerUp.bmp");
	velocity_faded_sb=loadBitmap("home/lcom/lcom1516-t6g01/proj/res/velocityPowerUpFaded.bmp");
	kick_sb = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/kickPowerUp.bmp");
	kick_faded_sb = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/kickPowerUpFaded.bmp");
	side_bar_create_map =loadBitmap("home/lcom/lcom1516-t6g01/proj/res/createmap_menu.bmp");
	side_bar_create_map1=loadBitmap("home/lcom/lcom1516-t6g01/proj/res/createmap_menu1.bmp");
	chamuscadito = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/playerLose.bmp");
}

void destroySideBarBmp(){
	deleteBitmap(side_bar_multiplayer);
	deleteBitmap(side_bar_singleplayer);
	deleteBitmap(side_bar_win1);
	deleteBitmap(side_bar_win2);
	deleteBitmap(bomb_sb);
	deleteBitmap(range_sb);
	deleteBitmap(range_faded_sb);
	deleteBitmap(velocity_sb);
	deleteBitmap(velocity_faded_sb);
	deleteBitmap(kick_sb);
	deleteBitmap(kick_faded_sb);
	deleteBitmap(side_bar_create_map);
	deleteBitmap(side_bar_create_map1);
	deleteBitmap(chamuscadito);
}



GAME_STATE Singleplayer() {

	int valid = 1;
	int counter = 0;
	time_info_t time;
	time.seconds = 0;
	time.minutes = 0;
	time.hours = 0;

	Settings setting;
	if (settings_menu(&setting))
		return MENU;

	map_t *map = create_map(setting.map_selection,SINGLEPLAYER);
	if (setting.map_selection == 2){
		if (editMap(map, SINGLEPLAYER) == MENU)
		{
			delete_map(map);
			return MENU;
		}

		place_portal(map);
	}

	player_t *player = create_player(8, 7, setting.player_selection);


	int irq_set_kbd = BIT(keyboard_subscribe_int());
	int irq_set_timer = BIT(timer_subscribe_int());


	int ipc_status;
	message msg;
	int r;
	int i;

	int teclas_player[5]={0};

	while(valid) {
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {

			if(_ENDPOINT_P(msg.m_source)==HARDWARE) {
				if(msg.NOTIFY_ARG & irq_set_timer){
					//update da velocidade e desenho dos jogadores


					update_player(player, teclas_player, map);

					if (check_player_hit(player, map)){
						valid = 0;
						draw_map(map);
						swapThirdBuffer();
						draw_player(player,get_key_direction(teclas_player));
						endGame_menu_singleplayer(Player_getWinnerBmp(player), &time,0);
					}
					if (check_player_on_portal(player, map)){
						valid = 0;
						draw_map(map);
						swapThirdBuffer();
						draw_player(player,get_key_direction(teclas_player));
						endGame_menu_singleplayer(Player_getWinnerBmp(player), &time,1);
					}


					draw_map(map);
					draw_PlayerBombs(player);
					draw_side_bar_singleplayer(player);

					swapThirdBuffer();
					if (counter % 60 == 0) {

						time.seconds = (time.seconds + 1) % 60;

						if (time.seconds == 0)
							time.minutes = (time.minutes + 1) % 60;

						if (time.seconds == 0 && time.minutes == 0)
							time.hours = (time.hours + 1) % 24;
					}
					displayTime(&time);
					counter++;

					draw_side_bar_singleplayer_counter(player);
					draw_player(player,get_key_direction(teclas_player));
					swapVideoMem();
				}
				if (msg.NOTIFY_ARG & irq_set_kbd) {
					int result;
					unsigned long scan;
					result = keyboard_int_handler(&scan);
					switch (result) {
					case -1:
						vg_exit();
						return;
						break;
					case 1:
						break;
					case 0:
						//update array teclas
						if(scan== KEY_UP(KEY_ESC)) {
							if (pause())
								valid = 0;
							map_setDraw(map);
						}
						else
							switch (scan) {
							case KEY_ARR_UP:teclas_player[0]=1;//makecode
							break;
							case KEY_UP(KEY_ARR_UP):teclas_player[0]=0;//breakcode
							break;
							case KEY_ARR_LEFT:teclas_player[1]=1;
							break;
							case KEY_UP(KEY_ARR_LEFT):teclas_player[1]=0;
							break;
							case KEY_ARR_DOWN:teclas_player[2]=1;
							break;
							case KEY_UP(KEY_ARR_DOWN):teclas_player[2]=0;
							break;
							case KEY_ARR_RIGHT:teclas_player[3]=1;
							break;
							case KEY_UP(KEY_ARR_RIGHT):teclas_player[3]=0;
							break;
							case KEY_ENTER:teclas_player[4]=1;
							break;
							case KEY_UP(KEY_ENTER):teclas_player[4]=0;
							break;
							default:
								break;
							}
						break;
					}
				}
			}
		}
	}

	delete_map(map);
	destroy_player(player);
	timer_unsubscribe_int();
	keyboard_unsubscribe_int();

	return MENU;
}




GAME_STATE editMap(map_t *map, GAME_STATE gs) {

	mouse_t *m = create_mouse();
	mouse_setPosition(m, 635, 290);
	int mouse_hook = MOUSE_HOOK_NOTIFICATION;
	int irq_set_mouse = BIT(mouse_subscribe_int(&mouse_hook));
	mouse_write(SET_STREAM_MODE);
	mouse_write(ENABLE_DATA_PACKETS);


	// timer
	int irq_set_timer = BIT(timer_subscribe_int());


	// keyboard
	int irq_set_kbd = BIT(keyboard_subscribe_int());


	int ipc_status;
	message msg;
	int r;

	int numberBlocks = 0;
	while(1){
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			if (_ENDPOINT_P(msg.m_source)==HARDWARE) {
				if(msg.NOTIFY_ARG & irq_set_timer){

					draw_map(map);
					swapThirdBuffer();

					if ((is_mouse_inside(m, 10, 635, 147, 680)) && numberBlocks < 60) {
						drawBitmap(side_bar_create_map1,0,0,ALIGN_LEFT, THIRD_BUFFER);
						drawBitmap(getBlocoDestrutivel(map),50,150,ALIGN_LEFT, THIRD_BUFFER);
						draw_text("60 BLOCKS", 25, 365, THIRD_BUFFER);
						draw_text("MINIMUM", 37, 365 + VER_SIZE + 2, THIRD_BUFFER);
					}
					else if(is_mouse_inside(m, 10, 635, 147, 680)){
						drawBitmap(side_bar_create_map1,0,0,ALIGN_LEFT, THIRD_BUFFER);
						drawBitmap(getBlocoDestrutivel(map),50,150,ALIGN_LEFT, THIRD_BUFFER);
					}
					else{
						drawBitmap(side_bar_create_map,0,0,ALIGN_LEFT, THIRD_BUFFER);
						drawBitmap(getBlocoDestrutivel(map),50,150,ALIGN_LEFT, THIRD_BUFFER);

						int x_pos = mouse_getX(m) - 174;
						int y_pos = mouse_getY(m) - 18;
						if (!(x_pos < 0 || y_pos < 0)) {
							int x = (x_pos + 6) / 50;
							int y = y_pos / 50;
							if( (gs == MULTIPLAYER && ((x < 3 && y < 3) || (x > 13 && y > 11))) ||
									(gs == SINGLEPLAYER) && ((x > 6 && x < 10 && y > 5 && y < 8))) {
								draw_text("PLAYER", 40, 365, THIRD_BUFFER);
								draw_text("SPAWN", 45, 365 + VER_SIZE + 2, THIRD_BUFFER);
							}
						}
					}


					if (numberBlocks < 10)
						draw_side_bar_number_int(numberBlocks, 70, 217);
					else
						draw_side_bar_number_int(numberBlocks, 65, 217);

					draw_mouse(m,THIRD_BUFFER);
					swapVideoMem();
				}
				if(msg.NOTIFY_ARG & irq_set_mouse) {
					update_mouse(m);
				}
				if (msg.NOTIFY_ARG & irq_set_kbd){
					unsigned long scan;
					int result = keyboard_int_handler(&scan);
					if (result == 0){
						if(scan == KEY_UP(KEY_ESC)) {
							return MENU;
						}
					}
				}
			}
		}


		if (getDone(m)) {

			if (is_mouse_inside(m, 10, 635, 147, 680)) {
				if (numberBlocks >= 60) {
					map_setDraw(map);
					break;
				}
			}

			int x_pos = mouse_getX(m) - 174;
			int y_pos = mouse_getY(m) - 18;

			if (!(x_pos < 0 || y_pos < 0)) {
				int x = (x_pos + 6) / 50;
				int y = y_pos / 50;

				if( (gs == MULTIPLAYER && !((x < 3 && y < 3) || (x > 13 && y > 11))) ||
						(gs == SINGLEPLAYER) && !((x > 6 && x < 10 && y > 5 && y < 8))) {
					if (map_getContent(map, x, y) == CAMINHO && numberBlocks < 100) {
						numberBlocks++;
						map_setContent(map, x, y, BLOCO_DESTRUTIVEL);
					}
					else if (map_getContent(map, x, y) == BLOCO_DESTRUTIVEL) {
						numberBlocks--;
						map_setContent(map, x, y, CAMINHO);
					}
				}
			}
			resetDone(m);
		}
	}


	timer_unsubscribe_int();
	keyboard_unsubscribe_int();
	mouse_write(DISABLE_STREAM_MODE);
	mouse_unsubscribe_int(&mouse_hook);
	destroy_mouse(m);
	return gs;
}





void displayTime(time_info_t *time) {
	char timeStr[9];
	sprintf(timeStr, "%02d:%02d:%02d", time->hours, time->minutes, time->seconds);
	draw_text(timeStr, 35, 375, THIRD_BUFFER);
}





void initializeGamePieces() {

	initializeMenuBmp();
	initializeBombBmp();
	initializeFont();
	initializeSideBarBmp();
	initializeMapBmp();
	initializeSpriteBmp();
}


void destroyGamePieces() {

	destroyMenuBmp();
	destroySideBarBmp();
	destroyBombBmp();
	destroyMapBmp();
	destroyFont();
	destroySpriteBmp();
}
