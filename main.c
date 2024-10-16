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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ls.h"


int main(int argc, char **argv)
{
    char dir[256];
    int  opt;
    ls_t ls;

    ls_init(&ls);
    bzero(dir, sizeof(dir));

    /* show list of options */
    if (argc > 1 && strncmp(argv[1], "--help", 6) == 0)
        ls_help();

    /* set flags */
    while ((opt = getopt(argc, argv, "lartS")) != -1) {
        switch (opt) {
            case 'l':
                ls.flags |= LS_FLAG_LONG_LIST;
                break;
            case 'a':
                ls.flags |= LS_FLAG_ALL;
                break;
            case 'r':
                ls.flags |= LS_FLAG_REVERSE;
                break;
            case 't':
                ls.flags |= LS_FLAG_TIME;
                break;
            case 'S':
                ls.flags |= LS_FLAG_SIZE;
                break;
            default:
                puts("Try 'ls --help' for more information.");
                exit(EXIT_FAILURE);
        }
    }

    /* set directory path */
    if (!ls.flags && argc > 1)
        strncpy(dir, argv[1], 256);
    else if (argc > 1 && argv[argc - 1][0] != '-')
        strncpy(dir, argv[argc - 1], 256);
    else
        strncpy(dir, "./", 3);

    /* open directory */
    ls_opendir(&ls, dir);

    /* read files from directory */
    ls_readdir(&ls);

    /* print directory content */
    ls_printdir(&ls);

    /* close directory */
    ls_destroy(&ls);

    return 0;
}