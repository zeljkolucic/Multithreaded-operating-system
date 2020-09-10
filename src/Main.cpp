/*
 * Main.cpp
 *
 *  Created on: Aug 13, 2020
 *      Author: OS1
 */

#include "System.h"
#include "PCB.h"
#include "PCBList.h"
#include <iostream.h>


extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {

	System::init();
	int returnValue = userMain(argc, argv);
	System::restore();
	return returnValue;

}
