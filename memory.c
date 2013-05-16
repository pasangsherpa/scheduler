/*
 * memory.c
 *
 *  Created on: May 12, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Goktruk
 *
 */

#include "memory.h"

MemoryPtr MemoryConstructor() {
	MemoryPtr mem = (MemoryPtr)malloc(sizeof(MemoryStr));

	return mem;
}
