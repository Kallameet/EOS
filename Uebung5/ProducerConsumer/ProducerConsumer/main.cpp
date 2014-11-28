#include <windows.h>
#include <iostream>
#include "SensorThread.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	SensorThread * pThread1 = new SensorThread;
	SensorThread * pThread2 = new SensorThread;
	SensorThread * pThread3 = new SensorThread;

	TSQueue RPMQueue;
	TSQueue TempQueue;
	TSQueue VoltageQueue;

	pThread1->Init(1, Speed, &RPMQueue);
	pThread2->Init(2, Temp, &TempQueue);
	pThread3->Init(3, Voltage, &VoltageQueue);

	int const n = 3;
	HANDLE Hdl[n];
	Hdl[0] = pThread1->GetDuplicateHdl();
	Hdl[1] = pThread2->GetDuplicateHdl();
	Hdl[2] = pThread3->GetDuplicateHdl();

	pThread1->Start();
	pThread2->Start();
	pThread3->Start();

	// do something in main thread
	while(true)
	{
		if (RPMQueue.GetSize() > 0)
		{
			cout << "Speed [rpm]: " << RPMQueue.GetFront() << endl;
			RPMQueue.PopFront();
		}
		if (TempQueue.GetSize() > 0)
		{
			cout << "Temp [deg C]: " << TempQueue.GetFront() << endl;
			TempQueue.PopFront();
		}
		if (VoltageQueue.GetSize() > 0)
		{
			cout << "V diff. [mV]: " << VoltageQueue.GetFront() << endl;
			VoltageQueue.PopFront();
		}

		Sleep(50);
	}

	return 0;
}
