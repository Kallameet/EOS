/*************************************************************************
 Header file of generic stream device driver for windows ce 6.0

*************************************************************************/


// Declare the external entry points here. Use declspec so we don't need 
// a .def file. Bracketed with extern C to avoid mangling in C++
#ifdef __cplusplus
extern "C"{
#endif //__cplusplus
	__declspec(dllexport) DWORD LED_Init(LPCSTR pContext, DWORD dwBusContext);
	__declspec(dllexport) DWORD LED_Open(DWORD dwContext, DWORD dwAccess, DWORD dwShare);

	/* ToDo: implement missing functions as defined !!!                                             */
	__declspec(dllexport) BOOL LED_PreDeinit(DWORD dwContext);
	__declspec(dllexport) BOOL LED_Deinit(DWORD dwContext);
	__declspec(dllexport) BOOL LED_PreClose(DWORD dwOpen);
	__declspec(dllexport) BOOL LED_Close(DWORD dwOpen);
	__declspec(dllexport) DWORD LED_Read(DWORD dwOpen, LPVOID pBuffer, DWORD dwCount);
	__declspec(dllexport) DWORD LED_Write(DWORD dwOpen, LPVOID pBuffer, DWORD dwCount);
	__declspec(dllexport) DWORD LED_Seek(DWORD dwOpen, long lDelta, WORD wType);
	__declspec(dllexport) DWORD LED_IOControl(DWORD dwOpen, DWORD dwCode, PBYTE pIn, DWORD dwIn, 
											  PBYTE pOut, DWORD dwOut, DWORD *pdwBytesWritten);
	__declspec(dllexport) void LED_PowerDown(DWORD dwContext);
	__declspec(dllexport) void LED_PowerUp(DWORD dwContext);
	/***********************************************************************************************/


#ifdef __cplusplus
} // extern "C"
#endif //__cplusplus

