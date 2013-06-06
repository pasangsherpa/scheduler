/*
 * keyThread.C
 *
 *  Created on: June 5, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "global.h"
#include "queue.h"
#include "interrupt.h"
#include "interruptController.h"
#include "keyThread.h"

KeyThreadPtr KeyThreadConstructor(ICPtr ic) {
	KeyThreadPtr key = (KeyThreadPtr) malloc(sizeof(KeyThreadStr));
	key -> interruptController = ic;
	key -> c = '?';

	pthread_create(&key -> key_thread, NULL, KeyRun, (void *) key);
	return key;
}

void KeyThreadDestructor(KeyThreadPtr this) {
	pthread_cancel(this -> key_thread);
	free(this);
}

void *KeyRun(void *args) {
	KeyThreadPtr key = (KeyThreadPtr) args;

	while (true) {
		key -> c = getchar();
		interruptCPU(key -> interruptController, KEYBOARD_SERVICE_REQ, key -> c);
	}
}


/*
int main() {
	ICPtr ic = (ICPtr) ICConstructor();
	KeyThreadPtr key = (KeyThreadPtr) KeyThreadConstructor(ic);

	while (true) {
		if (isInterruptWaiting(key->interruptController)) {
			printf("\nChar %c pressed", retrieveInterrupt(key->interruptController)->kb_data);
			getchar();
		}
	}
}
*/