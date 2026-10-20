/* sonda vim r5: mutatus VERUM sine mutatione (x in linea vacua, d$ ultra finem, dG infra contentum) */
#include "latina.h"
#include "piscina.h"
#include "tabula_characterum.h"
#include "vim.h"
#include <stdio.h>
#include <string.h>

interior character imago[LXVIII * LVI];

interior vacuum
_photo (TabulaCharacterum* t)
{
    memcpy(imago, t->cellulae, (size_t)(t->latitudo * t->altitudo));
}

interior integer
_mutata (TabulaCharacterum* t)
{
    redde memcmp(imago, t->cellulae, (size_t)(t->latitudo * t->altitudo)) != 0;
}

integer
principale (vacuum)
{
    Piscina* piscina;
    TabulaCharacterum tabula;
    VimStatus s;

    piscina = piscina_generare_dynamicum("sonda", 65536);

    /* x in linea vacua (linea 1) */
    tabula_ex_literis(&tabula, piscina, "alpha\n\ngamma");
    s = vim_initiare(&tabula);
    s = vim_ponere_cursor(s, 1, 0);
    _photo(&tabula);
    s = vim_tractare_clavem(s, 'x');
    imprimere("x linea vacua: mutatus=%d tabula_mutata=%d\n", (int)s.mutatus, _mutata(&tabula));

    /* Delete in linea vacua */
    _photo(&tabula);
    s = vim_tractare_clavem(s, VIM_CLAVIS_DELETE);
    imprimere("Delete linea vacua: mutatus=%d tabula_mutata=%d\n", (int)s.mutatus, _mutata(&tabula));

    /* contra: x ultra finem lineae non vacuae */
    s = vim_ponere_cursor(s, 0, 10);
    _photo(&tabula);
    s = vim_tractare_clavem(s, 'x');
    imprimere("x ultra finem 'alpha': mutatus=%d tabula_mutata=%d\n", (int)s.mutatus, _mutata(&tabula));

    /* d$ cursor ultra finem */
    s = vim_ponere_cursor(s, 0, 10);
    _photo(&tabula);
    s = vim_tractare_clavem(s, 'd');
    s = vim_tractare_clavem(s, '$');
    imprimere("d$ ultra finem: mutatus=%d tabula_mutata=%d\n", (int)s.mutatus, _mutata(&tabula));

    /* d$ in linea vacua */
    s = vim_ponere_cursor(s, 1, 0);
    _photo(&tabula);
    s = vim_tractare_clavem(s, 'd');
    s = vim_tractare_clavem(s, '$');
    imprimere("d$ linea vacua: mutatus=%d tabula_mutata=%d\n", (int)s.mutatus, _mutata(&tabula));

    /* dG cursor infra ultimam lineam cum contentu */
    s = vim_ponere_cursor(s, 10, 0);
    _photo(&tabula);
    s = vim_tractare_clavem(s, 'd');
    s = vim_tractare_clavem(s, 'G');
    imprimere("dG infra contentum: mutatus=%d tabula_mutata=%d\n", (int)s.mutatus, _mutata(&tabula));

    /* tabula vacua: x et dG ad (0,0) */
    tabula_ex_literis(&tabula, piscina, "");
    s = vim_initiare(&tabula);
    _photo(&tabula);
    s = vim_tractare_clavem(s, 'x');
    imprimere("x tabula vacua: mutatus=%d tabula_mutata=%d\n", (int)s.mutatus, _mutata(&tabula));
    _photo(&tabula);
    s = vim_tractare_clavem(s, 'd');
    s = vim_tractare_clavem(s, 'G');
    imprimere("dG tabula vacua: mutatus=%d tabula_mutata=%d\n", (int)s.mutatus, _mutata(&tabula));

    piscina_destruere(piscina);
    redde 0;
}
