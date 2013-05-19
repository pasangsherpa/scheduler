/*
 * cpu.h
 *
 *  Created on: May 12, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
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
	int max_step_Count; //Number of steps this CPU will run.
	int INT; //Whether INT line is asserted or not (0,1)
	int int_enabled; //Current state of the CPU, whether it is servicing an INT	

} CPUStr, *CPUPtr;

// Constructor
CPUPtr CPUConstructor();

// Destructor
void CPUDestructor(CPUPtr this);

//METHODS
int checkINT(CPUPtr this);
int saveState(CPUPtr this, ProcessPtr currentProcess);
int enableINT(CPUPtr this);
int disableINT(CPUPtr this);
int timer_int_handler(CPUPtr this,SchedulerPtr scheduler);
int io_int_handler(CPUPtr this); //task: IO device queue
int io_req_trap_handler(CPUPtr this, SchedulerPtr scheduler);//task:I/O device queue
int mutex_lock_trap_handler(CPUPtr this,MutexPtr mutex); 
int mutex_unlock_trap_handler(CPUPtr this, ProcessPtr process); //assumption: we know which process
																//is waiting on which lock+ a process can only be in one queue.
int cond_wait_trap_handler(CPUPtr this);//task:Condition pointer
int cond_signal_trap_handler(CPUPtr this);

#endif /* CPU_H */
