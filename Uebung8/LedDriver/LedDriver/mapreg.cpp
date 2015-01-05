#include "stdafx.h"
#include <windows.h>
#include "MapReg.h"

extern "C"{
BOOL VirtualCopy(LPVOID lpvDest, LPVOID lpvSrc, DWORD cbSize, DWORD fdwProtect);
}

void* MapRegister(DWORD pa)
{
	// allocate memory first and map it if allocation succeeds
	void * addr = VirtualAlloc((LPVOID) pa, PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (addr != NULL) {
		bool ret = VirtualCopy(addr, (LPVOID) pa, PAGE_SIZE, PAGE_READWRITE | PAGE_NOCACHE | PAGE_PHYSICAL);
		if (ret) {
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
