#include "headers.h"

#include "cd.c"
#include "echo.c"
#include "pwd.c"
#include "ls.c"
#include "process.c"
#include "pinfo.c"
#include "discover.c"
#include "history.c"

void prompt(int bg_var, time_t bg)
{
    if (getcwd(pwdir, PATH_MAX) == NULL)
    {
        perror("error");
        return;
    }
    char *ptr = strstr(pwdir, parentdir);

    if (ptr != NULL)
    {
        if (bg_var == 0)
            printf("%s@%s:~%s ", username, hostname, ptr + strlen(parentdir));
        if (bg_var == 1)
        {
            printf("%s@%s:~%s>took %lds~ ", username, hostname, ptr + strlen(parentdir), bg);
            bg_var = 0;
        }
    }
    else
    {
        if (bg_var == 0)
            printf("%s@%s:%s ", username, hostname, pwdir);
        if (bg_var == 1)
        {
            printf("%s@%s:%s>took %lds~ ", username, hostname, pwdir, bg);
            bg_var = 0;
        }
    }
    return;
}

int main()
{
    char input[MAX_LEN];
    
    int bg_var = 0; 
    time_t bg;

    getlogin_r(username, MAX_LEN);
    gethostname(hostname, HOST_NAME_MAX);
    getcwd(parentdir, PATH_MAX);
    
    strcpy(path, "/home/");
    strcpy(&path[6], username);
    strcpy(&path[6 + strlen(username)], "/history.txt");

    signal(SIGCHLD, handler);

    read_histfile();

    while (1)
    {
        prompt(bg_var, bg);

        scanf("%[^\n]%*c", input);
    
        update_hist_array(input);
        
        write_to_histfile();

        char *tokens[MAX_LEN];
        int index = 0;
        tokens[index] = strtok(input, ";");
        while (tokens[index] != NULL)
        {
            index++;
            tokens[index] = strtok(NULL, ";");
        }
        for (int i = 0; i < index; i++)
        {
            if (!strcmp(tokens[i], "\n"))
            {
                continue;
            }
            char *strings[MAX_LEN];
            int j = 0;
            strings[j] = strtok(tokens[i], " ");
            while (strings[j] != NULL)
            {
                j++;
                strings[j] = strtok(NULL, " ");
            }
            
            if (!strcmp(strings[0], "quit"))
            {
                free_hist_array();
                exit(0);
            }

            else if (!strcmp(strings[0], "cd"))
            {
                cd(strings, j);
            }

            else if (!strcmp(strings[0], "echo"))
            {

                echo(strings, j);
            }

            else if (!strcmp(strings[0], "pwd"))
            {
                pwd();
            }

            else if (!strcmp(strings[0], "ls"))
            {
                ls(strings, j);
            }

            else if (!strcmp(strings[0], "pinfo"))
            {
                pinfo(strings, j);
            }

            else if (!strcmp(strings[0], "discover"))
            {
                discover(strings, j);
            }

            else if (!strcmp(strings[0], "history"))
            {
                print_history();
            }

            else if (!strcmp(strings[j - 1], "&"))
            {
                proc_run(strings, j, 1);
            }

            else if (strings[j - 1][strlen(strings[j - 1]) - 1] == '&')
            {
                strings[j - 1][strlen(strings[j - 1]) - 1] = '\0';
                proc_run(strings, j, 1);
            }

            else
            {
                time_t start = time(NULL);

                proc_run(strings, j, 0);

                time_t end = time(NULL);
                if (end - start > 1)
                {
                    bg_var = 1;
                    bg = end - start;
                }
            }
        }
    }
}