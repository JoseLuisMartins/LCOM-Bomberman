#ifndef _I8042_H
#define _I8042_H

#include "i8254.h"


// SCAN

#define DELAY_US 20000

#define SHIFT_LEFT(arg,n)      (arg << n)
#define BREAK_CODE(scancode)   (scancode & BIT(7))
#define BREAK_CODE_BIT			BIT(7)


#define STATUS_REGISTER		    0x64
#define OUT_BUF 				0x60
#define IRQ_1      				   1
#define MSB_EXTRA_BYTE          0xE0
#define ESC_BREAK_CODE          0x81
#define STOP					  -1

#define KBD_HOOK_NOTIFICATION	   1


#define false					   0
#define true					   1




// LEDS


#define KBD_SWITCH_LED_CMD		0xED
#define	ACK						0xFA
#define RESEND					0xFE
#define ERROR					0xFC
#define KBD_CMD_REG				0x60
#define KBC_CMD_REG				0x64
#define IBF					    BIT(1)
#define OBF						BIT(0)
#define PAR_ERR					BIT(7)
#define TIMEOUT 				BIT(6)

#define NUMBER_TRIES			5

//Mouse

#define IRQ_MOUSE 				12
#define WRITE_BYTE_TO_MOUSE		0xD4
#define NACK 					0xFE
#define ERROR 					0xFC
#define MOUSE_HOOK_NOTIFICATION 12
#define SET_STREAM_MODE         0xEA
#define ENABLE_DATA_PACKETS     0xF4
#define DISABLE_STREAM_MODE		0xF5
#define STATUS_REQUEST          0xE9


#endif
