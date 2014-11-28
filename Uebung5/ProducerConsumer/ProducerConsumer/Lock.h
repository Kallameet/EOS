#ifndef LOCK_INCLUDED
#define LOCK_INCLUDED

#include <Windows.h>


// mutual exclusion for synchronization
// initialization of the critical section
class CritSec {
	friend class CSLock;

public:
	// CTor
	CritSec() {
		InitializeCriticalSection(&mCs);
	}

	// DTor
	~CritSec() {
		DeleteCriticalSection(&mCs);
	}

private:
	void Acquire() {
		EnterCriticalSection(&mCs);
	}
	void Release() {
		LeaveCriticalSection(&mCs);
	}

	CritSec(CritSec const &);				// do not allow copying
	CritSec & operator=(CritSec const &);	// do not allow assignment

	CRITICAL_SECTION mCs;		// THE critical section
};


// apply and release the lock
class CSLock {
public:
	// lock for exclusive access
	CSLock(CritSec & CSObj) : mCSAccess(CSObj) {
		mCSAccess.Acquire();
	}

	// release lock

	~CSLock() {
		mCSAccess.Release();
	}

private:
	CSLock();							// do not allow default CTor
	CSLock(CSLock const &);				// do not allow copying
	CSLock & operator=(CSLock const &);	// do not allow assignment

	CritSec &mCSAccess;
};


#endif