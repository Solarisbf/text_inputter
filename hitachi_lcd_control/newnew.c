/*
 * XHYPER255_TKU3
 * key test.c
 * 2004-07-01   <newboder@hybus.net>
 * 
 */

#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <string.h>
#include <asm/ioctls.h>

#define KEY_NUM1    1
#define KEY_NUM2    2
#define KEY_NUM3    3
#define KEY_NUM4    4
#define KEY_NUM5    5
#define KEY_NUM6    6
#define KEY_NUM7    7
#define KEY_NUM8    8
#define KEY_NUM9    9
#define KEY_NUM10   10
#define KEY_NUM11   11
#define KEY_NUM12   12
#define KEY_NUM13   13
#define KEY_NUM14   14
#define KEY_NUM15   15
#define KEY_NUM16   16

#define dbg(x...)       printf(x) 

#define MAXFND	8
#define fnd_dev	"/dev/fnd"

static char keyDev[] = "/dev/key";
static int  keyFd = (-1);
static int dev=(-1);
static int bufCount = MAXFND-1;
static unsigned char buf[MAXFND+1];  // human sight 1~10 ,a~f on led
static unsigned int FND_NUM[MAXFND+1];//  real number in machine
static int DEC_OR_HEX = 10;       // DEC MODE : 10 ,  HEX MODE : 16
static int DEC_Value = 0;          //jinyong added
static int HEX_Value = 0;         // jinyong added

void OPEN_FND()
{
	dev = open(fnd_dev, O_RDWR);
	if (dev < 0){
		fprintf(stderr, "cannot open FND (%d)", dev);
		exit(2);
	}

}
void OPEN_KEY(){
	if((keyFd = open(keyDev, O_RDONLY)) < 0){
	perror("open faile /dev/key");
	exit(-1);
	}
}

unsigned char key_get(int tmo)

{
	unsigned char b;
	if (tmo)
	{
		if (tmo < 0)
			tmo = ~tmo * 1000;
		else
			tmo *= 1000000;
		while (tmo > 0) 
		{
			usleep(550000);
			ioctl(keyFd, FIONREAD, &b);
			if (b) return(b);
			tmo -= 10000;
		}
		return(-1); /* timeout */
	}
	else
	{
		read(keyFd, &b, sizeof(b));
		return(b);
	}
}

unsigned char asc_to_fnd(int n)
{
	unsigned char c;
		switch (n) {
			case  0: c = 0x3f;  break;	//0
			case  1: c = 0x06;  break;	//1
			case  2: c = 0x5b;  break;	//2
			case  3: c = 0x4f;  break;	//3
			case  4: c = 0x66;  break;	//4
			case  5: c = 0x6d;  break;	//5
			case  6: c = 0x7d;  break;	//6
			case  7: c = 0x07;  break;	//7
			case  8: c = 0x7f;  break;	//8
			case  9: c = 0x67;  break;	//9
			case 10: c = 0x77;  break;	//A
			case 11: c = 0x7c;  break;	//b
			case 12: c = 0x58;  break;	//c
			case 13: c = 0x5e;  break;	//d
			case 14: c = 0x79;  break;	//E
			case 15: c = 0x71;  break;	//F
			case 16: c = 0x50; break;	//r
			case 17: c = 0x5c; break;	//o
			default: c = 0x00; break;
		}
	return c;

}

 void Clear_FND_NUM() 
{
	int i;
	for ( i =0 ; i <= MAXFND -1; i++) {
		FND_NUM[i]=0;
	}
} 

void Clear_Buf(){
	int i;
	for(i = 0; i <= MAXFND -1; i++){
		buf[i] = asc_to_fnd(18);
		}
	
}

void Shift_Buf()
{
        int i=0;
        for ( i = 0; i <= MAXFND-1 ; i++)
        {
                buf[i]=buf[i+1];
		FND_NUM[i]=FND_NUM[i+1];    //// FND_NUM HUMAN number		
        }
}
void Back_Space()
{	
	int i;
	
	/*buf[7]=buf[6];
	buf[6]=buf[5];
	buf[5]=buf[4];
	buf[4]=buf[3];
	buf[3]=buf[2];
	buf[2]=buf[1];
	buf[1]=buf[0];
	buf[0]=0; */

	for(i = 7; i > 0 ; i--)
		buf[i] = buf[i-1];

		buf[0] = 0;
	
	if ( buf[7] == 0) {
		for(i=7; i >=0 ; i--)
			FND_NUM[i] = 0;
	} else {
	for(i = 7; i > 0 ; i--)
		FND_NUM[i]=FND_NUM[i-1];
	
		FND_NUM[0] = 0;
	}
		
	write(dev,buf,MAXFND);
	
	
}
void Err_Message()
{
	buf[MAXFND-1]=asc_to_fnd(16);
	buf[MAXFND-2]=asc_to_fnd(17);
        buf[MAXFND-3]=asc_to_fnd(16);
        buf[MAXFND-4]=asc_to_fnd(16);
        buf[MAXFND-5]=asc_to_fnd(14);
		
	Clear_FND_NUM();
	//Clear_Buf();
}
void DEC_to_HEX(int result){
	int num=0;
	int i;
	int count = 7;
	for(i=10000000; i>=1; i /= 10){
		num = result % 16;
		if ( result == 0 )
			num = 18;
		result = result/16;
		
		
		//if ( result == 0 )
		//	num =18; /// default
		//FND_NUM[count] = num;
		buf[count--]=asc_to_fnd(num);
		}
		write(dev, buf, MAXFND);
		usleep(1300000);

		//Clear_Buf();
		//write(dev,buf,MAXFND);
		//Clear_FND_NUM();
		DEC_Value = 0;
}
void HEX_to_DEC(int result) {
	int num = 0;	
	int i;
	int  count = 7;
	
	for ( i = 268435456; i >=1 ; i /= 16) 
	{
		if ( result <= -1 ) {  // overflow result > 2^32 
			Err_Message();
			write(dev,buf,MAXFND);
			usleep(1000000);
			Clear_Buf();
			write(dev,buf,MAXFND);

			break;
		}
		num = result % 10;
		if ( result == 0 )
			num = 18;
		result = result /10;

		/*if ( result == 0)
 		{ 		  //for deleting default 0 ex) result A 
			num = 18; // A ->  00000010 on FND -> 10 on FNd
		 } */
		//FND_NUM[count] = num;
		buf[count--]=asc_to_fnd(num);
	}
	write(dev,buf,MAXFND);
	usleep(1300000);
	
	//Clear_Buf();	
	//write(dev,buf,MAXFND);
	//Clear_FND_NUM();
	HEX_Value = 0;

}

void get_DEC_Value()  // J added
{
	int i,x;
	for ( i = 7 ; i >= 0 ; i--)
	{
		switch(i) {
			case 0 : x = 10000000; break;
			case 1 : x = 1000000; break;
			case 2 : x = 100000; break;
			case 3 : x = 10000; break;
			case 4 : x = 1000; break;
			case 5 : x = 100; break;
			case 6 : x = 10; break;
			case 7 : x = 1; break;
		}
	DEC_Value += FND_NUM[i] * x;	
	}
	printf("DEC_VALUE : %d \n",DEC_Value);
}
void get_HEX_Value() 
{
	int i,x;
	for ( i = 7 ; i >= 0 ; i-- )
	{
		switch(i) {
			case 0 : x = 16*16*16*16*16*16*16; break;	
			case 1 : x = 16*16*16*16*16*16; break;
			case 2 : x = 16*16*16*16*16; break;
			case 3 : x = 16*16*16*16; break;
			case 4 : x = 16*16*16; break;
			case 5 : x = 16*16; break;
			case 6 : x = 16; break;
			case 7 : x = 1; break;
	}
	HEX_Value += FND_NUM[i] * x;
	}
	printf("HEX_VALUE : %d \n",HEX_Value);
}	



void Input_Key_HEX(){
	int i;
	unsigned char c;

	//get_HEX_Value();
	//HEX_to_DEC(HEX_Value);

	while(1){
	/*if ( DEC_OR_HEX == 10)
	{		
		get_HEX_Value();
		HEX_to_DEC(HEX_Value);
	
		break;
	} else */  
        c = key_get(10);
        switch (c){
        case KEY_NUM1:  if(bufCount==7) {
                                buf[MAXFND-1]=asc_to_fnd(10);
				FND_NUM[MAXFND-1] = 10;
			}
                        else if(bufCount!=7){
                                Shift_Buf();
                                buf[MAXFND-1]=asc_to_fnd(10);	
				FND_NUM[MAXFND-1] = 10;
				
                        }
                        bufCount--;
                        write(dev,buf,MAXFND);
                        break;
        case KEY_NUM2:  if(bufCount==7) {
                                buf[MAXFND-1]=asc_to_fnd(11);
				FND_NUM[MAXFND-1] = 11;
			}
                        else if(bufCount!=7){
                                Shift_Buf();
                                buf[MAXFND-1]=asc_to_fnd(11);
				FND_NUM[MAXFND-1] = 11;
                        }
                        bufCount--;
                        write(dev,buf,MAXFND);
                        break;
        case KEY_NUM3:  if(bufCount==7) {
                                buf[MAXFND-1]=asc_to_fnd(12);
				FND_NUM[MAXFND-1] = 12;
			}
                        else if(bufCount!=7){
                                Shift_Buf();
                                buf[MAXFND-1]=asc_to_fnd(12);
				FND_NUM[MAXFND-1] = 12;
                        }
                        bufCount--;
                        write(dev,buf,MAXFND);
                        break;
        case KEY_NUM4:  if(bufCount==7){
                                Err_Message();
                                Clear_Buf();
                        }
                        else if(bufCount!=7){
                                Back_Space();
                        }
                        break;
	case KEY_NUM5:  if(bufCount==7) {
                                buf[MAXFND-1]=asc_to_fnd(13);
				FND_NUM[MAXFND-1] = 13;
			}
                        else if(bufCount!=7){
                                Shift_Buf();
                                buf[MAXFND-1]=asc_to_fnd(13);
				FND_NUM[MAXFND-1] = 13;
                        }
                        bufCount--;
                        write(dev,buf,MAXFND);
                        break;
        case KEY_NUM6:  if(bufCount==7) {
                                buf[MAXFND-1]=asc_to_fnd(14);
				FND_NUM[MAXFND-1] = 14;
			}
                        else if(bufCount!=7){
                                Shift_Buf();
                                buf[MAXFND-1]=asc_to_fnd(14);
				FND_NUM[MAXFND-1] = 14;
                        }
                        bufCount--;
                        write(dev,buf,MAXFND);
                        break;
        case KEY_NUM7:  if(bufCount==7) {
                                buf[MAXFND-1]=asc_to_fnd(15);
				FND_NUM[MAXFND-1] = 15;
			}
                        else if(bufCount!=7){
                                Shift_Buf();
                                buf[MAXFND-1]=asc_to_fnd(15);
				FND_NUM[MAXFND-1] = 15;
                        }
                        bufCount--;
                        write(dev,buf,MAXFND);
                        break;
        case KEY_NUM8:  break;
        case KEY_NUM12: DEC_OR_HEX = 10;
			break;
	case KEY_NUM13:	Clear_Buf();
			Clear_FND_NUM();            //clear
                        bufCount=7;
                        write(dev,buf,MAXFND);	
			break;
		}
	if ( DEC_OR_HEX == 10)
	{
		Clear_Buf();
		break;	
	}
    }
  
}
void Input_Key_DEC(){
	int i;
	unsigned char c;
	
	//get_DEC_Value();	
	//DEC_to_HEX(DEC_Value);

	while(1){
	/*if ( DEC_OR_HEX = 16)
	{
		get_DEC_Value();
		DEC_to_HEX(DEC_Value);
		break;
	} else*/  
	c = key_get(10);
	switch (c){
	case KEY_NUM1:	if(bufCount==7) {
				buf[MAXFND-1]=asc_to_fnd(1);
				FND_NUM[MAXFND-1] = 1;
			}
			else if(bufCount!=7){
				Shift_Buf();
				buf[MAXFND-1]=asc_to_fnd(1);
				FND_NUM[MAXFND-1] = 1;
			}
			bufCount--;
			write(dev,buf,MAXFND);
			break;
	case KEY_NUM2:	if(bufCount==7) {
                                buf[MAXFND-1]=asc_to_fnd(2);
				FND_NUM[MAXFND-1] = 2; 
			}
                        else if(bufCount!=7){
                                Shift_Buf();
                                buf[MAXFND-1]=asc_to_fnd(2);
				FND_NUM[MAXFND-1] = 2;
                        }
                        bufCount--;
                        write(dev,buf,MAXFND);
			break;
	case KEY_NUM3:	if(bufCount==7) {
                                buf[MAXFND-1]=asc_to_fnd(3);
				FND_NUM[MAXFND-1] = 3;
			}
                        else if(bufCount!=7){
                                Shift_Buf();
                                buf[MAXFND-1]=asc_to_fnd(3);
				FND_NUM[MAXFND-1] = 3;
                        }
                        bufCount--;
			write(dev,buf,MAXFND);
			break;
	case KEY_NUM4:	if(bufCount==7){
				Err_Message();
				write(dev,buf,MAXFND);
				usleep(500000);
				Clear_Buf();
			}
			else if(bufCount!=7){
				Back_Space();
			}	
                        break;
	case KEY_NUM5:	if(bufCount==7) {
                                buf[MAXFND-1]=asc_to_fnd(4);
				FND_NUM[MAXFND-1] = 4; 
 			}
                        else if(bufCount!=7){
                                Shift_Buf();
                                buf[MAXFND-1]=asc_to_fnd(4);
				FND_NUM[MAXFND-1] = 4;
                        }
                        bufCount--;
                        write(dev,buf,MAXFND);
                        break;
	case KEY_NUM6:	if(bufCount==7) {
                                buf[MAXFND-1]=asc_to_fnd(5);
				FND_NUM[MAXFND-1] = 5;
 			}
                        else if(bufCount!=7){
                                Shift_Buf();
                                buf[MAXFND-1]=asc_to_fnd(5);
				FND_NUM[MAXFND-1] = 5;
                        }
                        bufCount--;
                        write(dev,buf,MAXFND);
                        break;
	case KEY_NUM7:	if(bufCount==7) {
                                buf[MAXFND-1]=asc_to_fnd(6);
				FND_NUM[MAXFND-1] = 6;
			}
                        else if(bufCount!=7){
                                Shift_Buf();
                                buf[MAXFND-1]=asc_to_fnd(6);
				FND_NUM[MAXFND-1] = 6;
                        }
                        bufCount--;
                        write(dev,buf,MAXFND);
                        break;
	case KEY_NUM8:	DEC_OR_HEX = 16;
			break; //HEX
                      
	case KEY_NUM9:	if(bufCount==7) {
                                buf[MAXFND-1]=asc_to_fnd(7);
				FND_NUM[MAXFND-1] = 7;
 			}
                        else if(bufCount!=7){
                                Shift_Buf();
                                buf[MAXFND-1]=asc_to_fnd(7);
				FND_NUM[MAXFND-1] = 7;
                        }
                        bufCount--;
                        write(dev,buf,MAXFND);
                        break;
	case KEY_NUM10: if(bufCount==7) {
                                buf[MAXFND-1]=asc_to_fnd(8);
				FND_NUM[MAXFND-1] = 8;
			}
                        else if(bufCount!=7){
                                Shift_Buf();
                                buf[MAXFND-1]=asc_to_fnd(8);
				FND_NUM[MAXFND-1] = 8;
                        }
                        bufCount--;
                        write(dev,buf,MAXFND);
                        break;
	case KEY_NUM11:	if(bufCount==7) {
                                buf[MAXFND-1]=asc_to_fnd(9);
				FND_NUM[MAXFND-1] = 9;
			}
                        else if(bufCount!=7){
                                Shift_Buf();
                                buf[MAXFND-1]=asc_to_fnd(9);
				FND_NUM[MAXFND-1] = 9;
                        }
                        bufCount--;
                        write(dev,buf,MAXFND);
                        break;
	case KEY_NUM12:	break; //DEC
	case KEY_NUM13:	Clear_Buf();		//clear
			Clear_FND_NUM();
			bufCount=7;
			write(dev,buf,MAXFND);
			break;
	case KEY_NUM14:	if(bufCount!=7){
				Shift_Buf();
                                buf[MAXFND-1]=asc_to_fnd(0);
				FND_NUM[MAXFND-1] = 0;
				bufCount--;
			}
                        else{
			break;
			}
                        write(dev,buf,MAXFND);
                        break;
	case KEY_NUM15:	break;
	case KEY_NUM16:	break;

		}
	   
	if ( DEC_OR_HEX == 16)
	{	
		Clear_Buf();
		break;

	}
     }
  
}
int main()
{
	OPEN_KEY();
	OPEN_FND();
	memset(buf, 0, sizeof(buf));
	
	while(1) {
	
	if ( DEC_OR_HEX == 10 ) { 
	Input_Key_DEC();

	get_DEC_Value();
	DEC_to_HEX(DEC_Value);

	}
	else if ( DEC_OR_HEX == 16) {
	Input_Key_HEX();
	
	get_HEX_Value();
	HEX_to_DEC(HEX_Value);
	}
	
		}
	return 0;
}
