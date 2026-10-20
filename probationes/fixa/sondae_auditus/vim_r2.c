/* sonda vim r2: linea plena (68 = latitudo) per yy/p et dd/u */
#include "latina.h"
#include "piscina.h"
#include "tabula_characterum.h"
#include "vim.h"
#include <stdio.h>

integer
principale (vacuum)
{
    Piscina* piscina;
    TabulaCharacterum tabula;
    VimClipboard clip;
    VimUndoAcervus* undo;
    VimStatus s;
    character textus[LXX];
    i32 c;

    piscina = piscina_generare_dynamicum("sonda", 65536);
    per (c = ZEPHYRUM; c < LXVIII; c++) textus[c] = (character)('a' + (c % XXVI));
    textus[LXVII] = 'Z';
    textus[LXVIII] = '\0';

    tabula_ex_literis(&tabula, piscina, textus);
    imprimere("latitudo=%u finis=%d cell[67]='%c'\n", (unsigned)tabula.latitudo,
        (int)tabula_invenire_finem_contenti(&tabula, 0), tabula_cellula(&tabula, 0, 67));
    vim_clipboard_initiare(&clip);
    undo = vim_undo_creare(piscina);
    s = vim_initiare_cum_contextu(&tabula, &clip, undo);
    s = vim_tractare_clavem(s, 'y');
    s = vim_tractare_clavem(s, 'y');
    imprimere("clip[66]='%c' clip[67]=%d\n", clip.lineae[0][66], (int)clip.lineae[0][67]);
    s = vim_tractare_clavem(s, 'p');
    imprimere("post p: linea1 finis=%d cell[67]=%d ('%c')\n",
        (int)tabula_invenire_finem_contenti(&tabula, 1),
        (int)tabula_cellula(&tabula, 1, 67), tabula_cellula(&tabula, 1, 67));

    /* dd + u */
    tabula_ex_literis(&tabula, piscina, textus);
    undo = vim_undo_creare(piscina);
    s = vim_initiare_cum_contextu(&tabula, &clip, undo);
    s = vim_tractare_clavem(s, 'd');
    s = vim_tractare_clavem(s, 'd');
    s = vim_tractare_clavem(s, 'u');
    imprimere("post dd u: linea0 finis=%d cell[67]=%d ('%c')\n",
        (int)tabula_invenire_finem_contenti(&tabula, 0),
        (int)tabula_cellula(&tabula, 0, 67), tabula_cellula(&tabula, 0, 67));

    piscina_destruere(piscina);
    redde 0;
}
