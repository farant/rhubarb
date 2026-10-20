/* sonda vim r3: vim_tractare_clavem (tempus 0.0) -> 'fd' semper escape */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_characterum.h"
#include "vim.h"
#include "pagina.h"
#include <stdio.h>

interior vacuum
_linea (TabulaCharacterum* t, i32 l)
{
    i32 c;
    imprimere("  [%u] '", (unsigned)l);
    per (c = ZEPHYRUM; c < XX; c++)
    {
        character ch = tabula_cellula(t, l, c);
        si (ch == '\0') frange;
        imprimere("%c", ch);
    }
    imprimere("'\n");
}

integer
principale (vacuum)
{
    Piscina* piscina;
    TabulaCharacterum tabula;
    VimStatus s;
    Pagina pagina;
    chorda id;
    constans character* p;

    piscina = piscina_generare_dynamicum("sonda", 65536);

    /* directe */
    tabula_ex_literis(&tabula, piscina, "");
    s = vim_initiare(&tabula);
    s = vim_tractare_clavem(s, 'i');
    per (p = "halfdozen"; *p; p++) s = vim_tractare_clavem(s, (s32)*p);
    imprimere("directe: modo=%s cursor=(%u,%u)\n", vim_nomen_modi(s.modo),
        (unsigned)s.cursor_linea, (unsigned)s.cursor_columna);
    _linea(&tabula, 0); _linea(&tabula, 1);

    /* cum tempore distanti: idem textus, 1 s inter claves */
    tabula_ex_literis(&tabula, piscina, "");
    s = vim_initiare(&tabula);
    s = vim_tractare_clavem_cum_tempore(s, 'i', 0.0);
    {
        f64 t = 1.0;
        per (p = "halfdozen"; *p; p++) { s = vim_tractare_clavem_cum_tempore(s, (s32)*p, t); t += 1.0; }
    }
    imprimere("cum tempore 1s: modo=%s\n", vim_nomen_modi(s.modo));
    _linea(&tabula, 0); _linea(&tabula, 1);

    /* per paginam */
    id = chorda_ex_literis("page:0", piscina);
    pagina_initiare(&pagina, piscina, &id);
    pagina_inserere_textum(&pagina, "halfdozen");
    imprimere("pagina_inserere_textum:\n");
    _linea(&pagina.tabula, 0); _linea(&pagina.tabula, 1);

    piscina_destruere(piscina);
    redde 0;
}
