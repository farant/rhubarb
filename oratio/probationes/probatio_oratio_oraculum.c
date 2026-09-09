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
/* T22 (orthographia media): chartae tectae 897/888 -> 922/913 */
#define CIRCSE_TECTA_PINNA     941
#define LLCT_DEV_TECTA_PINNA   922
#define LLCT_TEST_TECTA_PINNA  913
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
/* T21 (enclitica scissa): Seneca 799 -> 834 (rangae CCCII -> XXXI:
 * -que elementum proprium), chartae 838/831 -> 840/833 (lucanus
 * glossario, utraque) */
/* T22 (orthographia media, contractus IV): chartae 840/833 -> 863/856
 * (formae ignotae MDCXXII -> MIX, MDCLXIV -> MLXI), Seneca 834 -> 835
 * (mecum temet glossario) */
#define CIRCSE_PRIMARIA_PINNA    835
#define LLCT_DEV_PRIMARIA_PINNA  864   /* T30 c: variantes mediae formis notis 863 -> 865; T30 e: 865 -> 864 CAUSA NOMINATA (decisio Frani): ordo gradus laxi genitivo primo - regulae genitivi ligationes remotas a nominativi capiunt, classis V verba */
#define LLCT_TEST_PRIMARIA_PINNA 858   /* T30 c: 856 -> 858 */


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
/* T21: Seneca 765 -> 764 CAUSA NOMINATA - encliticum elementum inter
 * hospitem et vocabulum proximum stat (senatus que populus), regulae
 * capitis strictae (-proximo) paria V pauciora vident, laxae ea
 * minore accuratione capiunt; cursor strictus encliticis perspicuus =
 * mensura proxima (T21b), machinae exemplarium res */
/* T23 CASUS (2026-09-08): casus lectionis primae ubi classis recta et
 * aurum casum fert - mensura quam primarium non videt; EWT relata (0) */
/* T24 (prior casuum post regulas, verbis apertis): 609/565/567 ->
 * 645/622/637; primarium et coactae immota */
/* T26 LIGATIO (2026-09-08): praecisio ligationum nostrarum contra capita
 * aurea (umbra capitis: socius == caput carrier; umbra obiecti
 * adpositionis/particulae/auxiliaris: socius == caput carrier (UD: casus
 * et mark a nomine/verbo pendent); obiecti verbi: carrier == caput
 * socii), solum crescens; revocatio relata */
/* T27 (2026-09-08): gradus strictus regularum capitis UNITUS (XIV -> II,
 * $cas utrimque, EXEMPLAR quaesitio="omnes", politica parium socium
 * proximum eligens): casus 645/622/637 -> 662/644/665, ligatio
 * 431/364/377 -> 437/368/378, coactae Senecae 764 -> 765; coactae
 * chartarum 712 -> 711 et 713 -> 712 CAUSA NOMINATA (decisio Frani):
 * gradus legis umbrarum crescit (ordo lectionis victae socium suum
 * decernit; recusatio eius mensurata et peior), numerator paene idem
 * (+3 / -3 verba); thesauris IX nihil aliud descendit, scrinium casu
 * et ligatione ubique ascendit.
 * T27 b (2026-09-09): EMENDATIO_SEMEL_PER_SENTENTIAM (casus classis
 * explicitae semel per sententiam emendatus - prima vincit trans gradus):
 * ligatio 437/368/378 -> 441/386/394, casus chartarum 644/665 ->
 * 654/671, scrinium casus +4..+7 et ligatio +10..+12; casus Senecae
 * 662 -> 660 CAUSA NOMINATA (decisio Frani, IX verba).
 * T28 (2026-09-09): LEX I CLAUSULAE gradu II (ligatio trans clausulas
 * recusata, relationes omnes, strata pura) columna ligationis iudicata:
 * ligatio 441/386/394 -> 462/398/412, EWT dev 815 -> 817, coactae
 * 765/711/712 -> 769/713/717, casus Senecae 660 -> 666; casus chartarum
 * dev 654 -> 653 CAUSA NOMINATA (decisio Frani, VII verba). */
#define CIRCSE_LIGATIONIS_PINNA    463   /* T27: 431 -> 437; T27 b: 437 -> 441; T28: 441 -> 462; T30 b: 462 -> 463 */
#define LLCT_DEV_LIGATIONIS_PINNA  411   /* T27: 364 -> 368; T27 b: 368 -> 386; T28: 386 -> 398; T30 c: 398 -> 413; T30 e: 413 -> 411 CAUSA NOMINATA (decisio Frani): arcus remoti genitivi minus saepe caput aureum */
#define LLCT_TEST_LIGATIONIS_PINNA 427   /* T27: 377 -> 378; T27 b: 378 -> 394; T28: 394 -> 412; T30 c: 412 -> 428; T30 e: 428 -> 427 CAUSA NOMINATA (decisio Frani) */
#define EWT_DEV_LIGATIONIS_PINNA   817   /* T28: 815 -> 817 */
#define EWT_TEST_LIGATIONIS_PINNA  826
#define CIRCSE_CASUUM_PINNA      670   /* T27: 645 -> 662; T27 b: 662 -> 660 CAUSA NOMINATA; T28: 660 -> 666; T30 e: 666 -> 670 */
#define CIRCSE_NUMERI_PINNA   920   /* T29: nativitas 914; T30 b: ordo frequentiae 914 -> 917; T30 e: 917 -> 920 */
#define CIRCSE_GENERIS_PINNA  908   /* T29: nativitas 837; T30 a: 837 -> 847 valores aurei plures (Fem,Masc), 847 -> 897 conventio communis (decisio Frani); T30 b: 897 -> 907 ordo frequentiae; T30 d: 907 -> 908 gerundivum masculinum demotum */
#define CIRCSE_VOCIS_PINNA          982   /* T30 d: nativitas (raritas passivi secundae; 930 ante) */
#define CIRCSE_FORMAE_VERBI_PINNA   978   /* T30 d: nativitas (925 ante T30 c; conventio Part/Ger = gerundivum + raritas) */
#define LLCT_DEV_CASUUM_PINNA    665   /* T27: 622 -> 644; T27 b: 644 -> 654; T28: 654 -> 653 CAUSA NOMINATA; T30 c: 653 -> 662; T30 e: 662 -> 665 */
#define LLCT_DEV_NUMERI_PINNA   928   /* T29: nativitas 928; T30 c: 928 -> 925 CAUSA NOMINATA (decisio Frani): gradus laxus litteralis nominativo primo paria pluralia sanctae/ecclesiae ligat; T30 e: genitivo primo 925 -> 928 */
#define LLCT_DEV_GENERIS_PINNA  928   /* T29: nativitas 813; T30 a: conventio communis 813 -> 890; T30 b: ordo frequentiae 890 -> 895; T30 c: 895 -> 913; T30 d: 913 -> 929; T30 e: 929 -> 928 CAUSA NOMINATA (decisio Frani) */
#define LLCT_DEV_VOCIS_PINNA          999   /* T30 d: nativitas (867 ante) */
#define LLCT_DEV_FORMAE_VERBI_PINNA   986   /* T30 d: nativitas (840 ante) */
#define LLCT_TEST_CASUUM_PINNA   692   /* T27: 637 -> 665; T27 b: 665 -> 671; T30 c: 671 -> 685; T30 e: 685 -> 692 */
#define LLCT_TEST_NUMERI_PINNA   939   /* T29: nativitas 933; T30 c: 933 -> 932 CAUSA NOMINATA (decisio Frani), ut dev; T30 e: 932 -> 939 */
#define LLCT_TEST_GENERIS_PINNA  930   /* T29: nativitas 810; T30 a: conventio communis 810 -> 885; T30 b: ordo frequentiae 885 -> 892; T30 c: 892 -> 912; T30 d: 912 -> 930 */
#define LLCT_TEST_VOCIS_PINNA          999   /* T30 d: nativitas (870 ante) */
#define LLCT_TEST_FORMAE_VERBI_PINNA   989   /* T30 d: nativitas (832 ante) */
#define EWT_DEV_CASUUM_PINNA     0
#define EWT_DEV_NUMERI_PINNA   0   /* T29 */
#define EWT_DEV_GENERIS_PINNA  0   /* T29 */
#define EWT_DEV_VOCIS_PINNA          0   /* T30 d */
#define EWT_DEV_FORMAE_VERBI_PINNA   0   /* T30 d */
#define EWT_TEST_CASUUM_PINNA    0
#define EWT_TEST_NUMERI_PINNA   0   /* T29 */
#define EWT_TEST_GENERIS_PINNA  0   /* T29 */
#define EWT_TEST_VOCIS_PINNA          0   /* T30 d */
#define EWT_TEST_FORMAE_VERBI_PINNA   0   /* T30 d */
#define CIRCSE_COACTAE_PINNA     769   /* T27: 764 -> 765; T28: 765 -> 769 */
#define LLCT_DEV_COACTAE_PINNA   718   /* T22: 710 -> 712; T27: 712 -> 711 CAUSA NOMINATA; T28: 711 -> 713; T30 c: 713 -> 719; T30 e: 719 -> 718 CAUSA NOMINATA (decisio Frani) */
#define LLCT_TEST_COACTAE_PINNA  727   /* T22: 711 -> 713; T27: 713 -> 712 CAUSA NOMINATA; T28: 712 -> 717; T30 c: 717 -> 728; T30 e: 728 -> 727 CAUSA NOMINATA (decisio Frani) */
#define EWT_DEV_COACTAE_PINNA    913
#define EWT_TEST_COACTAE_PINNA   913
/* T20a (2026-09-08, decisio XLVII): CLAUSULAE - PURITAS verborum
 * positorum (clausula aurea == maior clausulae nostrae; permille,
 * solum crescens) et PARES (sententiae quarum numerus clausularum
 * nostrarum == aurearum; permille). Coactio == M (strata I-III
 * verbum quodque ponunt; minor = mutatio definitionis nominanda).
 * Pinnae ex permille impresso portae natali 2026-09-08 (strata I-III:
 * semen 890/957/953, extentum 759/857/887, clausura 491/650/657 -
 * infimum ut praedictum -, unica 740/984/986; Anglica unica fere
 * tota 727/738; residuum = verbum finitum alterum SINE semine:
 * asyndeton, T20c). T20a bis (eodem die): SCISSIO verbi (stratum V)
 * 738/829/864 -> 821/847/881, pares 518/740/771 -> 653/762/788.
 * T20a ter (eodem die): variationes clausurae IV (qui supra, copula,
 * coordinans ante semen, certitudo Latina) 821/847/881 ->
 * 850/870/893; EWT 726/739 -> 724/737 DEFINITIO MUTATA: documentum
 * Anglicum lectionibus Latinis non amplius seminatur (it sit do verba
 * Latina finita, si cum semina falsa) - parentheses solae manent;
 * pares relatae, non pinnatae (concordia parium pinna). T20a quater:
 * 'id est' formula + nomen post verbum: concordia 819/811/798 ->
 * 820/816/802. T20a quinquies: una cum, cum per subiunctivum aut
 * ablativo sequente, quantus relativum: 820/826/809; puritas relata.
 */
#define CIRCSE_PURITAS_PINNA      850
#define LLCT_DEV_PURITAS_PINNA    870
#define LLCT_TEST_PURITAS_PINNA   893
#define EWT_DEV_PURITAS_PINNA     724
#define EWT_TEST_PURITAS_PINNA    737
/* T20a ter (2026-09-08): CONCORDIA PARIUM (Rand super paria verborum
 * intra sententiam; puritas sola segmentationem nimiam praemiat) -
 * mensura decisionum variationum, pinnata solum crescens. T20a sexies
 * (Fran): pinna = NUMERUS parium concordantium EXACTUS, non permille
 * - permille truncatum XXXIX paria Senecae modo ostendebat modo
 * celabat pro loco limitis (820.4 celat, 820.0 ostendit); numerus
 * exactus fortunam tollit et regulam (thesaurus nullus cadat)
 * strictissime legit. Paria tota per thesaurum relata. */
/* T21: paria universa crescunt (enclitica alignata: Seneca CXXV.XXVII
 * -> CXXXII.CCCIII), concordantia cum eis */
#define CIRCSE_CONCORDIA_PINNA    109193
#define LLCT_DEV_CONCORDIA_PINNA  439845   /* T22: verba recuperata semina et verba certa mutant */
#define LLCT_TEST_CONCORDIA_PINNA 445916
#define EWT_DEV_CONCORDIA_PINNA   105006
#define EWT_TEST_CONCORDIA_PINNA  107375
#define CIRCSE_PARES_PINNA       653
#define LLCT_DEV_PARES_PINNA     762
#define LLCT_TEST_PARES_PINNA    788
#define EWT_DEV_PARES_PINNA      638
#define EWT_TEST_PARES_PINNA     659


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
                                                      i32  pinna_coactae,
                                                      i32  pinna_casuum,
                           i32  pinna_numeri,
                           i32  pinna_generis,
                           i32  pinna_vocis,
                           i32  pinna_formae_verbi,
                           i32  pinna_ligationis,
                           i32  pinna_puritatis,
                           i32  pinna_parium,
                           i32  pinna_concordiae)
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
        /* T23 CASUS: pinnata solum crescens ubi populus adest (EWT:
         * pronomina sola casum ferunt, lectiones Latinae rarae - relata) */
        {
            i32 casus_permille = _permille(census.casus_recti,
                census.casus_verba);

            imprimere("    casus %d/%d = %d permille (pinna %d, solum"
                " crescens)\n", (integer)census.casus_recti,
                (integer)census.casus_verba, (integer)casus_permille,
                (integer)pinna_casuum);
                        si (pinna_casuum > ZEPHYRUM)
                        {
                CREDO_VERUM (census.casus_verba > ZEPHYRUM);
                CREDO_VERUM (casus_permille >= pinna_casuum);
                        }
        }
        /* T29 NOTAE: numerus et genus pinnati solum crescentes, cetera
         * relata (persona, modus, vox, forma verbi, tempus) */
        /* T22 b (T30 c): dialectus documenti ex censu thesauri toti -
         * chartae MEDIUS, ceteri CLASSICUS */
        {
            s32 speratus = strstr(plagula, "llct") != NIHIL
                ? (s32)ORATIO_DIALECTUS_MEDIUS
                : (s32)ORATIO_DIALECTUS_CLASSICUS;

            imprimere("    dialectus %s (recuperata %d de %d)\n",
                ORATIO_TITULI_DIALECTORUM[(i32)census.dialectus],
                (integer)census.dialectus_recuperata,
                (integer)census.dialectus_verba);
            CREDO_AEQUALIS_S32 (census.dialectus, speratus);
        }
        {
            i32 k;

            per (k = ZEPHYRUM; k < ORATIO_ORACULUM_NOTAE; k++)
            {
                i32 permille = _permille(census.notae_recti[k],
                    census.notae_verba[k]);
                i32 pinna = k == ZEPHYRUM ? pinna_numeri
                    : k == I ? pinna_generis
                    : k == (i32)IV ? pinna_vocis
                    : k == (i32)V ? pinna_formae_verbi : ZEPHYRUM;

                imprimere("    nota %-12s %d/%d = %d permille (pinna %d)"
                    "  conventione %d\n",
                    oratio_oraculum_nota_titulus(k),
                    (integer)census.notae_recti[k],
                    (integer)census.notae_verba[k], (integer)permille,
                    (integer)pinna,
                    (integer)census.notae_conventione[k]);
                si (pinna > ZEPHYRUM)
                {
                    CREDO_VERUM (census.notae_verba[k] > ZEPHYRUM);
                    CREDO_VERUM (permille >= pinna);
                }
                /* T30: lex erratorum notae - summa numerorum == verba -
                 * recti accidentis; generis errata prima III relata
                 * (forma, aurea, nostra, auctor, attingibile) */
                {
                    Xar* es = oratio_oraculum_errata_notarum(p, &census,
                        (s32)k);
                    i32 summa = ZEPHYRUM;
                    i32 m;

                    CREDO_NON_NIHIL (es);
                    per (m = ZEPHYRUM; es != NIHIL
                        && m < xar_numerus(es);
                         m++)
                    {
                        constans OratioOraculumErratumNotae* d =
                            *(OratioOraculumErratumNotae**)xar_obtinere(
                            es, m);

                        summa = summa + d->numerus;
                        si (k == I && m < (i32)III)
                        {
                            constans character* aurea =
                                oratio_oraculum_nota_valor_titulus(k,
                                d->aurea);
                            constans character* nostra =
                                oratio_oraculum_nota_valor_titulus(k,
                                d->nostra);

                            imprimere("      erratum %-14.*s aurea %-10s"
                                " nostra %-10s %-28.*s %s %d\n",
                                (integer)d->forma.mensura,
                                (constans character*)d->forma.datum,
                                aurea != NIHIL ? aurea : "?",
                                nostra != NIHIL ? nostra : "?",
                                (integer)d->auctor.mensura,
                                d->auctor.mensura > ZEPHYRUM
                                    ? (constans character*)d->auctor.datum
                                    : "-",
                                d->attingibile ? "attingibile" : "absens",
                                (integer)d->numerus);
                        }
                    }
                    CREDO_AEQUALIS_I32 (summa,
                        census.notae_verba[k] - census.notae_recti[k]);
                }
            }
        }
        /* T26 LIGATIO: praecisio pinnata solum crescens, revocatio relata */
        {
            i32 ligatio_permille = _permille(census.ligationes_rectae,
                census.ligationes_nostrae);

            imprimere("    ligatio praecisio %d/%d = %d permille (pinna %d,"
                " solum crescens)  revocatio %d/%d = %d permille\n",
                (integer)census.ligationes_rectae,
                (integer)census.ligationes_nostrae,
                (integer)ligatio_permille,
                (integer)pinna_ligationis,
                (integer)census.ligationes_rectae,
                (integer)census.arcus_aurei,
                (integer)_permille(census.ligationes_rectae,
                    census.arcus_aurei));
            si (pinna_ligationis > ZEPHYRUM)
            {
                CREDO_VERUM (census.ligationes_nostrae > ZEPHYRUM);
                CREDO_VERUM (ligatio_permille >= pinna_ligationis);
            }
        }
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
    /* T20a: CLAUSULAE - puritas per causam relata, puritas positorum
     * et pares pinnatae; leges: summa causarum + apertae == iudicata,
     * iudicata <= verba - inalignata, coactio M, errata == verba -
     * rectae positorum */
    {
        i32 positae  = ZEPHYRUM;
        i32 rectae   = ZEPHYRUM;
        i32 summa    = ZEPHYRUM;
        i32 puritas;
        i32 pares;
        i32 i;

        per (i = ZEPHYRUM; i <= (i32)ORATIO_CLAUSULA_CAUSA_NUMERUS; i++)
        {
            summa = summa + census.clausulae_verba[i];
            si (i < (i32)ORATIO_CLAUSULA_CAUSA_NUMERUS)
            {
                positae  = positae + census.clausulae_verba[i];
                rectae   = rectae + census.clausulae_rectae[i];
            }
            si (census.clausulae_verba[i] > ZEPHYRUM)
            {
                imprimere("    clausula %-9s %6d  puritas %d permille\n",
                    i < (i32)ORATIO_CLAUSULA_CAUSA_NUMERUS
                        ? ORATIO_TITULI_CAUSARUM_CLAUSULAE[i] : "apertae",
                    (integer)census.clausulae_verba[i],
                    (integer)_permille(census.clausulae_rectae[i],
                        census.clausulae_verba[i]));
            }
        }
        puritas  = _permille(rectae, positae);
        pares    = _permille(census.clausulae_pares,
            census.clausulae_sententiae);
        imprimere("    clausulae: puritas %d permille (pinna %d, solum"
            " crescens)  coactio %d permille  nostrae %d aureae %d  pares"
            " %d permille (pinna %d)\n", (integer)puritas,
            (integer)pinna_puritatis,
            (integer)_permille(positae, census.clausulae_iudicata),
            (integer)census.clausulae_nostrae,
            (integer)census.clausulae_aureae, (integer)pares,
            (integer)pinna_parium);
        CREDO_AEQUALIS_I32 (summa, census.clausulae_iudicata);
        CREDO_VERUM (census.clausulae_iudicata > ZEPHYRUM);
        CREDO_VERUM (census.clausulae_iudicata
            <= census.verba - census.inalignata);
        CREDO_AEQUALIS_I32 (_permille(positae,
            census.clausulae_iudicata),
            (i32)1000);
        CREDO_VERUM (census.clausulae_nostrae > ZEPHYRUM);
        CREDO_VERUM (census.clausulae_aureae > ZEPHYRUM);
        /* T20a quinquies: PURITAS relata, non pinnata - unilateralis ut
         * pares (segmentationem nimiam praemiat: LLCT test 893 -> 890 dum
         * concordia 802 -> 809 et numerus ad aurum); CONCORDIA parium sola
         * pinna decisionis */
        (vacuum)pinna_puritatis;
        /* T20a ter: PARES relatae, non pinnatae - numerus solus ordinem non
         * videt et puritas sola segmentationem nimiam praemiat; CONCORDIA
         * parium utrumque punit et sola pinna decisionis est (pinna parium
         * servata ut relatio) */
        (vacuum)pinna_parium;
        {
            i32 concordia = _permille(census.clausulae_paria_concordia,
                census.clausulae_paria);

            imprimere("    clausulae: concordia parium %d permille - %d de %d paria"
                " (pinna %d paria, solum crescens)\n",
                (integer)concordia,
                (integer)census.clausulae_paria_concordia,
                (integer)census.clausulae_paria,
                (integer)pinna_concordiae);
            CREDO_VERUM (census.clausulae_paria > ZEPHYRUM);
            CREDO_VERUM (census.clausulae_paria_concordia
                >= pinna_concordiae);
            /* T20b: catena relata - discordiae (ligationes trans clausulas)
             * numeratae, nihil motum (mensuratum) */
            imprimere("    catena: discordiae %d  catenatae %d\n",
                (integer)census.catena_discordiae,
                (integer)census.catena_catenatae);
            CREDO_AEQUALIS_I32 (census.catena_catenatae, ZEPHYRUM);
        }
        /* errata clausularum: summa numerorum == positae - rectae */
        {
            Xar* es = oratio_oraculum_errata_clausularum(p, &census);
            i32  summa_erratorum = ZEPHYRUM;
            i32  m;

            CREDO_NON_NIHIL (es);
            per (m = ZEPHYRUM; es != NIHIL && m < xar_numerus(es); m++)
            {
                constans OratioOraculumErratumClausulae* d =
                    *(OratioOraculumErratumClausulae**)xar_obtinere(es,
                    m);

                summa_erratorum = summa_erratorum + d->numerus;
                si (m < (i32)III)
                {
                    imprimere("      erratum clausulae %s %.*s species %d"
                        " radix %.*s %d\n",
                        ORATIO_TITULI_CAUSARUM_CLAUSULAE[d->causa],
                        (integer)d->forma.mensura,
                        (constans character*)d->forma.datum,
                        (integer)d->species, (integer)d->radix.mensura,
                        (constans character*)d->radix.datum,
                        (integer)d->numerus);
                }
            }
            CREDO_AEQUALIS_I32 (summa_erratorum, positae - rectae);
        }
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
                    I);   /* T21: pronum scissum a que, contra elementum suum solum */
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
                (i32)CIRCSE_COACTAE_PINNA,
        (i32)CIRCSE_CASUUM_PINNA,
        (i32)CIRCSE_NUMERI_PINNA,
        (i32)CIRCSE_GENERIS_PINNA,
        (i32)CIRCSE_VOCIS_PINNA,
        (i32)CIRCSE_FORMAE_VERBI_PINNA,
        (i32)CIRCSE_LIGATIONIS_PINNA,
        (i32)CIRCSE_PURITAS_PINNA,
        (i32)CIRCSE_PARES_PINNA,
        (i32)CIRCSE_CONCORDIA_PINNA);

    _thesaurus_arborum(piscina, &vocabularia, programma, radix,
        "la_llct-ud-dev.conllu",
        (i32)850,
        (i32)LLCT_DEV_TECTA_PINNA,
        (i32)LLCT_DEV_PRIMARIA_PINNA,
                (i32)LLCT_DEV_COACTAE_PINNA,
        (i32)LLCT_DEV_CASUUM_PINNA,
        (i32)LLCT_DEV_NUMERI_PINNA,
        (i32)LLCT_DEV_GENERIS_PINNA,
        (i32)LLCT_DEV_VOCIS_PINNA,
        (i32)LLCT_DEV_FORMAE_VERBI_PINNA,
        (i32)LLCT_DEV_LIGATIONIS_PINNA,
        (i32)LLCT_DEV_PURITAS_PINNA,
        (i32)LLCT_DEV_PARES_PINNA,
        (i32)LLCT_DEV_CONCORDIA_PINNA);

    _thesaurus_arborum(piscina, &vocabularia, programma, radix,
        "la_llct-ud-test.conllu",
        (i32)884,
        (i32)LLCT_TEST_TECTA_PINNA,
        (i32)LLCT_TEST_PRIMARIA_PINNA,
                (i32)LLCT_TEST_COACTAE_PINNA,
        (i32)LLCT_TEST_CASUUM_PINNA,
        (i32)LLCT_TEST_NUMERI_PINNA,
        (i32)LLCT_TEST_GENERIS_PINNA,
        (i32)LLCT_TEST_VOCIS_PINNA,
        (i32)LLCT_TEST_FORMAE_VERBI_PINNA,
        (i32)LLCT_TEST_LIGATIONIS_PINNA,
        (i32)LLCT_TEST_PURITAS_PINNA,
        (i32)LLCT_TEST_PARES_PINNA,
        (i32)LLCT_TEST_CONCORDIA_PINNA);

    _thesaurus_arborum(piscina, &vocabularia, programma, radix,
        "en_ewt-ud-dev.conllu",
        (i32)2001,
        (i32)EWT_DEV_TECTA_PINNA,
        (i32)EWT_DEV_PRIMARIA_PINNA,
                (i32)EWT_DEV_COACTAE_PINNA,
        (i32)EWT_DEV_CASUUM_PINNA,
        (i32)EWT_DEV_NUMERI_PINNA,
        (i32)EWT_DEV_GENERIS_PINNA,
        (i32)EWT_DEV_VOCIS_PINNA,
        (i32)EWT_DEV_FORMAE_VERBI_PINNA,
        (i32)EWT_DEV_LIGATIONIS_PINNA,
        (i32)EWT_DEV_PURITAS_PINNA,
        (i32)EWT_DEV_PARES_PINNA,
        (i32)EWT_DEV_CONCORDIA_PINNA);

    _thesaurus_arborum(piscina, &vocabularia, programma, radix,
        "en_ewt-ud-test.conllu",
        (i32)2077,
        (i32)EWT_TEST_TECTA_PINNA,
        (i32)EWT_TEST_PRIMARIA_PINNA,
                (i32)EWT_TEST_COACTAE_PINNA,
        (i32)EWT_TEST_CASUUM_PINNA,
        (i32)EWT_TEST_NUMERI_PINNA,
        (i32)EWT_TEST_GENERIS_PINNA,
        (i32)EWT_TEST_VOCIS_PINNA,
        (i32)EWT_TEST_FORMAE_VERBI_PINNA,
        (i32)EWT_TEST_LIGATIONIS_PINNA,
        (i32)EWT_TEST_PURITAS_PINNA,
        (i32)EWT_TEST_PARES_PINNA,
        (i32)EWT_TEST_CONCORDIA_PINNA);


    imprimere("\n");
    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        credo_claudere();
        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
