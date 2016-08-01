#include "drFont.h"
#include <string.h>
#include <stdio.h>

unsigned short atoh(char *buf){
	unsigned short h = 0;
	int j;
	for (j = 0; j < strlen(buf); j++){
		unsigned char a = buf[j];

		if ('0' <= a && a <= '9')
			a = a - '0';
		else if ('A' <= a && a <= 'F')
			a = (a - 'A') + 10;

		h = h << 4;
		h = h | a;
	}
	return h;
}

unsigned short getOffset(DRFONT *font, unsigned short findCh, unsigned int _offset){
	FILE *fp = fopen(font->fileName, "r");
	char buf[32];
	char buf2[32];
	unsigned short bH;
	unsigned int offset = _offset;
	unsigned int temoff = 0;
	unsigned int filesize = 0;
	int temp;

	if (findCh == 0){
		fclose(fp);
		return 0;
	}

	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (font->data_size <= 0){
		fscanf(fp, "%s %d", buf, &font->pixel_size);
		fscanf(fp, "%s %d", buf, &font->data_size);
	}
	else{
		fseek(fp, offset, SEEK_SET);
	}

	sprintf(buf2, "%04X", findCh);
	do{
		if (fscanf(fp, "%s", buf) == EOF) break;
		offset = ftell(fp) - strlen(buf);
		if (strcmp("CHAR", buf) == 0){
			fscanf(fp, "%s", buf);
			bH = atoh(buf);
			if (findCh == bH){
				fclose(fp);
				return offset;
			}
			else{
				if (findCh > bH){
					temp = findCh - bH;
					do{
						temoff = offset + (font->data_size*(temp));
						temp = temp / 2;
					} while (temoff > filesize);
					fseek(fp, temoff, SEEK_SET);
					temoff = offset;
				}
				else{
					filesize = offset;
					offset = temoff + (offset - temoff) / 2;
					fseek(fp, offset, SEEK_SET);
				}
			}
		}
	} while (1);

	fclose(fp);
	return 0;
}

void init(DRFONT *font, char filename[]){
	int i = 0;
	unsigned int offset = 0x00000000;
	strcpy(font->fileName, filename);
	font->pixel_size = 0;
	font->data_size = 0;
	font->offsetChar[0] = 0x0020;
	font->offsetChar[1] = 0x0023;
	font->offsetChar[2] = 0x002A;
	font->offsetChar[3] = 0x0030;
	font->offsetChar[4] = 0x0041;
	font->offsetChar[5] = 0x0061;
	font->offsetChar[6] = 0x3131;
	font->offsetChar[7] = 0x314F;
	font->offsetChar[8] = 0xAC00;
	for (i = 0; i < 9; i++){
		font->offset[i] = getOffset(font, font->offsetChar[i], offset);
		offset = font->offset[i];
	}
}
int getFontBit(char(*fontbuf)[9], unsigned short ch, DRFONT *font){	// return buf size
	FILE *fp = fopen(font->fileName, "r");
	char buf[32];
	char buf2[32];
	unsigned short bH;
	unsigned int offset = 0;
	unsigned int temoff = 0;
	unsigned int filesize = 0;
	int temp;
	int i = 0;

	if (ch == 0){
		fclose(fp);
		return 0;
	}

	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	fseek(fp, 0, SEEK_SET);


	for (i = 0; i < 9; i++){
		if (ch >= font->offsetChar[i]){
			offset = font->offset[i];
		}
	}

	if (font->data_size <= 0){
		fscanf(fp, "%s %d", buf, &font->pixel_size);
		fscanf(fp, "%s %d", buf, &font->data_size);
	}
	else{
		fseek(fp, offset, SEEK_SET);
	}

	sprintf(buf2, "%04X", ch);
	do{
		if (fscanf(fp, "%s", buf) == EOF) break;
		offset = ftell(fp) - strlen(buf);
		if (strcmp("CHAR", buf) == 0){
			fscanf(fp, "%s", buf);
			bH = atoh(buf);
			if (ch == bH){
				for (i = 0; i < font->pixel_size; i++){
					fscanf(fp, "%s", buf);
					strcpy(fontbuf[i], buf);
				}
				fclose(fp);
				return font->pixel_size;
			}
			else{
				if (ch > bH){
					temp = ch - bH;
					do{
						temoff = offset + (font->data_size*(temp));
						temp = temp / 2;
					} while (temoff > filesize);
					fseek(fp, temoff, SEEK_SET);
					temoff = offset;
				}
				else{
					filesize = offset;
					offset = temoff + (offset - temoff) / 2;
					fseek(fp, offset, SEEK_SET);
				}
			}
		}
	} while (1);

	fclose(fp);
	return 0;
}
