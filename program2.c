#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>

//Main method
int main(void)
{
    //Declare Variables
    int i, j, k;            // Loop iterators
    int pid;                // Process ID
    char filepath[80];      // Filepath for room directory
    char fileNames[6][7];   // Array of filenames for rooms in directory
    char *roomNames[10];    // Array of possible room names
    int chosenRooms[7];     // Array of actually selected room choices
    int connectionArr[7][7];// Array of connections between rooms
    int startRoom;          // chosenRooms index of Start Room
    int endRoom;            // chosenRooms index of End Room
    FILE *currentFile;      // Pointer to the current file


    //Get Process ID and Username
    pid = getpid();
    //Concatenate hardcoded path and the Process ID for grading
    snprintf(filepath, 80, "./thorenje.rooms.%d", pid);
    //Create and change directory
    mkdir(filepath, 0770);
    chdir(filepath);

    //Create list of room names
    roomNames[0] = "Popcorn Stadium";
    roomNames[1] = "Waffle Emporium";
    roomNames[2] = "House of Pancakes";
    roomNames[3] = "Sandwich Place";
    roomNames[4] = "Candy Court";
    roomNames[5] = "Soda Springs";
    roomNames[6] = "Chocolate Canyon";
    roomNames[7] = "Pizza Planet";
    roomNames[8] = "Fruity Fountain";
    roomNames[9] = "Delicious Deli";

    /* Randomly select 7 rooms.
     * Chooses 7 unique integers in the range [0,9] */
    //Set RNG
    srand(time(NULL));
    //Store 7 unique room selections.
    for(i=0; i<7; i++)
    {
        //Loop until we get a new random selection
        while(1)
        {
            //Choose a new candidate number
            int candidateNum = rand()%10;
            
            //Loop through the existing numbers to see if we already have it
            int found = 0;
            for(j=0; j<i; j++)
            {
                //If we find it, flag that we found it, else continue on
                if(candidateNum == chosenRooms[j])
                    found = 1;
            }

            //If we didn't find it in the existing list, add it.
            if(found != 1)
            {
                chosenRooms[i] = candidateNum;
                //escape the while loop after we add the new number
                break;
            }
        }         
    }

    //Choose a start room
    startRoom = rand()%7;
    //Choose an end room. Make sure it's different than the start room
    do{
        endRoom = rand()%7;
    }while(startRoom == endRoom);

    //Choose connections
    //Initialize connection Array with 0s.
    for(i = 0; i < 7; i++)
    {
        for(j = 0; j < 7; j++)
        {
            connectionArr[i][j] = 0;
        }
    }
    /* Loop through, randomly select rooms to form connections
     * continue on if there are 3 connections to the room.
     * 0 if no connection, 1 if connection
     * The self connection (connectionArr[n][n]) holds the number
     * of connections that room n has to other rooms.*/
    for(i = 0; i < 7; i++)
    {
        //Stay in this step of the for loop until there are 3 or more conn.
        while(connectionArr[i][i] < 3)
        {
            //Choose a new candidate connection (not same room)
            int newconn;
            do{
                newconn = rand()%7;
            }while(newconn == i);
            
            /*If connectionArr[i][newconn]==0, it's a new connection
             * so we need to add the new connection in and update the 
             * connection counters. If it's equal to 1, it's an existing
             * connection so do nothing.*/
            if(connectionArr[i][newconn]==0)
            {
                //Make association and reciprocal association
                connectionArr[i][newconn] = 1;
                connectionArr[newconn][i] = 1;
                //Increment connection counters for room i and room newconn
                connectionArr[i][i] +=1;
                connectionArr[newconn][newconn] +=1;
            }
        }
    }


    //Make and Populate a new room file for each of the rooms.
    for(i = 0; i < 7; i++)
    {
        //Create filenames for rooms 0 through 6.
        snprintf(fileNames[i], 6, "room%d", i);

        //Open file for reading/writing
        currentFile = fopen(fileNames[i], "w+");
        
        //Write the room name
        fprintf(currentFile, "ROOM NAME: %s\n", roomNames[chosenRooms[i]]);

        //Write the room connections
        k = 1; //connection counter
        //Iterate through connectionArr[i][x] and write existing connections
        for(j = 0; j < 7; j++)
        {
            //If we have a connection and we're not looking at the same room
            if(connectionArr[i][j] == 1 && i != j)
            {
                fprintf(currentFile, "CONNECTION %d: %s\n", k, roomNames[chosenRooms[j]]);
                //Increment the connection counter
                k++;
            }
        }


        //Write the Room Type
        if(i == startRoom){
            fprintf(currentFile, "ROOM TYPE: START_ROOM\n");
        }
        else if(i == endRoom){
            fprintf(currentFile, "ROOM TYPE: END_ROOM\n");
        }
        else{
            fprintf(currentFile, "ROOM TYPE: MID_ROOM\n");

        }

        //Close the file
        fclose(currentFile);


    }

    return 0;
}

