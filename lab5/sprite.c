#include "sprite.h"
#include <stdlib.h>


#define COLOR_BLACK			0

Sprite *create_sprite(char *pic[],int xi,int yi,int vx,int vy) {
	//allocate space for the "object"
	Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
	if( sp == NULL )
		return NULL;
	// read the sprite pixmap
	sp->map = (char*) read_xpm(pic, &(sp->width), &(sp->height));
	if( sp->map == NULL ) {
		free(sp);
		return NULL;
	}

	sp->x=xi;
	sp->y=yi;
	sp->xspeed=vx;
	sp->yspeed=vy;

	return sp;
}

void destroy_sprite(Sprite *sp) {
	if( sp == NULL )
		return;
	free(sp->map);
	free(sp);
	sp = NULL;
}


int remove_sprite(Sprite* s)
{
	int i,j;
	for (i = 0; i < s->height; i++) {
		for (j = 0; j < s->width; ++j) {
			if(setPixel((unsigned short) (j + s->x), (unsigned short) (i + s->y), COLOR_BLACK))
				return 1;
		}
	}
	return 0;
}


int animate_sprite(Sprite *sp, Limits lim) {
	int ipc_status;
	message msg;
	int irq_set_timer;
	int irq_set_kbd;
	int r;


	int ret = timer_subscribe_int();
	if (ret < 0)
		return 1;

	irq_set_timer = BIT(ret);

	ret = keyboard_subscribe_int();
	if (ret < 0)
		return 1;

	irq_set_kbd = BIT(ret);

	unsigned long scancode;
	while(1) {
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			if (_ENDPOINT_P(msg.m_source)==HARDWARE) {
				if (msg.NOTIFY_ARG & irq_set_timer) {
					if ((sp->x <= lim.xmax) || (sp->y <= lim.ymax)){
						if (remove_sprite(sp))
							return 1;

						sp->x = sp->x + (sp->xspeed / STANDARD_FREQ);
						sp->y = sp->y + (sp->yspeed / STANDARD_FREQ);

						if (draw_pixmap(sp))
							return 1;
					}
				}
				if(msg.NOTIFY_ARG & irq_set_kbd){
					if(kbd_read_response(&scancode))
						return 1;

					if(scancode == ESC_BREAK_CODE)
						break;
				}
			}
		}
	}


	if(timer_unsubscribe_int() )
	{
		vg_exit();
		printf("Ola6\n");
		return 1;
	}
	if( keyboard_unsubscribe_int()){
		vg_exit();
		printf("Ola7\n");
		return 1;
	}
	return 0;
}

static int check_collision(Sprite *sp, char *base){

}

static int draw_sprite(Sprite *sp, char *base){

}
