#include<linux/module.h>
#include<linux/string.h>
#include<linux/fs.h>
#include<asm/uaccess.h>
//#define MAJOR_NO 72

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LDD DEMO");
MODULE_AUTHOR("Suchnadan Dey");

static char msg[100] = {0};
static short readpos = 0;
static int times = 0;

//prototypes
static int dev_open(struct inode *, struct file *);
static int dev_rel(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *,size_t, loff_t *);

// structure containing callbacks
static struct file_operations fops = 
{
      .open = dev_open,	
      .read = dev_read,
      .write = dev_write,
      .release = dev_rel
};

//init module
int init_module(void)
{
    int t = register_chrdev(240,"myDev", &fops);

    if(t<0)
      printk(KERN_ALERT "DEVICE registration failed..\n");
    else
      printk(KERN_ALERT "DEVICE resgistrered");

    return t;
}


void cleanup_module(void)
{
	printk(KERN_ALERT "Removing devices");
	unregister_chrdev(240,"myDev");
}

static int dev_open(struct inode * in, struct file *fl)
{

  times++;
  printk(KERN_ALERT "Device opened %d times\n", times);
  return 0;
}

//called when read system call is done on the device file
//static ssize_t dev_read(struct inode *, char *, size_t, loff_t *);
static ssize_t dev_read(struct file *fp, char *buff, size_t len, loff_t *off)
{
  short count = 0;
  while(len && (msg[readpos]!=0))
  {
    put_user(msg[readpos],buff++);
    printk(KERN_INFO "%c",msg[readpos]);
    count++;
    len--;
    readpos++;
  }
  return count;
}


// called when write system call is done on the device file
//static ssize_t dev_write(struct file *, const char *,size_t, loff_t *);
static ssize_t dev_write(struct file *fp, const char *buff, size_t len, loff_t *off)
{
  short ind = len -1;
  short count  = 0;
  memset(msg, 0,100);
  readpos = 0;
  while(len > 0)
  {
    //copy the given string to driver but in reverse
    msg[count] = buff[ind--]; 
    len--;
    printk(KERN_INFO "%c",msg[count++]);
  }
  return count;
}

//called when close system call is done on the system file
static int dev_rel(struct inode *inod, struct file *fl)
{
    printk(KERN_ALERT "DEVICE closed....\n");
    return 0;
}
//module_init(init_module);
//module_exit(cleanup_module);
