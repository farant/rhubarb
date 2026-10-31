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

#endif /* HPACK_H */
