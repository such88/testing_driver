#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<asm/uaccess.h>

#include"chardev.h"
#define SUCCESS 0
#define DEVICE_NAME "char_dev"
#define BUF_LEN 100
MODULE_LICENSE("GPL");

static int dev_open = 0;
static char msg[BUF_LEN];
static char *msg_ptr;

//prototypes
/*
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *,size_t, loff_t *);
*/

static int device_open(struct inode *inode, struct file *fp)
{
 #ifdef DEBUG 
  printk(KERN_INFO "device_open (%p) \n", fp);
 #endif

  if(dev_open)
    return -EBUSY;

  dev_open++;
  msg_ptr = msg;
  try_module_get(THIS_MODULE);
  return SUCCESS;

}

static int device_release(struct inode *inode, struct file *fp)
{
  #ifdef DEBUG
    printk(KERN_INFO "device release %p\n", fp);
  #endif
    dev_open--;
    module_put(THIS_MODULE);
    return SUCCESS;

}

static ssize_t device_read(struct file *fp, char __user *buffer,
                            size_t length, loff_t *offset)
{
  int bytes_read = 0;
 #ifdef DEBUG
  printk(KERN_INFO "device read(%p, %p, %d)\n",fp,buffer,length);
 #endif

  if(*msg_ptr == 0)
    return 0;
  while(length && *msg_ptr)
  {
  	printk(KERN_INFO "Inside the loop\n");
    put_user(*(msg_ptr++), buffer++);
    length--;
    bytes_read++;
  }
 #ifdef DEBUG
  printk(KERN_INFO "Read %d bytes, %d left\n", bytes_read, length);
 #endif

  return bytes_read;
  
}

static ssize_t device_write(struct file *fp, const char __user * buffer, size_t length, loff_t *offset)
{
  int i;
 #ifdef DEBUG
  printk(KERN_INFO "device write ( %p, %s, %d)\n", fp, buffer, length);
 #endif

  for(i=0; i<length && i< BUF_LEN; i++)
  {
    get_user(msg[i], buffer+i);
//    msg_ptr = msg;
  }
  msg_ptr = msg;
  return i;
}

int device_ioctl(struct inode *inode, struct file *fp, unsigned int ioctl_num, unsigned long ioctl_param)
{
  int i;
  char *temp = "....Hi....";
  char ch = '1';

  switch(ioctl_num)
  {
    case IOCTL_SET_MSG:
      temp = (char *)ioctl_param;
      get_user(ch, temp);
      for(i=0; ch && i<BUF_LEN ; i++, temp++)
      {
        get_user(ch,temp);
      }
      device_write(fp, (char *) ioctl_param,i,0);
      break;
    case IOCTL_GET_MSG:
      i = device_read(fp,(char*)ioctl_param,99,0);
      put_user('\0',(char*)ioctl_param+1);
      break;
    case IOCTL_GET_NTH_BYTE:
      return msg[ioctl_param];
      break;

  }
  return SUCCESS;
}

static struct file_operations fops = 
{
  .open = device_open,
  .read = device_read,
  .write = device_write,
  .unlocked_ioctl = device_ioctl,
  .release = device_release

};

int init_module(void)
{
  int ret_val;
  ret_val = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);

  if(ret_val<0)
  {
    printk(KERN_ALERT "%s failed with %d\n",
        "Sorry, registering the character device", ret_val);
    return ret_val;
  }
 else
 	{
	    printk(KERN_ALERT "registered the character device with major no %d", MAJOR_NUM);
	
	}	
  return 0;
}

void cleanup_module()
{
	unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
	printk(KERN_ALERT "Error: unregister_chrdev: %d\n",MAJOR_NUM);

}
