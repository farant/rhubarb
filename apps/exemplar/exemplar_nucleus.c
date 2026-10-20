/* exemplar_nucleus.c - applicatio probans (vide exemplar_nucleus.h)
 *
 * Rationes per ianuam (lib/ianua: cives, credentialia, invitationes,
 * sessiones in tabulis - ianua T5). Status in memoria solum ordo
 * notarum, ex actis replicatus ad initium (nota.scripta cum civi;
 * acta vetera cum nomine per exemplar_nomina_vetera); omnis mutatio
 * prius actum scribit, deinde memoriam mutat. Volumen ante ianuam
 * SEMEL migratur (_cives_migrare), transactione una. */
#include "postulata_posix.h"
#define EXEMPLAR_PROBATIO
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
#include "xar.h"
#include "sigillum.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/* ==================================================
 * AUXILIA
 * ================================================== */

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

/* civis notae veteris: ULID ex mappa nominum (migratio T5) */
interior chorda
_civis_veteris (
     Exemplar* e,
       chorda  nomen_usoris,
      Piscina* p)
{
    ScriniumEnuntiatum* q = scrinium_praeparare(volumen_scrinium(
        e->volumen), "SELECT civis FROM exemplar_nomina_vetera"
        " WHERE nomen_usoris = ?");
                 chorda civis = chorda_ex_literis("", p);

    si (q)
    {
        scrinium_ligare_textum(q, I, nomen_usoris);
        si (scrinium_gradi(q) == SCRINIUM_ORDO)
        {
            civis = scrinium_columna_textus(q, ZEPHYRUM, p);
        }
        scrinium_finire(q);
    }
    redde civis;
}

/* notae ex actis replicatae (post migrationem) */
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

        si (!chorda_incipit(a->genus, chorda_ex_literis("nota.", p)))
        {
            perge;
        }
        r = json_legere(a->datum, p);
        si (!r.successus)
        {
            perge;
        }
        si (chorda_aequalis_literis(a->genus, "nota.scripta"))
        {
              Nota* n = (Nota*)xar_addere(e->notae);
            chorda  civis = json_capere_chorda(r.radix, "civis",
                vacua);

            si (civis.mensura == ZEPHYRUM)
            {
                civis = _civis_veteris(e, json_capere_chorda(r.radix,
                    "usor", vacua), p);
            }
            memset(n, 0, magnitudo(*n));
            n->id     = a->seq;
            n->civis  = _copia(civis, e->piscina);
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
 * IANUA (rationes - ianua T5)
 * ================================================== */

/* pagina ianuae in fabrica exemplaris (stilus, capita tuta) */
interior vacuum
_pagina_ianuae (
                 vacuum* datum,
    HospitiumColloquium* col,
                    i32  status,
     constans character* titulus,
                 chorda  corpus)
{
    ChordaAedificator* a = _paginam_incipere(col, titulus);

    (vacuum)datum;
    chorda_aedificator_appendere_chorda(a, corpus);
    _paginam_mittere(col, status, a);
}

/* intratus: civis (activus) + signum CSRF sessionis */
nomen structura {
     Civis civis;
    chorda csrf;
} Intratus;

interior b32
_intratus (
               Exemplar* e,
    HospitiumColloquium* col,
               Intratus* exitus)
{
    redde ianua_intratus(e->ianua, col, &exitus->civis, &exitus->csrf);
}

/* Origin == schema://Host (methodi non tutae) */
interior b32
_origo_bona (
    HospitiumColloquium* col)
{
    redde custodia_origo_congruit(colloquium_caput(col, "Origin"),
        colloquium_schema(col), colloquium_caput(col, "Host"));
}

/* Origo + sessio + CSRF formae (ianua_custos_postis) */
interior b32
_csrf_bonum (
               Exemplar* e,
    HospitiumColloquium* col,
               Intratus* i)
{
    (vacuum)i;
    redde ianua_custos_postis(e->ianua, col, NIHIL);
}

interior vacuum
_radix (
    HospitiumColloquium* col)
{
    Exemplar* e = (Exemplar*)colloquium_datum(col);

    colloquium_redirigere(col, CCCIII, ianua_intratus(e->ianua, col,
        NIHIL,
        NIHIL) ? "/notae" : "/intrare");
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
    csrf  = i.csrf;
    a     = _paginam_incipere(col, "Notae");
    chorda_aedificator_appendere_literis(a, "<p>salve, ");
    _evadere(a, i.civis.titulus);
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

        si (n->deleta || !chorda_aequalis(n->civis, i.civis.id))
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
        "<p><a href=\"/applicationes\">applicationes connexae</a></p>"
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
    _ponere(a, "civis", i.civis.id, p);
    _ponere(a, "textus", textus, p);
    _ponere(a, "via", chorda_ex_literis("retis", p), p);
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
        _ponere(f, "civis", i.civis.id, p);
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
        n->civis   = _copia(i.civis.id, e->piscina);
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
    si (!n || n->deleta || !chorda_aequalis(n->civis, i.civis.id))
    {
        _errare(col, CDIV, "nota ignota");
        redde;
    }
    si (!exemplar_notam_delere(e, i.civis.id, id,
            colloquium_piscina(col)))
    {
        _errare(col, D, "volumen scribi nequit");
        redde;
    }
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
            && chorda_aequalis(x->civis, i.civis.id))
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

s64
exemplar_notam_scribere (
    Exemplar* e,
       chorda  civis,
       chorda  textus,
       chorda  cliens,
     Piscina* p)
{
    JsonValor* a;
          s64  id;
         Nota* n;

    si (   !e || textus.mensura == 0
        || textus.mensura > EXEMPLAR_TEXTUS_MAXIMUS)
    {
        redde 0;
    }
    a = json_objectum_creare(p);
    _ponere(a, "civis", civis, p);
    _ponere(a, "textus", textus, p);
    _ponere(a, "via", chorda_ex_literis("claude", p), p);
    _ponere(a, "cliens", cliens, p);
    id = _actum(e, "nota.scripta", a, p);
    si (id == 0)
    {
        redde 0;
    }
    n = (Nota*)xar_addere(e->notae);
    memset(n, 0, magnitudo(*n));
    n->id        = id;
    n->civis     = _copia(civis, e->piscina);
    n->textus    = _copia(textus, e->piscina);
    n->sigillum  = _copia(chorda_ex_literis("", p), e->piscina);
    redde id;
}

b32
exemplar_notam_delere (
     Exemplar* e,
       chorda  civis,
          s64  id,
      Piscina* p)
{
         Nota* n = e ? _nota(e, id) : NIHIL;
    JsonValor* a;

    si (!n || n->deleta || !chorda_aequalis(n->civis, civis))
    {
        redde FALSUM;
    }
    a = json_objectum_creare(p);
    json_objectum_ponere(a, "id", json_integer_creare(p, id));
    si (_actum(e, "nota.deleta", a, p) == 0)
    {
        redde FALSUM;
    }
    n->deleta = VERUM;
    redde VERUM;
}

/* momentum (ms) ut "AAAA-MM-DD HH:MM UTC" */
interior vacuum
_momentum_scribere (
    character* exitus,
          s64  ms)
{
         time_t t    = (time_t)(ms / M);
    structura tm* g  = gmtime(&t);

    si (!g)
    {
        strcpy(exitus, "?");
        redde;
    }
    sprintf(exitus, "%04d-%02d-%02d %02d:%02d UTC", g->tm_year + MCM,
        g->tm_mon + I, g->tm_mday, g->tm_hour, g->tm_min);
}

/* /applicationes: connexiones personae (Claude etc.), revocare */
interior vacuum
_applicationes_get (
    HospitiumColloquium* col)
{
             Exemplar* e = (Exemplar*)colloquium_datum(col);
             Intratus  i;
    ChordaAedificator* a;
               chorda  csrf;
                  Xar* lista;
                  i32  k;

    si (!_intratus(e, col, &i))
    {
        colloquium_redirigere(col, CCCIII,
            "/intrare?redire=%2Fapplicationes");
        redde;
    }
    csrf   = i.csrf;
    lista  = e->admissio ? admissio_concessiones(e->admissio,
        i.civis.id, colloquium_piscina(col)) : NIHIL;
    a = _paginam_incipere(col, "Applicationes");
    si (!lista || xar_numerus(lista) == 0)
    {
        chorda_aedificator_appendere_literis(a,
            "<p>nullae applicationes connexae</p>");
    }
    per (k = 0; lista && k < xar_numerus(lista); k++)
    {
        AdmissioConcessio* g = (AdmissioConcessio*)xar_obtinere(lista,
            k);
                character m[LXIV];

        chorda_aedificator_appendere_literis(a,
            "<div class=\"nota\"><p><b>");
        _evadere(a, g->titulus_clientis.mensura
            > 0 ? g->titulus_clientis
            : g->cliens);
        chorda_aedificator_appendere_literis(a, "</b></p><p>scopi: ");
        _evadere(a, g->scopi);
        chorda_aedificator_appendere_literis(a, "</p><p>creata: ");
        _momentum_scribere(m, g->creata_ms);
        chorda_aedificator_appendere_literis(a, m);
        chorda_aedificator_appendere_literis(a,
            " &middot; usus ultimus: ");
        _momentum_scribere(m, g->usus_ultimus_ms);
        chorda_aedificator_appendere_literis(a, m);
        chorda_aedificator_appendere_literis(a,
            "</p><form method=\"post\" action=\"/applicationes/");
        _evadere(a, g->id);
        chorda_aedificator_appendere_literis(a, "/revocare\">");
        _csrf_campus(a, csrf);
        chorda_aedificator_appendere_literis(a,
            "<button>revocare</button></form></div>");
    }
    chorda_aedificator_appendere_literis(a,
        "<p><a href=\"/notae\">notae</a></p>");
    _paginam_mittere(col, CC, a);
}

interior vacuum
_applicationem_revocare (
    HospitiumColloquium* col)
{
    Exemplar* e = (Exemplar*)colloquium_datum(col);
    Intratus  i;

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
    si (   !e->admissio
        || !admissio_revocare(e->admissio, i.civis.id,
               colloquium_param(col, "id")))
    {
        _errare(col, CDIV, "applicatio ignota");
        redde;
    }
    colloquium_redirigere(col, CCCIII, "/applicationes");
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

/* ---- migratio civium (ianua T5): volumen ante ianuam ---- */

hic_manens constans character* constans SCHEMA_EXEMPLARIS[] = {
    "CREATE TABLE IF NOT EXISTS exemplar_migrationes"
    " (titulus TEXT PRIMARY KEY, facta_ms INTEGER NOT NULL)",
    "CREATE TABLE IF NOT EXISTS exemplar_nomina_vetera"
    " (nomen_usoris TEXT PRIMARY KEY, civis TEXT NOT NULL)",
    NIHIL
};

nomen structura {
    chorda nomen_usoris;
    chorda phc;
} UsorVetus;

interior b32
_sententia (
               Scrinium* s,
     constans character* sql,
                 chorda  a,
                 chorda  b)
{
     ScriniumEnuntiatum* q = scrinium_praeparare(s, sql);
                    b32  bene;

    si (!q)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(q, I, a);
    scrinium_ligare_textum(q, II, b);
    bene = scrinium_gradi(q) == SCRINIUM_FACTUM;
    scrinium_finire(q);
    redde bene;
}

/* VERUM si migratio facta (aut iam facta); FALSUM = nihil mutatum,
 * causa ad stderr. Transactione UNA: civis + tessera ultima per
 * nomen vetus, mappa nominum, concessiones/codices admissionis
 * re-clavati, actum exemplar.migratio, signum migrationis. */
interior b32
_cives_migrare (
    Exemplar* e)
{
           Scrinium* s = volumen_scrinium(e->volumen);
            Piscina* p = piscina_generare_dynamicum("exemplar_migratio",
                             CXXVIII * MXXIV);
                   Xar* acta;
                   Xar* veteres;
    ScriniumEnuntiatum* q;
             JsonValor* mappa;
             JsonValor* d;
    constans character* causa = NIHIL;
                   i32  k;
                   i32  j;
                   b32  facta = FALSUM;

    si (!p)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; SCHEMA_EXEMPLARIS[k]; k++)
    {
        si (!scrinium_exsequi(s, SCHEMA_EXEMPLARIS[k]))
        {
            piscina_destruere(p);
            redde FALSUM;
        }
    }
    q = scrinium_praeparare(s,
        "SELECT count(*) FROM exemplar_migrationes"
        " WHERE titulus = 'cives'");
    si (q && scrinium_gradi(q) == SCRINIUM_ORDO)
    {
        facta = scrinium_columna_numerus(q, ZEPHYRUM) > ZEPHYRUM;
    }
    si (q)
    {
        scrinium_finire(q);
    }
    si (facta)
    {
        piscina_destruere(p);
        redde VERUM;
    }
    /* tessera ULTIMA per nomen, ordine primae apparitionis */
    acta     = volumen_acta_legere(e->volumen, ZEPHYRUM, p);
    veteres  = xar_creare(p, (i32)magnitudo(UsorVetus));
    per (k = ZEPHYRUM; acta && veteres && k < xar_numerus(acta); k++)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(acta, k);
        JsonResultus  r;
              chorda  n;
              chorda  phc;
           UsorVetus* u = NIHIL;

        si (   !chorda_aequalis_literis(a->genus, "usor.creatus")
            && !chorda_aequalis_literis(a->genus, "tessera.mutata"))
        {
            perge;
        }
        r = json_legere(a->datum, p);
        si (!r.successus)
        {
            perge;
        }
        n    = json_capere_chorda(r.radix, "usor", chorda_ex_literis("",
            p));
        phc  = json_capere_chorda(r.radix, "phc", chorda_ex_literis("",
            p));
        per (j = ZEPHYRUM; j < xar_numerus(veteres); j++)
        {
            UsorVetus* x = (UsorVetus*)xar_obtinere(veteres, j);

            si (chorda_aequalis(x->nomen_usoris, n))
            {
                u = x;
            }
        }
        si (!u)
        {
            u                = (UsorVetus*)xar_addere(veteres);
            u->nomen_usoris  = n;
        }
        u->phc = phc;
    }
    si (!veteres || !volumen_transactionem_incipere(e->volumen))
    {
        fprintf(stderr, "exemplar: migratio civium fracta: volumen "
            "occupatum\n");
        piscina_destruere(p);
        redde FALSUM;
    }
    mappa = json_objectum_creare(p);
    per (k = ZEPHYRUM; !causa && k < xar_numerus(veteres); k++)
    {
         UsorVetus* u = (UsorVetus*)xar_obtinere(veteres, k);
            chorda  id;

        si (!cives_creare(ianua_cives(e->ianua), u->nomen_usoris,
            u->nomen_usoris, &id, p))
        {
            causa = "civis creari nequit (nomen iam civis?)";
        }
        alioquin si (!credentialia_tesseram_ponere(ianua_credentialia(
                     e->ianua), id, u->phc))
        {
            causa = "tessera poni nequit";
        }
        alioquin si (   !_sententia(s,
                     "INSERT INTO exemplar_nomina_vetera"
                         " (nomen_usoris, civis) VALUES (?, ?)",
                         u->nomen_usoris, id)
                     || !_sententia(s, "UPDATE admissio_concessiones"
                         " SET usor = ? WHERE usor = ?", id,
                         u->nomen_usoris)
                     || !_sententia(s, "UPDATE admissio_codices"
                         " SET usor = ? WHERE usor = ?", id,
                         u->nomen_usoris))
        {
            causa = "mappa aut admissio re-clavari nequit";
        }
        alioquin
        {
            json_objectum_ponere(mappa, chorda_ut_cstr(u->nomen_usoris,
                p),
                json_chorda_creare(p, id));
        }
    }
    si (!causa && xar_numerus(veteres) > ZEPHYRUM)
    {
        d = json_objectum_creare(p);
        json_objectum_ponere(d, "cives", mappa);
        si (volumen_actum_appendere(e->volumen, "exemplar.migratio",
            json_scribere(d, p)) == ZEPHYRUM)
        {
            causa = "actum scribi nequit";
        }
    }
    si (!causa)
    {
        q = scrinium_praeparare(s, "INSERT INTO exemplar_migrationes"
            " (titulus, facta_ms) VALUES ('cives', ?)");
        si (q)
        {
            scrinium_ligare_numerum(q, I, (s64)time(NIHIL) * (s64)M);
            facta = scrinium_gradi(q) == SCRINIUM_FACTUM;
            scrinium_finire(q);
        }
        si (!q || !facta)
        {
            causa = "signum migrationis scribi nequit";
        }
    }
    si (!causa && !volumen_transactionem_committere(e->volumen))
    {
        causa = "commissio fracta";
    }
    si (causa)
    {
        (vacuum)volumen_transactionem_revolvere(e->volumen);
        fprintf(stderr, "exemplar: migratio civium fracta: %s\n",
            causa);
        piscina_destruere(p);
        redde FALSUM;
    }
    si (xar_numerus(veteres) > ZEPHYRUM)
    {
        fprintf(stderr, "exemplar: migratio civium facta (%u)\n",
            (insignatus integer)xar_numerus(veteres));
    }
    piscina_destruere(p);
    redde VERUM;
}

/* creatio fracta: quod iam stat destruitur, sera liberatur */
interior Exemplar*
_creatio_fracta (
    Exemplar* e)
{
    si (e->hospitium)
    {
        hospitium_destruere(e->hospitium);
    }
    si (e->volumen)
    {
        volumen_claudere(e->volumen);
    }
    si (e->reactor && e->reactor_proprius)
    {
        reactor_destruere(e->reactor);
    }
    filum_seram_liberare(e->sera);
    redde NIHIL;
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
           IanuaOptiones  optiones_ianuae;

    si (!configuratio || !configuratio->via_datorum || !piscina)
    {
        redde NIHIL;
    }
    e = (Exemplar*)piscina_allocare(piscina, (i64)magnitudo(Exemplar));
    memset(e, 0, magnitudo(*e));
    e->piscina  = piscina;
    e->cfg      = *configuratio;

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
    /* scriptor UNUS (ianua P1): servus alter aut -invitare currens */
    si (   !_via(via, configuratio->via_datorum, "sera")
        || !(e->sera = filum_seram_capere(via, ZEPHYRUM, piscina)))
    {
        fprintf(stderr, "exemplar: alius processus data tenet (%s) - "
            "servus iam currit?\n", configuratio->via_datorum);
        redde NIHIL;
    }

    memset(&cfg_custodiae, 0, magnitudo(cfg_custodiae));
    si (   !_via(via, configuratio->via_datorum, "secretum")
        || !custodia_secretum_parare(via, cfg_custodiae.secretum))
    {
        fprintf(stderr,
            "exemplar: secretum servi (data/secretum, 0600) "
            "legi aut creari nequit\n");
        redde _creatio_fracta(e);
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
        redde _creatio_fracta(e);
    }

    e->reactor_proprius = configuratio->reactor == NIHIL;
    e->reactor = configuratio->reactor ? configuratio->reactor
        : reactor_creare(piscina);
    e->custodia = custodia_creare(&cfg_custodiae, e->reactor, piscina);
    memset(cfg_custodiae.secretum, 0,
        magnitudo(cfg_custodiae.secretum));
    e->notae = xar_creare(piscina, (i32)magnitudo(Nota));
    memset(&optiones_ianuae, ZEPHYRUM, magnitudo(optiones_ianuae));
    optiones_ianuae.via_post_intrationem  = "/notae";
    optiones_ianuae.invitatio_necessaria  = VERUM;
    optiones_ianuae.pagina                = _pagina_ianuae;
    optiones_ianuae.datum                 = e;
    e->ianua = e->reactor && e->custodia ? ianua_creare(e->volumen,
        e->custodia, e->reactor, &optiones_ianuae, piscina) : NIHIL;
    si (!e->reactor || !e->custodia || !e->notae || !e->ianua)
    {
        fprintf(stderr, "exemplar: custodia aut ianua deficit\n");
        redde _creatio_fracta(e);
    }

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
        redde _creatio_fracta(e);
    }
    (vacuum)hospitium_praebere(e->hospitium, HTTP_GET, "/", _radix, e);
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
    si (!ianua_montare(e->ianua, e->hospitium))
    {
        fprintf(stderr, "exemplar: ianua montari nequit\n");
        redde _creatio_fracta(e);
    }
    si (configuratio->basis)
    {
        si (strlen(configuratio->basis) >= EXEMPLAR_VIA_MAXIMA)
        {
            fprintf(stderr, "exemplar: basis nimis longa\n");
            redde _creatio_fracta(e);
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
    (vacuum)hospitium_praebere(e->hospitium, HTTP_GET, "/applicationes",
        _applicationes_get, e);
    (vacuum)hospitium_praebere(e->hospitium, HTTP_POST,
        "/applicationes/:id/revocare", _applicationem_revocare, e);
    si (!exemplar_mcp_praebere(e))
    {
        fprintf(stderr, "exemplar: servus MCP aut admissio deficit\n");
        redde _creatio_fracta(e);
    }
    /* post admissionem (tabulae eius re-clavantur), ante replicationem
     * (mappa nominum veterum) */
    si (!_cives_migrare(e))
    {
        redde _creatio_fracta(e);
    }
    _replicare(e);
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
    filum_seram_liberare(exemplar->sera);
}

s32
exemplar_invitare (
     constans character* via_datorum,
     constans character* nota,
                 chorda* codex_exitus,
                Piscina* piscina)
{
           FilumSera* sera;
             Volumen* v;
        Invitationes* inv;
     VolumenOptiones  optiones;
              chorda  id;
           character  via[EXEMPLAR_VIA_MAXIMA];
                 b32  bene = FALSUM;

    si (!via_datorum || !codex_exitus || !piscina)
    {
        redde I;
    }
    si (   !filum_directorium_creare_cum_modo(via_datorum, 0700)
        || !_via(via, via_datorum, "sera"))
    {
        redde I;
    }
    sera = filum_seram_capere(via, ZEPHYRUM, piscina);
    si (!sera)
    {
        redde II;
    }
    memset(&optiones, ZEPHYRUM, magnitudo(optiones));
    optiones.durabilitas_plena = VERUM;
    v = _via(via, via_datorum, "exemplar.volumen")
        ? volumen_aperire_aut_creare_cum(piscina, via,
        &optiones) : NIHIL;
    inv = v ? invitationes_aperire(v, NIHIL, piscina) : NIHIL;
    si (inv && volumen_transactionem_incipere(v))
    {
        bene = invitationes_creare(inv, chorda_ex_literis("", piscina),
            chorda_ex_literis(nota ? nota : "", piscina), ZEPHYRUM,
            ZEPHYRUM, codex_exitus, &id, piscina)
            && volumen_transactionem_committere(v);
        si (!bene)
        {
            (vacuum)volumen_transactionem_revolvere(v);
        }
    }
    si (v)
    {
        (vacuum)scrinium_exsequi(volumen_scrinium(v),
            "PRAGMA wal_checkpoint(TRUNCATE)");
        volumen_claudere(v);
    }
    filum_seram_liberare(sera);
    redde bene ? ZEPHYRUM : I;
}

b32
exemplar_invitationem_probationis (
    Exemplar* e,
      chorda* codex_exitus,
     Piscina* piscina)
{
    chorda id;
       b32 bene;

    si (!e || !volumen_transactionem_incipere(e->volumen))
    {
        redde FALSUM;
    }
    bene = invitationes_creare(ianua_invitationes(e->ianua),
        chorda_ex_literis("", piscina), chorda_ex_literis("", piscina),
        ZEPHYRUM, ZEPHYRUM, codex_exitus, &id, piscina)
        && volumen_transactionem_committere(e->volumen);
    si (!bene)
    {
        (vacuum)volumen_transactionem_revolvere(e->volumen);
    }
    redde bene;
}
