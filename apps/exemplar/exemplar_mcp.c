/* exemplar_mcp.c - viae MCP et OAuth exemplaris (vide exemplar_mcp.h)
 *
 * T7: servus MCP tertius. Quattuor instrumenta notarum (notas
 * personae SOLUM, per usorem tesserae); acta MCP via "claude" cum
 * cliente. admissio (OAuth 2.1) in volumine exemplaris, intratio per
 * sessionem exemplaris; mcp_servus_http cum admissio_verificator.
 *
 * Sonda (T1): petitionem unam lineam unam scribit: methodus, uri,
 * OMNIA capita (Authorization et Proxy-Authorization schema solum,
 * Cookie et Set-Cookie redacta), corpus usque ad IV KB effugiis tutum
 * - corpora /oauth/token et POST /oauth/authorize numquam, corpora
 * /mcp ut compendium solum (methodus, versio, cliens, instrumentum;
 * numquam argumenta). Nunc per
 * speculatores bibliothecarum
 * vocatur.
 */
#include "postulata_posix.h"
#include "exemplar_mcp.h"
#include "json.h"
#include "chorda_aedificator.h"
#include "http.h"
#include "mcp_servus_http.h"

#include <stdio.h>
#include <string.h>

#define SONDA_CORPUS_MAXIMUM (IV * MXXIV)

hic_manens constans character* constans SCOPI[] = {
    "notae:legere", "notae:scribere", NIHIL
};


/* ==================================================
 * SONDA
 * ================================================== */

interior b32
_titulus_est (
                 chorda  titulus,
     constans character* nomen_capitis)
{
    s32 i;
    s32 n = (s32)strlen(nomen_capitis);

    si ((s32)titulus.mensura != n)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        character a = (character)titulus.datum[i];
        character b = nomen_capitis[i];

        si (a >= 'A' && a <= 'Z')
        {
            a = (character)(a - 'A' + 'a');
        }
        si (b >= 'A' && b <= 'Z')
        {
            b = (character)(b - 'A' + 'a');
        }
        si (a != b)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* octeti tuti: ASCII imprimibilis praeter '\\'; cetera \xHH */
interior vacuum
_tutum_appendere (
    ChordaAedificator* a,
               chorda  s,
                  s32  maximum)
{
    s32 i;
    s32 n = (s32)s.mensura < maximum ? (s32)s.mensura : maximum;

    per (i = ZEPHYRUM; i < n; i++)
    {
        i32 o = (i32)s.datum[i];

        si (o >= 0x20 && o < 0x7F && o != '\\')
        {
            chorda_aedificator_appendere_character(a, (character)o);
        }
        alioquin
        {
            character h[VIII];

            sprintf(h, "\\x%02X", (insignatus integer)o);
            chorda_aedificator_appendere_literis(a, h);
        }
    }
    si ((s32)s.mensura > maximum)
    {
        chorda_aedificator_appendere_literis(a, "...");
    }
}

/* compendium corporis JSON-RPC: methodus, id, versio protocolli,
 * cliens (initialize), instrumentum (tools/call) - nihil aliud */
interior vacuum
_compendium_mcp (
    ChordaAedificator* a,
               chorda  corpus,
              Piscina* p)
{
    JsonResultus  r = json_legere(corpus, p);
       JsonValor* params;
       JsonValor* v;
       character  n[XXXII];

    si (!r.successus || !json_est_objectum(r.radix))
    {
        sprintf(n, "[non objectum, %u octeti]",
            (insignatus integer)corpus.mensura);
        chorda_aedificator_appendere_literis(a, n);
        redde;
    }
    params = json_objectum_capere(r.radix, "params");
    chorda_aedificator_appendere_literis(a, "methodus=");
    _tutum_appendere(a, json_ad_chorda(json_objectum_capere(r.radix,
        "method")), LXIV);
    v = json_objectum_capere(r.radix, "id");
    si (v)
    {
        chorda_aedificator_appendere_literis(a, " id=");
        _tutum_appendere(a, json_scribere(v, p), XXXII);
    }
    v = json_objectum_capere(params, "protocolVersion");
    si (!v)
    {
        v = json_objectum_capere(json_objectum_capere(params, "_meta"),
            "io.modelcontextprotocol/protocolVersion");
    }
    si (json_est_chorda(v))
    {
        chorda_aedificator_appendere_literis(a, " versio=");
        _tutum_appendere(a, json_ad_chorda(v), XXXII);
    }
    v = json_objectum_capere(params, "clientInfo");
    si (json_est_objectum(v))
    {
        chorda_aedificator_appendere_literis(a, " cliens=");
        _tutum_appendere(a, json_ad_chorda(json_objectum_capere(v,
            "name")), LXIV);
        chorda_aedificator_appendere_literis(a, "/");
        _tutum_appendere(a, json_ad_chorda(json_objectum_capere(v,
            "version")), XXXII);
    }
    v = json_objectum_capere(params, "name");
    si (json_est_chorda(v))
    {
        chorda_aedificator_appendere_literis(a, " instrumentum=");
        _tutum_appendere(a, json_ad_chorda(v), LXIV);
    }
}

interior vacuum
_sonda (
               Exemplar* e,
    HospitiumColloquium* col)
{
     constans HttpPetitioServeri* r = colloquium_petitio(col);
               ChordaAedificator* a;
                             s32  i;
                          chorda  linea;

    si (!e->cfg.sonda || r == NIHIL)
    {
        redde;
    }
    a = chorda_aedificator_creare(colloquium_piscina(col), DXII);
    chorda_aedificator_appendere_literis(a, "sonda ");
    chorda_aedificator_appendere_literis(a, http_methodus_nomen(
        r->methodus));
    chorda_aedificator_appendere_character(a, ' ');
    _tutum_appendere(a, r->uri, DXII);
    per (i = ZEPHYRUM; i < (s32)r->capita_numerus; i++)
    {
        chorda t = r->capita[i].titulus;
        chorda v = r->capita[i].valor;

        chorda_aedificator_appendere_literis(a, " | ");
        _tutum_appendere(a, t, LXIV);
        chorda_aedificator_appendere_literis(a, ": ");
        si (   _titulus_est(t, "authorization")
            || _titulus_est(t, "proxy-authorization"))
        {
            s32 k = ZEPHYRUM;

            /* schema solum ("Bearer"), numquam tessera */
            dum (k < (s32)v.mensura && v.datum[k] != ' ')
            {
                k++;
            }
            _tutum_appendere(a, chorda_sectio(v, ZEPHYRUM, (i32)k),
                XVI);
            chorda_aedificator_appendere_literis(a, " [redacta]");
        }
        alioquin si (   _titulus_est(t, "cookie")
                     || _titulus_est(t, "set-cookie"))
        {
            chorda_aedificator_appendere_literis(a, "[redacta]");
        }
        alioquin
        {
            _tutum_appendere(a, v, CCLVI);
        }
    }
    /* corpora cum secretis: token (codex, tessera renovationis),
     * consensus (signum CSRF) */
    si (   chorda_aequalis_literis(r->via, "/oauth/token")
        || (   chorda_aequalis_literis(r->via, "/oauth/authorize")
            && r->methodus == HTTP_POST))
    {
        chorda_aedificator_appendere_literis(a,
            " | corpus: [redactum]");
    }
    alioquin si (r->corpus_via != NIHIL)
    {
        chorda_aedificator_appendere_literis(a, " | corpus: [in filo]");
    }
    alioquin si (   r->corpus.mensura > ZEPHYRUM
                 && chorda_aequalis_literis(r->via, "/mcp"))
    {
        /* /mcp: COMPENDIUM solum - argumenta instrumentorum textum
         * notarum ferunt, numquam in actis */
        chorda_aedificator_appendere_literis(a, " | corpus: ");
        _compendium_mcp(a, r->corpus, colloquium_piscina(col));
    }
    alioquin si (r->corpus.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, " | corpus: ");
        _tutum_appendere(a, r->corpus, SONDA_CORPUS_MAXIMUM);
    }
    linea = chorda_aedificator_finire(a);
    si (e->cfg.sonda_scriptor)
    {
        e->cfg.sonda_scriptor(e->cfg.sonda_datum, linea);
    }
    alioquin
    {
        fprintf(stderr, "%.*s\n", (integer)linea.mensura,
            (constans character*)linea.datum);
    }
}


/* speculator bibliothecarum: sonda */
interior vacuum
_speculator (
                 vacuum* datum,
    HospitiumColloquium* col)
{
    _sonda((Exemplar*)datum, col);
}


/* ==================================================
 * INSTRUMENTA NOTARUM
 * ================================================== */

#define NOTAE_PER_PAGINAM XX

interior Exemplar*
_e (
    McpVocatio* v)
{
    redde (Exemplar*)mcp_vocatio_datum(v);
}

/* nota viva personae; NIHIL si ignota aut aliena (idem responsum:
 * nihil de notis alienis proditur) */
interior Nota*
_nota_personae (
    McpVocatio* v,
           s64  id)
{
     Exemplar* e = _e(v);
          i32  k;

    per (k = ZEPHYRUM; k < xar_numerus(e->notae); k++)
    {
        Nota* n = (Nota*)xar_obtinere(e->notae, k);

        si (   n->id == id && !n->deleta
            && chorda_aequalis(n->civis, mcp_vocatio_persona(v)->usor))
        {
            redde n;
        }
    }
    redde NIHIL;
}

interior JsonValor*
_nota_json (
       Nota* n,
    Piscina* p)
{
    JsonValor* o = json_objectum_creare(p);

    json_objectum_ponere(o, "id", json_integer_creare(p, n->id));
    json_objectum_ponere(o, "textus", json_chorda_creare(p, n->textus));
    si (n->sigillum.mensura > ZEPHYRUM)
    {
        JsonValor* f = json_objectum_creare(p);

        json_objectum_ponere(f, "titulus", json_chorda_creare(p,
            n->titulus));
        json_objectum_ponere(f, "mimen", json_chorda_creare(p,
            n->mimen));
        json_objectum_ponere(f, "mensura", json_integer_creare(p,
            (s64)n->mensura));
        json_objectum_ponere(o, "filum", f);
    }
    redde o;
}

interior vacuum
_notas_enumerare (
    McpVocatio* v)
{
       Exemplar* e       = _e(v);
        Piscina* p       = mcp_vocatio_piscina(v);
            s64  pagina  = I;
      JsonValor* r       = json_objectum_creare(p);
      JsonValor* t       = json_tabulatum_creare(p);
            s32  k;
            s64  numerus = ZEPHYRUM;
            s64  initium;

    (vacuum)mcp_argumentum_integer(v, "pagina", &pagina);
    si (pagina < I)
    {
        mcp_respondere_vitium(v, chorda_ex_literis("pagina >= 1", p));
        redde;
    }
    initium = (pagina - I) * NOTAE_PER_PAGINAM;
    /* recentissimae primae */
    per (k = (s32)xar_numerus(e->notae) - I; k >= ZEPHYRUM; k--)
    {
        Nota* n = (Nota*)xar_obtinere(e->notae, (i32)k);

        si (   n->deleta
            || !chorda_aequalis(n->civis, mcp_vocatio_persona(v)->usor))
        {
            perge;
        }
        si (numerus >= initium && numerus < initium + NOTAE_PER_PAGINAM)
        {
            json_tabulatum_addere(t, _nota_json(n, p));
        }
        numerus++;
    }
    json_objectum_ponere(r, "pagina", json_integer_creare(p, pagina));
    json_objectum_ponere(r, "paginae", json_integer_creare(p,
        (numerus + NOTAE_PER_PAGINAM - I) / NOTAE_PER_PAGINAM));
    json_objectum_ponere(r, "notae", t);
    mcp_respondere_json(v, r);
}

interior vacuum
_notam_legere (
    McpVocatio* v)
{
     s64  id = ZEPHYRUM;
    Nota* n;

    (vacuum)mcp_argumentum_integer(v, "id", &id);
    n = _nota_personae(v, id);
    si (!n)
    {
        mcp_respondere_vitium(v, chorda_ex_literis("nota non inventa",
            mcp_vocatio_piscina(v)));
        redde;
    }
    mcp_respondere_json(v, _nota_json(n, mcp_vocatio_piscina(v)));
}

interior vacuum
_notam_addere (
    McpVocatio* v)
{
                Piscina* p = mcp_vocatio_piscina(v);
                 chorda  textus;
                    s64  id;
              JsonValor* r;
    constans McpPersona* q = mcp_vocatio_persona(v);

    (vacuum)mcp_argumentum_chorda(v, "textus", &textus);
    si (   textus.mensura == ZEPHYRUM
        || textus.mensura > EXEMPLAR_TEXTUS_MAXIMUS)
    {
        mcp_respondere_vitium(v, chorda_ex_literis(
            "textus: I-XVI KB requiritur", p));
        redde;
    }
    id = exemplar_notam_scribere(_e(v), q->usor, textus, q->cliens, p);
    si (id == ZEPHYRUM)
    {
        mcp_respondere_vitium(v, chorda_ex_literis(
            "volumen scribi nequit", p));
        redde;
    }
    r = json_objectum_creare(p);
    json_objectum_ponere(r, "id", json_integer_creare(p, id));
    mcp_respondere_json(v, r);
}

interior vacuum
_notam_delere (
    McpVocatio* v)
{
       Piscina* p   = mcp_vocatio_piscina(v);
           s64  id  = ZEPHYRUM;
     JsonValor* r;

    (vacuum)mcp_argumentum_integer(v, "id", &id);
    si (   !_nota_personae(v, id)
        || !exemplar_notam_delere(_e(v), mcp_vocatio_persona(v)->usor,
        id,
               p))
    {
        mcp_respondere_vitium(v, chorda_ex_literis("nota non inventa",
            p));
        redde;
    }
    r = json_objectum_creare(p);
    json_objectum_ponere(r, "id", json_integer_creare(p, id));
    json_objectum_ponere(r, "deleta", json_boolean_creare(p, VERUM));
    mcp_respondere_json(v, r);
}

hic_manens constans McpArgumentum ARG_PAGINAE[] = {
    { "pagina", "pagina (1 = recentissimae XX)", MCP_INTEGER, FALSUM,
        NIHIL }
};

hic_manens constans McpArgumentum ARG_ID[] = {
    { "id", "numerus notae (ex notas_enumerare)", MCP_INTEGER, VERUM,
        NIHIL }
};

hic_manens constans McpArgumentum ARG_TEXTUS[] = {
    { "textus", "textus notae (usque ad XVI KB)", MCP_CHORDA, VERUM,
        NIHIL }
};

hic_manens constans McpInstrumentum INSTRUMENTA[] = {
    { "notas_enumerare",
        "Notas personae enumerat, recentissimas primas,"
        " XX per paginam.", ARG_PAGINAE, I, "notae:legere", VERUM,
            FALSUM,
        _notas_enumerare },
    { "notam_legere", "Notam unam legit: textus et nomina mensuraeque "
        "filorum adiunctorum.", ARG_ID, I, "notae:legere", VERUM,
            FALSUM,
        _notam_legere },
    { "notam_addere", "Notam novam personae addit.", ARG_TEXTUS, I,
        "notae:scribere", FALSUM, FALSUM, _notam_addere },
    { "notam_delere", "Notam personae delet (non revocabile).", ARG_ID,
        I,
        "notae:scribere", FALSUM, VERUM, _notam_delere }
};


/* ==================================================
 * MONTATIO
 * ================================================== */

/* clientes non usi et tesserae expiratae: semel in hora */
interior vacuum
_putare (
    vacuum* datum)
{
    Exemplar* e = (Exemplar*)datum;

    (vacuum)admissio_putare(e->admissio);
}

b32
exemplar_mcp_praebere (
    Exemplar* e)
{
     McpConfiguratio mc;
    AdmissioOptiones optiones_admissionis;
     McpHttpOptiones optiones_vecturae;
           character audientia[EXEMPLAR_VIA_MAXIMA + VIII];

    sprintf(audientia, "%s/mcp", e->basis);
    memset(&mc, ZEPHYRUM, magnitudo(mc));
    mc.titulus  = "exemplar";
    mc.versio   = "1";
    mc.instructiones  =
        "Notae personae quae Claudium adhibet (Franisci aut "
        "Amandae): enumerare, legere, addere, delere. Quaeque "
        "persona suas solum videt.";
    mc.instrumenta  = INSTRUMENTA;
    mc.numerus      = IV;
    mc.datum        = e;
    e->mcp          = mcp_servus_creare(&mc, e->piscina);
    memset(&optiones_admissionis, ZEPHYRUM,
        magnitudo(optiones_admissionis));
    optiones_admissionis.auctoritas  = e->basis;
    optiones_admissionis.audientia   = audientia;
    optiones_admissionis.scopi       = SCOPI;
    e->admissio = admissio_creare(e->volumen, &optiones_admissionis,
        e->piscina);
    si (!e->mcp || !e->admissio)
    {
        redde FALSUM;
    }
    admissio_speculatorem_ponere(e->admissio, _speculator, e);
    memset(&optiones_vecturae, ZEPHYRUM, magnitudo(optiones_vecturae));
    optiones_vecturae.via         = "/mcp";
    optiones_vecturae.audientia   = audientia;
    optiones_vecturae.auctoritas  = e->basis;
    optiones_vecturae.scopi =
        "notae:legere notae:scribere";
    optiones_vecturae.verificator          = admissio_verificator;
    optiones_vecturae.verificatoris_datum  = e->admissio;
    optiones_vecturae.speculator           = _speculator;
    optiones_vecturae.speculatoris_datum   = e;
    (vacuum)admissio_putare(e->admissio);
    (vacuum)reactor_timer_adicere(e->reactor, LX * LX * M, VERUM,
        _putare,
        e);
    redde admissio_montare(e->admissio, e->hospitium,
               ianua_admissionis_intratus, e->ianua, "/intrare")
        && mcp_servus_http_montare(e->hospitium, e->mcp,
        &optiones_vecturae,
        e->piscina);
}
