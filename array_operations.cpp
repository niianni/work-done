#include "array_operations.hh"
#include <iostream>
#include <string>


int greatest_v1(int* itemptr, int size){
      int* pointer = itemptr;
      int greatest = *pointer;
      while ( pointer < itemptr+size ) {
        if ( *pointer > greatest ) {
            greatest = *pointer;
        }
        ++pointer;
    }
    return greatest;
}



int greatest_v2(int* itemptr, int* endptr) {
    int* pointer = itemptr;
    int greatest = *pointer;
    while ( pointer != endptr ) {
       if ( *pointer > greatest ) {
            greatest = *pointer;
        }
        ++pointer;
    }
    return greatest;    
}

void copy(int* itemptr, int* endptr, int* targetptr) {
    int size = endptr-itemptr;
    for ( int i = 0; i < size; ++i ){
        *(targetptr + i) = *(itemptr + i);
    }
}

void reverse(int* leftptr, int* rightptr){
    int size = rightptr-leftptr;

    for ( int i = 0; i < (size/2); ++i ) {
        int num1 = *(leftptr+i);
        int num2 = *(rightptr-1-i);
        *(leftptr+i) = num2;
        *(rightptr-1-i) = num1;
    }
}
