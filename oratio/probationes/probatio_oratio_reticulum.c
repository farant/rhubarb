/* probatio_oratio_reticulum.c - Instrumentum reticuli (T35 d)
 *
 * I.   Fixtura fixa/reticulum/proba.tsv (cratis II probationis
 *      partitio_aestimatio ut ordines PROBA cum ordine COLUMNAE,
 *      commentario et ordine generis alieni): ordines machinae
 *      RETICULUM-* octetim aequales fixa/reticulum/exspectata.tsv.
 * II.  Relatio humana: gradus recusatus causam nominat (cadit C);
 *      sors una recusatur.
 * III. Recusationes nominatae (instrumentum: exitus II) - genus sine
 *      capite, ordo ante caput, caput discrepans, latitudo falsa,
 *      titulus ignotus, aurum sine praefixo, aurum ut nota, gradus non
 *      numericus, nulli ordines post -ubi.
 * IV.  -ubi f=u: XII ordines manent.
 * Culpa plantata: -ubi neglectum (IV et recusatio 'nulli ordines'
 * RUBRAE).
 */

#include "latina.h"
#include "credo.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "oratio_reticulum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior b32
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                chorda* fructus)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
     character* memoria;

    si (f == NIHIL)
    {
        redde FALSUM;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo <= 0L)
    {
        fclose(f);
        redde FALSUM;
    }
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo);
    si (   memoria                                 == NIHIL
        || fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde FALSUM;
    }
    fclose(f);
    fructus->datum    = (i8*)memoria;
    fructus->mensura  = (i32)longitudo;
    redde VERUM;
}

interior b32
_aequalis (
                chorda  c,
    constans character* literae)
{
    redde (b32)(c.mensura == (i32)strlen(literae)
        && (c.mensura == ZEPHYRUM
            || memcmp(c.datum, literae, (size_t)c.mensura)
                == ZEPHYRUM));
}

interior b32
_continet (
                chorda  c,
    constans character* literae)
{
    i32 longitudo = (i32)strlen(literae);
    i32 i;

    per (i = ZEPHYRUM; i + longitudo <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, literae, (size_t)longitudo) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* lineas conferre; prima differens imprimitur */
interior b32
_lineas_conferre (
    chorda nostra,
    chorda exspectata)
{
    i32 i      = ZEPHYRUM;
    i32 j      = ZEPHYRUM;
    i32 linea  = I;

    dum (i < nostra.mensura || j < exspectata.mensura)
    {
        i32 finis_i = i;
        i32 finis_j = j;

        dum (finis_i < nostra.mensura && nostra.datum[finis_i] != '\n')
        {
            finis_i = finis_i + I;
        }
        dum (   finis_j < exspectata.mensura
             && exspectata.datum[finis_j] != '\n')
        {
            finis_j = finis_j + I;
        }
        si (   finis_i - i != finis_j - j
            || memcmp(nostra.datum + i, exspectata.datum + j,
                (size_t)(finis_i - i)) != ZEPHYRUM)
        {
            imprimere("  linea %d differt:\n    nostra     |%.*s|\n"
                "    exspectata |%.*s|\n", (integer)linea,
                (integer)(finis_i - i),
                (constans character*)nostra.datum + i,
                (integer)(finis_j - j),
                (constans character*)exspectata.datum + j);
            redde FALSUM;
        }
        i      = finis_i + I;
        j      = finis_j + I;
        linea  = linea + I;
    }
    redde VERUM;
}

interior vacuum
_titulum_addere (
               Piscina* piscina,
                   Xar* lista,
    constans character* titulus)
{
    chorda* c = (chorda*)xar_addere(lista);

    si (c != NIHIL)
    {
        *c = chorda_ex_literis(titulus, piscina);
    }
}

interior vacuum
_ubi_addere (
               Piscina* piscina,
                   Xar* ubi,
    constans character* titulus,
    constans character* valor)
{
    OratioReticulumUbi* u = (OratioReticulumUbi*)xar_addere(ubi);

    si (u == NIHIL)
    {
        redde;
    }
    u->titulus = chorda_ex_literis(titulus, piscina);
    u->valores = (chorda*)piscina_allocare(piscina, magnitudo(chorda));
    u->numerus = ZEPHYRUM;
    si (u->valores != NIHIL)
    {
        u->valores[ZEPHYRUM]  = chorda_ex_literis(valor, piscina);
        u->numerus            = I;
    }
}

interior vacuum
_optiones_basis (
                    Piscina* piscina,
    OratioReticulumOptiones* optiones)
{
    oratio_reticulum_optiones_initium(piscina, optiones);
    optiones->genus   = "PROBA";
    optiones->aurum   = "aurum-x";
    optiones->sortes  = "thesaurus";
    optiones->limen   = II;
    optiones->lucrum  = I;
}

interior vacuum
_causam_probare (
    OratioReticulumExitus  exitus,
     constans character*  exspectata)
{
    CREDO_FALSUM (exitus.iudicatum);
    si (!_aequalis(exitus.causa, exspectata))
    {
        imprimere("  causa |%.*s|\n  expectata |%s|\n",
            (integer)exitus.causa.mensura,
            (constans character*)exitus.causa.datum, exspectata);
    }
    CREDO_VERUM (_aequalis(exitus.causa, exspectata));
}

s32
principale (vacuum)
{
                     Piscina* piscina;
          constans character* radix;
                   character  via_proba[1024];
                   character  via_exspectata[1024];
                      chorda  proba;
                      chorda  exspectata;
     OratioReticulumOptiones  optiones;
       OratioReticulumExitus  exitus;
                         b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_oratio_reticulum",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    sprintf(via_proba, "%s/oratio/probationes/fixa/reticulum/proba.tsv",
        radix);
    sprintf(via_exspectata,
        "%s/oratio/probationes/fixa/reticulum/exspectata.tsv", radix);
    CREDO_VERUM (_plagulam_legere(piscina, via_proba, &proba));
    CREDO_VERUM (_plagulam_legere(piscina, via_exspectata,
        &exspectata));
    si (proba.datum == NIHIL || exspectata.datum == NIHIL)
    {
        credo_imprimere_compendium();
        redde I;
    }

    imprimere("\n--- I. Fixtura: ordines machinae ---\n");
    _optiones_basis(piscina, &optiones);
    optiones.machina = VERUM;
    exitus = oratio_reticulum_currere(piscina, proba, "fixa",
        &optiones);
    CREDO_VERUM (exitus.iudicatum);
    si (exitus.iudicatum)
    {
        CREDO_VERUM (_lineas_conferre(exitus.relatio, exspectata));
    }
    alioquin
    {
        imprimere("  causa |%.*s|\n", (integer)exitus.causa.mensura,
            (constans character*)exitus.causa.datum);
    }

    imprimere("\n--- II. Relatio humana ---\n");
    _optiones_basis(piscina, &optiones);
    exitus = oratio_reticulum_currere(piscina, proba, "fixa",
        &optiones);
    CREDO_VERUM (exitus.iudicatum);
    si (exitus.iudicatum)
    {
        imprimere("%.*s", (integer)exitus.relatio.mensura,
            (constans character*)exitus.relatio.datum);
        CREDO_VERUM  (_continet(exitus.relatio, "cadit C"));
        CREDO_FALSUM (_continet(exitus.relatio, "INAESTIMABILIS"));
    }
    _optiones_basis(piscina, &optiones);
    optiones.sortes = "h";
    _causam_probare(oratio_reticulum_currere(piscina, proba, "fixa",
        &optiones), "reticulum: sortes pauciores quam II: 1");

    imprimere("\n--- III. Recusationes nominatae ---\n");
    _optiones_basis(piscina, &optiones);
    _causam_probare(oratio_reticulum_currere(piscina,
        chorda_ex_literis("fixa\tALIUD\t1\n", piscina), "fixa",
        &optiones), "reticulum: genus sine ordine COLUMNAE: PROBA");
    _causam_probare(oratio_reticulum_currere(piscina,
        chorda_ex_literis("fixa\tPROBA\tu\tA\tO\n"
        "fixa\tCOLUMNAE\tPROBA\tf\tthesaurus\taurum-x\n", piscina),
        "fixa", &optiones), "reticulum: ordo ante COLUMNAE: linea 1");
    _causam_probare(oratio_reticulum_currere(piscina,
        chorda_ex_literis("fixa\tCOLUMNAE\tPROBA\tf\tthesaurus"
        "\taurum-x\n"
        "fixa\tCOLUMNAE\tPROBA\tf\tsors\taurum-x\n", piscina),
        "fixa", &optiones),
        "reticulum: ordo COLUMNAE discrepans: linea 2");
    _causam_probare(oratio_reticulum_currere(piscina,
        chorda_ex_literis("fixa\tCOLUMNAE\tPROBA\tf\tthesaurus"
        "\taurum-x\n"
        "fixa\tPROBA\tu\tA\n", piscina), "fixa", &optiones),
        "reticulum: latitudo ordinis falsa: linea 2"
        " (4 campi, 5 exspectati)");

    _optiones_basis(piscina, &optiones);
    _ubi_addere(piscina, optiones.ubi, "zz", "1");
    _causam_probare(oratio_reticulum_currere(piscina, proba, "fixa",
        &optiones), "reticulum: titulus ignotus: zz");

    _optiones_basis(piscina, &optiones);
    optiones.aurum = "thesaurus";
    _causam_probare(oratio_reticulum_currere(piscina, proba, "fixa",
        &optiones),
        "reticulum: columna auri praefixum aurum- non fert: thesaurus");

    _optiones_basis(piscina, &optiones);
    _titulum_addere(piscina, optiones.columnae, "aurum-x");
    _causam_probare(oratio_reticulum_currere(piscina, proba, "fixa",
        &optiones), "reticulum: columna aurea ut nota: aurum-x");

    _optiones_basis(piscina, &optiones);
    _titulum_addere(piscina, optiones.gradus, "f");
    _causam_probare(oratio_reticulum_currere(piscina, proba, "fixa",
        &optiones),
        "reticulum: gradus in valore non numerico: f, linea 3");

    _optiones_basis(piscina, &optiones);
    _ubi_addere(piscina, optiones.ubi, "f", "w");
    _causam_probare(oratio_reticulum_currere(piscina, proba, "fixa",
        &optiones), "reticulum: nulli ordines post -ubi");

    imprimere("\n--- IV. -ubi f=u ---\n");
    _optiones_basis(piscina, &optiones);
    optiones.machina = VERUM;
    _ubi_addere(piscina, optiones.ubi, "f", "u");
    exitus = oratio_reticulum_currere(piscina, proba, "fixa",
        &optiones);
    CREDO_VERUM (exitus.iudicatum);
    CREDO_VERUM (exitus.iudicatum
        && _continet(exitus.relatio,
        "fixa\tRETICULUM-COLUMNA\th\t1\t12\t"));

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
