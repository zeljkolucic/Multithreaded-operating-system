/*
 * KernelS.cpp
 *
 *  Created on: Aug 20, 2020
 *      Author: OS1
 */

#include "KernelS.h"
#include "System.h"
#include "PCBList.h"
#include "PCB.h"
#include "Thread.h"
#include "SCHEDULE.H"

KernelSem::KernelSem(int init) {
	lock
	value = init;
	blocked = new PCBList();
	unlock
}

KernelSem::~KernelSem() {
	lock
	delete blocked;
	unlock
}

int KernelSem::wait(int maxTimeToWait) {
	lock
	if (--value < 0) {
		if (maxTimeToWait == 0) {
			PCB::running->waitingOnTime = 0;
			PCB::running->status = PCB::BLOCKED;
			blocked->add((PCB*) PCB::running);
			unlock
			dispatch();
			return 1;
		} else if (maxTimeToWait > 0) {
			PCB::running->waitingOnTime = 1;
			PCB::running->timeToWait = maxTimeToWait;
			PCB::running->status = PCB::BLOCKED;
			blocked->add((PCB*) PCB::running);
			unlock
			dispatch();
			if (PCB::running->unblockedBySignal == 1) {
				PCB::running->unblockedBySignal = 0;
				return 1;
			}
			return 0;
		}
	}
	unlock
	return 1;
}

int KernelSem::signal(int n) {

	lock
	if (n == 0) {
		if (value++ < 0)
			unblock();
		unlock
		return 0;
	} else if (n > 0) {
		value += n;
		int toUnblock = (blocked->getSize() > n) ? n : blocked->getSize();
		for (int i = 0; i < toUnblock; i++)
			unblock();
		unlock
		return toUnblock;
	}
	unlock
	return n;
}

void KernelSem::unblock() {
	PCB* unblocked = blocked->removeFirst();
	if (unblocked->waitingOnTime == 1) {
		unblocked->waitingOnTime = 0;
		unblocked->unblockedBySignal = 1;
	}
	unblocked->status = PCB::READY;
	Scheduler::put(unblocked);
}

int KernelSem::getValue() const {
	return value;
}

void KernelSem::updateWaitingThreads() {
	blocked->updateWaitingThreads(this);
}

