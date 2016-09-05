/** @file */

#include "keyboard.h"

static int hook_id;
int two_bytes = false;
unsigned short led_status = 0;


int keyboard_subscribe_int() {
	hook_id = KBD_HOOK_NOTIFICATION;
	if (sys_irqsetpolicy(IRQ_1, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) == OK)
		return KBD_HOOK_NOTIFICATION;
	else
		return -1;

	if(sys_irqenable(&hook_id)!= OK)
		return -1;
}




int keyboard_unsubscribe_int(){
	if(sys_irqdisable(&hook_id)!= OK)
		return 1;
	if (sys_irqrmpolicy(&hook_id) != OK)
	{
		printf("Error unsubscribing (keyboard_unsubscribe_int()).\n");
		return 1;
	}

	return OK;
}




int keyboard_int_handler(unsigned long *scancode){
	if(kbd_read_response(scancode))
		return -1;


	if(*scancode == MSB_EXTRA_BYTE){
		two_bytes = true;
		return 1;
	}

	if(two_bytes == true){
		*scancode |= SHIFT_LEFT(MSB_EXTRA_BYTE,8);
		two_bytes = false;
	}
	return 0;
}




int kbd_read_response(unsigned long *data)
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
