#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

// extern int processInfo(uint);

int isValidPID(char* str){

    int len = 0 , i = 0 , digit = 0;

    len = strlen(str);

    for(i = 0;i < len ; i++){

        digit = str[i] - 48;
        if(digit >= 0 && digit <= 9 )
            continue;
        else
        return 0; 

    }

    return 1;

}

int main(int argc, char  *argv[])
{
    uint pid = 0;
    
    int len = 0 , i = 0 , digit = 0;     
    
    len = strlen(argv[1]);


    if(argc == 2 && isValidPID(argv[1])){
       
        for(i = 0 ;i < len ; i++){

            digit = argv[1][i] - 48;

            pid = pid*10 + digit;

        }


        // printf(1,"%d\n",pid);
        processInfo(pid);
        // currentlyRunning();

    }else{
        printf(1,"Illegal command or arguments\n");
    }

    exit(0);
}