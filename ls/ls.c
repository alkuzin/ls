/*
UNIX projects
@alkuzin - 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "ls.h"


void ls_init(ls_t *ls)
{
    bzero(ls->filenames, sizeof(ls->filenames));
    ls->dir  = NULL;
    ls->dp   = NULL;
    ls->size = 0;
}

void ls_opendir(ls_t *ls, const char *path)
{
    ls->dir = opendir(path);

    if (!ls->dir) {
        perror("error to open directory");
        exit(EXIT_FAILURE);
    }
}

void ls_readdir(ls_t *ls)
{
    char *filename;
    int  files;

    files = 0;

    for (;;) {
        ls->dp = readdir(ls->dir);
        
        if (!ls->dp) {
            if (errno) {
                perror("readdir");
                exit(EXIT_FAILURE);
            }
            break;
        }

        filename = ls->dp->d_name;

        if (strncmp(filename, ".", 1) == 0 || strncmp(filename, "..", 2) == 0) {
            ls->size--;
            continue;
        }
        
        strncpy(ls->filenames[files], ls->dp->d_name, strlen(ls->dp->d_name));
        files++;
    }

    ls->size = files;
}

void ls_printdir(ls_t *ls)
{
    char *filename;
    size_t total_filenames_len;

    total_filenames_len = 0;
    
    for (size_t i = 0; i < ls->size; i++) {
        filename = ls->filenames[i];
        total_filenames_len += strlen(filename);
    }

    if (total_filenames_len + (2 * ls->size) <= 96)
    {
        for (size_t i = 0; i < ls->size; i++) {

            filename = ls->filenames[i];
        
            if (strchr(filename, ' '))
                printf("\'%s\'", filename);
            else
                printf("%s  ", filename);
        }

        putchar('\n');
    }

    else if ((total_filenames_len / 5) + (2 * ls->size) <= 96) {
        for (size_t i = 0; i < ls->size; i++) {

            filename = ls->filenames[i];
        
            if (strchr(filename, ' '))
                printf("\'%s\'  ", filename);
            else
                printf("%s  ", filename);

            if (i && (i + 1) % 5 == 0)
                putchar('\n');
        }

        putchar('\n');
    }
    else {
        for (size_t i = 0; i < ls->size; i++) {

            filename = ls->filenames[i];
        
            if (strchr(filename, ' '))
                printf("\'%s\'\n", filename);
            else
                printf("%s\n", filename);
        }
    }
}

void ls_closedir(ls_t *ls)
{
    int status;

    status = closedir(ls->dir);

    if (status == -1) {
        perror("error to close directory");
        exit(EXIT_FAILURE);
    }
}
