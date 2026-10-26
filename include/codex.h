/* codex.h - liber EPUB (lector L2; spec project-specs/lector-spec.md):
 * metadata, capita (unum per introitum indicis, et prooemium ante
 * primum), caput = textus planus in clausulas sectus (titulus,
 * paragraphus, versus), sententiae cum offsetibus stabilibus,
 * emendationes (iungere, findere) ut data. Arca per sarcina; index et
 * OPF per xml.h, capita per html_lexema (tolerans: XHTML pravum
 * legitur). Nulla UI, nullum volumen.
 *
 * TEXTUS CAPITIS (contractus stabilis): clausulae ordine documenti
 * "\n\n" iunctae; in titulo et versu <br/> = '\n'; spatia contracta,
 * lineae purgatae; <i> <b> etc. solum textum dant. Offsets = octeti in
 * hoc textu. Paginae involucrorum imaginum (nihil textus extra <a>)
 * omittuntur (imagines non in v1). */
#ifndef CODEX_H
#define CODEX_H

/* <aedilis corpus="lib/codex.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"

nomen structura Codex Codex;   /* opacus */

nomen enumeratio {
    CODEX_TITULUS = ZEPHYRUM,  /* h1-h6 */
    CODEX_PARAGRAPHUS,         /* p, li, ceteri */
    CODEX_VERSUS               /* poem/stanza/verse in classe */
} CodexGenus;

nomen structura {
    CodexGenus genus;
           i32 initium;   /* octeti in textu capitis */
           i32 finis;     /* exclusivus */
} CodexClausula;

nomen structura {
    chorda  titulus;     /* nomen in indice; vacuum = prooemium */
    chorda  textus;      /* textus capitis (contractus supra) */
       Xar* clausulae;   /* CodexClausula */
} CodexCaput;

nomen structura {
    i32 initium;      /* octeti in textu capitis = IDENTITAS */
    i32 finis;
    i32 clausula;     /* index clausulae */
} CodexSententia;

nomen enumeratio {
    CODEX_IUNGERE = ZEPHYRUM,   /* sententia ad positionem + sequens */
    CODEX_FINDERE               /* nova sententia ad positionem */
} CodexEmendationisGenus;

nomen structura {
    CodexEmendationisGenus genus;
                       i32 positio;
} CodexEmendatio;

/* datum = arca EPUB (filum lectum); NIHIL et *vitium si non est */
Codex*
codex_aperire (
                 chorda  datum,
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda* vitium);

/* dc:identifier unicus (package@unique-identifier), aliter primus */
chorda
codex_identitas (
    constans Codex* c);

/* dc:title */
chorda
codex_titulus (
    constans Codex* c);

/* dc:creator primus */
chorda
codex_auctor (
    constans Codex* c);

/* dc:language */
chorda
codex_lingua (
    constans Codex* c);

i32
codex_numerus_capitum (
    constans Codex* c);

/* titulus capitis k in indice (vacuum = prooemium, aut k extra
 * ambitum) - sine lectione capitis (index libri in L3) */
chorda
codex_caput_titulus (
    constans Codex* c,
               i32  k);

/* caput k (textus, clausulae) in piscina; NIHIL et *vitium si k extra
 * ambitum aut membrum deest aut legi nequit */
CodexCaput*
codex_caput_legere (
    constans Codex* c,
               i32  k,
           Piscina* piscina,
            chorda* vitium);

/* sententiae capitis ordine, emendationibus applicatis (ordo
 * emendationum non refert); Xar de CodexSententia. Titulus = sententia
 * una, versus = una per lineam, paragraphus per sententia_fissio.
 * Fissurae omnes ante iuncturas: findere solum intra sententiam
 * (initium < positio < finis; pars nova ad primum non-spatium incipit);
 * iungere solum sententiam cuius initium = positio, cum sequente in
 * EADEM clausula. Cetera (positio ignota, finis capitis aut clausulae,
 * idem bis) nihil mutant. */
Xar*
codex_sententiae (
        constans CodexCaput* caput,
    constans CodexEmendatio* emendationes,
                        i32  numerus,
                    Piscina* piscina);

/* sententiam ancoratam reperire: positio + vestigium (primi octeti
 * eius textus) -> positio vera; congruit -> positio; aliter quaeritur
 * prope (+/- M octeti) - inventa -> ea, aliter -1 */
s32
codex_reperire (
     constans CodexCaput* caput,
                     i32  positio,
                  chorda  vestigium);

#endif /* CODEX_H */
