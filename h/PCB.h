/*
 * PCB.h
 *
 *  Created on: Aug 12, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "Thread.h"

class PCBList;
class KernelSem;
class HandlerList;
class SignalList;

class PCB {
public:
	unsigned sp;
	unsigned ss;
	unsigned bp;
	unsigned *stack;
	unsigned stackSize;
	Time timeToUse;
	Time timeLeft;
	Time timeToWait;

	int waitingOnTime;
	int unblockedBySignal;

	enum Status {
		CREATED, READY, BLOCKED, FINISHED
	};
	Status status;

	static ID stID;
	ID id;

	Thread *myThread;

	PCBList* waitingForMe;
	static PCBList *listOfAllPCB;
	static volatile PCB* running;
	static void wrapper();

	void waitToComplete();

	PCB(Thread* thread, StackSize stackSize, Time timeSlice);

	virtual ~PCB();

	void signal(SignalId signal);

	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);

	void handleAll();
	static void zeroEntryHandler();

	PCB *parentThread;
	SignalList *signalsToHandle;
	HandlerList *handlers[16];
	int blockedSignals[16];

};

#endif /* PCB_H_ */
