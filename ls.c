#include "headers.h"

int comparator(const struct dirent **a, const struct dirent **b)
{
    int len_a = strlen((*a)->d_name);
    int len_b = strlen((*b)->d_name);
    int i = 0, j = 0;
    char ch1[len_a], ch2[len_b];
    strcpy(ch1, (*a)->d_name);
    strcpy(ch2, (*b)->d_name);

    if (ch1[i] >= 'A' && ch1[i] <= 'Z')
        ch1[i] = ch1[i] + 32;
    if (ch2[j] >= 'A' && ch2[j] <= 'Z')
        ch2[j] = ch2[j] + 32;

    if (strcmp(&ch1[i], &ch2[j]) < 0)
    {
        return 1;
    }
    else if (strcmp(&ch1[i], &ch2[j]) > 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void ls_details(char *target, int flag)
{
    char time[12];
    struct stat stats;
    struct group *gp;
    if (stat(target, &stats) == 0)
    {
        if (S_ISREG(stats.st_mode))
        {
            printf((S_ISDIR(stats.st_mode)) ? "d" : "-");
            printf((stats.st_mode & S_IRUSR) ? "r" : "-");
            printf((stats.st_mode & S_IWUSR) ? "w" : "-");
            printf((stats.st_mode & S_IXUSR) ? "x" : "-");
            printf((stats.st_mode & S_IRGRP) ? "r" : "-");
            printf((stats.st_mode & S_IWGRP) ? "w" : "-");
            printf((stats.st_mode & S_IXGRP) ? "x" : "-");
            printf((stats.st_mode & S_IROTH) ? "r" : "-");
            printf((stats.st_mode & S_IWOTH) ? "w" : "-");
            printf((stats.st_mode & S_IXOTH) ? "x " : "- ");
            strncpy(time, ctime(&stats.st_mtime) + 4, 12);
            gp = getgrgid(stats.st_gid);
            printf("%3lu %6s %6s %7ld %12s %-40s\n", stats.st_nlink, username, gp->gr_name, stats.st_size, time, target);
            return;
        }
    }
    chdir(target);
    DIR *dh = opendir(".");
    if (!dh)
    {
        if (errno = ENOENT)
        {
            perror("Directory doesn't exist");
        }
        else
        {
            perror("Unable to read directory");
        }
        return;
    }
    struct dirent *d;
    long long int blocks = 0;
    while ((d = readdir(dh)) != NULL)
    {
        if (stat(d->d_name, &stats) == 0)
        {
            if (flag == 0)
            {
                if (d->d_name[0] == '.')
                    continue;
            }
            blocks = blocks + stats.st_blocks;
        }
    }
    printf("total %lld\n", blocks / 2);
    struct dirent **namelist;
    int n = scandir(".", &namelist, NULL, comparator);
    if (n == -1)
    {
        perror("scandir");
        return;
    }
    while (n--)
    {
        if (namelist[n] == NULL)
        {
            continue;
        }
        if (flag == 0)
        {
            if (namelist[n]->d_name[0] == '.')
                continue;
        }
        if (stat(namelist[n]->d_name, &stats) != 0)
        {
            perror("File doesn't exist");
            continue;
        }
        printf((S_ISDIR(stats.st_mode)) ? "d" : "-");
        printf((stats.st_mode & S_IRUSR) ? "r" : "-");
        printf((stats.st_mode & S_IWUSR) ? "w" : "-");
        printf((stats.st_mode & S_IXUSR) ? "x" : "-");
        printf((stats.st_mode & S_IRGRP) ? "r" : "-");
        printf((stats.st_mode & S_IWGRP) ? "w" : "-");
        printf((stats.st_mode & S_IXGRP) ? "x" : "-");
        printf((stats.st_mode & S_IROTH) ? "r" : "-");
        printf((stats.st_mode & S_IWOTH) ? "w" : "-");
        printf((stats.st_mode & S_IXOTH) ? "x " : "- ");
        strncpy(time, ctime(&stats.st_mtime) + 4, 12);
        gp = getgrgid(stats.st_gid);

        printf("%3lu %6s %6s %7ld %12s %-40s\n", stats.st_nlink, username, gp->gr_name, stats.st_size, time, namelist[n]->d_name);

        free(namelist[n]);
    }
    free(namelist);
    closedir(dh);
    chdir(pwdir);
    return;
}

void ls_normal(char *target, int flag)
{
    char time[12];
    struct stat stats;
    struct group *gp;
    if (stat(target, &stats) == 0)
    {
        if (S_ISREG(stats.st_mode))
        {
            printf("%s\n", target);
            return;
        }
    }
    DIR *dh = opendir(target);
    if (!dh)
    {
        if (errno = ENOENT)
        {
            perror("Directory doesn't exist");
        }
        else
        {
            perror("Unable to read directory");
        }
        return;
    }
    struct dirent **namelist;
    int n = scandir(target, &namelist, NULL, comparator);
    if (n == -1)
    {
        perror("scandir");
        return;
    }

    while (n--)
    {
        if (namelist[n] == NULL)
        {
            continue;
        }
        if (flag == 0)
        {
            if (namelist[n]->d_name[0] == '.')
                continue;
        }
        printf("%s\n", namelist[n]->d_name);
        free(namelist[n]);
    }
    free(namelist);
    closedir(dh);
    return;
}

void ls(char **input, int size)
{
    getcwd(pwdir, PATH_MAX);
    struct dirent **namelist;
    int n;
    long long blocks = 0;
    struct dirent *d;
    DIR *dh;
    struct stat stats, vfs;
    struct tm *values;
    struct group *gp;
    char path[PATH_MAX];

    int flag_var = 0, var = 0;
    int file_var = 0, file_count = 0;
    for (int i = 1; i < size; i++)
    {
        if (!strcmp(input[i], "-la") || !strcmp(input[i], "-al"))
        {
            flag_var = 1;
            var = 1;
        }
        if (!strcmp(input[i], "-a"))
        {
            flag_var = 1;
        }
        if (!strcmp(input[i], "-l"))
        {
            var = 1;
        }
        if (input[i][0] != '-')
        {
            file_count++;
        }
    }

    if (file_count == 0)
    {
        if (var == 1)
        {
            ls_details(".", flag_var);
        }
        else
        {
            ls_normal(".", flag_var);
        }
    }
    else
    {
        if (var == 1)
        {
            for (int i = 1; i < size; i++)
            {
                if (input[i][0] != '-')
                {
                    if (stat(input[i], &stats) != 0)
                    {
                        printf("ls: cannot access %s : No such file or directory\n", input[i]);
                        continue;
                    }
                    if (file_count > 1)
                        printf("%s:\n", input[i]);
                    ls_details(input[i], flag_var);
                }
            }
        }
        else
        {
            for (int i = 1; i < size; i++)
            {
                if (input[i][0] != '-')
                {
                    if (stat(input[i], &stats) != 0)
                    {
                        printf("ls: cannot access %s : No such file or directory\n", input[i]);
                        continue;
                    }
                    if (file_count > 1)
                        printf("%s:\n", input[i]);
                    ls_normal(input[i], flag_var);
                }
            }
        }
    }
    return;
}
