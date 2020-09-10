/*
 * SignList.h
 *
 *  Created on: Aug 25, 2020
 *      Author: OS1
 */

#ifndef SIGNLIST_H_
#define SIGNLIST_H_

#include "Thread.h"

class SignalList {
public:
	SignalList();
	~SignalList();
	void add(SignalId id);
	int getSize() const;
	void removeById(SignalId id);
	void handleAll();
	void inherit(SignalList *sl);

private:
	struct Elem {
		SignalId id;
		Elem *next;
		Elem(SignalId sid) : id(sid), next(0) {}
	};

	Elem *first, *last;
	int size;

};

#endif /* SIGNLIST_H_ */
