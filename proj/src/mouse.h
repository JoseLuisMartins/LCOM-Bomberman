#ifndef __MOUSE_H
#define __MOUSE_H
/** @defgroup mouse mouse
 * @{
 *
 * Functions for using the mouse
 */

#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"

struct mouse;
typedef struct mouse mouse_t;


/**
 * @brief Writes a command passed by argument to a port also passed by argument
 *
 * @param port Port in which the command is written
 * @param cmd Command that is written to the port
 */
static int i8042_write(unsigned long port,unsigned long cmd);
/**
 * @brief Reads the data in the OUT_BUF to the variable passed by pointer
 *
 * @param data Variable used to store the content read from the OUT_BUF
 *
 */
int i8042_read(unsigned long *data);
/**
 * @brief Subscribes and enables mouse interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int mouse_subscribe_int(int *mouse_hook);
/**
 * @brief Unsubscribes mouse interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_unsubscribe_int(int *mouse_hook);
/**
 * @brief writes the command passed by argument to the mouse
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_write(unsigned long cmd);
/**
 *@brief Mouse handler responsible for reading the mouse packet`s
 *
 *@return Return 0 if the mouse packet is fully read and 1 otherwise
 */
static int mouse_packet_handler();
/**
 * @brief Synchronizes the mouse
 *
 *@param byte byte used to check if it is the first byte of the mouse packet
 *@return  Return 0 if the mouse is synchronized and 1 otherwise
 */
static int synchronize(unsigned long byte);
/**
 *@brief Set`s the values of the  mouse_packet package according to the mouse current state
 */
static void setPackageValues();


/** @brief  Creates a mouse
 */
mouse_t* create_mouse();
/**
 *  @brief updates the mouse \n
 *
 *  Updates the mouse position and the buttons pressed or released on the mouse
 *
 *	@param m The mouse
 *	@return Pointer to the mouse created
 */
void update_mouse(mouse_t* m);
/**
 * @brief Draws the mouse
 *
 * @param m  The mouse
 * @param buffer The buffer where the mouse is drawn
 */
void draw_mouse(mouse_t* m,int buffer);
/**
 * @brief Return if left button of the mouse was pressed
 *
 * @param m  The mouse
 * @return return 1 if the left button waas pressed and 0 in the other case
 */
int getDone(mouse_t* m);
/**
 * @brief Return if the mouse needs to be redrawn
 *
 * @param m  The mouse
 * @return return 1 if the mouse needs to be redrawn or 0 in the other case
 */
int getDraw(mouse_t* m);
/**
 * @brief Return whether the mouse is inside the recatangle or not
 *
 * @param m  The mouse
 * @param xi x coordenate of the top-left corner of the rectangle
 * @param yi y coordenate of the top-left corner of the rectangle
 * @param xf x coordenate of the bottom-right corner of the rectangle
 * @param yf y coordenate of the bottom-right corner of the rectangle
 *
 * @return  1 if the mouse is inside the rectangle 0 is the other case
 */
int is_mouse_inside(mouse_t *m,int xi,int yi,int xf,int yf);
/**
 * @brief Destroys the mouse
 *
 * @param m  The mouse
 */
void destroy_mouse(mouse_t *m);
/**
 * @brief Set`s the mouse position to the default position
 *
 * @param m  The mouse
 */
void setDefaultPosition(mouse_t *m);
/**
 * @brief Set`s the flag done of the mouse to 1
 *
 * @param m  The mouse
 */
void setDone(mouse_t* m);
/**
 * @brief Reset`s the mouse done flag
 *
 * @param m  The mouse
 */
void resetDone(mouse_t* m);
/**
 * @brief Returns x coordenate of the mouse cursor
 *
 * @param m  The mouse
 * @return integer with the  x coordenate of the mouse cursor
 */
int mouse_getX(mouse_t* m);
/**
 * @brief Returns y coordenate of the mouse cursor
 *
 * @param m  The mouse
 * @return integer with the  y coordenate of the mouse cursor
 */
int mouse_getY(mouse_t* m);
/**
 * @brief Sets the mouse cursor position to the one passing by the arguments x and y
 *
 * @param m  The mouse
 * @param x  The new x coordenate of the mouse cursor
 * @param y	 The new y coordenate of the mouse cursor
 */
void mouse_setPosition(mouse_t* m, int x, int y);


/**@} */
#endif
