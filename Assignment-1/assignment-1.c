/*
This is my password cracking program to hack the file "hackme".
This program displays ways of cracking the password with and without forking to compare efficiency.

Written by: Nathan Orgera (251175297)
Date: Jan 31st, 2023
*/



//librarys used
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "checkPassword.h"
#include "errno.h"


/*
brute force password cracking function.
utilizes the checkpassword function to crack a password of length 3.
uses a triple for loop to find each of the 3 characters.

void return value, as it only prints the password.
parameters: starting position of full password.

*/
void pass(int start) {
    char attempt[4] = { 0 }; //array of chars

    for (int i = 33; i <= 126; i++) { // first loop iterates the first char
        attempt[0] = (char) i;         // stores correct char in attempt array

        // program continues to do the same for second loop and third loop
        for (int j = 33; j <= 126; j++) { //^^
            attempt[1] = (char) j;
            for (int k = 33; k <= 126; k++) { // ^^
                attempt[2] = (char) k;

                if (checkPassword(attempt, start) == 0) { //uses checkpassword function to verify correct password
                    printf("%s\n", attempt);               //prints attempt
                    fflush(stdout);                         
                    return;
                }
            }
        }
    }
    printf("wrong ASCII range\n");
}



/*
Main function
Accepts command line arguments to enable forking
returns int to kill program (return 0;)
*/
int main( int argc, char *argv[]) {

    //initializers for pids
    pid_t id1, id2, id11, id111;
    
    int forkbool = 0;  // default boolean for forking
    //int pbool = 0; 

    /* testing if command line argument == "-f" to allow forking
     uses strcmp from <string.h> library
    */
    if (argc > 1 && strcmp(argv[1], "-f") == 0) {
        forkbool = 1;
    }

    
    /*
    could not get the -p to work :(

    if (argc == 3 && strcmp(argv[2], "-p") == 0) {
        pbool = 1;
    }
    */


  
    if (forkbool == 0){ //runs program without forking enabled.

            pass(0);
            pass(3);
            pass(6);
            pass(9);
            return 0;

    }else{ // runs program with working

        id1 = fork();
        id2 = fork();

        if (id1 == 0) {
            if (id2 == 0) { // we are process y (1.1.1)

                // PID print statements
                id111 = getpid();
                id11 = getppid();
                printf("PID 1.1 IS %d. Child 1.1.1 IS %d\n", id11, id111);
                printf("PID 1.1.1 IS %d. PPID 1.1 IS %d\n", id111, id11);
                printf("PID 1.2 IS %d. PPID 1.0 IS %d\n", id11 + 1, getppid() - 1);
                fflush(stdout);
                pass(9); // run passeord cracker function at starting position 9


            } else { // we are process x (1.1)

                pass(6); // run passeord cracker function at starting position 6
                wait(NULL); // wait for child to complete
            }
        } else {
            if (id2 == 0) { // we are process z (1.2)

                pass(3); // run passeord cracker function at starting position 3
                wait(NULL); // wait for child to finish

            } else { // we are parent process (1.0)

                // PID print statements
                printf("PID 1.0 IS %d. Child 1.1 IS %d\n", getpid(), id1);
                printf("PID 1.1 IS %d. PPID IS 1.0 %d\n", id1, getpid());
                printf("PID 1.0 IS %d. Child 1.2 IS %d\n", getpid(), id2);
                fflush(stdout);
                pass(0);
                wait(NULL);
                wait(NULL);




                //if (pbool == 1) {
                    //execl("hackme", "", NULL);
                //}else{
                //    return 0;
                //}
                
                
                
            }
        }
return 0;
    }
}
