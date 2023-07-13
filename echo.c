
void echo(char **input, int size)
{
    for (int i = 1; i < size; i++)
    {
        if (i < size - 1)
            printf("%s ", input[i]);
        else
        {
            printf("%s", input[i]);
            printf("\n");
        }
    }
    return;
}