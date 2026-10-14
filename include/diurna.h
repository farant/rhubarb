/*
 * diurna.h - ACTA DIURNA: taberna actorum (logs)
 *
 * Franus 2026-10-13: "metrics and logs are a different thing ... a
 * solution tailored to their use case". Tria genera recordationum
 * (project-specs/analytica-spec.md par. III semis): SPANS (cursus, in
 * volumine mensurae), ACTA (haec bibliotheca), METRA (ex spans
 * derivata; taberna propria = Metrum, parcata). Iunguntur per CLAVEM
 * solam (id cursus) - diurna nec mensuram nec volumen novit.
 *
 * EXEMPLAR: Loki, non Elasticsearch. Labella pauca et cardinalitatis
 * humilis (titulus, fractum); clavis = identitas introitus, NUMQUAM
 * labellum. Quaestio = corpora intra fenestram labellorum/temporis
 * legere, non index verborum.
 *
 * FORMA SUB RADICE:
 *   corpora/II-hex/LXII-hex      corpus crudum (SHA-256 contenti)
 *   corpora/II-hex/LXII-hex.gz   forma compressa (post purgationem)
 *   index.tsv                        lineae solum appensae:
 *     positum  momentum clavis seq titulus fractum sigillum mensura
 *              originalis
 *     fixum    momentum clavis           (servatur in aeternum)
 *     compressum momentum sigillum       (corporis, non clavis)
 *     deletum  momentum sigillum
 *
 * CLAVIS + SEQ: seq ZEPHYRUM = corpus totum cursus. Fluxus (residentes,
 * servitores: fragmenta ordine temporis) eandem clavem cum seq I, II...
 * postea adhibebunt - additio, non forma nova.
 *
 * CORPUS UNUM, CLAVES MULTAE: corpus idem (effusio quieta) semel
 * conditur. Retentio per CLAVEM iudicat, repositio per CORPUS: corpus
 * comprimitur aut deletur solum cum nulla clavis id recentius poscit.
 * Corpus deletum lineam suam servat: legere "deletum, momento M" dicit,
 * non "ignotum".
 *
 * SCRIPTURA: corpus PRIMUM (nomen temporarium, deinde rename - lector
 * numquam corpus dimidium videt; praesens non rescribitur), DEINDE
 * linea indicis (appensio firma) - linea numquam ad corpus absens
 * spectat. Sutura UNA corporis: diurna_ponere (redactio postea intra).
 */

#ifndef DIURNA_H
#define DIURNA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "sigillum.h"

/* corpus maximum servatum: cauda servatur, caput praeciditur */
#define DIURNA_CORPUS_MAXIMUM (VIII * MXXIV * MXXIV)

nomen structura Diurna Diurna;

nomen enumeratio {
    DIURNA_CRUDUM = ZEPHYRUM,  /* corpus in disco ut positum */
    DIURNA_COMPRESSUM,         /* .gz solum */
    DIURNA_DELETUM             /* corpus abest, linea manet */
} DiurnaForma;

nomen enumeratio {
    DIURNA_ADEST = ZEPHYRUM,
    DIURNA_IGNOTUM,            /* clavis numquam posita */
    DIURNA_ABLATUM,            /* posita, corpus deletum */
    DIURNA_CORRUPTUM           /* corpus non congruit sigillo */
} DiurnaStatus;

/* introitus unus per (clavis, seq): positum ultimum vincit */
nomen structura {
       chorda clavis;
          i32 seq;
          s64 momentum;          /* ms ab epocha, positum */
       chorda titulus;
          b32 fractum;
          b32 fixum;
    character sigillum[SIGILLUM_HEX_MENSURA];
          s64 mensura;           /* octeti servati */
          s64 originalis;        /* octeti ante praecisionem */
  DiurnaForma forma;
          s64 momentum_formae;   /* ms: compressum/deletum; 0 crudum */
} DiurnaIntroitus;

/* radix creatur si abest (corpora/ quoque). NIHIL si creari nequit. */
Diurna*
diurna_aperire (
    constans character* radix,
               Piscina* piscina);

/* corpus ponere sub (clavis, seq ZEPHYRUM). clavis et titulus puri
 * (sine TAB, linea nova) et clavis non vacua - aliter FALSUM, nihil
 * scriptum. Corpus > DIURNA_CORPUS_MAXIMUM: cauda servatur, linea
 * '[... N octeti praecisi]' praeponitur. positum_out (NIHIL licet):
 * introitus scriptus. */
b32
diurna_ponere (
               Diurna* diurna,
               chorda  clavis,
               chorda  titulus,
                  b32  fractum,
               chorda  corpus,
      DiurnaIntroitus* positum_out);

/* corpus clavis (seq ZEPHYRUM): .gz inflatur; sigillum verificatur.
 * status_out (NIHIL licet). Chorda vacua nisi DIURNA_ADEST. */
chorda
diurna_legere (
          Diurna* diurna,
          chorda  clavis,
         Piscina* piscina,
    DiurnaStatus* status_out);

/* clavem figere: corpus eius numquam comprimitur nec deletur (e.g. id
 * cursus in tabulario aut worklog nominatum). FALSUM si ignota. */
b32
diurna_figere (
    Diurna* diurna,
    chorda  clavis);

/* status omnium introituum, ordine positionis primae. Xar de
 * DiurnaIntroitus (chordae ex piscina vocantis). */
Xar*
diurna_introitus (
     Diurna* diurna,
    Piscina* piscina);

constans character*
diurna_status_nomen (
    DiurnaStatus status);

#endif /* DIURNA_H */
