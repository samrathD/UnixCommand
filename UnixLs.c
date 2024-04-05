#include "UnixLs.h"

int main(int argc, char**argv){
    int show_i = 0;
    int show_l = 0;
    
    //Parse the command line arguments
    int opt;
    //Use getopt a bulit in funciton to parse the command line arguments
    while ((opt = getopt(argc, argv, "il")) != -1) {
        switch (opt) {
            case 'i':
                show_i = 1;
                break;
            case 'l':
                show_l = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-il]\n", argv[0]);
                return 1;
        }
    }

    //if there are no files passed then show files of current directory
    if(optind >= argc){
        listFiles(".", show_i, show_l, 1);
    }
    else{
        for(int i = optind; i<argc; i++){
            listFiles(argv[i],show_i, show_l,1);
        }
    }
    return 0;
}

// list files in a directory
void listFiles(char* dirPath, int show_i, int show_l, int printDir) {
    int type = fileType(dirPath); // determine the type of file

    // if directory doesn't exist: 
    if (type == -1) {
        fprintf(stderr, "UnixLs: cannot access '%s': No such file or directory\n", dirPath);
        exit(EXIT_FAILURE);
    }

    // if the path is a directory:
    else if (type == 1) {
        DIR *dirp = opendir(dirPath); // Open the directory
        struct dirent *dir;

        if (dirp != NULL) {
            // print directory name 
            if (printDir) {
                printf("\n%s:\n", dirPath);
            }

            // print file names 
            while ((dir = readdir(dirp)) != NULL) {
                if (dir->d_name[0] != '.') {
                    fileInfo(dir->d_name, dirPath, show_i, show_l, 1);
                }
            }

            // print directory contents if show_l is true
            if (show_l == 1) {
                rewinddir(dirp);
                while ((dir = readdir(dirp)) != NULL) {
                    if (dir->d_name[0] == '.') {
                        continue;
                    }
                    struct stat fileStat;
                    char path[MAX_LEN];
                    memset(path, 0, sizeof(path));
                    snprintf(path, sizeof(path), "%s/%s", dirPath, dir->d_name);

                    // get file information using lstat
                    if (lstat(path, &fileStat) < 0) {
                        fprintf(stderr, "lstat failed for: %s\n", path);
                        exit(EXIT_FAILURE);
                    }

                    // if the file is a directory, list its contents recursively
                    // if(S_ISDIR(fileStat.st_mode)) {
                    //     listFiles(path, show_i, show_l, 1);
                    // }
                }
            }

            closedir(dirp); // close the directory
        }
    }

    //if the path exists but is not a directory
    else {
        fileInfo(dirPath, "", show_i, show_l, 0);
    }
}


//determines the type of file (directory or not)
int fileType(char* path) {
    //Stores information about the file file specified by the file path
    struct stat fileStat;

    //Return -1 if an error has occured
    if (lstat(path, &fileStat) < 0) {
        return -1; // Return -1 if lstat fails
    }
    //S_ISDIR checks if directory or not
    return S_ISDIR(fileStat.st_mode); // Return 1 if directory, 0 otherwise
}

// prints file information
void fileInfo(char* fName, char* dirPath, int show_i, int show_l, int concat) {
    char path[MAX_LEN];

    // concatenating the file name at the end of the file path if concat is true
    if (concat) {
        memset(path, 0, sizeof(path));
        snprintf(path, sizeof(path), "%s/%s", dirPath, fName);
    } else {
        strcpy(path, fName);
    }

    struct stat fileStat;

    // getting file information using lstat
    if (lstat(path, &fileStat) < 0) {
        fprintf(stderr, "File not found: %s\n", path);
        exit(EXIT_FAILURE);
    }

    // printing inode number if show_i is true
    if (show_i) {
        printf("%8lu ", fileStat.st_ino);
    }

    // printing detailed file information if show_l is true
    if (show_l) {
        modeInfo(fileStat.st_mode); //print file permissions
        printf("%2lu ", fileStat.st_nlink); //print number of hard links
        userInfo(fileStat.st_uid); //print user information
        groupInfo(fileStat.st_gid); //print group information
        printf("%6ld ", fileStat.st_size); //print file size
        timeInfo(fileStat.st_mtime); //print last modification time
    }

    printf("%s", fName); // print file name

    // print symbolic link target if file is a symbolic link and show_l is true
    if (show_l) {
        if (S_ISLNK(fileStat.st_mode)) {
            char real_path[MAX_LEN];
            memset(real_path, 0, sizeof(real_path));

            // get the target of the symbolic link
            if (readlink(path, real_path, sizeof(real_path) - 1) < 0) {
                fprintf(stderr, "readlink failed for: %s\n", path);
                exit(EXIT_FAILURE);
            }

            printf(" -> %s", real_path); // print the symbolic link target
        }
    }

    printf("\n"); // print a newline after each file
}

void modeInfo(mode_t mode) {
    if (S_ISDIR(mode)) {
        printf("d");
    } else if (S_ISLNK(mode)) {
        printf("l");
    } else {
        printf("-");
    }
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
    printf(" ");
}

//retrieve and print user information based on user ID
void userInfo(uid_t userid) {
    struct passwd *pw = NULL;
    pw = getpwuid(userid); //get user information based on user ID

    if (pw) {
        printf("%8s ", pw->pw_name); // print username
    } else {
        printf("Unknown user %u\n", userid); //error message if user not found
    }
}

//retrieve and print group information based on group number
void groupInfo(gid_t groupNum) {
    struct group *grp;
    grp = getgrgid(groupNum); // get group information based on group number

    if (grp) {
        printf("%6s ", grp->gr_name); //print group name
    } else {
        printf("Unknown group %u\n", groupNum); //error message if group not found
    }
}

//retrieve and print time information
void timeInfo(time_t time) {
    struct tm *timeinfo;
    timeinfo = localtime(&time); //convert time to local time representation

    switch (timeinfo->tm_mon) {
        case 0: printf("Jan "); break;
        case 1: printf("Feb "); break;
        case 2: printf("Mar "); break;
        case 3: printf("Apr "); break;
        case 4: printf("May "); break;
        case 5: printf("Jun "); break;
        case 6: printf("Jul "); break;
        case 7: printf("Aug "); break;
        case 8: printf("Sep "); break;
        case 9: printf("Oct "); break;
        case 10: printf("Nov "); break;
        case 11: printf("Dec "); break;
        default: printf("Error"); break; //error message if month is out of range
    }

    printf("%2d ", timeinfo->tm_mday); // Print day
    printf("%d ", timeinfo->tm_year + 1900); // print year
    printf("%02d:%02d ", timeinfo->tm_hour, timeinfo->tm_min); //print hour and minute
}

//set flags based on user input
void setFlags(char *input, int *opts) {
    int flag_pos = 1;
    while (1) {
        if (input[flag_pos] == 'i') {
            opts[0] = 1; // Set 'i' flag
        } else if (input[flag_pos] == 'l') {
            opts[1] = 1; // Set 'l' flag
        } else if (input[flag_pos] == 'R') {
            opts[2] = 1; // Set 'R' flag
        } else {
            // If an illegal option is detected or no options are provided
            if (input[flag_pos] != '\0' || flag_pos == 1) {
                fprintf(stderr, "Illegal option: %s\n", input);
                fprintf(stderr, "Usage: ./UnixLs [-ilR] [file ...]\n"); // print usage message
                exit(EXIT_FAILURE); // Exit fail
            }
            break;
        }
        flag_pos++;
    }
}