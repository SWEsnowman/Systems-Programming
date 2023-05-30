#include "insertion.h"
#include <string.h>

/*
	We emphasize this: because this is a generic implementation, in insertion.c you must not use if-else to
	discriminate different data types in your code. Your code must be able to deal with any data type possible.
	Therefore, we limit the usage of the following primitive types:
	- int or size_t: only allowed as loop iterators;
	- char or u_int8_t: can be used as you wish;
	- Types that are used in the declarations of iSort() and iPrint(): feel free to use them for their _original_
      purpose;
	- All other types are not allowed to appear in insertion.c: no exceptions, no matter if you actually used
	  them, or what purpose it is. Once it appears, you will receive 30% reduction of the homework.

	You are free to create helper functions only in insertion.c; you must declare and implement them in the
    .c file instead of the header files. Do not change any of the header files.
	
*/

void swap (void *p, void *p2, const size_t width) {
    char *buffer = (char *)malloc(sizeof(char)*width);
    memcpy(buffer, p, width);
    memcpy(p, p2, width);
    memcpy(p2, buffer, width);
    free(buffer);
}

void iSort(void* base, size_t nel, size_t width, int (*compare)(void*,void*)) {
	char * basepointer = (char *) base;
	for(int i = 1; i < nel; i++){
		char * pointer = basepointer;
		char * pointer2 = ((char *) base) + (i)*width;
		for(int j = (i-1); j>=0; j--){
			if((* compare)((void *) pointer,(void *) pointer2) == 1){
				swap((void *)pointer, (void*)pointer2,width);
			}
			pointer = pointer - width;
			pointer2 = pointer2 - width;
		}
		basepointer =  basepointer + width;
	}
	
	
}


void iPrint(void* base, size_t nel, size_t width, void (*print)(void*)) {
	char * basepointer = (char *) base;
	for(int i = 0; i < nel; i++){
		(*print)((void*) basepointer);
		basepointer = basepointer + width;
	}
	
	
}