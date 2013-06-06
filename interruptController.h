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


 typedef struct IC {
	Queue interruptQueue;
	interruptPtr timer;

} ICStr, *ICPtr;

// Constructor
ICPtr ICConstructor();

// Returns bool true if there is an interrupt waiting to be serviced.
bool isInterruptWaiting(ICPtr this);

// Returns the interrupt that is waiting.
interruptPtr retrieveInterrupt(ICPtr this);

// Used by processes and devices to signal an interrupt
void interruptCPU(ICPtr this, int the_IRQ, char the_data);
