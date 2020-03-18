#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>     /* cdev structure */
#include <linux/types.h>    /* dev_t strcture*/
#include <linux/kdev_t.h>   /* MAJOR, MINOR and MKDEV macros */
#include <linux/fs.h>       /* alloc_chrdev_region() and unregister_chrdev_region() function prototypes, file_operations structure */
#include <linux/device.h>   /* class_create() and device_create() function prototype */
#include <linux/uaccess.h>  /* copy_to_user and copy_from_user */

static dev_t scull_num;
static struct cdev scull_cdev;
static struct class *cl;

static char last = 0;


static int scull_open(struct inode *i, struct file *fp)
{
    printk (KERN_INFO "SCULL: OPEN\n");
    return 0;
}

static int scull_close(struct inode *i, struct file *fp)
{
    printk (KERN_INFO "SCULL: CLOSE\n");
    return 0;
}

static ssize_t scull_read(struct file *fp, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "SCULL: READ\n");

    if (*off == 0)2
    {
        if (copy_to_user((void *)&buf[0], (void *)&last, 1) != 0)
        {
            return -EFAULT;
        }
        else
        {
            (*off)++;
            return 1;
        }
    }
    else
    {
        return 0;
    }
    
}

static ssize_t scull_write(struct file *fp, const char *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "SCULL: WRITE\n");

    if (copy_from_user(&last, &buf[len - 1], 1) != 0)
        return -EFAULT;

    return len;
}


static struct file_operations scull_fops =
{
    .owner = THIS_MODULE,
    .read = scull_read,
    .write = scull_write,
    .open = scull_open,
    .release = scull_close
};


static int __init scull_init(void)
{
    int ret = 0;
    struct device *dev_ret = NULL;

    printk(KERN_INFO "Namaskar: Scull device registered\n");

    /* Allocate a range of character device number */ 
    if ( (ret = alloc_chrdev_region(&scull_num, 0, 1, "scull_device")) < 0 )
    {
        return ret;
    }

    if ( IS_ERR(cl = class_create(THIS_MODULE, "chrdev")) )
    {
        unregister_chrdev_region(scull_num, 1);
        return PTR_ERR(cl);

    }

    if ( IS_ERR(dev_ret = device_create(cl, NULL, scull_num, NULL, "scull_device")))
    {
        class_destroy(cl);
        unregister_chrdev_region(scull_num, 1);
        return PTR_ERR(dev_ret);
    }

    cdev_init(&scull_cdev, &scull_fops);

    if ( (ret = cdev_add(&scull_cdev, scull_num, 1)) < 0 )
    {
        device_destroy(cl, scull_num);
        class_destroy(cl);
        unregister_chrdev_region(scull_num, 1);

        return ret;
    }

    return ret;
}

static void __exit scull_exit(void)
{
    device_destroy(cl, scull_num);
    class_destroy(cl);
    unregister_chrdev_region(scull_num, 1);

    printk(KERN_INFO "Alvida: scull device unregistered\n");
}

module_init(scull_init);
module_exit(scull_exit);

MODULE_AUTHOR("SUMIT AGARWAL");
MODULE_DESCRIPTION("SCULL DEVICE DRIVER");
MODULE_LICENSE("GPL");