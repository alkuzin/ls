/**
 * Command to list computer files and directories.
 * Copyright (C) 2024  Alexander (@alkuzin).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _LS_H_
#define _LS_H_

#include <sys/stat.h>
#include <dirent.h>
#include <stdint.h>


typedef struct stat stat_t;

/* file structure */
typedef struct ls_file_s {
    char   filename[256];
    char   group[256];
    char   user[256];
    char   mode[12];
    char   date[16];
    size_t nlink;
    size_t size;
    time_t mtime;
} ls_file_t;

/* ls structure */
typedef struct ls_s {
    ls_file_t files[256];
    char    dirname[256];
    struct  dirent *dp;
    size_t  size;
    DIR     *dir;
    uint8_t flags;
} ls_t;

/* ls flags -l -a -r -t -S */
#define LS_FLAG_LONG_LIST 0x01
#define LS_FLAG_ALL       0x02
#define LS_FLAG_REVERSE   0x04
#define LS_FLAG_TIME      0x08
#define LS_FLAG_SIZE      0x10

/* initialize ls struct */
void ls_init(ls_t *ls);

/* open directory */
void ls_opendir(ls_t *ls, const char *path);

/* read directory */
void ls_readdir(ls_t *ls);

/* print directory contents */
void ls_printdir(ls_t *ls);

/* close directory & free ls struct */
void ls_destroy(ls_t *ls);

/* show list of options */
void ls_help(void);

#endif /* _LS_H_ */