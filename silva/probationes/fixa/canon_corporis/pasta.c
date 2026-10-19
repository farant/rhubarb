/* pasta.c - fixtura CONGELATA porta corporis canonis (GRADUS III):
 * concatenatio lexematum (##) quam corpus vivum lib/ non iam fert.
 * Olim sola exempla in arbor/arbor2 stabant; post archivationem
 * (2026-10-19) praesentia formae hic asseritur, non in corpore vivo -
 * corpus vivum invariantia sola fert (quaestio 01M365VXAF). */

#define IUNGERE(a, b)        a ## b
#define FUNCTIO_NOMINATA(x)  functio_ ## x

static int
functio_prima (void)
{
    return 1;
}

int
functio_secunda (void)
{
    int IUNGERE(val, or) = FUNCTIO_NOMINATA(prima)();

    return valor;
}
