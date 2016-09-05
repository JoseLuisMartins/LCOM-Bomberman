/** @file */
#include "Player.h"

static int number_players = 0;


struct player {
	int x, y;//posicoes no array do mapa
	int numVictories;
	PLAYER_STATE playerState;

	int max_bombs;
	int placed_bombs;
	int velocity_boost;	 //on when the player has a Velocity boost
	int range_boost;	 //on when the player has a Range boost
	int kick_power_up; 	 // on when the player has the kick power up
	int bitmaps_until_refresh;
	int animation_index;
	int bomb_range; 			//range of the player bombs
	bomb_t *bombs[MAX_NUM_BOMBS];
	Sprite *sprite;//tem as posicoes em pixels...

	//power-up`s counters
	double range_counter;
	double velocity_counter;
	double kick_counter;
};



player_t* create_player(int x, int y, int player_color){
	player_t *player = malloc(sizeof(player_t));

	player->x = x;
	player->y = y;
	player->numVictories = 0;

	int i;
	for (i = 0; i < MAX_NUM_BOMBS; ++i) {
		player->bombs[i]=NULL;
	}

	player->playerState = ALIVE;

	player->max_bombs = 1;
	player->placed_bombs = 0;

	player->velocity_boost = 0;
	player->range_boost = 0;
	player->kick_power_up = 0;

	player->bomb_range = DEFAULT_RANGE;

	player->sprite = create_sprite(174+50*player->x, 18+50*player->y,player_color);

	player->bitmaps_until_refresh=0;
	player->animation_index=0;

	number_players++;

	player->range_counter = 0;
	player->velocity_counter = 0;
	player->kick_counter = 0;

	return player;
}


void move_player_handler(player_t *player,map_t* map,DIRECTION d){
	int x_pos = player->sprite->x - 174;
	int y_pos = player->sprite->y - 18;

	int notAligned;
	int offset=25;

	if (d==UP)
	{
		notAligned = x_pos % 50;
		if(notAligned<=offset){//offset
			player->sprite->x -= notAligned;
			notAligned=0;
		}
		else if(notAligned>=50-offset){
			player->sprite->x += (50-notAligned);
			notAligned=0;
		}

		updatePlayerPosition(player);

		if (!notAligned)
			if(check_collision(map, player->x, player->y, x_pos, y_pos, d) == 0)
				player->sprite->y -= player->sprite->yspeed;
	}else if (d==DOWN)
	{
		notAligned = x_pos % 50;
		if(notAligned<=offset){//offset
			player->sprite->x -= notAligned;
			notAligned=0;
		}
		else if(notAligned>=50-offset){
			player->sprite->x += (50-notAligned);
			notAligned=0;
		}

		updatePlayerPosition(player);

		if (!notAligned)
			if(check_collision(map, player->x, player->y,x_pos,y_pos, d) == 0)
				player->sprite->y += player->sprite->yspeed;
	}else if (d==LEFT)
	{
		notAligned = y_pos % 50;
		if(notAligned<=offset){//offset
			player->sprite->y -= notAligned;
			notAligned=0;
		}
		else if(notAligned>=50-offset){
			player->sprite->y += (50-notAligned);
			notAligned=0;
		}


		updatePlayerPosition(player);

		if (!notAligned)
			if(check_collision(map, player->x, player->y,x_pos,y_pos, d) == 0)
				player->sprite->x -= player->sprite->xspeed;
	}else if (d==RIGHT)
	{
		notAligned = y_pos % 50;
		if(notAligned<=offset){//offset
			player->sprite->y -= notAligned;
			notAligned=0;
		}
		else if(notAligned>=50-offset){
			player->sprite->y += (50-notAligned);
			notAligned=0;
		}

		updatePlayerPosition(player);

		if (!notAligned)
			if(check_collision(map, player->x, player->y,x_pos,y_pos, d) == 0)
				player->sprite->x += player->sprite->xspeed;
	}

	updatePlayerPosition(player);

}

void updatePlayerPosition(player_t *player){
	//atualizar o indice x e y da personagem no mapa
	int x_pos = player->sprite->x - 174;
	int y_pos = player->sprite->y - 18;

	if(x_pos % 50 > 25)
		player->x = x_pos/50 + 1;
	else
		player->x = x_pos/50;

	if(y_pos % 50 > 25)
		player->y = y_pos/50 + 1;
	else
		player->y = y_pos/50;
}

void bombs_player_handler(player_t *player,map_t* map,int place_bomb){
	int i;

	//colocar bombas

	//if (teclas_player[BOMB_INDEX])
	if(place_bomb)
	{
		if(map_getContent(map,player->x,player->y)!=BOMBA)
			if (player->placed_bombs < player->max_bombs)
			{
				for (i = 0; i < player->max_bombs; ++i) {
					if(player->bombs[i]==NULL){
						player->bombs[i]=create_bomb(player->x,player->y,player->bomb_range);
						break;
					}
				}
				player->placed_bombs++;
			}
	}

	//atualizar bombas
	for (i = 0; i < MAX_NUM_BOMBS; ++i) {
		if(player->bombs[i]!=NULL){
			update_bomb(player->bombs[i]);
			update_map(map, player->bombs[i]);
		}
	}

	//apagar bombas
	for (i = 0;  i < MAX_NUM_BOMBS; ++i) {
		if(player->bombs[i]!=NULL)
			if(Bomb_getState(player->bombs[i]) == DONE){
				delete_bomb(player->bombs[i]);
				player->bombs[i]=NULL;
				player->placed_bombs--;
			}
	}

}



void draw_PlayerBombs(player_t *player){
	int i;

	for (i = 0;  i < MAX_NUM_BOMBS; ++i) {
		if(player->bombs[i]!=NULL)
			if(Bomb_getState(player->bombs[i]) != EXPLODING){
				draw_Bomb(player->bombs[i]);
			}
	}
}


void power_ups_player_handler(player_t *player, map_t* map,DIRECTION d){
	int i;
	//power_up bomba
	if(map_getContent(map,player->x,player->y)==BOMBA_POWER_UP){
		if (player->max_bombs < 20)
			player->max_bombs++;
		map_setContent(map,player->x,player->y,CAMINHO);
		map_setDraw(map);
	}

	//power_up range bomba
	if(map_getContent(map,player->x,player->y)==RANGE_POWER_UP){
		player->range_counter += 10;
		map_setContent(map,player->x,player->y,CAMINHO);
		map_setDraw(map);
		player->bomb_range++;
		player->range_boost=1;
	}

	if(player->range_counter > 0){
		player->range_counter -=(1/60.0);
	}else if(player->range_boost){
		player->bomb_range=DEFAULT_RANGE;
		player->range_boost = 0;
		player->range_counter = 0;
	}

	//power_up velocidade player
	if(map_getContent(map,player->x,player->y)==VELOCITY_POWER_UP){
		player->velocity_counter += 10;
		map_setContent(map,player->x,player->y,CAMINHO);
		map_setDraw(map);
		if(player->velocity_boost==0){
			player->sprite->x=174+player->x*50;
			player->sprite->y=18+player->y*50;
			player->sprite->xspeed=10;
			player->sprite->yspeed=10;
			player->velocity_boost=1;
		}
	}

	if(player->velocity_counter > 0){
		player->velocity_counter -=(1/60.0);
	}else if(player->velocity_boost){
		player->sprite->xspeed=5;
		player->sprite->yspeed=5;
		player->velocity_boost = 0;
		player->velocity_counter = 0;
	}

	//power up chutar a bomba

	if(map_getContent(map,player->x,player->y)==KICK_POWER_UP){
		map_setContent(map,player->x,player->y,CAMINHO);
		map_setDraw(map);
		player->kick_counter+=10;
		player->kick_power_up = 1;
	}


	if(player->kick_counter > 0){
		player->kick_counter -=(1/60.0);
		switch (d) {
		case UP:
			if(player->y > 1 && map_getContent(map,player->x,(player->y-1)) == BOMBA){
				for (i = 0;  i < MAX_NUM_BOMBS; ++i) {
					if(player->bombs[i]!=NULL){
						if(Bomb_getX(player->bombs[i]) == player->x && Bomb_getY(player->bombs[i]) == (player->y-1)){
							int y = Bomb_getY(player->bombs[i]);
							int deslocamento = 0;
							while(y > 0){
								if(map_getContent(map,player->x,y-1)==CAMINHO || map_getContent(map,player->x,y-1)==CAM_HOR_EXPLODE || map_getContent(map,player->x,y-1)==CAM_VER_EXPLODE ){
									deslocamento++;
									y--;
								}else
									break;
							}
							if(deslocamento > 0){
								map_setContent(map,Bomb_getX(player->bombs[i]),Bomb_getY(player->bombs[i]),CAMINHO);
								move_bomb(player->bombs[i],UP,player->x*50+174,(Bomb_getY(player->bombs[i])-deslocamento)*50+18);
							}
							else
								break;
						}
					}
				}
			}
			break;
		case DOWN:
			if(player->y < 13 && map_getContent(map,player->x,(player->y+1)) == BOMBA){
				for (i = 0;  i < MAX_NUM_BOMBS; ++i) {
					if(player->bombs[i]!=NULL){
						if(Bomb_getX(player->bombs[i]) == player->x && Bomb_getY(player->bombs[i]) == (player->y+1)){
							int y = Bomb_getY(player->bombs[i]);
							int deslocamento = 0;
							while(y < 14){
								if(map_getContent(map,player->x,y+1)==CAMINHO || map_getContent(map,player->x,y+1)==CAM_HOR_EXPLODE || map_getContent(map,player->x,y+1)==CAM_VER_EXPLODE){
									deslocamento++;
									y++;
								}else
									break;
							}
							if(deslocamento > 0){
								map_setContent(map,Bomb_getX(player->bombs[i]),Bomb_getY(player->bombs[i]),CAMINHO);
								move_bomb(player->bombs[i],DOWN,player->x*50+174,(Bomb_getY(player->bombs[i])+deslocamento)*50+18);
							}
							else
								break;
						}
					}
				}
			}
			break;
		case LEFT:
			if(player->x > 1  && map_getContent(map,(player->x-1),player->y) == BOMBA){
				for (i = 0;  i < MAX_NUM_BOMBS; ++i) {
					if(player->bombs[i]!=NULL){
						if(Bomb_getX(player->bombs[i]) == (player->x-1) && Bomb_getY(player->bombs[i]) == player->y){
							int x = Bomb_getX(player->bombs[i]);
							int deslocamento = 0;
							while(x > 0){
								if(map_getContent(map,x-1,player->y)==CAMINHO || map_getContent(map,x-1,player->y)==CAM_HOR_EXPLODE || map_getContent(map,x-1,player->y)==CAM_VER_EXPLODE){
									deslocamento++;
									x--;
								}else
									break;
							}
							if(deslocamento > 0){
								map_setContent(map,Bomb_getX(player->bombs[i]),Bomb_getY(player->bombs[i]),CAMINHO);
								move_bomb(player->bombs[i],LEFT,(Bomb_getX(player->bombs[i])-deslocamento)*50+174,player->y*50+18);
							}
							else
								break;
						}
					}
				}
			}
			break;
		case RIGHT:
			if(player->x < 16  && map_getContent(map,(player->x+1),player->y) == BOMBA){
				for (i = 0;  i < MAX_NUM_BOMBS; ++i) {
					if(player->bombs[i]!=NULL){
						if(Bomb_getX(player->bombs[i]) == (player->x+1) && Bomb_getY(player->bombs[i]) == player->y){
							int x = Bomb_getX(player->bombs[i]);
							int deslocamento = 0;
							while(x < 17){
								if(map_getContent(map,x+1,player->y)==CAMINHO || map_getContent(map,x+1,player->y)==CAM_HOR_EXPLODE || map_getContent(map,x+1,player->y) == CAM_VER_EXPLODE){
									deslocamento++;
									x++;
								}else
									break;
							}
							if(deslocamento > 0){
								map_setContent(map,Bomb_getX(player->bombs[i]),Bomb_getY(player->bombs[i]),CAMINHO);
								move_bomb(player->bombs[i],RIGHT,(Bomb_getX(player->bombs[i])+deslocamento)*50+174,player->y*50+18);
							}
							else
								break;
						}
					}
				}
			}
			break;
		}
	}else if(player->kick_power_up){
		player->kick_power_up = 0;
		player->kick_counter = 0;
	}
}



void update_player(player_t *player, int *teclas_player, map_t* map){

	DIRECTION d = get_key_direction(teclas_player);

	power_ups_player_handler(player,map,d);
	move_player_handler(player,map,d);
	bombs_player_handler(player,map,teclas_player[BOMB_INDEX]);

}



void draw_player(player_t *player,DIRECTION d){

	switch (d) {
	case UP:
		drawBitmap(player->sprite->up[(player->animation_index)%(ANIMATION_ARRAY_SIZE)],  player->sprite->x,  player->sprite->y, ALIGN_LEFT, THIRD_BUFFER);
		break;
	case DOWN:
		drawBitmap(player->sprite->down[(player->animation_index)%(ANIMATION_ARRAY_SIZE)],  player->sprite->x,  player->sprite->y, ALIGN_LEFT, THIRD_BUFFER);
		break;
	case LEFT:
		drawBitmap(player->sprite->left[(player->animation_index)%(ANIMATION_ARRAY_SIZE)],  player->sprite->x,  player->sprite->y, ALIGN_LEFT, THIRD_BUFFER);
		break;
	case RIGHT:
		drawBitmap(player->sprite->right[(player->animation_index)%(ANIMATION_ARRAY_SIZE)],  player->sprite->x,  player->sprite->y, ALIGN_LEFT, THIRD_BUFFER);
		break;
	default:
		drawBitmap(player->sprite->down[0],  player->sprite->x,  player->sprite->y, ALIGN_LEFT, THIRD_BUFFER);
		break;
	}
	if(player->bitmaps_until_refresh==4){
		player->animation_index++;
		player->bitmaps_until_refresh=0;
	}
	player->bitmaps_until_refresh++;
}


void destroy_player(player_t *player)
{
	destroy_sprite(player->sprite);
	free(player);
}




int Player_getX(player_t *player)
{
	int out;
	asm ("movl %1, %%eax;  movl %%eax, %0;"
		:"=r"(out)        /* output */
		:"r"(player->x)         /* input */
		:"%eax"         /* clobbered register */
	);
	return out;
}



int Player_getY(player_t *player)
{
	int out;
	asm ("movl %1, %%eax;  movl %%eax, %0;"
		:"=r"(out)        /* output */
		:"r"(player->y)         /* input */
		:"%eax"         /* clobbered register */
		);
	return out;
}



bomb_t** Player_getBombs(player_t *player)
{
	return player->bombs;
}


int Player_MaxBombs(player_t * player) {
	int out;
	asm ("movl %1, %%eax;  movl %%eax, %0;"
		:"=r"(out)        /* output */
		:"r"(player->max_bombs)         /* input */
		:"%eax"         /* clobbered register */
		);
	return out;
}


int Player_PlacedBombs(player_t *player)
{
	int out;
	asm ("movl %1, %%eax;  movl %%eax, %0;"
	     :"=r"(out)        /* output */
	     :"r"(player->placed_bombs)         /* input */
	     :"%eax"         /* clobbered register */
	     );
	return out;
}



void Player_decreaseBombs(player_t *player)
{
	player->placed_bombs--;
}


int check_player_hit(player_t *player,map_t *map){

	if (map_getContent(map, player->x, player->y) >= BOMB_EXPLOSION &&  map_getContent(map, player->x, player->y) <= DES_VER_EXPLODE)
		return 1;

	return 0;
}


int check_player_on_portal(player_t *player,map_t *map) {

	if (map_getContent(map, player->x, player->y)  == PORTAL)
		return 1;
	else
		return 0;
}



Bitmap* Player_getBitmap(player_t *player){
	return player->sprite->down[0];
}


int Player_getVelocity_boost(player_t *player){
	int out;
	asm ("movl %1, %%eax;  movl %%eax, %0;"
		:"=r"(out)        /* output */
		:"r"(player->velocity_boost)         /* input */
		:"%eax"         /* clobbered register */
		);
	return out;
}



int Player_getrange_boost(player_t *player){
	int out;
	asm ("movl %1, %%eax;  movl %%eax, %0;"
		:"=r"(out)        /* output */
		:"r"(player->range_boost)         /* input */
		:"%eax"         /* clobbered register */
		);
	return out;
}


int Player_getKickBoost(player_t *player) {
	int out;
	asm ("movl %1, %%eax;  movl %%eax, %0;"
		:"=r"(out)        /* output */
		:"r"(player->kick_power_up)         /* input */
		:"%eax"         /* clobbered register */
		);
	return out;
}


double Player_getRangeCounter(player_t *player) {
	double out;
	asm ("movl %1, %%eax;  movl %%eax, %0;"
		:"=r"(out)        /* output */
		:"r"(player->range_counter)         /* input */
		:"%eax"         /* clobbered register */
		);
	return out;
}


double Player_getVelocityCounter(player_t *player) {
	double out;
	asm ("movl %1, %%eax;  movl %%eax, %0;"
		:"=r"(out)        /* output */
		:"r"(player->velocity_counter)         /* input */
		:"%eax"         /* clobbered register */
		);
	return out;
}


double Player_getKickCounter(player_t *player) {
	double out;
	asm ("movl %1, %%eax;  movl %%eax, %0;"
		:"=r"(out)        /* output */
		:"r"(player->kick_counter)         /* input */
		:"%eax"         /* clobbered register */
		);
	return out;
}



Bitmap* Player_getWinnerBmp(player_t *player) {
	return player->sprite->player_win;
}



int Player_getNumVictories(player_t *player) {
	int out;
	asm ("movl %1, %%eax;  movl %%eax, %0;"
		:"=r"(out)        					/* output */
		:"r"(player->numVictories)         /* input */
		:"%eax"        					  /* clobbered register */
	);
	return out;
}


void Player_incVictories(player_t *player) {
	player->numVictories++;
}



void resetPlayer(player_t *player, int x, int y) {

	player->x = x;
	player->y = y;
	player->sprite->x = 174+50*player->x;
	player->sprite->y = 18+50*player->y;

	int i;
	for (i = 0; i < MAX_NUM_BOMBS; ++i) {
		player->bombs[i] = NULL;
	}

	player->playerState = ALIVE;

	player->max_bombs = 1;
	player->placed_bombs = 0;

	player->velocity_boost = 0;
	player->range_boost = 0;
	player->kick_power_up = 0;
	player->bomb_range = DEFAULT_RANGE;

	player->range_counter = 0;
	player->velocity_counter = 0;
	player->kick_counter = 0;
	player->sprite->xspeed = 5;
	player->sprite->yspeed = 5;
}




void playerDead(player_t *player) {
	player->playerState = DEAD;
}


PLAYER_STATE Player_getState(player_t *player) {
	return player->playerState;
}
