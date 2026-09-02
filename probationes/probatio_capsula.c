/* probatio_capsula.c - Tests for capsula library */

#include "../include/latina.h"
#include "../include/piscina.h"
#include "../include/capsula.h"
#include "../include/chorda.h"
#include "../include/filum.h"
#include "capsula_assets.h"

#include <stdio.h>
#include <string.h>


/* ========================================================================
 * PROBATIONES
 * ======================================================================== */

interior i32
proba_aperire (
    Piscina* piscina)
{
    Capsula* capsula;

    printf("  proba_aperire: ");

    capsula = capsula_aperire(&capsula_assets, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    si (capsula_numerus(capsula) != II)
    {
        printf("FALSUM - expectavit 2 fila, obtinuit %d\n",
               capsula_numerus(capsula));
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}

interior i32
proba_habet (
    Piscina* piscina)
{
    Capsula* capsula;

    printf("  proba_habet: ");

    capsula = capsula_aperire(&capsula_assets, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    si (!capsula_habet(capsula, "test_assets/hello.txt"))
    {
        printf("FALSUM - non invenit 'test_assets/hello.txt'\n");
        redde FALSUM;
    }

    si (!capsula_habet(capsula, "test_assets/lorem.txt"))
    {
        printf("FALSUM - non invenit 'test_assets/lorem.txt'\n");
        redde FALSUM;
    }

    si (capsula_habet(capsula, "non_existit.txt"))
    {
        printf("FALSUM - invenit 'non_existit.txt' quod non existit\n");
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}

interior i32
proba_legere_hello (
    Piscina* piscina)
{
               Capsula* capsula;
        CapsulaFructus  res;
    constans character* expected = "Hello, World!\n"
                                   "This is a test file for capsula embedding.\n";

    printf("  proba_legere_hello: ");

    capsula = capsula_aperire(&capsula_assets, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    res = capsula_legere(capsula, "test_assets/hello.txt", piscina);

    si (res.status != CAPSULA_OK)
    {
        printf("FALSUM - capsula_legere status = %d\n", res.status);
        redde FALSUM;
    }

    si (res.datum.mensura != (i32)strlen(expected))
    {
        printf("FALSUM - expectavit %d bytes, obtinuit %d\n",
               (i32)strlen(expected), (i32)res.datum.mensura);
        redde FALSUM;
    }

    si (memcmp(res.datum.datum, expected, res.datum.mensura) != 0)
    {
        printf("FALSUM - contentum non congruit\n");
        printf("    Expectatum: '%s'\n", expected);
        printf("    Obtentum: '%.*s'\n", (i32)res.datum.mensura,
            res.datum.datum);
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}

interior i32
proba_legere_lorem (
    Piscina* piscina)
{
               Capsula* capsula;
        CapsulaFructus  res;
    constans character* expected =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit.\n"
        "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
        "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris.\n";

    printf("  proba_legere_lorem: ");

    capsula = capsula_aperire(&capsula_assets, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    res = capsula_legere(capsula, "test_assets/lorem.txt", piscina);

    si (res.status != CAPSULA_OK)
    {
        printf("FALSUM - capsula_legere status = %d\n", res.status);
        redde FALSUM;
    }

    si (res.datum.mensura != (i32)strlen(expected))
    {
        printf("FALSUM - expectavit %d bytes, obtinuit %d\n",
               (i32)strlen(expected), (i32)res.datum.mensura);
        redde FALSUM;
    }

    si (memcmp(res.datum.datum, expected, res.datum.mensura) != 0)
    {
        printf("FALSUM - contentum non congruit\n");
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}

interior i32
proba_non_inventum (
    Piscina* piscina)
{
           Capsula* capsula;
    CapsulaFructus  res;

    printf("  proba_non_inventum: ");

    capsula = capsula_aperire(&capsula_assets, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    res = capsula_legere(capsula, "non_existit.txt", piscina);

    si (res.status != CAPSULA_NON_INVENTUM)
    {
        printf("FALSUM - expectavit CAPSULA_NON_INVENTUM, obtinuit %d\n",
               res.status);
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}

interior i32
proba_iter (
    Piscina* piscina)
{
        Capsula* capsula;
    CapsulaIter  iter;
            i32  numerus;

    printf("  proba_iter: ");

    capsula = capsula_aperire(&capsula_assets, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    iter     = capsula_iter(capsula);
    numerus  = 0;

    dum (capsula_iter_proximus(&iter))
    {
        CapsulaIndexum* idx;

        idx = capsula_indexum(capsula, iter.positus - I);
        si (idx != NIHIL)
        {
            numerus++;
        }
    }

    si (numerus != II)
    {
        printf("FALSUM - expectavit 2 iterationes, obtinuit %d\n",
            numerus);
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}

interior i32
proba_indexum (
    Piscina* piscina)
{
           Capsula* capsula;
    CapsulaIndexum* idx;

    printf("  proba_indexum: ");

    capsula = capsula_aperire(&capsula_assets, piscina);

    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire rediit NIHIL\n");
        redde FALSUM;
    }

    idx = capsula_indexum(capsula, 0);

    si (idx == NIHIL)
    {
        printf("FALSUM - capsula_indexum(0) rediit NIHIL\n");
        redde FALSUM;
    }

    si (idx->via.mensura == 0)
    {
        printf("FALSUM - via vacua\n");
        redde FALSUM;
    }

    si (idx->mensura_cruda == 0)
    {
        printf("FALSUM - mensura_cruda = 0\n");
        redde FALSUM;
    }

    /* Index extra fines */
    idx = capsula_indexum(capsula, 100);

    si (idx != NIHIL)
    {
        printf("FALSUM - capsula_indexum(100) non rediit NIHIL\n");
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}


/* ========================================================================
 * MODUS DISCI
 * ========================================================================
 *
 * Ansa eadem, fons alius. Quod hic probatur non est "legit plagulam"
 * sed PROMISSUM modi evolutionis: plagula mutata statim recens legitur,
 * sine aedificatione. Probatio quae semel solum legeret id numquam
 * videret - lectio prima recens est utroque modo.
 */

#define AREA_DISCI "/tmp/probatio_capsula_disci"
#define RADIX_DISCI AREA_DISCI "/radix"

interior b32
_aream_parare (vacuum);

interior b32
_aream_parare (vacuum)
{
    si (   !filum_directorium_creare_si_necesse(AREA_DISCI)
        || !filum_directorium_creare_si_necesse(RADIX_DISCI)
        || !filum_directorium_creare_si_necesse(RADIX_DISCI "/sub"))
    {
        redde FALSUM;
    }
    si (   !filum_scribere_literis(RADIX_DISCI "/index.html", "SALVE")
        || !filum_scribere_literis(RADIX_DISCI "/vacuum.txt", "")
        || !filum_scribere_literis(RADIX_DISCI "/sub/intus.txt",
        "INTUS")
        || !filum_scribere_literis(AREA_DISCI "/extra.txt", "EXTRA"))
    {
        redde FALSUM;
    }
    redde VERUM;
}


interior i32
proba_disci_legere(Piscina* piscina)
{
           Capsula* capsula;
    CapsulaFructus  fructus;

    printf("  proba_disci_legere: ");

    si (!_aream_parare())
    {
        printf("FALSUM - area probationis parari non potuit\n");
        redde FALSUM;
    }

    capsula = capsula_aperire_e_disco(RADIX_DISCI, piscina);
    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula_aperire_e_disco rediit NIHIL\n");
        redde FALSUM;
    }

    fructus = capsula_legere(capsula, "index.html", piscina);
    si (fructus.status != CAPSULA_OK)
    {
        printf("FALSUM - status %s\n",
               capsula_status_nuntium(fructus.status));
        redde FALSUM;
    }
    si (   fructus.datum.mensura                           != (i32)V
        || memcmp(fructus.datum.datum, "SALVE", (size_t)V) != 0)
    {
        printf("FALSUM - contentum discrepat\n");
        redde FALSUM;
    }

    /* nidus quoque: ambulatio recursiva est */
    fructus = capsula_legere(capsula, "sub/intus.txt", piscina);
    si (   fructus.status        != CAPSULA_OK
        || fructus.datum.mensura != (i32)V)
    {
        printf("FALSUM - plagula nidificata non lecta\n");
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}


/* CARDO: idem capsula, plagula mutata, lectio SECUNDA recens. */
interior i32
proba_disci_recens(Piscina* piscina)
{
           Capsula* capsula;
    CapsulaFructus  prima;
    CapsulaFructus  secunda;

    printf("  proba_disci_recens: ");

    si (!_aream_parare())
    {
        printf("FALSUM - area probationis parari non potuit\n");
        redde FALSUM;
    }

    capsula = capsula_aperire_e_disco(RADIX_DISCI, piscina);
    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula NIHIL\n");
        redde FALSUM;
    }

    prima = capsula_legere(capsula, "index.html", piscina);
    si (prima.status != CAPSULA_OK || prima.datum.mensura != (i32)V)
    {
        printf("FALSUM - lectio prima fracta\n");
        redde FALSUM;
    }

    /* Disco muta - capsulam NON reaperi */
    si (!filum_scribere_literis(RADIX_DISCI "/index.html", "MUTATUM!"))
    {
        printf("FALSUM - scriptio fracta\n");
        redde FALSUM;
    }

    secunda = capsula_legere(capsula, "index.html", piscina);
    si (secunda.status != CAPSULA_OK)
    {
        printf("FALSUM - lectio secunda status %s\n",
               capsula_status_nuntium(secunda.status));
        redde FALSUM;
    }
    si (   secunda.datum.mensura != (i32)VIII
        || memcmp(secunda.datum.datum, "MUTATUM!", (size_t)VIII) != 0)
    {
        printf("FALSUM - lectio secunda VETUS est (mensura %d)"
               " - promissum modi evolutionis fractum\n",
               (integer)secunda.datum.mensura);
        redde FALSUM;
    }

    /* pristinum restituere ne probationes aliae pendeant ab ordine */
    (vacuum)filum_scribere_literis(RADIX_DISCI "/index.html", "SALVE");

    printf("VERUM\n");
    redde VERUM;
}


interior i32
proba_disci_traversalis(Piscina* piscina)
{
           Capsula* capsula;
    CapsulaFructus  fructus;

    printf("  proba_disci_traversalis: ");

    si (!_aream_parare())
    {
        printf("FALSUM - area probationis parari non potuit\n");
        redde FALSUM;
    }

    capsula = capsula_aperire_e_disco(RADIX_DISCI, piscina);
    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula NIHIL\n");
        redde FALSUM;
    }

    /* extra.txt VERE exsistit, sed EXTRA radicem: si custodia deest,
     * haec lectio succedit - ergo probatio inter 'reiectum' et 'non
     * inventum' discernere potest. */
    fructus = capsula_legere(capsula, "../extra.txt", piscina);
    si (fructus.status == CAPSULA_OK)
    {
        printf("FALSUM - TRAVERSALIS PERMISSA (../extra.txt lecta)\n");
        redde FALSUM;
    }
    si (capsula_habet(capsula, "../extra.txt"))
    {
        printf("FALSUM - habet() traversalem admittit\n");
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}


/* Filum vacuum NON est filum absens - filum_legere_totum utrique
 * mensuram ZEPHYRUM reddit, ergo custodia existentiae probanda est. */
interior i32
proba_disci_vacuum(Piscina* piscina)
{
           Capsula* capsula;
    CapsulaFructus  fructus;

    printf("  proba_disci_vacuum: ");

    si (!_aream_parare())
    {
        printf("FALSUM - area probationis parari non potuit\n");
        redde FALSUM;
    }

    capsula = capsula_aperire_e_disco(RADIX_DISCI, piscina);
    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula NIHIL\n");
        redde FALSUM;
    }

    si (!capsula_habet(capsula, "vacuum.txt"))
    {
        printf("FALSUM - filum vacuum ut absens habitum\n");
        redde FALSUM;
    }
    fructus = capsula_legere(capsula, "vacuum.txt", piscina);
    si (fructus.status != CAPSULA_OK)
    {
        printf("FALSUM - filum vacuum status %s (OK exspectatus)\n",
               capsula_status_nuntium(fructus.status));
        redde FALSUM;
    }
    si (fructus.datum.mensura != ZEPHYRUM)
    {
        printf("FALSUM - filum vacuum mensuram %d habet\n",
               (integer)fructus.datum.mensura);
        redde FALSUM;
    }

    /* et absens absens maneat */
    fructus = capsula_legere(capsula, "nusquam.txt", piscina);
    si (fructus.status == CAPSULA_OK)
    {
        printf("FALSUM - filum absens ut praesens lectum\n");
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}


interior i32
proba_disci_enumerare(Piscina* piscina)
{
    Capsula* capsula;
        i32  numerus;
        i32  i;
        b32  index_inventus;
        b32  nidus_inventus;

    printf("  proba_disci_enumerare: ");

    si (!_aream_parare())
    {
        printf("FALSUM - area probationis parari non potuit\n");
        redde FALSUM;
    }

    capsula = capsula_aperire_e_disco(RADIX_DISCI, piscina);
    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula NIHIL\n");
        redde FALSUM;
    }

    numerus = capsula_numerus(capsula);
    si (numerus != (i32)III)
    {
        printf("FALSUM - III plagulae exspectatae, %d receptae\n",
               (integer)numerus);
        redde FALSUM;
    }

    index_inventus = FALSUM;
    nidus_inventus = FALSUM;
    per (i = 0; i < numerus; i++)
    {
        CapsulaIndexum* ix = capsula_indexum(capsula, i);

        si (ix == NIHIL)
        {
            printf("FALSUM - indexum %d NIHIL\n", (integer)i);
            redde FALSUM;
        }
        /* viae RELATIVAE radici sint, sine '/' ductore - eadem forma
         * qua capsula infixa eas servat (vitrea utramque eodem modo
         * quaerit, ergo formae congruere DEBENT) */
        si (ix->via.mensura > 0 && ix->via.datum[0] == '/')
        {
            printf("FALSUM - via absoluta in indice\n");
            redde FALSUM;
        }
        si (chorda_aequalis_literis(ix->via, "index.html"))
        {
            index_inventus = VERUM;
            si (ix->mensura_cruda != (i32)V)
            {
                printf("FALSUM - mensura_cruda %d (V exspectata)\n",
                       (integer)ix->mensura_cruda);
                redde FALSUM;
            }
        }
        si (chorda_aequalis_literis(ix->via, "sub/intus.txt"))
        {
            nidus_inventus = VERUM;
        }
    }

    si (!index_inventus || !nidus_inventus)
    {
        printf("FALSUM - enumeratio plagulas exspectatas non tulit\n");
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}


/* UBI EXACTE limes photographiae iacet.
 *
 * Lectio indicem NON adit - discum adit. Ergo plagula NOVA post
 * aperturam addita LEGI potest statim (id est quod usor vere agit:
 * plagulam addere, eam ex HTML citare, recargare). Sola ENUMERATIO
 * photographiam videt. Sine hac probatione limes coniectura est. */
interior i32
proba_disci_plagula_nova(Piscina* piscina)
{
           Capsula* capsula;
    CapsulaFructus  fructus;
               i32  numerus_ante;

    printf("  proba_disci_plagula_nova: ");

    si (!_aream_parare())
    {
        printf("FALSUM - area probationis parari non potuit\n");
        redde FALSUM;
    }
    (vacuum)filum_delere(RADIX_DISCI "/postnata.txt");

    capsula = capsula_aperire_e_disco(RADIX_DISCI, piscina);
    si (capsula == NIHIL)
    {
        printf("FALSUM - capsula NIHIL\n");
        redde FALSUM;
    }
    numerus_ante = capsula_numerus(capsula);

    /* POST aperturam nascitur */
    si (!filum_scribere_literis(RADIX_DISCI "/postnata.txt", "NOVA"))
    {
        printf("FALSUM - scriptio fracta\n");
        redde FALSUM;
    }

    /* LEGI potest - semita usoris integra est */
    fructus = capsula_legere(capsula, "postnata.txt", piscina);
    si (   fructus.status        != CAPSULA_OK
        || fructus.datum.mensura != (i32)IV)
    {
        printf("FALSUM - plagula postnata legi NON potest"
               " (status %s) - limes latior est quam creditum\n",
               capsula_status_nuntium(fructus.status));
        (vacuum)filum_delere(RADIX_DISCI "/postnata.txt");
        redde FALSUM;
    }
    si (!capsula_habet(capsula, "postnata.txt"))
    {
        printf("FALSUM - habet() plagulam postnatam negat\n");
        (vacuum)filum_delere(RADIX_DISCI "/postnata.txt");
        redde FALSUM;
    }

    /* ENUMERATIO autem photographiam tenet - id est limes, et hic
     * FIGITUR ne tacite mutetur */
    si (capsula_numerus(capsula) != numerus_ante)
    {
        printf("FALSUM - enumeratio mutata est; caput contrarium"
               " dicit (photographia aperiendo)\n");
        (vacuum)filum_delere(RADIX_DISCI "/postnata.txt");
        redde FALSUM;
    }

    (vacuum)filum_delere(RADIX_DISCI "/postnata.txt");
    printf("VERUM\n");
    redde VERUM;
}


interior i32
proba_disci_radix_prava(Piscina* piscina)
{
    printf("  proba_disci_radix_prava: ");

    si (!_aream_parare())
    {
        printf("FALSUM - area probationis parari non potuit\n");
        redde FALSUM;
    }

    /* radix quae directorium non est - RECUSATIO LOQUAX, non capsula
     * vacua quae postea silentio omnia CDIV redderet */
    si (capsula_aperire_e_disco(RADIX_DISCI "/index.html",
            piscina) != NIHIL)
    {
        printf("FALSUM - filum ut radix acceptum\n");
        redde FALSUM;
    }
    si (capsula_aperire_e_disco("/tmp/nusquam_omnino_XYZ",
            piscina) != NIHIL)
    {
        printf("FALSUM - radix absens accepta\n");
        redde FALSUM;
    }
    si (capsula_aperire_e_disco(NIHIL, piscina) != NIHIL)
    {
        printf("FALSUM - radix NIHIL accepta\n");
        redde FALSUM;
    }

    printf("VERUM\n");
    redde VERUM;
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

integer
principale (vacuum)
{
    Piscina* piscina;
        i32  successus;
        i32  fallitae;

    piscina = piscina_generare_dynamicum("probatio_capsula", IV * M);

    printf("\nProbationes Capsula\n");
    printf("===================\n\n");

    successus  = 0;
    fallitae   = 0;

    si (proba_aperire(piscina)) successus++; alioquin fallitae++;
    si (proba_habet(piscina)) successus++; alioquin fallitae++;
    si (proba_legere_hello(piscina)) successus++; alioquin fallitae++;
    si (proba_legere_lorem(piscina)) successus++; alioquin fallitae++;
    si (proba_non_inventum(piscina)) successus++; alioquin fallitae++;
    si (proba_iter(piscina)) successus++; alioquin fallitae++;
    si (proba_indexum(piscina)) successus++; alioquin fallitae++;

    printf("\n  -- modus disci --\n");
    si (proba_disci_legere(piscina)) successus++; alioquin fallitae++;
    si (proba_disci_recens(piscina)) successus++; alioquin fallitae++;
    si (proba_disci_traversalis(piscina)) successus++; alioquin fallitae++;
    si (proba_disci_vacuum(piscina)) successus++; alioquin fallitae++;
    si (proba_disci_enumerare(piscina)) successus++; alioquin fallitae++;
    si (proba_disci_plagula_nova(piscina)) successus++; alioquin fallitae++;
    si (proba_disci_radix_prava(piscina)) successus++; alioquin fallitae++;

    printf("\n");
    printf("Summa: %d successus, %d fallitae\n", successus, fallitae);

    piscina_destruere(piscina);

    redde fallitae > 0 ? I : 0;
}
