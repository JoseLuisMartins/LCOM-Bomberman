#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H


/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

#define VRAM_PHYS_ADDR			0xE0000000
#define H_RES             		1024
#define V_RES		  			768
#define BITS_PER_PIXEL	  		8
#define BYTE_SIZE				8


#define TRANSPARENCY			0x07E0

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);
/**
 * @brief Returns a pointer to the second buffer
 *
 *@return Pointer to the second buffer
 */
char *getSecond_Buffer();
/**
 * @brief Returns a pointer to the third buffer
 *
 *@return Pointer to the third buffer
 *
 */
char* getThird_Buffer();
/**
 * @brief Returns the horizontal resolution of the current video mode
 *
 *@return  horizontal resolution of the current video mode
 */
unsigned getHorResolution();
/**
 * @brief Returns the vertical resolution of the current video mode
 *
 *@return  Vertical resolution of the current video mode
 */
unsigned getVerResolution();
/**
 * @brief Set`s a pixel in the position with the coordenates (x,y) passed by argument on the buffer passed by argument\n
 *  with the color also received by argument
 *
 *@param x coordinate x where the pixel is set
 *@param y coordinate y where the pixel is set
 *@param color color to which the pixel is set
 *@param buffer buffer in wich the pixel is set
 *
 *@return  Return 0 upon success and non-zero otherwise
 */
int setPixel(unsigned short x, unsigned short y,unsigned long color,int buffer);
/**
 * @brief copies the content of the third buffer to the second buffer
 *
 */
void swapThirdBuffer();
/**
 * @brief copies the content of the second buffer to the video mem
 *
 */
void swapVideoMem();
/**
 * @brief Draws a rectangle at the coordenates passed by argument
 *
 * @param xi x coordenate of the top-left corner of the rectangle
 * @param yi y coordenate of the top-left corner of the rectangle
 * @param xf x coordenate of the bottom-right corner of the rectangle
 * @param yf y coordenate of the bottom-right corner of the rectangle
 * @param color Color of the rectangle bords
 *
 */
void draw_rectangle(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color);
/**
 * @brief Draws a line
 * @param x1 x coordenate of the first point of the line
 * @param y1 y coordenate of the first point of the line
 * @param x2 x coordenate of the second point of the line
 * @param y2 y coordenate of the second point of the line
 * @param color Color of the line
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_line(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long color);


/** @} end of video_gr */

#endif /* __VIDEO_GR_H */

