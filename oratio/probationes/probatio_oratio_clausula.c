/* probatio_oratio_clausula.c - Stampa clausularum (T20a, 2026-09-08;
 * spec par. II decisiones XLIII-XLVII, par. VII 'Design - T20')
 *
 * I.   DATA seminum: listae NIHIL terminatae, lemmata exspectata (si
 *      certum, cum corroborandum, qui relativum, et coordinans), -que
 *      numquam.
 * II.  CASUS MANU IUDICATI per cursum totum (parsura, annotatio,
 *      stampa): subordinata comma clausa post verbum certum (Caesar,
 *      cum venisset, urbem cepit), coordinatae sorores (Puella bona
 *      ambulat et puer currit), 'si quis' initiale (quis NON semen;
 *      principalis pigre nata post clausuram), sententia sine semine
 *      (unica), relativum conectens initiale (Qui cum venisset ...),
 *      corroboratio negata (Cum puella.), parenthesis, Anglica (unica),
 *      idempotentia, umbrae clausulae (verbum-finitum forma finita,
 *      subiectum nominativus), proiectio (attributa clausula /
 *      clausula-causa, nodi clausula) et circuitus STML.
   *      SCISSIO verbi (stratum V, T20a bis): Puella cantat puer currit,
 *      Puer currit, puella cantat; subordinata non scissa.

 * III. LEGES SUMMAE: causae summa == elementa posita; species summa ==
 *      clausulae; censu resolutionis idem.

 * Culpa plantata: 'si' e ORATIO_SEMINA_CERTA sublatum - casus 'si quis'
 * clausulam unam dat, porta RUBRA.
 */

#include "latina.h"
#include "credo.h"
#include "oratio_clausula.h"
#include "oratio_arbor.h"
#include "oratio_partes.h"
#include "oratio_registrum.h"
#include "oratio_resolutio.h"
#include "oratio_stml.h"
#include "oratio_lexicon.h"
#include "oratio_vocabularia.h"
#include "internamentum.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "chorda.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior b32
_in_lista (
    constans character* constans* lista,
              constans character* lemma)
{
    i32 i;

    per (i = ZEPHYRUM; lista[i] != NIHIL; i++)
    {
        si (strcmp(lista[i], lemma) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* documentum parsatum et annotatum */
interior MateriaNodus*
_documentum (
                       Piscina* piscina,
    constans OratioVocabularia* vocabularia,
            constans character* fons)
{
    MateriaNodus* doc = oratio_arbor_parsare(piscina, fons,
        (i32)strlen(fons));

    si (   doc == NIHIL
        || !oratio_partes_annotare(piscina, vocabularia, doc, NIHIL))
    {
        redde NIHIL;
    }
    redde doc;
}

/* sententia prima documenti */
interior MateriaNodus*
_sententia (
    constans MateriaNodus* doc)
{
    constans MateriaValor* paragraphi =
        &doc->loci[ORATIO_DOCUMENTUM_PARAGRAPHI];
    constans MateriaNodus* par;
    constans MateriaValor* sententiae;

    si (   paragraphi->genus != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*paragraphi) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    par = materia_valor_lista_obtinere(*paragraphi,
        ZEPHYRUM)->datum.nodus;
    sententiae = &par->loci[ORATIO_PARAGRAPHUS_SENTENTIAE];
    si (   sententiae->genus != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*sententiae) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde materia_valor_lista_obtinere(*sententiae,
        ZEPHYRUM)->datum.nodus;
}

/* elementum k sententiae primae */
interior MateriaNodus*
_elementum (
    constans MateriaNodus* doc,
                      i32  k)
{
    constans MateriaNodus* sen = _sententia(doc);
    constans MateriaValor* elementa;

    si (sen == NIHIL)
    {
        redde NIHIL;
    }
    elementa = &sen->loci[ORATIO_SENTENTIA_ELEMENTA];
    si (   elementa->genus != MATERIA_VALOR_LISTA
        || k               >= materia_valor_lista_numerus(*elementa))
    {
        redde NIHIL;
    }
    redde materia_valor_lista_obtinere(*elementa, k)->datum.nodus;
}

interior s32
_index_loci (
    constans MateriaNodus* nodus,
                      s32  locus)
{
    si (   nodus                    == NIHIL || locus < ZEPHYRUM
        || (i32)locus               >= nodus->numerus_locorum
        || nodus->loci[locus].genus != MATERIA_VALOR_INDEX)
    {
        redde (s32)-I;
    }
    redde nodus->loci[locus].datum.index;
}

/* clausula elementi k (-I aperta) et causa */
interior s32
_clausula (
    constans MateriaNodus* doc,
                      i32  k)
{
    constans MateriaNodus* e = _elementum(doc, k);

    redde e == NIHIL ? (s32)-I : _index_loci(e,
        oratio_locus_clausulae((OratioGenus)e->genus, FALSUM));
}

interior s32
_causa (
    constans MateriaNodus* doc,
                      i32  k)
{
    constans MateriaNodus* e = _elementum(doc, k);

    redde e == NIHIL ? (s32)-I : _index_loci(e,
        oratio_locus_clausulae((OratioGenus)e->genus, VERUM));
}

/* numerus clausularum sententiae primae */
interior i32
_numerus_clausularum (
    constans MateriaNodus* doc)
{
    constans MateriaNodus* sen = _sententia(doc);
    constans MateriaValor* clausulae;

    si (sen == NIHIL)
    {
        redde ZEPHYRUM;
    }
    clausulae = &sen->loci[ORATIO_SENTENTIA_CLAUSULAE];
    redde clausulae->genus == MATERIA_VALOR_LISTA
        ? materia_valor_lista_numerus(*clausulae) : ZEPHYRUM;
}

interior constans MateriaNodus*
_nodus_clausulae (
    constans MateriaNodus* doc,
                      i32  c)
{
    constans MateriaNodus* sen = _sententia(doc);
    constans MateriaValor* clausulae;

    si (sen == NIHIL)
    {
        redde NIHIL;
    }
    clausulae = &sen->loci[ORATIO_SENTENTIA_CLAUSULAE];
    si (   clausulae->genus != MATERIA_VALOR_LISTA
        || c                >= materia_valor_lista_numerus(*clausulae))
    {
        redde NIHIL;
    }
    redde materia_valor_lista_obtinere(*clausulae, c)->datum.nodus;
}

interior s32
_species (
    constans MateriaNodus* doc,
                      i32  c)
{
    redde _index_loci(_nodus_clausulae(doc, c),
        (s32)ORATIO_CLAUSULA_SPECIES);
}

interior s32
_semen (
    constans MateriaNodus* doc,
                      i32  c)
{
    redde _index_loci(_nodus_clausulae(doc, c),
        (s32)ORATIO_CLAUSULA_SEMEN);
}

interior s32
_pater (
    constans MateriaNodus* doc,
                      i32  c)
{
    redde _index_loci(_nodus_clausulae(doc, c),
        (s32)ORATIO_CLAUSULA_PATER);
}

/* umbra u clausulae c */
interior constans MateriaNodus*
_umbra (
    constans MateriaNodus* doc,
                      i32  c,
                      i32  u)
{
    constans MateriaNodus* cl = _nodus_clausulae(doc, c);
    constans MateriaValor* umbrae;

    si (cl == NIHIL)
    {
        redde NIHIL;
    }
    umbrae = &cl->loci[ORATIO_CLAUSULA_UMBRAE];
    si (   umbrae->genus != MATERIA_VALOR_LISTA
        || u             >= materia_valor_lista_numerus(*umbrae))
    {
        redde NIHIL;
    }
    redde materia_valor_lista_obtinere(*umbrae, u)->datum.nodus;
}

/* seminare et leges summae censu */
interior b32
_seminare (
              Piscina* piscina,
         MateriaNodus* doc,
    OratioClausulaCensus* census)
{
    i32 summa_causarum   = ZEPHYRUM;
    i32 summa_specierum  = ZEPHYRUM;
    i32 i;

    oratio_clausula_census_vacare(census);
    si (!oratio_clausulas_seminare(piscina, doc, census))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < (i32)ORATIO_CLAUSULA_CAUSA_NUMERUS; i++)
    {
        summa_causarum = summa_causarum + census->causae[i];
    }
    per (i = ZEPHYRUM; i < (i32)ORATIO_SPECIES_CLAUSULAE_NUMERUS; i++)
    {
        summa_specierum = summa_specierum + census->species[i];
    }
    CREDO_AEQUALIS_I32 (summa_specierum, census->clausulae);
    redde (b32)(summa_causarum > ZEPHYRUM);
}

s32
principale (vacuum)
{
                  Piscina* piscina;
       constans character* radix;
        OratioVocabularia  vocabularia;
 OratioVocabulariumVitium  vitium;
     OratioClausulaCensus  census;
             MateriaNodus* doc;

    piscina = piscina_generare_dynamicum("probatio_oratio_clausula",
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
    si (vocabularia.la == NIHIL || vocabularia.en == NIHIL)
    {
        credo_imprimere_compendium();
        redde I;
    }

    imprimere("\n--- I. Data seminum ---\n");
    CREDO_VERUM (_in_lista(ORATIO_SEMINA_CERTA, "si"));
    CREDO_VERUM (_in_lista(ORATIO_SEMINA_CERTA, "quia"));
    CREDO_VERUM (_in_lista(ORATIO_SEMINA_CERTA, "quod"));
    CREDO_FALSUM (_in_lista(ORATIO_SEMINA_CERTA, "cum"));
    CREDO_FALSUM (_in_lista(ORATIO_SEMINA_CERTA, "ut"));
    CREDO_VERUM (_in_lista(ORATIO_SEMINA_CORROBORANDA, "cum"));
    CREDO_VERUM (_in_lista(ORATIO_SEMINA_CORROBORANDA, "ut"));
    CREDO_VERUM (_in_lista(ORATIO_SEMINA_CORROBORANDA, "ubi"));
    CREDO_FALSUM (_in_lista(ORATIO_SEMINA_CORROBORANDA, "quam"));
    CREDO_VERUM (_in_lista(ORATIO_SEMINA_RELATIVA, "qui"));
    CREDO_VERUM (_in_lista(ORATIO_SEMINA_COORDINANTIA, "et"));
    CREDO_VERUM (_in_lista(ORATIO_SEMINA_COORDINANTIA, "sed"));
    CREDO_FALSUM (_in_lista(ORATIO_SEMINA_COORDINANTIA, "que"));
    CREDO_FALSUM (_in_lista(ORATIO_SEMINA_COORDINANTIA, "vel"));

    imprimere("\n--- II. Casus manu iudicati ---\n");
    /* (a) subordinata clausa verbo certo, comma post, principalis circum:
     * Caesar(0) ,(1) cum(2) venisset(3) ,(4) urbem(5) cepit(6) .(7) */
    doc = _documentum(piscina, &vocabularia,
        "Caesar, cum venisset, urbem cepit.\n");
    CREDO_NON_NIHIL (doc);
    CREDO_VERUM (_seminare(piscina, doc, &census));
    CREDO_AEQUALIS_I32 (_numerus_clausularum(doc), (i32)II);
    CREDO_AEQUALIS_S32 (_species(doc, ZEPHYRUM),
        (s32)ORATIO_SPECIES_CLAUSULAE_PRINCIPALIS);
    CREDO_AEQUALIS_S32 (_semen(doc, ZEPHYRUM), (s32)-I);
    CREDO_AEQUALIS_S32 (_pater(doc, ZEPHYRUM), (s32)-I);
    CREDO_AEQUALIS_S32 (_species(doc, I),
        (s32)ORATIO_SPECIES_CLAUSULAE_SUBORDINATA);
    CREDO_AEQUALIS_S32 (_semen(doc, I), (s32)II);
    CREDO_AEQUALIS_S32 (_pater(doc, I), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_clausula(doc, ZEPHYRUM), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_causa(doc, ZEPHYRUM),
        (s32)ORATIO_CLAUSULA_CAUSA_EXTENTUM);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)II), (s32)I);
    CREDO_AEQUALIS_S32 (_causa(doc, (i32)II),
        (s32)ORATIO_CLAUSULA_CAUSA_SEMEN);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)III), (s32)I);
    CREDO_AEQUALIS_S32 (_causa(doc, (i32)III),
        (s32)ORATIO_CLAUSULA_CAUSA_EXTENTUM);
    /* comma post verbum certum: clausula iam clausa - ad principalem,
     * causa clausura; urbem cepit item */
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)IV), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_causa(doc, (i32)IV),
        (s32)ORATIO_CLAUSULA_CAUSA_CLAUSURA);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)V), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_causa(doc, (i32)V),
        (s32)ORATIO_CLAUSULA_CAUSA_CLAUSURA);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)VI), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census.clausae_verbo, I);
    CREDO_AEQUALIS_I32 (census.clausae_signo, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census.corroborata, I);   /* cum + venisset */
    CREDO_AEQUALIS_I32 (census.causae[ORATIO_CLAUSULA_CAUSA_SEMEN], I);
    CREDO_AEQUALIS_I32 (census.causae[ORATIO_CLAUSULA_CAUSA_UNICA],
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census.apertae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census.sententiae, I);
    /* umbrae clausulae e schemate: verbum-finitum (forma finitum) et
     * subiectum (nominativus), impletio vacua */
    {
        constans MateriaNodus* verbum     = _umbra(doc, I, ZEPHYRUM);
        constans MateriaNodus* subiectum  = _umbra(doc, I, I);

        CREDO_NON_NIHIL (verbum);
        CREDO_NON_NIHIL (subiectum);
        CREDO_NIHIL (_umbra(doc, I, (i32)II));
        CREDO_AEQUALIS_S32 (_index_loci(verbum,
            (s32)ORATIO_UMBRA_RELATIO),
            (s32)ORATIO_RELATIO_VERBUM_FINITUM);
        CREDO_AEQUALIS_S32 (_index_loci(verbum,
            (s32)ORATIO_UMBRA_FORMA_VERBI),
            (s32)ORATIO_FORMA_VERBI_FINITUM);
        CREDO_AEQUALIS_S32 (_index_loci(verbum,
            (s32)ORATIO_UMBRA_IMPLETIO_VOCABULUM), (s32)-I);
        CREDO_AEQUALIS_S32 (_index_loci(subiectum,
            (s32)ORATIO_UMBRA_RELATIO), (s32)ORATIO_RELATIO_SUBIECTUM);
        CREDO_AEQUALIS_S32 (_index_loci(subiectum,
            (s32)ORATIO_UMBRA_CASUS),
            (s32)ORATIO_CASUS_NOMINATIVUS);
    }
    /* idempotens: cursus alter nihil addit */
    CREDO_VERUM (oratio_clausulas_seminare(piscina, doc, &census));
    CREDO_AEQUALIS_I32 (_numerus_clausularum(doc), (i32)II);
    CREDO_AEQUALIS_I32 (census.sententiae, I);
    /* proiectio: attributa clausula/clausula-causa et nodi clausula;
     * circuitus scribere -> legere -> scribere idem */
    {
          MateriaLexiconRatum ratum;
           MateriaLexIudicium iudicium;
        MateriaArborConsilium consilium;
        MateriaArborScriptura s1;
        MateriaArborScriptura s2;
        MateriaNodus*          lecta;
        MateriaArborVitium     arboris_vitium;

        CREDO_VERUM (materia_lexicon_ratum_facere(&ratum,
            &ORATIO_LEXICON,
            &iudicium));
        oratio_stml_consilium(&consilium, &ratum);
        s1 = materia_arbor_scribere_nodum(piscina, doc, &consilium);
        CREDO_VERUM (s1.successus);
        CREDO_NON_NIHIL (strstr((constans character*)s1.textus.datum,
            "clausula=\"1\""));
        CREDO_NON_NIHIL (strstr((constans character*)s1.textus.datum,
            "clausula-causa=\"semen\""));
        CREDO_NON_NIHIL (strstr((constans character*)s1.textus.datum,
            "clausula-causa=\"clausura\""));
        CREDO_NON_NIHIL (strstr((constans character*)s1.textus.datum,
            "<clausulae>"));
                CREDO_NON_NIHIL (strstr((constans character*)s1.textus.datum,
                    "<species"));
        lecta = materia_arbor_legere(piscina, NIHIL, s1.textus,
            &consilium,
            &arboris_vitium);
        CREDO_NON_NIHIL (lecta);
        si (lecta != NIHIL)
        {
            s2 = materia_arbor_scribere_nodum(piscina, lecta,
                &consilium);
            CREDO_VERUM (s2.successus);
            CREDO_AEQUALIS_I32 (s2.textus.mensura, s1.textus.mensura);
            CREDO_VERUM (memcmp(s1.textus.datum, s2.textus.datum,
                (size_t)s1.textus.mensura) == ZEPHYRUM);
            CREDO_AEQUALIS_I32 (_numerus_clausularum(lecta), (i32)II);
            CREDO_AEQUALIS_S32 (_clausula(lecta, (i32)II), (s32)I);
        }
    }

    /* (b) coordinatae sorores: Puella(0) bona(1) ambulat(2) et(3) puer(4)
     * currit(5) .(6) - et inter verba finita capacia */
    doc = _documentum(piscina, &vocabularia,
        "Puella bona ambulat et puer currit.\n");
    CREDO_NON_NIHIL (doc);
    CREDO_VERUM (_seminare(piscina, doc, &census));
    CREDO_AEQUALIS_I32 (_numerus_clausularum(doc), (i32)II);
    CREDO_AEQUALIS_S32 (_species(doc, I),
        (s32)ORATIO_SPECIES_CLAUSULAE_COORDINATA);
    CREDO_AEQUALIS_S32 (_semen(doc, I), (s32)III);
    CREDO_AEQUALIS_S32 (_pater(doc, I), (s32)-I);   /* soror principalis */
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)II), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)III), (s32)I);
    CREDO_AEQUALIS_S32 (_causa(doc, (i32)III),
        (s32)ORATIO_CLAUSULA_CAUSA_SEMEN);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)IV), (s32)I);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)V), (s32)I);
    CREDO_AEQUALIS_S32 (_causa(doc, (i32)V),
        (s32)ORATIO_CLAUSULA_CAUSA_EXTENTUM);
    /* (b2) et inter substantiva: nullum semen */
    doc = _documentum(piscina, &vocabularia,
        "Puella et puer ambulant.\n");
    CREDO_NON_NIHIL (doc);
    CREDO_VERUM (_seminare(piscina, doc, &census));
    CREDO_AEQUALIS_I32 (_numerus_clausularum(doc), I);
    CREDO_AEQUALIS_S32 (_causa(doc, I),
        (s32)ORATIO_CLAUSULA_CAUSA_UNICA);
    CREDO_AEQUALIS_I32 (census.reiecta, I);

    /* (c) si quis initiale: Si(0) quis(1) hoc(2) fecerit(3) ,(4) poenam(5)
     * dabit(6) .(7) - quis NON semen (forma indefinita); subordinata
     * primo nata (index 0), principalis pigre post clausuram (index 1)
     * et pater subordinatae ei figitur */
    doc = _documentum(piscina, &vocabularia,
        "Si quis hoc fecerit, poenam dabit.\n");
    CREDO_NON_NIHIL (doc);
    CREDO_VERUM (_seminare(piscina, doc, &census));
    CREDO_AEQUALIS_I32 (_numerus_clausularum(doc), (i32)II);
    CREDO_AEQUALIS_S32 (_species(doc, ZEPHYRUM),
        (s32)ORATIO_SPECIES_CLAUSULAE_SUBORDINATA);
    CREDO_AEQUALIS_S32 (_semen(doc, ZEPHYRUM), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_pater(doc, ZEPHYRUM), (s32)I);
    CREDO_AEQUALIS_S32 (_species(doc, I),
        (s32)ORATIO_SPECIES_CLAUSULAE_PRINCIPALIS);
    CREDO_AEQUALIS_S32 (_clausula(doc, I), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_causa(doc, I),
        (s32)ORATIO_CLAUSULA_CAUSA_EXTENTUM);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)III), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)V), (s32)I);
    CREDO_AEQUALIS_S32 (_causa(doc, (i32)V),
        (s32)ORATIO_CLAUSULA_CAUSA_CLAUSURA);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)VI), (s32)I);

    /* (d) sine semine: unica ubique */
    doc = _documentum(piscina, &vocabularia, "Puella rosam amat.\n");
    CREDO_NON_NIHIL (doc);
    CREDO_VERUM (_seminare(piscina, doc, &census));
    CREDO_AEQUALIS_I32 (_numerus_clausularum(doc), I);
    CREDO_AEQUALIS_S32 (_species(doc, ZEPHYRUM),
        (s32)ORATIO_SPECIES_CLAUSULAE_PRINCIPALIS);
    CREDO_AEQUALIS_I32 (census.causae[ORATIO_CLAUSULA_CAUSA_UNICA],
        (i32)IV);
    CREDO_AEQUALIS_S32 (_causa(doc, (i32)II),
        (s32)ORATIO_CLAUSULA_CAUSA_UNICA);

    /* (e) relativum conectens initiale: Qui(0) cum(1) venisset(2) ,(3)
     * urbem(4) cepit(5) - Qui principalem aperit, cum subordinatam */
    doc = _documentum(piscina, &vocabularia,
        "Qui cum venisset, urbem cepit.\n");
    CREDO_NON_NIHIL (doc);
    CREDO_VERUM (_seminare(piscina, doc, &census));
    CREDO_AEQUALIS_I32 (_numerus_clausularum(doc), (i32)II);
    CREDO_AEQUALIS_S32 (_species(doc, ZEPHYRUM),
        (s32)ORATIO_SPECIES_CLAUSULAE_PRINCIPALIS);
    CREDO_AEQUALIS_S32 (_semen(doc, ZEPHYRUM), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_species(doc, I),
        (s32)ORATIO_SPECIES_CLAUSULAE_SUBORDINATA);
    CREDO_AEQUALIS_S32 (_pater(doc, I), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)IV), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_causa(doc, (i32)IV),
        (s32)ORATIO_CLAUSULA_CAUSA_CLAUSURA);
    /* (e2) relativum medium: Puella(0) quae(1) ambulat(2) cantat(3) */
    doc = _documentum(piscina, &vocabularia,
        "Puella quae ambulat cantat.\n");
    CREDO_NON_NIHIL (doc);
    CREDO_VERUM (_seminare(piscina, doc, &census));
    CREDO_AEQUALIS_I32 (_numerus_clausularum(doc), (i32)II);
    CREDO_AEQUALIS_S32 (_species(doc, I),
        (s32)ORATIO_SPECIES_CLAUSULAE_RELATIVA);
    CREDO_AEQUALIS_S32 (_clausula(doc, I), (s32)I);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)II), (s32)I);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)III), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_causa(doc, (i32)III),
        (s32)ORATIO_CLAUSULA_CAUSA_CLAUSURA);

    /* (f) corroboratio negata: Cum puella. - nullum verbum: unica */
    doc = _documentum(piscina, &vocabularia, "Cum puella.\n");
    CREDO_NON_NIHIL (doc);
    CREDO_VERUM (_seminare(piscina, doc, &census));
    CREDO_AEQUALIS_I32 (_numerus_clausularum(doc), I);
    CREDO_AEQUALIS_I32 (census.reiecta, I);
    CREDO_AEQUALIS_I32 (census.corroborata, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_causa(doc, ZEPHYRUM),
        (s32)ORATIO_CLAUSULA_CAUSA_UNICA);

    /* (g) parenthesis: Caesar(0) ((1) dux(2) )(3) venit(4) .(5) */
    doc = _documentum(piscina, &vocabularia, "Caesar (dux) venit.\n");
    CREDO_NON_NIHIL (doc);
    CREDO_VERUM (_seminare(piscina, doc, &census));
    CREDO_AEQUALIS_I32 (_numerus_clausularum(doc), (i32)II);
    CREDO_AEQUALIS_S32 (_species(doc, I),
        (s32)ORATIO_SPECIES_CLAUSULAE_PARENTHETICA);
    CREDO_AEQUALIS_S32 (_semen(doc, I), (s32)I);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)II), (s32)I);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)III), (s32)I);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)IV), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census.clausae_signo, I);

        /* (i) SCISSIO verbi (stratum V, T20a bis): Puella(0) cantat(1)
     * puer(2) currit(3) .(4) - sine semine, verbum certum alterum
     * clausulam coordinatam aperit; verba media (puer) ad novam (ordo
     * verbo finali, variatio A mensurata melior); non UNICA */
    doc = _documentum(piscina, &vocabularia,
        "Puella cantat puer currit.\n");
    CREDO_NON_NIHIL (doc);
    CREDO_VERUM (_seminare(piscina, doc, &census));
    CREDO_AEQUALIS_I32 (_numerus_clausularum(doc), (i32)II);
    CREDO_AEQUALIS_S32 (_species(doc, I),
        (s32)ORATIO_SPECIES_CLAUSULAE_COORDINATA);
    CREDO_AEQUALIS_S32 (_semen(doc, I), (s32)III);
    CREDO_AEQUALIS_S32 (_clausula(doc, ZEPHYRUM), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_causa(doc, ZEPHYRUM),
        (s32)ORATIO_CLAUSULA_CAUSA_EXTENTUM);
    CREDO_AEQUALIS_S32 (_clausula(doc, I), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)II), (s32)I);
    CREDO_AEQUALIS_S32 (_causa(doc, (i32)II),
        (s32)ORATIO_CLAUSULA_CAUSA_VERBUM);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)III), (s32)I);
    CREDO_AEQUALIS_S32 (_causa(doc, (i32)III),
        (s32)ORATIO_CLAUSULA_CAUSA_VERBUM);
    CREDO_AEQUALIS_S32 (_causa(doc, (i32)IV),
        (s32)ORATIO_CLAUSULA_CAUSA_VERBUM);
    CREDO_AEQUALIS_I32 (census.scissae, I);
    CREDO_AEQUALIS_I32 (census.causae[ORATIO_CLAUSULA_CAUSA_UNICA],
        ZEPHYRUM);
        /* (i2) Puer(0) currit(1) ,(2) puella(3) cantat(4) .(5): comma
     * principalem non claudit; scissio ad cantat, verba media (comma,
     * puella) ad novam. (Veni, vidi, vici non valet: forma 'veni'
     * lectionem non verbalem fert - incertum, ergo nulla scissio) */
    doc = _documentum(piscina, &vocabularia,
        "Puer currit, puella cantat.\n");
    CREDO_NON_NIHIL (doc);
    CREDO_VERUM (_seminare(piscina, doc, &census));
    CREDO_AEQUALIS_I32 (_numerus_clausularum(doc), (i32)II);
    CREDO_AEQUALIS_S32 (_clausula(doc, ZEPHYRUM), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_clausula(doc, I), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)II), (s32)I);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)III), (s32)I);
    CREDO_AEQUALIS_S32 (_clausula(doc, (i32)IV), (s32)I);
    CREDO_AEQUALIS_S32 (_causa(doc, (i32)III),
        (s32)ORATIO_CLAUSULA_CAUSA_VERBUM);
    CREDO_AEQUALIS_I32 (census.scissae, I);
    /* (i3) verbum in clausula subordinata alterum non scindit:
     * subordinata ad primum clauditur (Si venit, vidit.) */
    doc = _documentum(piscina, &vocabularia, "Si venit, vidit.\n");
    CREDO_NON_NIHIL (doc);
    CREDO_VERUM (_seminare(piscina, doc, &census));
    CREDO_AEQUALIS_I32 (_numerus_clausularum(doc), (i32)II);
    CREDO_AEQUALIS_I32 (census.scissae, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_causa(doc, (i32)III),
        (s32)ORATIO_CLAUSULA_CAUSA_CLAUSURA);

    /* (h) Anglica: nulla lectio finita capax (Moby sine modo) - unica */
    doc = _documentum(piscina, &vocabularia,
        "The cat sat because it was tired.\n");
    CREDO_NON_NIHIL (doc);
    CREDO_VERUM (_seminare(piscina, doc, &census));
    CREDO_AEQUALIS_I32 (_numerus_clausularum(doc), I);
    CREDO_AEQUALIS_S32 (_causa(doc, (i32)III),
        (s32)ORATIO_CLAUSULA_CAUSA_UNICA);

    imprimere("\n--- III. Per resolutionem (census) ---\n");
    {
         InternamentumChorda* intern = internamentum_creare(piscina);
         MateriaLexiconRatum  ratum;
          MateriaLexIudicium  iudicium;
             OratioProgramma* programma;
       OratioResolutioCensus  rc;

        CREDO_NON_NIHIL (intern);
        CREDO_VERUM (materia_lexicon_ratum_facere(&ratum,
            &ORATIO_LEXICON,
            &iudicium));
        programma = oratio_resolutio_programma_onerare(piscina, intern,
            radix, &vitium);
        CREDO_NON_NIHIL (programma);
        doc = _documentum(piscina, &vocabularia,
            "Caesar, cum venisset, urbem cepit. Puella rosam amat.\n");
        CREDO_NON_NIHIL (doc);
        oratio_resolutio_census_vacare(&rc);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &rc));
        CREDO_AEQUALIS_I32 (rc.clausulae.sententiae, (i32)II);
        CREDO_AEQUALIS_I32 (rc.clausulae.clausulae, (i32)III);
        CREDO_AEQUALIS_I32 (rc.clausulae.causae[ORATIO_CLAUSULA_CAUSA_UNICA],
            (i32)IV);
        CREDO_AEQUALIS_I32 (rc.clausulae.species[
            ORATIO_SPECIES_CLAUSULAE_SUBORDINATA], I);
        CREDO_AEQUALIS_I32 (_numerus_clausularum(doc), (i32)II);
        /* cursus alter: stampa idempotens, census sententiarum nullus */
        oratio_resolutio_census_vacare(&rc);
        CREDO_VERUM (oratio_resolutio_applicare(piscina, intern, &ratum,
            programma, (s32)-I, "latina", doc, &rc));
        CREDO_AEQUALIS_I32 (rc.clausulae.sententiae, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus_clausularum(doc), (i32)II);
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
