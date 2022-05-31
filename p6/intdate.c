////////////////////////////////////////////////////////////////////////////////
// Main File:        (name of file with main function)
// This File:        intdate.c
// Other Files:      (name of all other files if any)
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <limits.h>

/* global count variable to record how many times SIGURUSR1 is received */
int count = 0;


/* 
 * a signal handler for SIGALRM
 * prints out process ID, current time for every 3 seconds
 *
 * sig: signal number
 */
void sigalrm_handler(int sig) {

    time_t now;
    time(&now);

    printf("PID: %ld | Current Time: %s", (long)getpid(), ctime(&now));
    
    alarm(3);
}

/*
 * a signal handler for SIGUSR1
 * increases count everytime called
 * prints out a message
 *
 */
void sigusr1_handler() {
    // increase sigusr1 catch count
    count++;
    // send out message
    printf("\nSIGUSR1 caught!\n");

}



/*
 * a signal handler for SIGINT
 * prints out messages and terminate the program
 * 
 */
void sigint_handler() {
    
    printf("\nSIGINT received\n");
    printf("SIGUSR1 was received %d times. Exiting now.\n", count);
    exit(0);
}



/* The main function of this program
 *
 * This function sets memories and handlers for three sigactions.
 * It runs an alarm signal after 3 seconds of execution then run an infinite loop until terminated.
 *
 */
int main() {
    
    struct sigaction sig_alrm, sig_usr1, sig_int;
    
    // message display for users 
    printf("Pid and time will be printed every 3 seconds.\n");
    printf("Enter ^C to end the program.\n");

    // setting memory for sigalrm
    if (memset(&sig_alrm, 0, sizeof(sig_alrm)) != &sig_alrm) {
        printf("Error setting memory for SIGALRM\n");
        exit(1);

    }
    
    // setting memory for sigusr1
    if (memset(&sig_usr1, 0, sizeof(sig_usr1)) != &sig_usr1) {
        printf("Error setting memory for SIGUSR1\n");
        exit(1);

    }
    
    // setting memory for sigint
    if (memset(&sig_int, 0, sizeof(sig_int)) != &sig_int) {
        printf("Error setting memory for SIGINT\n");
        exit(1);
    }

    // setting SIGALRM
    sig_alrm.sa_handler = sigalrm_handler;
    sig_alrm.sa_flags = 0; // restart system call if possible

    if(sigaction(SIGALRM, &sig_alrm, NULL) != 0) {
	    printf("Error registering SIGALRM\n");
	    exit(1);
    }

    // setting SIGUSR1
    sig_usr1.sa_handler = sigusr1_handler;
    sig_usr1.sa_flags = 0; // restart system call if possible

    if(sigaction(SIGUSR1, &sig_usr1, NULL) != 0) {
        printf("Error registering SIGUSR1\n");
        exit(1);
    }

    // setting SIGINT
    sig_int.sa_handler = sigint_handler;
    sig_int.sa_flags = 0; // restart system call if possible

    if(sigaction(SIGINT, &sig_int, NULL) != 0) {
        printf("Error registering SIGINT\n");
        exit(1);
    }


    // run alarm signal after 3seconds
    alarm(3);
    // going into an infinite loop
    while(1);

    return 0;
}

