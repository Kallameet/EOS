/* ***************************************************************************
 * File        : TSQueue.h
 * Author      : Sebastian Ratzenböck
 * Version     : 1
 * Date        : 04.05.2013
 * Description :
 *	Interface for the module TSQueue.
 * **************************************************************************/

#ifndef TSQUEUE_H
#define TSQUEUE_H

// Thread-safe STL queue
#include <queue>
#include "Lock.h"
#include "EProcessData.h"

class TSQueue {

	typedef std::queue<int> TQueue;

public:
	// CTor
	TSQueue();
	// DTor
	virtual ~TSQueue();

	// access first element of the queue
	int GetFront() const;
	// access last element of the queue
	int GetLast() const;
	// insert Data at the end of the queue
	void PushBack(int const Data);
	// delete Data at the beginning of the queue
	void PopFront();

	size_t GetSize() const;
	CritSec & GetCS() const;

protected:
	TSQueue(TSQueue const&);	// do not allow copying
	TSQueue & operator=(TSQueue const&);	// do not allow assignment

private:
	TQueue mQ;				// the local unsafe queue
	mutable CritSec mCS;	// critical section object
							// mutable allows usage within const methods
};

#endif // TSQUEUE_H
