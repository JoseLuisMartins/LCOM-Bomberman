/** @file */
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <math.h>
#include "vbe.h"
#include "keyboard.h"
#include "sprite.h"
#include "utilities.h"


static char *video_mem;		/* Process address to which VRAM is mapped */
static char *second_buffer;
static char *third_buffer;


static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */


char *getSecond_Buffer(){
	return second_buffer;
}

char *getThird_Buffer() {
	return third_buffer;
}

unsigned getHorResolution(){
	return h_res;
}

unsigned getVerResolution(){
	return v_res;
}

void*vg_init(unsigned short mode) {
	struct reg86u r;
	vbe_mode_info_t info;
	unsigned size;
	struct mem_range mr;

	r.u.b.ah = VBE_FUNCTION_PREFIX;
	r.u.b.intno = VBE_INTERRUPT_VECTOR;
	r.u.b.al = VBE_SET_MODE;
	r.u.w.bx = LINEAR_FRAME_BUFFER | mode;

	if (sys_int86(&r) == OK) {

		if (vbe_get_mode_info(mode, &info) != 1) {

			h_res = info.XResolution;
			v_res = info.YResolution;
			bits_per_pixel = info.BitsPerPixel;

			mr.mr_base = info.PhysBasePtr;
			size = h_res * v_res * bits_per_pixel/8;
			mr.mr_limit = mr.mr_base + size;

			if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))
			{
				return NULL;
			}

			video_mem = vm_map_phys(SELF, (void *) mr.mr_base, size);

			if(video_mem != MAP_FAILED)
			{
				if ((second_buffer = malloc(h_res * v_res * bits_per_pixel / 8)) != NULL)
				{
					if ((third_buffer = malloc(h_res * v_res * bits_per_pixel / 8)) != NULL)
					{
						return video_mem;
					}
				}
			}
		}
	}


	return NULL;
}



int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = VBE_INTERRUPT_VECTOR; 		/* BIOS video services */

	reg86.u.b.ah = VBE_VIDEO_MODE_FUNCTION; 		/* Set Video Mode function */
	reg86.u.b.al = SET_TEXT_MODE; 					/* 80x25 text mode*/

	if (sys_int86(&reg86) != OK) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}



int setPixel(unsigned short x, unsigned short y,unsigned long color,int buffer)
{
	char *tmp;
	if(buffer == THIRD_BUFFER)
		tmp = third_buffer;
	else if(buffer == SECOND_BUFFER)
		tmp = second_buffer;
	else
		tmp = video_mem;

	if(x < 0 || x >= h_res || y < 0 || y >= v_res){
		return 1;
	}

	if (color != TRANSPARENCY)
	{
		tmp += (x + y*h_res)*bits_per_pixel/BYTE_SIZE;
		*tmp=(0xFF  & (color));
		tmp++;
		*tmp=(0xFF & (color >> 8));
	}

	return 0;
}

void swapThirdBuffer(){
	memcpy(third_buffer, second_buffer, h_res * v_res * bits_per_pixel / 8);
}

void swapVideoMem(){
	memcpy(video_mem, third_buffer, h_res * v_res * bits_per_pixel / 8);
}


void draw_rectangle(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color)
{
	unsigned short h_pos;
	unsigned short v_pos;

	for (h_pos = xi; h_pos <= xf; h_pos++) {
		setPixel(h_pos, yi, color, THIRD_BUFFER);
		setPixel(h_pos, yi + 1, color, THIRD_BUFFER);
		setPixel(h_pos, yi + 2, color, THIRD_BUFFER);
		setPixel(h_pos, yf, color, THIRD_BUFFER);
		setPixel(h_pos, yf - 1, color, THIRD_BUFFER);
		setPixel(h_pos, yf - 2, color, THIRD_BUFFER);
	}

	for (v_pos = yi; v_pos < yf; v_pos++) {
		setPixel(xi, v_pos, color, THIRD_BUFFER);
		setPixel(xi + 1, v_pos, color, THIRD_BUFFER);
		setPixel(xi + 2, v_pos, color, THIRD_BUFFER);
		setPixel(xf, v_pos, color, THIRD_BUFFER);
		setPixel(xf - 1, v_pos, color, THIRD_BUFFER);
		setPixel(xf - 2, v_pos, color, THIRD_BUFFER);
	}
}



int draw_line(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long color)
{

	  float dy = y2 - y1;
	  float dx = x2 - x1;

	  float size = sqrt(dx*dx+dy*dy);

	  int d;
	  for (d = 0; d < size; d++) {
		  unsigned short y= y1+(dy*d)/size;
		  unsigned short x= x1+(dx*d)/size;

		  if(setPixel(x,y,color,SECOND_BUFFER)){
			  return 1;
		  }
	}
	return 0;
}




