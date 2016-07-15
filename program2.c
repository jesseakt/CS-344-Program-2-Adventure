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
            //Chose a new candidate number
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
        //Choose a random number of extra connections (3-6)
        //Choose a random number between 0-3 and add 3.
        int extraConnections = rand()%4 + 3;
        //Define an array of room indices to use as extra connections.
        int newConnections[extraConnections];
        int candidateNew; //Used to hold the candidate new value
        //Select "extraConnections" unique room indices
        for(j=0; j<extraConnections; j++)
        {
            //Loop until we actually add a unique index.
            while(1)
            {
                //Get a new index that isn't the room we're in.
                do{
                    candidateNew = rand()%7;
                }while(candidateNew == i);
                //Loop through existing indices to see if we aready have it
                int found = 0;
                for(k=0; k<j; k++)
                {
                    //If we find it flag that it's already here
                    if(candidateNew == newConnections[k])
                        found = 1;
                }
                
                //If we didn't find it, add it to the list
                if(found != 1)
                {
                    newConnections[j] = candidateNew;
                    //escape the while loop after adding
                    break;
                }
            }
            //Add new index to the connections list.
            fprintf(currentFile, "CONNECTION %d: %s\n", j+1, roomNames[chosenRooms[newConnections[j]]]);
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

