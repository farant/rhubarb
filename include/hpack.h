/* hpack.h - compressio capitum HTTP/2 (RFC 7541; http2-spec III.1)
 *
 * PURA: nullus socket, nullum horologium, nihil extra piscinam
 * vocantis; idem exitus pro eodem introitu.
 *
 * Gradus I (http2-plan T1): primitiva - integra cum praefixo N
 * bitorum (5.1), chordae Huffman (5.2, Appendix B), tabula statica
 * (Appendix A). Tabula dynamica, decodator et codificator: T2. */
#ifndef HPACK_H
#define HPACK_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

/* numerus entrium tabulae staticae (Appendix A) */
#define HPACK_STATICA_NUMERUS  LXI

/* integrum 'valor' cum praefixo 'prefixum' bitorum (I..VIII) in
 * exitus; 'primus' = bita alta iam in octeto primo (e.g. 0x80
 * repraesentationis indicatae). Reddit octetos scriptos, 0 si
 * capacitas deficit aut praefixum malum. */
i32
hpack_integrum_scribere (
    i32  valor,
    i32  prefixum,
    i32  primus,
     i8* exitus,
    i32  capacitas);

/* integrum ex datum (bita alta octeti primi neglecta); consumpti =
 * octeti lecti. FALSUM si truncatum aut valor supra 2^31-1. */
b32
hpack_integrum_legere (
    constans i8* datum,
            i32  n,
            i32  prefixum,
            i32* valor,
            i32* consumpti);

/* octeti codificationis Huffman chordae s (impletione inclusa) */
i32
hpack_huffman_longitudo (
    chorda s);

/* s codificata Huffman in exitus, impletio bitis unius (EOS
 * praefixum); reddit octetos, 0 si capacitas deficit */
i32
hpack_huffman_scribere (
     chorda  s,
         i8* exitus,
        i32  capacitas);

/* decodificatio in piscina; FALSUM si impletio plus VII bitis, si
 * impletio non bitis unius, aut EOS intra datum (5.2) */
b32
hpack_huffman_legere (
    constans i8* datum,
            i32  n,
        Piscina* piscina,
         chorda* exitus);

/* entrium tabulae staticae (I..LXI); FALSUM extra. Chordae in
 * memoria statica (numquam liberandae). */
b32
hpack_staticum (
       i32  index,
    chorda* titulus,
    chorda* valor);


/* ==================================================
 * T2: tabula dynamica, decodator, codificator
 * ================================================== */

/* caput: titulus (minusculis, HTTP/2) et valor; numquam = repraesentatio
 * "never indexed" (6.2.3) - decodator id servat, codificator id
 * honorat (et sponte ponit pro authorization, proxy-authorization,
 * cookie brevi) */
nomen structura {
    chorda titulus;
    chorda valor;
       b32 numquam;
} HpackCaput;

nomen enumeratio {
    HPACK_HUFFMAN_SI_BREVIOR = ZEPHYRUM,
    HPACK_HUFFMAN_SEMPER,
    HPACK_HUFFMAN_NUMQUAM
} HpackHuffman;

nomen enumeratio {
    HPACK_BENE = ZEPHYRUM,
    HPACK_ERROR_INDEX,       /* index 0 aut extra tabulas */
    HPACK_ERROR_INTEGRUM,    /* integrum supra 2^31-1 */
    HPACK_ERROR_HUFFMAN,     /* chorda Huffman mala (5.2) */
    HPACK_ERROR_MAGNITUDO,   /* renovatio supra limitem aut non initio */
    HPACK_ERROR_TRUNCATUM,   /* massa medio abscissa */
    HPACK_ERROR_NIMIA        /* index capitum supra capita_maxima */
} HpackError;

/* tabula dynamica unius directionis (decodator aut codificator);
 * opaca */
nomen structura HpackTabula HpackTabula;

/* 'mensura' = SETTINGS_HEADER_TABLE_SIZE in vigore et capacitas
 * octetorum (numquam crescit ultra). Memoria fixa: evictio non
 * accumulat. NIHIL si piscina deficit. */
HpackTabula*
hpack_tabulam_creare (
    Piscina* piscina,
        i32  mensura);

/* codificator: renovatio magnitudinis (min(mensura, capacitas)) in
 * initio massae proximae; decodator: limes novus renovationum */
vacuum
hpack_magnitudinem_ponere (
    HpackTabula* t,
            i32  mensura);

/* codificator: quando Huffman (ordinarie SI_BREVIOR) */
vacuum
hpack_huffman_ponere (
     HpackTabula* t,
    HpackHuffman  modus);

/* massa capitum -> HpackCaput in 'capita' (chordae in piscina);
 * capita_maxima: summa (titulus + valor + XXXII) maxima, 0 = sine
 * limite (RFC 9113 6.5.2). Post errorem tabula incerta - connexio
 * moritur (COMPRESSION_ERROR). */
HpackError
hpack_decodere (
    HpackTabula* t,
    constans i8* datum,
            i32  n,
            i32  capita_maxima,
        Piscina* piscina,
            Xar* capita);

/* capita -> massa; reddit octetos, 0 si capacitas deficit (tabula tum
 * immutata). Consilium (http2-plan C2): par exactum -> indicatum;
 * aliter literale cum indice (titulus indicatus si notus); numquam /
 * secreta -> literale numquam indicatum; entrium maius tabula ->
 * literale sine indice. */
i32
hpack_codificare (
            HpackTabula* t,
    constans HpackCaput* capita,
                    i32  numerus,
                     i8* exitus,
                    i32  capacitas);

/* status tabulae (probationes, C.3-C.6): entria, magnitudo (summa
 * titulus + valor + XXXII), entrium i (I = recentissimum) */
i32
hpack_tabula_numerus (
    constans HpackTabula* t);

i32
hpack_tabula_magnitudo (
    constans HpackTabula* t);

b32
hpack_tabula_obtinere (
    constans HpackTabula* t,
                     i32  i,
                  chorda* titulus,
                  chorda* valor);

#endif /* HPACK_H */
