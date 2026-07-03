/* silva_amalgama.h - Machina amalgamationis (dev-time, Phase 7 C /
 * tessera Phase C)
 *
 * MECHANISMUS sine manifesto: scansor unitatum C89, renominator
 * lexemate-conscius (lexatore silvae), iniectio static, detectio
 * definitionum duplicatarum (S41), sublatio includendorum systematis,
 * regula CADENDA (typi capitis manu scripti), emissio dati latini
 * (optionalis), compositio exitus. MANIFESTUM = codex in plagula
 * principali cuiusque proiecti (silva: principalia/amalgamator.c;
 * tessera: tessera/instrumenta/principalia/amalgamator.c).
 *
 * Extractum ex amalgamatore silvae (Phase 6) mutatione mechanica:
 * novem sedes globales campi manifesti factae; PORTA: silva.c
 * byte-identicum regenerat.
 */

#ifndef SILVA_AMALGAMA_H
#define SILVA_AMALGAMA_H

#include "latina.h"
#include "piscina.h"

/* Plagula manifesti */
nomen structura {
    constans character*  via;      /* relativa ad radicem */
    constans character* constans* servanda;   /* NIHIL = omnia */
    constans character* constans* excludenda; /* cadenda; NIHIL = nulla */
    b32                  est_corpus;    /* definitiones (S41) */
    b32                  est_vendicata; /* static iniectio + rename */
} AmalgamaPlagula;

/* Renominatio (typi exacti / praefixa functionum) */
nomen structura {
    constans character* vetus;
    constans character* novum;
} Renominatio;

/* Manifestum totum - quod proiectum amalgamandum definit */
nomen structura {
    constans AmalgamaPlagula* capita_vendicata;
    i32                       numerus_capitum_vendicatorum;
    constans AmalgamaPlagula* corpora_vendicata;
    i32                       numerus_corporum_vendicatorum;
    constans AmalgamaPlagula* capita_propria;
    i32                       numerus_capitum_propriorum;
    constans AmalgamaPlagula* corpora_propria;
    i32                       numerus_corporum_propriorum;

    /* Typi quos caput manu scriptum possidet (NIHIL-terminata) */
    constans character* constans* cadenda_typedef;
    constans character* constans* cadenda_definitio;
    /* Functiones vendicatae publicae manentes (NIHIL-terminata) */
    constans character* constans* non_statica;

    constans Renominatio* typi_exacti;
    i32                   numerus_typorum;
    constans Renominatio* praefixa_functionum;  /* longissimum primum */
    i32                   numerus_praefixorum;

    constans character* via_capitis;  /* caput manu scriptum, verbatim */
    constans character* prooemium;    /* commentarius initialis exitus */
    constans character* assertiones;  /* textus C post capita vendicata;
                                       * NIHIL licet */

    /* Datum latinum (silva solum); via_capitis NIHIL = non emittere */
    constans character* latina_datum_via_capitis;
    constans character* latina_datum_via_corporis;
    constans character* latina_datum_prooemium;  /* commentarius capitis
                                                  * INTEGER (sine custode) */
    constans character* latina_datum_custos;
    constans character* latina_datum_titulus_textus;
    constans character* latina_datum_titulus_mensurae;
} AmalgamaManifestum;

/* Currere: legere, transformare, componere, scribere. VERUM = bene. */
b32
silva_amalgama_currere (
    Piscina*                   piscina,
    constans character*        radix,
    constans character*        via_exitus,
    constans AmalgamaManifestum* manifestum);

#endif /* SILVA_AMALGAMA_H */
