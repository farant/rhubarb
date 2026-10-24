/* cives.c - personae rationum (vide include/cives.h)
 *
 * Omnis scriptura: lineae tabularum + actum in transactione VOCATORIS
 * (ianua P3). Ordo: probationes primum (nomen validum, liberum, id
 * notum), deinde scripturae - refutatio nullum actum relinquit.
 */

#include "postulata_posix.h"
#include "cives.h"
#include "json.h"
#include "scrinium.h"

#include <string.h>
#include <sys/time.h>

structura Cives {
     Volumen* volumen;
    Scrinium* scrinium;
          s64  (*nunc_ms)(vacuum);
};

hic_manens constans character* constans SCHEMA[] = {
    "CREATE TABLE IF NOT EXISTS cives"
    " (id TEXT PRIMARY KEY, nomen_usoris TEXT NOT NULL UNIQUE,"
    " titulus TEXT NOT NULL, status INTEGER NOT NULL,"
    " creatus_ms INTEGER NOT NULL)",
    "CREATE TABLE IF NOT EXISTS cives_nomina"
    " (nomen_usoris TEXT PRIMARY KEY, civis TEXT NOT NULL)",
    NIHIL
};


/* ==================================================
 * auxilia
 * ================================================== */

interior s64
_tempus_verum_ms (vacuum)
{
    structura timeval tv;

    si (gettimeofday(&tv, NIHIL) != ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde (s64)tv.tv_sec * M + (s64)(tv.tv_usec / M);
}

interior s64
_nunc (
    Cives* c)
{
    redde c->nunc_ms ? c->nunc_ms() : _tempus_verum_ms();
}

/* numerus ex quaestione cum uno argumento textus; -I = error */
interior s64
_numerus (
                  Cives* c,
     constans character* sql,
                 chorda  textus)
{
     ScriniumEnuntiatum* e = scrinium_praeparare(c->scrinium, sql);
                    s64  n = -I;

    si (!e)
    {
        redde -I;
    }
    scrinium_ligare_textum(e, I, textus);
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        n = scrinium_columna_numerus(e, ZEPHYRUM);
    }
    scrinium_finire(e);
    redde n;
}

/* nomen in cives_nomina iam est (etiam olim) */
interior b32
_nomen_usum (
     Cives* c,
    chorda  nomen_usoris)
{
    redde _numerus(c, "SELECT count(*) FROM cives_nomina"
        " WHERE nomen_usoris = ?", nomen_usoris) != ZEPHYRUM;
}

interior b32
_id_notum (
     Cives* c,
    chorda  id)
{
    redde _numerus(c, "SELECT count(*) FROM cives WHERE id = ?", id)
        == I;
}

/* sententia cum duobus textibus (et numero optionali) - VERUM si
 * consummata */
interior b32
_exsequi (
                  Cives* c,
     constans character* sql,
                 chorda  a,
                 chorda  b)
{
     ScriniumEnuntiatum* e = scrinium_praeparare(c->scrinium, sql);
                    b32  bene;

    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, I, a);
    scrinium_ligare_textum(e, II, b);
    bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
    scrinium_finire(e);
    redde bene;
}

interior b32
_actum (
                  Cives* c,
     constans character* genus,
              JsonValor* d,
                Piscina* p)
{
    redde volumen_actum_appendere(c->volumen, genus, json_scribere(d,
        p))
        > ZEPHYRUM;
}

/* ordo SELECT id, nomen_usoris, titulus, status, creatus_ms */
interior vacuum
_ordinem_legere (
    ScriniumEnuntiatum* e,
                 Civis* x,
               Piscina* p)
{
    x->id            = scrinium_columna_textus(e, ZEPHYRUM, p);
    x->nomen_usoris  = scrinium_columna_textus(e, I, p);
    x->titulus       = scrinium_columna_textus(e, II, p);
    x->status        = (CivisStatus)scrinium_columna_numerus(e, III);
    x->creatus_ms    = scrinium_columna_numerus(e, IV);
}

interior b32
_legere (
                  Cives* c,
     constans character* sql,
                 chorda  clavis,
                  Civis* x,
                Piscina* p)
{
     ScriniumEnuntiatum* e = scrinium_praeparare(c->scrinium, sql);
                    b32  inventum = FALSUM;

    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, I, clavis);
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        _ordinem_legere(e, x, p);
        inventum = VERUM;
    }
    scrinium_finire(e);
    redde inventum;
}


/* ==================================================
 * API
 * ================================================== */

Cives*
cives_aperire (
     Volumen* volumen,
         s64  (*nunc_ms)(vacuum),
     Piscina* piscina)
{
    Cives* c;
      i32  k;

    si (!volumen || !piscina)
    {
        redde NIHIL;
    }
    c = (Cives*)piscina_allocare(piscina, (i64)magnitudo(Cives));
    si (!c)
    {
        redde NIHIL;
    }
    memset(c, ZEPHYRUM, magnitudo(*c));
    c->volumen   = volumen;
    c->scrinium  = volumen_scrinium(volumen);
    c->nunc_ms   = nunc_ms;
    per (k = ZEPHYRUM; SCHEMA[k]; k++)
    {
        si (!c->scrinium || !scrinium_exsequi(c->scrinium, SCHEMA[k]))
        {
            redde NIHIL;
        }
    }
    redde c;
}

b32
cives_nomen_validum (
    chorda nomen_usoris)
{
    i32 i;

    si (   !nomen_usoris.datum || nomen_usoris.mensura < I
        || nomen_usoris.mensura > CIVES_NOMEN_MAXIMUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < nomen_usoris.mensura; i++)
    {
        i8 s = nomen_usoris.datum[i];

        si (!((s >= 'a' && s <= 'z') || (s >= '0' && s <= '9')
              || s == '.' || s == '_' || s == '-'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

chorda
cives_nomen_normare (
      chorda  nomen_usoris,
     Piscina* piscina)
{
    chorda r;
       i32 i;

    r.mensura  = nomen_usoris.mensura;
    r.datum    = (i8*)piscina_allocare(piscina,
        (i64)nomen_usoris.mensura
        + I);
    per (i = ZEPHYRUM; i < nomen_usoris.mensura; i++)
    {
        i8 s = nomen_usoris.datum[i];

        r.datum[i] = (s >= 'A' && s <= 'Z') ? (i8)(s - 'A' + 'a') : s;
    }
    r.datum[r.mensura] = '\0';
    redde r;
}

b32
cives_creare (
        Cives* cives,
       chorda  nomen_usoris,
       chorda  titulus,
       chorda* id_exitus,
      Piscina* piscina)
{
             character  ulid[SCRINIUM_ULID_MENSURA];
                chorda  id;
    ScriniumEnuntiatum* e;
                   b32  bene;
             JsonValor* d;

    si (   !cives || !id_exitus || !piscina
        || !cives_nomen_validum(nomen_usoris) || !titulus.datum
        || _nomen_usum(cives, nomen_usoris))
    {
        redde FALSUM;
    }
    scrinium_ulid(ulid);
    id = chorda_ex_literis(ulid, piscina);
    e  = scrinium_praeparare(cives->scrinium,
        "INSERT INTO cives (id, nomen_usoris, titulus, status,"
        " creatus_ms) VALUES (?, ?, ?, ?, ?)");
    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, I, id);
    scrinium_ligare_textum(e, II, nomen_usoris);
    scrinium_ligare_textum(e, III, titulus);
    scrinium_ligare_numerum(e, IV, (s64)CIVIS_ACTIVUS);
    scrinium_ligare_numerum(e, V, _nunc(cives));
    bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
    scrinium_finire(e);
    si (   !bene
        || !_exsequi(cives, "INSERT INTO cives_nomina (nomen_usoris,"
               " civis) VALUES (?, ?)", nomen_usoris, id))
    {
        redde FALSUM;
    }
    d = json_objectum_creare(piscina);
    json_objectum_ponere(d, "id", json_chorda_creare(piscina, id));
    json_objectum_ponere(d, "nomen_usoris", json_chorda_creare(piscina,
        nomen_usoris));
    json_objectum_ponere(d, "titulus", json_chorda_creare(piscina,
        titulus));
    si (!_actum(cives, "civis.creatus", d, piscina))
    {
        redde FALSUM;
    }
    *id_exitus = id;
    redde VERUM;
}

b32
cives_per_id (
        Cives* cives,
       chorda  id,
        Civis* exitus,
      Piscina* piscina)
{
    si (!cives || !exitus || !piscina || !id.datum)
    {
        redde FALSUM;
    }
    redde _legere(cives, "SELECT id, nomen_usoris, titulus, status,"
        " creatus_ms FROM cives WHERE id = ?", id, exitus, piscina);
}

b32
cives_per_nomen (
        Cives* cives,
       chorda  nomen_usoris,
        Civis* exitus,
      Piscina* piscina)
{
    si (!cives || !exitus || !piscina || !nomen_usoris.datum)
    {
        redde FALSUM;
    }
    redde _legere(cives, "SELECT id, nomen_usoris, titulus, status,"
        " creatus_ms FROM cives WHERE nomen_usoris = ?", nomen_usoris,
        exitus, piscina);
}

b32
cives_nomen_mutare (
      Cives* cives,
     chorda  id,
     chorda  nomen_novum)
{
      Piscina* p;
    JsonValor* d;
          b32  bene;

    si (   !cives || !id.datum || !cives_nomen_validum(nomen_novum)
        || !_id_notum(cives, id) || _nomen_usum(cives, nomen_novum))
    {
        redde FALSUM;
    }
    si (   !_exsequi(cives, "UPDATE cives SET nomen_usoris = ?"
               " WHERE id = ?", nomen_novum, id)
        || !_exsequi(cives, "INSERT INTO cives_nomina (nomen_usoris,"
               " civis) VALUES (?, ?)", nomen_novum, id))
    {
        redde FALSUM;
    }
    p = piscina_generare_dynamicum("cives_actum", DXII);
    si (!p)
    {
        redde FALSUM;
    }
    d = json_objectum_creare(p);
    json_objectum_ponere(d, "id", json_chorda_creare(p, id));
    json_objectum_ponere(d, "nomen_usoris", json_chorda_creare(p,
        nomen_novum));
    bene = _actum(cives, "civis.renominatus", d, p);
    piscina_destruere(p);
    redde bene;
}

/* status + actum (genus dati) */
interior b32
_statum_scribere (
                  Cives* c,
                 chorda  id,
            CivisStatus  status,
     constans character* genus)
{
     ScriniumEnuntiatum* e;
                Piscina* p;
              JsonValor* d;
                    b32  bene;

    si (!c || !id.datum || !_id_notum(c, id))
    {
        redde FALSUM;
    }
    e = scrinium_praeparare(c->scrinium,
        "UPDATE cives SET status = ? WHERE id = ?");
    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_numerum(e, I, (s64)status);
    scrinium_ligare_textum(e, II, id);
    bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
    scrinium_finire(e);
    si (!bene)
    {
        redde FALSUM;
    }
    p = piscina_generare_dynamicum("cives_actum", DXII);
    si (!p)
    {
        redde FALSUM;
    }
    d = json_objectum_creare(p);
    json_objectum_ponere(d, "id", json_chorda_creare(p, id));
    si (status != CIVIS_DELETUS)
    {
        json_objectum_ponere(d, "status", json_integer_creare(p,
            (s64)status));
    }
    bene = _actum(c, genus, d, p);
    piscina_destruere(p);
    redde bene;
}

b32
cives_statum_ponere (
           Cives* cives,
          chorda  id,
     CivisStatus  status)
{
    redde _statum_scribere(cives, id, status, "civis.status");
}

b32
cives_delere (
     Cives* cives,
    chorda  id)
{
    redde _statum_scribere(cives, id, CIVIS_DELETUS, "civis.deletus");
}

Xar*
cives_enumerare (
      Cives* cives,
    Piscina* piscina)
{
                   Xar* l;
    ScriniumEnuntiatum* e;

    si (!cives || !piscina)
    {
        redde NIHIL;
    }
    l = xar_creare(piscina, (i32)magnitudo(Civis));
    e = l ? scrinium_praeparare(cives->scrinium,
        "SELECT id, nomen_usoris,"
        " titulus, status, creatus_ms FROM cives ORDER BY creatus_ms,"
        " rowid") : NIHIL;
    si (!e)
    {
        redde NIHIL;
    }
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        Civis* x = (Civis*)xar_addere(l);

        si (!x)
        {
            frange;
        }
        _ordinem_legere(e, x, piscina);
    }
    scrinium_finire(e);
    redde l;
}
