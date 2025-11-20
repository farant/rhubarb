#include "internamentum.h"
#include <string.h>

/* ==================================================
 * Creatio
 * ================================================== */

InternamentumChorda*
internamentum_creare(
    Piscina* piscina)
{
    InternamentumChorda* intern;

    si (!piscina)
    {
        redde NIHIL;
    }

    intern = (InternamentumChorda*)piscina_allocare(
        piscina,
        magnitudo(InternamentumChorda));

    si (!intern)
    {
        redde NIHIL;
    }

    /* Creare tabulam dispersam pro chordas */
    intern->tabula = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    si (!intern->tabula)
    {
        redde NIHIL;
    }

    intern->piscina = piscina;

    redde intern;
}


/* ==================================================
 * Internatio
 * ================================================== */

chorda*
chorda_internare(
    InternamentumChorda* intern,
    chorda s)
{
    vacuum* valor_existens;
    chorda* canonica;

    si (!intern || !s.datum)
    {
        redde NIHIL;
    }

    /* Verificare si iam internata */
    si (tabula_dispersa_invenire(intern->tabula, s, &valor_existens))
    {
        /* Iam internata - redde canonicam */
        redde (chorda*)valor_existens;
    }

    /* Nova chorda - creare copiam canonicam */
    canonica = (chorda*)piscina_allocare(
        intern->piscina,
        magnitudo(chorda));

    si (!canonica)
    {
        redde NIHIL;
    }

    /* Transcribere chordam in piscinam */
    *canonica = chorda_transcribere(s, intern->piscina);
    si (!canonica->datum)
    {
        redde NIHIL;
    }

    /* Inserere in tabulam (clavis et valor sunt idem) */
    tabula_dispersa_inserere(intern->tabula, *canonica, canonica);

    redde canonica;
}


chorda*
chorda_internare_ex_literis(
    InternamentumChorda* intern,
    constans character* cstr)
{
    vacuum* valor_existens;
    chorda* canonica;

    si (!intern || !cstr)
    {
        redde NIHIL;
    }

    /* Verificare si iam internata (nullum allocationem!) */
    si (tabula_dispersa_invenire_literis(intern->tabula, cstr, &valor_existens))
    {
        redde (chorda*)valor_existens;
    }

    /* Non internata - allocare et copiare */
    canonica = (chorda*)piscina_allocare(intern->piscina, magnitudo(chorda));
    si (!canonica)
    {
        redde NIHIL;
    }

    *canonica = chorda_ex_literis(cstr, intern->piscina);
    si (!canonica->datum)
    {
        redde NIHIL;
    }

    /* Inserere in tabulam */
    tabula_dispersa_inserere(intern->tabula, *canonica, canonica);

    redde canonica;
}


/* ==================================================
 * Quaestio
 * ================================================== */

b32
chorda_est_internata(
    InternamentumChorda* intern,
    chorda s)
{
    si (!intern)
    {
        redde FALSUM;
    }

    redde tabula_dispersa_continet(intern->tabula, s);
}


/* ==================================================
 * Statisticae
 * ================================================== */

i32
internamentum_numerus(
    InternamentumChorda* intern)
{
    si (!intern || !intern->tabula)
    {
        redde ZEPHYRUM;
    }

    redde tabula_dispersa_numerus(intern->tabula);
}


vacuum
internamentum_vacare(
    InternamentumChorda* intern)
{
    si (!intern || !intern->tabula)
    {
        redde;
    }

    tabula_dispersa_vacare(intern->tabula);
}
