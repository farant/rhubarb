/* probatio_oratio_resolutio.c - Resolutio per contextum (gradus V, T17)
 *
 * I.   Programma: lectio (regulae II cum titulis), programma malum
 *      recusatum cum linea, radix absens = causa nominata, lingua ex
 *      censu.
 * II.  Skeleton ambulans: 'Cum puella ambulat.' - puella ablativus
 *      primus post resolutionem (ordo fontis NOM VOC ABL), compendia
 *      reposita, census (sententiae, ordines, applicatae, per regulam),
 *      idempotentia (cursus alter nihil mutat), regulae 0 = nihil,
 *      'In urbem venit.' accusativus (iam primus: ordo lectus, nihil
 *      permutatum), sententia sine praepositione intacta.
 * III. Planum iudicatum: proiectio sententiae + programma expansa
 *      canoni orationis 0 vitia (regula/consilium/praelatio); ordo
 *      malus (ordinalis extra fines) recusatus, nihil mutatum.
 * IV.  Verbum: permutatio non-permutatio refutata (materia).
 */

#include "latina.h"
#include "credo.h"
#include "oratio_arbor.h"
#include "oratio_partes.h"
#include "oratio_registrum.h"
#include "oratio_resolutio.h"
#include "oratio_stml.h"
#include "oratio_lexicon.h"
#include "oratio_vocabularia.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "canon.h"
#include "stml.h"
#include "stml_macros.h"
#include "internamentum.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* an chorda literas contineat (sine fine NUL) */
interior b32
_continet (
                chorda  c,
    constans character* literae)
{
    i32 l = (i32)strlen(literae);
    i32 k;

    per (k = ZEPHYRUM; k + l <= c.mensura; k++)
    {
        si (memcmp(c.datum + k, literae, (size_t)l) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                chorda* exitus)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    exitus->datum    = NIHIL;
    exitus->mensura  = ZEPHYRUM;
    f                = fopen(via, "rb");
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
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde FALSUM;
    }
    memoria[longitudo]  = ZEPHYRUM;
    exitus->datum       = (i8*)memoria;
    exitus->mensura     = (i32)longitudo;
    redde VERUM;
}

/* vocabulum n-tum sententiae primae paragraphi primi */
interior MateriaNodus*
_vocabulum (
    MateriaNodus* doc,
             i32  n)
{
    constans MateriaValor* paragraphi =
        &doc->loci[ORATIO_DOCUMENTUM_PARAGRAPHI];
    constans MateriaNodus* par;
    constans MateriaNodus* sen;
    constans MateriaValor* sententiae;
    constans MateriaValor* elementa;

    si (paragraphi->genus != MATERIA_VALOR_LISTA)
    {
        redde NIHIL;
    }
    par = materia_valor_lista_obtinere(*paragraphi,
        ZEPHYRUM)->datum.nodus;
    sententiae = &par->loci[ORATIO_PARAGRAPHUS_SENTENTIAE];
    sen = materia_valor_lista_obtinere(*sententiae,
        ZEPHYRUM)->datum.nodus;
    elementa = &sen->loci[ORATIO_SENTENTIA_ELEMENTA];
    si (n >= materia_valor_lista_numerus(*elementa))
    {
        redde NIHIL;
    }
    redde materia_valor_lista_obtinere(*elementa, n)->datum.nodus;
}

/* casus analysis k-tae vocabuli (INDEX), -I si absens */
/* nodus analysis k vocabuli (T19d); NIHIL si absens */
interior constans MateriaNodus*
_analysis (
    constans MateriaNodus* vocabulum,
                      i32  k)
{
    constans MateriaValor* analyses =
        &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];

    si (   analyses->genus != MATERIA_VALOR_LISTA
        || k               >= materia_valor_lista_numerus(*analyses))
    {
        redde NIHIL;
    }
    redde materia_valor_lista_obtinere(*analyses, k)->datum.nodus;
}

interior s32
_casus (
    constans MateriaNodus* vocabulum,
                      i32  k)
{
    constans MateriaValor* analyses =
        &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
    constans MateriaNodus* a;
                      s32  locus;

    si (   analyses->genus != MATERIA_VALOR_LISTA
        || k               >= materia_valor_lista_numerus(*analyses))
    {
        redde (s32)-I;
    }
    a     = materia_valor_lista_obtinere(*analyses, k)->datum.nodus;
    locus =
        oratio_partes_locus(oratio_genus_classis((OratioGenus)a->genus),
        "casus");
    si (locus < ZEPHYRUM || a->loci[locus].genus != MATERIA_VALOR_INDEX)
    {
        redde (s32)-I;
    }
    redde a->loci[locus].datum.index;
}

/* lingua analysis k-tae vocabuli (INDEX), -I si absens */
interior s32
_lingua (
    constans MateriaNodus* vocabulum,
                      i32  k)
{
    constans MateriaValor* analyses =
        &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
    constans MateriaNodus* a;
                      s32  locus;

    si (   analyses->genus != MATERIA_VALOR_LISTA
        || k               >= materia_valor_lista_numerus(*analyses))
    {
        redde (s32)-I;
    }
    a     = materia_valor_lista_obtinere(*analyses, k)->datum.nodus;
    locus =
        oratio_partes_locus(oratio_genus_classis((OratioGenus)a->genus),
        "lingua");
    si (locus < ZEPHYRUM || a->loci[locus].genus != MATERIA_VALOR_INDEX)
    {
        redde (s32)-I;
    }
    redde a->loci[locus].datum.index;
}

/* classis analysis k-tae vocabuli (ex genere nodi), -I si absens */
interior s32
_classis_analysis (
    constans MateriaNodus* vocabulum,
                      i32  k)
{
    constans MateriaValor* analyses =
        &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];

    si (   analyses->genus != MATERIA_VALOR_LISTA
        || k               >= materia_valor_lista_numerus(*analyses))
    {
        redde (s32)-I;
    }
    redde (s32)oratio_genus_classis((OratioGenus)
        materia_valor_lista_obtinere(*analyses, k)->datum.nodus->genus);
}

interior b32
_incipit (
                chorda  c,
    constans character* literae)
{
    i32 m = (i32)strlen(literae);

    redde c.mensura >= m
        && memcmp(c.datum, literae, (size_t)m) == ZEPHYRUM;
}

interior chorda
_compendium (
    constans MateriaNodus* vocabulum,
                      i32  locus)
{
    constans MateriaValor* v = &vocabulum->loci[locus];
                   chorda  vacua;

    si (v->genus == MATERIA_VALOR_TOKEN && v->datum.token != NIHIL)
    {
        redde v->datum.token->valor;
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

/* numerus analysium vocabuli */
interior i32
_numerus_analysium (
    constans MateriaNodus* vocabulum)
{
    constans MateriaValor* analyses =
        &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];

    redde analyses->genus == MATERIA_VALOR_LISTA
        ? materia_valor_lista_numerus(*analyses) : ZEPHYRUM;
}

/* decisio vocabuli (T19g): INDEX aut -I */
interior s32
_decisio (
    constans MateriaNodus* vocabulum)
{
    constans MateriaValor* v =
        &vocabulum->loci[ORATIO_VOCABULUM_DECISIO];

    redde v->genus == MATERIA_VALOR_INDEX ? v->datum.index : (s32)-I;
}

/* umbra 'u' lectionis: nodus aut NIHIL */
interior constans MateriaNodus*
_umbra (
    constans MateriaNodus* lectio,
                      i32  u)
{
    OratioClassis classis =
        oratio_genus_classis((OratioGenus)lectio->genus);
                      s32  locus;
    constans MateriaValor* umbrae;
    constans MateriaValor* valor;

    si (classis >= ORATIO_CLASSIS_NUMERUS_CLASSIUM)
    {
        redde NIHIL;
    }
    locus = oratio_partes_locus(classis, "umbrae");
    si (locus < ZEPHYRUM || (i32)locus >= lectio->numerus_locorum)
    {
        redde NIHIL;
    }
    umbrae = &lectio->loci[locus];
    si (   umbrae->genus != MATERIA_VALOR_LISTA
        || u             >= materia_valor_lista_numerus(*umbrae))
    {
        redde NIHIL;
    }
    valor = materia_valor_lista_obtinere(*umbrae, u);
    redde valor != NIHIL && valor->genus == MATERIA_VALOR_NODUS
        ? valor->datum.nodus : NIHIL;
}

/* index INDEX loci umbrae 'u' lectionis; -I si absens aut non
 * scriptus */
interior s32
_umbra_index (
    constans MateriaNodus* lectio,
                      i32  u,
                      i32  locus)
{
    constans MateriaNodus* umbra = _umbra(lectio, u);

    si (   umbra                    == NIHIL
        || umbra->loci[locus].genus == MATERIA_VALOR_NIHIL)
    {
        redde (s32)-I;
    }
    redde umbra->loci[locus].datum.index;
}

interior s32
_umbra_impletio_vocabulum (
    constans MateriaNodus* lectio,
                      i32  u)
{
    redde _umbra_index(lectio, u, (i32)ORATIO_UMBRA_IMPLETIO_VOCABULUM);
}

interior s32
_umbra_impletio_analysis (
    constans MateriaNodus* lectio,
                      i32  u)
{
    redde _umbra_index(lectio, u, (i32)ORATIO_UMBRA_IMPLETIO_ANALYSIS);
}

/* accidens lectionis per titulum loci: -I si genus sine aut non
 * scriptum */
interior s32
_accidens (
    constans MateriaNodus* lectio,
       constans character* titulus)
{
    OratioClassis classis =
        oratio_genus_classis((OratioGenus)lectio->genus);
              s32 locus;

    si (classis >= ORATIO_CLASSIS_NUMERUS_CLASSIUM)
    {
        redde (s32)-I;
    }
    locus = oratio_partes_locus(classis, titulus);
    si (   locus < ZEPHYRUM || (i32)locus >= lectio->numerus_locorum
        || lectio->loci[locus].genus == MATERIA_VALOR_NIHIL)
    {
        redde (s32)-I;
    }
    redde lectio->loci[locus].datum.index;
}

/* CORPUS: umbrae ligatae documenti totius probare - implens exsistit
 * et accidentia umbrae (casus, numerus, genus, ubi scripta) lectionis
 * implentis sunt; ligatae/discordes numerantur */
interior vacuum
_umbras_probare (
    constans MateriaNodus* doc,
                      i32* ligatae,
                      i32* discordes)
{
    constans MateriaValor* paragraphi =
        &doc->loci[ORATIO_DOCUMENTUM_PARAGRAPHI];
                      i32 p;

    si (paragraphi->genus != MATERIA_VALOR_LISTA)
    {
        redde;
    }
    per (p = ZEPHYRUM; p
        < materia_valor_lista_numerus(*paragraphi); p++)
    {
        constans MateriaNodus* paragraphus =
            materia_valor_lista_obtinere(*paragraphi, p)->datum.nodus;
        constans MateriaValor* sententiae =
            &paragraphus->loci[ORATIO_PARAGRAPHUS_SENTENTIAE];
                          i32 s;

        si (sententiae->genus != MATERIA_VALOR_LISTA)
        {
            perge;
        }
        per (s = ZEPHYRUM; s < materia_valor_lista_numerus(*sententiae);
             s++)
        {
            constans MateriaNodus* sententia =
                materia_valor_lista_obtinere(*sententiae,
                s)->datum.nodus;
            constans MateriaValor* elementa =
                &sententia->loci[ORATIO_SENTENTIA_ELEMENTA];
                              i32 ne;
                              i32 k;

            si (elementa->genus != MATERIA_VALOR_LISTA)
            {
                perge;
            }
            ne = materia_valor_lista_numerus(*elementa);
            per (k = ZEPHYRUM; k < ne; k++)
            {
                constans MateriaNodus* vocabulum =
                    materia_valor_lista_obtinere(*elementa,
                    k)->datum.nodus;
                                  i32 a;

                si (vocabulum->genus != (s32)ORATIO_GENUS_VOCABULUM)
                {
                    perge;
                }
                per (a = ZEPHYRUM; a
                    < _numerus_analysium(vocabulum); a++)
                {
                    constans MateriaNodus* lectio = _analysis(vocabulum,
                        a);
                                      i32 u;

                    per (u = ZEPHYRUM; _umbra(lectio, u) != NIHIL; u++)
                    {
                        constans MateriaNodus* implens_vocabulum;
                        constans MateriaNodus* implens;
                                          s32  w;
                                          s32  b;
                                          s32  c;

                        w = _umbra_impletio_vocabulum(lectio, u);
                        b = _umbra_impletio_analysis(lectio, u);
                        si (w < ZEPHYRUM)
                        {
                            perge;   /* vacua */
                        }
                        si (   w >= (s32)ne || b < ZEPHYRUM)
                        {
                            *discordes = *discordes + I;
                            perge;
                        }
                        implens_vocabulum =
                            materia_valor_lista_obtinere(
                            *elementa, (i32)w)->datum.nodus;
                        si (   implens_vocabulum->genus
                                != (s32)ORATIO_GENUS_VOCABULUM
                            || b >= (s32)_numerus_analysium(
                                implens_vocabulum))
                        {
                            *discordes = *discordes + I;
                            perge;
                        }
                        implens = _analysis(implens_vocabulum, (i32)b);
                        c = _umbra_index(lectio, u,
                            (i32)ORATIO_UMBRA_CASUS);
                        /* T19f: implens casus IGNOTI (accidens non
                         * scriptum, -I) umbrae cuivis congruit */
                        si (   (   c >= ZEPHYRUM
                                && _accidens(implens, "casus")
                                    >= ZEPHYRUM
                                && _accidens(implens, "casus") != c)
                            || (_umbra_index(lectio, u,
                                    (i32)ORATIO_UMBRA_NUMERUS)
                                        >= ZEPHYRUM
                                && _accidens(implens, "numerus")
                                    >= ZEPHYRUM
                                && _accidens(implens, "numerus")
                                    != _umbra_index(lectio, u,
                                        (i32)ORATIO_UMBRA_NUMERUS))
                            || (_umbra_index(lectio, u,
                                    (i32)ORATIO_UMBRA_GENUS) >= ZEPHYRUM
                                && _accidens(implens, "genus")
                                    >= ZEPHYRUM
                                && _accidens(implens, "genus")
                                    != _umbra_index(lectio, u,
                                        (i32)ORATIO_UMBRA_GENUS)))
                        {
                            *discordes = *discordes + I;
                            perge;
                        }
                        *ligatae = *ligatae + I;
                    }
                }
            }
        }
    }
}

/* documentum parsatum et annotatum */
interior MateriaNodus*
_documentum (

                       Piscina* piscina,
    constans OratioVocabularia* vocabularia,
            constans character* fons,
            OratioPartesCensus* census)
{
    MateriaNodus* doc = oratio_arbor_parsare(piscina, fons,
        (i32)strlen(fons));

    si (   doc == NIHIL
        || !oratio_partes_annotare(piscina, vocabularia, doc,
            census))
    {
        redde NIHIL;
    }
    redde doc;
}

/* index regulae per titulum (-I si absens) */
interior s32
_index_regulae (
    constans OratioProgramma* programma,
          constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(programma->regulae); i++)
    {
        constans OratioRegula* r =
            (constans OratioRegula*)xar_obtinere(programma->regulae, i);

        si (_aequalis(r->titulus, titulus))
        {
            redde (s32)i;
        }
    }
    redde (s32)-I;
}


s32
principale (vacuum)
{
                     Piscina* piscina;
          constans character* radix;
           OratioVocabularia  vocabularia;
    OratioVocabulariumVitium  vitium;
         InternamentumChorda* intern;
         MateriaLexiconRatum  ratum;
          MateriaLexIudicium  iudicium;
             OratioProgramma* programma;

    piscina = piscina_generare_dynamicum("probatio_oratio_resolutio",
        268435456);
    credo_aperire(piscina);
    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    CREDO_VERUM (oratio_vocabularia_onerare(piscina, radix,
        &vocabularia,
        &vitium));
    intern = internamentum_creare(piscina);
    CREDO_NON_NIHIL (intern);
    CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &ORATIO_LEXICON,
        &iudicium));
    si (vocabularia.la == NIHIL || intern == NIHIL)
    {
        credo_imprimere_compendium();
        redde I;
    }

    imprimere("\n--- I. Programma ---\n");
    programma = oratio_resolutio_programma_onerare(piscina, intern,
        radix,
        &vitium);
    CREDO_NON_NIHIL (programma);
    si (programma == NIHIL)
    {
        imprimere("  %s:%d %s\n", vitium.plagula ? vitium.plagula : "?",
            (integer)vitium.linea, vitium.causa ? vitium.causa : "-");
        credo_imprimere_compendium();
        redde I;
    }
        /* T18: regula linguae PRIMA (prioritas: lingua documenti ante
     * casus), deinde adpositiones */
        /* T19b: regulae priorum classium (XI, lingua custodita) ANTE
     * regulam linguae (prima quae valet vincit), casus post; ordo per
     * TITULOS iudicatur, non per indices literales */
    CREDO_VERUM (xar_numerus(programma->regulae) >= (i32)XIV);
    {
                s32 auxiliare_la   = _index_regulae(programma,
                    "auxiliare-primum-latinum");
        s32 auxiliare_en   = _index_regulae(programma,
            "auxiliare-primum-anglicum");
        s32 supra          = _index_regulae(programma,
            "supra-adverbium-latinum");
        s32 adpositio_la   = _index_regulae(programma,
            "adpositio-prima-latina");
        s32 subordinans_la = _index_regulae(programma,
            "subordinans-prima-latina");
        s32 numerale_en    = _index_regulae(programma,
            "numerale-primum-anglicum");
        s32 lingua         = _index_regulae(programma,
            "lingua-documenti-anglica");
                s32 accusativum    = _index_regulae(programma,
                    "umbra-obiectum-accusativi");
        s32 ablativum      = _index_regulae(programma,
            "umbra-obiectum-ablativi");


        /* T19e: regulae Anglicae testimonii IV in capite (exceptio a,
         * nomen proprium capitale, umbrae verbi II) ANTE priores */
        CREDO_AEQUALIS_S32 (_index_regulae(programma,
            "a-determinans-anglicum"), ZEPHYRUM);
        CREDO_AEQUALIS_S32 (auxiliare_la, (s32)IV);
        CREDO_AEQUALIS_S32 (auxiliare_en, (s32)V);
        CREDO_VERUM (supra >= ZEPHYRUM && adpositio_la > supra);
        CREDO_VERUM (subordinans_la > adpositio_la
            && numerale_en > subordinans_la);
                CREDO_VERUM (lingua > numerale_en
                    && accusativum > lingua
                    && ablativum > accusativum);
        /* T19d beta: regulae capitis XIV (gradus II) post umbras
         * obiecti (gradus I); gradus lectus ex attributo, absens = I */
        /* T19e: regulae Anglicae IV in capite (exceptio a, nomen
         * proprium capitale, umbrae verbi II) + prior contractionum:
         * gradus I = XIX regulae (0..XVIII) */
                /* T19i: prior determinantium certorum XVII ante umbras obiecti
         * (caput lectionis determinantis ligatur) */
        CREDO_AEQUALIS_S32 (_index_regulae(programma,
            "determinans-primum-latinum"), (s32)XVII);
        CREDO_AEQUALIS_S32 (accusativum, (s32)XVIII);
        CREDO_AEQUALIS_S32 (ablativum, (s32)XIX);
        /* T19f: implentes casus ignoti (nomen proprium capitale) -
         * obiectum XX (gradus I), capita post (gradus II/III) */
                CREDO_AEQUALIS_S32 (_index_regulae(programma,
                    "umbra-obiectum-incerti-proprii"), (s32)XX);
                /* T19k: implens ignotus XXI (gradus I); capita ignoti mensurata
         * et ablata (53-57 % chartis) */
        CREDO_AEQUALIS_S32 (_index_regulae(programma,
            "umbra-obiectum-ignoti"), (s32)XXI);
                /* T19h: IANUA STRICTA capitum - regulae capitis XVI cursu
         * stricto gradu II (XX..XXXV, titulo -proximo), eaedem cursu
                  * fratrum gradu III; regulae LIII (T19i: + prior determinantium
         * gradu I: strictae XXI..XXXVI, laxae XXXVII..LII) */
                /* T27: gradus strictus unitus XIV -> II: regulae XLII
                 * (strictae XXII..XXV cum incertis, laxae XXVI..XLI) */
                CREDO_AEQUALIS_I32 (xar_numerus(programma->regulae),
                                (i32)XLII);
        {
            i32 k;

            per (k = ZEPHYRUM; k < xar_numerus(programma->regulae); k++)
            {
                constans OratioRegula* r =
                    (constans OratioRegula*)xar_obtinere(
                    programma->regulae, k);

                                                CREDO_AEQUALIS_I32 (r->gradus,
                                                    k <= (i32)XXI ? I
                                                                        : (k
                                                                            <= (i32)XXV ? (i32)II : (i32)III));
            }
            {
                constans OratioRegula* prima =
                    (constans OratioRegula*)xar_obtinere(
                                                            programma->regulae,
                                                            (i32)XXII);
                constans OratioRegula* laxa =
                    (constans OratioRegula*)xar_obtinere(
                                        programma->regulae,
                                        (i32)XXVI);

                CREDO_VERUM (_aequalis(prima->titulus,
                    "umbra-caput-sequente-proximo"));
                CREDO_VERUM (_aequalis(laxa->titulus,
                    "umbra-caput-nominativus-sequente"));
            }
        }
    }

    CREDO_VERUM (((constans OratioRegula*)xar_obtinere(
        programma->regulae, ZEPHYRUM))->textus.mensura > (i32)100);

    imprimere("  regulae %d\n",
        (integer)xar_numerus(programma->regulae));
    /* programma malum: sine titulo -> linea; textus non STML */
    {
        OratioProgramma* malum =
            oratio_resolutio_programma_legere(piscina,
            intern, _l("<regula><consilium/></regula>\n"), &vitium);

        CREDO_NIHIL (malum);
        CREDO_VERUM (vitium.causa != NIHIL
            && strstr(vitium.causa, "titulo") != NIHIL);
        CREDO_AEQUALIS_I32 (vitium.linea, I);
        malum = oratio_resolutio_programma_legere(piscina, intern,
            _l("<regula titulus=\"x\">\n"), &vitium);
        CREDO_NIHIL (malum);
        malum = oratio_resolutio_programma_legere(piscina, intern,
            _l("<nihil/>\n"), &vitium);
        CREDO_NON_NIHIL (malum);
        CREDO_AEQUALIS_I32 (malum == NIHIL ? (i32)-I
            : xar_numerus(malum->regulae), ZEPHYRUM);
        /* radix absens */
        CREDO_NIHIL (oratio_resolutio_programma_onerare(piscina, intern,
            "/nemo/hic", &vitium));
        CREDO_VERUM (vitium.causa != NIHIL
            && strcmp(vitium.causa, "plagula absens") == ZEPHYRUM);
    }
    /* lingua ex censu */
    {
        i32 linguae[ORATIO_LINGUA_NUMERUS];

        linguae[ORATIO_LINGUA_LATINA]   = (i32)3;
        linguae[ORATIO_LINGUA_ANGLICA]  = (i32)7;
        CREDO_VERUM (strcmp(oratio_resolutio_lingua_censu(linguae),
            "anglica") == ZEPHYRUM);
        linguae[ORATIO_LINGUA_ANGLICA] = (i32)3;
        CREDO_VERUM (strcmp(oratio_resolutio_lingua_censu(linguae),
            "latina") == ZEPHYRUM);
    }

    imprimere("\n--- II. Skeleton: adpositio casum regit ---\n");
    {
           OratioPartesCensus  census_partium;
        OratioResolutioCensus  census;
                 MateriaNodus* doc = _documentum(piscina, &vocabularia,
                     "Cum puella ambulat.\n", &census_partium);
        MateriaNodus* puella;

        CREDO_NON_NIHIL (doc);
        puella = _vocabulum(doc, I);
        CREDO_NON_NIHIL (puella);
        /* ordo fontis: NOM VOC ABL */
        CREDO_AEQUALIS_S32 (_casus(puella, ZEPHYRUM),
            (s32)ORATIO_CASUS_NOMINATIVUS);
        CREDO_AEQUALIS_S32 (_casus(puella, (i32)II),
            (s32)ORATIO_CASUS_ABLATIVUS);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I,
            oratio_resolutio_lingua_censu(census_partium.vocabula_linguarum),
            doc,
            &census));
        /* ABL primus, NOM VOC sequuntur ordine suo; nihil deletum */
        CREDO_AEQUALIS_S32 (_casus(puella, ZEPHYRUM),
            (s32)ORATIO_CASUS_ABLATIVUS);
        CREDO_AEQUALIS_S32 (_casus(puella, I),
            (s32)ORATIO_CASUS_NOMINATIVUS);
        CREDO_AEQUALIS_S32 (_casus(puella, (i32)II),
            (s32)ORATIO_CASUS_VOCATIVUS);
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(
            puella->loci[ORATIO_VOCABULUM_ANALYSES]), (i32)III);
        CREDO_VERUM (_aequalis(_compendium(puella,
            (i32)ORATIO_VOCABULUM_CLASSES), "substantivum"));
        CREDO_VERUM (_aequalis(_compendium(puella,
            (i32)ORATIO_VOCABULUM_LINGUAE), "latina"));
                CREDO_AEQUALIS_I32 (census.sententiae, I);
        CREDO_AEQUALIS_I32 (census.fractae, ZEPHYRUM);
                /* T19b: ordines II, permutatae II - 'cum' a regula
         * adpositio-prima-latina (WORDS adverbium primum, adpositio
         * quinta: permutata), 'puella' ab ablativum-regit */
        CREDO_AEQUALIS_I32 (census.ordines, (i32)II);
        CREDO_AEQUALIS_I32 (census.applicatae, (i32)II);

        CREDO_AEQUALIS_I32 (census.recusatae, ZEPHYRUM);
        CREDO_NON_NIHIL (census.per_regulam);
        CREDO_AEQUALIS_I32 (census.per_regulam == NIHIL ? ZEPHYRUM
            : xar_numerus(census.per_regulam), (i32)II);
        si (   census.per_regulam              != NIHIL
            && xar_numerus(census.per_regulam) == (i32)II)
        {
            constans OratioResolutioRegulaCensus* r0 =
                (constans OratioResolutioRegulaCensus*)xar_obtinere(
                census.per_regulam, ZEPHYRUM);
            constans OratioResolutioRegulaCensus* r1 =
                (constans OratioResolutioRegulaCensus*)xar_obtinere(
                census.per_regulam, I);

            CREDO_VERUM (_aequalis(r0->titulus,
                "adpositio-prima-latina"));
                        CREDO_VERUM (_aequalis(r1->titulus,
                            "umbra-obiectum-ablativi"));
            CREDO_AEQUALIS_I32 (r1->numerus, I);
        }
        /* T19d: umbra obiecti lectionis adpositionis 'cum' (nunc prima)
         * LIGATA ad puella (vocabulum I) analysin ablativam (post
         * permutationem primam: index 0 remissus); impletae I */
        CREDO_AEQUALIS_I32 (census.impletae, I);
        {
            constans MateriaNodus* cum_vocabulum = _vocabulum(doc,
                ZEPHYRUM);
            constans MateriaNodus* adpositio_lectio;
            constans MateriaValor* umbrae;
            constans MateriaNodus* umbra;
                              s32  locus;

            adpositio_lectio = _analysis(cum_vocabulum, ZEPHYRUM);
            CREDO_NON_NIHIL (adpositio_lectio);
            CREDO_AEQUALIS_S32 (adpositio_lectio->genus,
                (s32)ORATIO_GENUS_ANALYSIS_ADPOSITIONIS);
            locus = oratio_partes_locus(ORATIO_CLASSIS_ADPOSITIO,
                "umbrae");
            CREDO_VERUM (locus >= ZEPHYRUM);
            umbrae = &adpositio_lectio->loci[locus];
            CREDO_AEQUALIS_S32 ((s32)umbrae->genus,
                (s32)MATERIA_VALOR_LISTA);
            CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(*umbrae),
                I);
            umbra = materia_valor_lista_obtinere(*umbrae,
                ZEPHYRUM)->datum.nodus;
            CREDO_AEQUALIS_S32 (umbra->genus, (s32)ORATIO_GENUS_UMBRA);
            CREDO_AEQUALIS_S32 (umbra->loci[ORATIO_UMBRA_RELATIO]
                .datum.index, (s32)ORATIO_RELATIO_OBIECTUM);
            CREDO_AEQUALIS_S32 (umbra->loci[ORATIO_UMBRA_CASUS]
                .datum.index, (s32)ORATIO_CASUS_ABLATIVUS);
            CREDO_AEQUALIS_S32 ((s32)umbra->loci[
                ORATIO_UMBRA_IMPLETIO_VOCABULUM].genus,
                (s32)MATERIA_VALOR_INDEX);
            CREDO_AEQUALIS_S32 (umbra->loci[
                ORATIO_UMBRA_IMPLETIO_VOCABULUM].datum.index, (s32)I);
            CREDO_AEQUALIS_S32 (umbra->loci[
                ORATIO_UMBRA_IMPLETIO_ANALYSIS].datum.index, ZEPHYRUM);
        }

        /* idempotens: cursus alter nihil mutat */
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &census));
        CREDO_AEQUALIS_I32 (census.ordines, (i32)II);
        CREDO_AEQUALIS_I32 (census.applicatae, ZEPHYRUM);

        CREDO_AEQUALIS_S32 (_casus(puella, ZEPHYRUM),
            (s32)ORATIO_CASUS_ABLATIVUS);
        /* regulae 0 = nihil */
        doc = _documentum(piscina, &vocabularia,
            "Cum puella ambulat.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        oratio_resolutio_census_vacare(&census);
                CREDO_VERUM (oratio_resolutio_applicare(piscina, intern,
                    &ratum,
                    programma, ZEPHYRUM, "latina", doc, &census));
        /* T24: regula nulla (ordines 0), sed prior casuum verbum apertum
         * ordinat - ablativus ante nominativum (abl nom voc) */
        CREDO_AEQUALIS_S32 (_casus(_vocabulum(doc, I), ZEPHYRUM),
            (s32)ORATIO_CASUS_ABLATIVUS);
        CREDO_AEQUALIS_I32 (census.ordines, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (census.prior_casuum, I);
                /* regula prima sola (T19b: auxiliare-primum-latinum - sine
         * auxiliari nihil): cum puella intacta */
        oratio_resolutio_census_vacare(&census);
                CREDO_VERUM (oratio_resolutio_applicare(piscina, intern,
                    &ratum,
                    programma, I, "latina", doc, &census));
        CREDO_AEQUALIS_S32 (_casus(_vocabulum(doc, I), ZEPHYRUM),
            (s32)ORATIO_CASUS_ABLATIVUS);   /* T24: prior, non regula */
        CREDO_AEQUALIS_I32 (census.ordines, ZEPHYRUM);

        /* In urbem venit: accusativus iam primus - ordo lectus, nihil
         * permutatum */
        doc = _documentum(piscina, &vocabularia, "In urbem venit.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        oratio_resolutio_census_vacare(&census);
                CREDO_VERUM (oratio_resolutio_applicare(piscina, intern,
                    &ratum,
                    programma, (s32)-I, "latina", doc, &census));
        CREDO_AEQUALIS_I32 (census.ordines, (i32)II);   /* T19b: + In */
        CREDO_AEQUALIS_I32 (census.applicatae, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_casus(_vocabulum(doc, I), ZEPHYRUM),
            (s32)ORATIO_CASUS_ACCUSATIVUS);

        /* sine praepositione: nihil */
        doc = _documentum(piscina, &vocabularia, "Puella ambulat.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &census));
                CREDO_AEQUALIS_I32 (census.sententiae, I);
        CREDO_AEQUALIS_I32 (census.ordines, ZEPHYRUM);
        /* T24: sine regula prior casuum solus - abl ante nom */
        CREDO_AEQUALIS_S32 (_casus(_vocabulum(doc, ZEPHYRUM), ZEPHYRUM),
            (s32)ORATIO_CASUS_ABLATIVUS);
        /* programma sine regulis: nihil, nulla fractura */
        {
            OratioProgramma* vacuum_programma =
                oratio_resolutio_programma_legere(piscina, intern,
                _l("<nihil/>\n"), &vitium);

            oratio_resolutio_census_vacare(&census);
            CREDO_VERUM (oratio_resolutio_applicare(piscina, intern,
                &ratum,
                vacuum_programma, (s32)-I, "latina", doc, &census));
            CREDO_AEQUALIS_I32 (census.ordines, ZEPHYRUM);
        }
    }

    /* T24 PRIOR CASUUM: verbum APERTUM (nulla regula loquitur) ordinem
     * casuum mensuratum accipit post regulas omnes - puellae: ordo fontis
     * GEN DAT NOM VOC LOC, post priorem GEN NOM DAT VOC LOC (gradus
     * abl > acc > gen > nom > dat > voc > loc); nihil deletum, classis
     * immota, census.prior_casuum numerat */
    imprimere("\n--- II b. Prior casuum (T24) ---\n");
    {
           OratioPartesCensus  census_partium;
        OratioResolutioCensus  census;
                 MateriaNodus* doc = _documentum(piscina, &vocabularia,
                     "Puellae rosam dat.\n", &census_partium);
                 MateriaNodus* puellae;
                          i32  n_ante;

        CREDO_NON_NIHIL (doc);
        puellae = _vocabulum(doc, ZEPHYRUM);
        CREDO_NON_NIHIL (puellae);
        n_ante = materia_valor_lista_numerus(
            puellae->loci[ORATIO_VOCABULUM_ANALYSES]);
        CREDO_VERUM (n_ante >= (i32)IV);
                oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I,
            oratio_resolutio_lingua_censu(census_partium.vocabula_linguarum),
            doc,
            &census));
        CREDO_AEQUALIS_S32 (_casus(puellae, ZEPHYRUM),
            (s32)ORATIO_CASUS_GENITIVUS);
        CREDO_AEQUALIS_S32 (_casus(puellae, I),
            (s32)ORATIO_CASUS_NOMINATIVUS);
        CREDO_AEQUALIS_S32 (_casus(puellae, (i32)II),
            (s32)ORATIO_CASUS_DATIVUS);
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(
            puellae->loci[ORATIO_VOCABULUM_ANALYSES]), n_ante);
        CREDO_VERUM (census.prior_casuum >= I);
        CREDO_AEQUALIS_S32 ((s32)puellae->loci[ORATIO_VOCABULUM_DECISIO].genus,
            (s32)MATERIA_VALOR_NIHIL);   /* ordo, non decisio */
    }

    /* T25 POLITICA PARIUM: adiectivum cum candidatis duobus non vicinis
     * (Terra puella non magna est: non inter puellam et magnam) ordine
     * exemplaris ad PRIMUM (Terra, remotius) ligabatur; politica ordines
     * regulae intra (carrier, umbra) proximo primo ordinat -> puella (I) */
    imprimere("\n--- II c. Politica parium (T25) ---\n");
    {
           OratioPartesCensus  census_partium;
        OratioResolutioCensus  census;
                 MateriaNodus* doc = _documentum(piscina, &vocabularia,
                     "Terra puella non magna est.\n", &census_partium);
        constans MateriaNodus* magna;

        CREDO_NON_NIHIL (doc);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I,
            oratio_resolutio_lingua_censu(census_partium.vocabula_linguarum),
            doc,
            &census));
        magna = _analysis(_vocabulum(doc, (i32)III), ZEPHYRUM);
        CREDO_NON_NIHIL (magna);
        CREDO_AEQUALIS_S32 (_umbra_impletio_vocabulum(magna, ZEPHYRUM),
            I);
        CREDO_AEQUALIS_S32 (_casus(_vocabulum(doc, (i32)III), ZEPHYRUM),
            (s32)ORATIO_CASUS_NOMINATIVUS);
    }

    imprimere("\n--- III. Planum iudicatum; ordo malus recusatus"
        " ---\n");
    {
        OratioPartesCensus  census_partium;
              MateriaNodus* doc = _documentum(piscina, &vocabularia,
                  "Cum puella ambulat.\n", &census_partium);
        MateriaNodus* sententia;
        OratioStmlOrnatus ornatus;
        MateriaArborConsilium consilium;
        MateriaArborScriptura scriptura;
        character via[1024];
        chorda fons_canonis;
        chorda causa;
        Canon* canon;

        CREDO_NON_NIHIL (doc);
        sententia = (MateriaNodus*)_vocabulum(doc, ZEPHYRUM)->pater;
        CREDO_NON_NIHIL (sententia);
        CREDO_AEQUALIS_S32 (sententia->genus,
            (s32)ORATIO_GENUS_SENTENTIA);
        oratio_stml_consilium_ornatum(&consilium, &ratum, &ornatus,
            "latina");
        scriptura = materia_arbor_scribere_nodum(piscina, sententia,
            &consilium);
        CREDO_VERUM (scriptura.successus);
        sprintf(via, "%s/oratio/grammatica/oratio.canon", radix);
        CREDO_VERUM (_plagulam_legere(piscina, via, &fons_canonis));
        canon = canon_legere(fons_canonis, piscina, intern, &causa);
        CREDO_NON_NIHIL (canon);
        si (scriptura.successus && canon != NIHIL)
        {
                                    /* proiectio (ordinales, lingua) + programma -> expansio ->
             * documentum totum iudicatum: 0 vitia. Regulae TRES per
             * titulum (T19b: indices 0-2 nunc priores classium sunt,
             * quae in 'cum puella' nihil dant): lingua, umbra obiecti
             * accusativi, umbra obiecti ablativi (T19d) */

            constans OratioRegula* r0 =
                (constans OratioRegula*)xar_obtinere(programma->regulae,
                (i32)_index_regulae(programma,
                "lingua-documenti-anglica"));
            constans OratioRegula* r1 =
                (constans OratioRegula*)xar_obtinere(programma->regulae,
                                (i32)_index_regulae(programma,
                    "umbra-obiectum-accusativi"));

            constans OratioRegula* r2 =
                (constans OratioRegula*)xar_obtinere(programma->regulae,
                                (i32)_index_regulae(programma,
                    "umbra-obiectum-ablativi"));


            i32 mensura = scriptura.textus.mensura + r0->textus.mensura
                + r1->textus.mensura + r2->textus.mensura + (i32)IV;
            character* textus = (character*)piscina_allocare(piscina,
                (memoriae_index)mensura + I);
                          chorda  fons;
                    StmlResultus  lectio;
            StmlExpansioResultus  expansio;
                             Xar* vitia;

            CREDO_NON_NIHIL (textus);
                        sprintf(textus, "%.*s\n%.*s\n%.*s\n%.*s\n",
                            (integer)scriptura.textus.mensura,
                            (constans character*)scriptura.textus.datum,
                            (integer)r0->textus.mensura,
                            (constans character*)r0->textus.datum,
                            (integer)r1->textus.mensura,
                            (constans character*)r1->textus.datum,
                            (integer)r2->textus.mensura,
                            (constans character*)r2->textus.datum);
            fons = _l(textus);
            CREDO_VERUM (strstr(textus, " n=\"1\"") != NIHIL);
            CREDO_VERUM (strstr(textus, "lingua=\"latina\"") != NIHIL);
            lectio = stml_legere(fons, piscina, intern);
            CREDO_VERUM (lectio.successus);
            expansio = stml_expandere(lectio.radix, piscina, intern);
            CREDO_VERUM (expansio.successus);
            si (expansio.successus)
            {
                chorda expansa = stml_scribere(expansio.radix_expansa,
                    piscina, VERUM);

                /* T19d: regulae umbrarum ordines <impletio> pariunt,
                 * linguae in sententia Latina nullum: <praelatio> abest */
                CREDO_VERUM (!_continet(expansa, "<praelatio"));
                CREDO_VERUM (_continet(expansa, "<impletio"));
                CREDO_VERUM (_continet(expansa,
                    "regula=\"umbra-obiectum-ablativi\""));
                vitia = canon_iudicare(canon, expansio.radix_expansa,
                    piscina);
                CREDO_NON_NIHIL (vitia);
                si (vitia != NIHIL)
                {
                    i32 k;

                    per (k = ZEPHYRUM; k < xar_numerus(vitia)
                        && k < (i32)X;
                         k++)
                    {
                        CanonVitium* v =
                            (CanonVitium*)xar_obtinere(vitia,
                            k);

                        imprimere("  VITIUM %s",
                            canon_nuntius(v->genus));
                        si (v->elementum != NIHIL)
                        {
                            imprimere(" <%.*s>",
                                (integer)v->elementum->mensura,
                                (constans character*)
                                    v->elementum->datum);
                        }
                        imprimere("\n");
                    }
                    CREDO_AEQUALIS_I32 (xar_numerus(vitia), ZEPHYRUM);
                }
            }
        }
        /* ordo malus: analysis extra fines - recusatus, nihil
         * mutatum */
        {
            OratioResolutioCensus census;
                  OratioProgramma* malum;

            malum = oratio_resolutio_programma_legere(piscina, intern,
                _l(
                "<regula titulus=\"malus\"><consilium>"
                "<praelatio vocabulum=\"1\" analysis=\"9\"/>"
                "<praelatio vocabulum=\"7\" analysis=\"0\"/>"
                "<praelatio vocabulum=\"x\" analysis=\"0\"/>"
                "<praelatio vocabulum=\"1\" analysis=\"2\"/>"
                "</consilium></regula>\n"), &vitium);

            CREDO_NON_NIHIL (malum);
            oratio_resolutio_census_vacare(&census);
            CREDO_VERUM (oratio_resolutio_applicare(piscina, intern,
                &ratum,
                malum, (s32)-I, "latina", doc, &census));
            CREDO_AEQUALIS_I32 (census.recusatae, (i32)III);
            CREDO_AEQUALIS_I32 (census.ordines, I);
            CREDO_AEQUALIS_I32 (census.applicatae, I);
            CREDO_AEQUALIS_S32 (_casus(_vocabulum(doc, I), ZEPHYRUM),
                (s32)ORATIO_CASUS_ABLATIVUS);
        }
    }

    imprimere("\n--- IV. Verbum: permutatio probata ---\n");
    {
        OratioPartesCensus  census_partium;
              MateriaNodus* doc = _documentum(piscina, &vocabularia,
                  "Puella ambulat.\n", &census_partium);
        MateriaNodus* puella;
        i32 ordo[III];

        CREDO_NON_NIHIL (doc);
        puella          = _vocabulum(doc, ZEPHYRUM);
        ordo[ZEPHYRUM]  = I;
        ordo[I]         = I;
        ordo[II]        = ZEPHYRUM;
        CREDO_FALSUM (materia_nodus_lista_permutare(piscina, puella,
            (i32)ORATIO_VOCABULUM_ANALYSES, ordo, (i32)III));
        CREDO_AEQUALIS_S32 (_casus(puella, ZEPHYRUM),
            (s32)ORATIO_CASUS_NOMINATIVUS);
        ordo[I] = (i32)II;
        CREDO_VERUM (materia_nodus_lista_permutare(piscina, puella,
            (i32)ORATIO_VOCABULUM_ANALYSES, ordo, (i32)III));
        CREDO_AEQUALIS_S32 (_casus(puella, ZEPHYRUM),
            (s32)ORATIO_CASUS_VOCATIVUS);
    }

        imprimere("\n--- V. Regula linguae: documentum Anglicum lectiones"
            " Anglicas praefert (T18) ---\n");
    {
           OratioPartesCensus  census_partium;
        OratioResolutioCensus  census;
                 MateriaNodus* doc = _documentum(piscina, &vocabularia,
                     "The cat sat in the house.\n", &census_partium);
        MateriaNodus* adpositio;
        MateriaNodus* sat;
        MateriaNodus* articulus;

        CREDO_NON_NIHIL (doc);
        /* census suffragiis VOCABULORUM (non analysibus: 'in' Latina
         * analyses II fert) - vocabula VI omnia Anglica */
        CREDO_VERUM (strcmp(oratio_resolutio_lingua_censu(
            census_partium.vocabula_linguarum), "anglica") == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (
            census_partium.vocabula_linguarum[ORATIO_LINGUA_ANGLICA],
            (i32)VI);
        CREDO_VERUM (census_partium.linguae[ORATIO_LINGUA_ANGLICA]
            > ZEPHYRUM);
        adpositio  = _vocabulum(doc, (i32)III);
        sat        = _vocabulum(doc, (i32)II);
        articulus  = _vocabulum(doc, ZEPHYRUM);
        CREDO_NON_NIHIL (adpositio);
        CREDO_NON_NIHIL (sat);
        CREDO_NON_NIHIL (articulus);
        /* ordo fontis: Latina prior ubi adest ('in' adpositio ablativa,
         * 'sat' adverbium), Anglica sola ubi sola ('the') */
        CREDO_AEQUALIS_S32 (_lingua(adpositio, ZEPHYRUM),
            (s32)ORATIO_LINGUA_LATINA);
        CREDO_AEQUALIS_S32 (_lingua(sat, ZEPHYRUM),
            (s32)ORATIO_LINGUA_LATINA);
        CREDO_AEQUALIS_S32 (_lingua(articulus, ZEPHYRUM),
            (s32)ORATIO_LINGUA_ANGLICA);
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(
            adpositio->loci[ORATIO_VOCABULUM_ANALYSES]), (i32)VII);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "anglica", doc, &census));
        /* Anglica prima; 'in' ADPOSITIO (lex casus: in\PvAN ante In\N
         * indium), Latinae sequuntur ordine suo, nihil deletum */
        CREDO_AEQUALIS_S32 (_lingua(adpositio, ZEPHYRUM),
            (s32)ORATIO_LINGUA_ANGLICA);
        CREDO_AEQUALIS_S32 (_classis_analysis(adpositio, ZEPHYRUM),
            (s32)ORATIO_CLASSIS_ADPOSITIO);
        CREDO_AEQUALIS_S32 (_lingua(adpositio, I),
            (s32)ORATIO_LINGUA_LATINA);
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(
            adpositio->loci[ORATIO_VOCABULUM_ANALYSES]), (i32)VII);
        CREDO_AEQUALIS_S32 (_lingua(sat, ZEPHYRUM),
            (s32)ORATIO_LINGUA_ANGLICA);
        CREDO_VERUM (_incipit(_compendium(adpositio,
            (i32)ORATIO_VOCABULUM_CLASSES), "adpositio"));
        CREDO_VERUM (_aequalis(_compendium(adpositio,
            (i32)ORATIO_VOCABULUM_LINGUAE), "anglica latina"));
                /* ordo unus per vocabulum Anglicum (VI) a regula linguae + I
         * pro 'in' a regula adpositio-prima-anglica (T19b, prior: prima
         * quae valet vincit); permutatae II (sat, in), ceterae iam
         * primae; in censu regula adpositionis prima, linguae adest */
        CREDO_AEQUALIS_I32 (census.ordines, (i32)VII);
        CREDO_AEQUALIS_I32 (census.recusatae, ZEPHYRUM);
        CREDO_VERUM (census.applicatae >= (i32)II
            && census.applicatae <= (i32)VII);
        CREDO_NON_NIHIL (census.per_regulam);
        si (   census.per_regulam != NIHIL
            && xar_numerus(census.per_regulam) > I)
        {
            constans OratioResolutioRegulaCensus* rc =
                (constans OratioResolutioRegulaCensus*)xar_obtinere(
                census.per_regulam, ZEPHYRUM);
            constans OratioResolutioRegulaCensus* rl =
                (constans OratioResolutioRegulaCensus*)xar_obtinere(
                census.per_regulam, I);

            CREDO_VERUM (_aequalis(rc->titulus,
                "adpositio-prima-anglica"));
            CREDO_VERUM (_aequalis(rl->titulus,
                "lingua-documenti-anglica"));
        }

        /* idempotens */
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "anglica", doc, &census));
        CREDO_AEQUALIS_S32 (_lingua(adpositio, ZEPHYRUM),
            (s32)ORATIO_LINGUA_ANGLICA);
        CREDO_AEQUALIS_I32 (census.applicatae, ZEPHYRUM);
    }
    {
                /* documentum Latinum: regula prima sola (auxiliare-primum-
         * latinum, T19b) sine auxiliari tacet, ordo fontis manet */

           OratioPartesCensus  census_latinum;
        OratioResolutioCensus  census;
                 MateriaNodus* documentum_latinum = _documentum(piscina,
                     &vocabularia, "Cum puella ambulat.\n",
                     &census_latinum);
        MateriaNodus* puella;

        CREDO_NON_NIHIL (documentum_latinum);
        puella = _vocabulum(documentum_latinum, I);
        CREDO_NON_NIHIL (puella);
        CREDO_VERUM (strcmp(oratio_resolutio_lingua_censu(
            census_latinum.vocabula_linguarum), "latina") == ZEPHYRUM);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)I, "latina", documentum_latinum, &census));
                CREDO_AEQUALIS_I32 (census.ordines, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_casus(puella, ZEPHYRUM),
            (s32)ORATIO_CASUS_ABLATIVUS);   /* T24: prior casuum */
    }

        imprimere("\n--- VI. Umbrae capitis, gradus II ---\n");
    {
           OratioPartesCensus census_partium;
        OratioResolutioCensus census;
                 MateriaNodus* doc;
                 MateriaNodus* bona;
                 MateriaNodus* puella;

        /* concordantia nominativa: bona (substantivum crudum primum) ->
         * adiectivum NOM S F per caput Puella */
        doc = _documentum(piscina, &vocabularia,
            "Puella bona ambulat.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        bona = _vocabulum(doc, I);
        CREDO_AEQUALIS_S32 (_classis_analysis(bona, ZEPHYRUM),
            (s32)ORATIO_CLASSIS_SUBSTANTIVUM);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &census));
        CREDO_AEQUALIS_S32 (_classis_analysis(bona, ZEPHYRUM),
            (s32)ORATIO_CLASSIS_ADIECTIVUM);
        /* T27 b: ABLATIVA - regula stricta unita (quaesitio omnes) paria
         * concordantia tria fert (NOM/ABL/VOC S F aeque vicina), politica
         * parium gradu casuum (ORDO_CASUUM_PARIUM, ablativus primus)
         * eligit: thesauris IX melior (nominativo primo 662/644/665 ->
         * 656/634/646), hic falsa - pretium notatum */
        CREDO_AEQUALIS_S32 (_casus(bona, ZEPHYRUM),
            (s32)ORATIO_CASUS_ABLATIVUS);
        CREDO_VERUM (census.impletae >= I);
        /* gradus I solus (regulae XV primae): umbrae capitis absunt -
         * bona substantivum manet */
        doc = _documentum(piscina, &vocabularia,
            "Puella bona ambulat.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)XV, "latina", doc, &census));
        CREDO_AEQUALIS_S32 (_classis_analysis(_vocabulum(doc, I),
            ZEPHYRUM), (s32)ORATIO_CLASSIS_SUBSTANTIVUM);
        CREDO_AEQUALIS_I32 (census.impletae, ZEPHYRUM);
        /* gradus II proiectionem NOVAM videt: cum puella (ablativa
         * gradu I) bona -> ablativa per caput, NON nominativa (lex
         * vocabuli vindicati: lectio prima sola implet) */
        doc = _documentum(piscina, &vocabularia,
            "Cum puella bona ambulat.\n", &census_partium);
        CREDO_NON_NIHIL (doc);
        puella  = _vocabulum(doc, I);
        bona    = _vocabulum(doc, (i32)II);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &census));
        CREDO_AEQUALIS_S32 (_casus(puella, ZEPHYRUM),
            (s32)ORATIO_CASUS_ABLATIVUS);
        CREDO_AEQUALIS_S32 (_classis_analysis(bona, ZEPHYRUM),
            (s32)ORATIO_CLASSIS_ADIECTIVUM);
        CREDO_AEQUALIS_S32 (_casus(bona, ZEPHYRUM),
            (s32)ORATIO_CASUS_ABLATIVUS);
        CREDO_VERUM (census.repetitae >= I);   /* regula NOM recusata */
        CREDO_AEQUALIS_I32 (census.impletae, (i32)II);   /* cum, bona */
        /* determinans per caput: hoc templum (templum GEN P crudum
         * primum - lectio NOM per caput inventa et praelata); hoc solum
         * pronomen manet */
        doc = _documentum(piscina, &vocabularia, "Hoc templum est.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &census));
        CREDO_AEQUALIS_S32 (_classis_analysis(_vocabulum(doc, ZEPHYRUM),
            ZEPHYRUM), (s32)ORATIO_CLASSIS_DETERMINANS);
        /* T27 b: neutrum NOM/ACC aeque vicinum - gradus casuum accusativum
         * ante nominativum ponit (vide 'Puella bona' supra): hoc templum
         * ACCUSATIVA leguntur, pretium notatum */
        CREDO_AEQUALIS_S32 (_casus(_vocabulum(doc, ZEPHYRUM), ZEPHYRUM),
            (s32)ORATIO_CASUS_ACCUSATIVUS);
        CREDO_AEQUALIS_S32 (_casus(_vocabulum(doc, I), ZEPHYRUM),
            (s32)ORATIO_CASUS_ACCUSATIVUS);
        doc = _documentum(piscina, &vocabularia, "Hoc est.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &census));
                /* T19i: hic DET sine exceptione thesauris tribus (CIRCSE CI, LLCT
         * CCCLII; PRON 0) - prior determinantium certorum; umbra vacua
         * manet (inventum) */
        CREDO_AEQUALIS_S32 (_classis_analysis(_vocabulum(doc, ZEPHYRUM),
            ZEPHYRUM), (s32)ORATIO_CLASSIS_DETERMINANS);
        CREDO_AEQUALIS_S32 (_decisio(_vocabulum(doc, ZEPHYRUM)),
            (s32)ORATIO_DECISIO_PRAELATIO);
        CREDO_AEQUALIS_I32 (census.impletae, ZEPHYRUM);
    }


        imprimere("\n--- VII. Lex umbrarum (T19d gamma) ---\n");
    {
           OratioPartesCensus census_partium;
        OratioResolutioCensus census;
                 MateriaNodus* doc;
                 MateriaNodus* verbum;
        constans MateriaNodus* lectio;

        /* promotio super decisionem STRUCTURALEM: gradu I 'in' bonam
         * substantivam ablativam ut obiectum ligat (prima), gradu II
         * caput terra (vicina) lectionem adiectivam implet - lex eam
         * primam facit; ligatio obiecti in substantivum remissa
         * manet */
        doc = _documentum(piscina, &vocabularia, "In bona terra est.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        verbum = _vocabulum(doc, I);
        CREDO_AEQUALIS_S32 (_classis_analysis(verbum, ZEPHYRUM),
            (s32)ORATIO_CLASSIS_SUBSTANTIVUM);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &census));
        CREDO_AEQUALIS_S32 (_classis_analysis(verbum, ZEPHYRUM),
            (s32)ORATIO_CLASSIS_ADIECTIVUM);
        CREDO_VERUM (census.umbris_ordinata >= I);
        lectio = _analysis(verbum, ZEPHYRUM);
        CREDO_NON_NIHIL (lectio);
        CREDO_AEQUALIS_S32 (_umbra_impletio_vocabulum(lectio, ZEPHYRUM),
            (s32)II);
        /* obiectum 'in': umbra ablativa in lectionem bonae quae
         * ABLATIVA est (index per legem remissus) */
        lectio = _analysis(_vocabulum(doc, ZEPHYRUM), ZEPHYRUM);
        CREDO_NON_NIHIL (lectio);
        CREDO_AEQUALIS_S32 (_umbra_impletio_vocabulum(lectio, ZEPHYRUM),
            (s32)I);
        CREDO_AEQUALIS_S32 (_casus(verbum,
            (i32)_umbra_impletio_analysis(lectio, ZEPHYRUM)),
            (s32)ORATIO_CASUS_ABLATIVUS);
        CREDO_VERUM (_umbra_impletio_analysis(lectio, ZEPHYRUM)
            > ZEPHYRUM);
        /* decisio EXPLICITA manet: supra adverbium (exceptio T19b)
         * quamvis umbra obiecti dicto accusativo vicino impleta */
        doc = _documentum(piscina, &vocabularia, "Supra dictum est.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        verbum = _vocabulum(doc, ZEPHYRUM);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &census));
        CREDO_AEQUALIS_S32 (_classis_analysis(verbum, ZEPHYRUM),
            (s32)ORATIO_CLASSIS_ADVERBIUM);
        CREDO_AEQUALIS_S32 (_classis_analysis(verbum, I),
            (s32)ORATIO_CLASSIS_ADPOSITIO);
        lectio = _analysis(verbum, I);
        CREDO_NON_NIHIL (lectio);
        CREDO_AEQUALIS_S32 (_umbra_impletio_vocabulum(lectio, ZEPHYRUM),
            (s32)I);
        /* implens REMOTUS non tollit: bona (obiectum 'in', substantivum
         * prima) lectionem adiectivam ablativam a terra (distantia III)
         * impletam habet, substantivum manet */
        doc = _documentum(piscina, &vocabularia,
            "In bona sunt et terra.\n", &census_partium);
        CREDO_NON_NIHIL (doc);
        verbum = _vocabulum(doc, I);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &census));
        CREDO_AEQUALIS_S32 (_classis_analysis(verbum, ZEPHYRUM),
            (s32)ORATIO_CLASSIS_SUBSTANTIVUM);
        CREDO_AEQUALIS_I32 (census.umbris_ordinata, ZEPHYRUM);
        {
            i32 a;
            i32 remotae = ZEPHYRUM;

            per (a = ZEPHYRUM; a < _numerus_analysium(verbum); a++)
            {
                lectio = _analysis(verbum, a);
                si (   _classis_analysis(verbum, a)
                        == (s32)ORATIO_CLASSIS_ADIECTIVUM
                    && _umbra_impletio_vocabulum(lectio, ZEPHYRUM)
                        == (s32)IV)
                {
                    remotae = remotae + I;
                }
            }
            CREDO_VERUM (remotae >= I);
        }
        /* documentum Anglicum: 'a' determinans (exceptio + regula
         * linguae) manet quamvis adpositio Latina ablativa 'die'
         * ablativo Latino vicino impleta */
        doc = _documentum(piscina, &vocabularia,
            "I saw a die today. The rest is fine.\n", &census_partium);
        CREDO_NON_NIHIL (doc);
        verbum = _vocabulum(doc, (i32)II);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "anglica", doc, &census));
        CREDO_AEQUALIS_S32 (_classis_analysis(verbum, ZEPHYRUM),
            (s32)ORATIO_CLASSIS_DETERMINANS);
        CREDO_AEQUALIS_S32 (_lingua(verbum, ZEPHYRUM),
            (s32)ORATIO_LINGUA_ANGLICA);
        {
            i32 a;
            i32 impletae = ZEPHYRUM;

            per (a = ZEPHYRUM; a < _numerus_analysium(verbum); a++)
            {
                lectio = _analysis(verbum, a);
                si (   _classis_analysis(verbum, a)
                        == (s32)ORATIO_CLASSIS_ADPOSITIO
                    && _umbra_impletio_vocabulum(lectio, ZEPHYRUM)
                        == (s32)III)
                {
                    impletae = impletae + I;
                }
            }
            /* T19e: 'die' per regulam linguae EXPLICITE Anglicum
             * decisum - lex implentis intra gradum umbram Latinam
             * vacuam relinquit (ordo repetitus), 'a' determinans
             * manet */
            CREDO_AEQUALIS_I32 (impletae, ZEPHYRUM);
            CREDO_VERUM (census.repetitae >= I);
        }
        /* remissio indicum: hoc templum - lectio determinantis
         * ACCUSATIVA secunda, umbra eius in lectionem templi
         * ACCUSATIVAM (index post permutationem) */
        doc = _documentum(piscina, &vocabularia, "Hoc templum est.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        verbum = _vocabulum(doc, ZEPHYRUM);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &census));
                /* T19i: hoc prior explicitus (classis), casus per regulam
         * nominativi strictam refinitus (lex classis explicitae) -
         * lectio determinantis ACCUSATIVA quaeritur, non secunda */
        CREDO_AEQUALIS_S32 (_classis_analysis(verbum, ZEPHYRUM),
            (s32)ORATIO_CLASSIS_DETERMINANS);
        /* T27 b: accusativus (neutrum aeque vicinum, gradus casuum) */
        CREDO_AEQUALIS_S32 (_casus(verbum, ZEPHYRUM),
            (s32)ORATIO_CASUS_ACCUSATIVUS);
        {
            i32 a;
            i32 inventa = ZEPHYRUM;

            per (a = ZEPHYRUM; a < _numerus_analysium(verbum); a++)
            {
                si (   _classis_analysis(verbum, a)
                        != (s32)ORATIO_CLASSIS_DETERMINANS
                    || _casus(verbum, a)
                        != (s32)ORATIO_CASUS_ACCUSATIVUS)
                {
                    perge;
                }
                lectio = _analysis(verbum, a);
                si (   lectio != NIHIL
                    && _umbra_impletio_vocabulum(lectio, ZEPHYRUM)
                        == (s32)I
                    && _casus(_vocabulum(doc, I),
                        (i32)_umbra_impletio_analysis(lectio, ZEPHYRUM))
                        == (s32)ORATIO_CASUS_ACCUSATIVUS)
                {
                    inventa = inventa + I;
                }
            }
            CREDO_VERUM (inventa >= I);
        }
        /* CORPUS (Hilarius): umbra quaeque ligata in lectionem casus
         * sui spectat (numero genere quoque ubi scripta) - lex
         * remissionis super sententias veras; ligatae > C */
        {
               chorda  fons;
            character* textus;
            character  via_hilarii[1024];
                  i32  ligatae    = ZEPHYRUM;
                  i32  discordes  = ZEPHYRUM;

            sprintf(via_hilarii,
                "%s/oratio/probationes/fixa/txt/hilarius.txt", radix);
            CREDO_VERUM (_plagulam_legere(piscina, via_hilarii, &fons));
            textus = (character*)piscina_allocare(piscina,
                (memoriae_index)fons.mensura + I);
            CREDO_NON_NIHIL (textus);
            memcpy(textus, fons.datum, (size_t)fons.mensura);
            textus[fons.mensura] = '\0';
            doc = _documentum(piscina, &vocabularia, textus,
                &census_partium);
            CREDO_NON_NIHIL (doc);
            oratio_resolutio_census_vacare(&census);
            CREDO_VERUM (oratio_resolutio_applicare(piscina, intern,
                &ratum, programma, (s32)-I, "latina", doc, &census));
            _umbras_probare(doc, &ligatae, &discordes);
            imprimere("  hilarius: umbrae ligatae %d, discordes %d, "
                "vocabula umbris ordinata %d\n", (integer)ligatae,
                (integer)discordes, (integer)census.umbris_ordinata);
            CREDO_VERUM (ligatae > (i32)C);
            CREDO_AEQUALIS_I32 (discordes, ZEPHYRUM);
            CREDO_VERUM (census.umbris_ordinata >= I);
        }
    }

    imprimere("\n--- VIII. Anglica: nomen proprium capitale, umbrae "
        "per classem (T19e) ---\n");
    {
           OratioPartesCensus census_partium;
        OratioResolutioCensus census;
                 MateriaNodus* doc;
                 MateriaNodus* verbum;
        constans MateriaNodus* lectio;
                          i32  a;

        /* Debra initiale substantivum manet (exclusum), Bush nomen
         * proprium; 'to the' adpositio (umbra particulae vacua), 'to
         * buy' particula umbra verbo proximo impleta, buy verbum */
        doc = _documentum(piscina, &vocabularia,
            "Debra saw Bush go to the store to buy food.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "anglica", doc, &census));
        CREDO_AEQUALIS_S32 (_classis_analysis(_vocabulum(doc, ZEPHYRUM),
            ZEPHYRUM), (s32)ORATIO_CLASSIS_SUBSTANTIVUM);
        CREDO_AEQUALIS_S32 (_classis_analysis(_vocabulum(doc, (i32)II),
            ZEPHYRUM), (s32)ORATIO_CLASSIS_NOMEN_PROPRIUM);
        verbum = _vocabulum(doc, (i32)IV);
        CREDO_AEQUALIS_S32 (_classis_analysis(verbum, ZEPHYRUM),
            (s32)ORATIO_CLASSIS_ADPOSITIO);
        per (a = ZEPHYRUM; a < _numerus_analysium(verbum); a++)
        {
            si (_classis_analysis(verbum, a)
                == (s32)ORATIO_CLASSIS_PARTICULA)
            {
                lectio = _analysis(verbum, a);
                CREDO_AEQUALIS_S32 (_umbra_index(lectio, ZEPHYRUM,
                    (i32)ORATIO_UMBRA_CLASSIS),
                    (s32)ORATIO_CLASSIS_VERBUM);
                CREDO_AEQUALIS_S32 (_umbra_impletio_vocabulum(lectio,
                    ZEPHYRUM), (s32)-I);
            }
        }
        verbum = _vocabulum(doc, (i32)VII);
        CREDO_AEQUALIS_S32 (_classis_analysis(verbum, ZEPHYRUM),
            (s32)ORATIO_CLASSIS_PARTICULA);
        lectio = _analysis(verbum, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_umbra_impletio_vocabulum(lectio, ZEPHYRUM),
            (s32)VIII);
        CREDO_AEQUALIS_S32 (_classis_analysis(_vocabulum(doc,
            (i32)VIII),
            ZEPHYRUM), (s32)ORATIO_CLASSIS_VERBUM);
        CREDO_VERUM (census.impletae >= I);
        /* have + been: auxiliare (implens auxiliare verum ante verbum:
         * been auxiliare manet) */
        doc = _documentum(piscina, &vocabularia,
            "We have been there.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "anglica", doc, &census));
        CREDO_AEQUALIS_S32 (_classis_analysis(_vocabulum(doc, I),
            ZEPHYRUM), (s32)ORATIO_CLASSIS_AUXILIARE);
        CREDO_AEQUALIS_S32 (_umbra_impletio_vocabulum(_analysis(
            _vocabulum(doc, I), ZEPHYRUM), ZEPHYRUM), (s32)II);
        CREDO_AEQUALIS_S32 (_classis_analysis(_vocabulum(doc, (i32)II),
            ZEPHYRUM), (s32)ORATIO_CLASSIS_AUXILIARE);
        /* have a car: 'a' determinans per exceptionem (explicita) -
         * lectio verbi Moby 'a' umbram 'have' non implet (lex implentis
         * intra gradum), have verbum per regulam linguae */
        doc = _documentum(piscina, &vocabularia, "I have a car.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "anglica", doc, &census));
        CREDO_AEQUALIS_S32 (_classis_analysis(_vocabulum(doc, I),
            ZEPHYRUM), (s32)ORATIO_CLASSIS_VERBUM);
        CREDO_AEQUALIS_S32 (_classis_analysis(_vocabulum(doc, (i32)II),
            ZEPHYRUM), (s32)ORATIO_CLASSIS_DETERMINANS);
        CREDO_VERUM (census.repetitae >= I);
        /* contractio: It's auxiliare per priorem contractionum, numquam
         * nomen proprium (regula capitalis: lectio pronominis) */
        doc = _documentum(piscina, &vocabularia, "It's done.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "anglica", doc, &census));
        CREDO_AEQUALIS_S32 (_classis_analysis(_vocabulum(doc, ZEPHYRUM),
            ZEPHYRUM), (s32)ORATIO_CLASSIS_AUXILIARE);
    }

    imprimere("\n--- IX. Implentes casus ignoti (T19f) ---\n");
    {
           OratioPartesCensus census_partium;
        OratioResolutioCensus census;
                 MateriaNodus* doc;
        constans MateriaNodus* lectio;

        /* In Lucca: nomen proprium regulae capitalis (sine casu)
         * umbram ablativam 'in' implet - lectio ablativa prima, Lucca
         * nomen proprium prima */
        doc = _documentum(piscina, &vocabularia, "In Lucca est.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &census));
        CREDO_AEQUALIS_S32 (_classis_analysis(_vocabulum(doc, ZEPHYRUM),
            ZEPHYRUM), (s32)ORATIO_CLASSIS_ADPOSITIO);
        CREDO_AEQUALIS_S32 (_casus(_vocabulum(doc, ZEPHYRUM), ZEPHYRUM),
            (s32)ORATIO_CASUS_ABLATIVUS);
        lectio = _analysis(_vocabulum(doc, ZEPHYRUM), ZEPHYRUM);
        CREDO_NON_NIHIL (lectio);
        CREDO_AEQUALIS_S32 (_umbra_impletio_vocabulum(lectio, ZEPHYRUM),
            (s32)I);
        CREDO_AEQUALIS_S32 (_classis_analysis(_vocabulum(doc, I),
            ZEPHYRUM), (s32)ORATIO_CLASSIS_NOMEN_PROPRIUM);
        CREDO_VERUM (census.impletae >= I);
    }

            imprimere("\n--- XI. Lectio ignoti (T19k) ---\n");
    {
           OratioPartesCensus census_partium;
        OratioResolutioCensus census;
                 MateriaNodus* doc;
        constans MateriaNodus* lectio;

        /* In xyzzy est: vocabulum ignotum lectionem ignoti fert (una,
         * classis ignotum, lingua non scripta) quam umbra obiecti 'in'
         * implet (regula ignoti); decisio in xyzzy nulla (lectio una) */
        doc = _documentum(piscina, &vocabularia, "In xyzzy est.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        CREDO_AEQUALIS_I32 (_numerus_analysium(_vocabulum(doc, I)), I);
        CREDO_AEQUALIS_S32 (_classis_analysis(_vocabulum(doc, I),
            ZEPHYRUM),
            (s32)ORATIO_CLASSIS_IGNOTUM);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &census));
        lectio = _analysis(_vocabulum(doc, ZEPHYRUM), ZEPHYRUM);
        CREDO_NON_NIHIL (lectio);
        CREDO_AEQUALIS_S32 (_umbra_impletio_vocabulum(lectio, ZEPHYRUM),
            (s32)I);
        CREDO_AEQUALIS_S32 (_umbra_impletio_analysis(lectio, ZEPHYRUM),
            ZEPHYRUM);
        CREDO_VERUM (_aequalis(_compendium(_vocabulum(doc, I),
            (i32)ORATIO_VOCABULUM_CLASSES), "ignotum"));
        CREDO_AEQUALIS_S32 (_decisio(_vocabulum(doc, I)), (s32)-I);
        CREDO_VERUM (census.impletae >= I);
    }

    imprimere("\n--- X. Decisio in arbore (T19g, decretum SUDOKU) ---\n");
    {
           OratioPartesCensus census_partium;
        OratioResolutioCensus census;
                 MateriaNodus* doc;
                 MateriaNodus* verbum;

        /* Puella bona ambulat: bona (carrier) et Puella (implens) per
         * ordinem IMPLETIONIS decisae, auctor = titulus regulae capitis
         * nominativi praecedentis; ambulat a nemine (loci non scripti) */
        doc = _documentum(piscina, &vocabularia,
            "Puella bona ambulat.\n", &census_partium);
        CREDO_NON_NIHIL (doc);
        verbum = _vocabulum(doc, I);
        CREDO_AEQUALIS_S32 (_decisio(verbum), (s32)-I);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &census));
        CREDO_AEQUALIS_S32 (_decisio(verbum),
            (s32)ORATIO_DECISIO_IMPLETIO);
                CREDO_VERUM (_aequalis(_compendium(verbum,
                    (i32)ORATIO_VOCABULUM_AUCTOR),
                    "umbra-caput-praecedente-proximo"));
        CREDO_AEQUALIS_S32 (_decisio(_vocabulum(doc, ZEPHYRUM)),
            (s32)ORATIO_DECISIO_IMPLETIO);
        CREDO_AEQUALIS_S32 (_decisio(_vocabulum(doc, (i32)II)),
            (s32)-I);
        CREDO_AEQUALIS_S32 ((s32)_vocabulum(doc,
            (i32)II)->loci[ORATIO_VOCABULUM_AUCTOR].genus,
            (s32)MATERIA_VALOR_NIHIL);
        CREDO_AEQUALIS_I32 (census.decisae[ORATIO_DECISIO_IMPLETIO],
            (i32)II);
        CREDO_AEQUALIS_I32 (census.decisae[ORATIO_DECISIO_PRAELATIO],
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (census.decisae[ORATIO_DECISIO_UMBRA],
            ZEPHYRUM);
        /* cursus alter idem reponit: decisio immota */
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &census));
        CREDO_AEQUALIS_S32 (_decisio(verbum),
            (s32)ORATIO_DECISIO_IMPLETIO);
        CREDO_AEQUALIS_I32 (census.decisae[ORATIO_DECISIO_IMPLETIO],
            (i32)II);
        /* proiectio: attributum decisio= per uncum nodi, locus auctor */
        {
                OratioStmlOrnatus ornatus;
            MateriaArborConsilium consilium;
            MateriaArborScriptura scriptura;

            oratio_stml_consilium_ornatum(&consilium, &ratum, &ornatus,
                "latina");
            scriptura = materia_arbor_scribere_nodum(piscina, doc,
                &consilium);
            CREDO_VERUM (scriptura.successus);
            CREDO_NON_NIHIL (strstr(
                (constans character*)scriptura.textus.datum,
                "decisio=\"impletio\""));
            CREDO_NON_NIHIL (strstr(
                (constans character*)scriptura.textus.datum,
                "<auctor"));
        }
                /* In bona terra est: bona per LEGEM umbrarum (auctor
         * lex-umbrarum, actus decisivus ultimus super impletionem
         * gradus I), terra per impletionem capitis, est per
         * praelationem (prior auxiliaris). INVENTUM T19g: in per
         * PRAELATIONEM prioris adpositionis Latinae (index X), non per
         * impletionem obiecti (XVIII) - prior Latinus ante testimonium
         * loquitur (ordo Anglicus inversus est); mensurandum */
        doc = _documentum(piscina, &vocabularia, "In bona terra est.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &census));
        CREDO_AEQUALIS_S32 (_decisio(_vocabulum(doc, I)),
            (s32)ORATIO_DECISIO_UMBRA);
        CREDO_VERUM (_aequalis(_compendium(_vocabulum(doc, I),
            (i32)ORATIO_VOCABULUM_AUCTOR), "lex-umbrarum"));
                CREDO_AEQUALIS_S32 (_decisio(_vocabulum(doc, ZEPHYRUM)),
                    (s32)ORATIO_DECISIO_PRAELATIO);
        CREDO_VERUM (_aequalis(_compendium(_vocabulum(doc, ZEPHYRUM),
            (i32)ORATIO_VOCABULUM_AUCTOR), "adpositio-prima-latina"));
        CREDO_AEQUALIS_S32 (_decisio(_vocabulum(doc, (i32)II)),
            (s32)ORATIO_DECISIO_IMPLETIO);
        CREDO_AEQUALIS_S32 (_decisio(_vocabulum(doc, (i32)III)),
            (s32)ORATIO_DECISIO_PRAELATIO);
        CREDO_VERUM (_aequalis(_compendium(_vocabulum(doc, (i32)III),
            (i32)ORATIO_VOCABULUM_AUCTOR), "auxiliare-primum-latinum"));
        CREDO_AEQUALIS_I32 (census.decisae[ORATIO_DECISIO_UMBRA], I);
        /* Supra dictum est: exceptio = praelatio auctore suo */
        doc = _documentum(piscina, &vocabularia, "Supra dictum est.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &census));
        CREDO_AEQUALIS_S32 (_decisio(_vocabulum(doc, ZEPHYRUM)),
            (s32)ORATIO_DECISIO_PRAELATIO);
        CREDO_VERUM (_aequalis(_compendium(_vocabulum(doc, ZEPHYRUM),
            (i32)ORATIO_VOCABULUM_AUCTOR), "supra-adverbium-latinum"));
    }

    imprimere("\n");
    credo_imprimere_compendium();


    {
        b32 praeteritus = credo_omnia_praeterierunt();

        credo_claudere();
        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
