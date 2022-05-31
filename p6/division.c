////////////////////////////////////////////////////////////////////////////////
// Main File:        division.c
// This File:        division.c
// Other Files:      intdate.c sendsig.c division.c
// Semester:         CS 354 Spring 2020
//
// Author:           Jiwon Song
// Email:            jsong99@wisc.edu
// CS Login:         jis
//
/////////////////////////// OTHER SOURCES OF HELP /////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
///////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* maximum number of input length*/
#define MAX 100

/* global variable to count number of operations completed  */
int operation = 0;


/*
 * a signal handler for SIGINT
 * 
 * prints out the number of successfully completed division operations and exit the program
 *
 * sig: signal number
*/
void sigint_handler(int sig) {
   
    printf("\nTotal number of operations completed successfully: %d\n", operation);
    printf("The program will be terminated.\n");
    exit(0);
}



/*
 * a signal handler for SIGFPE
 *
 * prints out the error message and
 * the number of successfully completed division operations and exit the program
 *
 * sig: signal number
*/
void sigfpe_handler(int sig) {

    printf("Error: a division by 0 operation was attempted.\n");
    printf("Total number of operations completed successfully: %d\n", operation);
    printf("The program will be terminated.\n");
    exit(0);
}


/* The main function of this program
 * 
 * sets memory and handlers
 * prompts the user for two integers
 * prints out result of the division and remainder
 * runs an infinite loop until terminated
 *
*/
int main() {

    struct sigaction sigint, sigfpe;
    
    // setting memory
    if (memset(&sigint, 0, sizeof(sigint)) != &sigint) {
        printf("Error setting memory for sigint.\n");
        exit(1);
    }
    
    // setting memory
    if (memset(&sigfpe, 0, sizeof(sigfpe)) != &sigfpe) {
        printf("Error setting memory for sigfpe.\n");
        exit(1);
    }

    // setting handler
    sigfpe.sa_handler = sigfpe_handler;
    sigfpe.sa_flags = 0;

    if (sigaction(SIGFPE, &sigfpe, NULL) != 0) {
        printf("Error binding SIGFPE handler.\n");
        exit(1);
    }
   

    // setting handler
    sigint.sa_handler = sigint_handler;
    sigint.sa_flags = 0;
    
    if (sigaction(SIGINT, &sigint, NULL) != 0) {
        printf("Error binding SIGINT handler.\n");
        exit(1);
    }
    
    // going into an infinite loop
    while(1) {
        int num1 = 0;
        int num2 = 0;
        
        char input1[MAX];
        char input2[MAX];

        // prompt the user for two integers
        printf("Enter first integer: ");
        fgets(input1, MAX, stdin);
        num1 = atoi(input1);
        
        printf("Enter second integer: ");
        fgets(input2, MAX, stdin);
        num2 = atoi(input2);
    
        int div = num1 / num2;
        int remainder = num1 % num2;
        // sends out the result
        printf("%d / %d is %d with a remainder of %d\n",num1, num2, div, remainder);
        operation++;
    }
    


    return 0;
}


