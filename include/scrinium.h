/* scrinium.h - scrinium (arca documentorum): involucrum Latinum
 * SQLite + cursor migrationum + ULID
 *
 * NOMEN NONDUM SIGILLATUM (tabularium-gradus.md phase A). SQLite
 * vendicatum (vendor/sqlite3.c, compilatum laxum in obiecto proprio,
 * ut libc tractatum - visio §II). Vexilla decisa ad tempus
 * vendicationis: FTS5 INCLUSUM, THREADSAFE=0 (disciplina scriptoris
 * unius - lex domus), DQS=0, sine extensionibus onerabilibus.
 *
 * DISCIPLINA SCRIPTORIS UNIUS: processus unus scribit; WAL +
 * busy_timeout lectores paralleles tolerant. Pragmata ad aperturam:
 * journal_mode=WAL, busy_timeout=5000, synchronous=NORMAL,
 * foreign_keys=ON (exemplar libraries.ts sqliteOpen).
 *
 * MIGRATIONES: solum-appende, indice numeratae, in tabula
 * _migrationes memoratae, sine migrationibus deorsum - historia
 * immutabilis (exemplar libraries.ts sqliteMigrate).
 *
 * ULID: identitas ORDINUM (creatione-ordinata, 26 characteres
 * Crockford base32, monotona intra ms eundem). Identitas CONTENTI
 * est sigillum (sigillum.h) - genera duo identitatis, quodque suo
 * muneri (gradus doc, messis bun-libraries).
 *
 * Usus:
 *   Scrinium* s = scrinium_aperire(piscina, "build/probatio.db");
 *   scrinium_migrare(s, MIGRATIONES, III);
 *   ScriniumEnuntiatum* e = scrinium_praeparare(s,
 *       "INSERT INTO res (id, titulus) VALUES (?, ?)");
 *   scrinium_ligare_textum(e, I, id_chorda);
 *   scrinium_ligare_textum(e, II, titulus_chorda);
 *   dum (scrinium_gradi(e) == SCRINIUM_ORDO) { ... }
 *   scrinium_finire(e);
 *   scrinium_claudere(s);
 */

#ifndef SCRINIUM_H
#define SCRINIUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

nomen structura Scrinium Scrinium;
nomen structura ScriniumEnuntiatum ScriniumEnuntiatum;

/* fructus scrinium_gradi */
#define SCRINIUM_ORDO    1   /* ordo praesens - columnae legendae */
#define SCRINIUM_FACTUM  2   /* consummatum - nihil amplius */
#define SCRINIUM_ERROR   0   /* vitium - scrinium_error consule */

/* aperire (via ":memoria:" = in memoria); NIHIL si fractum.
 * Piscina structuras involucri possidet (vita scrinii). */
Scrinium*
scrinium_aperire (
    Piscina*            piscina,
    constans character* via);

vacuum
scrinium_claudere (
    Scrinium* scrinium);

/* sql sine fructibus (CREATE/INSERT/BEGIN...); VERUM = successus */
b32
scrinium_exsequi (
    Scrinium*           scrinium,
    constans character* sql);

/* nuntius erroris ultimi (litterae SQLite - vita scrinii) */
constans character*
scrinium_error (
    constans Scrinium* scrinium);

s64
scrinium_ultimum_id (
    constans Scrinium* scrinium);

/* enuntiata praeparata (indices ligaminum 1-basati) */
ScriniumEnuntiatum*
scrinium_praeparare (
    Scrinium*           scrinium,
    constans character* sql);

b32
scrinium_ligare_textum (
    ScriniumEnuntiatum* enuntiatum,
    integer             index,
    chorda              textus);

b32
scrinium_ligare_numerum (
    ScriniumEnuntiatum* enuntiatum,
    integer             index,
    s64                 numerus);

b32
scrinium_ligare_nihil (
    ScriniumEnuntiatum* enuntiatum,
    integer             index);

/* massa = datum binarium (BLOB; chorda datum binarium fert).
 * massa vacua (mensura 0) = blobum vacuum, non NULL. */
b32
scrinium_ligare_massam (
    ScriniumEnuntiatum* enuntiatum,
    integer             index,
    chorda              massa);

integer
scrinium_gradi (
    ScriniumEnuntiatum* enuntiatum);

/* columnae 0-basatae; textus in piscinam COPIATUR (chorda stabilis
 * post gradum sequentem) */
chorda
scrinium_columna_textus (
    ScriniumEnuntiatum* enuntiatum,
    integer             index,
    Piscina*            piscina);

s64
scrinium_columna_numerus (
    ScriniumEnuntiatum* enuntiatum,
    integer             index);

/* blobum in piscinam copiatur; blobum vacuum = chorda vacua
 * (datum NIHIL, mensura 0) - a NULL non distinguitur, vocator
 * scrinium_columna_nihil_est ante consulat si refert */
chorda
scrinium_columna_massa (
    ScriniumEnuntiatum* enuntiatum,
    integer             index,
    Piscina*            piscina);

b32
scrinium_columna_nihil_est (
    ScriniumEnuntiatum* enuntiatum,
    integer             index);

/* retexere: reuti (ligamina purgata) */
vacuum
scrinium_retexere (
    ScriniumEnuntiatum* enuntiatum);

vacuum
scrinium_finire (
    ScriniumEnuntiatum* enuntiatum);

/* transactiones (trias tenuis super exsequi - gesta K1: via
 * scripturae eventus totam involvit) */
b32
scrinium_incipere (
    Scrinium* scrinium);

b32
scrinium_committere (
    Scrinium* scrinium);

b32
scrinium_revolvere (
    Scrinium* scrinium);

/* migrationes solum-appende: quaeque semel, transactione una;
 * VERUM = successus (etiam si nihil novum) */
b32
scrinium_migrare (
    Scrinium*                     scrinium,
    constans character* constans* migrationes,
    integer                       numerus);

#define SCRINIUM_ULID_MENSURA 27   /* 26 characteres + terminator */

/* ULID: 26 characteres Crockford base32 in effusio (>= 27 octeti);
 * monotona intra ms eundem (fasciculus processus) */
vacuum
scrinium_ulid (
    character* effusio);

#endif /* SCRINIUM_H */
