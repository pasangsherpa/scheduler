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

typedef struct timer {
	CPUPtr cpu;
	pthread_t clock_thread;

} SysTimerStr, *SysTimerPtr;


/***************
** Constructor **
****************
* Returns a pointer to the new System Timer and starts the timer.
*/
SysTimerPtr SysTimerConstructor();

/**************
** Destructor **
***************
* The System timer that is pointed to will be destroyed.
*/
void SysTimerDestructor(SysTimerPtr);

/******************************************************************

/*******************
***** Functions *****
*******************/

// The function that runs the clock.
// Called from constructor- shouldnt be called from anywhere else.
void *SysTimerRun(void *);


#endif /* SYSTEMTIMER_H */
