#include <cassert>
#include <iostream>
#include "SensorThread.h"

SensorThread::SensorThread() 
{
	assert(ThreadBase::IsThreadCreated());

	IsInitialized = false;
	ThreadNum = 0;

	ThreadPriority = GetThreadPriority(GetThreadHdl());
	assert(ThreadPriority != THREAD_PRIORITY_ERROR_RETURN);

	std::cout << "Initial Thread priority: " << ThreadPriority << std::endl;
}

SensorThread::~SensorThread() 
{

}

void SensorThread::Init(int Number, int Prio) 
{
	ThreadNum = Number;
	ThreadPriority = Prio;

	SetThreadPriority(GetThreadHdl(), ThreadPriority);

	IsInitialized = true;
}

int SensorThread::Run() 
{
	assert(IsInitialized);
	if (!IsInitialized) 
	{
		return 0;				// specify error code
	}

	// ---- enter your personal code here ----- //
	
	std::cout << "Hello World " << Number << endl;

	// ---- end of personal code

	delete this;

	return 0;
}