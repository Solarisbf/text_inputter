/*
 * XHYPER255_TKU3
 * fnd test.c
 * 2004-07-01   <newboder@hybus.net>
 *
 * ]# ./fnd_test 12345678 
 */

#include	<stdio.h>
#include	<fcntl.h>

#define fnd_dev	"/dev/fnd"

#define		MAXFND	8

/*
 *	bit# pos
 *
 *	  --0--
 *	5|     |1
 *	  --6--
 *	4|     |2
 *	  --3--  o7
 */
unsigned char asc_to_fnd(int n){
	unsigned char c;

		switch (n) {
			/* 여러가지 문자를 추가할수 있다 */
			case  0: c = 0x3f; break;
			case  1: c = 0x06; break;
			case  2: c = 0x5b; break;
			case  3: c = 0x4f; break;
			case  4: c = 0x66; break;
			case  5: c = 0x6d; break;
			case  6: c = 0x7d; break;
			case  7: c = 0x07; break;
			case  8: c = 0x7f; break;
			case  9: c = 0x67; break;
			default: c = 0x00; break;
		}
	return c;
}

main(int ac, char *av[])
{
	int n, count, dev;
	unsigned char		buf[MAXFND+1];

	dev = open( fnd_dev, O_RDWR);
	if (dev < 0) {
		fprintf(stderr, "cannot open FND (%d)", dev);
		exit(2);
	}
	memset(buf, 0, sizeof(buf));

	for (n = 0 ; n <= 9; n++)
	{
		for( count = 0 ; count < MAXFND; count++){
		buf[count]= asc_to_fnd(n);
		}
		write(dev, buf,MAXFND);
		usleep(500000);
	}
}
/*	EOF	*/
