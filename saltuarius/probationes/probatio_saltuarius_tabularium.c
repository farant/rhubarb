/* probatio_saltuarius_tabularium.c - Exemplar indicis (Chunk A1)
 *
 * Tabulae fictae iniectae - discus numquam tangitur (sutura
 * probationum ipsa). Probantur: ordinatio (directoria primum,
 * ordine octetorum), filtrum punctatorum, selectio clausa,
 * primum/ultimum, saltus f<littera> (cyclicus, insensibilis
 * maiusculis), volumen aptatum.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "saltuarius_tabularium.h"
#include <stdio.h>

interior DirectoriumIntroitus
_introitus (
    constans character* titulus,
        IntroitusGenus  genus,
               Piscina* piscina)
{
    DirectoriumIntroitus intr;

    intr.titulus  = chorda_ex_literis(titulus, piscina);
    intr.genus    = genus;
    intr.mensura  = ZEPHYRUM;
    redde intr;
}


/* ==================================================
 * PROBARE: impletio - ordinatio + filtrum punctatorum
 * ================================================== */

interior vacuum
_implere_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

    {
        SaltuariusTabularium* tab;
        DirectoriumIntroitus  fontes[VI];


        fontes[ZEPHYRUM] = _introitus("zeta.c", INTROITUS_FILUM,
            piscina);
        fontes[I] = _introitus("alpha", INTROITUS_DIRECTORIUM,
            piscina);
        fontes[II] = _introitus(".git", INTROITUS_DIRECTORIUM,
            piscina);
        fontes[III]  = _introitus("beta.c", INTROITUS_FILUM, piscina);
        fontes[IV]   = _introitus("Marmor", INTROITUS_FILUM, piscina);
        fontes[V] = _introitus("gamma", INTROITUS_DIRECTORIUM,
            piscina);

        tab = saltuarius_tabularium_creare(piscina);
        CREDO_NON_NIHIL (tab);
        saltuarius_tabularium_implere(tab, piscina, fontes, VI);

        /* .git filtratum; directoria primum; ordo octetorum
         * ('M' < 'b' in ASCII) */
        CREDO_AEQUALIS_I32 ((i32)tab->numerus, V);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            tab->introitus[ZEPHYRUM].titulus, "alpha");
        CREDO_CHORDA_AEQUALIS_LITERIS (
            tab->introitus[I].titulus, "gamma");
        CREDO_CHORDA_AEQUALIS_LITERIS (
            tab->introitus[II].titulus, "Marmor");
        CREDO_CHORDA_AEQUALIS_LITERIS (
            tab->introitus[III].titulus, "beta.c");
        CREDO_CHORDA_AEQUALIS_LITERIS (
            tab->introitus[IV].titulus, "zeta.c");
        CREDO_AEQUALIS_S32 (tab->selectio, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (tab->volumen, ZEPHYRUM);


        /* ====================================================
         * PROBARE: motus clausus + selectum
         * ==================================================== */

        CREDO_GRADUS ("motum (clausum)");

        saltuarius_tabularium_movere(tab, II);
        CREDO_AEQUALIS_S32 (tab->selectio, II);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            saltuarius_tabularium_selectum(tab)->titulus, "Marmor");
        saltuarius_tabularium_movere(tab, -X);
        CREDO_AEQUALIS_S32 (tab->selectio, ZEPHYRUM);
        saltuarius_tabularium_movere(tab, C);
        CREDO_AEQUALIS_S32 (tab->selectio, IV);


        /* ====================================================
         * PROBARE: primum / ultimum (gg / G)
         * ==================================================== */

        saltuarius_tabularium_primum(tab);
        CREDO_AEQUALIS_S32 (tab->selectio, ZEPHYRUM);
        saltuarius_tabularium_ultimum(tab);
        CREDO_AEQUALIS_S32 (tab->selectio, IV);


        /* ====================================================
         * PROBARE: saltus f<littera> - cyclicus, insensibilis
         * ==================================================== */

        CREDO_GRADUS ("saltum ad litteram");

        saltuarius_tabularium_primum(tab);
        CREDO_VERUM (saltuarius_tabularium_saltare(tab, (s32)'b'));
        CREDO_AEQUALIS_S32 (tab->selectio, III);

        /* 'm' minuscula invenit "Marmor" (plicatio) - retro
         * cyclice */
        CREDO_VERUM (saltuarius_tabularium_saltare(tab, (s32)'m'));
        CREDO_AEQUALIS_S32 (tab->selectio, II);

        /* solus 'm': cyclus plenus ad se ipsum redit */
        CREDO_VERUM (saltuarius_tabularium_saltare(tab, (s32)'m'));
        CREDO_AEQUALIS_S32 (tab->selectio, II);

        /* littera absens: FALSUM, selectio immota */
        CREDO_FALSUM (saltuarius_tabularium_saltare(tab, (s32)'x'));
        CREDO_AEQUALIS_S32 (tab->selectio, II);


        /* ====================================================
         * PROBARE: volumen aptatum (fenestra III alta)
         * ==================================================== */

        CREDO_GRADUS ("volumen");

        saltuarius_tabularium_ultimum(tab);   /* selectio IV */
        saltuarius_tabularium_aptare(tab, III);
        CREDO_AEQUALIS_S32 (tab->volumen, II);
        saltuarius_tabularium_primum(tab);
        saltuarius_tabularium_aptare(tab, III);
        CREDO_AEQUALIS_S32 (tab->volumen, ZEPHYRUM);
        /* selectio intra fenestram: volumen immotum */
        saltuarius_tabularium_movere(tab, II);
        saltuarius_tabularium_aptare(tab, III);
        CREDO_AEQUALIS_S32 (tab->volumen, ZEPHYRUM);
    }
}


/* ==================================================
 * PROBARE: tabularium vacuum (omnia punctata)
 * ================================================== */

interior vacuum
_vacuum_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

    {
        SaltuariusTabularium* tab;
        DirectoriumIntroitus  fontes[II];


        fontes[ZEPHYRUM] = _introitus(".a", INTROITUS_FILUM,
            piscina);
        fontes[I] = _introitus(".b", INTROITUS_DIRECTORIUM,
            piscina);

        tab = saltuarius_tabularium_creare(piscina);
        saltuarius_tabularium_implere(tab, piscina, fontes, II);
        CREDO_AEQUALIS_I32 ((i32)tab->numerus, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (tab->selectio, -I);
        CREDO_AEQUALIS_PTR (
            (constans vacuum*)saltuarius_tabularium_selectum(tab),
            (constans vacuum*)NIHIL);
        saltuarius_tabularium_movere(tab, I);      /* non cadit */
        CREDO_AEQUALIS_S32 (tab->selectio, -I);
        CREDO_FALSUM (saltuarius_tabularium_saltare(tab, (s32)'a'));
        saltuarius_tabularium_aptare(tab, X);
        CREDO_AEQUALIS_S32 (tab->volumen, ZEPHYRUM);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "impletio, motus, saltus, volumen", _implere_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "tabularium vacuum (omnia punctata)", _vacuum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("saltuarius_tabularium", SECTIONES);
}
