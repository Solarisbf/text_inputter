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

#define KEY_NUM1	1
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

static char keyDev[] = "/dev/key";
static int  keyFd = (-1);

unsigned char key_get(int tmo)
{   
    unsigned char b;

    if (tmo) { 
        if (tmo < 0)
            tmo = ~tmo * 1000;
        else
            tmo *= 1000000;
        while (tmo > 0) {
            usleep(10000);
            ioctl(keyFd, FIONREAD, &b);
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

int main()
{
    unsigned char c;

    if((keyFd = open( keyDev, O_RDONLY )) < 0){         // KEY open
		perror("open faile /dev/key");
		exit(-1);
	}

	while(1){
		c = key_get(10);
        switch (c) {
			case KEY_NUM1:  dbg("KEY_NUM1\n"); break;
			case KEY_NUM2:  dbg("KEY_NUM2\n"); break;
			case KEY_NUM3:  dbg("KEY_NUM3\n"); break;
			case KEY_NUM4:  dbg("KEY_NUM4\n"); break;
			case KEY_NUM5:  dbg("KEY_NUM5\n"); break;
			case KEY_NUM6:  dbg("KEY_NUM6\n"); break;
			case KEY_NUM7:  dbg("KEY_NUM7\n"); break;
			case KEY_NUM8:  dbg("KEY_NUM8\n"); break;
			case KEY_NUM9:  dbg("KEY_NUM9\n"); break;
			case KEY_NUM10:  dbg("KEY_NUM10\n"); break;
			case KEY_NUM11:  dbg("KEY_NUM11\n"); break;
			case KEY_NUM12:  dbg("KEY_NUM12\n"); break;
			case KEY_NUM13:  dbg("KEY_NUM13\n"); break;
			case KEY_NUM14:  dbg("KEY_NUM14\n"); break;
			case KEY_NUM15:  dbg("KEY_NUM15\n"); break;
			case KEY_NUM16:  dbg("KEY_NUM16\n"); break;
			default: /* timeout */ break;
        }
	}
	return 0;
}

/* EOF */
