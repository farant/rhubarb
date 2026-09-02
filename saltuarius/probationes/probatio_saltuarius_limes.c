/* probatio_saltuarius_limes.c - Limes disci: probatio integrationis
 * (Chunk A3)
 *
 * UNICA probatio quae discum verum tangit (arbor ficta /tmp -
 * praecedens domus: probatio_iter_directoria). Probantur: indices
 * ex disco (ordinati, parens selectum per titulum), praevisus
 * omnium generum (directorium/textus/magnum/binarium/vacuum/non
 * intrabile/non legibile), selectandum in ascensu, MEMORIA FRENATA
 * (refectio arenae: usus non crescit trans refectiones).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "saltuarius_limes.h"
#include <stdio.h>
#include <stdlib.h>

#define FIXUM "/tmp/saltuarius_limes_fixum"

interior vacuum
_arborem_figere (vacuum)
{
    FILE* f;
    i32 k;

    (vacuum)system("rm -rf " FIXUM
        " && mkdir -p " FIXUM "/alpha " FIXUM "/beta");
    f = fopen(FIXUM "/alpha/x.c", "wb");
    si (f)
    { fputs("x\n", f); fclose(f);
    }
    f = fopen(FIXUM "/alpha/y.c", "wb");
    si (f)
    { fputs("y\n", f); fclose(f);
    }
    f = fopen(FIXUM "/planus.txt", "wb");
    si (f)
    { fputs("salve\nmunde\n", f); fclose(f);
    }
    f = fopen(FIXUM "/.occultum", "wb");
    si (f)
    { fputs("celatum\n", f); fclose(f);
    }
    f = fopen(FIXUM "/binarius.dat", "wb");
    si (f)
    {
        fputs("ab", f);
        fputc(ZEPHYRUM, f);
        fputs("cd", f);
        fclose(f);
    }
    /* magnus: sparsus, mensura MAXIMUS + I */
    f = fopen(FIXUM "/magnus.bin", "wb");
    si (f)
    {
        fseek(f, (longus)SALT_PRAEVISUS_MAXIMUS, SEEK_SET);
        fputc('x', f);
        fclose(f);
    }
    (vacuum)k;
}

s32
principale (vacuum)
{
                     b32  praeteritus;
                 Piscina* piscina;
           SaltuariusRes* res;
      SaltuariusColumnae* columnae;
    SaltuariusTabularium* parens;
    SaltuariusTabularium* currens;
    SaltuariusTabularium* praevisus;
         SaltuariusLimes* limes;

    piscina = piscina_generare_dynamicum("probatio_salt_limes",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    _arborem_figere();

    res        = saltuarius_res_creare(piscina, FIXUM);
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
        imprimere("\n--- Probans indices ex disco ---\n");

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

        /* parens = /tmp; introitus fixi selectus */
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
        imprimere("\n--- Probans praevisum generum ---\n");

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

        imprimere("\n--- Probans memoriam frenatam ---\n");

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

        imprimere("\n--- Probans descensum et ascensum ---\n");

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
        imprimere("\n--- Probans non legibile ---\n");

        (vacuum)system("chmod 000 " FIXUM "/beta");

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

        (vacuum)system("chmod 755 " FIXUM "/beta");
    }


    /* ========================================================
     * PROBARE: librum aperire (B3) - textus, custodiae
     * ======================================================== */

    {
           SaltuariusNexus* nexus = saltuarius_nexus_creare(piscina);
           SaltuariusLiber* liber;
        constans character* causa;

        imprimere("\n--- Probans librum aperire ---\n");

        (vacuum)system("chmod 755 " FIXUM "/beta");
        saltuarius_res_ascendere(res);   /* retro ad FIXUM */
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

    (vacuum)system("rm -rf " FIXUM);

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    saltuarius_limes_destruere(limes);
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
