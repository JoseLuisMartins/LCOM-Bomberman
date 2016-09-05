#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>
#include "i8254.h"
#include <minix/com.h>

static int hook_id;
static unsigned int counter=0;

unsigned long BINARY_TO_BCD(unsigned long num){
	unsigned long result=0;
	unsigned long tmp=num;
	int i=0;
	while(num!=0){
		tmp %=10;
		result |= tmp<<(4*i);
		i++;
		num/=10;
		tmp=num;
	}

	return result;
}


int timer_set_square(unsigned long timer, unsigned long freq) {
	if (freq < 1 || freq > TIMER_FREQ)
	{
		printf("Invalid frequency (must be higher or equal to 1 and lower than 1193182).\n");
		return 1;
	}

	unsigned char status;
	timer_get_conf(timer,&status);

	unsigned char mask = (BIT(3) | BIT(2) | BIT(1) | BIT(0));
	status &= mask;
	status |= TIMER_LSB_MSB;

	if (timer == 0)
		status |= TIMER_SEL0;
	else if (timer == 1)
		status |= TIMER_SEL1;
	else if (timer == 2)
		status |= TIMER_SEL2;
	else
	{
		printf("Invalid timer. Must be [0,1,2].\n");
		return 1;
	}


	if (sys_outb(TIMER_CTRL, status))
	{
		printf("Writing error (sys_outb()).\n");
		return 1;
	}

	unsigned long divisor = TIMER_FREQ/freq;
	if (status & TIMER_BIN != TIMER_BIN)
		divisor = BINARY_TO_BCD(divisor);

	unsigned char LSB = (char) divisor;
	unsigned char MSB = (char) (divisor >> 8);

	if (status & TIMER_BIN != TIMER_BIN)
	{
		LSB = BINARY_TO_BCD(LSB);
		MSB = BINARY_TO_BCD(MSB);
	}


	if(sys_outb(TIMER_0 + timer, LSB) || sys_outb(TIMER_0 + timer, MSB))
	{
		printf("Writing error (sys_outb()).\n");
		return 1;
	}

	return 0;
}

int timer_subscribe_int(void ) {
	hook_id = 0;
	return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE,&hook_id);
}

int timer_unsubscribe_int() {
	if ((sys_irqdisable(&hook_id) != OK) || (sys_irqrmpolicy(&hook_id) != OK))
	{
		printf("Error unsubscribing (timer_unsubscribe_int()).\n");
		return 1;
	}

	return OK;
}

void timer_int_handler() {
	counter++;
}

int timer_get_conf(unsigned long timer, unsigned char *st) {//podemos mudar para unsigned long?? (sys_inb)

	unsigned long RB = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));
	if(sys_outb(TIMER_CTRL, RB)){
		printf("Writing error (sys_outb()).\n");
		return 1;
	}



	unsigned long status;
	int err;

	if(timer == 0)
		err=sys_inb(TIMER_0,&status);
	else if(timer == 1)
		err=sys_inb(TIMER_1,&status);
	else
		err=sys_inb(TIMER_2,&status);

	*st= (char) status;

	if(err)
		printf("Reading error (sys_inb()).\n");
	return err;
}

int timer_display_conf(unsigned char conf) {
	printf("\n\n============================");
	printf("\n|Configuration of the Timer|\n");
	printf("============================\n");
	printf(" Status Byte(HEX): 0x%x \n",conf);
	printf(" Output: %d",((conf & BIT(7)) >> 7));
	printf("\n Null Count: %d",(conf & BIT(6)));
	printf("\n Type of access: ");

	if((conf & TIMER_LSB_MSB) == TIMER_LSB_MSB)
		printf("LSB followed by MSB.\n");
	else if((conf & TIMER_LSB)==TIMER_LSB)
		printf("LSB.\n");
	else
		printf("MSB.\n");


	printf(" Programmed Mode: ");

	 if((conf & MODE_5)==MODE_5)
		 printf("5 - Hardware Triggered Strobe.\n");
	 else if((conf & TIMER_SQR_WAVE)==TIMER_SQR_WAVE)
	 		printf("3 - Square Wave Mode.\n");
	else if((conf & TIMER_RATE_GEN))
		printf("2 -  Rate Generator.\n");
	else if(conf & MODE_4)
		printf("4 - Software Triggered Strobe.\n");
	else if (conf & MODE_1)
		printf("1 - Hardware Retriggerable One-Shot\n");
	else
		printf("0 - Interrupt On Terminal Count.\n");

	printf(" BCD: ");
	if(conf &  TIMER_BCD)
		printf("yes.\n");
	else
		printf("no.\n");


	return 0;
}

int timer_test_square(unsigned long freq) {
	return timer_set_square(0,freq);
}

int timer_test_int(unsigned long time) {
	int ipc_status;
	message msg;
	int irq_set;
	int r;

	timer_set_square(0,STANDARD_FREQ);

	int ret = timer_subscribe_int();
	if (ret < 0)
	{
		printf("Error subscribing.\n");
		return 1;
	}
	irq_set = BIT(ret);


	while(counter < time*STANDARD_FREQ) {
		/* Get a request message. */
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					timer_int_handler();
					if (counter % STANDARD_FREQ == 0)
						printf("Second: %d\n",counter/STANDARD_FREQ);
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	return timer_unsubscribe_int();
}

int timer_test_config(unsigned long timer) {
	unsigned char st;

	if(timer_get_conf(timer,&st))
		return 1;

	timer_display_conf(st);
	return 0;
}
