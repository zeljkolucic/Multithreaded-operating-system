/*
 * PCB.cpp
 *
 *  Created on: Aug 12, 2020
 *      Author: OS1
 */
#include "PCB.h"
#include <dos.h>
#include "PCBList.h"
#include "System.h"
#include "KernelS.h"
#include "SCHEDULE.H"
#include "Semaphor.h"
#include "HandList.h"
#include "SignList.h"
#include "SemList.h"

ID PCB::stID = 0;

volatile PCB* PCB::running = 0;

PCBList * PCB::listOfAllPCB = new PCBList();

PCB::PCB(Thread* thread, StackSize size, Time timeSlice) :
		sp(0), ss(0), bp(0), myThread(thread), status(CREATED), timeToUse(
				timeSlice), timeLeft(timeSlice), timeToWait(0), waitingOnTime(
				0), unblockedBySignal(0), parentThread(0) {

	id = ++stID;

	if (id > 2)
		parentThread = (PCB*) running;

	waitingForMe = new PCBList();

	stackSize = (size > 65536 ? 65536 : size) / sizeof(unsigned);
	lock
	stack = new unsigned[stackSize];
#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 1] = 0x200;
	stack[stackSize - 2] = FP_SEG(&wrapper);
	stack[stackSize - 3] = FP_OFF(&wrapper);
	ss = FP_SEG(stack + stackSize - 12);
	sp = FP_OFF(stack + stackSize - 12);
	bp = FP_OFF(stack + stackSize - 12);
#endif
	unlock

	listOfAllPCB->add(this);

	signalsToHandle = new SignalList();

	for (int i = 0; i < 16; i++) {
		handlers[i] = new HandlerList();
		if (i == 0 && id != 1 && id != 2)
			handlers[i]->registerHandler(&zeroEntryHandler);
		blockedSignals[i] = 0;
	}

	if (parentThread != 0) {
		signalsToHandle->inherit(parentThread->signalsToHandle);
		for (int j = 0; j < 16; j++) {
			blockedSignals[j] = parentThread->blockedSignals[j];
			handlers[j]->inherit(parentThread->handlers[j]);
		}
	}

}

void PCB::wrapper() {
	running->myThread->run();
	lock
	running->parentThread->signal(1);
	running->handlers[2]->handle();
	running->status = FINISHED;
	running->waitingForMe->unblock();
	unlock
	dispatch();
}

void PCB::waitToComplete() {
	lock
	if (running->id != id && System::mainThread->myPCB != this
			&& System::idleThread->myPCB != this && status != CREATED
			&& status != FINISHED) {
		running->status = BLOCKED;
		waitingForMe->add((PCB*) running);
		unlock
		dispatch();
		return;
	}
	unlock
}

PCB::~PCB() {
	lock
	listOfAllPCB->removeById(id);
	delete waitingForMe;
	delete[] stack;
	for (int i = 0; i < 16; i++)
		delete handlers[i];
	delete signalsToHandle;
	unlock
}

void PCB::signal(SignalId signal) {
	if (signal < 0 || signal > 15)
		return;
	if (running->id == id) {
		if (blockedSignals[signal] == 0)
			handlers[id]->handle();
	} else
		signalsToHandle->add(signal);
}

void PCB::registerHandler(SignalId signal, SignalHandler handler) {
	if (signal < 0 || signal > 15)
		return;
	handlers[signal]->registerHandler(handler);
}

void PCB::unregisterAllHandlers(SignalId id) {
	if (id < 0 || id > 15)
		return;
	handlers[id]->unregisterAllHandlers();
}

void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	if (id < 0 || id > 15)
		return;
	handlers[id]->swap(hand1, hand2);
}

void PCB::blockSignal(SignalId signal) {
	if (signal < 0 || signal > 15)
		return;
	blockedSignals[signal] = 1;
}

void PCB::blockSignalGlobally(SignalId signal) {
	if (signal < 0 || signal > 15)
		return;
	listOfAllPCB->blockSignalGlobally(signal);
}

void PCB::unblockSignal(SignalId signal) {
	if (signal < 0 || signal > 15)
		return;
	blockedSignals[signal] = 0;
}

void PCB::unblockSignalGlobally(SignalId signal) {
	if (signal < 0 || signal > 15)
		return;
	listOfAllPCB->unblockSignalGlobally(signal);
}

void PCB::handleAll() {
	signalsToHandle->handleAll();
}

void PCB::zeroEntryHandler() {
	running->status = FINISHED;
	if (running->parentThread != 0)
		running->parentThread->signal(1);
	running->handlers[2]->handle();
	running->waitingForMe->unblock();
	dispatch();
}

