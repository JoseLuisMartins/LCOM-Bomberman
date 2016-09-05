/** @file */

#include "Bitmap.h"
#include "stdio.h"
#include "video_gr.h"
#include "utilities.h"

static Bitmap* font_sb = NULL;


Bitmap* loadBitmap(const char* filename) {
	Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));

	// open filename in read binary mode
	FILE *filePtr;
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// read the bitmap file header
	BitmapFileHeader bitmapFileHeader;
	fread(&bitmapFileHeader, 2, 1, filePtr);

	// verify that this is a bmp file by check bitmap id
	if (bitmapFileHeader.type != 0x4D42) {
		fclose(filePtr);
		return NULL;
	}

	int rd;
	do {
		if ((rd = fread(&bitmapFileHeader.size, 4, 1, filePtr)) != 1)
			break;
		if ((rd = fread(&bitmapFileHeader.reserved, 4, 1, filePtr)) != 1)
			break;
		if ((rd = fread(&bitmapFileHeader.offset, 4, 1, filePtr)) != 1)
			break;
	} while (0);

	if (rd = !1) {
		fprintf(stderr, "Error reading file\n");
		exit(-1);
	}

	// read the bitmap info header
	BitmapInfoHeader bitmapInfoHeader;
	fread(&bitmapInfoHeader, sizeof(BitmapInfoHeader), 1, filePtr);

	// move file pointer to the begining of bitmap data
	fseek(filePtr, bitmapFileHeader.offset, SEEK_SET);

	// allocate enough memory for the bitmap image data
	unsigned char* bitmapImage = (unsigned char*) malloc(
			bitmapInfoHeader.imageSize);

	// verify memory allocation
	if (!bitmapImage) {
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// read in the bitmap image data
	fread(bitmapImage, bitmapInfoHeader.imageSize, 1, filePtr);

	// make sure bitmap image data was read
	if (bitmapImage == NULL) {
		fclose(filePtr);
		return NULL;
	}

	// close file and return bitmap image data
	fclose(filePtr);

	bmp->bitmapData = bitmapImage;
	bmp->bitmapInfoHeader = bitmapInfoHeader;

	return bmp;
}



void drawBitmap(Bitmap* bmp, int x, int y, Alignment alignment, int buffer) {
	if (bmp == NULL)
		return;

	int width = bmp->bitmapInfoHeader.width;
	int drawWidth = width;
	int height = bmp->bitmapInfoHeader.height;

	if (alignment == ALIGN_CENTER)
		x -= width / 2;
	else if (alignment == ALIGN_RIGHT)
		x -= width;

	if (x + width < 0 || x > getHorResolution() || y + height < 0
			|| y > getVerResolution())
		return;

	int xCorrection = 0;
	if (x < 0) {
		xCorrection = -x;
		drawWidth -= xCorrection;
		x = 0;

		if (drawWidth > getHorResolution())
			drawWidth = getHorResolution();
	} else if (x + drawWidth >= getHorResolution()) {
		drawWidth = getHorResolution() - x;
	}


	int i;
	for (i = 0; i < height; i++) { // altura

		int j,xbit;

		for(j = 0; j < width ; j++){
			unsigned long color = (bmp->bitmapData[j * 2 + i * width * 2] << 0) | (bmp->bitmapData[j * 2 + i * width * 2 + 1] << 8);
			setPixel(x + j ,  height-i-1+y, color, buffer);
		}
	}
}





void drawBitmapWithBoundaries(Bitmap* bmp, int xScreen, int yScreen, int xStart, int yStart, int xEnd, int yEnd, Alignment alignment, int buffer) {
	if (bmp == NULL)
		return;

	int width = bmp->bitmapInfoHeader.width;
	int drawWidth = width;
	int height = bmp->bitmapInfoHeader.height;

	if (alignment == ALIGN_CENTER)
		xScreen -= width / 2;
	else if (alignment == ALIGN_RIGHT)
		xScreen -= width;

	if (xScreen + width < 0 || xScreen > getHorResolution() || yScreen + height < 0
			|| yScreen > getVerResolution())
		return;

	int xCorrection = 0;
	if (xScreen < 0) {
		xCorrection = -xScreen;
		drawWidth -= xCorrection;
		xScreen = 0;

		if (drawWidth > getHorResolution())
			drawWidth = getHorResolution();
	} else if (xScreen + drawWidth >= getHorResolution()) {
		drawWidth = getHorResolution() - xScreen;
	}


	int i;
	for (i = yStart; i < yEnd + yStart; i++) { // altura
		int j;
		for(j = xStart; j < xEnd + xStart; j++){
			unsigned long color = (bmp->bitmapData[j * 2 + i * width * 2] << 0) | (bmp->bitmapData[j * 2 + i * width * 2 + 1] << 8);
			setPixel(xScreen + j - xStart,  height-i-1 + yScreen, color, buffer);
		}
	}
}




void deleteBitmap(Bitmap* bmp) {
	if (bmp == NULL)
		return;

	free(bmp->bitmapData);
	free(bmp);
}





void initializeFont() {
	font_sb = loadBitmap("home/lcom/lcom1516-t6g01/proj/res/font.bmp");
}


void destroyFont() {
	deleteBitmap(font_sb);
}

void draw_side_bar_number_int(int drawNumber, int x, int y) {
	char str[4];
	sprintf(str, "%d", drawNumber);

	int numberSize = 0;
	do {
		numberSize++;
		drawNumber /= 10;
	} while (drawNumber != 0);

	int i;
	for (i = 0; i < numberSize; i++) {
		drawBitmapWithBoundaries(font_sb, x  + i * (HOR_SIZE + 1), y, (str[i]-'0' + 26) * (HOR_SIZE + HOR_SPACE), 0, HOR_SIZE, VER_SIZE, ALIGN_LEFT, THIRD_BUFFER);
	}
}



void draw_side_bar_number_double(double drawNumber, int x, int y) {
	if (drawNumber > 1/60.0) {
		char str[4];
		sprintf(str, "%.1f", drawNumber);
		int i = 0;
		while (1) {
			if (str[i] != '.')
				drawBitmapWithBoundaries(font_sb, x  + i * (HOR_SIZE + 1), y, (str[i]-'0' + 26) * (HOR_SIZE + HOR_SPACE), 0, HOR_SIZE, VER_SIZE, ALIGN_LEFT, THIRD_BUFFER);
			else
				drawBitmapWithBoundaries(font_sb, x  + i * (HOR_SIZE + 1), y, 36 * (HOR_SIZE + HOR_SPACE), 0, HOR_SIZE, VER_SIZE, ALIGN_LEFT, THIRD_BUFFER);

			if (i >= 4 || str[i-1] == '.')
				break;
			i++;
		}
	}
}


void draw_text(char* str, int x, int y, int buffer) {
	int i = 0;
	while (str[i] != '\0') {

		if (str[i] >= 'A' && str[i] <= 'Z')
			drawBitmapWithBoundaries(font_sb, x + i*(HOR_SIZE + 2), y, (str[i]-'A') * (HOR_SIZE + HOR_SPACE), 0, HOR_SIZE, VER_SIZE, ALIGN_LEFT, buffer);
		else if (str[i] >= '0' && str[i] <= '9')
			drawBitmapWithBoundaries(font_sb, x  + i * (HOR_SIZE + 1), y, (str[i]-'0' + 26) * (HOR_SIZE + HOR_SPACE), 0, HOR_SIZE, VER_SIZE, ALIGN_LEFT, buffer);
		else if (str[i] == '.')
			drawBitmapWithBoundaries(font_sb, x  + i * (HOR_SIZE + 1), y, 36 * (HOR_SIZE + HOR_SPACE), 0, HOR_SIZE, VER_SIZE, ALIGN_LEFT, buffer);
		else if (str[i] == ':')
			drawBitmapWithBoundaries(font_sb, x  + i * (HOR_SIZE + 1), y, 37 * (HOR_SIZE + HOR_SPACE), 0, HOR_SIZE, VER_SIZE, ALIGN_LEFT, buffer);
		else if (str[i] == ',')
			drawBitmapWithBoundaries(font_sb, x  + i * (HOR_SIZE + 1) + HOR_SPACE, y + 8, 38 * (HOR_SIZE + HOR_SPACE), 0, HOR_SIZE, VER_SIZE, ALIGN_LEFT, buffer);
		else
			drawBitmapWithBoundaries(font_sb, x  + i * (HOR_SIZE + 1), y, 39 * (HOR_SIZE + HOR_SPACE) + 1, 0, HOR_SIZE, VER_SIZE, ALIGN_LEFT, buffer);

		i++;
	}
}
