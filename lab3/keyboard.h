#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
#include "i8254.h"
#include "timer.h"




int keyboard_subscribe_int();
int keyboard_unsubscribe_int();
int keyboard_int_handler();
int kbd_write(unsigned long cmd);
int kbd_read_response(unsigned long *data);
int keyboard_subscribe_int_2(void);
int toggle_led( unsigned short led);

