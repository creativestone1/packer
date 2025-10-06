#pragma once
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdint.h>
int getFileType(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        perror("stat");
        return -1;
    }

    if (S_ISDIR(path_stat.st_mode))
        return DT_DIR;
    if (S_ISREG(path_stat.st_mode))
        return DT_REG;
	return 0;
}
mode_t getFilePermissions(const char* path) {
    struct stat st;
    if (stat(path, &st) != 0) {
        perror("stat failed");
        return (mode_t)-1;
    }
	//07777 - это маска с st_mode где лежат права файла
    return st.st_mode & 07777;
}

int setFilePermissions(const char* path, mode_t mode) {
    if (chmod(path, mode) != 0) {
        perror("chmod failed");
        return -1;
    }
    return 0;
}
