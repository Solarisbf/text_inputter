#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "TextEditer.h"

void initStringBuf(StringBuf *buf){
	buf->count = 0;
	buf->head = NULL;
	buf->cursor = NULL;
}

unsigned short getPrevCh(StringBuf *buf){
	if (buf->cursor != NULL && buf->cursor->prev != NULL){
		return buf->cursor->prev->ch;
	}
	return 0x00;
}

unsigned short getCursorCh(StringBuf *buf){
	if (buf->cursor != NULL){
		return buf->cursor->ch;
	}
	return 0x00;
}

int getPrevEditMode(StringBuf *buf){
	if (buf->cursor != NULL && buf->cursor->prev != NULL){
		return buf->cursor->prev->isEdit;
	}
	return 0;
}

int getCursorEditMode(StringBuf *buf){
	if (buf->cursor != NULL){
		return buf->cursor->isEdit;
	}
	return 0;
}

void insertText(StringBuf *buf, unsigned short chN){

	node *nch = (node*)malloc(sizeof(node));
	nch->ch = chN;
	nch->isEdit = 1;
	nch->next = NULL;
	nch->prev = NULL;

	if (buf->cursor == NULL){
		if (buf->head != NULL){
			nch->next = buf->head;
			buf->head->prev = nch;
			buf->head = nch;
			buf->cursor = nch;
		}
		else{
			buf->head = nch;
			buf->cursor = nch;
		}
		return;
	}


	if (buf->cursor->next != NULL){
		nch->next = buf->cursor->next;
		nch->next->prev = nch;
		nch->prev = buf->cursor;
		buf->cursor->next = nch;
		buf->cursor = nch;
		return;
	}

	if (buf->cursor->next == NULL){
		nch->prev = buf->cursor;
		buf->cursor->next = nch;
		buf->cursor = nch;
		return;
	}
}

void updateCursorText(StringBuf *buf, unsigned short ch){
	if (buf->cursor != NULL){
		buf->cursor->ch = ch;
	}
}

void updatePrevText(StringBuf *buf, unsigned short ch){
	if (buf->cursor != NULL && buf->cursor->prev != NULL){
		buf->cursor->prev->ch = ch;
	}
}


void deleteText(StringBuf *buf){
	node *ctemp = buf->cursor;
	if (ctemp == NULL)
		return;

	node *ptemp = buf->cursor->prev;
	node *ntemp = buf->cursor->next;


	if (ptemp == NULL){
		if (ntemp != NULL){
			buf->head = ntemp;
			ntemp->prev = NULL;
			buf->cursor = NULL;
		}
		else{
			buf->head = NULL;
			buf->cursor = NULL;
		}		
	}
	else{
		if (ntemp != NULL){
			ptemp->next = ntemp;
			ntemp->prev = ptemp;
			buf->cursor = ptemp;
		}
		else{
			ptemp->next = NULL;
			buf->cursor = ptemp;
		}
	}
	free(ctemp);
}


void setCursorEditMode(StringBuf *buf, int isEdit){
	if (buf->cursor != NULL){
		buf->cursor->isEdit = isEdit;
	}
}
void setPrevEditMode(StringBuf *buf, int isEdit){
	if (buf->cursor != NULL && buf->cursor->prev != NULL){
		buf->cursor->prev->isEdit = isEdit;
	}
}

void showAllString(StringBuf *buf){
	node *p = buf->head;
	while (p != NULL){
		unsigned short ch = p->ch;
		printf(" 0x%4X \n", ch);
		p = p->next;
	}
}

void moveCursor(StringBuf *buf, int cursor){
	int i=0;
	node *p = buf->head;

	if( cursor == 0 )
		buf->cursor = NULL;

	while (p != NULL){
		i++;
		buf->cursor = p;
		p = p->next;
		if( cursor == i ){
			return ;
		}
	}
}

int getCursorIndex(StringBuf *buf){
	int i=0;
	node *p = buf->head;

	while (p != NULL){
		i++;
		if( p == buf->cursor ){
			return i;
		}
		p = p->next;
	}
	return i;
}
