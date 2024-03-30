/*
UNIX projects
@alkuzin - 2024
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
