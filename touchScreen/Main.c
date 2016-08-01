#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h> /* for ioctl */
#include <sys/types.h>
#include <sys/mman.h> // for mmap
#include <linux/fb.h> /* for fb_var_screeninfo, FBIOGET_VSCREENINFO */
#include "TextEditer.h"
#include "MakeCharacter.h"
#include "drFont.h"

#define EVENT_BTN	0x00001000
#define EVENT_CUR	0x00002000
#define BUTTON_KEY	0x000000FF

// Open Devices ----------------------------------------------------------------------------
static int fd,fbfd;
static struct fb_var_screeninfo fbvar;
static unsigned short *pfbdata;

void openDevices(){
	int ret;
	fbfd = open("/dev/fb0", O_RDWR);
	if(fbfd < 0) {
		perror("fbdev open");
		exit(1);
	}
	ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &fbvar);
	if(ret < 0) {
		perror("fbdev ioctl");
		exit(1);
	}
	if(fbvar.bits_per_pixel != 16) {
		fprintf(stderr, "bpp is not 16\n");
		exit(1);
	}
	fd = open("/dev/ts", O_RDWR);
	pfbdata = (unsigned short *)mmap(0,fbvar.xres*fbvar.yres*16/8,PROT_READ|PROT_WRITE,MAP_SHARED,fbfd,0);
}
//===========================================================================================


// Get Touch--- -----------------------------------------------------------------------------
/*unsigned char getTouch(int tmo)
{
	unsigned char b;

	if (tmo) {
		if (tmo < 0)
			tmo = ~tmo * 1000;
		else
			tmo *= 70000;//1000000;
		while (tmo > 0) {
			usleep(10000);
			ioctl(keyFd, FIONREAD, &b);
			if (b) return(b);
			tmo -= 10000;
		}
		return KEY_TIMEOUT;

	}
	else {
		read(keyFd, &b, sizeof(b));
		return(b);
	}


	// 	unsigned short buf[4];		
	//	
	//	unsigned int x,y,bt,b;
	//	unsigned int p = 0;
	//		
	//	read(fd,buf,sizeof(buf));
}*/

int getButtonIndex(int x, int y){
	int b = -1;
	// first row -------------------------------
	if (x > 120 && x < 215 && y > 240 && y < 295) {
		b = 0;
	}

	if (x > 220 && x < 315 && y > 240 && y < 295) {
		b = 1;
	}

	if (x > 320 && x < 415 && y > 240 && y < 295) {
		b = 2;
	}

	if (x > 420 && x < 520 && y > 240 && y < 295) {
		b = 3;
	}
	//--------------------------------------------

	// second row ------------------------------
	if (x > 120 && x < 215 && y > 300 && y < 355) {
		b = 4;
	}
	if (x > 220 && x < 315 && y > 300 && y < 355) {
		b = 5;
	}
	if (x > 320 && x < 415 && y > 300 && y < 355) {
		b = 6;
	}
	if (x > 420 && x < 520 && y > 300 && y < 355) {
		b = 7;
	}
	//---------------------------------------------

	// third row --------------------------------
	if (x > 120 && x < 215 && y > 360 && y < 415) {
		b = 8;
	}
	if (x > 220 && x < 315 && y > 360 && y < 415) {
		b = 9;
	}
	if (x > 320 && x < 415 && y > 360 && y < 415) {
		b = 10;
	}
	if (x > 420 && x < 520 && y > 360 && y < 415) {
		b = 11;
	}
	//-----------------------------------------------

	// forth row ------------------------------------
	if (x > 120 && x < 215 && y > 420 && y < 480) {
		b = 12;
	}
	if (x > 220 && x < 315 && y > 420 && y < 480) {
		b = 13;
	}
	if (x > 320 && x < 415 && y > 420 && y < 480) {
		b = 14;
	}
	if (x > 420 && x < 520 && y > 420 && y < 480) {
		b = 15;
	}
	//--------------------------------------------------
	return b;
}

int boardLocation(unsigned int x, unsigned int y)
{
	int rx = -1;
	int ry = -1;

	if (x > 240 && x < 256) rx = 0;
	if (x > 256 && x < 272) rx = 1;
	if (x > 272 && x < 288) rx = 2;
	if (x > 288 && x < 304) rx = 3;
	if (x > 304 && x < 320) rx = 4;
	if (x > 320 && x < 336) rx = 5;
	if (x > 336 && x < 352) rx = 6;
	if (x > 352 && x < 368) rx = 7;
	if (x > 368 && x < 384) rx = 8;
	if (x > 384 && x < 400) rx = 9;

	if (y > 140 && y < 156) ry = 0;
	if (y > 156 && y < 172) ry = 1;
	if (y > 172 && y < 188) ry = 2;
	if (y > 188 && y < 204) ry = 3;
	if (y > 204 && y < 220) ry = 4;

	if (rx < 0 || ry < 0){
		return -1;
	}

	return (ry*10)+rx;
}

/*
 get Touch Value
 return 0xFFFFFFFF
 0x____F___ : touch point (button 1, textEdit 2, else 0)
 0x_____F__ : touch Type (touch 1 , non touch 0 , drag 2)
 0x______FF : button or cursor
*/
unsigned int stateValue()
{
	unsigned int state = 0x00;
	unsigned short buf[4];
	unsigned int x,y,p;
	int idx;
	int tmo = 1;

	while (1){
		read(fd, buf, sizeof(buf));
		x = buf[1];
		y = buf[2];
		p = buf[0];
		printf("%d , %d  ;; %d\n",x,y,p);
		if( p == 0){
			idx = getButtonIndex(x,y);
			if( idx >= 0 )
				return EVENT_BTN | idx;
			
			idx = boardLocation(x,y);
			if( idx >= 0 )
				return EVENT_CUR | idx;
		}
	}
return 0;
}

//===========================================================================================

// Draw -------------------------------------------------------------------------------------
typedef unsigned char ubyte;
unsigned short makepixel(ubyte r,ubyte g, ubyte b)
{
	return (unsigned short)(((r>>3)<<11)|((g>>2)<<5)|(b>>3));
}

void cleanLcd()
{
	int i,offset,x,y;
	unsigned short pixel;
	int x_screen, y_screen;

	pixel = makepixel(255,255,255);

	for (x = 0; x < fbvar.xres; x++) {
		for (y = 0; y < fbvar.yres; y++)
		{
			offset = y*fbvar.xres+x;
			*(pfbdata+offset) = pixel;
		}
	}

	for (x_screen = 220; x_screen <= 420; x_screen++) {//for word cover
		for (y_screen = 120; y_screen <= 240; y_screen++)
		{
			offset = y_screen*fbvar.xres + x_screen;
			pixel = makepixel(134, 229, 127);
			*(pfbdata + offset) = pixel;
		}
	}
	for (x_screen = 240; x_screen <= 400; x_screen++) {//for word
		for (y_screen = 140; y_screen <= 220; y_screen++)
		{
			offset = y_screen*fbvar.xres + x_screen;
			pixel = makepixel(206, 251, 201);
			*(pfbdata + offset) = pixel;
		}
	}
}

void printBit(char *buf, int x, int y, int color, int colorbg){
	int i;
	int j;
	int offset;
	int pixel;

	for (j = 0; j < strlen(buf); j++){
		unsigned char a = buf[j];
		unsigned char p;

		if ('0' <= a && a <= '9')
			a = a - '0';
		else if ('A' <= a && a <= 'F')
			a = (a - 'A') + 10;

		p = 0x8;
		for (i = 0; i < 4; i++){
			if (a & p){
				pixel = color; 
				offset = y*fbvar.xres + (x++);
				*(pfbdata + offset) = pixel;
			}
			else{
				pixel = colorbg;
				offset = y*fbvar.xres + (x++);
				*(pfbdata + offset) = pixel;
			}
			p = p >> 1;
		}
	}
}

int drawVoid(int x, int y){
	int i,j;
	int offset;
	int pixel;
	for (i = 0; i < 16; i++){
		for (j=0; j<16; j++){
			pixel = makepixel(206, 251, 201);
			offset = (y+j)*fbvar.xres + (x+i);
			*(pfbdata + offset) = pixel;
		}
	}
}

int drawfont(unsigned short ch, int x, int y, DRFONT *font, int color, int colorbg){
	char fontbuf[22][9];
	int n = 0;
	int i;

	if (ch == 0)
		return 0;

	n = getFontBit(fontbuf, ch, font);

	for (i = 0; i < n; i++){
		printBit(fontbuf[i], x, y++,color,colorbg);
	}


	return 0;
}
void drawTextCur(StringBuf *buf, DRFONT *font, int cursor){
	int x = 240;
	int y = 140;
	int j, count;
	int startdraw = 0;
	int maxShowChar = 50;
	int offset;
	int pixel;

	node *n = buf->head;

	if (buf->cursor == NULL){
		startdraw = 1;
	}
	count = 0;
	while (n != NULL){
		maxShowChar--;
		count++;
		if(count == cursor) startdraw = 1;

		if (startdraw){
			drawVoid(x,y);
			drawfont(n->ch, x, y, font,makepixel(0, 0, 0),makepixel(206, 251, 201)); 
		}

		if (n == buf->cursor){
			for (j=0; j<16; j++){
				pixel = makepixel(0, 0, 0);
				offset = (y+j)*fbvar.xres + (x+15);
				*(pfbdata + offset) = pixel;
			}
		}
		x += 16;
		if( x >= ((16*10)+240)){
			y += 16;
			x = 240;
		}
		n = n->next;
	}
	if( maxShowChar > 0 ){
		drawVoid(x,y);
	}
}

void drawText(StringBuf *buf, DRFONT *font){
	int x = 240;
	int y = 140;
	int j;
	int startdraw = 0;
	int maxShowChar = 50;
	int offset;
	int pixel;

	node *n = buf->head;

	if (buf->cursor == NULL){
		startdraw = 1;
	}

	while (n != NULL){
		maxShowChar--;
		if( maxShowChar < 0 ) break;
		if (buf->cursor != NULL && buf->cursor->prev != NULL && n == buf->cursor->prev){
			startdraw = 1;
		}
		else if (buf->cursor != NULL && n == buf->cursor){
			startdraw = 1;
		}

		if (startdraw){
			drawVoid(x,y);
			drawfont(n->ch, x, y, font,makepixel(0, 0, 0),makepixel(206, 251, 201)); 
		}

		if (n == buf->cursor){
			for (j=0; j<16; j++){
				pixel = makepixel(0, 0, 0);
				offset = (y+j)*fbvar.xres + (x+15);
				*(pfbdata + offset) = pixel;
			}
		}
		x += 16;
		if( x >= ((16*10)+240)){
			y += 16;
			x = 240;
		}
		n = n->next;
	}
	if( maxShowChar > 0 ){
		drawVoid(x,y);
	}
}


void drawButton(int keyType, DRFONT *font)
{
	unsigned short pixel;
	unsigned short ch = 0;
	int x_key, y_key, x_screen, y_screen;
	int offset;
	int y;
	int c,bg,bg2;

	for (x_key = 120; x_key <= 520; x_key++) { // partition x 100 pixel 100 * 4 = 400

		if (x_key > 215 && x_key < 220)
			continue;
		if (x_key > 315 && x_key < 320)
			continue;
		if (x_key > 415 && x_key < 420)
			continue;

		for (y_key = 240; y_key < 480; y_key++)    // partition y 60 pixel 60 * 4 = 240
		{
			if (y_key > 295 && y_key < 300)
				continue;
			if (y_key > 355 && y_key < 360)
				continue;
			if (y_key > 415 && y_key < 420)
				continue;

			offset = y_key*(fbvar.xres) + x_key;

			if (x_key >= 420) {
				bg2 = makepixel(93, 93, 93);
				*(pfbdata + offset) = bg2;
			}
			else{
				bg = makepixel(53, 53, 53);
				*(pfbdata + offset) = bg;
			}
		}
	}

	if (keyType == KEYPAD_KOR ){
		y = 260;
		ch = 0x3131;
		c = makepixel(255, 255, 255);

		drawfont(0x3131,160, y, font,c,bg);
		drawfont(0x3134,260, y, font,c,bg);
		drawfont(0x314F,340, y, font,c,bg);
		drawfont(0x3153,362, y, font,c,bg);
		drawfont(0xC9C0,450, y, font,c,bg2);
		drawfont(0xC6C0,470, y, font,c,bg2);

		y = 320;
		drawfont(0x3139,160, y, font,c,bg);
		drawfont(0x3141,260, y, font,c,bg);
		drawfont(0x3157,340, y, font,c,bg);
		drawfont(0x315C,362, y, font,c,bg);
		drawfont(0xD55C,450, y, font,c,bg2);
		drawfont(0xC601,470, y, font,c,bg2);

		y = 380;
		drawfont(0x3145,160, y, font,c,bg);
		drawfont(0x3147,260, y, font,c,bg);
		drawfont(0x3163,350, y, font,c,bg);

		y = 440;
		drawfont(0xD68D,140, y, font,c,bg);
		drawfont(0xCD94,160, y, font,c,bg);
		drawfont(0xAC00,180, y, font,c,bg);
		drawfont(0x3161,260, y, font,c,bg);
		drawfont(0xC30D,340, y, font,c,bg);
		drawfont(0xC790,360, y, font,c,bg);
		drawfont(0xC74C,380, y, font,c,bg);
		drawfont(0xB744,450, y, font,c,bg2);
		drawfont(0xC6C0,470, y, font,c,bg2);
	}else if (keyType == KEYPAD_ENG ){
		y = 260;
		ch = 0x3131;
		c = makepixel(255, 255, 255);

		drawfont(0x0061,250, y, font,c,bg);
		drawfont(0x0062,265, y, font,c,bg);
		drawfont(0x0063,280, y, font,c,bg);
		drawfont(0x0064,340, y, font,c,bg);
		drawfont(0x0065,355, y, font,c,bg);
		drawfont(0x0066,370, y, font,c,bg);
		drawfont(0xC9C0,450, y, font,c,bg2);
		drawfont(0xC6C0,470, y, font,c,bg2);

		y = 320;
		drawfont(0x0067,150, y, font,c,bg);
		drawfont(0x0068,165, y, font,c,bg);
		drawfont(0x0069,180, y, font,c,bg);
		drawfont(0x006A,250, y, font,c,bg);
		drawfont(0x006B,265, y, font,c,bg);
		drawfont(0x006C,280, y, font,c,bg);
		drawfont(0x006D,335, y, font,c,bg);
		drawfont(0x006E,355, y, font,c,bg);
		drawfont(0x006F,370, y, font,c,bg);
		drawfont(0xD55C,450, y, font,c,bg2);
		drawfont(0xC601,470, y, font,c,bg2);

		y = 380;
		drawfont(0x0070,145, y, font,c,bg);
		drawfont(0x0071,160, y, font,c,bg);
		drawfont(0x0072,175, y, font,c,bg);
		drawfont(0x0073,190, y, font,c,bg);
		drawfont(0x0074,250, y, font,c,bg);
		drawfont(0x0075,265, y, font,c,bg);
		drawfont(0x0076,280, y, font,c,bg);
		drawfont(0x0077,335, y, font,c,bg);
		drawfont(0x0078,355, y, font,c,bg);
		drawfont(0x0079,370, y, font,c,bg);
		drawfont(0x007A,385, y, font,c,bg);
		drawfont(0x0043,430, y, font,c,bg2);
		drawfont(0x0041,445, y, font,c,bg2);
		drawfont(0x0050,470, y, font,c,bg2);
		drawfont(0x0053,485, y, font,c,bg2);

		y = 440;
		drawfont(0x002A,160, y, font,c,bg);
		drawfont(0x0023,360, y, font,c,bg);
		drawfont(0xB744,450, y, font,c,bg2);
		drawfont(0xC6C0,470, y, font,c,bg2);

	}else if (keyType == KEYPAD_ENG_CAPS ){
		y = 260;
		ch = 0x3131;
		c = makepixel(255, 255, 255);

		drawfont(0x0041,250, y, font,c,bg);
		drawfont(0x0042,265, y, font,c,bg);
		drawfont(0x0043,280, y, font,c,bg);
		drawfont(0x0044,340, y, font,c,bg);
		drawfont(0x0045,355, y, font,c,bg);
		drawfont(0x0046,370, y, font,c,bg);
		drawfont(0xC9C0,450, y, font,c,bg2);
		drawfont(0xC6C0,470, y, font,c,bg2);

		y = 320;
		drawfont(0x0047,150, y, font,c,bg);
		drawfont(0x0048,165, y, font,c,bg);
		drawfont(0x0049,180, y, font,c,bg);
		drawfont(0x004A,250, y, font,c,bg);
		drawfont(0x004B,265, y, font,c,bg);
		drawfont(0x004C,280, y, font,c,bg);
		drawfont(0x004D,335, y, font,c,bg);
		drawfont(0x004E,355, y, font,c,bg);
		drawfont(0x004F,370, y, font,c,bg);
		drawfont(0xD55C,450, y, font,c,bg2);
		drawfont(0xC601,470, y, font,c,bg2);

		y = 380;
		drawfont(0x0050,145, y, font,c,bg);
		drawfont(0x0051,160, y, font,c,bg);
		drawfont(0x0052,175, y, font,c,bg);
		drawfont(0x0053,190, y, font,c,bg);
		drawfont(0x0054,250, y, font,c,bg);
		drawfont(0x0055,265, y, font,c,bg);
		drawfont(0x0056,280, y, font,c,bg);
		drawfont(0x0057,335, y, font,c,bg);
		drawfont(0x0058,355, y, font,c,bg);
		drawfont(0x0059,370, y, font,c,bg);
		drawfont(0x005A,385, y, font,c,bg);
		drawfont(0x0043,430, y, font,c,bg2);
		drawfont(0x0041,445, y, font,c,bg2);
		drawfont(0x0050,470, y, font,c,bg2);
		drawfont(0x0053,485, y, font,c,bg2);

		y = 440;
		drawfont(0x002A,160, y, font,c,bg);
		drawfont(0x0023,360, y, font,c,bg);
		drawfont(0xB744,450, y, font,c,bg2);
		drawfont(0xC6C0,470, y, font,c,bg2);
	}
	else if (keyType == KEYPAD_NUM){
		y = 260;
		ch = 0x3131;
		c = makepixel(255, 255, 255);

		drawfont(0x0031, 150, y, font, c, bg);
		drawfont(0x0032, 265, y, font, c, bg);
		drawfont(0x0033, 365, y, font, c, bg);
		drawfont(0xC9C0, 450, y, font, c, bg2);
		drawfont(0xC6C0, 470, y, font, c, bg2);

		y = 320;
		drawfont(0x0034, 150, y, font, c, bg);
		drawfont(0x0035, 265, y, font, c, bg);
		drawfont(0x0036, 365, y, font, c, bg);
		drawfont(0xD55C, 450, y, font, c, bg2);
		drawfont(0xC601, 470, y, font, c, bg2);

		y = 380;
		drawfont(0x0037, 150, y, font, c, bg);
		drawfont(0x0038, 265, y, font, c, bg);
		drawfont(0x0039, 365, y, font, c, bg);

		y = 440;
		drawfont(0x002A, 150, y, font, c, bg);
		drawfont(0x0030, 265, y, font, c, bg);
		drawfont(0x0023, 360, y, font, c, bg);
		drawfont(0xB744, 450, y, font, c, bg2);
		drawfont(0xC6C0, 470, y, font, c, bg2);
	}

}
//===========================================================================================

// onEvent -------------------------------------------------------------------------------------
void onButton(unsigned int key, StringBuf *buf, int *keypadType){
	int info;
	int type;
	MakeBoard mb;
	mb.Char = mb.newChar = mb.prevChar = 0x00;

	if (getCursorEditMode(buf)){
		mb.Char = getCursorCh(buf);
	}
	if (getPrevEditMode(buf)){
		mb.prevChar = getPrevCh(buf);
	}

	info = makeCharacter(&mb, (key & BUTTON_KEY), *keypadType);
	type = info & RT_MASK;

	if ((type&RT_CHMOD) == RT_CHMOD){
		*keypadType = info & 0x0F;
	}
	else{
		if ((type&RT_MIX) == RT_MIX){
			if ((info&RT_MIX_C) == RT_MIX_C){
				updateCursorText(buf, mb.Char);
			}
			if ((info&RT_MIX_P) == RT_MIX_P){
				updatePrevText(buf, mb.prevChar);
			}
		}
		if ((type&RT_ENDEDIT) == RT_ENDEDIT){
			if ((info&RT_ENDEDIT_C) == RT_ENDEDIT_C){
				setCursorEditMode(buf, 0);
			}
			if ((info&RT_ENDEDIT_P) == RT_ENDEDIT_P){
				setPrevEditMode(buf, 0);
			}
		}
		if ((type&RT_DEL) == RT_DEL){
			deleteText(buf);
		}
		if ((type&RT_NEW) == RT_NEW){
			if ((info&RT_NEW_ENDEDTER) == RT_NEW_ENDEDTER){
				insertText(buf, mb.newChar);
				setCursorEditMode(buf, 0);
			} else{
				insertText(buf,mb.newChar);
			}
		}
	}
}
//===========================================================================================

int main(void) {
	StringBuf buf;
	int keypadType;
	int tempType;
	DRFONT mainFont, btnFont;
	unsigned int state;
	int cursor;

	openDevices();

	init(&mainFont, "H2PORM_11.drf");
	init(&btnFont, "H2PORM_16.drf");

	initStringBuf(&buf);
	keypadType = KEYPAD_KOR;

	cleanLcd();

	drawButton(keypadType, &btnFont);

	while (1) {
		state = stateValue();
		printf("state 0x%8X\n",state);
		if ((state & EVENT_BTN) == EVENT_BTN){ // BUTTON
			tempType = keypadType;
			onButton(state, &buf, &keypadType);
			if( keypadType != tempType){
				drawButton(keypadType, &btnFont);
			}else{
				cursor = getCursorIndex(&buf);
				if( cursor > 50 ){
					moveCursor(&buf,50);
				}
				drawText(&buf, &mainFont);
			}
		}
		else if ((state & EVENT_CUR) == EVENT_CUR){ // TOUCH
			cursor = getCursorIndex(&buf);
			moveCursor(&buf,(state&0xFF));
			if( cursor < (state&0xFF) ){
				drawTextCur(&buf, &mainFont,cursor);
			}else{
				drawText(&buf, &mainFont);
			}
		}
	}

	munmap(pfbdata, fbvar.xres*fbvar.yres * 16 / 8);
	close(fd);
	close(fbfd);
	return 0;
}
