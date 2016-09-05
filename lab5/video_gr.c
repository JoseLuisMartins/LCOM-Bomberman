#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <math.h>
#include "vbe.h"
#include "sprite.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR			0xE0000000
#define H_RES             		1024
#define V_RES		  			768
#define BITS_PER_PIXEL	  		8
#define BYTE_SIZE				8

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res = 1024;		/* Horizontal screen resolution in pixels */
static unsigned v_res = 768;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel = 8; /* Number of VRAM bits per pixel */


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
			size = h_res * v_res * bits_per_pixel;
			mr.mr_limit = mr.mr_base + size;

			if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))
			{
				return NULL;
			}

			video_mem = vm_map_phys(SELF, (void *) mr.mr_base, size);

			if (video_mem == MAP_FAILED) {
				panic("video_txt couldn't map video memory");
			}
		}
		else
			return NULL;

	}
	else
		return NULL;

	return video_mem;
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



int setPixel(unsigned short x, unsigned short y,unsigned long color)
{
	char *tmp = video_mem;
	if(x < 0 || x >= h_res || y < 0 || y >= v_res){
		return 1;
	}

	tmp += (x + y*h_res)*bits_per_pixel/BYTE_SIZE;
	*tmp=color;

	return 0;
}



int draw_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color)
{
	if ((x+size > h_res) || (y+size > v_res))
		return 1;

	unsigned short h_pos;
	for (h_pos = x; h_pos < x + size; h_pos++)
		setPixel(h_pos, y, color);

	unsigned short v_pos;
	for (v_pos = y + 1; v_pos <= y + (size-2); v_pos++)
	{
		setPixel(x, v_pos, color);
		setPixel(x + size - 1, v_pos, color);
	}

	for (h_pos = x; h_pos < x + size; h_pos++)
		setPixel(h_pos, v_pos, color);

	return 0;
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

		  if(setPixel(x,y,color)){
			  return 1;
		  }
	}
	return 0;
}

int draw_pixmap(Sprite *s){

	int i,j;
	for (i = 0; i < s->height; i++) {
		for (j = 0; j < s->width; ++j) {
			if(setPixel((unsigned short) (j + s->x), (unsigned short) (i + s->y), s->map[(i*s->width + j)*bits_per_pixel/8]))
				return 1;
		}
	}
	return 0;
}

