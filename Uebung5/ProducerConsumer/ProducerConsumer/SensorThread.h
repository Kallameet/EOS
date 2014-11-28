#ifndef ThreadImpl_INCLUDED
#define ThreadImple_INCLUDED

#include "ThreadBase.h"
#include "EProcessData.h"
#include "TSQueue.h"

class SensorThread : public ThreadBase 
{

public:
	SensorThread();
	virtual ~SensorThread();

	virtual void Init(int Number, ProcessDataType type, TSQueue* queue, int ThreadPrio = THREAD_PRIORITY_NORMAL);

	virtual int Run();

private:
	int ThreadPriority;
	int ThreadNum;
	bool IsInitialized;

	ProcessDataType Type;
	TSQueue* Queue;
};

#endif