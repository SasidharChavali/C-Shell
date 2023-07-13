#include "headers.h"

static int display_info(const char *fpath, const struct stat *sb, int tflag)
{
    printf("%s\n", fpath);
    return 0;
}

static int display_info_d(const char *fpath, const struct stat *sb, int tflag)
{
    if (S_ISDIR(sb->st_mode))
        printf("%s\n", fpath);
    return 0;
}

static int display_info_f(const char *fpath, const struct stat *sb, int tflag)
{
    if (S_ISREG(sb->st_mode))
        printf("%s\n", fpath);
    return 0;
}

static int find_file(const char *fpath, const struct stat *sb, int tflag)
{
    int length = strlen(fpath), var = 1;
    if (length >= len)
    {
        for (int i = 0; i < len; i++)
        {
            if (dis[i] != fpath[length - len + i])
            {
                var = 0;
                break;
            }
        }
        if (var == 1 && S_ISREG(sb->st_mode))
        {
            printf("%s\n", fpath);
        }
    }
    return 0;
}

static int find_target(const char *fpath, const struct stat *sb, int tflag)
{
    int length = strlen(fpath), var = 1;
    if (length >= len)
    {
        for (int i = 0; i < len; i++)
        {
            if (dis[i] != fpath[length - len + i])
            {
                var = 0;
                break;
            }
        }
        if (var == 1)
        {
            printf("%s\n", fpath);
        }
    }
    return 0;
}

static int find_dir(const char *fpath, const struct stat *sb, int tflag)
{
    int length = strlen(fpath), var = 1;
    if (length >= len)
    {
        for (int i = 0; i < len; i++)
        {
            if (dis[i] != fpath[length - len + i])
            {
                var = 0;
                break;
            }
        }
        if (var == 1 && S_ISDIR(sb->st_mode))
        {
            printf("%s\n", fpath);
        }
    }
    return 0;
}

void discover(char **input, int size)
{
    int file_var = 0;
    if (size == 1 || (size == 3 && ((!strcmp(input[1], "-d") && !strcmp(input[2], "-f")) || (!strcmp(input[1], "-f") && !strcmp(input[2], "-d")))))
    {
        if (ftw(".", display_info, 20) == -1)
        {
            perror("ftw()");
            return;
        }
    }
    if (size > 1)
    {
        for (int i = 1; i < size; i++)
        {
            if (input[i][0] == '"')
            {
                len = strlen(input[i]) - 2;
                if (len > 0)
                {
                    file_var = 1;
                    strncpy(dis, &input[i][1], len);
                    break;
                }
            }
        }
    }
    if (size == 2)
    {
        if (!strcmp(input[1], ".."))
        {
            if (ftw(input[1], display_info, 20) == -1)
            {
                perror("ftw()");
                return;
            }
        }
        if (!strcmp(input[1], "~"))
        {
            if (ftw(parentdir, display_info, 20) == -1)
            {
                perror("ftw()");
                return;
            }
        }
        if (!strcmp(input[1], "-d"))
        {
            if (ftw(".", display_info_d, 20) == -1)
            {
                perror("ftw()");
                return;
            }
        }
        if (!strcmp(input[1], "-f"))
        {
            if (ftw(".", display_info_f, 20) == -1)
            {
                perror("ftw()");
                return;
            }
        }
        if (input[1][0] != '-')
        {
            if (input[1][0] != '"')
            {
                if (ftw(input[1], display_info, 20) == -1)
                {
                    perror("ftw()");
                    return;
                }
            }
            else
            {
                if (file_var == 1)
                {
                    if (ftw(".", find_target, 20) == -1)
                    {
                        perror("ftw()");
                        return;
                    }
                }
            }
        }
    }
    if (size == 3)
    {
        if (!(strcmp(input[1], "~")))
        {
            strcpy(input[1], parentdir);
        }
        if (!strcmp(input[1], "-f") && input[2][0] == '"')
        {
            if (file_var == 1)
            {
                if (ftw(".", find_file, 20) == -1)
                {
                    perror("ftw()");
                    return;
                }
            }
        }
        if (!strcmp(input[1], "-d") && input[2][0] == '"')
        {
            if (file_var == 1)
            {
                if (ftw(".", find_dir, 20) == -1)
                {
                    perror("ftw()");
                    return;
                }
            }
        }
        if ((input[1][0] != '-' && input[1][0] != '"') && input[2][0] == '"')
        {
            if (file_var == 1)
            {
                if (ftw(input[1], find_target, 20) == -1)
                {
                    perror("ftw()");
                    return;
                }
            }
        }
        if ((input[1][0] != '-' && input[1][0] != '"') && !strcmp(input[2], "-d"))
        {
            if (ftw(input[1], display_info_d, 20) == -1)
            {
                perror("ftw()");
                return;
            }
        }
        if ((input[1][0] != '-' && input[1][0] != '"') && !strcmp(input[2], "-f"))
        {
            if (ftw(input[1], display_info_f, 20) == -1)
            {
                perror("ftw()");
                return;
            }
        }
    }
    if (size == 4)
    {
        if (!(strcmp(input[1], "~")))
        {
            strcpy(input[1], parentdir);
        }
        if ((!strcmp(input[2], "-d") && !strcmp(input[3], "-f")) ||
            (!strcmp(input[2], "-f") && !strcmp(input[3], "-d")))
        {
            if (ftw(input[1], display_info, 20) == -1)
            {
                perror("ftw()");
                return;
            }
        }
        if (!strcmp(input[2], "-d"))
        {
            if (file_var == 1)
            {
                if (ftw(input[1], find_dir, 20) == -1)
                {
                    perror("ftw()");
                    return;
                }
            }
        }
        if (!strcmp(input[2], "-f"))
        {
            if (file_var == 1)
            {
                if (ftw(input[1], find_file, 20) == -1)
                {
                    perror("ftw()");
                    return;
                }
            }
        }
        return;
    }
    if (size == 5)
    {
        if (file_var == 1)
        {
            if (!strcmp(input[1], "~"))
            {
                if (ftw(parentdir, find_target, 20) == -1)
                {
                    perror("ftw()");
                    return;
                }
            }
            else
            {
                if (ftw(input[1], find_target, 20) == -1)
                {
                    perror("ftw()");
                    return;
                }
            }
        }
        return;
    }
    return;
}