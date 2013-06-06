/*
 * keyThread.h
 *
 *  Created on: June 5, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */
#ifndef KEYTHREAD_H
#define KEYTHREAD_H

#ifndef INTERRUPTCONTROLLER_H
#include "interruptController.h"
#endif

#ifndef GLOBAL_H
#include "global.h"
#endif

#ifndef QUEUE_H
#include "queue.h"
#endif

#ifndef INTERRUPT_H
#include "interrupt.h"
#endif

typedef struct key {
	 ICPtr interruptController;
	pthread_t key_thread;
 	char c;

} KeyThreadStr, *KeyThreadPtr;

// Constructor
KeyThreadPtr KeyThreadConstructor(ICPtr ic);

//Destructor
void KeyThreadDestructor(KeyThreadPtr this);


void *KeyRun(void *args);

#endif /* KEYTHREAD_H */

