/* sonda vim r1: o, O, dG undo? header vim.h:88 dicit retexibiles */
#include "latina.h"
#include "piscina.h"
#include "tabula_characterum.h"
#include "vim.h"
#include <stdio.h>

interior vacuum
_linea (TabulaCharacterum* t, i32 l)
{
    i32 c;
    imprimere("  [%u] '", (unsigned)l);
    per (c = ZEPHYRUM; c < t->latitudo; c++)
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
    VimClipboard clip;
    VimUndoAcervus* undo;
    VimStatus s;

    piscina = piscina_generare_dynamicum("sonda", 65536);

    /* casus o */
    tabula_ex_literis(&tabula, piscina, "alpha\nbeta");
    vim_clipboard_initiare(&clip);
    undo = vim_undo_creare(piscina);
    s = vim_initiare_cum_contextu(&tabula, &clip, undo);
    s = vim_tractare_clavem(s, 'o');
    s = vim_tractare_clavem(s, 'X');
    s = vim_tractare_clavem(s, VIM_CLAVIS_ESCAPE);
    imprimere("post o: undo vacuus=%d\n", (int)vim_undo_est_vacuus(undo));
    s = vim_tractare_clavem(s, 'u');
    imprimere("post u (o): mutatus=%d\n", (int)s.mutatus);
    _linea(&tabula, 0); _linea(&tabula, 1); _linea(&tabula, 2);

    /* casus O */
    tabula_ex_literis(&tabula, piscina, "alpha\nbeta");
    undo = vim_undo_creare(piscina);
    s = vim_initiare_cum_contextu(&tabula, &clip, undo);
    s = vim_tractare_clavem(s, 'O');
    s = vim_tractare_clavem(s, VIM_CLAVIS_ESCAPE);
    imprimere("post O: undo vacuus=%d\n", (int)vim_undo_est_vacuus(undo));

    /* casus dG */
    tabula_ex_literis(&tabula, piscina, "alpha\nbeta\ngamma");
    undo = vim_undo_creare(piscina);
    s = vim_initiare_cum_contextu(&tabula, &clip, undo);
    s = vim_tractare_clavem(s, 'j');
    s = vim_tractare_clavem(s, 'd');
    s = vim_tractare_clavem(s, 'G');
    imprimere("post dG: undo vacuus=%d mutatus=%d\n", (int)vim_undo_est_vacuus(undo), (int)s.mutatus);
    s = vim_tractare_clavem(s, 'u');
    imprimere("post u (dG): mutatus=%d\n", (int)s.mutatus);
    _linea(&tabula, 0); _linea(&tabula, 1); _linea(&tabula, 2);

    /* contra: dd undo operatur */
    tabula_ex_literis(&tabula, piscina, "alpha\nbeta");
    undo = vim_undo_creare(piscina);
    s = vim_initiare_cum_contextu(&tabula, &clip, undo);
    s = vim_tractare_clavem(s, 'd');
    s = vim_tractare_clavem(s, 'd');
    imprimere("post dd: undo vacuus=%d\n", (int)vim_undo_est_vacuus(undo));

    piscina_destruere(piscina);
    redde 0;
}
