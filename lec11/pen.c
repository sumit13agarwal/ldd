#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/usb.h>


static int pen_probe(struct usb_interface *inf, const struct usb_device_id *id)
{
	printk("KERN_INFO Pen drive (%04X:%04X) plugged\n", id->idVendor, id->idProduct);
	return 0;
}

static void pen_disconnect(struct usb_interface *inf)
{
	printk("KERN_INFO Pen drive un-plugged\n");	
}

static struct usb_device_id pen_table[] = {
	{USB_DEVICE(0x0781, 0x5567)},
	{}
};

static struct usb_driver pen_driver = {
	.probe = pen_probe,
	.disconnect = pen_disconnect,
	.id_table = pen_table,
	.name = "PEN_DRIVER"
};

static int __init pen_init(void)
{
	usb_register(&pen_driver);
	return 0;
}

static void __exit pen_exit(void)
{
	usb_deregister(&pen_driver);
}


module_init(pen_init);
module_exit(pen_exit);


MODULE_AUTHOR("SUMIT AGARWAL");
MODULE_DESCRIPTION("SAMPLE USB DRIVER");
MODULE_LICENSE("GPL");
