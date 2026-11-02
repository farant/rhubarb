/* hpack_huffman.h - tabula codicis Huffman HPACK (RFC 7541
 * Appendix B), ab hpack.c sola legenda. */
#ifndef HPACK_HUFFMAN_H
#define HPACK_HUFFMAN_H

#include "latina.h"

/* octeti 0..255 et EOS (256) */
#define HPACK_HUFFMAN_SYMBOLA  CCLVII
#define HPACK_HUFFMAN_EOS      CCLVI

/* codex alineatus ad dextram (bita 'longitudo' inferiora) */
externus constans i32 HPACK_HUFFMAN_CODICES[HPACK_HUFFMAN_SYMBOLA];
/* longitudo codicis in bitis (V..XXX) */
externus constans  i8 HPACK_HUFFMAN_LONGITUDINES[HPACK_HUFFMAN_SYMBOLA];

#endif /* HPACK_HUFFMAN_H */
