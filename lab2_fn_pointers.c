// Credit: Shudong Hao

#include <stdio.h>

// TODO: addone() must accept parameters that include a function pointer,
// the number of elements in the array and the size of each element
// it must NOT accept any parameters directly revealing the data type

// THEN: add the functionality to find the maximum in an array. 
// The function should return the index of the maximum in the array.
// Implementing it for one type is sufficient.

void increment_int (void * a) {
    (*(int*)a)++;
}
void increment_double(void * a){
    (*(double*)a)++;
}
void increment_char(void * a){
    (*(char*)a)++;
}

void addone (void (*func)(void *elem), int num_elements, int bytes, const void * arr) {
    char * pointer = (char * )arr;
    for(int i = 0; i < num_elements; i++){
        (*func)((void *)pointer);
        pointer = pointer + bytes; 

    }
    
}

int main() {
    int test1[] = {1,2,3,4,5};
    addone(increment_int, 5, 4, &test1);
    for (size_t i = 0; i < 5; i ++) printf("%d\n", test1[i]);

    double test2[] = {0.3, -0.2, 1.34};
    addone(increment_double, 3, 8, &test2);
    for (size_t i = 0; i < 3; i ++) printf("%lf\n", test2[i]);

    char test3[] = {'3', 'a', 'Z', '?'};
    addone(increment_char,4,1,&test3);
    for (size_t i = 0; i < 4; i ++) printf("%c\n", test3[i]);

    return 0;
}


