/* probatio_tls13_descensus.c - descensus 1.3 -> 1.2 (tls13-spec II.3,
 * tls13-plan T7) SINE rete: s_server localis et servi ficti.
 *
 * descensus: s_server -tls1_2 et vectura TLS13 -> eadem TlsConnexio
 * per BearSSL perficitur (tls_vectura BEARSSL, tls_versio 0x0303),
 * gradatim (http_vocare et directe) et obstruens. servus_vetus: servus
 * fictus 1.3 ignorans ServerHello 1.2 reddit -> redintegratio (acceptio
 * secunda, ClientHello BearSSL). sine_descensu: "solum TLS 1.2".
 * signum: s_server ordinarius (1.3 novit) per descensum coactum ->
 * DOWNGRD in server_random -> "descensus vetitus" (P5).
 * nulla_descensio: alerta alia (42) -> error, acceptio UNA. versio_13:
 * 0x0304. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "reactor.h"
#include "http.h"
#include "http_vocatio.h"
#include "tcp.h"
#include "tls.h"
#include "filum.h"
#include "servus_tls.h"
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define PETITIO "GET /parvum.txt HTTP/1.0\r\n\r\n"

nomen structura {
    character dir[DXII];
    character fixa[DXII];
    character ancora[DXII];
} Locus;

nomen structura {
                  i32  finis;
    ChordaAedificator* corpus;
} Observatio;

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior vacuum
_suitam_parare (CredoContextus* c)
{
                  Locus* l = (Locus*)piscina_allocare(c->piscina,
                      magnitudo(Locus));
     constans character* area = credo_area(c);
     constans character* argumenta[III];
              character  via[MXXIV];
      ProcessusResultus  pr;

    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_NECESSE_NON_NIHIL(area);
    strcpy(l->dir, area);
    sprintf(l->fixa, "%s/certa", area);
    sprintf(l->ancora, "%s/ca.der", l->fixa);
    argumenta[ZEPHYRUM] = "probationes/fixa/tls/generare.sh";
    argumenta[I] = l->fixa;
    argumenta[II] = NIHIL;
    pr = processus_exsequi(argumenta, XXX * M, c->piscina);
    CREDO_NECESSE(pr.successus && pr.codex_exitus == ZEPHYRUM);
    sprintf(via, "%s/parvum.txt", l->dir);
    CREDO_NECESSE(filum_scribere_literis(via, "salve per tls\n"));
    (vacuum)setenv("RHUBARB_TLS", "tls13", I);
    c->datum = l;
}

interior vacuum
_suitam_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    (vacuum)unsetenv("RHUBARB_TLS");
}

interior b32
_continet (
                 chorda  c,
     constans character* fragmentum)
{
    i32 k = (i32)strlen(fragmentum);
    i32 i;

    per (i = ZEPHYRUM; i + k <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, fragmentum, k) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_rivus (
         vacuum* datum,
    constans i8* octeti,
            i32  n)
{
    Observatio* o = (Observatio*)datum;
           i32  k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        chorda_aedificator_appendere_character(o->corpus,
            (character)octeti[k]);
    }
}

interior vacuum
_finis (
         vacuum* datum,
    HttpVocatio* vocatio)
{
    (vacuum)vocatio;
    ((Observatio*)datum)->finis++;
}

interior vacuum
_exspectare_fd (
    integer fd,
        b32 scribere,
        i32 ms)
{
    structura pollfd pf;

    pf.fd       = fd;
    pf.events   = (brevis)(scribere ? POLLOUT : POLLIN);
    pf.revents  = ZEPHYRUM;
    (vacuum)poll(&pf, I, (integer)ms);
}

interior TcpConnexio*
_tcp_non_obstruens (
         i32  portus,
     Piscina* p)
{
    TcpResultus r = tcp_connectere("127.0.0.1", portus, p);
        integer f;

    si (!r.successus)
    {
        redde NIHIL;
    }
    f = (integer)tcp_obtinere_fd(r.connexio);
    (vacuum)fcntl(f, F_SETFL, fcntl(f, F_GETFL, ZEPHYRUM) | O_NONBLOCK);
    redde r.connexio;
}

/* tls_gradus usque ad PERFECTUS aut ERROR; fossa per tcp_obtinere_fd
 * quaeritur OMNI gradu (numerus post redintegrationem idem manet) */
interior TlsGradus
_pacisci (
    TlsConnexio* t,
    TcpConnexio* tcp)
{
    TlsGradus g = TLS_GRADUS_LEGERE;
          i32 k;

    per (k = ZEPHYRUM; k < D && g != TLS_GRADUS_PERFECTUS
         && g != TLS_GRADUS_ERROR; k++)
    {
        g = tls_gradus(t);
        si (g == TLS_GRADUS_LEGERE || g == TLS_GRADUS_SCRIBERE)
        {
            _exspectare_fd((integer)tcp_obtinere_fd(tcp),
                g == TLS_GRADUS_SCRIBERE, CC);
        }
    }
    redde g;
}

/* GET gradatim usque ad finem; reddit corpus lectum */
interior b32
_petere_gradatim (
    TlsConnexio* t,
    TcpConnexio* tcp)
{
    character capsa[IV * MXXIV];
          i32 missa = ZEPHYRUM;
          i32 lecta = ZEPHYRUM;
          i32 k;
          s32 n = -I;

    per (k = ZEPHYRUM; k < D && missa < (i32)strlen(PETITIO); k++)
    {
        n = tls_mittere(t, (constans i8*)PETITIO + missa,
            (i32)strlen(PETITIO) - missa);
        si (n == TLS_ITERUM)
        {
            _exspectare_fd((integer)tcp_obtinere_fd(tcp), VERUM, CC);
            perge;
        }
        si (n <= ZEPHYRUM)
        {
            redde FALSUM;
        }
        missa += (i32)n;
    }
    per (k = ZEPHYRUM; k < D; k++)
    {
        n = tls_recipere(t, (i8*)capsa + lecta,
            (i32)magnitudo(capsa) - lecta - I);
        si (n == TLS_ITERUM)
        {
            _exspectare_fd((integer)tcp_obtinere_fd(tcp), FALSUM, CC);
            perge;
        }
        si (n <= ZEPHYRUM)
        {
            frange;
        }
        lecta += (i32)n;
    }
    capsa[lecta] = '\0';
    redde n == ZEPHYRUM && strstr(capsa, "salve per tls\n") != NIHIL;
}

/* s_server -tls1_2: http_vocare (RHUBARB_TLS=tls13), directe gradatim,
 * obstruens - omnes per BearSSL perficiuntur */
interior vacuum
_sectio_descensus (CredoContextus* c)
{
         constans Locus* l = (constans Locus*)c->suita;
              ServusTls  s;
                Reactor* r = reactor_creare(c->piscina);
             Observatio  o;
            HttpVocatio* v;
    HttpVocatioOptiones  optiones_vocationis;
            TlsOptiones  opt = tls_optiones_default();
            TlsResultus  tr;
            TcpConnexio* tcp;
              character  url[CCLVI];
                    s64  initium;

    CREDO_NECESSE(servum_tls_incipere_cum(&s, l->dir, l->fixa,
        "localhost", "-tls1_2", c->piscina));
    /* http_vocare */
    memset(&optiones_vocationis, ZEPHYRUM,
        magnitudo(optiones_vocationis));
    optiones_vocationis.mora_connexionis_ms  = V * M;
    optiones_vocationis.tls_ancora_via       = l->ancora;
    o.finis                                  = ZEPHYRUM;
    o.corpus = chorda_aedificator_creare(c->piscina,
        MXXIV);
    sprintf(url, "https://localhost:%u/parvum.txt",
        (insignatus integer)s.portus);
    v = http_vocare(r, http_petitio_creare(c->piscina, HTTP_GET, url),
        _rivus, &o, _finis, &o, &optiones_vocationis, c->piscina);
    initium = _ms_nunc();
    dum (v && !http_vocatio_finita(v) && _ms_nunc() - initium < X * M)
    {
        (vacuum)reactor_poll(r, XX);
    }
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_AEQUALIS_I32((i32)http_vocatio_status(v),
        (i32)HTTP_VOCATIO_FINITA);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(o.corpus),
        "salve per tls\n");
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    /* directe gradatim */
    opt.vectura     = TLS_VECTURA_TLS13;
    opt.ancora_via  = l->ancora;
    tcp             = _tcp_non_obstruens(s.portus, c->piscina);
    CREDO_NECESSE_NON_NIHIL(tcp);
    tr = tls_incipere(tcp, "localhost", &opt, c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_AEQUALIS_I32((i32)_pacisci(tr.connexio, tcp),
        (i32)TLS_GRADUS_PERFECTUS);
    CREDO_AEQUALIS_I32((i32)tls_vectura(tr.connexio),
        (i32)TLS_VECTURA_BEARSSL);
    CREDO_AEQUALIS_I32(tls_versio(tr.connexio), 0x0303);
    CREDO_VERUM(_petere_gradatim(tr.connexio, tcp));
    tls_claudere(tr.connexio);
    /* obstruens */
    opt.timeout_ms = V * M;
    tr = tls_connectere_cum_optionibus("localhost", s.portus, &opt,
        c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_AEQUALIS_I32((i32)tls_vectura(tr.connexio),
        (i32)TLS_VECTURA_BEARSSL);
    CREDO_AEQUALIS_I32(tls_versio(tr.connexio), 0x0303);
    CREDO_VERUM(tls_mittere_omnia(tr.connexio, (constans i8*)PETITIO,
        (i32)strlen(PETITIO)));
    tls_claudere(tr.connexio);
    servum_tls_claudere(&s);
}

/* 1.3: tls_versio 0x0304, vectura TLS13 (obstruens) */
interior vacuum
_sectio_versio_13 (CredoContextus* c)
{
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
       TlsOptiones  opt = tls_optiones_default();
       TlsResultus  tr;

    opt.vectura     = TLS_VECTURA_TLS13;
    opt.ancora_via  = l->ancora;
    opt.timeout_ms  = V * M;
    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa, "localhost",
        c->piscina));
    tr = tls_connectere_cum_optionibus("localhost", s.portus, &opt,
        c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_AEQUALIS_I32((i32)tls_vectura(tr.connexio),
        (i32)TLS_VECTURA_TLS13);
    CREDO_AEQUALIS_I32(tls_versio(tr.connexio), 0x0304);
    tls_claudere(tr.connexio);
    servum_tls_claudere(&s);
}

/* sine_descensu: 1.3 aut nihil */
interior vacuum
_sectio_sine_descensu (CredoContextus* c)
{
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
       TlsOptiones  opt = tls_optiones_default();
       TlsResultus  tr;
       TcpConnexio* tcp;

    opt.vectura        = TLS_VECTURA_TLS13;
    opt.ancora_via     = l->ancora;
    opt.timeout_ms     = V * M;
    opt.sine_descensu  = VERUM;
    CREDO_NECESSE(servum_tls_incipere_cum(&s, l->dir, l->fixa,
        "localhost", "-tls1_2", c->piscina));
    tr = tls_connectere_cum_optionibus("localhost", s.portus, &opt,
        c->piscina);
    CREDO_FALSUM(tr.successus);
    CREDO_AEQUALIS_I32((i32)tr.error, (i32)TLS_ERROR_HANDSHAKE);
    CREDO_VERUM(_continet(tr.error_descriptio, "solum TLS 1.2"));
    tcp = _tcp_non_obstruens(s.portus, c->piscina);
    CREDO_NECESSE_NON_NIHIL(tcp);
    tr = tls_incipere(tcp, "localhost", &opt, c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_AEQUALIS_I32((i32)_pacisci(tr.connexio, tcp),
        (i32)TLS_GRADUS_ERROR);
    CREDO_VERUM(_continet(tls_gradus_error(tr.connexio),
        "solum TLS 1.2"));
    tls_claudere(tr.connexio);
    servum_tls_claudere(&s);
}

/* signum: servus 1.3 noscens per descensum coactum -> DOWNGRD\1 in
 * server_random a nobis inspectum (P5) */
interior vacuum
_sectio_signum (CredoContextus* c)
{
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
       TlsOptiones  opt = tls_optiones_default();
       TlsResultus  tr;
       TcpConnexio* tcp;

    opt.vectura            = TLS_VECTURA_TLS13;
    opt.ancora_via         = l->ancora;
    opt.timeout_ms         = V * M;
    opt.descensus_coactus  = VERUM;
    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa, "localhost",
        c->piscina));
    tr = tls_connectere_cum_optionibus("localhost", s.portus, &opt,
        c->piscina);
    CREDO_FALSUM(tr.successus);
    CREDO_AEQUALIS_I32((i32)tr.error, (i32)TLS_ERROR_HANDSHAKE);
    CREDO_VERUM(_continet(tr.error_descriptio, "descensus vetitus"));
    tcp = _tcp_non_obstruens(s.portus, c->piscina);
    CREDO_NECESSE_NON_NIHIL(tcp);
    tr = tls_incipere(tcp, "localhost", &opt, c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_AEQUALIS_I32((i32)_pacisci(tr.connexio, tcp),
        (i32)TLS_GRADUS_ERROR);
    CREDO_VERUM(_continet(tls_gradus_error(tr.connexio),
        "descensus vetitus"));
    tls_claudere(tr.connexio);
    servum_tls_claudere(&s);
}

/* ---- servi ficti (tcp_servus, eodem filo) ---- */

interior TcpResultus
_accipere (
    TcpServus* servus,
          i32  ms,
      Piscina* p)
{
    TcpResultus acc;
            s64 initium = _ms_nunc();

    fac
    {
        _exspectare_fd(tcp_servus_obtinere_fd(servus), FALSUM, L);
        acc = tcp_servus_accipere(servus, p);
    } dum (!acc.successus && acc.error == TCP_ERROR_ITERUM
           && _ms_nunc() - initium < (s64)ms);
    redde acc;
}

/* recordum ClientHello a pare legere (usque ad X conatus) */
interior s32
_salutationem_legere (
    TcpConnexio* par,
             i8* capsa,
            i32  capacitas)
{
    s32 n = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < XX && n <= ZEPHYRUM; k++)
    {
        _exspectare_fd((integer)tcp_obtinere_fd(par), FALSUM, CC);
        n = tcp_recipere(par, capsa, capacitas);
    }
    redde n;
}

interior b32
_continet_octetos (
    constans i8* fenum,
            i32  n,
    constans i8* acus,
            i32  m)
{
    i32 k;

    per (k = ZEPHYRUM; k + m <= n; k++)
    {
        si (memcmp(fenum + k, acus, m) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior TcpServus*
_servum_fictum (Piscina* p)
{
    TcpServusOptiones so = tcp_servus_optiones_default();
    TcpServusResultus sr = tcp_servus_creare_cum_optionibus("127.0.0.1",
        ZEPHYRUM, &so, p);

    CREDO_NECESSE(sr.successus);
    CREDO_NECESSE(tcp_servus_auscultare(sr.servus, IV) == TCP_OK);
    redde sr.servus;
}

/* servus 1.3 ignorans: ClientHello nostrum cifras 1.2 quoque offert
 * (tls13-plan R23) -> ServerHello 1.2 (sine supported_versions) ->
 * descensus: acceptio SECUNDA, ClientHello BearSSL, vectura BEARSSL */
interior vacuum
_sectio_servus_vetus (CredoContextus* c)
{
                 TcpServus* servus  = _servum_fictum(c->piscina);
               TlsOptiones  opt     = tls_optiones_default();
               TlsResultus  tr;
               TcpConnexio* tcp;
               TcpResultus  prima;
               TcpResultus  secunda;
                        i8  capsa[IV * MXXIV];
                       s32  n;
                       i32  k;
                       i32  responsum_n = ZEPHYRUM;
                        i8  responsum[XLVII];
    hic_manens constans i8  CIFRA_1_2[II] = { 0xc0, 0x2b };
    hic_manens constans i8  CAPUT[XI] = {
        XXII, III, III, ZEPHYRUM, XLII,          /* recordum */
        II, ZEPHYRUM, ZEPHYRUM, XXXVIII,          /* ServerHello */
        III, III                                  /* versio 1.2 */
    };

    memcpy(responsum, CAPUT, XI);
    responsum_n = XI;
    per (k = ZEPHYRUM; k < XXXII; k++)
    {
        responsum[responsum_n++] = (i8)(k + I);   /* temere */
    }
    responsum[responsum_n++]  = ZEPHYRUM;           /* sessio vacua */
    responsum[responsum_n++]  = 0xc0;
    responsum[responsum_n++]  = 0x2b;
    responsum[responsum_n++]  = ZEPHYRUM;           /* compressio */
    opt.vectura               = TLS_VECTURA_TLS13;
    tcp = _tcp_non_obstruens(tcp_servus_obtinere_portum(servus),
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(tcp);
    prima = _accipere(servus, II * M, c->piscina);
    CREDO_NECESSE(prima.successus);
    tr = tls_incipere(tcp, "localhost", &opt, c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_AEQUALIS_I32((i32)tls_gradus(tr.connexio),
        (i32)TLS_GRADUS_LEGERE);
    n = _salutationem_legere(prima.connexio, capsa, IV * MXXIV);
    CREDO_NECESSE(n > ZEPHYRUM);
    CREDO_VERUM(_continet_octetos(capsa, (i32)n, CIFRA_1_2, II));
    CREDO_VERUM(tcp_mittere_omnia(prima.connexio, responsum,
        responsum_n));
    /* gradus usque ad ClientHello BearSSL missum */
    per (k = ZEPHYRUM; k < L; k++)
    {
        TlsGradus g = tls_gradus(tr.connexio);

        si (   g == TLS_GRADUS_ERROR
            || (g == TLS_GRADUS_LEGERE
                && tls_vectura(tr.connexio) == TLS_VECTURA_BEARSSL))
        {
            frange;
        }
        _exspectare_fd((integer)tcp_obtinere_fd(tcp),
            g == TLS_GRADUS_SCRIBERE, L);
    }
    CREDO_AEQUALIS_I32((i32)tls_vectura(tr.connexio),
        (i32)TLS_VECTURA_BEARSSL);
    secunda = _accipere(servus, II * M, c->piscina);
    CREDO_NECESSE(secunda.successus);
    n = _salutationem_legere(secunda.connexio, capsa, IV * MXXIV);
    CREDO_NECESSE(n > ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)capsa[ZEPHYRUM], XXII);
    tls_claudere(tr.connexio);
    tcp_claudere(prima.connexio);
    tcp_claudere(secunda.connexio);
    tcp_servus_claudere(servus);
}

/* alerta bad_certificate ad ClientHello: error nominatum, NULLA
 * redintegratio (solum SOLUM_1_2 descendit, spec II.4) */
interior vacuum
_sectio_nulla_descensio (CredoContextus* c)
{
                 TcpServus* servus  = _servum_fictum(c->piscina);
               TlsOptiones  opt     = tls_optiones_default();
               TlsResultus  tr;
               TcpConnexio* tcp;
               TcpResultus  prima;
               TcpResultus  altera;
                        i8  capsa[IV * MXXIV];
    hic_manens constans i8  ALERTA[VII] = {
        XXI, III, III, ZEPHYRUM, II, II, XLII
    };

    opt.vectura = TLS_VECTURA_TLS13;
    tcp = _tcp_non_obstruens(tcp_servus_obtinere_portum(servus),
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(tcp);
    prima = _accipere(servus, II * M, c->piscina);
    CREDO_NECESSE(prima.successus);
    tr = tls_incipere(tcp, "localhost", &opt, c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_AEQUALIS_I32((i32)tls_gradus(tr.connexio),
        (i32)TLS_GRADUS_LEGERE);
    CREDO_NECESSE(_salutationem_legere(prima.connexio, capsa,
        IV * MXXIV) > ZEPHYRUM);
    CREDO_VERUM(tcp_mittere_omnia(prima.connexio, ALERTA, VII));
    CREDO_AEQUALIS_I32((i32)_pacisci(tr.connexio, tcp),
        (i32)TLS_GRADUS_ERROR);
    CREDO_VERUM(_continet(tls_gradus_error(tr.connexio),
        "bad_certificate"));
    altera = _accipere(servus, CCC, c->piscina);
    CREDO_FALSUM(altera.successus);
    tls_claudere(tr.connexio);
    tcp_claudere(prima.connexio);
    tcp_servus_claudere(servus);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "descensus", _sectio_descensus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "versio_13", _sectio_versio_13, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine_descensu", _sectio_sine_descensu, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "signum", _sectio_signum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "servus_vetus", _sectio_servus_vetus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "nulla_descensio", _sectio_nulla_descensio, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("tls13_descensus", SECTIONES,
        _suitam_parare, _suitam_purgare);
}
