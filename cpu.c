/*
 * cpu.c
 *
 *  Created on: May 12, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */
#include "cpu.h"

#ifndef GLOBAL_H
#include "global.h"
#endif

CPUPtr CPUConstructer(int no_processes, int prod_cons_pair, int cycle) {
	CPUPtr cpu = (CPUPtr) malloc(sizeof(CPUStr));
	cpu->PC = 0;
	cpu->memory = MemoryConstructor();
	cpu->scheduler = SchedulerConstructor();
	cpu->no_processes = no_processes;
	cpu->process_list = (ProcessPtr*)(malloc(MAX_PROCESS *(sizeof(ProcessStr))));

	//Pointers to functions.

	return cpu;
}

//Destructor.
int CPUDestructor(CPUPtr this) {
	free(this);
	return NO_ERROR;
}
