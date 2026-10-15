/* probatio_tls_bearssl.c - vectura BearSSL (tls-bearssl-spec II.6, T4)
 * SINE rete: openssl s_server localis (probationes/servus_tls.h),
 * certificata in tempore generata (probationes/fixa/tls/generare.sh).
 *
 * Per http_vocare (RHUBARB_TLS=bearssl in fixura suitae - http_vocatio
 * vecturam non nominat): parvum, folium RSA, reiecta (alienus, CA
 * ignota, sine ancora) cum causa fiduciae. Directe (tls_incipere +
 * tls_gradus super fossam non-blocantem): pactio + GET + lectio usque
 * ad finem, verificatio omissa, par in pactione claudens (ERROR
 * nominatum, numquam pendet). */
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

nomen structura {
    character dir[DXII];       /* fasciculi servi */
    character fixa[DXII];      /* certificata generata */
    character ancora[DXII];    /* ca.der */
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

/* fixura suitae: certificata semel, parvum.txt, RHUBARB_TLS=bearssl */
interior vacuum
_suitam_parare (
    CredoContextus* c)
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
    CREDO_NECESSE(filum_existit(l->ancora));
    sprintf(via, "%s/parvum.txt", l->dir);
    CREDO_NECESSE(filum_scribere_literis(via, "salve per tls\n"));
    (vacuum)setenv("RHUBARB_TLS", "bearssl", I);
    c->datum = l;
}

interior vacuum
_suitam_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    (vacuum)unsetenv("RHUBARB_TLS");
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

/* GET https://localhost:portus/via (ancora NIHIL = repositorium) */
interior HttpVocatio*
_petere (
               Reactor* r,
                   i32  portus,
    constans character* via,
    constans character* ancora,
            Observatio* o,
               Piscina* p)
{
              character  url[CCLVI];
    HttpVocatioOptiones  opt;
            HttpVocatio* v;
                    s64  initium;

    memset(&opt, ZEPHYRUM, magnitudo(opt));
    opt.mora_connexionis_ms  = V * M;
    opt.tls_ancora_via       = ancora;
    o->finis                 = ZEPHYRUM;
    o->corpus                = chorda_aedificator_creare(p, MXXIV);
    sprintf(url, "https://localhost:%u/%s", (insignatus integer)portus,
        via);
    v = http_vocare(r, http_petitio_creare(p, HTTP_GET, url), _rivus, o,
        _finis, o, &opt, p);
    initium = _ms_nunc();
    dum (v && !http_vocatio_finita(v) && _ms_nunc() - initium < X * M)
    {
        (vacuum)reactor_poll(r, XX);
    }
    redde v;
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

/* pactio per http_vocare: 200 + corpus, reactor vacuus post finem */
interior vacuum
_acceptum (
         CredoContextus* c,
    constans character* certificatum)
{
           Reactor* r = reactor_creare(c->piscina);
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
        Observatio  o;
       HttpVocatio* v;
      HttpResultus  res;

    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa, certificatum,
        c->piscina));
    v = _petere(r, s.portus, "parvum.txt", l->ancora, &o, c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    res = http_vocatio_resultus(v);
    CREDO_AEQUALIS_I32(o.finis, I);
    CREDO_AEQUALIS_I32((i32)http_vocatio_status(v),
        (i32)HTTP_VOCATIO_FINITA);
    CREDO_NECESSE(res.successus && res.responsum != NIHIL);
    CREDO_AEQUALIS_I32(res.responsum->status, CC);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(o.corpus),
        "salve per tls\n");
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    servum_tls_claudere(&s);
}

interior vacuum
_sectio_parvum (
    CredoContextus* c)
{
    _acceptum(c, "localhost");
}

interior vacuum
_sectio_rsa (
    CredoContextus* c)
{
    _acceptum(c, "localhost_rsa");
}

/* reiectum: IRRITA, CONNEXIO, causa fiduciae nominata, celeriter */
interior vacuum
_reiectum (
         CredoContextus* c,
    constans character* certificatum,
                   b32  cum_ancora)
{
           Reactor* r = reactor_creare(c->piscina);
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
        Observatio  o;
       HttpVocatio* v;
      HttpResultus  res;
               s64  initium;

    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa, certificatum,
        c->piscina));
    initium = _ms_nunc();
    v = _petere(r, s.portus, "parvum.txt",
        cum_ancora ? l->ancora : NIHIL,
        &o, c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    res = http_vocatio_resultus(v);
    CREDO_AEQUALIS_I32((i32)http_vocatio_status(v),
        (i32)HTTP_VOCATIO_IRRITA);
    CREDO_AEQUALIS_I32((i32)res.error, (i32)HTTP_ERROR_CONNEXIO);
    CREDO_VERUM(_continet(res.error_descriptio,
        "certificatum reiectum"));
    CREDO_VERUM(_ms_nunc() - initium < II * M);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    servum_tls_claudere(&s);
}

interior vacuum
_sectio_alienus (
    CredoContextus* c)
{
    _reiectum(c, "alienus", VERUM);
}

interior vacuum
_sectio_ignotus (
    CredoContextus* c)
{
    _reiectum(c, "ignotus", VERUM);
}

interior vacuum
_sectio_sine_ancora (
    CredoContextus* c)
{
    _reiectum(c, "localhost", FALSUM);
}

/* fossa non-blocans: tcp_connectere + O_NONBLOCK */
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

/* fossam exspectare (POLLIN/POLLOUT) usque ad CC ms */
interior vacuum
_exspectare (
    TcpConnexio* tcp,
            b32  scribere)
{
    structura pollfd pf;

    pf.fd       = (integer)tcp_obtinere_fd(tcp);
    pf.events   = (brevis)(scribere ? POLLOUT : POLLIN);
    pf.revents  = ZEPHYRUM;
    (vacuum)poll(&pf, I, CC);
}

/* tls_gradus usque ad PERFECTUS aut ERROR (D gradus summum) */
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
            _exspectare(tcp, g == TLS_GRADUS_SCRIBERE);
        }
    }
    redde g;
}

/* pactio gradatim directa: PERFECTUS, GET, lectio usque ad 0 (finis
 * mundus), vectura BEARSSL memorata, claudere bis tutum */
interior vacuum
_sectio_gradus (
    CredoContextus* c)
{
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
       TlsOptiones  opt = tls_optiones_default();
       TlsResultus  tr;
       TcpConnexio* tcp;
constans character* petitio = "GET /parvum.txt HTTP/1.0\r\n\r\n";
         character  capsa[IV * MXXIV];
               i32  missa  = ZEPHYRUM;
               i32  lecta  = ZEPHYRUM;
               s32  n      = -I;
               i32  k;

    opt.vectura     = TLS_VECTURA_BEARSSL;
    opt.ancora_via  = l->ancora;
    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa, "localhost",
        c->piscina));
    tcp = _tcp_non_obstruens(s.portus, c->piscina);
    CREDO_NECESSE_NON_NIHIL(tcp);
    tr = tls_incipere(tcp, "localhost", &opt, c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_AEQUALIS_I32((i32)tls_vectura(tr.connexio),
        (i32)TLS_VECTURA_BEARSSL);
    CREDO_NECESSE(_pacisci(tr.connexio, tcp) == TLS_GRADUS_PERFECTUS);
    per (k = ZEPHYRUM; k < D && missa < (i32)strlen(petitio); k++)
    {
        n = tls_mittere(tr.connexio, (constans i8*)petitio + missa,
            (i32)strlen(petitio) - missa);
        si (n == TLS_ITERUM)
        {
            _exspectare(tcp, VERUM);
            perge;
        }
        CREDO_NECESSE(n > ZEPHYRUM);
        missa += (i32)n;
    }
    per (k = ZEPHYRUM; k < D; k++)
    {
        n = tls_recipere(tr.connexio, (i8*)capsa + lecta,
            (i32)magnitudo(capsa) - lecta - I);
        si (n == TLS_ITERUM)
        {
            _exspectare(tcp, FALSUM);
            perge;
        }
        si (n <= ZEPHYRUM)
        {
            frange;
        }
        lecta += (i32)n;
    }
    CREDO_AEQUALIS_I32((i32)n, ZEPHYRUM);
    capsa[lecta] = '\0';
    CREDO_VERUM(strstr(capsa, "salve per tls\n") != NIHIL);
    tls_claudere(tr.connexio);
    CREDO_FALSUM(tls_est_valida(tr.connexio));
    tls_claudere(tr.connexio);
    servum_tls_claudere(&s);
}

/* verificatio omissa: CA ignota sine ancora, tamen PERFECTUS */
interior vacuum
_sectio_omissa (
    CredoContextus* c)
{
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
       TlsOptiones  opt = tls_optiones_default();
       TlsResultus  tr;
       TcpConnexio* tcp;

    opt.vectura                  = TLS_VECTURA_BEARSSL;
    opt.verificare_certificatum  = FALSUM;
    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa, "ignotus",
        c->piscina));
    tcp = _tcp_non_obstruens(s.portus, c->piscina);
    CREDO_NECESSE_NON_NIHIL(tcp);
    tr = tls_incipere(tcp, "localhost", &opt, c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_AEQUALIS_I32((i32)_pacisci(tr.connexio, tcp),
        (i32)TLS_GRADUS_PERFECTUS);
    tls_claudere(tr.connexio);
    servum_tls_claudere(&s);
}

/* acceptio cum exspectatione (ut probatio_tcp_incipere.c) */
interior TcpResultus
_accipere (
    TcpServus* servus,
      Piscina* p)
{
    structura pollfd pf;
         TcpResultus acc;
                 s64 initium = _ms_nunc();

    fac
    {
        pf.fd       = tcp_servus_obtinere_fd(servus);
        pf.events   = POLLIN;
        pf.revents  = 0;
        (vacuum)poll(&pf, I, (integer)L);
        acc = tcp_servus_accipere(servus, p);
    } dum (!acc.successus && acc.error == TCP_ERROR_ITERUM
           && _ms_nunc() - initium < II * M);
    redde acc;
}

/* par in pactione claudit MUNDE: ClientHello mittitur, par eum LEGIT
 * (ne RST pro FIN) et claudit -> cliens EOF in pactione videt: ERROR
 * nominatum "a pare clausa in pactione" (ramus EOF antliae, non
 * clausura munda nec I/O), numquam pendet. */
interior vacuum
_sectio_clausa_a_pare (
    CredoContextus* c)
{
    TcpServusOptiones so = tcp_servus_optiones_default();
    TcpServusResultus sr =
        tcp_servus_creare_cum_optionibus("127.0.0.1",
        ZEPHYRUM, &so, c->piscina);
          TcpResultus  acceptum;
          TlsOptiones  opt = tls_optiones_default();
          TlsResultus  tr;
          TcpConnexio* tcp;
            character  salutatio[MXXIV];
                  s32  n = ZEPHYRUM;
                  i32  k;

    CREDO_NECESSE(sr.successus);
    CREDO_NECESSE(tcp_servus_auscultare(sr.servus, IV) == TCP_OK);
    tcp = _tcp_non_obstruens(tcp_servus_obtinere_portum(sr.servus),
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(tcp);
    acceptum = _accipere(sr.servus, c->piscina);
    CREDO_NECESSE(acceptum.successus);
    opt.vectura  = TLS_VECTURA_BEARSSL;
    tr           = tls_incipere(tcp, "localhost", &opt, c->piscina);
    CREDO_NECESSE(tr.successus);
    /* gradus primus: ClientHello in fossam, deinde responsum
     * exspectat */
    CREDO_AEQUALIS_I32((i32)tls_gradus(tr.connexio),
        (i32)TLS_GRADUS_LEGERE);
    per (k = ZEPHYRUM; k < XX && n <= ZEPHYRUM; k++)
    {
        _exspectare(acceptum.connexio, FALSUM);
        n = tcp_recipere(acceptum.connexio, (i8*)salutatio,
            (i32)magnitudo(salutatio));
    }
    CREDO_NECESSE(n > ZEPHYRUM);
    tcp_claudere(acceptum.connexio);
    CREDO_AEQUALIS_I32((i32)_pacisci(tr.connexio, tcp),
        (i32)TLS_GRADUS_ERROR);
    CREDO_VERUM(_continet(tls_gradus_error(tr.connexio),
        "a pare clausa in pactione"));
    tls_claudere(tr.connexio);
    tcp_servus_claudere(sr.servus);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "parvum", _sectio_parvum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rsa", _sectio_rsa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "alienus", _sectio_alienus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ignotus", _sectio_ignotus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine ancora", _sectio_sine_ancora, NIHIL, NIHIL, NIHIL, NIHIL },
    { "gradus", _sectio_gradus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "omissa", _sectio_omissa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "clausa a pare", _sectio_clausa_a_pare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("tls_bearssl", SECTIONES,
        _suitam_parare, _suitam_purgare);
}
