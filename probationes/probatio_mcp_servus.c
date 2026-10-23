/* probatio_mcp_servus.c - nucleus MCP (mcp-remotus T3): servus
 * probationis cum tribus instrumentis (legere: notae:legere, id
 * integer necessarium; scribere: notae:scribere, textus necessarius,
 * color inter electiones; differre: postea respondet). Index contra
 * plagulam auream (probationes/fixa/mcp_servus/instrumenta_index.json,
 * sha256 infra, manu scripta ANTE nucleum), argumenta iudicata ante
 * tractatorem, scopi a nucleo, responsa dilata cum generatione,
 * involucrum JSON-RPC, initialize, stdio per tubos.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "filum.h"
#include "json.h"
#include "mcp_servus.h"
#include "sigillum.h"
#include "tabellarius.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define VIA_INDICIS "probationes/fixa/mcp_servus/instrumenta_index.json"
#define SIGILLUM_INDICIS \
    "565b0db1ae1e2d07a10694705b3ef1259c31e622c7fdcfcdbc1ec2b6d692efb7"

nomen structura {
                    i32  vocationes;   /* tractatores cursi */
                    i32  reddita;      /* reddere vocatum */
        McpReditusGenus  genus;
                 chorda  corpus;
     constans character* scopus;
     constans character* instrumentum;
     constans character* exitus;
             McpDilatum  dilata[LXV];
                    i32  numerus_dilatorum;
                Piscina* piscina;
} Status;

interior chorda
_c (
    constans character* s,
               Piscina* p)
{
    redde chorda_ex_literis(s, p);
}

interior b32
_continet (
                 chorda  fenum,
     constans character* acus)
{
    chorda a;

    a.datum    = (i8*)(vacuum*)(memoriae_index)acus;
    a.mensura  = (i32)strlen(acus);
    redde chorda_continet(fenum, a);
}


/* ==================================================
 * tractatores
 * ================================================== */

interior vacuum
_legere (
    McpVocatio* v)
{
       Status* st       = (Status*)mcp_vocatio_datum(v);
          s64  numerus  = ZEPHYRUM;
    character  t[LXIV];

    st->vocationes++;
    si (!mcp_argumentum_integer(v, "id", &numerus))
    {
        mcp_respondere_vitium(v, _c("id deest",
            mcp_vocatio_piscina(v)));
        redde;
    }
    sprintf(t, "nota %ld", (longus)numerus);
    mcp_respondere_textum(v, _c(t, mcp_vocatio_piscina(v)));
}

/* "<textus>|<color>" aut "<textus>|sine colore" */
interior vacuum
_scribere (
    McpVocatio* v)
{
               Status* st = (Status*)mcp_vocatio_datum(v);
               chorda  textus;
               chorda  color;
    ChordaAedificator* a;

    st->vocationes++;
    a = chorda_aedificator_creare(mcp_vocatio_piscina(v), LXIV);
    si (!mcp_argumentum_chorda(v, "textus", &textus))
    {
        mcp_respondere_vitium(v, _c("textus deest",
            mcp_vocatio_piscina(v)));
        redde;
    }
    chorda_aedificator_appendere_chorda(a, textus);
    chorda_aedificator_appendere_literis(a, "|");
    si (mcp_argumentum_chorda(v, "color", &color))
    {
        chorda_aedificator_appendere_chorda(a, color);
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(a, "sine colore");
    }
    mcp_respondere_textum(v, chorda_aedificator_finire(a));
}

interior vacuum
_differre (
    McpVocatio* v)
{
    Status* st = (Status*)mcp_vocatio_datum(v);

    st->vocationes++;
    si (st->numerus_dilatorum < LXV)
    {
        st->dilata[st->numerus_dilatorum++] = mcp_differre(v);
    }
}

hic_manens constans character* constans COLORES[] = {
    "rubrum", "viride", NIHIL
};

hic_manens constans McpArgumentum ARG_LEGERE[] = {
    { "id", "numerus notae", MCP_INTEGER, VERUM, NIHIL }
};

hic_manens constans McpArgumentum ARG_SCRIBERE[] = {
    { "textus", "textus notae", MCP_CHORDA, VERUM, NIHIL },
    { "color", "color notae", MCP_CHORDA, FALSUM, COLORES }
};

hic_manens constans McpInstrumentum INSTRUMENTA[] = {
    { "legere", "notam legit", ARG_LEGERE, I, "notae:legere", VERUM,
        FALSUM, _legere },
    { "scribere", "notam scribit", ARG_SCRIBERE, II, "notae:scribere",
        FALSUM, FALSUM, _scribere },
    { "differre", "postea respondet", NIHIL, ZEPHYRUM, NIHIL, FALSUM,
        FALSUM, _differre }
};


/* ==================================================
 * auxilia
 * ================================================== */

interior vacuum
_reddere (
                 vacuum* datum,
    constans McpReditus* r)
{
    Status* st = (Status*)datum;

    st->reddita++;
    st->genus   = r->genus;
    st->corpus  = r->corpus.mensura > ZEPHYRUM
        ? chorda_transcribere(r->corpus, st->piscina) : r->corpus;
    st->scopus        = r->scopus_requisitus;
    st->instrumentum  = r->instrumentum;
    st->exitus        = r->exitus;
}

interior McpServus*
_servus (
    CredoContextus* c,
            Status* st)
{
    McpConfiguratio cfg;

    memset(st, ZEPHYRUM, magnitudo(*st));
    st->piscina        = c->piscina;
    cfg.titulus        = "probatio";
    cfg.versio         = "0.1";
    cfg.instructiones  = "instructiones probationis";
    cfg.instrumenta    = INSTRUMENTA;
    cfg.numerus        = III;
    cfg.datum          = st;
    redde mcp_servus_creare(&cfg, c->piscina);
}

interior McpPersona
_persona (
    constans character* scopi,
               Piscina* p)
{
    McpPersona q;

    q.usor    = _c("fran", p);
    q.cliens  = _c("probatio", p);
    q.scopi   = _c(scopi, p);
    redde q;
}

/* petitio una per nucleum; reddita numerantur in st */
interior b32
_petere (
         CredoContextus* c,
              McpServus* s,
                 Status* st,
     constans character* corpus,
    constans McpPersona* persona,
                    b32  initium)
{
    redde mcp_servus_tractare(s, _c(corpus, c->piscina), persona,
        initium, _reddere, st, c->piscina);
}


/* ==================================================
 * sectiones
 * ================================================== */

interior vacuum
_sectio_index (CredoContextus* c)
{
        Status  st;
     McpServus* s       = _servus(c, &st);
    McpPersona  q       = _persona("", c->piscina);
        chorda  aureum  = filum_legere_totum(VIA_INDICIS, c->piscina);
      Sigillum  sg;
     character  hex[SIGILLUM_HEX_MENSURA];

    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_NECESSE(aureum.mensura > I);
    sg = sigillum_computare(aureum.datum,
        (memoriae_index)aureum.mensura);
    sigillum_hex(&sg, hex);
    CREDO_VERUM(strcmp(hex, SIGILLUM_INDICIS) == ZEPHYRUM);
    aureum.mensura--;   /* '\n' finalis */
    CREDO_VERUM(_petere(c, s, &st,
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"tools/list\"}", &q,
        FALSUM));
    CREDO_AEQUALIS_I32(st.reddita, I);
    CREDO_AEQUALIS_S32((s32)st.genus, (s32)MCP_REDITUS_RESPONSUM);
    CREDO_CHORDA_AEQUALIS(st.corpus, aureum);
}

/* -32602 nominans argumentum, tractator numquam cursus */
interior vacuum
_refutatum (
       CredoContextus* c,
            McpServus* s,
               Status* st,
    constans character* corpus,
    constans McpPersona* q,
    constans character* nominatum)
{
    i32 ante = st->vocationes;

    CREDO_VERUM(_petere(c, s, st, corpus, q, FALSUM));
    CREDO_AEQUALIS_I32(st->vocationes, ante);
    CREDO_AEQUALIS_S32((s32)st->genus, (s32)MCP_REDITUS_RESPONSUM);
    CREDO_VERUM(_continet(st->corpus, "-32602"));
    CREDO_VERUM(_continet(st->corpus, nominatum));
}

#define VOCATIO(instr, arg) \
    "{\"jsonrpc\":\"2.0\",\"id\":7,\"method\":\"tools/call\"," \
    "\"params\":{\"name\":\"" instr "\",\"arguments\":" arg "}}"

interior vacuum
_sectio_argumenta (CredoContextus* c)
{
        Status  st;
     McpServus* s = _servus(c, &st);
    McpPersona  q = _persona("notae:legere notae:scribere", c->piscina);

    CREDO_NECESSE_NON_NIHIL(s);
    _refutatum(c, s, &st, VOCATIO("legere", "{\"id\":1,\"extra\":2}"),
        &q, "extra");
    _refutatum(c, s, &st, VOCATIO("legere", "{}"), &q, "id");
    _refutatum(c, s, &st, VOCATIO("legere", "{\"id\":\"7\"}"), &q,
        "id");
    _refutatum(c, s, &st, VOCATIO("scribere",
        "{\"textus\":\"a\",\"color\":\"caeruleum\"}"), &q, "color");
    _refutatum(c, s, &st, VOCATIO("ignotum", "{}"), &q, "ignotum");
    _refutatum(c, s, &st, VOCATIO("legere", "[1]"), &q, "arguments");
    /* recte: tractator currit */
    CREDO_VERUM(_petere(c, s, &st, VOCATIO("legere", "{\"id\":4}"), &q,
        FALSUM));
    CREDO_AEQUALIS_I32(st.vocationes, I);
    CREDO_VERUM(_continet(st.corpus,
        "\"content\":[{\"type\":\"text\",\"text\":\"nota 4\"}]"));
    CREDO_VERUM(_continet(st.corpus, "\"isError\":false"));
}

/* ut clientes vere mittunt (Review focus 4) */
interior vacuum
_sectio_argumenta_vera (CredoContextus* c)
{
        Status  st;
     McpServus* s = _servus(c, &st);
    McpPersona  q = _persona("notae:legere notae:scribere", c->piscina);
    McpVocatio* v;

    CREDO_NECESSE_NON_NIHIL(s);
    /* arguments absens, nulla necessaria */
    CREDO_FALSUM(_petere(c, s, &st,
        "{\"jsonrpc\":\"2.0\",\"id\":8,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"differre\"}}", &q, FALSUM));
    CREDO_AEQUALIS_I32(st.vocationes, I);
    v = mcp_dilatum_vocatio(st.dilata[ZEPHYRUM]);
    CREDO_NECESSE_NON_NIHIL(v);
    mcp_respondere_textum(v, _c("factum", c->piscina));
    CREDO_AEQUALIS_I32(st.reddita, I);
    /* params._meta ignoratur */
    CREDO_VERUM(_petere(c, s, &st,
        "{\"jsonrpc\":\"2.0\",\"id\":9,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"legere\",\"arguments\":{\"id\":3},"
        "\"_meta\":{\"progressToken\":1}}}", &q, FALSUM));
    CREDO_VERUM(_continet(st.corpus, "nota 3"));
    /* null pro optionali = absens */
    CREDO_VERUM(_petere(c, s, &st, VOCATIO("scribere",
        "{\"textus\":\"a\",\"color\":null}"), &q, FALSUM));
    CREDO_VERUM(_continet(st.corpus, "a|sine colore"));
    CREDO_VERUM(_petere(c, s, &st, VOCATIO("scribere",
        "{\"textus\":\"b\",\"color\":\"viride\"}"), &q, FALSUM));
    CREDO_VERUM(_continet(st.corpus, "b|viride"));
    /* numerus pro chorda, null pro necessario, fluitans pro integro */
    _refutatum(c, s, &st, VOCATIO("scribere", "{\"textus\":5}"), &q,
        "textus");
    _refutatum(c, s, &st, VOCATIO("legere", "{\"id\":null}"), &q, "id");
    _refutatum(c, s, &st, VOCATIO("legere", "{\"id\":2.5}"), &q, "id");
}

interior vacuum
_sectio_scopi (CredoContextus* c)
{
        Status  st;
     McpServus* s = _servus(c, &st);
    McpPersona  q = _persona("notae:legere", c->piscina);
    McpPersona  r = _persona("notae:scriberex notae:legere",
        c->piscina);
    McpPersona nemo = _persona("", c->piscina);

    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_VERUM(_petere(c, s, &st, VOCATIO("scribere",
        "{\"textus\":\"a\"}"), &q, FALSUM));
    CREDO_AEQUALIS_S32((s32)st.genus, (s32)MCP_REDITUS_SCOPUS);
    CREDO_NECESSE_NON_NIHIL(st.scopus);
    CREDO_VERUM(strcmp(st.scopus, "notae:scribere") == ZEPHYRUM);
    CREDO_VERUM(_continet(st.corpus, "notae:scribere"));
    CREDO_AEQUALIS_I32(st.vocationes, ZEPHYRUM);
    /* signum integrum, non praefixum */
    CREDO_VERUM(_petere(c, s, &st, VOCATIO("scribere",
        "{\"textus\":\"a\"}"), &r, FALSUM));
    CREDO_AEQUALIS_S32((s32)st.genus, (s32)MCP_REDITUS_SCOPUS);
    CREDO_VERUM(_petere(c, s, &st, VOCATIO("legere", "{\"id\":1}"),
        &nemo, FALSUM));
    CREDO_AEQUALIS_S32((s32)st.genus, (s32)MCP_REDITUS_SCOPUS);
    CREDO_VERUM(_petere(c, s, &st, VOCATIO("legere", "{\"id\":1}"),
        NIHIL, FALSUM));
    CREDO_AEQUALIS_S32((s32)st.genus, (s32)MCP_REDITUS_SCOPUS);
    CREDO_AEQUALIS_I32(st.vocationes, ZEPHYRUM);
    /* scopus habitus: currit; instrumentum sine scopo: omnibus */
    CREDO_VERUM(_petere(c, s, &st, VOCATIO("legere", "{\"id\":1}"), &q,
        FALSUM));
    CREDO_AEQUALIS_S32((s32)st.genus, (s32)MCP_REDITUS_RESPONSUM);
    CREDO_AEQUALIS_I32(st.vocationes, I);
    /* index omnia monstrat, scopis non obstantibus */
    CREDO_VERUM(_petere(c, s, &st,
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"tools/list\"}",
        &nemo, FALSUM));
    CREDO_VERUM(_continet(st.corpus, "\"name\":\"legere\""));
    CREDO_VERUM(_continet(st.corpus, "\"name\":\"scribere\""));
    CREDO_VERUM(_continet(st.corpus, "\"name\":\"differre\""));
}

#define DIFFERRE \
    "{\"jsonrpc\":\"2.0\",\"id\":5,\"method\":\"tools/call\"," \
    "\"params\":{\"name\":\"differre\",\"arguments\":{}}}"

interior vacuum
_sectio_dilatum (CredoContextus* c)
{
         Status  st;
      McpServus* s = _servus(c, &st);
     McpPersona  q = _persona("", c->piscina);
     McpVocatio* v;
     McpVocatio* w;
     McpDilatum  prior;
            i32  k;

    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_FALSUM(_petere(c, s, &st, DIFFERRE, &q, FALSUM));
    CREDO_AEQUALIS_I32(st.reddita, ZEPHYRUM);
    prior  = st.dilata[ZEPHYRUM];
    v      = mcp_dilatum_vocatio(prior);
    CREDO_NECESSE_NON_NIHIL(v);
    mcp_respondere_textum(v, _c("serius", c->piscina));
    CREDO_AEQUALIS_I32(st.reddita, I);
    CREDO_VERUM(_continet(st.corpus, "serius"));
    CREDO_VERUM(_continet(st.corpus, "\"id\":5"));
    /* responsum alterum nihil agit */
    CREDO_NIHIL(mcp_dilatum_vocatio(prior));
    mcp_respondere_textum(v, _c("iterum", c->piscina));
    CREDO_AEQUALIS_I32(st.reddita, I);
    /* locus reusus, generatio nova: manubrium vetus irritum */
    CREDO_FALSUM(_petere(c, s, &st, DIFFERRE, &q, FALSUM));
    CREDO_NIHIL(mcp_dilatum_vocatio(prior));
    w = mcp_dilatum_vocatio(st.dilata[I]);
    CREDO_NECESSE_NON_NIHIL(w);
    mcp_respondere_textum(w, _c("novum", c->piscina));
    CREDO_AEQUALIS_I32(st.reddita, II);
    CREDO_VERUM(_continet(st.corpus, "novum"));
    /* LXIV pendentes; LXV-a vocatio: error internus, non ruina */
    st.numerus_dilatorum  = ZEPHYRUM;
    st.reddita            = ZEPHYRUM;
    per (k = ZEPHYRUM; k < MCP_VOCATIONES_MAXIMAE; k++)
    {
        CREDO_FALSUM(_petere(c, s, &st, DIFFERRE, &q, FALSUM));
    }
    CREDO_AEQUALIS_I32(st.reddita, ZEPHYRUM);
    CREDO_VERUM(_petere(c, s, &st, DIFFERRE, &q, FALSUM));
    CREDO_AEQUALIS_I32(st.reddita, I);
    CREDO_VERUM(_continet(st.corpus, "-32603"));
    CREDO_AEQUALIS_I32(st.numerus_dilatorum, MCP_VOCATIONES_MAXIMAE);
    per (k = ZEPHYRUM; k < MCP_VOCATIONES_MAXIMAE; k++)
    {
        v = mcp_dilatum_vocatio(st.dilata[k]);
        si (v)
        {
            mcp_respondere_textum(v, _c("finis", c->piscina));
        }
    }
    CREDO_AEQUALIS_I32(st.reddita, I + MCP_VOCATIONES_MAXIMAE);
    CREDO_VERUM(_petere(c, s, &st, VOCATIO("differre", "{}"), &q,
        FALSUM) == FALSUM);
}

interior vacuum
_sectio_rpc (CredoContextus* c)
{
        Status  st;
     McpServus* s = _servus(c, &st);
    McpPersona  q = _persona("", c->piscina);

    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_VERUM(_petere(c, s, &st, "{nope", &q, FALSUM));
    CREDO_AEQUALIS_S32((s32)st.genus, (s32)MCP_REDITUS_PRAVUM);
    CREDO_VERUM(_continet(st.corpus, "-32700"));
    CREDO_VERUM(_continet(st.corpus, "\"id\":null"));
    CREDO_VERUM(_petere(c, s, &st, "{\"jsonrpc\":\"2.0\",\"id\":1}", &q,
        FALSUM));
    CREDO_AEQUALIS_S32((s32)st.genus, (s32)MCP_REDITUS_PRAVUM);
    CREDO_VERUM(_continet(st.corpus, "-32600"));
    CREDO_VERUM(_petere(c, s, &st,
        "[{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"ping\"}]", &q,
        FALSUM));
    CREDO_AEQUALIS_S32((s32)st.genus, (s32)MCP_REDITUS_PRAVUM);
    CREDO_VERUM(_continet(st.corpus, "-32600"));
    CREDO_VERUM(_petere(c, s, &st,
        "{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":\"res/ignota\"}", &q,
        FALSUM));
    CREDO_AEQUALIS_S32((s32)st.genus, (s32)MCP_REDITUS_RESPONSUM);
    CREDO_VERUM(_continet(st.corpus, "-32601"));
    CREDO_VERUM(_petere(c, s, &st,
        "{\"jsonrpc\":\"2.0\","
        "\"method\":\"notifications/initialized\"}",
        &q, FALSUM));
    CREDO_AEQUALIS_S32((s32)st.genus, (s32)MCP_REDITUS_NUNTIATIO);
    CREDO_AEQUALIS_I32(st.corpus.mensura, ZEPHYRUM);
    CREDO_VERUM(_petere(c, s, &st,
        "{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":\"ping\"}", &q,
        FALSUM));
    CREDO_AEQUALIS_S32((s32)st.genus, (s32)MCP_REDITUS_RESPONSUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(st.corpus,
        "{\"jsonrpc\":\"2.0\",\"id\":3,\"result\":{}}");
    CREDO_AEQUALIS_I32(st.reddita, VI);
}

#define INITIUM(versio) \
    "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\"," \
    "\"params\":{\"protocolVersion\":\"" versio "\"," \
    "\"capabilities\":{},\"clientInfo\":{\"name\":\"Anthropic\"," \
    "\"version\":\"1.0.0\"}}}"

#define INDEX "{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":\"tools/list\"}"

/* protocolVersion in resultato */
interior chorda
_versio (
    CredoContextus* c,
            chorda  corpus)
{
    JsonResultus r = json_legere(corpus, c->piscina);

    redde json_ad_chorda(json_via(r.radix, "result.protocolVersion"));
}

interior vacuum
_sectio_initium (CredoContextus* c)
{
          Status  st;
       McpServus* s = _servus(c, &st);
          Status  st2;
       McpServus* t = _servus(c, &st2);
      McpPersona  q = _persona("", c->piscina);
    JsonResultus  r;

    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_NECESSE_NON_NIHIL(t);
    /* initium postulatum (stdio): index ante initialize -32002 */
    CREDO_VERUM(_petere(c, s, &st, INDEX, &q, VERUM));
    CREDO_VERUM(_continet(st.corpus, "-32002"));
    CREDO_VERUM(_petere(c, s, &st, INITIUM("2025-06-18"), &q, VERUM));
    CREDO_CHORDA_AEQUALIS_LITERIS(_versio(c, st.corpus), "2025-06-18");
    r = json_legere(st.corpus, c->piscina);
    CREDO_VERUM(json_est_objectum(json_via(r.radix,
        "result.capabilities.tools")));
    CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(json_via(r.radix,
        "result.serverInfo.name")), "probatio");
    CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(json_via(r.radix,
        "result.serverInfo.version")), "0.1");
    CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(json_via(r.radix,
        "result.instructions")), "instructiones probationis");
    CREDO_VERUM(_petere(c, s, &st, INDEX, &q, VERUM));
    CREDO_VERUM(_continet(st.corpus, "\"tools\":["));
    /* versiones: nota reddita, ignota -> novissima nostra */
    CREDO_VERUM(_petere(c, s, &st, INITIUM("2025-11-25"), &q, VERUM));
    CREDO_CHORDA_AEQUALIS_LITERIS(_versio(c, st.corpus), "2025-11-25");
    CREDO_VERUM(_petere(c, s, &st, INITIUM("2025-03-26"), &q, VERUM));
    CREDO_CHORDA_AEQUALIS_LITERIS(_versio(c, st.corpus), "2025-03-26");
    CREDO_VERUM(_petere(c, s, &st, INITIUM("2026-07-28"), &q, VERUM));
    CREDO_CHORDA_AEQUALIS_LITERIS(_versio(c, st.corpus), "2025-11-25");
    CREDO_VERUM(_petere(c, s, &st, INITIUM("1999-01-01"), &q, VERUM));
    CREDO_CHORDA_AEQUALIS_LITERIS(_versio(c, st.corpus), "2025-11-25");
    CREDO_VERUM(_petere(c, s, &st,
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\"}", &q,
        VERUM));
    CREDO_CHORDA_AEQUALIS_LITERIS(_versio(c, st.corpus), "2025-11-25");
    /* sine initio postulato (HTTP): index statim */
    CREDO_VERUM(_petere(c, t, &st2, INDEX, &q, FALSUM));
    CREDO_VERUM(_continet(st2.corpus, "\"tools\":["));
}

/* tubi: tres lineae intrant, tres exeunt, EOF -> 0 */
interior vacuum
_sectio_stdio (CredoContextus* c)
{
         Status  st;
      McpServus* s = _servus(c, &st);
     McpPersona  q = _persona("notae:legere", c->piscina);
        integer  a[II];
        integer  b[II];
          FILE*  scriptor;
          FILE*  intra;
          FILE*  extra;
          FILE*  lector;
            s32  rc;
         chorda  lineae[IV];
            i32  n = ZEPHYRUM;
            b32  finitus = FALSUM;

    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_NECESSE(pipe(a) == ZEPHYRUM);
    CREDO_NECESSE(pipe(b) == ZEPHYRUM);
    scriptor = fdopen(a[I], "w");
    CREDO_NECESSE_NON_NIHIL(scriptor);
    fputs(INDEX "\n", scriptor);
    fputs(INITIUM("2025-11-25") "\n", scriptor);
    fputs(VOCATIO("legere", "{\"id\":6}") "\n", scriptor);
    fclose(scriptor);
    intra = fdopen(a[ZEPHYRUM], "r");
    extra = fdopen(b[I], "w");
    CREDO_NECESSE_NON_NIHIL(intra);
    CREDO_NECESSE_NON_NIHIL(extra);
    rc = mcp_servus_stdio_currere(s, intra, extra, &q);
    fclose(intra);
    fclose(extra);
    CREDO_AEQUALIS_S32(rc, ZEPHYRUM);
    lector = fdopen(b[ZEPHYRUM], "r");
    CREDO_NECESSE_NON_NIHIL(lector);
    dum (n < IV)
    {
        chorda l = tabellarius_lineam_legere(lector, c->piscina,
            &finitus);

        si (finitus)
        {
            frange;
        }
        lineae[n++] = l;
    }
    fclose(lector);
    CREDO_AEQUALIS_I32(n, III);
    si (n == III)
    {
        CREDO_VERUM(_continet(lineae[ZEPHYRUM], "-32002"));
        CREDO_VERUM(_continet(lineae[I], "\"protocolVersion\""));
        CREDO_VERUM(_continet(lineae[II], "nota 6"));
    }
}

interior b32
_par (
    constans character* a,
    constans character* b)
{
    redde (a == NIHIL
        || b == NIHIL) ? a == b : strcmp(a, b) == ZEPHYRUM;
}

/* instrumentum et exitus in reditu (acta vecturae ea legunt) */
interior vacuum
_sectio_exitus (CredoContextus* c)
{
        Status  st;
     McpServus* s = _servus(c, &st);
    McpPersona  q = _persona("notae:legere", c->piscina);

    CREDO_NECESSE_NON_NIHIL(s);
    (vacuum)_petere(c, s, &st, VOCATIO("legere", "{\"id\":1}"), &q,
        FALSUM);
    CREDO_VERUM(_par(st.instrumentum, "legere"));
    CREDO_VERUM(_par(st.exitus, "bene"));
    (vacuum)_petere(c, s, &st, VOCATIO("legere", "{}"), &q, FALSUM);
    CREDO_VERUM(_par(st.instrumentum, "legere"));
    CREDO_VERUM(_par(st.exitus, "error"));
    (vacuum)_petere(c, s, &st, VOCATIO("ignotum", "{}"), &q, FALSUM);
    CREDO_VERUM(_par(st.instrumentum, NIHIL));
    CREDO_VERUM(_par(st.exitus, "error"));
    (vacuum)_petere(c, s, &st, VOCATIO("scribere",
        "{\"textus\":\"a\"}"),
        &q, FALSUM);
    CREDO_VERUM(_par(st.instrumentum, "scribere"));
    CREDO_VERUM(_par(st.exitus, "scopus"));
    (vacuum)_petere(c, s, &st,
        "{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":\"ping\"}", &q,
        FALSUM);
    CREDO_VERUM(_par(st.instrumentum, NIHIL));
    CREDO_VERUM(_par(st.exitus, "bene"));
    (vacuum)_petere(c, s, &st,
        "{\"jsonrpc\":\"2.0\",\"method\":\"notifications/x\"}", &q,
        FALSUM);
    CREDO_VERUM(_par(st.exitus, "nuntiatio"));
    (vacuum)_petere(c, s, &st, "{nope", &q, FALSUM);
    CREDO_VERUM(_par(st.exitus, "pravum"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "index", _sectio_index, NIHIL, NIHIL, NIHIL, NIHIL },
    { "argumenta", _sectio_argumenta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "argumenta_vera", _sectio_argumenta_vera, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "scopi", _sectio_scopi, NIHIL, NIHIL, NIHIL, NIHIL },
    { "dilatum", _sectio_dilatum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rpc", _sectio_rpc, NIHIL, NIHIL, NIHIL, NIHIL },
    { "initium", _sectio_initium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "stdio", _sectio_stdio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "exitus", _sectio_exitus, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("mcp_servus", SECTIONES);
}
