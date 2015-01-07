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
#include "eos_defines.h"

// trun on retail messages
#define RETAIL_ON TRUE

// Used as a prefix string for all debug zone messages.
#define DTAG	TEXT("LEDDrv: ")

// globals
HINSTANCE hInst;							// dll instance handle
ISR_INFO isr_info;
bool KillFlag = false;

//driver instance structure
//typedef struct{
//	DWORD dwSize;
//	int nNumOpens;
//	GPIOREG * pGpioRegs;
//}DRVCONTEXT, *PDRVCONTEXT;


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

DWORD WINAPI ISTFunction(LPVOID lpParam)
{	
	isr_info.pGPIORegs->gafr2_l &= ~(1 << 31 | 1 << 30); // set alternate function 0 for GPIO79
	isr_info.pGPIORegs->gpdr2   |= (1 << 15);  //set GPIO79 as output

	RETAILMSG(RETAIL_ON, (TEXT("ISTFunction start \r\n")));
	if (lpParam != 0)
	{
		HANDLE hEvent = (HANDLE) lpParam;
		if (hEvent != 0)
		{			
			while (!KillFlag)
			{
				WaitForSingleObject(hEvent, INFINITE);

				if (!KillFlag)
				{
					RETAILMSG(RETAIL_ON, (TEXT("ISTFunction setLed 1 \r\n")));
					isr_info.pGPIORegs->gpsr2 = (1 << 15); // reset GPIO79
					RETAILMSG(RETAIL_ON, (TEXT("ISTFunction clearLed 1 \r\n")));
					isr_info.pGPIORegs->gpcr2 = (1 << 15); // set GPIO79
				}
			}
		}
	}
	RETAILMSG(RETAIL_ON, (TEXT("ISTFunction end \r\n")));
	return 0;
}

//=======================================================================
// LED_Init - Driver initialization function
//
DWORD LED_Init(LPCSTR pContext, DWORD dwBusContext){
	PDRVCONTEXT pDrv;
	RETAILMSG(RETAIL_ON, (TEXT("Test debug output without context \r\n")));
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

	DWORD dwGpio = 15;
	DWORD dwIrq = 0;	
	DWORD dwEdge = GPIO_EDGE_RISING;

	if (!KernelIoControl(IOCTL_HAL_GPIO2IRQ, &dwGpio, sizeof(DWORD), &dwIrq, sizeof(DWORD), NULL))
	{
		RETAILMSG(RETAIL_ON, (DTAG TEXT("LED_Init failure. IOCTL_HAL_GPIO2IRQ\r\n"), pContext));
		return 0;
	}

	if (!KernelIoControl(IOCTL_HAL_IRQEDGE, &dwIrq, sizeof(BYTE), &dwEdge, sizeof(BYTE), NULL))
	{
		RETAILMSG(RETAIL_ON, (DTAG TEXT("LED_Init failure. IOCTL_HAL_IRQEDGE\r\n"), pContext));
		return 0;
	}

	if (!KernelIoControl(IOCTL_HAL_REQUEST_SYSINTR, &dwIrq, sizeof(DWORD), &(pDrv->dwSysIntr), sizeof(DWORD), NULL))
	{
		RETAILMSG(RETAIL_ON, (DTAG TEXT("LED_Init failure. IOCTL_HAL_REQUEST_SYSINTR\r\n"), pContext));
		return 0;
	}

	pDrv->hEvent = CreateEvent(NULL, false, false, TEXT("ISTEvent"));
	if (!(pDrv->hEvent))
	{
		RETAILMSG(RETAIL_ON, (DTAG TEXT("LED_Init failure. Create Event failed.\r\n"), pContext));
		return 0;
	}

	if (!InterruptInitialize(pDrv->dwSysIntr, pDrv->hEvent, NULL, 0))
	{
		RETAILMSG(RETAIL_ON, (DTAG TEXT("LED_Init failure. InterruptInitialize failed.\r\n"), pContext));
		return 0;
	}

	pDrv->hIsrHandle = LoadIntChainHandler(TEXT("\\Program Files\\Drivers\\ISRDll.dll"), TEXT("ISRHandler"), dwIrq);
	if (!pDrv->hIsrHandle)
	{
		RETAILMSG(RETAIL_ON, (DTAG TEXT("LED_Init failure. LoadIntChainHandler failed.\r\n"), pContext));
		return 0;
	}

	isr_info.SysIntr = pDrv->dwSysIntr;

	isr_info.pGPIORegs = (GPIOREG*)CreateStaticMapping(GPIO_BASE >> 8, sizeof(GPIOREG));
	if (!isr_info.pGPIORegs)
	{
		RETAILMSG(RETAIL_ON, (DTAG TEXT("LED_Init failure. CreateStaticMapping failed.\r\n"), pContext));
		return 0;
	}

	if (!KernelLibIoControl(pDrv->hIsrHandle, IOCTL_ISR_INFO, &isr_info, sizeof(isr_info), NULL, 0, NULL))
	{
		RETAILMSG(RETAIL_ON, (DTAG TEXT("LED_Init failure. IOCTL_HAL_RELEASE_SYSINTR failed.\r\n"), pContext));
		return 0;
	}

	pDrv->hIntThread = CreateThread(NULL, 0, ISTFunction, pDrv->hEvent, 0, NULL);
	if (!pDrv->hIntThread)
	{
		RETAILMSG(RETAIL_ON, (DTAG TEXT("LED_Init failure. CreateThread failed.\r\n"), pContext));
		return 0;
	}


	RETAILMSG(RETAIL_ON, (DTAG TEXT("LED_Init-- pDrv: %x\r\n"), pDrv));

	return (DWORD)pDrv;

FREECHAINHANDLER:
	FreeIntChainHandler(pDrv->hIsrHandle);
INTERRUPTDISABLE:
	InterruptDisable(pDrv->dwSysIntr);
KERNELIOCONTROL:
	KernelIoControl(IOCTL_HAL_RELEASE_SYSINTR, &pDrv->dwSysIntr, sizeof(DWORD), NULL, 0, NULL);
CLOSEHANDLE:
	CloseHandle(pDrv->hEvent);
DELETESTATICMAPPING:
	DeleteStaticMapping((LPVOID)isr_info.pGPIORegs, sizeof(GPIOREG));
}

BOOL LED_Deinit(DWORD dwContext)
{
	PDRVCONTEXT pDrv = (PDRVCONTEXT)dwContext;
	
	RETAILMSG(RETAIL_ON, (DTAG TEXT("LED_Deinit++ dwContext: %x\r\n"), dwContext));

	// Verify that the context handle is valid
	if(pDrv && (pDrv->dwSize != sizeof(DRVCONTEXT))){
		return 0;
	}

	// terminate thread
	KillFlag = true;
	SetEvent(pDrv->hEvent);

	FreeIntChainHandler(pDrv->hIsrHandle);
	InterruptDisable(pDrv->dwSysIntr);
	KernelIoControl(IOCTL_HAL_RELEASE_SYSINTR, &pDrv->dwSysIntr, sizeof(DWORD), NULL, 0, NULL);
	CloseHandle(pDrv->hEvent);
	DeleteStaticMapping((LPVOID)isr_info.pGPIORegs, sizeof(GPIOREG));

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
