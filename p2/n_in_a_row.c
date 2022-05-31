///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        n_in_a_row.c
// This File:        n_in_a_row.c
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
// Persons:          -
//                  
//
// Online sources:   -
//                   
//                   
////////////////////////////////////////////////////////////////////////////////
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
     
char *COMMA = ",";  
     
/* COMPLETED:       
 * Retrieves from the first line of the input file,
 * the size of the board (number of rows and columns).
 * 
 * fp: file pointer for input file
 * size: pointer to size
 */
void get_dimensions(FILE *fp, int *size) {      
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, fp) == -1) {
        printf("Error in reading the file.\n");
        exit(1);
    }

    char *token = NULL;
    token = strtok(line, COMMA);
    *size = atoi(token);
}       
 
   
  
/* 
 * Returns 1 if and only if the board is in a valid state.
 * Otherwise returns 0.
 * 
 * board: heap allocated 2D board
 * size: number of rows and columns
 */
int n_in_a_row(int * board[], int size) {
    // 1 == x , 2 == y, 0 == blank

    // Check whether the size is odd, return invalid if the size is even
    if ((size % 2) == 0) {
		return 0;
    }
    
    // initialize variables 
    int xCount = 0;
    int yCount = 0;
    int zero = 0;

    int diag_win = 0;
    int row_win = 0;
    int col_win = 0;
    
    // iterate through the board and count how many Xs and Os are there
    for (int i = 0; i < size; i++) { 
        for (int j = 0; j < size; j++) {
	    
	    	int value = *(*(board+i)+j);	    
	    	// Counting Xs and Os
	   		if ( 1 == value) {
	      	
				xCount += 1;
	    	}	
	    	else if (2 == value) {
	      		yCount += 1;
            }
            else {
	      		zero += 1;
	    	}	

		}
    }
    
    
    // Check whether there are same numbers Xs as Os or 1 more X than O
    if (!(xCount == yCount) && !(xCount == (yCount+1))) 
    {
	    return 0;
    }
    
    // Check rows 
    for (int i = 0; i < size; i++) {
		
		xCount = 0;
		yCount = 0;
		zero = 0;
		
		// Check whether each elements are equal
		for (int j = 0; j < size; j++ ) {
	    	int value = *(*(board+i)+j);
	     	
			if (1 == value) {
				xCount += 1;
			}
			else if (2 == value) {
				yCount += 1;
			}
			else {
				zero += 1;
			}
		
			if (xCount == size || yCount == size) {
				row_win += 1;
			}
		}	
    }
    
    // Check columns
    
    for ( int i = 0; i < size; i++ ) {
	    // reinitialize the values to store information
	    xCount = 0;
	    yCount = 0;
	    zero = 0;

	    for (int j = 0; j < size; j++) {
		
			int value = *(*(board+j)+i);
			if (1 == value) {
				xCount += 1;
			}
			else if (2 == value) {
				yCount += 1;
			}
			else {
				zero += 1;
			}
		    	    
	   	}
	   
	    if (xCount == size || yCount == size) {
		    col_win += 1;
	    }

    }


    // Check left to right diagnal
    xCount = 0;
    yCount = 0;

    for (int i = 0; i < size; i ++) {

		int value = *(*(board+i)+i);
	    if (1 == value) {
        	xCount += 1;
        }
      	else if (2 == value) {
            yCount += 1;
        }
        else {
            zero += 1;
        }
    }
    if (xCount == size || yCount == size) {
		diag_win += 1;
    }


    // Check right to left diagnal
    xCount = 0;
    yCount = 0;

    for (int i = 0; i < size; i ++) {

		int value = *(*(board+i)+(size-1-i));
		//board[i][size-1-i]
	   	if (1 == value) {
			xCount += 1;
		}
       	else if (2 == value) {
            yCount += 1;
		}
        else {
            zero += 1;
        }

    }
    if (xCount == size || yCount == size) {
    	diag_win += 1;
    }
    
   
    // Check whether there is one winning line or two winning lines
    if (row_win + col_win + diag_win > 2) {
        return 0;
    }
    // Check whether there is any parallel winning lines
    if (row_win > 1 || col_win > 1) {
		return 0;
    }

    // If it passes all the test, return the valid value
    return 1;   
}    
  
 
   
/* PARTIALLY COMPLETED:
 * This program prints Valid if the input file contains
 * a game board with either 1 or no winners; and where
 * there is at most 1 more X than O.
 * 
 * argc: CLA count
 * argv: CLA value
 */
int main(int argc, char *argv[]) {              
     
    // Check the whether the argument contains 2 arguments
    if (argc != 2) {
	printf("Usage: ./n_in_a_row <input_filename>\n");
	exit(1);
    }


    
    //Open the file and check if it opened successfully.
    FILE *fp = fopen(*(argv + 1), "r");
    if (fp == NULL) {
        printf("Can't open file for reading.\n");
        exit(1);
    }

    //Declare local variables.
    int size;

    //Call get_dimensions to retrieve the board size.
    get_dimensions(fp,&size);
    
    //Dynamically allocate a 2D array of dimensions retrieved above.
    int ** dimension = malloc(sizeof(int*)*size);

    for (int i = 0; i < size; i++) {
	*(dimension+i) = malloc(sizeof(int)*size);
    }
    
    
    //Read the file line by line.
    //Tokenize each line wrt comma to store the values in the 2D array 'dimension'.
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
    for (int i = 0; i < size; i++) {

        if (getline(&line, &len, fp) == -1) {
            printf("Error while reading the file.\n");
            exit(1);
        }

        token = strtok(line, COMMA);
        for (int j = 0; j < size; j++) {

            //Initializing the 2D array.
	    *(*(dimension+i)+j)  = atoi(token);
            token = strtok(NULL, COMMA);
        }
    }
 
    //Call the function n_in_a_row and print the appropriate
    //output depending on the function's return value.
    if (n_in_a_row((int**)dimension,size) == 1) {
       printf("valid\n");
    }
    else {
       printf("invalid\n");
    }

    //Free all dynamically allocated memory.
    for (int i =0; i< size; i++) {
	    free(*(dimension+i));
    }

    free(dimension);
    dimension = NULL;

    //Close the file.
    if (fclose(fp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
    } 
     
    return 0;       
}       
