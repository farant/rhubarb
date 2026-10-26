/* scrinium.c - involucrum Latinum SQLite - vide scrinium.h */

/* sqlite3.h ANTE latina.h: numeri Romani (C = 100...) parametros
 * capitis vendicati aliter vorant ("char C" -> "char 100") -
 * exemplar imago.c/stb_image */
#include "../vendor/sqlite3.h"

#include "scrinium.h"
#include "lectiones.h"
#include "moneta.h"

#include <string.h>
#include <stdio.h>

structura Scrinium {
    sqlite3* ansa;
};

structura ScriniumEnuntiatum {
    sqlite3_stmt* ansa;
        Scrinium* scrinium;
};

structura ScriniumMassa {
    sqlite3_blob* ansa;
};


/* ==================================================
 * VFS VESTIGII (iudicium per librum solum, nota …NS2M): sqlite IO
 * suum CRUDUM agit - liber lectionum (lectiones) eius lectiones et
 * scripturas non videbat. VFS involvens "rhubarb-vestigium" omnia ad
 * VFS ordinarium (unix) transmittit et notat: lectio prima (xRead,
 * xFetch) L, scriptura prima aut truncatio S, deletio S, xShmMap
 * primum S pro '<db>-shm' (WAL memoriam communem per mmap scribit,
 * numquam per xWrite). Plagulae sine nomine (temporariae) non
 * notantur; xAccess non notatur (sqlite diaria calida assidue
 * quaerit - absentia eorum ingressus non est). Sine FABRICA_LECTIONES
 * lectiones_notare nihil agit. Structura VFS ordinarii copiatur:
 * functiones unix praeter xOpen/xDelete argumentum VFS non adhibent.
 * ================================================== */

nomen structura {
           sqlite3_file  basis;     /* pMethods = &_methodi_vestigii */
           sqlite3_file* verum;     /* plagula VFS ordinarii sequens */
     constans character* via;       /* zName (vivit usque ad xClose) */
                    b32  lectum;
                    b32  scriptum;
                    b32  communis;  /* -shm notatum */
} FilumVestigii;

interior vacuum
_notare_semel (
                    b32* signum,
            LectioGenus  genus,
     constans character* via)
{
    si (!*signum && via != NIHIL)
    {
        *signum = VERUM;
        lectiones_notare(genus, via);
    }
}

interior sqlite3_file*
_verum (
    sqlite3_file* f)
{
    redde ((FilumVestigii*)f)->verum;
}

interior integer
_fv_claudere (
    sqlite3_file* f)
{
    redde _verum(f)->pMethods->xClose(_verum(f));
}

interior integer
_fv_legere (
      sqlite3_file* f,
            vacuum* capsa,
           integer  n,
     sqlite3_int64  offset)
{
    FilumVestigii* p = (FilumVestigii*)f;

    _notare_semel(&p->lectum, LECTIO_LEGIT, p->via);
    redde p->verum->pMethods->xRead(p->verum, capsa, n, offset);
}

interior integer
_fv_scribere (
       sqlite3_file* f,
    constans vacuum* octeti,
            integer  n,
      sqlite3_int64  offset)
{
    FilumVestigii* p = (FilumVestigii*)f;

    _notare_semel(&p->scriptum, LECTIO_SCRIPSIT, p->via);
    redde p->verum->pMethods->xWrite(p->verum, octeti, n, offset);
}

interior integer
_fv_truncare (
     sqlite3_file* f,
    sqlite3_int64  mensura)
{
    FilumVestigii* p = (FilumVestigii*)f;

    _notare_semel(&p->scriptum, LECTIO_SCRIPSIT, p->via);
    redde p->verum->pMethods->xTruncate(p->verum, mensura);
}

interior integer
_fv_firmare (
    sqlite3_file* f,
         integer  vexilla)
{
    redde _verum(f)->pMethods->xSync(_verum(f), vexilla);
}

interior integer
_fv_mensura (
     sqlite3_file* f,
    sqlite3_int64* mensura)
{
    redde _verum(f)->pMethods->xFileSize(_verum(f), mensura);
}

interior integer
_fv_claustrum (
    sqlite3_file* f,
         integer  gradus)
{
    redde _verum(f)->pMethods->xLock(_verum(f), gradus);
}

interior integer
_fv_reserare (
    sqlite3_file* f,
         integer  gradus)
{
    redde _verum(f)->pMethods->xUnlock(_verum(f), gradus);
}

interior integer
_fv_reservatum (
    sqlite3_file* f,
         integer* fructus)
{
    redde _verum(f)->pMethods->xCheckReservedLock(_verum(f), fructus);
}

interior integer
_fv_imperium (
    sqlite3_file* f,
         integer  mandatum,
          vacuum* argumentum)
{
    redde _verum(f)->pMethods->xFileControl(_verum(f), mandatum,
        argumentum);
}

interior integer
_fv_sector (
    sqlite3_file* f)
{
    redde _verum(f)->pMethods->xSectorSize(_verum(f));
}

interior integer
_fv_proprietates (
    sqlite3_file* f)
{
    redde _verum(f)->pMethods->xDeviceCharacteristics(_verum(f));
}

/* memoria communis WAL: '<db>-shm' a VFS ordinario ipso aperitur et
 * per mmap scribitur - nota S semel, ante mappam */
interior integer
_fv_communem_mappare (
            sqlite3_file*  f,
                 integer   pagina,
                 integer   mensura_paginae,
                 integer   extendere,
        vacuum volatilis** mappa)
{
    FilumVestigii* p = (FilumVestigii*)f;

    si (!p->communis && p->via != NIHIL)
    {
        character via_communis[MMXLVIII];
           size_t longitudo = strlen(p->via);

        p->communis = VERUM;
        si (longitudo + V < magnitudo(via_communis))
        {
            memcpy(via_communis, p->via, longitudo);
            memcpy(via_communis + longitudo, "-shm", V);
            lectiones_notare(LECTIO_SCRIPSIT, via_communis);
        }
    }
    redde p->verum->pMethods->xShmMap(p->verum, pagina, mensura_paginae,
        extendere, mappa);
}

interior integer
_fv_communem_claudere (
    sqlite3_file* f,
         integer  offset,
         integer  n,
         integer  vexilla)
{
    redde _verum(f)->pMethods->xShmLock(_verum(f), offset, n, vexilla);
}

interior vacuum
_fv_communem_saepire (
    sqlite3_file* f)
{
    _verum(f)->pMethods->xShmBarrier(_verum(f));
}

interior integer
_fv_communem_solvere (
    sqlite3_file* f,
         integer  delere)
{
    redde _verum(f)->pMethods->xShmUnmap(_verum(f), delere);
}

interior integer
_fv_capere (
      sqlite3_file*  f,
     sqlite3_int64   offset,
           integer   n,
            vacuum** pp)
{
    FilumVestigii* p = (FilumVestigii*)f;

    _notare_semel(&p->lectum, LECTIO_LEGIT, p->via);
    redde p->verum->pMethods->xFetch(p->verum, offset, n, pp);
}

interior integer
_fv_dimittere (
     sqlite3_file* f,
    sqlite3_int64  offset,
           vacuum* pagina)
{
    redde _verum(f)->pMethods->xUnfetch(_verum(f), offset, pagina);
}

hic_manens constans sqlite3_io_methods _methodi_vestigii = {
    III,
    _fv_claudere,
    _fv_legere,
    _fv_scribere,
    _fv_truncare,
    _fv_firmare,
    _fv_mensura,
    _fv_claustrum,
    _fv_reserare,
    _fv_reservatum,
    _fv_imperium,
    _fv_sector,
    _fv_proprietates,
    _fv_communem_mappare,
    _fv_communem_claudere,
    _fv_communem_saepire,
    _fv_communem_solvere,
    _fv_capere,
    _fv_dimittere
};

interior integer
_systema_aperire (
         sqlite3_vfs* systema,
    sqlite3_filename  via,
        sqlite3_file* f,
             integer  vexilla,
             integer* vexilla_out)
{
    FilumVestigii* p      = (FilumVestigii*)f;
      sqlite3_vfs* basis  = (sqlite3_vfs*)systema->pAppData;
          integer  rc;

    p->basis.pMethods  = NIHIL;
    p->verum           = (sqlite3_file*)(vacuum*)((i8*)(vacuum*)p
        + magnitudo(FilumVestigii));
    p->verum->pMethods = NIHIL;
    p->via = via;
    p->lectum = FALSUM;
    p->scriptum = FALSUM;
    p->communis = FALSUM;
    rc = basis->xOpen(basis, via, p->verum, vexilla, vexilla_out);
    /* methodi solum si plagula vera eas habet: sqlite tum xClose
     * vocat, etiam post defectum */
    si (p->verum->pMethods != NIHIL)
    {
        p->basis.pMethods = &_methodi_vestigii;
    }
    redde rc;
}

interior integer
_systema_delere (
            sqlite3_vfs* systema,
     constans character* via,
                integer  directorium_firmare)
{
    sqlite3_vfs* basis = (sqlite3_vfs*)systema->pAppData;

    lectiones_notare(LECTIO_SCRIPSIT, via);
    redde basis->xDelete(basis, via, directorium_firmare);
}

hic_manens sqlite3_vfs _systema_vestigii;
hic_manens         b32 _systema_paratum = FALSUM;

/* nomen VFS vestigii, semel inscripti; NIHIL = VFS ordinarius (si
 * inscriptio deficit, sqlite ut olim) */
interior constans character*
_systema_vestigii_nomen (vacuum)
{
    sqlite3_vfs* basis;

    si (!_systema_paratum)
    {
        basis = sqlite3_vfs_find(NIHIL);
        si (basis == NIHIL)
        {
            redde NIHIL;
        }
        _systema_vestigii           = *basis;
        _systema_vestigii.pNext     = NIHIL;
        _systema_vestigii.zName     = "rhubarb-vestigium";
        _systema_vestigii.pAppData  = basis;
        _systema_vestigii.szOsFile  = (integer)magnitudo(FilumVestigii)
            + basis->szOsFile;
        _systema_vestigii.xOpen    = _systema_aperire;
        _systema_vestigii.xDelete  = _systema_delere;
        si (sqlite3_vfs_register(&_systema_vestigii, 0) != SQLITE_OK)
        {
            redde NIHIL;
        }
        _systema_paratum = VERUM;
    }
    redde _systema_vestigii.zName;
}

Scrinium*
scrinium_aperire (
               Piscina* piscina,
    constans character* via)
{
    redde scrinium_aperire_cum(piscina, via, NIHIL);
}

Scrinium*
scrinium_aperire_cum (
                      Piscina* piscina,
           constans character* via,
    constans ScriniumOptiones* optiones)
{
    Scrinium* s;
     sqlite3* ansa = NIHIL;

    /* VFS vestigii: lectiones et scripturae sqlite in libro
     * (sqlite3_open eadem vexilla: READWRITE | CREATE) */
    si (sqlite3_open_v2(via, &ansa, SQLITE_OPEN_READWRITE
        | SQLITE_OPEN_CREATE, _systema_vestigii_nomen()) != SQLITE_OK)
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
    (vacuum)scrinium_exsequi(s, (optiones
        && optiones->synchronus_plenus)
        ? "PRAGMA synchronous = FULL" : "PRAGMA synchronous = NORMAL");
    (vacuum)scrinium_exsequi(s, "PRAGMA foreign_keys = ON");
    redde s;
}

vacuum
scrinium_claudere (
    Scrinium* scrinium)
{
    si (scrinium == NIHIL || scrinium->ansa == NIHIL)
    {
        redde;
    }
    sqlite3_close(scrinium->ansa);
    scrinium->ansa = NIHIL;
}

b32
scrinium_exsequi (
              Scrinium* scrinium,
    constans character* sql)
{
    redde sqlite3_exec(scrinium->ansa, sql, NIHIL, NIHIL, NIHIL)
        == SQLITE_OK ? VERUM : FALSUM;
}

constans character*
scrinium_error (
    constans Scrinium* scrinium)
{
    redde sqlite3_errmsg(scrinium->ansa);
}

s64
scrinium_ultimum_id (
    constans Scrinium* scrinium)
{
    redde (s64)sqlite3_last_insert_rowid(scrinium->ansa);
}

ScriniumEnuntiatum*
scrinium_praeparare (
              Scrinium* scrinium,
    constans character* sql)
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
    e->ansa      = ansa;
    e->scrinium  = scrinium;
    redde e;
}

b32
scrinium_ligare_textum (
    ScriniumEnuntiatum* enuntiatum,
               integer  index,
                chorda  textus)
{
    /* chorda NON null-terminata: (index, mensura) recte transfertur;
     * SQLITE_TRANSIENT = sqlite copiat (chorda effimera tuta) */
    redde sqlite3_bind_text(enuntiatum->ansa, index,
        (constans character*)textus.datum, (int)textus.mensura,
        SQLITE_TRANSIENT) == SQLITE_OK ? VERUM : FALSUM;
}

b32
scrinium_ligare_numerum (
    ScriniumEnuntiatum* enuntiatum,
               integer  index,
                   s64  numerus)
{
    redde sqlite3_bind_int64(enuntiatum->ansa, index,
        (sqlite3_int64)numerus) == SQLITE_OK ? VERUM : FALSUM;
}

b32
scrinium_ligare_nihil (
    ScriniumEnuntiatum* enuntiatum,
               integer  index)
{
    redde sqlite3_bind_null(enuntiatum->ansa, index) == SQLITE_OK
        ? VERUM : FALSUM;
}

b32
scrinium_ligare_massam (
    ScriniumEnuntiatum* enuntiatum,
               integer  index,
                chorda  massa)
{
    /* datum NIHIL sqlite ut NULL interpretaretur - massa vacua
     * blobum vacuum VERUM esse debet, ergo zeroblob(0) */
    si (massa.datum == NIHIL || massa.mensura == ZEPHYRUM)
    {
        redde sqlite3_bind_zeroblob(enuntiatum->ansa, index, 0)
            == SQLITE_OK ? VERUM : FALSUM;
    }
    redde sqlite3_bind_blob(enuntiatum->ansa, index,
        (constans vacuum*)massa.datum, (int)massa.mensura,
        SQLITE_TRANSIENT) == SQLITE_OK ? VERUM : FALSUM;
}

b32
scrinium_ligare_massam_nullam (
    ScriniumEnuntiatum* enuntiatum,
               integer  index,
                   i32  mensura)
{
    redde sqlite3_bind_zeroblob(enuntiatum->ansa, index, (int)mensura)
        == SQLITE_OK ? VERUM : FALSUM;
}

integer
scrinium_gradi (
    ScriniumEnuntiatum* enuntiatum)
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
scrinium_columna_textus (
    ScriniumEnuntiatum* enuntiatum,
               integer  index,
               Piscina* piscina)
{
                           chorda  fructus;
    constans insignatus character* textus =
        sqlite3_column_text(enuntiatum->ansa, index);
    integer mensura = sqlite3_column_bytes(enuntiatum->ansa, index);

    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;
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
scrinium_columna_numerus (
    ScriniumEnuntiatum* enuntiatum,
               integer  index)
{
    redde (s64)sqlite3_column_int64(enuntiatum->ansa, index);
}

chorda
scrinium_columna_massa (
    ScriniumEnuntiatum* enuntiatum,
               integer  index,
               Piscina* piscina)
{
             chorda  fructus;
    constans vacuum* datum =
        sqlite3_column_blob(enuntiatum->ansa, index);
    integer mensura = sqlite3_column_bytes(enuntiatum->ansa, index);

    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;
    si (datum == NIHIL || mensura <= 0)
    {
        redde fructus;
    }
    fructus.datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)mensura);
    si (fructus.datum == NIHIL)
    {
        redde fructus;
    }
    memcpy(fructus.datum, datum, (memoriae_index)mensura);
    fructus.mensura = (i32)mensura;
    redde fructus;
}

b32
scrinium_columna_nihil_est (
    ScriniumEnuntiatum* enuntiatum,
               integer  index)
{
    redde sqlite3_column_type(enuntiatum->ansa, index) == SQLITE_NULL
        ? VERUM : FALSUM;
}

vacuum
scrinium_retexere (
    ScriniumEnuntiatum* enuntiatum)
{
    (vacuum)sqlite3_reset(enuntiatum->ansa);
    (vacuum)sqlite3_clear_bindings(enuntiatum->ansa);
}

vacuum
scrinium_finire (
    ScriniumEnuntiatum* enuntiatum)
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
scrinium_incipere (
    Scrinium* scrinium)
{
    redde scrinium_exsequi(scrinium, "BEGIN");
}

b32
scrinium_committere (
    Scrinium* scrinium)
{
    redde scrinium_exsequi(scrinium, "COMMIT");
}

b32
scrinium_revolvere (
    Scrinium* scrinium)
{
    redde scrinium_exsequi(scrinium, "ROLLBACK");
}

b32
scrinium_migrare (
    Scrinium* scrinium,
    constans character* constans* migrationes,
    integer numerus)
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
        si (   e                 == NIHIL
            || !scrinium_ligare_numerum(e, I, (s64)k)
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

/* massa per partes (sqlite3_blob) - involucrum ex sqlite3_malloc ut
 * enuntiata (vita brevis, piscina non ligata) */
ScriniumMassa*
scrinium_massam_aperire (
              Scrinium* scrinium,
    constans character* tabula,
    constans character* columna,
                   s64  ordo,
                   b32  scribere)
{
    ScriniumMassa* m;
     sqlite3_blob* ansa = NIHIL;

    si (sqlite3_blob_open(scrinium->ansa, "main", tabula, columna,
        (sqlite3_int64)ordo, scribere ? I : 0, &ansa) != SQLITE_OK)
    {
        si (ansa != NIHIL)
        {
            sqlite3_blob_close(ansa);
        }
        redde NIHIL;
    }
    m = (ScriniumMassa*)sqlite3_malloc((int)magnitudo(ScriniumMassa));
    si (m == NIHIL)
    {
        sqlite3_blob_close(ansa);
        redde NIHIL;
    }
    m->ansa = ansa;
    redde m;
}

i32
scrinium_massae_mensura (
    ScriniumMassa* massa)
{
    redde (i32)sqlite3_blob_bytes(massa->ansa);
}

b32
scrinium_massam_legere (
    ScriniumMassa* massa,
               i8* capsa,
              i32  n,
              i32  offset)
{
    redde sqlite3_blob_read(massa->ansa, capsa, (int)n, (int)offset)
        == SQLITE_OK ? VERUM : FALSUM;
}

b32
scrinium_massam_scribere (
    ScriniumMassa* massa,
      constans i8* octeti,
              i32  n,
              i32  offset)
{
    redde sqlite3_blob_write(massa->ansa, octeti, (int)n, (int)offset)
        == SQLITE_OK ? VERUM : FALSUM;
}

vacuum
scrinium_massam_claudere (
    ScriniumMassa* massa)
{
    si (massa == NIHIL)
    {
        redde;
    }
    sqlite3_blob_close(massa->ansa);
    sqlite3_free(massa);
}

/* ULID: delegatio ad monetam (cusio una repositorii - lib/moneta.c;
 * implementatio illuc mota 2026-07-21, sqlite nimis grave pro
 * instrumentis solum cudentibus) */
vacuum
scrinium_ulid (
    character* effusio)
{
    moneta_ulid(effusio);
}
