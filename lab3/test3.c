#include "test3.h"
#include "i8042.h"
#include "i8254.h"
#include "keyboard.h"

static unsigned int counter = 0;
int kbd_test_scan(unsigned short ass) {
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


	while(1) {
		/* Get a request message. */
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					{
						if(ass==0)
						scan =	keyboard_int_handler();
						else
						scan = kbd_int_handler_ass();
					}

					break;
			default:
				break; /* no other notifications expected: do nothing */
				}
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
		if (scan == STOP)
			break;
	}
	printf("End.\n");
	return keyboard_unsubscribe_int();
}



int kbd_test_leds(unsigned short n, unsigned short *leds) {
	int ipc_status;
	message msg;
	int irq_set;
	int r;

	timer_set_square(0,STANDARD_FREQ);

	int ret = keyboard_subscribe_int_2();
	if (ret < 0)
	{
		printf("Error subscribing.\n");
		return 1;
	}


	ret = timer_subscribe_int();
	if (ret < 0)
	{
		printf("Error subscribing.\n");
		return 1;
	}


	irq_set = BIT(ret);

	unsigned counter = 0;
	unsigned i = 0;
	int result;
	while(counter < n*STANDARD_FREQ) {
		/* Get a request message. */
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					counter++;
					if (counter % STANDARD_FREQ == 0)
					{
						result = toggle_led(leds[i]);
						i++;
						if (result != 0)
							printf("Erro toggle.\n");
					}
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



int kbd_test_timed_scan(unsigned short n) {
	int ipc_status;
	message msg;
	int irq_set_kbd;
	int irq_set_timer;
	int r;

	unsigned long scan = 0;

	int ret = keyboard_subscribe_int();
	if (ret < 0)
	{
		printf("Error subscribing.\n");
		return 1;
	}

	irq_set_kbd=BIT(ret);

	ret = timer_subscribe_int();
	if (ret < 0)
	{
		printf("Error subscribing.\n");
		return 1;
	}

	irq_set_timer = BIT(ret);

	while(counter < n*STANDARD_FREQ) {
		/* Get a request message. */
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_kbd) { /* subscribed interrupt */
					counter = 0;
					scan = keyboard_int_handler();
				}
				if (msg.NOTIFY_ARG & irq_set_timer) { /* subscribed interrupt */
					counter++;
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		}
		else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
		if (scan == STOP)
			break;
	}
	if (counter >= n*STANDARD_FREQ)
	{
		printf("Timeout.\n");
	}
	printf("End.\n");
	return keyboard_unsubscribe_int();
}

