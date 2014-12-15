#ifndef LEDSWITCH_H_
#define LEDSWITCH_H_

#include "gpio.h"

bool initLeds(GPIOREG * pGpioReg);
void setLeds(GPIOREG * pGpioReg, char data);

/* ToDo: implement missing functions as defined !!! */
bool initSwitches(GPIOREG * pGpioReg);
char readSwitches(GPIOREG * pGpioReg);

bool initPushButtons(GPIOREG * pGpioReg);
char readPushButtons(GPIOREG * pGpioReg);
/****************************************************/


#endif