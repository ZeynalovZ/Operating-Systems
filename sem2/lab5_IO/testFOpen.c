#include <stdio.h>
int main()
{
    FILE *f1 = fopen("res.txt", "w");
    FILE *f2 = fopen("res.txt", "w");
    for (char c = 'a'; c <= 'z'; c++)
    {
        if (c % 2 == 1)
            fprintf(f1, "%c", c);
        else
            fprintf(f2, "%c", c);
    }
    fclose(f1);
    fclose(f2);
}