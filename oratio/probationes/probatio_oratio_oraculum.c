/* probatio_oratio_oraculum.c - Oraculum treebank UD (T13)
 *
 * I.   LECTOR CoNLL-U inlinearis: sententiae, commentaria (sent_id,
 *      text), ranga 'a-b', nodus vacuus 'a.b' omissus, SpaceAfter=No,
 *      textus ex '# text' aut reconstructus (rangae superficies), CRLF;
 *      vitia cum linea: campi non X, ID non numerus, ranga mala.
 * II.  ORACULUM inlineare: 'Puella rosam amat.' contra aurum (NOUN NOUN
 *      VERB PUNCT) - tecta IV/IV, primaria III (rosam: verbum prius),
 *      lemmata III (interpunctio sine lemmate); ranga pronumque (ADJ +
 *      CCONJ sub elemento uno), ignotum (xyzzy) cum exemplo, verbum
 *      inalignatum, UPOS extra tabulam.
 * III. TREEBANKS VENDITAE (fixa/ud, CC BY-SA): CIRCSE test, LLCT dev et
 *      test - sententiae pinnatae, tabula per classem publicata,
 *      COVERAGE pinnata 'solum crescens' (permille), primaria/lemmata
 *      relata, inalignata < 2 %, fractae 0.
 * Culpa plantata: UPOS 'PROPN' e tabula sublata (nomen proprium
 * numquam tectum).
 */

#include "latina.h"
#include "credo.h"
#include "oratio_conllu.h"
#include "oratio_resolutio.h"
#include "internamentum.h"
#include "oratio_oraculum.h"
#include "oratio_registrum.h"
#include "oratio_vocabularium.h"
#include "oratio_vocabularium_la.h"
#include "oratio_glossarium.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* PINNAE coverage (permille verborum aureorum tectorum), solum crescentes;
 * 0 = nondum mensuratum (nativitas) */
/* Pinnae coverage permille, solum crescentes; causa cuiusque motus:
 * T13 regulae secundariae Latinae (CIRCSE 842 -> 937, LLCT 719 -> 889,
 * 725 -> 882); T16 2026-09-05 vocabularia ambo + regula capitalis
 * amplior (vocabulum capitale quod nullus fons Latinus novit: nomina
 * Senecae Moby nota) + signum = interpunctio ET symbolum: CIRCSE 940,
 * LLCT 895 / 887; EWT nativitas 771 / 770, post cursum II Anglicum
 * (auxiliaria, subordinantes, particulae, numeralia, interiectiones,
 * possessivum et contractiones, nomen proprium capitale) 913 / 918. */
#define CIRCSE_TECTA_PINNA     940
#define LLCT_DEV_TECTA_PINNA   895
#define LLCT_TEST_TECTA_PINNA  887
#define EWT_DEV_TECTA_PINNA    913
#define EWT_TEST_TECTA_PINNA   918
/* PRIMARIUM permille, solum crescens ab regula prima (T17, decisio
 * XXXIV): programma oratio/partes/resolutio.stml regulis II (adpositio
 * accusativum / ablativum regit) - crudum CIRCSE 674, LLCT 678 / 677,
 * EWT 566 / 569. */
/* T19k 2026-09-08 DEFINITIO MUTATA: decisio in vocabulo lectionis UNIUS
 * vana, non notata (cellula soluta) - coactae inflatae erant implentibus
 * unius lectionis (rectis): 761/762/758/913/918 -> 765/710/711/913/913;
 * primarium immotum (799/838/831/786/789) */
/* T19i 2026-09-08 determinantia certa + lex classis explicitae: 769/796/792
 * -> 799/838/831; coactae 723/694/694 -> 761/762/758 */
/* T19h 2026-09-08 ianua stricta capitum (gradus II strictus, III laxus):
 * 768/795/790 -> 769/796/792; coactae 721/692/688 -> 723/694/694 */
#define CIRCSE_PRIMARIA_PINNA    799
#define LLCT_DEV_PRIMARIA_PINNA  838
#define LLCT_TEST_PRIMARIA_PINNA 831


/* T18 (2026-09-07): regula linguae documenti + lex casus recordorum
 * Moby (in\PvAN ante In\N) + census suffragiis vocabulorum per
 * DOCUMENTUM: EWT 566/569 -> 677/679 (crudum 566 -> 600 lege casus,
 * regula +78); Latinae immotae (678/679/679). */
/* T19b (2026-09-07): regulae priorum classium XI + exceptiones II
 * (supra, a) ante regulam linguae + glossarium cursus III (priores
 * lemmatum) + listae ambiguae: CIRCSE 678 -> 727, LLCT 679/679 ->
 * 770/771, EWT 677/679 -> 753/754 (permille truncata); tecta immota.
 * T19d (2026-09-07): regulae adpositionum in UMBRAS obiecti versae
 * (lectio et implens praelatae): CIRCSE 728, LLCT 771/771, EWT immota
 * (permille ex numeris truncata, non ex centesimis). */
#define EWT_DEV_PRIMARIA_PINNA   786
#define EWT_TEST_PRIMARIA_PINNA  789
/* T19g (2026-09-08, decretum SUDOKU decisio XL): accuratio decisionum
 * COACTARUM (impletio + umbra: testimonium) permille, solum crescens -
 * decisio coacta falsa est corruptio strati. Pinnae ex permille
 * impresso portae natali 2026-09-08 (CIRCSE 721, LLCT 692/688, EWT
 * 913/918 - Latine decisiones coactae tertium INFIMUM: priores 85-98,
 * apertae 80-92; lex umbrarum ipsa 51-57, regula dativi sequens 37-41
 * in chartis, contractio Anglica 0 = artificium rangae). */
#define CIRCSE_COACTAE_PINNA     765
#define LLCT_DEV_COACTAE_PINNA   710
#define LLCT_TEST_COACTAE_PINNA  711
#define EWT_DEV_COACTAE_PINNA    913
#define EWT_TEST_COACTAE_PINNA   913

interior b32
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                chorda* exitus)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
            i8* memoria;

    exitus->datum    = NIHIL;
    exitus->mensura  = ZEPHYRUM;
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde FALSUM;
    }
    memoria = (i8*)piscina_allocare(piscina, (memoriae_index)longitudo
        + I);
    si (   memoria                                 == NIHIL
        || fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde FALSUM;
    }
    fclose(f);
    memoria[longitudo]  = ZEPHYRUM;
    exitus->datum       = memoria;
    exitus->mensura     = (i32)longitudo;
    redde VERUM;
}

interior chorda
_l (
    constans character* literae)
{
    unio {
        constans character* c;
                        i8* m;
    } u;
    chorda c;

    u.c        = literae;
    c.datum    = u.m;
    c.mensura  = (i32)strlen(literae);
    redde c;
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

interior constans OratioConlluLexema*
_lexema (
    constans OratioConlluSententia* s,
                               i32  k)
{
    redde (constans OratioConlluLexema*)xar_obtinere(s->lexemata, k);
}

interior i32
_permille (
    i32 a,
    i32 b)
{
    redde b > ZEPHYRUM ? (i32)((s64)a * (s64)1000 / (s64)b) : ZEPHYRUM;
}

interior vacuum
_tabulam_imprimere (
    constans OratioOraculumCensus* c,
               constans character* titulus)
{
    i32 i;

    imprimere("  %s: sententiae %d (fractae %d) verba %d rangae %d\n",
        titulus, (integer)c->sententiae, (integer)c->sententiae_fractae,
        (integer)c->verba, (integer)c->rangae);
    imprimere("    TECTA %d.%d%%  primaria %d.%d%%  lemmata %d.%d%%  ignota %d.%d%%  inalignata %d\n",
        (integer)(_permille(c->tecta, c->verba) / (i32)X),
        (integer)(_permille(c->tecta, c->verba) % (i32)X),
        (integer)(_permille(c->primaria, c->verba) / (i32)X),
        (integer)(_permille(c->primaria, c->verba) % (i32)X),
        (integer)(_permille(c->lemmata, c->verba) / (i32)X),
        (integer)(_permille(c->lemmata, c->verba) % (i32)X),
        (integer)(_permille(c->ignota, c->verba) / (i32)X),
        (integer)(_permille(c->ignota, c->verba) % (i32)X),
        (integer)c->inalignata);
    per (i = ZEPHYRUM; i <= (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM; i++)
    {
        constans OratioOraculumClassis* k = &c->classes[i];

        si (k->verba == ZEPHYRUM)
        {
            perge;
        }
        imprimere("    %-24s %6d  tecta %5.1f%%  primaria %5.1f%%  lemmata %5.1f%%\n",
            i < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
                ? oratio_classis_titulus((OratioClassis)i) : "(extra)",
            (integer)k->verba,
            (duplex)_permille(k->tecta, k->verba) / 10.0,
            (duplex)_permille(k->primaria, k->verba) / 10.0,
            (duplex)_permille(k->lemmata, k->verba) / 10.0);
    }
}

/* treebank venditum: legere, iudicare, tabula, pinnae */
interior vacuum
_thesaurus_arborum (
                       Piscina* piscina,
    constans OratioVocabularia* vocabularia,
      constans OratioProgramma* programma,
            constans character* radix,
            constans character* plagula,
                           i32  sententiae_exspectatae,
                           i32  pinna_permille,
                           i32  pinna_primaria,
                           i32  pinna_coactae)
{
    Piscina* p = piscina_generare_dynamicum("oraculum_treebank",
        268435456);
                   character  via[1024];
                      chorda  fons;
                         Xar* sententiae;
    OratioVocabulariumVitium  vitium;
        OratioOraculumCensus  census;
                     clock_t  ante;
                         i32  tecta_permille;

    (vacuum)piscina;
    sprintf(via, "%s/oratio/probationes/fixa/ud/%s", radix, plagula);
    si (!_plagulam_legere(p, via, &fons))
    {
        CREDO_CULPA ("treebank venditum absens");
        piscina_destruere(p);
        redde;
    }
    sententiae = oratio_conllu_legere(p, fons, &vitium);
    CREDO_NON_NIHIL (sententiae);
    si (sententiae == NIHIL)
    {
        imprimere("  %s: linea %d: %s\n", plagula,
            (integer)vitium.linea,
            vitium.causa ? vitium.causa : "?");
        piscina_destruere(p);
        redde;
    }
    CREDO_AEQUALIS_I32 (xar_numerus(sententiae),
        sententiae_exspectatae);
    oratio_oraculum_census_vacare(&census);
    ante = clock();
    /* T17: ordo crudus relatus, ordo resolutus pinnatus (primarium) */
    oratio_oraculum_census_vacare(&census);
    CREDO_VERUM (oratio_oraculum_iudicare(p, vocabularia, sententiae,
        &census));
    imprimere("  primaria cruda %.1f%%\n", census.verba > ZEPHYRUM
        ? 100.0 * (duplex)census.primaria / (duplex)census.verba : 0.0);
    /* T19a: discrepantiae primarii in ordine CRUDO - lex summae per
     * classem (numeri == tecta - primaria), ordo numeri non crescens,
     * classis sine discrepantia lista vacua; CIRCSE: 'est' aurea
     * auxiliare cum nostra prima verbum (WORDS/glossarium verbum ante
     * auxiliare ponit - data regulae T19b) */
    {
        i32 violationes  = ZEPHYRUM;
        i32 est_numerus  = ZEPHYRUM;
        i32 ultima       = (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM;
        i32 i;

        per (i = ZEPHYRUM; i <= ultima; i++)
        {
            constans OratioOraculumClassis* k = &census.classes[i];
            Xar* ds = oratio_oraculum_discrepantiae(p, &census,
                (OratioClassis)i);
            i32 summa = ZEPHYRUM;
            i32 prior = (i32)0x7fffffff;
            i32 j;

            CREDO_NON_NIHIL (ds);
            si (ds == NIHIL)
            {
                perge;
            }
            per (j = ZEPHYRUM; j < xar_numerus(ds); j++)
            {
                constans OratioOraculumDiscrepantia* d =
                    *(OratioOraculumDiscrepantia**)xar_obtinere(ds, j);

                summa = summa + d->numerus;
                si (d->numerus > prior || d->aurea != (OratioClassis)i)
                {
                    violationes = violationes + I;
                }
                prior = d->numerus;
                si (   i         == (i32)ORATIO_CLASSIS_AUXILIARE
                    && d->nostra == ORATIO_CLASSIS_VERBUM
                    && _aequalis(d->forma, "est"))
                {
                    est_numerus = d->numerus;
                }
            }
            si (summa != k->tecta - k->primaria)
            {
                imprimere("    discrepantiae %s: summa %d != tecta %d"
                    " - primaria %d\n",
                    i < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
                        ? oratio_classis_titulus((OratioClassis)i)
                        : "(extra)",
                    (integer)summa, (integer)k->tecta,
                    (integer)k->primaria);
                violationes = violationes + I;
            }
        }
        CREDO_AEQUALIS_I32 (violationes, ZEPHYRUM);
        si (strstr(plagula, "circse") != NIHIL)
        {
            imprimere("    discrepantia cruda: est auxiliare/verbum %d\n",
                (integer)est_numerus);
            CREDO_VERUM (est_numerus >= (i32)L);
        }
    }
    oratio_oraculum_census_vacare(&census);
    CREDO_VERUM (oratio_oraculum_iudicare_resolutum(p, vocabularia,
        programma,
        (s32)-I, sententiae, &census));
    _tabulam_imprimere(&census, plagula);
    imprimere("    %.0f ms\n", 1000.0 * (duplex)(clock() - ante)
        / (duplex)CLOCKS_PER_SEC);
    tecta_permille = _permille(census.tecta, census.verba);
    imprimere("    coverage %d permille (pinna %d, solum crescens)\n",
        (integer)tecta_permille, (integer)pinna_permille);
    {
        i32 primaria_permille = census.verba > ZEPHYRUM
            ? (i32)((longus)census.primaria * 1000L
                / (longus)census.verba)
            : ZEPHYRUM;

        imprimere("    primarium %d permille (pinna %d, solum"
            " crescens)\n",
            (integer)primaria_permille, (integer)pinna_primaria);
        CREDO_VERUM (primaria_permille >= pinna_primaria);
    }
    /* T19g: partitio decisionum; accuratio COACTARUM pinnata solum
     * crescens (decretum SUDOKU XL) */
    {
        i32 coactae = census.partitio_verba[ORATIO_DECISIO_IMPLETIO]
            + census.partitio_verba[ORATIO_DECISIO_UMBRA];
        i32 rectae = census.partitio_primaria[ORATIO_DECISIO_IMPLETIO]
            + census.partitio_primaria[ORATIO_DECISIO_UMBRA];
        i32 coactae_permille  = _permille(rectae, coactae);
        i32 summa             = ZEPHYRUM;
        i32 i;

        per (i = ZEPHYRUM; i < ORATIO_ORACULUM_PARTITIO_NUMERUS; i++)
        {
            imprimere("    partitio %-10s %6d (%d permille)  primaria %d permille\n",
                ORATIO_ORACULUM_TITULI_PARTITIONIS[i],
                (integer)census.partitio_verba[i],
                (integer)_permille(census.partitio_verba[i],
                census.verba),
                (integer)_permille(census.partitio_primaria[i],
                    census.partitio_verba[i]));
            summa = summa + census.partitio_verba[i];
        }
        imprimere("    coactae %d, accuratio %d permille (pinna %d, solum"
            " crescens)\n", (integer)coactae, (integer)coactae_permille,
            (integer)pinna_coactae);
        /* lex summae: verbum aureum alignatum quodque genus unum */
        CREDO_AEQUALIS_I32 (summa, census.verba - census.inalignata);
        /* auctores: summa verborum == decisa (praelatio + impletio +
         * umbra); tabula relata (quae regula decisiones falsas facit) */
        {
            Xar* auctores = oratio_oraculum_auctores(p, &census);
            i32  decisa   =
                census.partitio_verba[ORATIO_DECISIO_PRAELATIO]
                + coactae;
            i32 summa_auctorum = ZEPHYRUM;
            i32 j;

            CREDO_NON_NIHIL (auctores);
            per (j = ZEPHYRUM; auctores != NIHIL
                 && j < xar_numerus(auctores); j++)
            {
                constans OratioOraculumAuctor* a =
                    *(OratioOraculumAuctor**)xar_obtinere(auctores, j);

                summa_auctorum = summa_auctorum + a->verba;
                imprimere("    auctor %-44.*s %6d  primaria %d permille"
                    "  vicina %d %d  remota %d %d\n",
                    (integer)a->titulus.mensura,
                    (constans character*)a->titulus.datum,
                    (integer)a->verba,
                    (integer)_permille(a->primaria, a->verba),
                    (integer)a->vicina,
                    (integer)_permille(a->vicina_primaria, a->vicina),
                    (integer)a->remota,
                    (integer)_permille(a->remota_primaria, a->remota));
            }
            CREDO_AEQUALIS_I32 (summa_auctorum, decisa);
            /* T19g bis: errata auctoris - summa numerorum == verba -
             * primaria eius; errata prima III auctorum II maximorum relata
             * (forma, aurea, nostra, socius) */
            {
                i32 violationes_erratorum = ZEPHYRUM;

                per (j = ZEPHYRUM; auctores != NIHIL
                     && j < xar_numerus(auctores); j++)
                {
                    constans OratioOraculumAuctor* a =
                        *(OratioOraculumAuctor**)xar_obtinere(auctores,
                        j);
                    Xar* es = oratio_oraculum_errata(p, &census,
                        a->titulus);
                    i32 summa_erratorum = ZEPHYRUM;
                    i32 m;

                    CREDO_NON_NIHIL (es);
                    si (es == NIHIL)
                    {
                        perge;
                    }
                    per (m = ZEPHYRUM; m < xar_numerus(es); m++)
                    {
                        constans OratioOraculumErratum* d =
                            *(OratioOraculumErratum**)xar_obtinere(es,
                            m);

                        summa_erratorum = summa_erratorum + d->numerus;
                        si (j < (i32)II && m < (i32)III)
                        {
                            imprimere("      erratum %-.32s: %-14.*s aurea %s"
                                " nostra %s socius %.*s @%d %d\n",
                                (constans character*)a->titulus.datum,
                                (integer)d->forma.mensura,
                                (constans character*)d->forma.datum,
                                oratio_classis_titulus(d->aurea),
                                (i32)d->nostra
                                    < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
                                    ? oratio_classis_titulus(d->nostra)
                                    : "(nulla)",
                                (integer)d->socius.mensura,
                                d->socius.mensura > ZEPHYRUM
                                    ? (constans character*)d->socius.datum
                                    : "",
                                (integer)d->distantia,
                                (integer)d->numerus);
                        }
                    }
                    si (summa_erratorum != a->verba - a->primaria)
                    {
                        violationes_erratorum = violationes_erratorum
                            + I;
                    }
                }
                CREDO_AEQUALIS_I32 (violationes_erratorum, ZEPHYRUM);
            }
        }
        CREDO_VERUM (coactae > ZEPHYRUM);
        CREDO_VERUM (coactae_permille >= pinna_coactae);
    }
        /* T18: lingua documenti censa = lingua thesauri (en_ ewt Anglica,
     * cetera Latina); census sententiarum: Latinae nullae Anglicae */
    CREDO_AEQUALIS_S32 (census.lingua_documenti,
        strstr(plagula, "en_") != NIHIL ? (s32)ORATIO_LINGUA_ANGLICA
        : (s32)ORATIO_LINGUA_LATINA);
    si (strstr(plagula, "en_") == NIHIL)
    {
        CREDO_AEQUALIS_I32 (census.sententiae_linguae[ORATIO_LINGUA_ANGLICA],
            ZEPHYRUM);
    }
    CREDO_AEQUALIS_I32 (census.sententiae_fractae, ZEPHYRUM);
    CREDO_VERUM (census.inalignata * (i32)L < census.verba);   /* < II % */
    CREDO_VERUM (census.verba > (i32)10000);
    CREDO_VERUM (tecta_permille >= pinna_permille);
    piscina_destruere(p);
}

s32
principale (vacuum)
{
                  Piscina* piscina;
       constans character* radix;
        OratioVocabularia  vocabularia;
 OratioVocabulariumVitium  vitium;
      InternamentumChorda* intern;
          OratioProgramma* programma;

    piscina = piscina_generare_dynamicum("probatio_oratio_oraculum",
        536870912);
    credo_aperire(piscina);
    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    CREDO_VERUM (oratio_vocabularia_onerare(piscina, radix,
        &vocabularia,
        &vitium));
    si (vocabularia.la == NIHIL || vocabularia.en == NIHIL)
    {
        credo_imprimere_compendium();
        redde I;
    }
    /* T17: programma resolutionis necessarium (pinnae primarii) */
    intern = internamentum_creare(piscina);
    CREDO_NON_NIHIL (intern);
    programma = intern == NIHIL ? NIHIL
        : oratio_resolutio_programma_onerare(piscina, intern, radix,
        &vitium);
    CREDO_NON_NIHIL (programma);
    si (programma == NIHIL)
    {
        imprimere("  programma: %s:%d %s\n",
            vitium.plagula ? vitium.plagula
            : "?", (integer)vitium.linea,
            vitium.causa ? vitium.causa : "-");
        credo_imprimere_compendium();
        redde I;
    }

    imprimere("\n--- I. Lector CoNLL-U ---\n");
    {
        Xar* s = oratio_conllu_legere(piscina, _l(
            "# sent_id = a-1\n"
            "# text = Puella rosam amat.\n"
            "1\tPuella\tpuella\tNOUN\t_\tCase=Nom\t3\tnsubj\t_\t_\n"
            "2\trosam\trosa\tNOUN\t_\t_\t3\tobj\t_\t_\n"
            "3\tamat\tamo\tVERB\t_\t_\t0\troot\t_\tSpaceAfter=No\n"
            "3.1\tzzz\tzzz\tX\t_\t_\t_\t_\t_\t_\n"
            "4\t.\t.\tPUNCT\t_\t_\t3\tpunct\t_\t_\n"
            "\n"
            "\n"
            "# sent_id = a-2\r\n"
            "1-2\tpronumque\t_\t_\t_\t_\t_\t_\t_\t_\r\n"
            "1\tpronum\tpronus\tADJ\t_\t_\t3\tadvcl\t_\t_\r\n"
            "2\tque\tque\tCCONJ\t_\t_\t3\tcc\t_\t_\r\n"
            "3\tabstulit\taufero\tVERB\t_\t_\t0\troot\t_\tSpaceAfter=No\r\n"
            "4\t.\t.\tPUNCT\t_\t_\t3\tpunct\t_\t_\r\n"), &vitium);
        constans OratioConlluSententia* a;
        constans OratioConlluSententia* b;
                                chorda  textus;

        CREDO_NON_NIHIL (s);
        CREDO_AEQUALIS_I32 (s ? xar_numerus(s) : ZEPHYRUM, (i32)II);
        si (s == NIHIL || xar_numerus(s) != (i32)II)
        {
            credo_imprimere_compendium();
            redde I;
        }
        a = (constans OratioConlluSententia*)xar_obtinere(s, ZEPHYRUM);
        b = (constans OratioConlluSententia*)xar_obtinere(s, I);
        CREDO_VERUM (_aequalis(a->id, "a-1"));
        CREDO_VERUM (_aequalis(a->textus, "Puella rosam amat."));
        CREDO_AEQUALIS_I32 (xar_numerus(a->lexemata), (i32)IV);   /* 3.1 omissus */
        CREDO_VERUM (_aequalis(_lexema(a, ZEPHYRUM)->forma, "Puella"));
        CREDO_VERUM (_aequalis(_lexema(a, ZEPHYRUM)->lemma, "puella"));
        CREDO_VERUM (_aequalis(_lexema(a, ZEPHYRUM)->upos, "NOUN"));
        CREDO_VERUM (_aequalis(_lexema(a, ZEPHYRUM)->feats,
            "Case=Nom"));
        CREDO_VERUM (_aequalis(_lexema(a, ZEPHYRUM)->deprel, "nsubj"));
        CREDO_AEQUALIS_I32 (_lexema(a, ZEPHYRUM)->a, I);
        CREDO_FALSUM (_lexema(a, ZEPHYRUM)->ranga);
        CREDO_VERUM (_lexema(a, ZEPHYRUM)->spatium_post);
        CREDO_FALSUM (_lexema(a, (i32)II)->spatium_post);
        CREDO_AEQUALIS_I32 (_lexema(a, (i32)III)->linea, (i32)VII);
        textus = oratio_conllu_textus(piscina, a);
        CREDO_VERUM (_aequalis(textus, "Puella rosam amat."));
        /* b: sine '# text', CRLF, ranga */
        CREDO_VERUM (_aequalis(b->id, "a-2"));
        CREDO_AEQUALIS_I32 (b->textus.mensura, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(b->lexemata), (i32)V);
        CREDO_VERUM (_lexema(b, ZEPHYRUM)->ranga);
        CREDO_AEQUALIS_I32 (_lexema(b, ZEPHYRUM)->a, I);
        CREDO_AEQUALIS_I32 (_lexema(b, ZEPHYRUM)->b, (i32)II);
        CREDO_VERUM (_aequalis(_lexema(b, ZEPHYRUM)->forma,
            "pronumque"));
        CREDO_VERUM (_aequalis(_lexema(b, ZEPHYRUM)->upos, "_"));
        CREDO_VERUM (_aequalis(_lexema(b, I)->forma, "pronum"));
        textus = oratio_conllu_textus(piscina, b);
        CREDO_VERUM (_aequalis(textus, "pronumque abstulit."));
        CREDO_VERUM (oratio_conllu_spatium_post(_l("_")));
        CREDO_FALSUM (oratio_conllu_spatium_post(_l("SpaceAfter=No")));
        CREDO_FALSUM (oratio_conllu_spatium_post(_l("X=1|SpaceAfter=No|Y=2")));
        CREDO_VERUM (oratio_conllu_spatium_post(_l("NoSpaceAfter=No")));
        /* vitia cum linea */
        CREDO_NIHIL (oratio_conllu_legere(piscina, _l(
            "1\tx\tx\tNOUN\t_\t_\t0\troot\t_\n"), &vitium));
        CREDO_AEQUALIS_I32 (vitium.linea, I);
        CREDO_VERUM (strcmp(vitium.causa, "campi non X") == ZEPHYRUM);
        CREDO_NIHIL (oratio_conllu_legere(piscina, _l(
            "# c\n1\tx\tx\tNOUN\t_\t_\t0\troot\t_\t_\n"
            "y\tx\tx\tNOUN\t_\t_\t0\troot\t_\t_\n"), &vitium));
        CREDO_AEQUALIS_I32 (vitium.linea, (i32)III);
        CREDO_VERUM (strcmp(vitium.causa, "ID non numerus")
            == ZEPHYRUM);
        CREDO_NIHIL (oratio_conllu_legere(piscina, _l(
            "3-2\tx\t_\t_\t_\t_\t_\t_\t_\t_\n"), &vitium));
        CREDO_VERUM (strcmp(vitium.causa, "ranga mala") == ZEPHYRUM);
        /* fons vacuus: sententiae nullae, non vitium */
        s = oratio_conllu_legere(piscina, _l("\n\n# c\n\n"), &vitium);
        CREDO_NON_NIHIL (s);
        CREDO_AEQUALIS_I32 (s ? xar_numerus(s) : (i32)-I, ZEPHYRUM);
    }

    imprimere("\n--- II. Oraculum inlineare ---\n");
    {
        Xar* s = oratio_conllu_legere(piscina, _l(
            "# text = Puella rosam amat.\n"
            "1\tPuella\tpuella\tNOUN\t_\t_\t3\tnsubj\t_\t_\n"
            "2\trosam\trosa\tNOUN\t_\t_\t3\tobj\t_\t_\n"
            "3\tamat\tamo\tVERB\t_\t_\t0\troot\t_\tSpaceAfter=No\n"
            "4\t.\t.\tPUNCT\t_\t_\t3\tpunct\t_\t_\n"), &vitium);
        OratioOraculumCensus c;

        CREDO_NON_NIHIL (s);
        oratio_oraculum_census_vacare(&c);
        CREDO_VERUM (oratio_oraculum_iudicare(piscina, &vocabularia, s,
            &c));
        _tabulam_imprimere(&c, "Puella rosam amat.");
        CREDO_AEQUALIS_I32 (c.sententiae, I);
        CREDO_AEQUALIS_I32 (c.sententiae_fractae, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (c.verba, (i32)IV);
        CREDO_AEQUALIS_I32 (c.tecta, (i32)IV);
        CREDO_AEQUALIS_I32 (c.primaria, (i32)III);   /* rosam: verbum prius */
        CREDO_AEQUALIS_I32 (c.lemmata, (i32)III);    /* '.' sine lemmate nostro */
        CREDO_AEQUALIS_I32 (c.ignota, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (c.inalignata, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_SUBSTANTIVUM].verba,
            (i32)II);
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_SUBSTANTIVUM].tecta,
            (i32)II);
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_SUBSTANTIVUM].primaria,
            I);
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_VERBUM].tecta, I);
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_VERBUM].lemmata,
            I);
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_INTERPUNCTIO].tecta,
            I);
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_INTERPUNCTIO].lemmata,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_SUBSTANTIVUM]
            .numerus_exemplorum, ZEPHYRUM);

        /* ranga, ignotum, inalignatum, UPOS extra tabulam */
        s = oratio_conllu_legere(piscina, _l(
            "# text = pronumque abstulit xyzzy\n"
            "1-2\tpronumque\t_\t_\t_\t_\t_\t_\t_\t_\n"
            "1\tpronum\tpronus\tADJ\t_\t_\t3\tadvcl\t_\t_\n"
            "2\tque\tque\tCCONJ\t_\t_\t3\tcc\t_\t_\n"
            "3\tabstulit\taufero\tVERB\t_\t_\t0\troot\t_\t_\n"
            "4\txyzzy\txyzzy\tNOUN\t_\t_\t3\tobj\t_\t_\n"
            "5\tzzz\tzzz\tADV\t_\t_\t3\tadvmod\t_\t_\n"
            "6\tabstulit\taufero\tFOO\t_\t_\t3\tdep\t_\t_\n"), &vitium);
        CREDO_NON_NIHIL (s);
        oratio_oraculum_census_vacare(&c);
        CREDO_VERUM (oratio_oraculum_iudicare(piscina, &vocabularia, s,
            &c));
        _tabulam_imprimere(&c, "pronumque abstulit xyzzy");
        CREDO_AEQUALIS_I32 (c.rangae, I);
        CREDO_AEQUALIS_I32 (c.verba, (i32)VI);   /* pronum que abstulit xyzzy zzz abstulit */
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_ADIECTIVUM].tecta,
            I);
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_ADIECTIVUM].primaria,
            ZEPHYRUM);   /* tackon prior */
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_CONIUNCTIO_COORDINANS]
            .tecta, I);
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_VERBUM].tecta, I);
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_VERBUM].primaria,
            I);
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_VERBUM].lemmata,
            I);
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_SUBSTANTIVUM].tecta,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_SUBSTANTIVUM].ignota,
            I);
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_SUBSTANTIVUM]
            .numerus_exemplorum, I);
        CREDO_VERUM (_aequalis(c.classes[ORATIO_CLASSIS_SUBSTANTIVUM]
            .exempla[ZEPHYRUM].forma, "xyzzy"));
        CREDO_VERUM (_aequalis(c.classes[ORATIO_CLASSIS_SUBSTANTIVUM]
            .exempla[ZEPHYRUM].classes, "ignotum"));
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_ADVERBIUM].inalignata,
            I);
        CREDO_AEQUALIS_I32 (c.inalignata, (i32)II);   /* zzz, abstulit II (cursor post) */
        CREDO_AEQUALIS_I32 (c.classes[ORATIO_CLASSIS_NUMERUS_CLASSIUM].verba,
            I);   /* FOO */
        CREDO_AEQUALIS_I32 ((i32)oratio_oraculum_classis_ex_upos(_l("PROPN")),
            (i32)ORATIO_CLASSIS_NOMEN_PROPRIUM);
        CREDO_AEQUALIS_I32 ((i32)oratio_oraculum_classis_ex_upos(_l("SCONJ")),
            (i32)ORATIO_CLASSIS_CONIUNCTIO_SUBORDINANS);
        CREDO_AEQUALIS_I32 ((i32)oratio_oraculum_classis_ex_upos(_l("_")),
            (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM);
    }

    imprimere("\n--- III. Treebanks venditae (CC BY-SA) ---\n");
    _thesaurus_arborum(piscina, &vocabularia, programma, radix,
        "la_circse-ud-test.conllu",
        (i32)893,
        (i32)CIRCSE_TECTA_PINNA,
        (i32)CIRCSE_PRIMARIA_PINNA,
        (i32)CIRCSE_COACTAE_PINNA);
    _thesaurus_arborum(piscina, &vocabularia, programma, radix,
        "la_llct-ud-dev.conllu",
        (i32)850,
        (i32)LLCT_DEV_TECTA_PINNA,
        (i32)LLCT_DEV_PRIMARIA_PINNA,
        (i32)LLCT_DEV_COACTAE_PINNA);
    _thesaurus_arborum(piscina, &vocabularia, programma, radix,
        "la_llct-ud-test.conllu",
        (i32)884,
        (i32)LLCT_TEST_TECTA_PINNA,
        (i32)LLCT_TEST_PRIMARIA_PINNA,
        (i32)LLCT_TEST_COACTAE_PINNA);
    _thesaurus_arborum(piscina, &vocabularia, programma, radix,
        "en_ewt-ud-dev.conllu",
        (i32)2001,
        (i32)EWT_DEV_TECTA_PINNA,
        (i32)EWT_DEV_PRIMARIA_PINNA,
        (i32)EWT_DEV_COACTAE_PINNA);
    _thesaurus_arborum(piscina, &vocabularia, programma, radix,
        "en_ewt-ud-test.conllu",
        (i32)2077,
        (i32)EWT_TEST_TECTA_PINNA,
        (i32)EWT_TEST_PRIMARIA_PINNA,
        (i32)EWT_TEST_COACTAE_PINNA);

    imprimere("\n");
    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        credo_claudere();
        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
