#include <windows.h>
#include <commctrl.h>
#include "MapReg.h"

#define RETAIL_ON TRUE

extern "C"{
BOOL VirtualCopy(LPVOID lpvDest, LPVOID lpvSrc, DWORD cbSize, DWORD fdwProtect);
}

void* MapRegister(DWORD pa)
{
	// allocate memory first and map it if allocation succeeds
	LPVOID addr = VirtualAlloc(0, PAGE_SIZE,  MEM_RESERVE,  PAGE_NOACCESS);
	if (addr != NULL) {
		bool ret = VirtualCopy(addr, (LPVOID) (pa>>8), PAGE_SIZE, PAGE_READWRITE | PAGE_NOCACHE | PAGE_PHYSICAL);
		if (ret) {
			RETAILMSG(RETAIL_ON, (TEXT("VirtualCopy returned: %d\r\n"), ret));
			return addr;
		}
		else {
			return NULL;
		}
	}
	else {
		return NULL;
	}
}

void UnMapRegister(void* pRegs)
{
	// free memory here
	VirtualFree(pRegs, PAGE_SIZE, MEM_DECOMMIT | MEM_RELEASE);
}
