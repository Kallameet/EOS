// test_driver.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>
#include <iostream>
#include "RegEdit.h"
#include "DevDrv.h"


int _tmain(int argc, _TCHAR* argv[])
{
	// struct for registry entry
	RegEntry EosRegEntries[] = {
    {TEXT("Dll"),               // key DLL: specify the name of the dll which implements the driver
	 REG_SZ,                    // type is a zero-terminated unicode string
	 0,                         
	 TEXT("\\Program Files\\Drivers\\LedDriver.dll") },  // value: name of dll
    {TEXT("Prefix"),            // key Prefix: specify the three-letter name of the driver
	 REG_SZ,                    // type is a zero-terminated unicode string
	 0, 
	 TEXT("LED") },             // value: name of dll
    {TEXT("Order"),             // key Order: specify load order. drivers with lower numbers will be loaded before
	 REG_DWORD,                 // type is a 4-byte binary value
	 4, 
	 NULL },
     {NULL, 0, 0, NULL}         // terminating entry
    };

    HANDLE hDev = INVALID_HANDLE_VALUE;

    DWORD ret = AddEntryToRegistry(HKEY_LOCAL_MACHINE, EOSDRIVER_REG_KEY, EosRegEntries);
    ret = LoadDriver(&hDev, EOSDRIVER_REG_KEY);
	if(!ret){
		HANDLE hDrv = CreateFileW(TEXT("LED1:"),
								  GENERIC_READ | GENERIC_WRITE,
								  0,
								  NULL,
								  OPEN_EXISTING,
								  FILE_ATTRIBUTE_NORMAL,
								  NULL);
		DWORD err = GetLastError();

		char Buffer[] = "e";
		DWORD NrOfBytesWritten;
		DWORD NrOfBytesWrite = (DWORD)sizeof(Buffer); // driver uses only the first byte
		DWORD NrOfBytesRead = 0;
		bool read = false;
		bool written = false;
		char chr = 0xFF;

		do{
            
		// End program if button T4 was pressed
		}while(true);
		// close the handle (CreateFile)
		CloseHandle(hDrv);

        // we need no further CloseHandle because hDev is still invalid after
        // unloading the driver
		UnloadDriver(&hDev);
	}
	return 0;
}

