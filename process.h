/*
 * process.h
 *
 *  Created on: May 12, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */

#ifndef PROCESS_H
#define PROCESS_H

#ifndef PCB_H
#include "pcb.h"
#endif

typedef struct process {
	PCBPtr pcb;
	int proc_type;       // code for process type, e.g. 0=compute, 1=i0, 2=keyboard, etc.
	int	no_steps;		// number of time steps before resetting to 0 - number of instructions
	int no_requests;	// number of requests that will be generated during the process run
	int *requests;		// an array of requests, each request (e.g. io service) is issued at a specific
						// time step. These are the synchronous events that result in traps being issued.
						// You can get fancy and make this an array of RequestTypeStr which contains the
						// step number when issued, and a request type (e.g. 0=io, 1=sync, etc.)
} ProcessStr, *ProcessPtr;

ProcessPtr ProcessConstructor(int pid, int proc_type, int no_steps, int no_requests);
int ProcessDestructor(ProcessPtr this);
int run(ProcessPtr this);
#endif /* PROCESS_H */
