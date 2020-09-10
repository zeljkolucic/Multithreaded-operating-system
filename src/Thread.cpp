/*
 * Thread.cpp
 *
 *  Created on: Aug 12, 2020
 *      Author: OS1
 */
#include "Thread.h"
#include "PCB.h"
#include "System.h"
#include "SCHEDULE.H"
#include "PCBList.h"

Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock
	myPCB = new PCB(this, stackSize, timeSlice);
	unlock
}

Thread::~Thread() {
	lock
	this->waitToComplete();
	PCB::listOfAllPCB->removeById(myPCB->id);
	delete myPCB;
	unlock
}

void Thread::start() {
	lock
	if (myPCB->status == PCB::CREATED) {
		myPCB->status = PCB::READY;
		Scheduler::put(myPCB);
	}
	unlock
}

void Thread::waitToComplete() {
	lock
	myPCB->waitToComplete();
	unlock
}

ID Thread::getId() {
	return myPCB->id;
}

ID Thread::getRunningId() {
	return PCB::running->id;
}

Thread * Thread::getThreadById(ID id) {
	lock
	Thread *returnValue = PCB::listOfAllPCB->getThreadById(id);
	unlock
	return returnValue;
}

void dispatch() {
	lock
	System::contextSwitchOnDemand = 1;
	System::timer();
	unlock
}

void Thread::signal(SignalId signal) {
	System::lockFlag = 1;
	myPCB->signal(signal);
	System::lockFlag = 0;
}

void Thread::registerHandler(SignalId signal, SignalHandler handler) {
	System::lockFlag = 1;
	myPCB->registerHandler(signal, handler);
	System::lockFlag = 0;
}

void Thread::unregisterAllHandlers(SignalId id) {
	System::lockFlag = 1;
	myPCB->unregisterAllHandlers(id);
	System::lockFlag = 0;
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	System::lockFlag = 1;
	myPCB->swap(id, hand1, hand2);
	System::lockFlag = 0;
}

void Thread::blockSignal(SignalId signal) {
	System::lockFlag = 1;
	myPCB->blockSignal(signal);
	System::lockFlag = 0;
}

void Thread::blockSignalGlobally(SignalId signal) {
	System::lockFlag = 1;
	PCB::blockSignalGlobally(signal);
	System::lockFlag = 0;
}

void Thread::unblockSignal(SignalId signal) {
	System::lockFlag = 1;
	myPCB->unblockSignal(signal);
	System::lockFlag = 0;
}

void Thread::unblockSignalGlobally(SignalId signal) {
	System::lockFlag = 1;
	PCB::unblockSignalGlobally(signal);
	System::lockFlag = 0;
}

