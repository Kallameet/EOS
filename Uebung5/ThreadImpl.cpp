#include <cassert>
#include <iostream>
#include "ThreadImpl.h"

ThreadSort::ThreadSort() {
	assert(ThreadBase::IsThreadCreated());

	IsInitialized = false;
	ThreadNum = 0;

	ThreadPriority = GetThreadPriority(GetThreadHdl());
	assert(ThreadPriority != THREAD_PRIORITY_ERROR_RETURN);

	std::cout << "Initial Thread priority: " << ThreadPriority << std::endl;
}

ThreadSort::~ThreadSort() {

}

void ThreadSort::Init(int Number, int Prio) {
	ThreadNum = Number;
	ThreadPriority = Prio;

	SetThreadPriority(GetThreadHdl(), ThreadPriority);

	IsInitialized = true;
}

int ThreadSort::Run() {
	assert(IsInitialized);
	if (!IsInitialized) {
		return 0;				// specify error code
	}

	// ---- enter your personal code here ----- //
	DWORD ThreadId = GetCurrentThreadId();

	ThreadPriority = GetThreadPriority(GetThreadHdl());

	std::cout << "[TID" << ThreadId << ", Priority " << ThreadPriority << "] Thread "
		<< ThreadNum << " has been created successfully " << std::endl;

	// ---- end of personal code

	delete this;

	return 0;
}