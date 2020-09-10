/*
 * SignList.cpp
 *
 *  Created on: Aug 25, 2020
 *      Author: OS1
 */

#include "SignList.h"
#include "PCB.h"
#include "HandList.h"

SignalList::SignalList() :
		size(0), first(0), last(0) {

}

SignalList::~SignalList() {
	while (first) {
		Elem *old = first;
		first = first->next;
		old->next = 0;
		delete old;
	}
	first = last = 0;
	size = 0;
}

void SignalList::add(SignalId id) {
	Elem *newElem = new Elem(id);
	if (first == 0)
		first = newElem;
	else
		last->next = newElem;
	last = newElem;
	size++;
}

int SignalList::getSize() const {
	return size;
}

void SignalList::removeById(SignalId id) {
	Elem *prev = 0, *cur = first;
	while (cur) {
		if (cur->id == id) {
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
			return;
		} else {
			prev = cur;
			cur = cur->next;
		}
	}
}

void SignalList::handleAll() {
	int zeroEntryToHandle = 0;
	Elem *cur = first;
	while (cur) {
		if (PCB::running->blockedSignals[cur->id] == 0) {
			SignalId toHandle = cur->id;
			cur = cur->next;
			if (toHandle == 0)
				zeroEntryToHandle = 1;
			else {
				PCB::running->handlers[toHandle]->handle();
				removeById(toHandle);
			}
		} else
			cur = cur->next;
	}
	if (zeroEntryToHandle == 1) {
		zeroEntryToHandle = 0;
		PCB::zeroEntryHandler();
	}
}

void SignalList::inherit(SignalList *sl) {
	Elem *cur = sl->first;
	while (cur) {
		add(cur->id);
		cur = cur->next;
	}
}

