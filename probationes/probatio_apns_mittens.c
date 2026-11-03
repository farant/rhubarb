/* probatio_apns_mittens.c - mittens APNs non obstruens (pulsus-plan T4).
 *
 * Per suturam vecturae (semper): aetas tesserae (XXXIX minuta eadem,
 * XL renovata), ExpiredProviderToken (renovatio semel), iterationes
 * (429, transportus, retentabilis, morae per indicem), iteratio finita
 * (500 x IV, Review Focus 1), signum mortuum (410, BadDeviceToken),
 * recusationes, claudere cum pendentibus, memoria stabilis.
 *
 * Contra servum Go (http2_servus, praefixa signi): quinque exitus,
 * capita et corpus a servo visa, connexio post mortem servi renata.
 * Sine 'go' sectiones oraculi non currunt (NIHIL CURSUM nominatum).
 * Clavis P-256 in probatione ex semine facta. */
#include "postulata_posix.h"
#include "../vendor/bearssl_amalgama.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "processus.h"
#include "reactor.h"
#include "hpack.h"
#include "http2_nexus.h"
#include "apns.h"
#include "apns_mittens.h"
#include "servus_oraculi.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define ROGATIONES_MAXIMAE CCLVI
#define CORPUS_PROBATIONIS \
    "{\"aps\":{\"alert\":{\"title\":\"pictor\",\"body\":\"Fran shared " \
    "a picture\"},\"sound\":\"default\"}}"
#define HORA_PRIMA_MS ((s64)1792800000 * M)

nomen structura {
    Http2NexusFinis  finis;
             vacuum* ctx;
          character  auctoritas[DXII];
          character  via[CCLVI];
          character  ambitus[XVI];
          character  thema[CXXVIII];
          character  genus[XVI];
          character  prioritas[XVI];
          character  terminus[XXXII];
          character  corpus[CCLVI];
} Rogatio;

nomen structura {
    PulsusExitus exitus;
       character signum[CCLVI];
       character ratio[CXXVIII];
       character apparatus[LXIV];
} Exitus;

nomen structura {
    character dir[DXII];
    character fixa[DXII];
    character ancora[MXXIV];
    character binarium[MXXIV];
} Locus;

hic_manens     Piscina* piscina;
hic_manens     Reactor* reactor;
hic_manens ApnsMittens* mittens;
hic_manens     Rogatio* rogationes;
hic_manens         i32  rogationes_numerus;
hic_manens      Exitus* exitus;
hic_manens         i32  exitus_numerus;
hic_manens         s64  nunc_fictum;
hic_manens          i8* clavis;
hic_manens         i32  clavis_mensura;


/* ==================================================
 * Auxilia
 * ================================================== */

interior s64
_nunc_fictum (vacuum)
{
    redde nunc_fictum;
}

interior s64
_ms (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior vacuum
_copiare (
    character* d,
          i32  n,
       chorda  s)
{
    i32 k;

    k = s.mensura < n - I ? s.mensura : n - I;
    si (k > ZEPHYRUM)
    {
        memcpy(d, s.datum, (size_t)k);
    }
    d[k] = '\0';
}

interior chorda
_chorda (
    character* s)
{
    chorda c;

    c.datum    = (i8*)s;
    c.mensura  = (i32)strlen(s);
    redde c;
}

/* signum LXIV hex: praefixum + '0' + ultimum signum */
interior vacuum
signum_facere (
              character* exitus_,
     constans character* praefixum,
              character  ultimum)
{
    i32 i;

    strcpy(exitus_, praefixum);
    per (i = (i32)strlen(praefixum); i < LXIII; i++)
    {
        exitus_[i] = '0';
    }
    exitus_[LXIII]  = ultimum;
    exitus_[LXIV]   = '\0';
}

/* clavis PKCS#8 DER P-256 ex semine */
interior b32
clavem_facere (
     Piscina* p)
{
    br_hmac_drbg_context  fors;
       br_ec_private_key  privata;
        br_ec_public_key  publica;
           unsigned char  capsa_privata[BR_EC_KBUF_PRIV_MAX_SIZE];
           unsigned char  capsa_publica[BR_EC_KBUF_PUB_MAX_SIZE];
      constans character* semen = "apns mittens semen";
                  size_t  n;

    br_hmac_drbg_init(&fors, &br_sha256_vtable, semen, strlen(semen));
    si (   br_ec_keygen(&fors.vtable, br_ec_get_default(), &privata,
            capsa_privata, BR_EC_secp256r1) == ZEPHYRUM
        || br_ec_compute_pub(br_ec_get_default(), &publica,
            capsa_publica, &privata) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    n       = br_encode_ec_pkcs8_der(NIHIL, &privata, &publica);
    clavis  = (i8*)piscina_allocare(p, (memoriae_index)n);
    si (   !clavis || br_encode_ec_pkcs8_der(clavis, &privata, &publica)
        != n)
    {
        redde FALSUM;
    }
    clavis_mensura = (i32)n;
    redde VERUM;
}

interior ApnsConfiguratio
configuratio (vacuum)
{
    ApnsConfiguratio cfg;

    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.clavis_id  = chorda_ex_literis("PROBA12345", piscina);
    cfg.turma      = chorda_ex_literis("TURMA12345", piscina);
    cfg.thema      = chorda_ex_literis("earth.golden.pictor", piscina);
    cfg.hospes     = chorda_ex_literis("localhost", piscina);
    cfg.portus     = CDXLIII;
    redde cfg;
}

/* vectura ficta: rogationes servat, respondere eas finit */
interior b32
_vectura (
                  vacuum* sui,
                  chorda  ambitus,
      constans character* via,
     constans HpackCaput* capita,
                     i32  numerus,
                  chorda  corpus,
         Http2NexusFinis  finis,
                  vacuum* ctx,
                 Piscina* p)
{
     Rogatio* r;
         i32  i;

    (vacuum)sui;
    (vacuum)p;
    si (   rogationes_numerus >= ROGATIONES_MAXIMAE
        || strlen(via)        >= CCLVI)
    {
        redde FALSUM;
    }
    r = &rogationes[rogationes_numerus++];
    memset(r, ZEPHYRUM, magnitudo(Rogatio));
    r->finis  = finis;
    r->ctx    = ctx;
    strcpy(r->via, via);
    _copiare(r->ambitus, XVI, ambitus);
    _copiare(r->corpus, CCLVI, corpus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (chorda_aequalis_literis(capita[i].titulus, "authorization"))
        {
            _copiare(r->auctoritas, DXII, capita[i].valor);
        }
        alioquin si (chorda_aequalis_literis(capita[i].titulus,
                     "apns-topic"))
        {
            _copiare(r->thema, CXXVIII, capita[i].valor);
        }
        alioquin si (chorda_aequalis_literis(capita[i].titulus,
                     "apns-push-type"))
        {
            _copiare(r->genus, XVI, capita[i].valor);
        }
        alioquin si (chorda_aequalis_literis(capita[i].titulus,
                     "apns-priority"))
        {
            _copiare(r->prioritas, XVI, capita[i].valor);
        }
        alioquin si (chorda_aequalis_literis(capita[i].titulus,
                     "apns-expiration"))
        {
            _copiare(r->terminus, XXXII, capita[i].valor);
        }
    }
    redde VERUM;
}

/* responsum rogationi i (nihil in piscina allocatur: memoria) */
interior vacuum
respondere (
                    i32  i,
                    b32  successus,
                    i32  status,
     constans character* ratio,
                    b32  retentabilis)
{
    Http2Responsum  r;
         character  corpus[CCLVI];
         character  causa[CCLVI];
           Rogatio* q;

    q = &rogationes[i];
    memset(&r, ZEPHYRUM, magnitudo(r));
    r.successus     = successus;
    r.status        = status;
    r.retentabilis  = retentabilis;
    si (successus && ratio)
    {
        sprintf(corpus, "{\"reason\":\"%s\"}", ratio);
        r.corpus = _chorda(corpus);
    }
    si (!successus)
    {
        strcpy(causa, ratio ? ratio : "connexio fracta");
        r.causa = _chorda(causa);
    }
    q->finis(q->ctx, &r);
}

interior vacuum
_finis (
                   vacuum* ctx,
    constans PulsusMissio* m,
             PulsusExitus  e,
                   chorda  ratio)
{
    Exitus* x;

    (vacuum)ctx;
    si (exitus_numerus >= ROGATIONES_MAXIMAE)
    {
        redde;
    }
    x          = &exitus[exitus_numerus++];
    x->exitus  = e;
    _copiare(x->signum, CCLVI, m->signum);
    _copiare(x->ratio, CXXVIII, ratio);
    _copiare(x->apparatus, LXIV, m->apparatus);
}

/* missio ex tamponibus localibus (post reditum deleti: mittens
 * copiare debet) */
interior b32
mittere (
     constans character* signum,
     constans character* ambitus)
{
    PulsusMissio m;
       character s[CCLVI];
       character a[XXXII];
       character t[XXXII];
       character k[CCLVI];
       character d[XXXII];
             b32 b;

    si (strlen(signum) >= CCLVI || strlen(ambitus) >= XXXII)
    {
        redde FALSUM;
    }
    strcpy(s, signum);
    strcpy(a, ambitus);
    strcpy(t, "earth.golden.pictor");
    strcpy(k, CORPUS_PROBATIONIS);
    strcpy(d, "A_TELEPHONUM");
    m.signum     = _chorda(s);
    m.ambitus    = _chorda(a);
    m.thema      = _chorda(t);
    m.corpus     = _chorda(k);
    m.apparatus  = _chorda(d);
    b            = apns_mittens_mittere(mittens, &m, _finis, NIHIL);
    memset(s, 'x', magnitudo(s) - I);
    memset(k, 'x', magnitudo(k) - I);
    redde b;
}

interior b32
exspectare_rogationes (
    i32 n)
{
    s64 terminus = _ms() + MM;

    dum (rogationes_numerus < n && _ms() < terminus)
    {
        (vacuum)reactor_poll(reactor, V);
    }
    redde rogationes_numerus >= n;
}

interior b32
exspectare_exitus (
    i32 n,
    i32 ms)
{
    s64 terminus = _ms() + (s64)ms;

    dum (exitus_numerus < n && _ms() < terminus)
    {
        (vacuum)reactor_poll(reactor, X);
    }
    redde exitus_numerus >= n;
}

interior vacuum
quiescere (
    i32 ms)
{
    s64 terminus = _ms() + (s64)ms;

    dum (_ms() < terminus)
    {
        (vacuum)reactor_poll(reactor, V);
    }
}

interior constans Exitus*
exitum_invenire (
    constans character* signum)
{
    i32 i;

    per (i = ZEPHYRUM; i < exitus_numerus; i++)
    {
        si (strcmp(exitus[i].signum, signum) == ZEPHYRUM)
        {
            redde &exitus[i];
        }
    }
    redde NIHIL;
}

interior vacuum
moras_breves (vacuum)
{
    hic_manens constans i32 MORAE[III] = { V, V, V };

    apns_mittentis_moras_ponere(mittens, MORAE, III);
}

interior vacuum
_parare (
    CredoContextus* c)
{
    ApnsConfiguratio cfg;

    piscina             = c->piscina;
    rogationes_numerus  = ZEPHYRUM;
    exitus_numerus      = ZEPHYRUM;
    nunc_fictum         = HORA_PRIMA_MS;
    rogationes          = (Rogatio*)piscina_allocare(piscina,
        (memoriae_index)(magnitudo(Rogatio) * ROGATIONES_MAXIMAE));
    exitus = (Exitus*)piscina_allocare(piscina,
        (memoriae_index)(magnitudo(Exitus) * ROGATIONES_MAXIMAE));
    CREDO_NECESSE(rogationes && exitus);
    CREDO_NECESSE(clavem_facere(piscina));
    reactor = reactor_creare(piscina);
    CREDO_NECESSE(reactor != NIHIL);
    cfg      = configuratio();
    mittens  = apns_mittentem_aperire(reactor, &cfg, clavis,
        clavis_mensura, _nunc_fictum, piscina);
    CREDO_NECESSE(mittens != NIHIL);
    apns_mittentis_vecturam_ponere(mittens, _vectura, NIHIL);
    moras_breves();
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    si (mittens)
    {
        apns_mittentem_claudere(mittens);
        mittens = NIHIL;
    }
    si (reactor)
    {
        reactor_destruere(reactor);
        reactor = NIHIL;
    }
    piscina = NIHIL;
}


/* ==================================================
 * Sectiones per vecturam fictam
 * ================================================== */

interior vacuum
_sectio_tessera (
    CredoContextus* c)
{
    character s[LXV];

    (vacuum)c;
    signum_facere(s, "c8", 'a');
    CREDO_GRADUS("capita spec III.1, via, corpus, ambitus");
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_rogationes(I));
    CREDO_VERUM(strncmp(rogationes[ZEPHYRUM].via, "/3/device/", X)
        == ZEPHYRUM);
    CREDO_LITERAE_AEQUALES(rogationes[ZEPHYRUM].via + X, s);
    CREDO_LITERAE_AEQUALES(rogationes[ZEPHYRUM].ambitus, "productio");
    CREDO_LITERAE_AEQUALES(rogationes[ZEPHYRUM].thema,
        "earth.golden.pictor");
    CREDO_LITERAE_AEQUALES(rogationes[ZEPHYRUM].genus, "alert");
    CREDO_LITERAE_AEQUALES(rogationes[ZEPHYRUM].prioritas, "10");
    CREDO_LITERAE_AEQUALES(rogationes[ZEPHYRUM].terminus,
        "1792886400");
    CREDO_LITERAE_AEQUALES(rogationes[ZEPHYRUM].corpus,
        CORPUS_PROBATIONIS);
    CREDO_VERUM(strncmp(rogationes[ZEPHYRUM].auctoritas, "bearer ey",
        IX) == ZEPHYRUM);
    CREDO_AEQUALIS_I32(apns_mittentis_pendentes(mittens), I);
    respondere(ZEPHYRUM, VERUM, CC, NIHIL, FALSUM);
    CREDO_AEQUALIS_I32(exitus_numerus, I);
    CREDO_AEQUALIS_I32((i32)exitus[ZEPHYRUM].exitus,
        (i32)PULSUS_TRADITUM);
    CREDO_LITERAE_AEQUALES(exitus[ZEPHYRUM].signum, s);
    CREDO_LITERAE_AEQUALES(exitus[ZEPHYRUM].apparatus, "A_TELEPHONUM");
    CREDO_AEQUALIS_I32(apns_mittentis_pendentes(mittens), ZEPHYRUM);

    CREDO_GRADUS("XXXIX minuta: tessera eadem");
    nunc_fictum = HORA_PRIMA_MS + (s64)XXXIX * LX * M;
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_rogationes(II));
    CREDO_LITERAE_AEQUALES(rogationes[I].auctoritas,
        rogationes[ZEPHYRUM].auctoritas);
    respondere(I, VERUM, CC, NIHIL, FALSUM);

    CREDO_GRADUS("XL minuta: tessera renovata");
    nunc_fictum = HORA_PRIMA_MS + APNS_TESSERA_AETAS_MS;
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_rogationes(III));
    CREDO_VERUM(strcmp(rogationes[II].auctoritas,
        rogationes[ZEPHYRUM].auctoritas) != ZEPHYRUM);
    CREDO_VERUM(strncmp(rogationes[II].auctoritas, "bearer ey", IX)
        == ZEPHYRUM);
    respondere(II, VERUM, CC, NIHIL, FALSUM);

    CREDO_GRADUS("200 numquam iteratur");
    quiescere(XXX);
    CREDO_AEQUALIS_I32(rogationes_numerus, III);
    CREDO_AEQUALIS_I32(exitus_numerus, III);
    CREDO_AEQUALIS_I32((i32)exitus[II].exitus, (i32)PULSUS_TRADITUM);
}

interior vacuum
_sectio_provisor (
    CredoContextus* c)
{
    character s[LXV];

    (vacuum)c;
    signum_facere(s, "c403", 'a');
    CREDO_GRADUS("ExpiredProviderToken: renovata, iterata, deinde 200");
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_rogationes(I));
    nunc_fictum += (s64)V * M;
    respondere(ZEPHYRUM, VERUM, CDIII, "ExpiredProviderToken", FALSUM);
    CREDO_VERUM(exspectare_rogationes(II));
    CREDO_VERUM(strcmp(rogationes[I].auctoritas,
        rogationes[ZEPHYRUM].auctoritas) != ZEPHYRUM);
    CREDO_LITERAE_AEQUALES(rogationes[I].via, rogationes[ZEPHYRUM].via);
    CREDO_AEQUALIS_I32(exitus_numerus, ZEPHYRUM);
    respondere(I, VERUM, CC, NIHIL, FALSUM);
    CREDO_AEQUALIS_I32(exitus_numerus, I);
    CREDO_AEQUALIS_I32((i32)exitus[ZEPHYRUM].exitus,
        (i32)PULSUS_TRADITUM);

    CREDO_GRADUS("renovatio SEMEL: 403 iterum -> DEFECTUS");
    signum_facere(s, "c403", 'b');
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_rogationes(III));
    nunc_fictum += (s64)V * M;
    respondere(II, VERUM, CDIII, "ExpiredProviderToken", FALSUM);
    CREDO_VERUM(exspectare_rogationes(IV));
    respondere(III, VERUM, CDIII, "ExpiredProviderToken", FALSUM);
    quiescere(XXX);
    CREDO_AEQUALIS_I32(rogationes_numerus, IV);
    CREDO_AEQUALIS_I32(exitus_numerus, II);
    CREDO_AEQUALIS_I32((i32)exitus[I].exitus, (i32)PULSUS_DEFECTUS);
    CREDO_LITERAE_AEQUALES(exitus[I].ratio, "ExpiredProviderToken");

    CREDO_GRADUS("InvalidProviderToken quoque semel renovat");
    signum_facere(s, "c403", 'c');
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_rogationes(V));
    nunc_fictum += (s64)V * M;
    respondere(IV, VERUM, CDIII, "InvalidProviderToken", FALSUM);
    CREDO_VERUM(exspectare_rogationes(VI));
    CREDO_VERUM(strcmp(rogationes[V].auctoritas,
        rogationes[IV].auctoritas) != ZEPHYRUM);
    respondere(V, VERUM, CC, NIHIL, FALSUM);
    CREDO_AEQUALIS_I32((i32)exitus[II].exitus, (i32)PULSUS_TRADITUM);
}

interior vacuum
_sectio_iteratio (
    CredoContextus* c)
{
    character s[LXV];

    (vacuum)c;
    signum_facere(s, "c429", 'a');
    CREDO_GRADUS("429, 429, 200 -> TRADITUM post duas iterationes");
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_rogationes(I));
    respondere(ZEPHYRUM, VERUM, CDXXIX, "TooManyRequests", FALSUM);
    CREDO_VERUM(exspectare_rogationes(II));
    CREDO_LITERAE_AEQUALES(rogationes[I].via + X, s);
    CREDO_LITERAE_AEQUALES(rogationes[I].corpus, CORPUS_PROBATIONIS);
    respondere(I, VERUM, CDXXIX, "TooManyRequests", FALSUM);
    CREDO_VERUM(exspectare_rogationes(III));
    CREDO_AEQUALIS_I32(exitus_numerus, ZEPHYRUM);
    respondere(II, VERUM, CC, NIHIL, FALSUM);
    CREDO_AEQUALIS_I32(exitus_numerus, I);
    CREDO_AEQUALIS_I32((i32)exitus[ZEPHYRUM].exitus,
        (i32)PULSUS_TRADITUM);
    CREDO_LITERAE_AEQUALES(exitus[ZEPHYRUM].signum, s);
    quiescere(XXX);
    CREDO_AEQUALIS_I32(rogationes_numerus, III);

    CREDO_GRADUS("transportus fractus, deinde 200");
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_rogationes(IV));
    respondere(III, FALSUM, ZEPHYRUM, "connexio fracta", FALSUM);
    CREDO_VERUM(exspectare_rogationes(V));
    respondere(IV, VERUM, CC, NIHIL, FALSUM);
    CREDO_AEQUALIS_I32((i32)exitus[I].exitus, (i32)PULSUS_TRADITUM);

    CREDO_GRADUS("retentabilis (GOAWAY), deinde 200");
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_rogationes(VI));
    respondere(V, FALSUM, ZEPHYRUM, "GOAWAY", VERUM);
    CREDO_VERUM(exspectare_rogationes(VII));
    respondere(VI, VERUM, CC, NIHIL, FALSUM);
    CREDO_AEQUALIS_I32((i32)exitus[II].exitus, (i32)PULSUS_TRADITUM);

    CREDO_GRADUS("morae nullae (n 0): 500 statim DEFECTUS");
    apns_mittentis_moras_ponere(mittens, NIHIL, ZEPHYRUM);
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_rogationes(VIII));
    respondere(VII, VERUM, D, "InternalServerError", FALSUM);
    CREDO_AEQUALIS_I32(exitus_numerus, IV);
    CREDO_AEQUALIS_I32((i32)exitus[III].exitus, (i32)PULSUS_DEFECTUS);
    quiescere(XXX);
    CREDO_AEQUALIS_I32(rogationes_numerus, VIII);

    CREDO_GRADUS("ratio: defectus localis 'apns: ' praefixum, Apple nudum");
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_rogationes(IX));
    respondere(VIII, FALSUM, ZEPHYRUM, "connexio fracta", FALSUM);
    CREDO_AEQUALIS_I32((i32)exitus[IV].exitus, (i32)PULSUS_DEFECTUS);
    CREDO_LITERAE_AEQUALES(exitus[IV].ratio,
        "apns: transportus - connexio fracta");
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_rogationes(X));
    respondere(IX, VERUM, D, NIHIL, FALSUM);
    CREDO_LITERAE_AEQUALES(exitus[V].ratio, "status 500");
    CREDO_LITERAE_AEQUALES(exitus[III].ratio, "InternalServerError");
}

interior vacuum
_sectio_finita (
    CredoContextus* c)
{
    hic_manens constans i32 MORAE[III] = { V, CCC, V };
                  character s[LXV];
                        i32 k;

    (vacuum)c;
    apns_mittentis_moras_ponere(mittens, MORAE, III);
    signum_facere(s, "c500", 'a');
    CREDO_GRADUS("500 x IV -> DEFECTUS post III iterationes");
    CREDO_VERUM(mittere(s, "productio"));
    per (k = ZEPHYRUM; k < IV; k++)
    {
        CREDO_VERUM(exspectare_rogationes(k + I));
        si (k == II)
        {
            CREDO_GRADUS("mora secunda (CCC ms) servata");
        }
        respondere(k, VERUM, D, "InternalServerError", FALSUM);
        si (k == I)
        {
            quiescere(C);
            CREDO_AEQUALIS_I32(rogationes_numerus, II);
        }
    }
    CREDO_AEQUALIS_I32(exitus_numerus, I);
    CREDO_AEQUALIS_I32((i32)exitus[ZEPHYRUM].exitus,
        (i32)PULSUS_DEFECTUS);
    CREDO_LITERAE_AEQUALES(exitus[ZEPHYRUM].ratio,
        "InternalServerError");
    quiescere(L);
    CREDO_AEQUALIS_I32(rogationes_numerus, IV);
    CREDO_AEQUALIS_I32(apns_mittentis_pendentes(mittens), ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(reactor), ZEPHYRUM);
}

interior vacuum
_sectio_mortuum (
    CredoContextus* c)
{
    character s[LXV];

    (vacuum)c;
    CREDO_GRADUS("410 Unregistered -> SIGNUM_MORTUUM");
    signum_facere(s, "c410", 'a');
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_rogationes(I));
    respondere(ZEPHYRUM, VERUM, CDX, "Unregistered", FALSUM);
    CREDO_AEQUALIS_I32((i32)exitus[ZEPHYRUM].exitus,
        (i32)PULSUS_SIGNUM_MORTUUM);
    CREDO_LITERAE_AEQUALES(exitus[ZEPHYRUM].ratio, "Unregistered");

    CREDO_GRADUS("400 BadDeviceToken -> SIGNUM_MORTUUM");
    signum_facere(s, "ab", 'b');
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_rogationes(II));
    respondere(I, VERUM, CD, "BadDeviceToken", FALSUM);
    CREDO_AEQUALIS_I32((i32)exitus[I].exitus,
        (i32)PULSUS_SIGNUM_MORTUUM);

    CREDO_GRADUS("400 BadTopic, 413 -> DEFECTUS sine iteratione");
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_rogationes(III));
    respondere(II, VERUM, CD, "BadTopic", FALSUM);
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_rogationes(IV));
    respondere(III, VERUM, CDXIII, "PayloadTooLarge", FALSUM);
    CREDO_AEQUALIS_I32((i32)exitus[II].exitus, (i32)PULSUS_DEFECTUS);
    CREDO_AEQUALIS_I32((i32)exitus[III].exitus, (i32)PULSUS_DEFECTUS);
    quiescere(XXX);
    CREDO_AEQUALIS_I32(rogationes_numerus, IV);
    CREDO_AEQUALIS_I32(exitus_numerus, IV);
}

interior vacuum
_sectio_recusatio (
    CredoContextus* c)
{
    ApnsConfiguratio cfg;
           character s[LXV];
                  i8 mala[XXXII];

    (vacuum)c;
    signum_facere(s, "c8", 'a');
    CREDO_GRADUS("sandbox sine hospes_sandbox, ambitus ignotus -> FALSUM");
    CREDO_FALSUM(mittere(s, "sandbox"));
    CREDO_FALSUM(mittere(s, "proba"));
    CREDO_GRADUS("signum vacuum, non hex, impar -> FALSUM");
    CREDO_FALSUM(mittere("", "productio"));
    CREDO_FALSUM(mittere("zz", "productio"));
    CREDO_FALSUM(mittere("abc", "productio"));
    quiescere(X);
    CREDO_AEQUALIS_I32(rogationes_numerus, ZEPHYRUM);
    CREDO_AEQUALIS_I32(exitus_numerus, ZEPHYRUM);
    CREDO_AEQUALIS_I32(apns_mittentis_pendentes(mittens), ZEPHYRUM);

    CREDO_GRADUS("cum hospes_sandbox: sandbox accipitur");
    apns_mittentem_claudere(mittens);
    cfg                 = configuratio();
    cfg.hospes_sandbox  = chorda_ex_literis("localhost", piscina);
    mittens = apns_mittentem_aperire(reactor, &cfg, clavis,
        clavis_mensura, _nunc_fictum, piscina);
    CREDO_NECESSE(mittens != NIHIL);
    apns_mittentis_vecturam_ponere(mittens, _vectura, NIHIL);
    CREDO_VERUM(mittere(s, "sandbox"));
    CREDO_VERUM(exspectare_rogationes(I));
    CREDO_LITERAE_AEQUALES(rogationes[ZEPHYRUM].ambitus, "sandbox");
    respondere(ZEPHYRUM, VERUM, CC, NIHIL, FALSUM);

    CREDO_GRADUS("clavis mala, clavis_id non tutum -> NIHIL");
    memset(mala, 0x30, magnitudo(mala));
    CREDO_VERUM(apns_mittentem_aperire(reactor, &cfg, mala, XXXII,
        _nunc_fictum, piscina) == NIHIL);
    cfg.clavis_id = chorda_ex_literis("a b", piscina);
    CREDO_VERUM(apns_mittentem_aperire(reactor, &cfg, clavis,
        clavis_mensura, _nunc_fictum, piscina) == NIHIL);
}

interior vacuum
_sectio_claudere (
    CredoContextus* c)
{
    hic_manens constans i32 MORAE[I] = { XXX * M };
                  character s[LXV];

    (vacuum)c;
    apns_mittentis_moras_ponere(mittens, MORAE, I);
    signum_facere(s, "c8", 'a');
    CREDO_GRADUS("duae in volatu, una in mora iterationis");
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_rogationes(III));
    respondere(II, VERUM, D, "InternalServerError", FALSUM);
    CREDO_AEQUALIS_I32(apns_mittentis_pendentes(mittens), III);
    CREDO_AEQUALIS_I32(exitus_numerus, ZEPHYRUM);
    CREDO_GRADUS("claudere: tres DEFECTUS ante reditum, horae cancellatae");
    apns_mittentem_claudere(mittens);
    mittens = NIHIL;
    CREDO_AEQUALIS_I32(exitus_numerus, III);
    CREDO_AEQUALIS_I32((i32)exitus[ZEPHYRUM].exitus,
        (i32)PULSUS_DEFECTUS);
    CREDO_AEQUALIS_I32((i32)exitus[I].exitus, (i32)PULSUS_DEFECTUS);
    CREDO_AEQUALIS_I32((i32)exitus[II].exitus, (i32)PULSUS_DEFECTUS);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(reactor), ZEPHYRUM);
}

interior vacuum
_sectio_memoria (
    CredoContextus* c)
{
         character s[LXV];
    memoriae_index usus;
               i32 k;

    (vacuum)c;
    signum_facere(s, "c8", 'a');
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_rogationes(I));
    respondere(ZEPHYRUM, VERUM, CC, NIHIL, FALSUM);
    usus = piscina_summa_usus(piscina);
    CREDO_GRADUS("C missiones (429 deinde 200): piscina vocantis stabilis");
    per (k = ZEPHYRUM; k < C; k++)
    {
        CREDO_NECESSE(mittere(s, "productio"));
        CREDO_NECESSE(exspectare_rogationes(II * k + II));
        respondere(II * k + I, VERUM, CDXXIX, "TooManyRequests",
            FALSUM);
        CREDO_NECESSE(exspectare_rogationes(II * k + III));
        respondere(II * k + II, VERUM, CC, NIHIL, FALSUM);
    }
    CREDO_AEQUALIS_I32(exitus_numerus, CI);
    CREDO_AEQUALIS_I32(apns_mittentis_pendentes(mittens), ZEPHYRUM);
    CREDO_VERUM(piscina_summa_usus(piscina) == usus);
}


/* ==================================================
 * Sectiones contra servum Go
 * ================================================== */

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
                  Locus* l = (Locus*)piscina_allocare(c->piscina,
                      magnitudo(Locus));
     constans character* area = credo_area(c);
     constans character* argumenta[III];
      ProcessusResultus  pr;

    CREDO_NECESSE(l && area && strlen(area) < CD);
    strcpy(l->dir, area);
    sprintf(l->fixa, "%s/certa", area);
    sprintf(l->ancora, "%s/ca.der", l->fixa);
    sprintf(l->binarium, "%s/http2_servus", area);
    argumenta[ZEPHYRUM] = "probationes/fixa/tls/generare.sh";
    argumenta[I] = l->fixa;
    argumenta[II] = NIHIL;
    pr = processus_exsequi(argumenta, XXX * M, c->piscina);
    CREDO_NECESSE(pr.successus && pr.codex_exitus == ZEPHYRUM);
    CREDO_NECESSE(servum_oraculi_construere(area, l->binarium,
        c->piscina));
    c->datum = l;
}

interior vacuum
_suitam_purgare (
    CredoContextus* c)
{
    (vacuum)c;
}

/* mittens verus (vectura NIHIL, horologium verum) contra servum */
interior vacuum
mittentem_verum_aperire (
    CredoContextus* c,
               i32  portus)
{
     hic_manens constans i32  MORAE[III]  = { L, L, L };
              constans Locus* l           = (constans Locus*)c->suita;
            ApnsConfiguratio  cfg;

    apns_mittentem_claudere(mittens);
    cfg         = configuratio();
    cfg.portus  = portus;
    cfg.ancora  = chorda_ex_literis(l->ancora, piscina);
    mittens = apns_mittentem_aperire(reactor, &cfg, clavis,
        clavis_mensura, NIHIL, piscina);
    CREDO_NECESSE(mittens != NIHIL);
    apns_mittentis_moras_ponere(mittens, MORAE, III);
}

interior i32
occursus (
    chorda textus,
    chorda acus)
{
    i32 n;
    i32 i;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; acus.mensura > ZEPHYRUM
        && i + acus.mensura <= textus.mensura; i++)
    {
        si (memcmp(textus.datum + i, acus.datum, (size_t)acus.mensura)
            == ZEPHYRUM)
        {
            n++;
        }
    }
    redde n;
}

interior vacuum
_sectio_oraculum (
    CredoContextus* c)
{
    hic_manens constans character* constans PRAEFIXA[V] = {
        "c8", "c410", "c429", "c403", "c500"
    };
    hic_manens constans i32 EXITUS[V] = {
        PULSUS_TRADITUM, PULSUS_SIGNUM_MORTUUM, PULSUS_TRADITUM,
        PULSUS_TRADITUM, PULSUS_DEFECTUS
    };
    hic_manens constans i32  VICES[V]  = { I, I, III, II, IV };
             constans Locus* l         = (constans Locus*)c->suita;
              ServusOraculi  sv;
                  character  via[MXXIV];
                  character  signa[V][LXV];
                  character  acus[CCCLX];
                     chorda  tabula;
                     chorda  post;
                        s64  terminus;
                        s64  nunc_s;
                        i32  k;
                        i32  i;

    sprintf(via, "%s/oraculum.go.log", l->dir);
    CREDO_NECESSE(servum_oraculi_incipere(&sv, l->binarium, l->fixa,
        via,
        "", piscina));
    mittentem_verum_aperire(c, sv.portus);
    CREDO_GRADUS("quinque praefixa: TRADITUM, MORTUUM, TRADITUM x2, DEFECTUS");
    per (k = ZEPHYRUM; k < V; k++)
    {
        signum_facere(signa[k], PRAEFIXA[k], 'f');
        CREDO_VERUM(mittere(signa[k], "productio"));
    }
    CREDO_VERUM(exspectare_exitus(V, XX * M));
    per (k = ZEPHYRUM; k < V; k++)
    {
        constans Exitus* x = exitum_invenire(signa[k]);

        CREDO_VERUM(x != NIHIL);
        si (x)
        {
            CREDO_AEQUALIS_I32((i32)x->exitus, EXITUS[k]);
        }
    }
    CREDO_AEQUALIS_I32(apns_mittentis_pendentes(mittens), ZEPHYRUM);
    nunc_s = _ms() / M;
    apns_mittentem_claudere(mittens);
    mittens = NIHIL;
    servum_oraculi_claudere(&sv);

    CREDO_GRADUS("servus vidit: thema, genus, prioritas, corpus, vices");
    tabula = filum_legere_totum(via, piscina);
    per (k = ZEPHYRUM; k < V; k++)
    {
        sprintf(acus, "apns: signum=%s thema=earth.golden.pictor"
            " genus=alert prioritas=10 expiratio=", signa[k]);
        CREDO_AEQUALIS_I32(occursus(tabula, chorda_ex_literis(acus,
            piscina)), VICES[k]);
    }
    sprintf(acus, "corpus=%s\n", CORPUS_PROBATIONIS);
    CREDO_AEQUALIS_I32(occursus(tabula, chorda_ex_literis(acus,
        piscina)), XI);

    CREDO_GRADUS("apns-expiration = nunc + dies");
    sprintf(acus, "apns: signum=%s thema=earth.golden.pictor"
        " genus=alert prioritas=10 expiratio=", signa[ZEPHYRUM]);
    post      = chorda_ex_literis(acus, piscina);
    terminus  = -I;
    per (i = ZEPHYRUM; i + post.mensura < tabula.mensura; i++)
    {
        si (memcmp(tabula.datum + i, post.datum, (size_t)post.mensura)
            == ZEPHYRUM)
        {
            terminus = (s64)strtol((constans character*)tabula.datum
                + i
                + post.mensura, NIHIL, X);
            frange;
        }
    }
    CREDO_VERUM(terminus >= nunc_s + LXXXVI * M + CD - XC
        && terminus <= nunc_s + LXXXVI * M + CD + X);

    CREDO_GRADUS("authorization numquam indicatum (tabula Go)");
    CREDO_VERUM(chorda_continet(tabula, chorda_ex_literis(
        "\"authorization\" = \"bearer ", piscina)));
    CREDO_VERUM(chorda_continet(tabula, chorda_ex_literis("(sensitive)",
        piscina)));
}

interior vacuum
_sectio_renata (
    CredoContextus* c)
{
     constans Locus* l = (constans Locus*)c->suita;
      ServusOraculi  sv;
          character  via[MXXIV];
          character  s[LXV];
                i32  portus;

    sprintf(via, "%s/renata.go.log", l->dir);
    CREDO_NECESSE(servum_oraculi_incipere(&sv, l->binarium, l->fixa,
        via,
        "", piscina));
    portus = sv.portus;
    mittentem_verum_aperire(c, portus);
    CREDO_GRADUS("prima missio in connexione prima");
    signum_facere(s, "c8", 'a');
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_exitus(I, X * M));
    CREDO_AEQUALIS_I32((i32)exitus[ZEPHYRUM].exitus,
        (i32)PULSUS_TRADITUM);

    CREDO_GRADUS("servus moritur et renascitur in eodem portu");
    servum_oraculi_claudere(&sv);
    quiescere(CCC);
    sprintf(via, "%s/renata2.go.log", l->dir);
    CREDO_NECESSE(servum_oraculi_incipere_portu(&sv, l->binarium,
        l->fixa, via, "", portus, piscina));
    signum_facere(s, "c8", 'b');
    CREDO_VERUM(mittere(s, "productio"));
    CREDO_VERUM(exspectare_exitus(II, X * M));
    CREDO_AEQUALIS_I32((i32)exitus[I].exitus, (i32)PULSUS_TRADITUM);
    apns_mittentem_claudere(mittens);
    mittens = NIHIL;
    servum_oraculi_claudere(&sv);
}

#define SECTIONES_FICTAE \
    { "tessera", _sectio_tessera, _parare, _purgare, NIHIL, NIHIL }, \
    { "provisor", _sectio_provisor, _parare, _purgare, NIHIL, NIHIL }, \
    { "iteratio", _sectio_iteratio, _parare, _purgare, NIHIL, NIHIL }, \
    { "iteratio finita", _sectio_finita, _parare, _purgare, NIHIL, \
        NIHIL }, \
    { "mortuum", _sectio_mortuum, _parare, _purgare, NIHIL, NIHIL }, \
    { "recusatio", _sectio_recusatio, _parare, _purgare, NIHIL, NIHIL }, \
    { "claudere", _sectio_claudere, _parare, _purgare, NIHIL, NIHIL }, \
    { "memoria", _sectio_memoria, _parare, _purgare, NIHIL, NIHIL }

hic_manens constans CredoSectio SECTIONES[] = {
    SECTIONES_FICTAE,
    { "oraculum", _sectio_oraculum, _parare, _purgare, NIHIL, NIHIL },
    { "renata", _sectio_renata, _parare, _purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

hic_manens constans CredoSectio SECTIONES_SINE_ORACULO[] = {
    SECTIONES_FICTAE,
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    si (oraculi_via_quaerere() == NIHIL)
    {
        imprimere("NIHIL CURSUM: go deest - sectiones oraculum et renata"
            " (oraculum http2_servus)\n");
        redde credo_suitam_currere("apns_mittens",
            SECTIONES_SINE_ORACULO);
    }
    redde credo_suitam_currere_cum("apns_mittens", SECTIONES,
        _suitam_parare, _suitam_purgare);
}
