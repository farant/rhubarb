/* crusta_diagnostica.c - Vide crusta_diagnostica.h. */

#include "crusta_diagnostica.h"
#include "crusta_registrum.h"
#include <string.h>

Xar*
crusta_diagnostica (
                   Piscina* piscina,
     constans MateriaNodus* radix,
    constans CrustaParsura* relatio)
{
    redde materia_diagnostica_derivare(piscina, radix,
        &CRUSTA_REGISTRUM, &CRUSTA_DIAGNOSTICA, NIHIL,
        relatio != NIHIL ? relatio->diagnostica : NIHIL);
}

/* ordo tabulae per punctatorem codicis: ordo derivatus codicem
 * tabulae ipsum fert (idem punctator), ergo comparatio punctatorum
 * sufficit; codices substrati et parsatoris per strcmp separantur */
interior constans MateriaTabDiagnosticum*
_ordo_tabulae (
    constans character* codex)
{
    i32 k;

    per (k = ZEPHYRUM; k < CRUSTA_DIAGNOSTICA.numerus; k++)
    {
        si (CRUSTA_DIAGNOSTICA.tabula[k].codex == codex)
        {
            redde &CRUSTA_DIAGNOSTICA.tabula[k];
        }
    }
    redde NIHIL;
}

/* praesentia per classem (non numerus: machina arithmetica vestigium
 * unum pluries numerat - delta D7); classis prima discors servatur */
interior vacuum
_discors (
        CrustaConcordia* c,
                    b32  numerus_parsatoris,
                    b32  numerus_derivatus,
     constans character* classis)
{
    si (numerus_parsatoris != numerus_derivatus && c->causa == NIHIL)
    {
        c->causa      = classis;
        c->concordat  = FALSUM;
    }
}

b32
crusta_diagnostica_concordia (
                   Piscina* piscina,
     constans MateriaNodus* radix,
    constans CrustaParsura* relatio,
           CrustaConcordia* c)
{
    Xar* derivata;
    i32  k;

    memset(c, ZEPHYRUM, magnitudo(*c));
    c->concordat  = VERUM;
    derivata      = crusta_diagnostica(piscina, radix, relatio);
    si (derivata == NIHIL || relatio == NIHIL)
    {
        c->concordat  = FALSUM;
        c->causa      = "memoria aut relatio nihil";
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(derivata); k++)
    {
        constans MateriaDiagnosticum* r =
            (constans MateriaDiagnosticum*)xar_obtinere(derivata, k);
        constans MateriaTabDiagnosticum* t;

        si (r->gravitas == (s32)MATERIA_GRAVITAS_MONITUM)
        {
            c->monita++;
        }
        alioquin
        {
            c->errata++;
        }
        si (strcmp(r->codex, MATERIA_CODEX_ORDO) == ZEPHYRUM)
        {
            c->transpositae++;
            perge;
        }
        si (strcmp(r->codex, "ramus/probatio") == ZEPHYRUM)
        {
            c->vacuae++;
            perge;
        }
        t = _ordo_tabulae(r->codex);
        si (t == NIHIL)
        {
            c->concordat  = FALSUM;
            c->causa      = "codex ignotus";
            perge;
        }
        si (t->species == (s32)MATERIA_DIAGNOSTICUM_GENUS)
        {
            c->mala++;
        }
        alioquin si (t->species == (s32)MATERIA_DIAGNOSTICUM_VACUA)
        {
            c->vacuae++;
        }
        alioquin si (   t->genus == (s32)CRUSTA_GENUS_BINARIA
                     || t->genus == (s32)CRUSTA_GENUS_IUDICIUM_BINARIA)
        {
            /* machina arithmetica ea inter 'mala' numerat */
            c->mala++;
        }
        alioquin
        {
            c->clausurae++;
        }
    }
    _discors(c, (b32)(relatio->mala > ZEPHYRUM),
        (b32)(c->mala > ZEPHYRUM), "mala");
    _discors(c, (b32)(relatio->clausurae_absentes > ZEPHYRUM),
        (b32)(c->clausurae > ZEPHYRUM), "clausurae");
    _discors(c, (b32)(relatio->listae_vacuae > ZEPHYRUM),
        (b32)(c->vacuae > ZEPHYRUM), "vacuae");
    _discors(c, (b32)(relatio->heredoca_transposita > ZEPHYRUM),
        (b32)(c->transpositae > ZEPHYRUM), "transpositae");
    _discors(c, relatio->sana,
        (b32)(c->errata + c->monita == ZEPHYRUM), "sana");
    redde c->concordat;
}
