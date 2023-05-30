#include "utils.h"
#include <stdlib.h>


/*
	You are free to use any data type you like in this file.
    However, other than the five functions declared in "utils.h",
    DO NOT create any other functions.
	
*/

int cmpr_int(void* a, void* b) {
	
	int testa = *(int*) a;
	int testb = *(int*) b;
	if(testa > testb){
		return 1;
	}else if(testa == testb){
		return 0;
	}else{
		return -1;
	}
}

int cmpr_float(void* a,void* b) {
	
	float testa = *(float*) a;
	float testb = *(float*) b;
	if(testa > testb){
		return 1;
	}else if(testa == testb){
		return 0;
	}else{
		return -1;
	}
}

void print_int(void* a) {
	int printa = *(int*) a;
	printf("%d\n", printa);
}

void print_float(void* a) {
	float printa = *(float*) a;
	printf("%f\n", printa);
}


void* read_array(char* filename, char* format, size_t* len) {
	FILE* file = fopen(filename,"r");
	if(file == NULL){
		perror("The file doesn't exist.");
		exit(EXIT_FAILURE);
	}
	FILE* lenfile = fopen(filename,"r");
	size_t length = *len;
	int num_lines = 1;
	int c = fgetc(lenfile);
	while(!feof(lenfile)){
	  if(c == '\n'){
		num_lines++;
	  }
	  c = fgetc(lenfile);
   }
   	fclose(lenfile);
   	void * array;
	int iterator;
   	if (format[1] == 'd'){
		array = malloc(sizeof(int)*num_lines);
		iterator = sizeof(int);
   	} else if(format[1] == 'f'){
		array = malloc(sizeof(float)*num_lines);
		iterator = sizeof(float);
	}
	void * start = array;
	while(fscanf(file, format, array) != EOF){ 

		length++;
		array = (char*)array + iterator;
	}
	*len = length;
	fclose(file);
	return start;
}