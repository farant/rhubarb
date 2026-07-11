/* indicium_exemplum.c - fixtura M3 chunk 3 (lineae FIXAE:
 * probatio_officina_indicium numeros linearum figit - NOLI
 * lineas movere) */
#define GEMINARE(x) ((x) + (x))

int computare(int fons)
{
    int a;
    unsigned int b;
    char* p;

    a = fons + 1;
    b = 2u;
    p = 0;
    a = GEMINARE(a) + (int)b;
    if (p != 0)
    {
        a = 0;
    }
    return a;
}
