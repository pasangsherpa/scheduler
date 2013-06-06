/*
 * interruptController.h
 *
 *  Created on: June 4, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */

 #ifndef GLOBAL_H
 #include "global.h"
#endif


 typedef struct interrupt {
	int the_irq;
	char kb_data;				// Data from a keyboard interrupt.

} interruptStr, *interruptPtr;

 typedef struct interruptController {
	Queue interruptQueue;
	interrupt timer;

} ICStr, *ICPtr;


// Returns bool true if there is an interrupt waiting to be serviced.
bool isInterruptwaiting(ICPtr this);

// Returns the interrupt that is waiting.
interrupt retrieveInterrupt(ICPtr this);

// Used by processes and devices to signal an interrupt
void interruptCPU(ICPtr this, int the_IRQ, char the_data)
