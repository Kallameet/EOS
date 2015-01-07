#include "stdafx.h"
#include "DevDrv.h"
#include <windows.h>

DWORD LoadDriver(HANDLE *hDriver, LPCWSTR lpszDevKey){
    
    //check if given handle is still not in use
	if( *hDriver == INVALID_HANDLE_VALUE )
	{
        // load driver 
		*hDriver = ActivateDeviceEx(lpszDevKey,    // name of registry key under [HKEY_LOCAL_MACHINE]
									NULL,          // RegEntry which will be added to the drivers active key 
									0,             // number of RegEntry which will be added to the drivers active key
									NULL);         // data which will passed to the drivers init-function
        // if we got a valid handle, return true
		if (*hDriver != INVALID_HANDLE_VALUE && *hDriver != 0)
			return 0;
		else
		{
            return GetLastError();
		}
	}
	return -1;
}


DWORD UnloadDriver(HANDLE * hDriver){
    // check if given handle is valid
    if(*hDriver != INVALID_HANDLE_VALUE){
        // unload device driver delete active key
        if(!DeactivateDevice(*hDriver)){
            return GetLastError();
        }else{
		    // set handle to unvalid value because
            // after DeactivateDevice(*hDriver) the handle still
            // has the old value but it is not valid anymore
            *hDriver = INVALID_HANDLE_VALUE;
            return 0;
        }
    }
    return -1;
}