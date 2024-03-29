/*
UNIX projects
@alkuzin - 2024
*/

#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include "ls.h"


char ls_get_type(mode_t mode)
{
    switch (S_IFMT & mode) {
        case S_IFREG:
            return '-';
        case S_IFDIR:
            return 'd';
        case S_IFCHR:
            return 'c';
        case S_IFBLK:
            return 'b';
        case S_IFIFO:
            return 'p';
        case S_IFSOCK:
            return 's';
        case S_IFLNK:
            return 'l';
        default:
            return '?';
    }
}

void ls_get_mode(char *buffer, mode_t mode)
{
    buffer[0] = ls_get_type(mode);
    buffer[1] = (S_IRUSR & mode) ? 'r' : '-';
    buffer[2] = (S_IWUSR & mode) ? 'w' : '-';
    buffer[3] = (S_IXUSR & mode) ? 'x' : '-';
    buffer[4] = (S_IRGRP & mode) ? 'r' : '-';
    buffer[5] = (S_IWGRP & mode) ? 'w' : '-';
    buffer[6] = (S_IXGRP & mode) ? 'x' : '-';
    buffer[7] = (S_IROTH & mode) ? 'r' : '-';
    buffer[8] = (S_IWOTH & mode) ? 'w' : '-';
    buffer[9] = (S_IXOTH & mode) ? 'x' : '-';
}

static size_t ls_utoa_len(unsigned int n)
{
    size_t len;


    len = 0;

    if (n == 0)
        return 1;

    while (n >= 1)
    {
        len++;
        n /= 10;
    }

    return len;
}

int ls_get_max_size_len(ls_t *ls)
{
    int    max_size_len;
    ls_file_t *file;
    size_t size;

    max_size_len = 0;

    for (size_t i = 0; i < ls->size; i++) {
        file = &ls->files[i];
        size = ls_utoa_len(file->size);
    
        if ((size_t) max_size_len < size)
            max_size_len = size;
    }

    return max_size_len;
}

void ls_printdir_long_list_fmt(ls_t *ls)
{
    ls_file_t *file;

    int max_size_len;

    max_size_len = ls_get_max_size_len(ls);

    for (size_t i = 0; i < ls->size; i++) {
        
        file = &ls->files[i];

        printf("%s %2lu %s %s %*lu %-5s  %s\n", file->mode, file->nlink, file->user, 
        file->group, max_size_len, file->size, file->date, file->filename);
    }
}

void ls_init(ls_t *ls)
{
    bzero(ls, sizeof(ls_t));
    ls->dir   = NULL;
    ls->dp    = NULL;
    ls->size  = 0;
    ls->flags = 0;
}

void ls_opendir(ls_t *ls, const char *path)
{
    ls->dir = opendir(path);

    strncpy(ls->dirname, path, 256);

    if (!ls->dir) {
        printf("ls: cannot access \'%s\': %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void ls_readdir(ls_t *ls)
{
    char   *user, *group;
    size_t size, nbtime;
    struct tm *mdate;
    int    status, i;
    ls_file_t *file;
    time_t mtime;
    stat_t sb;

    i = 0;

    char filename[512];

    for (;;) {
        ls->dp = readdir(ls->dir);
        
        if (!ls->dp) {
            if (errno) {
                perror("ls: readdir");
                exit(EXIT_FAILURE);
            }
            break;
        }

        bzero(filename, sizeof(filename));
        strncpy(filename, ls->dirname, 256);
        strncat(filename, ls->dp->d_name, 256);

        if (strncmp(ls->dp->d_name, ".", 1) == 0 || strncmp(ls->dp->d_name, "..", 2) == 0) {
            ls->size--;
            continue;
        }

        status = lstat(filename, &sb);

        if (status == -1) {
            printf("FILENAME: %s\n", filename);
            perror("ls: stat error");
            exit(EXIT_FAILURE);
        }

        file  = &ls->files[i];
        user  = getpwuid(sb.st_uid)->pw_name;
        group = getgrgid(sb.st_gid)->gr_name;
        size  = sb.st_size;
        mtime = sb.st_mtime;
        mdate = localtime(&mtime);
        
        strncpy(file->filename, ls->dp->d_name, strlen(ls->dp->d_name));
        strncpy(file->group, group, strlen(group));
        strncpy(file->user, user, strlen(user));
        ls_get_mode(file->mode, sb.st_mode);
        file->nlink = sb.st_nlink;
        file->size  = size;
        nbtime      = strftime(file->date, sizeof(file->date), "%b %e %R", mdate);

        if (!nbtime) {
            perror("ls: strftime");
            exit(EXIT_FAILURE);
        }

        i++;
    }

    ls->size = i;
}

void ls_printdir(ls_t *ls)
{
    char   *filename, *filename1, *filename2;
    size_t total_filenames_len, group_size;
    size_t first_group_max_len, len;

    ls_file_t *file;

    group_size          = (ls->size / 2) + 1;
    total_filenames_len = 0;
    first_group_max_len = 0;
    len                 = 0;

    for (size_t i = 0; i < ls->size; i++) {

        file     = &ls->files[i];
        filename = file->filename;
        len      = strlen(filename);
        total_filenames_len += len;

        if (i < group_size) {
            if (first_group_max_len < len)
                first_group_max_len = len;
        }
    }

    if (total_filenames_len + (2 * ls->size) <= 96) {
        for (size_t i = 0; i < ls->size; i++) {

            file     = &ls->files[i];
            filename = file->filename;
        
            if (strchr(filename, ' '))
                printf("\'%s\'", filename);
            else
                printf("%s  ", filename);
        }

        putchar('\n');
    }
    else if ((total_filenames_len / 5) + (2 * ls->size) <= 96) {
        for (size_t i = 0; i < ls->size; i++) {

            file     = &ls->files[i];
            filename = file->filename;
        
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
        for (size_t i = 0; i < group_size; i++) {

            filename1 = ls->files[i].filename;
            filename2 = ls->files[i + group_size].filename;
            len       = strlen(filename1);
        
            if (strchr(filename1, ' '))
                printf("\'%-*s\'  ", (int) first_group_max_len, filename1);
            else
                printf("%-*s  ", (int) first_group_max_len, filename1);

            if (strchr(filename2, ' '))
                printf("\'%s\'\n", filename2);
            else
                printf("%s\n", filename2);
        }
    }
}

void ls_closedir(ls_t *ls)
{
    int status;

    status = closedir(ls->dir);

    if (status == -1) {
        perror("ls: error to close directory");
        exit(EXIT_FAILURE);
    }
}
