/* probatio_mcp_servus_http.c - vectura Streamable HTTP (mcp-remotus
 * T4): hospitium in processu, cliens TCP crudus (P8, auxilia
 * probatio_exemplar_applicatio.c copiata), verificator probationis
 * (bona-legere / bona-omnia; audientiam 'resource' ipse probat).
 * 401/403 octetim, tessera in quaestione 400, methodi 405, 202, fasces
 * 400, 413, 415, versio protocolli (2026-07-28 -> 400 -32022), Origo,
 * PRM duabus viis, Mcp-Session-Id ignoratum, responsum dilatum
 * (et cliens abiens), acta sine tesseris.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "tcp.h"
#include "hospitium.h"
#include "json.h"
#include "mcp_servus.h"
#include "mcp_servus_http.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUDIENTIA    "https://exemplar.example/mcp"
#define AUCTORITAS  "https://exemplar.example"
#define SCOPI       "notae:legere notae:scribere"
#define URL_DOCUMENTI \
    "https://exemplar.example/.well-known/oauth-protected-resource"
#define CAPUT_401 \
    "WWW-Authenticate: Bearer resource_metadata=\"" URL_DOCUMENTI \
    "\", scope=\"" SCOPI "\""
#define CAPUT_403 \
    "WWW-Authenticate: Bearer error=\"insufficient_scope\", " \
    "scope=\"notae:scribere\", resource_metadata=\"" URL_DOCUMENTI "\""
#define CORPUS_DOCUMENTI \
    "{\"resource\":\"" AUDIENTIA "\",\"authorization_servers\":[\"" \
    AUCTORITAS "\"],\"scopes_supported\":[\"notae:legere\"," \
    "\"notae:scribere\"],\"bearer_methods_supported\":[\"header\"]}"
#define CAPITA_N    (VIII * MXXIV)
#define ACTA_N      LXIV

/* ---- status communis ---- */

nomen structura {
     McpDilatum  dilata[VIII];
            i32  numerus_dilatorum;
            i32  vocationes;
         chorda  acta[ACTA_N];
            i32  numerus_actorum;
            i32  speculata;
        Piscina* piscina;
} Status;

/* ---- cliens: lector per connexionem ---- */

nomen structura {
    TcpConnexio* c;
             i8* data;
            s64  n;
            s64  capacitas;
            s64  finis_capitum;   /* -I = nondum */
            s64  corporis;        /* Content-Length */
            b32  finitum;
            i32  status;
} Lector;

interior vacuum
_pumpare (
    Hospitium* h,
          i32  vices)
{
    i32 i;

    per (i = ZEPHYRUM; i < vices; i++)
    {
        (vacuum)hospitium_gressus(h, I);
    }
}

interior b32
_aperire (
    Hospitium* h,
       Lector* l,
      Piscina* p,
          s64  capacitas)
{
    TcpOptiones o = tcp_optiones_default();
    TcpResultus r;

    memset(l, ZEPHYRUM, magnitudo(*l));
    o.timeout_ms = I;
    r = tcp_connectere_cum_optionibus("127.0.0.1", hospitium_portus(h),
        &o, p);
    si (!r.successus)
    {
        redde FALSUM;
    }
    l->c              = r.connexio;
    l->capacitas      = capacitas;
    l->data           = (i8*)piscina_allocare(p, (i64)capacitas + I);
    l->finis_capitum  = -I;
    l->corporis       = -I;
    redde VERUM;
}

interior b32
_mittere (
      Hospitium* h,
         Lector* l,
    constans i8* d,
            s64  n)
{
    s64 missa   = ZEPHYRUM;
    i32 inanes  = ZEPHYRUM;

    dum (missa < n)
    {
        s64 pars = n - missa;
        s32 k;

        si (pars > LXIV * MXXIV)
        {
            pars = LXIV * MXXIV;
        }
        k = tcp_mittere(l->c, d + missa, (i32)pars);
        si (k < ZEPHYRUM || (k == ZEPHYRUM && ++inanes > MM))
        {
            redde FALSUM;
        }
        si (k > ZEPHYRUM)
        {
            inanes  = ZEPHYRUM;
            missa   += (s64)k;
        }
        _pumpare(h, I);
    }
    redde VERUM;
}

/* recipere semel; VERUM si responsum totum */
interior b32
_progredi (Lector* l)
{
     i8 capsa[XVI * MXXIV];
    s32 k;

    si (l->finitum)
    {
        redde VERUM;
    }
    k = tcp_recipere(l->c, capsa, (i32)magnitudo(capsa));
    si (k == TCP_ITERUM)
    {
        redde FALSUM;
    }
    si (k <= ZEPHYRUM)
    {
        l->finitum = l->finis_capitum >= ZEPHYRUM;
        redde l->finitum;
    }
    si (l->n + k <= l->capacitas)
    {
        memcpy(l->data + l->n, capsa, (size_t)k);
    }
    l->n                                                 += (s64)k;
    l->data[l->n <= l->capacitas ? l->n : l->capacitas]  = '\0';
    si (l->finis_capitum < ZEPHYRUM)
    {
        character* f = strstr((character*)l->data, "\r\n\r\n");

        si (f != NIHIL)
        {
            character* cl = strstr((character*)l->data,
                "Content-Length: ");

            l->finis_capitum  = (s64)(f - (character*)l->data) + IV;
            l->corporis       = cl ? (s64)atol(cl + XVI) : ZEPHYRUM;
            l->status         = (i32)atoi((character*)l->data + IX);
        }
    }
    si (   l->finis_capitum >= ZEPHYRUM
        && l->n             >= l->finis_capitum + l->corporis)
    {
        l->finitum = VERUM;
    }
    redde l->finitum;
}

interior b32
_exspectare (
    Hospitium* h,
       Lector* l)
{
    i32 k;

    per (k = ZEPHYRUM; k < C * X; k++)
    {
        _pumpare(h, X);
        si (_progredi(l))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_claudere (
    Lector* l)
{
    si (l->c)
    {
        tcp_claudere(l->c);
        l->c = NIHIL;
    }
}

interior constans character*
_corpus (
    Lector* l)
{
    redde (constans character*)l->data + l->finis_capitum;
}

interior b32
_continet (
                Lector* l,
    constans character* s)
{
    redde l->finitum
        && strstr((constans character*)l->data, s) != NIHIL;
}

/* linea capitis integra ("\r\n<linea>\r\n") */
interior b32
_caput_est (
                 Lector* l,
                Piscina* p,
     constans character* linea)
{
    character* quaesitum = (character*)piscina_allocare(p,
        (i64)strlen(linea) + V);

    sprintf(quaesitum, "\r\n%s\r\n", linea);
    redde _continet(l, quaesitum);
}

/* petitio una; capita_extra lineas CRLF terminatas habent */
interior b32
_rogare (
              Hospitium* h,
                Piscina* p,
    constans character* linea,
    constans character* capita_extra,
            constans i8* corpus,
                    s64  n,
                 Lector* l)
{
    character* capita = (character*)piscina_allocare(p, CAPITA_N);

    si (!_aperire(h, l, p, LXIV * MXXIV))
    {
        redde FALSUM;
    }
    sprintf(capita, "%s HTTP/1.1\r\nHost: 127.0.0.1:%u\r\n"
        "Connection: close\r\n%sContent-Length: %ld\r\n\r\n", linea,
        (insignatus integer)hospitium_portus(h),
        capita_extra ? capita_extra : "", (longus)n);
    si (   !_mittere(h, l, (constans i8*)capita, (s64)strlen(capita))
        || (n > ZEPHYRUM && !_mittere(h, l, corpus, n)))
    {
        _claudere(l);
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
_rogare_et_legere (
              Hospitium* h,
                Piscina* p,
     constans character* linea,
     constans character* capita_extra,
     constans character* corpus,
                 Lector* l)
{
    b32 bene = _rogare(h, p, linea, capita_extra, (constans i8*)corpus,
                   corpus ? (s64)strlen(corpus) : ZEPHYRUM, l)
        && _exspectare(h, l);

    _claudere(l);
    redde bene;
}


/* ==================================================
 * servus probationis
 * ================================================== */

interior vacuum
_legere (
    McpVocatio* v)
{
       Status* st       = (Status*)mcp_vocatio_datum(v);
          s64  numerus  = ZEPHYRUM;
    character  t[LXIV];

    st->vocationes++;
    (vacuum)mcp_argumentum_integer(v, "id", &numerus);
    sprintf(t, "nota %ld", (longus)numerus);
    mcp_respondere_textum(v, chorda_ex_literis(t,
        mcp_vocatio_piscina(v)));
}

interior vacuum
_scribere (
    McpVocatio* v)
{
    Status* st = (Status*)mcp_vocatio_datum(v);

    st->vocationes++;
    mcp_respondere_textum(v, chorda_ex_literis("scriptum",
        mcp_vocatio_piscina(v)));
}

interior vacuum
_differre (
    McpVocatio* v)
{
    Status* st = (Status*)mcp_vocatio_datum(v);

    st->vocationes++;
    si (st->numerus_dilatorum < VIII)
    {
        st->dilata[st->numerus_dilatorum++] = mcp_differre(v);
    }
}

hic_manens constans McpArgumentum ARG_LEGERE[] = {
    { "id", "numerus notae", MCP_INTEGER, VERUM, NIHIL }
};

hic_manens constans McpInstrumentum INSTRUMENTA[] = {
    { "legere", "notam legit", ARG_LEGERE, I, "notae:legere", VERUM,
        FALSUM, _legere },
    { "scribere", "notam scribit", NIHIL, ZEPHYRUM, "notae:scribere",
        FALSUM, FALSUM, _scribere },
    { "differre", "postea respondet", NIHIL, ZEPHYRUM, NIHIL, FALSUM,
        FALSUM, _differre }
};

hic_manens constans character* constans ORIGINES[] = {
    "https://claude.ai", NIHIL
};

/* bona-legere / bona-omnia; audientia aliena refutatur */
interior b32
_verificator (
        vacuum* datum,
        chorda  tessera,
        chorda  audientia,
    McpPersona* exitus,
       Piscina* p)
{
    (vacuum)datum;
    si (!chorda_aequalis_literis(audientia, AUDIENTIA))
    {
        redde FALSUM;
    }
    si (chorda_aequalis_literis(tessera, "bona-legere"))
    {
        exitus->scopi = chorda_ex_literis("notae:legere", p);
    }
    alioquin si (chorda_aequalis_literis(tessera, "bona-omnia"))
    {
        exitus->scopi = chorda_ex_literis(SCOPI, p);
    }
    alioquin
    {
        redde FALSUM;
    }
    exitus->usor    = chorda_ex_literis("fran", p);
    exitus->cliens  = chorda_ex_literis("cliens-probationis", p);
    redde VERUM;
}

interior vacuum
_speculator (
                 vacuum* datum,
    HospitiumColloquium* colloquium)
{
    (vacuum)colloquium;
    ((Status*)datum)->speculata++;
}

interior vacuum
_acta (
     vacuum* datum,
     chorda  linea)
{
    Status* st = (Status*)datum;

    si (st->numerus_actorum < ACTA_N)
    {
        st->acta[st->numerus_actorum++] = chorda_transcribere(linea,
            st->piscina);
    }
}

/* hospitium + servus montatus; NIHIL si fractum */
interior Hospitium*
_hospitium (
    CredoContextus* c,
            Status* st)
{
    HospitiumConfiguratio  hospitii;
          McpConfiguratio  mc;
          McpHttpOptiones  o;
                Hospitium* h;
                McpServus* s;

    memset(st, ZEPHYRUM, magnitudo(*st));
    st->piscina = c->piscina;
    memset(&hospitii, ZEPHYRUM, magnitudo(hospitii));
    /* tectum hospitii maius: tectum nostrum (I MB) probetur */
    hospitii.petitio_maxima  = IV * MXXIV * MXXIV;
    h                        = hospitium_creare(c->piscina, &hospitii);
    si (!h)
    {
        redde NIHIL;
    }
    memset(&mc, ZEPHYRUM, magnitudo(mc));
    mc.titulus      = "probatio";
    mc.versio       = "0.1";
    mc.instrumenta  = INSTRUMENTA;
    mc.numerus      = III;
    mc.datum        = st;
    s               = mcp_servus_creare(&mc, c->piscina);
    memset(&o, ZEPHYRUM, magnitudo(o));
    o.via                  = "/mcp";
    o.audientia            = AUDIENTIA;
    o.auctoritas           = AUCTORITAS;
    o.scopi                = SCOPI;
    o.origines             = ORIGINES;
    o.verificator          = _verificator;
    o.verificatoris_datum  = st;
    o.acta                 = _acta;
    o.acta_datum           = st;
    o.speculator           = _speculator;
    o.speculatoris_datum   = st;
    si (!s || !mcp_servus_http_montare(h, s, &o, c->piscina))
    {
        hospitium_destruere(h);
        redde NIHIL;
    }
    redde h;
}

#define PING "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"ping\"}"
#define VOCATIO(instr, arg) \
    "{\"jsonrpc\":\"2.0\",\"id\":7,\"method\":\"tools/call\"," \
    "\"params\":{\"name\":\"" instr "\",\"arguments\":" arg "}}"
#define JSON_CT "Content-Type: application/json\r\n"
#define OMNIA   JSON_CT "Authorization: Bearer bona-omnia\r\n"
#define LEGERE  JSON_CT "Authorization: Bearer bona-legere\r\n"


/* ==================================================
 * sectiones
 * ================================================== */

interior vacuum
_sectio_auctoritas (CredoContextus* c)
{
        Status  st;
     Hospitium* h = _hospitium(c, &st);
        Lector  l;

    CREDO_NECESSE_NON_NIHIL(h);
    /* sine tessera, Basic, tessera ignota: 401 idem octetim */
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp", JSON_CT,
        PING, &l));
    CREDO_AEQUALIS_I32(l.status, CDI);
    CREDO_VERUM(_caput_est(&l, c->piscina, CAPUT_401));
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp",
        JSON_CT "Authorization: Basic Zm9vOmJhcg==\r\n", PING, &l));
    CREDO_AEQUALIS_I32(l.status, CDI);
    CREDO_VERUM(_caput_est(&l, c->piscina, CAPUT_401));
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp",
        JSON_CT "Authorization: Bearer mala\r\n", PING, &l));
    CREDO_AEQUALIS_I32(l.status, CDI);
    CREDO_VERUM(_caput_est(&l, c->piscina, CAPUT_401));
    /* tessera bona: 200 */
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp", OMNIA,
        PING, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(strcmp(_corpus(&l),
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"result\":{}}") == ZEPHYRUM);
    /* scopus deest: 403 octetim, tractator non cursus */
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp", LEGERE,
        VOCATIO("scribere", "{}"), &l));
    CREDO_AEQUALIS_I32(l.status, CDIII);
    CREDO_VERUM(_caput_est(&l, c->piscina, CAPUT_403));
    CREDO_AEQUALIS_I32(st.vocationes, ZEPHYRUM);
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp", LEGERE,
        VOCATIO("legere", "{\"id\":2}"), &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "nota 2"));
    /* tessera in quaestione: 400 etiam cum capite bono */
    CREDO_VERUM(_rogare_et_legere(h, c->piscina,
        "POST /mcp?access_token=bona-omnia", OMNIA, PING, &l));
    CREDO_AEQUALIS_I32(l.status, CD);
    hospitium_destruere(h);
}

interior vacuum
_sectio_methodi (CredoContextus* c)
{
        Status  st;
     Hospitium* h = _hospitium(c, &st);
        Lector  l;
            i8* magnum;
           s64  n = MXXIV * MXXIV + I;
        chorda  m;

    CREDO_NECESSE_NON_NIHIL(h);
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "GET /mcp", OMNIA,
        NIHIL,
        &l));
    CREDO_AEQUALIS_I32(l.status, CDV);
    CREDO_VERUM(_caput_est(&l, c->piscina, "Allow: POST"));
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "DELETE /mcp", OMNIA,
        NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CDV);
    /* nuntiatio: 202 sine corpore */
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp", OMNIA,
        "{\"jsonrpc\":\"2.0\","
        "\"method\":\"notifications/initialized\"}", &l));
    CREDO_AEQUALIS_I32(l.status, CCII);
    CREDO_AEQUALIS_S32((s32)l.corporis, ZEPHYRUM);
    /* fasces */
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp", OMNIA,
        "[" PING "]", &l));
    CREDO_AEQUALIS_I32(l.status, CD);
    CREDO_VERUM(_continet(&l, "-32600"));
    /* corpus > I MB: 413 */
    magnum = (i8*)piscina_allocare(c->piscina, (memoriae_index)n);
    memset(magnum, ' ', (size_t)n);
    m.datum    = magnum;
    m.mensura  = (i32)n;
    CREDO_VERUM(_rogare(h, c->piscina, "POST /mcp", OMNIA, m.datum,
        (s64)m.mensura, &l) && _exspectare(h, &l));
    _claudere(&l);
    CREDO_AEQUALIS_I32(l.status, CDXIII);
    /* Content-Type alienum: 415; charset licet */
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp",
        "Content-Type: text/plain\r\n"
        "Authorization: Bearer bona-omnia\r\n", PING, &l));
    CREDO_AEQUALIS_I32(l.status, CDXV);
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp",
        "Content-Type: application/json; charset=utf-8\r\n"
        "Authorization: Bearer bona-omnia\r\n", PING, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    hospitium_destruere(h);
}

interior vacuum
_sectio_versio (CredoContextus* c)
{
        Status  st;
     Hospitium* h = _hospitium(c, &st);
        Lector  l;

    CREDO_NECESSE_NON_NIHIL(h);
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp",
        OMNIA "MCP-Protocol-Version: 2026-07-28\r\n", PING, &l));
    CREDO_AEQUALIS_I32(l.status, CD);
    CREDO_VERUM(_continet(&l, "\"code\":-32022"));
    CREDO_VERUM(_continet(&l, "\"supported\":[\"2025-03-26\","
        "\"2025-06-18\",\"2025-11-25\"]"));
    CREDO_VERUM(_continet(&l, "\"requested\":\"2026-07-28\""));
    CREDO_VERUM(_continet(&l, "\"id\":1"));
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp",
        OMNIA "MCP-Protocol-Version: 2025-06-18\r\n", PING, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp", OMNIA,
        PING,
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    hospitium_destruere(h);
}

interior vacuum
_sectio_origo (CredoContextus* c)
{
        Status  st;
     Hospitium* h = _hospitium(c, &st);
        Lector  l;

    CREDO_NECESSE_NON_NIHIL(h);
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp",
        OMNIA "Origin: https://evil.example\r\n", PING, &l));
    CREDO_AEQUALIS_I32(l.status, CDIII);
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp",
        OMNIA "Origin: https://claude.ai\r\n", PING, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp", OMNIA,
        PING,
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    hospitium_destruere(h);
}

interior vacuum
_sectio_documentum (CredoContextus* c)
{
        Status  st;
     Hospitium* h = _hospitium(c, &st);
        Lector  l;

    CREDO_NECESSE_NON_NIHIL(h);
    CREDO_VERUM(_rogare_et_legere(h, c->piscina,
        "GET /.well-known/oauth-protected-resource", NIHIL, NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(strcmp(_corpus(&l), CORPUS_DOCUMENTI) == ZEPHYRUM);
    CREDO_VERUM(_continet(&l, "Content-Type: application/json"));
    CREDO_VERUM(_rogare_et_legere(h, c->piscina,
        "GET /.well-known/oauth-protected-resource/mcp", NIHIL, NIHIL,
        &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(strcmp(_corpus(&l), CORPUS_DOCUMENTI) == ZEPHYRUM);
    /* Mcp-Session-Id clientis ignoratur, numquam redditur */
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp",
        OMNIA "Mcp-Session-Id: sessio-clientis\r\n", PING, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_FALSUM(_continet(&l, "sessio-clientis"));
    CREDO_FALSUM(_continet(&l, "Mcp-Session-Id"));
    /* speculator: omnis via montata semel (PRM II + POST) */
    CREDO_AEQUALIS_I32(st.speculata, III);
    hospitium_destruere(h);
}

#define DIFFERRE VOCATIO("differre", "{}")

interior vacuum
_sectio_dilatum (CredoContextus* c)
{
         Status  st;
      Hospitium* h = _hospitium(c, &st);
         Lector  l;
     McpVocatio* v;
            i32  k;

    CREDO_NECESSE_NON_NIHIL(h);
    CREDO_VERUM(_rogare(h, c->piscina, "POST /mcp", OMNIA,
        (constans i8*)DIFFERRE, (s64)strlen(DIFFERRE), &l));
    per (k = ZEPHYRUM; k < L; k++)
    {
        _pumpare(h, II);
        CREDO_FALSUM(_progredi(&l));
    }
    CREDO_AEQUALIS_I32(st.numerus_dilatorum, I);
    v = mcp_dilatum_vocatio(st.dilata[ZEPHYRUM]);
    CREDO_NECESSE_NON_NIHIL(v);
    mcp_respondere_textum(v, chorda_ex_literis("serius", c->piscina));
    CREDO_VERUM(_exspectare(h, &l));
    _claudere(&l);
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "serius"));
    /* cliens abit ante responsum: nulla ruina, locus liberatur */
    CREDO_VERUM(_rogare(h, c->piscina, "POST /mcp", OMNIA,
        (constans i8*)DIFFERRE, (s64)strlen(DIFFERRE), &l));
    _pumpare(h, XX);
    CREDO_AEQUALIS_I32(st.numerus_dilatorum, II);
    _claudere(&l);
    _pumpare(h, XX);
    v = mcp_dilatum_vocatio(st.dilata[I]);
    CREDO_NECESSE_NON_NIHIL(v);
    mcp_respondere_textum(v, chorda_ex_literis("nemo", c->piscina));
    CREDO_NIHIL(mcp_dilatum_vocatio(st.dilata[I]));
    _pumpare(h, XX);
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp", OMNIA,
        VOCATIO("legere", "{\"id\":9}"), &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "nota 9"));
    hospitium_destruere(h);
}

interior b32
_actum_continet (
                Status* st,
                   i32  k,
    constans character* s)
{
    redde k < st->numerus_actorum
        && chorda_continet(st->acta[k], chorda_ex_literis(s,
               st->piscina));
}

interior vacuum
_sectio_acta (CredoContextus* c)
{
        Status  st;
     Hospitium* h = _hospitium(c, &st);
        Lector  l;
           i32  k;

    CREDO_NECESSE_NON_NIHIL(h);
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp", OMNIA,
        VOCATIO("legere", "{\"id\":3}"), &l));
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp",
        JSON_CT "Authorization: Bearer secretum-malum-XYZ\r\n", PING,
        &l));
    CREDO_VERUM(_rogare_et_legere(h, c->piscina, "POST /mcp", LEGERE,
        VOCATIO("scribere", "{}"), &l));
    CREDO_AEQUALIS_I32(st.numerus_actorum, III);
    CREDO_VERUM(_actum_continet(&st, ZEPHYRUM, "status=200"));
    CREDO_VERUM(_actum_continet(&st, ZEPHYRUM, "usor=fran"));
    CREDO_VERUM(_actum_continet(&st, ZEPHYRUM,
        "cliens=cliens-probationis"));
    CREDO_VERUM(_actum_continet(&st, ZEPHYRUM, "instrumentum=legere"));
    CREDO_VERUM(_actum_continet(&st, ZEPHYRUM, "exitus=bene"));
    CREDO_VERUM(_actum_continet(&st, ZEPHYRUM, " ms="));
    CREDO_VERUM(_actum_continet(&st, I, "status=401"));
    CREDO_VERUM(_actum_continet(&st, II, "status=403"));
    CREDO_VERUM(_actum_continet(&st, II, "exitus=scopus"));
    CREDO_VERUM(_actum_continet(&st, II, "instrumentum=scribere"));
    per (k = ZEPHYRUM; k < st.numerus_actorum; k++)
    {
        CREDO_FALSUM(_actum_continet(&st, k, "bona-"));
        CREDO_FALSUM(_actum_continet(&st, k, "secretum-malum-XYZ"));
    }
    hospitium_destruere(h);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "auctoritas", _sectio_auctoritas, NIHIL, NIHIL, NIHIL, NIHIL },
    { "methodi", _sectio_methodi, NIHIL, NIHIL, NIHIL, NIHIL },
    { "versio", _sectio_versio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "origo", _sectio_origo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "documentum", _sectio_documentum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "dilatum", _sectio_dilatum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "acta", _sectio_acta, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("mcp_servus_http", SECTIONES);
}
