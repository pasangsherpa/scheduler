/*
 * global.h
 *
 *  Created on: May 12, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Goktruk
 *
 */

#ifndef GLOBAL_H
#define GLOBAL_H

/********* GENERAL **********/
#define NO_ERROR			0
#define QUEUE_SIZE 			50
#define MAX_PROCESS			50
/******* END GENERAL ********/

/********* PROCESS **********/
//Process Type
#define COMPUTE 			0
#define IO 					1
#define KEYBOARD 			2
#define PRODUCER			3
#define CONSUMER			4
//Process State
#define RUNNING 			0
#define	READY				1
#define INTERRUPED			2
#define BLOCKED				3
/******* END PROCESS ********/

#endif /* GLOBAL_H */

