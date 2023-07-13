#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdint.h>
#include <ftw.h>
#include <grp.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/statvfs.h>
#include <fcntl.h>

#define MAX_LEN 1000
#define HISTORY_LEN 20
#define MAX_PROC_COUNT 30

int bg_count;

typedef struct bg_proc
{
    char name[MAX_LEN];
    pid_t pid;
} bg_proc;

bg_proc proc[MAX_PROC_COUNT];

char *hist[HISTORY_LEN];
int hist_count = 0;

int len;
char dis[MAX_LEN];
char path[MAX_LEN];
char pwdir[PATH_MAX];
char username[MAX_LEN];
char hostname[MAX_LEN];
char parentdir[PATH_MAX];


void prompt();

#endif