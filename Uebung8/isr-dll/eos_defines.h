#ifndef __EOS_DEFINES_H_
#define __EOS_DEFINES_H_

#include "gpio.h"

typedef struct{
	DWORD SysIntr;
	GPIOREG* pGPIORegs;
	// other registers or parameters may be necassary
} ISR_INFO;

//driver instance structure
typedef struct{
	DWORD dwSize;
	int nNumOpens;
	GPIOREG * pGpioRegs;						// pointer to gpio registers
	BOOL bStopIST;
	DWORD dwSysIntr;
	HANDLE  hIsrHandle;
	HANDLE	hIntThread;
	HANDLE  hEvent;
}DRVCONTEXT, *PDRVCONTEXT;


// define IOCTL codes for interrupt handling
#define CTL_CODE( DeviceType, Function, Method, Access ) (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))

#define FILE_DEVICE_HAL					0x00000101
#define METHOD_BUFFERED                 0
#define FILE_ANY_ACCESS                 0

#define GPIO_EDGE_RISING 1
#define GPIO_EDGE_FALLING 2

#define IOCTL_ISR_INFO	0
#define IOCTL_GET_VALUE	1
#define IOCTL_IST_PRIO	0

#define IOCTL_HAL_REQUEST_SYSINTR CTL_CODE(FILE_DEVICE_HAL, 38, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HAL_RELEASE_SYSINTR CTL_CODE(FILE_DEVICE_HAL, 54, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HAL_GPIO2IRQ CTL_CODE(FILE_DEVICE_HAL, 2048, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HAL_IRQ2GPIO CTL_CODE(FILE_DEVICE_HAL, 2049, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HAL_IRQEDGE CTL_CODE(FILE_DEVICE_HAL, 2050, METHOD_BUFFERED, FILE_ANY_ACCESS)

extern "C"{
BOOL KernelIoControl(DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned);

BOOL InterruptInitialize(DWORD idInt, HANDLE hEvent, LPVOID pvData, DWORD cbData);
void InterruptDone(DWORD idInt);
void InterruptDisable(DWORD idInt);
HANDLE LoadKernelLibrary(LPCWSTR lpszFileName);

HANDLE  LoadIntChainHandler(LPCWSTR lpFilename, LPCWSTR lpszFunctionName, BYTE bIRQ);
BOOL FreeIntChainHandler(  HANDLE hInstance);

BOOL KernelLibIoControl(HANDLE hModule, DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned);;

LPVOID CreateStaticMapping(DWORD dwPhysBase, DWORD dwSize);
BOOL DeleteStaticMapping(LPVOID pVirtBase, DWORD dwSize); 
}

#endif