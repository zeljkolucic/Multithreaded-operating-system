/*
 * IVTEntry.h
 *
 *  Created on: Aug 19, 2020
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "Event.h"
#include "System.h"

#define PREPAREENTRY(entryNo, flag)\
void interrupt intr##entryNo(...);\
IVTEntry ivte##entryNo(entryNo, intr##entryNo);\
void interrupt intr##entryNo(...){\
	(IVTEntry::IVTable[##entryNo])->signal();\
	if (flag == 1)\
		ivte##entryNo.oldRoutine();\
	dispatch();\
}\

class IVTEntry {
public:
	IVTEntry(IVTNo ivtNo, pInterrupt newRoutine);
	~IVTEntry();
	void signal();

	static IVTEntry * IVTable[256];

	friend class KernelEv;

	pInterrupt oldRoutine;

private:
	KernelEv* kernelEvent;
	IVTNo myEntry;
};



#endif /* IVTENTRY_H_ */
