#include "arbor_syntaxis_communia.inc"
interior vacuum
_decl(Piscina* p, InternamentumChorda* intern, constans character* fons)
{
    ArborSyntaxisResultus res = _sonda_parsere(p, intern, fons);
    i32 i, n;
    imprimere("%s\n", fons);
    _sonda_errores(res);
    si (res.radix == NIHIL) redde;
    n = xar_numerus(res.radix->datum.genericum.liberi);
    per (i = ZEPHYRUM; i < n; i++)
    {
        ArborNodus* d = *(ArborNodus**)xar_obtinere(res.radix->datum.genericum.liberi, i);
        si (d->genus == ARBOR_NODUS_DECLARATION)
            imprimere("  top %u: declaration, declaratores=%u\n", (insignatus integer)i,
                (insignatus integer)(d->datum.declaratio.declaratores ? xar_numerus(d->datum.declaratio.declaratores) : 0));
        alioquin si (d->genus == ARBOR_NODUS_FUNCTION_DEFINITION)
        {
            ArborNodus* c = d->datum.functio.corpus;
            i32 j, m = c->datum.compositum.sententiae ? xar_numerus(c->datum.compositum.sententiae) : 0;
            imprimere("  top %u: function-definition\n", (insignatus integer)i);
            per (j = ZEPHYRUM; j < m; j++)
            {
                ArborNodus* s = *(ArborNodus**)xar_obtinere(c->datum.compositum.sententiae, j);
                si (s->genus == ARBOR_NODUS_DECLARATION)
                    imprimere("    stmt %u: declaration, declaratores=%u\n", (insignatus integer)j,
                        (insignatus integer)(s->datum.declaratio.declaratores ? xar_numerus(s->datum.declaratio.declaratores) : 0));
                alioquin imprimere("    stmt %u: %s\n", (insignatus integer)j, arbor_nodus_genus_nomen(s->genus));
            }
        }
        alioquin imprimere("  top %u: %s\n", (insignatus integer)i, arbor_nodus_genus_nomen(d->genus));
    }
}
integer principale(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* intern = internamentum_creare(p);
    _decl(p, intern, "typedef int T; void f(void) { int T; T = 1; }");
    _decl(p, intern, "typedef int T; void f(void) { int x; x = 1; }");
    redde 0;
}
