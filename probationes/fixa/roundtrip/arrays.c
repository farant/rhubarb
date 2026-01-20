/* Test file for arrays, pointers, and complex declarations */

/* Simple arrays */
int arr[10];
int arr2[5] = {1, 2, 3, 4, 5};
char name[] = "hello";

/* Multi-dimensional arrays */
int matrix[3][3];
int grid[2][3] = {{1, 2, 3}, {4, 5, 6}};

/* Pointers */
int *ptr;
char *str;
void *generic;

/* Pointer to pointer */
int **pp;
char ***ppp;

/* Array of pointers */
int *ptrs[10];
char *strings[5];

/* Multi-declarator with pointers */
int *a, *b, c;
char x, *y, **z;

/* Const and volatile */
const int constant = 42;
volatile int flag;
const char *message;

/* Complex declarators */
int (*func_ptr)(int, int);
int *(*array_of_func_ptrs[5])(void);

/* Function with array parameter */
int sum(int arr[], int n)
{
    int i;
    int total = 0;

    for (i = 0; i < n; i++)
    {
        total = total + arr[i];
    }

    return total;
}

/* Function with pointer parameter */
void swap(int *a, int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

/* Function returning pointer */
int *find_max(int *arr, int n)
{
    int i;
    int *max = arr;

    for (i = 1; i < n; i++)
    {
        if (arr[i] > *max)
        {
            max = arr + i;
        }
    }

    return max;
}

/* String manipulation */
int strlen_custom(const char *s)
{
    int len = 0;

    while (*s != '\0')
    {
        len++;
        s++;
    }

    return len;
}
