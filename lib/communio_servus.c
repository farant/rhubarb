/* communio_servus.c - servus actorum communionis: registrum
 * apparatuum (communio-plan-2 T3) */

#include "postulata_posix.h"
#include "communio_servus.h"
#include "communio_forma.h"
#include "json.h"
#include "moneta.h"
#include "sigillum.h"
#include "base64.h"
#include "scrinium.h"

#include <string.h>
#include <stdio.h>
#include <sys/time.h>

#define DIGESTUM_LONGITUDO   LXIV
#define TESSERA_OCTETI       XXXII

#define EXSPECTANTES_MAXIMI  LXIV
#define MORA_MAXIMA_S        XXV

/* T5: tractio dilata (long-poll) - locus fixus, nulla allocatio */
nomen structura {
    structura CommunioServus* servus;
                         b32 vivus;
            HospitiumDilatum dilatum;
           CommunioApparatus apparatus;  /* chordae petitionis */
                         s64 post;
              ReactorTimerId hora;
                         b32 hora_viva;
} Exspectans;

structura CommunioServus {
          Piscina* piscina;
          Piscina* tempus;      /* scripturae: notare/reficere */
          Volumen* volumen;
         Scrinium* scrinium;
            Cives* cives;
               s64 (*nunc_ms)(vacuum);
           Reactor* reactor;    /* hospitii (montare) */
    /* plan-3 T4: epistulae aditus et frenum */
      Epistularius* epistularius;
          Custodia* custodia;
         character  mittens[CXXVIII];
         character  basis_publica[CCLVI];
        Exspectans  exspectantes[EXSPECTANTES_MAXIMI];
};

hic_manens constans character* constans SCHEMA[] = {
    "CREATE TABLE IF NOT EXISTS communio_membra"
    " (spatium TEXT NOT NULL, civis TEXT NOT NULL,"
    " PRIMARY KEY (spatium, civis))",
    "CREATE TABLE IF NOT EXISTS communio_permissi"
    " (epistula TEXT PRIMARY KEY, digestum TEXT NOT NULL UNIQUE,"
    " civis TEXT NOT NULL, spatium TEXT NOT NULL,"
    " creatus_ms INTEGER NOT NULL,"
    " revocatus INTEGER NOT NULL DEFAULT 0)",
    "CREATE TABLE IF NOT EXISTS communio_aditus"
    " (id TEXT PRIMARY KEY, epistula TEXT NOT NULL,"
    " civis TEXT NOT NULL, spatium TEXT NOT NULL,"
    " digestum TEXT NOT NULL, titulus TEXT NOT NULL,"
    " codex TEXT NOT NULL, signum TEXT NOT NULL,"
    " creatus_ms INTEGER NOT NULL, expirat_ms INTEGER NOT NULL,"
    " conatus INTEGER NOT NULL DEFAULT 0,"
    " confirmatus INTEGER NOT NULL DEFAULT 0, apparatus TEXT)",
    "CREATE TABLE IF NOT EXISTS communio_apparatus"
    " (id TEXT PRIMARY KEY, civis TEXT NOT NULL,"
    " spatium TEXT NOT NULL, titulus TEXT NOT NULL,"
    " digestum TEXT NOT NULL UNIQUE, replica TEXT UNIQUE,"
    " creatus_ms INTEGER NOT NULL,"
    " usus_ultimus_ms INTEGER NOT NULL DEFAULT 0,"
    " revocatus INTEGER NOT NULL DEFAULT 0)",
    NIHIL
};


/* ==================================================
 * Auxilia
 * ================================================== */

interior s64
_nunc (
    CommunioServus* s)
{
    structura timeval tv;

    si (s->nunc_ms)
    {
        redde s->nunc_ms();
    }
    si (gettimeofday(&tv, NIHIL) != ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde (s64)tv.tv_sec * M + (s64)(tv.tv_usec / M);
}

/* tessera -> SHA-256 hex in buffer (LXV) */
interior vacuum
_digestum (
        chorda  tessera,
     character* hex)
{
    Sigillum g;

    g = sigillum_computare(tessera.datum,
        (memoriae_index)tessera.mensura);
    sigillum_hex(&g, hex);
}

/* comparatio temporis constantis */
interior b32
_aequale_constans (
    chorda a,
    chorda b)
{
     i8 diff;
    i32 i;

    si (a.mensura != b.mensura)
    {
        redde FALSUM;
    }
    diff = ZEPHYRUM;
    per (i = ZEPHYRUM; i < a.mensura; i++)
    {
        diff = (i8)(diff | (a.datum[i] ^ b.datum[i]));
    }
    redde diff == ZEPHYRUM;
}

interior b32
_digestum_validum (
    chorda d)
{
    i32 i;

    si (d.mensura != DIGESTUM_LONGITUDO || !d.datum)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < d.mensura; i++)
    {
        si (   !(d.datum[i] >= '0' && d.datum[i] <= '9')
            && !(d.datum[i] >= 'a' && d.datum[i] <= 'f'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* transactio vocatoris iungitur, aliter propria */
interior b32
_incipere (
    CommunioServus* s,
               b32* propria)
{
    *propria = !volumen_in_transactione(s->volumen);
    redde !*propria || volumen_transactionem_incipere(s->volumen);
}

interior b32
_finire (
    CommunioServus* s,
               b32  propria,
               b32  bene)
{
    si (!propria)
    {
        redde bene;
    }
    si (bene)
    {
        redde volumen_transactionem_committere(s->volumen);
    }
    (vacuum)volumen_transactionem_revolvere(s->volumen);
    redde FALSUM;
}

interior b32
_actum (
    CommunioServus* s,
constans character* genus,
         JsonValor* o)
{
    redde o && volumen_actum_appendere(s->volumen, genus,
                   json_scribere(o, s->tempus)) > ZEPHYRUM;
}

interior b32
_persona_activa (
    CommunioServus* s,
            chorda  civis)
{
    Civis c;

    redde civis.mensura > ZEPHYRUM
        && cives_per_id(s->cives, civis, &c, s->tempus)
        && c.status == CIVIS_ACTIVUS;
}

interior b32
_membrum_est (
    CommunioServus* s,
            chorda  spatium,
            chorda  civis)
{
    ScriniumEnuntiatum* e;
                   b32  est;

    e = scrinium_praeparare(s->scrinium,
        "SELECT 1 FROM communio_membra"
        " WHERE spatium = ? AND civis = ?");
    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, I, spatium);
    scrinium_ligare_textum(e, II, civis);
    est = scrinium_gradi(e) == SCRINIUM_ORDO;
    scrinium_finire(e);
    redde est;
}

/* ordo -> CommunioApparatus (columnae: id, civis, spatium, titulus,
 * replica, revocatus) */
interior vacuum
_apparatum_legere (
     ScriniumEnuntiatum* e,
                integer  primus,
      CommunioApparatus* a,
                Piscina* piscina)
{
    a->id         = scrinium_columna_textus(e, primus, piscina);
    a->civis      = scrinium_columna_textus(e, primus + I, piscina);
    a->spatium    = scrinium_columna_textus(e, primus + II, piscina);
    a->titulus    = scrinium_columna_textus(e, primus + III, piscina);
    a->replica    = scrinium_columna_textus(e, primus + IV, piscina);
    a->revocatus  = scrinium_columna_numerus(e, primus + V) != ZEPHYRUM;
}


/* ==================================================
 * Vita
 * ================================================== */

CommunioServus*
communio_servum_creare (
    Piscina* piscina,
    Volumen* volumen,
      Cives* cives,
         s64 (*nunc_ms)(vacuum))
{
    CommunioServus* s;
               i32  k;

    si (!piscina || !volumen || !cives)
    {
        redde NIHIL;
    }
    s = (CommunioServus*)piscina_allocare(piscina,
        magnitudo(CommunioServus));
    si (!s)
    {
        redde NIHIL;
    }
    memset(s, ZEPHYRUM, magnitudo(*s));
    s->piscina   = piscina;
    s->volumen   = volumen;
    s->scrinium  = volumen_scrinium(volumen);
    s->cives     = cives;
    s->nunc_ms   = nunc_ms;
    s->tempus    = piscina_generare_dynamicum("communio_servus",
        (memoriae_index)M * IV);
    si (!s->scrinium || !s->tempus)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; SCHEMA[k]; k++)
    {
        si (!scrinium_exsequi(s->scrinium, SCHEMA[k]))
        {
            piscina_destruere(s->tempus);
            redde NIHIL;
        }
    }
    redde s;
}

interior vacuum
_exspectantem_solvere (
    Exspectans* x);

memoriae_index
communio_servi_usus (
    constans CommunioServus* servus)
{
    redde servus && servus->tempus ? piscina_summa_usus(servus->tempus)
                                   : (memoriae_index)ZEPHYRUM;
}

vacuum
communio_servum_destruere (
    CommunioServus* servus)
{
    i32 i;

    si (!servus)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < EXSPECTANTES_MAXIMI; i++)
    {
        si (servus->exspectantes[i].vivus)
        {
            _exspectantem_solvere(&servus->exspectantes[i]);
        }
    }
    si (servus->tempus)
    {
        piscina_destruere(servus->tempus);
        servus->tempus = NIHIL;
    }
}


/* ==================================================
 * Registrum
 * ================================================== */

b32
communio_membrum_addere (
    CommunioServus* servus,
            chorda  spatium,
            chorda  civis)
{
    ScriniumEnuntiatum* e;
        PiscinaNotatio  nota;
             JsonValor* o;
                   b32  propria;
                   b32  bene;

    si (!servus || spatium.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    nota = piscina_notare(servus->tempus);
    bene = _persona_activa(servus, civis);
    si (bene && !_membrum_est(servus, spatium, civis))
    {
        bene = _incipere(servus, &propria);
        si (bene)
        {
            e = scrinium_praeparare(servus->scrinium,
                "INSERT INTO communio_membra (spatium, civis)"
                " VALUES (?, ?)");
            bene = e != NIHIL;
            si (e)
            {
                scrinium_ligare_textum(e, I, spatium);
                scrinium_ligare_textum(e, II, civis);
                bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
                scrinium_finire(e);
            }
            o = json_objectum_creare(servus->tempus);
            si (bene && o)
            {
                json_objectum_ponere(o, "spatium", json_chorda_creare(
                    servus->tempus, spatium));
                json_objectum_ponere(o, "civis", json_chorda_creare(
                    servus->tempus, civis));
            }
            bene = _finire(servus, propria, bene && _actum(servus,
                "communio.membrum-additum", o));
        }
    }
    piscina_reficere(servus->tempus, nota);
    redde bene;
}

b32
communio_apparatum_addere (
    CommunioServus* servus,
            chorda  civis,
            chorda  spatium,
            chorda  titulus,
            chorda  digestum,
         character* id_exitus)
{
    ScriniumEnuntiatum* e;
        PiscinaNotatio  nota;
             JsonValor* o;
             character  id[MONETA_ULID_MENSURA];
                   b32  propria;
                   b32  bene;

    si (   !servus || !id_exitus || titulus.mensura == ZEPHYRUM
        || !_digestum_validum(digestum))
    {
        redde FALSUM;
    }
    nota = piscina_notare(servus->tempus);
    bene = _persona_activa(servus, civis)
        && _membrum_est(servus, spatium, civis)
        && _incipere(servus, &propria);
    si (bene)
    {
        moneta_ulid(id);
        e = scrinium_praeparare(servus->scrinium,
            "INSERT INTO communio_apparatus (id, civis, spatium,"
            " titulus, digestum, creatus_ms)"
            " VALUES (?, ?, ?, ?, ?, ?)");
        bene = e != NIHIL;
        si (e)
        {
            scrinium_ligare_textum(e, I, chorda_ex_literis(id,
                servus->tempus));
            scrinium_ligare_textum(e, II, civis);
            scrinium_ligare_textum(e, III, spatium);
            scrinium_ligare_textum(e, IV, titulus);
            scrinium_ligare_textum(e, V, digestum);
            scrinium_ligare_numerum(e, VI, _nunc(servus));
            bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
            scrinium_finire(e);
        }
        o = json_objectum_creare(servus->tempus);
        si (bene && o)
        {
            json_objectum_ponere(o, "id", json_chorda_creare_literis(
                servus->tempus, id));
            json_objectum_ponere(o, "civis", json_chorda_creare(
                servus->tempus, civis));
            json_objectum_ponere(o, "spatium", json_chorda_creare(
                servus->tempus, spatium));
            json_objectum_ponere(o, "titulus", json_chorda_creare(
                servus->tempus, titulus));
        }
        bene = _finire(servus, propria, bene && _actum(servus,
            "communio.apparatus-additus", o));
        si (bene)
        {
            memcpy(id_exitus, id, (size_t)MONETA_ULID_MENSURA);
        }
    }
    piscina_reficere(servus->tempus, nota);
    redde bene;
}

b32
communio_apparatum_revocare (
    CommunioServus* servus,
            chorda  id)
{
    ScriniumEnuntiatum* e;
        PiscinaNotatio  nota;
             JsonValor* o;
                   s64  revocatus;
                   b32  propria;
                   b32  bene;

    si (!servus || id.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    e = scrinium_praeparare(servus->scrinium,
        "SELECT revocatus FROM communio_apparatus WHERE id = ?");
    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, I, id);
    si (scrinium_gradi(e) != SCRINIUM_ORDO)
    {
        scrinium_finire(e);
        redde FALSUM;
    }
    revocatus = scrinium_columna_numerus(e, ZEPHYRUM);
    scrinium_finire(e);
    si (revocatus != ZEPHYRUM)
    {
        redde VERUM;
    }
    nota = piscina_notare(servus->tempus);
    bene = _incipere(servus, &propria);
    si (bene)
    {
        e = scrinium_praeparare(servus->scrinium,
            "UPDATE communio_apparatus SET revocatus = 1 WHERE id = ?");
        bene = e != NIHIL;
        si (e)
        {
            scrinium_ligare_textum(e, I, id);
            bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
            scrinium_finire(e);
        }
        o = json_objectum_creare(servus->tempus);
        si (bene && o)
        {
            json_objectum_ponere(o, "id", json_chorda_creare(
                servus->tempus, id));
        }
        bene = _finire(servus, propria, bene && _actum(servus,
            "communio.apparatus-revocatus", o));
    }
    piscina_reficere(servus->tempus, nota);
    redde bene;
}

Xar*
communio_apparatus_enumerare (
    CommunioServus* servus,
           Piscina* piscina)
{
    ScriniumEnuntiatum* e;
                   Xar* l;

    si (!servus || !piscina)
    {
        redde NIHIL;
    }
    l = xar_creare(piscina, (i32)magnitudo(CommunioApparatus));
    e = scrinium_praeparare(servus->scrinium,
        "SELECT id, civis, spatium, titulus, COALESCE(replica, ''),"
        " revocatus FROM communio_apparatus"
        " ORDER BY creatus_ms, rowid");
    si (!l || !e)
    {
        redde NIHIL;
    }
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        CommunioApparatus* a = (CommunioApparatus*)xar_addere(l);

        si (!a)
        {
            scrinium_finire(e);
            redde NIHIL;
        }
        _apparatum_legere(e, ZEPHYRUM, a, piscina);
    }
    scrinium_finire(e);
    redde l;
}

b32
communio_apparatum_verificare (
       CommunioServus* servus,
               chorda  tessera,
    CommunioApparatus* exitus,
              Piscina* piscina)
{
    ScriniumEnuntiatum* e;
             character  hex[SIGILLUM_HEX_MENSURA];
                chorda  digestum;
                   b32  bene;

    si (   !servus || !exitus || !piscina
        || tessera.mensura != COMMUNIO_TESSERA_MENSURA)
    {
        redde FALSUM;
    }
    _digestum(tessera, hex);
    digestum.datum    = (i8*)hex;
    digestum.mensura  = DIGESTUM_LONGITUDO;
    e = scrinium_praeparare(servus->scrinium,
        "SELECT digestum, id, civis, spatium, titulus,"
        " COALESCE(replica, ''), revocatus FROM communio_apparatus"
        " WHERE digestum = ?");
    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, I, digestum);
    bene = FALSUM;
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        bene = _aequale_constans(scrinium_columna_textus(e, ZEPHYRUM,
                   piscina), digestum);
        si (bene)
        {
            _apparatum_legere(e, I, exitus, piscina);
            bene = !exitus->revocatus;
        }
    }
    scrinium_finire(e);
    si (bene)
    {
        /* recensio 1: cives_per_id in tempus allocat - omnis petitio
         * hic transit, ergo notare/reficere (aliter piscina crescit) */
        PiscinaNotatio nota = piscina_notare(servus->tempus);

        bene = _persona_activa(servus, exitus->civis);
        piscina_reficere(servus->tempus, nota);
    }
    si (bene)
    {
        e = scrinium_praeparare(servus->scrinium,
            "UPDATE communio_apparatus SET usus_ultimus_ms = ?"
            " WHERE id = ?");
        si (e)
        {
            scrinium_ligare_numerum(e, I, _nunc(servus));
            scrinium_ligare_textum(e, II, exitus->id);
            (vacuum)scrinium_gradi(e);
            scrinium_finire(e);
        }
    }
    memset(hex, ZEPHYRUM, magnitudo(hex));
    redde bene;
}

s32
communio_replicam_ligare (
    CommunioServus* servus,
            chorda  id,
            chorda  replica)
{
    ScriniumEnuntiatum* e;
        PiscinaNotatio  nota;
             JsonValor* o;
                chorda  praesens;
                   b32  propria;
                   b32  bene;

    si (!servus || !communio_replica_valida(replica))
    {
        redde -I;
    }
    nota = piscina_notare(servus->tempus);
    e = scrinium_praeparare(servus->scrinium,
        "SELECT COALESCE(replica, '') FROM communio_apparatus"
        " WHERE id = ?");
    si (!e)
    {
        piscina_reficere(servus->tempus, nota);
        redde -I;
    }
    scrinium_ligare_textum(e, I, id);
    si (scrinium_gradi(e) != SCRINIUM_ORDO)
    {
        scrinium_finire(e);
        piscina_reficere(servus->tempus, nota);
        redde -I;
    }
    praesens = scrinium_columna_textus(e, ZEPHYRUM, servus->tempus);
    scrinium_finire(e);
    si (praesens.mensura > ZEPHYRUM)
    {
        /* recensio (venenum): comparatio ANTE refectionem - praesens in
         * tempus habitat */
        bene = chorda_aequalis(praesens, replica);
        piscina_reficere(servus->tempus, nota);
        redde bene ? ZEPHYRUM : I;
    }
    e = scrinium_praeparare(servus->scrinium,
        "SELECT 1 FROM communio_apparatus WHERE replica = ?");
    si (!e)
    {
        piscina_reficere(servus->tempus, nota);
        redde -I;
    }
    scrinium_ligare_textum(e, I, replica);
    bene = scrinium_gradi(e) == SCRINIUM_ORDO;
    scrinium_finire(e);
    si (bene)
    {
        piscina_reficere(servus->tempus, nota);
        redde II;
    }
    bene = _incipere(servus, &propria);
    si (bene)
    {
        e = scrinium_praeparare(servus->scrinium,
            "UPDATE communio_apparatus SET replica = ? WHERE id = ?");
        bene = e != NIHIL;
        si (e)
        {
            scrinium_ligare_textum(e, I, replica);
            scrinium_ligare_textum(e, II, id);
            bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
            scrinium_finire(e);
        }
        o = json_objectum_creare(servus->tempus);
        si (bene && o)
        {
            json_objectum_ponere(o, "id", json_chorda_creare(
                servus->tempus, id));
            json_objectum_ponere(o, "replica", json_chorda_creare(
                servus->tempus, replica));
        }
        bene = _finire(servus, propria, bene && _actum(servus,
            "communio.replica-ligata", o));
    }
    piscina_reficere(servus->tempus, nota);
    redde bene ? ZEPHYRUM : -I;
}

b32
communio_tesseram_facere (
    character exitus[XLIV],
    character digestum[LXV])
{
        i8 octeti[TESSERA_OCTETI];
       i32 n;
    chorda t;

    si (   !exitus || !digestum || !moneta_octeti_fortuiti(octeti,
            TESSERA_OCTETI))
    {
        redde FALSUM;
    }
    n = base64url_scribere(octeti, TESSERA_OCTETI, exitus, XLIV);
    memset(octeti, ZEPHYRUM, magnitudo(octeti));
    si (n != COMMUNIO_TESSERA_MENSURA)
    {
        redde FALSUM;
    }
    exitus[n]  = '\0';
    t.datum    = (i8*)exitus;
    t.mensura  = n;
    _digestum(t, digestum);
    redde VERUM;
}


/* ==================================================
 * Protocollum (T4)
 * ================================================== */

#define MASSAE_QUAESITAE_MAXIMAE M

interior vacuum
_json_respondere (
    HospitiumColloquium* col,
                    i32  status,
              JsonValor* o)
{
    colloquium_respondere(col, status, "application/json",
        json_scribere(o, colloquium_piscina(col)));
}

interior vacuum
_errare (
    HospitiumColloquium* col,
                    i32  status,
     constans character* causa)
{
      Piscina* p;
    JsonValor* o;

    p = colloquium_piscina(col);
    o = json_objectum_creare(p);
    json_objectum_ponere(o, "error", json_chorda_creare_literis(p,
        causa));
    _json_respondere(col, status, o);
}

/* "Bearer <tessera>" -> apparatus; aliter 401 missum, FALSUM */
interior b32
_admissus (
    HospitiumColloquium* col,
      CommunioApparatus* a)
{
    CommunioServus* s;
            chorda  h;
            chorda  t;

    s = (CommunioServus*)colloquium_datum(col);
    h = colloquium_caput(col, "Authorization");
    si (   h.datum && h.mensura > VII
        && memcmp(h.datum, "Bearer ", (size_t)VII) == ZEPHYRUM)
    {
        t.datum    = h.datum + VII;
        t.mensura  = h.mensura - VII;
        si (communio_apparatum_verificare(s, t, a,
                colloquium_piscina(col)))
        {
            redde VERUM;
        }
    }
    colloquium_caput_addere(col, "WWW-Authenticate", "Bearer");
    _errare(col, CDI, "tessera");
    redde FALSUM;
}

/* corpus petitionis ut JSON (NIHIL si non) */
interior JsonValor*
_corpus_json (
    HospitiumColloquium* col)
{
    constans HttpPetitioServeri* q;
                   JsonResultus  r;

    q = colloquium_petitio(col);
    si (!q || q->corpus_via)
    {
        redde NIHIL;
    }
    r = json_legere(q->corpus, colloquium_piscina(col));
    redde r.successus ? r.radix : NIHIL;
}

/* numerus decimalis non negativus (XVIII signa ad plurimum) */
interior b32
_numerus_legere (
    chorda  t,
       s64* exitus)
{
    s64 v;
    i32 i;

    si (t.mensura == ZEPHYRUM || t.mensura > XVIII)
    {
        redde FALSUM;
    }
    v = ZEPHYRUM;
    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        si (t.datum[i] < '0' || t.datum[i] > '9')
        {
            redde FALSUM;
        }
        v = v * X + (s64)(t.datum[i] - '0');
    }
    *exitus = v;
    redde VERUM;
}

interior vacuum
_ligare_post (
    HospitiumColloquium* col)
{
       CommunioServus* s;
    CommunioApparatus  a;
            JsonValor* corpus;
            JsonValor* o;
              Piscina* p;
               chorda  vacua;
                  s32  r;

    s = (CommunioServus*)colloquium_datum(col);
    si (!_admissus(col, &a))
    {
        redde;
    }
    p              = colloquium_piscina(col);
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    corpus         = _corpus_json(col);
    si (!corpus)
    {
        _errare(col, CD, "corpus");
        redde;
    }
    r = communio_replicam_ligare(s, a.id, json_capere_chorda(corpus,
        "replica", vacua));
    si (r == I || r == II)
    {
        _errare(col, CDIX, "replica");
        redde;
    }
    si (r != ZEPHYRUM)
    {
        _errare(col, CD, "replica");
        redde;
    }
    o = json_objectum_creare(p);
    json_objectum_ponere(o, "apparatus", json_chorda_creare(p, a.id));
    json_objectum_ponere(o, "spatium", json_chorda_creare(p,
        a.spatium));
    json_objectum_ponere(o, "servus", json_chorda_creare(p,
        volumen_replica(s->volumen)));
    _json_respondere(col, CC, o);
}

interior vacuum
_apparatus_get (
    HospitiumColloquium* col)
{
        CommunioServus* s;
     CommunioApparatus  a;
    ScriniumEnuntiatum* e;
               Piscina* p;
             JsonValor* o;
             JsonValor* l;

    s = (CommunioServus*)colloquium_datum(col);
    si (!_admissus(col, &a))
    {
        redde;
    }
    p = colloquium_piscina(col);
    e = scrinium_praeparare(s->scrinium,
        "SELECT replica, titulus, civis FROM communio_apparatus"
        " WHERE spatium = ? AND replica IS NOT NULL"
        " ORDER BY creatus_ms, rowid");
    si (!e)
    {
        _errare(col, D, "scrinium");
        redde;
    }
    scrinium_ligare_textum(e, I, a.spatium);
    l = json_tabulatum_creare(p);
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
         JsonValor* x      = json_objectum_creare(p);
            chorda  civis  = scrinium_columna_textus(e, II, p);
             Civis  persona;

        json_objectum_ponere(x, "replica", json_chorda_creare(p,
            scrinium_columna_textus(e, ZEPHYRUM, p)));
        json_objectum_ponere(x, "titulus", json_chorda_creare(p,
            scrinium_columna_textus(e, I, p)));
        json_objectum_ponere(x, "civis", json_chorda_creare(p, civis));
        si (cives_per_id(s->cives, civis, &persona, p))
        {
            json_objectum_ponere(x, "persona", json_chorda_creare(p,
                persona.titulus));
        }
        json_tabulatum_addere(l, x);
    }
    scrinium_finire(e);
    o = json_objectum_creare(p);
    json_objectum_ponere(o, "apparatus", l);
    _json_respondere(col, CC, o);
}

interior vacuum
_desunt_post (
    HospitiumColloquium* col)
{
       CommunioServus* s;
    CommunioApparatus  a;
            JsonValor* corpus;
            JsonValor* quaesitae;
            JsonValor* desunt;
            JsonValor* o;
              Piscina* p;
                  i32  i;

    s = (CommunioServus*)colloquium_datum(col);
    si (!_admissus(col, &a))
    {
        redde;
    }
    p       = colloquium_piscina(col);
    corpus  = _corpus_json(col);
    quaesitae = corpus ? json_objectum_capere(corpus,
        "massae") : NIHIL;
    si (   !quaesitae || quaesitae->genus != JSON_TABULATUM
        || json_tabulatum_numerus(quaesitae) > MASSAE_QUAESITAE_MAXIMAE)
    {
        _errare(col, CD, "massae");
        redde;
    }
    desunt = json_tabulatum_creare(p);
    per (i = ZEPHYRUM; i < json_tabulatum_numerus(quaesitae); i++)
    {
        JsonValor* h = json_tabulatum_obtinere(quaesitae, i);

        si (   !h || h->genus != JSON_CHORDA
            || !_digestum_validum(h->datum.chorda_valor))
        {
            _errare(col, CD, "massae");
            redde;
        }
        si (!volumen_massam_habet(s->volumen, h->datum.chorda_valor))
        {
            json_tabulatum_addere(desunt, json_chorda_creare(p,
                h->datum.chorda_valor));
        }
    }
    o = json_objectum_creare(p);
    json_objectum_ponere(o, "desunt", desunt);
    _json_respondere(col, CC, o);
}

interior vacuum
_massa_put (
    HospitiumColloquium* col)
{
                 CommunioServus* s;
              CommunioApparatus  a;
    constans HttpPetitioServeri* q;
                         chorda  hex;
                       Sigillum  g;
                      character  notum[SIGILLUM_HEX_MENSURA];
                      character  computatum[SIGILLUM_HEX_MENSURA];

    s = (CommunioServus*)colloquium_datum(col);
    si (!_admissus(col, &a))
    {
        redde;
    }
    hex  = colloquium_param(col, "sigillum");
    q    = colloquium_petitio(col);
    si (!_digestum_validum(hex) || !q)
    {
        _errare(col, CD, "sigillum");
        redde;
    }
    /* recensio 5: limes unus utrimque - quod cliens trahere nequit
     * (lector http) servus non accipit */
    si (   (q->corpus_via ? q->content_length : q->corpus.mensura)
        > COMMUNIO_MASSA_MAXIMA)
    {
        _errare(col, CDXIII, "massa nimia");
        redde;
    }
    memcpy(notum, hex.datum, (size_t)DIGESTUM_LONGITUDO);
    notum[DIGESTUM_LONGITUDO] = '\0';
    si (q->corpus_via)
    {
        si (!volumen_massam_condere_ex_filo(s->volumen, q->corpus_via,
                (i64)q->content_length, notum))
        {
            _errare(col, CD, "sigillum");
            redde;
        }
        colloquium_respondere(col, CCI, NIHIL, hex);
        redde;
    }
    g = sigillum_computare(q->corpus.datum,
        (memoriae_index)q->corpus.mensura);
    sigillum_hex(&g, computatum);
    si (memcmp(computatum, notum, (size_t)DIGESTUM_LONGITUDO)
        != ZEPHYRUM)
    {
        _errare(col, CD, "sigillum");
        redde;
    }
    si (   !volumen_massam_habet(s->volumen, hex)
        && !volumen_massam_condere(s->volumen, q->corpus, computatum))
    {
        _errare(col, D, "massa");
        redde;
    }
    colloquium_respondere(col, CCI, NIHIL, hex);
}

interior s32
_fons_massae_legere (
    vacuum* datum,
        i8* capsa,
       i32  n)
{
    redde volumen_massam_legere((VolumenMassaLector*)datum, capsa, n);
}

interior vacuum
_fons_massae_claudere (
    vacuum* datum)
{
    volumen_massam_claudere((VolumenMassaLector*)datum);
}

interior vacuum
_massa_get (
    HospitiumColloquium* col)
{
        CommunioServus* s;
     CommunioApparatus  a;
                chorda  hex;
    VolumenMassaLector* l;
                   i64  mensura;
         HospitiumFons  fons;

    s = (CommunioServus*)colloquium_datum(col);
    si (!_admissus(col, &a))
    {
        redde;
    }
    hex      = colloquium_param(col, "sigillum");
    mensura  = ZEPHYRUM;
    l = _digestum_validum(hex) ? volumen_massam_aperire(s->volumen, hex,
            &mensura, colloquium_piscina(col)) : NIHIL;
    si (!l)
    {
        _errare(col, CDIV, "massa");
        redde;
    }
    fons.mensura   = mensura;
    fons.legere    = _fons_massae_legere;
    fons.claudere  = _fons_massae_claudere;
    fons.datum     = l;
    colloquium_respondere_fontem(col, CC, "application/octet-stream",
        fons);
}

interior vacuum
_excitare (
    CommunioServus* s,
            chorda  spatium,
            chorda  replica);

interior constans character*
_causa_importationis (
    VolumenImportatio x)
{
    commutatio (x)
    {
        casus VOLUMEN_HORA_FUTURA:
            redde "hora-futura";
        casus VOLUMEN_CONFLICTUS:
            redde "conflictus";
        casus VOLUMEN_MASSA_DEEST:
            redde "massa-deest";
        ordinarius:
            redde "invalidum";
    }
}

interior vacuum
_acta_post (
    HospitiumColloquium* col)
{
                 CommunioServus* s;
              CommunioApparatus  a;
    constans HttpPetitioServeri* q;
                            Xar* acta;
                         chorda  causa;
                        Piscina* p;
                      JsonValor* o;
                      JsonValor* refusum;
                            i32  i;
                            i32  accepta;
                            i32  importata;

    s = (CommunioServus*)colloquium_datum(col);
    si (!_admissus(col, &a))
    {
        redde;
    }
    p = colloquium_piscina(col);
    q = colloquium_petitio(col);
    acta  = (q && !q->corpus_via) ? communio_acta_legere(q->corpus,
                &causa, p) : NIHIL;
    si (!acta)
    {
        _errare(col, CD, q && q->corpus_via ? "corpus nimium"
                                            : "acta mala");
        redde;
    }
    si (a.replica.mensura == ZEPHYRUM)
    {
        _errare(col, CDIII, "replica non ligata");
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(acta); i++)
    {
        si (!chorda_aequalis(((VolumenActum*)xar_obtinere(acta, i))
                ->identitas.replica, a.replica))
        {
            _errare(col, CDIII, "replica aliena");
            redde;
        }
    }
    accepta    = ZEPHYRUM;
    importata  = ZEPHYRUM;
    refusum    = json_nullum_creare(p);
    per (i = ZEPHYRUM; i < xar_numerus(acta); i++)
    {
        VolumenImportatio x = volumen_actum_importare(s->volumen,
            (VolumenActum*)xar_obtinere(acta, i));

        si (x == VOLUMEN_IMPORTATUM || x == VOLUMEN_IAM_ADEST)
        {
            accepta++;
            si (x == VOLUMEN_IMPORTATUM)
            {
                importata++;
            }
            perge;
        }
        refusum = json_objectum_creare(p);
        json_objectum_ponere(refusum, "index", json_integer_creare(p,
            (s64)i));
        json_objectum_ponere(refusum, "causa",
            json_chorda_creare_literis(
            p, _causa_importationis(x)));
        frange;
    }
    o = json_objectum_creare(p);
    json_objectum_ponere(o, "accepta", json_integer_creare(p,
        (s64)accepta));
    json_objectum_ponere(o, "refusum", refusum);
    /* recensio 2: ANTE responsum - respondere piscinam colloquii
     * reficere potest (connexio parata petitioni novae), et a.spatium /
     * a.replica in ea habitant */
    si (importata > ZEPHYRUM)
    {
        _excitare(s, a.spatium, a.replica);
    }
    _json_respondere(col, CC, o);
}

/* replicae ligatae spatii praeter propriam: tabula chordarum */
interior chorda*
_replicae_spatii (
    CommunioServus* s,
            chorda  spatium,
            chorda  propria,
               i32* numerus,
           Piscina* p)
{
    ScriniumEnuntiatum* e;
                   Xar* l;
                chorda* tabula;
                   i32  i;

    *numerus  = ZEPHYRUM;
    l         = xar_creare(p, (i32)magnitudo(chorda));
    e = scrinium_praeparare(s->scrinium,
        "SELECT replica FROM communio_apparatus WHERE spatium = ?"
        " AND replica IS NOT NULL AND replica <> ?");
    si (!l || !e)
    {
        redde NIHIL;
    }
    scrinium_ligare_textum(e, I, spatium);
    scrinium_ligare_textum(e, II, propria);
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        chorda* c = (chorda*)xar_addere(l);

        si (c)
        {
            *c = scrinium_columna_textus(e, ZEPHYRUM, p);
        }
    }
    scrinium_finire(e);
    *numerus = xar_numerus(l);
    tabula = (chorda*)piscina_allocare(p, (memoriae_index)(magnitudo(
        chorda) * (size_t)(*numerus + I)));
    per (i = ZEPHYRUM; tabula && i < *numerus; i++)
    {
        tabula[i] = *(chorda*)xar_obtinere(l, i);
    }
    redde tabula;
}

/* corpus tractionis post seq servi 'post' (NIHIL si error) */
interior JsonValor*
_tractio (
       CommunioServus* s,
    CommunioApparatus* a,
                  s64  post,
              Piscina* p)
{
       chorda* replicae;
          Xar* acta;
          Xar* massae;
    JsonValor* o;
    JsonValor* t;
    JsonValor* m;
          s64  cursor;
          i32  n;
          i32  i;
          i32  j;
          i32  k;

    replicae = _replicae_spatii(s, a->spatium, a->replica, &n, p);
    si (!replicae)
    {
        redde NIHIL;
    }
    acta = volumen_acta_replicarum_legere(s->volumen, post, replicae, n,
        COMMUNIO_ACTA_MAXIMA, p);
    massae  = xar_creare(p, (i32)magnitudo(chorda));
    o       = json_objectum_creare(p);
    t       = json_tabulatum_creare(p);
    m       = json_tabulatum_creare(p);
    si (!acta || !massae || !o || !t || !m)
    {
        redde NIHIL;
    }
    cursor = post;
    per (i = ZEPHYRUM; i < xar_numerus(acta); i++)
    {
        VolumenActum* x          = (VolumenActum*)xar_obtinere(acta, i);
                 Xar* nominatae  = communio_massas_nominatas(x, p);

        json_tabulatum_addere(t, communio_actum_scribere(x, p));
        cursor = x->seq;
        per (j = ZEPHYRUM; nominatae && j < xar_numerus(nominatae); j++)
        {
            chorda h    = *(chorda*)xar_obtinere(nominatae, j);
               b32 iam  = FALSUM;

            per (k = ZEPHYRUM; k < xar_numerus(massae); k++)
            {
                si (chorda_aequalis(*(chorda*)xar_obtinere(massae, k),
                        h))
                {
                    iam = VERUM;
                }
            }
            si (!iam && volumen_massam_habet(s->volumen, h))
            {
                *(chorda*)xar_addere(massae) = h;
                json_tabulatum_addere(m, json_chorda_creare(p, h));
            }
        }
    }
    json_objectum_ponere(o, "acta", t);
    json_objectum_ponere(o, "cursor", json_integer_creare(p, cursor));
    json_objectum_ponere(o, "plura", json_boolean_creare(p,
        xar_numerus(acta) >= COMMUNIO_ACTA_MAXIMA));
    json_objectum_ponere(o, "massae", m);
    json_objectum_ponere(o, "servus", json_chorda_creare(p,
        volumen_replica(s->volumen)));
    redde o;
}


/* ==================================================
 * Tractio dilata (T5)
 * ================================================== */

interior i32
_numerus_actorum (
    JsonValor* o)
{
    redde json_tabulatum_numerus(json_objectum_capere(o, "acta"));
}

interior vacuum
_exspectantem_solvere (
    Exspectans* x)
{
    si (x->hora_viva && x->servus->reactor)
    {
        (vacuum)reactor_timer_cancellare(x->servus->reactor, x->hora);
    }
    x->hora_viva  = FALSUM;
    x->vivus      = FALSUM;
}

/* responsum dilatum dare (tractio recens); solvit */
interior vacuum
_exspectanti_respondere (
             Exspectans* x,
    HospitiumColloquium* col)
{
    JsonValor* o;

    o = _tractio(x->servus, &x->apparatus, x->post,
        colloquium_piscina(col));
    si (o)
    {
        _json_respondere(col, CC, o);
    }
    alioquin
    {
        _errare(col, D, "tractio");
    }
    _exspectantem_solvere(x);
}

/* terminus: responsum (vacuum aut quod interim advenit) */
interior vacuum
_mora_exspirat (
    vacuum* datum)
{
             Exspectans* x;
    HospitiumColloquium* col;

    x             = (Exspectans*)datum;
    x->hora_viva  = FALSUM;
    col           = dilatum_colloquium(x->dilatum);
    si (col)
    {
        _exspectanti_respondere(x, col);
    }
    x->vivus = FALSUM;
}

/* petitionem differre; FALSUM (respondendum statim) si loci pleni aut
 * horologium deficit */
interior b32
_exspectare (
       CommunioServus* s,
  HospitiumColloquium* col,
    CommunioApparatus* a,
                  s64  post,
                  s64  mora)
{
    Exspectans* x;
           i32  i;

    si (!s->reactor)
    {
        redde FALSUM;
    }
    x = NIHIL;
    per (i = ZEPHYRUM; i < EXSPECTANTES_MAXIMI && !x; i++)
    {
        si (   s->exspectantes[i].vivus
            && !dilatum_valet(s->exspectantes[i].dilatum))
        {
            _exspectantem_solvere(&s->exspectantes[i]);
        }
        si (!s->exspectantes[i].vivus)
        {
            x = &s->exspectantes[i];
        }
    }
    si (!x)
    {
        redde FALSUM;
    }
    si (mora > MORA_MAXIMA_S)
    {
        mora = MORA_MAXIMA_S;
    }
    x->servus     = s;
    x->apparatus  = *a;
    x->post       = post;
    x->hora = reactor_timer_adicere(s->reactor, (i32)(mora * M), FALSUM,
        _mora_exspirat, x);
    si (x->hora == (ReactorTimerId)-I)
    {
        redde FALSUM;
    }
    x->hora_viva  = VERUM;
    x->vivus      = VERUM;
    x->dilatum    = colloquium_differre(col);
    redde VERUM;
}

/* post importationem: exspectantes spatii excitantur si nunc aliquid
 * habent; mortui solvuntur. Scriptor ipse praeteritur sine quaestione
 * (compendium tantum: _tractio propria iam excludit) */
interior vacuum
_excitare (
    CommunioServus* s,
            chorda  spatium,
            chorda  replica)
{
                    i32  i;
    HospitiumColloquium* col;
              JsonValor* o;

    per (i = ZEPHYRUM; i < EXSPECTANTES_MAXIMI; i++)
    {
        Exspectans* x = &s->exspectantes[i];

        si (!x->vivus)
        {
            perge;
        }
        col = dilatum_colloquium(x->dilatum);
        si (!col)
        {
            _exspectantem_solvere(x);
            perge;
        }
        si (   !chorda_aequalis(x->apparatus.spatium, spatium)
            || chorda_aequalis(x->apparatus.replica, replica))
        {
            perge;
        }
        o = _tractio(s, &x->apparatus, x->post,
            colloquium_piscina(col));
        si (o && _numerus_actorum(o) > ZEPHYRUM)
        {
            _json_respondere(col, CC, o);
            _exspectantem_solvere(x);
        }
    }
}

interior vacuum
_acta_get (
    HospitiumColloquium* col)
{
       CommunioServus* s;
    CommunioApparatus  a;
               chorda  t;
                  s64  post;
                  s64  mora;
            JsonValor* o;

    s = (CommunioServus*)colloquium_datum(col);
    si (!_admissus(col, &a))
    {
        redde;
    }
    post  = ZEPHYRUM;
    mora  = ZEPHYRUM;
    t     = colloquium_quaestio(col, "post");
    si (t.datum && !_numerus_legere(t, &post))
    {
        _errare(col, CD, "post");
        redde;
    }
    t = colloquium_quaestio(col, "mora");
    si (t.datum && !_numerus_legere(t, &mora))
    {
        _errare(col, CD, "mora");
        redde;
    }
    o = _tractio(s, &a, post, colloquium_piscina(col));
    si (!o)
    {
        _errare(col, D, "tractio");
        redde;
    }
    si (   mora > ZEPHYRUM && _numerus_actorum(o) == ZEPHYRUM
        && _exspectare(s, col, &a, post, mora))
    {
        redde;
    }
    _json_respondere(col, CC, o);
}


/* ==================================================
 * Viae aditus (plan-3 T4)
 * ================================================== */

#define TITULUS_EFFUGATUS  (LXIV * VI + I)

interior b32
_epistulam_normare (
        chorda  epistula,
     character* exitus);

vacuum
communio_servum_epistularium_ponere (
         CommunioServus* servus,
           Epistularius* epistularius,
     constans character* a,
     constans character* basis)
{
    si (!servus)
    {
        redde;
    }
    servus->epistularius = epistularius;
    sprintf(servus->mittens, "%.120s", a ? a : "");
    sprintf(servus->basis_publica, "%.250s", basis ? basis : "");
}

vacuum
communio_servum_custodiam_ponere (
    CommunioServus* servus,
          Custodia* custodia)
{
    si (servus)
    {
        servus->custodia = custodia;
    }
}

/* VERUM = pergere licet (sine custodia semper) */
interior b32
_frenum_licet (
    CommunioServus* s,
            chorda  clavis)
{
    redde !s->custodia
        || custodia_frenum(s->custodia, clavis, _nunc(s)) == ZEPHYRUM;
}

interior vacuum
_frenum_notare (
    CommunioServus* s,
            chorda  clavis,
               b32  successus)
{
    si (s->custodia)
    {
        custodia_frenum_notare(s->custodia, clavis, successus,
            _nunc(s));
    }
}

interior chorda
_clavis_freni (
    constans character* praefixum,
                chorda  valor,
               Piscina* p)
{
    character* t;

    t = (character*)piscina_allocare(p,
        (memoriae_index)(strlen(praefixum)
        + (size_t)valor.mensura + I));
    sprintf(t, "%s%.*s", praefixum, (integer)valor.mensura,
        (constans character*)valor.datum);
    redde chorda_ex_literis(t, p);
}

/* < > & " ' effugata in exitus (capacitas TITULUS_EFFUGATUS) */
interior vacuum
_html_effugere (
        chorda  textus,
     character* exitus)
{
    i32 i;
    i32 k;

    k = ZEPHYRUM;
    per (i = ZEPHYRUM; i < textus.mensura && i < LXIV; i++)
    {
        i8 c = textus.datum[i];

        si (c == '<')
        {
            strcpy(exitus + k, "&lt;");
            k += IV;
        }
        alioquin si (c == '>')
        {
            strcpy(exitus + k, "&gt;");
            k += IV;
        }
        alioquin si (c == '&')
        {
            strcpy(exitus + k, "&amp;");
            k += V;
        }
        alioquin si (c == '"')
        {
            strcpy(exitus + k, "&quot;");
            k += VI;
        }
        alioquin si (c == '\'')
        {
            strcpy(exitus + k, "&#39;");
            k += V;
        }
        alioquin
        {
            exitus[k++] = (character)c;
        }
    }
    exitus[k] = '\0';
}

interior vacuum
_epistula_finis (
    vacuum* ctx,
       b32  successus,
    chorda  causa)
{
    (vacuum)ctx;
    si (!successus)
    {
        fprintf(stderr, "communio: epistula aditus non missa: %.*s\n",
            (integer)causa.mensura, (constans character*)causa.datum);
    }
}

/* epistula aditus (C5, Anglice): nexus et codex; textus post
 * traditionem deletur (codex et signum in eo) */
interior vacuum
_epistulam_aditus_mittere (
    CommunioServus* s,
    CommunioAditus* a,
            chorda  titulus,
          Piscina* p)
{
    EpistulaNuntius  n;
          character  t_titulus[LXV];
          character  h_titulus[TITULUS_EFFUGATUS];
          character* textus;
          character* html;
          character* nexus;

    si (!s->epistularius)
    {
        fprintf(stderr, "communio: epistula aditus non missa "
            "(epistularius deest)\n");
        redde;
    }
    sprintf(t_titulus, "%.*s", (integer)(titulus.mensura > LXIV ? LXIV
        : titulus.mensura), (constans character*)titulus.datum);
    _html_effugere(titulus, h_titulus);
    nexus   = (character*)piscina_allocare(p, (memoriae_index)DXII);
    textus  = (character*)piscina_allocare(p, (memoriae_index)MXXIV);
    html    = (character*)piscina_allocare(p, (memoriae_index)MMXLVIII);
    si (!nexus || !textus || !html)
    {
        redde;
    }
    sprintf(nexus, "%s/communio/confirmare?aditus=%s&signum=%s",
        s->basis_publica, a->id, a->signum);
    sprintf(textus,
        "Someone asked to sign in the device \"%s\" to"
        " golden.earth with this address.\n\n"
        "Open this link to confirm:\n%s\n\n"
        "or type this code on the device:\nCode: %.4s-%.4s\n\n"
        "The link and the code work once, for 15 minutes.\n"
        "If you did not ask for this, you can ignore this email.\n",
        t_titulus, nexus, a->codex, a->codex + IV);
    sprintf(html,
        "<p>Someone asked to sign in the device <b>%s</b> to"
        " golden.earth with this address.</p>"
        "<p><a href=\"%s\">Confirm the sign-in</a></p>"
        "<p>or type this code on the device: <b>%.4s-%.4s</b></p>"
        "<p>The link and the code work once, for 15 minutes."
        " If you did not ask for this, you can ignore this"
        " email.</p>", h_titulus, nexus, a->codex, a->codex + IV);
    memset(&n, ZEPHYRUM, magnitudo(n));
    n.a   = chorda_ex_literis(s->mittens, p);
    n.ad  = chorda_ex_literis(a->epistula, p);
    n.titulus               =
        chorda_ex_literis("Sign in to golden.earth",
        p);
    n.textus                = chorda_ex_literis(textus, p);
    n.html                  = chorda_ex_literis(html, p);
    n.clavis_idempotentiae  = chorda_ex_literis(a->id, p);
    si (!epistulam_mittere(s->epistularius, &n, _epistula_finis, NIHIL))
    {
        fprintf(stderr, "communio: epistula aditus non missa "
            "(missiones plenae)\n");
    }
    memset(nexus, ZEPHYRUM, (size_t)DXII);
    memset(textus, ZEPHYRUM, (size_t)MXXIV);
    memset(html, ZEPHYRUM, (size_t)MMXLVIII);
    memset(n.textus.datum, ZEPHYRUM, (size_t)n.textus.mensura);
    memset(n.html.datum, ZEPHYRUM, (size_t)n.html.mensura);
}

interior vacuum
_intrare_post (
    HospitiumColloquium* col)
{
    CommunioServus* s;
         JsonValor* corpus;
         JsonValor* o;
           Piscina* p;
            chorda  vacua;
            chorda  epistula;
            chorda  digestum;
            chorda  titulus;
            chorda  clavis;
         character  norma[COMMUNIO_EPISTULA_MAXIMA + I];
    CommunioAditus  a;

    s              = (CommunioServus*)colloquium_datum(col);
    p              = colloquium_piscina(col);
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    corpus         = _corpus_json(col);
    epistula = corpus ? json_capere_chorda(corpus, "epistula", vacua)
                      : vacua;
    digestum = corpus ? json_capere_chorda(corpus, "digestum", vacua)
                      : vacua;
    titulus  = corpus ? json_capere_chorda(corpus, "titulus", vacua)
                      : vacua;
    si (   !corpus || digestum.mensura == ZEPHYRUM
        || titulus.mensura == ZEPHYRUM
        || !_epistulam_normare(epistula, norma))
    {
        _errare(col, CD, "corpus");
        redde;
    }
    clavis = _clavis_freni("communio-epistula:",
        chorda_ex_literis(norma,
        p), p);
    si (!_frenum_licet(s, clavis))
    {
        _errare(col, CDXXIX, "frenum");
        redde;
    }
    _frenum_notare(s, clavis, FALSUM);
    (vacuum)communio_aditum_petere(s, epistula, digestum, titulus, &a);
    si (a.permissus)
    {
        _epistulam_aditus_mittere(s, &a, titulus, p);
    }
    memset(a.codex, ZEPHYRUM, magnitudo(a.codex));
    memset(a.signum, ZEPHYRUM, magnitudo(a.signum));
    o = json_objectum_creare(p);
    json_objectum_ponere(o, "aditus", json_chorda_creare_literis(p,
        a.id));
    _json_respondere(col, CCII, o);
}

interior vacuum
_confirmare_post (
    HospitiumColloquium* col)
{
    CommunioServus* s;
         JsonValor* corpus;
         JsonValor* o;
           Piscina* p;
            chorda  vacua;
            chorda  clavis;
         character  apparatus[MONETA_ULID_MENSURA];
               s32  r;

    s = (CommunioServus*)colloquium_datum(col);
    p = colloquium_piscina(col);
    vacua.datum = NIHIL;
    vacua.mensura = ZEPHYRUM;
    clavis = _clavis_freni("communio-ip:", colloquium_cliens(col), p);
    si (!_frenum_licet(s, clavis))
    {
        _errare(col, CDXXIX, "frenum");
        redde;
    }
    corpus = _corpus_json(col);
    r = corpus ? communio_aditum_confirmare(s,
        json_capere_chorda(corpus,
            "aditus", vacua), json_capere_chorda(corpus, "codex",
            vacua),
            vacua, apparatus) : -I;
    _frenum_notare(s, clavis, r >= ZEPHYRUM);
    si (r < ZEPHYRUM)
    {
        _errare(col, CDIII, "aditus");
        redde;
    }
    o = json_objectum_creare(p);
    json_objectum_ponere(o, "apparatus", json_chorda_creare_literis(p,
        apparatus));
    _json_respondere(col, CC, o);
}

/* titulus apparatus per id (in piscina) */
interior chorda
_titulus_apparatus (
        CommunioServus* s,
    constans character* id,
               Piscina* p)
{
    ScriniumEnuntiatum* e;
                chorda  t;

    t.datum    = NIHIL;
    t.mensura  = ZEPHYRUM;
    e = scrinium_praeparare(s->scrinium,
        "SELECT titulus FROM communio_apparatus WHERE id = ?");
    si (!e)
    {
        redde t;
    }
    scrinium_ligare_textum(e, I, chorda_ex_literis(id, p));
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        t = scrinium_columna_textus(e, ZEPHYRUM, p);
    }
    scrinium_finire(e);
    redde t;
}

interior vacuum
_confirmare_get (
    HospitiumColloquium* col)
{
    CommunioServus* s;
           Piscina* p;
            chorda  vacua;
            chorda  clavis;
         character  apparatus[MONETA_ULID_MENSURA];
         character  h_titulus[TITULUS_EFFUGATUS];
         character* pagina;
               s32  r;

    s = (CommunioServus*)colloquium_datum(col);
    p = colloquium_piscina(col);
    vacua.datum = NIHIL;
    vacua.mensura = ZEPHYRUM;
    clavis = _clavis_freni("communio-ip:", colloquium_cliens(col), p);
    si (!_frenum_licet(s, clavis))
    {
        colloquium_respondere(col, CDXXIX, "text/html; charset=utf-8",
            chorda_ex_literis("<p>Too many attempts - try again"
            " later.</p>",
            p));
        redde;
    }
    r = communio_aditum_confirmare(s, colloquium_quaestio(col,
        "aditus"),
        vacua, colloquium_quaestio(col, "signum"), apparatus);
    _frenum_notare(s, clavis, r >= ZEPHYRUM);
    si (r < ZEPHYRUM)
    {
        colloquium_respondere(col, CDIII, "text/html; charset=utf-8",
            chorda_ex_literis("<p>This sign-in link is not valid"
            " (expired, already used, or wrong).</p>", p));
        redde;
    }
    _html_effugere(_titulus_apparatus(s, apparatus, p), h_titulus);
    pagina = (character*)piscina_allocare(p, (memoriae_index)DXII);
    sprintf(pagina,
        "<!doctype html><meta charset=\"utf-8\">"
        "<title>Signed in</title><p>Signed in: <b>%s</b>."
        " You can close this page.</p>", h_titulus);
    colloquium_respondere(col, CC, "text/html; charset=utf-8",
        chorda_ex_literis(pagina, p));
}

b32
communio_servum_montare (
    CommunioServus* servus,
         Hospitium* hospitium)
{
    si (!servus || !hospitium)
    {
        redde FALSUM;
    }
    servus->reactor = hospitium_reactor(hospitium);
    redde hospitium_praebere(hospitium, HTTP_POST,
        "/communio/apparatus",
               _ligare_post, servus)
        && hospitium_praebere(hospitium, HTTP_GET,
        "/communio/apparatus",
               _apparatus_get, servus)
        && hospitium_praebere(hospitium, HTTP_POST,
               "/communio/massae/desunt", _desunt_post, servus)
        && hospitium_praebere(hospitium, HTTP_PUT,
               "/communio/massae/:sigillum", _massa_put, servus)
        && hospitium_praebere(hospitium, HTTP_GET,
               "/communio/massae/:sigillum", _massa_get, servus)
        && hospitium_praebere(hospitium, HTTP_POST, "/communio/acta",
               _acta_post, servus)
        && hospitium_praebere(hospitium, HTTP_GET, "/communio/acta",
               _acta_get, servus)
        && hospitium_praebere(hospitium, HTTP_POST, "/communio/intrare",
               _intrare_post, servus)
        && hospitium_praebere(hospitium, HTTP_POST,
               "/communio/confirmare", _confirmare_post, servus)
        && hospitium_praebere(hospitium, HTTP_GET,
        "/communio/confirmare",
               _confirmare_get, servus);
}


/* ==================================================
 * Index permissorum et aditus (plan-3 T3)
 * ================================================== */

#define CROCKFORD "0123456789ABCDEFGHJKMNPQRSTVWXYZ"
#define SIGNUM_OCTETI XXXII

/* epistulam normare in buffer (spatia extrema, ASCII minusculae);
 * FALSUM si mala (longitudo, '@' unum non extremum, signa moderantia
 * aut spatia interna) */
interior b32
_epistulam_normare (
        chorda  epistula,
     character* exitus)
{
    i32 a;
    i32 z;
    i32 i;
    i32 n;
    i32 ad;

    a = ZEPHYRUM;
    z = epistula.mensura;
    dum (   a < z
         && (epistula.datum[a] == ' ' || epistula.datum[a] == '\t'))
    {
        a++;
    }
    dum (   z > a && (epistula.datum[z - I] == ' '
                   || epistula.datum[z - I] == '\t'
                   || epistula.datum[z - I] == '\n'
                   || epistula.datum[z - I] == '\r'))
    {
        z--;
    }
    n = z - a;
    si (!epistula.datum || n < III || n > COMMUNIO_EPISTULA_MAXIMA)
    {
        redde FALSUM;
    }
    ad = ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        i8 c = epistula.datum[a + i];

        si (c <= ' ' || c == (i8)CXXVII)
        {
            redde FALSUM;
        }
        si (c == '@')
        {
            si (i == ZEPHYRUM || i == n - I)
            {
                redde FALSUM;
            }
            ad++;
        }
        exitus[i] = (c >= 'A' && c <= 'Z') ? (character)(c - 'A' + 'a')
                                           : (character)c;
    }
    exitus[n] = '\0';
    redde ad == I;
}

/* SHA-256 hex textus in buffer LXV */
interior vacuum
_sigillum_textus (
     constans character* textus,
              character* hex)
{
    Sigillum g;

    g = sigillum_computare(textus, (memoriae_index)strlen(textus));
    sigillum_hex(&g, hex);
}

interior chorda
_chorda_buffer (
    character* buffer)
{
    chorda c;

    c.datum    = (i8*)buffer;
    c.mensura  = (i32)strlen(buffer);
    redde c;
}

b32
communio_epistulam_permittere (
    CommunioServus* servus,
            chorda  epistula,
            chorda  civis,
            chorda  spatium)
{
    ScriniumEnuntiatum* e;
        PiscinaNotatio  nota;
             JsonValor* o;
             character  norma[COMMUNIO_EPISTULA_MAXIMA + I];
             character  hex[SIGILLUM_HEX_MENSURA];
                   b32  propria;
                   b32  bene;

    si (!servus || !_epistulam_normare(epistula, norma))
    {
        redde FALSUM;
    }
    _sigillum_textus(norma, hex);
    nota = piscina_notare(servus->tempus);
    bene = _persona_activa(servus, civis)
        && _membrum_est(servus, spatium, civis)
        && _incipere(servus, &propria);
    si (bene)
    {
        e = scrinium_praeparare(servus->scrinium,
            "INSERT OR REPLACE INTO communio_permissi"
            " (epistula, digestum,"
            " civis, spatium, creatus_ms, revocatus)"
            " VALUES (?, ?, ?, ?, ?, 0)");
        bene = e != NIHIL;
        si (e)
        {
            scrinium_ligare_textum(e, I, _chorda_buffer(norma));
            scrinium_ligare_textum(e, II, _chorda_buffer(hex));
            scrinium_ligare_textum(e, III, civis);
            scrinium_ligare_textum(e, IV, spatium);
            scrinium_ligare_numerum(e, V, _nunc(servus));
            bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
            scrinium_finire(e);
        }
        o = json_objectum_creare(servus->tempus);
        si (bene && o)
        {
            json_objectum_ponere(o, "epistula_digestum",
                json_chorda_creare_literis(servus->tempus, hex));
            json_objectum_ponere(o, "civis", json_chorda_creare(
                servus->tempus, civis));
            json_objectum_ponere(o, "spatium", json_chorda_creare(
                servus->tempus, spatium));
        }
        bene = _finire(servus, propria, bene && _actum(servus,
            "communio.permissus", o));
    }
    piscina_reficere(servus->tempus, nota);
    redde bene;
}

b32
communio_epistulam_revocare (
    CommunioServus* servus,
            chorda  epistula)
{
    ScriniumEnuntiatum* e;
        PiscinaNotatio  nota;
             JsonValor* o;
             character  norma[COMMUNIO_EPISTULA_MAXIMA + I];
             character  hex[SIGILLUM_HEX_MENSURA];
                   b32  est;
                   b32  propria;
                   b32  bene;

    si (!servus || !_epistulam_normare(epistula, norma))
    {
        redde FALSUM;
    }
    e = scrinium_praeparare(servus->scrinium,
        "SELECT 1 FROM communio_permissi WHERE epistula = ?");
    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, I, _chorda_buffer(norma));
    est = scrinium_gradi(e) == SCRINIUM_ORDO;
    scrinium_finire(e);
    si (!est)
    {
        redde FALSUM;
    }
    _sigillum_textus(norma, hex);
    nota = piscina_notare(servus->tempus);
    bene = _incipere(servus, &propria);
    si (bene)
    {
        e = scrinium_praeparare(servus->scrinium,
            "UPDATE communio_permissi SET revocatus = 1"
            " WHERE epistula = ?");
        bene = e != NIHIL;
        si (e)
        {
            scrinium_ligare_textum(e, I, _chorda_buffer(norma));
            bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
            scrinium_finire(e);
        }
        o = json_objectum_creare(servus->tempus);
        si (bene && o)
        {
            json_objectum_ponere(o, "epistula_digestum",
                json_chorda_creare_literis(servus->tempus, hex));
        }
        bene = _finire(servus, propria, bene && _actum(servus,
            "communio.permissio-revocata", o));
    }
    piscina_reficere(servus->tempus, nota);
    redde bene;
}

Xar*
communio_permissos_enumerare (
    CommunioServus* servus,
           Piscina* piscina)
{
    ScriniumEnuntiatum* e;
                   Xar* l;

    si (!servus || !piscina)
    {
        redde NIHIL;
    }
    l = xar_creare(piscina, (i32)magnitudo(CommunioPermissus));
    e = scrinium_praeparare(servus->scrinium,
        "SELECT epistula, civis, spatium, revocatus"
        " FROM communio_permissi"
        " ORDER BY creatus_ms, rowid");
    si (!l || !e)
    {
        redde NIHIL;
    }
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        CommunioPermissus* x = (CommunioPermissus*)xar_addere(l);

        si (!x)
        {
            frange;
        }
        x->epistula   = scrinium_columna_textus(e, ZEPHYRUM, piscina);
        x->civis      = scrinium_columna_textus(e, I, piscina);
        x->spatium    = scrinium_columna_textus(e, II, piscina);
        x->revocatus  = scrinium_columna_numerus(e, III) != ZEPHYRUM;
    }
    scrinium_finire(e);
    redde l;
}

/* codex VIII signorum Crockford ex V octetis fortuitis (XL bits) */
interior b32
_codicem_cudere (
    character* exitus)
{
     i8 o[V];
    i64 bits;
    i32 i;

    si (!moneta_octeti_fortuiti(o, V))
    {
        redde FALSUM;
    }
    bits = ZEPHYRUM;
    per (i = ZEPHYRUM; i < V; i++)
    {
        bits = (bits << VIII) | (i64)(insignatus character)o[i];
    }
    per (i = ZEPHYRUM; i < COMMUNIO_CODEX_MENSURA; i++)
    {
        exitus[COMMUNIO_CODEX_MENSURA - I - i] =
            CROCKFORD[(integer)(bits & (i64)XXXI)];
        bits >>= V;
    }
    exitus[COMMUNIO_CODEX_MENSURA] = '\0';
    memset(o, ZEPHYRUM, magnitudo(o));
    redde VERUM;
}

/* codex ingressus normatur: lineolae et spatia omissa, maiusculae;
 * FALSUM si non VIII signa Crockford */
interior b32
_codicem_normare (
        chorda  codex,
     character* exitus)
{
    i32 i;
    i32 n;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < codex.mensura; i++)
    {
        i8 c = codex.datum[i];

        si (c == '-' || c == ' ')
        {
            perge;
        }
        si (c >= 'a' && c <= 'z')
        {
            c = (i8)(c - 'a' + 'A');
        }
        si (   n >= COMMUNIO_CODEX_MENSURA
            || !strchr(CROCKFORD, (integer)c)
            || c == '\0')
        {
            redde FALSUM;
        }
        exitus[n++] = (character)c;
    }
    exitus[n] = '\0';
    redde n == COMMUNIO_CODEX_MENSURA;
}

b32
communio_aditum_petere (
    CommunioServus* servus,
            chorda  epistula,
            chorda  digestum,
            chorda  titulus,
    CommunioAditus* exitus)
{
    ScriniumEnuntiatum* e;
        PiscinaNotatio  nota;
             character  hex_epistulae[SIGILLUM_HEX_MENSURA];
             character  hex_codicis[SIGILLUM_HEX_MENSURA];
             character  hex_signi[SIGILLUM_HEX_MENSURA];
                    i8  octeti[SIGNUM_OCTETI];
                chorda  civis;
                chorda  spatium;
                   b32  bene;
                   b32  propria;
                   i32  n;
                   s64  nunc;

    si (!servus || !exitus)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(*exitus));
    moneta_ulid(exitus->id);
    si (   !_epistulam_normare(epistula, exitus->epistula)
        || !_digestum_validum(digestum) || titulus.mensura == ZEPHYRUM
        || titulus.mensura > LXIV)
    {
        redde VERUM;
    }
    nota = piscina_notare(servus->tempus);
    nunc = _nunc(servus);
    /* aditus expirati olim deleti (tabula parva manet) */
    e = scrinium_praeparare(servus->scrinium,
        "DELETE FROM communio_aditus WHERE expirat_ms < ?");
    si (e)
    {
        scrinium_ligare_numerum(e, I, nunc - (s64)XXIV * LX * LX * M);
        (vacuum)scrinium_gradi(e);
        scrinium_finire(e);
    }
    e = scrinium_praeparare(servus->scrinium,
        "SELECT civis, spatium FROM communio_permissi"
        " WHERE epistula = ? AND revocatus = 0");
    bene           = e != NIHIL;
    civis.datum    = NIHIL;
    civis.mensura  = ZEPHYRUM;
    spatium        = civis;
    si (e)
    {
        scrinium_ligare_textum(e, I, _chorda_buffer(exitus->epistula));
        bene = scrinium_gradi(e) == SCRINIUM_ORDO;
        si (bene)
        {
            civis = scrinium_columna_textus(e, ZEPHYRUM,
                servus->tempus);
            spatium = scrinium_columna_textus(e, I, servus->tempus);
        }
        scrinium_finire(e);
    }
    bene = bene && _persona_activa(servus, civis)
        && _membrum_est(servus, spatium, civis);
    /* digestum iam apparatus alicuius: non permissus */
    si (bene)
    {
        e = scrinium_praeparare(servus->scrinium,
            "SELECT COUNT(*) FROM communio_apparatus"
            " WHERE digestum = ?");
        n = (i32)I;
        si (e)
        {
            scrinium_ligare_textum(e, I, digestum);
            si (scrinium_gradi(e) == SCRINIUM_ORDO)
            {
                n = (i32)scrinium_columna_numerus(e, ZEPHYRUM);
            }
            scrinium_finire(e);
        }
        bene = n == ZEPHYRUM;
    }
    bene = bene && _codicem_cudere(exitus->codex)
        && moneta_octeti_fortuiti(octeti, SIGNUM_OCTETI)
        && base64url_scribere(octeti, SIGNUM_OCTETI, exitus->signum,
               XLIV) == XLIII;
    memset(octeti, ZEPHYRUM, magnitudo(octeti));
    si (bene)
    {
        exitus->signum[XLIII] = '\0';
        _sigillum_textus(exitus->epistula, hex_epistulae);
        _sigillum_textus(exitus->codex, hex_codicis);
        _sigillum_textus(exitus->signum, hex_signi);
        bene = _incipere(servus, &propria);
    }
    si (bene)
    {
        e = scrinium_praeparare(servus->scrinium,
            "INSERT INTO communio_aditus (id, epistula, civis, spatium,"
            " digestum, titulus, codex, signum, creatus_ms, expirat_ms)"
            " VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        bene = e != NIHIL;
        si (e)
        {
            scrinium_ligare_textum(e, I, _chorda_buffer(exitus->id));
            scrinium_ligare_textum(e, II,
                _chorda_buffer(hex_epistulae));
            scrinium_ligare_textum(e, III, civis);
            scrinium_ligare_textum(e, IV, spatium);
            scrinium_ligare_textum(e, V, digestum);
            scrinium_ligare_textum(e, VI, titulus);
            scrinium_ligare_textum(e, VII, _chorda_buffer(hex_codicis));
            scrinium_ligare_textum(e, VIII, _chorda_buffer(hex_signi));
            scrinium_ligare_numerum(e, IX, nunc);
            scrinium_ligare_numerum(e, X, nunc
                + COMMUNIO_ADITUS_VITA_MS);
            bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
            scrinium_finire(e);
        }
        bene = _finire(servus, propria, bene);
    }
    memset(hex_codicis, ZEPHYRUM, magnitudo(hex_codicis));
    memset(hex_signi, ZEPHYRUM, magnitudo(hex_signi));
    si (bene)
    {
        exitus->permissus = VERUM;
    }
    alioquin
    {
        memset(exitus->codex, ZEPHYRUM, magnitudo(exitus->codex));
        memset(exitus->signum, ZEPHYRUM, magnitudo(exitus->signum));
    }
    piscina_reficere(servus->tempus, nota);
    redde VERUM;
}

s32
communio_aditum_confirmare (
    CommunioServus* servus,
            chorda  id,
            chorda  codex,
            chorda  signum,
         character* apparatus_exitus)
{
    ScriniumEnuntiatum* e;
        PiscinaNotatio  nota;
             character  norma[X];
             character  textus_signi[XLIV];
             character  hex[SIGILLUM_HEX_MENSURA];
             character  novum[MONETA_ULID_MENSURA];
                chorda  servatum;
                chorda  epistula;
                chorda  civis;
                chorda  spatium;
                chorda  digestum;
                chorda  titulus;
                chorda  apparatus;
                   s64  expirat;
                   s64  conatus;
                   b32  confirmatus;
                   b32  per_codicem;
                   b32  bene;
                   b32  propria;
                   b32  permissio;
                   s32  exitus;
             JsonValor* o;

    si (   !servus || !apparatus_exitus
        || (codex.mensura > ZEPHYRUM) == (signum.mensura > ZEPHYRUM))
    {
        redde -I;
    }
    per_codicem = codex.mensura > ZEPHYRUM;
    si (per_codicem)
    {
        si (!_codicem_normare(codex, norma))
        {
            norma[ZEPHYRUM] = '\0';
        }
        _sigillum_textus(norma, hex);
    }
    alioquin
    {
        si (signum.mensura != XLIII)
        {
            redde -I;
        }
        memcpy(textus_signi, signum.datum, (size_t)XLIII);
        textus_signi[XLIII] = '\0';
        _sigillum_textus(textus_signi, hex);
        memset(textus_signi, ZEPHYRUM, magnitudo(textus_signi));
    }
    nota    = piscina_notare(servus->tempus);
    exitus  = -I;
    e = scrinium_praeparare(servus->scrinium,
        per_codicem
        ? "SELECT codex, epistula, civis, spatium, digestum, titulus,"
          " expirat_ms, conatus, confirmatus, COALESCE(apparatus, '')"
          " FROM communio_aditus WHERE id = ?"
        : "SELECT signum, epistula, civis, spatium, digestum, titulus,"
          " expirat_ms, conatus, confirmatus, COALESCE(apparatus, '')"
          " FROM communio_aditus WHERE id = ?");
    si (!e)
    {
        piscina_reficere(servus->tempus, nota);
        redde -I;
    }
    scrinium_ligare_textum(e, I, id);
    si (scrinium_gradi(e) != SCRINIUM_ORDO)
    {
        scrinium_finire(e);
        piscina_reficere(servus->tempus, nota);
        redde -I;
    }
    servatum     = scrinium_columna_textus(e, ZEPHYRUM,
        servus->tempus);
    epistula     = scrinium_columna_textus(e, I, servus->tempus);
    civis        = scrinium_columna_textus(e, II, servus->tempus);
    spatium      = scrinium_columna_textus(e, III, servus->tempus);
    digestum     = scrinium_columna_textus(e, IV, servus->tempus);
    titulus      = scrinium_columna_textus(e, V, servus->tempus);
    expirat      = scrinium_columna_numerus(e, VI);
    conatus      = scrinium_columna_numerus(e, VII);
    confirmatus  = scrinium_columna_numerus(e, VIII) != ZEPHYRUM;
    apparatus    = scrinium_columna_textus(e, IX, servus->tempus);
    scrinium_finire(e);
    bene = expirat >= _nunc(servus)
        && conatus < COMMUNIO_ADITUS_CONATUS;
    si (bene && !_aequale_constans(servatum, _chorda_buffer(hex)))
    {
        /* secretum falsum: conatus crescit */
        e = scrinium_praeparare(servus->scrinium,
            "UPDATE communio_aditus SET conatus = conatus + 1"
            " WHERE id = ?");
        si (e)
        {
            scrinium_ligare_textum(e, I, id);
            (vacuum)scrinium_gradi(e);
            scrinium_finire(e);
        }
        bene = FALSUM;
    }
    si (bene && confirmatus)
    {
        si (apparatus.mensura == XXVI)
        {
            memcpy(apparatus_exitus, apparatus.datum, (size_t)XXVI);
            apparatus_exitus[XXVI]  = '\0';
            exitus                  = I;
        }
        bene = FALSUM;
    }
    si (bene)
    {
        /* index et persona etiam nunc valent? */
        e = scrinium_praeparare(servus->scrinium,
            "SELECT 1 FROM communio_permissi WHERE digestum = ?"
            " AND revocatus = 0 AND civis = ? AND spatium = ?");
        permissio = FALSUM;
        si (e)
        {
            scrinium_ligare_textum(e, I, epistula);
            scrinium_ligare_textum(e, II, civis);
            scrinium_ligare_textum(e, III, spatium);
            permissio = scrinium_gradi(e) == SCRINIUM_ORDO;
            scrinium_finire(e);
        }
        bene = permissio && _persona_activa(servus, civis)
            && _incipere(servus, &propria);
    }
    si (bene)
    {
        bene = communio_apparatum_addere(servus, civis, spatium,
            titulus,
            digestum, novum);
        si (bene)
        {
            e = scrinium_praeparare(servus->scrinium,
                "UPDATE communio_aditus SET confirmatus = 1,"
                " apparatus = ? WHERE id = ? AND confirmatus = 0");
            bene = e != NIHIL;
            si (e)
            {
                scrinium_ligare_textum(e, I, _chorda_buffer(novum));
                scrinium_ligare_textum(e, II, id);
                bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
                scrinium_finire(e);
            }
        }
        o = json_objectum_creare(servus->tempus);
        si (bene && o)
        {
            json_objectum_ponere(o, "id", json_chorda_creare(
                servus->tempus, id));
            json_objectum_ponere(o, "apparatus",
                json_chorda_creare_literis(servus->tempus, novum));
        }
        bene = _finire(servus, propria, bene && _actum(servus,
            "communio.aditus-confirmatus", o));
        si (bene)
        {
            memcpy(apparatus_exitus, novum,
                (size_t)MONETA_ULID_MENSURA);
            exitus = ZEPHYRUM;
        }
    }
    memset(hex, ZEPHYRUM, magnitudo(hex));
    memset(norma, ZEPHYRUM, magnitudo(norma));
    piscina_reficere(servus->tempus, nota);
    redde exitus;
}
