/*
 * global.h
 *
 *  Created on: May 12, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */

#ifndef GLOBAL_H
#define GLOBAL_H


/********* GENERAL **********/
#define NO_ERROR			0
#define QUEUE_SIZE 			50
#define MAX_PROCESS			50
typedef enum { false, true } bool;
/******* END GENERAL ********/

/********* PROCESS **********/
//Process Type
#define COMPUTE 			0
#define IO 					1
#define KEYBOARD 			2
#define PRODUCERS			3
#define CONSUMERS			4
//Process State
#define RUNNING 			0
#define	READY				1
#define INTERRUPED			2
#define BLOCKED				3
/******* END PROCESS ********/

/******TRAP VECTOR TABLE*****/
#define TIMER_INT		7
#define IO_INT			6
#define KEYBRD_INT		8
#define	IO_REQ			5
#define MUTEX_LOCK		4
#define MUTEX_UNLOCK	3	
#define COND_WAIT		2
#define COND_SIGNAL		1
/*******END TRAP************/

#endif /* GLOBAL_H */

