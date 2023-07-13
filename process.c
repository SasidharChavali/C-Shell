#include "headers.h"

void handler(int sig)
{
    int a, b = 0;
    char str[MAX_LEN];
    pid_t pid = waitpid(-1, &a, WNOHANG);

    if (pid > 0)
    {
        for (int i = 0; i < bg_count; i++)
        {
            if (proc[i].pid == pid)
            {

                strcpy(str, proc[i].name);
                for (int j = i; j < bg_count - 1; j++)
                {
                    strcpy(proc[i].name, proc[i + 1].name);
                    proc[i].pid = proc[i + 1].pid;
                }

                bg_count--;
                b = 1;
                break;
            }
        }

        if (WEXITSTATUS(a) == 0 && WIFEXITED(a) && b)
            fprintf(stderr, "%s with pid = %d exited normally\n", str, pid);

        else if (b)
            fprintf(stderr, "%s with pid = %d exited abnormally\n", str, pid);

        if (b)
            prompt();
        fflush(stdout);
    }
    return;
}

void proc_run(char **input, int size, int bg_flag)
{
    pid_t cpid = -1;
    int status;
    pid_t pid = fork(), wpid;

    input[size] = NULL;
    char ch[MAX_LEN];
    
    if (pid < 0)
    {
        perror("Error");
        return;
    }

    else if (pid == 0)
    {
        setpgid(0, 0);
        if (execvp(input[0], input) < 0)
        {
            perror("Command not found");
            return;
        }
    }
    else
    {
        if (bg_flag == 0)
        {
            cpid = pid;
            strcpy(ch, input[0]);

            for (int i = 1; i < size - 1; i++)
            {
                strcat(ch, " ");
                strcat(ch, input[i]);
            }

            signal(SIGTTIN, SIG_IGN);

            signal(SIGTTOU, SIG_IGN);
            tcsetpgrp(STDIN_FILENO, pid);
            wpid = waitpid(pid, &status, WUNTRACED);
            tcsetpgrp(STDIN_FILENO, getpgrp());
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

            if (WIFSTOPPED(status))
            {
                printf("%s with PID %d suspended\n", ch, pid);
                strcpy(proc[bg_count].name, ch);
                proc[bg_count].pid = cpid;
                bg_count++;
            }
        }

        else
        {
            strcpy(proc[bg_count].name, input[0]);

            for (int i = 1; i < size - 1; i++)
            {
                strcat(proc[bg_count].name, " ");
                strcat(proc[bg_count].name, input[i]);
            }

            proc[bg_count].pid = pid;
            bg_count++;
            printf("[%d] %d\n", bg_count, pid);
        }
    }

    return;
}
