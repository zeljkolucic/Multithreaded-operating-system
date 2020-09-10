/*
 * KernelEv.cpp
 *
 *  Created on: Aug 19, 2020
 *      Author: OS1
 */

#include "KernelEv.h"
#include "IVTEntry.h"
#include "PCB.h"
#include "System.h"
#include "SCHEDULE.H"

KernelEv::KernelEv(IVTNo ivtNo) {
	value = 0;
	owner = (PCB*) PCB::running;
	IVTEntry::IVTable[ivtNo]->kernelEvent = this;
	myEntry = IVTEntry::IVTable[ivtNo];
}

KernelEv::~KernelEv() {
	owner = 0;
	myEntry->kernelEvent = 0;
}

void KernelEv::wait() {
	if (PCB::running == owner) {
		if (value == 1)
			value--;
		else if (value == 0) {
			value--;
			owner->status = PCB::BLOCKED;
			dispatch();
		}
	}
}

void KernelEv::signal() {
	if(value == 1)
		return;
	else if(value == 0) {
		value++;
		return;
	}
	value = 0;
	owner->status = PCB::READY;
	Scheduler::put(owner);
}
