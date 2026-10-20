/* probatio_hospitium_auxilia.c - hospitium v3 T4: auxilia
 * petitionis. Quaestio ('+', %20, claves repetitae), caput sine casu,
 * forma (urlencoded et multipartita), crustula (lecta, duo posita,
 * defaltae tutae), procurator fidus (X-Forwarded-For ultimus saltus,
 * -Proto) solum si configuratum, redirectio 303 (CR/LF recusatum),
 * tectum capitum XXXII (XXXIII abiectum, in actis nominatum), curl
 * cum vase crustulorum (-c/-b). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "filum.h"
#include "moneta.h"
#include "processus.h"
#include "tcp.h"
#include "hospitium.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CAPSA_CAPITUM (IV * MXXIV)

interior vacuum
_pumpare (
    Hospitium* h,
          i32  vices)
{
    i32 i;

    per (i = ZEPHYRUM; i < vices; i++)
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
             Hospitium* h,
           TcpConnexio* c,
    constans character* s)
{
    b32 bene = tcp_mittere_omnia(c, (constans i8*)s, (i32)strlen(s));

    _pumpare(h, I);
    redde bene;
}

/* responsum totum (capita + corpus); capita in 'capita' */
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
                s32 longitudo_capitum = (s32)(finis - capita) + IV;
                s32 reliqua = (s32)n - (longitudo_capitum
                    - ((s32)accepta - (s32)m));

                corporis = cl != NIHIL ? (s64)atol(cl + XVI) : ZEPHYRUM;
                si (   reliqua > ZEPHYRUM && corpus != NIHIL
                    && (s64)reliqua <= corpus_n)
                {
                    memcpy(corpus, capsa + ((s32)n - reliqua),
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

interior b32
_aequalis (
                chorda  a,
    constans character* b)
{
    redde a.datum != NIHIL && a.mensura == (i32)strlen(b)
        && memcmp(a.datum, b, (size_t)a.mensura) == ZEPHYRUM;
}

interior i32
_numerare (
    constans character* a,
    constans character* b)
{
    i32 n = ZEPHYRUM;

    dum ((a = strstr(a, b)) != NIHIL)
    {
        n++;
        a += strlen(b);
    }
    redde n;
}

/* ---- status tractatorum (copiae in piscina sectionis) ---- */

nomen structura {
     Piscina* p;
         i32  vocationes;
      chorda  q_a;
      chorda  q_b;
      chorda  q_c;
      chorda  q_d;
         i32  q_numerus;
      chorda  q_a_secundum;
      chorda  caput_minusc;
      chorda  caput_maiusc;
      chorda  caput_absens;
      chorda  forma_titulus;
      chorda  forma_nota;
      chorda  forma_absens;
      chorda  crust_sid;
      chorda  crust_theme;
      chorda  cliens;
      chorda  schema;
         b32  posita;
} Status;

interior chorda
_copia (
      chorda  s,
     Piscina* p)
{
    chorda c;

    si (s.datum == NIHIL)
    {
        redde s;
    }
    c.mensura  = s.mensura;
    c.datum    = (i8*)piscina_allocare(p, (i64)s.mensura + I);
    si (s.mensura > ZEPHYRUM)
    {
        memcpy(c.datum, s.datum, (size_t)s.mensura);
    }
    redde c;
}

interior vacuum
_omnia (
    HospitiumColloquium* col)
{
       Status* s = (Status*)colloquium_datum(col);
      Piscina* p = s->p;
    UrlParams  q;

    s->vocationes++;
    s->q_a        = _copia(colloquium_quaestio(col, "a"), p);
    s->q_b        = _copia(colloquium_quaestio(col, "b"), p);
    s->q_c        = _copia(colloquium_quaestio(col, "c"), p);
    s->q_d        = _copia(colloquium_quaestio(col, "d"), p);
    q             = colloquium_quaestiones(col);
    s->q_numerus  = q.numerus;
    si (q.numerus > II)
    {
        s->q_a_secundum = _copia(q.valores[II], p);
    }
    s->caput_minusc   = _copia(colloquium_caput(col, "x-proba"), p);
    s->caput_maiusc   = _copia(colloquium_caput(col, "X-PROBA"), p);
    s->caput_absens   = _copia(colloquium_caput(col, "X-Absens"), p);
    s->forma_titulus  = _copia(colloquium_forma(col, "titulus"), p);
    s->forma_nota     = _copia(colloquium_forma(col, "nota"), p);
    s->forma_absens   = _copia(colloquium_forma(col, "absens"), p);
    s->crust_sid      = _copia(colloquium_crustulum(col, "sid"), p);
    s->crust_theme    = _copia(colloquium_crustulum(col, "theme"), p);
    s->cliens         = _copia(colloquium_cliens(col), p);
    s->schema         = _copia(colloquium_schema(col), p);
    s->posita = colloquium_crustulum_ponere(col, "sid",
        chorda_ex_literis("novum", colloquium_piscina(col)), NIHIL)
        && colloquium_crustulum_ponere(col, "theme",
            chorda_ex_literis("lux", colloquium_piscina(col)), NIHIL);
    colloquium_respondere(col, CC, "text/plain",
        chorda_ex_literis("bene", colloquium_piscina(col)));
}

interior vacuum
_redirigere (
    HospitiumColloquium* col)
{
    colloquium_redirigere(col, CCCIII, "/alibi");
}

interior vacuum
_locus_malus (
    HospitiumColloquium* col)
{
    colloquium_redirigere(col, CCCIII, "/a\r\nSet-Cookie: x=1");
}

interior vacuum
_tectum (
    HospitiumColloquium* col)
{
    character t[XXXII];
          i32 i;

    per (i = I; i <= XXXIII; i++)
    {
        sprintf(t, "X-H-%u", (insignatus integer)i);
        colloquium_caput_addere(col, t, "v");
    }
    colloquium_respondere(col, CC, "text/plain",
        chorda_ex_literis("tectum", colloquium_piscina(col)));
}

interior vacuum
_malum (
    HospitiumColloquium* col)
{
    colloquium_caput_addere(col, "X-Malum", "a\r\nSet-Cookie: x=1");
    colloquium_caput_addere(col, "X-Bonum", "b");
    colloquium_respondere(col, CC, "text/plain",
        chorda_ex_literis("malum", colloquium_piscina(col)));
}

/* vas crustulorum: prima vocatio ponit, secunda legit */
interior vacuum
_vas (
    HospitiumColloquium* col)
{
     Status* s    = (Status*)colloquium_datum(col);
     chorda  sid  = colloquium_crustulum(col, "sid");

    s->vocationes++;
    si (sid.datum == NIHIL)
    {
        (vacuum)colloquium_crustulum_ponere(col, "sid",
            chorda_ex_literis("xyz", colloquium_piscina(col)), NIHIL);
        colloquium_respondere(col, CC, "text/plain",
            chorda_ex_literis("novum", colloquium_piscina(col)));
        redde;
    }
    s->crust_sid = _copia(sid, s->p);
    colloquium_respondere(col, CC, "text/plain",
        chorda_ex_literis("vetus", colloquium_piscina(col)));
}

interior Hospitium*
_servum_creare (
    CredoContextus* c,
               b32  fidus,
               b32  acta,
            Status* s)
{
    HospitiumConfiguratio  cfg;
                Hospitium* h;

    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.procurator_fidus  = fidus;
    cfg.acta_accessus     = acta;
    h                     = hospitium_creare(c->piscina, &cfg);
    si (h != NIHIL)
    {
        memset(s, ZEPHYRUM, magnitudo(*s));
        s->p = c->piscina;
        (vacuum)hospitium_praebere(h, HTTP_GET, "/omnia", _omnia, s);
        (vacuum)hospitium_praebere(h, HTTP_POST, "/omnia", _omnia, s);
        (vacuum)hospitium_praebere(h, HTTP_GET, "/redirigere",
            _redirigere, s);
        (vacuum)hospitium_praebere(h, HTTP_GET, "/injectio",
            _locus_malus,
            s);
        (vacuum)hospitium_praebere(h, HTTP_GET, "/tectum", _tectum, s);
        (vacuum)hospitium_praebere(h, HTTP_GET, "/malum", _malum, s);
        (vacuum)hospitium_praebere(h, HTTP_GET, "/vas", _vas, s);
    }
    redde h;
}

/* petitio una in connexione nova; capita responsi in 'capita' */
interior s64
_rogare (
        CredoContextus* c,
             Hospitium* h,
    constans character* petitio,
             character* capita,
                   i32  capita_n)
{
     TcpConnexio* a = _cliens(c->piscina, hospitium_portus(h));
              i8  corpus[CCLVI];
             s64  n;

    si (a == NIHIL || !_mittere(h, a, petitio))
    {
        redde -I;
    }
    n = _legere_responsum(h, a, capita, capita_n, corpus, CCLVI, CC);
    tcp_claudere(a);
    redde n;
}

/* ---- sectiones ---- */

interior vacuum
_sectio_quaestio (CredoContextus* c)
{
       Status  s;
    Hospitium* h = _servum_creare(c, FALSUM, FALSUM, &s);
    character  capita[CAPSA_CAPITUM];

    CREDO_NECESSE_NON_NIHIL(h);
    CREDO_AEQUALIS_S32((s32)_rogare(c, h,
        "GET /omnia?a=1+2&b=%20x&a=3&c HTTP/1.1\r\nHost: x\r\n"
        "X-Proba: valor\r\n\r\n", capita, (i32)magnitudo(capita)),
        (s32)IV);
    CREDO_AEQUALIS_I32(s.vocationes, I);
    CREDO_VERUM(_aequalis(s.q_a, "1 2"));      /* primum vincit */
    CREDO_VERUM(_aequalis(s.q_b, " x"));
    CREDO_VERUM(s.q_c.datum != NIHIL);         /* praesens, vacuum */
    CREDO_AEQUALIS_I32(s.q_c.mensura, ZEPHYRUM);
    CREDO_NIHIL(s.q_d.datum);
    CREDO_AEQUALIS_I32(s.q_numerus, IV);
    CREDO_VERUM(_aequalis(s.q_a_secundum, "3"));
    /* caput sine casu */
    CREDO_VERUM(_aequalis(s.caput_minusc, "valor"));
    CREDO_VERUM(_aequalis(s.caput_maiusc, "valor"));
    CREDO_NIHIL(s.caput_absens.datum);
    hospitium_destruere(h);
}

interior vacuum
_sectio_forma (CredoContextus* c)
{
                Status  s;
             Hospitium* h = _servum_creare(c, FALSUM, FALSUM, &s);
             character  capita[CAPSA_CAPITUM];
    constans character* corpus_u =
        "titulus=Salve+munde&nota=%C3%A9t%C3%A9&titulus=secundus";
    constans character* corpus_m =
        "--LimesF\r\n"
        "Content-Disposition: form-data; name=\"titulus\"\r\n\r\n"
        "Ave\r\n"
        "--LimesF\r\n"
        "Content-Disposition: form-data; name=\"nota\"\r\n\r\n"
        "multipartita\r\n"
        "--LimesF--\r\n";
    character petitio[MXXIV];

    CREDO_NECESSE_NON_NIHIL(h);
    sprintf(petitio, "POST /omnia HTTP/1.1\r\nHost: x\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: %u\r\n\r\n%s",
        (insignatus integer)strlen(corpus_u), corpus_u);
    CREDO_AEQUALIS_S32((s32)_rogare(c, h, petitio, capita,
        (i32)magnitudo(capita)), (s32)IV);
    CREDO_VERUM(_aequalis(s.forma_titulus, "Salve munde"));
    CREDO_VERUM(_aequalis(s.forma_nota, "\xC3\xA9t\xC3\xA9"));
    CREDO_NIHIL(s.forma_absens.datum);

    sprintf(petitio, "POST /omnia HTTP/1.1\r\nHost: x\r\n"
        "Content-Type: multipart/form-data; boundary=LimesF\r\n"
        "Content-Length: %u\r\n\r\n%s",
        (insignatus integer)strlen(corpus_m), corpus_m);
    CREDO_AEQUALIS_S32((s32)_rogare(c, h, petitio, capita,
        (i32)magnitudo(capita)), (s32)IV);
    CREDO_VERUM(_aequalis(s.forma_titulus, "Ave"));
    CREDO_VERUM(_aequalis(s.forma_nota, "multipartita"));

    /* forma in GET (sine corpore): nihil */
    CREDO_AEQUALIS_S32((s32)_rogare(c, h,
        "GET /omnia?titulus=q HTTP/1.1\r\nHost: x\r\n\r\n", capita,
        (i32)magnitudo(capita)), (s32)IV);
    CREDO_NIHIL(s.forma_titulus.datum);
    hospitium_destruere(h);
}

interior vacuum
_sectio_crustula (CredoContextus* c)
{
       Status  s;
    Hospitium* h = _servum_creare(c, FALSUM, FALSUM, &s);
    character  capita[CAPSA_CAPITUM];

    CREDO_NECESSE_NON_NIHIL(h);
    CREDO_AEQUALIS_S32((s32)_rogare(c, h,
        "GET /omnia HTTP/1.1\r\nHost: x\r\n"
        "Cookie: sid=abc; theme=\"dark\"\r\n\r\n", capita,
        (i32)magnitudo(capita)), (s32)IV);
    CREDO_VERUM(_aequalis(s.crust_sid, "abc"));
    CREDO_VERUM(_aequalis(s.crust_theme, "dark"));
    CREDO_VERUM(s.posita);
    /* duo capita Set-Cookie, defaltae tutae, sine Secure (http) */
    CREDO_AEQUALIS_I32(_numerare(capita, "Set-Cookie: "), II);
    CREDO_VERUM(strstr(capita,
        "Set-Cookie: sid=novum; Path=/; HttpOnly; SameSite=Lax\r\n")
        != NIHIL);
    CREDO_VERUM(strstr(capita,
        "Set-Cookie: theme=lux; Path=/; HttpOnly; SameSite=Lax\r\n")
        != NIHIL);
    CREDO_NIHIL(strstr(capita, "Secure"));

    /* capita Cookie duo: utrumque legitur */
    CREDO_AEQUALIS_S32((s32)_rogare(c, h,
        "GET /omnia HTTP/1.1\r\nHost: x\r\nCookie: theme=a\r\n"
        "Cookie: sid=b\r\n\r\n", capita, (i32)magnitudo(capita)),
        (s32)IV);
    CREDO_VERUM(_aequalis(s.crust_sid, "b"));
    CREDO_VERUM(_aequalis(s.crust_theme, "a"));
    hospitium_destruere(h);
}

hic_manens constans character* PETITIO_PROCURATA =
    "GET /omnia HTTP/1.1\r\nHost: x\r\n"
    "X-Forwarded-For: 1.2.3.4, 5.6.7.8\r\n"
    "X-Forwarded-Proto: https\r\n\r\n";

interior vacuum
_sectio_procurator (CredoContextus* c)
{
       Status  s;
    Hospitium* h = _servum_creare(c, FALSUM, FALSUM, &s);
    character  capita[CAPSA_CAPITUM];

    /* non fidus: capita ignorantur */
    CREDO_NECESSE_NON_NIHIL(h);
    CREDO_AEQUALIS_S32((s32)_rogare(c, h, PETITIO_PROCURATA, capita,
        (i32)magnitudo(capita)), (s32)IV);
    CREDO_VERUM(_aequalis(s.cliens, "127.0.0.1"));
    CREDO_VERUM(_aequalis(s.schema, "http"));
    CREDO_NIHIL(strstr(capita, "Secure"));
    hospitium_destruere(h);

    /* fidus et par loopback: ultimus saltus, https, Secure */
    h = _servum_creare(c, VERUM, FALSUM, &s);
    CREDO_NECESSE_NON_NIHIL(h);
    CREDO_AEQUALIS_S32((s32)_rogare(c, h, PETITIO_PROCURATA, capita,
        (i32)magnitudo(capita)), (s32)IV);
    CREDO_VERUM(_aequalis(s.cliens, "5.6.7.8"));
    CREDO_VERUM(_aequalis(s.schema, "https"));
    CREDO_VERUM(strstr(capita,
        "Set-Cookie: sid=novum; Path=/; Secure; HttpOnly; SameSite=Lax")
        != NIHIL);

    /* fidus sed sine capitibus: par ipse */
    CREDO_AEQUALIS_S32((s32)_rogare(c, h,
        "GET /omnia HTTP/1.1\r\nHost: x\r\n\r\n", capita,
        (i32)magnitudo(capita)), (s32)IV);
    CREDO_VERUM(_aequalis(s.cliens, "127.0.0.1"));
    CREDO_VERUM(_aequalis(s.schema, "http"));
    hospitium_destruere(h);
}

interior vacuum
_sectio_redirigere (CredoContextus* c)
{
       Status  s;
    Hospitium* h = _servum_creare(c, FALSUM, FALSUM, &s);
    character  capita[CAPSA_CAPITUM];

    CREDO_NECESSE_NON_NIHIL(h);
    CREDO_VERUM(_rogare(c, h,
        "GET /redirigere HTTP/1.1\r\nHost: x\r\n\r\n", capita,
        (i32)magnitudo(capita)) >= ZEPHYRUM);
    CREDO_VERUM(strncmp(capita, "HTTP/1.1 303", XII) == ZEPHYRUM);
    CREDO_VERUM(strstr(capita, "\r\nLocation: /alibi\r\n") != NIHIL);

    CREDO_VERUM(_rogare(c, h,
        "GET /injectio HTTP/1.1\r\nHost: x\r\n\r\n", capita,
        (i32)magnitudo(capita)) >= ZEPHYRUM);
    CREDO_VERUM(strncmp(capita, "HTTP/1.1 500", XII) == ZEPHYRUM);
    CREDO_NIHIL(strstr(capita, "Set-Cookie"));
    CREDO_NIHIL(strstr(capita, "Location"));
    hospitium_destruere(h);
}

interior vacuum
_sectio_capita_tectum (CredoContextus* c)
{
       Status  s;
    Hospitium* h = _servum_creare(c, FALSUM, VERUM, &s);
    character  capita[CAPSA_CAPITUM];
    character  acta[CAPSA_CAPITUM];
      integer  fistula[II];
      integer  servatum;
      ssize_t  n;

    CREDO_NECESSE_NON_NIHIL(h);
    /* stderr in fistulam: acta accessus capere */
    fflush(stderr);
    servatum = dup(II);
    CREDO_VERUM(pipe(fistula) == ZEPHYRUM);
    dup2(fistula[I], II);

    CREDO_VERUM(_rogare(c, h, "GET /tectum HTTP/1.1\r\nHost: x\r\n\r\n",
        capita, (i32)magnitudo(capita)) >= ZEPHYRUM);
    CREDO_VERUM(_rogare(c, h, "GET /malum HTTP/1.1\r\nHost: x\r\n\r\n",
        acta, (i32)magnitudo(acta)) >= ZEPHYRUM);

    fflush(stderr);
    dup2(servatum, II);
    close(servatum);
    close(fistula[I]);
    {
                 character  a[CAPSA_CAPITUM];
        constans character* malum_capita = acta;

        (vacuum)malum_capita;
        n = read(fistula[ZEPHYRUM], a, CAPSA_CAPITUM - I);
        close(fistula[ZEPHYRUM]);
        a[n > ZEPHYRUM ? n : ZEPHYRUM] = '\0';
        /* XXXIII abiectum et nominatum; injectio abiecta */
        CREDO_VERUM(strstr(a, "X-H-33") != NIHIL);
        CREDO_VERUM(strstr(a, "X-Malum") != NIHIL);
    }
    CREDO_VERUM(strstr(capita, "\r\nX-H-32: v\r\n") != NIHIL);
    CREDO_NIHIL(strstr(capita, "X-H-33"));
    CREDO_VERUM(strstr(capita, "Content-Type: text/plain") != NIHIL);
    CREDO_NIHIL(strstr(acta, "X-Malum"));
    CREDO_NIHIL(strstr(acta, "Set-Cookie"));
    CREDO_VERUM(strstr(acta, "\r\nX-Bonum: b\r\n") != NIHIL);
    CREDO_VERUM(hospitium_fructus(h).capita_abiecta == (i64)II);
    hospitium_destruere(h);
}

/* curl cum vase crustulorum: secunda petitio crustulum primae refert */
interior vacuum
_sectio_vas_crustulorum (CredoContextus* c)
{
                Status  s;
             Hospitium* h;
             character  signa[MONETA_NANOID_PLAGULAE_MENSURA];
             character* vas;
             character* url;
    constans character* argumenta[VIII];
                   i32  k;
             character  effusa[II][VIII];

    si (!filum_existit("/usr/bin/curl"))
    {
        imprimere("  NOTA: /usr/bin/curl absens - sectio omissa\n");
        redde;
    }
    h = _servum_creare(c, FALSUM, FALSUM, &s);
    CREDO_NECESSE_NON_NIHIL(h);
    (vacuum)moneta_nanoid_plagulae(signa);
    vas = (character*)piscina_allocare(c->piscina, CXXVIII);
    url = (character*)piscina_allocare(c->piscina, CXXVIII);
    sprintf(vas, "/tmp/hospitium_vas_%s.txt", signa);
    sprintf(url, "http://127.0.0.1:%u/vas",
        (insignatus integer)hospitium_portus(h));
    argumenta[ZEPHYRUM]  = "/usr/bin/curl";
    argumenta[I]         = "-s";
    argumenta[II]        = "-c";
    argumenta[III]       = vas;
    argumenta[IV]        = "-b";
    argumenta[V]         = vas;
    argumenta[VI]        = url;
    argumenta[VII]       = NIHIL;
    per (k = ZEPHYRUM; k < II; k++)
    {
               Processus* pr = processus_incipere(argumenta, X * M,
                               c->piscina);
        ProcessusResultus r;

        CREDO_NECESSE_NON_NIHIL(pr);
        dum (processus_pulsare(pr) == PROCESSUS_CURRIT)
        {
            _pumpare(h, I);
        }
        r = processus_metere(pr);
        CREDO_VERUM(r.successus && r.effusio.mensura < VIII);
        memcpy(effusa[k], r.effusio.datum, (size_t)r.effusio.mensura);
        effusa[k][r.effusio.mensura] = '\0';
    }
    CREDO_VERUM(strcmp(effusa[ZEPHYRUM], "novum") == ZEPHYRUM);
    CREDO_VERUM(strcmp(effusa[I], "vetus") == ZEPHYRUM);
    CREDO_AEQUALIS_I32(s.vocationes, II);
    CREDO_VERUM(_aequalis(s.crust_sid, "xyz"));
    (vacuum)filum_delere(vas);
    hospitium_destruere(h);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "quaestio", _sectio_quaestio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "forma", _sectio_forma, NIHIL, NIHIL, NIHIL, NIHIL },
    { "crustula", _sectio_crustula, NIHIL, NIHIL, NIHIL, NIHIL },
    { "procurator", _sectio_procurator, NIHIL, NIHIL, NIHIL, NIHIL },
    { "redirigere", _sectio_redirigere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "capita_tectum", _sectio_capita_tectum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "curl_vas", _sectio_vas_crustulorum, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("hospitium_auxilia", SECTIONES);
}
