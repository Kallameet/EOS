/*
Copyright (C) 2007 Rainer Findenig
rainer.findenig@fh-hagenberg.at

Copyright (C) 2005 Rainer Findenig, Robert Priewasser
rainer.findenig@fh-hagenberg.at
r.priewasser@fh-hagenberg.at

This Software is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published
by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

This Software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this software; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef GPIO_DEFS_H
#define GPIO_DEFS_H

#include <linux/cdev.h>
#include <asm/semaphore.h>

/* information and settings concerning our driver */
#define DRV_NAME			"pxa27x_gpio"
#define DRV_VERSION			"0.01"
#define DRV_LONGNAME		"PXA27x GPIO Driver"

#define DEBUGLEVEL			1000

#define GPIO_FIRST_MINOR	0

#define GPIO_BASE			0x40E00000

typedef volatile struct {
    u32 gplr0;		// Level Reg. Bank 0
    u32 gplr1;		// Level Reg. Bank 1
    u32 gplr2;		// Level Reg. Bank 2
    u32 gpdr0;		// Direction Reg. Bank 0
    u32 gpdr1;		// Direction Reg. Bank 1
    u32 gpdr2;		// Direction Reg. Bank 2
    u32 gpsr0;		// Output Set Reg. Bank 0
    u32 gpsr1;		// Output Set Reg. Bank 1
    u32 gpsr2;		// Output Set Reg. Bank 2
    u32 gpcr0;		// Output Clr Reg. Bank 0
    u32 gpcr1;		// Output Clr Reg. Bank 1
    u32 gpcr2;		// Output Clr Reg. Bank 2
    u32 grer0;		// Ris. Edge Detect Enable Reg. Bank 0
    u32 grer1;		// Ris. Edge Detect Enable Reg. Bank 1
    u32 grer2;		// Ris. Edge Detect Enable Reg. Bank 2
    u32 gfer0;		// Fal. Edge Detect Enable Reg. Bank 0
    u32 gfer1;		// Fal. Edge Detect Enable Reg. Bank 1
    u32 gfer2;		// Fal. Edge Detect Enable Reg. Bank 2
    u32 gedr0;		// Edge Detect Status Reg. Bank 0
    u32 gedr1;		// Edge Detect Status Reg. Bank 1
    u32 gedr2;		// Edge Detect Status Reg. Bank 2
    u32 gafr0_l;	// Alt. Function Select Reg.[  0:15 ]
    u32 gafr0_u;	// Alt. Function Select Reg.[ 16:31 ]
    u32 gafr1_l;	// Alt. Function Select Reg.[ 32:47 ]
    u32 gafr1_u;	// Alt. Function Select Reg.[ 48:63 ]
    u32 gafr2_l;	// Alt. Function Select Reg.[ 64:79 ]
    u32 gafr2_u;	// Alt. Function Select Reg.[ 80:95 ]
    u32 gafr3_l;	// Alt. Function Select Reg.[ 96:111]
    u32 gafr3_u;	// Alt. Function Select Reg.[112:120]
    u32 res1[35];
    u32 gplr3;		// Level Detect Reg. Bank 3
    u32 res2[2];
    u32 gpdr3;		// Data Direction Reg. Bank 3
    u32 res3[2];
    u32 gpsr3;		// Pin Output Set Reg. Bank 3
    u32 res4[2];
    u32 gpcr3;		// Pin Output Clr Reg. Bank 3
    u32 res5[2];
    u32 grer3;		// Ris. Edge Detect Enable Reg. Bank 3
    u32 res6[2];
    u32 gfer3;		// Fal. Edge Detect Enable Reg. Bank 3
    u32 res7[2];
    u32 gedr3;		// Edge Detect Status Reg. Bank 3
} GpioRegs;

struct gpio_dev_t {
	GpioRegs 			*vaddr;
	dev_t 				first;
	struct cdev			cdev;
	struct semaphore 	sem;
};

/* stuff we're going to use later on. you should not have to change anything
 * below here*/
#define PFX                     DRV_NAME ": "
#define DRV_RELEASE_DATE        __DATE__ " " __TIME__

/* misc helpdul macros */
#define DBG_PRINT_REG(reg) \
	printk(KERN_ALERT PFX "REG DBG: " #reg ": %08x\n", ioread32(&(reg)))

#define io_or32(reg, val) 	iowrite32(ioread32(&(reg)) | (val), &(reg))
#define io_and32(reg, val) 	iowrite32(ioread32(&(reg)) & (val), &(reg))

#endif /* GPIO_DEFS_H */
