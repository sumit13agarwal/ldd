#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>


static int __init scull_init(void)
{
    printk(KERN_INFO "Namaskar: scull device driver registered\n");
    return 0;
}

static void __exit scull_exit(void)
{
    printk(KERN_INFO "Alvida: scull device driver unregistered\n");
}

module_init(scull_init);
module_exit(scull_exit);

MODULE_AUTHOR("SUMIT AGARWAL");
MODULE_DESCRIPTION("CHARACTER DEVICE DRIVER");
MODULE_LICENSE("GPL");