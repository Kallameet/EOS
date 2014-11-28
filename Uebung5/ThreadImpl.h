#ifndef ThreadImpl_INCLUDED
#define ThreadImple_INCLUDED

#include "ThreadBase.h"

class ThreadSort : public ThreadBase {

public:
	ThreadSort();
	virtual ~ThreadSort();

	virtual void Init(int Number, int ThreadPrio = THREAD_PRIORITY_NORMAL);

	virtual int Run();

private:
	int ThreadPriority;
	int ThreadNum;
	bool IsInitialized;
};

#endif