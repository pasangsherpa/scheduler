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