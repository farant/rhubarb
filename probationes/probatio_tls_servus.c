/* probatio_tls_servus.c - TLS verum SINE rete: openssl s_server localis
 * (probationes/servus_tls.h) cum certificatis probationis IN TEMPORE
 * generatis (probationes/fixa/tls/generare.sh in TMPDIR - macOS
 * certificata ultra DCCCXXV dies reicit, ergo nulla committuntur; CA
 * propria ut ANCORA)
 *
 * Via gradatim (http_vocare https): pactio ad PERFECTUS, lectio
 * fasciculi parvi et MAGNI (CCLVI KB: recorda multa uno impetu -
 * lectio usque ad ITERUM), et verificatio: hospes alienus, expiratus,
 * CA ignota, sine ancora (repositorium systematis CA probationis
 * ignorat) - omnes errores nominati, celeres. Via obstruens
 * (tls_connectere cum ancora): eadem verificatio. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "reactor.h"
#include "http.h"
#include "http_vocatio.h"
#include "tls.h"
#include "filum.h"
#include "servus_tls.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#define MAGNUM (CCLVI * MXXIV)

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

/* octetus i fasciculi magni (exemplar sine periodo brevi) */
interior character
_magni_octetus (
    i32 i)
{
    redde (character)('a' + (i * VII + i / CCLVI) % XXVI);
}

/* fixura suitae: area credo (TMPDIR, post purgare tota deleta) -
 * certificata semel generata, fasciculi servi scripti; sectiones
 * c->suita (Locus) SOLUM legunt */
interior vacuum
_suitam_parare (
    CredoContextus* c)
{
                  Locus* l = (Locus*)piscina_allocare(c->piscina,
                      magnitudo(Locus));
      constans character* area = credo_area(c);
      constans character* argumenta[III];
               character  via[MXXIV];
                  chorda  magnum;
                     i32  i;
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
    sprintf(via, "%s/magnum.bin", l->dir);
    magnum.datum    = (i8*)piscina_allocare(c->piscina, MAGNUM);
    magnum.mensura  = MAGNUM;
    CREDO_NECESSE_NON_NIHIL(magnum.datum);
    per (i = ZEPHYRUM; i < MAGNUM; i++)
    {
        magnum.datum[i] = (i8)_magni_octetus(i);
    }
    CREDO_NECESSE(filum_scribere(via, magnum));
    c->datum = l;
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

interior vacuum
_sectio_parvum (
    CredoContextus* c)
{
           Reactor* r = reactor_creare(c->piscina);
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
        Observatio  o;
       HttpVocatio* v;
      HttpResultus  res;

    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa, "localhost",
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

/* CCLVI KB: recorda TLS multa - lectio usque ad ITERUM, nihil perditum
 */
interior vacuum
_sectio_magnum (
    CredoContextus* c)
{
           Reactor* r = reactor_creare(c->piscina);
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
        Observatio  o;
       HttpVocatio* v;
            chorda  corpus;
               i32  i;
               i32  mala = ZEPHYRUM;

    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa, "localhost",
        c->piscina));
    v = _petere(r, s.portus, "magnum.bin", l->ancora, &o, c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_AEQUALIS_I32((i32)http_vocatio_status(v),
        (i32)HTTP_VOCATIO_FINITA);
    corpus = chorda_aedificator_finire(o.corpus);
    CREDO_AEQUALIS_I32(corpus.mensura, MAGNUM);
    per (i = ZEPHYRUM; i < corpus.mensura && i < MAGNUM; i++)
    {
        si ((character)corpus.datum[i] != _magni_octetus(i))
        {
            mala++;
        }
    }
    CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
    servum_tls_claudere(&s);
}

/* certificatum reiectum: IRRITA, CONNEXIO, descriptio, celeriter */
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
    CREDO_VERUM(res.error_descriptio.mensura > ZEPHYRUM);
    CREDO_VERUM(_ms_nunc() - initium < II * M);
    CREDO_AEQUALIS_I32(chorda_aedificator_spectare(o.corpus).mensura,
        ZEPHYRUM);
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

/* sine ancora: repositorium systematis CA probationis ignorat -
 * verificatio VERE agit */
interior vacuum
_sectio_sine_ancora (
    CredoContextus* c)
{
    _reiectum(c, "localhost", FALSUM);
}

/* via obstruens (tls_connectere) cum ancora: localhost acceptum,
 * alienus TLS_ERROR_CERTIFICATUM */
interior vacuum
_sectio_obstruens (
    CredoContextus* c)
{
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
       TlsOptiones  opt = tls_optiones_default();
       TlsResultus  tr;

    opt.ancora_via = l->ancora;
    opt.timeout_ms = V * M;
    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa, "localhost",
        c->piscina));
    tr = tls_connectere_cum_optionibus("localhost", s.portus, &opt,
        c->piscina);
    CREDO_VERUM(tr.successus);
    si (tr.successus)
    {
        tls_claudere(tr.connexio);
    }
    servum_tls_claudere(&s);
    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa, "alienus",
        c->piscina));
    tr = tls_connectere_cum_optionibus("localhost", s.portus, &opt,
        c->piscina);
    CREDO_FALSUM(tr.successus);
    CREDO_AEQUALIS_I32((i32)tr.error, (i32)TLS_ERROR_CERTIFICATUM);
    servum_tls_claudere(&s);
}

/* certificatum hospitis: summarium folii ex connexione aperta (per
 * fiducia_folium_describere) */
interior vacuum
_sectio_certificatum (
    CredoContextus* c)
{
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
       TlsOptiones  opt = tls_optiones_default();
       TlsResultus  tr;
   TlsCertificatum  cert;
               s64  dies;

    opt.ancora_via = l->ancora;
    opt.timeout_ms = V * M;
    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa, "localhost",
        c->piscina));
    tr = tls_connectere_cum_optionibus("localhost", s.portus, &opt,
        c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_VERUM(tls_certificatum_obtinere(tr.connexio, &cert));
    CREDO_VERUM(cert.valida);
    CREDO_CHORDA_AEQUALIS_LITERIS(cert.subiectum, "localhost");
    dies = (cert.non_post - cert.non_ante) / ((s64)LXXXVI * M + CD);
    CREDO_VERUM(dies >= CCCLXIV && dies <= CCCLXVI);
    tls_claudere(tr.connexio);
    servum_tls_claudere(&s);
}

/* folium RSA-2048 (ECDHE_RSA; tls-bearssl R12): gradatim, 200 +
 * corpus */
interior vacuum
_sectio_rsa (
    CredoContextus* c)
{
           Reactor* r = reactor_creare(c->piscina);
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
        Observatio  o;
       HttpVocatio* v;
      HttpResultus  res;

    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa,
        "localhost_rsa",
        c->piscina));
    v = _petere(r, s.portus, "parvum.txt", l->ancora, &o, c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    res = http_vocatio_resultus(v);
    CREDO_AEQUALIS_I32((i32)http_vocatio_status(v),
        (i32)HTTP_VOCATIO_FINITA);
    CREDO_NECESSE(res.successus && res.responsum != NIHIL);
    CREDO_AEQUALIS_I32(res.responsum->status, CC);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(o.corpus),
        "salve per tls\n");
    servum_tls_claudere(&s);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "parvum", _sectio_parvum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "magnum", _sectio_magnum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "alienus", _sectio_alienus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "expiratus", _sectio_expiratus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ignotus", _sectio_ignotus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine ancora", _sectio_sine_ancora, NIHIL, NIHIL, NIHIL, NIHIL },
    { "obstruens", _sectio_obstruens, NIHIL, NIHIL, NIHIL, NIHIL },
    { "certificatum", _sectio_certificatum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "rsa", _sectio_rsa, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("tls_servus", SECTIONES,
        _suitam_parare, NIHIL);
}
