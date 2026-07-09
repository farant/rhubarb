struct Par { int x; int y; };
struct Par facere(int x, int y);
int externa(int a, double b);
int scribens(int n, ...);

int voca(int n)
{
    return externa(n + 1, 2.5) + scribens(2, n, 7L);
}

struct Par duplicare(struct Par p)
{
    struct Par f;
    f.x = p.x * 2;
    f.y = p.y * 2;
    return f;
}

int summa_seriei(int* v, int n)
{
    int acies[4];
    int s = 0;
    int i;
    acies[0] = v[0];
    acies[n] = v[n] + 1;
    for (i = 0; i < n; i++) {
        s += v[i];
    }
    return s + acies[0];
}

struct Par copiat(struct Par* a)
{
    struct Par b;
    b = *a;
    b.x++;
    return b;
}

int vocat_indirecte(int (*f)(int), int x)
{
    return f(x) + (*f)(x);
}

struct Par rotunda(int n)
{
    struct Par p = duplicare(facere(n, n + 1));
    return p;
}
