#ifndef DEVDRV_H
#define DEVDRV_H

DWORD LoadDriver(HANDLE *hDriver, LPCWSTR lpszDevKey);
DWORD UnloadDriver(HANDLE * hDriver);

#endif