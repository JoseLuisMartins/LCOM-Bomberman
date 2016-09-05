/** @file */

#include "map.h"


static Bitmap *bloco_destrutivel;
static Bitmap *bloco_indestrutivel;
static Bitmap *bloco_caminho;
static Bitmap *barra_cima;
static Bitmap *barra_lado;
static Bitmap *portal[5];
static Bitmap *bomb_explosion[ANIMATION_ARRAY_SIZE];
static Bitmap *cam_hor_explosion[ANIMATION_ARRAY_SIZE];
static Bitmap *cam_ver_explosion[ANIMATION_ARRAY_SIZE];
static Bitmap *des_hor_explosion[ANIMATION_ARRAY_SIZE];
static Bitmap *des_ver_explosion[ANIMATION_ARRAY_SIZE];
static Bitmap *bomb_power_up;
static Bitmap *range_power_up;
static Bitmap *velocity_power_up;
static Bitmap *kick_power_up;




struct map {
	//map pieces
	Bitmap *bloco_destrutivel;
	Bitmap *bloco_indestrutivel;
	Bitmap *bloco_caminho;
	Bitmap *barra_cima;
	Bitmap *barra_lado;

	// portal
	Bitmap *portal[5];

	//explosion
	Bitmap *bomb_explosion[ANIMATION_ARRAY_SIZE];
	Bitmap *cam_hor_explosion[ANIMATION_ARRAY_SIZE];
	Bitmap *cam_ver_explosion[ANIMATION_ARRAY_SIZE];
	Bitmap *des_hor_explosion[ANIMATION_ARRAY_SIZE];
	Bitmap *des_ver_explosion[ANIMATION_ARRAY_SIZE];

	//power-ups
	Bitmap *bomb_power_up;
	Bitmap *range_power_up;
	Bitmap *velocity_power_up;
	Bitmap *kick_power_up;

	//map data
	int bitmaps_until_refresh;
	int animation_index;
	int mapa[17][15];
	int draw;
};



int even(int x){
	if(x%2==0)
		return 1;
	else
		return 0;
}


map_t* create_map(int selection,GAME_STATE gs){

	/* Initializes random number generator */
	time_t t;
	srand((unsigned) time(&t));

	map_t *m = malloc(sizeof(map_t));
	m->bloco_destrutivel = bloco_destrutivel;
	m->bloco_indestrutivel = bloco_indestrutivel;
	m->bloco_caminho = bloco_caminho;
	m->barra_cima = barra_cima;
	m->barra_lado = barra_lado;


	int i;
	if (gs == SINGLEPLAYER) {
		for (i = 0; i < 5; i++)
			m->portal[i] = portal[i];
	}

	for (i = 0; i < ANIMATION_ARRAY_SIZE; i++) {
		m->bomb_explosion[i] = bomb_explosion[i];
		m->cam_hor_explosion[i] = cam_hor_explosion[i];
		m->cam_ver_explosion[i] = cam_ver_explosion[i];
		m->des_hor_explosion[i] = des_hor_explosion[i];
		m->des_ver_explosion[i] = des_ver_explosion[i];
	}


	m->bomb_power_up = bomb_power_up;
	m->range_power_up = range_power_up;
	m->velocity_power_up = velocity_power_up;
	m->kick_power_up = kick_power_up;

	m->animation_index=0;
	m->bitmaps_until_refresh=0;


	if(selection==1){
		if (gs == MULTIPLAYER) {
			int x,y;
			for ( x= 0; x < 17; ++x) {
				for (y = 0; y < 15; ++y) {
					if(even(x) || even(y)){
						if((x < 3 && y < 3) || (x > 13 && y > 11)){
							m->mapa[x][y]=CAMINHO;
						}
						else{
							if((rand() % 2))
								m->mapa[x][y]=BLOCO_DESTRUTIVEL;
							else
								m->mapa[x][y]=CAMINHO;
						}
					}
					else
						m->mapa[x][y]=BLOCO_INDESTRUTIVEL;
				}
			}
		}
		else if (gs == SINGLEPLAYER) {
			int x,y;
			for ( x= 0; x < 17; ++x) {
				for (y = 0; y < 15; ++y) {
					if(even(x) || even(y)){
						if(x > 6 && x < 10 && y > 5 && y < 8){
							m->mapa[x][y]=CAMINHO;
						}
						else{
							if((rand() % 2))
								m->mapa[x][y]=BLOCO_DESTRUTIVEL;
							else
								m->mapa[x][y]=CAMINHO;
						}
					}
					else
						m->mapa[x][y]=BLOCO_INDESTRUTIVEL;
				}
			}

			place_portal(m);
		}
	}
	else{
		int x,y;
		for ( x= 0; x < 17; ++x) {
			for (y = 0; y < 15; ++y) {
				if(even(x) || even(y))
					m->mapa[x][y] = CAMINHO;
				else
					m->mapa[x][y] = BLOCO_INDESTRUTIVEL;
			}
		}

	}

	m->draw=1;
	return m;
}

void place_portal(map_t* m){
	int side = rand() % 8;
	switch (side) {
	case 0:
		m->mapa[rand() % 17][0] = PORTAL;
		break;
	case 1:
		m->mapa[rand() % 17][1] = PORTAL;
		break;
	case 2:
		m->mapa[rand() % 17][14] = PORTAL;
		break;
	case 3:
		m->mapa[rand() % 17][13] = PORTAL;
		break;
	case 4:
		m->mapa[0][rand() % 15] = PORTAL;
		break;
	case 5:
		m->mapa[1][rand() % 15] = PORTAL;
		break;
	case 6:
		m->mapa[16][rand() % 15] = PORTAL;
		break;
	case 7:
		m->mapa[15][rand() % 15] = PORTAL;
		break;

	}
}

int update_map(map_t* m, bomb_t* b)
{
	int x = Bomb_getX(b);
	int y = Bomb_getY(b);
	int range = Bomb_getRange(b);
	m->draw=1;

	/* Initializes random number generator for power ups*/
	time_t t;
	srand((unsigned) time(&t));
	BOMB_STATE currentState = Bomb_getState(b);

	if (currentState == DONE){
		int i;
		for (i = x - range; i <= x + range; i++)
		{
			if (i >= 0 && i <= 16)
			{
				if (m->mapa[i][y] >= CAM_HOR_EXPLODE && m->mapa[i][y] <= DES_VER_EXPLODE)
				{
					if(m->mapa[i][y] == DES_HOR_EXPLODE || m->mapa[i][y] == DES_VER_EXPLODE)
						m->mapa[i][y] = powerUpSelector();
					else
						m->mapa[i][y] = CAMINHO;
				}
			}
		}

		for (i = y - range; i <= y + range; i++)
		{
			if (i >= 0 && i <= 14)
			{
				if (m->mapa[x][i] >= CAM_HOR_EXPLODE && m->mapa[x][i] <= DES_VER_EXPLODE)
				{
					if(m->mapa[x][i] == DES_HOR_EXPLODE || m->mapa[x][i] == DES_VER_EXPLODE)
						m->mapa[x][i] = powerUpSelector();
					else
						m->mapa[x][i] = CAMINHO;
				}
			}
		}

		m->mapa[x][y] = CAMINHO;
	}
	else if(currentState == EXPLODING){
		int i;
		for (i = x-1; i >= x - range; i--)
		{
			if (i >= 0 && i <= 16){
				if (m->mapa[i][y] == BLOCO_INDESTRUTIVEL || m->mapa[i][y] == PORTAL)
					break;
				else {
					if(m->mapa[i][y] == BLOCO_DESTRUTIVEL || m->mapa[i][y] == DES_HOR_EXPLODE){
						m->mapa[i][y] = DES_HOR_EXPLODE;
						continue;
					}
					else
						m->mapa[i][y] = CAM_HOR_EXPLODE;
				}
			}
		}

		for (i = x+1; i <= x + range; i++)
		{
			if (i >= 0 && i <= 16)
				if (m->mapa[i][y] == BLOCO_INDESTRUTIVEL || m->mapa[i][y] == PORTAL)
					break;
				else if(m->mapa[i][y] == BLOCO_DESTRUTIVEL || m->mapa[i][y] == DES_HOR_EXPLODE )
					m->mapa[i][y] = DES_HOR_EXPLODE;
				else
					m->mapa[i][y] = CAM_HOR_EXPLODE;
		}

		for (i = y-1; i >= y - range; i--)
		{	if (i >= 0 && i <= 14)
			if (m->mapa[x][i] == BLOCO_INDESTRUTIVEL || m->mapa[x][i] == PORTAL)
				break;
			else if(m->mapa[x][i] == BLOCO_DESTRUTIVEL || m->mapa[x][i] == DES_VER_EXPLODE)
				m->mapa[x][i] = DES_VER_EXPLODE;
			else
				m->mapa[x][i] = CAM_VER_EXPLODE;
		}
		for (i = y+1; i <= y + range; i++)
		{	if (i >= 0 && i <= 14)
			if (m->mapa[x][i] == BLOCO_INDESTRUTIVEL || m->mapa[x][i] == PORTAL)
				break;
			else if(m->mapa[x][i] == BLOCO_DESTRUTIVEL || m->mapa[x][i] == DES_VER_EXPLODE)
				m->mapa[x][i] = DES_VER_EXPLODE;
			else
				m->mapa[x][i] = CAM_VER_EXPLODE;
		}
		m->mapa[x][y] = BOMB_EXPLOSION;
		return 1;
	} else if(Bomb_getKicked(b)==0)
		m->mapa[x][y] = BOMBA;


	return 0;
}



void draw_map(map_t *m){

	int x,y;

	drawBitmap(m->barra_cima,156,0,ALIGN_LEFT, SECOND_BUFFER);
	drawBitmap(m->barra_lado,156,18,ALIGN_LEFT, SECOND_BUFFER);

	for (x = 0; x < 17; ++x) {
		for (y = 0; y < 15; ++y) {
			switch (m->mapa[x][y]) {
			case BLOCO_DESTRUTIVEL:
				drawBitmap(m->bloco_destrutivel,174+x*50,18+y*50,ALIGN_LEFT, SECOND_BUFFER);
				break;
			case BLOCO_INDESTRUTIVEL:
				drawBitmap(m->bloco_indestrutivel,174+x*50,18+y*50,ALIGN_LEFT, SECOND_BUFFER);
				break;
			case BOMBA:
				drawBitmap(m->bloco_caminho,174+x*50,18+y*50,ALIGN_LEFT, SECOND_BUFFER);
				break;
			case BOMB_EXPLOSION:
				drawBitmap(m->bomb_explosion[(m->animation_index) % (ANIMATION_ARRAY_SIZE)],174+x*50,18+y*50,ALIGN_LEFT, SECOND_BUFFER);
				break;
			case CAM_HOR_EXPLODE:
				drawBitmap(m->cam_hor_explosion[(m->animation_index) % (ANIMATION_ARRAY_SIZE)],174+x*50,18+y*50,ALIGN_LEFT, SECOND_BUFFER);
				break;
			case CAM_VER_EXPLODE:
				drawBitmap(m->cam_ver_explosion[(m->animation_index) % (ANIMATION_ARRAY_SIZE)],174+x*50,18+y*50,ALIGN_LEFT, SECOND_BUFFER);
				break;
			case DES_HOR_EXPLODE:
				drawBitmap(m->des_hor_explosion[(m->animation_index) % (ANIMATION_ARRAY_SIZE)],174+x*50,18+y*50,ALIGN_LEFT, SECOND_BUFFER);
				break;
			case DES_VER_EXPLODE:
				drawBitmap(m->des_ver_explosion[(m->animation_index) % (ANIMATION_ARRAY_SIZE)],174+x*50,18+y*50,ALIGN_LEFT, SECOND_BUFFER);
				break;
			case BOMBA_POWER_UP:
				drawBitmap(m->bomb_power_up,174+x*50,18+y*50,ALIGN_LEFT, SECOND_BUFFER);
				break;
			case RANGE_POWER_UP:
				drawBitmap(m->range_power_up,174+x*50,18+y*50,ALIGN_LEFT, SECOND_BUFFER);
				break;
			case VELOCITY_POWER_UP:
				drawBitmap(m->velocity_power_up,174+x*50,18+y*50,ALIGN_LEFT, SECOND_BUFFER);
				break;
			case KICK_POWER_UP:
				drawBitmap(m->kick_power_up,174+x*50,18+y*50,ALIGN_LEFT, SECOND_BUFFER);
				break;
			case PORTAL:
				drawBitmap(m->portal[(m->animation_index) % 5],174+x*50,18+y*50,ALIGN_LEFT, SECOND_BUFFER);
				break;
			default:
				drawBitmap(m->bloco_caminho,174+x*50,18+y*50,ALIGN_LEFT, SECOND_BUFFER);
				break;
			}
		}
	}

	if(	m->bitmaps_until_refresh==7){
		m->animation_index++;
		m->bitmaps_until_refresh=0;
	}
	m->bitmaps_until_refresh++;

	m->draw=0;
}


int check_collision(map_t* m, int x, int y,int xpos,int ypos, DIRECTION d) {

	switch(d){
	case UP:
		if ((y==0 && ypos==0) || ((m->mapa[x][y-1] == BLOCO_DESTRUTIVEL || m->mapa[x][y-1] == BLOCO_INDESTRUTIVEL  || m->mapa[x][y-1] == BOMBA) && (ypos == y*50)))
			return 1;
		break;
	case DOWN:
		if ((y==14 && ypos==14*50)|| ((m->mapa[x][y+1] == BLOCO_DESTRUTIVEL || m->mapa[x][y+1] == BLOCO_INDESTRUTIVEL || m->mapa[x][y+1] == BOMBA) && (ypos == y*50)))
			return 1;
		break;
	case LEFT:
		if ((x==0 && xpos==0)|| ((m->mapa[x-1][y] == BLOCO_DESTRUTIVEL || m->mapa[x-1][y] == BLOCO_INDESTRUTIVEL || m->mapa[x-1][y] == BOMBA) && (xpos == x*50)))
			return 1;
		break;
	case RIGHT:
		if ((x==16 && xpos==16*50)|| ((m->mapa[x+1][y] == BLOCO_DESTRUTIVEL || m->mapa[x+1][y] == BLOCO_INDESTRUTIVEL || m->mapa[x+1][y] == BOMBA) && (xpos == x*50)))
			return 1;
		break;
	}

	return 0;

}


void delete_map(map_t* m){
	free(m);
}


int map_getContent(map_t* m, int x, int y)
{
	return m->mapa[x][y];
}

void map_setContent(map_t* m, int x, int y,int content){
	m->mapa[x][y]=content;
}

int map_getDraw(map_t* m){
	return m->draw;
}

void map_setDraw(map_t* m){
	m->draw=1;
}



int powerUpSelector() {
	switch (rand() % 8) {
	case 1:
		return BOMBA_POWER_UP;
		break;
	case 2:
		return BOMBA_POWER_UP;
		break;
	case 3:
		return RANGE_POWER_UP;
		break;
	case 4:
		return VELOCITY_POWER_UP;
		break;
	case 5:
		return KICK_POWER_UP;
		break;
	default:
		return CAMINHO;
		break;
	}
}



Bitmap* getBlocoDestrutivel(map_t* m) {
	return m->bloco_destrutivel;
}




void initializeMapBmp() {

	bloco_destrutivel = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/des.bmp");
	bloco_indestrutivel = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/ind.bmp");
	bloco_caminho = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/cam.bmp");
	barra_cima = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/barra_cima.bmp");
	barra_lado = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/barra_lado.bmp");

	portal[0] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/portal.bmp");
	portal[1] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/portal1.bmp");
	portal[2] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/portal2.bmp");
	portal[3] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/portal3.bmp");
	portal[4] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/portal4.bmp");

	bomb_explosion[0] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/bombExplosion.bmp");
	bomb_explosion[1] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/bombExplosion1.bmp");
	bomb_explosion[2] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/bombExplosion2.bmp");

	cam_hor_explosion[0] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/camExplosionHor.bmp");
	cam_hor_explosion[1] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/camExplosionHor1.bmp");
	cam_hor_explosion[2] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/camExplosionHor2.bmp");

	cam_ver_explosion[0] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/camExplosionVer.bmp");
	cam_ver_explosion[1] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/camExplosionVer1.bmp");
	cam_ver_explosion[2] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/camExplosionVer2.bmp");

	des_hor_explosion[0] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/desExplosion_HOR.bmp");
	des_hor_explosion[1] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/desExplosion_HOR1.bmp");
	des_hor_explosion[2] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/desExplosion_HOR2.bmp");

	des_ver_explosion[0] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/desExplosion_VER.bmp");
	des_ver_explosion[1] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/desExplosion_VER1.bmp");
	des_ver_explosion[2] = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/desExplosion_VER2.bmp");

	bomb_power_up = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/bombPowerUp.bmp");
	range_power_up = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/rangePowerUp.bmp");
	velocity_power_up = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/velocityPowerUp.bmp");
	kick_power_up = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/kickPowerUp.bmp");
}



void destroyMapBmp() {

	deleteBitmap(bloco_destrutivel);
	deleteBitmap(bloco_indestrutivel);
	deleteBitmap(bloco_caminho);
	deleteBitmap(barra_cima);
	deleteBitmap(barra_lado);

	int i;
	for (i = 0; i < 5; i++)
		deleteBitmap(portal[i]);

	for (i = 0; i < ANIMATION_ARRAY_SIZE; i++) {
		deleteBitmap(bomb_explosion[i]);
		deleteBitmap(cam_hor_explosion[i]);
		deleteBitmap(cam_ver_explosion[i]);
		deleteBitmap(des_hor_explosion[i]);
		deleteBitmap(des_ver_explosion[i]);
	}

	deleteBitmap(bomb_power_up);
	deleteBitmap(range_power_up);
	deleteBitmap(velocity_power_up);
	deleteBitmap(kick_power_up);
}
