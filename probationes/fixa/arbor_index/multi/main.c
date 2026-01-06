/* main.c - Utitur functiones ex utils.c */

int multiplicare(int a, int b);
int dividere(int a, int b);

int computare(int x, int y)
{
    int prod = multiplicare(x, y);
    int quot = dividere(x, y);
    return prod + quot;
}

int main(void)
{
    int resultatum = computare(10, 2);
    return resultatum;
}
