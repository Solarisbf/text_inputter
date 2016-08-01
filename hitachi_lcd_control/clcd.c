#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <string.h>
#include <asm/ioctls.h>
#include <asm/fcntl.h>

#define ADDRLCD 0x12380000
#define ADDRLCD_CMD 0x12300000
#define MAXFND  8
#define fnd_dev "/dev/fnd"
#define KEY_NUM1        1
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



void setcommand(unsigned short);
unsigned char           buf[MAXFND+1];
void init_clcd();
void writeb(char ch);
unsigned short *pTextlcd;
unsigned short *pTextlcd_cmd;
static char keyDev[] = "/dev/key";
static int  keyFd = (-1);
int cursorSpot, buf1length;
char ifchar, ifcaps, prenum, numnum, except;
int ii, leftend;
unsigned char inputbuf[64] ={ '\0',};
int offset = 0;


unsigned char key_get(int tmo)
{
    unsigned char b;

    if (tmo) {
        if (tmo < 0)
            tmo = ~tmo * 1000;
        else
            tmo *= 1000000;
        while (tmo > 0) {
            usleep(50000);
            ioctl(keyFd, FIONREAD, &b);
		if(b!=KEY_NUM1){
			if(b!=KEY_NUM3){
				usleep(180000);
			}
		}
            if (b) return(b);
            tmo -= 10000;
        }
        return(-1); /* timeout */
    }
    else {
        read(keyFd, &b, sizeof(b));
        return(b);
    }
}

void WTCLCD(char num){
	int j;
	numnum = num;

	if(ifchar == 0){
		if(num == 12){
			num = 43;
			writeb(num);
			cursorSpot++;
			buf1length++;
		}else if(num == 16){
			num = 48;
			writeb(num);
			cursorSpot++;
			buf1length++;
		}
		else{
			num = num+48;
			writeb(num+1);
			cursorSpot++;
			buf1length++;
		}
				
        }else if(ifchar !=0 && (num==16|num ==12)) {
                if(num == 16){
                        except = '#';
                        writeb(except);
                        cursorSpot++;
                        buf1length++;
                }else if(num == 12){
                        except = '*';
                        writeb(except);
                        cursorSpot++;
                        buf1length++;
                }
        }else{
			
		if(prenum == numnum){
			offset = offset + 1;
		}else
			offset = 0;		

		if(prenum == num){ ///같은키 입력
			if(cursorSpot == 16)
				setcommand(0x80 + cursorSpot);
			if(cursorSpot == 48)
				setcommand(0x80 + (cursorSpot-32));
			setcommand(0x10);
			num = (num-1)*3;

			if(num == 18|num==21)		// tuv, wxy 처리
				num = num +1;

			num = 97 + num + (offset%3);
			if(numnum == 6){
				if(offset ==1 || offset ==2)
					num = num+1;
			}
			if(ifcaps == 0 )
				num = num -32;
			if(numnum == 0){
				offset = offset%3;
				if(offset == 0){
					except = 46;
					writeb(except);
				}else if(offset == 1){
					except = 113;
					if(!ifcaps)
						except = except-32;
					writeb(except);
				}else{
					except = 122;
					if(!ifcaps)
						except = except-32;
					writeb(except);
				}
			}else
				writeb(num);
		}else {              		//다른키 입력
			num = (num-1)*3;

			if(num == 18|num==21)    	//// tuv, wxy 처리
				num = num +1;

			num = 97 + num;
			if(ifcaps == 0 )
				num = num -32;
			if(numnum == 0){
                                offset = offset%3;
                                if(offset == 0){
                                        except = 46;
                                        writeb(except);
                                }else if(offset == 1){
                                        except = 113;
                                        if(!ifcaps)
                                                except = except-32;
                                        writeb(except);
                                }else{
                                        except = 122;
                                        if(!ifcaps)
                                                except = except-32;
                                        writeb(except);
                                }
                        }else
				writeb(num);
			cursorSpot++;
			buf1length++;
		}	
			
	}


///////////////////////////입력 끝


	if(cursorSpot >=17|buf1length == 16){
		setcommand(0xC0+(cursorSpot-16));
		setcommand(0x10);
		setcommand(0x14);
	}
	
	if(cursorSpot ==32){
		setcommand(0x01);
		setcommand(0x80 + (cursorSpot -32));
	}

///////////////////////커서 옮기기

	if(ifchar ==0 | prenum != numnum){
		for( j= buf1length - cursorSpot+1; j>=0; j--){
			inputbuf[cursorSpot+j] = inputbuf[cursorSpot+j-1];
        	}
	}
//////////////////////////버퍼 다시 쓰기


	if(numnum == 0 && ifchar !=0)
		inputbuf[cursorSpot-1]=except;
	else if(numnum == 12|numnum == 16){
		if(ifchar !=0)
			inputbuf[cursorSpot-1]=except;
		else
			inputbuf[cursorSpot-1] =num ;
	}else if(ifchar ==0)
		inputbuf[cursorSpot-1]=num+1;

	else
		inputbuf[cursorSpot-1]=num;

	inputbuf[buf1length+1] = '\0';

for( ii = 0; ii<buf1length;ii++)
                printf("buf : %c\n", inputbuf[ii]);



////////////////////////////////커서 위치 재조정
	if(cursorSpot<=15)
		setcommand(0x80+cursorSpot);
	else if(cursorSpot>15&&cursorSpot<32)
		setcommand(0xC0+(cursorSpot-16));
	else if(cursorSpot>31&&cursorSpot<48)
		setcommand(0x80+(cursorSpot-32));
	else
		setcommand(0xc0+(cursorSpot-48));
	

	if(prenum !=numnum){
		for(j = 0; j<buf1length-cursorSpot;j++){
			leftend = cursorSpot+j+1;
              	if(leftend ==17||leftend ==49){
				setcommand(0xc0);
				setcommand(0x10);
				writeb(inputbuf[cursorSpot+j-1]);
			}
			writeb(inputbuf[cursorSpot+j]);
		if(cursorSpot <=31){
			if(cursorSpot + j >31)
				break;
		}
        	}
	}

	printf("cursorSpot = %d \n", cursorSpot);
	if(cursorSpot>=64)
		cursorSpot = 63;
//////////////////////////////////////커서 위치 재조정
	
	if(cursorSpot<=15)
		setcommand(0x80+cursorSpot);
	else if(cursorSpot>15&&cursorSpot<=31)
		setcommand(0xC0+(cursorSpot-16));
	else if(cursorSpot>31&&cursorSpot<=47)
		setcommand(0x80+(cursorSpot-32));
	else
		setcommand(0xc0+(cursorSpot-48));


	prenum = numnum;
	if(ifchar == 0)
		prenum = 100;
	if(ifchar !=0 && (num==16|num ==12))
		prenum = 100;
	
}

void cursorMove(char RL){
	int j;
	prenum = 100;
	if(RL==0){
		setcommand(0x14);
		cursorSpot++;
		if(cursorSpot > buf1length){
			buf1length=cursorSpot;
			inputbuf[buf1length-1]=' ';
                        inputbuf[buf1length] = '\0';
		}
		if(cursorSpot>=64)
                	cursorSpot = 63;
/////////////////////////////////커서위치 조정
		if(cursorSpot<=15)
                	setcommand(0x80+cursorSpot);
        	else if(cursorSpot>15&&cursorSpot<32)
                	setcommand(0xC0+(cursorSpot-16));
        	else if(cursorSpot>31&&cursorSpot<48)
                	setcommand(0x80+(cursorSpot-32));
        	else
                	setcommand(0xc0+(cursorSpot-48));

		if(cursorSpot ==32){
       		         setcommand(0x01);
			 setcommand(0x80);
       		         for(j = 32; j<32+(buf1length-cursorSpot);j++){
               		         leftend = j+1;
             		    if(leftend ==49){
                     	           setcommand(0xc0);
                   	     }
     	         	    writeb(inputbuf[j]);
        	        }
		setcommand(0x80+(cursorSpot-32));
        	}

	}else{
		setcommand(0x10);
		cursorSpot--;
		if(cursorSpot > buf1length)
			buf1length = cursorSpot;
		if(cursorSpot<0)
			cursorSpot = 0;
/////////////////////////////커서 위치 조정
		if(cursorSpot<=15)
	                setcommand(0x80+cursorSpot);
		else if(cursorSpot>15&&cursorSpot<=31)
	   		setcommand(0xC0+(cursorSpot-16));
      		else if(cursorSpot>31&&cursorSpot<=47)
            		setcommand(0x80+(cursorSpot-32));
        	else
			setcommand(0xc0+(cursorSpot-48));

		if(cursorSpot ==31){
			setcommand(0x01);
			for(j = 0; j<=32;j++){
				if(j ==17){
					setcommand(0xc0);
					// setcommand(0x10);
					writeb(inputbuf[j-1]);
				}
				writeb(inputbuf[j]);
				//printf("leftend :%d \n", leftend);
			}
			setcommand(0xc0+(cursorSpot-16));
		}
	}

}

void deleteClcd(){
	int i,j;
	if(cursorSpot<buf1length){
		for(i = 0; i<buf1length - cursorSpot; i++){
			inputbuf[cursorSpot+i] = inputbuf[cursorSpot+i+1];
		}

		buf1length -=1;
		inputbuf[buf1length+1] ='\0';
		inputbuf[buf1length] =' ';

		for(j = 0; j<buf1length-cursorSpot+1;j++){
			leftend = cursorSpot+j+1;
			if(leftend ==17 ||leftend ==49)
				setcommand(0xc0);
			writeb(inputbuf[cursorSpot+j]);
			if(cursorSpot <=31){
				if(cursorSpot + j >31)
					break;
			}
		}
		if(cursorSpot == buf1length)
			cursorSpot--;
		setcommand(0x10);

		if(cursorSpot<0)
			cursorSpot =0;

		if(cursorSpot ==31){
			setcommand(0x01);
			for(j = 0; j<=32;j++){
				if(j ==17){
					setcommand(0xc0);
					// setcommand(0x10);
					writeb(inputbuf[j-1]);
				}
				writeb(inputbuf[cursorSpot+j]);
			}
			setcommand(0xc0+(cursorSpot-16));
		}


		if(cursorSpot<=15)
			setcommand(0x80+cursorSpot);
		else if(cursorSpot>15&&cursorSpot<=31)
			setcommand(0xC0+(cursorSpot-16));
		else if(cursorSpot>31&&cursorSpot<=47)
			setcommand(0x80+(cursorSpot-32));
		else
			setcommand(0xc0+(cursorSpot-48));
	}
}

void statTofnd(char charac, char caps){
	int count;
	for( count = 0 ; count < MAXFND; count++){
		buf[count]= 0x00;
	}	
	if(!caps){
		buf[0] = 0x39;	
		buf[1] = 0x77;	
		buf[2] = 0x73;	
		buf[3] = 0x6d;	
	}
	if(!charac){
		buf[6] = 0x54;	
		buf[7] = 0x3e;	
	}
}



int main(int argc, char **argv) {
	int fd;
	unsigned char c;
	int dev;
	int i, buf2length=0;
	cursorSpot = 0;
	ifchar = 1;
	ifcaps = 1;
	prenum = 100;

	fd=open("/dev/mem", O_RDWR|O_SYNC);
	dev = open( fnd_dev, O_RDWR);
	memset(buf, 0, sizeof(buf));
	if((keyFd = open( keyDev, O_RDONLY )) < 0){         // KEY open
		perror("open faile /dev/key");
		exit(-1);
	}


	pTextlcd = mmap(NULL, 4, PROT_WRITE, MAP_SHARED, fd, ADDRLCD);
	pTextlcd_cmd = mmap(NULL, 4, PROT_WRITE, MAP_SHARED, fd, ADDRLCD_CMD);
	init_clcd();
	statTofnd(ifchar,ifcaps);
	write(dev, buf, MAXFND);
	while(1){
		c = key_get(10);
		switch (c) {
			case KEY_NUM1:  cursorMove(1); break;
			case KEY_NUM2:  deleteClcd(); break;
			case KEY_NUM3:  cursorMove(0); break;
			case KEY_NUM4: if(ifcaps == 0){//big
					       ifcaps = 1;//small
					       statTofnd(ifchar,ifcaps);
					       write(dev, buf, MAXFND);
				       }else{
					       ifcaps = 0;//big   
					       statTofnd(ifchar,ifcaps);
					       write(dev, buf, MAXFND);
				       }
					prenum = 100;
					break;

			case KEY_NUM5:  if(buf1length<64) WTCLCD(0); break;
			case KEY_NUM6: if(buf1length<64) WTCLCD(1); break;
			case KEY_NUM7: if(buf1length<64) WTCLCD(2); break;
			case KEY_NUM8:  if(ifchar == 0){//inti
						ifchar = 1;//char
						statTofnd(ifchar,ifcaps);
						write(dev, buf, MAXFND);
					}else{
						ifchar = 0;//inti   
						statTofnd(ifchar,ifcaps);
						write(dev, buf, MAXFND);
					}
				       break;
			case KEY_NUM9:  if(buf1length<64) WTCLCD(3); break;
			case KEY_NUM10: if(buf1length<64) WTCLCD(4); break;
			case KEY_NUM11: if(buf1length<64) WTCLCD(5); break;
			case KEY_NUM12: if(buf1length<64) WTCLCD(12); break;
			case KEY_NUM13: if(buf1length<64) WTCLCD(6); break;
			case KEY_NUM14: if(buf1length<64) WTCLCD(7); break;
			case KEY_NUM15: if(buf1length<64) WTCLCD(8); break;
			case KEY_NUM16: if(buf1length<64) WTCLCD(16); break; 

						break;

			default: /* timeout */ break;
		}
	}


	munmap(pTextlcd_cmd, 4);
	close(fd);
}

void setcommand(unsigned short command) {
	command &= 0x00FF;
	*pTextlcd_cmd = command|0x0000;
	*pTextlcd_cmd = command|0x0400;
	*pTextlcd_cmd = command|0x0000;
	usleep(50);
}

void writeb(char ch) {
	unsigned short data;
	data = ch & 0x00FF;
	*pTextlcd = data|0x100;
	*pTextlcd = data|0x500;
	*pTextlcd = data|0x100;
	usleep(50);
}

void init_clcd() {
 	setcommand(0x38);
 	setcommand(0x38);
 	setcommand(0x38);
 	setcommand(0x0f);
 	setcommand(0x02);
 	setcommand(0x01);
 	usleep(2000);
}
