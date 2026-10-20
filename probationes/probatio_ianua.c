/* probatio_ianua.c - ianua (ianua T4): ianua rationum in hospitio -
 * inscriptio per invitationem (transactio una: invitatio, civis,
 * tessera), certamen nominis (revolutio), invitatio mortua (forma
 * nulla), alius intratus, intrare/exire, nomen maiusculum, redire
 * tutum, Origo et CSRF, tessera mutata (sessiones aliae necantur),
 * pagina invitationum (codex SEMEL), civis suspensus, adapter
 * admissionis, nulla secreta in actis.
 *
 * Mundus: hospitium nudum + reactor + custodia (argon2 vilis) +
 * volumen temporarium; cliens HTTP ex probatio_exemplar_applicatio.c
 * adaptatus (Hospitium* pro Exemplar*).
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "tcp.h"
#include "reactor.h"
#include "hospitium.h"
#include "custodia.h"
#include "volumen.h"
#include "xar.h"
#include "cives.h"
#include "invitationes.h"
#include "ianua.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPITA_N  (VIII * MXXIV)
#define FORMA_U   "Content-Type: application/x-www-form-urlencoded"
#define TESSERA   "tessera-bona"
#define POST_INTR "/domus"


/* ==================================================
 * cliens HTTP
 * ================================================== */

nomen structura {
    TcpConnexio* c;
             i8* data;
            s64  n;
            s64  capacitas;
            s64  finis_capitum;
            s64  corporis;
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
      Piscina* p)
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
    l->capacitas      = CCLVI * MXXIV;
    l->data           = (i8*)piscina_allocare(p, (i64)l->capacitas + I);
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
        s32 k = tcp_mittere(l->c, d + missa, (i32)(n - missa));

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

    per (k = ZEPHYRUM; k < C * M; k++)
    {
        _pumpare(h, XX);
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

/* petitio missa, responsum NONDUM lectum */
interior b32
_rogare (
              Hospitium* h,
                Piscina* p,
    constans character* linea,
    constans character* capita_extra,
    constans character* corpus,
                 Lector* l)
{
    character* capita  = (character*)piscina_allocare(p, CAPITA_N);
          s64  n       = corpus ? (s64)strlen(corpus) : ZEPHYRUM;

    si (!_aperire(h, l, p))
    {
        redde FALSUM;
    }
    sprintf(capita, "%s HTTP/1.1\r\nHost: 127.0.0.1:%u\r\n"
        "Connection: close\r\n%sContent-Length: %ld\r\n\r\n", linea,
        (insignatus integer)hospitium_portus(h), capita_extra
        ? capita_extra : "", (longus)n);
    si (   !_mittere(h, l, (constans i8*)capita, (s64)strlen(capita))
        || (n > ZEPHYRUM && !_mittere(h, l, (constans i8*)corpus, n)))
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
    b32 bene = _rogare(h, p, linea, capita_extra, corpus, l)
        && _exspectare(h, l);

    _claudere(l);
    redde bene;
}

interior b32
_continet (
                Lector* l,
    constans character* s)
{
    redde l->finitum
        && strstr((constans character*)l->data, s) != NIHIL;
}

/* valor post praefixum usque ad terminum (in piscina), aut "" */
interior constans character*
_extrahere (
                 Lector* l,
     constans character* praefixum,
              character  terminus,
                Piscina* p)
{
    constans character* a = strstr((constans character*)l->data,
                                praefixum);
    constans character* b;
             character* v;

    si (!a)
    {
        redde "";
    }
    a += strlen(praefixum);
    b = strchr(a, terminus);
    si (!b)
    {
        redde "";
    }
    v = (character*)piscina_allocare(p, (i64)(b - a) + I);
    memcpy(v, a, (size_t)(b - a));
    v[b - a] = '\0';
    redde v;
}


/* ==================================================
 * mundus
 * ================================================== */

nomen structura {
      Reactor* r;
     Custodia* cu;
      Volumen* v;
    Hospitium* h;
        Ianua* ia;
    character  origo[LXIV];
          i32  inscripti;
} Mundus;

hic_manens constans Argon2idParametri VILES = { I, LXIV, I, XXXII,
    NIHIL, ZEPHYRUM, NIHIL, ZEPHYRUM };
/* tardiores: certamen inscriptionum (hash per multos gyros) */
hic_manens constans Argon2idParametri MEDII = { I, IV * MXXIV, I, XXXII,
    NIHIL, ZEPHYRUM, NIHIL, ZEPHYRUM };

interior vacuum
_post_inscriptionem (
    vacuum* datum,
    chorda  civis)
{
    Mundus* m = (Mundus*)datum;

    (vacuum)civis;
    m->inscripti++;
}

/* via probationis: adapter admissionis -> "usor=<id> csrf=<signum>" */
interior vacuum
_persona (
    HospitiumColloquium* col)
{
       Mundus* m = (Mundus*)colloquium_datum(col);
       chorda  usor;
       chorda  csrf;
       chorda  corpus;
    character* s;

    si (!ianua_admissionis_intratus(m->ia, col, &usor, &csrf))
    {
        colloquium_respondere(col, CDI, "text/plain", chorda_ex_literis(
            "nemo", colloquium_piscina(col)));
        redde;
    }
    s = (character*)piscina_allocare(colloquium_piscina(col), CCLVI);
    sprintf(s, "usor=%.*s csrf=%.*s|", (integer)usor.mensura,
        (constans character*)usor.datum, (integer)csrf.mensura,
        (constans character*)csrf.datum);
    corpus = chorda_ex_literis(s, colloquium_piscina(col));
    colloquium_respondere(col, CC, "text/plain", corpus);
}

interior b32
_mundus (
    CredoContextus* c,
            Mundus* m,
               b32  medii)
{
     CustodiaConfiguratio cfg_custodiae;
    HospitiumConfiguratio cfg_hospitii;
            IanuaOptiones o;

    memset(m, ZEPHYRUM, magnitudo(*m));
    m->r = reactor_creare(c->piscina);
    memset(&cfg_custodiae, ZEPHYRUM, magnitudo(cfg_custodiae));
    memset(cfg_custodiae.secretum, IX, CUSTODIA_SECRETUM);
    cfg_custodiae.tessera_parametri = medii ? MEDII : VILES;
    m->cu = m->r ? custodia_creare(&cfg_custodiae, m->r,
        c->piscina) : NIHIL;
    m->v = volumen_temporarium(c->piscina, "ianua");
    memset(&cfg_hospitii, ZEPHYRUM, magnitudo(cfg_hospitii));
    cfg_hospitii.reactor = m->r;
    m->h = m->cu
        && m->v ? hospitium_creare(c->piscina, &cfg_hospitii) : NIHIL;
    memset(&o, ZEPHYRUM, magnitudo(o));
    o.via_post_intrationem  = POST_INTR;
    o.invitatio_necessaria  = VERUM;
    o.post_inscriptionem    = _post_inscriptionem;
    o.datum                 = m;
    m->ia = m->h ? ianua_creare(m->v, m->cu, m->r, &o,
        c->piscina) : NIHIL;
    si (!m->ia || !ianua_montare(m->ia, m->h))
    {
        redde FALSUM;
    }
    (vacuum)hospitium_praebere(m->h, HTTP_GET, "/probatio/persona",
        _persona, m);
    sprintf(m->origo, "Origin: http://127.0.0.1:%u\r\n",
        (insignatus integer)hospitium_portus(m->h));
    redde VERUM;
}

interior vacuum
_mundum_claudere (
    Mundus* m)
{
    si (m->h)
    {
        hospitium_destruere(m->h);
    }
    si (m->v)
    {
        volumen_claudere(m->v);
    }
    si (m->r)
    {
        reactor_destruere(m->r);
    }
}

/* capita: Origin (nisi sine_origine), Cookie, Content-Type forma */
interior constans character*
_capita (
                 Mundus* m,
                Piscina* p,
     constans character* sid,
                    b32  forma)
{
    character* s = (character*)piscina_allocare(p, CCCLXXXIV);

    sprintf(s, "%s%s%s%s%s", m->origo, sid ? "Cookie: sid=" : "",
        sid ? sid : "", sid ? "\r\n" : "", forma ? FORMA_U "\r\n" : "");
    redde s;
}

/* invitatio directe (transactione propria); codex in piscina */
interior constans character*
_invitare (
                 Mundus* m,
     constans character* auctor,
                Piscina* p)
{
    chorda codex;
    chorda id;

    si (!volumen_transactionem_incipere(m->v))
    {
        redde "";
    }
    si (!invitationes_creare(ianua_invitationes(m->ia),
        chorda_ex_literis(
        auctor, p), chorda_ex_literis("", p), ZEPHYRUM, ZEPHYRUM,
        &codex,
        &id, p))
    {
        (vacuum)volumen_transactionem_revolvere(m->v);
        redde "";
    }
    (vacuum)volumen_transactionem_committere(m->v);
    redde chorda_ut_cstr(codex, p);
}

interior i32
_inscribere (
                 Mundus* m,
                Piscina* p,
     constans character* nomen_usoris,
     constans character* tessera,
     constans character* codex,
     constans character* sid)
{
       Lector l;
    character corpus[CCCLXXXIV];

    sprintf(corpus, "nomen=%s&titulus=%s&tessera=%s&invitatio=%s",
        nomen_usoris, nomen_usoris, tessera, codex);
    si (!_rogare_et_legere(m->h, p, "POST /inscribere", _capita(m, p,
        sid,
        VERUM), corpus, &l))
    {
        redde ZEPHYRUM;
    }
    redde l.status;
}

/* sid aut ""; *status */
interior constans character*
_intrare (
                 Mundus* m,
                Piscina* p,
     constans character* nomen_usoris,
     constans character* tessera,
                    i32* status)
{
       Lector l;
    character corpus[CCLVI];

    sprintf(corpus, "nomen=%s&tessera=%s", nomen_usoris, tessera);
    *status = ZEPHYRUM;
    si (!_rogare_et_legere(m->h, p, "POST /intrare", _capita(m, p,
        NIHIL,
        VERUM), corpus, &l))
    {
        redde "";
    }
    *status = l.status;
    redde _extrahere(&l, "Set-Cookie: sid=", ';', p);
}

/* civis novus per invitationem et inscriptionem; sid post intrationem */
interior constans character*
_civis (
                 Mundus* m,
                Piscina* p,
     constans character* nomen_usoris)
{
    i32 status;

    si (_inscribere(m, p, nomen_usoris, TESSERA, _invitare(m, "", p),
        NIHIL) != CCCIII)
    {
        redde "";
    }
    redde _intrare(m, p, nomen_usoris, TESSERA, &status);
}

/* signum CSRF paginae /tessera */
interior constans character*
_csrf (
                 Mundus* m,
                Piscina* p,
     constans character* sid)
{
    Lector l;

    si (   !_rogare_et_legere(m->h, p, "GET /tessera", _capita(m, p,
        sid,
        FALSUM), NIHIL, &l) || l.status != CC)
    {
        redde "";
    }
    redde _extrahere(&l, "name=\"csrf\" value=\"", '"', p);
}

/* status GET /tessera (CC intratus, CCCIII non) */
interior i32
_tessera_get (
                 Mundus* m,
                Piscina* p,
     constans character* sid)
{
    Lector l;

    si (!_rogare_et_legere(m->h, p, "GET /tessera", _capita(m, p, sid,
        FALSUM), NIHIL, &l))
    {
        redde ZEPHYRUM;
    }
    redde l.status;
}

interior chorda
_c (
    constans character* s,
               Piscina* p)
{
    redde chorda_ex_literis(s, p);
}

interior b32
_invitatio_valida (
                 Mundus* m,
     constans character* codex,
                Piscina* p)
{
    Invitatio x;

    redde invitationes_inspicere(ianua_invitationes(m->ia), _c(codex,
        p),
        &x, p);
}


/* ==================================================
 * sectiones
 * ================================================== */

interior vacuum
_sectio_inscriptio (CredoContextus* c)
{
               Piscina* p = c->piscina;
                Mundus  m;
                Lector  l;
             character  via[CXXVIII];
    constans character* codex;
                 Civis  x;

    CREDO_NECESSE(_mundus(c, &m, FALSUM));
    codex = _invitare(&m, "", p);
    sprintf(via, "GET /inscribere?invitatio=%s", codex);
    CREDO_VERUM(_rogare_et_legere(m.h, p, via, _capita(&m, p, NIHIL,
        FALSUM), NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "type=\"password\""));
    CREDO_VERUM(_continet(&l, codex));
    CREDO_AEQUALIS_I32(_inscribere(&m, p, "fran", TESSERA, codex,
        NIHIL),
        CCCIII);
    CREDO_VERUM(cives_per_nomen(ianua_cives(m.ia), _c("fran", p), &x,
        p));
    CREDO_FALSUM(_invitatio_valida(&m, codex, p));
    CREDO_AEQUALIS_I32(m.inscripti, I);
    /* codex usus iterum: refutatur */
    CREDO_AEQUALIS_I32(_inscribere(&m, p, "amanda", TESSERA, codex,
        NIHIL), CDIII);
    /* sine codice: refutatur */
    CREDO_AEQUALIS_I32(_inscribere(&m, p, "amanda", TESSERA, "", NIHIL),
        CDIII);
    /* nomen aut tessera mala */
    CREDO_AEQUALIS_I32(_inscribere(&m, p, "a b", TESSERA, _invitare(&m,
        "", p), NIHIL), CD);
    CREDO_AEQUALIS_I32(_inscribere(&m, p, "amanda", "brevis", _invitare(
        &m, "", p), NIHIL), CD);
    CREDO_AEQUALIS_I32(m.inscripti, I);
    _mundum_claudere(&m);
}

interior vacuum
_sectio_inscriptio_atomica (CredoContextus* c)
{
               Piscina* p = c->piscina;
                Mundus  m;
                Lector  a;
                Lector  b;
             character  corpus_a[CCLVI];
             character  corpus_b[CCLVI];
    constans character* codex_a;
    constans character* codex_b;
                   i32  status_a;
                   i32  status_b;
                 Civis  x;

    CREDO_NECESSE(_mundus(c, &m, VERUM));
    codex_a = _invitare(&m, "", p);
    codex_b = _invitare(&m, "", p);
    sprintf(corpus_a, "nomen=fran&titulus=A&tessera=%s&invitatio=%s",
        TESSERA, codex_a);
    sprintf(corpus_b, "nomen=fran&titulus=B&tessera=%s&invitatio=%s",
        TESSERA, codex_b);
    /* ambae missae ante quam hash ullum finitur */
    CREDO_NECESSE(_rogare(m.h, p, "POST /inscribere", _capita(&m, p,
        NIHIL, VERUM), corpus_a, &a));
    CREDO_NECESSE(_rogare(m.h, p, "POST /inscribere", _capita(&m, p,
        NIHIL, VERUM), corpus_b, &b));
    CREDO_VERUM(_exspectare(m.h, &a));
    CREDO_VERUM(_exspectare(m.h, &b));
    status_a = a.status;
    status_b = b.status;
    _claudere(&a);
    _claudere(&b);
    CREDO_VERUM(   (status_a == CCCIII && status_b == CDIX)
                || (status_a == CDIX && status_b == CCCIII));
    /* victor: invitatio usa; victus: invitatio intacta */
    CREDO_VERUM(_invitatio_valida(&m, status_a == CDIX ? codex_a
        : codex_b, p));
    CREDO_FALSUM(_invitatio_valida(&m, status_a == CCCIII ? codex_a
        : codex_b, p));
    CREDO_AEQUALIS_I32(m.inscripti, I);
    /* via celeris: nomen iam captum ante hash */
    CREDO_AEQUALIS_I32(_inscribere(&m, p, "fran", TESSERA, _invitare(&m,
        "", p), NIHIL), CDIX);
    /* codex IDEM, nomina diversa: ambo civem creant, victus invitationem
     * consumere nequit -> revolutio: civis eius NON manet */
    codex_a = _invitare(&m, "", p);
    sprintf(corpus_a, "nomen=amanda&titulus=A&tessera=%s&invitatio=%s",
        TESSERA, codex_a);
    sprintf(corpus_b, "nomen=marcus&titulus=B&tessera=%s&invitatio=%s",
        TESSERA, codex_a);
    CREDO_NECESSE(_rogare(m.h, p, "POST /inscribere", _capita(&m, p,
        NIHIL, VERUM), corpus_a, &a));
    CREDO_NECESSE(_rogare(m.h, p, "POST /inscribere", _capita(&m, p,
        NIHIL, VERUM), corpus_b, &b));
    CREDO_VERUM(_exspectare(m.h, &a));
    CREDO_VERUM(_exspectare(m.h, &b));
    status_a = a.status;
    status_b = b.status;
    _claudere(&a);
    _claudere(&b);
    CREDO_VERUM(   (status_a == CCCIII && status_b == CDIII)
                || (status_a == CDIII && status_b == CCCIII));
    CREDO_AEQUALIS_I32((i32)cives_per_nomen(ianua_cives(m.ia),
        _c("amanda",
        p), &x, p) + (i32)cives_per_nomen(ianua_cives(m.ia),
        _c("marcus",
        p), &x, p), I);
    CREDO_AEQUALIS_I32(m.inscripti, II);
    _mundum_claudere(&m);
}

interior vacuum
_sectio_invitatio_mortua (CredoContextus* c)
{
               Piscina* p = c->piscina;
                Mundus  m;
                Lector  l;
             character  via[CXXVIII];
    constans character* usa;
    constans character* mortuae[III];
                   i32  k;

    CREDO_NECESSE(_mundus(c, &m, FALSUM));
    usa = _invitare(&m, "", p);
    CREDO_AEQUALIS_I32(_inscribere(&m, p, "fran", TESSERA, usa, NIHIL),
        CCCIII);
    mortuae[ZEPHYRUM]  = usa;
    mortuae[I]         = "AAAAAAAAAAAAAAAAAAAAAA";
    mortuae[II]        = "";
    per (k = ZEPHYRUM; k < III; k++)
    {
        sprintf(via, "GET /inscribere?invitatio=%s", mortuae[k]);
        CREDO_VERUM(_rogare_et_legere(m.h, p, via, _capita(&m, p, NIHIL,
            FALSUM), NIHIL, &l));
        CREDO_AEQUALIS_I32(l.status, CDIII);
        CREDO_VERUM(_continet(&l, "invitatio non valet"));
        CREDO_FALSUM(_continet(&l, "type=\"password\""));
    }
    CREDO_VERUM(_rogare_et_legere(m.h, p, "GET /inscribere", _capita(&m,
        p, NIHIL, FALSUM), NIHIL, &l));
    CREDO_FALSUM(_continet(&l, "type=\"password\""));
    _mundum_claudere(&m);
}

interior vacuum
_sectio_alius_intratus (CredoContextus* c)
{
               Piscina* p = c->piscina;
                Mundus  m;
                Lector  l;
             character  via[CXXVIII];
    constans character* sid;
    constans character* codex;

    CREDO_NECESSE(_mundus(c, &m, FALSUM));
    sid    = _civis(&m, p, "fran");
    CREDO_NECESSE(sid[ZEPHYRUM] != '\0');
    codex  = _invitare(&m, "", p);
    sprintf(via, "GET /inscribere?invitatio=%s", codex);
    CREDO_VERUM(_rogare_et_legere(m.h, p, via, _capita(&m, p, sid,
        FALSUM), NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "fran"));
    CREDO_VERUM(_continet(&l, "action=\"/exire\""));
    CREDO_FALSUM(_continet(&l, "type=\"password\""));
    CREDO_AEQUALIS_I32(_inscribere(&m, p, "amanda", TESSERA, codex,
        sid),
        CDIX);
    CREDO_VERUM(_invitatio_valida(&m, codex, p));
    _mundum_claudere(&m);
}

interior vacuum
_sectio_intrare_exire (CredoContextus* c)
{
               Piscina* p = c->piscina;
                Mundus  m;
                Lector  l;
             character  corpus[CXXVIII];
    constans character* sid;
                   i32  status;

    CREDO_NECESSE(_mundus(c, &m, FALSUM));
    sid = _civis(&m, p, "fran");
    CREDO_NECESSE(sid[ZEPHYRUM] != '\0');
    CREDO_AEQUALIS_I32((i32)strlen(sid), XLIII);
    CREDO_AEQUALIS_I32(_tessera_get(&m, p, sid), CC);
    /* intratio iterata: Location via_post_intrationem */
    CREDO_VERUM(_rogare_et_legere(m.h, p, "POST /intrare", _capita(&m,
        p,
        NIHIL, VERUM), "nomen=fran&tessera=" TESSERA, &l));
    CREDO_AEQUALIS_I32(l.status, CCCIII);
    CREDO_VERUM(_continet(&l, "Location: " POST_INTR "\r\n"));
    /* tessera falsa, ignotus */
    (vacuum)_intrare(&m, p, "fran", "tessera-mala", &status);
    CREDO_AEQUALIS_I32(status, CDIII);
    (vacuum)_intrare(&m, p, "nemo", TESSERA, &status);
    CREDO_AEQUALIS_I32(status, CDIII);
    /* exire */
    sprintf(corpus, "csrf=%s", _csrf(&m, p, sid));
    CREDO_VERUM(_rogare_et_legere(m.h, p, "POST /exire", _capita(&m, p,
        sid, VERUM), corpus, &l));
    CREDO_AEQUALIS_I32(l.status, CCCIII);
    CREDO_AEQUALIS_I32(_tessera_get(&m, p, sid), CCCIII);
    _mundum_claudere(&m);
}

interior vacuum
_sectio_nomen_maiusculum (CredoContextus* c)
{
               Piscina* p = c->piscina;
                Mundus  m;
                Lector  l;
             character  cap[CXXVIII];
    constans character* sid;
                 Civis  x;
                   i32  status;

    CREDO_NECESSE(_mundus(c, &m, FALSUM));
    CREDO_NECESSE(_civis(&m, p, "fran")[ZEPHYRUM] != '\0');
    sid = _intrare(&m, p, "FRAN", TESSERA, &status);
    CREDO_AEQUALIS_I32(status, CCCIII);
    CREDO_NECESSE(sid[ZEPHYRUM] != '\0');
    CREDO_NECESSE(cives_per_nomen(ianua_cives(m.ia), _c("fran", p), &x,
        p));
    sprintf(cap, "Cookie: sid=%s\r\n", sid);
    CREDO_VERUM(_rogare_et_legere(m.h, p, "GET /probatio/persona", cap,
        NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    sprintf(cap, "usor=%.*s ", (integer)x.id.mensura,
        (constans character*)x.id.datum);
    CREDO_VERUM(_continet(&l, cap));
    _mundum_claudere(&m);
}

interior vacuum
_sectio_redire (CredoContextus* c)
{
               Piscina* p = c->piscina;
                Mundus  m;
                Lector  l;
    constans character* malae[III];
             character  corpus[CCLVI];
                   i32  k;

    CREDO_NECESSE(_mundus(c, &m, FALSUM));
    CREDO_NECESSE(_civis(&m, p, "fran")[ZEPHYRUM] != '\0');
    CREDO_VERUM(_rogare_et_legere(m.h, p,
        "GET /intrare?redire=%2Fnotae%3Fx%3D1", NIHIL, NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "name=\"redire\" value=\"/notae?x=1\""));
    CREDO_VERUM(_rogare_et_legere(m.h, p, "POST /intrare", _capita(&m,
        p,
        NIHIL, VERUM), "nomen=fran&tessera=" TESSERA
        "&redire=%2Fnotae%3Fx%3D1", &l));
    CREDO_AEQUALIS_I32(l.status, CCCIII);
    CREDO_VERUM(_continet(&l, "Location: /notae?x=1\r\n"));
    malae[ZEPHYRUM]  = "%2F%2Fmalus.example";
    malae[I]         = "%2F%5Cmalus.example";
    malae[II]        = "http%3A%2F%2Fmalus.example";
    per (k = ZEPHYRUM; k < III; k++)
    {
        sprintf(corpus, "nomen=fran&tessera=" TESSERA "&redire=%s",
            malae[k]);
        CREDO_VERUM(_rogare_et_legere(m.h, p, "POST /intrare",
            _capita(&m,
            p, NIHIL, VERUM), corpus, &l));
        CREDO_AEQUALIS_I32(l.status, CCCIII);
        CREDO_VERUM(_continet(&l, "Location: " POST_INTR "\r\n"));
    }
    _mundum_claudere(&m);
}

interior vacuum
_sectio_origo_csrf (CredoContextus* c)
{
               Piscina* p = c->piscina;
                Mundus  m;
                Lector  l;
             character  cap[CCLVI];
    constans character* sid;

    CREDO_NECESSE(_mundus(c, &m, FALSUM));
    sid = _civis(&m, p, "fran");
    CREDO_NECESSE(sid[ZEPHYRUM] != '\0');
    /* sine Origine */
    CREDO_VERUM(_rogare_et_legere(m.h, p, "POST /intrare",
        FORMA_U "\r\n",
        "nomen=fran&tessera=" TESSERA, &l));
    CREDO_AEQUALIS_I32(l.status, CDIII);
    /* Origo aliena */
    CREDO_VERUM(_rogare_et_legere(m.h, p, "POST /inscribere",
        "Origin: http://malus.example\r\n" FORMA_U "\r\n",
        "nomen=x&tessera=" TESSERA "&invitatio=x", &l));
    CREDO_AEQUALIS_I32(l.status, CDIII);
    /* sine CSRF (sessione praesente) */
    sprintf(cap, "%sCookie: sid=%s\r\n" FORMA_U "\r\n", m.origo, sid);
    CREDO_VERUM(_rogare_et_legere(m.h, p, "POST /exire", cap, "x=1",
        &l));
    CREDO_AEQUALIS_I32(l.status, CDIII);
    CREDO_VERUM(_rogare_et_legere(m.h, p, "POST /tessera", cap,
        "actualis=" TESSERA "&nova=tessera-nova", &l));
    CREDO_AEQUALIS_I32(l.status, CDIII);
    CREDO_VERUM(_rogare_et_legere(m.h, p, "POST /invitationes", cap,
        "nota=x", &l));
    CREDO_AEQUALIS_I32(l.status, CDIII);
    /* sessio adhuc vivit (exire sine CSRF nihil fecit) */
    CREDO_AEQUALIS_I32(_tessera_get(&m, p, sid), CC);
    _mundum_claudere(&m);
}

interior vacuum
_sectio_tessera_mutata (CredoContextus* c)
{
               Piscina* p = c->piscina;
                Mundus  m;
                Lector  l;
             character  corpus[CCLVI];
    constans character* sid_a;
    constans character* sid_b;
                   i32  status;

    CREDO_NECESSE(_mundus(c, &m, FALSUM));
    sid_a = _civis(&m, p, "fran");
    sid_b = _intrare(&m, p, "fran", TESSERA, &status);
    CREDO_NECESSE(sid_a[ZEPHYRUM] != '\0' && sid_b[ZEPHYRUM] != '\0');
    /* tessera actualis falsa */
    sprintf(corpus, "csrf=%s&actualis=mala-tessera&nova=tessera-nova",
        _csrf(&m, p, sid_a));
    CREDO_VERUM(_rogare_et_legere(m.h, p, "POST /tessera", _capita(&m,
        p,
        sid_a, VERUM), corpus, &l));
    CREDO_AEQUALIS_I32(l.status, CDIII);
    CREDO_AEQUALIS_I32(_tessera_get(&m, p, sid_b), CC);
    /* nova brevis */
    sprintf(corpus, "csrf=%s&actualis=" TESSERA "&nova=brevis",
        _csrf(&m, p, sid_a));
    CREDO_VERUM(_rogare_et_legere(m.h, p, "POST /tessera", _capita(&m,
        p,
        sid_a, VERUM), corpus, &l));
    CREDO_AEQUALIS_I32(l.status, CD);
    /* recta */
    sprintf(corpus, "csrf=%s&actualis=" TESSERA "&nova=tessera-nova",
        _csrf(&m, p, sid_a));
    CREDO_VERUM(_rogare_et_legere(m.h, p, "POST /tessera", _capita(&m,
        p,
        sid_a, VERUM), corpus, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "tessera mutata"));
    CREDO_AEQUALIS_I32(_tessera_get(&m, p, sid_a), CC);
    CREDO_AEQUALIS_I32(_tessera_get(&m, p, sid_b), CCCIII);
    (vacuum)_intrare(&m, p, "fran", TESSERA, &status);
    CREDO_AEQUALIS_I32(status, CDIII);
    (vacuum)_intrare(&m, p, "fran", "tessera-nova", &status);
    CREDO_AEQUALIS_I32(status, CCCIII);
    _mundum_claudere(&m);
}

interior vacuum
_sectio_invitationes_pagina (CredoContextus* c)
{
               Piscina* p = c->piscina;
                Mundus  m;
                Lector  l;
             character  corpus[CCLVI];
             character  via[CCLVI];
    constans character* sid;
    constans character* sid_amandae;
    constans character* codex;
    constans character* id;
                   i32  status;

    CREDO_NECESSE(_mundus(c, &m, FALSUM));
    sid = _civis(&m, p, "fran");
    CREDO_NECESSE(sid[ZEPHYRUM] != '\0');
    sprintf(corpus, "csrf=%s&nota=pro+Amanda&dies=7", _csrf(&m, p,
        sid));
    CREDO_VERUM(_rogare_et_legere(m.h, p, "POST /invitationes", _capita(
        &m, p, sid, VERUM), corpus, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "Cache-Control: no-store"));
    codex = _extrahere(&l, "/inscribere?invitatio=", '"', p);
    CREDO_AEQUALIS_I32((i32)strlen(codex), INVITATIONES_CODEX_MENSURA);
    CREDO_VERUM(_invitatio_valida(&m, codex, p));
    /* pagina iterum: nota et status, codex numquam */
    CREDO_VERUM(_rogare_et_legere(m.h, p, "GET /invitationes", _capita(
        &m, p, sid, FALSUM), NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    CREDO_VERUM(_continet(&l, "pro Amanda"));
    CREDO_VERUM(_continet(&l, "non usa"));
    CREDO_FALSUM(_continet(&l, codex));
    id = _extrahere(&l, "action=\"/invitationes/", '/', p);
    CREDO_AEQUALIS_I32((i32)strlen(id), XXVI);
    /* amanda utitur */
    CREDO_AEQUALIS_I32(_inscribere(&m, p, "amanda", TESSERA, codex,
        NIHIL), CCCIII);
    CREDO_VERUM(_rogare_et_legere(m.h, p, "GET /invitationes", _capita(
        &m, p, sid, FALSUM), NIHIL, &l));
    CREDO_VERUM(_continet(&l, "usa a amanda"));
    /* revocare: amanda non potest, fran potest */
    sid_amandae = _intrare(&m, p, "amanda", TESSERA, &status);
    CREDO_NECESSE(sid_amandae[ZEPHYRUM] != '\0');
    sprintf(via, "POST /invitationes/%s/revocare", id);
    sprintf(corpus, "csrf=%s", _csrf(&m, p, sid_amandae));
    CREDO_VERUM(_rogare_et_legere(m.h, p, via, _capita(&m, p,
        sid_amandae,
        VERUM), corpus, &l));
    CREDO_AEQUALIS_I32(l.status, CDIV);
    sprintf(corpus, "csrf=%s", _csrf(&m, p, sid));
    CREDO_VERUM(_rogare_et_legere(m.h, p, via, _capita(&m, p, sid,
        VERUM), corpus, &l));
    CREDO_AEQUALIS_I32(l.status, CCCIII);
    CREDO_VERUM(_rogare_et_legere(m.h, p, "GET /invitationes", _capita(
        &m, p, sid, FALSUM), NIHIL, &l));
    CREDO_VERUM(_continet(&l, "revocata"));
    _mundum_claudere(&m);
}

interior vacuum
_sectio_suspensus (CredoContextus* c)
{
               Piscina* p = c->piscina;
                Mundus  m;
    constans character* sid;
                 Civis  x;
                   i32  status;

    CREDO_NECESSE(_mundus(c, &m, FALSUM));
    sid = _civis(&m, p, "fran");
    CREDO_NECESSE(sid[ZEPHYRUM] != '\0');
    CREDO_AEQUALIS_I32(_tessera_get(&m, p, sid), CC);
    CREDO_NECESSE(cives_per_nomen(ianua_cives(m.ia), _c("fran", p), &x,
        p));
    CREDO_NECESSE(volumen_transactionem_incipere(m.v));
    CREDO_NECESSE(cives_statum_ponere(ianua_cives(m.ia), x.id,
        CIVIS_SUSPENSUS));
    CREDO_NECESSE(volumen_transactionem_committere(m.v));
    CREDO_AEQUALIS_I32(_tessera_get(&m, p, sid), CCCIII);
    (vacuum)_intrare(&m, p, "fran", TESSERA, &status);
    CREDO_AEQUALIS_I32(status, CDIII);
    _mundum_claudere(&m);
}

interior vacuum
_sectio_admissio_adapter (CredoContextus* c)
{
               Piscina* p = c->piscina;
                Mundus  m;
                Lector  l;
             character  cap[CXXVIII];
             character  exspectatum[CCLVI];
    constans character* sid;
    constans character* csrf;
                 Civis  x;

    CREDO_NECESSE(_mundus(c, &m, FALSUM));
    sid = _civis(&m, p, "fran");
    CREDO_NECESSE(sid[ZEPHYRUM] != '\0');
    csrf = _csrf(&m, p, sid);
    CREDO_AEQUALIS_I32((i32)strlen(csrf), XLIII);
    CREDO_NECESSE(cives_per_nomen(ianua_cives(m.ia), _c("fran", p), &x,
        p));
    sprintf(cap, "Cookie: sid=%s\r\n", sid);
    CREDO_VERUM(_rogare_et_legere(m.h, p, "GET /probatio/persona", cap,
        NIHIL, &l));
    sprintf(exspectatum, "usor=%.*s csrf=%s|", (integer)x.id.mensura,
        (constans character*)x.id.datum, csrf);
    CREDO_VERUM(_continet(&l, exspectatum));
    /* sine sessione */
    CREDO_VERUM(_rogare_et_legere(m.h, p, "GET /probatio/persona",
        NIHIL,
        NIHIL, &l));
    CREDO_AEQUALIS_I32(l.status, CDI);
    _mundum_claudere(&m);
}

interior vacuum
_sectio_sine_secretis (CredoContextus* c)
{
               Piscina* p = c->piscina;
                Mundus  m;
                Lector  l;
             character  corpus[CCLVI];
    constans character* sid;
    constans character* codex;
                   Xar* acta;
                   i32  k;

    CREDO_NECESSE(_mundus(c, &m, FALSUM));
    sid = _civis(&m, p, "fran");
    CREDO_NECESSE(sid[ZEPHYRUM] != '\0');
    sprintf(corpus, "csrf=%s&nota=x&dies=1", _csrf(&m, p, sid));
    CREDO_VERUM(_rogare_et_legere(m.h, p, "POST /invitationes", _capita(
        &m, p, sid, VERUM), corpus, &l));
    codex = _extrahere(&l, "/inscribere?invitatio=", '"', p);
    CREDO_AEQUALIS_I32(_inscribere(&m, p, "amanda", TESSERA, codex,
        NIHIL), CCCIII);
    sprintf(corpus, "csrf=%s&actualis=" TESSERA "&nova=tessera-nova",
        _csrf(&m, p, sid));
    CREDO_VERUM(_rogare_et_legere(m.h, p, "POST /tessera", _capita(&m,
        p,
        sid, VERUM), corpus, &l));
    CREDO_AEQUALIS_I32(l.status, CC);
    acta = volumen_acta_legere(m.v, ZEPHYRUM, p);
    CREDO_NECESSE_NON_NIHIL(acta);
    CREDO_MAIOR_I32(xar_numerus(acta), V);
    per (k = ZEPHYRUM; k < xar_numerus(acta); k++)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(acta, k);

        CREDO_FALSUM(chorda_continet(a->datum, _c(sid, p)));
        CREDO_FALSUM(chorda_continet(a->datum, _c(codex, p)));
        CREDO_FALSUM(chorda_continet(a->datum, _c("$argon2", p)));
        CREDO_FALSUM(chorda_continet(a->datum, _c(TESSERA, p)));
        CREDO_FALSUM(chorda_continet(a->datum, _c("tessera-nova", p)));
    }
    _mundum_claudere(&m);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "inscriptio", _sectio_inscriptio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "inscriptio_atomica", _sectio_inscriptio_atomica, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "invitatio_mortua", _sectio_invitatio_mortua, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "alius_intratus", _sectio_alius_intratus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "intrare_exire", _sectio_intrare_exire, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "nomen_maiusculum", _sectio_nomen_maiusculum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "redire", _sectio_redire, NIHIL, NIHIL, NIHIL, NIHIL },
    { "origo_csrf", _sectio_origo_csrf, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tessera_mutata", _sectio_tessera_mutata, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "invitationes_pagina", _sectio_invitationes_pagina, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "suspensus", _sectio_suspensus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "admissio_adapter", _sectio_admissio_adapter, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "sine_secretis", _sectio_sine_secretis, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("ianua", SECTIONES);
}
