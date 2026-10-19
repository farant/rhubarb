#include "arbor_syntaxis_communia.inc"
integer principale(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* intern = internamentum_creare(p);
    ArborSyntaxisResultus res;
    res = _sonda_parsere(p, intern, "int x = ;");
    _sonda_errores(res);
    redde 0;
}
