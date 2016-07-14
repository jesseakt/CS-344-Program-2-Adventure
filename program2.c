#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>

//Main method
int main(void)
{
    //Declare Variables
    int i, j;               // Loop iterators
    int pid;                // Process ID
    char filepath[80];      // Filepath for room directory
    char *roomNames[10];    // Array of possible room names
    int chosenRooms[7];     // Array of actually selected room choices


    //Get Process ID and Username
    pid = getpid();
    //Concatenate hardcoded path and the Process ID for grading
    snprintf(filepath, 80, "./thorenje.rooms.%d", pid);
    //Create Directory
    mkdir(filepath, 0770);

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
    for(i=0; i<7; i++)
    {
        printf("%s\n", roomNames[chosenRooms[i]]);
    }


    return 0;
}

