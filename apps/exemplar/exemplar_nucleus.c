/* exemplar_nucleus.c - applicatio probans (vide exemplar_nucleus.h)
 *
 * Status in memoria (tabulae usorum et sessionum, ordo notarum) ex
 * actis voluminis replicatur ad initium; omnis mutatio prius actum
 * scribit, deinde memoriam mutat. Opera dilata (inscriptio, intratio)
 * in tabula fixa: tessera ibi copiata dum hash currit, deinde
 * deleta. */
#include "postulata_posix.h"
#include "exemplar_nucleus.h"
#include "exemplar_internum.h"
#include "exemplar_mcp.h"
#include "custodia.h"
#include "crustula.h"
#include "volumen.h"
#include "scrinium.h"
#include "filum.h"
#include "json.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include "xar.h"
#include "sigillum.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/* ==================================================
 * AUXILIA
 * ================================================== */

interior s64
_nunc_ms (vacuum)
{
    redde (s64)time(NIHIL) * M;
}

interior chorda
_copia (
      chorda  s,
     Piscina* p)
{
    chorda c;

    c.mensura  = s.mensura;
    c.datum    = (i8*)piscina_allocare(p, (i64)s.mensura + I);
    si (s.mensura > 0)
    {
        memcpy(c.datum, s.datum, (size_t)s.mensura);
    }
    c.datum[s.mensura] = '\0';
    redde c;
}

/* aequalitas temporis constantis (invitatio) */
interior b32
_aequale_constans (
                chorda  a,
    constans character* b)
{
    i32 n = (i32)strlen(b);
    i32 i;
     i8 diff = 0;

    si (a.mensura != n || n == 0)
    {
        redde FALSUM;
    }
    per (i = 0; i < n; i++)
    {
        diff = (i8)(diff | (a.datum[i] ^ (i8)b[i]));
    }
    redde diff == 0;
}

/* nomen usoris: 1..XXXII signa [a-z0-9._-] */
interior b32
_nomen_validum (
    chorda n)
{
    i32 i;

    si (   n.datum == NIHIL || n.mensura < I || n.mensura
        > EXEMPLAR_NOMEN_MAXIMUM)
    {
        redde FALSUM;
    }
    per (i = 0; i < n.mensura; i++)
    {
        i8 c = n.datum[i];

        si (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')
              || c == '.' || c == '_' || c == '-'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* EVASIO HTML UNA: omnis textus usoris per hanc */
interior vacuum
_evadere (
    ChordaAedificator* a,
               chorda  s)
{
    i32 i;

    per (i = 0; i < s.mensura; i++)
    {
        character c = (character)s.datum[i];

        si (c == '&')
        {
            chorda_aedificator_appendere_literis(a, "&amp;");
        }
        alioquin si (c == '<')
        {
            chorda_aedificator_appendere_literis(a, "&lt;");
        }
        alioquin si (c == '>')
        {
            chorda_aedificator_appendere_literis(a, "&gt;");
        }
        alioquin si (c == '"')
        {
            chorda_aedificator_appendere_literis(a, "&quot;");
        }
        alioquin si (c == '\'')
        {
            chorda_aedificator_appendere_literis(a, "&#39;");
        }
        alioquin
        {
            chorda_aedificator_appendere_character(a, c);
        }
    }
}

interior chorda
_hex_sigilli (
    constans Sigillum* s,
              Piscina* p)
{
    character hex[SIGILLUM_HEX_MENSURA];

    sigillum_hex(s, hex);
    redde chorda_ex_literis(hex, p);
}


/* ==================================================
 * ACTA
 * ================================================== */

/* actum scribere (JSON ex objecto); seq aut 0 */
interior s64
_actum (
              Exemplar* e,
    constans character* genus,
             JsonValor* o,
               Piscina* p)
{
    redde volumen_actum_appendere(e->volumen, genus, json_scribere(o,
        p));
}

interior vacuum
_ponere (
             JsonValor* o,
    constans character* clavis,
                chorda  valor,
               Piscina* p)
{
    json_objectum_ponere(o, clavis, json_chorda_creare(p, valor));
}

interior vacuum
_usor_ponere (
     Exemplar* e,
       chorda  nomen_usoris,
       chorda  phc)
{
    vacuum* inventus = NIHIL;
      Usor* u;

    si (tabula_dispersa_invenire(e->usores, nomen_usoris, &inventus))
    {
        u = (Usor*)inventus;
    }
    alioquin
    {
        u = (Usor*)piscina_allocare(e->piscina,
            (i64)magnitudo(Usor));
        u->nomen_usoris = _copia(nomen_usoris, e->piscina);
        (vacuum)tabula_dispersa_inserere(e->usores, u->nomen_usoris, u);
    }
    u->phc = _copia(phc, e->piscina);
}

interior Usor*
_usor (
     Exemplar* e,
       chorda  nomen_usoris)
{
    vacuum* u = NIHIL;

    si (   nomen_usoris.mensura == 0
        || !tabula_dispersa_invenire(e->usores, nomen_usoris, &u))
    {
        redde NIHIL;
    }
    redde (Usor*)u;
}

interior vacuum
_sessionem_ponere (
     Exemplar* e,
       chorda  hex,
       chorda  usor,
          s64  expirat_ms)
{
    Sessio* s = (Sessio*)piscina_allocare(e->piscina,
                    (i64)magnitudo(Sessio));

    s->usor        = _copia(usor, e->piscina);
    s->expirat_ms  = expirat_ms;
    (vacuum)tabula_dispersa_inserere(e->sessiones, _copia(hex,
        e->piscina), s);
}

interior Nota*
_nota (
     Exemplar* e,
          s64  id)
{
    i32 i;

    per (i = 0; i < xar_numerus(e->notae); i++)
    {
        Nota* n = (Nota*)xar_obtinere(e->notae, i);

        si (n->id == id)
        {
            redde n;
        }
    }
    redde NIHIL;
}

/* status ex actis replicatus */
interior vacuum
_replicare (
    Exemplar* e)
{
    Piscina* p = piscina_generare_dynamicum("exemplar_replicatio",
                     LXIV * MXXIV);
         Xar* acta = volumen_acta_legere(e->volumen, 0, p);
         i32  i;
      chorda  vacua = chorda_ex_literis("", p);

    per (i = 0; acta && i < xar_numerus(acta); i++)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(acta, i);
        JsonResultus  r;

        si (   !chorda_incipit(a->genus, chorda_ex_literis("usor.", p))
            && !chorda_incipit(a->genus, chorda_ex_literis("tessera.",
            p))
            && !chorda_incipit(a->genus, chorda_ex_literis("sessio.",
            p))
            && !chorda_incipit(a->genus, chorda_ex_literis("nota.", p)))
        {
            perge;
        }
        r = json_legere(a->datum, p);
        si (!r.successus)
        {
            perge;
        }
        si (   chorda_aequalis_literis(a->genus, "usor.creatus")
            || chorda_aequalis_literis(a->genus, "tessera.mutata"))
        {
            _usor_ponere(e, json_capere_chorda(r.radix, "usor",
                vacua),
                json_capere_chorda(r.radix, "phc", vacua));
        }
        alioquin si (chorda_aequalis_literis(a->genus, "sessio.creata"))
        {
            _sessionem_ponere(e, json_capere_chorda(r.radix, "sigillum",
                vacua), json_capere_chorda(r.radix, "usor", vacua),
                json_capere_integer(r.radix, "expirat", 0));
        }
        alioquin si (chorda_aequalis_literis(a->genus, "sessio.deleta"))
        {
            tabula_dispersa_delere(e->sessiones, json_capere_chorda(
                r.radix, "sigillum", vacua));
        }
        alioquin si (chorda_aequalis_literis(a->genus, "nota.scripta"))
        {
            Nota* n = (Nota*)xar_addere(e->notae);

            memset(n, 0, magnitudo(*n));
            n->id       = a->seq;
            n->usor     = _copia(json_capere_chorda(r.radix, "usor",
                vacua), e->piscina);
            n->textus   = _copia(json_capere_chorda(r.radix, "textus",
                vacua), e->piscina);
            n->sigillum = _copia(json_capere_chorda(r.radix, "sigillum",
                vacua), e->piscina);
            n->titulus  = _copia(json_capere_chorda(r.radix, "titulus",
                vacua), e->piscina);
            n->mimen    = _copia(json_capere_chorda(r.radix, "mimen",
                vacua), e->piscina);
            n->mensura  = (i64)json_capere_integer(r.radix, "mensura",
                0);
        }
        alioquin si (chorda_aequalis_literis(a->genus, "nota.deleta"))
        {
            Nota* n = _nota(e, json_capere_integer(r.radix, "id", 0));

            si (n)
            {
                n->deleta = VERUM;
            }
        }
    }
    piscina_destruere(p);
}


/* ==================================================
 * PAGINAE ET RESPONSA
 * ================================================== */

hic_manens constans character* EXEMPLAR_STILUS =
    "body{font:16px/1.5 Georgia,serif;max-width:40em;margin:2em auto;"
    "padding:0 1em;color:#222;background:#fdfbf7}"
    "h1{font-weight:normal;border-bottom:1px solid #ccc}"
    "form{margin:1em 0}input,textarea{font:inherit;display:block;"
    "margin:.3em 0;width:100%;box-sizing:border-box}"
    "button{font:inherit;padding:.2em 1em}"
    ".nota{border-left:3px solid #8a6;padding:.2em .8em;margin:1em 0}"
    ".error{color:#a22}";

interior ChordaAedificator*
_paginam_incipere (
    HospitiumColloquium* col,
     constans character* titulus)
{
    ChordaAedificator* a = chorda_aedificator_creare(
        colloquium_piscina(col), MMXLVIII);

    chorda_aedificator_appendere_literis(a, "<!doctype html>\n"
        "<html lang=\"la\"><head><meta charset=\"utf-8\">"
        "<meta name=\"viewport\" content=\"width=device-width\">"
        "<title>");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "</title><style>");
    chorda_aedificator_appendere_literis(a, EXEMPLAR_STILUS);
    chorda_aedificator_appendere_literis(a,
        "</style></head><body><h1>");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "</h1>\n");
    redde a;
}

interior vacuum
_capita_tuta (
    HospitiumColloquium* col)
{
    colloquium_caput_addere(col, "X-Content-Type-Options", "nosniff");
    colloquium_caput_addere(col, "Content-Security-Policy",
        "default-src 'none'; style-src 'unsafe-inline'; "
        "form-action 'self'; frame-ancestors 'none'");
    colloquium_caput_addere(col, "Referrer-Policy", "same-origin");
}

interior vacuum
_paginam_mittere (
    HospitiumColloquium* col,
                    i32  status,
      ChordaAedificator* a)
{
    chorda_aedificator_appendere_literis(a, "</body></html>\n");
    _capita_tuta(col);
    colloquium_respondere(col, status, "text/html; charset=utf-8",
        chorda_aedificator_finire(a));
}

interior vacuum
_errare (
    HospitiumColloquium* col,
                    i32  status,
     constans character* nuntius)
{
    ChordaAedificator* a = _paginam_incipere(col, "Exemplar");

    chorda_aedificator_appendere_literis(a, "<p class=\"error\">");
    _evadere(a, chorda_ex_literis(nuntius, colloquium_piscina(col)));
    chorda_aedificator_appendere_literis(a,
        "</p><p><a href=\"/\">redi</a></p>");
    _paginam_mittere(col, status, a);
}

interior vacuum
_csrf_campus (
    ChordaAedificator* a,
               chorda  csrf)
{
    chorda_aedificator_appendere_literis(a,
        "<input type=\"hidden\" name=\"csrf\" value=\"");
    _evadere(a, csrf);
    chorda_aedificator_appendere_literis(a, "\">");
}


/* ==================================================
 * SESSIO ET CUSTODES PETITIONIS
 * ================================================== */

nomen structura {
      Sessio* sessio;
    Sigillum  sigillum;
      chorda  hex;
} Intratus;

interior b32
_intratus (
               Exemplar* e,
    HospitiumColloquium* col,
               Intratus* exitus)
{
     chorda  signum  = colloquium_crustulum(col, "sid");
     vacuum* s       = NIHIL;

    si (signum.datum == NIHIL || signum.mensura != XLIII)
    {
        redde FALSUM;
    }
    exitus->sigillum = custodia_signi_sigillum(signum);
    exitus->hex = _hex_sigilli(&exitus->sigillum,
        colloquium_piscina(col));
    si (!tabula_dispersa_invenire(e->sessiones, exitus->hex, &s))
    {
        redde FALSUM;
    }
    exitus->sessio = (Sessio*)s;
    redde exitus->sessio->expirat_ms > _nunc_ms()
        && _usor(e, exitus->sessio->usor) != NIHIL;
}

/* Origin == schema://Host (methodi non tutae) */
interior b32
_origo_bona (
    HospitiumColloquium* col)
{
    redde custodia_origo_congruit(colloquium_caput(col, "Origin"),
        colloquium_schema(col), colloquium_caput(col, "Host"));
}

interior b32
_csrf_bonum (
               Exemplar* e,
    HospitiumColloquium* col,
               Intratus* i)
{
    redde custodia_csrf_verificare(e->custodia, &i->sigillum,
        colloquium_forma(col, "csrf"));
}

interior b32
_crustulum_ponere (
    HospitiumColloquium* col,
     constans character* signum,
                    s64  aetas_s)
{
    CrustulumOptiones o;

    memset(&o, 0, magnitudo(o));
    o.max_aetas_s  = aetas_s;
    o.http_solum   = VERUM;
    o.securum      = colloquium_schema(col).mensura == V;   /* https */
    o.eadem        = CRUSTULUM_EADEM_LAXA;
    redde colloquium_crustulum_ponere(col, "sid", chorda_ex_literis(
        signum, colloquium_piscina(col)), &o);
}


/* ==================================================
 * OPERA DILATA (inscriptio, intratio)
 * ================================================== */

interior Opus*
_opus_capere (
               Exemplar* e,
              OpusGenus  genus,
    HospitiumColloquium* col,
                 chorda  nomen_usoris,
                 chorda  tessera)
{
    i32 i;

    si (   tessera.mensura > CUSTODIA_TESSERA_MAXIMA
        || nomen_usoris.mensura > EXEMPLAR_NOMEN_MAXIMUM)
    {
        redde NIHIL;
    }
    per (i = 0; i < EXEMPLAR_OPERA; i++)
    {
        Opus* o = &e->opera[i];

        si (o->genus == OPUS_LIBERUM)
        {
            memset(o, 0, magnitudo(*o));
            o->exemplar  = e;
            o->genus     = genus;
            o->tn        = tessera.mensura;
            memcpy(o->tessera, tessera.datum, (size_t)tessera.mensura);
            memcpy(o->nomen_usoris, nomen_usoris.datum,
                (size_t)nomen_usoris.mensura);
            o->d = colloquium_differre(col);
            redde o;
        }
    }
    redde NIHIL;
}

interior vacuum
_opus_liberare (
    Opus* o)
{
    memset(o->tessera, 0, magnitudo(o->tessera));
    o->genus = OPUS_LIBERUM;
}

interior chorda
_tessera_operis (
    Opus* o)
{
    redde chorda_ex_buffer(o->tessera, o->tn);
}

/* ---- inscriptio ---- */
interior vacuum
_inscriptio_facta (
     vacuum* datum,
     chorda  phc)
{
                   Opus* o = (Opus*)datum;
               Exemplar* e = o->exemplar;
    HospitiumColloquium* col;
                 chorda  nomen_usoris;
                Piscina* p;

    col = dilatum_colloquium(o->d);
    nomen_usoris = chorda_ex_literis(o->nomen_usoris, e->piscina);
    p = piscina_generare_dynamicum("exemplar_actum", MMXLVIII);

    si (phc.mensura == 0 || _usor(e, nomen_usoris) != NIHIL)
    {
        si (col)
        {
            _errare(col, phc.mensura == 0 ? D : CDIX,
                phc.mensura == 0 ? "error servi" : "nomen iam captum");
        }
    }
    alioquin
    {
        JsonValor* a = json_objectum_creare(p);

        _ponere(a, "usor", nomen_usoris, p);
        _ponere(a, "phc", phc, p);
        si (_actum(e, "usor.creatus", a, p) == 0)
        {
            si (col)
            {
                _errare(col, D, "volumen scribi nequit");
            }
        }
        alioquin
        {
            _usor_ponere(e, nomen_usoris, phc);
            si (col)
            {
                colloquium_redirigere(col, CCCIII, "/intrare");
            }
        }
    }
    piscina_destruere(p);
    _opus_liberare(o);
}

interior vacuum
_inscribere_post (
    HospitiumColloquium* col)
{
     Exemplar* e             = (Exemplar*)colloquium_datum(col);
       chorda  nomen_usoris  = colloquium_forma(col, "nomen");
       chorda  tessera       = colloquium_forma(col, "tessera");
         Opus* o;

    si (!_origo_bona(col))
    {
        _errare(col, CDIII, "origo aliena");
        redde;
    }
    si (   !e->cfg.invitatio
        || !_aequale_constans(colloquium_forma(col, "invitatio"),
               e->cfg.invitatio))
    {
        _errare(col, CDIII, "invitatio necessaria");
        redde;
    }
    si (!_nomen_validum(nomen_usoris))
    {
        _errare(col, CD, "nomen: I-XXXII signa a-z 0-9 . _ -");
        redde;
    }
    si (!custodia_tessera_valida(tessera))
    {
        _errare(col, CD, "tessera: VIII-MXXIV octeti");
        redde;
    }
    si (_usor(e, nomen_usoris) != NIHIL)
    {
        _errare(col, CDIX, "nomen iam captum");
        redde;
    }
    o = _opus_capere(e, OPUS_INSCRIBERE, col, nomen_usoris, tessera);
    si (!o)
    {
        _errare(col, DIII, "occupatum - iterum tempta");
        redde;
    }
    si (!custodia_tesseram_digerere(e->custodia, tessera,
        _inscriptio_facta, o))
    {
        HospitiumColloquium* c = dilatum_colloquium(o->d);

        _opus_liberare(o);
        si (c)
        {
            _errare(c, DIII, "occupatum - iterum tempta");
        }
    }
}

/* ---- intratio ---- */
interior vacuum
_renovatio_facta (
     vacuum* datum,
     chorda  phc)
{
        Opus* o = (Opus*)datum;
    Exemplar* e = o->exemplar;

    si (phc.mensura > 0)
    {
         Piscina* p = piscina_generare_dynamicum("exemplar_actum",
                          MMXLVIII);
         JsonValor* a = json_objectum_creare(p);
            chorda  n = chorda_ex_literis(o->nomen_usoris, p);

        _ponere(a, "usor", n, p);
        _ponere(a, "phc", phc, p);
        si (_actum(e, "tessera.mutata", a, p) > 0)
        {
            _usor_ponere(e, n, phc);
        }
        piscina_destruere(p);
    }
    _opus_liberare(o);
}

interior vacuum
_verificatio_facta (
     vacuum* datum,
        b32  bona,
        b32  renovanda)
{
                   Opus* o = (Opus*)datum;
               Exemplar* e = o->exemplar;
    HospitiumColloquium* col;
                    s64  nunc = _nunc_ms();
                Piscina* p;
                 chorda  n;

    col  = dilatum_colloquium(o->d);
    p    = piscina_generare_dynamicum("exemplar_actum", MMXLVIII);
    n    = chorda_ex_literis(o->nomen_usoris, p);

    custodia_frenum_notare(e->custodia,
        chorda_ex_literis(o->clavis_paris,
        p), bona, nunc);
    custodia_frenum_notare(e->custodia, chorda_ex_literis(
        o->clavis_usoris, p), bona, nunc);
    si (!bona)
    {
        si (col)
        {
            _errare(col, CDIII, "nomen aut tessera falsa");
        }
    }
    alioquin
    {
        CustodiaSessio  s;
             JsonValor* a = json_objectum_creare(p);
                chorda  hex;
                   s64  expirat = nunc
                       + (s64)EXEMPLAR_SESSIO_VITA_S * M;

        si (!custodia_sessionem_creare(&s))
        {
            si (col)
            {
                _errare(col, D, "fons fortuitus deficit");
            }
            piscina_destruere(p);
            _opus_liberare(o);
            redde;
        }
        hex = _hex_sigilli(&s.sigillum, p);
        /* SIGILLUM signi in actis, numquam signum ipsum */
        _ponere(a, "sigillum", hex, p);
        _ponere(a, "usor", n, p);
        json_objectum_ponere(a, "expirat", json_integer_creare(p,
            expirat));
        si (_actum(e, "sessio.creata", a, p) == 0)
        {
            si (col)
            {
                _errare(col, D, "volumen scribi nequit");
            }
        }
        alioquin
        {
            _sessionem_ponere(e, hex, n, expirat);
            si (col)
            {
                (vacuum)_crustulum_ponere(col, s.signum,
                    (s64)EXEMPLAR_SESSIO_VITA_S);
                colloquium_redirigere(col, CCCIII, "/notae");
            }
        }
        memset(&s, 0, magnitudo(s));
        si (   renovanda && custodia_tesseram_digerere(e->custodia,
            _tessera_operis(o), _renovatio_facta, o))
        {
            piscina_destruere(p);
            redde;   /* opus manet usque ad renovationem */
        }
    }
    piscina_destruere(p);
    _opus_liberare(o);
}

interior vacuum
_verificare_incipere (
    Opus* o)
{
     Exemplar* e = o->exemplar;
      Piscina* p = piscina_generare_dynamicum("exemplar_verif", CCLVI);
         Usor* u = _usor(e, chorda_ex_literis(o->nomen_usoris, p));
       chorda  phc = u ? u->phc : chorda_ex_literis("", p);

    si (!custodia_tesseram_verificare(e->custodia, phc,
        _tessera_operis(o),
        _verificatio_facta, o))
    {
        HospitiumColloquium* col = dilatum_colloquium(o->d);

        si (col)
        {
            _errare(col, DIII, "occupatum - iterum tempta");
        }
        _opus_liberare(o);
    }
    piscina_destruere(p);
}

interior vacuum
_mora_finita (
    vacuum* datum)
{
    _verificare_incipere((Opus*)datum);
}

interior vacuum
_intrare_post (
    HospitiumColloquium* col)
{
      Exemplar* e             = (Exemplar*)colloquium_datum(col);
        chorda  nomen_usoris  = colloquium_forma(col, "nomen");
        chorda  tessera       = colloquium_forma(col, "tessera");
        chorda  par           = colloquium_cliens(col);
     character  clavis_paris[LXXX];
     character  clavis_usoris[LXXX];
           s64  nunc = _nunc_ms();
           s32  mora_a;
           s32  mora_b;
          Opus* o;

    si (!_origo_bona(col))
    {
        _errare(col, CDIII, "origo aliena");
        redde;
    }
    si (   nomen_usoris.mensura > EXEMPLAR_NOMEN_MAXIMUM
        || tessera.mensura > CUSTODIA_TESSERA_MAXIMA
        || par.mensura > LX)
    {
        _errare(col, CD, "petitio mala");
        redde;
    }
    sprintf(clavis_paris, "par:%.*s", (int)par.mensura,
        (constans character*)par.datum);
    sprintf(clavis_usoris, "usor:%.*s", (int)nomen_usoris.mensura,
        nomen_usoris.datum ? (constans character*)nomen_usoris.datum
        : "");
    mora_a = custodia_frenum(e->custodia,
        chorda_ex_literis(clavis_paris,
        colloquium_piscina(col)), nunc);
    mora_b = custodia_frenum(e->custodia, chorda_ex_literis(
        clavis_usoris, colloquium_piscina(col)), nunc);
    si (mora_a < 0 || mora_b < 0)
    {
        _errare(col, CDXXIX, "conatus nimis multi - postea tempta");
        redde;
    }
    o = _opus_capere(e, OPUS_INTRARE, col, nomen_usoris, tessera);
    si (!o)
    {
        _errare(col, DIII, "occupatum - iterum tempta");
        redde;
    }
    strcpy(o->clavis_paris, clavis_paris);
    strcpy(o->clavis_usoris, clavis_usoris);
    si (mora_a > 0 || mora_b > 0)
    {
        /* mora DIFFERTUR (timer), numquam obstat */
        (vacuum)reactor_timer_adicere(e->reactor,
            (i32)(mora_a > mora_b ? mora_a : mora_b), FALSUM,
            _mora_finita, o);
        redde;
    }
    _verificare_incipere(o);
}


/* ==================================================
 * VIAE
 * ================================================== */

interior vacuum
_radix (
    HospitiumColloquium* col)
{
     Exemplar* e = (Exemplar*)colloquium_datum(col);
     Intratus  i;

    colloquium_redirigere(col, CCCIII, _intratus(e, col, &i) ? "/notae"
        : "/intrare");
}

interior vacuum
_inscribere_get (
    HospitiumColloquium* col)
{
    ChordaAedificator* a = _paginam_incipere(col, "Inscribere");

    chorda_aedificator_appendere_literis(a,
        "<form method=\"post\" action=\"/inscribere\">"
        "<label>nomen <input name=\"nomen\" maxlength=\"32\" required>"
        "</label><label>tessera <input type=\"password\" "
        "name=\"tessera\""
        " minlength=\"8\" required></label><label>invitatio "
        "<input name=\"invitatio\" required></label>"
        "<button>inscribere</button></form>"
        "<p><a href=\"/intrare\">intrare</a></p>");
    _paginam_mittere(col, CC, a);
}

interior vacuum
_intrare_get (
    HospitiumColloquium* col)
{
    ChordaAedificator* a = _paginam_incipere(col, "Intrare");

    chorda_aedificator_appendere_literis(a,
        "<form method=\"post\" action=\"/intrare\">"
        "<label>nomen <input name=\"nomen\" required></label>"
        "<label>tessera <input type=\"password\" name=\"tessera\" "
        "required></label><button>intrare</button></form>"
        "<p><a href=\"/inscribere\">inscribere</a></p>");
    _paginam_mittere(col, CC, a);
}

interior vacuum
_exire_post (
    HospitiumColloquium* col)
{
     Exemplar* e = (Exemplar*)colloquium_datum(col);
     Intratus  i;

    si (!_origo_bona(col))
    {
        _errare(col, CDIII, "origo aliena");
        redde;
    }
    si (_intratus(e, col, &i))
    {
         Piscina* p;
       JsonValor* a;

        si (!_csrf_bonum(e, col, &i))
        {
            _errare(col, CDIII, "signum formae deest");
            redde;
        }
        p = colloquium_piscina(col);
        a = json_objectum_creare(p);
        _ponere(a, "sigillum", i.hex, p);
        si (_actum(e, "sessio.deleta", a, p) > 0)
        {
            tabula_dispersa_delere(e->sessiones, i.hex);
        }
    }
    (vacuum)_crustulum_ponere(col, "", 0);
    colloquium_redirigere(col, CCCIII, "/intrare");
}

interior vacuum
_notae_get (
    HospitiumColloquium* col)
{
             Exemplar* e = (Exemplar*)colloquium_datum(col);
             Intratus  i;
    ChordaAedificator* a;
               chorda  csrf;
                  s32  k;

    si (!_intratus(e, col, &i))
    {
        colloquium_redirigere(col, CCCIII, "/intrare");
        redde;
    }
    csrf = custodia_csrf(e->custodia, &i.sigillum,
        colloquium_piscina(col));
    a = _paginam_incipere(col, "Notae");
    chorda_aedificator_appendere_literis(a, "<p>salve, ");
    _evadere(a, i.sessio->usor);
    chorda_aedificator_appendere_literis(a, "</p>"
        "<form method=\"post\" action=\"/notae\" "
        "enctype=\"multipart/form-data\">");
    _csrf_campus(a, csrf);
    chorda_aedificator_appendere_literis(a,
        "<textarea name=\"textus\" rows=\"4\"></textarea>"
        "<input type=\"file\" name=\"filum\">"
        "<button>notam addere</button></form>");
    per (k = (s32)xar_numerus(e->notae) - I; k >= 0; k--)
    {
             Nota* n = (Nota*)xar_obtinere(e->notae, (i32)k);
        character  id[XXIV];

        si (n->deleta || !chorda_aequalis(n->usor, i.sessio->usor))
        {
            perge;
        }
        chorda_aedificator_appendere_literis(a,
            "<div class=\"nota\"><p>");
        _evadere(a, n->textus);
        chorda_aedificator_appendere_literis(a, "</p>");
        si (n->sigillum.mensura > 0)
        {
            character m[XXXII];

            chorda_aedificator_appendere_literis(a,
                "<p><a href=\"/fila/");
            _evadere(a, n->sigillum);
            chorda_aedificator_appendere_literis(a, "\">");
            _evadere(a, n->titulus.mensura > 0 ? n->titulus
                : chorda_ex_literis("filum", colloquium_piscina(col)));
            sprintf(m, "</a> (%lu B)</p>",
                (insignatus longus)n->mensura);
            chorda_aedificator_appendere_literis(a, m);
        }
        sprintf(id, "%ld", (longus)n->id);
        chorda_aedificator_appendere_literis(a,
            "<form method=\"post\" action=\"/notae/");
        chorda_aedificator_appendere_literis(a, id);
        chorda_aedificator_appendere_literis(a, "/delere\">");
        _csrf_campus(a, csrf);
        chorda_aedificator_appendere_literis(a,
            "<button>delere</button></form></div>");
    }
    chorda_aedificator_appendere_literis(a,
        "<form method=\"post\" action=\"/exire\">");
    _csrf_campus(a, csrf);
    chorda_aedificator_appendere_literis(a,
        "<button>exire</button></form>");
    _paginam_mittere(col, CC, a);
}

interior vacuum
_notae_post (HospitiumColloquium* col)
{
                   Exemplar* e = (Exemplar*)colloquium_datum(col);
                   Intratus  i;
                     chorda  textus;
    constans HospitiumFilum* fila;
    constans HospitiumFilum* filum = NIHIL;
                        i32  numerus;
                        i32  k;
                    Piscina* p = colloquium_piscina(col);
                  JsonValor* a;
                        s64  id;

    si (!_origo_bona(col))
    {
        _errare(col, CDIII, "origo aliena");
        redde;
    }
    si (!_intratus(e, col, &i))
    {
        _errare(col, CDIII, "intrare necesse");
        redde;
    }
    si (!_csrf_bonum(e, col, &i))
    {
        _errare(col, CDIII, "signum formae deest");
        redde;
    }
    textus   = colloquium_forma(col, "textus");
    numerus  = colloquium_fila(col, &fila);
    per (k = 0; k < numerus; k++)
    {
        si (   chorda_aequalis_literis(fila[k].campus, "filum")
            && (fila[k].mensura > 0
            || fila[k].titulus_fili.mensura > 0))
        {
            filum = &fila[k];
            frange;
        }
    }
    si (   textus.mensura > EXEMPLAR_TEXTUS_MAXIMUS
        || (textus.mensura == 0 && filum == NIHIL))
    {
        _errare(col, CD, "nota vacua aut nimis longa");
        redde;
    }
    si (textus.datum == NIHIL)
    {
        textus = chorda_ex_literis("", p);
    }
    si (!volumen_transactionem_incipere(e->volumen))
    {
        _errare(col, D, "volumen occupatum");
        redde;
    }
    a = json_objectum_creare(p);
    _ponere(a, "usor", i.sessio->usor, p);
    _ponere(a, "textus", textus, p);
    si (filum)
    {
         JsonValor* f    = json_objectum_creare(p);
            chorda  sig  = chorda_ex_literis(filum->sigillum_hex, p);

        si (!volumen_massam_condere_ex_filo(e->volumen, filum->via,
            filum->mensura, filum->sigillum_hex))
        {
            (vacuum)volumen_transactionem_revolvere(e->volumen);
            _errare(col, D, "filum servari nequit");
            redde;
        }
        _ponere(f, "usor", i.sessio->usor, p);
        _ponere(f, "sigillum", sig, p);
        _ponere(f, "titulus", filum->titulus_fili, p);
        _ponere(f, "mimen", filum->mimen, p);
        json_objectum_ponere(f, "mensura", json_integer_creare(p,
            (s64)filum->mensura));
        si (_actum(e, "filum.receptum", f, p) == 0)
        {
            (vacuum)volumen_transactionem_revolvere(e->volumen);
            _errare(col, D, "volumen scribi nequit");
            redde;
        }
        _ponere(a, "sigillum", sig, p);
        _ponere(a, "titulus", filum->titulus_fili, p);
        _ponere(a, "mimen", filum->mimen, p);
        json_objectum_ponere(a, "mensura", json_integer_creare(p,
            (s64)filum->mensura));
    }
    id = _actum(e, "nota.scripta", a, p);
    si (id == 0 || !volumen_transactionem_committere(e->volumen))
    {
        (vacuum)volumen_transactionem_revolvere(e->volumen);
        _errare(col, D, "volumen scribi nequit");
        redde;
    }
    {
        Nota* n = (Nota*)xar_addere(e->notae);

        memset(n, 0, magnitudo(*n));
        n->id      = id;
        n->usor    = _copia(i.sessio->usor, e->piscina);
        n->textus  = _copia(textus, e->piscina);
        si (filum)
        {
            n->sigillum = _copia(chorda_ex_literis(filum->sigillum_hex,
                p),
                e->piscina);
            n->titulus  = _copia(filum->titulus_fili, e->piscina);
            n->mimen    = _copia(filum->mimen, e->piscina);
            n->mensura  = filum->mensura;
        }
        alioquin
        {
            n->sigillum = _copia(chorda_ex_literis("", p), e->piscina);
        }
    }
    colloquium_redirigere(col, CCCIII, "/notae");
}

interior vacuum
_nota_delere (
    HospitiumColloquium* col)
{
     Exemplar* e = (Exemplar*)colloquium_datum(col);
     Intratus  i;
       chorda  id_ch = colloquium_param(col, "id");
    character  id_l[XXIV];
          s64  id;
         Nota* n;

    si (!_origo_bona(col))
    {
        _errare(col, CDIII, "origo aliena");
        redde;
    }
    si (!_intratus(e, col, &i) || !_csrf_bonum(e, col, &i))
    {
        _errare(col, CDIII, "intrare necesse");
        redde;
    }
    si (id_ch.mensura < I || id_ch.mensura > XVIII)
    {
        _errare(col, CDIV, "nota ignota");
        redde;
    }
    memcpy(id_l, id_ch.datum, (size_t)id_ch.mensura);
    id_l[id_ch.mensura]  = '\0';
    id                   = (s64)atol(id_l);
    n                    = _nota(e, id);
    si (!n || n->deleta || !chorda_aequalis(n->usor, i.sessio->usor))
    {
        _errare(col, CDIV, "nota ignota");
        redde;
    }
    {
          Piscina* p = colloquium_piscina(col);
        JsonValor* a = json_objectum_creare(p);

        json_objectum_ponere(a, "id", json_integer_creare(p, id));
        si (_actum(e, "nota.deleta", a, p) == 0)
        {
            _errare(col, D, "volumen scribi nequit");
            redde;
        }
    }
    n->deleta = VERUM;
    colloquium_redirigere(col, CCCIII, "/notae");
}

/* fons secundus: massa voluminis per partes */
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

/* titulus pro Content-Disposition: ASCII imprimibilis, sine '"'
 * et '\\' */
interior chorda
_titulus_tutus (
      chorda  t,
     Piscina* p)
{
    chorda c = _copia(t, p);
       i32 i;

    per (i = 0; i < c.mensura; i++)
    {
        si (c.datum[i] < 0x20 || c.datum[i] > 0x7E || c.datum[i] == '"'
            || c.datum[i] == '\\')
        {
            c.datum[i] = '_';
        }
    }
    si (c.mensura == 0)
    {
        c = chorda_ex_literis("filum", p);
    }
    redde c;
}

interior vacuum
_filum_get (
    HospitiumColloquium* col)
{
               Exemplar* e = (Exemplar*)colloquium_datum(col);
               Intratus  i;
                 chorda  sig  = colloquium_param(col, "sigillum");
                   Nota* n    = NIHIL;
                    s32  k;
                Piscina* p = colloquium_piscina(col);
     VolumenMassaLector* l;
                    i64  mensura = 0;
          HospitiumFons  fons;
              character  dispositio[CCCLXXXIV];
                 chorda  titulus;

    /* nemo intratus, filum ignotum, filum alienum: idem 404 */
    si (!_intratus(e, col, &i) || sig.mensura != LXIV)
    {
        _errare(col, CDIV, "filum ignotum");
        redde;
    }
    per (k = 0; k < (s32)xar_numerus(e->notae); k++)
    {
        Nota* x = (Nota*)xar_obtinere(e->notae, (i32)k);

        si (   !x->deleta && chorda_aequalis(x->sigillum, sig)
            && chorda_aequalis(x->usor, i.sessio->usor))
        {
            n = x;
            frange;
        }
    }
    si (!n)
    {
        _errare(col, CDIV, "filum ignotum");
        redde;
    }
    l = volumen_massam_aperire(e->volumen, sig, &mensura, p);
    si (!l)
    {
        _errare(col, CDIV, "filum ignotum");
        redde;
    }
    titulus = _titulus_tutus(n->titulus, p);
    sprintf(dispositio, "attachment; filename=\"%.*s\"",
        (int)(titulus.mensura > CCC ? CCC : titulus.mensura),
        (constans character*)titulus.datum);
    colloquium_caput_addere(col, "Content-Disposition", dispositio);
    colloquium_caput_addere(col, "X-Content-Type-Options", "nosniff");
    colloquium_caput_addere(col, "Content-Security-Policy", "sandbox");
    fons.mensura   = (i64)mensura;
    fons.legere    = _fons_massae_legere;
    fons.claudere  = _fons_massae_claudere;
    fons.datum     = l;
    colloquium_respondere_fontem(col, CC, "application/octet-stream",
        fons);
}

interior vacuum
_salus (
    HospitiumColloquium* col)
{
     Exemplar* e = (Exemplar*)colloquium_datum(col);
    character  s[LXIV];

    sprintf(s, "{\"status\":\"ok\",\"acta\":%ld}",
        (longus)volumen_summa_actorum(e->volumen));
    colloquium_respondere(col, CC, "application/json",
        chorda_ex_literis(s, colloquium_piscina(col)));
}


/* ==================================================
 * CYCLUS VITAE
 * ================================================== */

interior b32
_via (
              character* exitus,
     constans character* directorium,
     constans character* nomen_fili)
{
    si (strlen(directorium) + strlen(nomen_fili) + II
        > EXEMPLAR_VIA_MAXIMA)
    {
        redde FALSUM;
    }
    sprintf(exitus, "%s/%s", directorium, nomen_fili);
    redde VERUM;
}

Exemplar*
exemplar_creare (
    constans ExemplarConfiguratio* configuratio,
                         Piscina* piscina)
{
                Exemplar* e;
               character  via[EXEMPLAR_VIA_MAXIMA];
    CustodiaConfiguratio  cfg_custodiae;
         VolumenOptiones  optiones_voluminis;
   HospitiumConfiguratio  cfg_hospitii;

    si (!configuratio || !configuratio->via_datorum || !piscina)
    {
        redde NIHIL;
    }
    e = (Exemplar*)piscina_allocare(piscina, (i64)magnitudo(Exemplar));
    memset(e, 0, magnitudo(*e));
    e->piscina  = piscina;
    e->cfg      = *configuratio;
    si (configuratio->invitatio)
    {
        e->cfg.invitatio = chorda_ut_cstr(chorda_ex_literis(
            configuratio->invitatio, piscina), piscina);
    }

    /* data 0700, tmp 0700 (idem systema filorum) */
    si (   !filum_directorium_creare_cum_modo(configuratio->via_datorum,
               0700)
        || !_via(e->via_temporaria, configuratio->via_datorum, "tmp")
        || !filum_directorium_creare_cum_modo(e->via_temporaria, 0700))
    {
        fprintf(stderr, "exemplar: data creari nequit: %s\n",
            configuratio->via_datorum);
        redde NIHIL;
    }

    memset(&cfg_custodiae, 0, magnitudo(cfg_custodiae));
    si (   !_via(via, configuratio->via_datorum, "secretum")
        || !custodia_secretum_parare(via, cfg_custodiae.secretum))
    {
        fprintf(stderr,
            "exemplar: secretum servi (data/secretum, 0600) "
            "legi aut creari nequit\n");
        redde NIHIL;
    }
    cfg_custodiae.frenum_conatus = configuratio->frenum_conatus;

    memset(&optiones_voluminis, 0, magnitudo(optiones_voluminis));
    optiones_voluminis.durabilitas_plena = VERUM;
    si (   !_via(via, configuratio->via_datorum, "exemplar.volumen")
        || !(e->volumen = volumen_aperire_aut_creare_cum(piscina, via,
               &optiones_voluminis)))
    {
        fprintf(stderr, "exemplar: volumen aperiri nequit\n");
        memset(cfg_custodiae.secretum, 0,
            magnitudo(cfg_custodiae.secretum));
        redde NIHIL;
    }

    e->reactor_proprius = configuratio->reactor == NIHIL;
    e->reactor = configuratio->reactor ? configuratio->reactor
        : reactor_creare(piscina);
    e->custodia = custodia_creare(&cfg_custodiae, e->reactor, piscina);
    memset(cfg_custodiae.secretum, 0,
        magnitudo(cfg_custodiae.secretum));
    e->usores     = tabula_dispersa_creare_chorda(piscina, LXIV);
    e->sessiones  = tabula_dispersa_creare_chorda(piscina, LXIV);
    e->notae      = xar_creare(piscina, (i32)magnitudo(Nota));
    si (   !e->reactor || !e->custodia || !e->usores || !e->sessiones
        || !e->notae)
    {
        volumen_claudere(e->volumen);
        redde NIHIL;
    }
    _replicare(e);

    memset(&cfg_hospitii, 0, magnitudo(cfg_hospitii));
    cfg_hospitii.hospes                   = configuratio->hospes;
    cfg_hospitii.portus                   = configuratio->portus;
    cfg_hospitii.reactor                  = e->reactor;
    cfg_hospitii.petitio_maxima           = EXEMPLAR_PETITIO_MAXIMA;
    cfg_hospitii.directorium_temporarium  = e->via_temporaria;
    cfg_hospitii.procurator_fidus =
        configuratio->procurator_fidus;
    cfg_hospitii.acta_accessus = configuratio->acta_accessus;
    e->hospitium = hospitium_creare(piscina,
        &cfg_hospitii);
    si (!e->hospitium)
    {
        fprintf(stderr, "exemplar: portus ligari nequit\n");
        volumen_claudere(e->volumen);
        redde NIHIL;
    }
    (vacuum)hospitium_praebere(e->hospitium, HTTP_GET, "/", _radix, e);
    (vacuum)hospitium_praebere(e->hospitium, HTTP_GET, "/inscribere",
        _inscribere_get, e);
    (vacuum)hospitium_praebere(e->hospitium, HTTP_POST, "/inscribere",
        _inscribere_post, e);
    (vacuum)hospitium_praebere(e->hospitium, HTTP_GET, "/intrare",
        _intrare_get, e);
    (vacuum)hospitium_praebere(e->hospitium, HTTP_POST, "/intrare",
        _intrare_post, e);
    (vacuum)hospitium_praebere(e->hospitium, HTTP_POST, "/exire",
        _exire_post, e);
    (vacuum)hospitium_praebere(e->hospitium, HTTP_GET, "/notae",
        _notae_get, e);
    (vacuum)hospitium_praebere(e->hospitium, HTTP_POST, "/notae",
        _notae_post, e);
    (vacuum)hospitium_praebere(e->hospitium, HTTP_POST,
        "/notae/:id/delere", _nota_delere, e);
    (vacuum)hospitium_praebere(e->hospitium, HTTP_GET,
        "/fila/:sigillum",
        _filum_get, e);
    (vacuum)hospitium_praebere(e->hospitium, HTTP_GET, "/salus", _salus,
        e);
    si (configuratio->basis)
    {
        si (strlen(configuratio->basis) >= EXEMPLAR_VIA_MAXIMA)
        {
            fprintf(stderr, "exemplar: basis nimis longa\n");
            volumen_claudere(e->volumen);
            redde NIHIL;
        }
        strcpy(e->basis, configuratio->basis);
        /* auctoritas sine '/' finali (RFC 8414 issuer) */
        dum (   strlen(e->basis) > ZEPHYRUM
             && e->basis[strlen(e->basis) - I] == '/')
        {
            e->basis[strlen(e->basis) - I] = '\0';
        }
    }
    alioquin
    {
        sprintf(e->basis, "http://%s:%u", configuratio->hospes
            ? configuratio->hospes : "127.0.0.1",
            (insignatus integer)hospitium_portus(e->hospitium));
    }
    exemplar_mcp_praebere(e);
    redde e;
}

Hospitium*
exemplar_hospitium (
    Exemplar* exemplar)
{
    redde exemplar ? exemplar->hospitium : NIHIL;
}

Reactor*
exemplar_reactor (
    Exemplar* exemplar)
{
    redde exemplar ? exemplar->reactor : NIHIL;
}

vacuum
exemplar_claudere (
    Exemplar* exemplar)
{
    si (!exemplar)
    {
        redde;
    }
    hospitium_destruere(exemplar->hospitium);
    /* WAL in plagulam principalem: copia (cp) post clausuram integra */
    (vacuum)scrinium_exsequi(volumen_scrinium(exemplar->volumen),
        "PRAGMA wal_checkpoint(TRUNCATE)");
    volumen_claudere(exemplar->volumen);
    si (exemplar->reactor_proprius)
    {
        reactor_destruere(exemplar->reactor);
    }
}
