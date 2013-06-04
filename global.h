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
#define ERROR				-1
#define QUEUE_SIZE 			50
#define MAX_PROCESS			50
#define QUANTA	1
#define IO_QUANTA_LOWER 0.1
#define IO_QUANTA_UPPER 0.5
typedef enum { false, true } bool;
/******* END GENERAL ********/

/********* PROCESS **********/
//Process Type
#define COMPUTE 			0
#define KEYBOARD 			1
#define PRODUCER			2
#define CONSUMER			3
#define IO_AUDIO				4
#define IO_VIDEO 			5
#define IO_DISK				6
//Process State
#define RUNNING 			0
#define	READY				1
#define INTERRUPTED			2
#define BLOCKED				3
//Process Messages
/******* END PROCESS ********/

/******TRAP AND INT(KERNEL SERVICE)VECTOR TABLE*****/
#define TIMER_INT				1 //represents a memory address
#define VIDEO_SERVICE_REQ		2
#define VIDEO_SERVICE_COMPLETED	3
#define KEYBOARD_SERVICE_REQ	4
#define KEYBOARD_COMPLETED		5
#define	AUDIO_SERVICE_REQ		6
#define AUDIO_SERVICE_COMPLETED	7
#define MUTEX_LOCK				8
#define MUTEX_UNLOCK			9
#define COND_WAIT				10
#define COND_SIGNAL				11
/*******END KERNEL SERVICE***************************/

/*******IRQ LINES***********/
#define IRQ_KEYBOARD	1
#define	AUDIO			2
#define	VIDEO			3
#define TIMER			4
/*******IRQ LINES**********/

/*******INT PRIORITY LEVELS***/
#define TIMER_P		2
#define	KEYBRD_P	0
#define AUDIO_P		1
#define VIDEO_P		1
/*******INT PRIORITY LEVELS***/

/*******CONDITION VARIABLES****/
#define NOT_FULL	1
#define NOT_EMPTY	0
/*******CONDITION VARIABLES***/

#endif /* GLOBAL_H */

