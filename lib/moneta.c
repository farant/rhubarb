/* moneta.c - Cusio identitatum (ULID; contractus in capite).
 * Implementatio ex lib/scrinium.c mota 2026-07-21 (exemplar
 * libraries.ts: monotona intra ms; semita POSIX gettimeofday -
 * exemplar tcp_posix). */

#include "moneta.h"
#include "fasti.h"   /* Momentum: millisecunda ab epocha (canon) */
#include <stdio.h>
#include <sys/time.h>

hic_manens constans character ULID_LITTERAE[] =
    "0123456789ABCDEFGHJKMNPQRSTVWXYZ";

hic_manens Momentum _ulid_tempus_ultimum = 0;
hic_manens i32 _ulid_fortuita[16];

/* octeti fortuiti ex /dev/urandom (stdio purum - semita POSIX sed
 * mechanismus vulgaris); in defectu, miscela temporis */
interior vacuum
_fortuita_implere (i8* effusio, memoriae_index mensura)
{
    FILE* fons = fopen("/dev/urandom", "rb");

    si (fons != NIHIL)
    {
        si (fread(effusio, I, mensura, fons) == mensura)
        {
            fclose(fons);
            redde;
        }
        fclose(fons);
    }
    {
        memoriae_index k;
        i64 semen = (i64)_ulid_tempus_ultimum
            ^ (i64)0x9e3779b97f4a7c15ULL;

        per (k = ZEPHYRUM; k < mensura; k++)
        {
            semen = semen * (i64)6364136223846793005ULL
                + (i64)1442695040888963407ULL;
            effusio[k] = (i8)((semen >> 33) & 0xffUL);
        }
    }
}

vacuum
moneta_ulid (character* effusio)
{
    structura timeval nunc_tv;
    Momentum nunc;
    integer k;

    gettimeofday(&nunc_tv, NIHIL);
    /* instans = secunda scalata + residuum microsecundorum ut MORA
     * (punctum + spatium; punctum + punctum machina recte vetuit) */
    nunc = (Momentum)nunc_tv.tv_sec * 1000
         + (Mora)nunc_tv.tv_usec / 1000;

    si (nunc <= _ulid_tempus_ultimum)
    {
        /* ms idem (aut horologium retro): fortuita augere -
         * ordo generationis = ordo lexicographicus */
        b32 lator = VERUM;

        per (k = 15; k >= 0 && lator; k--)
        {
            _ulid_fortuita[k]++;
            si (_ulid_fortuita[k] >= 32)
            {
                _ulid_fortuita[k] = 0;
            }
            alioquin
            {
                lator = FALSUM;
            }
        }
    }
    alioquin
    {
        i8 octeti[10];
        integer pars;

        _ulid_tempus_ultimum = nunc;
        _fortuita_implere(octeti, 10);
        per (pars = 0; pars < 2; pars++)
        {
            i64 n = 0;
            integer b;

            per (b = 0; b < 5; b++)
            {
                n = n * 256 + (i64)octeti[pars * 5 + b];
            }
            per (b = 7; b >= 0; b--)
            {
                _ulid_fortuita[pars * 8 + b] = (i32)(n % 32);
                n = n / 32;
            }
        }
    }

    {
        i64 t = (i64)_ulid_tempus_ultimum;   /* cifrae basi-32: bits
                                              * crudi - erasio consulta */

        per (k = 9; k >= 0; k--)
        {
            effusio[k] = ULID_LITTERAE[t % 32];
            t = t / 32;
        }
    }
    per (k = 0; k < 16; k++)
    {
        effusio[10 + k] = ULID_LITTERAE[_ulid_fortuita[k]];
    }
    effusio[26] = '\0';
}
