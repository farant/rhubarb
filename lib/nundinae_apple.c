/* nundinae_apple.c - cliens App Store Connect API (testa-ios C2 T1).
 * Vide include/nundinae_apple.h. Tessera ES256 per BearSSL (vendor),
 * petitiones per http_exsequi (obstruens: instrumentum lineae
 * mandatorum), JSON:API per json. Clavis secreta: lectio cruda (ut
 * horreum), modus 0600, DER in memoria solum, zephyrata in claudere. */
#include "postulata_posix.h"
/* ANTE latina.h: numeri Romani nomina BearSSL tangerent */
#include "../vendor/bearssl_amalgama.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include "base64.h"
#include "json.h"
#include "http.h"
#include "filum.h"
#include "nundinae_apple.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define CAPSA_CLAVIS  (IV * MXXIV)    /* octeti PEM summi */

hic_manens constans character* _radix_ordinaria =
    "https://api.appstoreconnect.apple.com";


/* ==================================================
 * Configuratio
 * ================================================== */

interior b32
_attributum (
    constans StmlNodus* nodus,
    constans character* titulus,
                chorda* valor,
                chorda* causa,
               Piscina* p)
{
    chorda* a;

    a = stml_attributum_capere(nodus, titulus);
    si (!a || a->mensura == ZEPHYRUM)
    {
        *causa = chorda_concatenare(chorda_ex_literis(
            "nundinae_apple: attributum absens: ", p),
            chorda_ex_literis(titulus, p), p);
        redde FALSUM;
    }
    *valor = *a;
    redde VERUM;
}

b32
nundinae_apple_configurationem_legere (
           constans character* via,
    NundinaeAppleConfiguratio* c,
                       chorda* causa,
                      Piscina* p)
{
                chorda  textus;
          StmlResultus  r;
    constans character* domus;

    memset(c, ZEPHYRUM, magnitudo(NundinaeAppleConfiguratio));
    textus = filum_legere_totum(via, p);
    si (textus.mensura == ZEPHYRUM)
    {
        *causa = chorda_concatenare(chorda_ex_literis(
            "nundinae_apple: configuratio legi nequit: ", p),
            chorda_ex_literis(via, p), p);
        redde FALSUM;
    }
    r = stml_legere(textus, p, internamentum_creare(p));
    si (!r.successus || !r.elementum_radix)
    {
        *causa =
            chorda_ex_literis("nundinae_apple: configuratio non STML",
            p);
        redde FALSUM;
    }
    si (   !_attributum(r.elementum_radix, "clavis_id", &c->clavis_id,
               causa, p)
        || !_attributum(r.elementum_radix, "emissor", &c->emissor,
        causa,
               p)
        || !_attributum(r.elementum_radix, "turma", &c->turma, causa, p)
        || !_attributum(r.elementum_radix, "via_clavis", &c->via_clavis,
               causa, p))
    {
        redde FALSUM;
    }
    domus = getenv("HOME");
    si (   domus && c->via_clavis.mensura >= II
        && c->via_clavis.datum[ZEPHYRUM] == '~'
        && c->via_clavis.datum[I]        == '/')
    {
        c->via_clavis = chorda_concatenare(chorda_ex_literis(domus, p),
            chorda_sectio(c->via_clavis, I, c->via_clavis.mensura), p);
    }
    redde VERUM;
}


/* ==================================================
 * Tessera ES256
 * ================================================== */

/* identificatores JSON sine effugio: litterae, numeri, - _ . */
interior b32
_tutum (
    chorda s)
{
    i32 i;

    si (s.mensura == ZEPHYRUM || s.mensura > CXXVIII)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        character k = (character)s.datum[i];

        si (!(   (k >= 'a' && k <= 'z') || (k >= 'A' && k <= 'Z')
              || (k >= '0' && k <= '9') || k == '-' || k == '_'
              || k == '.'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

chorda
nundinae_apple_tesseram_facere (
    constans i8* clavis_der,
            i32  mensura,
         chorda  clavis_id,
         chorda  emissor,
            s64  nunc_s,
            i32  duratio_s,
        Piscina* p)
{
    br_skey_decoder_context dc;
          br_sha256_context h;
              unsigned char digestum[XXXII];
              unsigned char signatura[LXIV];
                  character caput[CCLVI];
                  character asserta[DXII];
                     chorda vacua;
                     chorda initium;
                     chorda tessera;
                     size_t n;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (   !clavis_der || mensura <= ZEPHYRUM || !_tutum(clavis_id)
        || !_tutum(emissor) || duratio_s <= ZEPHYRUM)
    {
        redde vacua;
    }
    br_skey_decoder_init(&dc);
    br_skey_decoder_push(&dc, clavis_der, (size_t)mensura);
    si (   br_skey_decoder_last_error(&dc)    != ZEPHYRUM
        || br_skey_decoder_key_type(&dc)      != BR_KEYTYPE_EC
        || br_skey_decoder_get_ec(&dc)->curve != BR_EC_secp256r1)
    {
        memset(&dc, ZEPHYRUM, magnitudo(dc));
        redde vacua;
    }
    sprintf(caput,
        "{\"alg\":\"ES256\",\"kid\":\"%.*s\",\"typ\":\"JWT\"}",
        (int)clavis_id.mensura, (constans character*)clavis_id.datum);
    sprintf(asserta, "{\"iss\":\"%.*s\",\"iat\":%ld,\"exp\":%ld,"
        "\"aud\":\"appstoreconnect-v1\"}", (int)emissor.mensura,
        (constans character*)emissor.datum, (longus)nunc_s,
        (longus)(nunc_s + (s64)duratio_s));
    initium = chorda_concatenare(chorda_concatenare(
        base64url_codificare((constans i8*)caput, (i32)strlen(caput),
        p),
        chorda_ex_literis(".", p), p), base64url_codificare(
        (constans i8*)asserta, (i32)strlen(asserta), p), p);
    br_sha256_init(&h);
    br_sha256_update(&h, initium.datum, (size_t)initium.mensura);
    br_sha256_out(&h, digestum);
    n = br_ecdsa_i31_sign_raw(br_ec_get_default(), &br_sha256_vtable,
        digestum, br_skey_decoder_get_ec(&dc), signatura);
    memset(&dc, ZEPHYRUM, magnitudo(dc));
    si (n != (size_t)LXIV)
    {
        redde vacua;
    }
    tessera = chorda_concatenare(chorda_concatenare(initium,
        chorda_ex_literis(".", p), p), base64url_codificare(
        (constans i8*)signatura, LXIV, p), p);
    redde tessera;
}

b32
nundinae_apple_renovandum (
    s64 exspirat_s,
    s64 nunc_s)
{
    redde exspirat_s - nunc_s <= (s64)NUNDINAE_APPLE_RENOVATIO;
}

b32
nundinae_apple_iterandum (
    HttpMethodus methodus,
             i32 status,
             b32 rete_fractum)
{
    si (methodus != HTTP_GET)
    {
        redde FALSUM;
    }
    redde rete_fractum || status == CDXXIX
        || (status >= D && status <= DIV);
}


/* ==================================================
 * Causae et paginae
 * ================================================== */

chorda
nundinae_apple_causa_ex (
        i32  status,
     chorda  corpus,
    Piscina* p)
{
     JsonResultus  r;
        JsonValor* e;
        character  linea[CCCXX];
              i32  n;

    r = json_legere(corpus, p);
    e = r.successus ? json_via(r.radix, "errors.0") : NIHIL;
    si (e && json_est_objectum(e))
    {
        chorda vac  = chorda_ex_literis("", p);
        chorda st   = json_capere_chorda(e, "status", vac);
        chorda co   = json_capere_chorda(e, "code", vac);
        chorda ti   = json_capere_chorda(e, "title", vac);
        chorda de   = json_capere_chorda(e, "detail", vac);

        redde chorda_concatenare(chorda_concatenare(chorda_concatenare(
            st.mensura ? st : chorda_ex_literis("?", p),
            chorda_ex_literis(" ", p), p), chorda_concatenare(co,
            chorda_ex_literis(": ", p), p), p), chorda_concatenare(
            chorda_concatenare(ti, chorda_ex_literis(" - ", p), p), de,
            p),
            p);
    }
    n = corpus.mensura < NUNDINAE_APPLE_CAUSA_CORPUS ? corpus.mensura
        : NUNDINAE_APPLE_CAUSA_CORPUS;
    sprintf(linea, "%d: %.*s", (int)status, (int)n,
        corpus.datum ? (constans character*)corpus.datum : "");
    redde chorda_ex_literis(linea, p);
}

chorda
nundinae_apple_codex_ex (
     chorda  corpus,
    Piscina* p)
{
    JsonResultus  r;
       JsonValor* e;

    r = json_legere(corpus, p);
    e = r.successus ? json_via(r.radix, "errors.0") : NIHIL;
    redde e && json_est_objectum(e)
        ? json_capere_chorda(e, "code", chorda_ex_literis("", p))
        : chorda_ex_literis("", p);
}

interior constans character*
_methodus (
    HttpMethodus m)
{
    commutatio (m)
    {
        casus HTTP_GET:     redde "GET";
        casus HTTP_POST:    redde "POST";
        casus HTTP_PUT:     redde "PUT";
        casus HTTP_DELETE:  redde "DELETE";
        casus HTTP_PATCH:   redde "PATCH";
        casus HTTP_HEAD:    redde "HEAD";
        ordinarius:         redde "?";
    }
}

chorda
nundinae_apple_causam_componere (
    HttpMethodus  methodus,
          chorda  via,
          chorda  causa,
         Piscina* p)
{
    redde chorda_concatenare(chorda_concatenare(chorda_ex_literis(
        _methodus(methodus), p), chorda_concatenare(chorda_ex_literis(
        " ", p), via, p), p), chorda_concatenare(chorda_ex_literis(": ",
        p), causa, p), p);
}

chorda
nundinae_apple_proxima (
    JsonValor* responsum)
{
     JsonValor* n;
        chorda  vacua;

    vacua.datum = NIHIL;
    vacua.mensura = ZEPHYRUM;
    n = responsum ? json_via(responsum, "links.next") : NIHIL;
    redde n ? json_ad_chorda(n) : vacua;
}


/* ==================================================
 * Clavis (secretum)
 * ================================================== */

/* PEM PKCS#8 -> signa base64 (sine spatiis); numerus aut -I */
interior s32
_signa_clavis (
     constans character* textus_clavis,
                    s32  n,
              character* capsa,
                    s32  capacitas)
{
    constans character* initium;
    constans character* finis;
                   s32  k;

    initium  = strstr(textus_clavis, "-----BEGIN PRIVATE KEY-----");
    finis    = strstr(textus_clavis, "-----END PRIVATE KEY-----");
    (vacuum)n;
    si (!initium || !finis || finis < initium)
    {
        redde -I;
    }
    initium  += strlen("-----BEGIN PRIVATE KEY-----");
    k        = ZEPHYRUM;
    dum (initium < finis)
    {
        si (   *initium != '\n' && *initium != '\r' && *initium != ' '
            && *initium != '\t')
        {
            si (k >= capacitas)
            {
                redde -I;
            }
            capsa[k++] = *initium;
        }
        initium++;
    }
    redde k;
}

b32
nundinae_apple_aperire (
                         NundinaeApple* n,
    constans NundinaeAppleConfiguratio* c,
                                chorda* causa,
                               Piscina* p)
{
             integer fd;
      structura stat  st;
                    ssize_t lecta;
                  character textus_clavis[CAPSA_CLAVIS];
                  character signa[CAPSA_CLAVIS];
                  character via[DXII];
                        s32 k;
                     chorda b64;
              Base64Fructus der;
    br_skey_decoder_context dc;
                        b32 bona;

    memset(n, ZEPHYRUM, magnitudo(NundinaeApple));
    n->configuratio  = *c;
    n->radix_url     = _radix_ordinaria;
    n->piscina       = p;
    si (   c->via_clavis.mensura <= ZEPHYRUM
        || c->via_clavis.mensura >= DXII)
    {
        *causa = chorda_ex_literis("nundinae_apple: via clavis nulla",
            p);
        redde FALSUM;
    }
    memcpy(via, c->via_clavis.datum, (size_t)c->via_clavis.mensura);
    via[c->via_clavis.mensura] = '\0';
    /* <tolera codex="CANALIS_PRAETERITUS" (>clavis secreta Apple */
    fd = open(via, O_RDONLY | O_NOFOLLOW);
    si (fd < ZEPHYRUM)
    {
        *causa = chorda_concatenare(chorda_ex_literis(
            "nundinae_apple: clavis legi nequit: ", p),
            chorda_ex_literis(via, p), p);
        redde FALSUM;
    }
    si (fstat(fd, &st) != ZEPHYRUM || !S_ISREG(st.st_mode))
    {
        close(fd);
        *causa = chorda_ex_literis(
            "nundinae_apple: clavis non filum regulare", p);
        redde FALSUM;
    }
    si ((st.st_mode & 077) != ZEPHYRUM)
    {
        close(fd);
        *causa = chorda_concatenare(chorda_ex_literis(
            "nundinae_apple: clavis modus latior quam 0600 (chmod 600 ",
            p), chorda_concatenare(chorda_ex_literis(via, p),
            chorda_ex_literis(")", p), p), p);
        redde FALSUM;
    }
    lecta = read(fd, textus_clavis, magnitudo(textus_clavis) - I);
    close(fd);
    si (lecta <= ZEPHYRUM)
    {
        *causa = chorda_ex_literis("nundinae_apple: clavis vacua", p);
        redde FALSUM;
    }
    textus_clavis[lecta] = '\0';
    k = _signa_clavis(textus_clavis, (s32)lecta, signa,
        (s32)CAPSA_CLAVIS);
    memset(textus_clavis, ZEPHYRUM, magnitudo(textus_clavis));
    si (k <= ZEPHYRUM)
    {
        memset(signa, ZEPHYRUM, magnitudo(signa));
        *causa = chorda_ex_literis(
            "nundinae_apple: clavis non PEM 'PRIVATE KEY'", p);
        redde FALSUM;
    }
    b64.datum    = (i8*)signa;
    b64.mensura  = (i32)k;
    der          = base64_decodificare(b64, p);
    memset(signa, ZEPHYRUM, magnitudo(signa));
    si (!der.datum || der.mensura <= ZEPHYRUM)
    {
        *causa = chorda_ex_literis("nundinae_apple: clavis base64 mala",
            p);
        redde FALSUM;
    }
    br_skey_decoder_init(&dc);
    br_skey_decoder_push(&dc, der.datum, (size_t)der.mensura);
    bona = br_skey_decoder_last_error(&dc) == ZEPHYRUM
        && br_skey_decoder_key_type(&dc) == BR_KEYTYPE_EC
        && br_skey_decoder_get_ec(&dc)->curve == BR_EC_secp256r1;
    memset(&dc, ZEPHYRUM, magnitudo(dc));
    si (!bona)
    {
        memset(der.datum, ZEPHYRUM, (size_t)der.mensura);
        *causa = chorda_ex_literis(
            "nundinae_apple: clavis non PKCS#8 P-256", p);
        redde FALSUM;
    }
    n->clavis          = der.datum;
    n->clavis_mensura  = der.mensura;
    redde VERUM;
}

vacuum
nundinae_apple_claudere (
    NundinaeApple* n)
{
    si (n->clavis && n->clavis_mensura > ZEPHYRUM)
    {
        memset(n->clavis, ZEPHYRUM, (size_t)n->clavis_mensura);
    }
    si (n->tessera.datum && n->tessera.mensura > ZEPHYRUM)
    {
        memset(n->tessera.datum, ZEPHYRUM, (size_t)n->tessera.mensura);
    }
    n->clavis           = NIHIL;
    n->clavis_mensura   = ZEPHYRUM;
    n->tessera.datum    = NIHIL;
    n->tessera.mensura  = ZEPHYRUM;
    n->exspirat_s       = ZEPHYRUM;
}


/* ==================================================
 * Petitiones
 * ================================================== */

interior b32
_tesseram_parare (
    NundinaeApple* n,
           chorda* causa,
          Piscina* p)
{
    s64 nunc;

    nunc = (s64)time(NIHIL);
    si (   n->tessera.mensura > ZEPHYRUM
        && !nundinae_apple_renovandum(n->exspirat_s, nunc))
    {
        redde VERUM;
    }
    n->tessera = nundinae_apple_tesseram_facere(n->clavis,
        n->clavis_mensura, n->configuratio.clavis_id,
        n->configuratio.emissor, nunc, NUNDINAE_APPLE_DURATIO,
        n->piscina);
    si (n->tessera.mensura == ZEPHYRUM)
    {
        *causa =
            chorda_ex_literis("nundinae_apple: tessera fieri nequit",
            p);
        redde FALSUM;
    }
    n->exspirat_s = nunc + (s64)NUNDINAE_APPLE_DURATIO;
    redde VERUM;
}

NundinaeAppleResponsum
nundinae_apple_vocare (
         NundinaeApple* n,
          HttpMethodus  methodus,
    constans character* via,
                chorda  corpus_json,
               Piscina* p)
{
    NundinaeAppleResponsum  r;
               HttpPetitio* pet;
              HttpResultus  res;
                    chorda  url;
                    chorda  auctoritas;
                    chorda  repetere;
                    chorda  causa;
              JsonResultus  j;
                       i32  conatus;
                       s32  mora;
                       b32  fractus;
          structura timespec dormitio;
    hic_manens constans s32 morae[III] = { I, IV, XVI };

    memset(&r, ZEPHYRUM, magnitudo(r));
    si (!n->clavis || !via)
    {
        r.causa =
            chorda_ex_literis("nundinae_apple: cliens non apertus",
            p);
        redde r;
    }
    url = strncmp(via, "https://", VIII) == ZEPHYRUM
        ? chorda_ex_literis(via, p)
        : chorda_concatenare(chorda_ex_literis(n->radix_url, p),
              chorda_ex_literis(via, p), p);
    per (conatus = ZEPHYRUM; conatus
        < NUNDINAE_APPLE_CONATUS; conatus++)
    {
        si (!_tesseram_parare(n, &causa, p))
        {
            r.causa = causa;
            redde r;
        }
        auctoritas = chorda_concatenare(chorda_ex_literis("Bearer ", p),
            n->tessera, p);
        pet = http_petitio_creare(p, methodus, chorda_ut_cstr(url, p));
        si (!pet)
        {
            r.causa = chorda_ex_literis("nundinae_apple: petitio mala",
                p);
            redde r;
        }
        http_petitio_caput_addere(pet, "Authorization",
            chorda_ut_cstr(auctoritas, p));
        memset(auctoritas.datum, ZEPHYRUM, (size_t)auctoritas.mensura);
        si (corpus_json.mensura > ZEPHYRUM)
        {
            http_petitio_caput_addere(pet, "Content-Type",
                "application/json");
            http_petitio_corpus_ponere_chorda(pet, corpus_json);
        }
        res       = http_exsequi(pet, p);
        fractus   = !res.successus || !res.responsum;
        r.status  = fractus ? ZEPHYRUM : res.responsum->status;
        si (!fractus && r.status >= CC && r.status < CCC)
        {
            r.successus = VERUM;
            si (res.responsum->corpus.mensura > ZEPHYRUM)
            {
                j         = json_legere(res.responsum->corpus, p);
                r.corpus  = j.successus ? j.radix : NIHIL;
            }
            redde r;
        }
        causa = fractus ? res.error_descriptio
            : nundinae_apple_causa_ex(r.status, res.responsum->corpus,
            p);
        r.codex = fractus ? chorda_ex_literis("", p)
            : nundinae_apple_codex_ex(res.responsum->corpus, p);
        r.causa = nundinae_apple_causam_componere(methodus,
            chorda_ex_literis(via, p), causa, p);
        si (   conatus + I >= NUNDINAE_APPLE_CONATUS
            || !nundinae_apple_iterandum(methodus, r.status, fractus))
        {
            redde r;
        }
        mora = morae[conatus];
        si (!fractus)
        {
            repetere = http_responsum_caput(res.responsum,
                "Retry-After");
            si (repetere.mensura > ZEPHYRUM)
            {
                mora = atoi(chorda_ut_cstr(repetere, p));
                mora = mora < I ? I : (mora > LX ? LX : mora);
            }
        }
        dormitio.tv_sec   = (time_t)mora;
        dormitio.tv_nsec  = ZEPHYRUM;
        (vacuum)nanosleep(&dormitio, NIHIL);
    }
    redde r;
}


/* ==================================================
 * Status et res: applicationes
 * ================================================== */

b32
nundinae_apple_statum_probare (
             NundinaeApple* n,
    NundinaeAppleResponsum* r,
                   Piscina* p)
{
    *r = nundinae_apple_vocare(n, HTTP_GET, "/v1/apps?limit=1",
        chorda_ex_literis("", p), p);
    redde r->successus;
}

b32
nundinae_apple_applicationes (
    NundinaeApple* n,
              Xar* exitus,
           chorda* causa,
          Piscina* p)
{
    NundinaeAppleResponsum  r;
                    chorda  proxima;
                    chorda  vac;
                 JsonValor* data;
                       i32  i;
                       i32  numerus;
        constans character* via;

    vac = chorda_ex_literis("", p);
    via = "/v1/apps?limit=200&fields%5Bapps%5D=bundleId,name";
    dum (via)
    {
        r = nundinae_apple_vocare(n, HTTP_GET, via, vac, p);
        si (!r.successus)
        {
            *causa = r.causa;
            redde FALSUM;
        }
        data = r.corpus ? json_objectum_capere(r.corpus,
            "data") : NIHIL;
        numerus = data ? json_tabulatum_numerus(data) : ZEPHYRUM;
        per (i = ZEPHYRUM; i < numerus; i++)
        {
              JsonValor* d;
              JsonValor* a;
NundinaeAppleApplicatio* app;

            d    = json_tabulatum_obtinere(data, i);
            a    = d ? json_objectum_capere(d, "attributes") : NIHIL;
            app  = (NundinaeAppleApplicatio*)xar_addere(exitus);
            si (!app || !d)
            {
                perge;
            }
            app->id                = json_capere_chorda(d, "id", vac);
            app->signum_fasciculi  = a ? json_capere_chorda(a,
                "bundleId",
                vac) : vac;
            app->titulus           = a ? json_capere_chorda(a, "name",
                vac) : vac;
        }
        proxima = nundinae_apple_proxima(r.corpus);
        via = proxima.mensura > ZEPHYRUM ? chorda_ut_cstr(proxima, p)
                                         : NIHIL;
    }
    redde VERUM;
}
