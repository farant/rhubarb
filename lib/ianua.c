/* ianua.c - ianua rationum in hospitio (vide include/ianua.h)
 *
 * Status nullus in memoria praeter loculos operum dilatorum (IanuaOpus):
 * cives, credentialia, invitationes, sessiones in tabulis voluminis -
 * quaestio per petitionem. Loculus vivit a POST usque ad responsum
 * dilatum; tesseras NUMQUAM tenet (credentialia et custodia tenent),
 * codicem invitationis tenet (deletum in liberatione).
 *
 * Custodes ex exemplare translati (exemplar_nucleus.c :473-535):
 * redire tutum, Origo, CSRF, crustulum; evasio HTML copiata (ianua P9).
 */

#include "postulata_posix.h"
#include "ianua.h"
#include "chorda_aedificator.h"
#include "crustula.h"
#include "json.h"
#include "scrinium.h"
#include "sigillum.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define PRAEFIXUM_MAXIMUM CXXVIII
#define PHC_MAXIMUM       CCLVI
#define DIES_MS           ((s64)XXIV * LX * LX * M)
#define DIES_MAXIMI       XC

nomen enumeratio {
    IANUA_OPUS_LIBERUM = ZEPHYRUM,
    IANUA_OPUS_INSCRIBERE,
    IANUA_OPUS_INTRARE,
    IANUA_OPUS_TESSERA
} IanuaOpusGenus;

nomen structura {
               Ianua* ianua;
      IanuaOpusGenus  genus;
    HospitiumDilatum  d;
           character  nomen_usoris[CIVES_NOMEN_MAXIMUM + I];
           character  titulus[IANUA_TITULUS_MAXIMUS + I];
           character  codex[INVITATIONES_CODEX_MENSURA + I];
           character  redire[IANUA_REDIRE_MAXIMUM];
           character  civis[CIVES_ID_MENSURA + I];
           character  sigillum[SIGILLUM_HEX_MENSURA];
    /* tessera mutata: verificatio actualis et hash novae simul */
                 b32 verificatio_facta;
                 b32 bona;
                 b32 hash_factum;
           character phc[PHC_MAXIMUM];
} IanuaOpus;

structura Ianua {
          Volumen* volumen;
         Scrinium* scrinium;
         Custodia* custodia;
          Reactor* reactor;
          Piscina* piscina;
            Cives* cives;
     Credentialia* credentialia;
     Invitationes* invitationes;
    IanuaOptiones  o;
        character  praefixum[PRAEFIXUM_MAXIMUM];
        character  via_post[IANUA_REDIRE_MAXIMUM];
        IanuaOpus  opera[IANUA_OPERA];
};

hic_manens constans character* constans SCHEMA[] = {
    "CREATE TABLE IF NOT EXISTS ianua_sessiones"
    " (sigillum TEXT PRIMARY KEY, civis TEXT NOT NULL,"
    " creata_ms INTEGER NOT NULL, expirat_ms INTEGER NOT NULL,"
    " ultima_ms INTEGER NOT NULL)",
    "CREATE INDEX IF NOT EXISTS ianua_sessiones_civis"
    " ON ianua_sessiones (civis)",
    NIHIL
};

/* sessio intrata */
nomen structura {
       Civis civis;
    Sigillum sigillum;
      chorda hex;
} Intratus;


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
    Ianua* ia)
{
    redde ia->o.nunc_ms ? ia->o.nunc_ms() : _tempus_verum_ms();
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

/* chorda forma absens -> vacua */
interior chorda
_forma (
    HospitiumColloquium* col,
     constans character* campus)
{
    chorda c = colloquium_forma(col, campus);

    si (!c.datum)
    {
        c = _ch("");
    }
    redde c;
}

/* chorda in capsam terminatam (FALSUM si longior) */
interior b32
_copiare (
        character* capsa,
              i32  capacitas,
           chorda  s)
{
    si (s.mensura >= capacitas)
    {
        redde FALSUM;
    }
    si (s.mensura > ZEPHYRUM)
    {
        memcpy(capsa, s.datum, (size_t)s.mensura);
    }
    capsa[s.mensura] = '\0';
    redde VERUM;
}

/* EVASIO HTML UNA (copia exemplaris - ianua P9) */
interior vacuum
_evadere (
    ChordaAedificator* a,
               chorda  s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
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

/* via sub praefixo (in piscina) */
interior constans character*
_via (
                  Ianua* ia,
     constans character* suffixum,
                Piscina* p)
{
    character* v = (character*)piscina_allocare(p, (i64)(strlen(
        ia->praefixum) + strlen(suffixum) + I));

    strcpy(v, ia->praefixum);
    strcat(v, suffixum);
    redde v;
}

/* redire tutum: via LOCALIS solum ("/x", numquam "//x" aut "/\\x"),
 * sine signis moderantibus - nulla redirectio aperta */
interior b32
_redire_tutum (
    chorda r)
{
    i32 i;

    si (   !r.datum || r.mensura < I
        || r.mensura         >= IANUA_REDIRE_MAXIMUM
        || r.datum[ZEPHYRUM] != '/')
    {
        redde FALSUM;
    }
    si (r.mensura > I && (r.datum[I] == '/' || r.datum[I] == '\\'))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < r.mensura; i++)
    {
        si (r.datum[i] < 0x20 || r.datum[i] == 0x7F)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_origo_bona (
    HospitiumColloquium* col)
{
    redde custodia_origo_congruit(colloquium_caput(col, "Origin"),
        colloquium_schema(col), colloquium_caput(col, "Host"));
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

/* corpus in paginam: applicationis aut simplex */
interior vacuum
_pagina (
                  Ianua* ia,
    HospitiumColloquium* col,
                    i32  status,
     constans character* titulus,
      ChordaAedificator* corpus)
{
    chorda c = chorda_aedificator_finire(corpus);

    si (ia->o.pagina)
    {
        ia->o.pagina(ia->o.datum, col, status, titulus, c);
        redde;
    }
    {
        ChordaAedificator* a = chorda_aedificator_creare(
            colloquium_piscina(col), MMXLVIII);

        chorda_aedificator_appendere_literis(a, "<!doctype html>\n"
            "<html><head><meta charset=\"utf-8\">"
            "<meta name=\"viewport\" content=\"width=device-width\">"
            "<title>");
        chorda_aedificator_appendere_literis(a, titulus);
        chorda_aedificator_appendere_literis(a,
            "</title></head><body><h1>");
        chorda_aedificator_appendere_literis(a, titulus);
        chorda_aedificator_appendere_literis(a, "</h1>\n");
        chorda_aedificator_appendere_chorda(a, c);
        chorda_aedificator_appendere_literis(a, "</body></html>\n");
        _capita_tuta(col);
        colloquium_respondere(col, status, "text/html; charset=utf-8",
            chorda_aedificator_finire(a));
    }
}

interior ChordaAedificator*
_corpus (
    HospitiumColloquium* col)
{
    redde chorda_aedificator_creare(colloquium_piscina(col), MMXLVIII);
}

interior vacuum
_errare (
                  Ianua* ia,
    HospitiumColloquium* col,
                    i32  status,
     constans character* nuntius)
{
    ChordaAedificator* a = _corpus(col);

    chorda_aedificator_appendere_literis(a, "<p class=\"error\">");
    _evadere(a, _ch(nuntius));
    chorda_aedificator_appendere_literis(a, "</p>");
    _pagina(ia, col, status, "Ianua", a);
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

interior b32
_crustulum_ponere (
    HospitiumColloquium* col,
     constans character* signum,
                    s64  aetas_s)
{
    CrustulumOptiones o;

    memset(&o, ZEPHYRUM, magnitudo(o));
    o.max_aetas_s  = aetas_s;
    o.http_solum   = VERUM;
    o.securum      = chorda_aequalis_literis(colloquium_schema(col),
        "https");
    o.eadem        = CRUSTULUM_EADEM_LAXA;
    redde colloquium_crustulum_ponere(col, "sid", _ch(signum), &o);
}

/* viam ad ?redire= codificare (percent: praeter A-Z a-z 0-9 - _ . ~) */
interior vacuum
_codificare (
     ChordaAedificator* a,
    constans character* s)
{
    hic_manens constans character* HEX = "0123456789ABCDEF";

    dum (*s)
    {
        character c = *s++;

        si (   (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
            || (c >= '0' && c <= '9') || c == '-' || c == '_'
            || c == '.'
            || c == '~')
        {
            chorda_aedificator_appendere_character(a, c);
        }
        alioquin
        {
            chorda_aedificator_appendere_character(a, '%');
            chorda_aedificator_appendere_character(a,
                HEX[((insignatus character)c) >> IV]);
            chorda_aedificator_appendere_character(a,
                HEX[((insignatus character)c) & 0x0F]);
        }
    }
}

/* 303 ad P/intrare?redire=<via> */
interior vacuum
_ad_intrandum (
                  Ianua* ia,
    HospitiumColloquium* col,
     constans character* suffixum)
{
    ChordaAedificator* a = _corpus(col);

    chorda_aedificator_appendere_literis(a, _via(ia, "/intrare?redire=",
        colloquium_piscina(col)));
    _codificare(a, _via(ia, suffixum, colloquium_piscina(col)));
    colloquium_redirigere(col, CCCIII, chorda_ut_cstr(
        chorda_aedificator_finire(a), colloquium_piscina(col)));
}

interior b32
_actum (
                  Ianua* ia,
     constans character* genus,
              JsonValor* d,
                Piscina* p)
{
    redde volumen_actum_appendere(ia->volumen, genus, json_scribere(d,
        p))
        > ZEPHYRUM;
}


/* ==================================================
 * sessiones
 * ================================================== */

interior b32
_intratus (
                  Ianua* ia,
    HospitiumColloquium* col,
               Intratus* exitus)
{
                Piscina* p       = colloquium_piscina(col);
                 chorda  signum  = colloquium_crustulum(col, "sid");
     ScriniumEnuntiatum* e;
              character  hex[SIGILLUM_HEX_MENSURA];
                 chorda  civis;
                    s64  expirat = ZEPHYRUM;
                    b32  inventa = FALSUM;

    si (!signum.datum || signum.mensura != XLIII)
    {
        redde FALSUM;
    }
    exitus->sigillum = custodia_signi_sigillum(signum);
    sigillum_hex(&exitus->sigillum, hex);
    exitus->hex = chorda_ex_literis(hex, p);
    e = scrinium_praeparare(ia->scrinium,
        "SELECT civis, expirat_ms FROM"
        " ianua_sessiones WHERE sigillum = ?");
    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, I, exitus->hex);
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        civis    = scrinium_columna_textus(e, ZEPHYRUM, p);
        expirat  = scrinium_columna_numerus(e, I);
        inventa  = VERUM;
    }
    scrinium_finire(e);
    redde inventa && expirat > _nunc(ia)
        && cives_per_id(ia->cives, civis, &exitus->civis, p)
        && exitus->civis.status == CIVIS_ACTIVUS;
}

/* INTRA TRANSACTIONEM: sessionem delere + actum */
interior b32
_sessionem_delere (
      Ianua* ia,
     chorda  hex,
    Piscina* p)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(ia->scrinium,
        "DELETE FROM ianua_sessiones WHERE sigillum = ?");
             JsonValor* d;
                   b32  bene;

    si (!e)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, I, hex);
    bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
    scrinium_finire(e);
    si (!bene)
    {
        redde FALSUM;
    }
    d = json_objectum_creare(p);
    json_objectum_ponere(d, "sigillum", json_chorda_creare(p, hex));
    redde _actum(ia, "sessio.deleta", d, p);
}

/* sessio nova (transactione propria); signum in effusionem */
interior b32
_sessionem_creare (
      Ianua* ia,
     chorda  civis,
  character* signum,
    Piscina* p)
{
        CustodiaSessio  s;
             character  hex[SIGILLUM_HEX_MENSURA];
    ScriniumEnuntiatum* e;
             JsonValor* d;
                   s64  nunc     = _nunc(ia);
                   s64  expirat  = nunc + ia->o.sessio_vita_s * (s64)M;
                   b32  bene     = FALSUM;

    si (   !custodia_sessionem_creare(&s)
        || !volumen_transactionem_incipere(ia->volumen))
    {
        redde FALSUM;
    }
    sigillum_hex(&s.sigillum, hex);
    e = scrinium_praeparare(ia->scrinium, "INSERT INTO ianua_sessiones"
        " (sigillum, civis, creata_ms, expirat_ms, ultima_ms)"
        " VALUES (?, ?, ?, ?, ?)");
    si (e)
    {
        scrinium_ligare_textum(e, I, _ch(hex));
        scrinium_ligare_textum(e, II, civis);
        scrinium_ligare_numerum(e, III, nunc);
        scrinium_ligare_numerum(e, IV, expirat);
        scrinium_ligare_numerum(e, V, nunc);
        bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
        scrinium_finire(e);
    }
    si (bene)
    {
        d = json_objectum_creare(p);
        /* SIGILLUM signi in actis, numquam signum ipsum */
        json_objectum_ponere(d, "sigillum", json_chorda_creare(p,
            _ch(hex)));
        json_objectum_ponere(d, "civis", json_chorda_creare(p, civis));
        json_objectum_ponere(d, "expirat", json_integer_creare(p,
            expirat));
        bene = _actum(ia, "sessio.creata", d, p);
    }
    si (!bene || !volumen_transactionem_committere(ia->volumen))
    {
        (vacuum)volumen_transactionem_revolvere(ia->volumen);
        memset(&s, ZEPHYRUM, magnitudo(s));
        redde FALSUM;
    }
    strcpy(signum, s.signum);
    memset(&s, ZEPHYRUM, magnitudo(s));
    redde VERUM;
}


/* ==================================================
 * loculi operum
 * ================================================== */

interior IanuaOpus*
_opus_capere (
             Ianua* ia,
    IanuaOpusGenus  genus)
{
    i32 k;

    per (k = ZEPHYRUM; k < IANUA_OPERA; k++)
    {
        si (ia->opera[k].genus == IANUA_OPUS_LIBERUM)
        {
            IanuaOpus* o = &ia->opera[k];

            memset(o, ZEPHYRUM, magnitudo(*o));
            o->ianua = ia;
            o->genus = genus;
            redde o;
        }
    }
    redde NIHIL;
}

interior vacuum
_opus_liberare (
    IanuaOpus* o)
{
    memset(o->codex, ZEPHYRUM, magnitudo(o->codex));
    memset(o->phc, ZEPHYRUM, magnitudo(o->phc));
    o->genus = IANUA_OPUS_LIBERUM;
}


/* ==================================================
 * intrare
 * ================================================== */

interior vacuum
_intrare_get (
    HospitiumColloquium* col)
{
                Ianua* ia      = (Ianua*)colloquium_datum(col);
    ChordaAedificator* a       = _corpus(col);
               chorda  redire  = colloquium_quaestio(col, "redire");

    chorda_aedificator_appendere_literis(a,
        "<form method=\"post\" action=\"");
    _evadere(a, _ch(_via(ia, "/intrare", colloquium_piscina(col))));
    chorda_aedificator_appendere_literis(a, "\">");
    si (_redire_tutum(redire))
    {
        chorda_aedificator_appendere_literis(a,
            "<input type=\"hidden\" name=\"redire\" value=\"");
        _evadere(a, redire);
        chorda_aedificator_appendere_literis(a, "\">");
    }
    chorda_aedificator_appendere_literis(a,
        "<label>nomen <input name=\"nomen\" autocomplete=\"username\""
        " required></label><label>tessera <input type=\"password\""
        " name=\"tessera\" autocomplete=\"current-password\" required>"
        "</label><button>intrare</button></form>");
    _pagina(ia, col, CC, "Intrare", a);
}

interior vacuum
_verificatio_intrandi (
    vacuum* datum,
       b32  bona,
    chorda  civis)
{
               IanuaOpus* o    = (IanuaOpus*)datum;
                   Ianua* ia   = o->ianua;
     HospitiumColloquium* col  = dilatum_colloquium(o->d);
               character  signum[CUSTODIA_SIGNUM_MENSURA];

    si (!col)
    {
        _opus_liberare(o);
        redde;
    }
    si (!bona)
    {
        _errare(ia, col, CDIII, "nomen aut tessera falsa");
        _opus_liberare(o);
        redde;
    }
    si (!_sessionem_creare(ia, civis, signum, colloquium_piscina(col)))
    {
        _errare(ia, col, D, "sessio creari nequit");
        _opus_liberare(o);
        redde;
    }
    (vacuum)_crustulum_ponere(col, signum, ia->o.sessio_vita_s);
    memset(signum, ZEPHYRUM, magnitudo(signum));
    colloquium_redirigere(col, CCCIII, o->redire[ZEPHYRUM] ? o->redire
        : ia->via_post);
    _opus_liberare(o);
}

interior vacuum
_intrare_post (
    HospitiumColloquium* col)
{
                  Ianua* ia  = (Ianua*)colloquium_datum(col);
                Piscina* p   = colloquium_piscina(col);
                 chorda  nomen_usoris;
                 chorda  redire;
              IanuaOpus* o;
    CredentialiaInitium  initium;

    si (!_origo_bona(col))
    {
        _errare(ia, col, CDIII, "origo aliena");
        redde;
    }
    nomen_usoris  = cives_nomen_normare(_forma(col, "nomen"), p);
    o             = _opus_capere(ia, IANUA_OPUS_INTRARE);
    si (!o)
    {
        _errare(ia, col, DIII, "occupatum - iterum tempta");
        redde;
    }
    redire = colloquium_forma(col, "redire");
    si (_redire_tutum(redire))
    {
        (vacuum)_copiare(o->redire, IANUA_REDIRE_MAXIMUM, redire);
    }
    initium = credentialia_verificare(ia->credentialia, nomen_usoris,
        _forma(col, "tessera"), colloquium_cliens(col),
        _verificatio_intrandi,
        o);
    si (initium != CREDENTIALIA_COEPTUM)
    {
        _opus_liberare(o);
        _errare(ia, col, initium == CREDENTIALIA_FRENATUM ? CDXXIX
            : initium == CREDENTIALIA_OCCUPATUM ? DIII : CD,
            initium == CREDENTIALIA_FRENATUM
            ? "conatus nimis multi - postea tempta"
            : initium
                == CREDENTIALIA_OCCUPATUM ? "occupatum - iterum tempta"
            : "petitio mala");
        redde;
    }
    o->d = colloquium_differre(col);
}

interior vacuum
_exire_post (
    HospitiumColloquium* col)
{
       Ianua* ia = (Ianua*)colloquium_datum(col);
    Intratus  i;

    si (!_origo_bona(col))
    {
        _errare(ia, col, CDIII, "origo aliena");
        redde;
    }
    si (_intratus(ia, col, &i))
    {
        si (!custodia_csrf_verificare(ia->custodia, &i.sigillum,
            colloquium_forma(col, "csrf")))
        {
            _errare(ia, col, CDIII, "signum formae deest");
            redde;
        }
        si (volumen_transactionem_incipere(ia->volumen))
        {
            si (_sessionem_delere(ia, i.hex, colloquium_piscina(col)))
            {
                (vacuum)volumen_transactionem_committere(ia->volumen);
            }
            alioquin
            {
                (vacuum)volumen_transactionem_revolvere(ia->volumen);
            }
        }
    }
    (vacuum)_crustulum_ponere(col, "", ZEPHYRUM);
    colloquium_redirigere(col, CCCIII, _via(ia, "/intrare",
        colloquium_piscina(col)));
}


/* ==================================================
 * inscribere
 * ================================================== */

/* forma exitus (intratus alius) */
interior vacuum
_exitus_forma (
                  Ianua* ia,
    HospitiumColloquium* col,
      ChordaAedificator* a,
               Intratus* i)
{
    chorda_aedificator_appendere_literis(a, "<p>intratus es ut <b>");
    _evadere(a, i->civis.nomen_usoris);
    chorda_aedificator_appendere_literis(a, "</b>. Ad rationem novam "
        "creandam prius exi.</p><form method=\"post\" action=\"");
    _evadere(a, _ch(_via(ia, "/exire", colloquium_piscina(col))));
    chorda_aedificator_appendere_literis(a, "\">");
    _csrf_campus(a, custodia_csrf(ia->custodia, &i->sigillum,
        colloquium_piscina(col)));
    chorda_aedificator_appendere_literis(a,
        "<button>exire</button></form>");
}

interior vacuum
_inscribere_get (
    HospitiumColloquium* col)
{
                   Ianua* ia  = (Ianua*)colloquium_datum(col);
                 Piscina* p   = colloquium_piscina(col);
       ChordaAedificator* a   = _corpus(col);
                  chorda  codex = colloquium_quaestio(col,
                      "invitatio");
                Intratus i;
               Invitatio x;

    si (_intratus(ia, col, &i))
    {
        _exitus_forma(ia, col, a, &i);
        _pagina(ia, col, CC, "Inscribere", a);
        redde;
    }
    si (   ia->o.invitatio_necessaria
        && !invitationes_inspicere(ia->invitationes, codex.datum ? codex
               : _ch(""), &x, p))
    {
        chorda_aedificator_appendere_literis(a, "<p class=\"error\">"
            "invitatio non valet (usa, expirata, revocata aut ignota)."
            " Pete invitationem novam ab eo qui te invitavit.</p>");
        _pagina(ia, col, CDIII, "Inscribere", a);
        redde;
    }
    chorda_aedificator_appendere_literis(a,
        "<form method=\"post\" action=\"");
    _evadere(a, _ch(_via(ia, "/inscribere", p)));
    chorda_aedificator_appendere_literis(a, "\">");
    si (codex.datum)
    {
        chorda_aedificator_appendere_literis(a,
            "<input type=\"hidden\" name=\"invitatio\" value=\"");
        _evadere(a, codex);
        chorda_aedificator_appendere_literis(a, "\">");
    }
    chorda_aedificator_appendere_literis(a,
        "<label>nomen usoris (a-z 0-9 . _ -) <input name=\"nomen\""
        " maxlength=\"32\" autocomplete=\"username\" required></label>"
        "<label>nomen ostensum <input name=\"titulus\" maxlength=\"128\">"
        "</label><label>tessera <input type=\"password\" name=\"tessera\""
        " minlength=\"8\" autocomplete=\"new-password\" required></label>"
        "<button>inscribere</button></form>");
    _pagina(ia, col, CC, "Inscribere", a);
}

/* hash factum: transactio una - civis, invitatio, tessera */
interior vacuum
_inscriptio_hash (
    vacuum* datum,
    chorda  phc)
{
               IanuaOpus* o    = (IanuaOpus*)datum;
                   Ianua* ia   = o->ianua;
     HospitiumColloquium* col  = dilatum_colloquium(o->d);
                 Piscina* p;
                  chorda  civis;
                     i32  status      = ZEPHYRUM;
      constans character* nuntius     = NIHIL;
                     b32  transactio  = FALSUM;

    p = piscina_generare_dynamicum("ianua_inscriptio", MXXIV);
    si (!p || phc.mensura == ZEPHYRUM)
    {
        status   = D;
        nuntius  = "error servi";
    }
    alioquin si (!volumen_transactionem_incipere(ia->volumen))
    {
        status   = D;
        nuntius  = "volumen occupatum";
    }
    alioquin
    {
        transactio = VERUM;
        si (!cives_creare(ia->cives, _ch(o->nomen_usoris),
            _ch(o->titulus),
            &civis, p))
        {
            status   = CDIX;
            nuntius  = "nomen iam captum";
        }
        alioquin si (   ia->o.invitatio_necessaria
                     && !invitationes_consumere(ia->invitationes,
                            _ch(o->codex), civis))
        {
            status   = CDIII;
            nuntius  = "invitatio non valet";
        }
        alioquin si (!credentialia_tesseram_ponere(ia->credentialia,
                     civis,
                     phc))
        {
            status   = D;
            nuntius  = "tessera servari nequit";
        }
        alioquin si (!volumen_transactionem_committere(ia->volumen))
        {
            status   = D;
            nuntius  = "volumen scribi nequit";
        }
    }
    /* refutatio quaelibet post initium: nihil manet */
    si (nuntius && transactio)
    {
        (vacuum)volumen_transactionem_revolvere(ia->volumen);
    }
    si (!nuntius && ia->o.post_inscriptionem)
    {
        ia->o.post_inscriptionem(ia->o.datum, civis);
    }
    si (col)
    {
        si (nuntius)
        {
            _errare(ia, col, status, nuntius);
        }
        alioquin
        {
            colloquium_redirigere(col, CCCIII, _via(ia, "/intrare",
                colloquium_piscina(col)));
        }
    }
    si (p)
    {
        piscina_destruere(p);
    }
    _opus_liberare(o);
}

interior vacuum
_inscribere_post (
    HospitiumColloquium* col)
{
        Ianua* ia  = (Ianua*)colloquium_datum(col);
      Piscina* p   = colloquium_piscina(col);
       chorda  nomen_usoris;
       chorda  titulus;
       chorda  codex;
       chorda  tessera;
     Intratus  i;
    Invitatio  x;
        Civis  quis;
    IanuaOpus* o;

    si (!_origo_bona(col))
    {
        _errare(ia, col, CDIII, "origo aliena");
        redde;
    }
    si (_intratus(ia, col, &i))
    {
        _errare(ia, col, CDIX, "iam intratus - prius exi");
        redde;
    }
    nomen_usoris  = cives_nomen_normare(_forma(col, "nomen"), p);
    titulus       = _forma(col, "titulus");
    codex         = _forma(col, "invitatio");
    tessera       = _forma(col, "tessera");
    si (titulus.mensura == ZEPHYRUM)
    {
        titulus = nomen_usoris;
    }
    si (!cives_nomen_validum(nomen_usoris))
    {
        _errare(ia, col, CD, "nomen: I-XXXII signa a-z 0-9 . _ -");
        redde;
    }
    si (titulus.mensura > IANUA_TITULUS_MAXIMUS)
    {
        _errare(ia, col, CD, "nomen ostensum nimis longum");
        redde;
    }
    si (!custodia_tessera_valida(tessera))
    {
        _errare(ia, col, CD, "tessera: VIII-MXXIV octeti");
        redde;
    }
    si (   ia->o.invitatio_necessaria
        && !invitationes_inspicere(ia->invitationes, codex, &x, p))
    {
        _errare(ia, col, CDIII, "invitatio non valet");
        redde;
    }
    si (cives_per_nomen(ia->cives, nomen_usoris, &quis, p))
    {
        _errare(ia, col, CDIX, "nomen iam captum");
        redde;
    }
    o = _opus_capere(ia, IANUA_OPUS_INSCRIBERE);
    si (!o)
    {
        _errare(ia, col, DIII, "occupatum - iterum tempta");
        redde;
    }
    (vacuum)_copiare(o->nomen_usoris, magnitudo(o->nomen_usoris),
        nomen_usoris);
    (vacuum)_copiare(o->titulus, magnitudo(o->titulus), titulus);
    (vacuum)_copiare(o->codex, magnitudo(o->codex), codex);
    o->d = colloquium_differre(col);
    si (!credentialia_digerere(ia->credentialia, tessera,
        _inscriptio_hash, o))
    {
        HospitiumColloquium* c = dilatum_colloquium(o->d);

        _opus_liberare(o);
        si (c)
        {
            _errare(ia, c, DIII, "occupatum - iterum tempta");
        }
    }
}


/* ==================================================
 * tessera mutata
 * ================================================== */

interior vacuum
_tessera_get (
    HospitiumColloquium* col)
{
                Ianua* ia = (Ianua*)colloquium_datum(col);
    ChordaAedificator* a;
             Intratus  i;

    si (!_intratus(ia, col, &i))
    {
        _ad_intrandum(ia, col, "/tessera");
        redde;
    }
    a = _corpus(col);
    chorda_aedificator_appendere_literis(a,
        "<form method=\"post\" action=\"");
    _evadere(a, _ch(_via(ia, "/tessera", colloquium_piscina(col))));
    chorda_aedificator_appendere_literis(a, "\">");
    _csrf_campus(a, custodia_csrf(ia->custodia, &i.sigillum,
        colloquium_piscina(col)));
    chorda_aedificator_appendere_literis(a,
        "<label>tessera actualis <input type=\"password\""
        " name=\"actualis\" autocomplete=\"current-password\" required>"
        "</label><label>tessera nova <input type=\"password\" name=\"nova\""
        " minlength=\"8\" autocomplete=\"new-password\" required></label>"
        "<button>mutare</button></form>");
    _pagina(ia, col, CC, "Tessera", a);
}

/* ambo opera finita: tessera ponitur, sessiones aliae necantur */
interior vacuum
_tessera_finire (
    IanuaOpus* o)
{
                    Ianua* ia   = o->ianua;
      HospitiumColloquium* col  = dilatum_colloquium(o->d);
                  Piscina* p;
       ScriniumEnuntiatum* e;
                      Xar* aliae;
                      i32  k;
                      b32  bene = FALSUM;

    si (!o->verificatio_facta || !o->hash_factum)
    {
        redde;
    }
    si (!o->bona)
    {
        si (col)
        {
            _errare(ia, col, CDIII, "tessera actualis falsa");
        }
        _opus_liberare(o);
        redde;
    }
    si (o->phc[ZEPHYRUM] == '\0')
    {
        si (col)
        {
            _errare(ia, col, DIII, "occupatum - iterum tempta");
        }
        _opus_liberare(o);
        redde;
    }
    p      = piscina_generare_dynamicum("ianua_tessera", MMXLVIII);
    aliae  = p ? xar_creare(p, (i32)magnitudo(chorda)) : NIHIL;
    e      = aliae ? scrinium_praeparare(ia->scrinium, "SELECT sigillum"
        " FROM ianua_sessiones WHERE civis = ? AND sigillum != ?") : NIHIL;
    si (e)
    {
        scrinium_ligare_textum(e, I, _ch(o->civis));
        scrinium_ligare_textum(e, II, _ch(o->sigillum));
        dum (scrinium_gradi(e) == SCRINIUM_ORDO)
        {
            chorda* s = (chorda*)xar_addere(aliae);

            si (s)
            {
                *s = scrinium_columna_textus(e, ZEPHYRUM, p);
            }
        }
        scrinium_finire(e);
        si (volumen_transactionem_incipere(ia->volumen))
        {
            bene = credentialia_tesseram_ponere(ia->credentialia,
                _ch(o->civis), _ch(o->phc));
            per (k = ZEPHYRUM; bene && k < xar_numerus(aliae); k++)
            {
                bene = _sessionem_delere(ia,
                    *(chorda*)xar_obtinere(aliae,
                    k), p);
            }
            si (bene && volumen_transactionem_committere(ia->volumen))
            {
                bene = VERUM;
            }
            alioquin
            {
                (vacuum)volumen_transactionem_revolvere(ia->volumen);
                bene = FALSUM;
            }
        }
    }
    si (col)
    {
        si (bene)
        {
            ChordaAedificator* a = _corpus(col);

            chorda_aedificator_appendere_literis(a, "<p>tessera mutata."
                " Sessiones aliae exierunt.</p>");
            _pagina(ia, col, CC, "Tessera", a);
        }
        alioquin
        {
            _errare(ia, col, D, "tessera servari nequit");
        }
    }
    si (p)
    {
        piscina_destruere(p);
    }
    _opus_liberare(o);
}

interior vacuum
_tessera_verificata (
    vacuum* datum,
       b32  bona,
    chorda  civis)
{
    IanuaOpus* o = (IanuaOpus*)datum;

    /* bona solum si persona eadem quae intrata est */
    o->bona = bona
        && chorda_aequalis(civis, _ch(o->civis));
    o->verificatio_facta = VERUM;
    _tessera_finire(o);
}

interior vacuum
_tessera_digesta (
    vacuum* datum,
    chorda  phc)
{
    IanuaOpus* o = (IanuaOpus*)datum;

    (vacuum)_copiare(o->phc, PHC_MAXIMUM, phc);
    o->hash_factum = VERUM;
    _tessera_finire(o);
}

interior vacuum
_tessera_post (
    HospitiumColloquium* col)
{
                  Ianua* ia = (Ianua*)colloquium_datum(col);
                  Civis  civis;
                 chorda  nova;
               Intratus  i;
              IanuaOpus* o;
    CredentialiaInitium  initium;

    si (!ianua_custos_postis(ia, col, &civis))
    {
        _errare(ia, col, CDIII,
            "intrare necesse aut signum formae deest");
        redde;
    }
    nova = _forma(col, "nova");
    si (!custodia_tessera_valida(nova))
    {
        _errare(ia, col, CD, "tessera nova: VIII-MXXIV octeti");
        redde;
    }
    si (!_intratus(ia, col, &i))
    {
        _errare(ia, col, CDIII, "intrare necesse");
        redde;
    }
    o = _opus_capere(ia, IANUA_OPUS_TESSERA);
    si (!o)
    {
        _errare(ia, col, DIII, "occupatum - iterum tempta");
        redde;
    }
    (vacuum)_copiare(o->civis, magnitudo(o->civis), civis.id);
    (vacuum)_copiare(o->sigillum, magnitudo(o->sigillum), i.hex);
    initium = credentialia_verificare(ia->credentialia,
        civis.nomen_usoris,
        _forma(col, "actualis"), colloquium_cliens(col),
        _tessera_verificata, o);
    si (initium != CREDENTIALIA_COEPTUM)
    {
        _opus_liberare(o);
        _errare(ia, col, initium == CREDENTIALIA_FRENATUM ? CDXXIX
            : initium == CREDENTIALIA_OCCUPATUM ? DIII : CD,
            initium == CREDENTIALIA_FRENATUM
            ? "conatus nimis multi - postea tempta"
            : initium
                == CREDENTIALIA_OCCUPATUM ? "occupatum - iterum tempta"
            : "petitio mala");
        redde;
    }
    o->d = colloquium_differre(col);
    si (!credentialia_digerere(ia->credentialia, nova, _tessera_digesta,
        o))
    {
        /* verificatio currit; finis cum phc vacua -> 503 */
        o->hash_factum = VERUM;
    }
}


/* ==================================================
 * invitationes
 * ================================================== */

interior b32
_potest_invitare (
    Ianua* ia,
    Civis* civis)
{
    si (ia->o.potest_invitare)
    {
        redde ia->o.potest_invitare(ia->o.datum, civis);
    }
    redde civis->status == CIVIS_ACTIVUS;
}

interior vacuum
_momentum (
    character* exitus,
          s64  ms)
{
    time_t t         = (time_t)(ms / M);
    structura tm* g  = gmtime(&t);

    si (!g)
    {
        strcpy(exitus, "?");
        redde;
    }
    sprintf(exitus, "%04d-%02d-%02d %02d:%02d UTC", g->tm_year + MCM,
        g->tm_mon + I, g->tm_mday, g->tm_hour, g->tm_min);
}

interior vacuum
_invitationes_get (
    HospitiumColloquium* col)
{
                Ianua* ia  = (Ianua*)colloquium_datum(col);
              Piscina* p   = colloquium_piscina(col);
    ChordaAedificator* a;
             Intratus  i;
               chorda  csrf;
                  Xar* l;
                  i32  k;
                  s64  nunc = _nunc(ia);

    si (!_intratus(ia, col, &i))
    {
        _ad_intrandum(ia, col, "/invitationes");
        redde;
    }
    si (!_potest_invitare(ia, &i.civis))
    {
        _errare(ia, col, CDIII, "invitare non licet");
        redde;
    }
    csrf  = custodia_csrf(ia->custodia, &i.sigillum, p);
    a     = _corpus(col);
    chorda_aedificator_appendere_literis(a,
        "<form method=\"post\" action=\"");
    _evadere(a, _ch(_via(ia, "/invitationes", p)));
    chorda_aedificator_appendere_literis(a, "\">");
    _csrf_campus(a, csrf);
    chorda_aedificator_appendere_literis(a,
        "<label>nota (pro quo?) <input name=\"nota\" maxlength=\"128\">"
        "</label><label>dies validitatis <input name=\"dies\" value=\"7\""
        " size=\"3\"></label><button>invitationem creare</button></form>");
    l = invitationes_enumerare(ia->invitationes, i.civis.id, p);
    per (k = ZEPHYRUM; l && k < xar_numerus(l); k++)
    {
        Invitatio* x = (Invitatio*)xar_obtinere(l, k);
        character  m[LXIV];

        chorda_aedificator_appendere_literis(a,
            "<div class=\"nota\"><p><b>");
        _evadere(a, x->nota.mensura
            > ZEPHYRUM ? x->nota : _ch("(sine nota)"));
        chorda_aedificator_appendere_literis(a,
            "</b> &middot; creata ");
        _momentum(m, x->creata_ms);
        chorda_aedificator_appendere_literis(a, m);
        chorda_aedificator_appendere_literis(a, " &middot; ");
        si (x->revocata_ms > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a, "revocata");
        }
        alioquin si (x->usus_numerus >= x->usus_max)
        {
            Civis u;

            chorda_aedificator_appendere_literis(a, "usa a ");
            _evadere(a, cives_per_id(ia->cives, x->usa_a, &u, p)
                ? u.nomen_usoris : x->usa_a);
        }
        alioquin si (x->expirat_ms <= nunc)
        {
            chorda_aedificator_appendere_literis(a, "expirata");
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(a, "non usa &middot; "
                "expirat ");
            _momentum(m, x->expirat_ms);
            chorda_aedificator_appendere_literis(a, m);
        }
        chorda_aedificator_appendere_literis(a, "</p>");
        si (x->revocata_ms == ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a,
                "<form method=\"post\" action=\"");
            _evadere(a, _ch(_via(ia, "/invitationes/", p)));
            _evadere(a, x->id);
            chorda_aedificator_appendere_literis(a, "/revocare\">");
            _csrf_campus(a, csrf);
            chorda_aedificator_appendere_literis(a,
                "<button>revocare</button></form>");
        }
        chorda_aedificator_appendere_literis(a, "</div>");
    }
    _pagina(ia, col, CC, "Invitationes", a);
}

interior vacuum
_invitationes_post (
    HospitiumColloquium* col)
{
                Ianua* ia  = (Ianua*)colloquium_datum(col);
              Piscina* p   = colloquium_piscina(col);
                Civis  civis;
               chorda  nota;
               chorda  dies_ch;
               chorda  codex;
               chorda  id;
            character  dies_l[XII];
                  s64  dies = VII;
                  b32  bene = FALSUM;
    ChordaAedificator* a;

    si (!ianua_custos_postis(ia, col, &civis))
    {
        _errare(ia, col, CDIII,
            "intrare necesse aut signum formae deest");
        redde;
    }
    si (!_potest_invitare(ia, &civis))
    {
        _errare(ia, col, CDIII, "invitare non licet");
        redde;
    }
    nota     = _forma(col, "nota");
    dies_ch  = _forma(col, "dies");
    si (nota.mensura > IANUA_TITULUS_MAXIMUS)
    {
        _errare(ia, col, CD, "nota nimis longa");
        redde;
    }
    si (dies_ch.mensura > ZEPHYRUM && dies_ch.mensura < XI)
    {
        (vacuum)_copiare(dies_l, XII, dies_ch);
        dies = (s64)atol(dies_l);
        si (dies < I || dies > DIES_MAXIMI)
        {
            _errare(ia, col, CD, "dies: I-XC");
            redde;
        }
    }
    si (volumen_transactionem_incipere(ia->volumen))
    {
        bene = invitationes_creare(ia->invitationes, civis.id, nota,
            dies
            * DIES_MS, ZEPHYRUM, &codex, &id, p)
            && volumen_transactionem_committere(ia->volumen);
        si (!bene)
        {
            (vacuum)volumen_transactionem_revolvere(ia->volumen);
        }
    }
    si (!bene)
    {
        _errare(ia, col, D, "invitatio creari nequit");
        redde;
    }
    a = _corpus(col);
    chorda_aedificator_appendere_literis(a,
        "<p>Invitatio creata. Nexum "
        "mitte - <b>semel</b> ostenditur:</p><p><a href=\"");
    chorda_aedificator_appendere_chorda(a, colloquium_schema(col));
    chorda_aedificator_appendere_literis(a, "://");
    _evadere(a, colloquium_caput(col, "Host").datum ? colloquium_caput(
        col, "Host") : _ch("localhost"));
    _evadere(a, _ch(_via(ia, "/inscribere?invitatio=", p)));
    _evadere(a, codex);
    chorda_aedificator_appendere_literis(a,
        "\">nexus invitationis</a></p>"
        "<p>codex: <code>");
    _evadere(a, codex);
    chorda_aedificator_appendere_literis(a, "</code></p><p><a href=\"");
    _evadere(a, _ch(_via(ia, "/invitationes", p)));
    chorda_aedificator_appendere_literis(a, "\">invitationes</a></p>");
    colloquium_caput_addere(col, "Cache-Control", "no-store");
    _pagina(ia, col, CC, "Invitatio", a);
}

interior vacuum
_invitationem_revocare (
    HospitiumColloquium* col)
{
    Ianua* ia = (Ianua*)colloquium_datum(col);
    Civis  civis;
      b32  bene = FALSUM;

    si (!ianua_custos_postis(ia, col, &civis))
    {
        _errare(ia, col, CDIII,
            "intrare necesse aut signum formae deest");
        redde;
    }
    si (volumen_transactionem_incipere(ia->volumen))
    {
        bene = invitationes_revocare(ia->invitationes, civis.id,
            colloquium_param(col, "id"))
            && volumen_transactionem_committere(ia->volumen);
        si (!bene)
        {
            (vacuum)volumen_transactionem_revolvere(ia->volumen);
        }
    }
    si (!bene)
    {
        _errare(ia, col, CDIV, "invitatio ignota");
        redde;
    }
    colloquium_redirigere(col, CCCIII, _via(ia, "/invitationes",
        colloquium_piscina(col)));
}


/* ==================================================
 * API
 * ================================================== */

Ianua*
ianua_creare (
                   Volumen* volumen,
                  Custodia* custodia,
                   Reactor* reactor,
    constans IanuaOptiones* optiones,
                   Piscina* piscina)
{
    Ianua* ia;
      i32  k;

    si (!volumen || !custodia || !reactor || !optiones || !piscina)
    {
        redde NIHIL;
    }
    ia = (Ianua*)piscina_allocare(piscina, (i64)magnitudo(Ianua));
    si (!ia)
    {
        redde NIHIL;
    }
    memset(ia, ZEPHYRUM, magnitudo(*ia));
    ia->volumen   = volumen;
    ia->scrinium  = volumen_scrinium(volumen);
    ia->custodia  = custodia;
    ia->reactor   = reactor;
    ia->piscina   = piscina;
    ia->o         = *optiones;
    si (ia->o.sessio_vita_s <= ZEPHYRUM)
    {
        ia->o.sessio_vita_s = IANUA_SESSIO_VITA_S;
    }
    si (   (optiones->praefixum && strlen(optiones->praefixum)
            >= PRAEFIXUM_MAXIMUM)
        || (optiones->via_post_intrationem
            && strlen(optiones->via_post_intrationem)
            >= IANUA_REDIRE_MAXIMUM))
    {
        redde NIHIL;
    }
    strcpy(ia->praefixum,
        optiones->praefixum ? optiones->praefixum : "");
    strcpy(ia->via_post, optiones->via_post_intrationem
        ? optiones->via_post_intrationem : "/");
    ia->o.praefixum = ia->praefixum;
    ia->o.via_post_intrationem = ia->via_post;
    ia->cives = cives_aperire(volumen, optiones->nunc_ms, piscina);
    ia->credentialia = ia->cives ? credentialia_aperire(volumen,
        ia->cives,
        custodia, reactor, optiones->nunc_ms, piscina) : NIHIL;
    ia->invitationes = invitationes_aperire(volumen, optiones->nunc_ms,
        piscina);
    si (!ia->credentialia || !ia->invitationes || !ia->scrinium)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; SCHEMA[k]; k++)
    {
        si (!scrinium_exsequi(ia->scrinium, SCHEMA[k]))
        {
            redde NIHIL;
        }
    }
    redde ia;
}

b32
ianua_montare (
        Ianua* ianua,
    Hospitium* hospitium)
{
    b32 bene = VERUM;

    si (!ianua || !hospitium)
    {
        redde FALSUM;
    }
    bene = bene && hospitium_praebere(hospitium, HTTP_GET, _via(ianua,
        "/intrare", ianua->piscina), _intrare_get, ianua);
    bene = bene && hospitium_praebere(hospitium, HTTP_POST, _via(ianua,
        "/intrare", ianua->piscina), _intrare_post, ianua);
    bene = bene && hospitium_praebere(hospitium, HTTP_POST, _via(ianua,
        "/exire", ianua->piscina), _exire_post, ianua);
    bene = bene && hospitium_praebere(hospitium, HTTP_GET, _via(ianua,
        "/inscribere", ianua->piscina), _inscribere_get, ianua);
    bene = bene && hospitium_praebere(hospitium, HTTP_POST, _via(ianua,
        "/inscribere", ianua->piscina), _inscribere_post, ianua);
    bene = bene && hospitium_praebere(hospitium, HTTP_GET, _via(ianua,
        "/tessera", ianua->piscina), _tessera_get, ianua);
    bene = bene && hospitium_praebere(hospitium, HTTP_POST, _via(ianua,
        "/tessera", ianua->piscina), _tessera_post, ianua);
    bene = bene && hospitium_praebere(hospitium, HTTP_GET, _via(ianua,
        "/invitationes", ianua->piscina), _invitationes_get, ianua);
    bene = bene && hospitium_praebere(hospitium, HTTP_POST, _via(ianua,
        "/invitationes", ianua->piscina), _invitationes_post, ianua);
    bene = bene && hospitium_praebere(hospitium, HTTP_POST, _via(ianua,
        "/invitationes/:id/revocare", ianua->piscina),
        _invitationem_revocare, ianua);
    redde bene;
}

Cives*
ianua_cives (
    Ianua* ianua)
{
    redde ianua ? ianua->cives : NIHIL;
}

Credentialia*
ianua_credentialia (
    Ianua* ianua)
{
    redde ianua ? ianua->credentialia : NIHIL;
}

Invitationes*
ianua_invitationes (
    Ianua* ianua)
{
    redde ianua ? ianua->invitationes : NIHIL;
}

b32
ianua_intratus (
                  Ianua* ianua,
    HospitiumColloquium* col,
                  Civis* civis,
                 chorda* csrf)
{
    Intratus i;

    si (!ianua || !col || !_intratus(ianua, col, &i))
    {
        redde FALSUM;
    }
    si (civis)
    {
        *civis = i.civis;
    }
    si (csrf)
    {
        *csrf = custodia_csrf(ianua->custodia, &i.sigillum,
            colloquium_piscina(col));
    }
    redde VERUM;
}

b32
ianua_custos_postis (
                  Ianua* ianua,
    HospitiumColloquium* col,
                  Civis* civis)
{
    Intratus i;

    si (   !ianua || !col || !_origo_bona(col)
        || !_intratus(ianua, col, &i)
        || !custodia_csrf_verificare(ianua->custodia, &i.sigillum,
               colloquium_forma(col, "csrf")))
    {
        redde FALSUM;
    }
    si (civis)
    {
        *civis = i.civis;
    }
    redde VERUM;
}

b32
ianua_admissionis_intratus (
                 vacuum* ianua,
    HospitiumColloquium* col,
                 chorda* usor,
                 chorda* csrf)
{
    Civis civis;

    si (!usor || !ianua_intratus((Ianua*)ianua, col, &civis, csrf))
    {
        redde FALSUM;
    }
    *usor = civis.id;
    redde VERUM;
}
