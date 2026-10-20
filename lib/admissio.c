/* admissio.c - servus auctoritatis OAuth 2.1 (vide include/admissio.h)
 *
 * TABULAE (CREATE IF NOT EXISTS - scrinium_migrare numeratorem unum
 * cum volumine communicat, ergo admissio schemata sua ipse ponit et
 * versionem in admissio_versio servat):
 *
 *   admissio_clientes      id, titulus, redirectiones (JSON), ip,
 *                          creatus_ms
 *   admissio_concessiones  id (ULID), cliens, usor, scopi, audientia,
 *                          creata_ms, usus_ultimus_ms, revocata
 *   admissio_codices       digestum, ... (T6)
 *   admissio_tesserae      digestum (SHA-256 hex), genus (0 accessus,
 *                          I renovatio), concessio, familia,
 *                          expirat_ms, usa
 *
 * Secretum in claro NUMQUAM conditur: tessera -> sigillum -> hex.
 * Quaestio per digestum (clavis), deinde comparatio constans.
 */

#include "postulata_posix.h"
#define ADMISSIO_PROBATIO
#include "admissio.h"
#include "base64.h"
#include "chorda_aedificator.h"
#include "json.h"
#include "moneta.h"
#include "scrinium.h"
#include "sigillum.h"
#include "url.h"

#include <string.h>
#include <sys/time.h>

#define GENUS_ACCESSUS    ZEPHYRUM
#define GENUS_RENOVATIONIS I
#define MINUTUM_MS        ((s64)LX * M)
#define DIES_MS           ((s64)XXIV * LX * LX * M)

structura Admissio {
             Volumen* volumen;
            Scrinium* scrinium;
    AdmissioOptiones  o;
             Piscina* piscina;
    /* viae (admissio_montare) */
     AdmissioIntratus  intratus;
               vacuum* datum_intratus;
            character* via_intrandi;
            character* origo;           /* origo auctoritatis */
                vacuum  (*speculator)(vacuum* datum,
                            HospitiumColloquium* colloquium);
                vacuum* speculatoris_datum;
};

interior vacuum
_speculari (
               Admissio* a,
    HospitiumColloquium* c);

hic_manens constans character* constans SCHEMA[] = {
    "CREATE TABLE IF NOT EXISTS admissio_versio"
    " (versio INTEGER NOT NULL)",
    "CREATE TABLE IF NOT EXISTS admissio_clientes"
    " (id TEXT PRIMARY KEY, titulus TEXT NOT NULL,"
    " redirectiones TEXT NOT NULL, ip TEXT NOT NULL,"
    " creatus_ms INTEGER NOT NULL)",
    "CREATE INDEX IF NOT EXISTS admissio_clientes_ip"
    " ON admissio_clientes (ip, creatus_ms)",
    "CREATE TABLE IF NOT EXISTS admissio_concessiones"
    " (id TEXT PRIMARY KEY, cliens TEXT NOT NULL, usor TEXT NOT NULL,"
    " scopi TEXT NOT NULL, audientia TEXT NOT NULL,"
    " creata_ms INTEGER NOT NULL, usus_ultimus_ms INTEGER NOT NULL,"
    " revocata INTEGER NOT NULL DEFAULT 0)",
    "CREATE TABLE IF NOT EXISTS admissio_codices"
    " (digestum TEXT PRIMARY KEY, cliens TEXT NOT NULL,"
    " redirectio TEXT NOT NULL, provocatio TEXT NOT NULL,"
    " audientia TEXT NOT NULL, usor TEXT NOT NULL, scopi TEXT NOT NULL,"
    " expirat_ms INTEGER NOT NULL, usus INTEGER NOT NULL DEFAULT 0,"
    " concessio TEXT)",
    "CREATE TABLE IF NOT EXISTS admissio_tesserae"
    " (digestum TEXT PRIMARY KEY, genus INTEGER NOT NULL,"
    " concessio TEXT NOT NULL, familia TEXT NOT NULL,"
    " expirat_ms INTEGER NOT NULL, usa INTEGER NOT NULL DEFAULT 0)",
    "CREATE INDEX IF NOT EXISTS admissio_tesserae_concessio"
    " ON admissio_tesserae (concessio)",
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
    Admissio* a)
{
    redde a->o.nunc_ms ? a->o.nunc_ms() : _tempus_verum_ms();
}

interior character*
_literae (
                Piscina* p,
     constans character* s)
{
    memoriae_index  n = (memoriae_index)strlen(s);
         character* t = (character*)piscina_allocare(p, n + I);

    memcpy(t, s, (size_t)n + I);
    redde t;
}

interior chorda
_ch (
    constans character* s)
{
    chorda c;

    c.datum    = (i8*)(vacuum*)(memoriae_index)s;
    c.mensura  = (i32)strlen(s);
    redde c;
}

/* tessera -> SHA-256 hex (LXIV signa) */
interior chorda
_digestum (
      chorda  secretum,
     Piscina* p)
{
     Sigillum s = sigillum_computare(secretum.datum,
         (memoriae_index)secretum.mensura);
    character* hex = (character*)piscina_allocare(p,
        SIGILLUM_HEX_MENSURA);

    sigillum_hex(&s, hex);
    redde chorda_ex_buffer((i8*)hex, LXIV);
}

/* comparatio temporis constantis */
interior b32
_aequale_constans (
    chorda a,
    chorda b)
{
     i8 diff = ZEPHYRUM;
    i32 i;

    si (a.mensura != b.mensura)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < a.mensura; i++)
    {
        diff = (i8)(diff | (a.datum[i] ^ b.datum[i]));
    }
    redde diff == ZEPHYRUM;
}

/* n octeti fortuiti -> base64url; vacua si fons deficit */
interior chorda
_fortuitum (
         i32  n,
     Piscina* p)
{
        i8 o[XXXII];
    chorda t;

    t.datum    = NIHIL;
    t.mensura  = ZEPHYRUM;
    si (n > XXXII || !moneta_octeti_fortuiti(o, n))
    {
        redde t;
    }
    t = base64url_codificare(o, n, p);
    memset(o, ZEPHYRUM, magnitudo(o));
    redde t;
}

interior chorda
_error (
                Piscina* p,
     constans character* codex,
     constans character* descriptio)
{
    JsonValor* e = json_objectum_creare(p);

    json_objectum_ponere(e, "error", json_chorda_creare_literis(p,
        codex));
    json_objectum_ponere(e, "error_description",
        json_chorda_creare_literis(p, descriptio));
    redde json_scribere(e, p);
}

/* SELECT unius numeri: ligamina textus (I) et, si numerus_adest,
 * numeri (II); -I si vitium */
interior s64
_numerus (
              Admissio* a,
    constans character* sql,
                chorda  textus,
                   b32  numerus_adest,
                   s64  numerus)
{
     ScriniumEnuntiatum* e = scrinium_praeparare(a->scrinium, sql);
                    s64  n = -I;

    si (!e)
    {
        redde -I;
    }
    scrinium_ligare_textum(e, I, textus);
    si (numerus_adest)
    {
        scrinium_ligare_numerum(e, II, numerus);
    }
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        n = scrinium_columna_numerus(e, ZEPHYRUM);
    }
    scrinium_finire(e);
    redde n;
}

interior vacuum
_actum (
               Admissio* a,
     constans character* genus,
                 chorda  concessio,
                 chorda  usor,
                 chorda  cliens,
                 chorda  scopi,
                Piscina* p)
{
    JsonValor* d = json_objectum_creare(p);

    json_objectum_ponere(d, "concessio", json_chorda_creare(p,
        concessio));
    json_objectum_ponere(d, "usor", json_chorda_creare(p, usor));
    json_objectum_ponere(d, "cliens", json_chorda_creare(p, cliens));
    /* creata: nomen clientis et scopi (vestigium legibile) */
    si (scopi.datum)
    {
        ScriniumEnuntiatum* e = scrinium_praeparare(a->scrinium,
            "SELECT titulus FROM admissio_clientes WHERE id = ?");

        si (e)
        {
            scrinium_ligare_textum(e, I, cliens);
            si (scrinium_gradi(e) == SCRINIUM_ORDO)
            {
                json_objectum_ponere(d, "titulus_clientis",
                    json_chorda_creare(p, scrinium_columna_textus(e,
                        ZEPHYRUM, p)));
            }
            scrinium_finire(e);
        }
        json_objectum_ponere(d, "scopi", json_chorda_creare(p, scopi));
    }
    (vacuum)volumen_actum_appendere(a->volumen, genus, json_scribere(d,
        p));
}


/* ==================================================
 * creatio et metadata
 * ================================================== */

Admissio*
admissio_creare (
                    Volumen* volumen,
    constans AdmissioOptiones* optiones,
                    Piscina* piscina)
{
      Admissio*  a;
           i32   k;
           i32   n = ZEPHYRUM;
     character** scopi;

    si (   !volumen || !optiones || !piscina || !optiones->auctoritas
        || !optiones->audientia || !optiones->scopi)
    {
        redde NIHIL;
    }
    a = (Admissio*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)magnitudo(Admissio),
        (memoriae_index)magnitudo(vacuum*));
    si (!a)
    {
        redde NIHIL;
    }
    memset(a, ZEPHYRUM, magnitudo(*a));
    a->volumen       = volumen;
    a->scrinium      = volumen_scrinium(volumen);
    a->piscina       = piscina;
    a->o             = *optiones;
    a->o.auctoritas  = _literae(piscina, optiones->auctoritas);
    a->o.audientia   = _literae(piscina, optiones->audientia);
    dum (optiones->scopi[n])
    {
        n++;
    }
    scopi = (character**)piscina_allocare_ordinatum(piscina,
        (memoriae_index)(magnitudo(character*) * (memoriae_index)(n
            + I)),
        (memoriae_index)magnitudo(vacuum*));
    per (k = ZEPHYRUM; k < n; k++)
    {
        scopi[k] = _literae(piscina, optiones->scopi[k]);
    }
    scopi[n]    = NIHIL;
    a->o.scopi  = (constans character* constans*)scopi;
    si (a->o.vita_codicis_ms <= ZEPHYRUM)
    {
        a->o.vita_codicis_ms = MINUTUM_MS;
    }
    si (a->o.vita_accessus_ms <= ZEPHYRUM)
    {
        a->o.vita_accessus_ms = (s64)LX * MINUTUM_MS;
    }
    si (a->o.vita_renovationis_ms <= ZEPHYRUM)
    {
        a->o.vita_renovationis_ms = (s64)XXX * DIES_MS;
    }
    si (!a->scrinium || !scrinium_incipere(a->scrinium))
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; SCHEMA[k]; k++)
    {
        si (!scrinium_exsequi(a->scrinium, SCHEMA[k]))
        {
            (vacuum)scrinium_revolvere(a->scrinium);
            redde NIHIL;
        }
    }
    si (   _numerus(a,
        "SELECT COUNT(*) FROM admissio_versio WHERE ? = ''",
               _ch(""), FALSUM, ZEPHYRUM) == ZEPHYRUM
        && !scrinium_exsequi(a->scrinium,
               "INSERT INTO admissio_versio (versio) VALUES (1)"))
    {
        (vacuum)scrinium_revolvere(a->scrinium);
        redde NIHIL;
    }
    si (!scrinium_committere(a->scrinium))
    {
        redde NIHIL;
    }
    redde a;
}

interior chorda
_via (
                Piscina* p,
               Admissio* a,
     constans character* finis)
{
    ChordaAedificator* t = chorda_aedificator_creare(p, CXXVIII);

    chorda_aedificator_appendere_literis(t, a->o.auctoritas);
    chorda_aedificator_appendere_literis(t, finis);
    redde chorda_aedificator_finire(t);
}

interior JsonValor*
_tabula (
                Piscina* p,
     constans character* primum,
     constans character* secundum)
{
    JsonValor* t = json_tabulatum_creare(p);

    json_tabulatum_addere(t, json_chorda_creare_literis(p, primum));
    si (secundum)
    {
        json_tabulatum_addere(t, json_chorda_creare_literis(p,
            secundum));
    }
    redde t;
}

chorda
admissio_documentum (
    Admissio* admissio,
     Piscina* piscina)
{
    JsonValor* d;
    JsonValor* scopi;
          i32  k;
       chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (!admissio || !piscina)
    {
        redde vacua;
    }
    d      = json_objectum_creare(piscina);
    scopi  = json_tabulatum_creare(piscina);
    per (k = ZEPHYRUM; admissio->o.scopi[k]; k++)
    {
        json_tabulatum_addere(scopi, json_chorda_creare_literis(piscina,
            admissio->o.scopi[k]));
    }
    json_tabulatum_addere(scopi, json_chorda_creare_literis(piscina,
        "offline_access"));
    json_objectum_ponere(d, "issuer",
        json_chorda_creare_literis(piscina,
        admissio->o.auctoritas));
    json_objectum_ponere(d, "authorization_endpoint",
        json_chorda_creare(
        piscina, _via(piscina, admissio, "/oauth/authorize")));
    json_objectum_ponere(d, "token_endpoint",
        json_chorda_creare(piscina,
        _via(piscina, admissio, "/oauth/token")));
    json_objectum_ponere(d, "registration_endpoint", json_chorda_creare(
        piscina, _via(piscina, admissio, "/oauth/register")));
    json_objectum_ponere(d, "response_types_supported", _tabula(piscina,
        "code", NIHIL));
    json_objectum_ponere(d, "grant_types_supported", _tabula(piscina,
        "authorization_code", "refresh_token"));
    json_objectum_ponere(d, "code_challenge_methods_supported", _tabula(
        piscina, "S256", NIHIL));
    json_objectum_ponere(d, "token_endpoint_auth_methods_supported",
        _tabula(piscina, "none", NIHIL));
    json_objectum_ponere(d, "scopes_supported", scopi);
    json_objectum_ponere(d,
        "authorization_response_iss_parameter_supported",
        json_boolean_creare(piscina, VERUM));
    redde json_scribere(d, piscina);
}


/* ==================================================
 * inscriptio dynamica (RFC 7591)
 * ================================================== */

interior b32
_incipit (
                 chorda  c,
     constans character* praefixum)
{
    i32 n = (i32)strlen(praefixum);

    redde c.mensura >= n && memcmp(c.datum, praefixum, (size_t)n)
        == ZEPHYRUM;
}

/* https exacta (hospes non vacuus), aut http loopback quovis portu;
 * fragmentum numquam */
interior b32
_redirectio_valida (
    chorda r)
{
    i32 i;
    i32 initium;
    i32 finis;

    si (r.mensura == ZEPHYRUM || r.mensura > MMXLVIII)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < r.mensura; i++)
    {
        si (r.datum[i] == '#' || r.datum[i] < ' ')
        {
            redde FALSUM;
        }
    }
    si (_incipit(r, "https://"))
    {
        redde r.mensura > VIII && r.datum[VIII] != '/'
            && r.datum[VIII] != ':';
    }
    si (!_incipit(r, "http://"))
    {
        redde FALSUM;
    }
    initium  = VII;
    finis    = initium;
    dum (   finis < r.mensura && r.datum[finis] != ':'
         && r.datum[finis] != '/')
    {
        finis++;
    }
    si (   !(finis - initium == IX
             && memcmp(r.datum + initium, "127.0.0.1", IX) == ZEPHYRUM)
        && !(finis - initium == IX
             && memcmp(r.datum + initium, "localhost", IX) == ZEPHYRUM))
    {
        redde FALSUM;
    }
    si (finis < r.mensura && r.datum[finis] == ':')
    {
        i = finis + I;
        dum (i < r.mensura && r.datum[i] >= '0' && r.datum[i] <= '9')
        {
            i++;
        }
        si (i == finis + I || (i < r.mensura && r.datum[i] != '/'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* tabula chordarum quarum quaeque in licitis; absens = VERUM */
interior b32
_intra_licita (
              JsonValor* t,
     constans character* primum,
     constans character* secundum)
{
    i32 k;

    si (!t)
    {
        redde VERUM;
    }
    si (!json_est_tabulatum(t) || json_tabulatum_numerus(t) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < json_tabulatum_numerus(t); k++)
    {
        JsonValor* v = json_tabulatum_obtinere(t, k);
           chorda  c = json_ad_chorda(v);

        si (   !json_est_chorda(v)
            || !(chorda_aequalis_literis(c, primum)
                 || (secundum && chorda_aequalis_literis(c, secundum))))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

s32
admissio_inscribere (
    Admissio* admissio,
       chorda  corpus,
       chorda  cliens_ip,
      chorda* responsum,
     Piscina* piscina)
{
          JsonResultus  r;
             JsonValor* uris;
             JsonValor* modus;
             JsonValor* titulus_v;
             JsonValor* genera_concessionis;
             JsonValor* resp;
                chorda  titulus;
                chorda  id;
                   s64  nunc;
                   i32  k;
    ScriniumEnuntiatum* e;
                   b32  bene;

    si (!admissio || !responsum || !piscina)
    {
        redde D;
    }
    nunc = _nunc(admissio);
    /* frenum ANTE omnia (vilis) */
    si (_numerus(admissio, "SELECT COUNT(*) FROM admissio_clientes"
            " WHERE ip = ? AND creatus_ms > ?", cliens_ip, VERUM,
            nunc - MINUTUM_MS) >= ADMISSIO_INSCRIPTIONES_PER_MINUTUM)
    {
        *responsum = _error(piscina, "too_many_requests",
            "nimis multae inscriptiones");
        redde CDXXIX;
    }
    r = json_legere(corpus, piscina);
    si (!r.successus || !json_est_objectum(r.radix))
    {
        *responsum = _error(piscina, "invalid_client_metadata",
            "corpus objectum JSON esse debet");
        redde CD;
    }
    uris = json_objectum_capere(r.radix, "redirect_uris");
    si (   !uris || !json_est_tabulatum(uris)
        || json_tabulatum_numerus(uris) == ZEPHYRUM)
    {
        *responsum = _error(piscina, "invalid_redirect_uri",
            "redirect_uris requiruntur");
        redde CD;
    }
    per (k = ZEPHYRUM; k < json_tabulatum_numerus(uris); k++)
    {
        JsonValor* u = json_tabulatum_obtinere(uris, k);

        si (   !json_est_chorda(u)
            || !_redirectio_valida(json_ad_chorda(u)))
        {
            *responsum = _error(piscina, "invalid_redirect_uri",
                "redirectio https exacta aut loopback http requiritur");
            redde CD;
        }
    }
    modus = json_objectum_capere(r.radix, "token_endpoint_auth_method");
    si (   modus
        && !(json_est_chorda(modus)
             && chorda_aequalis_literis(json_ad_chorda(modus), "none")))
    {
        *responsum = _error(piscina, "invalid_client_metadata",
            "token_endpoint_auth_method none solum");
        redde CD;
    }
    genera_concessionis = json_objectum_capere(r.radix, "grant_types");
    si (   !_intra_licita(genera_concessionis, "authorization_code",
        "refresh_token")
        || !_intra_licita(json_objectum_capere(r.radix,
        "response_types"),
               "code", NIHIL))
    {
        *responsum = _error(piscina, "invalid_client_metadata",
            "grant_types / response_types non sustentata");
        redde CD;
    }
    titulus_v  = json_objectum_capere(r.radix, "client_name");
    titulus    = json_est_chorda(titulus_v) ? json_ad_chorda(titulus_v)
        : _ch("");
    si (titulus.mensura > CXXVIII)
    {
        *responsum = _error(piscina, "invalid_client_metadata",
            "client_name nimis longum");
        redde CD;
    }
    id = _fortuitum(XVI, piscina);
    si (id.mensura == ZEPHYRUM)
    {
        *responsum = _error(piscina, "server_error", "fons fortuitus");
        redde D;
    }
    e = scrinium_praeparare(admissio->scrinium,
        "INSERT INTO admissio_clientes (id, titulus, redirectiones, ip,"
        " creatus_ms) VALUES (?, ?, ?, ?, ?)");
    bene = e != NIHIL;
    si (bene)
    {
        scrinium_ligare_textum(e, I, id);
        scrinium_ligare_textum(e, II, titulus);
        scrinium_ligare_textum(e, III, json_scribere(uris, piscina));
        scrinium_ligare_textum(e, IV, cliens_ip);
        scrinium_ligare_numerum(e, V, nunc);
        bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
        scrinium_finire(e);
    }
    si (!bene)
    {
        *responsum = _error(piscina, "server_error", "scrinium");
        redde D;
    }
    resp = json_objectum_creare(piscina);
    json_objectum_ponere(resp, "client_id", json_chorda_creare(piscina,
        id));
    json_objectum_ponere(resp, "client_id_issued_at",
        json_integer_creare(
        piscina, nunc / M));
    json_objectum_ponere(resp, "client_name",
        json_chorda_creare(piscina,
        titulus));
    json_objectum_ponere(resp, "redirect_uris", uris);
    json_objectum_ponere(resp, "grant_types",
        genera_concessionis ? genera_concessionis
        : _tabula(piscina, "authorization_code", "refresh_token"));
    json_objectum_ponere(resp, "response_types", _tabula(piscina,
        "code",
        NIHIL));
    json_objectum_ponere(resp, "token_endpoint_auth_method",
        json_chorda_creare_literis(piscina, "none"));
    *responsum = json_scribere(resp, piscina);
    redde CCI;
}


/* ==================================================
 * concessiones et tesserae
 * ================================================== */

interior b32
_tesseram_condere (
       Admissio* a,
            s32  genus,
         chorda  concessio,
            s64  expirat,
         chorda* exitus,
        Piscina* p)
{
                chorda  t = _fortuitum(XXXII, p);
    ScriniumEnuntiatum* e;
                   b32  bene;

    si (t.mensura != ADMISSIO_TESSERA_MENSURA)
    {
        redde FALSUM;
    }
    e = scrinium_praeparare(a->scrinium,
        "INSERT INTO admissio_tesserae (digestum, genus, concessio,"
        " familia, expirat_ms) VALUES (?, ?, ?, ?, ?)");
    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, I, _digestum(t, p));
    scrinium_ligare_numerum(e, II, (s64)genus);
    scrinium_ligare_textum(e, III, concessio);
    scrinium_ligare_textum(e, IV, concessio);
    scrinium_ligare_numerum(e, V, expirat);
    bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
    scrinium_finire(e);
    si (bene)
    {
        *exitus = t;
    }
    redde bene;
}

/* concessio nova + tesserae (accessus, renovatio); T6 per codicem */
interior b32
_concessionem_creare (
       Admissio* a,
         chorda  usor,
         chorda  cliens,
         chorda  scopi,
            b32  cum_renovatione,
         chorda* id_exitus,
         chorda* accessus,
         chorda* renovatio,
        Piscina* p)
{
                   s64  nunc = _nunc(a);
             character  ulid[SCRINIUM_ULID_MENSURA];
                chorda  id;
    ScriniumEnuntiatum* e;
                   b32  bene;

    si (_numerus(a,
        "SELECT COUNT(*) FROM admissio_clientes WHERE id = ?",
            cliens, FALSUM, ZEPHYRUM) <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (!cum_renovatione)
    {
        renovatio->datum    = NIHIL;
        renovatio->mensura  = ZEPHYRUM;
    }
    scrinium_ulid(ulid);
    id = chorda_transcribere(_ch(ulid), p);
    si (!scrinium_incipere(a->scrinium))
    {
        redde FALSUM;
    }
    e = scrinium_praeparare(a->scrinium,
        "INSERT INTO admissio_concessiones (id, cliens, usor, scopi,"
        " audientia, creata_ms, usus_ultimus_ms) VALUES (?, ?, ?, ?, ?,"
        " ?, ?)");
    bene = e != NIHIL;
    si (bene)
    {
        scrinium_ligare_textum(e, I, id);
        scrinium_ligare_textum(e, II, cliens);
        scrinium_ligare_textum(e, III, usor);
        scrinium_ligare_textum(e, IV, scopi);
        scrinium_ligare_textum(e, V, _ch(a->o.audientia));
        scrinium_ligare_numerum(e, VI, nunc);
        scrinium_ligare_numerum(e, VII, nunc);
        bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
        scrinium_finire(e);
    }
    bene = bene
        && _tesseram_condere(a, GENUS_ACCESSUS, id,
               nunc + a->o.vita_accessus_ms, accessus, p)
        && (!cum_renovatione
            || _tesseram_condere(a, GENUS_RENOVATIONIS, id,
                   nunc + a->o.vita_renovationis_ms, renovatio, p));
    si (!bene)
    {
        (vacuum)scrinium_revolvere(a->scrinium);
        redde FALSUM;
    }
    si (!scrinium_committere(a->scrinium))
    {
        redde FALSUM;
    }
    _actum(a, "concessio.creata", id, usor, cliens, scopi, p);
    si (id_exitus)
    {
        *id_exitus = id;
    }
    redde VERUM;
}

b32
admissio_concessionem_creare_probationis (
     Admissio* admissio,
       chorda  usor,
       chorda  cliens,
       chorda  scopi,
       chorda* accessus,
       chorda* renovatio,
      Piscina* piscina)
{
    si (!admissio || !accessus || !renovatio || !piscina)
    {
        redde FALSUM;
    }
    redde _concessionem_creare(admissio, usor, cliens, scopi, VERUM,
        NIHIL, accessus,
        renovatio, piscina);
}

b32
admissio_verificator (
        vacuum* admissio,
        chorda  tessera,
        chorda  audientia,
    McpPersona* exitus,
       Piscina* piscina)
{
               Admissio* a = (Admissio*)admissio;
                 chorda  digestum;
     ScriniumEnuntiatum* e;
                    b32  bene = FALSUM;
                 chorda  concessio;

    si (   !a || !exitus || !piscina
        || tessera.mensura != ADMISSIO_TESSERA_MENSURA)
    {
        redde FALSUM;
    }
    digestum = _digestum(tessera, piscina);
    e = scrinium_praeparare(a->scrinium,
        "SELECT t.digestum, t.genus, t.expirat_ms, g.revocata,"
        " g.audientia, g.usor, g.cliens, g.scopi, g.id FROM"
        " admissio_tesserae t JOIN admissio_concessiones g ON g.id ="
        " t.concessio WHERE t.digestum = ?");
    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, I, digestum);
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        bene = _aequale_constans(scrinium_columna_textus(e, ZEPHYRUM,
                   piscina), digestum)
            && scrinium_columna_numerus(e, I) == GENUS_ACCESSUS
            && _nunc(a) < scrinium_columna_numerus(e, II)
            && scrinium_columna_numerus(e, III) == ZEPHYRUM
            && chorda_aequalis(scrinium_columna_textus(e, IV, piscina),
                   audientia);
        si (bene)
        {
            exitus->usor    = scrinium_columna_textus(e, V, piscina);
            exitus->cliens  = scrinium_columna_textus(e, VI, piscina);
            exitus->scopi   = scrinium_columna_textus(e, VII, piscina);
            concessio       = scrinium_columna_textus(e, VIII, piscina);
        }
    }
    scrinium_finire(e);
    si (bene)
    {
        e = scrinium_praeparare(a->scrinium,
            "UPDATE admissio_concessiones SET usus_ultimus_ms = ? WHERE"
            " id = ?");
        si (e)
        {
            scrinium_ligare_numerum(e, I, _nunc(a));
            scrinium_ligare_textum(e, II, concessio);
            (vacuum)scrinium_gradi(e);
            scrinium_finire(e);
        }
    }
    redde bene;
}

Xar*
admissio_concessiones (
     Admissio* admissio,
       chorda  usor,
      Piscina* piscina)
{
                   Xar* lista;
    ScriniumEnuntiatum* e;

    si (!admissio || !piscina)
    {
        redde NIHIL;
    }
    lista = xar_creare(piscina, (i32)magnitudo(AdmissioConcessio));
    e = scrinium_praeparare(admissio->scrinium,
        "SELECT g.id, g.cliens, COALESCE(c.titulus, ''), g.usor,"
        " g.scopi, g.creata_ms, g.usus_ultimus_ms"
        " FROM admissio_concessiones g"
        " LEFT JOIN admissio_clientes c ON c.id = g.cliens"
        " WHERE g.usor = ? AND g.revocata = 0"
        " ORDER BY g.creata_ms, g.id");
    si (!lista || !e)
    {
        redde lista;
    }
    scrinium_ligare_textum(e, I, usor);
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        AdmissioConcessio* g = (AdmissioConcessio*)xar_addere(lista);

        si (!g)
        {
            frange;
        }
        g->id                = scrinium_columna_textus(e, ZEPHYRUM,
            piscina);
        g->cliens            = scrinium_columna_textus(e, I, piscina);
        g->titulus_clientis  = scrinium_columna_textus(e, II, piscina);
        g->usor              = scrinium_columna_textus(e, III, piscina);
        g->scopi             = scrinium_columna_textus(e, IV, piscina);
        g->creata_ms         = scrinium_columna_numerus(e, V);
        g->usus_ultimus_ms   = scrinium_columna_numerus(e, VI);
        g->revocata          = FALSUM;
    }
    scrinium_finire(e);
    redde lista;
}

/* concessionem necare: revocata + tesserae deletae + actum (sine
 * iudicio domini - vocans iudicavit) */
interior b32
_concessionem_necare (
     Admissio* a,
       chorda  concessio,
       chorda  usor,
       chorda  cliens,
      Piscina* p)
{
    ScriniumEnuntiatum* u;
    ScriniumEnuntiatum* d;
                   b32  bene;

    si (!scrinium_incipere(a->scrinium))
    {
        redde FALSUM;
    }
    u = scrinium_praeparare(a->scrinium,
        "UPDATE admissio_concessiones SET revocata = 1 WHERE id = ?");
    d = scrinium_praeparare(a->scrinium,
        "DELETE FROM admissio_tesserae WHERE concessio = ?");
    bene = u && d;
    si (bene)
    {
        scrinium_ligare_textum(u, I, concessio);
        scrinium_ligare_textum(d, I, concessio);
        bene = scrinium_gradi(u) == SCRINIUM_FACTUM
            && scrinium_gradi(d) == SCRINIUM_FACTUM;
    }
    si (u)
    {
        scrinium_finire(u);
    }
    si (d)
    {
        scrinium_finire(d);
    }
    si (!bene)
    {
        (vacuum)scrinium_revolvere(a->scrinium);
        redde FALSUM;
    }
    si (!scrinium_committere(a->scrinium))
    {
        redde FALSUM;
    }
    _actum(a, "concessio.revocata", concessio, usor, cliens,
        chorda_ex_buffer(NIHIL, ZEPHYRUM), p);
    redde VERUM;
}

/* concessio viva: usor et cliens; usor vacuus (datum NIHIL) = quivis */
interior b32
_concessio_viva (
     Admissio* a,
       chorda  concessio,
       chorda  usor,
       chorda* usor_exitus,
       chorda* cliens_exitus,
      Piscina* p)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(a->scrinium,
        "SELECT usor, cliens FROM admissio_concessiones"
        " WHERE id = ? AND revocata = 0");
                   b32 bene = FALSUM;

    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, I, concessio);
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        *usor_exitus = scrinium_columna_textus(e, ZEPHYRUM, p);
        *cliens_exitus = scrinium_columna_textus(e, I, p);
        bene = !usor.datum || chorda_aequalis(*usor_exitus, usor);
    }
    scrinium_finire(e);
    redde bene;
}

b32
admissio_revocare (
     Admissio* admissio,
       chorda  usor,
       chorda  concessio)
{
     Piscina* p;
      chorda  u;
      chorda  cliens;
         b32  bene;

    si (!admissio || !usor.datum)
    {
        redde FALSUM;
    }
    p = piscina_generare_dynamicum("admissio_revocare",
        (memoriae_index)4096);
    si (!p)
    {
        redde FALSUM;
    }
    bene = _concessio_viva(admissio, concessio, usor, &u, &cliens, p)
        && _concessionem_necare(admissio, concessio, u, cliens, p);
    piscina_destruere(p);
    redde bene;
}

s32
admissio_putare (
    Admissio* admissio)
{
                   s64  limes;
                   s64  n;
    ScriniumEnuntiatum* e;

    si (!admissio)
    {
        redde ZEPHYRUM;
    }
    limes = _nunc(admissio) - DIES_MS;
    n = _numerus(admissio,
        "SELECT COUNT(*) FROM admissio_clientes WHERE"
        " ? = '' AND creatus_ms < ? AND id NOT IN (SELECT cliens FROM"
        " admissio_concessiones)", _ch(""), VERUM, limes);
    si (n <= ZEPHYRUM)
    {
        n = ZEPHYRUM;
    }
    alioquin
    {
        e = scrinium_praeparare(admissio->scrinium,
            "DELETE FROM admissio_clientes WHERE creatus_ms < ? AND id"
            " NOT IN (SELECT cliens FROM admissio_concessiones)");
        si (e)
        {
            scrinium_ligare_numerum(e, I, limes);
            (vacuum)scrinium_gradi(e);
            scrinium_finire(e);
        }
    }
    e = scrinium_praeparare(admissio->scrinium,
        "DELETE FROM admissio_tesserae WHERE expirat_ms <= ?");
    si (e)
    {
        scrinium_ligare_numerum(e, I, _nunc(admissio));
        (vacuum)scrinium_gradi(e);
        scrinium_finire(e);
    }
    e = scrinium_praeparare(admissio->scrinium,
        "DELETE FROM admissio_codices WHERE expirat_ms <= ?");
    si (e)
    {
        scrinium_ligare_numerum(e, I, _nunc(admissio));
        (vacuum)scrinium_gradi(e);
        scrinium_finire(e);
    }
    redde (s32)n;
}


/* ==================================================
 * VIAE HTTP (pars II)
 * ================================================== */

nomen enumeratio {
    IUDICIUM_BENE = ZEPHYRUM,
    IUDICIUM_RESPONSUM            /* pagina aut redirectio iam data */
} Iudicium;

/* petitio auctorizationis (GET quaestio aut POST forma) */
nomen structura {
        chorda cliens;
        chorda redirectio;
        chorda provocatio;
        chorda methodus;
        chorda scopus;
        chorda status;          /* "state" */
        chorda audientia;
        chorda genus;           /* response_type */
        chorda titulus;         /* clientis */
           b32 loopback_solum;
        chorda scopi[XVI];      /* scopi applicationis petiti */
           i32 numerus_scoporum;
           b32 offline;
} Petitio;

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

/* "scheme://host[:port]" ex URL */
interior character*
_origo_ex (
                Piscina* p,
     constans character* url)
{
    constans character* schema  = strstr(url, "://");
    constans character* finis   = schema ? strchr(schema + III, '/')
        : NIHIL;
     memoriae_index n = finis ? (memoriae_index)(finis - url)
         : (memoriae_index)strlen(url);
         character* t = (character*)piscina_allocare(p, n + I);

    memcpy(t, url, (size_t)n);
    t[n] = '\0';
    redde t;
}

interior vacuum
_html (
    ChordaAedificator* a,
               chorda  t)
{
    i32 i;

    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        character k = (character)t.datum[i];

        commutatio (k)
        {
            casus '&':
                chorda_aedificator_appendere_literis(a, "&amp;");
                frange;
            casus '<':
                chorda_aedificator_appendere_literis(a, "&lt;");
                frange;
            casus '>':
                chorda_aedificator_appendere_literis(a, "&gt;");
                frange;
            casus '"':
                chorda_aedificator_appendere_literis(a, "&quot;");
                frange;
            casus '\'':
                chorda_aedificator_appendere_literis(a, "&#39;");
                frange;
            ordinarius:
                chorda_aedificator_appendere_character(a, k);
                frange;
        }
    }
}

interior vacuum
_pagina_erroris (
    HospitiumColloquium* c,
                    i32  status,
     constans character* nuntius)
{
    ChordaAedificator* a =
        chorda_aedificator_creare(colloquium_piscina(c),
        DXII);

    chorda_aedificator_appendere_literis(a, "<!doctype html><html lang="
        "\"la\"><head><meta charset=\"utf-8\"><title>Error</title>"
        "</head>"
        "<body><h1>Admissio recusata</h1><p>");
    _html(a, _ch(nuntius));
    chorda_aedificator_appendere_literis(a, "</p></body></html>");
    colloquium_caput_addere(c, "Cache-Control", "no-store");
    colloquium_respondere(c, status, "text/html; charset=utf-8",
        chorda_aedificator_finire(a));
}

/* redirectio ad clientem cum parametris (iss semper) */
interior vacuum
_redirigere_clientem (
    HospitiumColloquium* c,
               Admissio* a,
                 chorda  redirectio,
     constans character* clavis,
                 chorda  valor,
                 chorda  status)
{
               Piscina* p = colloquium_piscina(c);
     ChordaAedificator* t = chorda_aedificator_creare(p, DXII);
                   i32  k;
                   b32  quaestio = FALSUM;

    chorda_aedificator_appendere_chorda(t, redirectio);
    per (k = ZEPHYRUM; k < redirectio.mensura; k++)
    {
        si (redirectio.datum[k] == '?')
        {
            quaestio = VERUM;
        }
    }
    chorda_aedificator_appendere_literis(t, quaestio ? "&" : "?");
    chorda_aedificator_appendere_literis(t, clavis);
    chorda_aedificator_appendere_literis(t, "=");
    chorda_aedificator_appendere_chorda(t, url_codificare(valor, p));
    si (status.datum)
    {
        chorda_aedificator_appendere_literis(t, "&state=");
        chorda_aedificator_appendere_chorda(t, url_codificare(status,
            p));
    }
    chorda_aedificator_appendere_literis(t, "&iss=");
    chorda_aedificator_appendere_chorda(t, url_codificare(_ch(
        a->o.auctoritas), p));
    colloquium_caput_addere(c, "Cache-Control", "no-store");
    colloquium_redirigere(c, CCCII, chorda_ut_cstr(
        chorda_aedificator_finire(t), p));
}

/* loopback http: URL sine portu ("http://127.0.0.1/cb"); vacua si non
 * loopback */
interior chorda
_sine_portu (
      chorda  r,
     Piscina* p)
{
     ChordaAedificator* t;
                   i32  finis = VII;
                   i32  i;

    si (   !_incipit(r, "http://127.0.0.1")
        && !_incipit(r, "http://localhost"))
    {
        redde _vacua();
    }
    dum (   finis < r.mensura && r.datum[finis] != ':'
         && r.datum[finis] != '/')
    {
        finis++;
    }
    si (finis - VII != IX)
    {
        redde _vacua();
    }
    t = chorda_aedificator_creare(p, (memoriae_index)r.mensura + I);
    per (i = ZEPHYRUM; i < finis; i++)
    {
        chorda_aedificator_appendere_character(t,
            (character)r.datum[i]);
    }
    si (finis < r.mensura && r.datum[finis] == ':')
    {
        finis++;
        dum (   finis < r.mensura && r.datum[finis] >= '0'
             && r.datum[finis] <= '9')
        {
            finis++;
        }
    }
    per (i = finis; i < r.mensura; i++)
    {
        chorda_aedificator_appendere_character(t,
            (character)r.datum[i]);
    }
    redde chorda_aedificator_finire(t);
}

/* redirectio petita inter registratas: exacta, aut loopback sine
 * portu (RFC 8252 7.3) */
interior b32
_redirectio_congruit (
     JsonValor* registratae,
        chorda  petita,
       Piscina* p)
{
    chorda petita_nuda = _sine_portu(petita, p);
       i32 k;

    per (k = ZEPHYRUM; k < json_tabulatum_numerus(registratae); k++)
    {
        chorda r = json_ad_chorda(json_tabulatum_obtinere(registratae,
            k));

        si (chorda_aequalis(r, petita))
        {
            redde VERUM;
        }
        si (   petita_nuda.datum
            && chorda_aequalis(_sine_portu(r, p), petita_nuda))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior chorda
_campus (
    HospitiumColloquium* c,
                    b32  post,
     constans character* titulus)
{
    redde post ? colloquium_forma(c, titulus)
        : colloquium_quaestio(c, titulus);
}

interior b32
_scopus_applicationis (
               Admissio* a,
                 chorda  s)
{
    i32 k;

    per (k = ZEPHYRUM; a->o.scopi[k]; k++)
    {
        si (chorda_aequalis_literis(s, a->o.scopi[k]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* parametra iudicare; IUDICIUM_RESPONSUM = pagina aut redirectio
 * erroris iam missa */
interior Iudicium
_petitionem_iudicare (
    HospitiumColloquium* c,
               Admissio* a,
                    b32  post,
                Petitio* q)
{
               Piscina* p = colloquium_piscina(c);
    ScriniumEnuntiatum* e;
             JsonValor* registratae = NIHIL;
                   i32  k;
                   i32  i;

    memset(q, ZEPHYRUM, magnitudo(*q));
    q->cliens      = _campus(c, post, "client_id");
    q->redirectio  = _campus(c, post, "redirect_uri");
    q->provocatio  = _campus(c, post, "code_challenge");
    q->methodus    = _campus(c, post, "code_challenge_method");
    q->scopus      = _campus(c, post, "scope");
    q->status      = _campus(c, post, "state");
    q->audientia   = _campus(c, post, "resource");
    q->genus       = _campus(c, post, "response_type");
    /* cliens et redirectio: PAGINA (numquam redirectio aperta) */
    si (!q->cliens.datum)
    {
        _pagina_erroris(c, CD, "client_id deest");
        redde IUDICIUM_RESPONSUM;
    }
    e = scrinium_praeparare(a->scrinium,
        "SELECT titulus, redirectiones FROM admissio_clientes"
        " WHERE id = ?");
    si (e)
    {
        scrinium_ligare_textum(e, I, q->cliens);
        si (scrinium_gradi(e) == SCRINIUM_ORDO)
        {
            JsonResultus r;

            q->titulus = scrinium_columna_textus(e, ZEPHYRUM, p);
            r = json_legere(scrinium_columna_textus(e, I, p), p);
            registratae = r.successus ? r.radix : NIHIL;
        }
        scrinium_finire(e);
    }
    si (!registratae || !json_est_tabulatum(registratae))
    {
        _pagina_erroris(c, CD, "cliens ignotus");
        redde IUDICIUM_RESPONSUM;
    }
    si (   !q->redirectio.datum
        && json_tabulatum_numerus(registratae) == I)
    {
        q->redirectio =
            json_ad_chorda(json_tabulatum_obtinere(registratae,
            ZEPHYRUM));
    }
    si (   !q->redirectio.datum
        || !_redirectio_congruit(registratae, q->redirectio, p))
    {
        _pagina_erroris(c, CD, "redirect_uri non registrata");
        redde IUDICIUM_RESPONSUM;
    }
    q->loopback_solum = VERUM;
    per (k = ZEPHYRUM; k < json_tabulatum_numerus(registratae); k++)
    {
        si (!_sine_portu(json_ad_chorda(json_tabulatum_obtinere(
                registratae, k)), p).datum)
        {
            q->loopback_solum = FALSUM;
        }
    }
    /* cetera: redirectio cum errore */
    si (!q->genus.datum || !chorda_aequalis_literis(q->genus, "code"))
    {
        _redirigere_clientem(c, a, q->redirectio, "error",
            _ch("unsupported_response_type"), q->status);
        redde IUDICIUM_RESPONSUM;
    }
    si (   !q->methodus.datum
        || !chorda_aequalis_literis(q->methodus, "S256")
        || q->provocatio.mensura < XLIII
        || q->provocatio.mensura > CXXVIII)
    {
        _redirigere_clientem(c, a, q->redirectio, "error",
            _ch("invalid_request"), q->status);
        redde IUDICIUM_RESPONSUM;
    }
    si (!q->audientia.datum)
    {
        q->audientia = _ch(a->o.audientia);
    }
    si (!chorda_aequalis_literis(q->audientia, a->o.audientia))
    {
        _redirigere_clientem(c, a, q->redirectio, "error",
            _ch("invalid_target"), q->status);
        redde IUDICIUM_RESPONSUM;
    }
    i = ZEPHYRUM;
    dum (q->scopus.datum && i < q->scopus.mensura)
    {
        chorda s;
           i32 initium;

        dum (i < q->scopus.mensura && q->scopus.datum[i] == ' ')
        {
            i++;
        }
        initium = i;
        dum (i < q->scopus.mensura && q->scopus.datum[i] != ' ')
        {
            i++;
        }
        si (i == initium)
        {
            frange;
        }
        s = chorda_ex_buffer(q->scopus.datum + initium, i - initium);
        si (chorda_aequalis_literis(s, "offline_access"))
        {
            q->offline = VERUM;
        }
        alioquin si (   _scopus_applicationis(a, s)
                     && q->numerus_scoporum < XVI)
        {
            q->scopi[q->numerus_scoporum++] = s;
        }
        alioquin
        {
            _redirigere_clientem(c, a, q->redirectio, "error",
                _ch("invalid_scope"), q->status);
            redde IUDICIUM_RESPONSUM;
        }
    }
    si (q->numerus_scoporum == ZEPHYRUM)
    {
        per (k = ZEPHYRUM; a->o.scopi[k] && k < XVI; k++)
        {
            q->scopi[q->numerus_scoporum++] = _ch(a->o.scopi[k]);
        }
    }
    redde IUDICIUM_BENE;
}

/* ad paginam intrandi cum redire = /oauth/authorize?<parametra> */
interior vacuum
_ad_intrandum (
    HospitiumColloquium* c,
               Admissio* a,
                Petitio* q)
{
               Piscina* p = colloquium_piscina(c);
     ChordaAedificator* reditus = chorda_aedificator_creare(p, MXXIV);
     ChordaAedificator* t = chorda_aedificator_creare(p, MMXLVIII);
    constans character* claves[VIII];
                chorda  valores[VIII];
                   i32  k;

    claves[ZEPHYRUM]   = "response_type";
    valores[ZEPHYRUM]  = q->genus;
    claves[I]          = "client_id";
    valores[I]         = q->cliens;
    claves[II]         = "redirect_uri";
    valores[II]        = q->redirectio;
    claves[III]        = "code_challenge";
    valores[III]       = q->provocatio;
    claves[IV]         = "code_challenge_method";
    valores[IV]        = q->methodus;
    claves[V]          = "scope";
    valores[V]         = q->scopus;
    claves[VI]         = "state";
    valores[VI]        = q->status;
    claves[VII]        = "resource";
    valores[VII]       = q->audientia;
    chorda_aedificator_appendere_literis(reditus, "/oauth/authorize?");
    per (k = ZEPHYRUM; k < VIII; k++)
    {
        si (!valores[k].datum)
        {
            perge;
        }
        si (k > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(reditus, "&");
        }
        chorda_aedificator_appendere_literis(reditus, claves[k]);
        chorda_aedificator_appendere_literis(reditus, "=");
        chorda_aedificator_appendere_chorda(reditus, url_codificare(
            valores[k], p));
    }
    chorda_aedificator_appendere_literis(t, a->via_intrandi);
    chorda_aedificator_appendere_literis(t, "?redire=");
    chorda_aedificator_appendere_chorda(t, url_codificare(
        chorda_aedificator_finire(reditus), p));
    colloquium_redirigere(c, CCCIII, chorda_ut_cstr(
        chorda_aedificator_finire(t), p));
}

interior vacuum
_occultum (
     ChordaAedificator* a,
    constans character* titulus,
                chorda  valor)
{
    si (!valor.datum)
    {
        redde;
    }
    chorda_aedificator_appendere_literis(a,
        "<input type=\"hidden\" name=\"");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "\" value=\"");
    _html(a, valor);
    chorda_aedificator_appendere_literis(a, "\">");
}

/* hospes redirectionis (sine schemate, portu, via) */
interior chorda
_hospes (
    chorda r)
{
    i32 initium = ZEPHYRUM;
    i32 i;

    per (i = ZEPHYRUM; i + II < r.mensura; i++)
    {
        si (   r.datum[i]      == ':' && r.datum[i + I] == '/'
            && r.datum[i + II] == '/')
        {
            initium = i + III;
            frange;
        }
    }
    i = initium;
    dum (i < r.mensura && r.datum[i] != '/' && r.datum[i] != ':')
    {
        i++;
    }
    redde chorda_ex_buffer(r.datum + initium, i - initium);
}

interior vacuum
_pagina_consensus (
    HospitiumColloquium* c,
               Admissio* a,
                Petitio* q,
                 chorda  csrf)
{
    ChordaAedificator* t =
        chorda_aedificator_creare(colloquium_piscina(c),
        IV * MXXIV);
                   i32 k;

    (vacuum)a;

    chorda_aedificator_appendere_literis(t, "<!doctype html><html lang="
        "\"la\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" "
        "content=\"width=device-width,initial-scale=1\">"
        "<title>Permissio"
        "</title></head><body><h1>Permissio</h1><p><b>");
    _html(t, q->titulus.mensura ? q->titulus : q->cliens);
    chorda_aedificator_appendere_literis(t,
        "</b> accessum petit ad <i>");
    _html(t, q->audientia);
    chorda_aedificator_appendere_literis(t, "</i>.</p><p>Responsum "
        "mittetur ad: <strong>");
    _html(t, _hospes(q->redirectio));
    chorda_aedificator_appendere_literis(t, "</strong></p>");
    si (q->loopback_solum)
    {
        chorda_aedificator_appendere_literis(t, "<p><b>Cave:</b> hic "
            "cliens est programma locale (redirectio ad hunc "
            "computatrum "
            "solum). Permitte solum si tu ipse id modo incepisti.</p>");
    }
    chorda_aedificator_appendere_literis(t, "<form method=\"post\" "
        "action=\"/oauth/authorize\">");
    _occultum(t, "response_type", q->genus);
    _occultum(t, "client_id", q->cliens);
    _occultum(t, "redirect_uri", q->redirectio);
    _occultum(t, "code_challenge", q->provocatio);
    _occultum(t, "code_challenge_method", q->methodus);
    _occultum(t, "scope", q->scopus);
    _occultum(t, "state", q->status);
    _occultum(t, "resource", q->audientia);
    _occultum(t, "csrf", csrf);
    per (k = ZEPHYRUM; k < q->numerus_scoporum; k++)
    {
        character n[XVI];

        sprintf(n, "s%u", (insignatus integer)k);
        chorda_aedificator_appendere_literis(t, "<p><label><input "
            "type=\"checkbox\" name=\"");
        chorda_aedificator_appendere_literis(t, n);
        chorda_aedificator_appendere_literis(t,
            "\" value=\"1\" checked> ");
        _html(t, q->scopi[k]);
        chorda_aedificator_appendere_literis(t, "</label></p>");
    }
    chorda_aedificator_appendere_literis(t,
        "<p><button name=\"decisio\" "
        "value=\"permittere\">Permittere</button> <button "
        "name=\"decisio\" value=\"negare\">Negare</button></p></form>"
        "</body></html>");
    colloquium_caput_addere(c, "Cache-Control", "no-store");
    colloquium_caput_addere(c, "X-Frame-Options", "DENY");
    colloquium_respondere(c, CC, "text/html; charset=utf-8",
        chorda_aedificator_finire(t));
}

interior vacuum
_auctorizare_get (
    HospitiumColloquium* c)
{
     Admissio* a = (Admissio*)colloquium_datum(c);
      Petitio  q;
       chorda  usor;
       chorda  csrf;

    _speculari(a, c);
    si (_petitionem_iudicare(c, a, FALSUM, &q) != IUDICIUM_BENE)
    {
        redde;
    }
    si (   !a->intratus
        || !a->intratus(a->datum_intratus, c, &usor, &csrf))
    {
        _ad_intrandum(c, a, &q);
        redde;
    }
    _pagina_consensus(c, a, &q, csrf);
}

/* codex: digestum conditum; vacua si fractum */
interior chorda
_codicem_dare (
     Admissio* a,
      Petitio* q,
       chorda  usor,
       chorda  scopi,
      Piscina* p)
{
                chorda  codex = _fortuitum(XXXII, p);
    ScriniumEnuntiatum* e;
                   b32  bene;

    si (codex.mensura != ADMISSIO_TESSERA_MENSURA)
    {
        redde _vacua();
    }
    e = scrinium_praeparare(a->scrinium,
        "INSERT INTO admissio_codices (digestum, cliens, redirectio,"
        " provocatio, audientia, usor, scopi, expirat_ms)"
        " VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    si (!e)
    {
        redde _vacua();
    }
    scrinium_ligare_textum(e, I, _digestum(codex, p));
    scrinium_ligare_textum(e, II, q->cliens);
    scrinium_ligare_textum(e, III, q->redirectio);
    scrinium_ligare_textum(e, IV, q->provocatio);
    scrinium_ligare_textum(e, V, q->audientia);
    scrinium_ligare_textum(e, VI, usor);
    scrinium_ligare_textum(e, VII, scopi);
    scrinium_ligare_numerum(e, VIII, _nunc(a) + a->o.vita_codicis_ms);
    bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
    scrinium_finire(e);
    redde bene ? codex : _vacua();
}

interior vacuum
_auctorizare_post (
    HospitiumColloquium* c)
{
               Admissio* a = (Admissio*)colloquium_datum(c);
                Piscina* p = colloquium_piscina(c);
                Petitio  q;
                 chorda  usor;
                 chorda  csrf;
                 chorda  missum;
                 chorda  origo;
                 chorda  codex;
      ChordaAedificator* scopi;
                    i32  k;
                    i32  electi = ZEPHYRUM;

    _speculari(a, c);
    si (_petitionem_iudicare(c, a, VERUM, &q) != IUDICIUM_BENE)
    {
        redde;
    }
    origo = colloquium_caput(c, "Origin");
    si (!origo.datum || !chorda_aequalis_literis(origo, a->origo))
    {
        _pagina_erroris(c, CDIII, "origo vetita");
        redde;
    }
    si (   !a->intratus
        || !a->intratus(a->datum_intratus, c, &usor, &csrf))
    {
        _ad_intrandum(c, a, &q);
        redde;
    }
    missum = colloquium_forma(c, "csrf");
    si (   !missum.datum || !csrf.datum
        || !_aequale_constans(missum, csrf))
    {
        _pagina_erroris(c, CDIII, "signum CSRF invalidum");
        redde;
    }
    si (!chorda_aequalis_literis(colloquium_forma(c, "decisio"),
            "permittere"))
    {
        _redirigere_clientem(c, a, q.redirectio, "error",
            _ch("access_denied"), q.status);
        redde;
    }
    scopi = chorda_aedificator_creare(p, CCLVI);
    per (k = ZEPHYRUM; k < q.numerus_scoporum; k++)
    {
        character n[XVI];

        sprintf(n, "s%u", (insignatus integer)k);
        si (colloquium_forma(c, n).datum)
        {
            si (electi++ > ZEPHYRUM)
            {
                chorda_aedificator_appendere_literis(scopi, " ");
            }
            chorda_aedificator_appendere_chorda(scopi, q.scopi[k]);
        }
    }
    si (electi == ZEPHYRUM)
    {
        _redirigere_clientem(c, a, q.redirectio, "error",
            _ch("access_denied"), q.status);
        redde;
    }
    si (q.offline)
    {
        chorda_aedificator_appendere_literis(scopi, " offline_access");
    }
    codex = _codicem_dare(a, &q, usor, chorda_aedificator_finire(scopi),
        p);
    si (!codex.datum)
    {
        _redirigere_clientem(c, a, q.redirectio, "error",
            _ch("server_error"), q.status);
        redde;
    }
    _redirigere_clientem(c, a, q.redirectio, "code", codex, q.status);
}


/* ==================================================
 * /oauth/token (RFC 6749 sectio 5)
 * ================================================== */

interior vacuum
_token_respondere (
    HospitiumColloquium* c,
                    i32  status,
                 chorda  corpus)
{
    colloquium_caput_addere(c, "Cache-Control", "no-store");
    colloquium_caput_addere(c, "Pragma", "no-cache");
    colloquium_respondere(c, status, "application/json", corpus);
}

interior vacuum
_token_error (
    HospitiumColloquium* c,
     constans character* codex,
     constans character* descriptio)
{
    _token_respondere(c, CD, _error(colloquium_piscina(c), codex,
        descriptio));
}

interior vacuum
_tesseras_respondere (
    HospitiumColloquium* c,
               Admissio* a,
                 chorda  accessus,
                 chorda  renovatio,
                 chorda  scopi)
{
      Piscina* p = colloquium_piscina(c);
    JsonValor* r = json_objectum_creare(p);

    json_objectum_ponere(r, "access_token", json_chorda_creare(p,
        accessus));
    json_objectum_ponere(r, "token_type", json_chorda_creare_literis(p,
        "Bearer"));
    json_objectum_ponere(r, "expires_in", json_integer_creare(p,
        a->o.vita_accessus_ms / M));
    si (renovatio.datum)
    {
        json_objectum_ponere(r, "refresh_token", json_chorda_creare(p,
            renovatio));
    }
    json_objectum_ponere(r, "scope", json_chorda_creare(p, scopi));
    _token_respondere(c, CC, json_scribere(r, p));
}

/* signum integrum in indice spatiis separato */
interior b32
_scopum_continet (
                 chorda  scopi,
     constans character* scopus)
{
    i32 n = (i32)strlen(scopus);
    i32 i = ZEPHYRUM;

    dum (i < scopi.mensura)
    {
        i32 initium;

        dum (i < scopi.mensura && scopi.datum[i] == ' ')
        {
            i++;
        }
        initium = i;
        dum (i < scopi.mensura && scopi.datum[i] != ' ')
        {
            i++;
        }
        si (   i - initium == n && n > ZEPHYRUM
            && memcmp(scopi.datum + initium, scopus, (size_t)n)
                == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* concessionem necare per id (dominum ipsa legit) */
interior vacuum
_familiam_necare (
     Admissio* a,
       chorda  concessio,
      Piscina* p)
{
    chorda u;
    chorda cliens;

    si (   concessio.datum
        && _concessio_viva(a, concessio, _vacua(), &u, &cliens, p))
    {
        (vacuum)_concessionem_necare(a, concessio, u, cliens, p);
    }
}

interior vacuum
_permutare (
    HospitiumColloquium* c,
               Admissio* a)
{
               Piscina* p       = colloquium_piscina(c);
                chorda  codex   = colloquium_forma(c, "code");
                chorda  cliens  = colloquium_forma(c, "client_id");
                chorda  redirectio = colloquium_forma(c,
                    "redirect_uri");
                chorda verificator = colloquium_forma(c,
                    "code_verifier");
                chorda audientia_petita = colloquium_forma(c,
                    "resource");
                chorda  digestum;
    ScriniumEnuntiatum* e;
                   b32  inventus = FALSUM;
                chorda  r_cliens;
                chorda  r_redirectio;
                chorda  r_provocatio;
                chorda  r_audientia;
                chorda  r_usor;
                chorda  r_scopi;
                chorda  r_concessio;
                   s64  r_expirat  = ZEPHYRUM;
                   s64  r_usus     = ZEPHYRUM;
                chorda  provocatio;
                chorda  id;
                chorda  accessus;
                chorda  renovatio;
              Sigillum  s;

    si (   !codex.datum || !cliens.datum || !redirectio.datum
        || !verificator.datum)
    {
        _token_error(c, "invalid_request", "parametra desunt");
        redde;
    }
    digestum = _digestum(codex, p);
    e = scrinium_praeparare(a->scrinium,
        "SELECT digestum, cliens, redirectio, provocatio, audientia,"
        " usor,"
        " scopi, expirat_ms, usus, concessio FROM admissio_codices"
        " WHERE digestum = ?");
    si (e)
    {
        scrinium_ligare_textum(e, I, digestum);
        si (   scrinium_gradi(e) == SCRINIUM_ORDO
            && _aequale_constans(scrinium_columna_textus(e, ZEPHYRUM,
            p),
                   digestum))
        {
            inventus      = VERUM;
            r_cliens      = scrinium_columna_textus(e, I, p);
            r_redirectio  = scrinium_columna_textus(e, II, p);
            r_provocatio  = scrinium_columna_textus(e, III, p);
            r_audientia   = scrinium_columna_textus(e, IV, p);
            r_usor        = scrinium_columna_textus(e, V, p);
            r_scopi       = scrinium_columna_textus(e, VI, p);
            r_expirat     = scrinium_columna_numerus(e, VII);
            r_usus        = scrinium_columna_numerus(e, VIII);
            r_concessio   = scrinium_columna_nihil_est(e, IX) ? _vacua()
                : scrinium_columna_textus(e, IX, p);
        }
        scrinium_finire(e);
    }
    si (!inventus)
    {
        _token_error(c, "invalid_grant", "codex ignotus");
        redde;
    }
    /* code iteratum: quidquid ex eo datum est necatur */
    si (r_usus)
    {
        _familiam_necare(a, r_concessio, p);
        _token_error(c, "invalid_grant", "codex iam adhibitus");
        redde;
    }
    /* uritur ANTE iudicia cetera: conatus falsus eum quoque consumit */
    e = scrinium_praeparare(a->scrinium,
        "UPDATE admissio_codices SET usus = 1 WHERE digestum = ?");
    si (e)
    {
        scrinium_ligare_textum(e, I, digestum);
        (vacuum)scrinium_gradi(e);
        scrinium_finire(e);
    }
    si (_nunc(a) >= r_expirat)
    {
        _token_error(c, "invalid_grant", "codex expiratus");
        redde;
    }
    si (   !chorda_aequalis(cliens, r_cliens)
        || !chorda_aequalis(redirectio, r_redirectio))
    {
        _token_error(c, "invalid_grant",
            "cliens aut redirectio aliena");
        redde;
    }
    si (   audientia_petita.datum
        && !chorda_aequalis(audientia_petita, r_audientia))
    {
        _token_error(c, "invalid_target", "audientia_petita alienum");
        redde;
    }
    s = sigillum_computare(verificator.datum,
        (memoriae_index)verificator.mensura);
    provocatio = base64url_codificare(s.octeti, SIGILLUM_OCTETI, p);
    si (   verificator.mensura < XLIII || verificator.mensura > CXXVIII
        || !_aequale_constans(provocatio, r_provocatio))
    {
        _token_error(c, "invalid_grant", "code_verifier non congruit");
        redde;
    }
    si (!_concessionem_creare(a, r_usor, r_cliens, r_scopi,
            _scopum_continet(r_scopi, "offline_access"), &id, &accessus,
            &renovatio, p))
    {
        _token_error(c, "server_error", "concessio creari non potuit");
        redde;
    }
    e = scrinium_praeparare(a->scrinium,
        "UPDATE admissio_codices SET concessio = ? WHERE digestum = ?");
    si (e)
    {
        scrinium_ligare_textum(e, I, id);
        scrinium_ligare_textum(e, II, digestum);
        (vacuum)scrinium_gradi(e);
        scrinium_finire(e);
    }
    _tesseras_respondere(c, a, accessus, renovatio, r_scopi);
}

interior vacuum
_renovare (
    HospitiumColloquium* c,
               Admissio* a)
{
               Piscina* p        = colloquium_piscina(c);
                chorda  tessera  = colloquium_forma(c, "refresh_token");
                chorda  cliens   = colloquium_forma(c, "client_id");
                chorda  digestum;
    ScriniumEnuntiatum* e;
                   b32  inventa   = FALSUM;
                   s64  genus     = -I;
                   s64  expirat   = ZEPHYRUM;
                   s64  usa       = ZEPHYRUM;
                   s64  revocata  = ZEPHYRUM;
                chorda  concessio;
                chorda  r_cliens;
                chorda  scopi;
                chorda  accessus;
                chorda  renovatio;
                   s64  nunc = _nunc(a);
                   b32  bene;

    si (!tessera.datum || !cliens.datum)
    {
        _token_error(c, "invalid_request", "parametra desunt");
        redde;
    }
    digestum = _digestum(tessera, p);
    e = scrinium_praeparare(a->scrinium,
        "SELECT t.digestum, t.genus, t.expirat_ms, t.usa, t.concessio,"
        " g.cliens, g.revocata, g.scopi FROM admissio_tesserae t"
        " JOIN admissio_concessiones g ON g.id = t.concessio"
        " WHERE t.digestum = ?");
    si (e)
    {
        scrinium_ligare_textum(e, I, digestum);
        si (   scrinium_gradi(e) == SCRINIUM_ORDO
            && _aequale_constans(scrinium_columna_textus(e, ZEPHYRUM,
            p),
                   digestum))
        {
            inventa    = VERUM;
            genus      = scrinium_columna_numerus(e, I);
            expirat    = scrinium_columna_numerus(e, II);
            usa        = scrinium_columna_numerus(e, III);
            concessio  = scrinium_columna_textus(e, IV, p);
            r_cliens   = scrinium_columna_textus(e, V, p);
            revocata   = scrinium_columna_numerus(e, VI);
            scopi      = scrinium_columna_textus(e, VII, p);
        }
        scrinium_finire(e);
    }
    si (!inventa || genus != GENUS_RENOVATIONIS || revocata)
    {
        _token_error(c, "invalid_grant",
            "tessera renovationis invalida");
        redde;
    }
    /* renovatio iam rotata: furtum suspectum - familia tota necatur */
    si (usa)
    {
        _familiam_necare(a, concessio, p);
        _token_error(c, "invalid_grant",
            "tessera renovationis iam adhibita");
        redde;
    }
    si (nunc >= expirat)
    {
        _token_error(c, "invalid_grant",
            "tessera renovationis expirata");
        redde;
    }
    si (!chorda_aequalis(cliens, r_cliens))
    {
        _token_error(c, "invalid_grant", "cliens alienus");
        redde;
    }
    si (!scrinium_incipere(a->scrinium))
    {
        _token_error(c, "server_error", "scrinium");
        redde;
    }
    e = scrinium_praeparare(a->scrinium,
        "UPDATE admissio_tesserae SET usa = 1 WHERE digestum = ?");
    bene = e != NIHIL;
    si (bene)
    {
        scrinium_ligare_textum(e, I, digestum);
        bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
        scrinium_finire(e);
    }
    bene = bene
        && _tesseram_condere(a, GENUS_ACCESSUS, concessio,
               nunc + a->o.vita_accessus_ms, &accessus, p)
        && _tesseram_condere(a, GENUS_RENOVATIONIS, concessio,
               nunc + a->o.vita_renovationis_ms, &renovatio, p);
    si (!bene)
    {
        (vacuum)scrinium_revolvere(a->scrinium);
        _token_error(c, "server_error", "rotatio fracta");
        redde;
    }
    si (!scrinium_committere(a->scrinium))
    {
        _token_error(c, "server_error", "scrinium");
        redde;
    }
    _tesseras_respondere(c, a, accessus, renovatio, scopi);
}

interior vacuum
_token (
    HospitiumColloquium* c)
{
     Admissio* a      = (Admissio*)colloquium_datum(c);
       chorda  typus  = colloquium_caput(c, "Content-Type");
       chorda  genus;

    _speculari(a, c);
    si (   !typus.datum
        || !_incipit(typus, "application/x-www-form-urlencoded"))
    {
        _token_error(c, "invalid_request",
            "application/x-www-form-urlencoded requiritur");
        redde;
    }
    genus = colloquium_forma(c, "grant_type");
    si (!genus.datum)
    {
        _token_error(c, "invalid_request", "grant_type deest");
    }
    alioquin si (chorda_aequalis_literis(genus, "authorization_code"))
    {
        _permutare(c, a);
    }
    alioquin si (chorda_aequalis_literis(genus, "refresh_token"))
    {
        _renovare(c, a);
    }
    alioquin
    {
        _token_error(c, "unsupported_grant_type", "grant_type ignotum");
    }
}

interior vacuum
_inscribere_via (
    HospitiumColloquium* c)
{
     Admissio* a = (Admissio*)colloquium_datum(c);
       chorda  responsum;
          s32  status;

    _speculari(a, c);
    status = admissio_inscribere(a, colloquium_petitio(c)->corpus,
        colloquium_cliens(c), &responsum, colloquium_piscina(c));
    colloquium_caput_addere(c, "Cache-Control", "no-store");
    colloquium_respondere(c, (i32)status, "application/json",
        responsum);
}

interior vacuum
_documentum_via (
    HospitiumColloquium* c)
{
    Admissio* a = (Admissio*)colloquium_datum(c);

    _speculari(a, c);
    colloquium_respondere(c, CC, "application/json",
        admissio_documentum(a,
        colloquium_piscina(c)));
}

vacuum
admissio_speculatorem_ponere (
                Admissio* admissio,
                  vacuum  (*speculator)(vacuum* datum,
                              HospitiumColloquium* colloquium),
                  vacuum* datum)
{
    si (admissio)
    {
        admissio->speculator          = speculator;
        admissio->speculatoris_datum  = datum;
    }
}

interior vacuum
_speculari (
               Admissio* a,
    HospitiumColloquium* c)
{
    si (a && a->speculator)
    {
        a->speculator(a->speculatoris_datum, c);
    }
}

b32
admissio_montare (
               Admissio* admissio,
              Hospitium* hospitium,
       AdmissioIntratus  intratus,
                 vacuum* datum,
     constans character* via_intrandi)
{
    si (!admissio || !hospitium || !via_intrandi)
    {
        redde FALSUM;
    }
    admissio->intratus        = intratus;
    admissio->datum_intratus  = datum;
    admissio->via_intrandi = _literae(admissio->piscina,
        via_intrandi);
    admissio->origo           = _origo_ex(admissio->piscina,
        admissio->o.auctoritas);
    redde hospitium_praebere(hospitium, HTTP_GET,
               "/.well-known/oauth-authorization-server",
               _documentum_via,
               admissio)
        && hospitium_praebere(hospitium, HTTP_POST, "/oauth/register",
               _inscribere_via, admissio)
        && hospitium_praebere(hospitium, HTTP_GET, "/oauth/authorize",
               _auctorizare_get, admissio)
        && hospitium_praebere(hospitium, HTTP_POST, "/oauth/authorize",
               _auctorizare_post, admissio)
        && hospitium_praebere(hospitium, HTTP_POST, "/oauth/token",
        _token,
               admissio);
}
