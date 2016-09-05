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




int keyboard_int_handler(){
	unsigned long scancode;

	if(kbd_read_response(&scancode))
		return 1;


	if(scancode == MSB_EXTRA_BYTE){
		two_bytes = true;
		return 0;
	}

	if(two_bytes == true){
		scancode |= SHIFT_LEFT(MSB_EXTRA_BYTE,8);
		two_bytes = false;
	}

	if(BREAK_CODE(scancode))
	{
		printf("Break Code: 0x%X.\n\n",scancode);
		if (scancode == ESC_BREAK_CODE)
		{
			return STOP;
		}
	}
	else
		printf("Make Code: 0x%X.\n",scancode);

	return 0;
}


int kbd_write(unsigned long cmd)
{
	unsigned long status;

	unsigned i = 0;
	while( i < NUMBER_TRIES ) {
		if (sys_inb(STATUS_REGISTER, &status) != OK)
			return -1;

		if( (status & IBF) == 0 ) {
			if(sys_outb(KBD_CMD_REG, cmd) != OK)
				return -1;
			else
				return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i++;
	}

	return -1;
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



int keyboard_subscribe_int_2(void) {
	hook_id = KBD_HOOK_NOTIFICATION;

	if (sys_irqsetpolicy(IRQ_1, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK)
		return 1;

	if(sys_irqdisable(&hook_id)!= OK)
		return 1;

	return 0;
}

int toggle_led( unsigned short led){
	unsigned long data = 0;
	unsigned tries = 0;
	led_status ^= BIT(led);
	do{
		kbd_write(KBD_SWITCH_LED_CMD);
		do {
			kbd_read_response(&data);
			tries++;
		}
		while (data == RESEND && tries < NUMBER_TRIES);

	}while(data==ERROR && tries < NUMBER_TRIES);


	if (tries >= NUMBER_TRIES)
		return 1;

	tries = 0;
	do{
		kbd_write(led_status);
		do {
			kbd_read_response(&data);
			tries++;
		}
		while (data == RESEND && tries < NUMBER_TRIES);

	}while(data==ERROR && tries < NUMBER_TRIES);


	if (tries >= NUMBER_TRIES)
		return 1;

	return 0;
}
