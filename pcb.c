/*
 * pcb.h
 *
 *  Created on: May 12, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */
#include "pcb.h"

#ifndef GLOBAL_H
#include "global.h"
#endif

//Constructor
PCBPtr PCBConstructor(int pid, ProcessPtr process){
	PCBPtr pcb = (PCBPtr)malloc(sizeof(PCBStr));
	pcb->pid = pid;
	pcb->process = process;
	pcb->state = READY;
	pcb->next_step = 0;
	pcb->waiting_on = 0;
	pcb->owns = 0;

	//Pointers to functions.
	pcb->destroy = PCBDestructor;

	return pcb;
}

//Destructor.
int PCBDestructor(PCBPtr this) {
	free(this);
	return NO_ERROR;
}

