#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/io.h>

#define VRAM_START 0xc0000
#define VRAM_END   0xc7fff
#define VRAM_SIZE  (VRAM_END - VRAM_START + 1) 

static void __iomem *vram;

static struct cdev scull_cdev;
static dev_t scull_num;
static struct class *cl;



static int scull_open(struct inode *i, struct file *f)
{
    printk (KERN_INFO "SCULL:OPEN\n");
    return 0;
}

static int scull_close(struct inode *i, struct file *f)
{
    printk (KERN_INFO "SCULL:CLOSE\n");
    return 0;
}

static ssize_t scull_read(struct file *f, __user char *buf, size_t len, loff_t *off)
{
    int i;
    u8 byte; 

    printk (KERN_INFO "SCULL:READ (%d)\n", len);

    if (*off >= VRAM_SIZE)
    {
    	printk (KERN_INFO "SCULL:VRAM SIZE greater than offset\n");
        return 0;
    }

    if ((*off + len) > VRAM_SIZE)
    {
    	printk (KERN_INFO "SCULL: len is adjusted\n");
        len = VRAM_SIZE - *off;
    }

    for (i = 0; i < len; i++)
    {
        byte = ioread8((u8 *)vram + *off + i);

        if (copy_to_user(&buf[i], &byte, 1) !=0 )
        {
    	    printk (KERN_INFO "SCULL: copy to user failed\n");
            return -EFAULT;
        }
    }

    *off += len;

    return len;

}

static ssize_t scull_write(struct file *f, const char *buf, size_t len, loff_t *off)
{
    int i;
    u8 byte;

    printk (KERN_INFO "SCULL:WRITE (%d)\n", len);

    if (*off >= VRAM_SIZE)
    {
    	printk (KERN_INFO "SCULL:VRAM SIZE greater than offset\n");
        return 0;
    }

    if (*off + len > VRAM_SIZE)
    {
    	printk (KERN_INFO "SCULL: len is adjusted\n");
        len = VRAM_SIZE - *off;
    }

    for (i = 0; i < len; i++)
    {
        if (copy_from_user(&byte, &buf[i], 1) != 0)
	{
    	    printk (KERN_INFO "SCULL: copy from user failed\n");
            return -EFAULT;
	}
    	printk (KERN_INFO "SCULL: copy from user successfull\n");
        iowrite8(byte, vram + *off + i);
    }

    *off += len;

    return len;
}

static struct file_operations scull_fops = {

    .owner = THIS_MODULE,
    .read = scull_read,
    .write = scull_write,
    .open = scull_open,
    .release = scull_close,
}; 

static int __init scull_init(void)
{
    int ret;

    struct class *cl;

    struct device *dev_ret;

    printk(KERN_INFO "Namate: Scull device driver registered\n");

    if ( (vram = ioremap(VRAM_START, VRAM_SIZE)) == NULL)
    {
        printk(KERN_INFO "Mapping Video RAM bus address to device address Failed");
    }

    if ((ret = alloc_chrdev_region(&scull_num, 0, 1, "vram")) < 0)
    {
        return ret;
    }

    if (IS_ERR(cl = class_create(THIS_MODULE, "chardev")))
    {
        unregister_chrdev_region(scull_num, 1);
        return PTR_ERR(cl);
    }

    if (IS_ERR(dev_ret = device_create(cl, NULL, scull_num, NULL, "vram")))
    {
        class_destroy(cl);
        unregister_chrdev_region(scull_num, 1);
        return PTR_ERR(dev_ret);
    }

    cdev_init(&scull_cdev, &scull_fops);

    if ((ret = cdev_add(&scull_cdev, scull_num, 1)) < 0)
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
    printk(KERN_INFO "Alvida: Scull device unregistered\n");
    cdev_del(&scull_cdev);
    device_destroy(cl, scull_num);
    class_destroy(cl);
    unregister_chrdev_region(scull_num, 1);
    iounmap(vram);
}


module_init(scull_init);
module_exit(scull_exit);

MODULE_AUTHOR("SUMIT AGARWAL");
MODULE_DESCRIPTION("SCULL on VRAM");
MODULE_LICENSE("GPL");

