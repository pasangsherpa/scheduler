/*
 * interrupt.c
 *
 *  Created on: June 4, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "interrupt.h"

 // Constructor
 interruptPtr interruptConstructor(int irq, char data) {
  	interruptPtr interrupt = (interruptPtr) malloc(sizeof(interruptStr));
  	interrupt->the_irq = irq;
  	interrupt->kb_data = data;

  	return interrupt;
}

void interruptDestructor(interruptPtr this) {
 	free(this);
}

/*
int main() {
	interruptPtr i = (interruptPtr) interruptConstructor(0, '0');
}
*/