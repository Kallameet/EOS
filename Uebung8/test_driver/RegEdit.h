#ifndef REGEDIT_H
#define REGEDIT_H



#define EOSDRIVER_REG_KEY (TEXT("Drivers\\Builtin\\LED"))

typedef struct {
	TCHAR *ValueName;
	DWORD Type;
	DWORD DwordValue;
	TCHAR *StringValue;
} RegEntry ;


DWORD AddEntryToRegistry(HKEY hKey, LPCWSTR lpSubKey, RegEntry RegEntries[]);

#endif