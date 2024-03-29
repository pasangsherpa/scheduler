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

#ifndef SCHEDULER_H
#include "scheduler.h"
#endif

#ifndef PROCESS_h
#include "process.h"
#endif

#ifndef PCB_H
#include "pcb.h"
#endif

#ifndef INTERRUPTCONTROLLER_H
#include "interruptController.h"
#endif

#ifndef GLOBAL_H
#include "global.h"
#endif

#ifndef QUEUE_H
#include "queue.h"
#endif

#ifndef IODEVICE_H
#include "iodevice.h"
#endif

#ifndef SYSTEMTIMER_H
#include "systemtimer.h"
#endif

#ifndef KEYTHREAD_H
#include "keyThread.h"
#endif

typedef struct cpu {
        int PC; 				//Program counter
		int max_step_count; 	//Number of steps this CPU will run.
        SchedulerPtr scheduler;
		int process_pid;
		char buffer_data;
		ProcessPtr current_process;	//currently running process pcb
		ICPtr interruptController;
		SysTimerPtr timer;
		KeyThreadPtr key;
		pthread_cond_t reset;

} CPUStr, *CPUPtr;

// Constructor
CPUPtr CPUConstructor();

// Destructor
int CPUDestructor(CPUPtr this);

//METHODS
//main thread
void runCPU(CPUPtr this);
void initCPU (CPUPtr this, int totalProcess, int totalKBProcess, int totalIOProcess,
int totalPrCoProcess, int totalComputeProcess, int the_max_step_count);
//helper methods
void setNextProcess(CPUPtr this);
void keyboardServiceRequest(CPUPtr this);
void keyboardServiceCompleted(CPUPtr this);
void getKey(CPUPtr this); //
void printState(CPUPtr this);
//void printQueue(Queue the_queue);


//int mutex_lock_trap_handler(CPUPtr this,MutexPtr mutex);
//int mutex_unlock_trap_handler(CPUPtr this, ProcessPtr process); //assumption: we know which process
                                                                                                                                //is waiting on which lock+ a process can only be in one queue.
//int cond_wait_trap_handler(CPUPtr this);//task:Condition pointer
//int cond_signal_trap_handler(CPUPtr this);

#endif /* CPU_H */
