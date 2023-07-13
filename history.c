#include "headers.h"

void read_histfile()
{
    FILE *f = fopen(path, "r");
    if (f == NULL)
    {
        f = fopen(path, "w");
        if (f != NULL)
        {
            fclose(f);
            f = fopen(path, "r");
        }
    }

    if (f == NULL)
    {
        perror("error");
        return;
    }

    size_t len = 0;
    ssize_t read;

    while (1)
    {
        read = getline(&hist[hist_count], &len, f);
        if (read == -1)
        {
            break;
        }
        if (!strcmp(hist[hist_count], "\n"))
        {
            continue;
        }

        hist_count++;
    }

    fclose(f);
    return;
}

void update_hist_array(char *input)
{
    if (hist_count == 0)
        hist[hist_count++] = strdup(input);

    else if (hist_count > 0)
    {
        if (hist_count < HISTORY_LEN)
        {
            if (strcmp(hist[hist_count - 1], input) != 0)
            {
                hist[hist_count++] = strdup(input);
            }
        }
        else
        {
            if (strcmp(hist[HISTORY_LEN - 1], input) != 0)
            {
                for (int index = 1; index < HISTORY_LEN; index++)
                {
                    hist[index - 1] = hist[index];
                }
                hist[HISTORY_LEN- 1] = strdup(input);
            }
        }
    }
    return;
}

void write_to_histfile()
{
    FILE *f = fopen(path, "w");
    if (f != NULL)
    {
        for (int i = 0; i < hist_count; i++)
        {
            fprintf(f, "%s\n", hist[i]);
        }
        fclose(f);
    }
    return;
}

void print_history()
{
    if (hist_count > 0)
    {
        if (hist_count < 10)
        {
            for (int i = 0; i < hist_count; i++)
                printf("%s\n", hist[i]);
        }
        else
        {
            for (int i = hist_count - 10; i < hist_count; i++)
                printf("%s\n", hist[i]);
        }
    }
    else
        return;
}

void free_hist_array()
{
    for (int i = 0; i < hist_count; i++)
    {
        free(hist[i]);
    }
    return;
}