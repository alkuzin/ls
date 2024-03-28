/*
UNIX projects
@alkuzin - 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ls.h"


int main(int argc, char **argv)
{
    ls_t ls;

    ls_init(&ls);

    /* open directory */
    if (argc == 1)
        ls_opendir(&ls, "./");
    else if (argc == 2)
        ls_opendir(&ls, argv[1]);

    /* read files from directory */
    ls_readdir(&ls);

    ls_printdir(&ls);

    /* close directory */
    ls_closedir(&ls);

    return 0;
}
