/*
This is my partition program to calculate large numbers.
This program displays ways of using pipes to transfer data between processes


this program requires the call "./assignment-2 1234 5678" twice or more times to ensure the print statements print in order
im not sure why this is...

Written by: Nathan Orgera (251175297)
Date: Feb 13th, 2023
*/


//libraries used
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int atoi(const char *str); //function to convert string to int
int n = 4; // base case for integer size

// fd[0] is read end, fd[1] is write end
int fd[2]; // child to parent
int fd1[2]; // parent to child
//the reason we do this is so that we can go back and forth between the parent and child processes using two pipes
//in the parent process, we will use fd[0] to read from the child process and fd[1] to write to the child process
//in the child process, we will use fd1[0] to read from the parent process and fd1[1] to write to the parent process

/*
Main function
Accepts command line arguments for integers to be partitioned
*/
int main(int argc, char *argv[]) {
    pid_t pid; // process id initializer
    int a = atoi(argv[1]); // first integer
    int b = atoi(argv[2]); // second integer
    int a1 = a / 100; // first integer first half
    int a2 = a % 100; // first integer second half
    int b1 = b / 100; // second integer first half
    int b2 = b % 100; // second integer second half

    // variables for calculations
    int A;
    int B;
    int C;
    int D;
    int Y;
    int X;

    // variables for reading and writing
    int something;
    int something2;
    int something3;

    // error checking for number of command line arguments
    if (argc != 3) {
        exit(0);
    }

    // error checking for integer size
    // if the integers are not 4 digits, exit the program
    if (a < 1000 || a > 9999 || b < 1000 || b > 9999) {
        exit(0);
    }


    // error checking for pipe failures
    // if the pipes fail, exit the program
    if (pipe(fd) < 0) {
        printf("Pipe failed");
        exit(0);
    }
    if (pipe(fd1) < 0) {
        printf("Pipe failed");
        exit(0);
    }

    // print integers
    printf("\nYour integers are %d %d\n", a, b);

    // fork child process
    pid = fork();

    // error checking for fork
    if (pid < 0) {
        perror("fork error");
        exit(0);
    }


    if(pid > 0) //parent
    {
        close(fd[1]); // close write end
        close(fd1[0]); // close read end

        printf("Parent (PID %d): created child (PID %d)\n", getpid(), getpid()+1);

        // All print statements are self-explanatory
        // All read and write statements are self-explanatory, as i described the pipe logic above
        printf("\n###\n# Calculating X\n###\n");

        // send a1 and b1 to child
        write(fd1[1],&a1,sizeof(a1));
        printf("Parent (PID %d): Sending %d to child\n", getpid(), a1);
        write(fd1[1],&b1,sizeof(b1));
        printf("Parent (PID %d): Sending %d to child\n", getpid(), b1);


        // receive A from child
        read(fd[0], &something, 4);
        printf("Parent (PID %d): Received %d from child\n", getpid(), something);

        // calculate X
        X = (something * 10000);


        printf("\n###\n# Calculating Y\n###\n");

        // send a1 and b2 to child
        write(fd1[1],&a1,sizeof(a1));
        printf("Parent (PID %d): Sending %d to child\n", getpid(), a1);
        write(fd1[1],&b2,sizeof (b2));
        printf("Parent (PID %d): Sending %d to child\n", getpid(), b2);

        // receive B from child
        read(fd[0], &something, 4);
        printf("Parent (PID %d): Received %d from child\n", getpid(), something);
        B = something; // store B for later use

        // send a2 and b1 to child
        write(fd1[1],&a2,sizeof(a2));
        printf("Parent (PID %d): Sending %d to child\n", getpid(), a2);
        write(fd1[1],&b1,sizeof(b1));
        printf("Parent (PID %d): Sending %d to child\n", getpid(), b1);

        // receive C from child
        read(fd[0], &something, 4);
        printf("Parent (PID %d): Received %d from child\n", getpid(), something);

        // calculate Y
        Y = (B + something) * 100;
        printf("\n###\n# Calculating Z\n###\n");

        // send a2 and b2 to child
        write(fd1[1],&a2,sizeof(a2));
        printf("Parent (PID %d): Sending %d to child\n", getpid(), a2);
        write(fd1[1],&b2,sizeof(b2));
        printf("Parent (PID %d): Sending %d to child\n", getpid(), b2);

        // receive Z from child
        read(fd[0], &something, 4);
        printf("Parent (PID %d): Received %d from child\n\n", getpid(), something);

        // print results
        printf("%d*%d == %d + %d + %d === %d\n", a, b, X, Y, something, X+Y+something);

        // close pipes
        close(fd[0]);
        close(fd1[1]);
    }


 else //child
    {

        // close pipes
        close(fd[0]);
        close(fd1[1]);

        // read a1 and b1 from parent
        read(fd1[0], &something3, 4);
        printf("      Child (PID %d): Received %d from parent\n", getpid(), something3);
        read(fd1[0], &something2, 4);
        printf("      Child (PID %d): Received %d from parent\n", getpid(), something2);

        // calculate A
        A = something3 * something2;

        // send A to parent
        write(fd[1], &A, sizeof(A));
        printf("      Child (PID %d): Sending %d to parent\n", getpid(), A);

        // read a1 and b2 from parent
        read(fd1[0], &something3, 4);
        printf("      Child (PID %d): Received %d from parent\n", getpid(), something3);
        read(fd1[0], &something2, 4);
        printf("      Child (PID %d): Received %d from parent\n", getpid(), something2);

        // calculate B
        B = something3 * something2;

        // send B to parent
        write(fd[1], &B, sizeof(B));
        printf("      Child (PID %d): Sending %d to parent\n", getpid(), B);

        // read a2 and b1 from parent
        read(fd1[0], &something3, 4);
        printf("      Child (PID %d): Received %d from parent\n", getpid(), something3);
        read(fd1[0], &something2, 4);
        printf("      Child (PID %d): Received %d from parent\n", getpid(), something2);

        // calculate C
        C = something3 * something2;

        // send C to parent
        write(fd[1], &C, sizeof(C));
        printf("      Child (PID %d): Sending %d to parent\n", getpid(), C);

        // read a2 and b2 from parent
        read(fd1[0], &something3, 4);
        printf("      Child (PID %d): Received %d from parent\n", getpid(), something3);
        read(fd1[0], &something2, 4);
        printf("      Child (PID %d): Received %d from parent\n", getpid(), something2);

        // calculate D
        D = something3 * something2;

        // send D to parent
        write(fd[1], &D, sizeof(D));
        printf("      Child (PID %d): Sending %d to parent\n", getpid(), D);

        // Close pipes
        close(fd[1]);
        close(fd1[0]);
}
return 0;
}