#include <cassert>
#include <iostream>
#include "SensorThread.h"
#include "RandomGen.h"

SensorThread::SensorThread() 
{
	assert(ThreadBase::IsThreadCreated());

	IsInitialized = false;
	ThreadNum = 0;

	ThreadPriority = GetThreadPriority(GetThreadHdl());
	assert(ThreadPriority != THREAD_PRIORITY_ERROR_RETURN);	
}

SensorThread::~SensorThread() 
{

}

void SensorThread::Init(int Number, ProcessDataType type, TSQueue* queue, int Prio) 
{
	ThreadNum = Number;
	ThreadPriority = Prio;

	SetThreadPriority(GetThreadHdl(), ThreadPriority);

	Type = type;
	Queue = queue;

	IsInitialized = true;
}

int randLimited(int limit)
{
	return rand() % limit;
}

int SensorThread::Run() 
{
	assert(IsInitialized);
	if (!IsInitialized) 
	{
		return 0;				// specify error code
	}

	// ---- enter your personal code here ----- //

	int Low = 0;
	int High = 0;
	size_t Intervall = 0;

	switch(Type)
	{
	case Speed:
		{
			Low = RPMLow;
			High = RPMHigh;
			Intervall = RPMIntervall;
			break;
		}
	case Temp:
		{
			Low = TempLow;
			High = TempHigh;
			Intervall = TempIntervall;
			break;
		}
	case Voltage:
		{
			Low = VoltageLow;
			High = VoltageHigh;
			Intervall = VoltageIntervall;
			break;
		}
	}

	while (true)
	{
		Queue->PushBack(rgen::GetRandVal(Low, High));
		Sleep(Intervall);
	}

	// ---- end of personal code

	delete this;

	return 0;
}