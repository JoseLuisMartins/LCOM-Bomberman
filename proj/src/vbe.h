#ifndef __VBE_H
#define __VBE_H

#include <stdint.h>

/** @defgroup vbe vbe
 * @{
 *
 * Functions related to the VBE standard
 */

/** @name VBE Mode Info Block */
/**@{
 *
 * Packed VBE Mode Info Block 
 */ 


/* VBE RETURNS */
#define VBE_FUNCTION_SUPPORTED  				0x4F
#define VBE_FUNCTION_COMPLETED_SUCCESSFULLY 	0x00
#define FUNCTION_CALL_FAILED					0x01
#define FUNCTION_NOT_SUPPORTED					0x02
#define FUNCTION_INVALID						0x03
/* ----------- */


#define VBE_INTERRUPT_VECTOR 					0x10
#define VBE_FUNCTION_PREFIX 					0x4F
#define VBE_GET_CONTROLLER_INFO					0x00
#define VBE_VIDEO_MODE_FUNCTION					0x00
#define VBE_MODE_INFORMATION 					0x01
#define VBE_SET_MODE 							0x02
#define SET_TEXT_MODE							0x03
#define LINEAR_FRAME_BUFFER						(1<<14)

#define MODE1									0x101
#define	MODE2									0x103
#define MODE3									0x105
#define MODE4									0x107

#define MODE_64K_COLORS							0x117
#define MODE_32K_COLORS							0x116
 
// Controller
#define DAC_BIT									0
#define VGA_BIT									1
#define RAMDAC_BIT								2
#define VIDEOMODEPTR_END						0x0FFFF


typedef struct {
  /*  Mandatory information for all VBE revisions 1*/
  uint16_t ModeAttributes; 	/**< @brief mode attributes */
  uint8_t WinAAttributes; 		/**< @brief window A attributes */
  uint8_t WinBAttributes; 		/**< @brief window B attributes */
  uint16_t WinGranularity; 	/**< @brief window granularity */
  uint16_t WinSize;		/**< @brief window size */
  uint16_t WinASegment;		/**< @brief window A start segment */
  uint16_t WinBSegment;		/**< @brief window B start segment */
  phys_bytes WinFuncPtr;	/**< @brief real mode/far pointer to window function */
  uint16_t BytesPerScanLine; 	/**< @brief bytes per scan line */

  /* Mandatory information for VBE 1.2 and above */

  uint16_t XResolution;      	/**< @brief horizontal resolution in pixels/characters */
  uint16_t YResolution;      	/**< @brief vertical resolution in pixels/characters */
  uint8_t XCharSize; 		/**< @brief character cell width in pixels */
  uint8_t YCharSize; 		/**< @brief character cell height in pixels */
  uint8_t NumberOfPlanes; 		/**< @brief number of memory planes */
  uint8_t BitsPerPixel; 		/**< @brief bits per pixel */
  uint8_t NumberOfBanks;		/**< @brief number of banks */
  uint8_t MemoryModel;		/**< @brief memory model type */
  uint8_t BankSize;		/**< @brief bank size in KB */
  uint8_t NumberOfImagePages;	/**< @brief number of images */
  uint8_t Reserved1;		/**< @brief reserved for page function */

  /* Direct Color fields (required for direct/6 and YUV/7 memory models) */
  
  uint8_t RedMaskSize;		/* size of direct color red mask in bits */
  uint8_t RedFieldPosition;	/* bit position of lsb of red mask */
  uint8_t GreenMaskSize;		/* size of direct color green mask in bits */
  uint8_t GreenFieldPosition;	/* bit position of lsb of green mask */
  uint8_t BlueMaskSize; 		/* size of direct color blue mask in bits */
  uint8_t BlueFieldPosition;	/* bit position of lsb of blue mask */
  uint8_t RsvdMaskSize;		/* size of direct color reserved mask in bits */
  uint8_t RsvdFieldPosition;	/* bit position of lsb of reserved mask */
  uint8_t DirectColorModeInfo;	/* direct color mode attributes */

  /* Mandatory information for VBE 2.0 and above */
  phys_bytes PhysBasePtr;       /**< @brief physical address for flat memory frame buffer */
  uint8_t Reserved2[4]; 		/**< @brief Reserved - always set to 0 */
  uint8_t Reserved3[2]; 		/**< @brief Reserved - always set to 0 */

  /* Mandatory information for VBE 3.0 and above */
  uint16_t LinBytesPerScanLine;    /* bytes per scan line for linear modes */
  uint8_t BnkNumberOfImagePages; 	/* number of images for banked modes */
  uint8_t LinNumberOfImagePages; 	/* number of images for linear modes */
  uint8_t LinRedMaskSize; 	        /* size of direct color red mask (linear modes) */
  uint8_t LinRedFieldPosition; 	/* bit position of lsb of red mask (linear modes) */
  uint8_t LinGreenMaskSize; 	/* size of direct color green mask (linear modes) */
  uint8_t LinGreenFieldPosition; /* bit position of lsb of green mask (linear  modes) */
  uint8_t LinBlueMaskSize; 	/* size of direct color blue mask (linear modes) */
  uint8_t LinBlueFieldPosition; 	/* bit position of lsb of blue mask (linear modes ) */
  uint8_t LinRsvdMaskSize; 	/* size of direct color reserved mask (linear modes) */
  uint8_t LinRsvdFieldPosition;	 /* bit position of lsb of reserved mask (linear modes) */
  uint32_t MaxPixelClock; 	         /* maximum pixel clock (in Hz) for graphics mode */
  uint8_t Reserved4[190]; 		 /* remainder of ModeInfoBlock */
} __attribute__((packed)) vbe_mode_info_t;

/** @} end of vbe_mode_info_t*/





typedef struct
{
	char VbeSignature[4]; 					// VBE Signature
	uint16_t VbeVersion;					// VBE Version
	uint32_t OemStringPt;					// Pointer to OEM String
	uint8_t Capabilities[4];				// Capabilities of graphics controller
	uint32_t VideoModePtr;					// Pointer to VideoModeList
	uint16_t TotalMemory;					// Number of 64kb memory blocks
	// Added for VBE 2.0
	uint16_t OemSoftwareRev;				// VBE implementation Software revision
	uint32_t OemVendorNamePtr;			// Pointer to Vendor Name String
	uint32_t OemProductNamePtr;			// Pointer to Product Name String
	uint32_t OemProductRevPtr;			// Pointer to Product Revision String
	uint8_t Reserved[222];					// Reserved for VBE implementation
	uint8_t OemData[256];					// Data Area for OEM Strings
}__attribute__((packed)) vbe_info_block_t;












/**
 * @brief Returns information on the input VBE mode, including screen dimensions, color depth and VRAM physical address
 * 
 * Initializes unpacked vbe_mode__info_t structure passed as an address with
 *  the information of the input mode, by calling VBE function 0x01
 *  Return VBE Mode Information and unpacking the ModeInfoBlock struct
 *  returned by that function.
 * 
 * @param mode mode whose information should be returned
 * @param vmi_p address of vbe_mode_info_t structure to be initialized
 * @return 0 on success, non-zero otherwise
 */
int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p);


/**
 * @brief Funtion that give us the video mode info
 *
 *
 */
/** @} end of vbe */
#endif /* __VBE_H */
