///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        makeMagic.c
// This File:        makeMagic.c
// Other Files:      -
// Semester:         CS 354 Spring 2020
//
// Author:           Jiwon Song
// Email:            jsong99@wisc.edu
// CS Login:         jis
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   Avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
    int size;     // dimension of the square
    int **msquare; // pointer to heap allocated magic square
} MSquare;

/* 
 * Prompts the user for the magic square's size, reads it,
 * checks if it's an odd number >= 3 (if not display the required
 * error message and exit), and returns the valid number.
 */
int getSize() {
    // declare variables 
	int size;
	char input[10];
    // prompt the user for the magic square's size
	printf("Enter magic square's size (odd integer >=3)\n");
    scanf("%s",input);
    // cast the input into integer variable
    size = atoi(input);
    
    // check if the size is valid
    if ((size % 2) ==0) {
        printf("Size must be odd.\n");
        exit(1);
    }
    else if (size < 3) {
        printf("Size must be >= 3.\n");
        exit(1);
    }
    else {
    	return size;
    }  
} 
   
/* completed:
 * Makes a magic square of size n using the Siamese algorithm
 * and returns a pointer to the completed MSquare struct.
 *
 * n the number of rosw and columns
 */
MSquare *generateMSquare(int n) {
	MSquare *square = malloc(sizeof(MSquare));
    // set size for square
    square -> size = n;
	// allocate memory for 2D array
	square -> msquare = malloc(sizeof(int*)*n);
	for (int i = 0; i < n; i++) {
		*(square -> msquare+i) = malloc(sizeof(int)*n);
	}
    // initialize each element in the 2D array with 0
    // 0 represents blank space
	for (int i = 0; i < square -> size; i++) {
		for (int j=0; j < square -> size; j++) {
			*(*(square -> msquare+i)+j) = 0;
		}
	}
	// declare variables for Siamese method
	int num = 1;
    // the max value is n^2
	int total = n * n;
    // starting point with 1
    // 1's location is always (n/2,n-1)
	int i = (n/2); //x
	int j = n-1; //y
	int x = i;
	int y = j;
	
    // for number from 1 to n^2
	for (num = 1; num <= total;num++) {
        // if the space is empty, fill with right number
        // move south east
		if (*(*(square -> msquare+i)+j) == 0) {	
			*(*(square -> msquare+i)+j) = num;
            x = i;
            y = j;
			
            i++;
            j++;
		    // relocate i and j values if they go out of the border
            if (i >= n) {
               i = 0;
            }
            if (j >= n) {
               j = 0;
            }
	
		}
        // if the space is occupied, move left
		else {
			i = x;
            j = y;
            j--;
            if ( j < 0) {
              j = n-1;
            }

             *(*(square -> msquare+i)+j) = num;

             x = i;
             y = j;
             i++;
             j++;
            // relocate i and j values if they go out of the border
			if (i >= n) {
				i = 0;
			}
			if (j >= n) {
				j =0; 
			}	
		}
	}		
    return square;    
} 

/*   
 * Opens a new file (or overwrites the existing file)
 * and writes the square in the specified format.
 *
 * msquare the magic square to output
 * filename the name of the output file
 */
void outputMSquare(MSquare *msquare, char *filename) {
    
    // open file with the filename
    FILE *fp = fopen(filename,"w");
    if (fp == NULL) {
        printf("Can not open the file\n");
        exit(1);
    }
    // the first line contains the value n  
    fprintf(fp, "%d\n",msquare -> size);
    // write the magic square to the file
    for (int i = 0; i < msquare ->size; i++) {
        for (int j = 0; j < msquare -> size; j++) {
            fprintf(fp,"%d",*(*(msquare -> msquare+i)+j));
            if ( j != (msquare -> size-1)) {
            fprintf(fp,"%s",",");
            }
        }
        fprintf(fp,"%s","\n");
    }
    // close the file
    fclose(fp);
}

/* 
 * Generates a magic square of the user specified size and
 * output the quare to the output filename
 */
int main(int argc, char *argv[]) {
    
	// Check input arguments to get output filename
	if (argc != 2) {
		printf("Usage: ./makeMagic <output_filename>\n");
		exit(1);
	}
    
    // Get magin square's size from user
    int size;
	size = getSize();
    
    
    // Generate the magic square
	MSquare * square  = generateMSquare(size);

    // call output function	
    outputMSquare(square,*(argv+1));
    
    // free memories of msquare
    for (int i = 0; i < size; i++) {
       free(*(square->msquare+i));
    }
    // free msquare doulbe pointer
    free(square->msquare);
    // free the struct pointer 
    free(square);
    return 0;
} 


   
