/*
 * Event.cpp
 *
 *  Created on: Aug 19, 2020
 *      Author: OS1
 */

#include "Event.h"
#include "KernelEv.h"
#include "System.h"


Event::Event(IVTNo ivtNo) {
	lock
	myImpl = new KernelEv(ivtNo);
	unlock
}

Event::~Event() {
	lock
	delete myImpl;
	unlock
}

void Event::wait() {
	lock
	myImpl->wait();
	unlock
}

void Event::signal() {
	lock
	myImpl->signal();
	unlock
}

