/* briar_lexema.h - Tabula linearum, classificatio linearum, fabrica
 * lexematum thistle.
 *
 * COLUMNA 0 SOLA LEGITUR (spec par. II): '<' + initium nominis aperit
 * tagum, '</nomen>' claudit, cetera prosa. SAEPTA markdown (``` aut
 * ~~~, indentatio <= III) commutant statum: intra saeptum omnis linea
 * prosa est. Linea prima '#!' = interpres. Lexema super [ab, ad)
 * quodlibet, etiam trans lineas - positio ex tabula linearum.
 */

#ifndef BRIAR_LEXEMA_H
#define BRIAR_LEXEMA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "materia_token.h"

nomen structura {
    s32 offset;       /* initium contenti in fonte */
    i32 mensura;      /* octeti contenti, sine terminatore */
    i32 terminator;   /* ZEPHYRUM (finis), I ("\n"|"\r"), II ("\r\n") */
    i32 numerus;      /* I-basatus */
} BriarLinea;

nomen structura {
    constans character* fons;
                   i32  mensura;
                   Xar* lineae;    /* Xar de BriarLinea (per valorem) */
} BriarLineae;

b32
briar_lineae_scindere (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
           BriarLineae* exitus);

i32
briar_lineae_numerus (
    constans BriarLineae* lineae);

/* NIHIL si index extra */
constans BriarLinea*
briar_linea (
    constans BriarLineae* lineae,
                     i32  index);

/* Visus in fontem, nulla copia */
chorda
briar_chorda_fontis (
    constans character* fons,
                   s32  ab,
                   s32  ad);

/* ---- classificatio ---- */

nomen enumeratio {
    BRIAR_LINEA_PROSA = 0,
    BRIAR_LINEA_INTERPRES,
    BRIAR_LINEA_TAG_APERTUM,
    BRIAR_LINEA_TAG_CLAUSUM
} BriarLineaGenus;

/* status saepti markdown, per lineas portatus */
nomen structura {
          b32 aperta;
    character signum;      /* '`' aut '~' */
          i32 longitudo;   /* cursus aperiens */
} BriarSaeptum;

nomen structura {
    BriarLineaGenus genus;
             chorda titulus;       /* nomen tagi (visus in fontem) */
                b32 crudus;        /* '!' post nomen */
                b32 dedens;        /* '\' post nomen aut '!' */
                b32 auto_clausum;  /* linea in "/>" desinit */
} BriarClassis;

/* Lineam classificare; saeptum RENOVATUR (apertura/clausura). */
BriarClassis
briar_lineam_classificare (
    constans BriarLineae* lineae,
                     i32  index,
            BriarSaeptum* saeptum);

/* Lexatio cruda: lexema unum per lineam (genus ex classificatione,
 * INTERPRES/TAG_APERTUM/TAG_CLAUSUM/PROSA), FINIS ultimum. Xar de
 * MateriaToken*. Valores concatenati == fons. Status regionum NON
 * fertur - porta classificationis, non arbor. */
Xar*
briar_lexare_crude (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

/* ---- fabrica lexematum ---- */

nomen structura {
              Piscina* piscina;
    constans BriarLineae* lineae;
    MateriaTokenForma  forma;
                  i32  linea_ultima;   /* cursor quaesitionis lineae */
} BriarFabrica;

vacuum
briar_fabrica_incipere (
             BriarFabrica* fabrica,
                  Piscina* piscina,
     constans BriarLineae* lineae);

/* Lexema super [ab, ad) - positio (linea, columna) octeti 'ab' ex
 * tabula; ad > ab aut ad == ab (vacuum) licet. NIHIL = memoria. */
MateriaToken*
briar_lexema_fabricare (
    BriarFabrica* fabrica,
             s32  genus,
             s32  ab,
             s32  ad);

/* FINIS post octetum ultimum */
MateriaToken*
briar_lexema_finis (
    BriarFabrica* fabrica);

/* Lexema DERIVATUM (fons I): byte_offset -I, positio originis */
MateriaToken*
briar_lexema_derivatum (
             BriarFabrica* fabrica,
                      s32  genus,
                   chorda  valor,
    constans MateriaToken* origo);

#endif /* BRIAR_LEXEMA_H */
