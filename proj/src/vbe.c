/** @file */
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	mmap_t map;
	struct reg86u registo;

	if(lm_init()==NULL)
		return 1;

	if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL) {
		printf("lm_alloc failed...\n");
		return 1;
	}

	registo.u.b.ah = VBE_FUNCTION_PREFIX;
	registo.u.b.intno = VBE_INTERRUPT_VECTOR ;
	registo.u.b.al = VBE_MODE_INFORMATION;
	registo.u.w.cx = mode;
	registo.u.w.es = PB2BASE(map.phys);
	registo.u.w.di = PB2OFF(map.phys);

	if (sys_int86(&registo)!=OK)
	{
		if ((registo.u.b.ah == FUNCTION_CALL_FAILED) || (registo.u.b.ah == FUNCTION_NOT_SUPPORTED) || (registo.u.b.ah == FUNCTION_INVALID)){
			lm_free(&map);
			return 1;
		}
	} else if(registo.u.b.ah==VBE_FUNCTION_COMPLETED_SUCCESSFULLY && registo.u.b.al==VBE_FUNCTION_SUPPORTED){
		*vmi_p = *((vbe_mode_info_t*) map.virtual);
		lm_free(&map);
	}
	return 0;
}



uint16_t* vbe_get_info_block(vbe_info_block_t *controller)
{
	mmap_t map;
	struct reg86u registo;

	if(lm_init()==NULL)
		return NULL;

	if (lm_alloc(sizeof(vbe_info_block_t), &map) == NULL) {
		printf("lm_alloc failed...\n");
		return NULL;
	}

	strcpy (controller->VbeSignature, "VBE2");

	registo.u.b.ah = VBE_FUNCTION_PREFIX;
	registo.u.b.intno = VBE_INTERRUPT_VECTOR ;
	registo.u.b.al = VBE_GET_CONTROLLER_INFO;
	registo.u.w.es = PB2BASE(map.phys);
	registo.u.w.di = PB2OFF(map.phys);

	if (sys_int86(&registo)!=OK)
	{
		if ((registo.u.b.ah == FUNCTION_CALL_FAILED) || (registo.u.b.ah == FUNCTION_NOT_SUPPORTED) || (registo.u.b.ah == FUNCTION_INVALID)){
			lm_free(&map);
			return NULL;
		}
	} else if(registo.u.b.ah==VBE_FUNCTION_COMPLETED_SUCCESSFULLY && registo.u.b.al==VBE_FUNCTION_SUPPORTED){
		*controller = *((vbe_info_block_t*) map.virtual);

		void* farptr = (void *)((controller->VideoModePtr & 0xFFFF0000) >> 12);
		farptr += PB2OFF(controller->VideoModePtr);
		farptr += (uint32_t) map.virtual & 0xF0000000;
		lm_free(&map);

		return farptr;
	}
}
