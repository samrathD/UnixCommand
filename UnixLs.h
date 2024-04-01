#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

#define MAX_LEN 4096 

// Print information of files in the specified directory 
void listFiles(char* dirName, int* flags, int dirFlag);

// print file info specified on path
void fileInfo(char* fileName, char* dirName, int* flags, int concatFlag);

// print the r/w/e permission
void modeInfo(mode_t mode);

// user and file info
void userInfo(uid_t userid);

// group info
void groupInfo(gid_t groupNum);

// last modified info
void timeInfo(time_t time);

// set flags based on input
void setFlags(char* input, int* flags);

// check file type
int fileType(char* filePath);