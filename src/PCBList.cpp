/*
 * PCBList.cpp
 *
 *  Created on: Aug 12, 2020
 *      Author: OS1
 */
#include "PCBList.h"
#include "PCB.h"
#include "System.h"
#include "SCHEDULE.H"
#include "Semaphor.h"
#include "KernelS.h"

PCBList::PCBList(): first(0), last(0), size(0) {}

PCBList::~PCBList() {
	lock
	while (first) {
		Elem *old = first;
		first = first->next;
		old->next = 0;
		delete old;
	}
	first = last = 0;
	size = 0;
	unlock
}

int PCBList::getSize() const {
	return size;
}

void PCBList::add(PCB* pcb) {
	lock
	Elem* newElem = new Elem(pcb);
	if (first == 0)
		first = newElem;
	else
		last->next = newElem;
	last = newElem;
	size++;
	unlock
}

PCB* PCBList::removeFirst() {
	if (first == 0)
		return 0;
	lock
	Elem *old = first;
	PCB *temp = old->pcb;
	first = first->next;
	if (first == 0)
		last = 0;
	old->next = 0;
	delete old;
	size--;
	unlock
	return temp;
}

void PCBList::removeById(ID id) {
	lock
	Elem *prev = 0, *cur = first;
	while (cur) {
		if (cur->pcb->id == id) {
			Elem *old = cur;
			if (prev != 0)
				prev->next = cur->next;
			else
				first = cur->next;
			if (old == last)
				last = prev;
			if (first == 0)
				last = 0;
			old->next = 0;
			delete old;
			size--;
			unlock
			return;
		} else {
			prev = cur;
			cur = cur->next;
		}
	}
	unlock
}

Thread * PCBList::getThreadById(ID id) {
	lock
	Elem *cur = first;
	while (cur && cur->pcb->id != id)
		cur = cur->next;
	unlock
	return cur ? cur->pcb->myThread : 0;
}

void PCBList::unblock() {
	lock
	while (first) {
		Elem *old = first;
		first->pcb->status = PCB::READY;
		Scheduler::put(first->pcb);
		first = first->next;
		old->next = 0;
		delete old;
	}
	first = last = 0;
	size = 0;
	unlock
}

void PCBList::updateWaitingThreads(KernelSem *s) {
	Elem *cur = first;
	while (cur) {
		if (cur->pcb->waitingOnTime == 1 && --cur->pcb->timeToWait == 0) {
			ID toRemove = cur->pcb->id;
			cur->pcb->waitingOnTime = 0;
			cur->pcb->status = PCB::READY;
			Scheduler::put(cur->pcb);
			cur = cur->next;
			s->value++;
			s->blocked->removeById(toRemove);
		} else
			cur = cur->next;
	}
}

void PCBList::blockSignalGlobally(SignalId signal) {
	Elem *cur = first;
	while (cur) {
		cur->pcb->blockSignal(signal);
		cur = cur->next;
	}
}

void PCBList::unblockSignalGlobally(SignalId signal) {
	Elem *cur = first;
	while (cur) {
		cur->pcb->unblockSignal(signal);
		cur = cur->next;
	}
}

