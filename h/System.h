/*
 * System.h
 *
 *  Created on: Aug 12, 2020
 *      Author: OS1
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "Thread.h"
#include "Idle.h"

class PCB;

extern void tick();

typedef void interrupt (*pInterrupt)(...);

#define lock asm pushf; asm cli; {}
#define unlock asm popf; {}

static volatile unsigned tss;
static volatile unsigned tsp;
static volatile unsigned tbp;

class System {
public:

	static Thread *mainThread;
	static Idle *idleThread;

	static pInterrupt old;

	static void interrupt timer(...);
	static void init();
	static void restore();

	static volatile int contextSwitchOnDemand;

	static int lockFlag;

};

#endif /* SYSTEM_H_ */
