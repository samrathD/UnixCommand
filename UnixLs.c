#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h> 

int main(int argc, char**argv){
    printf("The number of arguments are %d\n",argc);

    for(int i = 0; i<argc; i++){
        printf("The arguments are %s\n",argv[i][0]);
    }

    //First extract the proper information
}