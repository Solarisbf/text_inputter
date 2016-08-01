#ifndef TE_H
#define TE_H

typedef struct _node{
	unsigned short ch;
	int isEdit;
	struct _node *next;
	struct _node *prev;
}node;

typedef struct _list{
	int count;
	struct _node *head;
	struct _node *cursor;
}StringBuf;

void initStringBuf(StringBuf *buf);
void insertText(StringBuf *buf, unsigned short chN);
void updateCursorText(StringBuf *buf, unsigned short ch);
void updatePrevText(StringBuf *buf, unsigned short ch);
void deleteText(StringBuf *buf);


void setCursorEditMode(StringBuf *buf, int isEdit);
void setPrevEditMode(StringBuf *buf, int isEdit);

void moveCursor(StringBuf *buf, int cursor);
int getCursorIndex(StringBuf *buf);
void showAllString(StringBuf *buf);

unsigned short getCursorCh(StringBuf *buf);
unsigned short getPrevCh(StringBuf *buf);
int getCursorEditMode(StringBuf *buf);
int getPrevEditMode(StringBuf *buf);

#endif

