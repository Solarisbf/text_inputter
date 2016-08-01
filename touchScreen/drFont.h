#ifndef DRFONT_H
#define DRFONT_H

typedef struct _font{
	char fileName[25];
	unsigned int offset[9];
	unsigned short offsetChar[9];
	unsigned int data_size;
	unsigned int pixel_size;
}DRFONT;

void init(DRFONT *font, char filename[]);
int getFontBit(char(*fontbuf)[9], unsigned short ch, DRFONT *font);	// return buf size

#endif

