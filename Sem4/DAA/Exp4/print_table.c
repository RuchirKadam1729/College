void print_table(int rows, int cols, int table[rows][cols], int cellw)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {

            if (table[i][j] == -1)
            {
                printf("%*s%s", cellw - 1, " ", "_");
            }
            else
            {
                char buffer[10];
                int width = sprintf(buffer, "%d", table[i][j]);
                printf("%*s%s", cellw - width, " ", buffer);
            }
        }
        printf("\n");
    }
}