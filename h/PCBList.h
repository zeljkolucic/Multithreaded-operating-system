/*
 * PCBList.h
 *
 *  Created on: Aug 12, 2020
 *      Author: OS1
 */

#ifndef PCBLIST_H_
#define PCBLIST_H_

#include "Thread.h"
#include "PCB.h"
#include "System.h"

class KernelSem;

class PCBList {
public:

	PCBList();
	~PCBList();

	int getSize() const;
	void add(PCB* pcb);
	PCB* removeFirst();
	void removeById(ID id);
	Thread * getThreadById(ID id);
	void unblock();
	void updateWaitingThreads(KernelSem *s);

	void blockSignalGlobally(SignalId signal);
	void unblockSignalGlobally(SignalId signal);

	friend class PCB;
	friend class System;
	friend class SemList;

private:

	struct Elem{
		PCB *pcb;
		Elem *next;
		Elem(PCB* pcb): pcb(pcb), next(0) {}
	};

	Elem *first, *last;
	int size;
};



#endif /* PCBLIST_H_ */
