/* probatio_fasti_fontes.c - fontes dierum (nomenclator-plan P6):
 * annuum expandere (intervallum, anni duo, 29 II, maximum, vacuum),
 * fontes legere (ordo diei / generis / tituli, fons a portu positus,
 * NIHIL = vacuum), addere (titulus iteratus, plenus), aperire (fons
 * suus, ignotus, sine aperire) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "fasti.h"
#include "fasti_fontes.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior b32
dies_est (
    Dies d,
     s32 annus,
     s32 mensis,
     s32 dies)
{
    si (d.annus != annus || d.mensis != mensis || d.dies != dies)
    {
        imprimere("  dies: %d-%d-%d\n", (integer)d.annus,
            (integer)d.mensis, (integer)d.dies);
        redde FALSUM;
    }
    redde VERUM;
}

/* annuum: intervallum annuum, anni duo, 29 II (communis -> 28 II,
 * bissextilis manet), extra, maximum, intervallum inversum */
interior vacuum
_probare_annuum (
    CredoContextus* c)
{
    Dies exitus[IV];
     i32 n;

    (vacuum)c;
    n = fasti_annuum_expandere(III, II, fasti_dies(MMXXVI, I, I),
        fasti_dies(MMXXVI, XII, XXXI), exitus, IV);
    CREDO_AEQUALIS_I32(n, I);
    CREDO_VERUM(dies_est(exitus[ZEPHYRUM], MMXXVI, III, II));
    n = fasti_annuum_expandere(III, II, fasti_dies(MMXXV, XII, I),
        fasti_dies(MMXXVII, III, XXXI), exitus, IV);
    CREDO_AEQUALIS_I32(n, II);
    CREDO_VERUM(dies_est(exitus[ZEPHYRUM], MMXXVI, III, II));
    CREDO_VERUM(dies_est(exitus[I], MMXXVII, III, II));
    n = fasti_annuum_expandere(II, XXIX, fasti_dies(MMXXVI, II, I),
        fasti_dies(MMXXVI, II, XXVIII), exitus, IV);
    CREDO_AEQUALIS_I32(n, I);
    CREDO_VERUM(dies_est(exitus[ZEPHYRUM], MMXXVI, II, XXVIII));
    n = fasti_annuum_expandere(II, XXIX, fasti_dies(MMXXVIII, II, I),
        fasti_dies(MMXXVIII, III, I), exitus, IV);
    CREDO_AEQUALIS_I32(n, I);
    CREDO_VERUM(dies_est(exitus[ZEPHYRUM], MMXXVIII, II, XXIX));
    n = fasti_annuum_expandere(III, II, fasti_dies(MMXXVI, III, III),
        fasti_dies(MMXXVII, III, I), exitus, IV);
    CREDO_AEQUALIS_I32(n, ZEPHYRUM);
    n = fasti_annuum_expandere(I, I, fasti_dies(MMXX, I, I),
        fasti_dies(MMXXIX, I, I), exitus, IV);
    CREDO_AEQUALIS_I32(n, IV);
    CREDO_VERUM(dies_est(exitus[III], MMXXIII, I, I));
    n = fasti_annuum_expandere(III, II, fasti_dies(MMXXVI, XII, XXXI),
        fasti_dies(MMXXVI, I, I), exitus, IV);
    CREDO_AEQUALIS_I32(n, ZEPHYRUM);
}

/* fons probationis: ctx = character praefixum tituli; introitus duo
 * (dies V et II mensis primae, genera "natalis" et "aliud") */
nomen structura {
     constans character* praefixum;
                    s32  vocationes;
                    s32  apertum;   /* referentia ultima aperta */
} FonsFictus;

interior b32
fictum_legere (
      vacuum* ctx,
        Dies  prima,
        Dies  ultima,
         Xar* exitus,
     Piscina* piscina)
{
        FonsFictus* f;
    FastiIntroitus* t;
         character  titulus[XXXII];

    (vacuum)ultima;
    f = (FonsFictus*)ctx;
    f->vocationes++;
    t = (FastiIntroitus*)xar_addere(exitus);
    memset(t, ZEPHYRUM, magnitudo(FastiIntroitus));
    t->dies   = fasti_dies(prima.annus, prima.mensis, V);
    t->genus  = chorda_ex_literis("natalis", piscina);
    sprintf(titulus, "%s quintus", f->praefixum);
    t->titulus     = chorda_ex_literis(titulus, piscina);
    t->referentia  = V;
    t              = (FastiIntroitus*)xar_addere(exitus);
    memset(t, ZEPHYRUM, magnitudo(FastiIntroitus));
    t->dies   = fasti_dies(prima.annus, prima.mensis, II);
    t->genus  = chorda_ex_literis("aliud", piscina);
    sprintf(titulus, "%s secundus", f->praefixum);
    t->titulus     = chorda_ex_literis(titulus, piscina);
    t->referentia  = II;
    redde VERUM;
}

interior b32
fictum_aperire (
                     vacuum* ctx,
    constans FastiIntroitus* introitus)
{
    ((FonsFictus*)ctx)->apertum = introitus->referentia;
    redde VERUM;
}

interior b32
introitus_est (
                    Xar* l,
                    i32  i,
                    s32  dies,
     constans character* genus,
     constans character* titulus,
     constans character* fons)
{
    constans FastiIntroitus* t;

    si (!l || i >= xar_numerus(l))
    {
        redde FALSUM;
    }
    t = (constans FastiIntroitus*)xar_obtinere(l, i);
    si (   t->dies.dies != dies || !chorda_aequalis_literis(t->genus,
            genus) || !chorda_aequalis_literis(t->titulus, titulus)
        || !chorda_aequalis_literis(t->fons, fons))
    {
        imprimere("  introitus %d: %d '%.*s' '%.*s' '%.*s'\n",
            (integer)i, (integer)t->dies.dies,
            (integer)t->genus.mensura,
            (constans character*)t->genus.datum,
            (integer)t->titulus.mensura,
            (constans character*)t->titulus.datum,
            (integer)t->fons.mensura,
            (constans character*)t->fons.datum);
        redde FALSUM;
    }
    redde VERUM;
}

/* legere: fontes duo; ordo diei, deinde generis (litterarum: "aliud"
 * ante "natalis"), deinde tituli; fons a portu positus */
interior vacuum
_probare_legere (
    CredoContextus* c)
{
    FastiFontes* f;
     FonsFictus  a;
     FonsFictus  b;
            Xar* l;

    memset(&a, ZEPHYRUM, magnitudo(a));
    memset(&b, ZEPHYRUM, magnitudo(b));
    a.praefixum  = "Beta";
    b.praefixum  = "Alpha";
    f            = fasti_fontes_creare(c->piscina);
    CREDO_NECESSE_NON_NIHIL(f);
    CREDO_NECESSE(fasti_fontes_addere(f, "a", fictum_legere,
        fictum_aperire, &a));
    CREDO_NECESSE(fasti_fontes_addere(f, "b", fictum_legere, NIHIL,
        &b));
    l = fasti_fontes_legere(f, fasti_dies(MMXXVI, X, I), fasti_dies(
        MMXXVI, X, XXXI), c->piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), IV);
    CREDO_VERUM(introitus_est(l, ZEPHYRUM, II, "aliud",
        "Alpha secundus",
        "b"));
    CREDO_VERUM(introitus_est(l, I, II, "aliud", "Beta secundus", "a"));
    CREDO_VERUM(introitus_est(l, II, V, "natalis", "Alpha quintus",
        "b"));
    CREDO_VERUM(introitus_est(l, III, V, "natalis", "Beta quintus",
        "a"));
    CREDO_AEQUALIS_S32(a.vocationes, I);
    CREDO_AEQUALIS_S32(b.vocationes, I);
    /* f NIHIL: vacuum, non NIHIL */
    l = fasti_fontes_legere(NIHIL, fasti_dies(MMXXVI, X, I), fasti_dies(
        MMXXVI, X, XXXI), c->piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), ZEPHYRUM);
}

/* addere: titulus iteratus, plenus (VIII) */
interior vacuum
_probare_addere (
    CredoContextus* c)
{
    FastiFontes* f;
     FonsFictus  a;
      character  titulus[VIII];
            i32  i;

    memset(&a, ZEPHYRUM, magnitudo(a));
    a.praefixum  = "A";
    f            = fasti_fontes_creare(c->piscina);
    CREDO_NECESSE_NON_NIHIL(f);
    CREDO_VERUM(fasti_fontes_addere(f, "a", fictum_legere, NIHIL, &a));
    CREDO_FALSUM(fasti_fontes_addere(f, "a", fictum_legere, NIHIL, &a));
    per (i = I; i < FASTI_FONTES_MAXIMI; i++)
    {
        sprintf(titulus, "f%d", (integer)i);
        CREDO_VERUM(fasti_fontes_addere(f, titulus, fictum_legere,
            NIHIL,
            &a));
    }
    CREDO_FALSUM(fasti_fontes_addere(f, "ultimus", fictum_legere, NIHIL,
        &a));
    CREDO_FALSUM(fasti_fontes_addere(f, "b", NIHIL, NIHIL, &a));
}

/* aperire: fonti suo redditur; fons sine aperire aut ignotus FALSUM */
interior vacuum
_probare_aperire (
    CredoContextus* c)
{
       FastiFontes* f;
        FonsFictus  a;
        FonsFictus  b;
               Xar* l;
    FastiIntroitus  t;

    memset(&a, ZEPHYRUM, magnitudo(a));
    memset(&b, ZEPHYRUM, magnitudo(b));
    a.praefixum  = "Beta";
    b.praefixum  = "Alpha";
    f            = fasti_fontes_creare(c->piscina);
    CREDO_NECESSE(f && fasti_fontes_addere(f, "a", fictum_legere,
        fictum_aperire, &a)
            && fasti_fontes_addere(f, "b", fictum_legere,
        NIHIL, &b));
    l = fasti_fontes_legere(f, fasti_dies(MMXXVI, X, I), fasti_dies(
        MMXXVI, X, XXXI), c->piscina);
    CREDO_NECESSE(l && xar_numerus(l) == IV);
    CREDO_VERUM(fasti_fontes_aperire(f, (constans FastiIntroitus*)
        xar_obtinere(l, III)));
    CREDO_AEQUALIS_S32(a.apertum, V);
    CREDO_FALSUM(fasti_fontes_aperire(f, (constans FastiIntroitus*)
        xar_obtinere(l, II)));
    t       = *(constans FastiIntroitus*)xar_obtinere(l, III);
    t.fons  = chorda_ex_literis("ignotus", c->piscina);
    CREDO_FALSUM(fasti_fontes_aperire(f, &t));
    CREDO_FALSUM(fasti_fontes_aperire(NIHIL, &t));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "annuum", _probare_annuum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "legere", _probare_legere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "addere", _probare_addere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "aperire", _probare_aperire, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("fasti_fontes", SECTIONES);
}
