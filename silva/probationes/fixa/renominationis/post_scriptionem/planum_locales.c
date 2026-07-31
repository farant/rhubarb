/* fixura renominationis localium: umbrae, homonyma, captura */

static int copia = 3;

static int adhibere(int datum);
static int umbrae(int n);

static int
adhibere(int datum)
{
    int cursor;
    int summa = 0;

    for (cursor = 0; cursor < datum; cursor++)
    {
        summa += cursor + copia;
    }
    return summa;
}

static int
umbrae(int n)
{
    int acervus = n;

    if (n > 0)
    {
        int temp = acervus;
        n += temp;
    }
    else
    {
        int temp = -acervus;
        n -= temp;
    }
    return n + acervus;
}

int locales_summa(void);

int
locales_summa(void)
{
    return adhibere(2) + umbrae(3) + copia;
}

static int divergens(int primus);

static int
divergens(int alius)
{
    return alius + 1;
}

int divergens_usus(void);

int
divergens_usus(void)
{
    return divergens(4);
}
