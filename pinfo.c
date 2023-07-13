#include "headers.h"

void pinfo(char **input, int size)
{
    char path[MAX_LEN];
    char buf[MAX_LEN];
    int cpid, len = 0;
    if (size == 1)
    {
        cpid = getpid();
    }
    if (size == 2)
    {
        cpid = atoi(input[1]);
    }
    sprintf(path, "/proc/%d/stat", cpid);
    FILE *fp = fopen(path, "r");
    if (!fp)
    {
        perror("Error while opening the proc/pid/stat file");
    }
    else
    {
        printf("pid : %d\n", cpid);
        fgets(buf, MAX_LEN, fp);
        char *tokens[MAX_LEN];
        int index = 0;
        tokens[index] = strtok(buf, " ");
        while (tokens[index] != NULL)
        {
            index++;
            tokens[index] = strtok(NULL, " ");
        }
        if (!strcmp(tokens[4], tokens[7]))
            printf("process status : %s+\n", tokens[2]);
        else
            printf("process status : %s\n", tokens[2]);

        printf("Memory : %s {Virtual Memory}\n", tokens[22]);
        fclose(fp);
    }

    sprintf(path, "/proc/%d/exe", cpid);
    len = readlink(path, buf, MAX_LEN);
    if (len == -1)
    {
        perror("No path for executable");
    }
    else
    {
        buf[len] = '\0';
        char *p;
        p = strstr(buf, parentdir);
        if (p != NULL)
        {
            p += strlen(parentdir);
            printf("executable path : ~%s\n", p);
        }
        else
            printf("executable path : ~%s\n", buf);
    }
    return;
}
