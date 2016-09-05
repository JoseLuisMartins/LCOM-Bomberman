#include "mouse.h"

static int hook_id;
mouse_packet package;
mouse_config config;

static int vert_length = 0;
static int hor_length = 0;


int sameSignal(int x, int y)
{
	if (x*y >= 0)
		return 1;
	else
		return 0;
}


int i8042_write(unsigned long port,unsigned long cmd)
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

int mouse_subscribe_int(){
	hook_id = MOUSE_HOOK_NOTIFICATION;

	if (sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) == OK)
		return MOUSE_HOOK_NOTIFICATION;
	else
		return -1;
}
int mouse_unsubscribe_int(){
	if (sys_irqrmpolicy(&hook_id) != OK)
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

int mouse_packet_handler(){
	static int ind=0;
	unsigned long byte;
	if(sys_inb(OUT_BUF, &byte) != OK)
		return -1;

	if(ind==0 && synchronize(byte)==1){
		return 1;
	}

	 package.packet[ind % 3]= (unsigned char) byte;

	 ind = ind + 1;
	 if ((ind % 3) == 0){
		 setPackageValues();
		 mouse_print_package();
		 return 0;
	 }
	 else
		 return 1;
}

int mouse_print_package(){
	printf("B1=0x%02X  B2=0x%02X  B3=0x%02X   LB=%d   MB=%d   RB=%d   XOV=%d   YOV=%d   X=%d   Y=%d\n",
			package.packet[0], package.packet[1], package.packet[2], package.left_button, package.middle_button, package.right_button,
			package.x_ovf, package.y_ovf, package.x_value, package.y_value);

}

int synchronize(unsigned long byte){
	if((byte & BIT(3)) && (byte != ACK))
		return 0;
	else
		return 1;
}



void setPackageValues()
{
	if (package.packet[0] & BIT(0))
		package.left_button = 1;
	else
		package.left_button = 0;

	if (package.packet[0] & BIT(1))
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




void setConfigValues()
{
	config.right_button = config.packet[0] & BIT(0);
	config.middle_button = config.packet[0] & BIT(1);
	config.left_button = config.packet[0] & BIT(2);
	config.scaling = config.packet[0] & BIT(4);
	config.enable = config.packet[0] & BIT(5);
	config.remote = config.packet[0] & BIT(6);
	config.resolution = config.packet[1] & (BIT(1) | BIT(0));
	config.sample_rate = config.packet[2];
}



void mouse_print_config_package(){
	printf("\nMouse Config\n");
	printf("Remote: %s", config.remote ? "remote (polled) mode\n" : "stream mode\n");
	printf("Enable: %s", config.enable ? "data reporting enabled\n" : "disabled\n");
	printf("Scaling: %s", config.scaling ? "2:1\n" : "1:1\n");
	printf("Right button: %s", config.right_button ? "active\n" : "not active\n");
	printf("Left button: %s", config.left_button ? "active\n" : "not active\n");
	printf("Middle button: %s", config.middle_button ? "active\n" : "not active\n");
	// a documentação referente à configuração do rato troca os BITS relativos ao RB e LB
	// deixamos igual à documentação por razões de avaliação

	printf("Resolution: ");
	switch (config.resolution){
	case 0:
		printf ("1 count per mm\n");
		break;
	case 1:
		printf ("2 count per mm\n");
		break;
	case 2:
		printf ("4 count per mm\n");
		break;
	case 3:
		printf ("8 count per mm\n");
		break;
	}

	printf("Sample rate: %d\n", config.sample_rate);
}


int mouse_read_config(){
	if(mouse_write(STATUS_REQUEST))
		return 1;

	unsigned long data;
	int i;
	for (i=0; i < 3; i++) {
		if(i8042_read(&data))
			return 1;
		else
			config.packet[i]=(char)data;
	}

	setConfigValues();
	mouse_print_config_package();
	return 0;
}







int mouse_gesture_handler(short length, unsigned short tolerance)
{
	static int ind=0;
	unsigned long byte;
	if(sys_inb(OUT_BUF, &byte) != OK)
		return -1;

	if(ind==0 && synchronize(byte)==1){
		return 1;
	}
	package.packet[ind % 3] = (char) byte;

	ind = ind + 1;
	if ((ind % 3) == 0){
		setPackageValues();

		event_t evt;
		if (package.right_button)
		{
			evt.type = RDOW;
			if (check_vert_line(&evt, length, tolerance) == STOP)
				return STOP;
		}
		else
		{
			evt.type = RUP;
			if (check_vert_line(&evt, length, tolerance) == STOP)
				return STOP;
		}

		if ((package.x_value != 0) || (package.y_value != 0))
		{
			printf("vertical length: %d \n",vert_length);
			printf("horizontal length: %d \n\n",hor_length);
			evt.type = MOVE;
			if (check_vert_line(&evt, length, tolerance) == STOP)
					return STOP;
		}

		return 0;
	}
}


int check_vert_line(event_t *evt, int length, int tolerance) {
	static state_t state = INIT; // initial state; keep state
	switch (state) {
	case INIT:
		if( evt->type == RDOW )
			state = DRAW;
		break;
	case DRAW:
		if( evt->type == MOVE ) {
			if (sameSignal(length, package.y_value))
				vert_length += package.y_value;
			else
				vert_length = 0;

			hor_length += package.x_value;

			if (abs(hor_length) >= tolerance)
			{
				vert_length = 0;
				hor_length = 0;
			}

			if (abs(vert_length) >= abs(length))
				state = COMP;
		}
		else if( evt->type == RUP )
		{
			state = INIT;
			vert_length = 0;
			hor_length = 0;
		}
		break;
	default:
		break;
	}

	if (state == COMP)
		return STOP;
	else
		return 0;
}
