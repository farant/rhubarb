#include "arbor_syntaxis_communia.inc"
interior vacuum
_lit(Piscina* p, InternamentumChorda* intern, constans character* fons, integer expectatum)
{
    ArborSyntaxisResultus res = _sonda_parsere(p, intern, fons);
    ArborNodus* decl; ArborNodus* init; ArborNodus* val;
    si (!res.successus || res.radix == NIHIL) { imprimere("%s : parse fallit\n", fons); redde; }
    decl = *(ArborNodus**)xar_obtinere(res.radix->datum.genericum.liberi, ZEPHYRUM);
    init = *(ArborNodus**)xar_obtinere(decl->datum.declaratio.declaratores, ZEPHYRUM);
    val = init->datum.init_decl.initializer;
    imprimere("%-22s genus=%s valor=%d expectatum=%d %s\n", fons, arbor_nodus_genus_nomen(val->genus),
        (integer)val->datum.character_lit.valor, expectatum,
        ((integer)val->datum.character_lit.valor == expectatum) ? "BONUM" : "MALUM");
}
integer principale(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* intern = internamentum_creare(p);
    _lit(p, intern, "char c = 'a';", 'a');
    _lit(p, intern, "char c = '\\n';", '\n');
    _lit(p, intern, "char c = '\\x41';", 0x41);
    _lit(p, intern, "char c = '\\a';", 7);
    _lit(p, intern, "char c = '\\012';", 10);
    _lit(p, intern, "char c = '\\101';", 65);
    _lit(p, intern, "char c = '\\v';", 11);
    redde 0;
}
