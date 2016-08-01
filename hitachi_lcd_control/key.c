/*
 * XHYPER255_TKU3
 * key.c
 * 2004-07-01   <newboder@hybus.net>
 * 
 */
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/types.h>

#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/bitops.h>
#include <asm/hardware.h>
#include <asm/arch/pxa-regs.h>
#include <asm/io.h>
#include <asm/ioctls.h>

#define KEY_MAJOR           233 
#define KEY_NAME            "key"
#define SCAN_NUM	    4

/*
    FPGA KEY

   0x16400000 //FPGA_KEY_OUT# 
   KEYOUT_D[0:3] : X_D[0:3]

   0x16800000 //FPGA_KEY_IN# 
   KEYIN_D[0:3] : X_D[0:3]
*/
#define FPGA_KEY_OUT			(0x11D00000)
#define FPGA_KEY_IN			(0x11E00000)

// (*((volatile unsigned char *)(mem_base))) 

#define KEYOUT    		(*((volatile unsigned char *)(mem_key_out)))	
#define KEYIN   		(*((volatile unsigned char *)(mem_key_in)))	

#define dbg(x...)	//	printk(x) 

static void *mem_key_out, *mem_key_in;
static struct timer_list key_timer_str;   // timer 구조체를 정의 //
static void key_timer_function( unsigned long ptr);
static void init_add_timer(void);
static void remove_timer(void);

static char key_data;
static int n = 0;

void Print_Key(int out , int in)
{
    switch(out)
	{
		case 1:                
				if(in == 1)	{ dbg("key 4 is pressed\n");	key_data = 4;}
   				else if(in == 2){ dbg("key 8 is pressed\n");	key_data = 8;}
   				else if(in == 4){ dbg("key 12 is pressed\n");	key_data = 12;}
   				else if(in == 8){ dbg("key 16 is pressed\n");	key_data = 16;}
				break;
		case 2:                 
				if(in == 1)	{ dbg("key 3 is pressed\n");	key_data = 3;}
				else if(in == 2){ dbg("key 7 is pressed\n");	key_data = 7;}
				else if(in == 4){ dbg("key 11 is pressed\n");	key_data = 11;}
                		else if(in == 8){ dbg("key 15 is pressed\n");	key_data = 15;}
				break;
		case 3:                  
				if(in == 1)	{ dbg("key 2 is pressed\n");	key_data = 2;}
				else if(in == 2){ dbg("key 6 is pressed\n");	key_data = 6;}
				else if(in == 4){ dbg("key 10 is pressed\n");	key_data = 10;}
				else if(in == 8){ dbg("key 14 is pressed\n");	key_data = 14;}
				break;
		case 4:                   
				if(in == 1)	{ dbg("key 1 is pressed\n");	key_data = 1;}
				else if(in == 2){ dbg("key 5 is pressed\n");	key_data = 5;}
				else if(in == 4){ dbg("key 9 is pressed\n");	key_data = 9;}
				else if(in == 8){ dbg("key 13 is pressed\n");	key_data = 13;}
 				break;               
	}  
}         

void key_timer_function(unsigned long ptr)
{
	int in, out;

	KEYOUT = ((1 << (ptr-1)) & (0x0f));
//	KEYOUT = 1;

	out = ptr;
	in = KEYIN;

//	printk("KEYIN = %x\n", in & 0xF);
	if(in != 0)
	{
		Print_Key(out, in);
//		mdelay(200);
	}

    init_add_timer();
}

void init_add_timer(void)
{   
    ++n;
    if( n > SCAN_NUM)
        n = 1;

    init_timer(&key_timer_str); // timer  pointer 
    
    key_timer_str.function =&key_timer_function ;
    key_timer_str.data = (unsigned long)n;
    key_timer_str.expires = jiffies + (HZ/100);

    add_timer(&key_timer_str);
}

void remove_timer(void)
{
    del_timer(&key_timer_str);
}  

int key_open (struct inode *inode, struct file *filp)
{
   // MOD_INC_USE_COUNT;
    return 0;          /* success */
}

int key_release (struct inode *inode, struct file *filp)
{
   // MOD_DEC_USE_COUNT;
    return 0;
}

ssize_t key_read (struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	copy_to_user(buf, &key_data, sizeof(key_data));
	key_data = 0;
	return 0;
}

ssize_t key_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	return 0;
}

int key_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch(cmd)
    {
		case FIONREAD:  
        {
			copy_to_user((void *)arg, &key_data, sizeof(key_data));
			key_data = 0;
			break;
        }
        default: break;
    }
    return 0;
}
		
struct file_operations key_fops = {
    .read	=       key_read,
    .write	=       key_write,
    .open	=       key_open,
    .ioctl	=	key_ioctl,
    .release	=	key_release,
};

static int key_init(void)
{
	int result;
	unsigned long mem_addr_out, mem_addr_in, mem_len;		

	result = register_chrdev(KEY_MAJOR,  "KEY", &key_fops);
    	if (result < 0) {
        	printk(KERN_WARNING " can't get major \n");
        	return result;
    	}
	
	mem_addr_out =  FPGA_KEY_OUT;
	mem_addr_in  =  FPGA_KEY_IN;
	mem_len	= 0x1000;
	mem_key_out = ioremap_nocache (mem_addr_out, mem_len);
	if(!mem_key_out) {
		printk(" Error mapping key_out memory\n");
		return -EBUSY;
	}

	mem_key_in = ioremap_nocache (mem_addr_in, mem_len);
	if(!mem_key_in) {
		printk(" Error mapping key_in memory\n");
		return -EBUSY;
	}

	printk("X-Hyper 270TKU %s MAJOR %d\n", KEY_NAME, KEY_MAJOR);

	init_add_timer();
	return 0;
}

static void key_exit(void)
{
	remove_timer();
	unregister_chrdev(KEY_MAJOR, KEY_NAME);
}

module_init(key_init);
module_exit(key_exit);

/* EOF */
