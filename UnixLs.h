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
void listFiles(char* dirName, int show_i, int show_l, int dirFlag);

// print file info specified on path
void fileInfo(char* fileName, char* dirName, int show_i, int show_l, int concatFlag);

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

// check if file, directory or does not exist
int fileType(char* filePath);


// void listFiles(char* dirPath, int* opts, int printDir) {
//     int type = fileType(dirPath);

//     // If directory doesn't exist 
//     if (type == -1) {
//         fprintf(stderr, "File not found: %s\n", dirPath);
//         exit(EXIT_FAILURE);
//     }

//     // If the path is a directory 
//     else if (type == 1) {
//         DIR *dirp = opendir(dirPath);
//         struct dirent *dir;

//         if (dirp != NULL) {
//         // Print directory name 
//         if (printDir) {
//             printf("\n%s:\n", dirPath);
//         }

//         //print file names 
//         while ((dir = readdir(dirp)) != NULL) {
//             if (dir->d_name[0] == '.') {
//             continue;
//             }
//             fileInfo(dir->d_name, dirPath, opts, 1);
//         }

//         // print directory content recursively
//         if (opts[2] == 1) {
//             rewinddir(dirp);
//             while ((dir = readdir(dirp)) != NULL) {
//             if (dir->d_name[0] == '.') {
//                 continue;
//             }
//             struct stat fileStat;
//             char path[MAX_LEN];
//             memset(path, 0, sizeof(path));
//             snprintf(path, sizeof(path), "%s/%s", dirPath, dir->d_name);

//             if (lstat(path, &fileStat) < 0) {
//                 fprintf(stderr, "lstat failed for: %s\n", path);
//                 exit(EXIT_FAILURE);
//             }

//             if(S_ISDIR(fileStat.st_mode)) {
//                 listFiles(path, opts, 1);
//             }
//             }
//         }

//         closedir(dirp);
//         }
//     }

//     // If the path exists but is not a directory
//     else {
//         fileInfo(dirPath, "", opts, 0);
//     }
// }