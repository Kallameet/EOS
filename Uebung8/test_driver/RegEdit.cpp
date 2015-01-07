#include "stdafx.h"
#include "RegEdit.h"
#include <windows.h>

DWORD AddEntryToRegistry(HKEY hKey, LPCWSTR lpSubKey, RegEntry RegEntries[]){
    HKEY  hTargetKey;
    DWORD dwDisposition;
    DWORD RetVal;
    DWORD Index=0;
    BYTE * Value;
    DWORD uSize;

    // create the key if it doesn't exists
	RetVal = RegCreateKeyEx(hKey,                       // on of the root key constants e.g. HKEY_LOCAL_MACHINE
							lpSubKey,                   // subkey which should be opened or creatred
							0,                          // reserved parameter -> should be set to 0
							NULL,                       // Class parameter points to the class name
							REG_OPTION_NON_VOLATILE,    // specify if key is volatile or not. default: nonvolatile -> key will persist after restart
							KEY_ALL_ACCESS,             // access option
							NULL,                       // security attribute -> should be null
							&hTargetKey,                // variable which receives the handle
							&dwDisposition              // variable which receives disposition values
							);
    // if call was successful and we created a new entry, set data of entry
    if (RetVal == ERROR_SUCCESS && dwDisposition == REG_CREATED_NEW_KEY){
        // set data of array
        while( RegEntries[Index].ValueName != NULL )
        {   
            if( RegEntries[Index].Type == REG_DWORD )
            {
                Value = (BYTE *)&RegEntries[Index].DwordValue;
                uSize = sizeof( DWORD );
            }
            else
            {
                Value = (BYTE *)RegEntries[Index].StringValue;
                uSize = (wcslen( RegEntries[Index].StringValue ) + 1 ) * sizeof( TCHAR );
            }
           
            RetVal = RegSetValueEx(hTargetKey,
								   RegEntries[Index].ValueName,
								   0,
								   RegEntries[Index].Type,
								   (CONST BYTE *)Value,
								   uSize);
            Index++;
        }
    }

    return RetVal;
}