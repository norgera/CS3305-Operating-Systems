/*
This is my partition program to calculate the sum of prime numbers given command line perameters.
This program displays ways of using threads to make multiple processes

Written by: Nathan Orgera (251175297)
Date: Mar 7th, 2023
*/

//libraries used
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int atoi(const char *str); //function to convert string to int
int primes[1000]; // array to store prime numbers
int n = 0; // number of prime numbers

// function to check and store prime numbers
int prime(int low, int high) {
    int i, flag;
    // iteration until low is not equal to high
    while (low < high) {
        flag = 0;
        // ignore numbers less than 2
        if (low <= 1) {
            ++low;
            continue;
        }
        // if low is a non-prime number, flag will be 1
        for (i = 2; i <= low / 2; ++i) {
            if (low % i == 0) {
                flag = 1;
                break;
            }
        }
        if (flag == 0)
            primes[n] = low;
        // to check prime for the next number
        // increase low by 1
        ++n;
        ++low;
    }
    return n;
}

// function to calculate the sum of prime numbers
void* routine(void* arg) {
    int index = *(int*)arg; // index of prime numbers
    int sum = 0; // sum of prime numbers
    int low = index; // lower bound of prime numbers
    int high = index;  // upper bound of prime numbers
    for (int j = low; j < high; j++) { // iterate through the prime numbers
        sum += primes[j]; // sum of prime numbers
    }
    printf("local sum %d\n", sum); // print the sum of prime numbers for each thread
    *(int*)arg = sum; // store the sum of prime numbers for each thread
    return (arg); // return the sum of prime numbers for each thread
}

//The first command-line parameter indicates how many threads your program should create
//The second command-line parameter indicates the maximum number your prime counting/summing program should go up to.
int main(int argc, char *argv[]) {

    int numthreads = atoi(argv[1]); // number of threads
    int b = atoi(argv[2]); // max number count/sum

    prime(0, b); // call prime function
    int x = b / numthreads; // number of prime numbers per thread
    pthread_t threads[numthreads]; // array of threads
    int i; // iterator
    int *a = malloc(sizeof(int)); // pointer to store the sum of prime numbers for each thread
    for (i = 0; i < numthreads; i++) { // iterate through the number of threads

        if (i == numthreads - 1) { // if the last thread
            *a = i * x; // store the sum of prime numbers for each thread
        } else { // if not the last thread
            *a = i * x; // store the sum of prime numbers for each thread
        }

        if (pthread_create(&threads[i], NULL, &routine, a) != 0) { // create the threads
            perror("Error creating thread"); // error checking
        }
    }
    int global_sum = 0; // sum of prime numbers
    for (i = 0; i < numthreads; i++) {
        int *r; // pointer to store the sum of prime numbers for each thread
        if (pthread_join(threads[i], (void **) &r) != 0) { // join the threads
            perror("Error joining thread");
        }
        global_sum += *r; // sum of prime numbers
    }
    printf("global sum %d\n", global_sum); // print the sum of prime numbers
    return 0;
}
