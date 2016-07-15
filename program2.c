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
    char roomNames[10][50];    // Array of possible room names
    int chosenRooms[7];     // Array of actually selected room choices
    int connectionArr[7][7];// Array of connections between rooms
    int startRoom;          // chosenRooms index of Start Room
    int endRoom;            // chosenRooms index of End Room
    int currentRoom;        // chosenRooms index of Current Room
    int stepCounter;        // Number of steps taken
    FILE *stepHistory;      // Temp file to store all the steps taken.
    FILE *currentFile;      // Pointer to the current file


    //Get Process ID and Username
    pid = getpid();
    //Concatenate hardcoded path and the Process ID for grading
    snprintf(filepath, 80, "./thorenje.rooms.%d", pid);
    //Create and change directory
    mkdir(filepath, 0770);
    chdir(filepath);

    //Create list of room names
    strcpy(roomNames[0], "Popcorn Stadium");
    strcpy(roomNames[1], "Waffle Emporium");
    strcpy(roomNames[2], "House of Pancakes");
    strcpy(roomNames[3], "Sandwich Place");
    strcpy(roomNames[4], "Candy Court");
    strcpy(roomNames[5], "Soda Springs");
    strcpy(roomNames[6], "Chocolate Canyon");
    strcpy(roomNames[7], "Pizza Planet");
    strcpy(roomNames[8], "Fruity Fountain");
    strcpy(roomNames[9], "Delicious Deli");

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

    //Begin Prompt for Game
    system("clear");
    printf("***** Mall Munchies *****\n\n");
    printf("You find yourself in a mall when you are blindsided \nwith an intense bout of hunger.\n\nThis isn't like anything you've felt before... \nand you're not quite sure what you want to eat!\n\nBetter head down to the food court!\n\n*************************\n\n");

    //Set Current Room to Start Room
    currentRoom = startRoom;
    //Open Step History File in Read/Write Mode
    stepHistory = fopen("tempStep", "w+");
    //Initialize the step counter
    stepCounter = 0;

    //Loop until the Person reaches the end room
    while(currentRoom != endRoom)
    {
        //Didn't find the room you want message
        printf("Man... this place looks good, but it's not quite what you're looking for... \n\n");

        //Open File
        currentFile = fopen(fileNames[currentRoom], "r");

        //Get and Display Current Room
        char nextLine[50];
        fgets(nextLine, sizeof(nextLine), currentFile);
        printf("CURRENT LOCATION: ");
        for(i=11; i < strlen(nextLine); i++)
        {
            printf("%c", nextLine[i]);
        }
        
        //Get and Display Connections
        printf("POSSIBLE CONNECTIONS: ");
        fgets(nextLine, 50, currentFile);
        do{
            //Prints the next connection
            for(i=14; i < strlen(nextLine); i++)
            {
                if(nextLine[i] != 10) //Don't print line returns.
                    printf("%c", nextLine[i]);
            }
            fgets(nextLine, 50, currentFile);
            if(nextLine[0]==67) //Begins with Capital C
                printf(", ");
            else
                printf(".\n");
        }while(nextLine[0]==67); //Begins with Capital C

        //Get Next Room
        char userLoc[50];
        do{
            printf("WHERE TO?>");
            fgets(userLoc, 50, stdin);
        }while(userLoc[0] ==10);//Keep prompting for an entry if empty.
        //Remove NewLine Character from userLoc (came from fgets)
        userLoc[strcspn(userLoc, "\n")] = '\0';
        //Validate that this place is on the connections list
        int found = 0;
        for(i = 0; i < 7; i++)
        {
            //Only consider rooms where the connection exists.
            if(connectionArr[currentRoom][i] !=0 && currentRoom != i)
            {
                //Compare the name of the room to the user's name
                if(strcmp(roomNames[chosenRooms[i]], userLoc) == 0)
                {
                    //Set that we found it.
                    found = 1;
                    //Increase the step counter.
                    stepCounter++;
                    //Append the chosen room to the stepHistory doc
                    fprintf(stepHistory, "%s\n", roomNames[chosenRooms[i]]);
                    //Change the current room index to the user's room
                    currentRoom = i;
                    //Break the for loop
                    break;
                }
            }
        }

        //If we didn't find it, print the error message and loop through the while loop again
        if(found == 0)
        {
            printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.");
        }

        //Divider Line
        printf("\n*************************\n\n");

        //Close File
        fclose(currentFile);
    }
    
    //User found end: congrats, step counter, step history
    printf("AHH! This place really hit the spot!\n");
    printf("You took %d steps to satiate your hunger!\n", stepCounter);
    printf("Your path to victory was: \n");
    //Display step history from temp file
    rewind(stepHistory);
    //Copy a text file to the screen
    //Researched at stackoverflow.com/questions/3463426
    int c;
    while((c = getc(stepHistory)) != EOF)
        putchar(c);

    printf("\n");

    //History File Close and Deleted.
    fclose(stepHistory);
    remove("tempStep");

    //Return Successfully
    return 0;
}
