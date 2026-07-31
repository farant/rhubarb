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
    int copia = n;

    if (n > 0)
    {
        int temp = copia;
        n += temp;
    }
    else
    {
        int temp = -copia;
        n -= temp;
    }
    return n + copia;
}

int locales_summa(void);

int
locales_summa(void)
{
    return adhibere(2) + umbrae(3) + copia;
}
