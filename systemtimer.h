/*
 * systemtimer.h
 *
 *  Created on: May 12, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */

#ifndef SYSTEMTIMER_H
#define SYSTEMTIMER_H
#include <pthread.h>

#ifndef GLOBAL_H
#include "global.h"
#endif

#ifndef INTERRUPTCONTROLLER_H
#include "interruptController.h"
#endif

typedef struct timer {
	ICPtr interruptController;
	pthread_t clock_thread;
	pthread_cond_t reset;				// Condition variable CPU uses to signal timer.
	pthread_mutex_t mutex;			// Unused mutex only for signaling.

} SysTimerStr, *SysTimerPtr;


/***************
** Constructor **
****************
* Returns a pointer to the new System Timer and starts the timer.
* reset - this condition variable needs to be signaled to start the clock running again
*				after an interrupt.
*/
SysTimerPtr SysTimerConstructor(ICPtr, pthread_cond_t reset);

/**************
** Destructor **
***************
* The System timer that is pointed to will be destroyed.
*/
void SysTimerDestructor(SysTimerPtr);

/*******************
***** Functions *****
*******************/

// The function that runs the clock.
// Called from constructor- shouldnt be called from anywhere else.
void *SysTimerRun(void *);


#endif /* SYSTEMTIMER_H */
