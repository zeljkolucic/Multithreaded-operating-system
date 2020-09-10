/*
 * HandList.h
 *
 *  Created on: Aug 25, 2020
 *      Author: OS1
 */

#ifndef HANDLIST_H_
#define HANDLIST_H_

#include "Thread.h"

class HandlerList {
public:
	HandlerList();
	~HandlerList();
	void registerHandler(SignalHandler sh);
	void unregisterAllHandlers();
	int getSize() const;
	void handle();
	void swap(SignalHandler hand1, SignalHandler hand2);
	void inherit(HandlerList *hl);

private:
	struct Elem{
		SignalHandler handler;
		Elem *next;
		Elem(SignalHandler sh) : handler(sh), next(0) {}
	};

	Elem *first, *last;
	int size;

};



#endif /* HANDLIST_H_ */
