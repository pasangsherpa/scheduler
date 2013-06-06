/*
 * interruptController.c
 *
 *  Created on: June 4, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include "global.h"
 #include "interrupt.h"
 #include "queue.h"
 #include "interruptController.h"

// Constructor
ICPtr ICConstructor() {
 	ICPtr ic = (ICPtr) malloc(sizeof(ICStr));
 	ic->interruptQueue = CreateQueue(QUEUE_SIZE);
 	ic->timer = NULL;

 	return ic;
}

// Destructor
void ICDestructor(ICPtr this) {
	DisposeQueue(this->interruptQueue);
 	free(this);
}


// If there is an interrupt waiting, true is returned.
bool isInterruptwaiting(ICPtr this) {
	bool result = false;

	if ((!IsEmpty(this->interruptQueue)) || (this->timer != NULL) {
		result = true;
	}
	return result;
}

// Returns the interrupt that is waiting.
interruptPtr retrieveInterrupt(ICPtr this) {
	if (this->timer != NULL) {
		interruptPtr interrupt =
				(interruptPtr) interruptConstructor(this->timer->the_irq, this->timer->kb_data);
		this->timer = NULL;
		return interrupt;
	} else {
		return FrontAndDequeue(this->interruptQueue);
	}
}

// Used by processes and devices to signal an interrupt
void interruptCPU(ICPtr this, int the_IRQ, char the_data) {

	// Create an interrupt.
	interruptPtr interrupt = (interruptPtr) interruptConstructor(the_IRQ, the_data);

	// Timer interrupts get the highest priority.
	if (the_IRQ == TIMER_INT) {
		this->timer = interrupt;
	} else {
		Enqueue(interrupt, this->interruptQueue);
	}
}

int main() {
	ICPtr ic = (ICPtr) ICConstructor();
}