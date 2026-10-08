/* norma_stml.h - facies STML normae: plagulae .norma legere et
 * scribere (norma-spec-2 §II-§III)
 *
 * Dialectus in specificatione; canon norma.canon (infixus). Canon
 * PRIMUS iudicat: vocabularium, cardinalitas, genera attributorum,
 * citationes '#x', tituli unici, modificatores in genere alieno.
 * Lector deinde sensum: typus filius unus, fines (bini, parsabiles,
 * non inversi), electio, circuli, gignentes.
 *
 * STATUS: PROBATUM a Frano 2026-10-08 (norma-plan-3 A0).
 *
 * USUS:
 *   NormaStmlLectio l = norma_stml_legere(fons, NIHIL, 0, p);
 *   si (!l.successus) { ... l.vitia: NormaStmlVitium ... }
 *   Norma* r = norma_stml_quaerere(&l, "responsum");
 *   chorda s = norma_stml_scribere(normae, numerus, p, &causa);
 */
#ifndef NORMA_STML_H
#define NORMA_STML_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "norma.h"

/* registrum: gignens="titulus" -> functio */
nomen structura {
     constans character* titulus;
           NormaGignens  functio;
                 vacuum* datum;
} NormaGignensNominatum;

nomen enumeratio {
    NORMA_STML_FRACTUM = 0,          /* STML non parsabile */
    NORMA_STML_CANON,                /* vitium canonis (nuntius nominat) */
    NORMA_STML_LIBERI_TYPI,          /* typus filius non unus */
    NORMA_STML_FINIS_PRAVUS,         /* finis non parsabilis aut extra genus */
    NORMA_STML_FINIS_DIMIDIATUS,     /* minimum sine maximo aut contra */
    NORMA_STML_FINES_INVERSI,        /* minimum > maximum */
    NORMA_STML_ELECTIO_DUPLEX,       /* electio= et <licitum> simul */
    NORMA_STML_ELECTIO_VACUA,        /* electio sine valore */
    NORMA_STML_CIRCULUS,             /* catena <ad> in se redit */
    NORMA_STML_GIGNENS_IGNOTUM,      /* registro dato, titulus abest */
    NORMA_STML_GIGNENS_SINE_REGISTRO /* NOTA: titulus servatur */
} NormaStmlCausa;

nomen structura {
               i32 linea;     /* 1-basata; 0 = ignota */
               i32 columna;   /* 1-basata, octeti; 0 = ignota */
    NormaStmlCausa causa;
            chorda nuntius;
} NormaStmlVitium;

nomen structura {
    chorda  titulus;   /* sine '#' */
     Norma* norma;
} NormaNominata;

nomen structura {
     b32  successus;
     Xar* normae;   /* NormaNominata, ordine documenti; vacuum si !successus */
     Xar* vitia;    /* NormaStmlVitium: canonis primum, deinde lectoris */
     Xar* notae;    /* NormaStmlVitium: GIGNENS_SINE_REGISTRO */
} NormaStmlLectio;

/* gignentes NIHIL = sine registro (tituli servantur, nota quisque);
 * registro dato titulus absens = vitium. Omnia aut nihil: vitio
 * quolibet normae vacuae; vitia OMNIA redduntur. */
NormaStmlLectio
norma_stml_legere (
                               chorda  fons,
       constans NormaGignensNominatum* gignentes,
                                  i32  numerus_gignentium,
                              Piscina* piscina);

/* titulus sine '#' ('#' praefixum toleratur); NIHIL si abest */
Norma*
norma_stml_quaerere (
    constans NormaStmlLectio* lectio,
          constans character* titulus);

/* Forma canonica = stml_scribere pulchrum. Nodus normae nominatae
 * alterius -> <ad norma="#x"/>. Recusat (chorda vacua, *causa
 * posita): titulus vacuus, nodus pravus (SCHEMA_PRAVA), gignens cum
 * functione sine titulo, textus attributi quem STML ferre nequit
 * (" & < > linea nova), circulus (profunditas > CXXVIII). */
chorda
norma_stml_scribere (
    constans NormaNominata* normae,
                       i32  numerus,
                   Piscina* piscina,
                    chorda* causa);

#endif /* NORMA_STML_H */
