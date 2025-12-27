/* flatura.c - DEFLATE/gzip Compressio et Decompressio
 *
 * Implementatio DEFLATE (RFC 1951) et gzip (RFC 1952).
 */

#include "flatura.h"
#include <string.h>
#include <stdio.h>

/* Debug flag - set to 1 to enable debug output */
#define FLATURA_DEBUG 0

/* ========================================================================
 * CONSTANTIAE INTERNAE
 * ======================================================================== */

#define FLATURA_LITERAE_MAXIMA      286         /* 286 literal/length codes */
#define FLATURA_DISTANTIAE_MAXIMA   XXX         /* 30 distance codes */
#define FLATURA_CODEX_LONGITUDO_MAXIMA  XIX     /* 19 code length codes */
#define FLATURA_BITS_MAXIMA         XV          /* 15 max Huffman code length */
#define FLATURA_END_BLOCK           256         /* End of block symbol */
#define FLATURA_FIRST_LEN           257         /* First length symbol */


/* ========================================================================
 * CRC-32 TABULA
 * ======================================================================== */

hic_manens i32 crc32_tabula[CCLVI];
hic_manens b32 crc32_initiatum = FALSUM;

interior vacuum
_flatura_crc32_initium(vacuum)
{
    i32 i;
    i32 j;
    i32 c;

    si (crc32_initiatum)
    {
        redde;
    }

    per (i = 0; i < CCLVI; i++)
    {
        c = i;
        per (j = 0; j < VIII; j++)
        {
            si (c & I)
            {
                c = (i32)0xEDB88320 ^ ((c >> I) & 0x7FFFFFFF);
            }
            alioquin
            {
                c = (c >> I) & 0x7FFFFFFF;
            }
        }
        crc32_tabula[i] = c;
    }

    crc32_initiatum = VERUM;
}


i32
flatura_crc32_continuare(
    i32          crc,
    constans i8* datum,
    i32          mensura)
{
    i32 i;

    _flatura_crc32_initium();

    per (i = 0; i < mensura; i++)
    {
        crc = crc32_tabula[(crc ^ datum[i]) & 0xFF] ^ ((crc >> VIII) & 0xFFFFFF);
    }

    redde crc;
}


i32
flatura_crc32(
    constans i8* datum,
    i32          mensura)
{
    i32 crc;

    crc = 0xFFFFFFFF;
    crc = flatura_crc32_continuare(crc, datum, mensura);

    redde crc ^ 0xFFFFFFFF;
}


/* ========================================================================
 * BIT LECTOR (pro decompressione)
 * ======================================================================== */

nomen structura {
    constans i8* datum;
    i32          mensura;
    i32          positus;
    i32          bit_buffer;
    i32          bits_in_buffer;
} FlaturaLector;


interior vacuum
_flatura_lector_initium(
    FlaturaLector* lector,
    constans i8*   datum,
    i32            mensura)
{
    lector->datum = datum;
    lector->mensura = mensura;
    lector->positus = 0;
    lector->bit_buffer = 0;
    lector->bits_in_buffer = 0;
}


interior s32
_flatura_legere_bits(
    FlaturaLector* lector,
    i32            numerus_bits)
{
    i32 valor;

    /* Implere buffer si necessarium */
    dum (lector->bits_in_buffer < numerus_bits)
    {
        si (lector->positus >= lector->mensura)
        {
            redde -1; /* Non satis datum */
        }
        lector->bit_buffer |= ((i32)lector->datum[lector->positus] << lector->bits_in_buffer);
        lector->positus++;
        lector->bits_in_buffer += VIII;
    }

    /* Extrahere bits */
    valor = lector->bit_buffer & ((I << numerus_bits) - I);
    lector->bit_buffer >>= numerus_bits;
    lector->bits_in_buffer -= numerus_bits;

    redde (s32)valor;
}


interior vacuum
_flatura_saltare_ad_byte(FlaturaLector* lector)
{
    /* Discardere bits residuos in byte currenti */
    lector->bit_buffer = 0;
    lector->bits_in_buffer = 0;
}


/* ========================================================================
 * BIT SCRIPTOR (pro compressione)
 * ======================================================================== */

nomen structura {
    i8*      datum;
    i32      capacitas;
    i32      positus;
    i32      bit_buffer;
    i32      bits_in_buffer;
    Piscina* piscina;
} FlaturaScriptor;


interior vacuum
_flatura_scriptor_initium(
    FlaturaScriptor* scriptor,
    Piscina*         piscina,
    i32              capacitas_initialis)
{
    scriptor->piscina = piscina;
    scriptor->capacitas = capacitas_initialis;
    scriptor->datum = (i8*)piscina_allocare(piscina, (memoriae_index)capacitas_initialis);
    scriptor->positus = 0;
    scriptor->bit_buffer = 0;
    scriptor->bits_in_buffer = 0;
}


interior vacuum
_flatura_scriptor_crescere(FlaturaScriptor* scriptor)
{
    i32  nova_capacitas;
    i8*  novum_datum;

    nova_capacitas = scriptor->capacitas * II;
    novum_datum = (i8*)piscina_allocare(scriptor->piscina, (memoriae_index)nova_capacitas);
    memcpy(novum_datum, scriptor->datum, (size_t)scriptor->positus);
    scriptor->datum = novum_datum;
    scriptor->capacitas = nova_capacitas;
}


interior vacuum
_flatura_scribere_bits(
    FlaturaScriptor* scriptor,
    i32              valor,
    i32              numerus_bits)
{
    scriptor->bit_buffer |= (valor << scriptor->bits_in_buffer);
    scriptor->bits_in_buffer += numerus_bits;

    dum (scriptor->bits_in_buffer >= VIII)
    {
        si (scriptor->positus >= scriptor->capacitas)
        {
            _flatura_scriptor_crescere(scriptor);
        }
        scriptor->datum[scriptor->positus] = (i8)(scriptor->bit_buffer & 0xFF);
        scriptor->positus++;
        scriptor->bit_buffer >>= VIII;
        scriptor->bits_in_buffer -= VIII;
    }
}


interior vacuum
_flatura_scriptor_finire(FlaturaScriptor* scriptor)
{
    /* Flush bits residuos */
    si (scriptor->bits_in_buffer > 0)
    {
        si (scriptor->positus >= scriptor->capacitas)
        {
            _flatura_scriptor_crescere(scriptor);
        }
        scriptor->datum[scriptor->positus] = (i8)(scriptor->bit_buffer & 0xFF);
        scriptor->positus++;
        scriptor->bit_buffer = 0;
        scriptor->bits_in_buffer = 0;
    }
}


/* ========================================================================
 * HUFFMAN ARBOR
 * ======================================================================== */

/* Tabula pro decodificando Huffman symbola */
nomen structura {
    s16* symbola;       /* Symbolum pro quoque codice (-1 = invalidus) */
    i8*  longitudines;  /* Longitudo codi pro quoque codice */
    i32  numerus;       /* Numerus symbolorum */
    i32  maxima_bits;   /* Maxima longitudo codi */
} HuffmanTabula;


/* Extra bits pro longitudinibus (symbola 257-285) */
hic_manens constans i8 longitudo_extra_bits[XXIX] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 2, 2, 2, 2,
    3, 3, 3, 3, 4, 4, 4, 4,
    5, 5, 5, 5, 0
};

hic_manens constans i16 longitudo_basis[XXIX] = {
    3, 4, 5, 6, 7, 8, 9, 10,
    11, 13, 15, 17, 19, 23, 27, 31,
    35, 43, 51, 59, 67, 83, 99, 115,
    131, 163, 195, 227, 258
};


/* Extra bits pro distantiis (symbola 0-29) */
hic_manens constans i8 distantia_extra_bits[XXX] = {
    0, 0, 0, 0, 1, 1, 2, 2,
    3, 3, 4, 4, 5, 5, 6, 6,
    7, 7, 8, 8, 9, 9, 10, 10,
    11, 11, 12, 12, 13, 13
};

hic_manens constans i16 distantia_basis[XXX] = {
    1, 2, 3, 4, 5, 7, 9, 13,
    17, 25, 33, 49, 65, 97, 129, 193,
    257, 385, 513, 769, 1025, 1537, 2049, 3073,
    4097, 6145, 8193, 12289, 16385, 24577
};


/* Ordo codicum longitudinum pro dynamic Huffman */
hic_manens constans i8 codex_longitudo_ordo[XIX] = {
    16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
};


interior b32
_flatura_huffman_construere(
    HuffmanTabula* tabula,
    constans i8*   longitudines,
    i32            numerus,
    Piscina*       piscina)
{
    i32  bl_count[XVI];  /* Numerus codicum per longitudinem */
    i32  next_code[XVI]; /* Proximus codex pro quaque longitudine */
    s32* codices;
    i32  i;
    i32  bits;
    i32  codex;
    i32  maxima_bits;
    i32  tabula_magnitudo;

    /* Initium */
    per (i = 0; i < XVI; i++)
    {
        bl_count[i] = 0;
        next_code[i] = 0;
    }

    /* Numerare codices per longitudinem */
    maxima_bits = 0;
    per (i = 0; i < numerus; i++)
    {
        si (longitudines[i] > 0)
        {
            bl_count[(i32)longitudines[i]]++;
            si ((i32)longitudines[i] > maxima_bits)
            {
                maxima_bits = (i32)longitudines[i];
            }
        }
    }

    si (maxima_bits == 0)
    {
        redde FALSUM;
    }

    /* Calculare initium codicum */
    codex = 0;
    per (bits = 1; bits <= maxima_bits; bits++)
    {
        codex = (codex + bl_count[bits - I]) << I;
        next_code[bits] = codex;
    }

    /* Assignare codices */
    codices = (s32*)piscina_allocare(piscina, (memoriae_index)((i32)numerus * (s32)magnitudo(s32)));
    per (i = 0; i < numerus; i++)
    {
        si (longitudines[i] > 0)
        {
            codices[i] = (s32)next_code[(i32)longitudines[i]];
            next_code[(i32)longitudines[i]]++;
        }
        alioquin
        {
            codices[i] = -1;
        }
    }

    /* Construere tabula lookup */
    tabula_magnitudo = I << maxima_bits;
    tabula->symbola = (s16*)piscina_allocare(piscina, (memoriae_index)((i32)tabula_magnitudo * (s32)magnitudo(s16)));
    tabula->longitudines = (i8*)piscina_allocare(piscina, (memoriae_index)tabula_magnitudo);
    tabula->numerus = numerus;
    tabula->maxima_bits = maxima_bits;

    /* Initium cum valoribus invalidis */
    per (i = 0; i < tabula_magnitudo; i++)
    {
        tabula->symbola[i] = -1;
        tabula->longitudines[i] = 0;
    }

    /* Implere tabula */
    per (i = 0; i < numerus; i++)
    {
        si (longitudines[i] > 0)
        {
            i32 codex_rev;
            i32 bits_codex;
            i32 j;
            i32 incrementum;

            bits_codex = (i32)longitudines[i];

            /* Invertere bits (DEFLATE uses reversed bit order) */
            codex_rev = 0;
            per (j = 0; j < bits_codex; j++)
            {
                codex_rev = (codex_rev << I) | ((codices[i] >> j) & I);
            }

            /* Implere omnes entries cum hoc prefixo */
            incrementum = I << bits_codex;
            per (j = codex_rev; j < tabula_magnitudo; j += incrementum)
            {
                tabula->symbola[j] = (s16)i;
                tabula->longitudines[j] = (i8)bits_codex;
            }
        }
    }

    redde VERUM;
}


interior s32
_flatura_huffman_legere(
    FlaturaLector*  lector,
    HuffmanTabula*  tabula)
{
    s32 bits;
    s32 symbolum;
    i32 longitudo;
    i32 bits_non_usati;
    i32 bits_reponere;
    i32 bits_disponibiles;

    /* Calculare bits disponibiles */
    bits_disponibiles = lector->bits_in_buffer +
                        (lector->mensura - lector->positus) * VIII;

    si (bits_disponibiles <= 0)
    {
#if FLATURA_DEBUG
        printf("  [HUFFMAN] No bits available\n");
#endif
        redde -1;
    }

    /* Legere usque ad maxima bits (vel minus si in fine) */
    si (bits_disponibiles >= tabula->maxima_bits)
    {
        bits = _flatura_legere_bits(lector, tabula->maxima_bits);
    }
    alioquin
    {
        /* In fine - legere quod habemus et pad cum zeros */
        bits = _flatura_legere_bits(lector, bits_disponibiles);
        si (bits < 0)
        {
#if FLATURA_DEBUG
            printf("  [HUFFMAN] legere_bits failed at end\n");
#endif
            redde -1;
        }
        /* Bits iam in positu correcto (low bits), high bits sunt zero */
    }

    si (bits < 0)
    {
#if FLATURA_DEBUG
        printf("  [HUFFMAN] legere_bits failed: pos=%d, mensura=%d, bits_in_buffer=%d\n",
               lector->positus, lector->mensura, lector->bits_in_buffer);
#endif
        redde -1;
    }

    symbolum = tabula->symbola[bits];
    longitudo = (i32)tabula->longitudines[bits];

    si (symbolum < 0)
    {
#if FLATURA_DEBUG
        printf("  [HUFFMAN] Invalid symbol: bits=0x%X, lookup=%d\n", bits, symbolum);
#endif
        redde -1;
    }

    /* Verificare habuimus satis bits */
    si (bits_disponibiles < tabula->maxima_bits && longitudo > bits_disponibiles)
    {
#if FLATURA_DEBUG
        printf("  [HUFFMAN] Not enough bits for symbol: needed=%d, had=%d\n",
               longitudo, bits_disponibiles);
#endif
        redde -1;
    }

    /* Reponere bits non usatos - debent esse in FUNDO bit_buffer */
    si (bits_disponibiles >= tabula->maxima_bits)
    {
        bits_non_usati = tabula->maxima_bits - longitudo;
    }
    alioquin
    {
        bits_non_usati = bits_disponibiles - longitudo;
    }

    si (bits_non_usati > 0)
    {
        bits_reponere = ((i32)bits >> longitudo) & ((I << bits_non_usati) - I);
        lector->bit_buffer = (lector->bit_buffer << bits_non_usati) | bits_reponere;
        lector->bits_in_buffer += bits_non_usati;
    }

    redde symbolum;
}


/* ========================================================================
 * FIXED HUFFMAN TABULAE
 * ======================================================================== */

interior b32
_flatura_construere_tabulam_fixam_literarum(
    HuffmanTabula* tabula,
    Piscina*       piscina)
{
    i32 tabula_magnitudo;
    i32 i;
    i32 codex;
    i32 codex_rev;
    i32 j;
    i32 bits;
    i32 incrementum;

    /* Fixed Huffman: 9 bits maxima */
    tabula->maxima_bits = IX;
    tabula_magnitudo = I << IX;

    tabula->symbola = (s16*)piscina_allocare(piscina, (memoriae_index)((i32)tabula_magnitudo * (s32)magnitudo(s16)));
    tabula->longitudines = (i8*)piscina_allocare(piscina, (memoriae_index)tabula_magnitudo);
    tabula->numerus = 288;

    /* Initium */
    per (i = 0; i < tabula_magnitudo; i++)
    {
        tabula->symbola[i] = -1;
        tabula->longitudines[i] = 0;
    }

    /* Per RFC 1951 - exact codes, not canonical:
     *   0-143:   8 bits, codes 00110000-10111111 (0x30-0xBF)
     *   144-255: 9 bits, codes 110010000-111111111 (0x190-0x1FF)
     *   256-279: 7 bits, codes 0000000-0010111 (0x00-0x17)
     *   280-287: 8 bits, codes 11000000-11000111 (0xC0-0xC7)
     */

    /* 0-143: 8 bit codes starting at 0x30 */
    per (i = 0; i <= CXLIII; i++)
    {
        codex = 0x30 + i;
        bits = VIII;
        codex_rev = 0;
        per (j = 0; j < bits; j++)
        {
            codex_rev = (codex_rev << I) | ((codex >> j) & I);
        }
        incrementum = I << bits;
        per (j = codex_rev; j < tabula_magnitudo; j += incrementum)
        {
            tabula->symbola[j] = (s16)i;
            tabula->longitudines[j] = (i8)bits;
        }
    }

    /* 144-255: 9 bit codes starting at 0x190 */
    per (i = CXLIV; i <= CCLV; i++)
    {
        codex = 0x190 + (i - CXLIV);
        bits = IX;
        codex_rev = 0;
        per (j = 0; j < bits; j++)
        {
            codex_rev = (codex_rev << I) | ((codex >> j) & I);
        }
        /* For 9-bit codes, no incrementum needed */
        tabula->symbola[codex_rev] = (s16)i;
        tabula->longitudines[codex_rev] = (i8)bits;
    }

    /* 256-279: 7 bit codes starting at 0x00 */
    per (i = FLATURA_END_BLOCK; i <= 279; i++)
    {
        codex = i - FLATURA_END_BLOCK;
        bits = VII;
        codex_rev = 0;
        per (j = 0; j < bits; j++)
        {
            codex_rev = (codex_rev << I) | ((codex >> j) & I);
        }
        incrementum = I << bits;
        per (j = codex_rev; j < tabula_magnitudo; j += incrementum)
        {
            tabula->symbola[j] = (s16)i;
            tabula->longitudines[j] = (i8)bits;
        }
    }

    /* 280-287: 8 bit codes starting at 0xC0 */
    per (i = CCLXXX; i <= 287; i++)
    {
        codex = 0xC0 + (i - CCLXXX);
        bits = VIII;
        codex_rev = 0;
        per (j = 0; j < bits; j++)
        {
            codex_rev = (codex_rev << I) | ((codex >> j) & I);
        }
        incrementum = I << bits;
        per (j = codex_rev; j < tabula_magnitudo; j += incrementum)
        {
            tabula->symbola[j] = (s16)i;
            tabula->longitudines[j] = (i8)bits;
        }
    }

    redde VERUM;
}


interior b32
_flatura_construere_tabulam_fixam_distantiarum(
    HuffmanTabula* tabula,
    Piscina*       piscina)
{
    i8 longitudines[XXXII];  /* 32 symbola */
    i32 i;

    /* Per RFC 1951: omnes 5 bits */
    per (i = 0; i < XXXII; i++)
    {
        longitudines[i] = V;
    }

    redde _flatura_huffman_construere(tabula, longitudines, XXXII, piscina);
}


/* ========================================================================
 * FENESTRA MOBILIS (Sliding Window)
 * ======================================================================== */

nomen structura {
    i8* datum;
    i32 amplitudo;
    i32 positus;
} FlaturaFenestra;


interior vacuum
_flatura_fenestra_initium(
    FlaturaFenestra* fenestra,
    Piscina*         piscina)
{
    fenestra->datum = (i8*)piscina_allocare(piscina, FLATURA_FENESTRA_MAGNITUDO);
    fenestra->amplitudo = FLATURA_FENESTRA_MAGNITUDO;
    fenestra->positus = 0;
}


interior vacuum
_flatura_fenestra_scribere(
    FlaturaFenestra* fenestra,
    i8               valor)
{
    fenestra->datum[fenestra->positus & (fenestra->amplitudo - I)] = valor;
    fenestra->positus++;
}


interior i8
_flatura_fenestra_legere(
    FlaturaFenestra* fenestra,
    i32              distantia)
{
    i32 index;
    index = (fenestra->positus - distantia) & (fenestra->amplitudo - I);
    redde fenestra->datum[index];
}


/* ========================================================================
 * OUTPUT BUFFER
 * ======================================================================== */

nomen structura {
    i8*      datum;
    i32      capacitas;
    i32      positus;
    Piscina* piscina;
} FlaturaOutput;


interior vacuum
_flatura_output_initium(
    FlaturaOutput* output,
    Piscina*       piscina,
    i32            capacitas_initialis)
{
    output->piscina = piscina;
    output->capacitas = capacitas_initialis;
    output->datum = (i8*)piscina_allocare(piscina, (memoriae_index)capacitas_initialis);
    output->positus = 0;
}


interior vacuum
_flatura_output_crescere(FlaturaOutput* output)
{
    i32  nova_capacitas;
    i8*  novum_datum;

    nova_capacitas = output->capacitas * II;
    novum_datum = (i8*)piscina_allocare(output->piscina, (memoriae_index)nova_capacitas);
    memcpy(novum_datum, output->datum, (size_t)output->positus);
    output->datum = novum_datum;
    output->capacitas = nova_capacitas;
}


interior vacuum
_flatura_output_scribere(
    FlaturaOutput*   output,
    FlaturaFenestra* fenestra,
    i8               valor)
{
    si (output->positus >= output->capacitas)
    {
        _flatura_output_crescere(output);
    }
    output->datum[output->positus] = valor;
    output->positus++;

    _flatura_fenestra_scribere(fenestra, valor);
}


interior vacuum
_flatura_output_copiare(
    FlaturaOutput*   output,
    FlaturaFenestra* fenestra,
    i32              distantia,
    i32              longitudo)
{
    i32 i;
    i8  valor;

    per (i = 0; i < longitudo; i++)
    {
        valor = _flatura_fenestra_legere(fenestra, distantia);
        _flatura_output_scribere(output, fenestra, valor);
    }
}


/* ========================================================================
 * INFLATIO (Decompressio)
 * ======================================================================== */

interior FlaturaFructus
_flatura_inflare_internus(
    FlaturaLector*  lector,
    Piscina*        piscina)
{
    FlaturaFructus   fructus;
    FlaturaFenestra  fenestra;
    FlaturaOutput    output;
    HuffmanTabula    tabula_literarum;
    HuffmanTabula    tabula_distantiarum;
    b32              finalis;
    s32              modus;
#if FLATURA_DEBUG
    i32              symbola_count;
    symbola_count = 0;
#endif
    _flatura_fenestra_initium(&fenestra, piscina);
    _flatura_output_initium(&output, piscina, MMMMXCVI);

    finalis = FALSUM;

    dum (!finalis)
    {
        /* Legere block header */
        finalis = _flatura_legere_bits(lector, I) ? VERUM : FALSUM;
        modus = _flatura_legere_bits(lector, II);

        si (modus < 0)
        {
            fructus.status = FLATURA_STATUS_FRACTA_DATUM;
            fructus.datum = NIHIL;
            fructus.mensura = 0;
            redde fructus;
        }

        si (modus == 0)
        {
            /* Stored block (sine compressione) */
            s32 len;
            s32 nlen;
            i32 i;

            _flatura_saltare_ad_byte(lector);

            len = _flatura_legere_bits(lector, XVI);
            nlen = _flatura_legere_bits(lector, XVI);

            si (len < 0 || nlen < 0 || (len ^ 0xFFFF) != nlen)
            {
                fructus.status = FLATURA_STATUS_FRACTA_DATUM;
                fructus.datum = NIHIL;
                fructus.mensura = 0;
                redde fructus;
            }

            per (i = 0; (s32)i < len; i++)
            {
                s32 b;
                b = _flatura_legere_bits(lector, VIII);
                si (b < 0)
                {
                    fructus.status = FLATURA_STATUS_FRACTA_DATUM;
                    fructus.datum = NIHIL;
                    fructus.mensura = 0;
                    redde fructus;
                }
                _flatura_output_scribere(&output, &fenestra, (i8)b);
            }
        }
        alioquin si (modus == I || modus == II)
        {
            /* Huffman compressed block */

            si (modus == I)
            {
                /* Fixed Huffman */
                si (!_flatura_construere_tabulam_fixam_literarum(&tabula_literarum, piscina) ||
                    !_flatura_construere_tabulam_fixam_distantiarum(&tabula_distantiarum, piscina))
                {
                    fructus.status = FLATURA_STATUS_FRACTA_HUFFMAN;
                    fructus.datum = NIHIL;
                    fructus.mensura = 0;
                    redde fructus;
                }
            }
            alioquin
            {
                /* Dynamic Huffman */
                s32 hlit;
                s32 hdist;
                s32 hclen;
                i8  cl_lengths[XIX];
                i8  all_lengths[288 + XXXII];
                HuffmanTabula tabula_cl;
                i32 i;
                s32 total;
                s32 index;

                hlit = _flatura_legere_bits(lector, V) + FLATURA_FIRST_LEN;
                hdist = _flatura_legere_bits(lector, V) + 1;
                hclen = _flatura_legere_bits(lector, IV) + 4;

                si (hlit < 0 || hdist < 0 || hclen < 0)
                {
                    fructus.status = FLATURA_STATUS_FRACTA_DATUM;
                    fructus.datum = NIHIL;
                    fructus.mensura = 0;
                    redde fructus;
                }

                /* Legere code length code lengths */
                per (i = 0; i < XIX; i++)
                {
                    cl_lengths[i] = 0;
                }
                per (i = 0; (s32)i < hclen; i++)
                {
                    s32 len;
                    len = _flatura_legere_bits(lector, III);
                    si (len < 0)
                    {
                        fructus.status = FLATURA_STATUS_FRACTA_DATUM;
                        fructus.datum = NIHIL;
                        fructus.mensura = 0;
                        redde fructus;
                    }
                    cl_lengths[(i32)codex_longitudo_ordo[i]] = (i8)len;
                }

                /* Construere code length tabula */
                si (!_flatura_huffman_construere(&tabula_cl, cl_lengths, XIX, piscina))
                {
                    fructus.status = FLATURA_STATUS_FRACTA_HUFFMAN;
                    fructus.datum = NIHIL;
                    fructus.mensura = 0;
                    redde fructus;
                }

                /* Legere literal/length et distance lengths */
                total = hlit + hdist;
                index = 0;

                dum (index < total)
                {
                    s32 sym;
                    sym = _flatura_huffman_legere(lector, &tabula_cl);

                    si (sym < 0)
                    {
                        fructus.status = FLATURA_STATUS_FRACTA_DATUM;
                        fructus.datum = NIHIL;
                        fructus.mensura = 0;
                        redde fructus;
                    }

                    si (sym < XVI)
                    {
                        /* Valor literal */
                        all_lengths[index] = (i8)sym;
                        index++;
                    }
                    alioquin si (sym == XVI)
                    {
                        /* Repetere praecedentum 3-6 times */
                        s32 count;
                        i8  prev;

                        si (index == 0)
                        {
                            fructus.status = FLATURA_STATUS_FRACTA_DATUM;
                            fructus.datum = NIHIL;
                            fructus.mensura = 0;
                            redde fructus;
                        }

                        count = _flatura_legere_bits(lector, II) + 3;
                        prev = all_lengths[index - I];

                        per (i = 0; (s32)i < count && index < total; i++)
                        {
                            all_lengths[index] = prev;
                            index++;
                        }
                    }
                    alioquin si (sym == XVII)
                    {
                        /* Repetere 0 pro 3-10 times */
                        s32 count;
                        count = _flatura_legere_bits(lector, III) + 3;

                        per (i = 0; (s32)i < count && index < total; i++)
                        {
                            all_lengths[index] = 0;
                            index++;
                        }
                    }
                    alioquin /* sym == 18 */
                    {
                        /* Repetere 0 pro 11-138 times */
                        s32 count;
                        count = _flatura_legere_bits(lector, VII) + 11;

                        per (i = 0; (s32)i < count && index < total; i++)
                        {
                            all_lengths[index] = 0;
                            index++;
                        }
                    }
                }

                /* Construere tabulae */
                si (!_flatura_huffman_construere(&tabula_literarum, all_lengths, (i32)hlit, piscina))
                {
                    fructus.status = FLATURA_STATUS_FRACTA_HUFFMAN;
                    fructus.datum = NIHIL;
                    fructus.mensura = 0;
                    redde fructus;
                }

                si (!_flatura_huffman_construere(&tabula_distantiarum, all_lengths + hlit, (i32)hdist, piscina))
                {
                    fructus.status = FLATURA_STATUS_FRACTA_HUFFMAN;
                    fructus.datum = NIHIL;
                    fructus.mensura = 0;
                    redde fructus;
                }
            }

            /* Decodificare symbola */
            per (;;)
            {
                s32 sym;

                sym = _flatura_huffman_legere(lector, &tabula_literarum);
                si (sym < 0)
                {
#if FLATURA_DEBUG
                    printf("  [DEBUG] sym < 0 at output=%d, count=%d\n", output.positus, symbola_count);
#endif
                    fructus.status = FLATURA_STATUS_FRACTA_DATUM;
                    fructus.datum = NIHIL;
                    fructus.mensura = 0;
                    redde fructus;
                }

#if FLATURA_DEBUG
                symbola_count++;
#endif

                si (sym < CCLVI)
                {
                    /* Literal byte */
                    _flatura_output_scribere(&output, &fenestra, (i8)sym);
#if FLATURA_DEBUG
                    si (symbola_count <= X)
                    {
                        printf("  [DEBUG] sym %d: literal '%c'\n", symbola_count, sym);
                    }
#endif
                }
                alioquin si (sym == FLATURA_END_BLOCK)
                {
                    /* End of block */
#if FLATURA_DEBUG
                    printf("  [DEBUG] End of block at output=%d, count=%d\n", output.positus, symbola_count);
#endif
                    frange;
                }
                alioquin
                {
                    /* Length/distance pair */
                    s32 len_index;
                    i32 longitudo;
                    s32 extra;
                    s32 dist_sym;
                    i32 distantia;

                    len_index = sym - FLATURA_FIRST_LEN;

                    si (len_index >= XXIX)
                    {
#if FLATURA_DEBUG
                        printf("  [DEBUG] len_index >= 29: sym=%d, len_index=%d\n", sym, len_index);
#endif
                        fructus.status = FLATURA_STATUS_FRACTA_DATUM;
                        fructus.datum = NIHIL;
                        fructus.mensura = 0;
                        redde fructus;
                    }

                    longitudo = longitudo_basis[len_index];
                    si (longitudo_extra_bits[len_index] > 0)
                    {
                        extra = _flatura_legere_bits(lector, (i32)longitudo_extra_bits[len_index]);
                        si (extra < 0)
                        {
#if FLATURA_DEBUG
                            printf("  [DEBUG] Failed to read length extra bits\n");
#endif
                            fructus.status = FLATURA_STATUS_FRACTA_DATUM;
                            fructus.datum = NIHIL;
                            fructus.mensura = 0;
                            redde fructus;
                        }
                        longitudo += (i32)extra;
                    }

                    dist_sym = _flatura_huffman_legere(lector, &tabula_distantiarum);
                    si (dist_sym < 0 || dist_sym >= XXX)
                    {
#if FLATURA_DEBUG
                        printf("  [DEBUG] Invalid dist_sym=%d at output=%d\n", dist_sym, output.positus);
#endif
                        fructus.status = FLATURA_STATUS_FRACTA_DISTANTIA;
                        fructus.datum = NIHIL;
                        fructus.mensura = 0;
                        redde fructus;
                    }

                    distantia = distantia_basis[dist_sym];
                    si (distantia_extra_bits[dist_sym] > 0)
                    {
                        extra = _flatura_legere_bits(lector, (i32)distantia_extra_bits[dist_sym]);
                        si (extra < 0)
                        {
#if FLATURA_DEBUG
                            printf("  [DEBUG] Failed to read distance extra bits\n");
#endif
                            fructus.status = FLATURA_STATUS_FRACTA_DATUM;
                            fructus.datum = NIHIL;
                            fructus.mensura = 0;
                            redde fructus;
                        }
                        distantia += (i32)extra;
                    }

#if FLATURA_DEBUG
                    si (symbola_count <= X)
                    {
                        printf("  [DEBUG] sym %d: len=%d, dist=%d\n", symbola_count, longitudo, distantia);
                    }
#endif
                    _flatura_output_copiare(&output, &fenestra, distantia, longitudo);
                }
            }
        }
        alioquin
        {
            /* modus == 3: reservatus, error */
            fructus.status = FLATURA_STATUS_FRACTA_DATUM;
            fructus.datum = NIHIL;
            fructus.mensura = 0;
            redde fructus;
        }
    }

    fructus.status = FLATURA_STATUS_OK;
    fructus.datum = output.datum;
    fructus.mensura = output.positus;

    redde fructus;
}


FlaturaFructus
flatura_inflare(
    constans i8* datum,
    i32          mensura,
    Piscina*     piscina)
{
    FlaturaLector lector;

    si (datum == NIHIL || piscina == NIHIL)
    {
        FlaturaFructus fructus;
        fructus.status = FLATURA_STATUS_FRACTA_DATUM;
        fructus.datum = NIHIL;
        fructus.mensura = 0;
        redde fructus;
    }

    _flatura_lector_initium(&lector, datum, mensura);

    redde _flatura_inflare_internus(&lector, piscina);
}


/* ========================================================================
 * GZIP INFLATIO
 * ======================================================================== */

FlaturaFructus
flatura_gzip_inflare(
    constans i8* datum,
    i32          mensura,
    Piscina*     piscina)
{
    FlaturaFructus fructus;
    FlaturaLector  lector;
    i32            positus;
    i32            flags;
    i32            crc_calculatus;
    i32            crc_expectatus;
    i32            isize_expectatus;

    si (datum == NIHIL || piscina == NIHIL || mensura < XVIII)
    {
        fructus.status = FLATURA_STATUS_GZIP_FRACTA;
        fructus.datum = NIHIL;
        fructus.mensura = 0;
        redde fructus;
    }

    /* Verificare magic number */
    si (datum[0] != (i8)0x1F || datum[I] != (i8)0x8B)
    {
        fructus.status = FLATURA_STATUS_GZIP_FRACTA;
        fructus.datum = NIHIL;
        fructus.mensura = 0;
        redde fructus;
    }

    /* Verificare compression method (8 = deflate) */
    si (datum[II] != VIII)
    {
        fructus.status = FLATURA_STATUS_GZIP_FRACTA;
        fructus.datum = NIHIL;
        fructus.mensura = 0;
        redde fructus;
    }

    flags = (i32)datum[III];
    positus = X;  /* Post header fixum */

    /* Saltare FEXTRA si praesens */
    si (flags & IV)
    {
        i32 xlen;
        si (positus + II > mensura)
        {
            fructus.status = FLATURA_STATUS_GZIP_FRACTA;
            fructus.datum = NIHIL;
            fructus.mensura = 0;
            redde fructus;
        }
        xlen = (i32)datum[positus] | ((i32)datum[positus + I] << VIII);
        positus += II + xlen;
    }

    /* Saltare FNAME si praesens */
    si (flags & VIII)
    {
        dum (positus < mensura && datum[positus] != 0)
        {
            positus++;
        }
        positus++;  /* Saltare null terminator */
    }

    /* Saltare FCOMMENT si praesens */
    si (flags & XVI)
    {
        dum (positus < mensura && datum[positus] != 0)
        {
            positus++;
        }
        positus++;  /* Saltare null terminator */
    }

    /* Saltare FHCRC si praesens */
    si (flags & II)
    {
        positus += II;
    }

    si (positus >= mensura - VIII)
    {
        fructus.status = FLATURA_STATUS_GZIP_FRACTA;
        fructus.datum = NIHIL;
        fructus.mensura = 0;
        redde fructus;
    }

    /* Inflare DEFLATE datum */
    _flatura_lector_initium(&lector, datum + positus, mensura - positus - VIII);
    fructus = _flatura_inflare_internus(&lector, piscina);

    si (fructus.status != FLATURA_STATUS_OK)
    {
        redde fructus;
    }

    /* Legere et verificare CRC-32 et ISIZE ex trailer */
    {
        constans i8* trailer;
        trailer = datum + mensura - VIII;

        crc_expectatus = (i32)((i32)trailer[0] |
                               ((i32)trailer[I] << VIII) |
                               ((i32)trailer[II] << XVI) |
                               ((i32)trailer[III] << XXIV));

        isize_expectatus = (i32)((i32)trailer[IV] |
                                 ((i32)trailer[V] << VIII) |
                                 ((i32)trailer[VI] << XVI) |
                                 ((i32)trailer[VII] << XXIV));

        crc_calculatus = flatura_crc32(fructus.datum, fructus.mensura);

        si (crc_calculatus != crc_expectatus)
        {
            fructus.status = FLATURA_STATUS_GZIP_FRACTA;
            fructus.datum = NIHIL;
            fructus.mensura = 0;
            redde fructus;
        }

        si (fructus.mensura != isize_expectatus)
        {
            fructus.status = FLATURA_STATUS_GZIP_FRACTA;
            fructus.datum = NIHIL;
            fructus.mensura = 0;
            redde fructus;
        }
    }

    redde fructus;
}


/* ========================================================================
 * COMPRESSIO (DEFLATE)
 * ======================================================================== */

/* Hash tabula pro LZ77 matching */
#define HASH_MAGNITUDO  32768
#define HASH_MASK       0x7FFF
#define MIN_MATCH       III
#define MAX_MATCH       258

nomen structura {
    s16* capites;     /* Caput catenarum hash (-1 = nullus) */
    s16* praevia;     /* Praevius in catena */
    i32  fenestra_positus;
} HashTabula;


interior vacuum
_flatura_hash_initium(
    HashTabula* hash,
    Piscina*    piscina)
{
    i32 i;

    hash->capites = (s16*)piscina_allocare(piscina, HASH_MAGNITUDO * (memoriae_index)magnitudo(s16));
    hash->praevia = (s16*)piscina_allocare(piscina, FLATURA_FENESTRA_MAGNITUDO * (memoriae_index)magnitudo(s16));
    hash->fenestra_positus = 0;

    per (i = 0; i < HASH_MAGNITUDO; i++)
    {
        hash->capites[i] = -1;
    }
}


interior i32
_flatura_hash_calculare(
    constans i8* datum,
    i32          positus)
{
    i32 h;
    h = ((i32)datum[positus] << X) ^
        ((i32)datum[positus + I] << V) ^
        ((i32)datum[positus + II]);
    redde h & HASH_MASK;
}


interior vacuum
_flatura_hash_inserere(
    HashTabula*  hash,
    constans i8* datum,
    i32          positus)
{
    i32 h;
    i32 fenestra_index;

    h = _flatura_hash_calculare(datum, positus);
    fenestra_index = positus & (FLATURA_FENESTRA_MAGNITUDO - I);

    hash->praevia[fenestra_index] = hash->capites[h];
    hash->capites[h] = (s16)fenestra_index;
}


interior i32
_flatura_quaerere_concordantiam(
    HashTabula*  hash,
    constans i8* datum,
    i32          positus,
    i32          mensura,
    i32*         distantia_output)
{
    i32 h;
    i32 optima_longitudo;
    i32 optima_distantia;
    s16 currens;
    i32 catena_longitudo;

    si (positus + MIN_MATCH > mensura)
    {
        redde 0;
    }

    h = _flatura_hash_calculare(datum, positus);
    currens = hash->capites[h];
    optima_longitudo = 0;
    optima_distantia = 0;
    catena_longitudo = 0;

    dum (currens >= 0 && catena_longitudo < CXXVIII)
    {
        i32 candidatus_positus;
        i32 distantia;
        i32 longitudo;
        i32 max_longitudo;

        candidatus_positus = (positus & (i32)~(i32)(FLATURA_FENESTRA_MAGNITUDO - I)) | (i32)currens;

        /* Verificare si in fenestra */
        si (candidatus_positus >= positus)
        {
            candidatus_positus -= FLATURA_FENESTRA_MAGNITUDO;
        }

        distantia = positus - candidatus_positus;

        si (distantia <= 0 || distantia > FLATURA_FENESTRA_MAGNITUDO)
        {
            frange;
        }

        /* Comparare strings */
        max_longitudo = mensura - positus;
        si (max_longitudo > MAX_MATCH)
        {
            max_longitudo = MAX_MATCH;
        }

        longitudo = 0;
        dum (longitudo < max_longitudo &&
             datum[candidatus_positus + longitudo] == datum[positus + longitudo])
        {
            longitudo++;
        }

        si (longitudo > optima_longitudo)
        {
            optima_longitudo = longitudo;
            optima_distantia = distantia;

            si (longitudo >= MAX_MATCH)
            {
                frange;
            }
        }

        currens = hash->praevia[currens];
        catena_longitudo++;
    }

    si (optima_longitudo >= MIN_MATCH)
    {
        *distantia_output = optima_distantia;
        redde optima_longitudo;
    }

    redde 0;
}


/* Invenire symbolum longitudinis */
interior i32
_flatura_longitudo_ad_symbolum(i32 longitudo)
{
    i32 i;

    per (i = 0; i < XXVIII; i++)
    {
        si (longitudo <= longitudo_basis[i + I] - I)
        {
            redde FLATURA_FIRST_LEN + i;
        }
    }

    redde 285;  /* 258 */
}


/* Invenire symbolum distantiae */
interior i32
_flatura_distantia_ad_symbolum(i32 distantia)
{
    i32 i;

    per (i = 0; i < XXIX; i++)
    {
        si (distantia <= distantia_basis[i + I] - I)
        {
            redde i;
        }
    }

    redde XXIX;
}


/* Scribere fixed Huffman codex pro literal/length */
interior vacuum
_flatura_scribere_literalem_fixam(
    FlaturaScriptor* scriptor,
    i32              symbolum)
{
    i32 codex;
    i32 bits;
    i32 rev_codex;
    i32 i;

    si (symbolum <= CXLIII)
    {
        codex = 0x30 + symbolum;
        bits = VIII;
    }
    alioquin si (symbolum <= CCLV)
    {
        codex = 0x190 + (symbolum - CXLIV);
        bits = IX;
    }
    alioquin si (symbolum <= 279)
    {
        codex = symbolum - FLATURA_END_BLOCK;
        bits = VII;
    }
    alioquin
    {
        codex = 0xC0 + (symbolum - CCLXXX);
        bits = VIII;
    }

    /* Invertere bits */
    rev_codex = 0;
    per (i = 0; i < bits; i++)
    {
        rev_codex = (rev_codex << I) | ((codex >> i) & I);
    }

    _flatura_scribere_bits(scriptor, rev_codex, bits);
}


/* Scribere fixed Huffman codex pro distantia */
interior vacuum
_flatura_scribere_distantiam_fixam(
    FlaturaScriptor* scriptor,
    i32              symbolum)
{
    i32 rev_codex;
    i32 i;

    /* 5 bits, reversed */
    rev_codex = 0;
    per (i = 0; i < V; i++)
    {
        rev_codex = (rev_codex << I) | ((symbolum >> i) & I);
    }

    _flatura_scribere_bits(scriptor, rev_codex, V);
}


FlaturaFructus
flatura_deflare(
    constans i8*              datum,
    i32                       mensura,
    FlaturaCompressioNivellus nivellus,
    Piscina*                  piscina)
{
    FlaturaFructus   fructus;
    FlaturaScriptor  scriptor;
    HashTabula       hash;
    i32              positus;

    si (datum == NIHIL || piscina == NIHIL)
    {
        fructus.status = FLATURA_STATUS_FRACTA_DATUM;
        fructus.datum = NIHIL;
        fructus.mensura = 0;
        redde fructus;
    }

    _flatura_scriptor_initium(&scriptor, piscina, mensura + CCLVI);

    si (mensura == 0)
    {
        /* Datum vacuus - unus stored block vacuus */
        /* Block header: BFINAL=1, BTYPE=00 */
        _flatura_scribere_bits(&scriptor, I, I);
        _flatura_scribere_bits(&scriptor, 0, II);

        /* Align to byte */
        _flatura_scriptor_finire(&scriptor);

        /* LEN = 0 */
        _flatura_scribere_bits(&scriptor, 0, VIII);
        _flatura_scribere_bits(&scriptor, 0, VIII);

        /* NLEN = 0xFFFF */
        _flatura_scribere_bits(&scriptor, 0xFF, VIII);
        _flatura_scribere_bits(&scriptor, 0xFF, VIII);
    }
    alioquin si (nivellus == FLATURA_COMPRESSIO_NULLA)
    {
        /* Stored block (sine compressione) */
        i32 i;
        i32 block_positus;

        block_positus = 0;
        dum (block_positus < mensura)
        {
            i32 block_len;
            b32 finalis;

            block_len = mensura - block_positus;
            si (block_len > 65535)
            {
                block_len = 65535;
            }

            finalis = (block_positus + block_len >= mensura) ? VERUM : FALSUM;

            /* Block header: BFINAL=1/0, BTYPE=00 */
            _flatura_scribere_bits(&scriptor, finalis ? I : 0, I);
            _flatura_scribere_bits(&scriptor, 0, II);

            /* Align to byte */
            _flatura_scriptor_finire(&scriptor);

            /* LEN et NLEN */
            _flatura_scribere_bits(&scriptor, block_len & 0xFF, VIII);
            _flatura_scribere_bits(&scriptor, (block_len >> VIII) & 0xFF, VIII);
            _flatura_scribere_bits(&scriptor, (~block_len) & 0xFF, VIII);
            _flatura_scribere_bits(&scriptor, ((~block_len) >> VIII) & 0xFF, VIII);

            /* Datum */
            per (i = 0; i < block_len; i++)
            {
                _flatura_scribere_bits(&scriptor, (i32)datum[block_positus + i], VIII);
            }

            block_positus += block_len;
        }
    }
    alioquin
    {
        /* Fixed Huffman compression */

        /* Block header: BFINAL=1, BTYPE=01 */
        _flatura_scribere_bits(&scriptor, I, I);
        _flatura_scribere_bits(&scriptor, I, II);

        _flatura_hash_initium(&hash, piscina);

        positus = 0;
        dum (positus < mensura)
        {
            i32 longitudo;
            i32 distantia;

            longitudo = 0;
            si (positus + MIN_MATCH <= mensura)
            {
                longitudo = _flatura_quaerere_concordantiam(&hash, datum, positus, mensura, &distantia);
            }

            si (longitudo >= MIN_MATCH)
            {
                /* Emit length/distance pair */
                i32 len_sym;
                i32 dist_sym;
                i32 len_extra;
                i32 dist_extra;
                i32 len_index;
                i32 i;

                len_sym = _flatura_longitudo_ad_symbolum(longitudo);
                len_index = len_sym - FLATURA_FIRST_LEN;
                _flatura_scribere_literalem_fixam(&scriptor, len_sym);

                si (longitudo_extra_bits[len_index] > 0)
                {
                    len_extra = longitudo - longitudo_basis[len_index];
                    _flatura_scribere_bits(&scriptor, len_extra, (i32)longitudo_extra_bits[len_index]);
                }

                dist_sym = _flatura_distantia_ad_symbolum(distantia);
                _flatura_scribere_distantiam_fixam(&scriptor, dist_sym);

                si (distantia_extra_bits[dist_sym] > 0)
                {
                    dist_extra = distantia - distantia_basis[dist_sym];
                    _flatura_scribere_bits(&scriptor, dist_extra, (i32)distantia_extra_bits[dist_sym]);
                }

                /* Inserere in hash */
                per (i = 0; i < longitudo; i++)
                {
                    si (positus + i + MIN_MATCH <= mensura)
                    {
                        _flatura_hash_inserere(&hash, datum, positus + i);
                    }
                }

                positus += longitudo;
            }
            alioquin
            {
                /* Emit literal */
                _flatura_scribere_literalem_fixam(&scriptor, (i32)datum[positus]);

                si (positus + MIN_MATCH <= mensura)
                {
                    _flatura_hash_inserere(&hash, datum, positus);
                }

                positus++;
            }
        }

        /* End of block symbol (256) */
        _flatura_scribere_literalem_fixam(&scriptor, CCLVI);
    }

    _flatura_scriptor_finire(&scriptor);

    fructus.status = FLATURA_STATUS_OK;
    fructus.datum = scriptor.datum;
    fructus.mensura = scriptor.positus;

    redde fructus;
}


/* ========================================================================
 * GZIP DEFLATIO
 * ======================================================================== */

FlaturaFructus
flatura_gzip_deflare(
    constans i8*              datum,
    i32                       mensura,
    FlaturaCompressioNivellus nivellus,
    Piscina*                  piscina)
{
    FlaturaFructus   fructus;
    FlaturaFructus   deflate_fructus;
    i8*              output;
    i32              output_positus;
    i32              crc;

    si (datum == NIHIL || piscina == NIHIL)
    {
        fructus.status = FLATURA_STATUS_FRACTA_DATUM;
        fructus.datum = NIHIL;
        fructus.mensura = 0;
        redde fructus;
    }

    /* Deflare datum */
    deflate_fructus = flatura_deflare(datum, mensura, nivellus, piscina);

    si (deflate_fructus.status != FLATURA_STATUS_OK)
    {
        redde deflate_fructus;
    }

    /* Calculare CRC-32 */
    crc = flatura_crc32(datum, mensura);

    /* Allocare output: header(10) + deflate + trailer(8) */
    output = (i8*)piscina_allocare(piscina, (memoriae_index)(X + deflate_fructus.mensura + VIII));
    output_positus = 0;

    /* Gzip header */
    output[output_positus++] = (i8)0x1F;           /* Magic 1 */
    output[output_positus++] = (i8)0x8B;           /* Magic 2 */
    output[output_positus++] = VIII;               /* Compression method: deflate */
    output[output_positus++] = 0;                  /* Flags: nulla */
    output[output_positus++] = 0;                  /* MTIME[0] */
    output[output_positus++] = 0;                  /* MTIME[1] */
    output[output_positus++] = 0;                  /* MTIME[2] */
    output[output_positus++] = 0;                  /* MTIME[3] */
    output[output_positus++] = 0;                  /* XFL */
    output[output_positus++] = (i8)0xFF;           /* OS: unknown */

    /* Deflate datum */
    memcpy(output + output_positus, deflate_fructus.datum, (size_t)deflate_fructus.mensura);
    output_positus += deflate_fructus.mensura;

    /* Gzip trailer: CRC-32 (little-endian) */
    output[output_positus++] = (i8)(crc & 0xFF);
    output[output_positus++] = (i8)((crc >> VIII) & 0xFF);
    output[output_positus++] = (i8)((crc >> XVI) & 0xFF);
    output[output_positus++] = (i8)((crc >> XXIV) & 0xFF);

    /* Gzip trailer: ISIZE (little-endian) */
    output[output_positus++] = (i8)(mensura & 0xFF);
    output[output_positus++] = (i8)((mensura >> VIII) & 0xFF);
    output[output_positus++] = (i8)((mensura >> XVI) & 0xFF);
    output[output_positus++] = (i8)((mensura >> XXIV) & 0xFF);

    fructus.status = FLATURA_STATUS_OK;
    fructus.datum = output;
    fructus.mensura = output_positus;

    redde fructus;
}


/* ========================================================================
 * UTILITAS
 * ======================================================================== */

i32
flatura_maxima_magnitudo_compressa(i32 mensura_originalis)
{
    /* Worst case: stored blocks with overhead */
    /* Per 65535 bytes: 5 bytes header */
    i32 numerus_blocks;

    numerus_blocks = (mensura_originalis + 65534) / 65535;

    redde mensura_originalis + (numerus_blocks * V) + XVIII;  /* +18 pro gzip header/trailer */
}


constans character*
flatura_status_nuntium(FlaturaStatus status)
{
    commutatio (status)
    {
        casus FLATURA_STATUS_OK:
            redde "Successus";
        casus FLATURA_STATUS_FRACTA_DATUM:
            redde "Datum invalidum";
        casus FLATURA_STATUS_FRACTA_HUFFMAN:
            redde "Arbor Huffman invalida";
        casus FLATURA_STATUS_FRACTA_DISTANTIA:
            redde "Distantia invalida";
        casus FLATURA_STATUS_FRACTA_ALLOCATIO:
            redde "Allocatio fallita";
        casus FLATURA_STATUS_INCOMPLETUS:
            redde "Datum incompletus";
        casus FLATURA_STATUS_GZIP_FRACTA:
            redde "Gzip format invalidum";
        ordinarius:
            redde "Error ignotus";
    }
}
