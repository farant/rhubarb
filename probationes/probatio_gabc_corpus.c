/* probatio_gabc_corpus.c - specimen GregoBase (plan gabc T9)
 *
 * LXXXII cantus GregoBase (CC0), fontes omnes <= MCMXXX, in
 * probationes/fixa/gabc/gregobase/ cum manifesto
 * (probationes/fixa/gabc/gregobase.manifestum; electio et provenientia:
 * tools/gabc_arcessere.sh). Quisque legitur sine VITIO, figurae
 * nominantur, convertitur, et carmina_legere carmen accipit. Numeri
 * (monita per genus, figurae ignotae, notae, approximationes per genus)
 * FIXI sunt: mutatio lectoris aut conversoris eos movens hic videtur
 * (cauda longa, G5). Numeri nunc scripti mensurati sunt 2026-11-04.
 */

#include "latina.h"
#include "credo.h"
#include "gabc.h"
#include "gabc_carmen.h"
#include "carmen.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

#define VIA_MANIFESTI  "probationes/fixa/gabc/gregobase.manifestum"
#define VIA_FIXORUM    "probationes/fixa/gabc/gregobase/"
#define CANTUS_MAXIMI  CXXVIII

nomen structura {
    character plagulae[CANTUS_MAXIMI][XXIV];
          i32 numerus;
} Manifestum;

/* columna prima linearum (praeter 'DATA ...') */
interior b32
_manifestum_legere (
    CredoContextus* c,
        Manifestum* m)
{
    chorda d = filum_legere_totum(VIA_MANIFESTI, c->piscina);
       i32 i = ZEPHYRUM;

    m->numerus = ZEPHYRUM;
    si (d.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    dum (i < d.mensura)
    {
        i32 initium = i;
        i32 k;

        dum (i < d.mensura && d.datum[i] != '\n')
        {
            i++;
        }
        si (   i - initium > IV
            && memcmp(d.datum + initium, "DATA", IV) != ZEPHYRUM)
        {
            per (k = initium; k < i && d.datum[k] != '\t'; k++)
            {
            }
            si (k - initium >= XXIV || m->numerus >= CANTUS_MAXIMI)
            {
                redde FALSUM;
            }
            memcpy(m->plagulae[m->numerus], d.datum + initium,
                (size_t)(k - initium));
            m->plagulae[m->numerus][k - initium] = '\0';
            m->numerus++;
        }
        i++;
    }
    redde VERUM;
}

interior chorda
_cantum_legere (
               Piscina* piscina,
    constans character* plagula)
{
    character via[CXXVIII];

    strcpy(via, VIA_FIXORUM);
    strcat(via, plagula);
    redde filum_legere_totum(via, piscina);
}

interior vacuum
_manifestum_probare (
    CredoContextus* c)
{
    Manifestum m;
           i32 i;

    CREDO_NECESSE (_manifestum_legere(c, &m));
    CREDO_AEQUALIS_I32 (m.numerus, LXXXII);
    per (i = ZEPHYRUM; i < m.numerus; i++)
    {
        CREDO_VERUM (_cantum_legere(c->piscina, m.plagulae[i]).mensura
            > ZEPHYRUM);
    }
}

/* monita per genus (index GabcVitiumGenus) */
hic_manens constans s32 MONITA[] = {
    /* SINE_CAPITE */ 0, /* SEPARATOR_ITERATUS */ 1, /* CAPUT_MALUM */ 0,
    /* PARENTHESIS_APERTA */ 0, /* NOTA_IGNOTA */ 0,
    /* SIGNUM_APERTUM */ 0, /* SIGNUM_IGNOTUM */ 0,
    /* ALTERATIO_SINE_POSITIONE */ 0, /* CLAVIS_MALA */ 0,
    /* POSITIO_EXTRA */ 0, /* NON_UTF8 */ 0
};

#define GENERA_VITIORUM \
    ((i32)(magnitudo(MONITA) / magnitudo(MONITA[ZEPHYRUM])))

interior vacuum
_lectionem_probare (
    CredoContextus* c)
{
    Manifestum m;
           s32 monita[XVI];
           i32 vitia = ZEPHYRUM;
           i32 i;
           i32 k;

    memset(monita, ZEPHYRUM, magnitudo(monita));
    CREDO_NECESSE (_manifestum_legere(c, &m));
    per (i = ZEPHYRUM; i < m.numerus; i++)
    {
           Piscina* p = piscina_generare_dynamicum("gabc_corpus",
               MMMCMXCIX);
        GabcCantus* g;

        CREDO_NECESSE (p != NIHIL);
        g = gabc_legere(_cantum_legere(p, m.plagulae[i]), p);
        CREDO_NECESSE (g != NIHIL);
        per (k = ZEPHYRUM; k < g->numerus_vitiorum; k++)
        {
            si (g->vitia[k].gravitas == GABC_VITIUM)
            {
                imprimere("  VITIUM %s:%u:%u: %.*s\n", m.plagulae[i],
                    (insignatus integer)g->vitia[k].linea,
                    (insignatus integer)g->vitia[k].columna,
                    (integer)g->vitia[k].causa.mensura,
                    (constans character*)g->vitia[k].causa.datum);
                vitia++;
            }
            alioquin si ((i32)g->vitia[k].genus < XVI)
            {
                monita[g->vitia[k].genus]++;
            }
        }
        piscina_destruere(p);
    }
    CREDO_AEQUALIS_I32 (vitia, ZEPHYRUM);
    per (k = ZEPHYRUM; k < GENERA_VITIORUM; k++)
    {
        si (monita[k] != MONITA[k])
        {
            imprimere("  monita genere %u: %d (fixum %d)\n",
                (insignatus integer)k, (integer)monita[k],
                (integer)MONITA[k]);
        }
        CREDO_AEQUALIS_I32 ((i32)monita[k], (i32)MONITA[k]);
    }
}

#define NEUMAE_FIXAE   9368
#define IGNOTAE_FIXAE  814

interior vacuum
_figuras_probare (
    CredoContextus* c)
{
    Manifestum m;
           i32 neumae   = ZEPHYRUM;
           i32 ignotae  = ZEPHYRUM;
           i32 i;

    CREDO_NECESSE (_manifestum_legere(c, &m));
    per (i = ZEPHYRUM; i < m.numerus; i++)
    {
           Piscina* p = piscina_generare_dynamicum("gabc_corpus",
               MMMCMXCIX);
        GabcCantus* g;
               i32  s;
               i32  e;

        CREDO_NECESSE (p != NIHIL);
        g = gabc_legere(_cantum_legere(p, m.plagulae[i]), p);
        CREDO_NECESSE (g != NIHIL);
        per (s = ZEPHYRUM; s < g->numerus_syllabarum; s++)
        {
            per (e = ZEPHYRUM; e
                < g->syllabae[s].numerus_elementorum; e++)
            {
                constans GabcElementum* el =
                    &g->syllabae[s].elementa[e];

                si (el->genus != GABC_EL_NEUMA)
                {
                    perge;
                }
                neumae++;
                si (gabc_figura(g, el) == GABC_FIGURA_IGNOTA)
                {
                    ignotae++;
                }
            }
        }
        piscina_destruere(p);
    }
    imprimere("  figurae: neumae %d, ignotae %d\n", (integer)neumae,
        (integer)ignotae);
    CREDO_AEQUALIS_I32 (neumae, NEUMAE_FIXAE);
    CREDO_AEQUALIS_I32 (ignotae, IGNOTAE_FIXAE);
}

#define NOTAE_FIXAE  15389
hic_manens constans s32 APPROXIMATIONES[GABC_CARMEN_APPROXIMATIONUM_GENERA] =
    { 2, 0, 0 };

interior vacuum
_conversionem_probare (
    CredoContextus* c)
{
    Manifestum m;
           s32 approximationes[GABC_CARMEN_APPROXIMATIONUM_GENERA];
           i32 notae = ZEPHYRUM;
           i32 i;
           i32 k;

    memset(approximationes, ZEPHYRUM, magnitudo(approximationes));
    CREDO_NECESSE (_manifestum_legere(c, &m));
    per (i = ZEPHYRUM; i < m.numerus; i++)
    {
                 Piscina* p = piscina_generare_dynamicum("gabc_corpus",
                                  MMMCMXCIX);
              GabcCantus* g;
         GabcCarmenForma  f;
       GabcCarmenRelatio  r;
       ChordaAedificator* a;
       ChordaAedificator* b;
                 Carmina  liber;
          CarminisVitium  vitium;
                     b32  scriptum;
                     b32  lectum = FALSUM;

        CREDO_NECESSE (p != NIHIL);
        a = chorda_aedificator_creare(p, MMMCMXCIX);
        b = chorda_aedificator_creare(p, MMMCMXCIX);
        g = gabc_legere(_cantum_legere(p, m.plagulae[i]), p);
        CREDO_NECESSE (g != NIHIL && a != NIHIL && b != NIHIL);
        gabc_carmen_formam_ordinariam(&f);
        scriptum = gabc_carmen_scribere(g, &f, p, a, &r);
        si (scriptum)
        {
            chorda_aedificator_appendere_literis(b, "<carmina>\n");
            chorda_aedificator_appendere_chorda(b,
                chorda_aedificator_finire(a));
            chorda_aedificator_appendere_literis(b, "</carmina>\n");
            lectum = carmina_legere(chorda_aedificator_finire(b), p,
                &liber, &vitium);
        }
        si (!scriptum || !lectum)
        {
            imprimere("  %s: %s\n", m.plagulae[i], !scriptum
                ? "non conversus" : vitium.causa);
        }
        CREDO_VERUM (scriptum && lectum);
        notae += r.numerus_nexuum;
        per (k = ZEPHYRUM; k < r.numerus_approximationum; k++)
        {
            approximationes[r.approximationes[k].genus]++;
        }
        piscina_destruere(p);
    }
    imprimere("  conversio: notae carminis %d\n", (integer)notae);
    CREDO_AEQUALIS_I32 (notae, NOTAE_FIXAE);
    per (k = ZEPHYRUM; k < (i32)GABC_CARMEN_APPROXIMATIONUM_GENERA; k++)
    {
        imprimere("  approximationes %s: %d\n",
            gabc_approximationis_titulus((GabcApproximatioGenus)k),
            (integer)approximationes[k]);
        CREDO_AEQUALIS_I32 ((i32)approximationes[k],
            (i32)APPROXIMATIONES[k]);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "manifestum: LXXXII plagulae", _manifestum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "lectio: sine vitio, monita per genus fixa", _lectionem_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "figurae: neumae et ignotae fixae", _figuras_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "conversio: omnes, carmina_legere, approximationes fixae",
        _conversionem_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("gabc_corpus", SECTIONES, NIHIL,
        NIHIL);
}
