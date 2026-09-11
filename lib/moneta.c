/* moneta.c - Cusio identitatum (ULID, nanoid; contractus in capite).
 * Implementatio ULID ex lib/scrinium.c mota 2026-07-21 (exemplar
 * libraries.ts: monotona intra ms; semita POSIX gettimeofday -
 * exemplar tcp_posix). Nanoid addita 2026-09-10 (larva + abiectio
 * ut nanoid; fons sine reservo). */

#include "postulata_posix.h"
#include "moneta.h"
#include "fasti.h"   /* Momentum: millisecunda ab epocha (canon) */
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

hic_manens constans character ULID_LITTERAE[] =
    "0123456789ABCDEFGHJKMNPQRSTVWXYZ";

hic_manens Momentum _ulid_tempus_ultimum = 0;
hic_manens i32 _ulid_fortuita[16];

/* octeti fortuiti ex /dev/urandom (stdio purum - semita POSIX sed
 * mechanismus vulgaris); in defectu, miscela temporis */
interior vacuum
_fortuita_implere (
                i8* effusio,
    memoriae_index  mensura)
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
moneta_ulid (
    character* effusio)
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
                _ulid_fortuita[pars * 8 + b]  = (i32)(n % 32);
                n                             = n / 32;
            }
        }
    }

    {
        /* cifrae basi-32: bits crudi - erasio consulta */
        i64 t = (i64)_ulid_tempus_ultimum;

        per (k = 9; k >= 0; k--)
        {
            effusio[k]  = ULID_LITTERAE[t % 32];
            t           = t / 32;
        }
    }
    per (k = 0; k < 16; k++)
    {
        effusio[10 + k] = ULID_LITTERAE[_ulid_fortuita[k]];
    }
    effusio[26] = '\0';
}

vacuum
moneta_ulid_fortuita (
    character* effusio)
{
    structura timeval nunc_tv;
             Momentum nunc;
                   i8 octeti[10];
              integer k;
              integer pars;

    gettimeofday(&nunc_tv, NIHIL);
    nunc = (Momentum)nunc_tv.tv_sec * 1000
         + (Mora)nunc_tv.tv_usec / 1000;

    {
        i64 t = (i64)nunc;

        per (k = 9; k >= 0; k--)
        {
            effusio[k]  = ULID_LITTERAE[t % 32];
            t           = t / 32;
        }
    }
    /* fortuita RECENTIA quaque vocatione - status monotoniae
     * intactus (gemini eiusdem ms trans partem fortuitam TOTAM
     * differunt, non solum signo ultimo) */
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
            effusio[10 + pars * 8 + b]  = ULID_LITTERAE[n % 32];
            n                           = n / 32;
        }
    }
    effusio[26] = '\0';
}

/* octeti fortuiti sine reservo: pro secretis, ubi reservum
 * praevisibile peius est quam defectus apertus (vide moneta.h) */
b32
moneta_octeti_fortuiti (
     i8* effusio,
    i32  mensura)
{
    FILE* fons;

    si (effusio == NIHIL || mensura <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    fons = fopen("/dev/urandom", "rb");
    si (fons == NIHIL)
    {
        redde FALSUM;
    }
    si (fread(effusio, I, (size_t)mensura, fons) != (size_t)mensura)
    {
        fclose(fons);
        redde FALSUM;
    }
    fclose(fons);
    redde VERUM;
}

/* nanoid configurabile: larva + abiectio (contractus in moneta.h) */
b32
moneta_nanoid_alphabeto (
             character* effusio,
                   i32  longitudo,
    constans character* alphabetum)
{
     i8 octeti[CCLVI];
     i8 visa[CCLVI];
     i8 littera;
    i32 mensura;
    i32 larva;
    i32 haustus;
    i32 scripta;
    i32 locus;
    i32 k;

    si (effusio == NIHIL)
    {
        redde FALSUM;
    }
    effusio[ZEPHYRUM] = '\0';
    si (alphabetum == NIHIL || longitudo == ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* mensura alphabeti; signum bis datum recusatur. Signa distincta
     * non NUL plus 255 esse nequeunt, ergo recusatio mensuram quoque
     * terminat (visa numquam extra CCLVI). */
    memset(visa, ZEPHYRUM, magnitudo(visa));
    per (mensura = ZEPHYRUM; alphabetum[mensura] != '\0'; mensura++)
    {
        littera = (i8)alphabetum[mensura];
        si (visa[littera])
        {
            redde FALSUM;
        }
        visa[littera] = I;
    }
    si (mensura < II)
    {
        redde FALSUM;
    }

    /* larva: minima 2^k - 1 quae indicem ultimum tegit; 'mensura - I'
     * tutum quia mensura >= II supra (i32 INSIGNATUM) */
    larva = I;
    dum (larva < mensura - I)
    {
        larva = (larva << I) | I;
    }

    /* octeti per haustum ut nanoid: 1.6 x larva x longitudo ad
     * mensuram, ut haustus unus fere sufficiat; summum CCLVI */
    haustus = CCLVI;
    si (longitudo < CCLVI)
    {
        haustus = (XVI * larva * longitudo) / (X * mensura) + I;
        si (haustus > CCLVI)
        {
            haustus = CCLVI;
        }
    }

    scripta = ZEPHYRUM;
    dum (scripta < longitudo)
    {
        si (!moneta_octeti_fortuiti(octeti, haustus))
        {
            effusio[ZEPHYRUM] = '\0';
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < haustus && scripta < longitudo; k++)
        {
            locus = (i32)octeti[k] & larva;
            si (locus < mensura)
            {
                effusio[scripta] = alphabetum[locus];
                scripta++;
            }
        }
    }
    effusio[longitudo] = '\0';
    redde VERUM;
}

b32
moneta_nanoid (
    character* effusio)
{
    redde moneta_nanoid_alphabeto(effusio, MONETA_NANOID_LONGITUDO,
                                  MONETA_NANOID_ALPHABETUM);
}

b32
moneta_nanoid_plagulae (
    character* effusio)
{
    redde moneta_nanoid_alphabeto(effusio,
                                  MONETA_NANOID_PLAGULAE_LONGITUDO,
                                  MONETA_NANOID_PLAGULAE_ALPHABETUM);
}
