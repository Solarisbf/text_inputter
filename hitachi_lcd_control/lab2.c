#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <asm/fcntl.h>
#include <string.h>
#include <stdio.h>

#define ADDRESSOFTEXTLCD   	0x12380000
#define ADDRESSOFTEXTLCD_CMD   	0x12300000
#define ADDRESSOFFND	0x11000000    // jin added  for fnd without device driver. for mmap

#define MAXFND	8
#define fnd_dev	"/dev/fnd"

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
#define KEY_NUM16   16  // Jin added

void setcommand(unsigned short command);
void initialize_textlcd();
void writebyte(char ch);
void inputKey();  // Jin added
void OPEN_KEY();  // Jin added

unsigned char key_get(int tmo); // jin added
unsigned char asc_to_fnd(int n); //jin added

static int INS = 10;  //   10 : INSERT MODE    , 20 : NORMAL MODE
static int CAPS = 0;  //   0 : lower case     ,  1 : upper case
static int keyCount = 0; // Jin added for counting key  ex) 1 -> a  2 -> b 3-> c 4 -> a.....
static int WDCOUNT = 0;  // write 1 letter +1 , delete 1 letter -1 
static char keyDev[] = "/dev/key";
static int  keyFd = (-1);



unsigned short *pTextlcd;
unsigned short *pTextlcd_cmd;
unsigned char *pTextFnd;

int main(int argc, char **argv){
	int fd,dev;
	int i, len1=11,len2=19;
	char buf1[100];
    char buf2[100];

	OPEN_KEY(); // key open~

	if (( dev = open(fnd_dev,O_RDWR) ) < 0 ) {
			fprintf(stderr, "cannot open FND (%d)", dev);
		    exit(2);
	}

	if ((fd=open("/dev/mem",O_RDWR | O_SYNC)) < 0){
            perror("mem open failed \n");
            exit(1);
	}


// 물리어드레스를 가상 어드레스로 변환 
	   pTextFnd = mmap(NULL,1,PROT_WRITE,MAP_SHARED,keyFd,ADDRESSOFFND);  // fnd mmap !!
       pTextlcd = mmap(NULL,4,PROT_WRITE, MAP_SHARED, fd, ADDRESSOFTEXTLCD);
       pTextlcd_cmd = mmap(NULL,4,PROT_WRITE, MAP_SHARED, fd, ADDRESSOFTEXTLCD_CMD);

	if((int)pTextlcd < 0) {
              pTextlcd=NULL;
		      close(fd);
              printf("mmap error\n");
              return -1;
	}	
       
	initialize_textlcd();                //이쪽을 수정해야함.

	for(i=0;i<len1;i++) writebyte(buf1[i]);
	setcommand(0xC0);
	for(i=0;i<len2;i++) writebyte(buf2[i]);

	munmap(pTextlcd,4);
	close(fd);
}

// clcd 제어를 위한 각 함수의 구현

void setcommand(unsigned short command)
{
	command &= 0x00FF;
	*pTextlcd_cmd = command|0x0000;
	*pTextlcd_cmd = command|0x0400;
	*pTextlcd_cmd = command|0x0000;
	usleep(50);
}

void writebyte(char ch){
	unsigned short data;
data = ch & 0x00FF;
      *pTextlcd = data|0x0100;
      *pTextlcd = data|0x0500;
      *pTextlcd = data|0x0100;
 	usleep(50);
}

void initialize_textlcd(){	//CLCD 초기화
	setcommand(0x38); // 8비트 2줄
	setcommand(0x38); 
	setcommand(0x38); 
	setcommand(0x0e); // 디스플레이온 커서 온 
	setcommand(0x02); // 커서 홈 
	setcommand(0x01); // 표시 클리어 
	usleep(2000);
}


void OPEN_KEY(){
	if((keyFd = open(keyDev, O_RDONLY)) < 0){
	perror("open failed /dev/key");
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

unsigned char asc_to_fnd(int n)     // Caps or INS  showing on fnd
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
			case 18: c = 0x72; break;   //p
			case 19: c = 0x54; break;   // n
			default: c = 0x00; break;
		}
	return c;

}

void inputKey() {
	int i;
	unsigned char c;

	while(1){
		c = key_get(10);
        switch (c) {

		    case KEY_NUM1:  setcommand(0x10); break;  // <=
			case KEY_NUM2:  dbg("KEY_NUM2\n"); break;  // DEL
			case KEY_NUM3:  setcommand(0x14); break;  // =>
			case KEY_NUM4:  if(CAPS ==0)
							CAPS = 1;
							pTextFnd[4] = asc_to_fnd(12); //c
							pTextFnd[3] = asc_to_fnd(10); //A
							pTextFnd[2] = asc_to_fnd(18); //p
							pTextFnd[1] = asc_to_fnd(5); // s ( 5)
							if(CAPS ==1)
							CAPS = 0;
							pTextFnd[4] = asc_to_fnd(20);  // clear
							pTextFnd[3] = asc_to_fnd(20);
							pTextFnd[2] = asc_to_fnd(20);
							pTextFnd[1] = asc_to_fnd(20);
							break;  // CAPS
			case KEY_NUM5:  WDCOUNT++;
							if( keyCount % 3 == 1)
								writebyte('.');   //.
							if( keyCount % 3 == 2)
								CAPS == 0 ? writebyte('q') : writebyte('Q');    //q
							if( keyCount % 3 == 0)
								CAPS == 0 ? writebyte('z') : writebyte('Z');    //z
							// keyCountReset(); after 3 seconds , keyCount will be reset 0 
							
							break;  // .qz
			case KEY_NUM6:  WDCOUNT++;
							if( keyCount % 3 == 1)
								CAPS == 0 ? writebyte('a') : writebyte('A');// a
							if( keyCount % 3 == 2)
								CAPS == 0 ? writebyte('b') : writebyte('B');//b
							if( keyCount % 3 == 0)
								CAPS == 0 ? writebyte('c') : writebyte('C'); //c
							// keyCountReset(); after 3 seconds , keyCount will be reset 0 
							break;  // abc
			case KEY_NUM7:  WDCOUNT++;
							if( keyCount % 3 == 1)
								CAPS == 0 ? writebyte('d') : writebyte('D'); //d
							if( keyCount % 3 == 2)
								CAPS == 0 ? writebyte('e') : writebyte('E'); //e
							if( keyCount % 3 == 0)
								CAPS == 0 ? writebyte('f') : writebyte('F'); //f
							// keyCountReset(); after 3 seconds , keyCount will be reset 0 
						   break;  // def
			case KEY_NUM8:  if(INS == 10 )
								INS = 20;
								pTextFnd[4] = asc_to_fnd(1); //I
								pTextFnd[3] = asc_to_fnd(19); //n
								pTextFnd[2] = asc_to_fnd(5); //S (5)
							if(INS == 20 )
								INS = 10;
								pTextFnd[4] = asc_to_fnd(20);  // clear
								pTextFnd[3] = asc_to_fnd(20);
								pTextFnd[2] = asc_to_fnd(20);
								
							break;  // INS
			case KEY_NUM9:  WDCOUNT++;
							if( keyCount % 3 == 1)
								CAPS == 0 ? writebyte('g') : writebyte('G'); //g
							if( keyCount % 3 == 2)
								CAPS == 0 ? writebyte('h') : writebyte('H'); //h
							if( keyCount % 3 == 0)
								CAPS == 0 ? writebyte('i') : writebyte('I'); //i
							// keyCountReset(); after 3 seconds , keyCount will be reset 0 
								break;  // ghi
			case KEY_NUM10: WDCOUNT++;
							if( keyCount % 3 == 1)
								CAPS == 0 ? writebyte('j') : writebyte('J');//j
							if( keyCount % 3 == 2)
								CAPS == 0 ? writebyte('k') : writebyte('K'); //k
							if( keyCount % 3 == 0)
								CAPS == 0 ? writebyte('l') : writebyte('L'); //l
							// keyCountReset(); after 3 seconds , keyCount will be reset 0  
								break; // jkl
			case KEY_NUM11: WDCOUNT++;
							if( keyCount % 3 == 1)
								CAPS == 0 ? writebyte('m') : writebyte('M'); //m
							if( keyCount % 3 == 2)
								CAPS == 0 ? writebyte('n') : writebyte('N'); //n
							if( keyCount % 3 == 0)
								CAPS == 0 ? writebyte('o') : writebyte('O'); //o
							// keyCountReset(); after 3 seconds , keyCount will be reset 0  
								break; // mno
			case KEY_NUM12:  writebyte('*');
				             break; // *
			case KEY_NUM13: WDCOUNT++;
							if( keyCount % 3 == 1)
								CAPS == 0 ? writebyte('p') : writebyte('P'); //p
							if( keyCount % 3 == 2)
								CAPS == 0 ? writebyte('r') : writebyte('R'); //r
							if( keyCount % 3 == 0)
								CAPS == 0 ? writebyte('s') : writebyte('S'); //s
							// keyCountReset(); after 3 seconds , keyCount will be reset 0 
								break; // prs 
			case KEY_NUM14: WDCOUNT++; 
							if( keyCount % 3 == 1)
								CAPS == 0 ? writebyte('t') : writebyte('T'); //t
							if( keyCount % 3 == 2)
								CAPS == 0 ? writebyte('u') : writebyte('U'); //u
							if( keyCount % 3 == 0)
								CAPS == 0 ? writebyte('v') : writebyte('V'); //v
							// keyCountReset(); after 3 seconds , keyCount will be reset 0 
								break; // tuv
			case KEY_NUM15: WDCOUNT++; 
							if( keyCount % 3 == 1)
								CAPS == 0 ? writebyte('w') : writebyte('W'); //w
							if( keyCount % 3 == 2)
								CAPS == 0 ? writebyte('x') : writebyte('X'); //x
							if( keyCount % 3 == 0)
								CAPS == 0 ? writebyte('y') : writebyte('Y'); //y
							// keyCountReset(); after 3 seconds , keyCount will be reset 0 
								break; // wxy
			case KEY_NUM16:  writebyte('#');
							 break; // #
			default: /* timeout */ break;
        }
	}
  
}