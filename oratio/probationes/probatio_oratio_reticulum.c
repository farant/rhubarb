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
 * V.   (T36 b) Fixtura paritas.tsv - aurum = paritas columnarum f g h:
 *      singulae et paria nihil sciunt, triplex exacta (incrementum
 *      +XII); ordines machinae == exspectata_paritas.tsv; relatio
 *      humana: catenae congruunt, altitudo 0 latitudo 3.
 * VI.  (T36 b) Semen -initium f,g: gradus 0 f+g, gradus I h (+XII);
 *      -greges initium.
 * Culpae plantatae: -ubi neglectum (T35 d); incrementum sine
 * subtractione (T36 b: ordines INFIMUM paritatis).
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
                   character  via_paritas[1024];
                   character  via_exspectata_paritas[1024];
                      chorda  proba;
                      chorda  exspectata;
                      chorda  paritas;
                      chorda  exspectata_paritas;
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
    sprintf(via_paritas,
        "%s/oratio/probationes/fixa/reticulum/paritas.tsv", radix);
    sprintf(via_exspectata_paritas,
        "%s/oratio/probationes/fixa/reticulum/exspectata_paritas.tsv",
        radix);
    CREDO_VERUM (_plagulam_legere(piscina, via_proba, &proba));
    CREDO_VERUM (_plagulam_legere(piscina, via_exspectata,
        &exspectata));
    CREDO_VERUM (_plagulam_legere(piscina, via_paritas, &paritas));
    CREDO_VERUM (_plagulam_legere(piscina, via_exspectata_paritas,
        &exspectata_paritas));
    si (   proba.datum   == NIHIL || exspectata.datum == NIHIL
        || paritas.datum == NIHIL || exspectata_paritas.datum == NIHIL)
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
        /* T36 b: h constans nominata, gradus catenae liberae qui in C
         * cadit = candidatus condicionatus */
        CREDO_VERUM  (_continet(exitus.relatio, "constantes: h"));
        CREDO_VERUM  (_continet(exitus.relatio,
            "CANDIDATI CONDICIONATI"));
        CREDO_VERUM  (_continet(exitus.relatio,
            "f (gradus 1): C 7 -> 5"));
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

    /* T36 b: profunditas, initium, greges initium */
    _optiones_basis(piscina, &optiones);
    optiones.profunditas = V;
    _causam_probare(oratio_reticulum_currere(piscina, proba, "fixa",
        &optiones), "reticulum: profunditas maior quam IV: 5");

    _optiones_basis(piscina, &optiones);
    _titulum_addere(piscina, optiones.initium, "zz");
    _causam_probare(oratio_reticulum_currere(piscina, proba, "fixa",
        &optiones), "reticulum: titulus ignotus: zz");

    _optiones_basis(piscina, &optiones);
    _titulum_addere(piscina, optiones.initium, "aurum-x");
    _causam_probare(oratio_reticulum_currere(piscina, proba, "fixa",
        &optiones), "reticulum: columna aurea ut initium: aurum-x");

    _optiones_basis(piscina, &optiones);
    _titulum_addere(piscina, optiones.initium, "thesaurus");
    _causam_probare(oratio_reticulum_currere(piscina, proba, "fixa",
        &optiones),
        "reticulum: columna non inter notas ut initium: thesaurus");

    _optiones_basis(piscina, &optiones);
    _titulum_addere(piscina, optiones.initium, "h");
    _causam_probare(oratio_reticulum_currere(piscina, proba, "fixa",
        &optiones), "reticulum: columna constans ut initium: h");

    _optiones_basis(piscina, &optiones);
    optiones.greges = "initium";
    _causam_probare(oratio_reticulum_currere(piscina, proba, "fixa",
        &optiones), "reticulum: greges initium sine -initium");

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

    imprimere("\n--- V. Paritas: infima profunditate III ---\n");
    _optiones_basis(piscina, &optiones);
    optiones.machina      = VERUM;
    optiones.profunditas  = III;
    exitus = oratio_reticulum_currere(piscina, paritas, "fixa",
        &optiones);
    CREDO_VERUM (exitus.iudicatum);
    si (exitus.iudicatum)
    {
        CREDO_VERUM (_lineas_conferre(exitus.relatio,
            exspectata_paritas));
    }
    alioquin
    {
        imprimere("  causa |%.*s|\n", (integer)exitus.causa.mensura,
            (constans character*)exitus.causa.datum);
    }
    _optiones_basis(piscina, &optiones);
    optiones.profunditas = III;
    exitus = oratio_reticulum_currere(piscina, paritas, "fixa",
        &optiones);
    CREDO_VERUM (exitus.iudicatum);
    si (exitus.iudicatum)
    {
        imprimere("%.*s", (integer)exitus.relatio.mensura,
            (constans character*)exitus.relatio.datum);
        CREDO_VERUM (_continet(exitus.relatio, "catenae congruunt"));
        CREDO_VERUM (_continet(exitus.relatio,
            "altitudo 0, latitudo 3"));
        CREDO_VERUM (_continet(exitus.relatio,
            "f x g x h: greges 8, recti 24, incrementum +12"));
    }

    imprimere("\n--- VI. Semen f,g: catena h accipit ---\n");
    _optiones_basis(piscina, &optiones);
    optiones.machina = VERUM;
    _titulum_addere(piscina, optiones.initium, "f");
    _titulum_addere(piscina, optiones.initium, "g");
    exitus = oratio_reticulum_currere(piscina, paritas, "fixa",
        &optiones);
    CREDO_VERUM (exitus.iudicatum);
    CREDO_VERUM (exitus.iudicatum
        && _continet(exitus.relatio,
        "fixa\tRETICULUM-CATENA\t1\t0\tf+g\t4\t12\t0\t0\n"));
    CREDO_VERUM (exitus.iudicatum
        && _continet(exitus.relatio,
        "fixa\tRETICULUM-CATENA\t1\t1\th\t8\t24\t12\t0\n"));
    _optiones_basis(piscina, &optiones);
    _titulum_addere(piscina, optiones.initium, "f");
    _titulum_addere(piscina, optiones.initium, "g");
    optiones.greges = "initium";
    exitus = oratio_reticulum_currere(piscina, paritas, "fixa",
        &optiones);
    CREDO_VERUM (exitus.iudicatum);
    CREDO_VERUM (exitus.iudicatum
        && _continet(exitus.relatio, "GREGES initium"));
    CREDO_VERUM (exitus.iudicatum
        && _continet(exitus.relatio,
        "0 initium f x g: greges 4, recti 12"));

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
