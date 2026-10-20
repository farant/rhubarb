/* blake2b.c - BLAKE2b secundum RFC 7693 (par. 3, codex Appendix C
 * re-expressus): G cum rotationibus 32/24/16/63, XII circuli, clavis
 * ut massa prima impleta, massa ultima vexillo finali. */
#include "blake2b.h"

#include <string.h>

hic_manens constans i64 BLAKE2B_IV[VIII] = {
    0x6A09E667F3BCC908ULL, 0xBB67AE8584CAA73BULL,
    0x3C6EF372FE94F82BULL, 0xA54FF53A5F1D36F1ULL,
    0x510E527FADE682D1ULL, 0x9B05688C2B3E6C1FULL,
    0x1F83D9ABFB41BD6BULL, 0x5BE0CD19137E2179ULL
};

hic_manens constans i8 BLAKE2B_SIGMA[XII][XVI] = {
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
    { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 },
    { 11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4 },
    { 7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8 },
    { 9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13 },
    { 2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9 },
    { 12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11 },
    { 13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10 },
    { 6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5 },
    { 10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0 },
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
    { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 }
};

#define CIRCUMVOLVERE(x, n) (((x) >> (n)) | ((x) << (LXIV - (n))))

#define B2B_G(a, b, c, d, x, y)        \
    fac {                              \
        v[a] = v[a] + v[b] + (x);      \
        v[d] = CIRCUMVOLVERE(v[d] ^ v[a], XXXII); \
        v[c] = v[c] + v[d];            \
        v[b] = CIRCUMVOLVERE(v[b] ^ v[c], XXIV); \
        v[a] = v[a] + v[b] + (y);      \
        v[d] = CIRCUMVOLVERE(v[d] ^ v[a], XVI); \
        v[c] = v[c] + v[d];            \
        v[b] = CIRCUMVOLVERE(v[b] ^ v[c], LXIII); \
    } dum (0)

interior i64
_octoni_legere (
    constans i8* p)
{
    redde (i64)p[0] | ((i64)p[1] << VIII) | ((i64)p[2] << XVI)
        | ((i64)p[3] << XXIV) | ((i64)p[4] << XXXII)
        | ((i64)p[5] << XL) | ((i64)p[6] << XLVIII)
        | ((i64)p[7] << LVI);
}

/* F: compressio massae b; ultima = vexillum finale */
interior vacuum
_comprimere (
    Blake2b* s,
        b32  ultima)
{
    i64 v[XVI];
    i64 m[XVI];
    i32 i;

    per (i = 0; i < VIII; i++)
    {
        v[i]         = s->h[i];
        v[i + VIII]  = BLAKE2B_IV[i];
    }
    v[XII]   ^= s->t[0];
    v[XIII]  ^= s->t[1];
    si (ultima)
    {
        v[XIV] = ~v[XIV];
    }
    per (i = 0; i < XVI; i++)
    {
        m[i] = _octoni_legere(s->b + VIII * i);
    }
    per (i = 0; i < XII; i++)
    {
        constans i8* sg = BLAKE2B_SIGMA[i];

        B2B_G(0, IV, VIII, XII, m[sg[0]], m[sg[1]]);
        B2B_G(I, V, IX, XIII, m[sg[2]], m[sg[3]]);
        B2B_G(II, VI, X, XIV, m[sg[4]], m[sg[5]]);
        B2B_G(III, VII, XI, XV, m[sg[6]], m[sg[7]]);
        B2B_G(0, V, X, XV, m[sg[8]], m[sg[9]]);
        B2B_G(I, VI, XI, XII, m[sg[10]], m[sg[11]]);
        B2B_G(II, VII, VIII, XIII, m[sg[12]], m[sg[13]]);
        B2B_G(III, IV, IX, XIV, m[sg[14]], m[sg[15]]);
    }
    per (i = 0; i < VIII; i++)
    {
        s->h[i] ^= v[i] ^ v[i + VIII];
    }
}

b32
blake2b_incipere (
         Blake2b* status,
             i32  exitus_n,
     constans i8* clavis,
             i32  clavis_n)
{
    i32 i;

    si (   !status || exitus_n < I || exitus_n > BLAKE2B_EXITUS_MAXIMUS
        || clavis_n > BLAKE2B_CLAVIS_MAXIMA
        || (clavis_n > 0 && !clavis))
    {
        redde FALSUM;
    }
    per (i = 0; i < VIII; i++)
    {
        status->h[i] = BLAKE2B_IV[i];
    }
    /* parametrum: profunditas 1, fanout 1, clavis_n, exitus_n */
    status->h[0] ^= 0x01010000ULL ^ ((i64)clavis_n << VIII)
        ^ (i64)exitus_n;
    status->t[0]      = 0;
    status->t[1]      = 0;
    status->c         = 0;
    status->exitus_n  = exitus_n;
    memset(status->b, 0, BLAKE2B_MASSA);
    si (clavis_n > 0)
    {
        blake2b_addere(status, clavis, clavis_n);
        status->c = BLAKE2B_MASSA;   /* clavis = massa tota impleta */
    }
    redde VERUM;
}

vacuum
blake2b_addere (
         Blake2b* status,
     constans i8* octeti,
             i32  n)
{
    i32 i;

    per (i = 0; i < n; i++)
    {
        si (status->c == BLAKE2B_MASSA)
        {
            /* massa plena et plura adveniunt: non ultima */
            status->t[0] += (i64)status->c;
            si (status->t[0] < (i64)status->c)
            {
                status->t[1]++;
            }
            _comprimere(status, FALSUM);
            status->c = 0;
        }
        status->b[status->c++] = octeti[i];
    }
}

vacuum
blake2b_finire (
    Blake2b* status,
         i8* exitus)
{
    i32 i;

    status->t[0] += (i64)status->c;
    si (status->t[0] < (i64)status->c)
    {
        status->t[1]++;
    }
    dum (status->c < BLAKE2B_MASSA)
    {
        status->b[status->c++] = 0;
    }
    _comprimere(status, VERUM);
    per (i = 0; i < status->exitus_n; i++)
    {
        exitus[i] = (i8)((status->h[i >> III] >> (VIII * (i & VII)))
            & 0xFF);
    }
}

vacuum
blake2b (
              i8* exitus,
             i32  exitus_n,
     constans i8* octeti,
             i32  n,
     constans i8* clavis,
             i32  clavis_n)
{
    Blake2b s;

    si (!blake2b_incipere(&s, exitus_n, clavis, clavis_n))
    {
        redde;
    }
    blake2b_addere(&s, octeti, n);
    blake2b_finire(&s, exitus);
}
