/* mcp_servus_http.c - vectura Streamable HTTP (vide
 * include/mcp_servus_http.h)
 *
 * VITA MEMORIAE: piscina colloquii reficitur cum connexio finitur
 * (cliens abiens) - vocatio dilata in ea viveret ut cadaver. Ergo
 * vocatio quaeque piscinam PROPRIAM habet (corpus eo transcriptum,
 * nucleus in ea allocat); responsum synchronum: piscina post
 * mcp_servus_tractare destruitur; dilatum: _reddere eam destruit
 * postquam respondit (aut clientem abisse vidit). Corpus responsi in
 * piscinam colloquii transcribitur ante colloquium_respondere.
 */

#include "mcp_servus_http.h"
#include "chorda_aedificator.h"
#include "tabellarius.h"
#include "tempus.h"

#include <stdio.h>
#include <string.h>

nomen structura {
             McpServus* servus;
       McpHttpOptiones  o;        /* chordae in piscina montationis */
             character* url_documenti;
             character* caput_401;
                chorda  documentum;
} McpHttp;

/* vocatio una (in piscina sua) */
nomen structura {
                 McpHttp* m;
     HospitiumColloquium* colloquium;   /* synchrona solum */
                     b32  synchrona;
        HospitiumDilatum  dilatum;
                     f64  initium;
                  chorda  usor;
                  chorda  cliens;
                 Piscina* piscina;
} McpHttpVocatio;

hic_manens constans character* constans VERSIONES[] = {
    "2025-03-26", "2025-06-18", "2025-11-25", NIHIL
};

#define VIA_DOCUMENTI "/.well-known/oauth-protected-resource"


/* ==================================================
 * auxilia
 * ================================================== */

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

interior b32
_est (
                 chorda  c,
     constans character* literae)
{
    i32 n = (i32)strlen(literae);

    redde c.mensura == n
        && (n == ZEPHYRUM || memcmp(c.datum, literae, (size_t)n)
                == ZEPHYRUM);
}

/* praefixum sine casu (ASCII) */
interior b32
_incipit_sine_casu (
                 chorda  c,
     constans character* praefixum)
{
    i32 n = (i32)strlen(praefixum);
    i32 i;

    si (c.mensura < n)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        character a = (character)c.datum[i];
        character b = praefixum[i];

        si (a >= 'A' && a <= 'Z')
        {
            a = (character)(a - 'A' + 'a');
        }
        si (a != b)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* "https://h:p/via" -> "https://h:p" */
interior character*
_origo_audientiae (
                Piscina* p,
     constans character* audientia)
{
    constans character* schema  = strstr(audientia, "://");
    constans character* finis   = schema ? strchr(schema + III, '/')
        : NIHIL;
     memoriae_index n = finis ? (memoriae_index)(finis - audientia)
         : (memoriae_index)strlen(audientia);
         character* t = (character*)piscina_allocare(p, n + I);

    memcpy(t, audientia, (size_t)n);
    t[n] = '\0';
    redde t;
}

interior vacuum
_actum (
                 McpHttp* m,
                 Piscina* p,
                     i32  status,
                  chorda  usor,
                  chorda  cliens,
      constans character* instrumentum,
      constans character* exitus,
                     f64  initium)
{
     ChordaAedificator* a = chorda_aedificator_creare(p, CXXVIII);
                chorda  linea;
                   s32  ms = (s32)((tempus_nunc() - initium) * 1000.0);

    chorda_aedificator_appendere_literis(a, "mcp status=");
    chorda_aedificator_appendere_s32(a, (s32)status);
    chorda_aedificator_appendere_literis(a, " usor=");
    si (usor.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_chorda(a, usor);
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(a, "-");
    }
    chorda_aedificator_appendere_literis(a, " cliens=");
    si (cliens.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_chorda(a, cliens);
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(a, "-");
    }
    chorda_aedificator_appendere_literis(a, " instrumentum=");
    chorda_aedificator_appendere_literis(a, instrumentum ? instrumentum
        : "-");
    chorda_aedificator_appendere_literis(a, " exitus=");
    chorda_aedificator_appendere_literis(a, exitus ? exitus : "-");
    chorda_aedificator_appendere_literis(a, " ms=");
    chorda_aedificator_appendere_s32(a, ms < ZEPHYRUM ? ZEPHYRUM
        : ms);
    linea = chorda_aedificator_finire(a);
    si (m->o.acta)
    {
        m->o.acta(m->o.acta_datum, linea);
    }
    alioquin
    {
        fprintf(stderr, "%.*s\n", (integer)linea.mensura,
            (constans character*)linea.datum);
    }
}

interior vacuum
_json_respondere (
    HospitiumColloquium* c,
                    i32  status,
                 chorda  corpus)
{
    colloquium_respondere(c, status, "application/json",
        chorda_transcribere(corpus, colloquium_piscina(c)));
}

/* responsum ante nucleum (401, 400, 403 origo, 413, 415) + actum */
interior vacuum
_refutare (
    HospitiumColloquium* c,
                McpHttp* m,
                    i32  status,
     constans character* corpus,
     constans character* exitus,
                    f64  initium)
{
    chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    /* actum ANTE responsum: piscina colloquii post responsum
     * reficitur */
    _actum(m, colloquium_piscina(c), status, vacua, vacua, NIHIL,
        exitus,
        initium);
    _json_respondere(c, status, chorda_ex_literis(corpus,
        colloquium_piscina(c)));
}

/* "Bearer <t>": schema sine casu; tessera vacua = absens */
interior chorda
_tessera (
    HospitiumColloquium* c)
{
    chorda caput = colloquium_caput(c, "Authorization");
    chorda t;

    t.datum    = NIHIL;
    t.mensura  = ZEPHYRUM;
    si (!caput.datum || !_incipit_sine_casu(caput, "bearer "))
    {
        redde t;
    }
    t.datum    = caput.datum + VII;
    t.mensura  = caput.mensura - VII;
    dum (t.mensura > ZEPHYRUM && t.datum[ZEPHYRUM] == ' ')
    {
        t.datum++;
        t.mensura--;
    }
    dum (t.mensura > ZEPHYRUM && t.datum[t.mensura - I] == ' ')
    {
        t.mensura--;
    }
    redde t;
}

interior b32
_origo_licet (
       McpHttp* m,
        chorda  origo)
{
    i32 k;

    si (!m->o.origines)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; m->o.origines[k]; k++)
    {
        si (_est(origo, m->o.origines[k]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* application/json [; ...] */
interior b32
_typus_json (
    chorda ct)
{
    i32 n = (i32)strlen("application/json");

    si (!ct.datum || !_incipit_sine_casu(ct, "application/json"))
    {
        redde FALSUM;
    }
    redde ct.mensura == n || ct.datum[n] == ';' || ct.datum[n] == ' ';
}

/* UnsupportedProtocolVersionError (2026-07-28 lifecycle) */
interior chorda
_versio_ignota (
    HospitiumColloquium* c,
                 chorda  petita)
{
         Piscina* p         = colloquium_piscina(c);
    JsonResultus  r         = json_legere(colloquium_petitio(c)->corpus,
        p);
      JsonValor* radix    = json_objectum_creare(p);
      JsonValor* error    = json_objectum_creare(p);
      JsonValor* data     = json_objectum_creare(p);
      JsonValor* nostrae  = json_tabulatum_creare(p);
      JsonValor* id        = r.successus
          ? json_objectum_capere(r.radix, "id") : NIHIL;
             i32 k;

    per (k = ZEPHYRUM; VERSIONES[k]; k++)
    {
        json_tabulatum_addere(nostrae, json_chorda_creare_literis(p,
            VERSIONES[k]));
    }
    json_objectum_ponere(data, "supported", nostrae);
    json_objectum_ponere(data, "requested", json_chorda_creare(p,
        petita));
    json_objectum_ponere(error, "code", json_integer_creare(p,
        (s64)MCP_ERROR_VERSIO_IGNOTA));
    json_objectum_ponere(error, "message", json_chorda_creare_literis(p,
        "Unsupported protocol version"));
    json_objectum_ponere(error, "data", data);
    json_objectum_ponere(radix, "jsonrpc", json_chorda_creare_literis(p,
        "2.0"));
    json_objectum_ponere(radix, "id", id ? id : json_nullum_creare(p));
    json_objectum_ponere(radix, "error", error);
    redde json_scribere(radix, p);
}


/* ==================================================
 * reditus nuclei
 * ================================================== */

interior vacuum
_reddere (
                 vacuum* datum,
    constans McpReditus* r)
{
         McpHttpVocatio* v = (McpHttpVocatio*)datum;
                McpHttp* m = v->m;
    HospitiumColloquium* c = v->synchrona ? v->colloquium
        : dilatum_colloquium(v->dilatum);
                     i32 status = CC;

    commutatio (r->genus)
    {
        casus MCP_REDITUS_NUNTIATIO:
            status = CCII;
            frange;
        casus MCP_REDITUS_SCOPUS:
            status = CDIII;
            frange;
        casus MCP_REDITUS_PRAVUM:
            status = CD;
            frange;
        casus MCP_REDITUS_RESPONSUM:
        ordinarius:
            status = CC;
            frange;
    }
    si (c)
    {
        si (r->genus == MCP_REDITUS_SCOPUS)
        {
            ChordaAedificator* a = chorda_aedificator_creare(
                colloquium_piscina(c), CCLVI);

            chorda_aedificator_appendere_literis(a,
                "Bearer error=\"insufficient_scope\", scope=\"");
            chorda_aedificator_appendere_literis(a,
                r->scopus_requisitus ? r->scopus_requisitus : "");
            chorda_aedificator_appendere_literis(a,
                "\", resource_metadata=\"");
            chorda_aedificator_appendere_literis(a, m->url_documenti);
            chorda_aedificator_appendere_literis(a, "\"");
            colloquium_caput_addere(c, "WWW-Authenticate",
                chorda_ut_cstr(chorda_aedificator_finire(a),
                    colloquium_piscina(c)));
        }
        si (status == CCII)
        {
            chorda vacua;

            vacua.datum    = NIHIL;
            vacua.mensura  = ZEPHYRUM;
            colloquium_respondere(c, CCII, NIHIL, vacua);
        }
        alioquin
        {
            _json_respondere(c, status, r->corpus);
        }
    }
    /* cliens abiit (dilatum): status 0 in actis */
    _actum(m, v->piscina, c ? status : ZEPHYRUM, v->usor, v->cliens,
        r->instrumentum, r->exitus, v->initium);
    si (!v->synchrona)
    {
        piscina_destruere(v->piscina);
    }
}


/* ==================================================
 * tractatores
 * ================================================== */

interior vacuum
_post (
    HospitiumColloquium* c)
{
                        McpHttp* m = (McpHttp*)colloquium_datum(c);
    constans HttpPetitioServeri* petitio = colloquium_petitio(c);
                            f64  initium = tempus_nunc();
                         chorda  origo = colloquium_caput(c, "Origin");
                         chorda  tessera;
                         chorda  versio;
                     McpPersona  persona;
                        Piscina* q;
                 McpHttpVocatio* v;
                            i32  k;
                            b32  nota = FALSUM;

    si (origo.datum && !_origo_licet(m, origo))
    {
        _refutare(c, m, CDIII, "{\"error\":\"origo vetita\"}", "origo",
            initium);
        redde;
    }
    si (colloquium_quaestio(c, "access_token").datum)
    {
        _refutare(c, m, CD, "{\"error\":\"invalid_request\","
            "\"error_description\":\"tessera in quaestione vetita\"}",
            "quaestio", initium);
        redde;
    }
    tessera = _tessera(c);
    memset(&persona, ZEPHYRUM, magnitudo(persona));
    si (   tessera.mensura == ZEPHYRUM || !m->o.verificator
        || !m->o.verificator(m->o.verificatoris_datum, tessera,
               chorda_ex_literis(m->o.audientia, colloquium_piscina(c)),
               &persona, colloquium_piscina(c)))
    {
        colloquium_caput_addere(c, "WWW-Authenticate", m->caput_401);
        _refutare(c, m, CDI, "{\"error\":\"invalid_token\"}", "tessera",
            initium);
        redde;
    }
    si (   petitio->corpus_via
        || petitio->corpus.mensura > MCP_HTTP_CORPUS_MAXIMUM)
    {
        _refutare(c, m, CDXIII, "{\"error\":\"corpus nimis magnum\"}",
            "magnitudo", initium);
        redde;
    }
    si (!_typus_json(colloquium_caput(c, "Content-Type")))
    {
        _refutare(c, m, CDXV,
            "{\"error\":\"Content-Type application/json requiritur\"}",
            "typus", initium);
        redde;
    }
    versio = colloquium_caput(c, "MCP-Protocol-Version");
    si (versio.datum)
    {
        per (k = ZEPHYRUM; VERSIONES[k]; k++)
        {
            si (_est(versio, VERSIONES[k]))
            {
                nota = VERUM;
            }
        }
        si (!nota)
        {
            chorda vacua;

            vacua.datum    = NIHIL;
            vacua.mensura  = ZEPHYRUM;
            _actum(m, colloquium_piscina(c), CD, vacua, vacua, NIHIL,
                "versio", initium);
            _json_respondere(c, CD, _versio_ignota(c, versio));
            redde;
        }
    }
    q = piscina_generare_dynamicum("mcp_http", (memoriae_index)65536);
    v = q ? (McpHttpVocatio*)piscina_allocare_ordinatum(q,
        (memoriae_index)magnitudo(McpHttpVocatio),
        (memoriae_index)magnitudo(vacuum*)) : NIHIL;
    si (!v)
    {
        si (q)
        {
            piscina_destruere(q);
        }
        _refutare(c, m, D, "{\"error\":\"memoria deest\"}", "memoria",
            initium);
        redde;
    }
    memset(v, ZEPHYRUM, magnitudo(*v));
    v->m           = m;
    v->colloquium  = c;
    v->synchrona   = VERUM;
    v->initium     = initium;
    v->usor        = chorda_transcribere(persona.usor, q);
    v->cliens      = chorda_transcribere(persona.cliens, q);
    v->piscina     = q;
    si (mcp_servus_tractare(m->servus, chorda_transcribere(
            petitio->corpus, q), &persona, FALSUM, _reddere, v, q))
    {
        piscina_destruere(q);
        redde;
    }
    v->synchrona   = FALSUM;
    v->colloquium  = NIHIL;
    v->dilatum     = colloquium_differre(c);
}

interior vacuum
_non_licet (
    HospitiumColloquium* c)
{
    colloquium_caput_addere(c, "Allow", "POST");
    colloquium_respondere(c, CDV, "application/json",
        chorda_ex_literis("{\"error\":\"POST solum\"}",
            colloquium_piscina(c)));
}

interior vacuum
_documentum_protectum (
    HospitiumColloquium* c)
{
    McpHttp* m = (McpHttp*)colloquium_datum(c);

    _json_respondere(c, CC, m->documentum);
}


/* ==================================================
 * montatio
 * ================================================== */

b32
mcp_servus_http_montare (
                   Hospitium* hospitium,
                   McpServus* servus,
    constans McpHttpOptiones* optiones,
                     Piscina* piscina)
{
              McpHttp*  m;
    ChordaAedificator*  a;
            JsonValor*  doc;
            JsonValor*  servi;
            JsonValor*  scopi;
            JsonValor*  modi;
                  i32   n = ZEPHYRUM;
                  i32   k;
            character*  via_documenti;
            character** origines = NIHIL;

    si (   !hospitium || !servus || !optiones || !piscina
        || !optiones->via || !optiones->audientia
        || !optiones->auctoritas || !optiones->scopi)
    {
        redde FALSUM;
    }
    m = (McpHttp*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)magnitudo(McpHttp),
        (memoriae_index)magnitudo(vacuum*));
    si (!m)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(*m));
    m->servus        = servus;
    m->o             = *optiones;
    m->o.via         = _literae(piscina, optiones->via);
    m->o.audientia   = _literae(piscina, optiones->audientia);
    m->o.auctoritas  = _literae(piscina, optiones->auctoritas);
    m->o.scopi       = _literae(piscina, optiones->scopi);
    si (optiones->origines)
    {
        dum (optiones->origines[n])
        {
            n++;
        }
        origines = (character**)piscina_allocare_ordinatum(piscina,
            (memoriae_index)(magnitudo(character*) * (memoriae_index)(n
                + I)), (memoriae_index)magnitudo(vacuum*));
        per (k = ZEPHYRUM; k < n; k++)
        {
            origines[k] = _literae(piscina, optiones->origines[k]);
        }
        origines[n]    = NIHIL;
        m->o.origines  = (constans character* constans*)origines;
    }
    /* URL PRM in origine resource (non auctoritatis) */
    a = chorda_aedificator_creare(piscina, CXXVIII);
    chorda_aedificator_appendere_literis(a, _origo_audientiae(piscina,
        m->o.audientia));
    chorda_aedificator_appendere_literis(a, VIA_DOCUMENTI);
    m->url_documenti = chorda_ut_cstr(chorda_aedificator_finire(a),
        piscina);
    a = chorda_aedificator_creare(piscina, CCLVI);
    chorda_aedificator_appendere_literis(a,
        "Bearer resource_metadata=\"");
    chorda_aedificator_appendere_literis(a, m->url_documenti);
    chorda_aedificator_appendere_literis(a, "\", scope=\"");
    chorda_aedificator_appendere_literis(a, m->o.scopi);
    chorda_aedificator_appendere_literis(a, "\"");
    m->caput_401 = chorda_ut_cstr(chorda_aedificator_finire(a),
        piscina);
    /* RFC 9728 */
    doc    = json_objectum_creare(piscina);
    servi  = json_tabulatum_creare(piscina);
    scopi  = json_tabulatum_creare(piscina);
    modi   = json_tabulatum_creare(piscina);
    json_tabulatum_addere(servi, json_chorda_creare_literis(piscina,
        m->o.auctoritas));
    {
        character* s = (character*)(vacuum*)(memoriae_index)m->o.scopi;

        dum (*s)
        {
            character* initium;

            dum (*s == ' ')
            {
                s++;
            }
            initium = s;
            dum (*s && *s != ' ')
            {
                s++;
            }
            si (s > initium)
            {
                json_tabulatum_addere(scopi, json_chorda_creare(piscina,
                    chorda_ex_buffer((i8*)initium,
                        (i32)(s - initium))));
            }
        }
    }
    json_tabulatum_addere(modi, json_chorda_creare_literis(piscina,
        "header"));
    json_objectum_ponere(doc, "resource", json_chorda_creare_literis(
        piscina, m->o.audientia));
    json_objectum_ponere(doc, "authorization_servers", servi);
    json_objectum_ponere(doc, "scopes_supported", scopi);
    json_objectum_ponere(doc, "bearer_methods_supported", modi);
    m->documentum  = json_scribere(doc, piscina);
    a              = chorda_aedificator_creare(piscina, CXXVIII);
    chorda_aedificator_appendere_literis(a, VIA_DOCUMENTI);
    chorda_aedificator_appendere_literis(a, m->o.via);
    via_documenti = chorda_ut_cstr(chorda_aedificator_finire(a),
        piscina);
    redde hospitium_praebere(hospitium, HTTP_POST, m->o.via, _post, m)
        && hospitium_praebere(hospitium, HTTP_GET, m->o.via, _non_licet,
               m)
        && hospitium_praebere(hospitium, HTTP_DELETE, m->o.via,
               _non_licet, m)
        && hospitium_praebere(hospitium, HTTP_GET, VIA_DOCUMENTI,
        _documentum_protectum, m)
        && hospitium_praebere(hospitium, HTTP_GET, via_documenti,
        _documentum_protectum, m);
}
