/* probatio_admissio.c - admissio I (mcp-remotus T5): servus OAuth 2.1
 * - metadata RFC 8414 (campus per campum), inscriptio dynamica RFC
 * 7591 (https exacta, loopback quovis portu, refutationes nominatae),
 * turba inscriptionum (frenum per IP, putatio post diem), tesserae
 * opacae (plagula voluminis et WAL nullam tesseram in claro
 * continent), verificator (audientia, expiratio horologio iniecto,
 * revocatio), concessiones et revocare per usorem.
 */

#include "postulata_posix.h"
#define ADMISSIO_PROBATIO
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "filum.h"
#include "json.h"
#include "volumen.h"
#include "xar.h"
#include "mcp_servus.h"
#include "admissio.h"
#include <stdio.h>
#include <string.h>

#define AUCTORITAS  "https://exemplar.example"
#define AUDIENTIA   "https://exemplar.example/mcp"
#define CLAUDE \
    "{\"redirect_uris\":" \
    "[\"https://claude.ai/api/mcp/auth_callback\"]," \
    "\"client_name\":\"Claude\",\"token_endpoint_auth_method\":" \
    "\"none\",\"grant_types\":[\"authorization_code\"," \
    "\"refresh_token\"]}"
#define DIES_MS ((s64)XXIV * LX * LX * M)

hic_manens s64 g_nunc = (s64)1760000000 * M;

interior s64
_nunc (vacuum)
{
    redde g_nunc;
}

/* horologium ad initium omni sectione */
interior vacuum
_horologium_parare (
    CredoContextus* c)
{
    (vacuum)c;
    g_nunc = (s64)1760000000 * M;
}

hic_manens constans character* constans SCOPI[] = {
    "notae:legere", "notae:scribere", NIHIL
};

interior chorda
_c (
    constans character* s,
               Piscina* p)
{
    redde chorda_ex_literis(s, p);
}

interior Admissio*
_admissio (
    CredoContextus*  c,
           Volumen** v)
{
    AdmissioOptiones o;

    memset(&o, ZEPHYRUM, magnitudo(o));
    o.auctoritas  = AUCTORITAS;
    o.audientia   = AUDIENTIA;
    o.scopi       = SCOPI;
    o.nunc_ms     = _nunc;
    *v            = volumen_temporarium(c->piscina, "admissio");
    si (!*v)
    {
        redde NIHIL;
    }
    redde admissio_creare(*v, &o, c->piscina);
}

/* status inscriptionis; client_id in *id si 201 */
interior s32
_inscribere (
          CredoContextus* c,
                Admissio* a,
      constans character* corpus,
      constans character* ip,
                  chorda* responsum,
                  chorda* id)
{
          s32 status = admissio_inscribere(a, _c(corpus, c->piscina),
              _c(ip, c->piscina), responsum, c->piscina);
    JsonResultus r = json_legere(*responsum, c->piscina);

    si (id)
    {
        id->datum    = NIHIL;
        id->mensura  = ZEPHYRUM;
        si (r.successus)
        {
            *id = json_ad_chorda(json_objectum_capere(r.radix,
                "client_id"));
        }
    }
    redde status;
}

interior b32
_error_est (
          CredoContextus* c,
                  chorda  responsum,
      constans character* codex)
{
    JsonResultus r = json_legere(responsum, c->piscina);

    redde r.successus && chorda_aequalis_literis(json_ad_chorda(
        json_objectum_capere(r.radix, "error")), codex);
}

interior b32
_tabula_chordarum (
          JsonValor* t,
    constans character* constans* exspectatae)
{
    i32 k;

    per (k = ZEPHYRUM; exspectatae[k]; k++)
    {
        si (!chorda_aequalis_literis(json_ad_chorda(
                json_tabulatum_obtinere(t, k)), exspectatae[k]))
        {
            redde FALSUM;
        }
    }
    redde json_tabulatum_numerus(t) == k;
}


/* ==================================================
 * sectiones
 * ================================================== */

interior vacuum
_sectio_documentum (CredoContextus* c)
{
         Volumen* v;
        Admissio* a = _admissio(c, &v);
    JsonResultus  r;
       JsonValor* d;
    hic_manens constans character* constans RESPONSA[] = {
        "code", NIHIL };
    hic_manens constans character* constans GENERA[] = {
        "authorization_code", "refresh_token", NIHIL };
    hic_manens constans character* constans S256[] = { "S256", NIHIL };
    hic_manens constans character* constans NONE[] = { "none", NIHIL };
    hic_manens constans character* constans SCOPI_OMNES[] = {
        "notae:legere", "notae:scribere", "offline_access", NIHIL };

    CREDO_NECESSE_NON_NIHIL(a);
    r = json_legere(admissio_documentum(a, c->piscina), c->piscina);
    CREDO_NECESSE(r.successus);
    d = r.radix;
    CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(json_objectum_capere(d,
        "issuer")), AUCTORITAS);
    CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(json_objectum_capere(d,
        "authorization_endpoint")), AUCTORITAS "/oauth/authorize");
    CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(json_objectum_capere(d,
        "token_endpoint")), AUCTORITAS "/oauth/token");
    CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(json_objectum_capere(d,
        "registration_endpoint")), AUCTORITAS "/oauth/register");
    CREDO_VERUM(_tabula_chordarum(json_objectum_capere(d,
        "response_types_supported"), RESPONSA));
    CREDO_VERUM(_tabula_chordarum(json_objectum_capere(d,
        "grant_types_supported"), GENERA));
    CREDO_VERUM(_tabula_chordarum(json_objectum_capere(d,
        "code_challenge_methods_supported"), S256));
    CREDO_VERUM(_tabula_chordarum(json_objectum_capere(d,
        "token_endpoint_auth_methods_supported"), NONE));
    CREDO_VERUM(_tabula_chordarum(json_objectum_capere(d,
        "scopes_supported"), SCOPI_OMNES));
    CREDO_VERUM(json_ad_boolean(json_objectum_capere(d,
        "authorization_response_iss_parameter_supported")));
    /* nullum signum CIMD in B */
    CREDO_FALSUM(json_objectum_habet(d,
        "client_id_metadata_document_supported"));
    volumen_claudere(v);
}

interior vacuum
_sectio_inscriptio (CredoContextus* c)
{
         Volumen* v;
        Admissio* a = _admissio(c, &v);
          chorda  responsum;
          chorda  id;
          chorda  id2;
    JsonResultus  r;

    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_AEQUALIS_S32(_inscribere(c, a, CLAUDE, "160.79.106.1",
        &responsum, &id), CCI);
    CREDO_MAIOR_I32(id.mensura, XV);
    r = json_legere(responsum, c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(json_objectum_capere(
        r.radix, "client_name")), "Claude");
    CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(json_objectum_capere(
        r.radix, "token_endpoint_auth_method")), "none");
    CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(
        json_tabulatum_obtinere(json_objectum_capere(r.radix,
            "redirect_uris"), ZEPHYRUM)),
        "https://claude.ai/api/mcp/auth_callback");
    /* identitates diversae */
    CREDO_AEQUALIS_S32(_inscribere(c, a, CLAUDE, "160.79.106.1",
        &responsum, &id2), CCI);
    CREDO_FALSUM(chorda_aequalis(id, id2));
    /* corpus Claudis ut re vera missum (sonda T1) */
    CREDO_AEQUALIS_S32(_inscribere(c, a,
        "{\"redirect_uris\":"
        "[\"https://claude.ai/api/mcp/auth_callback\"],"
        "\"token_endpoint_auth_method\":\"none\",\"grant_types\":"
        "[\"authorization_code\",\"refresh_token\"],\"response_types\":"
        "[\"code\"],\"scope\":\"notae:legere notae:scribere "
        "offline_access\",\"client_name\":\"Claude\","
        "\"application_type\":\"web\"}", "160.79.106.2", &responsum,
        NIHIL), CCI);
    /* loopback quovis portu */
    CREDO_AEQUALIS_S32(_inscribere(c, a,
        "{\"redirect_uris\":[\"http://127.0.0.1/callback\"]}",
        "127.0.0.1",
        &responsum, NIHIL), CCI);
    CREDO_AEQUALIS_S32(_inscribere(c, a,
        "{\"redirect_uris\":[\"http://localhost:3118/callback\"]}",
        "127.0.0.1", &responsum, NIHIL), CCI);
    /* redirectiones malae */
    CREDO_AEQUALIS_S32(_inscribere(c, a,
        "{\"redirect_uris\":"
        "[\"http://claude.ai/api/mcp/auth_callback\"]}",
        "10.0.0.1", &responsum, NIHIL), CD);
    CREDO_VERUM(_error_est(c, responsum, "invalid_redirect_uri"));
    CREDO_AEQUALIS_S32(_inscribere(c, a,
        "{\"redirect_uris\":[\"https://claude.ai/cb#frag\"]}",
        "10.0.0.2",
        &responsum, NIHIL), CD);
    CREDO_VERUM(_error_est(c, responsum, "invalid_redirect_uri"));
    CREDO_AEQUALIS_S32(_inscribere(c, a, "{\"client_name\":\"x\"}",
        "10.0.0.3", &responsum, NIHIL), CD);
    CREDO_VERUM(_error_est(c, responsum, "invalid_redirect_uri"));
    CREDO_AEQUALIS_S32(_inscribere(c, a,
        "{\"redirect_uris\":[\"http://localhost.evil.example/cb\"]}",
        "10.0.0.4", &responsum, NIHIL), CD);
    CREDO_VERUM(_error_est(c, responsum, "invalid_redirect_uri"));
    /* methodus authenticationis aliena */
    CREDO_AEQUALIS_S32(_inscribere(c, a,
        "{\"redirect_uris\":[\"https://claude.ai/cb\"],"
        "\"token_endpoint_auth_method\":\"client_secret_basic\"}",
        "10.0.0.5", &responsum, NIHIL), CD);
    CREDO_VERUM(_error_est(c, responsum, "invalid_client_metadata"));
    CREDO_AEQUALIS_S32(_inscribere(c, a,
        "{\"redirect_uris\":[\"https://claude.ai/cb\"],"
        "\"grant_types\":[\"client_credentials\"]}", "10.0.0.6",
        &responsum, NIHIL), CD);
    CREDO_VERUM(_error_est(c, responsum, "invalid_client_metadata"));
    CREDO_AEQUALIS_S32(_inscribere(c, a, "non json", "10.0.0.7",
        &responsum, NIHIL), CD);
    CREDO_VERUM(_error_est(c, responsum, "invalid_client_metadata"));
    volumen_claudere(v);
}

/* Review focus 1: Claude iterum atque iterum se inscribit */
interior vacuum
_sectio_turba_inscriptionum (CredoContextus* c)
{
      Volumen* v;
     Admissio* a = _admissio(c, &v);
       chorda  responsum;
       chorda  ids[L];
       chorda  novum;
       chorda  accessus;
       chorda  renovatio;
          i32  bene     = ZEPHYRUM;
          i32  frenata  = ZEPHYRUM;
          i32  k;

    CREDO_NECESSE_NON_NIHIL(a);
    per (k = ZEPHYRUM; k < L; k++)
    {
        s32 s;

        g_nunc += C;   /* L inscriptiones intra V secunda */
        s = _inscribere(c, a, CLAUDE, "160.79.106.9", &responsum,
            &ids[k]);
        si (s == CCI)
        {
            bene++;
        }
        alioquin si (s == CDXXIX)
        {
            frenata++;
        }
    }
    CREDO_AEQUALIS_I32(bene, ADMISSIO_INSCRIPTIONES_PER_MINUTUM);
    CREDO_AEQUALIS_I32(frenata, L - ADMISSIO_INSCRIPTIONES_PER_MINUTUM);
    /* IP alia non frenatur */
    CREDO_AEQUALIS_S32(_inscribere(c, a, CLAUDE, "160.79.106.10",
        &responsum, NIHIL), CCI);
    /* cliens primus concessionem habet */
    CREDO_VERUM(admissio_concessionem_creare_probationis(a, _c("fran",
        c->piscina), ids[ZEPHYRUM], _c("notae:legere", c->piscina),
        &accessus, &renovatio, c->piscina));
    /* post diem: putatio; cliens novus manet */
    g_nunc += DIES_MS + I;
    CREDO_AEQUALIS_S32(_inscribere(c, a, CLAUDE, "160.79.106.9",
        &responsum, &novum), CCI);
    CREDO_AEQUALIS_S32(admissio_putare(a),
        ADMISSIO_INSCRIPTIONES_PER_MINUTUM);
    CREDO_VERUM(admissio_concessionem_creare_probationis(a, _c("fran",
        c->piscina), ids[ZEPHYRUM], _c("notae:legere", c->piscina),
        &accessus, &renovatio, c->piscina));
    CREDO_VERUM(admissio_concessionem_creare_probationis(a, _c("fran",
        c->piscina), novum, _c("notae:legere", c->piscina), &accessus,
        &renovatio, c->piscina));
    CREDO_FALSUM(admissio_concessionem_creare_probationis(a, _c("fran",
        c->piscina), ids[I], _c("notae:legere", c->piscina), &accessus,
        &renovatio, c->piscina));
    CREDO_AEQUALIS_S32(admissio_putare(a), ZEPHYRUM);
    volumen_claudere(v);
}

/* plagula + WAL: secretum in claro nusquam */
interior b32
_in_plagulis (
          CredoContextus* c,
                 Volumen* v,
                  chorda  secretum)
{
    character via[DXII];
       chorda f;
          i32 k;
          i32 i;

    per (k = ZEPHYRUM; k < II; k++)
    {
        sprintf(via, "%s%s", volumen_via(v), k ? "-wal" : "");
        f = filum_legere_totum(via, c->piscina);
        per (i = ZEPHYRUM; i + secretum.mensura <= f.mensura; i++)
        {
            si (memcmp(f.datum + i, secretum.datum,
                    (size_t)secretum.mensura) == ZEPHYRUM)
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

interior vacuum
_sectio_tesserae (CredoContextus* c)
{
              Volumen* v;
             Admissio* a = _admissio(c, &v);
               chorda  responsum;
               chorda  id;
               chorda  accessus;
               chorda  renovatio;
           McpPersona  q;
                  Xar* lista;
    AdmissioConcessio* g;

    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_AEQUALIS_S32(_inscribere(c, a, CLAUDE, "160.79.106.1",
        &responsum, &id), CCI);
    CREDO_VERUM(admissio_concessionem_creare_probationis(a, _c("fran",
        c->piscina), id, _c("notae:legere notae:scribere", c->piscina),
        &accessus, &renovatio, c->piscina));
    CREDO_AEQUALIS_I32(accessus.mensura, XLIII);
    CREDO_AEQUALIS_I32(renovatio.mensura, XLIII);
    CREDO_FALSUM(chorda_aequalis(accessus, renovatio));
    /* digesta sola in plagulis */
    CREDO_FALSUM(_in_plagulis(c, v, accessus));
    CREDO_FALSUM(_in_plagulis(c, v, renovatio));
    /* verificator */
    memset(&q, ZEPHYRUM, magnitudo(q));
    CREDO_VERUM(admissio_verificator(a, accessus, _c(AUDIENTIA,
        c->piscina), &q, c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(q.usor, "fran");
    CREDO_CHORDA_AEQUALIS(q.cliens, id);
    CREDO_CHORDA_AEQUALIS_LITERIS(q.scopi,
        "notae:legere notae:scribere");
    CREDO_FALSUM(admissio_verificator(a, accessus,
        _c("https://alius.example/mcp", c->piscina), &q, c->piscina));
    /* tessera renovationis non est tessera accessus */
    CREDO_FALSUM(admissio_verificator(a, renovatio, _c(AUDIENTIA,
        c->piscina), &q, c->piscina));
    CREDO_FALSUM(admissio_verificator(a, _c("ignota", c->piscina),
        _c(AUDIENTIA, c->piscina), &q, c->piscina));
    /* concessiones */
    lista = admissio_concessiones(a, _c("fran", c->piscina),
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(lista);
    CREDO_AEQUALIS_I32(xar_numerus(lista), I);
    g = (AdmissioConcessio*)xar_obtinere(lista, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(g->titulus_clientis, "Claude");
    CREDO_CHORDA_AEQUALIS_LITERIS(g->scopi,
        "notae:legere notae:scribere");
    CREDO_CHORDA_AEQUALIS_LITERIS(g->usor, "fran");
    CREDO_FALSUM(g->revocata);
    CREDO_AEQUALIS_I32(xar_numerus(admissio_concessiones(a, _c("amanda",
        c->piscina), c->piscina)), ZEPHYRUM);
    /* revocare: alius usor non potest; dominus potest */
    CREDO_FALSUM(admissio_revocare(a, _c("amanda", c->piscina), g->id));
    CREDO_VERUM(admissio_verificator(a, accessus, _c(AUDIENTIA,
        c->piscina), &q, c->piscina));
    CREDO_VERUM(admissio_revocare(a, _c("fran", c->piscina), g->id));
    CREDO_FALSUM(admissio_verificator(a, accessus, _c(AUDIENTIA,
        c->piscina), &q, c->piscina));
    CREDO_FALSUM(admissio_revocare(a, _c("fran", c->piscina), g->id));
    /* expiratio (horologium iniectum) */
    CREDO_VERUM(admissio_concessionem_creare_probationis(a, _c("fran",
        c->piscina), id, _c("notae:legere", c->piscina), &accessus,
        &renovatio, c->piscina));
    g_nunc += (s64)LX * LX * M - I;
    CREDO_VERUM(admissio_verificator(a, accessus, _c(AUDIENTIA,
        c->piscina), &q, c->piscina));
    g_nunc += I;
    CREDO_FALSUM(admissio_verificator(a, accessus, _c(AUDIENTIA,
        c->piscina), &q, c->piscina));
    volumen_claudere(v);
}

/* revocatio intra transactionem VOCATORIS (canalis T4, suspensio):
 * eam iungit - revoluta nihil revocat, commissa revocat, actum unum */
interior vacuum
_sectio_revocare_in_transactione (CredoContextus* c)
{
              Volumen* v;
             Admissio* a = _admissio(c, &v);
               chorda  responsum;
               chorda  id;
               chorda  accessus;
               chorda  renovatio;
           McpPersona  q;
    AdmissioConcessio* g;
                  Xar* acta;
                  i32  revocata = ZEPHYRUM;
                  i32  k;

    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_AEQUALIS_S32(_inscribere(c, a, CLAUDE, "160.79.106.1",
        &responsum, &id), CCI);
    CREDO_NECESSE(admissio_concessionem_creare_probationis(a, _c("fran",
        c->piscina), id, _c("notae:legere", c->piscina), &accessus,
        &renovatio, c->piscina));
    g = (AdmissioConcessio*)xar_obtinere(admissio_concessiones(a,
        _c("fran", c->piscina), c->piscina), ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(g);
    /* revoluta: concessio viva manet */
    CREDO_NECESSE(volumen_transactionem_incipere(v));
    CREDO_VERUM(admissio_revocare(a, _c("fran", c->piscina), g->id));
    CREDO_NECESSE(volumen_transactionem_revolvere(v));
    memset(&q, ZEPHYRUM, magnitudo(q));
    CREDO_VERUM(admissio_verificator(a, accessus, _c(AUDIENTIA,
        c->piscina), &q, c->piscina));
    /* commissa: revocata */
    CREDO_NECESSE(volumen_transactionem_incipere(v));
    CREDO_VERUM(admissio_revocare(a, _c("fran", c->piscina), g->id));
    CREDO_NECESSE(volumen_transactionem_committere(v));
    CREDO_FALSUM(admissio_verificator(a, accessus, _c(AUDIENTIA,
        c->piscina), &q, c->piscina));
    acta = volumen_acta_legere(v, ZEPHYRUM, c->piscina);
    per (k = ZEPHYRUM; acta && k < xar_numerus(acta); k++)
    {
        si (chorda_aequalis_literis(((VolumenActum*)xar_obtinere(acta,
            k))
                ->genus, "concessio.revocata"))
        {
            revocata++;
        }
    }
    CREDO_AEQUALIS_I32(revocata, I);
    volumen_claudere(v);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "documentum", _sectio_documentum, _horologium_parare, NIHIL,
        NIHIL,
        NIHIL },
    { "inscriptio", _sectio_inscriptio, _horologium_parare, NIHIL,
        NIHIL, NIHIL },
    { "turba_inscriptionum", _sectio_turba_inscriptionum,
        _horologium_parare, NIHIL,
        NIHIL, NIHIL },
    { "tesserae", _sectio_tesserae, _horologium_parare, NIHIL, NIHIL,
        NIHIL },
    { "revocare_in_transactione", _sectio_revocare_in_transactione,
        _horologium_parare, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("admissio", SECTIONES);
}
