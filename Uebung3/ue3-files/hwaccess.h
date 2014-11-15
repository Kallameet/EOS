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

#ifndef HWACCESS_H
#define HWACCESS_H

#include "gpio_defs.h"

void initLeds(GpioRegs *_pGPIORegs);
void setLeds(GpioRegs *_pGPIORegs, unsigned char _v);
void initSwitches(GpioRegs *_pGPIORegs);
unsigned char readSwitches(GpioRegs *_pGPIORegs);

#endif /* HWACCESS_H */
