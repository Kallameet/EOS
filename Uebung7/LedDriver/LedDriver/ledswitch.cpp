#include "stdafx.h"
#include <windows.h>
#include "ledswitch.h"

bool initLeds(GPIOREG * pGpioReg){
	// check if pointer is valid
	if(!pGpioReg){
		return false;
	}

	// set port direction and function
	pGpioReg->gafr2_l &= ~(1 << 31 | 1 << 30); // set alternate function 0 for GPIO79
	pGpioReg->gafr1_l &= ~(1 << 9 | 1 << 8);   // set alternate function 0 for GPIO36
	pGpioReg->gafr1_l &= ~(1 << 11 | 1 << 10); // set alternate function 0 for GPIO37
	pGpioReg->gafr1_l &= ~(1 << 7 | 1 << 6);   // set alternate function 0 for GPIO35

	pGpioReg->gpdr1   |= (1 << 3);   //set GPIO35 as output
	pGpioReg->gpdr1   |= (1 << 4);   //set GPIO36 as output
	pGpioReg->gpdr1   |= (1 << 5);   //set GPIO37 as output
	pGpioReg->gpdr2   |= (1 << 15);  //set GPIO79 as output

	// reset leds
	setLeds(pGpioReg, 0x00);

	return true;
}

void setLeds(GPIOREG * pGpioReg, char data){
	// mask several leds

	if(data & 0x01){
		pGpioReg->gpsr1 = (1 << 3); // reset GPIO35
	}else{
		pGpioReg->gpcr1 = (1 << 3); // set GPIO35
	}

	if(data & 0x02){
		pGpioReg->gpsr1 = (1 << 5); // reset GPIO37
	}else{
		pGpioReg->gpcr1 = (1 << 5); // set GPIO37
	}

	if(data & 0x04){
		pGpioReg->gpsr1 = (1 << 4); // reset GPIO36
	}else{
		pGpioReg->gpcr1 = (1 << 4); // set GPIO36
	}

	if(data & 0x08){
		pGpioReg->gpsr2 = (1 << 15); // reset GPIO79
	}else{
		pGpioReg->gpcr2 = (1 << 15); // set GPIO79
	}
}

bool initPushButtons(GPIOREG * pGpioReg){
	// check if pointer is valid
	if(!pGpioReg){
		return false;
	}

	pGpioReg->gafr0_u &= ~(1 << 19 | 1 << 18); //set alternate function 0 for GPIO25 - push button 1
	pGpioReg->gafr0_u &= ~(1 << 21 | 1 << 20); //set alternate function 0 for GPIO26 - push button 2
	pGpioReg->gafr0_u &= ~(1 << 15 | 1 << 14); //set alternate function 0 for GPIO23 - push button 3
	pGpioReg->gafr0_u &= ~(1 << 17 | 1 << 16); //set alternate function 0 for GPIO24 - push button 4

	pGpioReg->gpdr0 &= ~ (1 << 25); //set GPIO25 as input
	pGpioReg->gpdr0 &= ~ (1 << 26); //set GPIO26 as input
	pGpioReg->gpdr0 &= ~ (1 << 23); //set GPIO23 as input
	pGpioReg->gpdr0 &= ~ (1 << 24); //set GPIO24 as input

	return true;
}

char readPushButtons(GPIOREG * pGpioReg){
	char ret = 0;

	if(pGpioReg->gplr0 & (1 << 25)){  // read GPIO25
		ret |= 0x01;
	}

	if(pGpioReg->gplr0 & (1 << 26)){  // read GPIO26
		ret |= 0x02;
	}

	if(pGpioReg->gplr0 & (1 << 23)){  // read GPIO23
		ret |= 0x04;
	}

	if(pGpioReg->gplr0 & (1 << 24)){  // read GPIO24
		ret |= 0x08;
	}

	return ret;
}

bool initSwitches(GPIOREG * pGpioReg) {
	// check if pointer is valid
	if(!pGpioReg){
		return false;
	}

	// set the alternate functions to function 0
	pGpioReg->gafr0_l &= ~(1<<31 | 1<<30); // GPIO15
	pGpioReg->gafr2_u &= ~(1<<1  | 1<<0 ); // GPIO80

	pGpioReg->gpdr0 &= ~(1<<15); // set GPIO15 as input
	pGpioReg->gpdr2 &= ~(1<<16); // set GPIO80 as input

	return true;
}

/******************************************************************************/
/******************************************************************************/

char readSwitches(GPIOREG * pGpioReg) {
	unsigned char ret = 0;

	if (pGpioReg->gplr0 & (1<<15)) // GPIO15
		ret |= 0x01;
	if (pGpioReg->gplr2 & (1<<16)) // GPIO80
		ret |= 0x02;
	if (pGpioReg->gplr1 & (1<<20)) // GPIO52
		ret |= 0x04;
	if (pGpioReg->gplr0 & (1<<19)) // GPIO19
		ret |= 0x08;

	return ret;
}