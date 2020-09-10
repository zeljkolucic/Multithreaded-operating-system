/*
 * Semaphor.cpp
 *
 *  Created on: Aug 20, 2020
 *      Author: OS1
 */

#include "Semaphor.h"
#include "System.h"
#include "KernelS.h"
#include "SemList.h"

SemList* Semaphore::allSemaphores = new SemList();

Semaphore::Semaphore(int init) {
	lock
	myImpl = new KernelSem(init);
	allSemaphores->add(this);
	unlock
}

Semaphore::~Semaphore() {
	lock
	allSemaphores->remove(this);
	delete myImpl;
	unlock
}

int Semaphore::wait(Time maxTimeToWait) {
	lock
	int returnValue = myImpl->wait(maxTimeToWait);
	unlock
	return returnValue;
}

int Semaphore::signal(int n) {
	lock
	int returnValue = myImpl->signal(n);
	unlock
	return returnValue;
}

int Semaphore::val() const {
	lock
	int returnValue = myImpl->getValue();
	unlock
	return returnValue;
}

void Semaphore::updateWaitingThreads() {
	myImpl->updateWaitingThreads();
}


