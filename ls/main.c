/*
UNIX projects
@alkuzin - 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ls.h"

void ls_help(void);

int main(int argc, char **argv)
{
    char dir[256];
    int  opt;
    ls_t ls;

    ls_init(&ls);
    bzero(dir, sizeof(dir));
    strncpy(dir, "./", 3);

    if (argc > 1 && strncmp(argv[1], "--help", 6) == 0)
        ls_help();

    while ((opt = getopt(argc, argv, "lRart")) != -1) {
        switch (opt) {
            case 'l':
                ls.flags |= LS_FLAG_LONG_LIST;
                break;
            case 'R':
                ls.flags |= LS_FLAG_RECURCIVE;
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
            default:
                ls_help();
        }
    }

    if (!ls.flags && argc > 1)
        strncpy(dir, argv[1], 256);
    else if (argc > 1 && argv[argc - 1][0] != '-')
        strncpy(dir, argv[argc - 1], 256);

/*    printf("argc: %d\n", argc);
    printf("argv[argc - 1]: \"%s\"\n", argv[argc - 1]);
    printf("dir: \"%s\"\n", dir);
*/
    /* open directory */
    ls_opendir(&ls, dir);
    
    /* read files from directory */
    ls_readdir(&ls);

    ls_printdir_long_list_fmt(&ls);

    /* close directory */
    ls_closedir(&ls);

    return 0;

}

void ls_help(void)
{
    puts("Help ...");
    exit(EXIT_SUCCESS);
}
