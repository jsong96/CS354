
////////////////////////////////////////////////////////////////////////////////
// Main File:        cache2Dcols.c
// This File:        cache2Dcols.c
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


int arr2D[3000][500];

int main() {
    
    for (int col = 0; col < 500; col++) {
        for (int row = 0; row < 3000; row++) {
            arr2D[row][col] = row + col;
        }
    }


    return 0;
}
