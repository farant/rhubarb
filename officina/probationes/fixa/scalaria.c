int duplum(int n) { return n + n; }
int summa(int n)
{
    int s = 0;
    int i;
    for (i = 1; i <= n; i = i + 1) {
        s = s + i;
    }
    return s;
}
long mixtum(int a, long b)
{
    long c = a * 2 + b;
    c += a;
    return c ? c : -b;
}
unsigned int bitalia(unsigned int x)
{
    unsigned int y = x << 2;
    y = y | 7u;
    y ^= x >> 1;
    y &= ~x;
    return y - 1u;
}
double fluita(double x, int k)
{
    double y = x * 2.5;
    if (k > 0 && y < 100.0) {
        y = y + (double)k;
    }
    return -y;
}
int monstra(int* p)
{
    int x = 5;
    int* q = &x;
    *q = *p + 1;
    q = q + 1;
    q = q - 1;
    ++x;
    x--;
    return *q + (q == p);
}
long pecunia_exemplum(long cents, double n)
{
    long implicita = (cents * n);
    return implicita + (long)(cents * n);
}
