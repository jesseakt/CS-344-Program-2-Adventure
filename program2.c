#include <stdio.h>
#include <sys/types.h>

//Main method
int main(void)
{
    //Get Process ID and Username
    int pid = getpid();
    char filepath[80];
    //Concatenate a hardcoded path and the Process ID
    snprintf(filepath, 80, "./thorenje.rooms.%d", pid);
    //Create Directory
    mkdir(filepath, 0700);

    return 0;
}

