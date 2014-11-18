#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <linux/cdev.h>
#include "gpio_defs.h"
#include "hwaccess.h"

#define DEVICE_NAME "leds"
#define MINOR_NUMBER 0
#define NUMBER_OF_DEVICES 1

MODULE_LICENSE("Dual BSD/GPL");


struct gpio_dev_t gpio_dev;

static ssize_t ledDriver_read(struct file *filp,	/* see include/linux/fs.h   */
	char *buffer,	/* buffer to fill with data */
	size_t length,	/* length of the buffer     */
	loff_t * offset)
{
	buffer[0] = readSwitches(gpio_dev.vaddr);
	return 1;
}

static ssize_t ledDriver_write(struct file *filp, const char *buffer, size_t length, loff_t * offset)
{
	setLeds(gpio_dev.vaddr, buff[0]);
	return 0;
}

struct file_operations ledDriver_fops = {
	read:    ledDriver_read,
	write:	 ledDriver_write
};

static int ledDriver_init(void)
{
	if (request_mem_region(GPIO_BASE, sizeof(GpioRegs), DEVICE_NAME) == NULL)
	{
		printk(KERN_ALERT "Error requesting memory\n");
		return -1;
	}
	gpio_dev.vaddr = (GpioRegs*)ioremap_nocache(GPIO_BASE, sizeof(GpioRegs));

	if (alloc_chrdev_region(&gpio_dev.first, MINOR_NUMBER, NUMBER_OF_DEVICES, DEVICE_NAME))
	{
		goto fail_alloc_chrdev;
	}

	cdev_init(&gpio_dev.cdev, &ledDriver_fops);

	if (cdev_add(&gpio_dev.cdev, gpio_dev.first, NUMBER_OF_DEVICES) < 0)
	{
		goto fail_cdev_add;
	}

	initLeds(gpio_dev.vaddr);
	initSwitches(gpio_dev.vaddr);

	return 0;

fail_cdev_add:
	unregister_chrdev_region(gpio_dev.first, NUMBER_OF_DEVICES);

fail_alloc_chrdev:
	iounmap((void*)gpio_dev.vaddr);
	release_mem_region(GPIO_BASE, sizeof(GpioRegs));

	return -1;
}

static void ledDriver_exit(void)
{
	cdev_del(&gpio_dev.cdev);

	unregister_chrdev_region(gpio_dev.first, NUMBER_OF_DEVICES);

	iounmap((void*)gpio_dev.vaddr);
	release_mem_region(GPIO_BASE, sizeof(GpioRegs));
}

module_init(ledDriver_init);
module_exit(ledDriver_exit);
