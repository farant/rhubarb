/* pecunia.c - pecunia ut quantitates decimales exactae (vide
 * include/pecunia.h; spec project-specs/pecunia-spec.md) */
#include "pecunia.h"
#include "pecunia_monetae.h"
#include <string.h>

hic_manens constans character* VITIA_TITULI[] = {
    "bene",
    "forma mala",
    "plus quam IV decimales",
    "superfluxus (extra s64)",
    "moneta ignota",
    "moneta sine unitate minore",
    "monetae diversae",
    "totum non solvibile in unitates",
    "rationes malae (negativae aut summa nulla)"
};

constans character*
pecunia_vitium_titulus (
    PecuniaVitium vitium)
{
    si (   (s32)vitium < ZEPHYRUM
        || (s32)vitium > (s32)PECUNIA_VITIUM_RATIONES)
    {
        redde "vitium ignotum";
    }
    redde VITIA_TITULI[vitium];
}


/* ==================================================
 * Monetae
 * ================================================== */

/* index ordinis in tabula, aut -I (quaestio binaria; codex
 * majusculis, NUL terminatus) */
interior s32
_ordo (
    constans character* codex)
{
    s32 infra = ZEPHYRUM;
    s32 supra = (s32)PECUNIA_MONETAE_NUMERUS - I;

    dum (infra <= supra)
    {
        s32 medium  = infra + (supra - infra) / II;
        s32 c       = strcmp(codex, PECUNIA_MONETAE[medium].codex);

        si (c == ZEPHYRUM)
        {
            redde medium;
        }
        si (c < ZEPHYRUM)
        {
            supra = medium - I;
        }
        alioquin
        {
            infra = medium + I;
        }
    }
    redde -I;
}

PecuniaVitium
pecunia_moneta_ex_codice (
           chorda  codex,
    PecuniaMoneta* exitus)
{
    character c[IV];
          s32 i;

    si (exitus == NIHIL || codex.mensura != III || codex.datum == NIHIL)
    {
        redde PECUNIA_VITIUM_MONETA_IGNOTA;
    }
    per (i = ZEPHYRUM; i < III; i++)
    {
        character l = (character)codex.datum[i];

        si (l >= 'a' && l <= 'z')
        {
            l = (character)(l - 'a' + 'A');
        }
        si (l < 'A' || l > 'Z')
        {
            redde PECUNIA_VITIUM_MONETA_IGNOTA;
        }
        c[i] = l;
    }
    c[III] = '\0';
    si (_ordo(c) < ZEPHYRUM)
    {
        per (i = ZEPHYRUM; i
            < (s32)PECUNIA_MONETAE_SINE_UNITATE_NUMERUS;
             i++)
        {
            si (strcmp(c, PECUNIA_MONETAE_SINE_UNITATE[i]) == ZEPHYRUM)
            {
                redde PECUNIA_VITIUM_MONETA_SINE_UNITATE;
            }
        }
        redde PECUNIA_VITIUM_MONETA_IGNOTA;
    }
    memcpy(exitus->codex, c, IV);
    redde PECUNIA_BENE;
}

constans PecuniaMonetaInformatio*
pecunia_moneta_informatio (
    PecuniaMoneta m)
{
    s32 k;

    si (m.codex[III] != '\0')
    {
        redde NIHIL;
    }
    k = _ordo(m.codex);
    redde k < ZEPHYRUM ? NIHIL : &PECUNIA_MONETAE[k];
}

s32
pecunia_moneta_exponens (
    PecuniaMoneta m)
{
    constans PecuniaMonetaInformatio* i = pecunia_moneta_informatio(m);

    redde i ? i->exponens : -I;
}

b32
pecunia_moneta_aequalis (
    PecuniaMoneta a,
    PecuniaMoneta b)
{
    redde memcmp(a.codex, b.codex, IV) == ZEPHYRUM;
}
