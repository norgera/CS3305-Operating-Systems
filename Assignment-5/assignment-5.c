#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINES 100                //maximum number of lines to read from file
#define MAX_LEN  1000                //maximum length per line


struct account{

    int accountNum;                  //number of account
    int balance;                     //account structure holding balance of account
};

struct account accountArray[100];    //array of all accounts

struct client{

    int clientNum;                   //client number
    int accountChange[100];          //index = account number, integer value = change to account

};

pthread_mutex_t lock;                //lock

void* clientThread(void* clientArg){                                //thread for client, client struct as argument

    struct client thisClient = *(struct client*)clientArg;          //take in client struct and wrap

    for (int j = 0; j<100; j++){                                    //iterate through accountChange variable in c struct

        // before we access the shared memory, lock
        pthread_mutex_lock(&lock);
        // if the accountChange value isn't nothing, and it won't make the account negative
        if ((thisClient.accountChange[j] != 0) && (accountArray[j-1].balance + thisClient.accountChange[j] > 0)){
            //add or subtract this amount change to the main accountArray balance
            accountArray[j-1].balance += thisClient.accountChange[j];
        }
        //after were done accessing the critical section (accountArray), we can unlock
        pthread_mutex_unlock(&lock);

    }
}

int main()
{
    char lines [MAX_LINES][MAX_LEN];    //2d char array that will hold the lines read from the file
    FILE *fp;                           //file pointer
    fp = fopen("assignment_5_input.txt","r");   //open file

    if (fp == NULL){                    //if we were unable to open file

        printf("Error: Unable to open data file.");     //print error
        exit(1);
    }

    int line = 0;

    while (!feof (fp) && !ferror (fp)){                      //while we haven't reached end of file and no errors
        if (fgets (lines[line], MAX_LEN, fp) != NULL)        //assign line to corresponding line in array
        {			                                         //if fgets doesn't return null (end of file), continue
            line++;		//increment to next line
        }
    }

    //first we need to go through the array holding all of our lines
    //we will count how many lines contain account balances and how many contain client info

    int numAccounts = 0;        //variable to keep track of number of accounts
    int numClients = 0;         //variable to keep track of number of clients

    for (int i = 0; i<line; i++){

        if (lines[i][0] == 'c'){        //if line is a client line, increment client variable
            numClients +=1;
        }

        else if (lines[i][0] == 'a'){   //if line is a account line, increment account variable
            numAccounts +=1;
        }

    }

    //now that we have the amount of clients/accounts, we can create corresponding length respective arrays
    //these will be arrays of structures

    //struct account accountArray[numAccounts];       //array of all accounts

    for (int i = 0; i < numAccounts; i++) {
        accountArray[i].accountNum = i + 1;           //set account number
        //now we need to extract each balance from lines array
        if (lines[i][0] == 'a') {   //if line is an account line
            char *p = strrchr(lines[i], ' ');
            if (p && *(p + 1)) {
                accountArray[i].balance = atoi(p + 1);
            }
        }
    }

    struct client clientArray[numClients];

    //to populate our client array, we must iterate line by line and extract the actions

    //1. assign client nums as i
    //2. delete first element in the line
    //3. split the rest of the line into groups of 3 and place into temporary array
    //4. go through each line in temp array and see if its a 'd' or 'w', then get the final value again.
    // if d then final val is positive, if w then final val is negative.


    for (int i = 0; i < numClients; i++) {

        for (int n = 0; n <100; n++){               //filling array with 0
            clientArray[i].accountChange[n] = 0;
        }

        clientArray[i].clientNum = i + 1;               //assign client number
        char *clientLine = lines[i+numAccounts];
        while (*clientLine != 0 && *(clientLine++) != ' ') {}       //take just the activities done by this client


        char activities[numClients][100];       //array to hold all activities done by client (deposit or withdrawal)


        // now we have the client number and the line of what they do, now put every word in this line into an array

        char *p = strtok (clientLine, " ");     //split by spaces
        char *array[20];                                 //place into array (should make this dynamic)
        int numElements = 0;
        int j = 0;

        while (p != NULL){

            array[j++] = p;
            p = strtok (NULL, " ");
            numElements+=1;
        }
        //now we have an array of every word in the actions of the client, we will check them in groups of three



        //printf("%d\n", numElements);
        int count = 0;

        while (count != numElements) {

            bool isPositive = true;
            int accToChange = 0;
            int numChange = 0;

            if (array[0][0] == 'w') {        //checking if withdrawal
                isPositive = false;
            }

            accToChange += atoi(array[1] += 7); //determining account

            numChange += atoi(array[2]);        //determining amount

            if (isPositive) {
                //printf("Deposit to account %d $%d\n", accToChange, numChange);

                clientArray[i].accountChange[accToChange] += numChange;
            }

            if (!isPositive) {
                //printf("Withdrawal from account %d $%d\n", accToChange, numChange);

                clientArray[i].accountChange[accToChange] -= numChange;
            }

            count += 3;

            array[0] = array[count];            //after we take care of the first three, shift the next three into
            array[1] = array[count+1];          //their positions
            array[2] = array[count+2];

            //printf("Client%d: %s\n", clientArray[i].clientNum,activities[i]);
        }
    }
    //now that we have everything stored in array of structs, we can actually create our threads.


    pthread_t th[numClients];

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("Mutex lock init failed\n");                      //check if lock initialization failed
        return 1;
    }

    for (int i = 0; i < numClients; i++){
        if (pthread_create(&th[i],NULL,&clientThread,&clientArray[i]) != 0) {
            perror("Failed to create thread\n");                //check if thread creation failed
        }
    }

    for (int i = 0; i < numClients; i++){
        if (pthread_join(th[i],NULL) != 0) {
            perror("Failed to join thread\n");                      //check if thread join failed
        }
    }

    pthread_mutex_destroy(&lock);                                   //destroy lock

    printf("No. of Accounts: %d\n", numAccounts);
    printf("No. of Clients: %d\n", numClients);

    for (int i = 0; i<numAccounts; i++){
        printf("account%d, balance: %d\n", accountArray[i].accountNum, accountArray[i].balance);
    }

}