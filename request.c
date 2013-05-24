/*
 * request.c
 *
 *  Created on: May 23, 2013
 *
 *  @author: Pasang Sherpa
 *  @author: Seth Kramer
 *  @author: Mars Gokturk
 *
 */
#include "request.h"
#include <stdlib.h>
#include <stdio.h>


RequestPtr RequestConstructor(int the_type, int the_step_no){
	RequestPtr result = (RequestPtr) malloc(sizeof(RequestStr));
	result->process_type = the_type;
	result->process_step = the_step_no;
	
	return result;
}
void RequestDeconstructor(RequestPtr this){
	free(this);	
}

int getRequestType(RequestPtr this){
	return this->process_type;
}

int getRequestStep(RequestPtr this){
	return this->process_step;
}

int comp(const void *a,const void *b) {
	int x = *(int *) a;
	int y = *(int *) b;
	if(x>y){return +1;}
	else if(x == y){return 0;}
	else{return -1;}	
}

//Sort function to help sort the step numbers.
int * sort(int * unsorted, int length){
	//sort
	qsort(unsorted, length, sizeof(int), comp);	
	return unsorted;
}


/*int main(void){
	
	int numberofcall = 4;
	//array of step numbers
	//sort them
	//and for each in the array, construct a request, and add them to the array
	//make a function to get next request.
	
	int  * num_array = malloc(4 * sizeof(int));
	RequestPtr * requests = malloc(4 * sizeof(RequestStr));
	
	num_array[0] = 2;
	num_array[1] = 29;
	num_array[2] = 19;
	
	for(int i = 0; i < 4 ; i++){
		printf("%d, ",num_array[i]);
	}
	
	//sort
	qsort(num_array, 4, 4, comp);
	
	for(int i = 0; i < 4 ; i++){
		printf("%d, ",num_array[i]);
	}
	
	//add to the request array
	for(int j = 0; j < 4 ; j++){
		requests[j] = RequestConstructor(2,num_array[j]);
		printf("request step no = %d\n", getRequestStep(requests[j]));
	}
	
	//get next request

	return 0;
}*/