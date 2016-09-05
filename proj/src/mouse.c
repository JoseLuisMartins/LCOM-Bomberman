/** @file */
#include "mouse.h"
#include "sprite.h"
#include "video_gr.h"
#include "utilities.h"

#define INITIAL_X 635
#define INITIAL_Y 290


typedef struct{
	unsigned char packet[3];
	int y_ovf;
	int x_ovf;
	int y_sign;
	int x_sign;
	int middle_button;
	int right_button;
	int left_button;
	short x_value;
	short y_value;
} mouse_packet;



struct mouse {
	int x;
	int y;
	int done, draw;
	int current_click;
	int previous_click;
	Bitmap *icon;
};


typedef enum {INIT, DRAW, COMP} state_t;
typedef enum {RDOW, RUP, MOVE} event_type_t;

typedef struct{
	event_type_t type;
}event_t;




static int hook_id;
mouse_packet package;

static int vert_length = 0;
static int hor_length = 0;





static int i8042_write(unsigned long port,unsigned long cmd)
{
	unsigned long status;

	unsigned i = 0;
	while( i < NUMBER_TRIES ) {
		if (sys_inb(STATUS_REGISTER, &status) != OK)
			return -1;

		if( (status & IBF) == 0 ) {
			if(sys_outb(port, cmd) != OK)
				return -1;
			else
				return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i++;
	}

	return -1;
}

int i8042_read(unsigned long *data)
{
	unsigned long status;

	unsigned i = 0;
	while( i < NUMBER_TRIES ) {

		if (sys_inb(STATUS_REGISTER, &status) != OK)
			return -1;

		if( status & OBF ) {
			if (sys_inb(OUT_BUF, data) != OK)
				return -1;

			if ( (status &(PAR_ERR | TIMEOUT)) == 0 )
				return 0;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i++;
	}
}


int mouse_subscribe_int(int *mouse_hook){
	if (sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, mouse_hook) == OK)
		return MOUSE_HOOK_NOTIFICATION;
	else
		return -1;
}


int mouse_unsubscribe_int(int *mouse_hook){
	if (sys_irqrmpolicy(mouse_hook) != OK)
	{
		printf("Error unsubscribing mouse.\n");
		return 1;
	}

	return 0;
}

int mouse_write(unsigned long cmd){
	int i=0;
	unsigned long data;

	while(i<NUMBER_TRIES){
		if(i8042_write(KBC_CMD_REG,WRITE_BYTE_TO_MOUSE)==-1)
			return 1;
		if(i8042_read(&data)==-1)
			return 1;
		if(i8042_write(KBD_CMD_REG,cmd)==-1)
			return 1;
		if(i8042_read(&data)==-1)
			return 1;
		if(data==ACK)
			return 0;
		i++;
	}
	return 1;
}

static int mouse_packet_handler(){
	static int ind=0;
	unsigned long byte;
	if(sys_inb(OUT_BUF, &byte) == -1)
		return -1;

	if(ind==0 && synchronize(byte)==1){
		return 1;
	}

	 package.packet[ind]= (unsigned char) byte;

	 ind = (ind + 1) % 3;
	 if (ind == 0){
		 setPackageValues();
		 return 0;
	 }
	 else
		 return 1;
}



static int synchronize(unsigned long byte){
	if((byte & BIT(3)) && (byte != ACK))
		return 0;
	else
		return 1;
}



static void setPackageValues()
{
	if (package.packet[0] & BIT(1))
		package.left_button = 1;
	else
		package.left_button = 0;

	if (package.packet[0] & BIT(0))
		package.right_button = 1;
	else
		package.right_button = 0;

	if (package.packet[0] & BIT(2))
		package.middle_button = 1;
	else
		package.middle_button = 0;

	if (package.packet[0] & BIT(4))
		package.x_sign = 1;
	else
		package.x_sign = 0;

	if (package.packet[0] & BIT(5))
		package.y_sign = 1;
	else
		package.y_sign = 0;

	if (package.packet[0] & BIT(6))
		package.x_ovf = 1;
	else
		package.x_ovf = 0;

	if (package.packet[0] & BIT(7))
		package.y_ovf = 1;
	else
		package.y_ovf = 0;


	if (package.x_sign) {
		package.x_value = (short) package.packet[1];
		package.x_value |= 0xFF00;
	}
	else
		package.x_value = (short) package.packet[1];

	if (package.y_sign)
		package.y_value = 0xFF00 | (short) package.packet[2];
	else
		package.y_value = (short) package.packet[2];
}







//mouse class ---------------------

mouse_t* create_mouse()
{
	mouse_t *m = malloc(sizeof(mouse_t));
	m->x = INITIAL_X;
	m->y = INITIAL_Y;
	m->done = 0;
	m->draw = 1;
	m->icon = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/mouse_icon.bmp");
	return m;
}


void update_mouse(mouse_t* m)
{
	if (mouse_packet_handler() == 0) {
		int new_x = m->x + package.x_value;
		int new_y = m->y - package.y_value;

		if((new_x + m->icon->bitmapInfoHeader.width > 1024 || new_x < 0 )){
			if(!(new_y +  m->icon->bitmapInfoHeader.height > 768 || new_y < 0))
				m->y = new_y;
		}else if(( new_y + m->icon->bitmapInfoHeader.height > 768 || new_y < 0 )){
			if(!(new_x + m->icon->bitmapInfoHeader.width > 1024 || new_x < 0))
				m->x = new_x;
		} else {
			m->x = new_x;
			m->y = new_y;
		}


		if (m->current_click)
			m->previous_click = 1;
		else
			m->previous_click = 0;

		if (package.right_button)
			m->current_click = 1;
		else
			m->current_click = 0;

		if (m->previous_click == 1 && m->current_click == 0)
			m->done = 1;
		else
			m->done = 0;
	}
}

void draw_mouse(mouse_t *m,int buffer){
	drawBitmap(m->icon, m->x, m->y, ALIGN_LEFT, buffer);
}

int is_mouse_inside(mouse_t *m,int xi,int yi,int xf,int yf){
	if(m->x > xi  && m->x < xf && m->y > yi  && m->y < yf )
		return 1;
	else
		return 0;
}

int getDone(mouse_t* m){
	return m->done;
}


int getDraw(mouse_t* m){
	return m->draw;
}

void destroy_mouse(mouse_t *m){
	deleteBitmap(m->icon);
	free(m);
}



void setDefaultPosition(mouse_t *m)
{
	m->x = INITIAL_X;
	m->y = INITIAL_Y;
}



void setDone(mouse_t* m) {
	m->done = 1;
}


void resetDone(mouse_t* m) {
	m->previous_click = 0;
	m->done = 0;
}


int mouse_getX(mouse_t* m) {
	return m->x;
}


int mouse_getY(mouse_t* m) {
	return m->y;
}



void mouse_setPosition(mouse_t* m, int x, int y) {
	m->x = x;
	m->y = y;
}
