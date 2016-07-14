#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

//Method for creating room directory
void makeRoomDirectory(int pid, char *filepath)
{
    //Concatenate the string name
    strcpy(filepath, "./thorenje.rooms.");
    strcat(filepath, "temp");
    mkdir(filepath, 0700);
    return;
}


//Main method
int main(void)
{
    //Get Process ID and Username
    int pid = getpid();
    char filepath[80];

    //Create Room Directory and store the Path to the directory
    makeRoomDirectory(pid, filepath);
    printf("%s\n", filepath);
    return 0;
}

