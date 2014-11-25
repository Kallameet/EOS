/* ***************************************************************************
 * File        : TSQueue.cpp
 * Author      : Sebastian Ratzenböck
 * Version     : 1
 * Date        : 04.05.2013
 * Description :
 *	Module TSQueue.
 * **************************************************************************/

// Thread safe container
// implementation
#include <cassert>
#include "TSQueue.h"

// CTor
TSQueue::TSQueue() {}

// DTor
TSQueue::~TSQueue() {}

// access first element of the queue
int TSQueue::GetFront() const {

	// apply lock
	CSLock Lock(mCS);
	return mQ.front();
}

// access last element of the queue
int TSQueue::GetLast() const {

	// apply lock
	CSLock Lock(mCS);
	return mQ.back();
}

// insert Data at the end of the queue
void TSQueue::PushBack(int const Data) {

	// apply lock
	CSLock Lock(mCS);
	// Queue must not have more than 8 objects inside
	assert(mQ.size() <= 8);
	mQ.push(Data);
}

// delete Data at the beginning of the queue
void TSQueue::PopFront() {

	// apply lock
	CSLock Lock(mCS);
	mQ.pop();
}

size_t TSQueue::GetSize() const {

	// apply lock
	CSLock Lock(mCS);
	return mQ.size();
}

CritSec & TSQueue::GetCS() const {

	return mCS;
}