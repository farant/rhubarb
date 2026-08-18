/* silva_differre.h - machina differentiae unitatum C89 (communis)
 *
 * Extracta e principalia/differre.c (2026-08-18) ut silex eandem
 * machinam ferat (praeparatus omnibus - unum binarium). Fontes
 * contentorum ad vocatores pertinent (plagulae disci, massae git,
 * volumina silicis) - machina haec textus solos videt.
 *
 * Cursus: latus_ex_textu bis -> paria -> paria_emittere.
 * Semantica (vide principalia/differre.c pro doctrina plena):
 * partitio byte totalis, sigilla spatiorum internata, MOTA sigillo
 * aequali / MUTATA titulo aequali / ADDITA / REMOTA; MUTATA
 * classificantur substantiva / documentaria / cosmetica.
 */

#ifndef SILVA_DIFFERRE_H
#define SILVA_DIFFERRE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "differentia.h"

nomen structura {
    chorda   textus;
    Xar*     lexemata;     /* SilvaToken* */
    Xar*     unitates;     /* SilvaUnitas */
    i32*     initia;       /* byte initium unitatis k */
    i32*     fines;        /* byte finis (exclusivum) */
    vacuum** identitates;  /* sigilla internata (chorda*) */
    i32      numerus;
} SilvaDifferreLatus;

/* par status - a_index/b_index: -1 = latus vacat */
nomen structura {
    constans character* status;   /* MOTA|MUTATA|ADDITA|REMOTA */
    s32                 a_index;
    s32                 b_index;
} SilvaDifferrePar;

/* textus -> latus lexatum cum partitione unitatum; textus datum
 * NIHIL = latus vacuum honestum */
b32
silva_differre_latus_ex_textu (
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda               textus,
    constans character*  titulus,
    SilvaDifferreLatus*  latus);

/* spatium byte unitatis k (alias in textum lateris) */
chorda
silva_differre_spatium (
    constans SilvaDifferreLatus* latus,
    i32                          k);

/* differentia unitatum -> Xar de SilvaDifferrePar (MOTA sigillo,
 * MUTATA titulo, prima non sumpta vincit); *immotae_exitus
 * accumulat; NIHIL = defectus */
Xar*
silva_differre_paria (
    Piscina*                     piscina,
    constans SilvaDifferreLatus* a,
    constans SilvaDifferreLatus* b,
    i32*                         immotae_exitus);

/* emissio parium: -machina TSV aut lectio humana; via_machina non
 * NIHIL = columna viae praefixa; corpora = textus unificatus sub
 * MUTATA; additae/deletae accumulant */
vacuum
silva_differre_paria_emittere (
    Piscina*                     piscina,
    constans SilvaDifferreLatus* a,
    constans SilvaDifferreLatus* b,
    Xar*                         paria,
    b32                          machina,
    constans character*          via_machina,
    b32                          corpora,
    i32*                         additae_totae,
    i32*                         deletae_totae);

/* symbolum unum e textu: spatia concatenata unitatum OMNIUM
 * titulo congruentium (prototypum + definitio una identitate) */
nomen structura {
    b32    inventa;
    chorda textus;        /* spatia concatenata (piscina data) */
    chorda sigillum_hex;  /* identitas contenti (64 hex) */
} SilvaDifferreSymbolum;

/* textum parsare piscina brevi INTERNA (deleta ante reditum -
 * memoria vocatoris plana trans ambulationes), spatia in piscinam
 * datam concatenare. textus vacuus/NIHIL = latus vacuum honestum
 * (inventa FALSUM, VERUM redditur). FALSUM = defectus verus. */
b32
silva_differre_symbolum_ex_textu (
    Piscina*               piscina,
    chorda                 textus,
    constans character*    titulus,
    SilvaDifferreSymbolum* exitus);

/* classificatio duorum textuum planorum (sine unitatibus):
 * substantiva | documentaria | cosmetica */
constans character*
silva_differre_classificare_textus (
    Piscina* piscina,
    chorda   a,
    chorda   b);

/* summa linearum duorum textuum (differentia_linearum involuta;
 * defectus = summa nulla) */
DifferentiaSumma
silva_differre_summa_textuum (
    Piscina* piscina,
    chorda   vetus,
    chorda   novum);

/* titulum imprimere ("(sine titulo)" si vacuus) */
vacuum
silva_differre_titulum_imprimere (chorda titulus);

/* praefixum + titulus ut cstr (e.g. "a/functio_x") */
constans character*
silva_differre_titulum_cstr (
    Piscina*            piscina,
    constans character* praefixum,
    chorda              titulus);

#endif /* SILVA_DIFFERRE_H */
