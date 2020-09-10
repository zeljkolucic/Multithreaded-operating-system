/*
 * SemList.cpp
 *
 *  Created on: Aug 16, 2020
 *      Author: OS1
 */

#include "SemList.h"
#include "Semaphor.h"
#include "System.h"
#include "PCBList.h"
#include "KernelS.h"
#include "PCB.h"
#include "SCHEDULE.H"

SemList::SemList() : first(0), last(0), size(0) {}

SemList::~SemList() {
	lock
	Elem *old = 0, *cur = first;
	while (cur) {
		old = cur;
		cur = cur->next;
		old->next = 0;
		delete old;
	}
	first = last = 0;
	size = 0;
	unlock
}

void SemList::add(Semaphore *s) {
	lock
	Elem *newElem = new Elem(s);
	if (first == 0)
		first = newElem;
	else
		last->next = newElem;
	last = newElem;
	size++;
	unlock
}

void SemList::remove(Semaphore *s) {
	lock
	Elem *prev = 0, *cur = first;
	while (cur && cur->sem != s) {
		prev = cur;
		cur = cur->next;
	}
	if (cur) {
		if (prev)
			prev->next = cur->next;
		else
			first = cur->next;
		if(cur == last)
			last = prev;
		if(first == 0)
			last = 0;
		cur->next = 0;
		delete cur;
		size--;
	}
	unlock
}

void SemList::updateWaitingThreads() {
	lock
	Elem *cur = first;
	while(cur) {
		cur->sem->updateWaitingThreads();
		cur = cur->next;
	}
	unlock
}

void SemList::removeThreadById(ID id) {
	Elem *cur = first;
	while(cur) {
		cur->sem->myImpl->blocked->removeById(id);
		cur = cur->next;
	}
}

