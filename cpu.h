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

#ifndef PROCESS_h
#include "process.h"
#endif

typedef struct cpu {
        int PC; //Program counter
        MemoryPtr memory;
        SchedulerPtr scheduler;
        int no_processes; //Number of processes
        ProcessPtr *process_list;
        int max_step_Count; //Number of steps this CPU will run.
		Queue keyboard_queue;
        
} CPUStr, *CPUPtr;

// Constructor
CPUPtr CPUConstructor(int no_processes, int prod_cons_pair, int cycle);

// Destructor
int CPUDestructor(CPUPtr this);

//METHODS

int keyboardServiceRequest(CPUPtr this, SchedulerPtr the_pcb);
int keyboardServiceCompleted(CPUPtr this, char the_keyboard_data);
int poll(CPUPtr this); //thread	
int timer_int_handler(CPUPtr this,SchedulerPtr scheduler);


//int mutex_lock_trap_handler(CPUPtr this,MutexPtr mutex); 
//int mutex_unlock_trap_handler(CPUPtr this, ProcessPtr process); //assumption: we know which process
                                                                                                                                //is waiting on which lock+ a process can only be in one queue.
int cond_wait_trap_handler(CPUPtr this);//task:Condition pointer
int cond_signal_trap_handler(CPUPtr this);

#endif /* CPU_H */