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

structura CommunioServus {
          Piscina* piscina;
          Piscina* tempus;      /* scripturae: notare/reficere */
          Volumen* volumen;
         Scrinium* scrinium;
            Cives* cives;
               s64 (*nunc_ms)(vacuum);
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

vacuum
communio_servum_destruere (
    CommunioServus* servus)
{
    si (servus && servus->tempus)
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
        bene = _persona_activa(servus, exitus->civis);
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
        piscina_reficere(servus->tempus, nota);
        redde chorda_aequalis(praesens, replica) ? ZEPHYRUM : I;
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
