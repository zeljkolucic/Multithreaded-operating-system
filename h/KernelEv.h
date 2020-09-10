/*
 * KernelEv.h
 *
 *  Created on: Aug 19, 2020
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "Event.h"

class PCB;
class IVTEntry;

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait();
	void signal();

private:
	PCB* owner;
	IVTEntry *myEntry;
	int value;
};



#endif /* KERNELEV_H_ */
