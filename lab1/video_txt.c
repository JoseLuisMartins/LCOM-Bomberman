#include <minix/drivers.h>
#include <sys/video.h>
#include <sys/mman.h>
#include <string.h>
#include <math.h>

#include <assert.h>

#include "vt_info.h"

#include "video_txt.h"

/* Private global variables */

static char *video_mem;		/* Address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;	/* Height of screen in lines */

void vt_fill(char ch, char attr) {

	char *vptr;
	vptr = video_mem;

	unsigned int i;
	for (i = 0; i < (scr_width*scr_lines); i++)
	{
		*vptr = ch;
		vptr++;
		*vptr = attr;
		vptr++;
	}

}

void vt_blank() {

	char *vptr;
	vptr = video_mem;

	unsigned int i;
	for (i = 0; i < (scr_width*scr_lines); i++)
	{
		*vptr = 0;
		vptr ++;
		*vptr = 0x07;
		vptr++;
	}
}

int vt_print_char(char ch, char attr, int r, int c) {

	char *vptr;
	vptr = video_mem;

	vptr += 2*(scr_width*r + c);
	*vptr = ch;
	vptr++;
	*vptr = attr;

	return 0;
}

int vt_print_string(char *str, char attr, int r, int c) {

	if (r < 0 || (unsigned) r  > scr_lines || c < 0 || (unsigned) c > scr_width)
	{
		printf("Invalid Input. String out of range.\n");
		exit(1);
	}
	else if (strlen(str) > 80)
	{
		printf("String is too long (> 80).\n");
		exit(1);
	}
	else if ( (r == scr_lines-1) && (strlen(str) + c) > scr_width-1)
	{
		printf("Invalid Input. String out of bounds.\n");
		exit(1);
	}

	char *vptr;
	vptr = video_mem;

	vptr += 2*(scr_width*r + c);

	int i = 0;
	while (str[i] != '\0')
	{
		*vptr = str[i];
		vptr++;
		* vptr = attr;
		vptr++;
		i++;
	}

	return 0;
}

int vt_print_int(int num, char attr, int r, int c) {

	int length=1;
	int tmp=num;

	while(tmp/10 !=0){
		tmp /=10;
		length++;
	};

	if(r==24 && ((c + length) > 80)){
		printf("The number is to big\n");
		return 1;
	}

	char *vptr;
	vptr = video_mem;
	vptr += 2*(scr_width*r + c);

	int div= pow(10,(length-1));

	int i;
	for(i=0;i<length;i++,vptr++){
		*vptr = ((num/div) + 48);
		vptr++;
		*vptr =attr;
		num%=div;
		div /=10;
	}

	return 0;

}


int vt_draw_frame(int width, int height, char attr, int r, int c) {

	if ((unsigned) (r + height) > scr_lines || (unsigned) (c + width) > scr_width)
	{
		printf("Invalid Input. Frame out of range.\n");
		exit(1);
	}
	else
	{

		int row = r, column = c;
		int i=0;

		vt_print_char(0xC9, attr, row, column);
		column++;
		for (i = 0; i < (width-2); i++)
		{
			vt_print_char(0xCD, attr, row, column);
			column++;
		}
		vt_print_char(0xBB, attr, row, column);


		for (i = 0; i < (height-2); i++)
		{
			row++;
			column = c;
			vt_print_char(0xBA, attr, row, column);
			column = c + (width-1);
			vt_print_char(0xBA, attr, row, column);
		}


		row++;
		column = c;
		vt_print_char(0xC8, attr, row, column);
		column++;
		for (i = 0; i < (width-2); i++)
		{
			vt_print_char(0xCD, attr, row, column);
			column++;
		}
		vt_print_char(0xBC, attr, row, column);

		return 0;
	}
}



/*
 * THIS FUNCTION IS FINALIZED, do NOT touch it
 */

char *vt_init(vt_info_t *vi_p) {

  int r;
  struct mem_range mr;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes)(vi_p->vram_base);
  mr.mr_limit = mr.mr_base + vi_p->vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	  panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vi_p->vram_size);

  if(video_mem == MAP_FAILED)
	  panic("video_txt couldn't map video memory");

  /* Save text mode resolution */

  scr_lines = vi_p->scr_lines;
  scr_width = vi_p->scr_width;

  return video_mem;
}
