/* probatio_scrinium.c - Probationes Scrinii (SQLite vendicatum +
 * involucrum + migrationes + ULID + FTS5) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "scrinium.h"
#include <stdio.h>
#include <string.h>

interior chorda
_ch (
    constans character* literis)
{
    chorda c;
    unio { constans character* l; i8* m; } u;

    u.l        = literis;
    c.datum    = u.m;
    c.mensura  = (i32)strlen(literis);
    redde c;
}

s32 principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_scrinium", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: aperire in memoria + circuitus fundamentalis
     * ======================================================== */

    {
                  Scrinium* s;
        ScriniumEnuntiatum* e;

        imprimere("\n--- Probans circuitum fundamentalem ---\n");

        s = scrinium_aperire(piscina, ":memory:");
        CREDO_NON_NIHIL (s);
        CREDO_VERUM (scrinium_exsequi(s,
            "CREATE TABLE res (id TEXT PRIMARY KEY, titulus TEXT)"));

        e = scrinium_praeparare(s,
            "INSERT INTO res (id, titulus) VALUES (?, ?)");
        CREDO_NON_NIHIL (e);
        CREDO_VERUM (scrinium_ligare_textum(e, I, _ch("res-1")));
        CREDO_VERUM (scrinium_ligare_textum(e, II,
            _ch("prima res")));
        CREDO_AEQUALIS_I32 ((i32)scrinium_gradi(e),
            (i32)SCRINIUM_FACTUM);
        scrinium_retexere(e);
        CREDO_VERUM (scrinium_ligare_textum(e, I, _ch("res-2")));
        CREDO_VERUM (scrinium_ligare_textum(e, II,
            _ch("secunda res")));
        CREDO_AEQUALIS_I32 ((i32)scrinium_gradi(e),
            (i32)SCRINIUM_FACTUM);
        scrinium_finire(e);

        e = scrinium_praeparare(s,
            "SELECT titulus FROM res ORDER BY id");
        CREDO_NON_NIHIL (e);
        CREDO_AEQUALIS_I32 ((i32)scrinium_gradi(e),
            (i32)SCRINIUM_ORDO);
        {
            chorda t = scrinium_columna_textus(e, 0, piscina);

            CREDO_CHORDA_AEQUALIS_LITERIS (t, "prima res");
        }
        CREDO_AEQUALIS_I32 ((i32)scrinium_gradi(e),
            (i32)SCRINIUM_ORDO);
        CREDO_AEQUALIS_I32 ((i32)scrinium_gradi(e),
            (i32)SCRINIUM_FACTUM);
        scrinium_finire(e);

        /* chorda NON null-terminata: sectio media ligatur recte */
        {
            chorda totum = _ch("XXmediumYY");
            chorda sectio;

            sectio.datum    = totum.datum + II;
            sectio.mensura  = VI;   /* "medium" */
            e = scrinium_praeparare(s,
                "INSERT INTO res (id, titulus) VALUES ('res-3', ?)");
            CREDO_NON_NIHIL (e);
            CREDO_VERUM (scrinium_ligare_textum(e, I, sectio));
            CREDO_AEQUALIS_I32 ((i32)scrinium_gradi(e),
                (i32)SCRINIUM_FACTUM);
            scrinium_finire(e);

            e = scrinium_praeparare(s,
                "SELECT titulus FROM res WHERE id = 'res-3'");
            CREDO_AEQUALIS_I32 ((i32)scrinium_gradi(e),
                (i32)SCRINIUM_ORDO);
            {
                chorda t = scrinium_columna_textus(e, 0, piscina);

                CREDO_CHORDA_AEQUALIS_LITERIS (t, "medium");
            }
            scrinium_finire(e);
        }

        CREDO_VERUM (scrinium_ultimum_id(s) > (s64)ZEPHYRUM);
        scrinium_claudere(s);
    }


    /* ========================================================
     * PROBARE: migrationes solum-appende (idempotentes)
     * ======================================================== */

    {
        hic_manens constans character* constans MIGRATIONES_II[] = {
            "CREATE TABLE a (x INTEGER)",
            "CREATE TABLE b (y INTEGER)"
        };
        hic_manens constans character* constans MIGRATIONES_III[] = {
            "CREATE TABLE a (x INTEGER)",
            "CREATE TABLE b (y INTEGER)",
            "ALTER TABLE a ADD COLUMN z INTEGER"
        };
                  Scrinium* s;
        ScriniumEnuntiatum* e;

        imprimere("\n--- Probans migrationes ---\n");

        s = scrinium_aperire(piscina, ":memory:");
        CREDO_NON_NIHIL (s);
        CREDO_VERUM (scrinium_migrare(s, MIGRATIONES_II, II));
        /* iterum: nihil novum, successus */
        CREDO_VERUM (scrinium_migrare(s, MIGRATIONES_II, II));
        /* tertia additur sola */
        CREDO_VERUM (scrinium_migrare(s, MIGRATIONES_III, III));
        e = scrinium_praeparare(s,
            "SELECT COUNT(*) FROM _migrationes");
        CREDO_AEQUALIS_I32 ((i32)scrinium_gradi(e),
            (i32)SCRINIUM_ORDO);
        CREDO_AEQUALIS_I32 ((i32)scrinium_columna_numerus(e, 0),
            III);
        scrinium_finire(e);
        /* columna nova utenda */
        CREDO_VERUM (scrinium_exsequi(s,
            "INSERT INTO a (x, z) VALUES (1, 2)"));
        scrinium_claudere(s);
    }


    /* ========================================================
     * PROBARE: FTS5 (vexillum vendicationis probatur)
     * ======================================================== */

    {
                  Scrinium* s;
        ScriniumEnuntiatum* e;

        imprimere("\n--- Probans FTS5 ---\n");

        s = scrinium_aperire(piscina, ":memory:");
        CREDO_NON_NIHIL (s);
        CREDO_VERUM (scrinium_exsequi(s,
            "CREATE VIRTUAL TABLE notae USING fts5(titulus, corpus,"
            " tokenize='porter unicode61 remove_diacritics 2')"));
        CREDO_VERUM (scrinium_exsequi(s,
            "INSERT INTO notae VALUES"
            " ('parcum legati', 'corpus macronis plenum postulat"
            " parsuram ex recensere')"));
        CREDO_VERUM (scrinium_exsequi(s,
            "INSERT INTO notae VALUES"
            " ('rung quintum', 'ordo canonicus et fusio fluminis')"));

        /* INVENTUM: porter stemming ANGLICUM est - inflectiones
         * Latinas ('parsuram' accusativus) NON colligit. Idioma
         * corporis Latini = quaestio praefixi ('parsur*') - nota
         * in scrinium worklog; consilium B' tangit. */
        e = scrinium_praeparare(s,
            "SELECT titulus FROM notae WHERE notae MATCH 'parsur*'");
        CREDO_NON_NIHIL (e);
        CREDO_AEQUALIS_I32 ((i32)scrinium_gradi(e),
            (i32)SCRINIUM_ORDO);
        {
            chorda t = scrinium_columna_textus(e, 0, piscina);

            CREDO_CHORDA_AEQUALIS_LITERIS (t, "parcum legati");
        }
        CREDO_AEQUALIS_I32 ((i32)scrinium_gradi(e),
            (i32)SCRINIUM_FACTUM);
        scrinium_finire(e);
        scrinium_claudere(s);
    }


    /* ========================================================
     * PROBARE: plagula vera + WAL + persistentia
     * ======================================================== */

    {
        constans character* via = "build/probatio_scrinium.db";
                  Scrinium* s;
        ScriniumEnuntiatum* e;

        imprimere("\n--- Probans persistentiam WAL ---\n");

        remove(via);
        remove("build/probatio_scrinium.db-wal");
        remove("build/probatio_scrinium.db-shm");

        s = scrinium_aperire(piscina, via);
        CREDO_NON_NIHIL (s);
        /* modus diarii = wal in plagula vera */
        e = scrinium_praeparare(s, "PRAGMA journal_mode");
        CREDO_AEQUALIS_I32 ((i32)scrinium_gradi(e),
            (i32)SCRINIUM_ORDO);
        {
            chorda modus = scrinium_columna_textus(e, 0, piscina);

            CREDO_CHORDA_AEQUALIS_LITERIS (modus, "wal");
        }
        scrinium_finire(e);
        CREDO_VERUM (scrinium_exsequi(s,
            "CREATE TABLE perennis (valor TEXT)"));
        CREDO_VERUM (scrinium_exsequi(s,
            "INSERT INTO perennis VALUES ('supervivo')"));
        scrinium_claudere(s);

        /* reaperire - datum manet */
        s = scrinium_aperire(piscina, via);
        CREDO_NON_NIHIL (s);
        e = scrinium_praeparare(s, "SELECT valor FROM perennis");
        CREDO_AEQUALIS_I32 ((i32)scrinium_gradi(e),
            (i32)SCRINIUM_ORDO);
        {
            chorda t = scrinium_columna_textus(e, 0, piscina);

            CREDO_CHORDA_AEQUALIS_LITERIS (t, "supervivo");
        }
        scrinium_finire(e);
        scrinium_claudere(s);

        remove(via);
        remove("build/probatio_scrinium.db-wal");
        remove("build/probatio_scrinium.db-shm");
    }


    /* ========================================================
     * PROBARE: ULID (mensura, litterae, monotonia)
     * ======================================================== */

    {
        character a[SCRINIUM_ULID_MENSURA];
        character b[SCRINIUM_ULID_MENSURA];
          integer k;

        imprimere("\n--- Probans ULID ---\n");

        scrinium_ulid(a);
        scrinium_ulid(b);
        CREDO_AEQUALIS_I32 ((i32)strlen(a), 26);
        CREDO_AEQUALIS_I32 ((i32)strlen(b), 26);
        /* monotonia: generatio posterior > prior (etiam ms eodem) */
        CREDO_VERUM (strcmp(a, b) < ZEPHYRUM);
        /* litterae Crockford solae */
        per (k = ZEPHYRUM; k < 26; k++)
        {
            CREDO_VERUM (strchr(
                "0123456789ABCDEFGHJKMNPQRSTVWXYZ", a[k]) != NIHIL);
        }
        /* fasciculus: centum ULID, omnes crescentes */
        {
            character prior[SCRINIUM_ULID_MENSURA];
            character praesens[SCRINIUM_ULID_MENSURA];
                  b32 omnes_crescentes = VERUM;

            scrinium_ulid(prior);
            per (k = ZEPHYRUM; k < 100; k++)
            {
                scrinium_ulid(praesens);
                si (strcmp(prior, praesens) >= ZEPHYRUM)
                {
                    omnes_crescentes = FALSUM;
                }
                memcpy(prior, praesens, SCRINIUM_ULID_MENSURA);
            }
            CREDO_VERUM (omnes_crescentes);
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
