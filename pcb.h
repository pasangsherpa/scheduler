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
#ifndef PCB_H
#define PCB_H

typedef struct pcb {
	int	pid;
	int next_step;		// step count that the CPU had gotten to when this process was
						// prempted (like a PC register value)
	int state;			// e.g. 0=running, 1=ready, 2=interrupted, 3=blocked
	ProcessPtr process; // pointer to the actual process
	int waiting_on;		// which queue is it in if it is waiting on something (blocked)
	int owns;			// which mutex lock does it own

	//Pointers to functions.
	int(*destroy)(struct pcb*);


} PCBStr, *PCBPtr;

PCBPtr PCBConstructor(int process_id, ProcessPtr process);
int PCBDestructor(PCBPtr this);
int saveContext(PCBPtr this);
#endif /* PCB_H */
