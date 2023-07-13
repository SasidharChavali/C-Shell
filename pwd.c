#include "headers.h"

void pwd()
{
    getcwd(pwdir, PATH_MAX);
    printf("%s\n", pwdir);
    return;
}