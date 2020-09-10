/*
 * System.cpp
 *
 *  Created on: Aug 12, 2020
 *      Author: OS1
 */
#include "System.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "Idle.h"
#include "Semaphor.h"
#include "SemList.h"
#include <dos.h>
#include "PCBList.h"

volatile int System::contextSwitchOnDemand = 0;
Thread * System::mainThread = 0;
Idle * System::idleThread = 0;
pInterrupt System::old = 0;
int System::lockFlag = 0;

void interrupt System::timer(...) {
	if(contextSwitchOnDemand == 0) {
		Semaphore::allSemaphores->updateWaitingThreads();
		tick();
		asm int 60h;
	}
	if(contextSwitchOnDemand == 0 && PCB::running->timeToUse != 0)
		PCB::running->timeLeft--;
	if(contextSwitchOnDemand == 1 || (PCB::running->timeToUse != 0 && PCB::running->timeLeft == 0)) {
		if(lockFlag == 0) {
			contextSwitchOnDemand = 0;
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			if(PCB::running != idleThread->myPCB && PCB::running->status == PCB::READY)
				Scheduler::put((PCB*) PCB::running);

			PCB::running = Scheduler::get();

			if(PCB::running == 0)
				PCB::running = idleThread->myPCB;

			PCB::running->timeLeft = PCB::running->timeToUse;

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}

			if(PCB::running->id != idleThread->myPCB->id)
				PCB::running->myThread->myPCB->handleAll();

		} else
			contextSwitchOnDemand = 1;
	}
}

void System::init() {
	lock

#ifndef BCC_BLOCK_IGNORE
	old = getvect(0x8);
	setvect(0x8, &timer);
	setvect(0x60, old);
#endif

	mainThread = new Thread();
	PCB::running = (volatile PCB*) mainThread->myPCB;
	PCB::running->status = PCB::READY;

	idleThread = new Idle();
	idleThread->myPCB->status = PCB::READY;

	unlock
}

void System::restore() {
	lock

#ifndef BCC_BLOCK_IGNORE
	setvect(0x8, old);
#endif

	delete mainThread;
	delete idleThread;
	delete PCB::listOfAllPCB;
	delete Semaphore::allSemaphores;

	unlock
}

