/* toml_lector.h - Lector toml: functio (modi, positionis)
 *
 * DUO MODI (spec par. III; T3): CLAVIS (initium lineae, intra capita,
 * in tabula compacta ante '=') et VALOR (post '=', intra seriem).
 * Idem octetus modo decernente genera diversa fit: '[' caput aut
 * series; '1979-05-27' clavis nuda aut tempus; 'true' clavis aut
 * valor. Aedificator modum eligit - lector numquam relabellat.
 *
 * OMNIS OCTETUS IN LEXEMATE (totalitas, T4): spatia, commentaria,
 * lineae novae lexemata sunt; quod nulla regula accipit IGNOTUM fit
 * (sequentia UTF-8 una, aut octetus unus si invalida). Concatenatio
 * omnium lexematum = fons (porta lectoris id asserit).
 *
 * LINEA NOVA: LINEA_FINIS si profunditas uncorum ([ et { apertorum,
 * ab aedificatore posita) nulla, aliter LINEA (trivium). CRLF unum
 * lexema (duo octeti); CR solus IGNOTUM.
 *
 * LEXEMA NUMERO SIMILE: cursus avidus octetorum [A-Za-z0-9_+-.:];
 * cursus 'true'/'false' exacte = VERUM/FALSUM; ':' aut forma diei
 * (DDDD-) = TEMPUS; initium digitus, signum, 'inf'/'nan' = NUMERUS;
 * cetera IGNOTUM. Spatium UNUM diem integrum (DDDD-DD-DD) et horam
 * (DD:) iungit ('1979-05-27 07:32:00' lexema unum); '1979-05-27 #'
 * non. Classis vera (integer/fluitans, forma temporis) in coctione.
 *
 * VALIDITAS NON HIC: octeti intra commentaria et chordas (UTF-8
 * invalida, characteres moderatores) in lexemate manent; coctio
 * (toml_coctum) iudicat. utf8_decodere sequentias longiores,
 * surrogatas, ultra U+10FFFF, truncatas iam recusat (mensuratum
 * 2026-09-28) - lector ei innititur. BOM (U+FEFF) extra chordas
 * IGNOTUM est: tomllib eum recusat (planum Q1).
 */

#ifndef TOML_LECTOR_H
#define TOML_LECTOR_H

#include "latina.h"
#include "piscina.h"
#include "materia_token.h"
#include "toml_lexicon.h"

nomen enumeratio {
    TOML_MODUS_CLAVIS = 0,
    TOML_MODUS_VALOR,
    TOML_MODUS_NUMERUS_MODORUM
} TomlModus;

nomen structura {
    s32 cursor;
    i32 linea;
    s32 linea_initium;
} TomlSitus;

nomen structura {
                Piscina* piscina;
     constans character* fons;
                    s32  mensura;
              TomlSitus  situs;
                    i32  profunditas;   /* [ et { aperti */
      MateriaTokenForma  forma;
} TomlLector;

/* fons NON copiatur: lexemata in eum monstrant */
b32
toml_lector_incipere (
             TomlLector* lector,
                Piscina* piscina,
     constans character* fons,
                    s32  mensura);

/* Lexema proximum in modo dato; FINIS (vacuum) in fine, iterum et
 * iterum. NIHIL = memoria deficit. */
MateriaToken*
toml_lector_proximum (
     TomlLector* lector,
      TomlModus  modus);

TomlSitus
toml_lector_situs (
    constans TomlLector* lector);

vacuum
toml_lector_situm_reponere (
     TomlLector* lector,
      TomlSitus  situs);

vacuum
toml_lector_profunditatem_ponere (
    TomlLector* lector,
           i32  profunditas);

#endif /* TOML_LECTOR_H */
