/*
 * memSlot.c
 *
 *  Created on: May 26, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "memSlot.h"


MemSlotPtr MemSlotConstructor() {
	MemSlotPtr  memSlot = (MemSlotPtr) malloc(sizeof(MemSlot));
	return memSlot;
}

void MemSlotDestructor(MemSlotPtr this) {
	free(this);
}

void MSset(MemSlotPtr ms, void *obj) {
	ms -> Slot = obj;
}

void * MSget(MemSlotPtr ms) {
	return ms -> Slot;
}


//int main() {
//	MemSlotPtr mem = (MemSlotPtr) MemSlotConstructor();
//	return 0;
//}
