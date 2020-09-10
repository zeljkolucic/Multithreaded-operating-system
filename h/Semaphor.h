/*
 * Semaphor.h
 *
 *  Created on: Aug 16, 2020
 *      Author: OS1
 */

#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_

typedef unsigned int Time;

class KernelSem;
class SemList;

class Semaphore {
public:
	Semaphore(int init = 1);
	virtual ~Semaphore();

	virtual int wait(Time maxTimeToWait);
	virtual int signal(int n = 0);

	int val() const; // Returns the current value of the semaphore
	void updateWaitingThreads();

	static SemList *allSemaphores;

	friend class SemList;
	friend class System;

private:
	KernelSem* myImpl;
};

#endif /* SEMAPHOR_H_ */
