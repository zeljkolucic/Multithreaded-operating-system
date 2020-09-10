/*
 * IVTEntry.cpp
 *
 *  Created on: Aug 19, 2020
 *      Author: OS1
 */

#include "IVTEntry.h"
#include "KernelEv.h"
#include "System.h"
#include <dos.h>

IVTEntry * IVTEntry::IVTable[256] = { 0 };

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newRoutine) : myEntry(ivtNo), oldRoutine(0), kernelEvent(0) {
	lock
	IVTable[ivtNo] = this;

#ifndef BCC_BLOCK_IGNORE
	this->oldRoutine = getvect(ivtNo);
	setvect(ivtNo, newRoutine);
#endif

	unlock
}

IVTEntry::~IVTEntry() {
	lock
	(*oldRoutine)();
#ifndef BCC_BLOCK_IGNORE
	setvect(myEntry, oldRoutine);
#endif

	IVTable[myEntry] = 0;
	unlock
}

void IVTEntry::signal() {
	if(kernelEvent)
		kernelEvent->signal();
}


