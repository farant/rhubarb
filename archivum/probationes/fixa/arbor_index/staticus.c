/* staticus.c - Fixture pro nexus internus (static) */

static int numerus_privatus = 100;

static int functio_privata(int x)
{
    return x * 2;
}

int functio_publica(int x)
{
    return functio_privata(x) + numerus_privatus;
}

extern int numerus_externus;
