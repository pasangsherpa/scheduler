/*
 * interrupt.h
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

interruptPtr interruptConstructor(int irq, char data);
void interruptDestructor(interruptPtr this);
