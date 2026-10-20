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
};

hic_manens constans character* constans SCHEMA[] = {
    "CREATE TABLE IF NOT EXISTS admissio_versio (versio INTEGER NOT "
    "NULL)",
    "CREATE TABLE IF NOT EXISTS admissio_clientes (id TEXT PRIMARY KEY,"
    " titulus TEXT NOT NULL, redirectiones TEXT NOT NULL, ip TEXT NOT"
    " NULL, creatus_ms INTEGER NOT NULL)",
    "CREATE INDEX IF NOT EXISTS admissio_clientes_ip ON"
    " admissio_clientes (ip, creatus_ms)",
    "CREATE TABLE IF NOT EXISTS admissio_concessiones (id TEXT PRIMARY"
    " KEY, cliens TEXT NOT NULL, usor TEXT NOT NULL, scopi TEXT NOT "
    "NULL,"
    " audientia TEXT NOT NULL, creata_ms INTEGER NOT NULL,"
    " usus_ultimus_ms INTEGER NOT NULL, revocata INTEGER NOT NULL"
    " DEFAULT 0)",
    "CREATE TABLE IF NOT EXISTS admissio_codices (digestum TEXT PRIMARY"
    " KEY, cliens TEXT NOT NULL, redirectio TEXT NOT NULL, provocatio"
    " TEXT NOT NULL, audientia TEXT NOT NULL, usor TEXT NOT NULL, scopi"
    " TEXT NOT NULL, expirat_ms INTEGER NOT NULL, usus INTEGER NOT NULL"
    " DEFAULT 0, concessio TEXT)",
    "CREATE TABLE IF NOT EXISTS admissio_tesserae (digestum TEXT "
    "PRIMARY"
    " KEY, genus INTEGER NOT NULL, concessio TEXT NOT NULL, familia "
    "TEXT"
    " NOT NULL, expirat_ms INTEGER NOT NULL, usa INTEGER NOT NULL"
    " DEFAULT 0)",
    "CREATE INDEX IF NOT EXISTS admissio_tesserae_concessio ON"
    " admissio_tesserae (concessio)",
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
                Piscina* p)
{
    JsonValor* d = json_objectum_creare(p);

    json_objectum_ponere(d, "concessio", json_chorda_creare(p,
        concessio));
    json_objectum_ponere(d, "usor", json_chorda_creare(p, usor));
    json_objectum_ponere(d, "cliens", json_chorda_creare(p, cliens));
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
        && _tesseram_condere(a, GENUS_RENOVATIONIS, id,
               nunc + a->o.vita_renovationis_ms, renovatio, p);
    si (!bene)
    {
        (vacuum)scrinium_revolvere(a->scrinium);
        redde FALSUM;
    }
    si (!scrinium_committere(a->scrinium))
    {
        redde FALSUM;
    }
    _actum(a, "concessio.creata", id, usor, cliens, p);
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
    redde _concessionem_creare(admissio, usor, cliens, scopi, accessus,
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
        "SELECT g.id, g.cliens, COALESCE(c.titulus, ''), g.usor, "
        "g.scopi,"
        " g.creata_ms, g.usus_ultimus_ms FROM admissio_concessiones g"
        " LEFT JOIN admissio_clientes c ON c.id = g.cliens WHERE "
        "g.usor ="
        " ? AND g.revocata = 0 ORDER BY g.creata_ms, g.id");
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

b32
admissio_revocare (
     Admissio* admissio,
       chorda  usor,
       chorda  concessio)
{
               Piscina* p;
    ScriniumEnuntiatum* e;
                chorda  cliens;
                   b32  bene = FALSUM;

    si (!admissio)
    {
        redde FALSUM;
    }
    p = piscina_generare_dynamicum("admissio_revocare",
        (memoriae_index)4096);
    si (!p)
    {
        redde FALSUM;
    }
    e = scrinium_praeparare(admissio->scrinium,
        "SELECT cliens FROM admissio_concessiones WHERE id = ? AND "
        "usor ="
        " ? AND revocata = 0");
    si (e)
    {
        scrinium_ligare_textum(e, I, concessio);
        scrinium_ligare_textum(e, II, usor);
        si (scrinium_gradi(e) == SCRINIUM_ORDO)
        {
            cliens  = scrinium_columna_textus(e, ZEPHYRUM, p);
            bene    = VERUM;
        }
        scrinium_finire(e);
    }
    si (bene && scrinium_incipere(admissio->scrinium))
    {
        ScriniumEnuntiatum* u = scrinium_praeparare(admissio->scrinium,
            "UPDATE admissio_concessiones SET revocata = 1 WHERE id = "
            "?");
        ScriniumEnuntiatum* d = scrinium_praeparare(admissio->scrinium,
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
        bene = bene ? scrinium_committere(admissio->scrinium)
            : (scrinium_revolvere(admissio->scrinium) && FALSUM);
        si (bene)
        {
            _actum(admissio, "concessio.revocata", concessio, usor,
                cliens,
                p);
        }
    }
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
            "DELETE FROM admissio_clientes WHERE creatus_ms < ? AND id "
            "NOT"
            " IN (SELECT cliens FROM admissio_concessiones)");
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
