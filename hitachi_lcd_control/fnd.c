/*
 * XHYPER255_TKU3
 * fnd.c  OST2
 * 2004-07-01   <newboder@hybus.net>
 * 
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/rtc.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

#include <asm/hardware.h>
#include <asm/delay.h>
#include <asm/irq.h>
#include <asm/arch/pxa-regs.h>
#include <asm/io.h>

#define FND_MAJOR       231
#define FND_NAME        "fnd"
#define MAX_FND         8

#define FPGA_FND_CS0	(0x11000000)
#define FPGA_FND_CS1	(0x11100000)
#define FPGA_FND_CS2	(0x11200000)
#define FPGA_FND_CS3	(0x11300000)
#define FPGA_FND_CS4	(0x11400000)
#define FPGA_FND_CS5	(0x11500000)
#define FPGA_FND_CS6	(0x11600000)
#define FPGA_FND_CS7	(0x11700000)


/*
	FPGA FND address

   0x14000000 //FPGA_FND0# 
   0x14400000 //FPGA_FND1# 
   0x14800000 //FPGA_FND2# 
   0x14C00000 //FPGA_FND3# 
   0x15000000 //FPGA_FND4# 
   0x15400000 //FPGA_FND5# 
   0x15800000 //FPGA_FND6# 
   0x15C00000 //FPGA_FND7# 

   FND_D[0:7] : X_D[0:7]
*/

#define FND_CS0     (*((volatile unsigned char *)(mem_fnd_cs0)))
#define FND_CS1     (*((volatile unsigned char *)(mem_fnd_cs1)))
#define FND_CS2     (*((volatile unsigned char *)(mem_fnd_cs2)))
#define FND_CS3     (*((volatile unsigned char *)(mem_fnd_cs3)))
#define FND_CS4     (*((volatile unsigned char *)(mem_fnd_cs4)))
#define FND_CS5     (*((volatile unsigned char *)(mem_fnd_cs5)))
#define FND_CS6     (*((volatile unsigned char *)(mem_fnd_cs6)))
#define FND_CS7     (*((volatile unsigned char *)(mem_fnd_cs7)))

void *mem_fnd_cs0, *mem_fnd_cs1, *mem_fnd_cs2, *mem_fnd_cs3, *mem_fnd_cs4, *mem_fnd_cs5, *mem_fnd_cs6, *mem_fnd_cs7;
static unsigned char disp[8] = {0};
static int n = 0 ;

static int fnd_clear(void)
{
    FND_CS0 = 0x00;
    FND_CS1 = 0x00;
    FND_CS2 = 0x00;
    FND_CS3 = 0x00;
    FND_CS4 = 0x00;
    FND_CS5 = 0x00;
    FND_CS6 = 0x00;
    FND_CS7 = 0x00;
	return 0;
}

static int fnd_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    switch(cmd)
    {
        case 1:
                break;
        default: break;
    }
	return 0;
}

static int fnd_open(struct inode *minode, struct file *mfile)
{
   fnd_clear();

//   FND_CS0 = 0xff;
  // FND_CS1 = 0xff;
   
	return 0;
}

static int fnd_release(struct inode *minode, struct file *mfile)
{
	return 0;
}

static ssize_t fnd_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
    int i;

    i=1;
    //get_user(disp, (unsigned char*)buf);
    copy_from_user(disp, buf, count);
    //printk( "write data %x \n", disp[5]);
    
    for(i =1; i <= MAX_FND; i++){	
    switch(i)
    {	
        case  1:    FND_CS7 = disp[0];  break;
        case  2:    FND_CS6 = disp[1];  break;
        case  3:    FND_CS5 = disp[2];  break;
        case  4:    FND_CS4 = disp[3];  break;
        case  5:    FND_CS3 = disp[4];  break;
        case  6:    FND_CS2 = disp[5];  break;
        case  7:    FND_CS1 = disp[6];  break;
        case  8:    FND_CS0 = disp[7];  break;
        default :   break;
    }
    }
    return 0;
}

static struct file_operations device_fops = {
	.open =		fnd_open,
	.write =	fnd_write,
	.release =	fnd_release,
	.ioctl = 	fnd_ioctl,
};

static int fnd_init(void)
{
	int result;
	unsigned long mem_addr_fnd0, mem_addr_fnd1, mem_addr_fnd2, mem_addr_fnd3, mem_addr_fnd4, mem_addr_fnd5, mem_addr_fnd6, mem_addr_fnd7; 
	unsigned long mem_len;

	result = register_chrdev(FND_MAJOR, FND_NAME, &device_fops);
    if (result < 0) {
        printk(KERN_WARNING " can't get major \n");
        return result;
    }

    mem_addr_fnd0 = FPGA_FND_CS0;
    mem_addr_fnd1 = FPGA_FND_CS1;
    mem_addr_fnd2 = FPGA_FND_CS2;
    mem_addr_fnd3 = FPGA_FND_CS3;
    mem_addr_fnd4 = FPGA_FND_CS4;
    mem_addr_fnd5 = FPGA_FND_CS5;
    mem_addr_fnd6 = FPGA_FND_CS6;
    mem_addr_fnd7 = FPGA_FND_CS7;
    mem_len  =  0x1000;

     mem_fnd_cs0 = ioremap_nocache ( mem_addr_fnd0, mem_len);
        if( !mem_fnd_cs0) {
                printk("Error mapping fnd0 memery");
                return -EBUSY;
        }

     mem_fnd_cs1 = ioremap_nocache ( mem_addr_fnd1, mem_len);
        if( !mem_fnd_cs1) {
                printk("Error mapping fnd1 memery");
                return -EBUSY;
        }

     mem_fnd_cs2 = ioremap_nocache ( mem_addr_fnd2, mem_len);
        if( !mem_fnd_cs2) {
                printk("Error mapping fnd2 memery");
                return -EBUSY;
        }

     mem_fnd_cs3 = ioremap_nocache ( mem_addr_fnd3, mem_len);
        if( !mem_fnd_cs3) {
                printk("Error mapping fnd3 memery");
                return -EBUSY;
        }

     mem_fnd_cs4 = ioremap_nocache ( mem_addr_fnd4, mem_len);
        if( !mem_fnd_cs4) {
                printk("Error mapping fnd4 memery");
                return -EBUSY;
        }

     mem_fnd_cs5 = ioremap_nocache ( mem_addr_fnd5, mem_len);
        if( !mem_fnd_cs5) {
                printk("Error mapping fnd5 memery");
                return -EBUSY;
        }

     mem_fnd_cs6 = ioremap_nocache ( mem_addr_fnd6, mem_len);
        if( !mem_fnd_cs6) {
                printk("Error mapping fnd6 memery");
                return -EBUSY;
        }

     mem_fnd_cs7 = ioremap_nocache ( mem_addr_fnd7, mem_len);
        if( !mem_fnd_cs7) {
                printk("Error mapping fnd7 memery");
                return -EBUSY;
        }
	printk("X-Hyper 270TKU %s MAJOR %d\n", FND_NAME, FND_MAJOR);
	return 0;
}

static void fnd_exit(void)
{
	fnd_clear();
	unregister_chrdev(FND_MAJOR, FND_NAME);
	iounmap(mem_fnd_cs0);
	iounmap(mem_fnd_cs1);
	iounmap(mem_fnd_cs2);
	iounmap(mem_fnd_cs3);
	iounmap(mem_fnd_cs4);
	iounmap(mem_fnd_cs5);
	iounmap(mem_fnd_cs6);
	iounmap(mem_fnd_cs7);
	printk("%s unregisterd.\n", FND_NAME);
}

module_init(fnd_init);
module_exit(fnd_exit);

/* EOF */
