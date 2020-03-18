#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>  /* dev_t declaration */
#include <linux/kdev_t.h> /* MAJOR, MINOR, MKDEV macro */
#include <linux/fs.h>     /* alloc_chrdev_region and unregister_chrdev_region function prototype */


/* Global variable to store device major and minor number */
static dev_t scull_num; 


/* Driver initialization function */
static int __init scull_init(void)
{
    int ret = 0;
    printk(KERN_INFO "Namaskar: scull device driver registered\n");

    /* Allocate a single character device number with minor number zero */
    if ( (ret = alloc_chrdev_region(&scull_num, 0, 1, "scull_char_device")) < 0)
    {
        printk (KERN_INFO "Failed to register character devce number\n");
        return ret;
    }

    printk (KERN_INFO "Major and Mnor number: %d %d\n",MAJOR(scull_num), MINOR(scull_num));

    return 0;
}


/* Driver de-initialization function */
static void __exit scull_exit(void)
{
    unregister_chrdev_region(scull_num, 1);
    printk(KERN_INFO "Alvida: scull device driver unregistered\n");
}

module_init(scull_init);
module_exit(scull_exit);

MODULE_AUTHOR("SUMIT AGARWAL");
MODULE_DESCRIPTION("CHARACTER DEVICE DRIVER");
MODULE_LICENSE("GPL");


/*
 * Creating device file name and connecting it to the device number 
 * sudo mknod /dev/<file_name> c <major> <minor>
 */