/*
 * process.c
 *
 *  Created on: May 12, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Goktruk
 *
 */

#include "process.h"

#ifndef GLOBAL_H
#include "global.h"
#endif

//Constructor
ProcessPtr ProcessConstructor(int pid, int proc_type, int no_steps, int no_requests) {
	ProcessPtr process = (ProcessPtr)malloc(sizeof(ProcessStr));
	process->proc_type = proc_type;
	process->no_steps = no_steps;
	process->no_requests = no_steps;
	process->pcb = PCBConstructor(pid, process);
	return process;
}

//Destructor.
int ProcessDestructor(ProcessPtr this) {
	this->pcb->destroy(this->pcb);
	free(this);
	return NO_ERROR;
}
