#ifndef __MAPREG_H__
#define __MAPREG_H__

#define PAGE_SIZE	4096

/* ToDo: implement missing functions as defined !!! */
void* MapRegister(DWORD pa);
void UnMapRegister(void* pRegs);
/****************************************************/


#endif