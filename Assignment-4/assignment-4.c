/*
This is my partition program to calculate the average wait and turnaround times using various CPU scheduling algorithms given command line perameters.
This program uses First Come First Served, Shortest Job First, and Round Robin CPU scheduling algorithms

Written by: Nathan Orgera (251175297)
Date: Mar 21st, 2023
*/

//libraries used
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


int atoi(const char *str); //function to convert string to int
#define MAX_PROCESSES 20 // max number of processes
#define MAX_LINE_LENGTH 1024 // max length of line

// struct to store process information
typedef struct {
    char name[3];
    int arrival_time;
    int burst_time;
    int wait_time;
    int turnaround_time;
    bool finished;
} process_t; // process_t is a type

// function to calculate the average wait time
int main(int argc, char *argv[]) {

    // Check for sufficient arguments
    if (argc < 3) {
        printf("Insufficient arguments!\n");
        return 1;
    }
    char *filename = argv[argc - 1]; // filename is the last argument

    // First Come First Served
    if (strcmp(argv[1], "-f") == 0) { // if the first argument is -f then run First Come First Served
        printf("First Come First Served\n");

        // Initialize processes
        process_t processes[MAX_PROCESSES];
        int num_processes = 0;
        FILE *fp = fopen(filename, "r");

        // Check if file exists
        if (!fp) {
            fprintf(stderr, "Failed to open file\n");
            return 1;
        }

        // Read processes from file
        char line[MAX_LINE_LENGTH];
        while (fgets(line, MAX_LINE_LENGTH, fp)) { // read each line of the file
            char *name = strtok(line, ","); // split the line by commas
            char *burst_time_str = strtok(NULL, ","); // split the line by commas
            int burst_time = atoi(burst_time_str); // convert burst time to inta

            process_t process; // create a process
            // store process information
            strncpy(process.name, name, 3);
            process.arrival_time = 0;
            process.burst_time = burst_time;
            process.wait_time = 0;
            process.turnaround_time = 0;

            processes[num_processes++] = process; // add process to processes
        }

        fclose(fp); // close file

        // Run First Come First Served
        int current_time = 0;
        for (int i = 0; i < num_processes; i++) {
            process_t *process = &processes[i];

            // Calculate wait time
            process->wait_time = current_time - process->arrival_time;

            // Update current time
            current_time += process->burst_time;

            // Calculate turnaround time
            process->turnaround_time = current_time - process->arrival_time;

            // Print status for each time unit
            for (int j = 0; j < process->burst_time; j++) {
                printf("T%d : %s - Burst left %2d, Wait time %2d, Turnaround time %2d\n",
                       current_time - process->burst_time + j, process->name, process->burst_time - j,
                       process->wait_time, process->turnaround_time);
            }
        }

        // Calculate averages
        double total_wait_time = 0;
        double total_turnaround_time = 0;
        for (int i = 0; i < num_processes; i++) {
            total_wait_time += processes[i].wait_time;
            total_turnaround_time += processes[i].turnaround_time;
        }

        double avg_wait_time = total_wait_time / num_processes; // calculate average wait time
        double avg_turnaround_time = total_turnaround_time / num_processes; // calculate average turnaround time

        // Print averages
        printf("\n");
        for (int i = 0; i < num_processes; i++) {
            printf("P%d\n", i);
            printf("\tAverage waiting time:\t\t%.0d\n", processes[i].wait_time);
            printf("\tAverage turnaround time:\t%.0d\n", processes[i].turnaround_time);
            printf("\n");
        }
        printf("Total average waiting time:\t%.1f\n", avg_wait_time);
        printf("Total average turnaround time:\t%.1f\n", avg_turnaround_time);

        return 0;
    }


        // Shortest Job First
    else if (strcmp(argv[1], "-s") == 0) {
        printf("Shortest Job First\n");

        // Initialize processes
        FILE *fp = fopen(filename, "r");
        if (!fp) {
            fprintf(stderr, "Failed to open file\n");
            return 1;
        }

        // Read processes from file
        process_t processes[MAX_PROCESSES];
        int num_processes = 0; // number of processes
        char line[100]; // line of file
        while (fgets(line, sizeof(line), fp) != NULL) { // read each line of the file
            char *tok = strtok(line, ","); // split the line by commas
            tok = strtok(NULL, ","); // split the line by commas
            processes[num_processes].burst_time = atoi(tok); // convert burst time to int
            processes[num_processes].turnaround_time = processes[num_processes].burst_time; // set turnaround time to burst time
            num_processes++; // increment number of processes
        }
        fclose(fp); // close file

        // Sort processes by burst time (Shortest Job First)
        for (int i = 0; i < num_processes; i++) { // loop through processes
            for (int j = i + 1; j < num_processes; j++) { // loop through processes
                if (processes[i].burst_time > processes[j].burst_time) { // if the burst time of the first process is greater than the second process
                    process_t temp = processes[i]; // swap processes
                    processes[i] = processes[j]; // swap processes
                    processes[j] = temp; // swap processes
                }
            }
        }

        // Simulate process execution
        int current_time = 0; // current time
        float total_waiting_time = 0.0; // total waiting time
        float total_turnaround_time = 0.0; // total turnaround time

        printf("-- Shortest Job First --\n");
        printf("Shortest Job First\n");

        for (int i = 0; i < num_processes; i++) { // loop through processes
            process_t p = processes[i]; // get process
            p.wait_time = current_time; // set wait time
            p.turnaround_time = current_time + p.burst_time; // set turnaround time
            total_waiting_time += p.wait_time; // add wait time to total wait time
            total_turnaround_time += p.turnaround_time; // add turnaround time to total turnaround time

            // Print status for each time unit
            printf("T%d : P%s  - Burst left  %d, Wait time  %d, Turnaround time  %d\n", current_time, p.name, p.arrival_time, p.wait_time, p.turnaround_time);
            current_time += p.burst_time; // increment current time
        }

        // Calculate average waiting time and turnaround time
        float avg_waiting_time = total_waiting_time / num_processes; // calculate average waiting time
        float avg_turnaround_time = total_turnaround_time / num_processes; // calculate average turnaround time
        printf("\n");

        // Print averages
        for (int i = 0; i < num_processes; i++) {
            process_t p = processes[i];
            printf("P%s\n", p.name);
            printf("\tWaiting time:           %d\n", p.wait_time);
            printf("\tTurnaround time:        %d\n", p.turnaround_time);
            printf("\n");
        }
        printf("Total average waiting time:     %.1f\n", avg_waiting_time);
        printf("Total average turnaround time:  %.1f\n", avg_turnaround_time);

        return 0;
    }


        // Round Robin
    else if (strcmp(argv[1], "-r") == 0 && argc == 4) { // check if round robin
        int quantum = atoi(argv[2]); // get quantum

        // check if quantum is valid
        if (quantum <= 0) {
            printf("Invalid time quantum!\n");
            return 1;
        }

        printf("Round Robin with quantum %d\n", quantum);
        process_t processes[MAX_PROCESSES]; // array of processes
        int num_processes = 0; // number of processes
        FILE *file = fopen(filename, "r"); // open file
        if (file != NULL) { // check if file is valid
            char line[100]; // line of file
            while (fgets(line, sizeof(line), file)) { // read each line of the file

                // Read process data from file
                sscanf(line, "P%d,%d", &num_processes, &processes[num_processes].burst_time); // read process data from file
                processes[num_processes].arrival_time = processes[num_processes].burst_time; // set arrival time to burst time
                processes[num_processes].wait_time = 0; // set wait time to 0
                processes[num_processes].turnaround_time = 0;  // set turnaround time to 0
                processes[num_processes].finished = false; // set finished to false
            }
            fclose(file); // close file

            // Simulate process execution
        } else {
            printf("Error: Could not open file\n");
            return 1;
        }

        // Set the time quantum and initialize time variables

        int time = 0; // current time

        for (int i = 0; i < num_processes; i++) { // loop through processes
            if (!processes[i].finished) { // check if process is finished
                int burst_left = processes[i].arrival_time;  // get burst time left
                if (burst_left > quantum) { // check if burst time left is greater than quantum
                    burst_left = quantum; // set burst time left to quantum
                }
                processes[i].arrival_time -= burst_left; // decrement arrival time by burst time left
                time += burst_left; // increment time by burst time left
                processes[i].turnaround_time = time; // set turnaround time to time
                processes[i].wait_time = time - processes[i].burst_time; // set wait time to time minus burst time

                // Print status for each time unit
                printf("T%d : P%d  - Burst left  %d, Wait time  %d, Turnaround time  %d\n", time, i,
                       processes[i].arrival_time, processes[i].wait_time, processes[i].turnaround_time);

                // Check if the process has finished
                if (processes[i].arrival_time == 0) {
                    processes[i].finished = true;
                    break;
                }
            }



            // Calculate and print the average waiting and turnaround times
            float total_wait_time = 0; // total wait time
            float total_turnaround_time = 0;    // total turnaround time
            for (i = 0; i < num_processes; i++) {   // loop through processes
                total_wait_time += processes[i].wait_time;  // add wait time to total wait time
                total_turnaround_time += processes[i].turnaround_time;  // add turnaround time to total turnaround time
                printf("\n\nP%d\n", i);
                printf("\tWaiting time:         %d\n", processes[i].wait_time);
                printf("\tTurnaround time:      %d\n", processes[i].turnaround_time);
            }
            printf("\nTotal average waiting time:     %.1f\n", total_wait_time / num_processes);
            printf("Total average turnaround time:  %.1f\n", total_turnaround_time / num_processes);

            return 0;
        }
    }

        // Invalid algorithm parameter
        else {
            printf("Invalid algorithm parameter!\n");
            return 0;
        }
    }

