/* exemplar_mcp.c - viae MCP et OAuth exemplaris (vide exemplar_mcp.h)
 *
 * T1 (sonda): documenta inventionis ex json aedificata (nullae
 * chordae JSON manu scriptae), POST /mcp semper 401 (nulla tessera
 * adhuc emittitur), /oauth/register 503. Sonda petitionem unam lineam
 * unam scribit: methodus, uri, OMNIA capita (Authorization et
 * Proxy-Authorization schema solum, Cookie et Set-Cookie redacta),
 * corpus usque ad IV KB effugiis tutum - corpus /oauth/token numquam.
 */
#include "postulata_posix.h"
#include "exemplar_mcp.h"
#include "json.h"
#include "chorda_aedificator.h"
#include "http.h"

#include <stdio.h>
#include <string.h>

#define SONDA_CORPUS_MAXIMUM (IV * MXXIV)

hic_manens constans character* SCOPI[] = {
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
    si (chorda_aequalis_literis(r->via, "/oauth/token"))
    {
        chorda_aedificator_appendere_literis(a,
            " | corpus: [redactum]");
    }
    alioquin si (r->corpus_via != NIHIL)
    {
        chorda_aedificator_appendere_literis(a, " | corpus: [in filo]");
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


/* ==================================================
 * DOCUMENTA INVENTIONIS
 * ================================================== */

interior JsonValor*
_tabulatum (
    constans character* constans* valores,
                       Piscina* p)
{
    JsonValor* t = json_tabulatum_creare(p);
          s32  i;

    per (i = ZEPHYRUM; valores[i] != NIHIL; i++)
    {
        json_tabulatum_addere(t, json_chorda_creare_literis(p,
            valores[i]));
    }
    redde t;
}

interior JsonValor*
_basis_cum (
               Exemplar* e,
     constans character* via,
                Piscina* p)
{
    character s[EXEMPLAR_VIA_MAXIMA + LXIV];

    sprintf(s, "%s%s", e->basis, via);
    redde json_chorda_creare_literis(p, s);
}

interior vacuum
_json_respondere (
    HospitiumColloquium* col,
                    i32  status,
              JsonValor* o)
{
    colloquium_caput_addere(col, "Cache-Control", "no-store");
    colloquium_respondere(col, status, "application/json",
        json_scribere(o, colloquium_piscina(col)));
}

/* RFC 9728: documentum resource protecti */
interior vacuum
_documentum_protectum (
    HospitiumColloquium* col)
{
              Exemplar* e = (Exemplar*)colloquium_datum(col);
               Piscina* p = colloquium_piscina(col);
             JsonValor* o = json_objectum_creare(p);
    constans character* auctoritates[II];
    constans character* modi[II];

    _sonda(e, col);
    auctoritates[ZEPHYRUM]  = e->basis;
    auctoritates[I]         = NIHIL;
    modi[ZEPHYRUM]          = "header";
    modi[I]                 = NIHIL;
    json_objectum_ponere(o, "resource", _basis_cum(e, "/mcp", p));
    json_objectum_ponere(o, "authorization_servers",
        _tabulatum(auctoritates, p));
    json_objectum_ponere(o, "scopes_supported", _tabulatum(SCOPI, p));
    json_objectum_ponere(o, "bearer_methods_supported",
        _tabulatum(modi, p));
    _json_respondere(col, CC, o);
}

/* RFC 8414: documentum auctoritatis (T5 a bibliotheca admissio) */
interior vacuum
_auctoritas (
    HospitiumColloquium* col)
{
              Exemplar* e = (Exemplar*)colloquium_datum(col);
               Piscina* p = colloquium_piscina(col);
             JsonValor* o = json_objectum_creare(p);
    constans character* s256[II];
    constans character* nullus[II];
    constans character* codex[II];
    constans character* genera[III];
    constans character* scopi[IV];

    _sonda(e, col);
    s256[ZEPHYRUM]    = "S256";
    s256[I]           = NIHIL;
    nullus[ZEPHYRUM]  = "none";
    nullus[I]         = NIHIL;
    codex[ZEPHYRUM]   = "code";
    codex[I]          = NIHIL;
    genera[ZEPHYRUM]  = "authorization_code";
    genera[I]         = "refresh_token";
    genera[II]        = NIHIL;
    scopi[ZEPHYRUM]   = SCOPI[ZEPHYRUM];
    scopi[I]          = SCOPI[I];
    scopi[II]         = "offline_access";
    scopi[III]        = NIHIL;
    json_objectum_ponere(o, "issuer", json_chorda_creare_literis(p,
        e->basis));
    json_objectum_ponere(o, "authorization_endpoint",
        _basis_cum(e, "/oauth/authorize", p));
    json_objectum_ponere(o, "token_endpoint",
        _basis_cum(e, "/oauth/token", p));
    json_objectum_ponere(o, "registration_endpoint",
        _basis_cum(e, "/oauth/register", p));
    json_objectum_ponere(o, "response_types_supported",
        _tabulatum(codex,
        p));
    json_objectum_ponere(o, "grant_types_supported", _tabulatum(genera,
        p));
    json_objectum_ponere(o, "code_challenge_methods_supported",
        _tabulatum(s256, p));
    json_objectum_ponere(o, "token_endpoint_auth_methods_supported",
        _tabulatum(nullus, p));
    json_objectum_ponere(o, "scopes_supported", _tabulatum(scopi, p));
    json_objectum_ponere(o,
        "authorization_response_iss_parameter_supported",
        json_boolean_creare(p, VERUM));
    _json_respondere(col, CC, o);
}


/* ==================================================
 * MCP ET OAUTH (sonda)
 * ================================================== */

/* POST /mcp: tessera nulla adhuc emittitur - semper 401 cum indice
 * documenti (Claudius signum SOLUM in 401 incipit) */
interior vacuum
_mcp_post (
    HospitiumColloquium* col)
{
     Exemplar* e = (Exemplar*)colloquium_datum(col);
      Piscina* p = colloquium_piscina(col);
    JsonValor* o = json_objectum_creare(p);
    character  caput[EXEMPLAR_VIA_MAXIMA + CXXVIII];

    _sonda(e, col);
    sprintf(caput, "Bearer resource_metadata=\"%s/.well-known/"
        "oauth-protected-resource\", scope=\"%s %s\"", e->basis,
        SCOPI[ZEPHYRUM], SCOPI[I]);
    colloquium_caput_addere(col, "WWW-Authenticate", caput);
    json_objectum_ponere(o, "error", json_chorda_creare_literis(p,
        "invalid_token"));
    _json_respondere(col, CDI, o);
}

/* POST /oauth/register: sonda Claudium hic sistit (T5 eam implet) */
interior vacuum
_inscriptio_interim (
    HospitiumColloquium* col)
{
     Exemplar* e = (Exemplar*)colloquium_datum(col);
      Piscina* p = colloquium_piscina(col);
    JsonValor* o = json_objectum_creare(p);

    _sonda(e, col);
    json_objectum_ponere(o, "error", json_chorda_creare_literis(p,
        "temporarily_unavailable"));
    json_objectum_ponere(o, "error_description",
        json_chorda_creare_literis(p, "sonda: inscriptio nondum"));
    _json_respondere(col, DIII, o);
}

vacuum
exemplar_mcp_praebere (
    Exemplar* e)
{
    (vacuum)hospitium_praebere(e->hospitium, HTTP_POST, "/mcp",
        _mcp_post, e);
    (vacuum)hospitium_praebere(e->hospitium, HTTP_GET,
        "/.well-known/oauth-protected-resource", _documentum_protectum,
        e);
    (vacuum)hospitium_praebere(e->hospitium, HTTP_GET,
        "/.well-known/oauth-protected-resource/mcp",
        _documentum_protectum,
        e);
    (vacuum)hospitium_praebere(e->hospitium, HTTP_GET,
        "/.well-known/oauth-authorization-server", _auctoritas, e);
    (vacuum)hospitium_praebere(e->hospitium, HTTP_POST,
        "/oauth/register",
        _inscriptio_interim, e);
}
