/* probatio_saltuarius_limes.c - Limes disci: probatio integrationis
 * (Chunk A3)
 *
 * UNICA probatio quae discum verum tangit (arbor ficta in AREA credo
 * sectionis - praecedens domus: probatio_iter_directoria). Probantur:
 * indices
 * ex disco (ordinati, parens selectum per titulum), praevisus
 * omnium generum (directorium/textus/magnum/binarium/vacuum/non
 * intrabile/non legibile), selectandum in ascensu, MEMORIA FRENATA
 * (refectio arenae: usus non crescit trans refectiones), IMAGINES
 * (quadrans Q7: photographia -> cellulae ad pannum aptatae; JPEG
 * fractum -> nuntius; amplitudo ignota -> nuntius).
 *
 * AREA, NON /tmp (2026-10-23, parcum ...BPYFX): arbor olim sub /tmp
 * fixa erat, ergo columna parentis /tmp IPSUM indicabat - plus quam
 * SALT_INTROITUS_MAXIMI (4096) introitus (diaria mensoris vacua
 * cumulata) fixum ex indice expellebant et porta frigida rubuit. In
 * area propria parens solum fixa nostra continet; cursus paralleli se
 * non conturbant; credo aream post sectionem delet.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "lectiones.h"
#include "saltuarius_limes.h"
#include <stdio.h>
#include <stdlib.h>

/* 'radix/relativa' scribendum aperire */
interior FILE*
_aperire (
    constans character* radix,
    constans character* relativa)
{
    character via[DXII + CXXVIII];

    sprintf(via, "%s/%s", radix, relativa);
    redde lectiones_fopen(via, "wb");
}

/* mandatum conchae cum via fixi semel inserta (%s in forma) */
interior vacuum
_mandare (
    constans character* forma,
    constans character* radix)
{
    character mandatum[MXXIV + DXII];

    sprintf(mandatum, forma, radix);
    (vacuum)system(mandatum);
}

/* arbor ficta sub 'fixum' (in area sectionis, credo_area) */
interior vacuum
_arborem_figere (
    constans character* fixum)
{
    FILE* f;
    i32 k;

    _mandare("mkdir -p '%s/alpha'", fixum);
    _mandare("mkdir -p '%s/beta'", fixum);
    f = _aperire(fixum, "alpha/x.c");
    si (f)
    { fputs("x\n", f); fclose(f);
    }
    f = _aperire(fixum, "alpha/y.c");
    si (f)
    { fputs("y\n", f); fclose(f);
    }
    f = _aperire(fixum, "planus.txt");
    si (f)
    { fputs("salve\nmunde\n", f); fclose(f);
    }
    f = _aperire(fixum, ".occultum");
    si (f)
    { fputs("celatum\n", f); fclose(f);
    }
    f = _aperire(fixum, "binarius.dat");
    si (f)
    {
        fputs("ab", f);
        fputc(ZEPHYRUM, f);
        fputs("cd", f);
        fclose(f);
    }
    /* magnus: sparsus, mensura MAXIMUS + I */
    f = _aperire(fixum, "magnus.bin");
    si (f)
    {
        fseek(f, (longus)SALT_PRAEVISUS_MAXIMUS, SEEK_SET);
        fputc('x', f);
        fclose(f);
    }
    (vacuum)k;
}

/* Sectio UNA: probatio scaenica - blocci statum priorum ex proposito
 * continuant; gradus (CREDO_GRADUS) fracturam nominant. */
interior vacuum
_scaenam_probare (
    CredoContextus* c)
{
            Piscina* piscina = c->piscina;
           SaltuariusRes* res;
      SaltuariusColumnae* columnae;
    SaltuariusTabularium* parens;
    SaltuariusTabularium* currens;
    SaltuariusTabularium* praevisus;
         SaltuariusLimes* limes;
     constans character* area;
               character  fixum[DXII];
               character  fixum_imaginum[DXII];

    area = credo_area(c);
    CREDO_NECESSE_NON_NIHIL (area);
    sprintf(fixum, "%.400s/saltuarius_limes_fixum", area);
    sprintf(fixum_imaginum, "%.400s/saltuarius_limes_imagines", area);
    _arborem_figere(fixum);

    res        = saltuarius_res_creare(piscina, fixum);
    columnae   = saltuarius_columnae_creare(piscina);
    parens     = saltuarius_tabularium_creare(piscina);
    currens    = saltuarius_tabularium_creare(piscina);
    praevisus  = saltuarius_tabularium_creare(piscina);
    limes = saltuarius_limes_creare(piscina, parens, currens,
        praevisus);
    CREDO_NON_NIHIL (limes);


    /* ========================================================
     * PROBARE: indices ex disco - ordinati, punctata celata,
     * parens selectum per titulum
     * ======================================================== */

    {
        CREDO_GRADUS ("indices ex disco");

        CREDO_VERUM (saltuarius_limes_indices(limes, res, columnae,
            NIHIL));
        /* alpha beta (directoria) binarius.dat magnus.bin
         * planus.txt; .occultum celatum */
        CREDO_AEQUALIS_I32 ((i32)currens->numerus, V);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            currens->introitus[ZEPHYRUM].titulus, "alpha");
        CREDO_CHORDA_AEQUALIS_LITERIS (
            currens->introitus[I].titulus, "beta");
        CREDO_CHORDA_AEQUALIS_LITERIS (
            currens->introitus[II].titulus, "binarius.dat");
        CREDO_CHORDA_AEQUALIS_LITERIS (
            currens->introitus[III].titulus, "magnus.bin");
        CREDO_CHORDA_AEQUALIS_LITERIS (
            currens->introitus[IV].titulus, "planus.txt");
        CREDO_AEQUALIS_S32 (currens->selectio, ZEPHYRUM);

        /* parens = area sectionis; introitus fixi selectus */
        CREDO_NON_NIHIL (columnae->parens);
        CREDO_NON_NIHIL (saltuarius_tabularium_selectum(parens));
        CREDO_CHORDA_AEQUALIS_LITERIS (
            saltuarius_tabularium_selectum(parens)->titulus,
            "saltuarius_limes_fixum");
    }


    /* ========================================================
     * PROBARE: praevisus omnium generum
     * ======================================================== */

    {
        CREDO_GRADUS ("praevisum generum");

        /* directorium (alpha): index x.c y.c */
        currens->selectio = ZEPHYRUM;
        saltuarius_limes_praevisum(limes, res, columnae);
        CREDO_AEQUALIS_I32 ((i32)columnae->praevisus_genus,
            (i32)SALT_PRAEVISUS_DIRECTORIUM);
        CREDO_AEQUALIS_I32 ((i32)praevisus->numerus, II);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            praevisus->introitus[ZEPHYRUM].titulus, "x.c");

        /* directorium vacuum (beta) */
        currens->selectio = I;
        saltuarius_limes_praevisum(limes, res, columnae);
        CREDO_AEQUALIS_I32 ((i32)columnae->praevisus_genus,
            (i32)SALT_PRAEVISUS_DIRECTORIUM);
        CREDO_AEQUALIS_I32 ((i32)praevisus->numerus, ZEPHYRUM);

        /* textus (planus.txt) */
        currens->selectio = IV;
        saltuarius_limes_praevisum(limes, res, columnae);
        CREDO_AEQUALIS_I32 ((i32)columnae->praevisus_genus,
            (i32)SALT_PRAEVISUS_TEXTUS);
        CREDO_CHORDA_AEQUALIS_LITERIS (columnae->praevisus_textus,
            "salve\nmunde\n");

        /* magnum sine lectione */
        currens->selectio = III;
        saltuarius_limes_praevisum(limes, res, columnae);
        CREDO_AEQUALIS_I32 ((i32)columnae->praevisus_genus,
            (i32)SALT_PRAEVISUS_NUNTIUS);
        CREDO_CHORDA_AEQUALIS_LITERIS (columnae->praevisus_nuntius,
            "(filum magnum)");

        /* binarium */
        currens->selectio = II;
        saltuarius_limes_praevisum(limes, res, columnae);
        CREDO_AEQUALIS_I32 ((i32)columnae->praevisus_genus,
            (i32)SALT_PRAEVISUS_NUNTIUS);
        CREDO_CHORDA_AEQUALIS_LITERIS (columnae->praevisus_nuntius,
            "(filum binarium)");
    }


    /* ========================================================
     * PROBARE: memoria frenata - refectio arenae praevisus
     * ======================================================== */

    {
        memoriae_index usus_prior;
                   i32 k;

        CREDO_GRADUS ("memoriam frenatam");

        currens->selectio = IV;   /* planus.txt */
        saltuarius_limes_praevisum(limes, res, columnae);
        usus_prior = piscina_summa_usus(limes->praevisus);
        per (k = ZEPHYRUM; k < X; k++)
        {
            saltuarius_limes_praevisum(limes, res, columnae);
        }
        CREDO_AEQUALIS_I32 (
            (i32)piscina_summa_usus(limes->praevisus),
            (i32)usus_prior);
    }


    /* ========================================================
     * PROBARE: descensus + ascensus cum selectando
     * ======================================================== */

    {
        chorda selectandum;

        CREDO_GRADUS ("descensum et ascensum");

        saltuarius_res_intrare(res,
            chorda_ex_literis("alpha", piscina));
        CREDO_VERUM (saltuarius_limes_indices(limes, res, columnae,
            NIHIL));
        CREDO_AEQUALIS_I32 ((i32)currens->numerus, II);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            saltuarius_tabularium_selectum(parens)->titulus,
            "alpha");

        selectandum = chorda_ex_literis("alpha", piscina);
        saltuarius_res_ascendere(res);
        CREDO_VERUM (saltuarius_limes_indices(limes, res, columnae,
            &selectandum));
        CREDO_CHORDA_AEQUALIS_LITERIS (
            saltuarius_tabularium_selectum(currens)->titulus,
            "alpha");
    }


    /* ========================================================
     * PROBARE: non legibile (directorium + praevisus)
     * ======================================================== */

    {
        CREDO_GRADUS ("non legibile");

        _mandare("chmod 000 '%s/beta'", fixum);

        /* praevisus in beta */
        currens->selectio = I;
        saltuarius_limes_praevisum(limes, res, columnae);
        CREDO_AEQUALIS_I32 ((i32)columnae->praevisus_genus,
            (i32)SALT_PRAEVISUS_NUNTIUS);
        CREDO_CHORDA_AEQUALIS_LITERIS (columnae->praevisus_nuntius,
            "(non legibile)");

        /* indices in beta ipso */
        saltuarius_res_intrare(res,
            chorda_ex_literis("beta", piscina));
        CREDO_FALSUM (saltuarius_limes_indices(limes, res, columnae,
            NIHIL));
        CREDO_AEQUALIS_I32 ((i32)currens->numerus, ZEPHYRUM);

        _mandare("chmod 755 '%s/beta'", fixum);
    }


    /* ========================================================
     * PROBARE: librum aperire (B3) - textus, custodiae
     * ======================================================== */

    {
           SaltuariusNexus* nexus = saltuarius_nexus_creare(piscina);
           SaltuariusLiber* liber;
        constans character* causa;

        CREDO_GRADUS ("librum aperire");

        _mandare("chmod 755 '%s/beta'", fixum);
        saltuarius_res_ascendere(res);   /* retro ad fixum */
        CREDO_VERUM (saltuarius_limes_indices(limes, res, columnae,
            NIHIL));

        /* planus.txt (index IV) -> liber verus */
        currens->selectio = IV;
        liber = saltuarius_limes_librum_aperire(limes, piscina,
            nexus, res, &causa);
        CREDO_NON_NIHIL (liber);
        si (liber != NIHIL)
        {
            CREDO_AEQUALIS_I32 ((i32)liber->numerus_linearum, III);
            CREDO_CHORDA_AEQUALIS_LITERIS (
                saltuarius_liber_linea(liber, ZEPHYRUM), "salve");
            saltuarius_liber_destruere(liber);
        }

        /* magnus -> refusatum sine lectione */
        currens->selectio = III;
        liber = saltuarius_limes_librum_aperire(limes, piscina,
            nexus, res, &causa);
        CREDO_AEQUALIS_PTR ((constans vacuum*)liber,
            (constans vacuum*)NIHIL);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            chorda_ex_literis(causa, piscina), "(filum magnum)");

        /* binarius -> refusatum */
        currens->selectio = II;
        liber = saltuarius_limes_librum_aperire(limes, piscina,
            nexus, res, &causa);
        CREDO_AEQUALIS_PTR ((constans vacuum*)liber,
            (constans vacuum*)NIHIL);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            chorda_ex_literis(causa, piscina), "(filum binarium)");

        /* directorium -> non filum */
        currens->selectio = ZEPHYRUM;
        liber = saltuarius_limes_librum_aperire(limes, piscina,
            nexus, res, &causa);
        CREDO_AEQUALIS_PTR ((constans vacuum*)liber,
            (constans vacuum*)NIHIL);
    }

    _mandare("rm -rf '%s'", fixum);


    /* ========================================================
     * PROBARE: imagines (quadrans Q7) - arbor propria, ne indices
     * supra mutentur
     * ======================================================== */

    {
                 character  mandatum[MXXIV + DXII];
        constans character* radix = getenv("RHUBARB_RADIX");
        SaltuariusRes* res_imaginum;
        SaltuariusMetra metra;
        FILE* f;

        CREDO_GRADUS ("imagines");
        sprintf(mandatum,
            "mkdir -p '%s' && cp '%.400s/probationes/fixa/"
            "quadrans/christus_sculptus.jpg' '%s/pictura.jpg'",
            fixum_imaginum, radix != NIHIL ? radix : "..",
            fixum_imaginum);
        (vacuum)system(mandatum);
        f = _aperire(fixum_imaginum, "fracta.jpg");
        si (f)
        {
            fputs("\xFF\xD8\xFF\xE0 non est imago", f);
            fclose(f);
        }
        res_imaginum = saltuarius_res_creare(piscina, fixum_imaginum);
        CREDO_VERUM (saltuarius_limes_indices(limes, res_imaginum,
            columnae, NIHIL));
        /* fracta.jpg (0) pictura.jpg (I) */
        CREDO_AEQUALIS_I32 ((i32)currens->numerus, II);

        /* amplitudo ignota: imago non computatur, nuntius */
        currens->selectio = I;
        saltuarius_limes_praevisum(limes, res_imaginum, columnae);
        CREDO_AEQUALIS_I32 ((i32)columnae->praevisus_genus,
            (i32)SALT_PRAEVISUS_NUNTIUS);

        /* amplitudo LXXX x XXIV: pannus praevisus; imago aptata */
        saltuarius_limes_amplitudo(limes, LXXX, XXIV);
        saltuarius_columnae_metiri(LXXX, XXIV, &metra);
        saltuarius_limes_praevisum(limes, res_imaginum, columnae);
        CREDO_AEQUALIS_I32 ((i32)columnae->praevisus_genus,
            (i32)SALT_PRAEVISUS_IMAGO);
        CREDO_NON_NIHIL (columnae->praevisus_cellulae);
        CREDO_VERUM (columnae->praevisus_imago_lat > ZEPHYRUM);
        CREDO_VERUM (columnae->praevisus_imago_lat
            <= (i32)metra.praevisus_lat);
        CREDO_VERUM (columnae->praevisus_imago_alt
            <= (i32)metra.contentum_alt);
        /* christus_sculptus 800x729 in pannum: latitudo aut altitudo
         * plena (aptatio tangit limitem unum) */
        CREDO_VERUM (   columnae->praevisus_imago_lat
                         == (i32)metra.praevisus_lat
                     || columnae->praevisus_imago_alt
                         == (i32)metra.contentum_alt);

        /* JPEG fractum: nuntius, non binarium */
        currens->selectio = ZEPHYRUM;
        saltuarius_limes_praevisum(limes, res_imaginum, columnae);
        CREDO_AEQUALIS_I32 ((i32)columnae->praevisus_genus,
            (i32)SALT_PRAEVISUS_NUNTIUS);
        CREDO_CHORDA_AEQUALIS_LITERIS (columnae->praevisus_nuntius,
            "(imago non legibilis)");
    }


}

hic_manens constans CredoSectio SECTIONES[] = {
    { "limes: sessio", _scaenam_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("saltuarius_limes", SECTIONES);
}
