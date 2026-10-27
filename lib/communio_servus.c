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
        Exspectans  exspectantes[EXSPECTANTES_MAXIMI];
};

hic_manens constans character* constans SCHEMA[] = {
    "CREATE TABLE IF NOT EXISTS communio_membra"
    " (spatium TEXT NOT NULL, civis TEXT NOT NULL,"
    " PRIMARY KEY (spatium, civis))",
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
               _acta_get, servus);
}
