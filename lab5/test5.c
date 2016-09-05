#include "test5.h"
#include "i8042.h"
#include "i8254.h"
#include "keyboard.h"
#include "timer.h"
#include "vbe.h"
#include "video_gr.h"
#include "lmlib.h"
#include "sprite.h"

void *test_init(unsigned short mode, unsigned short delay) {

	if(vg_exit())
		return NULL;

	char* video_mem = vg_init(mode);
	if (video_mem == NULL)
		return NULL;

	vbe_mode_info_t vmi_p;
	if(vbe_get_mode_info(mode, &vmi_p)){
		vg_exit();
		return NULL;
	}

	int ipc_status;
	message msg;
	int irq_set;
	int r;
	int counter = 0;

	int ret = timer_subscribe_int();
	if (ret < 0)
		return NULL;

	irq_set = BIT(ret);
	while(counter < delay*STANDARD_FREQ) {
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			if (_ENDPOINT_P(msg.m_source)==HARDWARE) {
				if (msg.NOTIFY_ARG & irq_set)  /* subscribed interrupt */
					counter++;
			}
		}
	}


	if(vg_exit())
		return NULL;

	if(timer_unsubscribe_int())
		return NULL;


	printf("\nPhysical address: 0x%X \n", vmi_p.PhysBasePtr);
	return video_mem;
}



int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {

	if(vg_exit())
		return 1;

	char* video_mem = vg_init(MODE3);
	if (draw_square(x,y,size,color) != 0)
	{
		vg_exit();
		printf("Out of Range\n");
		return 1;
	}

	//Esc
	int ipc_status;
	message msg;
	int irq_set;
	int r;

	unsigned long scan;


	int ret = keyboard_subscribe_int();
	if (ret < 0)
	{
		printf("Error subscribing.\n");
		return 1;
	}

	irq_set = BIT(ret);
	unsigned long scancode;

	while(1) {
		/* Get a request message. */
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			if (_ENDPOINT_P(msg.m_source)==HARDWARE) {
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					{
						if(kbd_read_response(&scancode))
							return 1;
						if(scancode == ESC_BREAK_CODE)
							break;

					}
				}
			}
		}
	}


	if(vg_exit())
		return 1;

	return keyboard_unsubscribe_int();
}


int test_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color) {

	if(vg_exit())
		return 1;

	char* video_mem = vg_init(MODE3);
	if (draw_line(xi, yi, xf, yf, color))
	{
		vg_exit();
		printf("Out of Range\n");
		return 1;
	}

	//Esc
	int ipc_status;
	message msg;
	int irq_set;
	int r;

	unsigned long scan;


	int ret = keyboard_subscribe_int();
	if (ret < 0)
	{
		printf("Error subscribing.\n");
		return 1;
	}

	irq_set = BIT(ret);
	unsigned long scancode;

	while(1) {
		/* Get a request message. */
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			if (_ENDPOINT_P(msg.m_source)==HARDWARE) {
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					{
						if(kbd_read_response(&scancode))
							return 1;
						if(scancode == ESC_BREAK_CODE)
							break;

					}
				}
			}
		}
	}


	if(vg_exit())
		return 1;

	return keyboard_unsubscribe_int();
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	

	Sprite *s=create_sprite(xpm,xi,yi,0,0);

	if(vg_exit())
			return 1;

		char* video_mem = vg_init(MODE3);
		if (draw_pixmap(s))
		{
			vg_exit();
			printf("Out of Range\n");
			return 1;
		}


	//Esc
	int ipc_status;
	message msg;
	int irq_set;
	int r;

	unsigned long scan;


	int ret = keyboard_subscribe_int();
	if (ret < 0)
	{
		printf("Error subscribing.\n");
		return 1;
	}

	irq_set = BIT(ret);
	unsigned long scancode;

	while(1) {
		/* Get a request message. */
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			if (_ENDPOINT_P(msg.m_source)==HARDWARE) {
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					{
						if(kbd_read_response(&scancode))
							return 1;
						if(scancode == ESC_BREAK_CODE)
							break;

					}
				}
			}
		}
	}


	if(vg_exit())
		return 1;

	return keyboard_unsubscribe_int();
	
}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], unsigned short hor, short delta, unsigned short time) {

	if(vg_exit())
		return 1;

	char* video_mem = vg_init(MODE3);

	Limits lmts;
	double vx, vy;
	if (hor)
	{
		vx = ((double) delta) / time;
		vy = 0;
		lmts.xmax = xi + delta -1;
		lmts.ymax = 0;
	}
	else
	{
		vx = 0;
		vy = ((double) delta) / time;
		lmts.xmax = 0;
		lmts.ymax = yi + delta -1;
	}

	Sprite *s = create_sprite(xpm, xi, yi, vx, vy);
	if (animate_sprite(s, lmts))
	{
		vg_exit();
		return 1;
	}

	vg_exit();
	return 0;
}					

int test_controller() {

	vbe_info_block_t controller;
	uint16_t *farptr;
	if ((farptr = vbe_get_info_block(&controller)) == NULL)
		return 1;

	printf("\nThe controller capabilities:\n");
	printf("DAC %s\n", (controller.Capabilities[0] & BIT(DAC_BIT)) ? "width is switchable to 8 bits per primary color" :
		"is fixed width, with 6 bits per primary color");
	printf("Controller is %sVGA compatible\n", (controller.Capabilities[0] & BIT(VGA_BIT)) ? "not " : "");
	printf("%s\n", (controller.Capabilities[0] & BIT(RAMDAC_BIT)) ? "Normal RAMDAC operation" :
			"When programming large blocks of information to the RAMDAC, use the blank bit in Function 09h");


	printf("\nList of mode numbers (in hexadecimal) supported:\n");
	int i = 1;
	while (*farptr != VIDEOMODEPTR_END)
	{
		printf("%d - 0x%04X\t", i, (unsigned short)*farptr);
		farptr += 1;
		if (!(i % 5))
			printf("\n");
		i++;
	}
	printf("\n");

	printf("\nSize of VRAM memory:\n");
	printf("%d blocks of 64kb\n", controller.TotalMemory);
	return 0;
}
