#include "test4.h"
#include "mouse.h"
static unsigned int counter = 0;



int test_packet(unsigned short cnt){
	int ipc_status;
	message msg;
	int irq_set;
	int r;



	int ret = mouse_subscribe_int();
	if (ret < 0)
	{
		printf("Error subscribing Mouse.\n");
		return 1;
	}

	irq_set = BIT(ret);

	if(mouse_write(SET_STREAM_MODE))
		return 1;
	if(mouse_write(ENABLE_DATA_PACKETS))
		return 1;

	while(cnt > 0) {
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
						if(mouse_packet_handler()==0){
							cnt--;
						}
					}

					break;
			default:
				break; /* no other notifications expected: do nothing */
				}
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if(mouse_write(DISABLE_STREAM_MODE))
		return 1;


	printf("End.\n");
	return mouse_unsubscribe_int();
}

int test_async(unsigned short idle_time) {
	int ipc_status;
	message msg;
	int irq_set_mouse;
	int irq_set_timer;
	int r;

	unsigned long scan = 0;

	int ret = mouse_subscribe_int();
	if (ret < 0)
	{
		printf("Error subscribing.\n");
		return 1;
	}

	irq_set_mouse=BIT(ret);

	ret = timer_subscribe_int();
	if (ret < 0)
	{
		printf("Error subscribing.\n");
		return 1;
	}

	irq_set_timer = BIT(ret);

	if(mouse_write(SET_STREAM_MODE))
		return 1;
	if(mouse_write(ENABLE_DATA_PACKETS))
		return 1;

	while(counter < idle_time*STANDARD_FREQ) {
		/* Get a request message. */
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_mouse) { /* subscribed interrupt */
					if(mouse_packet_handler()==0){
						counter=0;
					}
				}
				if (msg.NOTIFY_ARG & irq_set_timer) { /* subscribed interrupt */
					counter++;
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		}
	}

	printf("End.\n");

	if(mouse_write(DISABLE_STREAM_MODE))
		return 1;

	if (mouse_unsubscribe_int() == 1)
		return 1;

	return timer_unsubscribe_int();
}



int test_config(void) {

	if(mouse_subscribe_int() < 0)
	{
		printf("Error subscribing.\n");
		return 1;
	}

	mouse_read_config();
	if (mouse_unsubscribe_int() == 1)
			return 1;

	return 0;

}


int test_gesture(short length, unsigned short tolerance) {
	int ipc_status;
	message msg;
	int irq_set_mouse;
	int irq_set_timer;
	int r;

	unsigned long scan = 0;

	int ret = mouse_subscribe_int();
	if (ret < 0)
	{
		printf("Error subscribing.\n");
		return 1;
	}

	irq_set_mouse=BIT(ret);

	if(mouse_write(SET_STREAM_MODE))
		return 1;
	if(mouse_write(ENABLE_DATA_PACKETS))
		return 1;

	while(1) {
		/* Get a request message. */
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) !=0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
				if (msg.NOTIFY_ARG & irq_set_mouse) { /* subscribed interrupt */
					if(mouse_gesture_handler(length, tolerance) == STOP){
						break;
					}
				}
			}
		}
	}

	printf("End.\n");

	if(mouse_write(DISABLE_STREAM_MODE))
		return 1;

	if (mouse_unsubscribe_int() == 1)
		return 1;

	return 0;
}
