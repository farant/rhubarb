/* probatio_tls13_servus.c - vectura TLS 1.3 propria (tls13-spec II.1,
 * tls13-plan T6) SINE rete: openssl s_server -tls1_3 localis
 * (probationes/servus_tls.h), certificata in tempore generata.
 *
 * Per http_vocare (RHUBARB_TLS=tls13 in fixura suitae): folium EC et
 * RSA (rsa_pss_rsae), tres cifrae singulae coactae, HRR (-groups
 * P-256), CertificateRequest optionalis (-verify) et necessarius
 * (-Verify: certificate_required nominatum), reiecta (alienus,
 * expiratus, ignotus, sine ancora), corpus magnum (256 KB, Review focus
 * 5). Directe (tls_incipere + tls_gradus): GET + lectio usque ad 0,
 * verificatio omissa, par in pactione claudens, EOF sine close_notify
 * post pactionem = 0 (Review focus 3). Obstruens: certificatum folii,
 * tempus excessum. */
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

#define MAGNUM (CCLVI * MXXIV)

nomen structura {
    character dir[DXII];       /* fasciculi servi */
    character fixa[DXII];      /* certificata generata */
    character ancora[MXXIV];   /* ca.der (fixa + nomen: gcc
                                    * -Wformat-overflow) */
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

/* octetus k corporis magni: forma non periodica brevi */
interior character
_magni_octetus (
    i32 k)
{
    redde (character)('a' + (k * VII + k / MXXIV) % XXVI);
}

/* fixura suitae: certificata semel, parvum.txt, magnum.txt,
 * RHUBARB_TLS=tls13 */
interior vacuum
_suitam_parare (
    CredoContextus* c)
{
                  Locus* l = (Locus*)piscina_allocare(c->piscina,
                      magnitudo(Locus));
     constans character* area = credo_area(c);
     constans character* argumenta[III];
              character  via[MXXIV];
              character* magnum = (character*)piscina_allocare(
                  c->piscina, MAGNUM + I);
      ProcessusResultus pr;
                    i32 k;

    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_NECESSE_NON_NIHIL(area);
    CREDO_NECESSE_NON_NIHIL(magnum);
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
    per (k = ZEPHYRUM; k < MAGNUM; k++)
    {
        magnum[k] = _magni_octetus(k);
    }
    magnum[MAGNUM] = '\0';
    sprintf(via, "%s/magnum.txt", l->dir);
    CREDO_NECESSE(filum_scribere_literis(via, magnum));
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

/* pactio per http_vocare contra s_server -tls1_3 + additamenta: 200 +
 * corpus, reactor vacuus post finem; reddit corpus */
interior chorda
_acceptum_cum (
         CredoContextus* c,
    constans character* certificatum,
    constans character* additamenta,
    constans character* via)
{
           Reactor* r = reactor_creare(c->piscina);
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
        Observatio  o;
       HttpVocatio* v;
      HttpResultus  res;
         character  vexilla[CCLVI];
            chorda  corpus;

    sprintf(vexilla, "-tls1_3 %s", additamenta);
    CREDO_NECESSE(servum_tls_incipere_cum(&s, l->dir, l->fixa,
        certificatum, vexilla, c->piscina));
    v = _petere(r, s.portus, via, l->ancora, &o, c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    res = http_vocatio_resultus(v);
    CREDO_AEQUALIS_I32(o.finis, I);
    CREDO_AEQUALIS_I32((i32)http_vocatio_status(v),
        (i32)HTTP_VOCATIO_FINITA);
    CREDO_NECESSE(res.successus && res.responsum != NIHIL);
    CREDO_AEQUALIS_I32(res.responsum->status, CC);
    corpus = chorda_aedificator_finire(o.corpus);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    servum_tls_claudere(&s);
    redde corpus;
}

interior vacuum
_parvum (
         CredoContextus* c,
    constans character* certificatum,
    constans character* additamenta)
{
    CREDO_CHORDA_AEQUALIS_LITERIS(_acceptum_cum(c, certificatum,
        additamenta, "parvum.txt"), "salve per tls\n");
}

interior vacuum
_sectio_parvum (
    CredoContextus* c)
{
    _parvum(c, "localhost", "");
}

/* folium RSA: CertificateVerify rsa_pss_rsae */
interior vacuum
_sectio_rsa (
    CredoContextus* c)
{
    _parvum(c, "localhost_rsa", "");
}

interior vacuum
_sectio_aes128 (
    CredoContextus* c)
{
    _parvum(c, "localhost", "-ciphersuites TLS_AES_128_GCM_SHA256");
}

interior vacuum
_sectio_aes256 (
    CredoContextus* c)
{
    _parvum(c, "localhost", "-ciphersuites TLS_AES_256_GCM_SHA384");
}

interior vacuum
_sectio_chacha (
    CredoContextus* c)
{
    _parvum(c, "localhost",
        "-ciphersuites TLS_CHACHA20_POLY1305_SHA256");
}

/* servus solum P-256 -> HelloRetryRequest; CH2 cum crustulo si datum,
 * CCS fictum ante CH2 (sessio nostra non vacua, tls13-plan R19) */
interior vacuum
_sectio_hrr (
    CredoContextus* c)
{
    _parvum(c, "localhost", "-groups P-256");
}

/* CertificateRequest optionalis: Certificate vacuum, servus admittit
 * (tls13-plan R17) */
interior vacuum
_sectio_cr_optionalis (
    CredoContextus* c)
{
    _parvum(c, "localhost", "-verify 1");
}

/* corpus 256 KB: recorda multa per lectionem fossae, omnia tradita
 * ordine (Review focus 5) */
interior vacuum
_sectio_magnum (CredoContextus* c)
{
    chorda corpus = _acceptum_cum(c, "localhost", "", "magnum.txt");
       i32 k;
       b32 aequalia = corpus.mensura == MAGNUM;

    CREDO_AEQUALIS_I32(corpus.mensura, MAGNUM);
    per (k = ZEPHYRUM; aequalia && k < MAGNUM; k++)
    {
        aequalia = corpus.datum[k] == (i8)_magni_octetus(k);
    }
    CREDO_VERUM(aequalia);
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

    CREDO_NECESSE(servum_tls_incipere_cum(&s, l->dir, l->fixa,
        certificatum, "-tls1_3", c->piscina));
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
_sectio_expiratus (
    CredoContextus* c)
{
    _reiectum(c, "expiratus", VERUM);
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

/* octeti omnes gradatim (TLS_ITERUM -> exspecta, iterum) */
interior vacuum
_mittere_gradatim (
          TlsConnexio* t,
          TcpConnexio* tcp,
    constans character* textus)
{
    i32 missa  = ZEPHYRUM;
    i32 n      = (i32)strlen(textus);
    i32 k;

    per (k = ZEPHYRUM; k < D && missa < n; k++)
    {
        s32 m = tls_mittere(t, (constans i8*)textus + missa, n - missa);

        si (m == TLS_ITERUM)
        {
            _exspectare(tcp, VERUM);
            perge;
        }
        CREDO_NECESSE(m > ZEPHYRUM);
        missa += (i32)m;
    }
    CREDO_AEQUALIS_I32(missa, n);
}

/* lectio gradatim usque ad reditum non TLS_ITERUM post 'minimum'
 * octetos (aut D conatus); reddit reditum ultimum */
interior s32
_legere_gradatim (
    TlsConnexio* t,
    TcpConnexio* tcp,
      character* capsa,
            i32  capacitas,
            i32* lecta,
            i32  minimum)
{
    s32 n = TLS_ITERUM;
    i32 k;

    per (k = ZEPHYRUM; k < D; k++)
    {
        n = tls_recipere(t, (i8*)capsa + *lecta, capacitas - *lecta
            - I);
        si (n == TLS_ITERUM)
        {
            si (minimum > ZEPHYRUM && *lecta >= minimum)
            {
                frange;
            }
            _exspectare(tcp, FALSUM);
            perge;
        }
        si (n <= ZEPHYRUM)
        {
            frange;
        }
        *lecta += (i32)n;
    }
    capsa[*lecta] = '\0';
    redde n;
}

/* pactio gradatim directa: PERFECTUS, vectura TLS13, GET, lectio usque
 * ad 0 (close_notify servi), claudere bis tutum */
interior vacuum
_sectio_gradus (CredoContextus* c)
{
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
       TlsOptiones  opt = tls_optiones_default();
       TlsResultus  tr;
       TcpConnexio* tcp;
         character  capsa[IV * MXXIV];
               i32  lecta = ZEPHYRUM;

    opt.vectura     = TLS_VECTURA_TLS13;
    opt.ancora_via  = l->ancora;
    CREDO_NECESSE(servum_tls_incipere_cum(&s, l->dir, l->fixa,
        "localhost", "-tls1_3", c->piscina));
    tcp = _tcp_non_obstruens(s.portus, c->piscina);
    CREDO_NECESSE_NON_NIHIL(tcp);
    tr = tls_incipere(tcp, "localhost", &opt, c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_AEQUALIS_I32((i32)tls_vectura(tr.connexio),
        (i32)TLS_VECTURA_TLS13);
    CREDO_NECESSE(_pacisci(tr.connexio, tcp) == TLS_GRADUS_PERFECTUS);
    _mittere_gradatim(tr.connexio, tcp,
        "GET /parvum.txt HTTP/1.0\r\n\r\n");
    CREDO_AEQUALIS_S32(_legere_gradatim(tr.connexio, tcp, capsa,
        (i32)magnitudo(capsa), &lecta, ZEPHYRUM), ZEPHYRUM);
    CREDO_VERUM(strstr(capsa, "salve per tls\n") != NIHIL);
    tls_claudere(tr.connexio);
    CREDO_FALSUM(tls_est_valida(tr.connexio));
    tls_claudere(tr.connexio);
    servum_tls_claudere(&s);
}

/* CertificateRequest necessarius (-Verify): pactio nostra perfecta,
 * deinde servus alertam certificate_required mittit -> lectio -1,
 * causa nominata (tls13-plan R17) */
interior vacuum
_sectio_cr_necessaria (CredoContextus* c)
{
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
       TlsOptiones  opt = tls_optiones_default();
       TlsResultus  tr;
       TcpConnexio* tcp;
         character  capsa[IV * MXXIV];
               i32  lecta = ZEPHYRUM;

    opt.vectura     = TLS_VECTURA_TLS13;
    opt.ancora_via  = l->ancora;
    CREDO_NECESSE(servum_tls_incipere_cum(&s, l->dir, l->fixa,
        "localhost", "-tls1_3 -Verify 1", c->piscina));
    tcp = _tcp_non_obstruens(s.portus, c->piscina);
    CREDO_NECESSE_NON_NIHIL(tcp);
    tr = tls_incipere(tcp, "localhost", &opt, c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_NECESSE(_pacisci(tr.connexio, tcp) == TLS_GRADUS_PERFECTUS);
    _mittere_gradatim(tr.connexio, tcp,
        "GET /parvum.txt HTTP/1.0\r\n\r\n");
    CREDO_AEQUALIS_S32(_legere_gradatim(tr.connexio, tcp, capsa,
        (i32)magnitudo(capsa), &lecta, ZEPHYRUM), -I);
    CREDO_AEQUALIS_I32(lecta, ZEPHYRUM);
    CREDO_VERUM(_continet(tls_gradus_error(tr.connexio),
        "116 (certificate_required)"));
    tls_claudere(tr.connexio);
    servum_tls_claudere(&s);
}

/* servus -rev: linea remissa, deinde servus ABRUPTUS (fossa a nucleo
 * clausa: FIN sine close_notify) -> post pactionem lectio 0, numquam
 * -1 (Review focus 3, ut ceterae vecturae) */
interior vacuum
_sectio_eof (CredoContextus* c)
{
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
       TlsOptiones  opt = tls_optiones_default();
       TlsResultus  tr;
       TcpConnexio* tcp;
         character  capsa[IV * MXXIV];
               i32  lecta = ZEPHYRUM;

    opt.vectura     = TLS_VECTURA_TLS13;
    opt.ancora_via  = l->ancora;
    CREDO_NECESSE(servum_tls_incipere_modo(&s, l->dir, l->fixa,
        "localhost", "-rev", "-tls1_3", c->piscina));
    tcp = _tcp_non_obstruens(s.portus, c->piscina);
    CREDO_NECESSE_NON_NIHIL(tcp);
    tr = tls_incipere(tcp, "localhost", &opt, c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_NECESSE(_pacisci(tr.connexio, tcp) == TLS_GRADUS_PERFECTUS);
    _mittere_gradatim(tr.connexio, tcp, "salve\n");
    (vacuum)_legere_gradatim(tr.connexio, tcp, capsa,
        (i32)magnitudo(capsa), &lecta, VI);
    CREDO_VERUM(strstr(capsa, "evlas") != NIHIL);
    servum_tls_claudere(&s);
    CREDO_AEQUALIS_S32(_legere_gradatim(tr.connexio, tcp, capsa,
        (i32)magnitudo(capsa), &lecta, ZEPHYRUM), ZEPHYRUM);
    tls_claudere(tr.connexio);
}

/* verificatio omissa: CA ignota sine ancora, tamen PERFECTUS (servus
 * ordinarius: 1.2 et 1.3 novit - cliens solum 1.3 offert) */
interior vacuum
_sectio_omissa (CredoContextus* c)
{
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
       TlsOptiones  opt = tls_optiones_default();
       TlsResultus  tr;
       TcpConnexio* tcp;

    opt.vectura                  = TLS_VECTURA_TLS13;
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

/* par in pactione claudit MUNDE (ClientHello lecto) -> ERROR nominatum
 * "a pare clausa in pactione", numquam pendet */
interior vacuum
_sectio_clausa_a_pare (CredoContextus* c)
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
    opt.vectura  = TLS_VECTURA_TLS13;
    tr           = tls_incipere(tcp, "localhost", &opt, c->piscina);
    CREDO_NECESSE(tr.successus);
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

/* obstruens: ancora -> successus, vectura TLS13, certificatum folii
 * (subiectum, CCCLXV dies), mittere_omnia; alienus ->
 * TLS_ERROR_CERTIFICATUM */
interior vacuum
_sectio_obstruens (CredoContextus* c)
{
     constans Locus* l = (constans Locus*)c->suita;
          ServusTls  s;
        TlsOptiones  opt = tls_optiones_default();
        TlsResultus  tr;
    TlsCertificatum  cert;
                s64  dies;

    opt.vectura     = TLS_VECTURA_TLS13;
    opt.ancora_via  = l->ancora;
    opt.timeout_ms  = V * M;
    CREDO_NECESSE(servum_tls_incipere_cum(&s, l->dir, l->fixa,
        "localhost", "-tls1_3", c->piscina));
    tr = tls_connectere_cum_optionibus("localhost", s.portus, &opt,
        c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_AEQUALIS_I32((i32)tls_vectura(tr.connexio),
        (i32)TLS_VECTURA_TLS13);
    CREDO_VERUM(tls_certificatum_obtinere(tr.connexio, &cert));
    CREDO_CHORDA_AEQUALIS_LITERIS(cert.subiectum, "localhost");
    dies = (cert.non_post - cert.non_ante) / ((s64)LXXXVI * M + CD);
    CREDO_VERUM(dies >= CCCLXIV && dies <= CCCLXVI);
    CREDO_VERUM(tls_mittere_omnia(tr.connexio,
        (constans i8*)"GET /parvum.txt HTTP/1.0\r\n\r\n", XXVIII));
    tls_claudere(tr.connexio);
    servum_tls_claudere(&s);
    CREDO_NECESSE(servum_tls_incipere_cum(&s, l->dir, l->fixa,
        "alienus", "-tls1_3", c->piscina));
    tr = tls_connectere_cum_optionibus("localhost", s.portus, &opt,
        c->piscina);
    CREDO_FALSUM(tr.successus);
    CREDO_AEQUALIS_I32((i32)tr.error, (i32)TLS_ERROR_CERTIFICATUM);
    CREDO_VERUM(_continet(tr.error_descriptio,
        "certificatum reiectum"));
    servum_tls_claudere(&s);
}

/* tempus: servus auscultat sed numquam respondet -> TLS_ERROR_TCP
 * intra moram, numquam pendet */
interior vacuum
_sectio_tempus (CredoContextus* c)
{
    TcpServusOptiones so = tcp_servus_optiones_default();
    TcpServusResultus sr =
        tcp_servus_creare_cum_optionibus("127.0.0.1",
        ZEPHYRUM, &so, c->piscina);
          TlsOptiones opt = tls_optiones_default();
          TlsResultus tr;
                  s64 initium;

    CREDO_NECESSE(sr.successus);
    CREDO_NECESSE(tcp_servus_auscultare(sr.servus, IV) == TCP_OK);
    opt.vectura     = TLS_VECTURA_TLS13;
    opt.timeout_ms  = D;
    initium         = _ms_nunc();
    tr = tls_connectere_cum_optionibus("127.0.0.1",
        tcp_servus_obtinere_portum(sr.servus), &opt, c->piscina);
    CREDO_FALSUM(tr.successus);
    CREDO_AEQUALIS_I32((i32)tr.error, (i32)TLS_ERROR_TCP);
    CREDO_VERUM(_continet(tr.error_descriptio,
        "Tempus handshake excessum"));
    CREDO_VERUM(_ms_nunc() - initium < (s64)III * M);
    tcp_servus_claudere(sr.servus);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "parvum", _sectio_parvum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rsa", _sectio_rsa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "aes128", _sectio_aes128, NIHIL, NIHIL, NIHIL, NIHIL },
    { "aes256", _sectio_aes256, NIHIL, NIHIL, NIHIL, NIHIL },
    { "chacha", _sectio_chacha, NIHIL, NIHIL, NIHIL, NIHIL },
    { "hrr", _sectio_hrr, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cr_optionalis", _sectio_cr_optionalis, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "cr_necessaria", _sectio_cr_necessaria, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "magnum", _sectio_magnum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "alienus", _sectio_alienus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "expiratus", _sectio_expiratus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ignotus", _sectio_ignotus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine ancora", _sectio_sine_ancora, NIHIL, NIHIL, NIHIL, NIHIL },
    { "gradus", _sectio_gradus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "eof", _sectio_eof, NIHIL, NIHIL, NIHIL, NIHIL },
    { "omissa", _sectio_omissa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "clausa a pare", _sectio_clausa_a_pare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "obstruens", _sectio_obstruens, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tempus", _sectio_tempus, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("tls13_servus", SECTIONES,
        _suitam_parare, _suitam_purgare);
}
