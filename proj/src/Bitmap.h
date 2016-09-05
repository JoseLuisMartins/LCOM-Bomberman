#ifndef __BITMAP_H
#define __BITMAP_H


/** @defgroup Bitmap Bitmap
 * @{
 * Functions for manipulating bitmaps
 */


#define HOR_SPACE		4
#define HOR_SIZE		10
#define VER_SIZE		23


typedef enum {
    ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT
} Alignment;

typedef struct {
    unsigned short type; // specifies the file type
    unsigned int size; // specifies the size in bytes of the bitmap file
    unsigned int reserved; // reserved; must be 0
    unsigned int offset; // specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} BitmapFileHeader;

typedef struct {
    unsigned int size; // specifies the number of bytes required by the struct
    int width; // specifies width in pixels
    int height; // specifies height in pixels
    unsigned short planes; // specifies the number of color planes, must be 1
    unsigned short bits; // specifies the number of bit per pixel
    unsigned int compression; // specifies the type of compression
    unsigned int imageSize; // size of image in bytes
    int xResolution; // number of pixels per meter in x axis
    int yResolution; // number of pixels per meter in y axis
    unsigned int nColors; // number of colors used by the bitmap
    unsigned int importantColors; // number of colors that are important
} BitmapInfoHeader;

/// Represents a Bitmap
typedef struct {
    BitmapInfoHeader bitmapInfoHeader;
    unsigned char* bitmapData;
} Bitmap;


/**
 * @brief Loads a bmp image
 *
 * @param filename Path of the image to load
 * @return Non NULL pointer to the image buffer
 */
Bitmap* loadBitmap(const char* filename);

/**
 * @brief Draws an unscaled, unrotated bitmap at the given position
 *
 * @param bitmap bitmap to be drawn
 * @param x destiny x coord
 * @param y destiny y coord
 * @param alignment image alignment
 * @param buffer buffer's flag
 */
void drawBitmap(Bitmap* bitmap, int x, int y, Alignment alignment, int buffer);



/**
 * @brief Draws a bitmap from a certain position
 *
 * @param bmp bitmap to be drawn
 * @param xScreen horizontal position on buffer
 * @param yScreen vertical position on buffer
 * @param xStart starting horizontal position on image
 * @param yStart starting vertical position on image
 * @param xSize number of pixels to be drawn horizontally
 * @param ySize number of pixels to be drawn vertically
 * @param alignment image alignment
 * @param buffer buffer's flag
 */
void drawBitmapWithBoundaries(Bitmap* bmp, int xScreen, int yScreen, int xStart, int yStart, int xSize, int ySize, Alignment alignment, int buffer);




/**
 * @brief Destroys the given bitmap, freeing all resources used by it.
 *
 * @param bmp bitmap to be destroyed
 */
void deleteBitmap(Bitmap* bmp);



/**
 * Initializes bitmap necessary for font
 */
void initializeFont();



/**
 * Frees memory used by font's bitmap
 */
void destroyFont();



/**
 * @brief Draws an integer\n
 * Converts number to string and then draws it on a desired position
 *
 * @param drawNumber number to be drawn
 * @param x initial horizontal position
 * @param y initial vertical position
 */
void draw_side_bar_number_int(int drawNumber, int x, int y);



/**
 * @brief Draws a double\n
 * Converts number to string and then draws it on a desired position
 *
 * @param drawNumber number to be drawn
 * @param x initial horizontal position
 * @param y initial vertical position
 */
void draw_side_bar_number_double(double drawNumber, int x, int y);



/**
 * @brief Draws a string on a desired position\n
 *
 * @param str string to be drawn
 * @param x initial horizontal position
 * @param y initial vertical position
 * @param buffer buffer's flag
 */
void draw_text(char* str, int x, int y, int buffer);



/**@}*/

#endif
