#include <windows.h>
#include <iostream>
#include "SensorThread.h"

int _tmain(int argc, _TCHAR* argv[])
{
	SensorThread * pThread1 = new SensorThread;
	SensorThread * pThread2 = new SensorThread;
	SensorThread * pThread3 = new SensorThread;

	pThread1->Init(1);
	pThread2->Init(2);
	pThread3->Init(3);

	int const n = 3;
	HANDLE Hdl[n];
	Hdl[0] = pThread1->GetDuplicateHdl();
	Hdl[1] = pThread2->GetDuplicateHdl();
	Hdl[2] = pThread3->GetDuplicateHdl();

	pThread1->Start();
	pThread2->Start();
	pThread3->Start();

	// do something in main thread
	Sleep(2000);

	WaitForMultipleObjects(n, Hdl, true, INFINITE);
	CloseHandle(Hdl[0]);
	CloseHandle(Hdl[1]);
	CloseHandle(Hdl[2]);

	std::cout << "All done. " << std::endl;

	return 0;
}