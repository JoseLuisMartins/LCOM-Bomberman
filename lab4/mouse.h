#ifndef __MOUSE_H
#define __MOUSE_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"

typedef struct{
        unsigned char packet[3];
        int y_ovf;
        int x_ovf;
        int y_sign;
        int x_sign;
        int middle_button;
        int right_button;
        int left_button;
        short x_value;
        short y_value;
}mouse_packet;


typedef struct{
        unsigned char packet[3];
        int right_button;
        int middle_button;
        int left_button;
        int scaling;
        int enable;
        int remote;
        int resolution;
        int sample_rate;
}mouse_config;


typedef enum {INIT, DRAW, COMP} state_t;
typedef enum {RDOW, RUP, MOVE} event_type_t;

typedef struct{
        event_type_t type;
}event_t;


int sameSignal(int x, int y);

int i8042_write(unsigned long port,unsigned long cmd);
int i8042_read(unsigned long *data);
int mouse_subscribe_int();
int mouse_unsubscribe_int();
int mouse_write(unsigned long cmd);
int mouse_packet_handler();
int print_package();
int synchronize(unsigned long byte);
void setPackageValues();
void setConfigValues();
void mouse_print_config_package();
int mouse_read_config();
int mouse_gesture_handler(short length, unsigned short tolerance);
int check_vert_line(event_t *evt, int length, int tolerance);

#endif
