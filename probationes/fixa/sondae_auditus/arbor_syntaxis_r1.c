#include "arbor_syntaxis_communia.inc"
integer principale(integer argc, character** argv)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* intern = internamentum_creare(p);
    ArborSyntaxisResultus res;
    constans character* fons = argc > 1 ? argv[1] : "int f(int a, ...);";
    imprimere("fons: %s\n", fons); fflush(stdout);
    res = _sonda_parsere(p, intern, fons);
    _sonda_errores(res);
    redde 0;
}
