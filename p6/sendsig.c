////////////////////////////////////////////////////////////////////////////////
// Main File:        sendsig.c
// This File:        sendsig.c
// Other Files:      division.c intdate.c sendsig.c
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
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////


#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>



/* The main function of this program
 *
 * sends out signals according to the function argument
 *
 * argc: size of argument 
 * argv: argument passed to this program
 */
int main(int argc, char* argv[]) {
    
    if (argc != 3) {
        printf("Usage: <signal type> <pid>\n");    
        return 0;
    }
    
    // obtain process id from the argument passed
    int pid = atoi(argv[2]);
    
    // sends out signal
    if (strcmp(argv[1], "-i")) {
        kill(pid, SIGINT);
    }


    if (strcmp(argv[1], "-u")) {
        kill(pid, SIGUSR1);
    }



    return 0;

}
