#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/printk.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uaccess.h> //changed to linux/ for newer kernels!
#include <linux/string.h>

#define DEVICE_NAME "comp3438_helloworld_device_name" 
#define N_D 1 //number of devices
#define S_N 1 //starting index for the minor number

static char msg[] = "Hello World!!!";
static char in_msg[256]={0}; //change 2 add char inmsg
static int major;
static dev_t devno;
static struct cdev dev_helloworld;

static int zili_demo_char1_open(struct inode *inode, struct file *fp){
  printk("Device " DEVICE_NAME " open.\n");
  return 0;
}

static ssize_t zili_demo_char1_read (struct file *fp, char *buf, size_t count, loff_t *position){
  int num;
  int ret;
  if (count < strlen(msg))
    num = count;
  else
    num = strlen(msg);
  ret = copy_to_user(buf, msg, num);
  if (ret < 0){
    printk("Fail to copy data from the kernel space to the user space!\n");
    return ret;
  }
  return num;
}

//change 1, add const to char
static ssize_t write_char(struct file *fp, const char *buf, size_t count, loff_t *position){

	//change 3, write all this lmao
	if (count > 0){
	
	if (copy_from_user(in_msg, buf, 256))
		return -EFAULT;
	}
	printk(KERN_INFO "ECHO ECHO: %s", in_msg);
	return count;
}
static int zili_demo_char1_release(struct inode* inode, struct file *fp){
  printk("Device " DEVICE_NAME " released.\n");
  return 0;
}


static struct file_operations zili_demo_fops = {
  owner: THIS_MODULE,
  open: zili_demo_char1_open,
  read: zili_demo_char1_read,
  write: write_char,
  release: zili_demo_char1_release
};

static int __init zili_demo_char1_init(void){
  int ret;
  ret = alloc_chrdev_region(&devno, S_N, N_D, DEVICE_NAME);
  if (ret < 0){
    printk("Device " DEVICE_NAME " cannot get major number.\n");
    return ret;
  }
  major = MAJOR(devno);
  printk("Device " DEVICE_NAME " initialized with major number = %d.\n", major);
  //the actual registration
  cdev_init(&dev_helloworld, &zili_demo_fops);
  dev_helloworld.owner = THIS_MODULE;
  //dev_helloworld.ops = &zili_demo_fops;
  ret = cdev_add(&dev_helloworld, devno, N_D);
  
  if (ret){
    printk("Device " DEVICE_NAME " register fail.\n");
    return ret;
  }
  return 0;
}

static void __exit zili_demo_char1_exit(void){
  cdev_del(&dev_helloworld);
  unregister_chrdev_region(devno, N_D);
  printk("Device " DEVICE_NAME " unloaded.\n");
}


module_init(zili_demo_char1_init);
module_exit(zili_demo_char1_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dr. Zili Shao");
MODULE_DESCRIPTION("Char hello world driver");
