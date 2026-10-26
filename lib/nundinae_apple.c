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
#include "chorda_aedificator.h"
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


/* ==================================================
 * Identitas distributionis (T2): clavis RSA, petitio PKCS#10
 * ================================================== */

interior i8*
_copia (
    constans unsigned char* d,
                    size_t  n,
                   Piscina* p)
{
    i8* c;

    c = (i8*)piscina_allocare(p,
        (memoriae_index)(n > ZEPHYRUM ? n : I));
    si (c && n > ZEPHYRUM)
    {
        memcpy(c, d, n);
    }
    redde c;
}

b32
nundinae_apple_clavem_rsa_generare (
    NundinaeAppleClavisRsa* k,
                   Piscina* p)
{
     br_hmac_drbg_context fors;
           br_prng_seeder seminator;
       br_rsa_private_key clavis_privata;
        br_rsa_public_key clavis_publica;
            unsigned char privata[BR_RSA_KBUF_PRIV_SIZE(MMXLVIII)];
            unsigned char publica[BR_RSA_KBUF_PUB_SIZE(MMXLVIII)];
                      b32 bona;

    memset(k, ZEPHYRUM, magnitudo(NundinaeAppleClavisRsa));
    seminator = br_prng_seeder_system(NIHIL);
    si (!seminator)
    {
        redde FALSUM;
    }
    br_hmac_drbg_init(&fors, &br_sha256_vtable, NIHIL, ZEPHYRUM);
    si (!seminator(&fors.vtable))
    {
        redde FALSUM;
    }
    si (br_rsa_keygen_get_default()(&fors.vtable, &clavis_privata,
        privata, &clavis_publica,
            publica, MMXLVIII, 65537) == ZEPHYRUM)
    {
        memset(privata, ZEPHYRUM, magnitudo(privata));
        redde FALSUM;
    }
    k->bits                 = (i32)clavis_privata.n_bitlen;
    k->modulus              = _copia(clavis_publica.n,
        clavis_publica.nlen, p);
    k->modulus_mensura      = (i32)clavis_publica.nlen;
    k->exponens             = _copia(clavis_publica.e,
        clavis_publica.elen, p);
    k->exponens_mensura     = (i32)clavis_publica.elen;
    k->primus_p             = _copia(clavis_privata.p,
        clavis_privata.plen, p);
    k->primus_p_mensura     = (i32)clavis_privata.plen;
    k->primus_q             = _copia(clavis_privata.q,
        clavis_privata.qlen, p);
    k->primus_q_mensura     = (i32)clavis_privata.qlen;
    k->exponens_p           = _copia(clavis_privata.dp,
        clavis_privata.dplen, p);
    k->exponens_p_mensura   = (i32)clavis_privata.dplen;
    k->exponens_q           = _copia(clavis_privata.dq,
        clavis_privata.dqlen, p);
    k->exponens_q_mensura   = (i32)clavis_privata.dqlen;
    k->coefficiens          = _copia(clavis_privata.iq,
        clavis_privata.iqlen, p);
    k->coefficiens_mensura  = (i32)clavis_privata.iqlen;
    memset(privata, ZEPHYRUM, magnitudo(privata));
    memset(&fors, ZEPHYRUM, magnitudo(fors));
    bona = k->modulus && k->exponens && k->primus_p && k->primus_q
        && k->exponens_p && k->exponens_q && k->coefficiens;
    si (!bona)
    {
        nundinae_apple_clavem_rsa_delere(k);
    }
    redde bona;
}

interior vacuum
_partem_delere (
    i8** d,
   i32*  n)
{
    si (*d && *n > ZEPHYRUM)
    {
        memset(*d, ZEPHYRUM, (size_t)*n);
    }
    *d = NIHIL;
    *n = ZEPHYRUM;
}

vacuum
nundinae_apple_clavem_rsa_delere (
    NundinaeAppleClavisRsa* k)
{
    _partem_delere(&k->modulus, &k->modulus_mensura);
    _partem_delere(&k->exponens, &k->exponens_mensura);
    _partem_delere(&k->primus_p, &k->primus_p_mensura);
    _partem_delere(&k->primus_q, &k->primus_q_mensura);
    _partem_delere(&k->exponens_p, &k->exponens_p_mensura);
    _partem_delere(&k->exponens_q, &k->exponens_q_mensura);
    _partem_delere(&k->coefficiens, &k->coefficiens_mensura);
    k->bits = ZEPHYRUM;
}

interior vacuum
_in_bearssl (
    constans NundinaeAppleClavisRsa* k,
                 br_rsa_private_key* clavis_privata,
                  br_rsa_public_key* clavis_publica)
{
    clavis_privata->n_bitlen  = (uint32_t)k->bits;
    clavis_privata->p         = k->primus_p;
    clavis_privata->plen      = (size_t)k->primus_p_mensura;
    clavis_privata->q         = k->primus_q;
    clavis_privata->qlen      = (size_t)k->primus_q_mensura;
    clavis_privata->dp        = k->exponens_p;
    clavis_privata->dplen     = (size_t)k->exponens_p_mensura;
    clavis_privata->dq        = k->exponens_q;
    clavis_privata->dqlen     = (size_t)k->exponens_q_mensura;
    clavis_privata->iq        = k->coefficiens;
    clavis_privata->iqlen     = (size_t)k->coefficiens_mensura;
    clavis_publica->n         = k->modulus;
    clavis_publica->nlen      = (size_t)k->modulus_mensura;
    clavis_publica->e         = k->exponens;
    clavis_publica->elen      = (size_t)k->exponens_mensura;
}

chorda
nundinae_apple_clavem_rsa_textum (
    constans NundinaeAppleClavisRsa* k,
                            Piscina* p)
{
    br_rsa_private_key  clavis_privata;
     br_rsa_public_key  clavis_publica;
         unsigned char  d[CCLVI];
                size_t  mensura_exponentis;
                size_t  n;
                    i8* der;
                chorda  b64;
                chorda  vacua;
     ChordaAedificator* a;
                   i32  i;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    _in_bearssl(k, &clavis_privata, &clavis_publica);
    mensura_exponentis = br_rsa_compute_privexp_get_default()(d,
        &clavis_privata, 65537);
    si (mensura_exponentis == ZEPHYRUM)
    {
        redde vacua;
    }
    n   = br_encode_rsa_raw_der(NIHIL, &clavis_privata, &clavis_publica,
        d, mensura_exponentis);
    der = (i8*)piscina_allocare(p, (memoriae_index)n);
    si (   !der || br_encode_rsa_raw_der(der, &clavis_privata,
        &clavis_publica, d, mensura_exponentis) != n)
    {
        memset(d, ZEPHYRUM, magnitudo(d));
        redde vacua;
    }
    memset(d, ZEPHYRUM, magnitudo(d));
    b64 = base64_codificare(der, (i32)n, p);
    memset(der, ZEPHYRUM, n);
    a = chorda_aedificator_creare(p, (i32)(b64.mensura + CXXVIII));
    chorda_aedificator_appendere_literis(a,
        "-----BEGIN RSA PRIVATE KEY-----\n");
    per (i = ZEPHYRUM; i < b64.mensura; i += LXIV)
    {
        chorda_aedificator_appendere_chorda(a, chorda_sectio(b64, i,
            i + LXIV < b64.mensura ? i + LXIV : b64.mensura));
        chorda_aedificator_appendere_literis(a, "\n");
    }
    chorda_aedificator_appendere_literis(a,
        "-----END RSA PRIVATE KEY-----\n");
    memset(b64.datum, ZEPHYRUM, (size_t)b64.mensura);
    redde chorda_aedificator_finire(a);
}

/* copia octetorum chordae in piscinam (chorda_concatenare fontes
 * acervi non tenet) */
interior chorda
_chorda_copia (
     chorda  c,
    Piscina* p)
{
    chorda r;

    r.datum    = (i8*)piscina_allocare(p, (memoriae_index)(c.mensura
        > ZEPHYRUM ? c.mensura : I));
    r.mensura  = r.datum ? c.mensura : ZEPHYRUM;
    si (r.datum && c.mensura > ZEPHYRUM)
    {
        memcpy(r.datum, c.datum, (size_t)c.mensura);
    }
    redde r;
}

/* DER: tag + longitudo + contentum */
interior chorda
_der (
      i8  tag,
  chorda  contentum,
 Piscina* p)
{
        i8 caput[V];
       i32 k;
    chorda c;

    caput[ZEPHYRUM] = tag;
    si (contentum.mensura < CXXVIII)
    {
        caput[I]  = (i8)contentum.mensura;
        k         = II;
    }
    alioquin si (contentum.mensura < CCLVI)
    {
        caput[I]   = 0x81;
        caput[II]  = (i8)contentum.mensura;
        k          = III;
    }
    alioquin
    {
        caput[I]    = 0x82;
        caput[II]   = (i8)((contentum.mensura >> VIII) & 0xFF);
        caput[III]  = (i8)(contentum.mensura & 0xFF);
        k           = IV;
    }
    c.datum    = caput;
    c.mensura  = k;
    redde chorda_concatenare(_chorda_copia(c, p), contentum, p);
}

/* INTEGER sine signo: zephyri praecedentes demuntur, 0x00 si bit
 * altum */
interior chorda
_der_integer (
    constans i8* v,
            i32  n,
        Piscina* p)
{
        i8* c;
       i32  i;
       i32  m;
    chorda  r;

    i = ZEPHYRUM;
    dum (i < n - I && v[i] == ZEPHYRUM)
    {
        i++;
    }
    m = n - i;
    c = (i8*)piscina_allocare(p, (memoriae_index)(m + I));
    si (!c)
    {
        r.datum    = NIHIL;
        r.mensura  = ZEPHYRUM;
        redde r;
    }
    si ((v[i] & 0x80) != ZEPHYRUM)
    {
        c[ZEPHYRUM] = ZEPHYRUM;
        memcpy(c + I, v + i, (size_t)m);
        m += I;
    }
    alioquin
    {
        memcpy(c, v + i, (size_t)m);
    }
    r.datum    = c;
    r.mensura  = m;
    redde _der(0x02, r, p);
}

interior chorda
_octeti (
    constans character* d,
                   i32  n,
               Piscina* p)
{
    chorda c;

    c.datum    = (i8*)piscina_allocare(p, (memoriae_index)n);
    c.mensura  = c.datum ? n : ZEPHYRUM;
    si (c.datum)
    {
        memcpy(c.datum, d, (size_t)n);
    }
    redde c;
}

chorda
nundinae_apple_petitionem_facere (
    constans NundinaeAppleClavisRsa* k,
                             chorda  nomen_commune,
                            Piscina* p)
{
    br_rsa_private_key clavis_privata;
     br_rsa_public_key clavis_publica;
     br_sha256_context h;
         unsigned char digestum[XXXII];
         unsigned char signatura[DXII];
                chorda nomen_c;
                chorda informatio_petitionis;
                chorda informatio_clavis;
                chorda vacua;
                chorda bits;
                    i8 nullum[I];

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (   !k->modulus || k->modulus_mensura > DXII
        || nomen_commune.mensura
        == ZEPHYRUM)
    {
        redde vacua;
    }
    nullum[ZEPHYRUM] = ZEPHYRUM;
    /* Name: SEQUENCE { SET { SEQUENCE { OID 2.5.4.3, UTF8String } }
     * } */
    nomen_c = _der(0x30, _der(0x31, _der(0x30, chorda_concatenare(
        _octeti("\x06\x03\x55\x04\x03", V, p), _der(0x0C, nomen_commune,
        p), p), p), p), p);
    /* SubjectPublicKeyInfo: rsaEncryption + BIT STRING {
     * RSAPublicKey } */
    bits = chorda_concatenare(_octeti("\x00", I, p), _der(0x30,
        chorda_concatenare(_der_integer(k->modulus, k->modulus_mensura,
            p),
        _der_integer(k->exponens, k->exponens_mensura, p), p), p), p);
    informatio_clavis = _der(0x30, chorda_concatenare(_octeti(
        "\x30\x0D\x06\x09\x2A\x86\x48\x86\xF7\x0D\x01\x01\x01\x05\x00",
        XV, p), _der(0x03, bits, p), p), p);
    /* CertificationRequestInfo: versio 0, nomen, informatio_clavis,
     * attributa [0] {} */
    informatio_petitionis = _der(0x30,
        chorda_concatenare(chorda_concatenare(_octeti(
        "\x02\x01\x00", III, p), nomen_c, p),
            chorda_concatenare(informatio_clavis,
        _octeti("\xA0\x00", II, p), p), p), p);
    br_sha256_init(&h);
    br_sha256_update(&h, informatio_petitionis.datum,
        (size_t)informatio_petitionis.mensura);
    br_sha256_out(&h, digestum);
    _in_bearssl(k, &clavis_privata, &clavis_publica);
    si (br_rsa_pkcs1_sign_get_default()(BR_HASH_OID_SHA256, digestum,
            XXXII, &clavis_privata, signatura) != I)
    {
        redde vacua;
    }
    (vacuum)nullum;
    redde _der(0x30,
        chorda_concatenare(chorda_concatenare(informatio_petitionis,
        _octeti(
        "\x30\x0D\x06\x09\x2A\x86\x48\x86\xF7\x0D\x01\x01\x0B\x05\x00",
        XV, p), p), _der(0x03, chorda_concatenare(_octeti("\x00", I, p),
        _octeti((constans character*)signatura, k->modulus_mensura, p),
        p), p), p), p);
}


/* ==================================================
 * Responsa T2 (purae)
 * ================================================== */

b32
nundinae_apple_certificata_legere (
    JsonValor* responsum,
          Xar* exitus,
      Piscina* p)
{
     JsonValor* data;
        chorda  vac;
           i32  i;

    data = responsum ? json_objectum_capere(responsum, "data") : NIHIL;
    si (!data)
    {
        redde FALSUM;
    }
    vac = chorda_ex_literis("", p);
    per (i = ZEPHYRUM; i < json_tabulatum_numerus(data); i++)
    {
                    JsonValor* d;
                    JsonValor* a;
    NundinaeAppleCertificatum* c;

        d = json_tabulatum_obtinere(data, i);
        a = d ? json_objectum_capere(d, "attributes") : NIHIL;
        si (   !a || !chorda_aequalis_literis(json_capere_chorda(a,
                "certificateType", vac), "DISTRIBUTION"))
        {
            perge;
        }
        c = (NundinaeAppleCertificatum*)xar_addere(exitus);
        si (!c)
        {
            redde FALSUM;
        }
        c->id                = json_capere_chorda(d, "id", vac);
        c->numerus_serialis  = json_capere_chorda(a, "serialNumber",
            vac);
        c->titulus           = json_capere_chorda(a, "displayName",
            vac);
        c->exspirat          = json_capere_chorda(a, "expirationDate",
            vac);
        c->contentum         = json_capere_chorda(a,
            "certificateContent",
            vac);
    }
    redde VERUM;
}

chorda
nundinae_apple_signum_invenire (
     JsonValor* responsum,
        chorda  identificator,
       Piscina* p)
{
     JsonValor* data;
        chorda  vac;
           i32  i;

    vac   = chorda_ex_literis("", p);
    data  = responsum ? json_objectum_capere(responsum, "data") : NIHIL;
    per (i = ZEPHYRUM; data && i < json_tabulatum_numerus(data); i++)
    {
        JsonValor* d;
        JsonValor* a;

        d = json_tabulatum_obtinere(data, i);
        a = d ? json_objectum_capere(d, "attributes") : NIHIL;
        si (   a && chorda_aequalis(json_capere_chorda(a, "identifier",
            vac),
                identificator))
        {
            redde json_capere_chorda(d, "id", vac);
        }
    }
    redde vac;
}

chorda
nundinae_apple_digestum_sha1 (
     chorda  datum,
    Piscina* p)
{
       br_sha1_context  h;
         unsigned char  d[XX];
             character  hex[XLI];
                   i32  i;
    constans character* digiti = "0123456789ABCDEF";

    br_sha1_init(&h);
    br_sha1_update(&h, datum.datum, (size_t)datum.mensura);
    br_sha1_out(&h, d);
    per (i = ZEPHYRUM; i < XX; i++)
    {
        hex[i * II]      = digiti[(d[i] >> IV) & 0xF];
        hex[i * II + I]  = digiti[d[i] & 0xF];
    }
    hex[XL] = '\0';
    redde chorda_ex_literis(hex, p);
}


/* ==================================================
 * Res rationis (T2; rete)
 * ================================================== */

/* {"data":{"type":T,"attributes":A[,"relationships":R]}} */
interior chorda
_corpus_rei (
    constans character* typus,
             JsonValor* attributa,
             JsonValor* relationes,
               Piscina* p)
{
    JsonValor* radix;
    JsonValor* data;

    data = json_objectum_creare(p);
    json_objectum_ponere(data, "type", json_chorda_creare_literis(p,
        typus));
    si (attributa)
    {
        json_objectum_ponere(data, "attributes", attributa);
    }
    si (relationes)
    {
        json_objectum_ponere(data, "relationships", relationes);
    }
    radix = json_objectum_creare(p);
    json_objectum_ponere(radix, "data", data);
    redde json_scribere(radix, p);
}

/* {"data":{"type":T,"id":ID}} aut {"data":[{...}]} */
interior JsonValor*
_relatio (
     constans character* typus,
                 chorda  id,
                    b32  plures,
                Piscina* p)
{
    JsonValor* r;
    JsonValor* res;
    JsonValor* tab;

    res = json_objectum_creare(p);
    json_objectum_ponere(res, "type", json_chorda_creare_literis(p,
        typus));
    json_objectum_ponere(res, "id", json_chorda_creare(p, id));
    r = json_objectum_creare(p);
    si (plures)
    {
        tab = json_tabulatum_creare(p);
        json_tabulatum_addere(tab, res);
        json_objectum_ponere(r, "data", tab);
    }
    alioquin
    {
        json_objectum_ponere(r, "data", res);
    }
    redde r;
}

/* valor pro URL: litterae, numeri, - _ . ~ manent, cetera %XX */
interior chorda
_url_valor (
     chorda  v,
    Piscina* p)
{
     ChordaAedificator* a;
                   i32  i;
             character  t[IV];
    constans character* hex = "0123456789ABCDEF";

    a = chorda_aedificator_creare(p, v.mensura * III + I);
    per (i = ZEPHYRUM; i < v.mensura; i++)
    {
        character k = (character)v.datum[i];

        si (   (k >= 'a' && k <= 'z') || (k >= 'A' && k <= 'Z')
            || (k >= '0' && k <= '9') || k == '-' || k == '_' || k
            == '.'
            || k == '~')
        {
            t[ZEPHYRUM]  = k;
            t[I]         = '\0';
        }
        alioquin
        {
            t[ZEPHYRUM]  = '%';
            t[I]         = hex[((i8)k >> IV) & 0xF];
            t[II]        = hex[(i8)k & 0xF];
            t[III]       = '\0';
        }
        chorda_aedificator_appendere_literis(a, t);
    }
    redde chorda_aedificator_finire(a);
}

interior constans character*
_via (
               Piscina* p,
    constans character* forma,
                chorda  a,
                chorda  b)
{
     ChordaAedificator* aed;
    constans character* s;

    aed = chorda_aedificator_creare(p, CCLVI);
    per (s = forma; *s; s++)
    {
        si (*s == '%' && s[I] == 'A')
        {
            chorda_aedificator_appendere_chorda(aed, a);
            s++;
        }
        alioquin si (*s == '%' && s[I] == 'B')
        {
            chorda_aedificator_appendere_chorda(aed, b);
            s++;
        }
        alioquin
        {
            character u[II];

            u[ZEPHYRUM]  = *s;
            u[I]         = '\0';
            chorda_aedificator_appendere_literis(aed, u);
        }
    }
    redde chorda_ut_cstr(chorda_aedificator_finire(aed), p);
}

interior b32
_vocatio_bona (
    NundinaeAppleResponsum  r,
                    chorda* causa)
{
    si (!r.successus)
    {
        *causa = r.causa;
        redde FALSUM;
    }
    redde VERUM;
}

b32
nundinae_apple_signum_fasciculi (
    NundinaeApple* n,
           chorda  identificator,
           chorda  titulus,
           chorda* id,
              b32* creatum,
           chorda* causa,
          Piscina* p)
{
    NundinaeAppleResponsum  r;
                 JsonValor* a;
                    chorda  vac;

    vac = chorda_ex_literis("", p);
    si (creatum)
    {
        *creatum = FALSUM;
    }
    r = nundinae_apple_vocare(n, HTTP_GET, _via(p,
        "/v1/bundleIds?filter%5Bidentifier%5D=%A&limit=200",
        _url_valor(identificator, p), vac), vac, p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    *id = nundinae_apple_signum_invenire(r.corpus, identificator, p);
    si (id->mensura > ZEPHYRUM)
    {
        redde VERUM;
    }
    a = json_objectum_creare(p);
    json_objectum_ponere(a, "identifier", json_chorda_creare(p,
        identificator));
    json_objectum_ponere(a, "name", json_chorda_creare(p, titulus));
    json_objectum_ponere(a, "platform", json_chorda_creare_literis(p,
        "IOS"));
    r = nundinae_apple_vocare(n, HTTP_POST, "/v1/bundleIds",
        _corpus_rei("bundleIds", a, NIHIL, p), p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    *id = json_capere_chorda(json_objectum_capere(r.corpus, "data"),
        "id",
        vac);
    si (creatum)
    {
        *creatum = VERUM;
    }
    redde id->mensura > ZEPHYRUM;
}

b32
nundinae_apple_certificata_distributionis (
    NundinaeApple* n,
              Xar* exitus,
           chorda* causa,
          Piscina* p)
{
    NundinaeAppleResponsum r;

    r = nundinae_apple_vocare(n, HTTP_GET,
        "/v1/certificates?filter%5BcertificateType%5D=DISTRIBUTION"
        "&limit=200", chorda_ex_literis("", p), p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    redde nundinae_apple_certificata_legere(r.corpus, exitus, p);
}

b32
nundinae_apple_certificatum_creare (
                NundinaeApple* n,
                       chorda  petitio_der,
    NundinaeAppleCertificatum* exitus,
                       chorda* causa,
                      Piscina* p)
{
    NundinaeAppleResponsum  r;
                 JsonValor* a;
                 JsonValor* involucrum;
                 JsonValor* tab;
                       Xar* unum;

    a = json_objectum_creare(p);
    json_objectum_ponere(a, "certificateType",
        json_chorda_creare_literis(p,
        "DISTRIBUTION"));
    json_objectum_ponere(a, "csrContent", json_chorda_creare(p,
        base64_codificare(petitio_der.datum, petitio_der.mensura, p)));
    r = nundinae_apple_vocare(n, HTTP_POST, "/v1/certificates",
        _corpus_rei("certificates", a, NIHIL, p), p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    /* responsum unius rei -> forma tabulati pro certificata_legere */
    tab = json_tabulatum_creare(p);
    json_tabulatum_addere(tab, json_objectum_capere(r.corpus, "data"));
    involucrum = json_objectum_creare(p);
    json_objectum_ponere(involucrum, "data", tab);
    unum = xar_creare(p, (i32)magnitudo(NundinaeAppleCertificatum));
    si (   !nundinae_apple_certificata_legere(involucrum, unum, p)
        || xar_numerus(unum) != I)
    {
        *causa = chorda_ex_literis(
            "nundinae_apple: certificatum creatum sed responsum "
                "ignotum", p);
        redde FALSUM;
    }
    *exitus = *(NundinaeAppleCertificatum*)xar_obtinere(unum, ZEPHYRUM);
    redde VERUM;
}

/* diploma 'id_diplomatis' certificatum 'id_certificati_quaesiti'
 * continet? */
interior b32
_diploma_certificatum_habet (
    NundinaeApple* n,
           chorda  id_diplomatis,
           chorda  id_certificati_quaesiti,
           chorda* causa,
          Piscina* p)
{
    NundinaeAppleResponsum  r;
                 JsonValor* data;
                       i32  i;

    r = nundinae_apple_vocare(n, HTTP_GET, _via(p,
        "/v1/profiles/%A/certificates?limit=200", id_diplomatis,
            id_certificati_quaesiti),
        chorda_ex_literis("", p), p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    data = json_objectum_capere(r.corpus, "data");
    per (i = ZEPHYRUM; data && i < json_tabulatum_numerus(data); i++)
    {
        JsonValor* d;

        d = json_tabulatum_obtinere(data, i);
        si (chorda_aequalis(json_capere_chorda(d, "id",
                chorda_ex_literis("", p)), id_certificati_quaesiti))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
nundinae_apple_diploma (
    NundinaeApple* n,
           chorda  id_signi,
           chorda  id_certificati,
           chorda  titulus,
           chorda* contentum,
              b32* creatum,
           chorda* causa,
          Piscina* p)
{
    NundinaeAppleResponsum  r;
                 JsonValor* data;
                 JsonValor* a;
                 JsonValor* rel;
                    chorda  vac;
                       i32  i;

    vac = chorda_ex_literis("", p);
    si (creatum)
    {
        *creatum = FALSUM;
    }
    r = nundinae_apple_vocare(n, HTTP_GET, _via(p,
        "/v1/bundleIds/%A/profiles?limit=200", id_signi, vac), vac, p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    data = json_objectum_capere(r.corpus, "data");
    per (i = ZEPHYRUM; data && i < json_tabulatum_numerus(data); i++)
    {
        JsonValor* d;

        d = json_tabulatum_obtinere(data, i);
        a = d ? json_objectum_capere(d, "attributes") : NIHIL;
        si (   a && chorda_aequalis_literis(json_capere_chorda(a,
                   "profileType", vac), "IOS_APP_STORE")
            && chorda_aequalis_literis(json_capere_chorda(a,
                   "profileState", vac), "ACTIVE")
            && _diploma_certificatum_habet(n, json_capere_chorda(d,
                "id",
                   vac), id_certificati, causa, p))
        {
            *contentum = json_capere_chorda(a, "profileContent", vac);
            redde contentum->mensura > ZEPHYRUM;
        }
    }
    a = json_objectum_creare(p);
    json_objectum_ponere(a, "name", json_chorda_creare(p, titulus));
    json_objectum_ponere(a, "profileType", json_chorda_creare_literis(p,
        "IOS_APP_STORE"));
    rel = json_objectum_creare(p);
    json_objectum_ponere(rel, "bundleId", _relatio("bundleIds",
        id_signi,
        FALSUM, p));
    json_objectum_ponere(rel, "certificates", _relatio("certificates",
        id_certificati, VERUM, p));
    r = nundinae_apple_vocare(n, HTTP_POST, "/v1/profiles",
        _corpus_rei("profiles", a, rel, p), p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    *contentum =
        json_capere_chorda(json_objectum_capere(json_objectum_capere(
        r.corpus, "data"), "attributes"), "profileContent", vac);
    si (creatum)
    {
        *creatum = VERUM;
    }
    redde contentum->mensura > ZEPHYRUM;
}

b32
nundinae_apple_applicatio (
    NundinaeApple* n,
           chorda  signum,
           chorda* id,
           chorda* causa,
          Piscina* p)
{
    NundinaeAppleResponsum  r;
                 JsonValor* data;
                    chorda  vac;
                       i32  i;

    vac = chorda_ex_literis("", p);
    r = nundinae_apple_vocare(n, HTTP_GET, _via(p,
        "/v1/apps?filter%5BbundleId%5D=%A&limit=200", _url_valor(signum,
        p), vac), vac, p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    data = json_objectum_capere(r.corpus, "data");
    per (i = ZEPHYRUM; data && i < json_tabulatum_numerus(data); i++)
    {
        JsonValor* d;

        d = json_tabulatum_obtinere(data, i);
        si (chorda_aequalis(json_capere_chorda(json_objectum_capere(d,
                "attributes"), "bundleId", vac), signum))
        {
            *id = json_capere_chorda(d, "id", vac);
            redde VERUM;
        }
    }
    *causa = chorda_concatenare(chorda_concatenare(chorda_ex_literis(
        "nundinae_apple: applicatio non exstat - crea eam in App Store "
        "Connect (Apps > + > New App), bundle ID ", p), signum, p),
        chorda_ex_literis(" (API applicationes creare nequit)", p), p);
    redde FALSUM;
}

b32
nundinae_apple_grex (
    NundinaeApple* n,
           chorda  id_applicationis,
           chorda  titulus,
           chorda* id,
              b32* creatum,
           chorda* causa,
          Piscina* p)
{
    NundinaeAppleResponsum  r;
                 JsonValor* data;
                 JsonValor* a;
                 JsonValor* rel;
                    chorda  vac;
                       i32  i;

    vac = chorda_ex_literis("", p);
    si (creatum)
    {
        *creatum = FALSUM;
    }
    r = nundinae_apple_vocare(n, HTTP_GET, _via(p,
        "/v1/apps/%A/betaGroups?limit=200", id_applicationis, vac), vac,
        p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    data = json_objectum_capere(r.corpus, "data");
    per (i = ZEPHYRUM; data && i < json_tabulatum_numerus(data); i++)
    {
        JsonValor* d;

        d = json_tabulatum_obtinere(data, i);
        a = json_objectum_capere(d, "attributes");
        si (   chorda_aequalis(json_capere_chorda(a, "name", vac),
            titulus)
            && json_ad_boolean(json_objectum_capere(a,
                "isInternalGroup")))
        {
            *id = json_capere_chorda(d, "id", vac);
            redde VERUM;
        }
    }
    a = json_objectum_creare(p);
    json_objectum_ponere(a, "name", json_chorda_creare(p, titulus));
    json_objectum_ponere(a, "isInternalGroup", json_boolean_creare(p,
        VERUM));
    rel = json_objectum_creare(p);
    json_objectum_ponere(rel, "app", _relatio("apps", id_applicationis,
        FALSUM, p));
    r = nundinae_apple_vocare(n, HTTP_POST, "/v1/betaGroups",
        _corpus_rei("betaGroups", a, rel, p), p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    *id = json_capere_chorda(json_objectum_capere(r.corpus, "data"),
        "id",
        vac);
    si (creatum)
    {
        *creatum = VERUM;
    }
    redde id->mensura > ZEPHYRUM;
}

b32
nundinae_apple_tester (
    NundinaeApple* n,
           chorda  id_gregis,
           chorda  email,
              b32* creatum,
           chorda* causa,
          Piscina* p)
{
    NundinaeAppleResponsum  r;
                 JsonValor* data;
                 JsonValor* a;
                 JsonValor* rel;
                    chorda  vac;
                    chorda  id_testeris;
                       i32  i;

    vac = chorda_ex_literis("", p);
    si (creatum)
    {
        *creatum = FALSUM;
    }
    /* iam in grege? */
    r = nundinae_apple_vocare(n, HTTP_GET, _via(p,
        "/v1/betaGroups/%A/betaTesters?limit=200", id_gregis, vac), vac,
        p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    data = json_objectum_capere(r.corpus, "data");
    per (i = ZEPHYRUM; data && i < json_tabulatum_numerus(data); i++)
    {
        a = json_objectum_capere(json_tabulatum_obtinere(data, i),
            "attributes");
        si (chorda_aequalis(json_capere_chorda(a, "email", vac), email))
        {
            redde VERUM;
        }
    }
    /* tester iam notus turmae -> relatio ad gregem */
    r = nundinae_apple_vocare(n, HTTP_GET, _via(p,
        "/v1/betaTesters?filter%5Bemail%5D=%A&limit=200",
            _url_valor(email,
        p), vac), vac, p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    id_testeris = json_capere_chorda(json_via(r.corpus, "data.0"), "id",
        vac);
    si (id_testeris.mensura > ZEPHYRUM)
    {
        JsonValor* radix;
        JsonValor* tab;
        JsonValor* res;

        res = json_objectum_creare(p);
        json_objectum_ponere(res, "type", json_chorda_creare_literis(p,
            "betaTesters"));
        json_objectum_ponere(res, "id", json_chorda_creare(p,
            id_testeris));
        tab = json_tabulatum_creare(p);
        json_tabulatum_addere(tab, res);
        radix = json_objectum_creare(p);
        json_objectum_ponere(radix, "data", tab);
        r = nundinae_apple_vocare(n, HTTP_POST, _via(p,
            "/v1/betaGroups/%A/relationships/betaTesters", id_gregis,
                vac),
            json_scribere(radix, p), p);
        si (r.successus)
        {
            si (creatum)
            {
                *creatum = VERUM;
            }
            redde VERUM;
        }
        /* 409 (vivum 2026-10-24): tester alterius applicationis
         * gregi novo assignari nequit - via altera: tester creatus
         * cum grege */
        si (r.status != CDIX)
        {
            *causa = r.causa;
            redde FALSUM;
        }
    }
    a = json_objectum_creare(p);
    json_objectum_ponere(a, "email", json_chorda_creare(p, email));
    rel = json_objectum_creare(p);
    json_objectum_ponere(rel, "betaGroups", _relatio("betaGroups",
        id_gregis, VERUM, p));
    r = nundinae_apple_vocare(n, HTTP_POST, "/v1/betaTesters",
        _corpus_rei("betaTesters", a, rel, p), p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    si (creatum)
    {
        *creatum = VERUM;
    }
    redde VERUM;
}


/* ==================================================
 * Distributio et missio (T4) - partes purae
 * ================================================== */

b32
nundinae_apple_cryptographia (
    constans chorda* obiecta,
                i32  numerus,
                Xar* nominata,
            Piscina* p)
{
    hic_manens constans character* signa[] = {
        "tls", "bearssl", "aes_", "aesctr", "chacha", "crypt", NIHIL
    };
    i32 i;
    i32 k;
    b32 inventa;

    (vacuum)p;
    inventa = FALSUM;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        chorda basis;
           i32 s;

        basis = obiecta[i];
        per (s = basis.mensura - I; s >= ZEPHYRUM; s--)
        {
            si (basis.datum[s] == '/')
            {
                frange;
            }
        }
        basis = chorda_sectio(basis, (i32)(s + I), basis.mensura);
        per (k = ZEPHYRUM; signa[k]; k++)
        {
            i32 m = (i32)strlen(signa[k]);

            si (   basis.mensura                            >= m
                && memcmp(basis.datum, signa[k], (size_t)m) == ZEPHYRUM)
            {
                chorda* n;

                n = (chorda*)xar_addere(nominata);
                si (n)
                {
                    *n = obiecta[i];
                }
                inventa = VERUM;
                frange;
            }
        }
    }
    redde inventa;
}

interior i32
_maximum_numericum (
             JsonValor* responsum,
    constans character* campus)
{
     JsonValor* data;
           i32  i;
           i32  maximum;

    maximum = ZEPHYRUM;
    data = responsum ? json_objectum_capere(responsum, "data") : NIHIL;
    per (i = ZEPHYRUM; data && i < json_tabulatum_numerus(data); i++)
    {
         JsonValor* a;
         JsonValor* v;
            chorda  t;
               i32  n;
               i32  k;
               b32  numericum;

        a = json_objectum_capere(json_tabulatum_obtinere(data, i),
            "attributes");
        v = a ? json_objectum_capere(a, campus) : NIHIL;
        si (!v)
        {
            perge;
        }
        t = json_ad_chorda(v);
        si (t.mensura == ZEPHYRUM || t.mensura > IX)
        {
            perge;
        }
        n          = ZEPHYRUM;
        numericum  = VERUM;
        per (k = ZEPHYRUM; k < t.mensura; k++)
        {
            si (t.datum[k] < '0' || t.datum[k] > '9')
            {
                numericum = FALSUM;
                frange;
            }
            n = n * X + (i32)(t.datum[k] - '0');
        }
        si (numericum && n > maximum)
        {
            maximum = n;
        }
    }
    redde maximum;
}

i32
nundinae_apple_versio_maxima (
    JsonValor* aedificationes,
    JsonValor* missiones)
{
    i32 a;
    i32 b;

    a = _maximum_numericum(aedificationes, "version");
    b = _maximum_numericum(missiones, "cfBundleVersion");
    redde a > b ? a : b;
}

b32
nundinae_apple_operationes_legere (
    JsonValor* responsum,
          Xar* operationes,
      Piscina* p)
{
     JsonValor* ops;
        chorda  vac;
           i32  i;

    vac = chorda_ex_literis("", p);
    ops = responsum ? json_via(responsum,
        "data.attributes.uploadOperations")
                    : NIHIL;
    si (!ops)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < json_tabulatum_numerus(ops); i++)
    {
                  JsonValor* d;
                  JsonValor* capita;
      NundinaeAppleOperatio* o;
                        i32  k;

        d = json_tabulatum_obtinere(ops, i);
        o = (NundinaeAppleOperatio*)xar_addere(operationes);
        si (!o || !d)
        {
            redde FALSUM;
        }
        o->methodus  = json_capere_chorda(d, "method", vac);
        o->url       = json_capere_chorda(d, "url", vac);
        o->offset     = (s64)json_ad_integer(json_objectum_capere(d,
            "offset"));
        o->longitudo  = (s64)json_ad_integer(json_objectum_capere(d,
            "length"));
        o->capita  = xar_creare(p,
            (i32)magnitudo(NundinaeAppleCaput));
        capita     = json_objectum_capere(d, "requestHeaders");
        per (k = ZEPHYRUM; capita && k < json_tabulatum_numerus(capita);
             k++)
        {
                     JsonValor* h;
            NundinaeAppleCaput* c;

            h = json_tabulatum_obtinere(capita, k);
            c = (NundinaeAppleCaput*)xar_addere(o->capita);
            si (c && h)
            {
                c->titulus  = json_capere_chorda(h, "name", vac);
                c->valor    = json_capere_chorda(h, "value", vac);
            }
        }
    }
    redde json_tabulatum_numerus(ops) > ZEPHYRUM;
}

chorda
nundinae_apple_md5_hex (
    constans i8* datum,
            i64  mensura,
        Piscina* p)
{
        br_md5_context  h;
         unsigned char  d[XVI];
             character  hex[XXXIII];
                   i32  i;
    constans character* digiti = "0123456789abcdef";

    br_md5_init(&h);
    br_md5_update(&h, datum, (size_t)mensura);
    br_md5_out(&h, d);
    per (i = ZEPHYRUM; i < XVI; i++)
    {
        hex[i * II]      = digiti[(d[i] >> IV) & 0xF];
        hex[i * II + I]  = digiti[d[i] & 0xF];
    }
    hex[XXXII] = '\0';
    redde chorda_ex_literis(hex, p);
}


/* ==================================================
 * Distributio et missio (T4) - rete
 * ================================================== */

interior vacuum
_nuntiare (
         NundinaeApple* n,
    constans character* linea)
{
    si (n->observator)
    {
        n->observator(n->observatoris_datum, linea);
    }
}

b32
nundinae_apple_aedificationem_proximam (
    NundinaeApple* n,
           chorda  id_applicationis,
              i32* proxima,
           chorda* causa,
          Piscina* p)
{
    NundinaeAppleResponsum a;
    NundinaeAppleResponsum b;
                    chorda vac;

    vac = chorda_ex_literis("", p);
    a = nundinae_apple_vocare(n, HTTP_GET, _via(p,
        "/v1/builds?filter%5Bapp%5D=%A&limit=200"
        "&fields%5Bbuilds%5D=version", id_applicationis, vac), vac, p);
    si (!_vocatio_bona(a, causa))
    {
        redde FALSUM;
    }
    b = nundinae_apple_vocare(n, HTTP_GET, _via(p,
        "/v1/apps/%A/buildUploads?limit=200"
        "&fields%5BbuildUploads%5D=cfBundleVersion", id_applicationis,
        vac), vac, p);
    si (!_vocatio_bona(b, causa))
    {
        redde FALSUM;
    }
    *proxima = nundinae_apple_versio_maxima(a.corpus, b.corpus) + I;
    redde VERUM;
}

interior HttpMethodus
_methodus_ex (
    chorda m)
{
    si (chorda_aequalis_literis(m, "PUT"))
    {
        redde HTTP_PUT;
    }
    si (chorda_aequalis_literis(m, "POST"))
    {
        redde HTTP_POST;
    }
    si (chorda_aequalis_literis(m, "PATCH"))
    {
        redde HTTP_PATCH;
    }
    redde HTTP_METHODUS_IGNOTUS;
}

/* pars una ad URL Apple (sine tessera): III conatus (eadem pars eodem
 * loco - idempotens) */
interior b32
_partem_mittere (
    constans NundinaeAppleOperatio* o,
                       constans i8* datum,
                            chorda* causa,
                           Piscina* p)
{
     HttpPetitio* pet;
    HttpResultus  res;
             i32  conatus;
             i32  k;
       character  linea[CCLVI];
    HttpMethodus  m;

    m = _methodus_ex(o->methodus);
    si (m == HTTP_METHODUS_IGNOTUS)
    {
        *causa = chorda_concatenare(chorda_ex_literis(
            "nundinae_apple: methodus operationis ignota: ", p),
            o->methodus, p);
        redde FALSUM;
    }
    per (conatus = ZEPHYRUM; conatus < III; conatus++)
    {
        pet = http_petitio_creare(p, m, chorda_ut_cstr(o->url, p));
        si (!pet)
        {
            *causa =
                chorda_ex_literis("nundinae_apple: URL operationis "
                "mala", p);
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < xar_numerus(o->capita); k++)
        {
            NundinaeAppleCaput* c;

            c = (NundinaeAppleCaput*)xar_obtinere(o->capita, k);
            http_petitio_caput_addere(pet, chorda_ut_cstr(c->titulus,
                p),
                chorda_ut_cstr(c->valor, p));
        }
        http_petitio_corpus_ponere(pet, (constans character*)(datum
            + o->offset), (i32)o->longitudo);
        res = http_exsequi(pet, p);
        si (   res.successus && res.responsum
            && res.responsum->status >= CC
            && res.responsum->status < CCC)
        {
            redde VERUM;
        }
        sprintf(linea, "nundinae_apple: pars (offset %ld, %ld octeti) "
            "fracta: %d", (longus)o->offset, (longus)o->longitudo,
            res.responsum ? (int)res.responsum->status : ZEPHYRUM);
        *causa = chorda_ex_literis(linea, p);
    }
    redde FALSUM;
}

interior vacuum
_dormire (
    s32 secundae)
{
    structura timespec t;

    t.tv_sec   = (time_t)secundae;
    t.tv_nsec  = ZEPHYRUM;
    (vacuum)nanosleep(&t, NIHIL);
}

/* errores statūs (StateDetail[]) in causam */
interior chorda
_errores_status (
    JsonValor* status,
      Piscina* p)
{
         JsonValor* e;
 ChordaAedificator* a;
               i32  i;

    a = chorda_aedificator_creare(p, CCLVI);
    e = status ? json_objectum_capere(status, "errors") : NIHIL;
    per (i = ZEPHYRUM; e && i < json_tabulatum_numerus(e); i++)
    {
        JsonValor* d = json_tabulatum_obtinere(e, i);

        chorda_aedificator_appendere_literis(a, i ? "; " : "");
        chorda_aedificator_appendere_chorda(a, json_capere_chorda(d,
            "code",
            chorda_ex_literis("", p)));
        chorda_aedificator_appendere_literis(a, ": ");
        chorda_aedificator_appendere_chorda(a, json_capere_chorda(d,
            "description", chorda_ex_literis("", p)));
    }
    redde chorda_aedificator_finire(a);
}

b32
nundinae_apple_mittere (
       NundinaeApple* n,
              chorda  id_applicationis,
  constans character* via_archivi,
              chorda  versio,
                 i32  aedificatio,
              chorda* id_aedificationis,
              chorda* causa,
             Piscina* p)
{
    NundinaeAppleResponsum  r;
                    chorda  archivum;
                    chorda  vac;
                    chorda  id_missionis;
                    chorda  id_plagulae;
                    chorda  md5;
                    chorda  status;
                 JsonValor* a;
                 JsonValor* rel;
                 JsonValor* summae;
                 JsonValor* summa;
                       Xar* operationes;
                 character  numerus[XVI];
                 character  linea[CCLVI];
        constans character* nomen_plagulae;
                       i32  i;
                       i32  vices;

    vac       = chorda_ex_literis("", p);
    archivum  = filum_legere_totum(via_archivi, p);
    si (archivum.mensura <= ZEPHYRUM)
    {
        *causa = chorda_concatenare(chorda_ex_literis(
            "nundinae_apple: archivum legi nequit: ", p),
            chorda_ex_literis(
            via_archivi, p), p);
        redde FALSUM;
    }
    nomen_plagulae = strrchr(via_archivi, '/');
    nomen_plagulae = nomen_plagulae ? nomen_plagulae + I : via_archivi;
    sprintf(numerus, "%d", (int)aedificatio);
    /* I. buildUpload */
    a = json_objectum_creare(p);
    json_objectum_ponere(a, "cfBundleShortVersionString",
        json_chorda_creare(
        p, versio));
    json_objectum_ponere(a, "cfBundleVersion",
        json_chorda_creare_literis(p,
        numerus));
    json_objectum_ponere(a, "platform", json_chorda_creare_literis(p,
        "IOS"));
    rel = json_objectum_creare(p);
    json_objectum_ponere(rel, "app", _relatio("apps", id_applicationis,
        FALSUM, p));
    r = nundinae_apple_vocare(n, HTTP_POST, "/v1/buildUploads",
        _corpus_rei("buildUploads", a, rel, p), p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    id_missionis = json_capere_chorda(json_objectum_capere(r.corpus,
        "data"),
        "id", vac);
    /* II. buildUploadFile */
    a = json_objectum_creare(p);
    json_objectum_ponere(a, "assetType", json_chorda_creare_literis(p,
        "ASSET"));
    json_objectum_ponere(a, "fileName", json_chorda_creare_literis(p,
        nomen_plagulae));
    json_objectum_ponere(a, "fileSize", json_integer_creare(p,
        (s64)archivum.mensura));
    json_objectum_ponere(a, "uti", json_chorda_creare_literis(p,
        "com.apple.ipa"));
    rel = json_objectum_creare(p);
    json_objectum_ponere(rel, "buildUpload", _relatio("buildUploads",
        id_missionis, FALSUM, p));
    r = nundinae_apple_vocare(n, HTTP_POST, "/v1/buildUploadFiles",
        _corpus_rei("buildUploadFiles", a, rel, p), p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    id_plagulae = json_capere_chorda(json_objectum_capere(r.corpus,
        "data"),
        "id", vac);
    operationes = xar_creare(p, (i32)magnitudo(NundinaeAppleOperatio));
    si (!nundinae_apple_operationes_legere(r.corpus, operationes, p))
    {
        *causa = chorda_ex_literis(
            "nundinae_apple: nullae operationes missionis", p);
        redde FALSUM;
    }
    /* III. partes */
    per (i = ZEPHYRUM; i < xar_numerus(operationes); i++)
    {
        NundinaeAppleOperatio* o;

        o = (NundinaeAppleOperatio*)xar_obtinere(operationes, i);
        si (   o->offset < ZEPHYRUM || o->longitudo <= ZEPHYRUM
            || o->offset + o->longitudo > (s64)archivum.mensura)
        {
            *causa = chorda_ex_literis(
                "nundinae_apple: operatio extra plagulam", p);
            redde FALSUM;
        }
        si (!_partem_mittere(o, archivum.datum, causa, p))
        {
            redde FALSUM;
        }
        sprintf(linea, "pars %d/%d missa", (int)(i + I),
            (int)xar_numerus(operationes));
        _nuntiare(n, linea);
    }
    /* IV. commissio cum MD5 plagulae: hex MAIUSCULAE ut entityTag
     * Apple (minusculae -> 409 ATTRIBUTE.INVALID); 'composite' Apple
     * ipse ex 'file' implet */
    md5 = nundinae_apple_md5_hex(archivum.datum,
        (i64)archivum.mensura, p);
    per (i = ZEPHYRUM; i < md5.mensura; i++)
    {
        si (md5.datum[i] >= 'a' && md5.datum[i] <= 'f')
        {
            md5.datum[i] = (i8)(md5.datum[i] - 'a' + 'A');
        }
    }
    summae  = json_objectum_creare(p);
    summa   = json_objectum_creare(p);
    json_objectum_ponere(summa, "algorithm",
        json_chorda_creare_literis(p,
        "MD5"));
    json_objectum_ponere(summa, "hash", json_chorda_creare(p, md5));
    json_objectum_ponere(summae, "file", summa);
    a = json_objectum_creare(p);
    json_objectum_ponere(a, "uploaded", json_boolean_creare(p, VERUM));
    json_objectum_ponere(a, "sourceFileChecksums", summae);
    {
        JsonValor* radix;
        JsonValor* data;

        data = json_objectum_creare(p);
        json_objectum_ponere(data, "type", json_chorda_creare_literis(p,
            "buildUploadFiles"));
        json_objectum_ponere(data, "id", json_chorda_creare(p,
            id_plagulae));
        json_objectum_ponere(data, "attributes", a);
        radix = json_objectum_creare(p);
        json_objectum_ponere(radix, "data", data);
        r = nundinae_apple_vocare(n, HTTP_PATCH, _via(p,
            "/v1/buildUploadFiles/%A", id_plagulae, vac),
            json_scribere(radix,
            p), p);
    }
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    _nuntiare(n, "missio commissa; Apple examinat");
    /* V. status missionis: COMPLETE (cum build) aut FAILED */
    id_aedificationis->mensura = ZEPHYRUM;
    per (vices = ZEPHYRUM; vices < CXX; vices++)
    {
        r = nundinae_apple_vocare(n, HTTP_GET, _via(p,
            "/v1/buildUploads/%A?include=build", id_missionis, vac),
            vac, p);
        si (!_vocatio_bona(r, causa))
        {
            redde FALSUM;
        }
        status = json_capere_chorda(json_via(r.corpus,
            "data.attributes.state"), "state", vac);
        si (chorda_aequalis_literis(status, "FAILED"))
        {
            *causa = chorda_concatenare(chorda_ex_literis(
                "nundinae_apple: Apple missionem refutavit: ", p),
                _errores_status(json_via(r.corpus,
                "data.attributes.state"),
                p), p);
            redde FALSUM;
        }
        *id_aedificationis = json_capere_chorda(json_via(r.corpus,
            "data.relationships.build.data"), "id", vac);
        si (   chorda_aequalis_literis(status, "COMPLETE")
            && id_aedificationis->mensura > ZEPHYRUM)
        {
            frange;
        }
        sprintf(linea, "missio: %.*s", (int)status.mensura,
            (constans character*)status.datum);
        _nuntiare(n, linea);
        _dormire(XV);
    }
    si (id_aedificationis->mensura == ZEPHYRUM)
    {
        *causa = chorda_concatenare(chorda_ex_literis(
            "nundinae_apple: missio non perfecta intra XXX min, "
            "status ",
            p),
            status, p);
        redde FALSUM;
    }
    _nuntiare(n, "missio COMPLETE; aedificatio processum exspectat");
    /* VI. processus aedificationis */
    per (vices = ZEPHYRUM; vices < LX; vices++)
    {
        r = nundinae_apple_vocare(n, HTTP_GET, _via(p,
            "/v1/builds/%A?fields%5Bbuilds%5D=processingState,version",
            *id_aedificationis, vac), vac, p);
        si (!_vocatio_bona(r, causa))
        {
            redde FALSUM;
        }
        status = json_capere_chorda(json_via(r.corpus,
            "data.attributes"),
            "processingState", vac);
        si (chorda_aequalis_literis(status, "VALID"))
        {
            redde VERUM;
        }
        si (   chorda_aequalis_literis(status, "INVALID")
            || chorda_aequalis_literis(status, "FAILED"))
        {
            *causa = chorda_concatenare(chorda_ex_literis(
                "nundinae_apple: aedificatio ", p), status, p);
            redde FALSUM;
        }
        sprintf(linea, "aedificatio: %.*s", (int)status.mensura,
            (constans character*)status.datum);
        _nuntiare(n, linea);
        _dormire(XXX);
    }
    *causa = chorda_ex_literis(
        "nundinae_apple: aedificatio non VALID intra XXX min", p);
    redde FALSUM;
}


/* ==================================================
 * TestFlight: notae, grex, testatores (T5)
 * ================================================== */

interior b32
_hex_minuscula (
    i8 c)
{
    redde (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
}

chorda
nundinae_apple_commissum_ultimum (
     chorda  notae,
    Piscina* p)
{
    chorda ultimum;
       i32 i;
       i32 initium;

    ultimum = chorda_ex_literis("", p);
    initium = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= notae.mensura; i++)
    {
        si (i < notae.mensura && notae.datum[i] != '\n')
        {
            perge;
        }
        /* linea [initium, i): 'commit ' + hex VII..XL, deinde finis aut
         * spatium */
        si (   i - initium > VII
            && memcmp(notae.datum + initium, "commit ", VII)
                == ZEPHYRUM)
        {
            i32 k;

            k = initium + VII;
            dum (k < i && _hex_minuscula(notae.datum[k]))
            {
                k++;
            }
            si (   k - (initium + VII) >= VII
                && k - (initium + VII) <= XL
                && (k == i || notae.datum[k] == ' '
                    || notae.datum[k] == '\r'))
            {
                ultimum = chorda_sectio(notae, initium + VII, k);
            }
        }
        initium = i + I;
    }
    redde ultimum;
}

/* localizatio en-US in 'included' per id */
interior JsonValor*
_inclusum (
     JsonValor* inclusa,
        chorda  id,
       Piscina* p)
{
    i32 i;

    per (i = ZEPHYRUM; inclusa && i < json_tabulatum_numerus(inclusa);
         i++)
    {
        JsonValor* r = json_tabulatum_obtinere(inclusa, i);

        si (chorda_aequalis(json_capere_chorda(r, "id",
            chorda_ex_literis(
                "", p)), id))
        {
            redde r;
        }
    }
    redde NIHIL;
}

chorda
nundinae_apple_commissum_ex_aedificationibus (
    JsonValor* responsum,
      Piscina* p)
{
     JsonValor* data;
     JsonValor* inclusa;
        chorda  vac;
           i32  i;

    vac   = chorda_ex_literis("", p);
    data  = responsum ? json_objectum_capere(responsum, "data") : NIHIL;
    inclusa = responsum ? json_objectum_capere(responsum, "included")
                        : NIHIL;
    per (i = ZEPHYRUM; data && i < json_tabulatum_numerus(data); i++)
    {
         JsonValor* loc;
               i32  k;

        loc = json_via(json_tabulatum_obtinere(data, i),
            "relationships.betaBuildLocalizations.data");
        per (k = ZEPHYRUM; loc && k < json_tabulatum_numerus(loc); k++)
        {
             JsonValor* r;
             JsonValor* a;
                chorda  c;

            r = _inclusum(inclusa, json_capere_chorda(
                json_tabulatum_obtinere(loc, k), "id", vac), p);
            a = r ? json_objectum_capere(r, "attributes") : NIHIL;
            si (   !a
                || !chorda_aequalis_literis(json_capere_chorda(a,
                       "locale", vac), "en-US"))
            {
                perge;
            }
            c = nundinae_apple_commissum_ultimum(json_capere_chorda(a,
                "whatsNew", vac), p);
            si (c.mensura > ZEPHYRUM)
            {
                redde c;
            }
        }
    }
    redde vac;
}

b32
nundinae_apple_testatores_legere (
    JsonValor* responsum,
          Xar* testatores,
      Piscina* p)
{
     JsonValor* data;
        chorda  vac;
           i32  i;

    vac   = chorda_ex_literis("", p);
    data  = responsum ? json_objectum_capere(responsum, "data") : NIHIL;
    si (!data)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < json_tabulatum_numerus(data); i++)
    {
                    JsonValor* d;
                    JsonValor* a;
        NundinaeAppleTestator* t;

        d = json_tabulatum_obtinere(data, i);
        a = json_objectum_capere(d, "attributes");
        t = (NundinaeAppleTestator*)xar_addere(testatores);
        si (!t)
        {
            redde FALSUM;
        }
        t->id      = json_capere_chorda(d, "id", vac);
        t->email   = a ? json_capere_chorda(a, "email", vac) : vac;
        t->status  = a ? json_capere_chorda(a, "state", vac) : vac;
    }
    redde VERUM;
}

b32
nundinae_apple_invitandus (
    constans NundinaeAppleTestator* t)
{
    redde chorda_aequalis_literis(t->status, "NOT_INVITED");
}

b32
nundinae_apple_ultimum (
    NundinaeApple* n,
           chorda  id_applicationis,
           chorda* commissum,
           chorda* causa,
          Piscina* p)
{
    NundinaeAppleResponsum r;
                    chorda vac;

    vac = chorda_ex_literis("", p);
    r = nundinae_apple_vocare(n, HTTP_GET, _via(p,
        "/v1/builds?filter%5Bapp%5D=%A&sort=-uploadedDate&limit=50"
        "&fields%5Bbuilds%5D=version,betaBuildLocalizations"
        "&include=betaBuildLocalizations"
        "&fields%5BbetaBuildLocalizations%5D=locale,whatsNew",
        id_applicationis, vac), vac, p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    *commissum = nundinae_apple_commissum_ex_aedificationibus(r.corpus,
        p);
    redde VERUM;
}

b32
nundinae_apple_aedificationem_invenire (
    NundinaeApple* n,
           chorda  id_applicationis,
              i32  aedificatio,
           chorda* id_aedificationis,
           chorda* causa,
          Piscina* p)
{
    NundinaeAppleResponsum  r;
                    chorda  vac;
                 JsonValor* data;
                 character  numerus[XVI];

    vac = chorda_ex_literis("", p);
    sprintf(numerus, "%d", (int)aedificatio);
    r = nundinae_apple_vocare(n, HTTP_GET, _via(p,
        "/v1/builds?filter%5Bapp%5D=%A&filter%5Bversion%5D=%B&limit=2"
        "&fields%5Bbuilds%5D=version,processingState", id_applicationis,
        chorda_ex_literis(numerus, p)), vac, p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    data = json_objectum_capere(r.corpus, "data");
    si (!data || json_tabulatum_numerus(data) != I)
    {
        *causa = chorda_concatenare(chorda_ex_literis(
            "nundinae_apple: aedificatio non inventa (aut non unica): ",
            p),
            chorda_ex_literis(numerus, p), p);
        redde FALSUM;
    }
    *id_aedificationis =
        json_capere_chorda(json_tabulatum_obtinere(data,
        ZEPHYRUM), "id", vac);
    redde id_aedificationis->mensura > ZEPHYRUM;
}

b32
nundinae_apple_notas_ponere (
    NundinaeApple* n,
           chorda  id_aedificationis,
           chorda  notae,
           chorda* causa,
          Piscina* p)
{
    NundinaeAppleResponsum  r;
                    chorda  vac;
                    chorda  id_loc;
                 JsonValor* data;
                 JsonValor* a;
                 JsonValor* rel;
                       i32  i;

    vac = chorda_ex_literis("", p);
    si (notae.mensura == ZEPHYRUM || notae.mensura > IV * M)
    {
        *causa = chorda_ex_literis("nundinae_apple: notae vacuae aut "
            "longiores quam MMMM octeti (limes Apple whatsNew)", p);
        redde FALSUM;
    }
    r = nundinae_apple_vocare(n, HTTP_GET, _via(p,
        "/v1/builds/%A/betaBuildLocalizations?limit=50",
        id_aedificationis, vac), vac, p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    id_loc  = vac;
    data    = json_objectum_capere(r.corpus, "data");
    per (i = ZEPHYRUM; data && i < json_tabulatum_numerus(data); i++)
    {
        JsonValor* d = json_tabulatum_obtinere(data, i);

        a = json_objectum_capere(d, "attributes");
        si (a && chorda_aequalis_literis(json_capere_chorda(a, "locale",
                vac), "en-US"))
        {
            id_loc = json_capere_chorda(d, "id", vac);
        }
    }
    a = json_objectum_creare(p);
    json_objectum_ponere(a, "whatsNew", json_chorda_creare(p, notae));
    si (id_loc.mensura > ZEPHYRUM)
    {
        JsonValor* radix;
        JsonValor* d;

        d = json_objectum_creare(p);
        json_objectum_ponere(d, "type", json_chorda_creare_literis(p,
            "betaBuildLocalizations"));
        json_objectum_ponere(d, "id", json_chorda_creare(p, id_loc));
        json_objectum_ponere(d, "attributes", a);
        radix = json_objectum_creare(p);
        json_objectum_ponere(radix, "data", d);
        r = nundinae_apple_vocare(n, HTTP_PATCH, _via(p,
            "/v1/betaBuildLocalizations/%A", id_loc, vac),
            json_scribere(
            radix, p), p);
    }
    alioquin
    {
        json_objectum_ponere(a, "locale", json_chorda_creare_literis(p,
            "en-US"));
        rel = json_objectum_creare(p);
        json_objectum_ponere(rel, "build", _relatio("builds",
            id_aedificationis, FALSUM, p));
        r = nundinae_apple_vocare(n, HTTP_POST,
            "/v1/betaBuildLocalizations",
            _corpus_rei("betaBuildLocalizations", a, rel, p), p);
    }
    redde _vocatio_bona(r, causa);
}

b32
nundinae_apple_gregi_addere (
    NundinaeApple* n,
           chorda  id_gregis,
           chorda  id_aedificationis,
           chorda* causa,
          Piscina* p)
{
    NundinaeAppleResponsum r;

    r = nundinae_apple_vocare(n, HTTP_POST, _via(p,
        "/v1/betaGroups/%A/relationships/builds", id_gregis,
        chorda_ex_literis("", p)), json_scribere(_relatio("builds",
        id_aedificationis, VERUM, p), p), p);
    redde _vocatio_bona(r, causa);
}

b32
nundinae_apple_testatores (
    NundinaeApple* n,
           chorda  id_gregis,
              Xar* testatores,
           chorda* causa,
          Piscina* p)
{
    NundinaeAppleResponsum r;
                    chorda vac;

    vac = chorda_ex_literis("", p);
    r = nundinae_apple_vocare(n, HTTP_GET, _via(p,
        "/v1/betaGroups/%A/betaTesters?limit=200"
        "&fields%5BbetaTesters%5D=email,state", id_gregis, vac), vac,
        p);
    si (!_vocatio_bona(r, causa))
    {
        redde FALSUM;
    }
    si (!nundinae_apple_testatores_legere(r.corpus, testatores, p))
    {
        *causa = chorda_ex_literis(
            "nundinae_apple: responsum betaTesters sine data", p);
        redde FALSUM;
    }
    redde VERUM;
}

b32
nundinae_apple_invitare (
    NundinaeApple* n,
           chorda  id_applicationis,
           chorda  id_testatoris,
           chorda* causa,
          Piscina* p)
{
    NundinaeAppleResponsum  r;
                 JsonValor* rel;

    rel = json_objectum_creare(p);
    json_objectum_ponere(rel, "app", _relatio("apps", id_applicationis,
        FALSUM, p));
    json_objectum_ponere(rel, "betaTester", _relatio("betaTesters",
        id_testatoris, FALSUM, p));
    r = nundinae_apple_vocare(n, HTTP_POST, "/v1/betaTesterInvitations",
        _corpus_rei("betaTesterInvitations", NIHIL, rel, p), p);
    redde _vocatio_bona(r, causa);
}
