/* probatio_sancti_corpus.c - CORPUS VERUM sanctorum (sancti/; project-
 * specs/sancti-plan.md): plagulae omnes sine vitio (canon + regulae
 * lectoris), iunctura sanctoralis CONSONA (titulus quisque in
 * lib/sanctorale.c die suo; sanctus iunctus festum generale aut usa
 * eodem die habet) et PLENA (celebratio quaeque sanctoralis iuncta -
 * S1 implet; ante S1 fracta exspectata). Excerpta in fontibus: tools/
 * sancti_excerpta.py (speculum extra repositorium - non hic). */
#include "latina.h"
#include "credo.h"
#include "sancti.h"
#include "calendarium_liturgicum.h"
#include <stdio.h>

#define CORPUS "sancti"

interior SanctiCorpus*
_corpus (
    CredoContextus* c)
{
    redde sancti_corpus_aperire(CORPUS, c->piscina,
        internamentum_creare(c->piscina));
}

interior vacuum
_sectio_sana (
    CredoContextus* c)
{
     SanctiCorpus* corpus;
              i32  i;

    corpus = _corpus(c);
    CREDO_NECESSE_NON_NIHIL(corpus);
    per (i = ZEPHYRUM; i < xar_numerus(sancti_vitia(corpus)); i++)
    {
        constans SanctiVitium* v = (constans SanctiVitium*)xar_obtinere(
            sancti_vitia(corpus), i);

        imprimere("  %.*s:%u: %.*s\n", (integer)v->via.mensura,
            (constans character*)v->via.datum,
            (insignatus integer)v->linea,
            (integer)v->nuntius.mensura,
            (constans character*)v->nuntius.datum);
    }
    CREDO_AEQUALIS_I32(xar_numerus(sancti_vitia(corpus)), ZEPHYRUM);
    CREDO_VERUM(sancti_numerus(corpus) >= I);
}

/* sanctus festum (generale aut usa) die dato habet? */
interior b32
_festum_die (
    constans Sanctus* s,
                 s32  mensis,
                 s32  dies)
{
    i32 j;

    per (j = ZEPHYRUM; j < xar_numerus(s->festa); j++)
    {
        constans SanctiFestum* f = (constans SanctiFestum*)xar_obtinere(
            s->festa, j);

        si (   (   f->calendarium == SANCTI_CALENDARIUM_GENERALE
                || f->calendarium == SANCTI_CALENDARIUM_USA)
            && f->mensis == mensis && f->dies == dies)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* CONSONA: celebratio quaeque sanctoralis iuncta sanctos fert quorum
 * festum (generale/usa) eodem die est */
interior vacuum
_sectio_consona (
    CredoContextus* c)
{
                SanctiCorpus* corpus;
                         s32  mensis;
                         s32  dies;
                         s32  n;
                         s32  k;
                         i32  j;
                         i32  dissona;
    constans SanctoraleDatum* d;

    corpus = _corpus(c);
    CREDO_NECESSE_NON_NIHIL(corpus);
    dissona = ZEPHYRUM;
    per (mensis = I; mensis <= XII; mensis++)
    {
        per (dies = I; dies <= XXXI; dies++)
        {
            d = sanctorale_obtinere(mensis, dies, &n);
            per (k = ZEPHYRUM; d && k < n; k++)
            {
                Xar* s = sancti_celebrationis(corpus, mensis, dies,
                    chorda_ex_literis(d[k].titulus, c->piscina),
                    c->piscina);

                per (j = ZEPHYRUM; s && j < xar_numerus(s); j++)
                {
                    constans Sanctus* x = *(constans Sanctus**)
                        xar_obtinere(s, j);

                    si (!_festum_die(x, mensis, dies))
                    {
                        imprimere("  %d/%d %s: %.*s sine festo\n",
                            (integer)mensis, (integer)dies,
                            d[k].titulus,
                            (integer)x->id.mensura,
                            (constans character*)x->id.datum);
                        dissona++;
                    }
                }
            }
        }
    }
    CREDO_AEQUALIS_I32(dissona, ZEPHYRUM);
}

/* PLENA: celebratio quaeque sanctoralis in sanctorale.tsv (S1) */
interior vacuum
_sectio_plena (
    CredoContextus* c)
{
                SanctiCorpus* corpus;
                         s32  mensis;
                         s32  dies;
                         s32  n;
                         s32  k;
                         i32  absentes;
    constans SanctoraleDatum* d;

    corpus = _corpus(c);
    CREDO_NECESSE_NON_NIHIL(corpus);
    absentes = ZEPHYRUM;
    per (mensis = I; mensis <= XII; mensis++)
    {
        per (dies = I; dies <= XXXI; dies++)
        {
            d = sanctorale_obtinere(mensis, dies, &n);
            per (k = ZEPHYRUM; d && k < n; k++)
            {
                si (sancti_celebrationis(corpus, mensis, dies,
                        chorda_ex_literis(d[k].titulus, c->piscina),
                        c->piscina) == NIHIL)
                {
                    absentes++;
                }
            }
        }
    }
    imprimere("  celebrationes sine iunctura: %u\n",
        (insignatus integer)absentes);
    CREDO_AEQUALIS_I32(absentes, ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "sana", _sectio_sana, NIHIL, NIHIL, NIHIL, NIHIL },
    { "consona", _sectio_consona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "plena", _sectio_plena, NIHIL, NIHIL, NIHIL,
      "sancti-plan S1: iunctura sanctoralis nondum plena" },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("sancti_corpus", SECTIONES);
}
