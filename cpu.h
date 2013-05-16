/*
 * cpu.h
 *
 *  Created on: May 12, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Goktruk
 *
 */

#ifndef CPU_H
#define CPU_H

#ifndef MEMORY_H
#include "memory.h"
#endif

#ifndef SCHEDULER_H
#include "scheduler.h"
#endif

typedef struct cpu {
	int PC; //Program counter
	MemoryPtr memory;
	SchedulerPtr scheduler;
	int no_processes; //Number of processes
	ProcessPtr *process_list;

	//Pointers to functions.

} CPUStr, *CPUPtr;

#endif /* CPU_H */
