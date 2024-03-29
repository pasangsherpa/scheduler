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
#include <stdlib.h>
#include "pcb.h"

#ifndef GLOBAL_H
#include "global.h"
#endif

//Constructor
PCBPtr PCBConstructor(int pid){
	PCBPtr pcb = (PCBPtr)malloc(sizeof(PCBStr));
	pcb->pid = pid;
	pcb->state = READY;
	pcb->next_step = -1;
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

