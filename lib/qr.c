/* qr.c - QR Code Generatio Implementatio */

#include "qr.h"
#include <string.h>


/* ===========================================================
 * CONSTANTES
 * =========================================================== */

/* Magnitudo pro versione: 17 + versio * 4 */
#define QR_MAGNITUDO_V1  21
#define QR_MAGNITUDO_V2  25
#define QR_MAGNITUDO_V3  29
#define QR_MAGNITUDO_V4  33

/* Capacitas datorum (bytes) pro byte mode per versio/ECC
 * [versio-1][ecc] */
interior constans i32 _capacitas_octeti[10][4] = {
    /* V1:  */ { 17, 14, 11,  7},   /* L, M, Q, H */
    /* V2:  */ { 32, 26, 20, 14},
    /* V3:  */ { 53, 42, 32, 24},
    /* V4:  */ { 78, 62, 46, 34},
    /* V5:  */ {106, 84, 60, 44},
    /* V6:  */ {134,106, 74, 58},
    /* V7:  */ {154,122, 86, 64},
    /* V8:  */ {192,152,108, 84},
    /* V9:  */ {230,180,130, 98},
    /* V10: */ {271,213,151,119}
};

/* Numerus ECC codewords per versio/ECC [versio-1][ecc] */
interior constans i32 _ecc_codewords[10][4] = {
    /* V1:  */ {  7, 10, 13, 17},
    /* V2:  */ { 10, 16, 22, 28},
    /* V3:  */ { 15, 26, 36, 44},
    /* V4:  */ { 20, 36, 52, 64},
    /* V5:  */ { 26, 48, 72, 88},
    /* V6:  */ { 36, 64, 96,112},
    /* V7:  */ { 40, 72,108,130},
    /* V8:  */ { 48, 88,132,156},
    /* V9:  */ { 60,110,160,192},
    /* V10: */ { 72,130,192,224}
};

/* Numerus data codewords totalis per versio (ante ECC) */
interior constans i32 _data_codewords_totalis[10] = {
    /* V1-V10 */ 26, 44, 70, 100, 134, 172, 196, 242, 292, 346
};

/* Numerus BLOCCORUM Reed-Solomon per versio/ECC [versio-1][ecc]
 * (ISO/IEC 18004 tabula characteristicarum correctionis).
 *
 * SOLA haec tabula nova est: ceterae mensurae DERIVANTUR, ne
 * tabula quinque columnarum manu transcripta erret -
 *   ec_per_bloccum  = _ecc_codewords[v][e] / bloci
 *   data_per_bloccum = data_totalis / bloci, cum
 *   (data_totalis % bloci) bloccis ULTIMIS unum codeword additum
 * ferentibus (ordo spec: bloci breviores primi).
 *
 * Custos: probatio_qr casum habet qui pro OMNI versione/gradu
 * verificat ec_total % bloci == 0 et summam intertexti == totalem.
 * Numerus manu scriptus qui arithmeticam non solvit clamat. */
interior constans i32 _bloci[10][4] = {
    /* V1:  */ { 1, 1, 1, 1},   /* L, M, Q, H */
    /* V2:  */ { 1, 1, 1, 1},
    /* V3:  */ { 1, 1, 2, 2},
    /* V4:  */ { 1, 2, 2, 4},
    /* V5:  */ { 1, 2, 4, 4},
    /* V6:  */ { 2, 4, 4, 4},
    /* V7:  */ { 2, 4, 6, 5},
    /* V8:  */ { 2, 4, 6, 6},
    /* V9:  */ { 2, 5, 8, 8},
    /* V10: */ { 4, 5, 8, 8}
};

/* mensura bloci b (0-basati) ex datis totalibus et numero bloccorum */
interior i32
_bloci_mensura (i32 data_totalis, i32 bloci, i32 b)
{
    i32 curtus = data_totalis / bloci;   /* 'brevis' = macro latinae */
    i32 longi  = data_totalis % bloci;   /* ultimi unum plus ferunt */

    redde curtus + ((b >= bloci - longi) ? I : ZEPHYRUM);
}

/* Informatio versionis (XVIII bits: VI versionis + XII BCH) pro
 * V7-V10. Versiones I-VI hanc aream NON habent - id ipsum est cur
 * V1-V6 legebantur et V7+ non (mensuratum oraculo). Valores ex
 * ISO 18004 tabula D.1; BCH non computatur quia tabula pro
 * ambitu nostro (V<=X) quattuor ordines solos habet. */
interior constans i32 _versio_info[4] = {
    /* V7:  */ 0x07C94,
    /* V8:  */ 0x085BC,
    /* V9:  */ 0x09A99,
    /* V10: */ 0x0A4D3
};

/* GF(256) tabulae pro Reed-Solomon */
interior i32 _gf256_exp[256];
interior i32 _gf256_log[256];
interior b32 _gf256_initiatus = FALSUM;

/* Format info (15 bits) - pre-calculated pro ECC-M, mask 0-7 */
interior constans i32 _format_info[4][8] = {
    /* ECC-L */ {0x77C4, 0x72F3, 0x7DAA, 0x789D, 0x662F, 0x6318, 0x6C41, 0x6976},
    /* ECC-M */ {0x5412, 0x5125, 0x5E7C, 0x5B4B, 0x45F9, 0x40CE, 0x4F97, 0x4AA0},
    /* ECC-Q */ {0x355F, 0x3068, 0x3F31, 0x3A06, 0x24B4, 0x2183, 0x2EDA, 0x2BED},
    /* ECC-H */ {0x1689, 0x13BE, 0x1CE7, 0x19D0, 0x0762, 0x0255, 0x0D0C, 0x083B}
};

/* Alignment pattern positiones per versio (ISO/IEC 18004)
 * [versio-1][0] = numerus positionum, [versio-1][1..] = positiones
 * V1 habet nullos alignment patterns */
interior constans i32 _alignment_positiones[10][8] = {
    /* V1:  */ {0, 0, 0, 0, 0, 0, 0, 0},           /* nullus */
    /* V2:  */ {2, 6,18, 0, 0, 0, 0, 0},           /* 1 pattern ad (18,18) */
    /* V3:  */ {2, 6,22, 0, 0, 0, 0, 0},
    /* V4:  */ {2, 6,26, 0, 0, 0, 0, 0},
    /* V5:  */ {2, 6,30, 0, 0, 0, 0, 0},
    /* V6:  */ {2, 6,34, 0, 0, 0, 0, 0},
    /* V7:  */ {3, 6,22,38, 0, 0, 0, 0},           /* 6 patterns (minus 3 overlapping finders) */
    /* V8:  */ {3, 6,24,42, 0, 0, 0, 0},
    /* V9:  */ {3, 6,26,46, 0, 0, 0, 0},
    /* V10: */ {3, 6,28,50, 0, 0, 0, 0}
};


/* ===========================================================
 * GF(256) ARITHMETICA (Reed-Solomon)
 * =========================================================== */

interior vacuum
_gf256_initiare(vacuum)
{
    i32 i;
    i32 x;

    si (_gf256_initiatus) redde;

    /* Generare exp et log tabulae
     * Primitive polynomial: x^8 + x^4 + x^3 + x^2 + 1 (0x11D) */
    x = I;
    per (i = ZEPHYRUM; i < CCLV; i++)
    {
        _gf256_exp[i] = x;
        _gf256_log[x] = i;
        x <<= I;
        si (x >= CCLVI)
        {
            x ^= 0x11D;
        }
    }
    _gf256_exp[CCLV] = _gf256_exp[ZEPHYRUM];
    _gf256_log[ZEPHYRUM] = ZEPHYRUM; /* Conventio: log(0) = 0 */

    _gf256_initiatus = VERUM;
}

interior i32
_gf256_multiplicare(i32 a, i32 b)
{
    si (a == ZEPHYRUM || b == ZEPHYRUM) redde ZEPHYRUM;
    redde _gf256_exp[(_gf256_log[a] + _gf256_log[b]) % CCLV];
}


/* ===========================================================
 * REED-SOLOMON CODIFICATIO
 * =========================================================== */

/* Generare generator polynomial pro n ECC codewords
 * Coefficientes in gen[0..n] (gen[n] = 1, leading) */
interior vacuum
_rs_generare_generator(i32* gen, i32 n)
{
    i32 i, j;

    /* Initium: g(x) = 1 */
    per (i = ZEPHYRUM; i <= n; i++) gen[i] = ZEPHYRUM;
    gen[ZEPHYRUM] = I;

    /* Multiplicare per (x - alpha^i) pro i = 0..n-1 */
    per (i = ZEPHYRUM; i < n; i++)
    {
        /* Shift et add */
        per (j = n; j > ZEPHYRUM; j--)
        {
            gen[j] = gen[j - I] ^ _gf256_multiplicare(gen[j], _gf256_exp[i]);
        }
        gen[ZEPHYRUM] = _gf256_multiplicare(gen[ZEPHYRUM], _gf256_exp[i]);
    }
}

/* Calculare ECC codewords pro data
 * data[0..data_len-1] = data codewords
 * ecc[0..ecc_len-1] = output ECC codewords */
interior vacuum
_rs_codificare(constans i8* data, i32 data_len, i8* ecc, i32 ecc_len)
{
    i32* gen;
    i32  i, j;
    i32  coef;
    Piscina* temp;

    /* Allocare generator temporarium */
    temp = piscina_generare_dynamicum("rs_temp", 1024);
    gen = (i32*)piscina_allocare(temp, (memoriae_index)(ecc_len + I) * magnitudo(i32));

    _rs_generare_generator(gen, ecc_len);

    /* Initializare remainder ad zero */
    per (i = ZEPHYRUM; i < ecc_len; i++) ecc[i] = ZEPHYRUM;

    /* Polynomial division */
    per (i = ZEPHYRUM; i < data_len; i++)
    {
        coef = ((i32)(i8)data[i]) ^ ((i32)(i8)ecc[ZEPHYRUM]);
        coef &= 0xFF;

        /* Shift remainder */
        per (j = ZEPHYRUM; j < ecc_len - I; j++)
        {
            ecc[j] = ecc[j + I];
        }
        ecc[ecc_len - I] = ZEPHYRUM;

        /* Add scaled generator */
        per (j = ZEPHYRUM; j < ecc_len; j++)
        {
            ecc[j] = (i8)(((i32)(i8)ecc[j]) ^ _gf256_multiplicare(gen[ecc_len - I - j], coef));
        }
    }

    piscina_destruere(temp);
}


/* ===========================================================
 * MATRIX CONSTRUCTIO
 * =========================================================== */

/* Ponere singularem modulum si intra fines */
interior vacuum
_ponere_modulum(i8* moduli, s32 mag, s32 x, s32 y, s32 valor)
{
    si (x >= ZEPHYRUM && x < mag && y >= ZEPHYRUM && y < mag)
    {
        moduli[y * mag + x] = (i8)valor;
    }
}

/* Ponere finder pattern (7x7) ad positionem */
interior vacuum
_ponere_finder(i8* moduli, s32 mag, s32 cx, s32 cy)
{
    s32 dx, dy;

    per (dy = -III; dy <= III; dy++)
    {
        per (dx = -III; dx <= III; dx++)
        {
            s32 x = cx + dx;
            s32 y = cy + dy;
            s32 adx = dx < ZEPHYRUM ? -dx : dx;
            s32 ady = dy < ZEPHYRUM ? -dy : dy;
            s32 dist = adx > ady ? adx : ady; /* Chebyshev distance */

            /* Pattern: outer=black, ring=white, center 3x3=black
             * dist 3: black (outer), dist 2: white, dist 0-1: black (center 3x3) */
            si (dist == II)
            {
                _ponere_modulum(moduli, mag, x, y, ZEPHYRUM); /* albus */
            }
            alioquin
            {
                _ponere_modulum(moduli, mag, x, y, I); /* niger */
            }
        }
    }
}

/* Ponere separator (zona alba) circa finder */
interior vacuum
_ponere_separator(i8* moduli, s32 mag, s32 cx, s32 cy, s32 dir_x, s32 dir_y)
{
    s32 i;

    /* Linea horizontalis */
    per (i = -IV; i <= IV; i++)
    {
        _ponere_modulum(moduli, mag, cx + i, cy + dir_y * IV, ZEPHYRUM);
    }
    /* Linea verticalis */
    per (i = -III; i <= III; i++)
    {
        _ponere_modulum(moduli, mag, cx + dir_x * IV, cy + i, ZEPHYRUM);
    }
}

/* Ponere alignment pattern (5x5) ad positionem
 * Pattern: outer black, middle white, center black */
interior vacuum
_ponere_alignment(i8* moduli, s32 mag, s32 cx, s32 cy)
{
    s32 dx, dy;

    per (dy = -II; dy <= II; dy++)
    {
        per (dx = -II; dx <= II; dx++)
        {
            s32 x = cx + dx;
            s32 y = cy + dy;
            s32 adx = dx < ZEPHYRUM ? -dx : dx;
            s32 ady = dy < ZEPHYRUM ? -dy : dy;
            s32 dist = adx > ady ? adx : ady;

            /* dist 2: black (outer), dist 1: white, dist 0: black (center) */
            si (dist == I)
            {
                _ponere_modulum(moduli, mag, x, y, ZEPHYRUM); /* albus */
            }
            alioquin
            {
                _ponere_modulum(moduli, mag, x, y, I); /* niger */
            }
        }
    }
}

/* Ponere omnes alignment patterns pro versione
 * Saltat positiones quae superponunt cum finder patterns */
interior vacuum
_ponere_alignment_patterns(i8* moduli, s32 mag, s32 versio)
{
    s32 num_pos;
    s32 i, j;
    s32 cx, cy;

    si (versio < II || versio > X) redde; /* V1 non habet, V11+ non supportatus */

    num_pos = (s32)_alignment_positiones[versio - I][ZEPHYRUM];
    si (num_pos < II) redde;

    /* Iterare per omnes combinationes positionum */
    per (i = I; i <= num_pos; i++)
    {
        per (j = I; j <= num_pos; j++)
        {
            cx = (s32)_alignment_positiones[versio - I][i];
            cy = (s32)_alignment_positiones[versio - I][j];

            /* Saltare si superponitur cum finder patterns (3 anguli) */
            /* Top-left finder: centrum ad (3,3), extendere ad 0-6 */
            si (cx <= VI && cy <= VI) perge;
            /* Top-right finder: centrum ad (mag-4,3) */
            si (cx >= mag - VII && cy <= VI) perge;
            /* Bottom-left finder: centrum ad (3,mag-4) */
            si (cx <= VI && cy >= mag - VII) perge;

            _ponere_alignment(moduli, mag, cx, cy);
        }
    }
}

/* Ponere timing patterns (alternating) */
interior vacuum
_ponere_timing(i8* moduli, s32 mag)
{
    s32 i;

    /* Timing horizontalis (row 6) */
    per (i = VIII; i < mag - VIII; i++)
    {
        _ponere_modulum(moduli, mag, i, VI, (i + I) % II);
    }

    /* Timing verticalis (column 6) */
    per (i = VIII; i < mag - VIII; i++)
    {
        _ponere_modulum(moduli, mag, VI, i, (i + I) % II);
    }
}

/* Ponere dark module (semper nigrum) */
interior vacuum
_ponere_dark_module(i8* moduli, s32 mag, s32 versio)
{
    /* Positio: (8, 4*versio + 9) */
    s32 y = IV * versio + IX;
    si (y < mag)
    {
        _ponere_modulum(moduli, mag, VIII, y, I);
    }
}

/* Informatio versionis (V7+): XVIII bits bis positi - infra-sinistra
 * (3 lata x 6 alta) et supra-dextra (6 lata x 3 alta), speculo. */
interior vacuum
_ponere_versio_info(i8* moduli, s32 mag, s32 versio)
{
    s32 info;
    s32 i;

    si (versio < VII || versio > X)
    {
        redde;
    }
    info = (s32)_versio_info[versio - VII];
    per (i = ZEPHYRUM; i < XVIII; i++)
    {
        s32 bit = (info >> i) & I;

        /* infra-sinistra: x = i/3, y = mag - 11 + i%3 */
        _ponere_modulum(moduli, mag, i / III, mag - XI + i % III, bit);
        /* supra-dextra: speculum per diagonalem */
        _ponere_modulum(moduli, mag, mag - XI + i % III, i / III, bit);
    }
}

/* Ponere format info (15 bits) circa finders */
interior vacuum
_ponere_format_info(i8* moduli, s32 mag, s32 format_bits)
{
    s32 i;
    s32 bit;

    /* Prima copia: circa top-left finder */
    /* Bits 0-5: sub finder (column 8, rows 0-5, skip row 6 timing) */
    per (i = ZEPHYRUM; i < VI; i++)
    {
        bit = (format_bits >> i) & I;
        _ponere_modulum(moduli, mag, VIII, i, bit);
    }
    /* Bit 6: row 7 */
    bit = (format_bits >> VI) & I;
    _ponere_modulum(moduli, mag, VIII, VII, bit);
    /* Bit 7: row 8 */
    bit = (format_bits >> VII) & I;
    _ponere_modulum(moduli, mag, VIII, VIII, bit);
    /* Bits 8-14: dextra finder (row 8, columns 7-0, skip column 6) */
    per (i = ZEPHYRUM; i < VII; i++)
    {
        bit = (format_bits >> (VIII + i)) & I;
        si (i < I)
        {
            _ponere_modulum(moduli, mag, VII, VIII, bit);
        }
        alioquin
        {
            _ponere_modulum(moduli, mag, V - (i - I), VIII, bit);
        }
    }

    /* Secunda copia: circa right et bottom finders */
    /* Bits 0-7: bottom-left (column 8, rows mag-1 to mag-8) */
    per (i = ZEPHYRUM; i < VIII; i++)
    {
        bit = (format_bits >> i) & I;
        _ponere_modulum(moduli, mag, VIII, mag - I - i, bit);
    }
    /* Bits 8-14: top-right (row 8, columns mag-8 to mag-1) */
    per (i = ZEPHYRUM; i < VII; i++)
    {
        bit = (format_bits >> (VIII + i)) & I;
        _ponere_modulum(moduli, mag, mag - VIII + i, VIII, bit);
    }
}


/* ===========================================================
 * DATA PLACEMENT
 * =========================================================== */

/* Verificare si positio est reservata (finder, timing, format, etc.) */
interior b32
_est_reservatum(s32 mag, s32 x, s32 y, s32 versio)
{
    /* Top-left finder + separator (9x9) */
    si (x < IX && y < IX) redde VERUM;

    /* Top-right finder + separator */
    si (x >= mag - VIII && y < IX) redde VERUM;

    /* Bottom-left finder + separator */
    si (x < IX && y >= mag - VIII) redde VERUM;

    /* Timing patterns */
    si (x == VI || y == VI) redde VERUM;

    /* Dark module */
    si (x == VIII && y == IV * versio + IX) redde VERUM;

    /* Informatio versionis (V7+): duo areae 3x6 (ISO 18004 §8.10) */
    si (versio >= VII)
    {
        si (x < VI && y >= mag - XI && y < mag - VIII) redde VERUM;
        si (y < VI && x >= mag - XI && x < mag - VIII) redde VERUM;
    }

    /* Alignment patterns (V2+) */
    si (versio >= II && versio <= X)
    {
        s32 num_pos = (s32)_alignment_positiones[versio - I][ZEPHYRUM];
        s32 i, j;

        per (i = I; i <= num_pos; i++)
        {
            per (j = I; j <= num_pos; j++)
            {
                s32 cx = (s32)_alignment_positiones[versio - I][i];
                s32 cy = (s32)_alignment_positiones[versio - I][j];
                s32 dx = x - cx;
                s32 dy = y - cy;

                /* Saltare si superponitur cum finder */
                si (cx <= VI && cy <= VI) perge;
                si (cx >= mag - VII && cy <= VI) perge;
                si (cx <= VI && cy >= mag - VII) perge;

                /* Verificare si intra 5x5 alignment pattern */
                si (dx >= -II && dx <= II && dy >= -II && dy <= II)
                {
                    redde VERUM;
                }
            }
        }
    }

    redde FALSUM;
}

/* Ponere data bits in zigzag pattern */
interior vacuum
_ponere_data(i8* moduli, s32 mag, constans i8* data, s32 data_len, s32 versio)
{
    s32 x, y;
    s32 bit_index;
    s32 upward;
    s32 col;

    bit_index = ZEPHYRUM;
    upward = VERUM;

    /* Iterare per columnas dextrae ad sinistram, 2 ad tempus */
    per (col = mag - I; col >= I; col -= II)
    {
        /* Skip timing column */
        si (col == VI) col = V;

        per (y = upward ? mag - I : ZEPHYRUM;
             upward ? y >= ZEPHYRUM : y < mag;
             y += upward ? -I : I)
        {
            /* Duae columnae: col et col-1 */
            per (x = col; x >= col - I && x >= ZEPHYRUM; x--)
            {
                si (!_est_reservatum(mag, x, y, versio))
                {
                    s32 byte_index = bit_index / VIII;
                    s32 bit_offset = VII - (bit_index % VIII);
                    s32 bit_val = ZEPHYRUM;

                    si (byte_index < data_len)
                    {
                        bit_val = (data[byte_index] >> bit_offset) & I;
                    }

                    moduli[y * mag + x] = (i8)bit_val;
                    bit_index++;
                }
            }
        }

        upward = !upward;
    }
}


/* ===========================================================
 * MASKING
 * =========================================================== */

/* Applicare mask pattern ad modulos
 * mask: 0-7 */
interior vacuum
_applicare_mask(i8* moduli, s32 mag, s32 mask, s32 versio)
{
    s32 x, y;

    per (y = ZEPHYRUM; y < mag; y++)
    {
        per (x = ZEPHYRUM; x < mag; x++)
        {
            b32 flip = FALSUM;

            si (_est_reservatum(mag, x, y, versio)) perge;

            commutatio (mask)
            {
                casus 0: flip = ((x + y) % II == ZEPHYRUM); frange;
                casus 1: flip = (y % II == ZEPHYRUM); frange;
                casus 2: flip = (x % III == ZEPHYRUM); frange;
                casus 3: flip = ((x + y) % III == ZEPHYRUM); frange;
                casus 4: flip = ((x / III + y / II) % II == ZEPHYRUM); frange;
                casus 5: flip = ((x * y) % II + (x * y) % III == ZEPHYRUM); frange;
                casus 6: flip = (((x * y) % II + (x * y) % III) % II == ZEPHYRUM); frange;
                casus 7: flip = (((x + y) % II + (x * y) % III) % II == ZEPHYRUM); frange;
            }

            si (flip)
            {
                moduli[y * mag + x] ^= I;
            }
        }
    }
}

/* Calculare penalty score pro mask evaluation */
interior s32
_calculare_poena(constans i8* moduli, s32 mag)
{
    s32 poena = ZEPHYRUM;
    s32 x, y;
    s32 count;
    s32 prev;

    /* Rule 1: Adjacent modules in row/column (5+ same color) */
    /* Rows */
    per (y = ZEPHYRUM; y < mag; y++)
    {
        count = I;
        prev = moduli[y * mag];
        per (x = I; x < mag; x++)
        {
            si (moduli[y * mag + x] == prev)
            {
                count++;
            }
            alioquin
            {
                si (count >= V) poena += III + (count - V);
                count = I;
                prev = moduli[y * mag + x];
            }
        }
        si (count >= V) poena += III + (count - V);
    }

    /* Columns */
    per (x = ZEPHYRUM; x < mag; x++)
    {
        count = I;
        prev = moduli[x];
        per (y = I; y < mag; y++)
        {
            si (moduli[y * mag + x] == prev)
            {
                count++;
            }
            alioquin
            {
                si (count >= V) poena += III + (count - V);
                count = I;
                prev = moduli[y * mag + x];
            }
        }
        si (count >= V) poena += III + (count - V);
    }

    /* Rule 2: 2x2 blocks of same color */
    per (y = ZEPHYRUM; y < mag - I; y++)
    {
        per (x = ZEPHYRUM; x < mag - I; x++)
        {
            s32 val = moduli[y * mag + x];
            si (moduli[y * mag + x + I] == val &&
                moduli[(y + I) * mag + x] == val &&
                moduli[(y + I) * mag + x + I] == val)
            {
                poena += III;
            }
        }
    }

    /* Rule 3: Finder-like patterns (simplified) */
    /* Full implementation would check for 1:1:3:1:1 patterns */

    /* Rule 4: Dark/light ratio */
    {
        s32 dark = ZEPHYRUM;
        s32 total = mag * mag;
        s32 percent;
        s32 deviation;

        per (y = ZEPHYRUM; y < mag; y++)
        {
            per (x = ZEPHYRUM; x < mag; x++)
            {
                si (moduli[y * mag + x]) dark++;
            }
        }

        percent = (dark * C) / total;
        deviation = percent - L;
        si (deviation < ZEPHYRUM) deviation = -deviation;
        poena += (deviation / V) * X;
    }

    redde poena;
}


/* ===========================================================
 * FUNCTIONES PUBLICAE
 * =========================================================== */

b32
qr_structura(i32 versio, QREcc ecc, i32* bloci, i32* data_codewords,
    i32* ecc_codewords)
{
    i32 ec;

    si (versio < I || versio > X) redde FALSUM;
    si (ecc < ZEPHYRUM || ecc > III) redde FALSUM;

    ec = _ecc_codewords[versio - I][ecc];
    si (bloci) *bloci = _bloci[versio - I][ecc];
    si (ecc_codewords) *ecc_codewords = ec;
    si (data_codewords)
    {
        *data_codewords = _data_codewords_totalis[versio - I] - ec;
    }
    redde VERUM;
}

i32
qr_amplitudo_versionis(i32 versio)
{
    si (versio < I || versio > XL) redde ZEPHYRUM;
    redde XVII + versio * IV;
}

i32
qr_versio_minima(i32 mensura_datorum, QREcc ecc)
{
    i32 v;

    si (ecc < ZEPHYRUM || ecc > III) redde ZEPHYRUM;

    per (v = I; v <= X; v++)
    {
        si (_capacitas_octeti[v - I][ecc] >= mensura_datorum)
        {
            redde v;
        }
    }

    redde ZEPHYRUM; /* Data nimis magna */
}

s32
qr_modulus(constans QR* qr, s32 x, s32 y)
{
    si (!qr || !qr->moduli) redde -I;
    si (x < ZEPHYRUM || x >= (s32)qr->amplitudo) redde -I;
    si (y < ZEPHYRUM || y >= (s32)qr->amplitudo) redde -I;

    redde qr->moduli[y * (s32)qr->amplitudo + x];
}

QR
qr_generare_ex_octets(constans i8* datum, i32 mensura, QREcc ecc, Piscina* piscina)
{
    QR qr;
    i32 versio;
    i32 mag;
    i32 data_codewords;
    i32 ecc_codewords;
    i32 total_codewords;
    i8* codewords;
    i32 bit_index;
    i32 i;
    s32 best_mask;
    s32 best_poena;
    i8* best_moduli;

    /* Initiare GF(256) */
    _gf256_initiare();

    /* Initiare fructus */
    qr.amplitudo = ZEPHYRUM;
    qr.moduli = NIHIL;
    qr.versio = ZEPHYRUM;
    qr.successus = FALSUM;

    si (!datum || mensura <= ZEPHYRUM || !piscina) redde qr;
    si (ecc < ZEPHYRUM || ecc > III) redde qr;

    /* Invenire versio minima */
    versio = qr_versio_minima(mensura, ecc);
    si (versio == ZEPHYRUM) redde qr; /* Data nimis magna */

    /* Calculare amplitudines */
    mag = qr_amplitudo_versionis(versio);
    ecc_codewords = _ecc_codewords[versio - I][ecc];
    total_codewords = _data_codewords_totalis[versio - I];
    /* CODEWORDS data, NON characteres capacitatis: _capacitas_octeti
     * capacitatem NUNTII fert (sine capite modi+numeri), quae
     * codewordis II minor est (III pro V10, ubi numerus XVI bitis
     * scribitur). Hausta illinc, ECC in offset falso sedebat et
     * codewords ultimi zephyri manebant. */
    data_codewords = total_codewords - ecc_codewords;

    /* Allocare codewords buffer */
    codewords = (i8*)piscina_allocare(piscina, (memoriae_index)total_codewords);
    memset(codewords, ZEPHYRUM, (memoriae_index)total_codewords);

    /* Codificare data in byte mode */
    bit_index = ZEPHYRUM;

    /* Mode indicator (4 bits): 0100 = byte mode */
    codewords[ZEPHYRUM] = (i8)0x40;
    bit_index = IV;

    /* Character count: VIII bitis pro V1-V9, XVI pro V10+ (modus
     * octetus, ISO 18004 tabula 3). Ansa datorum infra offset
     * arbitrarium bitorum tractat, ergo bit_index solus mutat. */
    si (versio < X)
    {
        codewords[ZEPHYRUM] |= (i8)((mensura >> IV) & 0x0F);
        codewords[I] = (i8)((mensura & 0x0F) << IV);
        bit_index = XII;
    }
    alioquin
    {
        codewords[ZEPHYRUM] |= (i8)((mensura >> XII) & 0x0F);
        codewords[I] = (i8)((mensura >> IV) & 0xFF);
        codewords[II] = (i8)((mensura & 0x0F) << IV);
        bit_index = XX;
    }

    /* Data bytes */
    per (i = ZEPHYRUM; i < mensura; i++)
    {
        i32 byte_pos = bit_index / VIII;
        i32 bit_off = bit_index % VIII;

        si (bit_off == ZEPHYRUM)
        {
            codewords[byte_pos] = datum[i];
        }
        alioquin
        {
            codewords[byte_pos] |= (i8)(((i32)(i8)datum[i] >> bit_off) & 0xFF);
            codewords[byte_pos + I] = (i8)((datum[i] << (VIII - bit_off)) & 0xFF);
        }
        bit_index += VIII;
    }

    /* Terminator (4 bits of 0) - si spatium */
    /* Padding ad byte boundary */
    /* Padding bytes: 0xEC, 0x11 alternating */
    {
        i32 data_bits = bit_index;
        i32 capacity_bits = data_codewords * VIII;
        i32 pad_byte;

        /* Add terminator si spatium */
        si (data_bits + IV <= capacity_bits)
        {
            /* Terminator jam zeros */
            bit_index += IV;
        }

        /* Pad ad byte */
        bit_index = (bit_index + VII) / VIII * VIII;

        /* Add padding bytes */
        pad_byte = ZEPHYRUM;
        dum (bit_index / VIII < data_codewords)
        {
            codewords[bit_index / VIII] = (i8)(pad_byte ? 0x11 : 0xEC);
            pad_byte = !pad_byte;
            bit_index += VIII;
        }
    }

    /* ECC PER BLOCCUM + INTERTEXTIO (ISO 18004 §8.6).
     *
     * Codewords data et ECC per bloccos INTERTEXENDA sunt, non
     * simpliciter appendenda. Appensio recta est bloco UNO solo -
     * ergo versiones humiles laborabant (V1-V3 gradu M) et V4+
     * non, quod mensuratum est (oraculum: probatio_qr_gyrus.sh). */
    {
        i32 bloci     = _bloci[versio - I][ecc];
        i32 ec_per_bl = ecc_codewords / bloci;
        i32 offset;
        i32 b, j, k;
        i8* ec_omnia;
        i8* textum;

        ec_omnia = (i8*)piscina_allocare(piscina,
            (memoriae_index)ecc_codewords);
        textum = (i8*)piscina_allocare(piscina,
            (memoriae_index)total_codewords);
        si (!ec_omnia || !textum) redde qr;

        /* ECC seorsum per bloccum */
        offset = ZEPHYRUM;
        per (b = ZEPHYRUM; b < bloci; b++)
        {
            i32 len = _bloci_mensura(data_codewords, bloci, b);

            _rs_codificare(codewords + offset, len,
                ec_omnia + b * ec_per_bl, ec_per_bl);
            offset += len;
        }

        /* intertextio datorum: codeword j cuiusque bloci per vices
         * (bloci breviores unum codeword ultimum carent - saltantur) */
        k = ZEPHYRUM;
        per (j = ZEPHYRUM; j <= data_codewords / bloci; j++)
        {
            offset = ZEPHYRUM;
            per (b = ZEPHYRUM; b < bloci; b++)
            {
                i32 len = _bloci_mensura(data_codewords, bloci, b);

                si (j < len)
                {
                    textum[k++] = codewords[offset + j];
                }
                offset += len;
            }
        }

        /* intertextio ECC: bloci ECC omnes eiusdem longitudinis */
        per (j = ZEPHYRUM; j < ec_per_bl; j++)
        {
            per (b = ZEPHYRUM; b < bloci; b++)
            {
                textum[k++] = ec_omnia[b * ec_per_bl + j];
            }
        }

        memcpy(codewords, textum, (memoriae_index)total_codewords);
    }

    /* Allocare moduli */
    qr.moduli = (i8*)piscina_allocare(piscina, (memoriae_index)(mag * mag));
    memset(qr.moduli, ZEPHYRUM, (memoriae_index)(mag * mag));

    /* Construere matrix */
    /* Finder patterns */
    _ponere_finder(qr.moduli, (s32)mag, III, III);           /* top-left */
    _ponere_finder(qr.moduli, (s32)mag, (s32)mag - IV, III); /* top-right */
    _ponere_finder(qr.moduli, (s32)mag, III, (s32)mag - IV); /* bottom-left */

    /* Separators */
    _ponere_separator(qr.moduli, (s32)mag, III, III, I, I);
    _ponere_separator(qr.moduli, (s32)mag, (s32)mag - IV, III, -I, I);
    _ponere_separator(qr.moduli, (s32)mag, III, (s32)mag - IV, I, -I);

    /* Timing patterns */
    _ponere_timing(qr.moduli, (s32)mag);

    /* Alignment patterns (V2+) */
    _ponere_alignment_patterns(qr.moduli, (s32)mag, (s32)versio);

    /* Dark module */
    _ponere_dark_module(qr.moduli, (s32)mag, (s32)versio);

    /* Ponere data */
    _ponere_data(qr.moduli, (s32)mag, codewords, (s32)total_codewords, (s32)versio);

    /* Probare omnes masks et eligere optimum */
    best_mask = ZEPHYRUM;
    best_poena = 0x7FFFFFFF;
    best_moduli = (i8*)piscina_allocare(piscina, (memoriae_index)(mag * mag));

    per (i = ZEPHYRUM; i < VIII; i++)
    {
        i8* temp_moduli;
        s32 poena;

        temp_moduli = (i8*)piscina_allocare(piscina, (memoriae_index)(mag * mag));
        memcpy(temp_moduli, qr.moduli, (memoriae_index)(mag * mag));

        _applicare_mask(temp_moduli, (s32)mag, (s32)i, (s32)versio);
        _ponere_format_info(temp_moduli, (s32)mag, (s32)_format_info[ecc][i]);
        /* informatio versionis (V7+) POST larvam: sicut informatio
         * formati, larvanda NON est */
        _ponere_versio_info(temp_moduli, (s32)mag, (s32)versio);

        poena = _calculare_poena(temp_moduli, (s32)mag);

        si (poena < best_poena)
        {
            best_poena = poena;
            best_mask = (s32)i;
            memcpy(best_moduli, temp_moduli, (memoriae_index)(mag * mag));
        }
    }

    /* Usare optimum mask */
    memcpy(qr.moduli, best_moduli, (memoriae_index)(mag * mag));
    (vacuum)best_mask; /* Silentium: reservatum pro futuro usu */

    qr.amplitudo = mag;
    qr.versio = versio;
    qr.successus = VERUM;

    redde qr;
}

QR
qr_generare(chorda datum, QREcc ecc, Piscina* piscina)
{
    si (datum.datum == NIHIL || datum.mensura <= ZEPHYRUM)
    {
        QR qr_vacuus;
        qr_vacuus.amplitudo = ZEPHYRUM;
        qr_vacuus.moduli = NIHIL;
        qr_vacuus.versio = ZEPHYRUM;
        qr_vacuus.successus = FALSUM;
        redde qr_vacuus;
    }

    redde qr_generare_ex_octets(datum.datum, datum.mensura, ecc, piscina);
}
