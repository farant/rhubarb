/* tonus.c - Tonus lineae unius (vide include/tonus.h)
 *
 * Transitus unus per notas partis (frusta ordine pulsus, notae ordine
 * in frusto): simultas (initium ante finem prioris) iudicium tollit;
 * ambitus, longitudines per classem, clausulae (pausa post notam,
 * fermata ad initium, ultima). Clavis ad notam ultimam finalem dat;
 * deinde nota ultima classis finalis plagalem decernit.
 */

#include "tonus.h"
#include "partitura.h"
#include <string.h>

/* numerus toni authentici per modum (CarmenClavis.modus 1..6) */
hic_manens constans s32 NUMERI[VI] = { XI, I, III, V, VII, IX };
/* tenor exspectatus a finali per numerum toni (0 nullus) */
hic_manens constans s32 TENORES[XIII] = {
    ZEPHYRUM, VII, III, VIII, V, VII, IV, VII, V, VII, III, VII, IV
};
/* gradus collectionis ubi finalis modi stat */
hic_manens constans s32 GRADUS_FINALIS[VI] = {
    ZEPHYRUM, II, IV, V, VII, IX
};
hic_manens constans character* MODI[VI] = {
    "ionius", "dorius", "phrygius", "lydius", "mixolydius", "aeolius"
};
hic_manens constans character* ROMANI[XIII] = {
    "", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X",
    "XI", "XII"
};

interior s32
_modulus (
    s32 a,
    s32 m)
{
    s32 r = a % m;

    redde r < ZEPHYRUM ? r + m : r;
}

/* fermata initium notae tegit */
interior b32
_sub_corona (
    constans Carmen* carmen,
                s64  pulsus)
{
    i32 i;

    per (i = ZEPHYRUM; i < carmen->numerus_coronarum; i++)
    {
        si (   carmen->coronae[i].pulsus <= pulsus
            && pulsus < carmen->coronae[i].finis)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior constans CarmenClavis*
_clavis (
    constans Carmen* carmen,
                s64  pulsus)
{
    constans CarmenClavis* k = &carmen->claves[ZEPHYRUM];
                      i32  i;

    per (i = ZEPHYRUM; i < carmen->numerus_clavium; i++)
    {
        si (carmen->claves[i].pulsus <= pulsus)
        {
            k = &carmen->claves[i];
        }
    }
    redde k;
}

interior vacuum
_clausulam_addere (
                  Tonus* tonus,
    constans CarmenNota* nota,
                    s64  pulsus)
{
    i32 n = tonus->numerus_clausularum;

    si (   n > ZEPHYRUM && n <= TONUS_CLAUSULAE
        && tonus->clausulae[n - I] == nota)
    {
        redde;
    }
    si (n < TONUS_CLAUSULAE)
    {
        tonus->clausulae[n]           = nota;
        tonus->pulsus_clausularum[n]  = pulsus;
    }
    tonus->numerus_clausularum++;
}

b32
tonus_partis (
    constans Carmen* carmen,
                i32  pars,
              Tonus* tonus)
{
           constans CarmenPars* p;
           constans CarmenNota* prior = NIHIL;
                           s64  finis = ZEPHYRUM;
                           s64  prius = ZEPHYRUM;
                           s64  longitudines[XII];
         constans CarmenClavis* k;
                           s32  finalis_nota = -I;
                           s32  optima;
                           i32  f;
                           i32  j;
                           s32  c;

    memset(tonus, ZEPHYRUM, magnitudo(*tonus));
    memset(longitudines, ZEPHYRUM, magnitudo(longitudines));
    tonus->tenor = -I;
    si (pars >= carmen->numerus_partium)
    {
        redde FALSUM;
    }
    p = &carmen->partes[pars];
    per (f = ZEPHYRUM; f < p->numerus_frustorum; f++)
    {
        constans CarmenFrustum* fr = &p->frusta[f];

        per (j = ZEPHYRUM; j < fr->numerus; j++)
        {
            constans CarmenNota* n   = &fr->notae[j];
                            s64  ab  = fr->pulsus + n->pulsus;

            si (n->praeparatum.mensura > ZEPHYRUM)
            {
                perge;
            }
            si (prior != NIHIL && ab < finis)
            {
                redde FALSUM;
            }
            si (prior != NIHIL && ab > finis)
            {
                _clausulam_addere(tonus, prior, prius);
            }
            si (_sub_corona(carmen, ab))
            {
                _clausulam_addere(tonus, n, ab);
            }
            si (tonus->ima == NIHIL || n->nota < tonus->ima->nota)
            {
                tonus->ima = n;
            }
            si (tonus->summa == NIHIL || n->nota > tonus->summa->nota)
            {
                tonus->summa = n;
            }
            longitudines[n->nota % XII]  += n->longitudo;
            prior                        = n;
            prius                        = ab;
            finis                        = ab + n->longitudo;
            tonus->frustum_ultimae       = (s32)f;
        }
    }
    si (prior == NIHIL)
    {
        redde FALSUM;
    }
    _clausulam_addere(tonus, prior, prius);
    tonus->ultima          = prior;
    tonus->pulsus_ultimae  = prius;
    k                      = _clavis(carmen, prius);
    si (k->modus <= ZEPHYRUM || k->modus > VI)
    {
        redde FALSUM;
    }
    tonus->modus      = k->modus;
    tonus->finalis    = _modulus(VII * k->quintae
                                 + GRADUS_FINALIS[k->modus - I], XII);
    tonus->in_finali  = (s32)(prior->nota % XII) == tonus->finalis;
    /* finalis in linea: nota ultima classis eius; nulla: proxima infra
     * imam (aut ipsa) - authenticus usitatus */
    per (f = ZEPHYRUM; f < p->numerus_frustorum; f++)
    {
        per (j = ZEPHYRUM; j < p->frusta[f].numerus; j++)
        {
            constans CarmenNota* n = &p->frusta[f].notae[j];

            si (   n->praeparatum.mensura == ZEPHYRUM
                && (s32)(n->nota % XII)   == tonus->finalis)
            {
                finalis_nota = (s32)n->nota;
            }
        }
    }
    si (finalis_nota < ZEPHYRUM)
    {
        finalis_nota = (s32)tonus->ima->nota
            - _modulus((s32)(tonus->ima->nota % XII) - tonus->finalis,
                       XII);
    }
    tonus->plagalis  = (s32)tonus->ima->nota <= finalis_nota - V;
    tonus->numerus   = NUMERI[k->modus - I] + (tonus->plagalis ? I : 0);
    tonus->tenor_exspectatus = _modulus(tonus->finalis
        + TENORES[tonus->numerus], XII);
    /* tenor: longitudo maxima praeter finalem; aequales: exspectatus,
     * deinde classis minima */
    optima = -I;
    per (c = ZEPHYRUM; c < XII; c++)
    {
        si (c == tonus->finalis || longitudines[c] == ZEPHYRUM)
        {
            perge;
        }
        si (   optima < ZEPHYRUM
            || longitudines[c] > longitudines[optima]
            || (   longitudines[c] == longitudines[optima]
                && c == tonus->tenor_exspectatus))
        {
            optima = c;
        }
    }
    tonus->tenor = optima;
    redde VERUM;
}

/* littera classis (orthographia clavis): "D", "Bb", "F#" */
interior b32
_classem_scribere (
       constans Carmen* carmen,
                   s32  classis,
                   s64  pulsus,
     ChordaAedificator* textus)
{
       CarmenNota n;
    PartituraNota o;
              b32 bene;
              s32 a;

    memset(&n, ZEPHYRUM, magnitudo(n));
    n.nota  = (i32)(LX + classis);
    o       = partitura_orthographia(carmen, &n, pulsus);
    bene    = chorda_aedificator_appendere_character(textus, o.littera);
    per (a = o.alteratio; a > ZEPHYRUM; a--)
    {
        bene = chorda_aedificator_appendere_character(textus, '#')
            && bene;
    }
    per (a = o.alteratio; a < ZEPHYRUM; a++)
    {
        bene = chorda_aedificator_appendere_character(textus, 'b')
            && bene;
    }
    redde bene;
}

b32
tonus_scribere (
       constans Carmen* carmen,
        constans Tonus* tonus,
     ChordaAedificator* textus)
{
    s64 t = tonus->pulsus_ultimae;
    b32 bene;
    i32 i;
    i32 n;

    si (tonus->numerus < I || tonus->numerus > XII)
    {
        redde FALSUM;
    }
    bene = chorda_aedificator_appendere_literis(textus,
        ROMANI[tonus->numerus]);
    bene = chorda_aedificator_appendere_literis(textus, " (") && bene;
    bene = chorda_aedificator_appendere_literis(textus,
        MODI[tonus->modus - I]) && bene;
    bene = chorda_aedificator_appendere_literis(textus,
        tonus->plagalis ? " plagalis): finalis "
                        : " authenticus): finalis ") && bene;
    bene = _classem_scribere(carmen, tonus->finalis, t, textus) && bene;
    bene = chorda_aedificator_appendere_literis(textus, ", ambitus ")
        && bene;
    bene = partitura_notam_scribere(carmen, tonus->ima, t, textus)
        && bene;
    bene = chorda_aedificator_appendere_character(textus, '-') && bene;
    bene = partitura_notam_scribere(carmen, tonus->summa, t, textus)
        && bene;
    bene = chorda_aedificator_appendere_literis(textus, ", tenor ")
        && bene;
    si (tonus->tenor < ZEPHYRUM)
    {
        bene = chorda_aedificator_appendere_literis(textus, "nullus")
            && bene;
    }
    alioquin
    {
        bene = _classem_scribere(carmen, tonus->tenor, t, textus)
            && bene;
        si (tonus->tenor != tonus->tenor_exspectatus)
        {
            bene = chorda_aedificator_appendere_literis(textus,
                " (exspectatus ") && bene;
            bene = _classem_scribere(carmen, tonus->tenor_exspectatus,
                t,
                                     textus) && bene;
            bene = chorda_aedificator_appendere_character(textus, ')')
                && bene;
        }
    }
    bene = chorda_aedificator_appendere_literis(textus, ", clausulae")
        && bene;
    n = tonus->numerus_clausularum < TONUS_CLAUSULAE
        ? tonus->numerus_clausularum : TONUS_CLAUSULAE;
    per (i = ZEPHYRUM; i < n; i++)
    {
        bene = chorda_aedificator_appendere_character(textus, ' ')
            && bene;
        bene = _classem_scribere(carmen,
            (s32)(tonus->clausulae[i]->nota % XII),
            tonus->pulsus_clausularum[i], textus) && bene;
    }
    redde bene;
}
