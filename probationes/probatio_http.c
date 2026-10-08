/* probatio_http.c - Probationes pro HTTP bibliotheca (cliens)
 *
 * OFFLINE: fixtura localis (hospitium in processu filiato, portus 0
 * auto, portus per fistulam relatus) pro httpbin.org substituitur.
 * Probationes TLS vivae in probatio_tls manent (-reticularis).
 */

#include "postulata_posix.h"
#include "http.h"
#include "hospitium.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"
#include "tcp.h"   /* fixtura prava: servus crudus misbehavens */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


/* ========================================================================
 * FIXTURA - SERVUS FILIATUS
 * ======================================================================== */

interior vacuum
_fixtura_get (
    HospitiumColloquium* colloquium)
{
    chorda corpus = chorda_ex_literis(
        "{\"url\": \"/get\", \"origo\": \"fixtura localis\"}",
        colloquium_piscina(colloquium));
    colloquium_respondere(colloquium, CC, "application/json", corpus);
}

interior vacuum
_fixtura_post (
    HospitiumColloquium* colloquium)
{
    /* echo corporis petitionis */
    constans HttpPetitioServeri* petitio =
        colloquium_petitio(colloquium);
    colloquium_respondere(colloquium, CC, "application/json",
                          petitio->corpus);
}

interior vacuum
_fixtura_capita (
    HospitiumColloquium* colloquium)
{
    chorda corpus = chorda_ex_literis("{\"capita\": \"visa\"}",
                                      colloquium_piscina(colloquium));
    colloquium_respondere(colloquium, CC, "application/json", corpus);
}

interior vacuum
_fixtura_status (
    HospitiumColloquium* colloquium)
{
    chorda codex_ch  = colloquium_param(colloquium, "codex");
       i32 codex     = 0;
       i32 i;
    chorda vacua;

    per (i = 0; i < codex_ch.mensura; i++)
    {
        si (codex_ch.datum[i] >= '0' && codex_ch.datum[i] <= '9')
        {
            codex = codex * X + (i32)(codex_ch.datum[i] - '0');
        }
    }
    si (codex < C || codex > D + XCIX)
    {
        codex = CC;
    }

    vacua.datum    = NIHIL;
    vacua.mensura  = 0;
    colloquium_respondere(colloquium, codex, NIHIL, vacua);
}

/* Incipere fixturam in processu filiato.
 * Redde portum (parens) vel 0 si defectus. */
interior i32
_fixturam_incipere (
    pid_t* pid_exitus)
{
    integer fistula[II];
      pid_t pid;
        i32 portus = 0;

    si (pipe(fistula) != 0)
    {
        redde 0;
    }

    pid = fork();
    si (pid < 0)
    {
        close(fistula[0]);
        close(fistula[I]);
        redde 0;
    }

    si (pid == 0)
    {
        /* infans = servus; numquam ad probationes redit */
        Piscina* p = piscina_generare_dynamicum("fixtura_http", M * M);
        HospitiumConfiguratio cfg;
        Hospitium* h;
        i32 portus_infantis;

        close(fistula[0]);
        memset(&cfg, 0, magnitudo(cfg));
        h = hospitium_creare(p, &cfg);
        si (h == NIHIL)
        {
            _exit(I);
        }

        hospitium_praebere(h, HTTP_GET, "/get", _fixtura_get, NIHIL);
        hospitium_praebere(h, HTTP_POST, "/post", _fixtura_post, NIHIL);
        hospitium_praebere(h, HTTP_GET, "/headers", _fixtura_capita,
            NIHIL);
        hospitium_praebere(h, HTTP_GET, "/status/:codex",
            _fixtura_status,
                           NIHIL);

        portus_infantis = hospitium_portus(h);
        si (write(fistula[I], &portus_infantis,
                  magnitudo(portus_infantis))
            != (ssize_t)magnitudo(portus_infantis))
        {
            _exit(I);
        }
        close(fistula[I]);

        hospitium_currere(h);  /* usque ad SIGTERM ex parente */
        _exit(0);
    }

    /* parens */
    close(fistula[I]);
    si (read(fistula[0], &portus, magnitudo(portus))
        != (ssize_t)magnitudo(portus))
    {
        portus = 0;
    }
    close(fistula[0]);

    *pid_exitus = pid;
    redde portus;
}

interior vacuum
_fixturam_terminare (
    pid_t pid)
{
    si (pid > 0)
    {
        kill(pid, SIGTERM);
        waitpid(pid, NIHIL, 0);
    }
}

interior constans character*
_url_fixturae (
             character* buffer,
                   i32  portus,
    constans character* via)
{
    sprintf(buffer, "http://127.0.0.1:%u%s", portus, via);
    redde buffer;
}


/* ========================================================================
 * FIXTURA PRAVA - servus crudus misbehavens (semitae erroris ansae
 * haurientis, 01KYANH7AN): connexionem UNAM accipit, partialiter
 * respondet, deinde per modum claudit aut dormit aut mentitur.
 * ======================================================================== */

#define FIXTURA_PRAVA_MINUSCULUM    I  /* "ok" solum, claudere */
#define FIXTURA_PRAVA_TRUNCATUM    II  /* capita + X ex C octetis, claudere */
#define FIXTURA_PRAVA_DORMIENS    III  /* capita + X octeti, dormire II s */
#define FIXTURA_PRAVA_CAPUT_INGENS IV  /* Content-Length involutivum */
#define FIXTURA_CHUNKED_INTEGRUM    V  /* chunked cum fragmento '0' */
#define FIXTURA_CHUNKED_TRUNCATUM  VI  /* chunked sine fine, claudere */
#define FIXTURA_CHUNKED_INGENS    VII  /* magnitudo involutiva */
#define FIXTURA_CAPUT_SOLUM      VIII  /* capita sine corpore (HEAD) */
#define FIXTURA_CORPUS_MAGNUM      IX  /* CC milia octetorum */

#define CORPUS_MAGNUM_MENSURA  (CC * M)

interior i32
_fixturam_pravam_incipere (
      pid_t* pid_exitus,
    integer  modus)
{
    integer fistula[II];
      pid_t pid;
        i32 portus = 0;

    *pid_exitus = 0;
    si (pipe(fistula) != 0)
    {
        redde 0;
    }
    pid = fork();
    si (pid < 0)
    {
        close(fistula[0]);
        close(fistula[I]);
        redde 0;
    }
    si (pid == 0)
    {
        Piscina* p = piscina_generare_dynamicum("fixtura_prava", M * M);
        TcpServusOptiones so = tcp_servus_optiones_default();
        TcpServusResultus sr;
        TcpResultus conn;
        i8 buffer[M];
        i32 portus_infantis;

        /* accipere BLOCANS - defaltum non_blocans est (mundus
         * reactoris); sine hoc infans ante clientem pollit,
         * 'nullae connexiones' videt et exit - RST clienti */
        so.non_blocans = FALSUM;
        sr = tcp_servus_creare_cum_optionibus(NIHIL, 0, &so, p);

        close(fistula[0]);
        si (   !sr.successus
            || tcp_servus_auscultare(sr.servus, I) != TCP_OK)
        {
            _exit(I);
        }
        portus_infantis = tcp_servus_obtinere_portum(sr.servus);
        si (write(fistula[I], &portus_infantis,
                  magnitudo(portus_infantis))
            != (ssize_t)magnitudo(portus_infantis))
        {
            _exit(I);
        }
        close(fistula[I]);

        conn = tcp_servus_accipere(sr.servus, p);
        si (!conn.successus)
        {
            _exit(I);
        }
        {
            /* haurire petitionem TOTAM (usque '\r\n\r\n') - clausura
             * cum octetis illectis RST pro FIN gigneret et cliens
             * errorem IO pro EOF mundo videret */
            s32 n;
            b32 completa     = FALSUM;
            i32 iterationes  = 0;

            fac
            {
                n = tcp_recipere(conn.connexio, buffer,
                                 (i32)magnitudo(buffer));
                si (n >= IV)
                {
                    i32 k;

                    per (k = 0; k + III < (i32)n; k++)
                    {
                        si (   buffer[k]       == '\r'
                            && buffer[k + I]   == '\n'
                            && buffer[k + II]  == '\r'
                            && buffer[k + III] == '\n')
                        {
                            completa = VERUM;
                        }
                    }
                }
                iterationes++;
            } dum (!completa && n > 0 && iterationes < X);
        }

        si (modus == FIXTURA_PRAVA_MINUSCULUM)
        {
            (vacuum)tcp_mittere_omnia(conn.connexio,
                (constans i8*)"ok", II);
        }
        alioquin si (   modus == FIXTURA_CHUNKED_INTEGRUM
                     || modus == FIXTURA_CHUNKED_TRUNCATUM
                     || modus == FIXTURA_CHUNKED_INGENS
                     || modus == FIXTURA_CAPUT_SOLUM)
        {
            constans character* r;

            si (modus == FIXTURA_CHUNKED_INTEGRUM)
            {
                r = "HTTP/1.1 200 OK\r\n"
                    "Transfer-Encoding: chunked\r\n\r\n"
                    "5\r\nsalve\r\n7\r\n munde!\r\n0\r\n\r\n";
            }
            alioquin si (modus == FIXTURA_CHUNKED_TRUNCATUM)
            {
                r = "HTTP/1.1 200 OK\r\n"
                    "Transfer-Encoding: chunked\r\n\r\n"
                    "5\r\nsalve\r\n7\r\n mu";
            }
            alioquin si (modus == FIXTURA_CHUNKED_INGENS)
            {
                r = "HTTP/1.1 200 OK\r\n"
                    "Transfer-Encoding: chunked\r\n\r\n"
                    "100000005\r\nsalve\r\n0\r\n\r\n";
            }
            alioquin
            {
                r = "HTTP/1.1 200 OK\r\n"
                    "Content-Length: 5000\r\n\r\n";
            }
            (vacuum)tcp_mittere_omnia(conn.connexio,
                (constans i8*)r, (i32)strlen(r));
        }
        alioquin si (modus == FIXTURA_CORPUS_MAGNUM)
        {
            constans character* capita =
                "HTTP/1.1 200 OK\r\n"
                "Content-Length: 200000\r\n\r\n";
            i8* corpus = (i8*)piscina_allocare(p,
                (i64)CORPUS_MAGNUM_MENSURA);
            i32 k;

            per (k = 0; k < CORPUS_MAGNUM_MENSURA; k++)
            {
                corpus[k] = (i8)('a' + (character)(k % XXVI));
            }
            (vacuum)tcp_mittere_omnia(conn.connexio,
                (constans i8*)capita, (i32)strlen(capita));
            (vacuum)tcp_mittere_omnia(conn.connexio, corpus,
                CORPUS_MAGNUM_MENSURA);
        }
        alioquin si (modus == FIXTURA_PRAVA_CAPUT_INGENS)
        {
            constans character* r =
                "HTTP/1.1 200 OK\r\n"
                "Content-Length: 99999999999999999999\r\n\r\n"
                "parvum";
            (vacuum)tcp_mittere_omnia(conn.connexio,
                (constans i8*)r, (i32)strlen(r));
        }
        alioquin
        {
            /* TRUNCATUM et DORMIENS: capita integra, X ex C octetis */
            constans character* r =
                "HTTP/1.1 200 OK\r\n"
                "Content-Length: 100\r\n\r\n"
                "0123456789";
            (vacuum)tcp_mittere_omnia(conn.connexio,
                (constans i8*)r, (i32)strlen(r));
            si (modus == FIXTURA_PRAVA_DORMIENS)
            {
                usleep(M * M * II);   /* II s - ultra tempus clientis */
            }
        }
        tcp_claudere(conn.connexio);
        _exit(0);
    }

    close(fistula[I]);
    si (read(fistula[0], &portus, magnitudo(portus))
        != (ssize_t)magnitudo(portus))
    {
        portus = 0;
    }
    close(fistula[0]);
    *pid_exitus = pid;
    redde portus;
}


/* ========================================================================
 * PROBATIONES - METHODUS NOMEN
 * ======================================================================== */

interior vacuum
probatio_methodus_nomen(Piscina* piscina)
{
    (vacuum)piscina;

    printf("--- Probans methodus nomen ---\n");

    CREDO_VERUM(strcmp(http_methodus_nomen(HTTP_GET), "GET") == 0);
    CREDO_VERUM(strcmp(http_methodus_nomen(HTTP_POST), "POST") == 0);
    CREDO_VERUM(strcmp(http_methodus_nomen(HTTP_PUT), "PUT") == 0);
    CREDO_VERUM(strcmp(http_methodus_nomen(HTTP_DELETE), "DELETE")
        == 0);
    CREDO_VERUM(strcmp(http_methodus_nomen(HTTP_PATCH), "PATCH") == 0);
    CREDO_VERUM(strcmp(http_methodus_nomen(HTTP_HEAD), "HEAD") == 0);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - ERROR DESCRIPTIO
 * ======================================================================== */

interior vacuum
probatio_error_descriptio(Piscina* piscina)
{
    constans character* desc;

    (vacuum)piscina;

    printf("--- Probans error descriptio ---\n");

    desc = http_error_descriptio(HTTP_OK);
    CREDO_NON_NIHIL(desc);

    desc = http_error_descriptio(HTTP_ERROR_URL);
    CREDO_NON_NIHIL(desc);

    desc = http_error_descriptio(HTTP_ERROR_CONNEXIO);
    CREDO_NON_NIHIL(desc);

    desc = http_error_descriptio(HTTP_ERROR_PARSE);
    CREDO_NON_NIHIL(desc);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PETITIO CREARE (parse solum, nulla connexio)
 * ======================================================================== */

interior vacuum
probatio_petitio_creare(Piscina* piscina)
{
    HttpPetitio* pet;

    printf("--- Probans petitio creare ---\n");

    /* HTTPS URL */
    pet = http_petitio_creare(piscina, HTTP_GET,
        "https://httpbin.org/get");
    CREDO_NON_NIHIL(pet);

    /* HTTP URL */
    pet = http_petitio_creare(piscina, HTTP_POST,
        "http://httpbin.org/post");
    CREDO_NON_NIHIL(pet);

    /* URL cum portus */
    pet = http_petitio_creare(piscina, HTTP_GET,
        "https://httpbin.org:443/get");
    CREDO_NON_NIHIL(pet);

    /* URL invalida */
    pet = http_petitio_creare(piscina, HTTP_GET, "invalid-url");
    CREDO_NIHIL(pet);

    /* NIHIL argumenta */
    pet = http_petitio_creare(NIHIL, HTTP_GET,
        "https://httpbin.org/get");
    CREDO_NIHIL(pet);

    pet = http_petitio_creare(piscina, HTTP_GET, NIHIL);
    CREDO_NIHIL(pet);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - GET (FIXTURA LOCALIS)
 * ======================================================================== */

interior vacuum
probatio_get_localis(Piscina* piscina, i32 portus)
{
     HttpPetitio* pet;
    HttpResultus  res;
       character  url[CXXVIII];

    printf("--- Probans GET (fixtura localis) ---\n");

    pet = http_petitio_creare(piscina, HTTP_GET,
                              _url_fixturae(url, portus, "/get"));
    CREDO_NON_NIHIL(pet);

    res = http_exsequi(pet, piscina);

    CREDO_VERUM(res.successus);
    CREDO_NON_NIHIL(res.responsum);
    CREDO_AEQUALIS_I32(res.responsum->status, CC);
    CREDO_VERUM(res.responsum->corpus.mensura > XX);

    printf("  Status: %d, corpus: %d bytes\n",
           res.responsum->status, res.responsum->corpus.mensura);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - POST (FIXTURA LOCALIS)
 * ======================================================================== */

interior vacuum
probatio_post_localis(Piscina* piscina, i32 portus)
{
           HttpPetitio* pet;
          HttpResultus  res;
             character  url[CXXVIII];
    constans character* corpus = "{\"name\": \"test\", \"value\": 42}";

    printf("--- Probans POST echo (fixtura localis) ---\n");

    pet = http_petitio_creare(piscina, HTTP_POST,
                              _url_fixturae(url, portus, "/post"));
    CREDO_NON_NIHIL(pet);

    http_petitio_caput_addere(pet, "Content-Type", "application/json");
    http_petitio_corpus_ponere(pet, corpus, (i32)strlen(corpus));

    res = http_exsequi(pet, piscina);

    CREDO_VERUM(res.successus);
    CREDO_NON_NIHIL(res.responsum);
    CREDO_AEQUALIS_I32(res.responsum->status, CC);

    /* fixtura corpus resonat - echo probat corpus trans filum */
    CREDO_VERUM(chorda_aequalis_literis(res.responsum->corpus, corpus));

    printf("  Status: %d, echo: %d bytes\n",
           res.responsum->status, res.responsum->corpus.mensura);
    printf("\n");
}


/* ========================================================================
 * PROBATIONES - HEADERS (FIXTURA LOCALIS)
 * ======================================================================== */

interior vacuum
probatio_headers(Piscina* piscina, i32 portus)
{
     HttpPetitio* pet;
    HttpResultus  res;
       character  url[CXXVIII];
          chorda  content_type;

    printf("--- Probans headers (fixtura localis) ---\n");

    pet = http_petitio_creare(piscina, HTTP_GET,
                              _url_fixturae(url, portus, "/headers"));
    CREDO_NON_NIHIL(pet);

    http_petitio_caput_addere(pet, "X-Custom-Header", "test-value");
    http_petitio_caput_addere(pet, "Accept", "application/json");

    res = http_exsequi(pet, piscina);

    CREDO_VERUM(res.successus);
    CREDO_AEQUALIS_I32(res.responsum->status, CC);

    content_type = http_responsum_caput(res.responsum, "Content-Type");
    CREDO_VERUM(content_type.mensura > 0);
    CREDO_VERUM(chorda_aequalis_literis(content_type,
        "application/json"));
    printf("  Content-Type: %.*s\n", content_type.mensura,
        content_type.datum);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - STATUS CODES (FIXTURA LOCALIS)
 * ======================================================================== */

interior vacuum
probatio_status_codes(Piscina* piscina, i32 portus)
{
     HttpPetitio* pet;
    HttpResultus  res;
       character  url[CXXVIII];

    printf("--- Probans status codes (fixtura localis) ---\n");

    /* 404 Not Found per /status/:codex */
    pet = http_petitio_creare(piscina, HTTP_GET,
                              _url_fixturae(url, portus,
                              "/status/404"));
    CREDO_NON_NIHIL(pet);

    res = http_exsequi(pet, piscina);
    CREDO_VERUM(res.successus);
    CREDO_AEQUALIS_I32(res.responsum->status, CDIV);
    printf("  404: %.*s\n",
           res.responsum->status_descriptio.mensura,
           res.responsum->status_descriptio.datum);

    /* 503 quoque */
    pet = http_petitio_creare(piscina, HTTP_GET,
                              _url_fixturae(url, portus,
                              "/status/503"));
    CREDO_NON_NIHIL(pet);

    res = http_exsequi(pet, piscina);
    CREDO_VERUM(res.successus);
    CREDO_AEQUALIS_I32(res.responsum->status, DIII);
    printf("  503: %.*s\n",
           res.responsum->status_descriptio.mensura,
           res.responsum->status_descriptio.datum);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - CONNEXIO INVALIDA
 * ======================================================================== */

interior vacuum
probatio_connexio_invalida(Piscina* piscina)
{
     HttpPetitio* pet;
    HttpResultus  res;

    printf("--- Probans connexio invalida ---\n");

    pet = http_petitio_creare(piscina, HTTP_GET,
                              "https://non.existens.hospes.example/path");
    CREDO_NON_NIHIL(pet);

    res = http_exsequi(pet, piscina);

    CREDO_FALSUM(res.successus);
    CREDO_AEQUALIS_I32(res.error, HTTP_ERROR_CONNEXIO);

    printf("  Error: %s\n", http_error_descriptio(res.error));
    printf("\n");
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */


/* ========================================================================
 * PROBATIONES - SEMITAE ERRORIS ANSAE HAURIENTIS (fixtura prava)
 * ======================================================================== */

/* responsum minusculum (II octeti): olim scansio capitum
 * 'total_size - III' involvebatur et extra fines currebat;
 * nunc PARSE error mundus */
interior vacuum
probatio_responsum_minusculum(Piscina* piscina)
{
           pid_t  pid = 0;
             i32  portus;
       character  url[CXXVIII];
     HttpPetitio* pet;
    HttpResultus  res;

    printf("--- Probans responsum minusculum (scansio intra fines) ---\n");

    portus = _fixturam_pravam_incipere(&pid, FIXTURA_PRAVA_MINUSCULUM);
    CREDO_VERUM(portus > 0);

    pet = http_petitio_creare(piscina, HTTP_GET,
                              _url_fixturae(url, portus, "/"));
    CREDO_NON_NIHIL(pet);
    res = http_exsequi(pet, piscina);

    CREDO_FALSUM(res.successus);
    CREDO_AEQUALIS_I32((i32)res.error, (i32)HTTP_ERROR_PARSE);
    si (!res.successus && res.error_descriptio.mensura > 0)
    {
        printf("  [descriptio] %.*s\n",
               (int)res.error_descriptio.mensura,
               (constans character*)res.error_descriptio.datum);
    }

    _fixturam_terminare(pid);
    printf("\n");
}

/* truncatio cum EOF MUNDO: hospes claudit post X ex C octetis.
 * OLIM successus cum corpore X octetorum (EOF ut terminus legitimus
 * habebatur); sed Content-Length C promisit - RFC 9112 6.3: nuntius
 * incompletus. Corpus mendax peius quam error honestus (lapide
 * briar-feedback bugs/016: README 7351 octetorum ut 0 redibat). */
interior vacuum
probatio_truncatum_eof(Piscina* piscina)
{
           pid_t  pid = 0;
             i32  portus;
       character  url[CXXVIII];
     HttpPetitio* pet;
    HttpResultus  res;

    printf("--- Probans truncationem cum EOF mundo ---\n");

    portus = _fixturam_pravam_incipere(&pid, FIXTURA_PRAVA_TRUNCATUM);
    CREDO_VERUM(portus > 0);

    pet = http_petitio_creare(piscina, HTTP_GET,
                              _url_fixturae(url, portus, "/"));
    CREDO_NON_NIHIL(pet);
    res = http_exsequi(pet, piscina);

    CREDO_FALSUM(res.successus);
    CREDO_AEQUALIS_I32((i32)res.error, (i32)HTTP_ERROR_IO);
    /* error NOMINAT: promissum et receptum */
    CREDO_VERUM(chorda_aequalis_literis(res.error_descriptio,
        "Corpus truncatum: Content-Length 100, recepti 10"
        " (corpus partiale abiectum)"));
    printf("  [descriptio] %.*s\n", (int)res.error_descriptio.mensura,
           (constans character*)res.error_descriptio.datum);

    _fixturam_terminare(pid);
    printf("\n");
}

/* hospes DORMIENS post corpus partiale: SO_RCVTIMEO ictum ->
 * HTTP_ERROR_TIMEOUT, corpus partiale abiectum (01KYANH7AN -
 * olim 'successus' cum corpore truncato) */
interior vacuum
probatio_tempus_receptionis(Piscina* piscina)
{
           pid_t  pid = 0;
             i32  portus;
       character  url[CXXVIII];
     HttpPetitio* pet;
    HttpResultus  res;

    printf("--- Probans tempus receptionis (hospes dormiens) ---\n");

    portus = _fixturam_pravam_incipere(&pid, FIXTURA_PRAVA_DORMIENS);
    CREDO_VERUM(portus > 0);

    pet = http_petitio_creare(piscina, HTTP_GET,
                              _url_fixturae(url, portus, "/"));
    CREDO_NON_NIHIL(pet);
    http_petitio_tempus_ponere(pet, D);   /* D ms << II s somni */
    res = http_exsequi(pet, piscina);

    CREDO_FALSUM(res.successus);
    CREDO_AEQUALIS_I32((i32)res.error, (i32)HTTP_ERROR_TIMEOUT);

    _fixturam_terminare(pid);
    printf("\n");
}

/* Content-Length involutivum (XX digiti): saturatio pro involutione
 * (01KY05Q8AH) - expected_len saturatum > body_len -> error
 * truncati (involutio numerum parvum daret et corpus mendax
 * transiret; olim successus cum corpore reali VI octetorum) */
interior vacuum
probatio_caput_ingens(Piscina* piscina)
{
           pid_t  pid = 0;
             i32  portus;
       character  url[CXXVIII];
     HttpPetitio* pet;
    HttpResultus  res;

    printf("--- Probans Content-Length involutivum (saturatio) ---\n");

    portus = _fixturam_pravam_incipere(&pid,
        FIXTURA_PRAVA_CAPUT_INGENS);
    CREDO_VERUM(portus > 0);

    pet = http_petitio_creare(piscina, HTTP_GET,
                              _url_fixturae(url, portus, "/"));
    CREDO_NON_NIHIL(pet);
    res = http_exsequi(pet, piscina);

    CREDO_FALSUM(res.successus);
    CREDO_AEQUALIS_I32((i32)res.error, (i32)HTTP_ERROR_IO);
    printf("  [descriptio] %.*s\n", (int)res.error_descriptio.mensura,
           (constans character*)res.error_descriptio.datum);

    _fixturam_terminare(pid);
    printf("\n");
}

/* Fixtura una, petitio una: modus, methodus -> resultatus */
interior HttpResultus
_petere_a_fixtura (
         Piscina* piscina,
         integer  modus,
    HttpMethodus  methodus)
{
           pid_t  pid = 0;
             i32  portus;
       character  url[CXXVIII];
     HttpPetitio* pet;
    HttpResultus  res;

    portus = _fixturam_pravam_incipere(&pid, modus);
    CREDO_VERUM(portus > 0);
    pet = http_petitio_creare(piscina, methodus,
                              _url_fixturae(url, portus, "/"));
    CREDO_NON_NIHIL(pet);
    res = http_exsequi(pet, piscina);
    si (!res.successus)
    {
        printf("  [descriptio] %.*s\n",
            (int)res.error_descriptio.mensura,
               (constans character*)res.error_descriptio.datum);
    }
    _fixturam_terminare(pid);
    redde res;
}

/* chunked: integrum decodificatur; sine fragmento '0' = error
 * (olim 'copiare quod possumus' cum successu); magnitudo '100000005'
 * (0x100000005) in i32 ad V involvebatur - 'salve' ut corpus validum
 * transibat (et 'FFFFFFFF' custodiam 'i + chunk_size <= len' ipsam
 * involvebat: memcpy IV GB) */
interior vacuum
probatio_chunked(Piscina* piscina)
{
    HttpResultus res;

    printf("--- Probans chunked (integrum, truncatum, ingens) ---\n");

    res = _petere_a_fixtura(piscina, FIXTURA_CHUNKED_INTEGRUM,
        HTTP_GET);
    CREDO_VERUM(res.successus);
    si (res.successus)
    {
        CREDO_VERUM(chorda_aequalis_literis(res.responsum->corpus,
                                            "salve munde!"));
    }

    res = _petere_a_fixtura(piscina, FIXTURA_CHUNKED_TRUNCATUM,
        HTTP_GET);
    CREDO_FALSUM(res.successus);
    CREDO_AEQUALIS_I32((i32)res.error, (i32)HTTP_ERROR_IO);

    res = _petere_a_fixtura(piscina, FIXTURA_CHUNKED_INGENS, HTTP_GET);
    CREDO_FALSUM(res.successus);
    CREDO_AEQUALIS_I32((i32)res.error, (i32)HTTP_ERROR_IO);

    printf("\n");
}

/* HEAD: Content-Length nuntiat magnitudinem, corpus nullum per legem -
 * successus, non error truncati */
interior vacuum
probatio_head_sine_corpore(Piscina* piscina)
{
    HttpResultus res;

    printf("--- Probans HEAD (Content-Length sine corpore) ---\n");

    res = _petere_a_fixtura(piscina, FIXTURA_CAPUT_SOLUM, HTTP_HEAD);
    CREDO_VERUM(res.successus);
    si (res.successus)
    {
        CREDO_AEQUALIS_I32(res.responsum->corpus.mensura, 0);
    }

    printf("\n");
}

/* corpus CC milia octetorum: multae lectiones, octeti omnes, ordine */
interior vacuum
probatio_corpus_magnum(Piscina* piscina)
{
    HttpResultus res;

    printf("--- Probans corpus magnum (CC milia octetorum) ---\n");

    res = _petere_a_fixtura(piscina, FIXTURA_CORPUS_MAGNUM, HTTP_GET);
    CREDO_VERUM(res.successus);
    si (res.successus)
    {
        i32 k;
        b32 ordo = VERUM;

        CREDO_AEQUALIS_I32(res.responsum->corpus.mensura,
                           CORPUS_MAGNUM_MENSURA);
        per (k = 0; k < res.responsum->corpus.mensura; k++)
        {
            si (res.responsum->corpus.datum[k]
                != (i8)('a' + (character)(k % XXVI)))
            {
                ordo = FALSUM;
                frange;
            }
        }
        CREDO_VERUM(ordo);
    }

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - VISUS PETITIONIS ET VECTURA (vates-plan-1 T2)
 * ======================================================================== */

interior vacuum
probatio_petitio_visus(Piscina* piscina)
{
         HttpPetitio* pet;
    HttpPetitioVisus  v;

    printf("--- Probans http_petitio_visus ---\n");

    pet = http_petitio_creare(piscina, HTTP_POST,
        "https://api.example.com:8443/v1/messages?beta=1");
    CREDO_NON_NIHIL(pet);
    http_petitio_caput_addere(pet, "content-type", "application/json");
    http_petitio_caput_addere(pet, "anthropic-version", "2023-06-01");
    http_petitio_corpus_ponere(pet, "{\"a\":1}", VII);
    http_petitio_tempus_ponere(pet, DC * M);

    v = http_petitio_visus(pet);
    CREDO_VERUM(v.methodus == HTTP_POST);
    CREDO_CHORDA_AEQUALIS_LITERIS(v.schema, "https");
    CREDO_CHORDA_AEQUALIS_LITERIS(v.hospes, "api.example.com");
    CREDO_AEQUALIS_I32(v.portus, 8443);
    CREDO_CHORDA_INCIPIT(v.via, chorda_ex_literis("/v1/messages",
        piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(v.corpus, "{\"a\":1}");
    CREDO_AEQUALIS_I32(v.capita_numerus, II);
    CREDO_CHORDA_AEQUALIS_LITERIS(v.capita[I].titulus,
        "anthropic-version");
    CREDO_CHORDA_AEQUALIS_LITERIS(v.capita[I].valor, "2023-06-01");
    CREDO_AEQUALIS_S32(v.tempus_ms, DC * M);

    /* NIHIL -> visus vacuus, non ruina */
    v = http_petitio_visus(NIHIL);
    CREDO_AEQUALIS_I32(v.capita_numerus, 0);
    CREDO_NIHIL(v.capita);

    printf("\n");
}

hic_manens i32 _vectura_ficta_vocata = 0;

interior HttpResultus
_vectura_ficta (
    HttpPetitio* petitio,
        Piscina* piscina,
         vacuum* datum)
{
     HttpResultus  res;
    HttpResponsum* resp;

    (vacuum)petitio;
    _vectura_ficta_vocata += *(i32*)datum;
    resp = (HttpResponsum*)piscina_allocare(piscina,
        (i64)magnitudo(HttpResponsum));
    memset(resp, 0, magnitudo(*resp));
    resp->status = CDXVIII;
    resp->corpus = chorda_ex_literis("ficta", piscina);
    memset(&res, 0, magnitudo(res));
    res.successus = VERUM;
    res.responsum = resp;
    redde res;
}

interior vacuum
probatio_vectura(Piscina* piscina, i32 portus)
{
     HttpVectura  v;
     HttpPetitio* pet;
    HttpResultus  res;
       character  url[CCLVI];
             i32  incrementum = VII;

    printf("--- Probans HttpVectura ---\n");

    /* ordinaria = http_exsequi verum, contra fixturam localem */
    pet = http_petitio_creare(piscina, HTTP_GET,
                              _url_fixturae(url, portus, "/get"));
    res = http_vectura_exsequi(http_vectura_ordinaria(), pet, piscina);
    CREDO_VERUM(res.successus);
    si (res.successus)
    {
        CREDO_AEQUALIS_I32(res.responsum->status, CC);
        CREDO_CHORDA_CONTINET(res.responsum->corpus,
            chorda_ex_literis("fixtura localis", piscina));
    }

    /* ficta: functio vocatur cum datis suis, responsum eius redditur */
    v.exsequi  = _vectura_ficta;
    v.datum    = &incrementum;
    res        = http_vectura_exsequi(v, pet, piscina);
    CREDO_VERUM(res.successus);
    CREDO_AEQUALIS_I32(_vectura_ficta_vocata, VII);
    CREDO_AEQUALIS_I32(res.responsum->status, CDXVIII);

    /* sine functione: error nominatus, non ruina */
    v.exsequi  = NIHIL;
    res        = http_vectura_exsequi(v, pet, piscina);
    CREDO_FALSUM(res.successus);
    CREDO_VERUM(res.error == HTTP_ERROR_CONNEXIO);

    printf("\n");
}


integer
principale(vacuum)
{
    Piscina* piscina;
        b32  successus;
      pid_t  fixtura_pid = 0;
        i32  portus;

    printf("\n");
    printf("========================================\n");
    printf("PROBATIONES HTTP (fixtura localis)\n");
    printf("========================================\n");
    printf("\n");

    piscina = piscina_generare_dynamicum("probatio_http", CXXVIII * M);
    credo_aperire(piscina);

    /* Probationes locales (nulla connexio) */
    probatio_methodus_nomen(piscina);
    probatio_error_descriptio(piscina);
    probatio_petitio_creare(piscina);
    probatio_petitio_visus(piscina);
    probatio_connexio_invalida(piscina);

    /* Fixtura: hospitium filiatum, portus auto */
    portus = _fixturam_incipere(&fixtura_pid);
    CREDO_VERUM(portus > 0);
    printf("[fixtura] hospitium filiatum in portu %u (pid %ld)\n\n",
           portus, (longus)fixtura_pid);

    si (portus > 0)
    {
        probatio_get_localis(piscina, portus);
        probatio_post_localis(piscina, portus);
        probatio_headers(piscina, portus);
        probatio_status_codes(piscina, portus);
        probatio_vectura(piscina, portus);
    }

    _fixturam_terminare(fixtura_pid);

    /* semitae erroris (fixturae pravae propriae, una connexio) */
    probatio_responsum_minusculum(piscina);
    probatio_truncatum_eof(piscina);
    probatio_tempus_receptionis(piscina);
    probatio_caput_ingens(piscina);
    probatio_chunked(piscina);
    probatio_head_sine_corpore(piscina);
    probatio_corpus_magnum(piscina);

    credo_imprimere_compendium();

    printf("========================================\n");
    printf("\n");

    successus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    redde successus ? 0 : I;
}
