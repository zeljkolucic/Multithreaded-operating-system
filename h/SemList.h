/*
 * SemList.h
 *
 *  Created on: Aug 16, 2020
 *      Author: OS1
 */

#ifndef SEMLIST_H_
#define SEMLIST_H_

class Semaphore;
#include "Thread.h"

class SemList {
public:
	SemList();
	~SemList();

	void add(Semaphore *s);
	void remove(Semaphore *s);
	void updateWaitingThreads();
	void removeThreadById(ID id);

private:
	struct Elem {
		Semaphore *sem;
		Elem *next;
		Elem(Semaphore *s) : sem(s), next(0) {}
	};

	Elem *first, *last;
	int size;
};



#endif /* SEMLIST_H_ */
