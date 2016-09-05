/** @file */
#include "menu.h"


static Bitmap *m_menu;
static Bitmap *m_singleplayer;
static Bitmap *m_multiplayer;
static Bitmap *m_exit;
static Bitmap *m_menu_multiplayer;
static Bitmap *m_menu_quick_game;
static Bitmap *m_menu_bestof3;
static Bitmap *m_menu_bestof5;
static Bitmap *m_pause;
static Bitmap *m_continue;
static Bitmap *m_quit;
static Bitmap *m_settings;
static Bitmap *m_white[4];
static Bitmap *m_blue[4];
static Bitmap *end;
static Bitmap *end_highlighted;
static Bitmap *chamuscadito;
static Bitmap *m_singleplayer_win1;
static Bitmap *m_singleplayer_win2;
static Bitmap *m_singleplayer_lose1;
static Bitmap *m_singleplayer_lose2;

GAME_STATE main_menu() {

	// mouse
	mouse_t *m = create_mouse();
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
	int selection=-1;

	while(!(getDone(m) && (selection==0 || selection==1 || selection==2))){
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			if (_ENDPOINT_P(msg.m_source)==HARDWARE) {
				if(msg.NOTIFY_ARG & irq_set_timer){
					if(is_mouse_inside(m,561,429,901,462)){
						drawBitmap(m_singleplayer, 0,  0, ALIGN_LEFT, SECOND_BUFFER);
						selection=0;
					}
					else if(is_mouse_inside(m,601,503,901,538)){
						drawBitmap(m_multiplayer, 0,  0, ALIGN_LEFT, SECOND_BUFFER);
						selection=1;
					}
					else if(is_mouse_inside(m,801,585,901,621)){
						drawBitmap(m_exit, 0,  0, ALIGN_LEFT, SECOND_BUFFER);
						selection=2;
					}
					else{
						drawBitmap(m_menu, 0,  0, ALIGN_LEFT, SECOND_BUFFER);
						selection=3;
					}
					swapThirdBuffer();
					displayDateAndTime();
					draw_mouse(m,THIRD_BUFFER);
					swapVideoMem();
				}
				if (msg.NOTIFY_ARG & irq_set_mouse){
					update_mouse(m);
				}
				if (msg.NOTIFY_ARG & irq_set_kbd){
					unsigned long scan;
					int result = keyboard_int_handler(&scan);
					if (result == 0){
						if(scan == KEY_UP(KEY_ESC)) {
							return END;
						}
					}
				}
			}
		}
	}



	timer_unsubscribe_int();
	keyboard_unsubscribe_int();
	mouse_write(DISABLE_STREAM_MODE);
	mouse_unsubscribe_int(&mouse_hook);
	destroy_mouse(m);


	if(selection==0)
		return SINGLEPLAYER;
	else if(selection==1)
		return MULTIPLAYERSELECT;
	else
		return END;
}


GAME_STATE multiplayer_select() {

	// mouse
	mouse_t *m = create_mouse();
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
	int selection=-1;

	while (!(getDone(m) && (selection==0 || selection==1 || selection==2 || selection == 3))){
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			if (_ENDPOINT_P(msg.m_source)==HARDWARE) {
				if(msg.NOTIFY_ARG & irq_set_timer){
					if(is_mouse_inside(m,430,371,760,431)){
						drawBitmap(m_menu_quick_game, 0,  0, ALIGN_LEFT, SECOND_BUFFER);
						selection=0;
					}
					else if(is_mouse_inside(m,430,474,760,527)){
						drawBitmap(m_menu_bestof3, 0,  0, ALIGN_LEFT, SECOND_BUFFER);
						selection=1;
					}
					else if(is_mouse_inside(m,430,571,760,633)){
						drawBitmap(m_menu_bestof5, 0,  0, ALIGN_LEFT, SECOND_BUFFER);
						selection=2;
					}
					else if(is_mouse_inside(m,0,0,430,768) ||  is_mouse_inside(m,760,0,1024,768)){
						drawBitmap(m_menu_multiplayer, 0,  0, ALIGN_LEFT, SECOND_BUFFER);
						selection=3;
					}
					else{
						drawBitmap(m_menu_multiplayer, 0,  0, ALIGN_LEFT, SECOND_BUFFER);
						selection=4;
					}
					swapThirdBuffer();
					displayDateAndTime();
					draw_mouse(m,THIRD_BUFFER);
					swapVideoMem();
				}
				if (msg.NOTIFY_ARG & irq_set_mouse){
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
	}



	timer_unsubscribe_int();
	keyboard_unsubscribe_int();
	mouse_write(DISABLE_STREAM_MODE);
	mouse_unsubscribe_int(&mouse_hook);
	destroy_mouse(m);


	if(selection==0)
		return BEST_OF_1;
	else if(selection==1)
		return BEST_OF_3;
	else if (selection == 2)
		return BEST_OF_5;
	else
		return MENU;
}


int pause_menu() {

	//mouse
	mouse_t *m = create_mouse();
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
	int selection=-1;

	while(!(selection==0 || selection==1)){
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			if (_ENDPOINT_P(msg.m_source)==HARDWARE) {
				if(msg.NOTIFY_ARG & irq_set_timer){
					if(is_mouse_inside(m,310,418,484,470)){
						drawBitmap(m_continue, 0,  0, ALIGN_LEFT, THIRD_BUFFER);

						if (getDone(m)) {
							selection=0;
							resetDone(m);
						}
					}
					else if(is_mouse_inside(m,525,417,694,470)){
						drawBitmap(m_quit, 0,  0, ALIGN_LEFT, THIRD_BUFFER);

						if (getDone(m)) {
							selection=1;
							resetDone(m);
						}
					}
					else{
						drawBitmap(m_pause, 0,  0, ALIGN_LEFT, THIRD_BUFFER);
						selection=2;
					}
					swapVideoMem();
					draw_mouse(m,VIDEO_MEM);

				}
				if (msg.NOTIFY_ARG & irq_set_mouse){
					update_mouse(m);
				}
				if (msg.NOTIFY_ARG & irq_set_kbd){
					unsigned long scan;
					int result = keyboard_int_handler(&scan);
					if (result == 0){
						if(scan == KEY_UP(KEY_ESC) || scan == KEY_UP(KEY_ENTER)) {
							selection = 0;
						}
					}
				}
			}
		}
	}



	timer_unsubscribe_int();
	keyboard_unsubscribe_int();
	mouse_write(DISABLE_STREAM_MODE);
	mouse_unsubscribe_int(&mouse_hook);
	destroy_mouse(m);

	if(selection==0)
		return 1;
	else
		return 0;
}


int settings_menu(Settings *setting){

	int animation_index=0;
	int bitmaps_until_refresh=0;

	//mouse
	mouse_t *m = create_mouse();
	mouse_setPosition(m, 512, 384);
	int mouse_hook = MOUSE_HOOK_NOTIFICATION;
	int irq_set_mouse = BIT( mouse_subscribe_int(&mouse_hook));
	mouse_write(SET_STREAM_MODE);
	mouse_write(ENABLE_DATA_PACKETS);

	//timer
	int irq_set_timer = BIT(timer_subscribe_int());

	//keyboard
	int irq_set_kbd = BIT(keyboard_subscribe_int());


	int ipc_status;
	message msg;
	int r;
	int selection=0;
	int player_selection = 0;
	int map_selection = 0;


	drawBitmap(m_settings, 0,  0, ALIGN_LEFT, SECOND_BUFFER);

	while(!(selection && player_selection && map_selection)){
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			if (_ENDPOINT_P(msg.m_source)==HARDWARE) {
				if(msg.NOTIFY_ARG & irq_set_timer){
					swapThirdBuffer();


					if(player_selection==1){
						drawBitmap(m_white[animation_index % 4], 0,0, ALIGN_LEFT, THIRD_BUFFER);
						draw_rectangle(73,80, 426, 356, COLOR_RED);
					}
					else if(player_selection==2){
						drawBitmap(m_blue[animation_index % 4], 0,0, ALIGN_LEFT, THIRD_BUFFER);
						draw_rectangle(550, 80, 903, 356, COLOR_RED);
					}

					if(map_selection==1){
						draw_rectangle(79, 500, 399, 740, COLOR_RED);
					}
					else if(map_selection==2){
						draw_rectangle(584, 500, 905, 740, COLOR_RED);
					}


					if(is_mouse_inside(m,879,340,993,437)){
						draw_rectangle(880, 441, 995, 442, COLOR_YELLOW);
						if (getDone(m))
							selection=1;
						else
							selection=0;
					}
					else if(is_mouse_inside(m,73,101,426,356)){ // player branco
						if (player_selection != 1)
							draw_rectangle(73, 80, 426, 356, COLOR_YELLOW);
						if (getDone(m)) {
							if(player_selection==1)
								player_selection=0;
							else
								player_selection=1;
							resetDone(m);
						}
					}
					else if(is_mouse_inside(m,550,101,903,356)){ //player azul
						if (player_selection != 2)
							draw_rectangle(550, 80, 903, 356, COLOR_YELLOW);
						if (getDone(m)) {
							if(player_selection==2)
								player_selection=0;
							else
								player_selection=2;

							resetDone(m);
						}
					}
					else if(is_mouse_inside(m,79,500,399,740)){ //mapa aleatorio
						if (map_selection != 1)
							draw_rectangle(79, 500, 399, 740, COLOR_YELLOW);
						if (getDone(m)) {
							if(map_selection==1)
								map_selection=0;
							else
								map_selection=1;

							resetDone(m);
						}
					}
					else if(is_mouse_inside(m,584,500,905,740)){ //criar mapa
						if (map_selection != 2)
							draw_rectangle(584, 500, 905, 740, COLOR_YELLOW);
						if (getDone(m)) {
							if(map_selection==2)
								map_selection=0;
							else
								map_selection=2;

							resetDone(m);
						}
					}


					draw_mouse(m,THIRD_BUFFER);
					swapVideoMem();

					if(bitmaps_until_refresh==7){
						animation_index++;
						bitmaps_until_refresh=0;
					}
					bitmaps_until_refresh++;

				}
				if (msg.NOTIFY_ARG & irq_set_mouse){
					update_mouse(m);
				}
				if (msg.NOTIFY_ARG & irq_set_kbd){
					unsigned long scan;
					int result = keyboard_int_handler(&scan);
					if (result == 0){
						if(scan == KEY_UP(KEY_ESC)) {
							return 1;
						}
					}
				}
			}
		}
	}


	setting->player_selection = player_selection;
	setting->map_selection = map_selection;

	timer_unsubscribe_int();
	keyboard_unsubscribe_int();
	mouse_write(DISABLE_STREAM_MODE);
	mouse_unsubscribe_int(&mouse_hook);
	destroy_mouse(m);

	return 0;
}





void displayDateAndTime() {
	time_info_t time;
	date_info_t date;

	static counter = 0;
	static char timeStr[9];
	static char dateStr[22];        // 22 is the max length (when the day is Wednesday)

	if (counter == 0) {
		if (getCurrentTime(&time) == 0) {
			sprintf(timeStr, "%02d:%02d:%02d", time.hours, time.minutes, time.seconds);
		}

		if (getCurrentDate(&date) == 0) {
			char temp[12];
			switch (date.week_day) {
			case SUNDAY:
				sprintf(temp, "SUNDAY, ");
				break;
			case MONDAY:
				sprintf(temp, "MONDAY, ");
				break;
			case TUESDAY:
				sprintf(temp, "TUESDAY, ");
				break;
			case WEDNESDAY:
				sprintf(temp, "WEDNESDAY, ");
				break;
			case THURSDAY:
				sprintf(temp, "THURSDAY, ");
				break;
			case FRIDAY:
				sprintf(temp, "FRIDAY, ");
				break;
			case SATURDAY:
				sprintf(temp, "SATURDAY, ");
				break;
			}

			char data[13];
			sprintf(data, "%02d.%02d.%02d", date.month_day, date.month, date.year);

			memcpy(&dateStr, temp, strlen(temp));
			memcpy(&dateStr[strlen(temp)], data, strlen(data));
		}
	}

	counter = (counter + 1) % 60;

	draw_text(dateStr, 790, 120, THIRD_BUFFER);
	draw_text(timeStr, 790, 170, THIRD_BUFFER);
}


int pause() {
	timer_unsubscribe_int();
	keyboard_unsubscribe_int();
	if(pause_menu()==0)
		return 1;
	keyboard_subscribe_int();
	timer_subscribe_int();

	return 0;
}


void endGame_menu_multiplayer(Bitmap *winner){

	time_info_t time;
	int counter = 0;

	//mouse
	mouse_t *m = create_mouse();
	mouse_setPosition(m, 512, 384);
	int mouse_hook = MOUSE_HOOK_NOTIFICATION;
	int irq_set_mouse = BIT( mouse_subscribe_int(&mouse_hook));
	mouse_write(SET_STREAM_MODE);
	mouse_write(ENABLE_DATA_PACKETS);

	//timer
	int irq_set_timer = BIT(timer_subscribe_int());

	//keyboard
	int irq_set_kbd = BIT(keyboard_subscribe_int());


	int ipc_status;
	message msg;
	int r;
	int selection=0;

	while(selection != 1){
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			if (_ENDPOINT_P(msg.m_source)==HARDWARE) {
				if(msg.NOTIFY_ARG & irq_set_timer){
					if(is_mouse_inside(m,754,421,879,471)){
						drawBitmap(end_highlighted, 0,  0, ALIGN_LEFT, THIRD_BUFFER);
						if (getDone(m)) {
							selection = 1;
							resetDone(m);
						}
					}
					else
						drawBitmap(end, 0,  0, ALIGN_LEFT, THIRD_BUFFER);

					drawBitmap(winner, 475,  355, ALIGN_LEFT, THIRD_BUFFER);
					drawBitmap(chamuscadito, 475,  415, ALIGN_LEFT, THIRD_BUFFER);

					if (counter % 60 == 0)
						getCurrentTime(&time);
					displayTime(&time);
					counter++;

					swapVideoMem();
					draw_mouse(m,VIDEO_MEM);

				}
				if (msg.NOTIFY_ARG & irq_set_mouse){
					update_mouse(m);
				}
				if (msg.NOTIFY_ARG & irq_set_kbd){
					unsigned long scan;
					int result = keyboard_int_handler(&scan);
					if (result == 0){
						if(scan == KEY_UP(KEY_ENTER)) {
							selection = 1;
						}
					}
				}
			}
		}
	}



	timer_unsubscribe_int();
	keyboard_unsubscribe_int();
	mouse_write(DISABLE_STREAM_MODE);
	mouse_unsubscribe_int(&mouse_hook);
	destroy_mouse(m);
}

void endGame_menu_singleplayer(Bitmap *winner,time_info_t *game_time,int win){

	char timeStr[9];
	sprintf(timeStr, "%02d:%02d:%02d", game_time->hours, game_time->minutes, game_time->seconds);

	time_info_t time;
	int counter = 0;

	//mouse
	mouse_t *m = create_mouse();
	mouse_setPosition(m, 512, 384);
	int mouse_hook = MOUSE_HOOK_NOTIFICATION;
	int irq_set_mouse = BIT( mouse_subscribe_int(&mouse_hook));
	mouse_write(SET_STREAM_MODE);
	mouse_write(ENABLE_DATA_PACKETS);

	//timer
	int irq_set_timer = BIT(timer_subscribe_int());

	//keyboard
	int irq_set_kbd = BIT(keyboard_subscribe_int());


	int ipc_status;
	message msg;
	int r;
	int selection=0;

	while(selection != 1){
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			if (_ENDPOINT_P(msg.m_source)==HARDWARE) {
				if(msg.NOTIFY_ARG & irq_set_timer){
					if(win){
						if(is_mouse_inside(m,839,432,980,486)){
							drawBitmap(m_singleplayer_win2, 0,  0, ALIGN_LEFT, THIRD_BUFFER);
							if (getDone(m)) {
								selection = 1;
								resetDone(m);
							}
						}
						else
							drawBitmap(m_singleplayer_win1, 0,  0, ALIGN_LEFT, THIRD_BUFFER);

						draw_text(timeStr, 380, 445, THIRD_BUFFER);
						drawBitmap(winner, 390,  360, ALIGN_LEFT, THIRD_BUFFER);
					}
					else{
						if(is_mouse_inside(m,850,423,990,481)){
							drawBitmap(m_singleplayer_lose2, 0,  0, ALIGN_LEFT, THIRD_BUFFER);
							if (getDone(m)) {
								selection = 1;
								resetDone(m);
							}
						}
						else
							drawBitmap(m_singleplayer_lose1, 0,  0, ALIGN_LEFT, THIRD_BUFFER);

						drawBitmap(chamuscadito, 390,  350, ALIGN_LEFT, THIRD_BUFFER);
					}




					if (counter % 60 == 0)
						getCurrentTime(&time);
					displayTime(&time);
					counter++;

					swapVideoMem();
					draw_mouse(m,VIDEO_MEM);

				}
				if (msg.NOTIFY_ARG & irq_set_mouse){
					update_mouse(m);
				}
				if (msg.NOTIFY_ARG & irq_set_kbd){
					unsigned long scan;
					int result = keyboard_int_handler(&scan);
					if (result == 0){
						if(scan == KEY_UP(KEY_ENTER)) {
							selection = 1;
						}
					}
				}
			}
		}
	}



	timer_unsubscribe_int();
	keyboard_unsubscribe_int();
	mouse_write(DISABLE_STREAM_MODE);
	mouse_unsubscribe_int(&mouse_hook);
	destroy_mouse(m);
}


void initializeMenuBmp() {

	// main menu
	m_menu = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/menu_init.bmp");
	m_singleplayer = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/menu_init1.bmp");
	m_multiplayer = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/menu_init2.bmp");
	m_exit = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/menu_init3.bmp");

	// multiplayer selector menu
	m_menu_multiplayer = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/menumultiplayer.bmp");
	m_menu_quick_game = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/menumultiplayer1.bmp");
	m_menu_bestof3 = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/menumultiplayer2.bmp");
	m_menu_bestof5 = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/menumultiplayer3.bmp");

	// pause menu
	m_pause = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/pause_menu.bmp");
	m_continue = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/pause_menu1.bmp");
	m_quit = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/pause_menu2.bmp");

	// settings menu
	m_settings = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/settings.bmp");

	m_white[0] = m_settings;
	m_white[1] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2settings1.bmp");
	m_white[2] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2settings2.bmp");
	m_white[3] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/2settings3.bmp");
	m_blue[0] = m_settings;
	m_blue[1] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/settings1.bmp");
	m_blue[2] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/settings2.bmp");
	m_blue[3] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/settings3.bmp");

	// end game menu
	end = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/win_menu1.bmp");
	end_highlighted = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/win_menu2.bmp");
	chamuscadito = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/playerLose.bmp");
	//end_game_menu_singleplayer
	m_singleplayer_win1 = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/win_menu_singleplayer1.bmp");
	m_singleplayer_win2= loadBitmap("home/lcom/lcom1516-t6g01/proj/res/win_menu_singleplayer2.bmp");
	m_singleplayer_lose1 = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/lose_menu_singleplayer1.bmp");
	m_singleplayer_lose2 = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/lose_menu_singleplayer2.bmp");
}



void destroyMenuBmp() {

	deleteBitmap(m_menu);
	deleteBitmap(m_singleplayer);
	deleteBitmap(m_multiplayer);
	deleteBitmap(m_exit);

	deleteBitmap(m_menu_multiplayer);
	deleteBitmap(m_menu_quick_game);
	deleteBitmap(m_menu_bestof3);
	deleteBitmap(m_menu_bestof5);

	deleteBitmap(m_pause);
	deleteBitmap(m_continue);
	deleteBitmap(m_quit);

	deleteBitmap(m_settings);

	int i;
	for (i = 1; i < 4; i++) {
		deleteBitmap(m_white[i]);
		deleteBitmap(m_blue[i]);
	}

	deleteBitmap(end);
	deleteBitmap(end_highlighted);
	deleteBitmap(chamuscadito);

	deleteBitmap(m_singleplayer_win1);
	deleteBitmap(m_singleplayer_win2);
	deleteBitmap(m_singleplayer_lose1);
	deleteBitmap(m_singleplayer_lose2);
}
