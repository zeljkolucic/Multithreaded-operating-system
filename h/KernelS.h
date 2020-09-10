/*
 * KernelS.h
 *
 *  Created on: Aug 20, 2020
 *      Author: OS1
 */

#ifndef KERNELS_H_
#define KERNELS_H_

#include "Semaphor.h"

class PCBList;

class KernelSem {
public:
	KernelSem(int init);
	~KernelSem();
	int wait(int maxTimeToWait);
	int signal(int n);
	int getValue() const;
	void updateWaitingThreads();

	friend class PCB;
	friend class PCBList;
	friend class SemList;

private:
	void unblock();

	int value;
	PCBList *blocked;
};



#endif /* KERNELS_H_ */
