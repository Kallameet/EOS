//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
// isrdll.c
//
// Installable Interrupt Service Routine.
//
//
#include <windows.h>
#include "gpio.h"
#include "isr.h"

// NOTE: It's very importat that this dll doesn't use any Windows APIs or 
//       any other reference to external dlls. (it will not load otherwise)

#define SYSINTR_NOP         0
#define SYSINTR_CHAIN       3


DWORD g_dwSysIntr;
GPIOREG* g_pGPIORegs;
DWORD g_dwValue;


// Globals

/*
 @doc INTERNAL
 @func    BOOL | DllEntry | Process attach/detach api.
 *
 @rdesc The return is a BOOL, representing success (TRUE) or failure (FALSE).
 */
BOOL __stdcall DllEntry(HINSTANCE hinstDll,         // @parm Instance pointer.
						DWORD dwReason,             // @parm Reason routine is called.
						LPVOID lpReserved)           // @parm system parameter.
{
     if (dwReason == DLL_PROCESS_ATTACH) {
     }

     if (dwReason == DLL_PROCESS_DETACH) {
     }

     return TRUE;
}


// this can be used for ISR dll that have multile instances (like giisr.dll)
DWORD CreateInstance(void){
    return 0;
}

void DestroyInstance(DWORD InstanceIndex){
}



BOOL IOControl(DWORD   InstanceIndex,
			   DWORD   IoControlCode, 
			   LPVOID  pInBuf, 
			   DWORD   InBufSize,
			   LPVOID  pOutBuf, 
			   DWORD   OutBufSize, 
			   LPDWORD pBytesReturned){

	if (pBytesReturned) {
        *pBytesReturned = 0;
    }

    switch (IoControlCode){
		case IOCTL_ISR_INFO:
			// Copy instance information
			if ((InBufSize != sizeof(ISR_INFO)) || !pInBuf) {
				// Invalid size of output buffer or input buffer pointer
				return FALSE;
			}
			g_dwSysIntr=((ISR_INFO*)pInBuf)->SysIntr;
			g_pGPIORegs=((ISR_INFO*)pInBuf)->pGPIORegs;

			// Do some initializations:
			g_dwValue=0;
			g_pGPIORegs->gafr2_l &= ~(1 << 31 | 1 << 30); // set alternate function 0 for GPIO79
			g_pGPIORegs->gpdr2   |= (1 << 15);  //set GPIO79 as output

			RETAILMSG(1, (TEXT("Inst. ISR DLL: ") TEXT("IOCTL: got SYSINTR: %u , GPIO: %u \r\n"),g_dwSysIntr, g_pGPIORegs));

			break;

		
	case IOCTL_GET_VALUE:
        // Copy instance information
        if ((OutBufSize != sizeof(DWORD)) || !pOutBuf) {
            // Invalid size of output buffer or input buffer pointer
            return FALSE;
        }

        // The compiler may generate a memcpy call for a structure assignment,
        // and we're not linking with the CRT, so use our own copy routine.
        *((DWORD*)pOutBuf) = g_dwValue;
		if(pBytesReturned) *pBytesReturned=sizeof(DWORD);

        break;

    default:
        // Invalid IOCTL
        return FALSE;
    }
    return TRUE;
}




DWORD ISRHandler(DWORD InstanceIndex){
	
	g_pGPIORegs->gpsr2 = (1 << 15); // set GPIO79 -> LED1 on
	
	g_dwValue++;
	
	g_pGPIORegs->gpcr2 = (1 << 15); // reset GPIO79 -> LED1 off

	// if this isr is used to handle a shared interrupt, here we should check if this interrupt is really intended for us.
	// If it's for us we retrun g_dwSysIntr, if it's not for us we should retrun SYSINTR_CHAIN

    return g_dwSysIntr;
}
