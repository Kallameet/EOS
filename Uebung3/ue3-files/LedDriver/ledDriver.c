#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <linux/cdev.h>
#include "gpio_defs.h"

#define DEVICE_NAME "leds"
#define MINOR_NUMBER 0
#define NUMBER_OF_DEVICES 1

MODULE_LICENSE("Dual BSD/GPL");

struct file_operations ledDriver_fops = {
    read:     ledDriver_read,
    write:    ledDriver_write    
};

gpio_dev_t gpio_dev;
cdev device;
dev_t dev;

static int ledDriver_init(void)
{
		if (request_mem_region(GPIO_BASE, sizeof(GpioRegs), DEVICE_NAME) == NULL)
		{
		    printk(KERN_ALERT "Error requesting memory\n");			
				return -1;		
		}		
		gpio_dev.vaddr = (GpioRegs*) ioremap_nocache(GPIO_BASE, sizeof(GpioRegs));
		
		if (alloc_chrdev_region(&dev, MINOR_NUMBER, NUMBER_OF_DEVICES, DEVICE_NAME))
		{
				goto fail_alloc_chrdev;
		}

		cdev_init(&device, &ledDriver_fops);
		cdev.ops = &ledDriver_fops;

		if (cdev_add(&device, dev, NUMBER_OF_DEVICES))
		{
				goto fail_cdev_add;
		}

    printk(KERN_ALERT "Hello, world\n");
    return 0;
}

static void ledDriver_exit(void)
{
		cdev_del(&device);	

fail_cdev_add:
		unregister_chrdev_region(dev, NUMBER_OF_DEVICES);

fail_alloc_chrdev:
		iounmap(gpio_dev.vaddr);
		release_mem_region(GPIO_BASE, sizeof(GpioRegs));
    printk(KERN_ALERT "Goodbye, cruel world\n");
}

static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
			   char *buffer,	/* buffer to fill with data */
			   size_t length,	/* length of the buffer     */
			   loff_t * offset)
{
	return 0;
}


static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t * offset)
{	
	return 0;
}


module_init(ledDriver_init);
module_exit(ledDriver_exit);

