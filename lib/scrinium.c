/* scrinium.c - involucrum Latinum SQLite - vide scrinium.h */

/* sqlite3.h ANTE latina.h: numeri Romani (C = 100...) parametros
 * capitis vendicati aliter vorant ("char C" -> "char 100") -
 * exemplar imago.c/stb_image */
#include "../vendor/sqlite3.h"

#include "scrinium.h"

#include <string.h>
#include <stdio.h>
#include <sys/time.h>   /* gettimeofday pro ULID (exemplar tcp_posix) */

structura Scrinium {
    sqlite3* ansa;
};

structura ScriniumEnuntiatum {
    sqlite3_stmt* ansa;
    Scrinium*     scrinium;
};

Scrinium*
scrinium_aperire (Piscina* piscina, constans character* via)
{
    Scrinium* s;
    sqlite3* ansa = NIHIL;

    si (sqlite3_open(via, &ansa) != SQLITE_OK)
    {
        si (ansa != NIHIL)
        {
            sqlite3_close(ansa);
        }
        redde NIHIL;
    }
    s = (Scrinium*)piscina_allocare(piscina, magnitudo(Scrinium));
    si (s == NIHIL)
    {
        sqlite3_close(ansa);
        redde NIHIL;
    }
    s->ansa = ansa;
    /* pragmata domus (libraries.ts sqliteOpen): WAL pro
     * lectoribus paralleliis, NORMAL tutum sub WAL */
    (vacuum)scrinium_exsequi(s, "PRAGMA journal_mode = WAL");
    (vacuum)scrinium_exsequi(s, "PRAGMA busy_timeout = 5000");
    (vacuum)scrinium_exsequi(s, "PRAGMA synchronous = NORMAL");
    (vacuum)scrinium_exsequi(s, "PRAGMA foreign_keys = ON");
    redde s;
}

vacuum
scrinium_claudere (Scrinium* scrinium)
{
    si (scrinium == NIHIL || scrinium->ansa == NIHIL)
    {
        redde;
    }
    sqlite3_close(scrinium->ansa);
    scrinium->ansa = NIHIL;
}

b32
scrinium_exsequi (Scrinium* scrinium, constans character* sql)
{
    redde sqlite3_exec(scrinium->ansa, sql, NIHIL, NIHIL, NIHIL)
        == SQLITE_OK ? VERUM : FALSUM;
}

constans character*
scrinium_error (constans Scrinium* scrinium)
{
    redde sqlite3_errmsg(scrinium->ansa);
}

s64
scrinium_ultimum_id (constans Scrinium* scrinium)
{
    redde (s64)sqlite3_last_insert_rowid(scrinium->ansa);
}

ScriniumEnuntiatum*
scrinium_praeparare (Scrinium* scrinium, constans character* sql)
{
    /* enuntiata effimera sunt - allocatio sqlite ipsius sufficit;
     * involucrum parvum ex piscina non petimus ne vita piscinae
     * enuntiatis brevibus ligetur. Allocatio statica hic non licet
     * (enuntiata plura simul) - malloc sqlite intus iam adhibetur,
     * ergo sqlite3_malloc pro involucro quoque: mundus unus. */
    ScriniumEnuntiatum* e;
    sqlite3_stmt* ansa = NIHIL;

    si (sqlite3_prepare_v2(scrinium->ansa, sql, -1, &ansa, NIHIL)
        != SQLITE_OK)
    {
        redde NIHIL;
    }
    e = (ScriniumEnuntiatum*)sqlite3_malloc(
        (int)magnitudo(ScriniumEnuntiatum));
    si (e == NIHIL)
    {
        sqlite3_finalize(ansa);
        redde NIHIL;
    }
    e->ansa = ansa;
    e->scrinium = scrinium;
    redde e;
}

b32
scrinium_ligare_textum (ScriniumEnuntiatum* enuntiatum, integer index,
    chorda textus)
{
    /* chorda NON null-terminata: (index, mensura) recte transfertur;
     * SQLITE_TRANSIENT = sqlite copiat (chorda effimera tuta) */
    redde sqlite3_bind_text(enuntiatum->ansa, index,
        (constans character*)textus.datum, (int)textus.mensura,
        SQLITE_TRANSIENT) == SQLITE_OK ? VERUM : FALSUM;
}

b32
scrinium_ligare_numerum (ScriniumEnuntiatum* enuntiatum, integer index,
    s64 numerus)
{
    redde sqlite3_bind_int64(enuntiatum->ansa, index,
        (sqlite3_int64)numerus) == SQLITE_OK ? VERUM : FALSUM;
}

b32
scrinium_ligare_nihil (ScriniumEnuntiatum* enuntiatum, integer index)
{
    redde sqlite3_bind_null(enuntiatum->ansa, index) == SQLITE_OK
        ? VERUM : FALSUM;
}

integer
scrinium_gradi (ScriniumEnuntiatum* enuntiatum)
{
    integer fructus = sqlite3_step(enuntiatum->ansa);

    si (fructus == SQLITE_ROW)
    {
        redde SCRINIUM_ORDO;
    }
    si (fructus == SQLITE_DONE)
    {
        redde SCRINIUM_FACTUM;
    }
    redde SCRINIUM_ERROR;
}

chorda
scrinium_columna_textus (ScriniumEnuntiatum* enuntiatum, integer index,
    Piscina* piscina)
{
    chorda fructus;
    constans insignatus character* textus =
        sqlite3_column_text(enuntiatum->ansa, index);
    integer mensura = sqlite3_column_bytes(enuntiatum->ansa, index);

    fructus.datum = NIHIL;
    fructus.mensura = ZEPHYRUM;
    si (textus == NIHIL || mensura <= 0)
    {
        redde fructus;
    }
    fructus.datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)mensura);
    si (fructus.datum == NIHIL)
    {
        redde fructus;
    }
    memcpy(fructus.datum, textus, (memoriae_index)mensura);
    fructus.mensura = (i32)mensura;
    redde fructus;
}

s64
scrinium_columna_numerus (ScriniumEnuntiatum* enuntiatum,
    integer index)
{
    redde (s64)sqlite3_column_int64(enuntiatum->ansa, index);
}

b32
scrinium_columna_nihil_est (ScriniumEnuntiatum* enuntiatum,
    integer index)
{
    redde sqlite3_column_type(enuntiatum->ansa, index) == SQLITE_NULL
        ? VERUM : FALSUM;
}

vacuum
scrinium_retexere (ScriniumEnuntiatum* enuntiatum)
{
    (vacuum)sqlite3_reset(enuntiatum->ansa);
    (vacuum)sqlite3_clear_bindings(enuntiatum->ansa);
}

vacuum
scrinium_finire (ScriniumEnuntiatum* enuntiatum)
{
    si (enuntiatum == NIHIL)
    {
        redde;
    }
    sqlite3_finalize(enuntiatum->ansa);
    sqlite3_free(enuntiatum);
}

/* trias transactionum (gesta K1) - tenuis super exsequi */
b32
scrinium_incipere (Scrinium* scrinium)
{
    redde scrinium_exsequi(scrinium, "BEGIN");
}

b32
scrinium_committere (Scrinium* scrinium)
{
    redde scrinium_exsequi(scrinium, "COMMIT");
}

b32
scrinium_revolvere (Scrinium* scrinium)
{
    redde scrinium_exsequi(scrinium, "ROLLBACK");
}

b32
scrinium_migrare (Scrinium* scrinium,
    constans character* constans* migrationes, integer numerus)
{
    s64 initium = ZEPHYRUM;
    integer k;

    si (!scrinium_exsequi(scrinium,
            "CREATE TABLE IF NOT EXISTS _migrationes ("
            "  versio INTEGER PRIMARY KEY,"
            "  applicata TEXT NOT NULL DEFAULT "
            "    (strftime('%Y-%m-%dT%H:%M:%fZ', 'now'))"
            ")"))
    {
        redde FALSUM;
    }
    {
        ScriniumEnuntiatum* e = scrinium_praeparare(scrinium,
            "SELECT COALESCE(MAX(versio), -1) FROM _migrationes");

        si (e == NIHIL)
        {
            redde FALSUM;
        }
        si (scrinium_gradi(e) != SCRINIUM_ORDO)
        {
            scrinium_finire(e);
            redde FALSUM;
        }
        initium = scrinium_columna_numerus(e, 0) + I;
        scrinium_finire(e);
    }
    si (initium >= (s64)numerus)
    {
        redde VERUM;   /* nihil novum */
    }
    si (!scrinium_exsequi(scrinium, "BEGIN"))
    {
        redde FALSUM;
    }
    per (k = (integer)initium; k < numerus; k++)
    {
        ScriniumEnuntiatum* e;

        si (!scrinium_exsequi(scrinium, migrationes[k]))
        {
            (vacuum)scrinium_exsequi(scrinium, "ROLLBACK");
            redde FALSUM;
        }
        e = scrinium_praeparare(scrinium,
            "INSERT INTO _migrationes (versio) VALUES (?)");
        si (e == NIHIL || !scrinium_ligare_numerum(e, I, (s64)k)
            || scrinium_gradi(e) != SCRINIUM_FACTUM)
        {
            scrinium_finire(e);
            (vacuum)scrinium_exsequi(scrinium, "ROLLBACK");
            redde FALSUM;
        }
        scrinium_finire(e);
    }
    redde scrinium_exsequi(scrinium, "COMMIT");
}

/* ==================================================
 * ULID (exemplar libraries.ts: monotona intra ms)
 * ================================================== */

hic_manens constans character ULID_LITTERAE[] =
    "0123456789ABCDEFGHJKMNPQRSTVWXYZ";

hic_manens i64 _ulid_tempus_ultimum = 0;
hic_manens i32 _ulid_fortuita[16];

/* octeti fortuiti ex /dev/urandom (stdio purum - semita POSIX sed
 * mechanismus vulgaris); in defectu, miscela temporis (deterioris
 * qualitatis sed numquam duplicata intra processum ob monotoniam) */
hic_manens vacuum
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
        i64 semen = _ulid_tempus_ultimum ^ (i64)0x9e3779b97f4a7c15ULL;

        per (k = ZEPHYRUM; k < mensura; k++)
        {
            semen = semen * (i64)6364136223846793005ULL
                + (i64)1442695040888963407ULL;
            effusio[k] = (i8)((semen >> 33) & 0xffUL);
        }
    }
}

vacuum
scrinium_ulid (character* effusio)
{
    structura timeval nunc_tv;
    i64 nunc;
    integer k;

    gettimeofday(&nunc_tv, NIHIL);
    nunc = (i64)nunc_tv.tv_sec * 1000
         + (i64)nunc_tv.tv_usec / 1000;

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
        i64 t = _ulid_tempus_ultimum;

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
