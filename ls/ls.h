/*
UNIX projects
@alkuzin - 2024
*/

#ifndef _UNIX_PROJECT_LS_H_
#define _UNIX_PROJECT_LS_H_

#include <sys/stat.h>
#include <dirent.h>
#include <stdint.h>


typedef struct ls_file_s {
    char   filename[256];
    char   group[256];
    char   user[256];
    char   mode[12];
    char   date[16];
    size_t nlink;
    size_t size;
} ls_file_t;

typedef struct ls_s {
    ls_file_t files[256];
    char    dirname[256];
    struct dirent *dp;
    size_t  size;
    DIR     *dir;
    uint8_t flags;
} ls_t;

typedef struct stat stat_t;

#define LS_FLAG_LONG_LIST 0x01
#define LS_FLAG_RECURCIVE 0x02
#define LS_FLAG_ALL       0x04
#define LS_FLAG_REVERSE   0x08
#define LS_FLAG_TIME      0x10

void ls_init(ls_t *ls);
void ls_opendir(ls_t *ls, const char *path);
void ls_readdir(ls_t *ls);
void ls_printdir_long_list_fmt(ls_t *ls);
void ls_printdir(ls_t *ls);
void ls_closedir(ls_t *ls);


#endif /* _UNIX_PROJECT_LS_H_ */
