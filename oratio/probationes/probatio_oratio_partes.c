/* probatio_oratio_partes.c - Annotatio vocabulorum (T12)
 *
 * I.   DESCRIPTIONES ex WORDS/glossario (oratio_partes_la): tabula
 *      codicum (casus numerus genus persona tempus vox modus gradus
 *      species), formae XX contra decisionem accidentium: puellam
 *      (substantivum ACC S F decl I, nativum "N 1 1 ACC S C"), Roma et
 *      Caesar (nomen proprium: kind L/N), amat (verbum III S praes ind
 *      act, coni. I), amavit (perfectum), amatus (participium perf pass
 *      + adiectivum), amandi (gerundivum), amare (infinitivum +
 *      adiectivum + adverbium), loquitur (deponens), bonus/melior/optime
 *      (gradus), in (adpositio ABL et ACC), et/ut (coniunctiones),
 *      tres (numerale cardinale), ego/hic (pronomen), virumque (tackon
 *      coordinans + substantiva), agantur (forma unica: subiunctivus
 *      passiva III P coni. III), est (glossarium: sum III S praes ind
 *      act, fons glossarium), offset (ignotum-permissum -> ignotum,
 *      lingua anglica).
 * II.  ANNOTATIO arboris "Puella amat. Xyzzy virumque.": analyses ordine
 *      fontis (nodi generum analysis-*, loci communes + accidentia),
 *      classes/linguae derivata, ignotum sine analysi cum classes
 *      'ignotum', semel scribitur (cursus secundus nihil agit), census.
 * III. PROIECTIO STML: attributa classes/linguae in <vocabulum> (uncus
 *      nodi materiae), elementa analysis-*, circuitus scribere-legere-
 *      scribere octetim + STRUCTURALIS + FIDELIS, canon oratio 0 vitia.
 * IV.  SELECTIO super documentum: vocabulum[classes~=verbum] I,
 *      [classes~=substantivum] II, [linguae~=latina] III,
 *      [classes~=ignotum] I, analysis-verbi >= I.
 * V.   CORPUS: fixturae Latinae annotatae (vocabula, analyses, ignota,
 *      classes, ms) - limina (ignota < XII %, analyses >= vocabula);
 *      circuitus arboris annotatae (Hilarius).
 * Culpa plantata: genus stirpis N ignoratum (puellam genus C).
 */

#include "latina.h"
#include "credo.h"
#include "canon.h"
#include "internamentum.h"
#include "oratio_arbor.h"
#include "oratio_stml.h"
#include "oratio_lexicon.h"
#include "oratio_registrum.h"
#include "oratio_partes.h"
#include "oratio_partes_en.h"
#include "oratio_partes_la.h"
#include "oratio_vocabularium.h"
#include "oratio_vocabularium_la.h"
#include "oratio_glossarium.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "selectio.h"
#include "stml.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

/* descriptiones formae (omnes analyses) */
interior Xar*
_describere (
                           Piscina* piscina,
     constans OratioVocabulariumLa* voc,
                constans character* forma)
{
    Xar* analyses = oratio_vocabularium_la_quaerere(piscina, voc,
        _l(forma));
    Xar* exitus = xar_creare(piscina,
        (i32)magnitudo(OratioDescriptio));
    i32 k;

    si (analyses == NIHIL || exitus == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(analyses); k++)
    {
        si (!oratio_partes_la_describere(piscina, voc,
                (constans OratioAnalysis*)xar_obtinere(analyses, k),
                exitus))
        {
            redde NIHIL;
        }
    }
    redde exitus;
}

/* descriptio prima classis datae, NIHIL si nulla */
interior constans OratioDescriptio*
_prima (
              Xar* x,
    OratioClassis  classis)
{
    i32 k;

    per (k = ZEPHYRUM; x != NIHIL && k < xar_numerus(x); k++)
    {
        constans OratioDescriptio* d =
            (constans OratioDescriptio*)xar_obtinere(x, k);

        si (d->classis == classis)
        {
            redde d;
        }
    }
    redde NIHIL;
}

/* descriptio prima classis cum forma verbi data */
interior constans OratioDescriptio*
_prima_formae (
              Xar* x,
    OratioClassis  classis,
              s32  forma_verbi)
{
    i32 k;

    per (k = ZEPHYRUM; x != NIHIL && k < xar_numerus(x); k++)
    {
        constans OratioDescriptio* d =
            (constans OratioDescriptio*)xar_obtinere(x, k);

        si (d->classis == classis && d->forma_verbi == forma_verbi)
        {
            redde d;
        }
    }
    redde NIHIL;
}

interior b32
_habet (
              Xar* x,
    OratioClassis  classis)
{
    redde (b32)(_prima(x, classis) != NIHIL);
}

/* vocabulum e-tum sententiae s paragraphi p documenti */
interior MateriaNodus*
_vocabulum (
    MateriaNodus* doc,
             i32  p,
             i32  s,
             i32  e)
{
    MateriaNodus* par = materia_valor_lista_obtinere(
        doc->loci[ORATIO_DOCUMENTUM_PARAGRAPHI], p)->datum.nodus;
    MateriaNodus* sen = materia_valor_lista_obtinere(
        par->loci[ORATIO_PARAGRAPHUS_SENTENTIAE], s)->datum.nodus;
    MateriaNodus* el  = materia_valor_lista_obtinere(
        sen->loci[ORATIO_SENTENTIA_ELEMENTA], e)->datum.nodus;

    redde el;
}

interior i32
_numerus_analysium (
    constans MateriaNodus* v)
{
    constans MateriaValor* a = &v->loci[ORATIO_VOCABULUM_ANALYSES];

    si (a->genus != MATERIA_VALOR_LISTA)
    {
        redde ZEPHYRUM;
    }
    redde materia_valor_lista_numerus(*a);
}

interior chorda
_derivatum (
    constans MateriaNodus* n,
                      i32  locus)
{
    constans MateriaValor* v = &n->loci[locus];
                   chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (v->genus != MATERIA_VALOR_TOKEN || v->datum.token == NIHIL)
    {
        redde vacua;
    }
    redde v->datum.token->valor;
}

interior s32
_index (
    constans MateriaNodus* n,
                      i32  locus)
{
    constans MateriaValor* v = &n->loci[locus];

    si (v->genus != MATERIA_VALOR_INDEX)
    {
        redde (s32)-I;
    }
    redde v->datum.index;
}

/* Circuitus plenus super arborem datam (ut probatio_oratio_stml). */
interior b32
_circuitum_arboris (
                           Piscina* piscina,
    constans MateriaArborConsilium* consilium,
                      MateriaNodus* radix,
                               b32  fidelis)
{
    MateriaNodus* lecta;
    MateriaArborScriptura s1;
    MateriaArborScriptura s2;
    MateriaArborVitium vitium;
    MateriaArborDifferentia d;

    s1 = materia_arbor_scribere_nodum(piscina, radix, consilium);
    si (!s1.successus)
    {
        imprimere("    (scriptura I: %s)\n", s1.causa ? s1.causa : "-");
        redde FALSUM;
    }
    lecta = materia_arbor_legere(piscina, NIHIL, s1.textus, consilium,
        &vitium);
    si (lecta == NIHIL)
    {
        imprimere("    (lectio I: %s, linea %d)\n",
            vitium.causa ? vitium.causa : "-", (integer)vitium.linea);
        redde FALSUM;
    }
    s2 = materia_arbor_scribere_nodum(piscina, lecta, consilium);
    si (   !s2.successus || s1.textus.mensura != s2.textus.mensura
        || memcmp(s1.textus.datum, s2.textus.datum,
            (size_t)s1.textus.mensura) != ZEPHYRUM)
    {
        imprimere("    (documenta I/II dispari)\n");
        redde FALSUM;
    }
    si (!materia_arbor_aequalis(radix, lecta,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d))
    {
        imprimere("    (arbor dispar: %s @ %s)\n",
            d.campus ? d.campus : "-", d.via);
        redde FALSUM;
    }
    si (   fidelis && !materia_arbor_aequalis(radix, lecta,
            MATERIA_ARBOR_COMPARATIO_FIDELITAS, &d))
    {
        imprimere("    (fidelitas dispar: %s @ %s)\n",
            d.campus ? d.campus : "-", d.via);
        redde FALSUM;
    }
    redde VERUM;
}

/* descriptiones formae Anglicae: analyses Moby + regulae, deinde
 * lectiones secundariae listarum (T16) */
interior Xar*
_describere_en (
                          Piscina* piscina,
    constans OratioVocabulariumEn* en,
               constans character* forma)
{
    Xar* analyses = oratio_vocabularium_en_analysare(piscina, en,
        _l(forma));
    Xar* exitus = xar_creare(piscina, (i32)magnitudo(OratioDescriptio));
    i32  k;

    si (analyses == NIHIL || exitus == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(analyses); k++)
    {
        si (!oratio_partes_en_describere(piscina, en,
                (constans OratioAnalysisEn*)xar_obtinere(analyses, k),
                exitus))
        {
            redde NIHIL;
        }
    }
    si (!oratio_partes_en_secundariae(piscina, _l(forma), exitus))
    {
        redde NIHIL;
    }
    redde exitus;
}

/* descriptio prima classis datae; NIHIL si absens */
interior constans OratioDescriptio*
_descriptio_en (
              Xar* x,
    OratioClassis  classis)
{
    i32 k;

    si (x == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(x); k++)
    {
        constans OratioDescriptio* d =
            (constans OratioDescriptio*)xar_obtinere(x, k);

        si (d->classis == classis)
        {
            redde d;
        }
    }
    redde NIHIL;
}

s32
principale (vacuum)
{
                                     Piscina* piscina;
                          constans character* radix;
                                   character  via[1024];
                           OratioVocabularia  vocabularia;
               constans OratioVocabulariumLa* voc;
                    OratioVocabulariumVitium  vitium;
                         MateriaLexiconRatum  ratum;
                          MateriaLexIudicium  iudicium;
                       MateriaArborConsilium  consilium;
                         InternamentumChorda* intern;

    piscina = piscina_generare_dynamicum("probatio_oratio_partes",
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
    voc = vocabularia.la;
    CREDO_NON_NIHIL (voc);
    CREDO_NON_NIHIL (vocabularia.en);
    si (voc == NIHIL || vocabularia.en == NIHIL)
    {
        credo_imprimere_compendium();
        redde I;
    }
    CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &ORATIO_LEXICON,
        &iudicium));
    oratio_stml_consilium(&consilium, &ratum);
    intern = internamentum_creare(piscina);
    CREDO_NON_NIHIL (intern);

    imprimere("\n--- I. Descriptiones ex WORDS et glossario ---\n");
    CREDO_AEQUALIS_S32 (oratio_partes_la_casus(_l("NOM")),
        (s32)ORATIO_CASUS_NOMINATIVUS);
    CREDO_AEQUALIS_S32 (oratio_partes_la_casus(_l("VOC")),
        (s32)ORATIO_CASUS_VOCATIVUS);
    CREDO_AEQUALIS_S32 (oratio_partes_la_casus(_l("LOC")),
        (s32)ORATIO_CASUS_LOCATIVUS);
    CREDO_AEQUALIS_S32 (oratio_partes_la_casus(_l("X")), (s32)-I);
    CREDO_AEQUALIS_S32 (oratio_partes_la_tempus(_l("FUTP")),
        (s32)ORATIO_TEMPUS_FUTURUM_EXACTUM);
    CREDO_AEQUALIS_S32 (oratio_partes_la_persona(_l("0")), (s32)-I);
    CREDO_AEQUALIS_S32 (oratio_partes_la_species(_l("ADVERB")),
        (s32)ORATIO_SPECIES_NUMERALIS_ADVERBIALE);
    CREDO_AEQUALIS_S32 (oratio_partes_index_tituli(ORATIO_TITULI_CASUUM,
        (i32)ORATIO_CASUS_NUMERUS, _l("genitivus")),
        (s32)ORATIO_CASUS_GENITIVUS);
    CREDO_AEQUALIS_S32 (oratio_partes_index_tituli(ORATIO_TITULI_PERSONARUM,
        (i32)ORATIO_PERSONA_NUMERUS, _l("III")),
        (s32)ORATIO_PERSONA_TERTIA);
    {
                              Xar* x;
        constans OratioDescriptio* d;

        x = _describere(piscina, voc, "puellam");
        CREDO_NON_NIHIL (x);
        d = _prima(x, ORATIO_CLASSIS_SUBSTANTIVUM);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_AEQUALIS_S32 (d->casus_grammaticus,
                (s32)ORATIO_CASUS_ACCUSATIVUS);
            CREDO_AEQUALIS_S32 (d->numerus,
                (s32)ORATIO_NUMERUS_GRAMMATICUS_SINGULARIS);
            CREDO_AEQUALIS_S32 (d->genus,
                (s32)ORATIO_GENUS_GRAMMATICUM_FEMININUM);   /* stirps F */
            CREDO_AEQUALIS_S32 (d->declinatio, (s32)I);
            CREDO_VERUM (_aequalis(d->lemma, "puella"));
            CREDO_VERUM (_aequalis(d->nativum, "N 1 1 ACC S C"));
            CREDO_AEQUALIS_S32 ((s32)d->fons,
                (s32)ORATIO_FONS_ANALYSIS_VOCABULARIUM_LA);
            CREDO_AEQUALIS_S32 ((s32)d->lingua,
                (s32)ORATIO_LINGUA_LATINA);
            CREDO_VERUM (d->sensus.mensura > ZEPHYRUM);
            CREDO_AEQUALIS_S32 (d->persona, (s32)-I);
        }
        x = _describere(piscina, voc, "Roma");
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_NOMEN_PROPRIUM));   /* kind L */
        CREDO_FALSUM (_habet(x, ORATIO_CLASSIS_SUBSTANTIVUM));
        x = _describere(piscina, voc, "Caesar");
        d = _prima(x, ORATIO_CLASSIS_NOMEN_PROPRIUM);            /* kind N */
        CREDO_NON_NIHIL (d);
        CREDO_VERUM (d != NIHIL
            && d->genus == (s32)ORATIO_GENUS_GRAMMATICUM_MASCULINUM);
        x = _describere(piscina, voc, "vir");
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_SUBSTANTIVUM));   /* kind P */
        x = _describere(piscina, voc, "amat");
        d = _prima(x, ORATIO_CLASSIS_VERBUM);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_AEQUALIS_S32 (d->persona, (s32)ORATIO_PERSONA_TERTIA);
            CREDO_AEQUALIS_S32 (d->numerus,
                (s32)ORATIO_NUMERUS_GRAMMATICUS_SINGULARIS);
            CREDO_AEQUALIS_S32 (d->tempus, (s32)ORATIO_TEMPUS_PRAESENS);
            CREDO_AEQUALIS_S32 (d->modus,
                (s32)ORATIO_MODUS_INDICATIVUS);
            CREDO_AEQUALIS_S32 (d->vox, (s32)ORATIO_VOX_ACTIVA);
            CREDO_AEQUALIS_S32 (d->forma_verbi,
                (s32)ORATIO_FORMA_VERBI_FINITUM);
            CREDO_AEQUALIS_S32 (d->coniugatio, (s32)I);
            CREDO_AEQUALIS_S32 (d->casus_grammaticus, (s32)-I);
            CREDO_VERUM (_aequalis(d->lemma, "amo"));
            CREDO_VERUM (_aequalis(d->nativum,
                "V 1 1 PRES ACTIVE IND 3 S"));
        }
        x = _describere(piscina, voc, "amavit");
        d = _prima(x, ORATIO_CLASSIS_VERBUM);
        CREDO_VERUM (d != NIHIL
            && d->tempus == (s32)ORATIO_TEMPUS_PERFECTUM);
        x = _describere(piscina, voc, "amatus");
        d = _prima_formae(x, ORATIO_CLASSIS_VERBUM,
            (s32)ORATIO_FORMA_VERBI_PARTICIPIUM);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_AEQUALIS_S32 (d->tempus,
                (s32)ORATIO_TEMPUS_PERFECTUM);
            CREDO_AEQUALIS_S32 (d->vox, (s32)ORATIO_VOX_PASSIVA);
            CREDO_AEQUALIS_S32 (d->casus_grammaticus,
                (s32)ORATIO_CASUS_NOMINATIVUS);
            CREDO_AEQUALIS_S32 (d->genus,
                (s32)ORATIO_GENUS_GRAMMATICUM_MASCULINUM);
            CREDO_AEQUALIS_S32 (d->persona, (s32)-I);
        }
        d = _prima(x, ORATIO_CLASSIS_ADIECTIVUM);
        CREDO_VERUM (d != NIHIL
            && d->gradus == (s32)ORATIO_GRADUS_POSITIVUS);
        x = _describere(piscina, voc, "amandi");
        CREDO_NON_NIHIL (_prima_formae(x, ORATIO_CLASSIS_VERBUM,
            (s32)ORATIO_FORMA_VERBI_GERUNDIVUM));
        x = _describere(piscina, voc, "amare");
        d = _prima_formae(x, ORATIO_CLASSIS_VERBUM,
            (s32)ORATIO_FORMA_VERBI_INFINITIVUM);
        CREDO_NON_NIHIL (d);
        CREDO_VERUM (d != NIHIL
            && d->modus == (s32)ORATIO_MODUS_INFINITIVUS
            && d->persona == (s32)-I);
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_ADIECTIVUM));
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_ADVERBIUM));
        x = _describere(piscina, voc, "loquitur");
        d = _prima(x, ORATIO_CLASSIS_VERBUM);
        CREDO_VERUM (d != NIHIL && d->vox == (s32)ORATIO_VOX_DEPONENS
            && d->coniugatio == (s32)III);
        x = _describere(piscina, voc, "bonus");
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_SUBSTANTIVUM));
        d = _prima(x, ORATIO_CLASSIS_ADIECTIVUM);
        CREDO_VERUM (d != NIHIL
            && d->gradus == (s32)ORATIO_GRADUS_POSITIVUS
            && d->declinatio == (s32)I);
        x = _describere(piscina, voc, "melior");
        d = _prima(x, ORATIO_CLASSIS_ADIECTIVUM);
        CREDO_VERUM (d != NIHIL
            && d->gradus == (s32)ORATIO_GRADUS_COMPARATIVUS
            && _aequalis(d->lemma, "bonus"));
        x = _describere(piscina, voc, "optime");
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_ADVERBIUM));
        d = _prima(x, ORATIO_CLASSIS_ADIECTIVUM);
        CREDO_VERUM (d != NIHIL
            && d->gradus == (s32)ORATIO_GRADUS_SUPERLATIVUS);
        x = _describere(piscina, voc, "in");
        CREDO_NON_NIHIL (x);
        CREDO_AEQUALIS_I32 (xar_numerus(x), (i32)II);
        {
            i32 k;
            b32 ablativus_adest    = FALSUM;
            b32 accusativus_adest  = FALSUM;

            per (k = ZEPHYRUM; x != NIHIL && k < xar_numerus(x); k++)
            {
                constans OratioDescriptio* descriptio =
                    (constans OratioDescriptio*)xar_obtinere(x, k);

                CREDO_AEQUALIS_S32 ((s32)descriptio->classis,
                    (s32)ORATIO_CLASSIS_ADPOSITIO);
                ablativus_adest = (b32)(ablativus_adest
                    || descriptio->casus_grammaticus
                    == (s32)ORATIO_CASUS_ABLATIVUS);
                accusativus_adest = (b32)(accusativus_adest
                    || descriptio->casus_grammaticus
                    == (s32)ORATIO_CASUS_ACCUSATIVUS);
            }
            CREDO_VERUM (ablativus_adest && accusativus_adest);
        }
        x = _describere(piscina, voc, "et");
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_CONIUNCTIO_COORDINANS));
        CREDO_FALSUM (_habet(x, ORATIO_CLASSIS_CONIUNCTIO_SUBORDINANS));
        x = _describere(piscina, voc, "ut");
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_CONIUNCTIO_SUBORDINANS));
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_VERBUM));
        x = _describere(piscina, voc, "tres");
        d = _prima(x, ORATIO_CLASSIS_NUMERALE);
        CREDO_VERUM (d != NIHIL
            && d->species == (s32)ORATIO_SPECIES_NUMERALIS_CARDINALE
            && d->numerus == (s32)ORATIO_NUMERUS_GRAMMATICUS_PLURALIS);
        x = _describere(piscina, voc, "ego");
        d = _prima(x, ORATIO_CLASSIS_PRONOMEN);
        CREDO_VERUM (d != NIHIL
            && d->casus_grammaticus == (s32)ORATIO_CASUS_NOMINATIVUS
            && d->numerus
                == (s32)ORATIO_NUMERUS_GRAMMATICUS_SINGULARIS);
        x = _describere(piscina, voc, "hic");
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_PRONOMEN));
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_ADVERBIUM));
        x = _describere(piscina, voc, "virumque");
        d = _prima(x, ORATIO_CLASSIS_CONIUNCTIO_COORDINANS);
        CREDO_NON_NIHIL (d);
        CREDO_VERUM (d != NIHIL && _aequalis(d->nativum, "TACKON que")
            && _aequalis(d->lemma, "que"));
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_SUBSTANTIVUM));
        x = _describere(piscina, voc, "agantur");
        d = _prima(x, ORATIO_CLASSIS_VERBUM);   /* forma unica prima */
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_AEQUALIS_S32 (d->modus,
                (s32)ORATIO_MODUS_SUBIUNCTIVUS);
            CREDO_AEQUALIS_S32 (d->vox, (s32)ORATIO_VOX_PASSIVA);
            CREDO_AEQUALIS_S32 (d->persona, (s32)ORATIO_PERSONA_TERTIA);
            CREDO_AEQUALIS_S32 (d->numerus,
                (s32)ORATIO_NUMERUS_GRAMMATICUS_PLURALIS);
            CREDO_AEQUALIS_S32 (d->coniugatio, (s32)III);
            CREDO_VERUM (d->nativum.mensura > (i32)20
                && memcmp(d->nativum.datum,
                "V 3 1 PRES PASSIVE SUB 3 P",
                    (size_t)26) == ZEPHYRUM);
        }
        x = _describere(piscina, voc, "est");
        d = _prima(x, ORATIO_CLASSIS_VERBUM);   /* glossarium primum: sum */
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_VERUM (_aequalis(d->lemma, "sum"));
            CREDO_AEQUALIS_S32 ((s32)d->fons,
                (s32)ORATIO_FONS_ANALYSIS_GLOSSARIUM);
            CREDO_AEQUALIS_S32 (d->persona, (s32)ORATIO_PERSONA_TERTIA);
            CREDO_AEQUALIS_S32 (d->numerus,
                (s32)ORATIO_NUMERUS_GRAMMATICUS_SINGULARIS);
            CREDO_AEQUALIS_S32 (d->tempus, (s32)ORATIO_TEMPUS_PRAESENS);
            CREDO_AEQUALIS_S32 (d->modus,
                (s32)ORATIO_MODUS_INDICATIVUS);
            CREDO_AEQUALIS_S32 (d->vox, (s32)ORATIO_VOX_ACTIVA);
            CREDO_AEQUALIS_S32 (d->forma_verbi,
                (s32)ORATIO_FORMA_VERBI_FINITUM);
            CREDO_VERUM (_aequalis(d->nativum, "glossarium verbum"));
        }
        x = _describere(piscina, voc, "offset");
        d = _prima(x, ORATIO_CLASSIS_IGNOTUM);   /* ignotum-permissum */
        CREDO_NON_NIHIL (d);
        CREDO_VERUM (d != NIHIL && d->lingua == ORATIO_LINGUA_ANGLICA
            && d->fons == ORATIO_FONS_ANALYSIS_GLOSSARIUM);
                x = _describere(piscina, voc, "xyzzy");
        CREDO_NON_NIHIL (x);
        CREDO_AEQUALIS_I32 (xar_numerus(x), ZEPHYRUM);
        /* regulae secundariae ex oraculo (T13): copia post primariam */
        x = _describere(piscina, voc, "est");
        CREDO_VERUM (_prima(x, ORATIO_CLASSIS_VERBUM) != NIHIL);
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_AUXILIARE));
        CREDO_AEQUALIS_S32 ((s32)((constans OratioDescriptio*)xar_obtinere(x,
            ZEPHYRUM))->classis, (s32)ORATIO_CLASSIS_VERBUM);
        x = _describere(piscina, voc, "hic");
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_DETERMINANS));
        x = _describere(piscina, voc, "meas");
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_DETERMINANS));
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_ADIECTIVUM));
        x = _describere(piscina, voc, "non");
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_PARTICULA));
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_ADVERBIUM));
        x = _describere(piscina, voc, "cum");
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_CONIUNCTIO_SUBORDINANS));
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_ADPOSITIO));
        x = _describere(piscina, voc, "tertiae");
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_NUMERALE));
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_ADIECTIVUM));
        x = _describere(piscina, voc, "deorum");
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_NOMEN_PROPRIUM));
        CREDO_VERUM (_habet(x, ORATIO_CLASSIS_SUBSTANTIVUM));
        x = _describere(piscina, voc, "puellam");
        CREDO_FALSUM (_habet(x, ORATIO_CLASSIS_DETERMINANS));
        CREDO_FALSUM (_habet(x, ORATIO_CLASSIS_NOMEN_PROPRIUM));
    }

    imprimere("\n--- II. Annotatio arboris ---\n");
    {
        constans character* fons = "Puella amat. Xyzzy virumque.\n";
              MateriaNodus* doc = oratio_arbor_parsare(piscina, fons,
                  (i32)strlen(fons));
        OratioPartesCensus census;
        MateriaNodus* v;

        CREDO_NON_NIHIL (doc);
        CREDO_VERUM (oratio_partes_annotare(piscina, &vocabularia, doc,
            &census));
        imprimere("  vocabula %d annotata %d analyses %d ignota %d\n",
            (integer)census.vocabula, (integer)census.annotata,
            (integer)census.analyses, (integer)census.ignota);
                CREDO_AEQUALIS_I32 (census.vocabula, (i32)IV);
        CREDO_AEQUALIS_I32 (census.annotata, (i32)IV);
        CREDO_AEQUALIS_I32 (census.ignota, ZEPHYRUM);   /* Xyzzy capitale: regula */
        CREDO_VERUM (census.analyses >= (i32)XII);
        CREDO_VERUM (census.classes[ORATIO_CLASSIS_SUBSTANTIVUM]
            >= (i32)IV);
        CREDO_VERUM (census.classes[ORATIO_CLASSIS_VERBUM] >= I);
        /* Puella: analyses III (NOM VOC ABL), classes, linguae */
        v = _vocabulum(doc, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (v->genus, (s32)ORATIO_GENUS_VOCABULUM);
        CREDO_AEQUALIS_I32 (_numerus_analysium(v), (i32)III);
        CREDO_VERUM (_aequalis(_derivatum(v,
            (i32)ORATIO_VOCABULUM_CLASSES),
            "substantivum"));
        CREDO_VERUM (_aequalis(_derivatum(v,
            (i32)ORATIO_VOCABULUM_LINGUAE),
            "latina"));
        {
            constans MateriaNodus* a = materia_valor_lista_obtinere(
                v->loci[ORATIO_VOCABULUM_ANALYSES],
                ZEPHYRUM)->datum.nodus;
            s32 locus_casus =
                oratio_partes_locus(ORATIO_CLASSIS_SUBSTANTIVUM,
                "casus");

            CREDO_AEQUALIS_S32 (a->genus,
                (s32)ORATIO_GENUS_ANALYSIS_SUBSTANTIVI);
            CREDO_VERUM (_aequalis(_derivatum(a,
                (i32)ORATIO_ANALYSIS_LEMMA),
                "puella"));
            CREDO_VERUM (_aequalis(_derivatum(a,
                (i32)ORATIO_ANALYSIS_NATIVUM),
                "N 1 1 NOM S C"));
            CREDO_VERUM (_derivatum(a,
                (i32)ORATIO_ANALYSIS_SENSUS).mensura
                > ZEPHYRUM);
            CREDO_AEQUALIS_S32 (_index(a, (i32)ORATIO_ANALYSIS_LINGUA),
                (s32)ORATIO_LINGUA_LATINA);
            CREDO_AEQUALIS_S32 (_index(a, (i32)ORATIO_ANALYSIS_FONS),
                (s32)ORATIO_FONS_ANALYSIS_VOCABULARIUM_LA);
            CREDO_VERUM (locus_casus >= ZEPHYRUM);
            CREDO_AEQUALIS_S32 (_index(a, (i32)locus_casus),
                (s32)ORATIO_CASUS_NOMINATIVUS);
            CREDO_AEQUALIS_S32 (_index(a, (i32)oratio_partes_locus(
                ORATIO_CLASSIS_SUBSTANTIVUM, "genus")),
                (s32)ORATIO_GENUS_GRAMMATICUM_FEMININUM);
            CREDO_AEQUALIS_S32 (_index(a, (i32)oratio_partes_locus(
                ORATIO_CLASSIS_SUBSTANTIVUM, "declinatio")), (s32)I);
            /* lexema derivatum: origo = pars prima (linea I, f=derivatus) */
            CREDO_AEQUALIS_I32 (
                materia_valor_lista_obtinere(v->loci[ORATIO_VOCABULUM_PARTES],
                    ZEPHYRUM)->datum.token->linea,
                _vocabulum(doc, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM)
                    ->loci[ORATIO_VOCABULUM_CLASSES].datum.token->linea);
        }
        /* amat: verbum (elementum I; '.' elementum II) */
        v = _vocabulum(doc, ZEPHYRUM, ZEPHYRUM, I);
        CREDO_VERUM (_aequalis(_derivatum(v,
            (i32)ORATIO_VOCABULUM_CLASSES),
            "verbum"));
        CREDO_AEQUALIS_S32 (materia_valor_lista_obtinere(
            v->loci[ORATIO_VOCABULUM_ANALYSES],
            ZEPHYRUM)->datum.nodus->genus,
            (s32)ORATIO_GENUS_ANALYSIS_VERBI);
                /* Xyzzy: ignotum capitale -> nomen proprium fonte regula (T13) */
        v = _vocabulum(doc, ZEPHYRUM, I, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus_analysium(v), I);
        CREDO_VERUM (_aequalis(_derivatum(v,
            (i32)ORATIO_VOCABULUM_CLASSES),
            "nomen-proprium"));
        CREDO_VERUM (_aequalis(_derivatum(v,
            (i32)ORATIO_VOCABULUM_LINGUAE),
            "latina"));
        /* xyzzy minusculum: ignotum - analyses nullae, classes 'ignotum',
         * linguae absens */
        {
            constans character* fons3 = "Puella xyzzy amat.\n";
            MateriaNodus* doc3 = oratio_arbor_parsare(piscina, fons3,
                (i32)strlen(fons3));
            OratioPartesCensus census3;
            MateriaNodus* x3;

            CREDO_NON_NIHIL (doc3);
            CREDO_VERUM (oratio_partes_annotare(piscina, &vocabularia,
                doc3,
                &census3));
            CREDO_AEQUALIS_I32 (census3.ignota, I);
            x3 = _vocabulum(doc3, ZEPHYRUM, ZEPHYRUM, I);
            CREDO_AEQUALIS_I32 (_numerus_analysium(x3), ZEPHYRUM);
            CREDO_VERUM (_aequalis(_derivatum(x3,
                (i32)ORATIO_VOCABULUM_CLASSES),
                "ignotum"));
            CREDO_AEQUALIS_S32 ((s32)x3->loci[ORATIO_VOCABULUM_LINGUAE].genus,
                (s32)MATERIA_VALOR_NIHIL);
        }
        /* virumque: tackon primus (WORDS ordo), substantiva post */
        v = _vocabulum(doc, ZEPHYRUM, I, I);
        CREDO_VERUM (_aequalis(_derivatum(v,
            (i32)ORATIO_VOCABULUM_CLASSES),
            "coniunctio-coordinans substantivum"));
        CREDO_VERUM (_numerus_analysium(v) >= (i32)IX);
                /* regula capitalis (T13): ignotum capitale = nomen proprium */
        {
            constans character* fons2 = "Karolus rex.\n";
            MateriaNodus* doc2 = oratio_arbor_parsare(piscina, fons2,
                (i32)strlen(fons2));
            OratioPartesCensus census2;
            MateriaNodus* k;

            CREDO_NON_NIHIL (doc2);
            CREDO_VERUM (oratio_partes_annotare(piscina, &vocabularia,
                doc2,
                &census2));
            CREDO_AEQUALIS_I32 (census2.ignota, ZEPHYRUM);
            k = _vocabulum(doc2, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);
            CREDO_VERUM (_aequalis(_derivatum(k,
                (i32)ORATIO_VOCABULUM_CLASSES),
                "nomen-proprium"));
            CREDO_AEQUALIS_I32 (_numerus_analysium(k), I);
            {
                constans MateriaNodus* a = materia_valor_lista_obtinere(
                    k->loci[ORATIO_VOCABULUM_ANALYSES],
                    ZEPHYRUM)->datum.nodus;

                CREDO_AEQUALIS_S32 (a->genus,
                    (s32)ORATIO_GENUS_ANALYSIS_NOMINIS_PROPRII);
                CREDO_VERUM (_aequalis(_derivatum(a,
                    (i32)ORATIO_ANALYSIS_NATIVUM),
                    "capitalis"));
                CREDO_VERUM (_aequalis(_derivatum(a,
                    (i32)ORATIO_ANALYSIS_LEMMA),
                    "karolus"));
                CREDO_AEQUALIS_S32 (_index(a,
                    (i32)ORATIO_ANALYSIS_FONS),
                    (s32)ORATIO_FONS_ANALYSIS_REGULA);
            }
        }
        /* semel: cursus secundus nihil annotat */
        CREDO_VERUM (oratio_partes_annotare(piscina, &vocabularia, doc,
            &census));
        CREDO_AEQUALIS_I32 (census.vocabula, (i32)IV);
        CREDO_AEQUALIS_I32 (census.annotata, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus_analysium(
            _vocabulum(doc, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM)), (i32)III);

        imprimere("\n--- III. Proiectio STML, circuitus, canon ---\n");
        {
            MateriaArborScriptura s =
                materia_arbor_scribere_nodum(piscina,
                doc, &consilium);
            chorda fons_canonis;

            CREDO_VERUM (s.successus);
            si (s.successus)
            {
                CREDO_NON_NIHIL (strstr((character*)s.textus.datum,
                    "<vocabulum classes=\"substantivum\" linguae=\"latina\">"));
                                CREDO_NON_NIHIL (strstr((character*)s.textus.datum,
                                    "<vocabulum classes=\"nomen-proprium\""));
                CREDO_NON_NIHIL (strstr((character*)s.textus.datum,
                    "<analysis-substantivi>"));
                CREDO_NON_NIHIL (strstr((character*)s.textus.datum,
                    "<analysis-verbi>"));
                CREDO_NON_NIHIL (strstr((character*)s.textus.datum,
                    "<analysis-coniunctionis-coordinantis>"));
                CREDO_NON_NIHIL (strstr((character*)s.textus.datum,
                    "<casus(> 0"));   /* INDEX ut textus: nominativus */
                CREDO_NON_NIHIL (strstr((character*)s.textus.datum,
                    "<lemma>"));
                CREDO_NON_NIHIL (strstr((character*)s.textus.datum,
                    "<nativum>"));
                imprimere("  STML %d octeti\n",
                    (integer)s.textus.mensura);
            }
            CREDO_VERUM (_circuitum_arboris(piscina, &consilium, doc,
                VERUM));
            sprintf(via, "%s/oratio/grammatica/oratio.canon", radix);
            si (!_plagulam_legere(piscina, via, &fons_canonis))
            {
                CREDO_CULPA ("oratio.canon absens");
            }
            alioquin
            {
                      chorda  causa;
                       Canon* canon;
                StmlResultus  res;

                causa.datum    = NIHIL;
                causa.mensura  = ZEPHYRUM;
                canon = canon_legere(fons_canonis, piscina, intern,
                    &causa);

                CREDO_NON_NIHIL (canon);
                res = stml_legere(s.textus, piscina, intern);
                CREDO_VERUM (res.successus);
                si (canon != NIHIL && res.successus)
                {
                    Xar* vitia = canon_iudicare(canon,
                        res.elementum_radix,
                        piscina);
                    i32 k;

                    CREDO_NON_NIHIL (vitia);
                    per (k = ZEPHYRUM; vitia != NIHIL
                        && k < xar_numerus(vitia)
                        && k < (i32)V; k++)
                    {
                        CanonVitium* vitium_canonis =
                            (CanonVitium*)xar_obtinere(vitia, k);

                        imprimere("  VITIUM %s",
                            canon_nuntius(vitium_canonis->genus));
                        si (vitium_canonis->elementum != NIHIL)
                        {
                            imprimere(" <%.*s>",
                                (integer)vitium_canonis->elementum->mensura,
                                (constans character*)vitium_canonis->elementum->datum);
                        }
                        imprimere("\n");
                    }
                    CREDO_AEQUALIS_I32 (vitia
                        != NIHIL ? xar_numerus(vitia)
                        : (i32)-I, ZEPHYRUM);

                    imprimere("\n--- IV. Selectio super documentum ---\n");
                    {
                        Xar* q;

                        q = stml_quaerere_omnes(res.elementum_radix,
                            "vocabulum[classes~=verbum]", piscina,
                            intern);
                        CREDO_NON_NIHIL (q);
                        CREDO_AEQUALIS_I32 (q ? xar_numerus(q) : ZEPHYRUM,
                            I);
                        q = stml_quaerere_omnes(res.elementum_radix,
                            "vocabulum[classes~=substantivum]", piscina,
                            intern);
                        imprimere("  [classes~=substantivum] %d\n",
                            (integer)(q ? xar_numerus(q) : ZEPHYRUM));
                        CREDO_AEQUALIS_I32 (q ? xar_numerus(q) : ZEPHYRUM,
                            (i32)II);
                                                q =
                                                    stml_quaerere_omnes(res.elementum_radix,
                                                    "vocabulum[linguae~=latina]",
                                                    piscina,
                                                    intern);
                        CREDO_AEQUALIS_I32 (q ? xar_numerus(q) : ZEPHYRUM,
                            (i32)IV);
                        q = stml_quaerere_omnes(res.elementum_radix,
                            "vocabulum[classes~=ignotum]", piscina,
                            intern);
                        CREDO_AEQUALIS_I32 (q ? xar_numerus(q) : ZEPHYRUM,
                            ZEPHYRUM);
                        q = stml_quaerere_omnes(res.elementum_radix,
                            "vocabulum[classes~=nomen-proprium]",
                            piscina,
                            intern);
                        CREDO_AEQUALIS_I32 (q ? xar_numerus(q) : ZEPHYRUM,
                            I);
                        q = stml_quaerere_omnes(res.elementum_radix,
                            "vocabulum[classes~=coniunctio-coordinans]",
                            piscina,
                            intern);
                        CREDO_AEQUALIS_I32 (q ? xar_numerus(q) : ZEPHYRUM,
                            I);
                        q = stml_quaerere_omnes(res.elementum_radix,
                            "analysis-verbi", piscina, intern);
                        CREDO_VERUM (q != NIHIL && xar_numerus(q) >= I);
                        q = stml_quaerere_omnes(res.elementum_radix,
                            "vocabulum[classes~=adiectivum]", piscina,
                            intern);
                        CREDO_AEQUALIS_I32 (q ? xar_numerus(q) : ZEPHYRUM,
                            ZEPHYRUM);
                    }
                }
            }
        }
    }

    imprimere("\n--- V. Corpus: fixturae Latinae annotatae ---\n");
    {
        hic_manens constans character* fixturae[] = {
            "hilarius.txt", "propertius.txt", "cicero.txt"
        };
        i32 f;
        i32 summa_vocabula  = ZEPHYRUM;
        i32 summa_ignota    = ZEPHYRUM;

        per (f = ZEPHYRUM; f < (i32)III; f++)
        {
            Piscina* p = piscina_generare_dynamicum("partes_corpus",
                268435456);
            chorda textus;
            MateriaNodus* doc;
            OratioPartesCensus census;
            clock_t ante;
            duplex ms;

            sprintf(via, "%s/oratio/probationes/fixa/txt/%s", radix,
                fixturae[f]);
            si (!_plagulam_legere(p, via, &textus))
            {
                CREDO_CULPA ("fixtura absens");
                piscina_destruere(p);
                perge;
            }
            doc = oratio_arbor_parsare(p,
                (constans character*)textus.datum,
                textus.mensura);
            CREDO_NON_NIHIL (doc);
            ante = clock();
            CREDO_VERUM (oratio_partes_annotare(p, &vocabularia, doc,
                &census));
            ms = 1000.0 * (duplex)(clock() - ante)
                / (duplex)CLOCKS_PER_SEC;
            imprimere("  %-16s vocabula %6d  analyses %7d  ignota %5d (%.1f%%)  %.0f ms  [subst %d verb %d adi %d adv %d pron %d adp %d coni %d]\n",
                fixturae[f], (integer)census.vocabula,
                (integer)census.analyses, (integer)census.ignota,
                census.vocabula > ZEPHYRUM
                    ? 100.0 * (duplex)census.ignota
                        / (duplex)census.vocabula
                    : 0.0,
                ms,
                (integer)census.classes[ORATIO_CLASSIS_SUBSTANTIVUM],
                (integer)census.classes[ORATIO_CLASSIS_VERBUM],
                (integer)census.classes[ORATIO_CLASSIS_ADIECTIVUM],
                (integer)census.classes[ORATIO_CLASSIS_ADVERBIUM],
                (integer)census.classes[ORATIO_CLASSIS_PRONOMEN],
                (integer)census.classes[ORATIO_CLASSIS_ADPOSITIO],
                (integer)(census.classes[ORATIO_CLASSIS_CONIUNCTIO_COORDINANS]
                    + census.classes[ORATIO_CLASSIS_CONIUNCTIO_SUBORDINANS]));
            CREDO_VERUM (census.vocabula > (i32)1000);
            CREDO_VERUM (census.analyses >= census.vocabula);
            CREDO_VERUM (census.ignota * (i32)C
                < census.vocabula * (i32)XII);
            summa_vocabula  = summa_vocabula + census.vocabula;
            summa_ignota    = summa_ignota + census.ignota;
            si (f == ZEPHYRUM)
            {
                /* circuitus arboris annotatae (Hilarius) */
                CREDO_VERUM (_circuitum_arboris(p, &consilium, doc,
                    VERUM));
            }
            piscina_destruere(p);
        }
        imprimere("  SUMMA vocabula %d  ignota %d (%.1f%%)\n",
            (integer)summa_vocabula, (integer)summa_ignota,
            summa_vocabula > ZEPHYRUM
                ? 100.0 * (duplex)summa_ignota
                    / (duplex)summa_vocabula : 0.0);
    }

    imprimere("\n--- VI. Anglica: Moby + regulae -> descriptiones"
        " (T16) ---\n");
    {
                              Xar* x;
        constans OratioDescriptio* d;

        /* the\Dv: una per classem litterarum, ordine Moby */
        x = _describere_en(piscina, vocabularia.en, "the");
        CREDO_NON_NIHIL (x);
        CREDO_AEQUALIS_I32 (xar_numerus(x), (i32)II);
        d = _descriptio_en(x, ORATIO_CLASSIS_DETERMINANS);
        CREDO_NON_NIHIL (d);
        CREDO_VERUM (_aequalis(d->lemma, "the"));
        CREDO_VERUM (_aequalis(d->nativum, "Dv"));
        CREDO_AEQUALIS_S32 ((s32)d->lingua, (s32)ORATIO_LINGUA_ANGLICA);
        CREDO_AEQUALIS_S32 ((s32)d->fons,
            (s32)ORATIO_FONS_ANALYSIS_VOCABULARIUM_EN);
        CREDO_AEQUALIS_S32 (d->numerus, (s32)-I);
        CREDO_AEQUALIS_I32 (d->sensus.mensura, ZEPHYRUM);
        CREDO_NON_NIHIL (_descriptio_en(x, ORATIO_CLASSIS_ADVERBIUM));
        /* cats: pluralis-s super basin cat (N, NV) - substantivum
         * pluralis ET verbum personae III */
        x = _describere_en(piscina, vocabularia.en, "cats");
        d = _descriptio_en(x, ORATIO_CLASSIS_SUBSTANTIVUM);
        CREDO_NON_NIHIL (d);
        CREDO_AEQUALIS_S32 (d->numerus,
            (s32)ORATIO_NUMERUS_GRAMMATICUS_PLURALIS);
        CREDO_VERUM (_aequalis(d->lemma, "cat"));
        CREDO_VERUM (_aequalis(d->nativum, "N pluralis-s"));
        d = _descriptio_en(x, ORATIO_CLASSIS_VERBUM);
        CREDO_NON_NIHIL (d);
        CREDO_AEQUALIS_S32 (d->persona, (s32)ORATIO_PERSONA_TERTIA);
        CREDO_AEQUALIS_S32 (d->numerus,
            (s32)ORATIO_NUMERUS_GRAMMATICUS_SINGULARIS);
        CREDO_AEQUALIS_S32 (d->tempus, (s32)ORATIO_TEMPUS_PRAESENS);
        CREDO_AEQUALIS_S32 (d->forma_verbi,
            (s32)ORATIO_FORMA_VERBI_FINITUM);
        /* planned: V exacta, deinde praeteritum-ed-geminatum: finitum +
         * participium (basis plan) */
        x = _describere_en(piscina, vocabularia.en, "planned");
        CREDO_AEQUALIS_I32 (xar_numerus(x), (i32)III);
        d = (constans OratioDescriptio*)xar_obtinere(x, I);
        CREDO_AEQUALIS_S32 ((s32)d->classis,
            (s32)ORATIO_CLASSIS_VERBUM);
        CREDO_AEQUALIS_S32 (d->tempus, (s32)ORATIO_TEMPUS_PRAETERITUM);
        CREDO_AEQUALIS_S32 (d->modus, (s32)ORATIO_MODUS_INDICATIVUS);
        CREDO_AEQUALIS_S32 (d->forma_verbi,
            (s32)ORATIO_FORMA_VERBI_FINITUM);
        CREDO_VERUM (_aequalis(d->lemma, "plan"));
        CREDO_VERUM (_aequalis(d->nativum,
            "NV praeteritum-ed-geminatum"));
        d = (constans OratioDescriptio*)xar_obtinere(x, (i32)II);
        CREDO_AEQUALIS_S32 (d->forma_verbi,
            (s32)ORATIO_FORMA_VERBI_PARTICIPIUM);
        /* quickly: v exacta + adverbium-ly (quick) */
        x = _describere_en(piscina, vocabularia.en, "quickly");
        CREDO_AEQUALIS_I32 (xar_numerus(x), (i32)II);
        d = (constans OratioDescriptio*)xar_obtinere(x, I);
        CREDO_VERUM (_aequalis(d->lemma, "quick"));
        /* higher: comparativus-er (high) */
        x = _describere_en(piscina, vocabularia.en, "higher");
        d = _descriptio_en(x, ORATIO_CLASSIS_ADIECTIVUM);
        CREDO_NON_NIHIL (d);
        {
            i32 k;
            b32 comparativus = FALSUM;

            per (k = ZEPHYRUM; k < xar_numerus(x); k++)
            {
                constans OratioDescriptio* e =
                    (constans OratioDescriptio*)xar_obtinere(x, k);

                si (   e->gradus == (s32)ORATIO_GRADUS_COMPARATIVUS
                    && _aequalis(e->lemma, "high"))
                {
                    comparativus = VERUM;
                }
            }
            CREDO_VERUM (comparativus);
        }
        /* Fran's: possessivum - substantivum genitivus + particula
         * (POS) + auxiliare */
        x = _describere_en(piscina, vocabularia.en, "Fran's");
        d = _descriptio_en(x, ORATIO_CLASSIS_SUBSTANTIVUM);
        CREDO_NON_NIHIL (d);
        CREDO_AEQUALIS_S32 (d->casus_grammaticus,
            (s32)ORATIO_CASUS_GENITIVUS);
        CREDO_VERUM (_aequalis(d->lemma, "fran"));
        CREDO_NON_NIHIL (_descriptio_en(x, ORATIO_CLASSIS_PARTICULA));
        CREDO_NON_NIHIL (_descriptio_en(x, ORATIO_CLASSIS_AUXILIARE));
        /* it's: basis pronomen (rN) genitivo */
        x = _describere_en(piscina, vocabularia.en, "it's");
        d = _descriptio_en(x, ORATIO_CLASSIS_PRONOMEN);
        CREDO_NON_NIHIL (d);
        CREDO_AEQUALIS_S32 (d->casus_grammaticus,
            (s32)ORATIO_CASUS_GENITIVUS);
        /* don't: contractio-n't - verbum do + particula */
        x = _describere_en(piscina, vocabularia.en, "don't");
        {
            i32 k;
            b32 facere = FALSUM;

            per (k = ZEPHYRUM; k < xar_numerus(x); k++)
            {
                constans OratioDescriptio* e =
                    (constans OratioDescriptio*)xar_obtinere(x, k);

                si (   e->classis == ORATIO_CLASSIS_VERBUM
                    && _aequalis(e->lemma, "do"))
                {
                    facere = VERUM;
                }
            }
            CREDO_VERUM (facere);
        }
        CREDO_NON_NIHIL (_descriptio_en(x, ORATIO_CLASSIS_PARTICULA));
        /* we're: pronomen we + auxiliare */
        x = _describere_en(piscina, vocabularia.en, "we're");
        d = _descriptio_en(x, ORATIO_CLASSIS_PRONOMEN);
        CREDO_NON_NIHIL (d);
        CREDO_VERUM (_aequalis(d->lemma, "we"));
        CREDO_NON_NIHIL (_descriptio_en(x, ORATIO_CLASSIS_AUXILIARE));
        /* listae (cursus II): is that to two hello - fonte regula */
        d = _descriptio_en(_describere_en(piscina, vocabularia.en,
            "is"),
            ORATIO_CLASSIS_AUXILIARE);
        CREDO_NON_NIHIL (d);
        CREDO_AEQUALIS_S32 ((s32)d->fons,
            (s32)ORATIO_FONS_ANALYSIS_REGULA);
        CREDO_VERUM (_aequalis(d->nativum, "auxiliaria"));
        CREDO_NON_NIHIL (_descriptio_en(_describere_en(piscina,
            vocabularia.en, "that"),
            ORATIO_CLASSIS_CONIUNCTIO_SUBORDINANS));
        CREDO_NON_NIHIL (_descriptio_en(_describere_en(piscina,
            vocabularia.en, "to"), ORATIO_CLASSIS_PARTICULA));
        CREDO_NON_NIHIL (_descriptio_en(_describere_en(piscina,
            vocabularia.en, "two"), ORATIO_CLASSIS_NUMERALE));
        CREDO_NON_NIHIL (_descriptio_en(_describere_en(piscina,
            vocabularia.en, "Hello"), ORATIO_CLASSIS_INTERIECTIO));
        /* ignotum Moby: nihil */
        x = _describere_en(piscina, vocabularia.en, "xyzzy");
        CREDO_NON_NIHIL (x);
        CREDO_AEQUALIS_I32 (xar_numerus(x), ZEPHYRUM);
    }

    imprimere("\n--- VII. Annotatio Anglica; vocabularia ambo et"
        " Latina sola ---\n");
    {
        constans character* fons = "The cats ran quickly. Bush won.\n";
              MateriaNodus* doc = oratio_arbor_parsare(piscina, fons,
                  (i32)strlen(fons));
        OratioPartesCensus census;
        MateriaNodus* v;
        OratioVocabularia sola;
        OratioVocabularia vacua;
        OratioVocabulariumVitium vitium;

        CREDO_NON_NIHIL (doc);
        CREDO_VERUM (oratio_partes_annotare(piscina, &vocabularia, doc,
            &census));
        CREDO_AEQUALIS_I32 (census.vocabula, (i32)VI);
        CREDO_AEQUALIS_I32 (census.ignota, ZEPHYRUM);
        CREDO_VERUM (census.linguae[ORATIO_LINGUA_ANGLICA] >= (i32)X);
        CREDO_AEQUALIS_I32 (census.linguae[ORATIO_LINGUA_LATINA]
            + census.linguae[ORATIO_LINGUA_ANGLICA], census.analyses);
        /* The: determinans adverbium - sine nomine proprio (nullum
         * substantivum) */
        v = _vocabulum(doc, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);
        CREDO_VERUM (_aequalis(_derivatum(v,
            (i32)ORATIO_VOCABULUM_CLASSES),
            "determinans adverbium"));
        CREDO_VERUM (_aequalis(_derivatum(v,
            (i32)ORATIO_VOCABULUM_LINGUAE),
            "anglica"));
        /* cats */
        v = _vocabulum(doc, ZEPHYRUM, ZEPHYRUM, I);
        CREDO_VERUM (_aequalis(_derivatum(v,
            (i32)ORATIO_VOCABULUM_CLASSES),
            "substantivum verbum"));
        /* Bush: Moby soli notum ut substantivum, capitale -> + nomen
         * proprium (regula), lingua anglica */
        v = _vocabulum(doc, ZEPHYRUM, I, ZEPHYRUM);
        CREDO_VERUM (_aequalis(_derivatum(v,
            (i32)ORATIO_VOCABULUM_CLASSES),
            "substantivum adiectivum verbum nomen-proprium"));
        {
                              i32  n = _numerus_analysium(v);
            constans MateriaNodus* a = materia_valor_lista_obtinere(
                v->loci[ORATIO_VOCABULUM_ANALYSES], n - I)->datum.nodus;

            CREDO_AEQUALIS_S32 (a->genus,
                (s32)ORATIO_GENUS_ANALYSIS_NOMINIS_PROPRII);
            CREDO_AEQUALIS_S32 (_index(a, (i32)ORATIO_ANALYSIS_LINGUA),
                (s32)ORATIO_LINGUA_ANGLICA);
            CREDO_AEQUALIS_S32 (_index(a, (i32)ORATIO_ANALYSIS_FONS),
                (s32)ORATIO_FONS_ANALYSIS_REGULA);
        }
        /* Latina sola (en NIHIL): nihil Anglicum */
        sola.la = vocabularia.la;
        sola.en = NIHIL;
        fons = "Puella amat.\n";
        doc = oratio_arbor_parsare(piscina, fons, (i32)strlen(fons));
        CREDO_NON_NIHIL (doc);
        CREDO_VERUM (oratio_partes_annotare(piscina, &sola, doc,
            &census));
        CREDO_AEQUALIS_I32 (census.linguae[ORATIO_LINGUA_ANGLICA],
            ZEPHYRUM);
        CREDO_VERUM (census.linguae[ORATIO_LINGUA_LATINA] >= (i32)IV);
        /* onerator: radix absens -> FALSUM cum plagula nominata */
        CREDO_VERUM (!oratio_vocabularia_onerare(piscina, "/nemo/hic",
            &vacua, &vitium));
        CREDO_NON_NIHIL (vitium.plagula);
        CREDO_VERUM (vacua.la == NIHIL && vacua.en == NIHIL);
        CREDO_VERUM (strstr(vitium.plagula, "la.bin") != NIHIL);
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
