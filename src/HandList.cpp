/*
 * HandList.cpp
 *
 *  Created on: Aug 25, 2020
 *      Author: OS1
 */

#include "HandList.h"
#include "System.h"

HandlerList::HandlerList() :
		size(0), first(0), last(0) {

}

HandlerList::~HandlerList() {
	unregisterAllHandlers();
}

void HandlerList::registerHandler(SignalHandler sh) {
	Elem *newElem = new Elem(sh);
	if (first == 0)
		first = newElem;
	else
		last->next = newElem;
	last = newElem;
	size++;
}

int HandlerList::getSize() const {
	return size;
}

void HandlerList::unregisterAllHandlers() {
	while (first) {
		Elem *old = first;
		first = first->next;
		old->next = 0;
		delete old;
	}
	first = last = 0;
	size = 0;
}

void HandlerList::handle() {
	Elem *cur = first;
	while (cur) {
		System::lockFlag = 1;
		(*cur->handler)();
		System::lockFlag = 0;
		cur = cur->next;
	}
}

void HandlerList::swap(SignalHandler hand1, SignalHandler hand2) {
	Elem *firstHandler = 0, *secondHandler = 0;
	Elem *cur = first;
	while (cur) {
		if (cur->handler == hand1)
			firstHandler = cur;
		else if (cur->handler == hand2)
			secondHandler = cur;
		if (firstHandler != 0 && secondHandler != 0)
			break;
		cur = cur->next;
	}
	if (firstHandler != 0 && secondHandler != 0) {
		firstHandler->handler = hand2;
		secondHandler->handler = hand1;
	}
}

void HandlerList::inherit(HandlerList *hl) {
	Elem *cur = hl->first;
	while (cur) {
		registerHandler(cur->handler);
		cur = cur->next;
	}
}

