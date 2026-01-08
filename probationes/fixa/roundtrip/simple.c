/* Simplex test file - no preprocessor directives */

int global_x;
int global_y = 42;

void empty_function(void)
{
}

int add(int a, int b)
{
    return a + b;
}

void control_flow(int x)
{
    if (x > 0)
    {
        x = x - 1;
    }
    else
    {
        x = 0;
    }
}

void loops(int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        n = n + 1;
    }

    while (n > 0)
    {
        n = n - 1;
    }
}
