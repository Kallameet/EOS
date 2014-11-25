/* ***************************************************************************
 * File        : TSVector.cpp
 * Author      : Sebastian Ratzenböck
 * Version     : 1
 * Date        : 02.04.2013
 * Description :
 *	Module TSVector.
 * **************************************************************************/

// Thread safe container
// implementation
#include <cassert>
#include "TSVector.h"

void TSVector::PushBack(int const Data) {

	// apply lock
	CSLock Lock(mCS);

	mV.push_back(Data);
}

int TSVector::GetAt(size_t const n) const {

	// apply lock
	CSLock Lock(mCS);

	assert(mV.size() > n);
	return mV[n];
}

void TSVector::EraseAt(size_t const n) {

	// apply lock
	CSLock Lock(mCS);

	assert(mV.size() > n);
	mV.erase(mV.begin() + n);
}

size_t TSVector::GetSize() const {

	// apply lock
	CSLock Lock(mCS);

	return mV.size();
}

void TSVector::Clear() {

	// apply lock
	CSLock Lock(mCS);

	mV.clear();
}

// for lock-scopes outside the class
CritSec & TSVector::GetCS() const {

	return mCS;
}
