#include "UnixLs.h"

int main(int argc, char**argv){
    printf("The number of arguments are %d\n",argc);

    for(int i = 0; i<argc; i++){
        printf("The arguments are %d\n",argv[i][0]);
    }

    //First extract the proper information
}

void listFiles(char* dirPath, int* opts, int printDir) {
    int type = fileType(dirPath);

    // If directory doesn't exist 
    if (type == -1) {
        fprintf(stderr, "File not found: %s\n", dirPath);
        exit(EXIT_FAILURE);
    }

    // If the path is a directory 
    else if (type == 1) {
        DIR *dirp = opendir(dirPath);
        struct dirent *dir;

        if (dirp != NULL) {
        // Print directory name 
        if (printDir) {
            printf("\n%s:\n", dirPath);
        }

        //print file names 
        while ((dir = readdir(dirp)) != NULL) {
            if (dir->d_name[0] == '.') {
            continue;
            }
            fileInfo(dir->d_name, dirPath, opts, 1);
        }

        // print directory content recursively
        if (opts[2] == 1) {
            rewinddir(dirp);
            while ((dir = readdir(dirp)) != NULL) {
            if (dir->d_name[0] == '.') {
                continue;
            }
            struct stat fileStat;
            char path[MAX_LEN];
            memset(path, 0, sizeof(path));
            snprintf(path, sizeof(path), "%s/%s", dirPath, dir->d_name);

            if (lstat(path, &fileStat) < 0) {
                fprintf(stderr, "lstat failed for: %s\n", path);
                exit(EXIT_FAILURE);
            }

            if(S_ISDIR(fileStat.st_mode)) {
                listFiles(path, opts, 1);
            }
            }
        }

        closedir(dirp);
        }
    }

    // If the path exists but is not a directory
    else {
        fileInfo(dirPath, "", opts, 0);
    }
}

int fileType(char* path) {
    struct stat fileStat;
    if (lstat(path, &fileStat) < 0) {
        return -1;
    }
    return S_ISDIR(fileStat.st_mode);
}

void fileInfo(char* fName, char* dirPath, int* opts, int concat) {
    char path[MAX_LEN];
    if (concat) {
        memset(path, 0, sizeof(path));
        snprintf(path, sizeof(path), "%s/%s", dirPath, fName);
    } else {
        strcpy(path, fName);
    }

    struct stat fileStat;

    if (lstat(path, &fileStat) < 0) {
        fprintf(stderr, "File not found: %s\n", path);
        exit(EXIT_FAILURE);
    }

    if (opts[0]) {
        printf("%8lu ", fileStat.st_ino);
    }

    if (opts[1]) {
        modeInfo(fileStat.st_mode);
        printf("%2lu ", fileStat.st_nlink);
        userInfo(fileStat.st_uid);
        groupInfo(fileStat.st_gid);
        printf("%6ld ", fileStat.st_size);
        timeInfo(fileStat.st_mtime);
    }

    printf("%s", fName);

    if (opts[1]) {
        if (S_ISLNK(fileStat.st_mode)) {
        char real_path[MAX_LEN];
        memset(real_path, 0, sizeof(real_path));

        if (readlink(path, real_path, sizeof(real_path) - 1) < 0) {
            fprintf(stderr, "readlink failed for: %s\n", path);
            exit(EXIT_FAILURE);
        }

        printf(" -> %s", real_path);
        }
    }

    printf("\n");
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

void userInfo(uid_t userid) {
    struct passwd *pw = NULL;
    pw = getpwuid(userid);

    if (pw) {
        printf("%8s ", pw->pw_name);
    } else {
        printf("Unknown user %u\n", userid);
    }
}

void groupInfo(gid_t groupNum) {
    struct group *grp;
    grp = getgrgid(groupNum);

    if (grp) {
        printf("%6s ", grp->gr_name);
    } else {
        printf("Unknown group %u\n", groupNum);
    }
}

void timeInfo(time_t time) {
    struct tm *timeinfo;
    timeinfo = localtime (&time);

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
        default: printf("Error"); break;
    }

    printf("%2d ", timeinfo->tm_mday);
    printf("%d ", timeinfo->tm_year + 1900);
    printf("%02d:%02d ", timeinfo->tm_hour, timeinfo->tm_min);
}

void setFlags(char* input, int* opts) {
    int flag_pos = 1;
    while (1) {
        if (input[flag_pos] == 'i') {
        opts[0] = 1;
        } else if (input[flag_pos] == 'l') {
        opts[1] = 1;
        } else if (input[flag_pos] == 'R') {
        opts[2] = 1;
        } else {
        if (input[flag_pos] != '\0' || flag_pos == 1) {
            fprintf(stderr, "Illegal option: %s\n", input);
            fprintf(stderr, "Usage: ./UnixLs [-ilR] [file ...]\n");
            exit(EXIT_FAILURE);
        }
        break;
        }
        flag_pos++;
    }
}
