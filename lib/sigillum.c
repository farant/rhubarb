/* sigillum.c - SHA-256 (FIPS 180-4) - vide sigillum.h */

#include "sigillum.h"

#include <string.h>
#include <stdio.h>

/* constantes rotundarum (radices cubicae primorum 64, pars
 * fractionalis - FIPS 180-4 §4.2.2) */
hic_manens constans i32 SIGILLUM_K[64] = {
    0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL,
    0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
    0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
    0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
    0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
    0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
    0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL,
    0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
    0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
    0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
    0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL,
    0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
    0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL,
    0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
    0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
    0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

#define SIGILLUM_ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

/* bloccum unum (64 octeti) comprimere */
hic_manens vacuum
_bloccum_comprimere (i32* status, constans i8* bloccus)
{
    i32 w[64];
    i32 a;
    i32 b;
    i32 c;
    i32 d;
    i32 e;
    i32 f;
    i32 g;
    i32 h;
    integer t;

    per (t = 0; t < 16; t++)
    {
        w[t] = ((i32)bloccus[t * 4] << 24)
             | ((i32)bloccus[t * 4 + 1] << 16)
             | ((i32)bloccus[t * 4 + 2] << 8)
             | ((i32)bloccus[t * 4 + 3]);
    }
    per (t = 16; t < 64; t++)
    {
        i32 s0 = SIGILLUM_ROTR(w[t - 15], 7)
               ^ SIGILLUM_ROTR(w[t - 15], 18)
               ^ (w[t - 15] >> 3);
        i32 s1 = SIGILLUM_ROTR(w[t - 2], 17)
               ^ SIGILLUM_ROTR(w[t - 2], 19)
               ^ (w[t - 2] >> 10);

        w[t] = w[t - 16] + s0 + w[t - 7] + s1;
    }

    a = status[0];
    b = status[1];
    c = status[2];
    d = status[3];
    e = status[4];
    f = status[5];
    g = status[6];
    h = status[7];

    per (t = 0; t < 64; t++)
    {
        i32 s1 = SIGILLUM_ROTR(e, 6) ^ SIGILLUM_ROTR(e, 11)
               ^ SIGILLUM_ROTR(e, 25);
        i32 electio = (e & f) ^ ((~e) & g);
        i32 temp1 = h + s1 + electio + SIGILLUM_K[t] + w[t];
        i32 s0 = SIGILLUM_ROTR(a, 2) ^ SIGILLUM_ROTR(a, 13)
               ^ SIGILLUM_ROTR(a, 22);
        i32 maioritas = (a & b) ^ (a & c) ^ (b & c);
        i32 temp2 = s0 + maioritas;

        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    status[0] += a;
    status[1] += b;
    status[2] += c;
    status[3] += d;
    status[4] += e;
    status[5] += f;
    status[6] += g;
    status[7] += h;
}

vacuum
sigillum_incipere (SigillumContextus* contextus)
{
    /* radices quadratae primorum 8, pars fractionalis (§5.3.3) */
    contextus->status[0] = 0x6a09e667UL;
    contextus->status[1] = 0xbb67ae85UL;
    contextus->status[2] = 0x3c6ef372UL;
    contextus->status[3] = 0xa54ff53aUL;
    contextus->status[4] = 0x510e527fUL;
    contextus->status[5] = 0x9b05688cUL;
    contextus->status[6] = 0x1f83d9abUL;
    contextus->status[7] = 0x5be0cd19UL;
    contextus->massa_mensura = 0;
    contextus->totalis = 0;
}

vacuum
sigillum_addere (SigillumContextus* contextus, constans vacuum* datum,
    memoriae_index mensura)
{
    constans i8* octeti = (constans i8*)datum;
    memoriae_index k = 0;

    contextus->totalis += (i64)mensura;
    dum (k < mensura)
    {
        contextus->massa[contextus->massa_mensura] = octeti[k];
        contextus->massa_mensura++;
        k++;
        si (contextus->massa_mensura == 64)
        {
            _bloccum_comprimere(contextus->status, contextus->massa);
            contextus->massa_mensura = 0;
        }
    }
}

Sigillum
sigillum_finire (SigillumContextus* contextus)
{
    Sigillum fructus;
    i64 bitae = contextus->totalis * 8;
    integer k;

    /* farcimen: 0x80, deinde zephyra usque ad 56 mod 64, deinde
     * longitudo bitarum 64-bitis maiore-fine */
    {
        i8 unus = 0x80;

        sigillum_addere(contextus, &unus, 1);
    }
    {
        i8 zephyrum_o = 0;

        dum (contextus->massa_mensura != 56)
        {
            sigillum_addere(contextus, &zephyrum_o, 1);
        }
    }
    /* longitudo manu (sigillum_addere totalis augeret) */
    per (k = 7; k >= 0; k--)
    {
        contextus->massa[contextus->massa_mensura] =
            (i8)((bitae >> (k * 8)) & 0xffUL);
        contextus->massa_mensura++;
    }
    _bloccum_comprimere(contextus->status, contextus->massa);

    per (k = 0; k < 8; k++)
    {
        fructus.octeti[k * 4]     =
            (i8)((contextus->status[k] >> 24) & 0xffUL);
        fructus.octeti[k * 4 + 1] =
            (i8)((contextus->status[k] >> 16) & 0xffUL);
        fructus.octeti[k * 4 + 2] =
            (i8)((contextus->status[k] >> 8) & 0xffUL);
        fructus.octeti[k * 4 + 3] =
            (i8)(contextus->status[k] & 0xffUL);
    }
    redde fructus;
}

Sigillum
sigillum_computare (constans vacuum* datum, memoriae_index mensura)
{
    SigillumContextus contextus;

    sigillum_incipere(&contextus);
    sigillum_addere(&contextus, datum, mensura);
    redde sigillum_finire(&contextus);
}

vacuum
sigillum_hex (constans Sigillum* sigillum, character* effusio)
{
    hic_manens constans character DIGITI[] = "0123456789abcdef";
    integer k;

    per (k = 0; k < SIGILLUM_OCTETI; k++)
    {
        effusio[k * 2]     = DIGITI[(sigillum->octeti[k] >> 4) & 0x0f];
        effusio[k * 2 + 1] = DIGITI[sigillum->octeti[k] & 0x0f];
    }
    effusio[SIGILLUM_OCTETI * 2] = '\0';
}

b32
sigillum_aequale (constans Sigillum* a, constans Sigillum* b)
{
    /* TEMPORE CONSTANTI: memcmp ad primam differentiam exit, ergo
     * tempus responsi quot octeti congruant prodit. Ubi sigillum
     * SECRETUM custodit (tessera, clavis), id oppugnatori sinit
     * octetum post octetum divinare. Nulla sedes umquam exitum
     * praematurum voluit - sigilla longitudinis fixae sunt et
     * XXXII disiunctiones nihil constant - ergo functio ipsa
     * emendatur, non altera addita. */
    i32 i;
    i32 differentia = ZEPHYRUM;

    si (a == NIHIL || b == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < SIGILLUM_OCTETI; i++)
    {
        differentia |= (i32)(a->octeti[i] ^ b->octeti[i]);
    }
    redde (differentia == ZEPHYRUM) ? VERUM : FALSUM;
}
