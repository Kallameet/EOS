/* ***************************************************************************
 * File        : TSVector.h
 * Author      : Sebastian Ratzenböck
 * Version     : 1
 * Date        : 02.04.2013
 * Description :
 *	Interface for the module TSVector.
 * **************************************************************************/

#ifndef TSVECTOR_H
#define TSVECTOR_H

// Thread-safe STL vector
#include <vector>
#include "Lock.h"

class TSVector {
	
	typedef std::vector<int> TVector;

public:
	// CTor
	TSVector() {}
	// DTor
	virtual ~TSVector() {}

	void PushBack(int const Data);
	int GetAt(size_t const n) const;
	void EraseAt(size_t const n);
	size_t GetSize() const;
	void Clear();
	CritSec & GetCS() const;

protected:
	TSVector(TSVector const&);	// do not allow copying
	TSVector & operator=(TSVector const&);	// do not allow assignment

private:
	TVector mV;				// the local unsafe vector
	mutable CritSec mCS;	// critical section object
							// mutable allows usage within const methods
};

#endif // TSVECTOR_H
