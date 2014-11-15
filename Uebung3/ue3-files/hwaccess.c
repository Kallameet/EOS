/*
Copyright (C) 2007 Rainer Findenig
rainer.findenig@fh-hagenberg.at

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

#include <asm/io.h>
#include <asm/arch/colibri.h>

#include "hwaccess.h"

/******************************************************************************/
/******************************************************************************/

void initLeds(GpioRegs *_pGPIORegs) {
	setLeds(_pGPIORegs, 0x00);
	io_or32(_pGPIORegs->gpdr1, 1<<3); // set GPIO35 as output
	io_or32(_pGPIORegs->gpdr1, 1<<4); // set GPIO36 as output
	io_or32(_pGPIORegs->gpdr1, 1<<5); // set GPIO37 as output
	io_or32(_pGPIORegs->gpdr2, 1<<15); // set GPIO79 as output

	// set the alternate functions to function 0
	io_and32(_pGPIORegs->gafr1_l, ~(1<<7 | 1<<6)); // GPIO35
	io_and32(_pGPIORegs->gafr1_l, ~(1<<9 | 1<<8)); // GPIO36
	io_and32(_pGPIORegs->gafr1_l, ~(1<<11 | 1<<10)); // GPIO37
	io_and32(_pGPIORegs->gafr2_l, ~(1<<31 | 1<<30)); // GPIO79
}

/******************************************************************************/
/******************************************************************************/

void setLeds(GpioRegs *_pGPIORegs, unsigned char _v) {
	// GPIO35
	if (_v & 0x01)
		io_or32(_pGPIORegs->gpcr1, 1<<3);
	else
		io_or32(_pGPIORegs->gpsr1, 1<<3);

	// GPIO37
	if (_v & 0x02)
		io_or32(_pGPIORegs->gpcr1, 1<<5);
	else
		io_or32(_pGPIORegs->gpsr1, 1<<5);

	// GPIO36
	if (_v & 0x04)
		io_or32(_pGPIORegs->gpcr1, 1<<4);
	else
		io_or32(_pGPIORegs->gpsr1, 1<<4);

	// GPIO79
	if (_v & 0x08)
		io_or32(_pGPIORegs->gpcr2, 1<<15);
	else
		io_or32(_pGPIORegs->gpsr2, 1<<15);
}

/******************************************************************************/
/******************************************************************************/

void initSwitches(GpioRegs *_pGPIORegs) {
        // set pins as input
	io_and32(_pGPIORegs->gpdr0, ~(1<<15)); // set GPIO15 as input
        io_and32(_pGPIORegs->gpdr2, ~(1<<16)); // set GPIO80 as input

        // set the alternate functions to function 0
        io_and32(_pGPIORegs->gafr0_l, ~(1<<31 | 1<<30)); // GPIO15
        io_and32(_pGPIORegs->gafr2_u, ~(1<<1  | 1<<0 )); // GPIO80
}

/******************************************************************************/
/******************************************************************************/

unsigned char readSwitches(GpioRegs *_pGPIORegs) {
	unsigned char ret = 0;

	if (ioread32(&_pGPIORegs->gplr0) & (1<<15)) // GPIO15
		ret |= 0x01;
	if (ioread32(&_pGPIORegs->gplr2) & (1<<16)) // GPIO80
		ret |= 0x02;
	if (ioread32(&_pGPIORegs->gplr1) & (1<<20)) // GPIO52
		ret |= 0x04;
	if (ioread32(&_pGPIORegs->gplr0) & (1<<19)) // GPIO19
		ret |= 0x08;

	return ret;
}

