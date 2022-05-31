
////////////////////////////////////////////////////////////////////////////////
// Main File:        cache2Dclash.c
// This File:        cache2Dclash.c
// Other Files:      -
// Semester:         CS 354 Spring 2020
//
// Author:           Jiwon Song
// Email:            jsong99@wisc.edu
// CS Login:         jis
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   
//////////////////////////// 80 columns wide ///////////////////////////////////


#include <stdio.h>

int arr2D[128][8];

int main() {
    for (int iter = 0; iter < 100; iter++) {
        for ( int row = 0; row < 128; row+=64) {
            for ( int col = 0; col < 8; col++) {
                arr2D[row][col] = iter + row + col;
            }
        }    
    }

    return 0;
}
