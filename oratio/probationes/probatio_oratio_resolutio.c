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
    CREDO_VERUM (xar_numerus(programma->regulae) >= (i32)II);
    CREDO_VERUM (_aequalis(((constans OratioRegula*)xar_obtinere(
        programma->regulae, ZEPHYRUM))->titulus,
        "adpositio-accusativum-regit"));
    CREDO_VERUM (_aequalis(((constans OratioRegula*)xar_obtinere(
        programma->regulae, I))->titulus, "adpositio-ablativum-regit"));
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
            oratio_resolutio_lingua_censu(census_partium.linguae), doc,
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
        CREDO_AEQUALIS_I32 (census.ordines, I);
        CREDO_AEQUALIS_I32 (census.applicatae, I);
        CREDO_AEQUALIS_I32 (census.recusatae, ZEPHYRUM);
        CREDO_NON_NIHIL (census.per_regulam);
        CREDO_AEQUALIS_I32 (census.per_regulam == NIHIL ? ZEPHYRUM
            : xar_numerus(census.per_regulam), I);
        si (census.per_regulam != NIHIL)
        {
            constans OratioResolutioRegulaCensus* r =
                (constans OratioResolutioRegulaCensus*)xar_obtinere(
                census.per_regulam, ZEPHYRUM);

            CREDO_VERUM (_aequalis(r->titulus,
                "adpositio-ablativum-regit"));
            CREDO_AEQUALIS_I32 (r->numerus, I);
        }
        /* idempotens: cursus alter nihil mutat */
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &census));
        CREDO_AEQUALIS_I32 (census.ordines, I);
        CREDO_AEQUALIS_I32 (census.applicatae, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_casus(puella, ZEPHYRUM),
            (s32)ORATIO_CASUS_ABLATIVUS);
        /* regulae 0 = nihil */
        doc = _documentum(piscina, &vocabularia,
            "Cum puella ambulat.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, ZEPHYRUM, "latina", doc, &census));
        CREDO_AEQUALIS_S32 (_casus(_vocabulum(doc, I), ZEPHYRUM),
            (s32)ORATIO_CASUS_NOMINATIVUS);
        CREDO_AEQUALIS_I32 (census.ordines, ZEPHYRUM);
        /* regula prima sola (accusativum): cum puella intacta */
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, I, "latina", doc, &census));
        CREDO_AEQUALIS_S32 (_casus(_vocabulum(doc, I), ZEPHYRUM),
            (s32)ORATIO_CASUS_NOMINATIVUS);
        CREDO_AEQUALIS_I32 (census.ordines, ZEPHYRUM);
        /* In urbem venit: accusativus iam primus - ordo lectus, nihil
         * permutatum */
        doc = _documentum(piscina, &vocabularia, "In urbem venit.\n",
            &census_partium);
        CREDO_NON_NIHIL (doc);
        oratio_resolutio_census_vacare(&census);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &census));
        CREDO_AEQUALIS_I32 (census.ordines, I);
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
        CREDO_AEQUALIS_S32 (_casus(_vocabulum(doc, ZEPHYRUM), ZEPHYRUM),
            (s32)ORATIO_CASUS_NOMINATIVUS);
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
             * documentum totum iudicatum: 0 vitia */
            constans OratioRegula* r0 =
                (constans OratioRegula*)xar_obtinere(programma->regulae,
                ZEPHYRUM);
            constans OratioRegula* r1 =
                (constans OratioRegula*)xar_obtinere(programma->regulae,
                I);
            i32 mensura = scriptura.textus.mensura + r0->textus.mensura
                + r1->textus.mensura + (i32)III;
            character* textus = (character*)piscina_allocare(piscina,
                (memoriae_index)mensura + I);
                          chorda  fons;
                    StmlResultus  lectio;
            StmlExpansioResultus  expansio;
                             Xar* vitia;

            CREDO_NON_NIHIL (textus);
            sprintf(textus, "%.*s\n%.*s\n%.*s\n",
                (integer)scriptura.textus.mensura,
                (constans character*)scriptura.textus.datum,
                (integer)r0->textus.mensura,
                (constans character*)r0->textus.datum,
                (integer)r1->textus.mensura,
                (constans character*)r1->textus.datum);
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

                CREDO_VERUM (_continet(expansa, "<praelatio"));
                CREDO_VERUM (_continet(expansa,
                    "regula=\"adpositio-ablativum-regit\""));
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

    imprimere("\n");
    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        credo_claudere();
        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
