/** @file */
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>
#include "i8254.h"
#include <minix/com.h>

static int hook_id;
static int counter=0;


int timer_subscribe_int(void ) {
	hook_id = TIMER0_IRQ;
	return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE,&hook_id);
}

int timer_unsubscribe_int() {

	if (sys_irqrmpolicy(&hook_id) != OK)
	{
		printf("Error unsubscribing (timer_unsubscribe_int()).\n");
		return 1;
	}

	return OK;
}

void timer_int_handler() {
	counter++;
}
