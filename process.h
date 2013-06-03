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

#ifndef REQUEST_H
#include "request.h"
#endif

#ifndef GLOBAL_H
#include "global.h"
#endif

typedef struct process {
	PCBPtr pcb;
	int proc_type;       // code for process type, e.g. 0=compute, 1=i0, 2=keyboard, etc.
	int	no_steps;		// number of time steps before resetting to 0 - number of instructions
	int no_requests;	// number of requests that will be generated during the process run
	RequestPtr * requests;		// an array of requests, each request (e.g. io service) is issued at a specific
						// time step. These are the synchronous events that result in traps being issued.
						// You can get fancy and make this an array of RequestTypeStr which contains the
						// step number when issued, and a request type (e.g. 0=io, 1=sync, etc.)
	int req_index;		//keeps track of the next call and step num.
} ProcessStr, *ProcessPtr;

/****************************************************************************
*Basic Constructor
***********************
* pid = the ID number for the process.
* proc_type = the type of process (see global.h).
* no_steps = the total number of steps the process takes to complete.
*						(Must be greater than or equal to no_requests or there is an infinite
*						 loop, as requests doesnt allow for duplicates).
* no_requests = the total number of request the process will make before completion.
***************************************************************************/
ProcessPtr ProcessConstructor(int pid, int proc_type, int no_steps, int no_requests);

// Basic Destructor
int ProcessDestructor(ProcessPtr this);

/***************************
*** FUNCTIONS ***********
***************************/

// Returns the step number of when the next interrupt will occur.
int getNextTrapStep(ProcessPtr this);

// Returns the trap code (see global.h) of the next interrupt.
int getNextTrapCode(ProcessPtr this);

// Advances the request array.
// Loops around if at the end of the array.
// This function should be called every time the PC == getNextTrapStep.
void advanceRequest(ProcessPtr this);

// The PC of the cpu is passed in.
// Retuns true if the PC is greater than or equal to no_steps.
bool isProcessDone(ProcessPtr this, int PC);

// Prints the predefined interrupt message for the process.
// Returns NO_ERROR if successful.
int printMessage(ProcessPtr this);

/***************************
*** HELPER FUNCTIONS ***
***************************/
int addToRequestArray(RequestPtr * req_array, int * num_array, int * proc_type, int the_length);

#endif /* PROCESS_H */
