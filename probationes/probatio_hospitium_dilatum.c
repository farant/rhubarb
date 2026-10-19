/* probatio_hospitium_dilatum.c - hospitium v3 T2: responsum dilatum
 * (labor reactoris respondet; terminus 503; generatio custodit locum
 * reusum), reactor communis, fontes corporis (filum magnum per
 * partes, piscina connexionis parva; clientes alii interim serviti;
 * directorium per fontem), sistere molliter, lector octetorum fili.
 *
 * Exemplar probatio_hospitium, sed mora receptionis clientis BREVIS
 * (XX ms): cliens et servus in eodem filo alternant - mora longa
 * gressus servi suspenderet (termini responsi et sistendi falso
 * praeterirent). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "filum.h"
#include "tcp.h"
#include "reactor.h"
#include "hospitium.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define MAGNUM       (VI * MXXIV * MXXIV)
#define APEX_MAXIMUS (CCLVI * MXXIV)

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior vacuum
_pumpare (
    Hospitium* h,
          i32  vices)
{
    i32 i;

    per (i = 0; i < vices; i++)
    {
        (vacuum)hospitium_gressus(h, X);
    }
}

interior TcpConnexio*
_cliens (
    Piscina* p,
        i32  portus)
{
    TcpOptiones o = tcp_optiones_default();
    TcpResultus r;

    o.timeout_ms = XX;
    r = tcp_connectere_cum_optionibus("127.0.0.1", portus, &o, p);
    redde r.successus ? r.connexio : NIHIL;
}

interior b32
_mittere (
           TcpConnexio* c,
    constans character* petitio)
{
    redde tcp_mittere_omnia(c, (constans i8*)petitio,
        (i32)strlen(petitio));
}

/* responsum totum (capita + corpus Content-Length) legere, pumpando;
 * reddit longitudinem corporis, -I si fractum; capita in 'capita' */
interior s64
_legere_responsum (
       Hospitium* h,
     TcpConnexio* c,
       character* capita,
              i32 capita_n,
              i8* corpus,
              s64 corpus_n,
              i32 vices_maximae)
{
     i8 capsa[XVI * MXXIV];
    i32 accepta   = ZEPHYRUM;
    s64 corporis  = -I;
    s64 lecta     = ZEPHYRUM;
    i32 k;
    s64 initium_corporis = ZEPHYRUM;

    capita[ZEPHYRUM] = '\0';
    per (k = ZEPHYRUM; k < vices_maximae; k++)
    {
        s32 n;

        _pumpare(h, I);
        n = tcp_recipere(c, capsa, (i32)magnitudo(capsa));
        si (n == TCP_ITERUM)
        {
            perge;
        }
        si (n <= ZEPHYRUM)
        {
            frange;
        }
        si (corporis < ZEPHYRUM)
        {
            constans character* finis;
                           i32  m = (i32)n;

            si (accepta + m >= capita_n)
            {
                m = capita_n - accepta - I;
            }
            memcpy(capita + accepta, capsa, (size_t)m);
            accepta          += m;
            capita[accepta]  = '\0';
            finis            = strstr(capita, "\r\n\r\n");
            si (finis != NIHIL)
            {
                constans character* cl = strstr(capita,
                    "Content-Length: ");
                i32 longitudo_capitum = (i32)(finis - capita) + IV;
                s32 reliqua = (s32)n - (s32)(longitudo_capitum
                    - (accepta
                    - m));

                corporis = cl != NIHIL ? (s64)atol(cl + XVI) : ZEPHYRUM;
                initium_corporis = (s64)((s32)n - reliqua);
                si (reliqua > ZEPHYRUM && corpus != NIHIL)
                {
                    memcpy(corpus, capsa + initium_corporis,
                        (size_t)reliqua);
                }
                lecta                      = reliqua;
                capita[longitudo_capitum]  = '\0';
            }
        }
        alioquin
        {
            si (corpus != NIHIL && lecta + n <= corpus_n)
            {
                memcpy(corpus + lecta, capsa, (size_t)n);
            }
            lecta += n;
        }
        si (corporis >= ZEPHYRUM && lecta >= corporis)
        {
            frange;
        }
    }
    redde corporis >= ZEPHYRUM && lecta == corporis ? lecta : -I;
}

/* ---- contextus tractatorum ---- */

nomen structura {
       HospitiumDilatum  d;
              Hospitium* h;
                    i32  gradus;
                    i32  gradus_finis;
     constans character* responsum;
                Piscina* piscina_connexionis;
                Piscina* h_piscina;      /* chordae responsi */
     constans character* via_fili;
} Contextus;

interior b32
_labor_respondere (
    vacuum* datum)
{
    Contextus* x = (Contextus*)datum;

    x->gradus++;
    si (x->gradus < x->gradus_finis)
    {
        redde FALSUM;
    }
    {
        HospitiumColloquium* col = dilatum_colloquium(x->d);

        si (col != NIHIL)
        {
            colloquium_respondere(col, CC, "text/plain",
                chorda_ex_literis(x->responsum, x->h_piscina));
        }
    }
    redde VERUM;
}

interior vacuum
_tardum (
    HospitiumColloquium* col)
{
    Contextus* x = (Contextus*)colloquium_datum(col);

    x->d       = colloquium_differre(col);
    x->gradus  = ZEPHYRUM;
    si (x->gradus_finis > ZEPHYRUM)
    {
        CREDO_VERUM(reactor_laborem_adicere(hospitium_reactor(x->h),
            _labor_respondere, x));
    }
}

interior vacuum
_parvum (
    HospitiumColloquium* col)
{
    colloquium_respondere(col, CC, "text/plain", chorda_ex_literis(
        "parvum", colloquium_piscina(col)));
}

interior vacuum
_magnum (
    HospitiumColloquium* col)
{
    Contextus* x = (Contextus*)colloquium_datum(col);

    x->piscina_connexionis = colloquium_piscina(col);
    colloquium_respondere_filum(col, CC, "application/octet-stream",
        x->via_fili);
}

interior Hospitium*
_servum_creare (CredoContextus* c, Reactor* r, i32 responsum_ms)
{
    HospitiumConfiguratio  cfg;
                Hospitium* h;

    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.reactor               = r;
    cfg.responsum_maximum_ms  = (MoraAngusta)responsum_ms;
    h                         = hospitium_creare(c->piscina, &cfg);
    CREDO_NECESSE_NON_NIHIL(h);
    redde h;
}

interior constans character*
_filum_magnum (CredoContextus* c)
{
     character* via  = (character*)piscina_allocare(c->piscina, DXII);
            i8* d    = (i8*)piscina_allocare(c->piscina, MAGNUM);
           s64  k;

    CREDO_NECESSE(via != NIHIL && d != NIHIL);
    sprintf(via, "%s/magnum.bin", credo_area(c));
    per (k = ZEPHYRUM; k < MAGNUM; k++)
    {
        d[k] = (i8)((k * XXXI + k / MXXIV) & 0xff);
    }
    CREDO_NECESSE(filum_scribere(via, chorda_ex_buffer(d,
        (i32)MAGNUM)));
    redde via;
}

interior b32
_aequale_magno (
    constans i8* d,
            s64  n)
{
    s64 k;

    si (n != MAGNUM)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (d[k] != (i8)((k * XXXI + k / MXXIV) & 0xff))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* ---- sectiones ---- */

interior vacuum
_sectio_dilatum (CredoContextus* c)
{
      Hospitium* h = _servum_creare(c, NIHIL, ZEPHYRUM);
      Contextus  x;
    TcpConnexio* a;
    TcpConnexio* b;
      character  capita[MXXIV];
             i8  corpus[LXIV];

    memset(&x, ZEPHYRUM, magnitudo(x));
    x.h             = h;
    x.h_piscina     = c->piscina;
    x.gradus_finis  = III;
    x.responsum     = "tardum";
    CREDO_VERUM(hospitium_praebere(h, HTTP_GET, "/tardum", _tardum,
        &x));
    CREDO_VERUM(hospitium_praebere(h, HTTP_GET, "/parvum", _parvum,
        NIHIL));
    a = _cliens(c->piscina, hospitium_portus(h));
    b = _cliens(c->piscina, hospitium_portus(h));
    CREDO_NECESSE(a != NIHIL && b != NIHIL);
    CREDO_VERUM(_mittere(a, "GET /tardum HTTP/1.1\r\nHost: x\r\n\r\n"));
    _pumpare(h, I);
    CREDO_VERUM(_mittere(b, "GET /parvum HTTP/1.1\r\nHost: x\r\n\r\n"));
    /* b prius servitur quam labor a finit */
    CREDO_AEQUALIS_S32((s32)_legere_responsum(h, b, capita, MXXIV,
        corpus,
        LXIV, X), (s32)VI);
    CREDO_VERUM(x.gradus < III);
    CREDO_AEQUALIS_S32((s32)_legere_responsum(h, a, capita, MXXIV,
        corpus,
        LXIV, XX), (s32)VI);
    CREDO_VERUM(memcmp(corpus, "tardum", VI) == ZEPHYRUM);
    CREDO_VERUM(strstr(capita, "200") != NIHIL);
    tcp_claudere(a);
    tcp_claudere(b);
    hospitium_destruere(h);
}

/* nemo respondet: 503 post terminum; respondere sero nihil agit */
interior vacuum
_sectio_expiratum (CredoContextus* c)
{
      Hospitium* h = _servum_creare(c, NIHIL, CC);
      Contextus  x;
    TcpConnexio* a;
      character  capita[MXXIV];
            s64  initium;

    memset(&x, ZEPHYRUM, magnitudo(x));
    x.h             = h;
    x.h_piscina     = c->piscina;
    x.gradus_finis  = ZEPHYRUM;   /* nullus labor */
    CREDO_VERUM(hospitium_praebere(h, HTTP_GET, "/mutum", _tardum, &x));
    a = _cliens(c->piscina, hospitium_portus(h));
    CREDO_NECESSE_NON_NIHIL(a);
    initium = _ms_nunc();
    CREDO_VERUM(_mittere(a, "GET /mutum HTTP/1.1\r\nHost: x\r\n\r\n"));
    CREDO_AEQUALIS_S32((s32)_legere_responsum(h, a, capita, MXXIV,
        NIHIL,
        ZEPHYRUM, CC), ZEPHYRUM);
    CREDO_VERUM(strstr(capita, "503") != NIHIL);
    CREDO_VERUM(_ms_nunc() - initium < MD);
    CREDO_FALSUM(dilatum_valet(x.d));
    CREDO_NIHIL(dilatum_colloquium(x.d));   /* sero: nihil, nulla ruina */
    tcp_claudere(a);
    hospitium_destruere(h);
}

/* cliens abit dum responsum differtur: locus liberatur; cliens novus
 * locum eundem capit; responsum vetus ad eum NON pervenit */
interior vacuum
_sectio_generatio (CredoContextus* c)
{
      Hospitium* h = _servum_creare(c, NIHIL, ZEPHYRUM);
      Contextus  x;
      Contextus  y;
    TcpConnexio* a;
    TcpConnexio* b;
      character  capita[MXXIV];
             i8  corpus[LXIV];

    memset(&x, ZEPHYRUM, magnitudo(x));
    x.h             = h;
    x.h_piscina     = c->piscina;
    x.gradus_finis  = ZEPHYRUM;
    x.responsum     = "vetus";
    memset(&y, ZEPHYRUM, magnitudo(y));
    y.h             = h;
    y.h_piscina     = c->piscina;
    y.gradus_finis  = ZEPHYRUM;
    CREDO_VERUM(hospitium_praebere(h, HTTP_GET, "/tardum", _tardum,
        &x));
    CREDO_VERUM(hospitium_praebere(h, HTTP_GET, "/alterum", _tardum,
        &y));
    a = _cliens(c->piscina, hospitium_portus(h));
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_VERUM(_mittere(a, "GET /tardum HTTP/1.1\r\nHost: x\r\n\r\n"));
    _pumpare(h, III);
    CREDO_VERUM(dilatum_valet(x.d));
    tcp_claudere(a);
    _pumpare(h, V);
    CREDO_FALSUM(dilatum_valet(x.d));
    b = _cliens(c->piscina, hospitium_portus(h));
    CREDO_NECESSE_NON_NIHIL(b);
    CREDO_VERUM(_mittere(b,
        "GET /alterum HTTP/1.1\r\nHost: x\r\n\r\n"));
    _pumpare(h, III);
    /* b quoque differtur in LOCO EODEM: solum generatio manubrium
     * vetus a novo distinguit */
    CREDO_AEQUALIS_I32(y.d.locus, x.d.locus);
    CREDO_VERUM(dilatum_valet(y.d));
    CREDO_FALSUM(dilatum_valet(x.d));
    CREDO_NIHIL(dilatum_colloquium(x.d));
    colloquium_respondere(dilatum_colloquium(y.d), CC, "text/plain",
        chorda_ex_literis("novum", c->piscina));
    CREDO_AEQUALIS_S32((s32)_legere_responsum(h, b, capita, MXXIV,
        corpus,
        LXIV, X), (s32)V);
    CREDO_VERUM(memcmp(corpus, "novum", V) == ZEPHYRUM);
    tcp_claudere(b);
    hospitium_destruere(h);
}

nomen structura {
    i32 timer;
    i32 labor;
} Signa;

interior vacuum
_timer_venit (
    vacuum* datum)
{
    ((Signa*)datum)->timer++;
}

interior b32
_labor_semel (
    vacuum* datum)
{
    ((Signa*)datum)->labor++;
    redde VERUM;
}

/* reactor applicationis communis: timer, labor, petitio - ansa una */
interior vacuum
_sectio_reactor_communis (CredoContextus* c)
{
        Reactor* r = reactor_creare(c->piscina);
      Hospitium* h = _servum_creare(c, r, ZEPHYRUM);
          Signa  s;
    TcpConnexio* a;
      character  capita[MXXIV];
             i8  corpus[LXIV];
            i32  k;

    memset(&s, ZEPHYRUM, magnitudo(s));
    CREDO_VERUM(hospitium_reactor(h) == r);
    CREDO_VERUM(hospitium_praebere(h, HTTP_GET, "/parvum", _parvum,
        NIHIL));
    CREDO_VERUM(reactor_timer_adicere(r, L, FALSUM, _timer_venit, &s)
        != REACTOR_TIMER_ERROR);
    CREDO_VERUM(reactor_laborem_adicere(r, _labor_semel, &s));
    a = _cliens(c->piscina, hospitium_portus(h));
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_VERUM(_mittere(a, "GET /parvum HTTP/1.1\r\nHost: x\r\n\r\n"));
    CREDO_AEQUALIS_S32((s32)_legere_responsum(h, a, capita, MXXIV,
        corpus,
        LXIV, X), (s32)VI);
    per (k = ZEPHYRUM; k < X && s.timer == ZEPHYRUM; k++)
    {
        _pumpare(h, I);
    }
    CREDO_AEQUALIS_I32(s.timer, I);
    CREDO_AEQUALIS_I32(s.labor, I);
    tcp_claudere(a);
    hospitium_destruere(h);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r), ZEPHYRUM);  /* vivit */
    reactor_destruere(r);
}

/* filum VI MB per fontem: octeti exacti, piscina connexionis parva */
interior vacuum
_sectio_filum_magnum (CredoContextus* c)
{
      Hospitium* h = _servum_creare(c, NIHIL, ZEPHYRUM);
      Contextus  x;
    TcpConnexio* a;
      character  capita[MXXIV];
             i8* corpus = (i8*)piscina_allocare(c->piscina, MAGNUM);

    CREDO_NECESSE_NON_NIHIL(corpus);
    memset(&x, ZEPHYRUM, magnitudo(x));
    x.via_fili = _filum_magnum(c);
    CREDO_VERUM(hospitium_praebere(h, HTTP_GET, "/magnum", _magnum,
        &x));
    a = _cliens(c->piscina, hospitium_portus(h));
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_VERUM(_mittere(a, "GET /magnum HTTP/1.1\r\nHost: x\r\n\r\n"));
    CREDO_VERUM(_aequale_magno(corpus, _legere_responsum(h, a, capita,
        MXXIV, corpus, MAGNUM, MMM)));
    CREDO_NECESSE_NON_NIHIL(x.piscina_connexionis);
    CREDO_VERUM(piscina_summa_apex_usus(x.piscina_connexionis)
        < APEX_MAXIMUS);
    tcp_claudere(a);
    hospitium_destruere(h);
}

/* dum A lente legit filum magnum, B parvum accipit */
interior vacuum
_sectio_fons_par (CredoContextus* c)
{
      Hospitium* h = _servum_creare(c, NIHIL, ZEPHYRUM);
      Contextus  x;
    TcpConnexio* a;
    TcpConnexio* b;
      character  capita[MXXIV];
             i8  corpus[LXIV];
             i8  capsa[LXIV * MXXIV];
            s32  n;

    memset(&x, ZEPHYRUM, magnitudo(x));
    x.via_fili = _filum_magnum(c);
    CREDO_VERUM(hospitium_praebere(h, HTTP_GET, "/magnum", _magnum,
        &x));
    CREDO_VERUM(hospitium_praebere(h, HTTP_GET, "/parvum", _parvum,
        NIHIL));
    a = _cliens(c->piscina, hospitium_portus(h));
    b = _cliens(c->piscina, hospitium_portus(h));
    CREDO_NECESSE(a != NIHIL && b != NIHIL);
    CREDO_VERUM(_mittere(a, "GET /magnum HTTP/1.1\r\nHost: x\r\n\r\n"));
    _pumpare(h, V);
    n = tcp_recipere(a, capsa, (i32)magnitudo(capsa));   /* paulum */
    CREDO_VERUM(n > ZEPHYRUM);
    CREDO_VERUM(_mittere(b, "GET /parvum HTTP/1.1\r\nHost: x\r\n\r\n"));
    CREDO_AEQUALIS_S32((s32)_legere_responsum(h, b, capita, MXXIV,
        corpus,
        LXIV, XX), (s32)VI);
    tcp_claudere(a);
    tcp_claudere(b);
    hospitium_destruere(h);
}

interior vacuum
_sectio_directorium_magnum (CredoContextus* c)
{
      Hospitium* h = _servum_creare(c, NIHIL, ZEPHYRUM);
    TcpConnexio* a;
      character  capita[MXXIV];
             i8* corpus = (i8*)piscina_allocare(c->piscina, MAGNUM);

    CREDO_NECESSE_NON_NIHIL(corpus);
    (vacuum)_filum_magnum(c);
    CREDO_VERUM(hospitium_directorium_servire(h, "/stat",
        credo_area(c)));
    a = _cliens(c->piscina, hospitium_portus(h));
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_VERUM(_mittere(a,
        "GET /stat/magnum.bin HTTP/1.1\r\nHost: x\r\n\r\n"));
    CREDO_VERUM(_aequale_magno(corpus, _legere_responsum(h, a, capita,
        MXXIV, corpus, MAGNUM, MMM)));
    tcp_claudere(a);
    hospitium_destruere(h);
}

/* sistere molliter: petitio dilata perficitur, connexio nova recusatur */
interior vacuum
_sectio_molliter (CredoContextus* c)
{
      Hospitium* h = _servum_creare(c, NIHIL, ZEPHYRUM);
      Contextus  x;
    TcpConnexio* a;
    TcpConnexio* b;
      character  capita[MXXIV];
             i8  corpus[LXIV];
            i32  portus = hospitium_portus(h);
            i32  k;

    memset(&x, ZEPHYRUM, magnitudo(x));
    x.h             = h;
    x.h_piscina     = c->piscina;
    x.gradus_finis  = V;
    x.responsum     = "molle";
    CREDO_VERUM(hospitium_praebere(h, HTTP_GET, "/tardum", _tardum,
        &x));
    a = _cliens(c->piscina, portus);
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_VERUM(_mittere(a, "GET /tardum HTTP/1.1\r\nHost: x\r\n\r\n"));
    _pumpare(h, II);
    hospitium_sistere_molliter(h, MM);
    b = _cliens(c->piscina, portus);
    CREDO_NIHIL(b);
    CREDO_AEQUALIS_S32((s32)_legere_responsum(h, a, capita, MXXIV,
        corpus,
        LXIV, XX), (s32)V);
    CREDO_VERUM(memcmp(corpus, "molle", V) == ZEPHYRUM);
    per (k = ZEPHYRUM; k < L && hospitium_gressus(h, X); k++)
    {
        ;
    }
    CREDO_VERUM(hospitium_sistit(h));
    tcp_claudere(a);
    hospitium_destruere(h);
}

/* lector octetorum fili: partes VII KB, aequales; via absens NIHIL */
interior vacuum
_sectio_filum_octeti (CredoContextus* c)
{
       constans character* via  = _filum_magnum(c);
     FilumOctetorumLector* l    = filum_octeti_aperire(via, c->piscina);
                       i8* d = (i8*)piscina_allocare(c->piscina,
                           MAGNUM);
                      s64 lecta = ZEPHYRUM;
                      s32 n;
                character absens[DXII];

    CREDO_NECESSE(l != NIHIL && d != NIHIL);
    dum ((n = filum_octeti_legere(l, d + lecta, VII * MXXIV))
        > ZEPHYRUM)
    {
        lecta += n;
    }
    CREDO_AEQUALIS_S32(n, ZEPHYRUM);
    CREDO_VERUM(_aequale_magno(d, lecta));
    filum_octeti_claudere(l);
    sprintf(absens, "%s/nusquam.bin", credo_area(c));
    CREDO_NIHIL(filum_octeti_aperire(absens, c->piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "dilatum", _sectio_dilatum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "expiratum", _sectio_expiratum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "generatio", _sectio_generatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "reactor_communis", _sectio_reactor_communis, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "filum_magnum", _sectio_filum_magnum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fons_par", _sectio_fons_par, NIHIL, NIHIL, NIHIL, NIHIL },
    { "directorium_magnum", _sectio_directorium_magnum, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "molliter", _sectio_molliter, NIHIL, NIHIL, NIHIL, NIHIL },
    { "filum_octeti", _sectio_filum_octeti, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("hospitium_dilatum", SECTIONES);
}
