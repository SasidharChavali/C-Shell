#include "headers.h"

void cd(char **input, int args)
{
    if (args > 2)
    {
        perror("Too many arguments");
        return;
    }

    else if (args == 1 || (args == 2 && !strcmp(input[1], "~")))
    {
        chdir(parentdir);
    }
    
    else if ((args == 2 && !strcmp(input[1], "-")))
    {
        char *prev;
        prev = getcwd(prev, 0);
        if (prev == NULL)
        {
            perror("OLDPWD not set");
            return;
        }
        chdir(prev);
        free(prev);
    }
 
    else if (chdir(input[1]) != 0)
    {
        perror("bash");
    }

    else if(args==2)
    {
        chdir(input[1]);
    }
    return;
}