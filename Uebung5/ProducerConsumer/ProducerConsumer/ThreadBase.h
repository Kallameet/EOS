/* ***************************************************************************
 * File        : ThreadBase.h
 * Author      : Sebastian Ratzenböck
 * Version     : 1
 * Date        : 22.03.2013
 * Description :
 *	Interface for the module ThreadBase.
 * **************************************************************************/

#ifndef THREADBASE_H
#define THREADBASE_H

#include <Windows.h>

class ThreadBase {
public:
	ThreadBase();
	virtual ~ThreadBase();

	// The thread function
	virtual int Run() = 0;
	// control thread behaviour
	virtual void Start() const;
	virtual void Stop() const;

	// state check
	virtual bool IsThreadCreated() const;

	// this handle is even valid after destroying this thread object
	// call CloseHandle() after usage!
	virtual HANDLE GetDuplicateHdl() const;

protected:
	// internal thread handle => derived classes only
	virtual HANDLE GetThreadHdl() const;

	ThreadBase(ThreadBase const &);
	ThreadBase & operator = (ThreadBase const &);

private:
	static unsigned long WINAPI ThreadFunc(void * Param);

	HANDLE ThreadHdl;
	unsigned long ThreadId;
};


#endif // THREADBASE_H
