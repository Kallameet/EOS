/*************************************************************************
Source file of generic stream device driver for windows ce 6.0

*************************************************************************/

#include "stdafx.h"
#include <windows.h>						// for all that windows stuff
#include <commctrl.h>
#include "led_driver.h"						// local program includes
#include "gpio.h"
#include "mapreg.h"
#include "ledswitch.h"

// trun on retail messages
#define RETAIL_ON TRUE

// Used as a prefix string for all debug zone messages.
#define DTAG	TEXT("LEDDrv: ")

// globals
HINSTANCE hInst;							// dll instance handle

//driver instance structure
typedef struct{
	DWORD dwSize;
	int nNumOpens;
	GPIOREG * pGpioRegs;
}DRVCONTEXT, *PDRVCONTEXT;


//=======================================================================
//DLLMain - DLL initialization entry point
//
BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	hInst = (HINSTANCE)hModule;

	switch(ul_reason_for_call){
		case DLL_PROCESS_ATTACH:
			RETAILMSG(RETAIL_ON, (TEXT("LedDriver DLL_PROCESS_ATTACH \r\n")));
			// improve performance by passing on thread attach calls
			DisableThreadLibraryCalls(hInst);
			break;

		case DLL_PROCESS_DETACH:
			RETAILMSG(RETAIL_ON, (TEXT("LedDriver DLL_PROCESS_DETACH\r\n")));

			break;
	}
	return TRUE;
}

//=======================================================================
// LED_Init - Driver initialization function
//
DWORD LED_Init(LPCSTR pContext, DWORD dwBusContext){
	PDRVCONTEXT pDrv;

	RETAILMSG(RETAIL_ON, (TEXT("LED_Init++ dwContext: %x\r\n"), pContext));

	// Allocate a driver instance structure - required if we want to manage
	// more instances
	pDrv = (PDRVCONTEXT)LocalAlloc(LPTR, sizeof(DRVCONTEXT));

	if(pDrv){
		// initialize structure
		memset((PBYTE) pDrv, 0, sizeof(DRVCONTEXT));
		pDrv->dwSize = sizeof(DRVCONTEXT);

		// read registry to determine the size of the disk
		// GetConfigData((DWORD)pContext);
	}else{
		RETAILMSG(RETAIL_ON, (DTAG TEXT("LED_Init failure. Out of memory\r\n"), pContext));
	}



	RETAILMSG(RETAIL_ON, (DTAG TEXT("LED_Init-- pDrv: %x\r\n"), pDrv));

	return (DWORD)pDrv;
}

BOOL LED_Deinit(DWORD dwContext)
{
	return true;
}


//=======================================================================
// LED_Open - Called when driver opened
// Use dwAccess and dwShare flags to manage access rights
//
DWORD LED_Open(DWORD dwContext, DWORD dwAccess, DWORD dwShare){
	PDRVCONTEXT pDrv = (PDRVCONTEXT)dwContext;

	RETAILMSG(RETAIL_ON, (DTAG TEXT("LED_Open++ dwContext: %x\r\n"), dwContext));

	// Verify that the context handle is valid
	if(pDrv && (pDrv->dwSize != sizeof(DRVCONTEXT))){
		return 0;
	}

	GPIOREG* pGPIORegs = (GPIOREG*)MapRegister(GPIO_BASE);
	if (pGPIORegs)
	{
		pDrv->pGpioRegs = pGPIORegs;
	}
	else
	{
		return 0;
	}
	initLeds(pGPIORegs);
	initPushButtons(pGPIORegs);
	initSwitches(pGPIORegs);

	// Count the number of opens
	InterlockedIncrement((long *)&pDrv->nNumOpens);

	RETAILMSG(RETAIL_ON, (TEXT("LED_Open-- \r\n")));

	return (DWORD)pDrv;
}


BOOL LED_Close(DWORD dwOpen)
{
	PDRVCONTEXT pDrv = (PDRVCONTEXT)dwOpen;

	RETAILMSG(RETAIL_ON, (DTAG TEXT("LED_Close++ dwContext: %x\r\n"), dwOpen));

	// Verify that the context handle is valid
	if(pDrv && (pDrv->dwSize != sizeof(DRVCONTEXT))){
		return false;
	}

	UnMapRegister((void*)pDrv->pGpioRegs);

	// Count the number of opens
	InterlockedDecrement((long *)&pDrv->nNumOpens);

	RETAILMSG(RETAIL_ON, (TEXT("LED_Close-- \r\n")));

	return true;
}

DWORD LED_Read(DWORD dwOpen, LPVOID pBuffer, DWORD dwCount)
{
	PDRVCONTEXT pDrv = (PDRVCONTEXT)dwOpen;

	RETAILMSG(RETAIL_ON, (DTAG TEXT("LED_Read++ dwContext: %x\r\n"), dwOpen));

	// Verify that the context handle is valid
	if(pDrv && (pDrv->dwSize != sizeof(DRVCONTEXT))){
		return 0;
	}

	char* result = (char*) pBuffer;

	*result = readSwitches(pDrv->pGpioRegs);
	*result = (readPushButtons(pDrv->pGpioRegs) << 4);


	// Count the number of opens
	InterlockedDecrement((long *)&pDrv->nNumOpens);

	RETAILMSG(RETAIL_ON, (TEXT("LED_Read-- \r\n")));

	return 1;
}

DWORD LED_Write(DWORD dwOpen, LPVOID pBuffer, DWORD dwCount)
{
	PDRVCONTEXT pDrv = (PDRVCONTEXT)dwOpen;

	RETAILMSG(RETAIL_ON, (DTAG TEXT("LED_Write++ dwContext: %x\r\n"), dwOpen));

	// Verify that the context handle is valid
	if(pDrv && (pDrv->dwSize != sizeof(DRVCONTEXT))){
		return 0;
	}

	char* input = (char*) pBuffer;

	setLeds(pDrv->pGpioRegs, *input);

	// Count the number of opens
	InterlockedDecrement((long *)&pDrv->nNumOpens);

	RETAILMSG(RETAIL_ON, (TEXT("LED_Write-- \r\n")));

	return 1;
}
