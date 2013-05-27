/*
 * memSlot.h
 *
 *  Created on: May 26, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */

 typedef struct {
 	void *Slot;
 } MemSlot, *MemSlotPtr;

// Constructor
MemSlotPtr MemSlotConstructor();

// Destructor
void MemSlotDestructor(MemSlotPtr);

/*******************************************
//Functions
*************************************/

// Sets the Memory slot to the passed in address.
void MSset(MemSlotPtr ms, void *obj);

// Retrieves the pointer that the Memory slot is pointing to.
void * MSget(MemSlotPtr ms);