#include "UnixLs.h"

int main(int argc, char**argv){
    printf("The number of arguments are %d\n",argc);

    for(int i = 0; i<argc; i++){
        printf("The arguments are %s\n",argv[i][0]);
    }

    //First extract the proper information
}