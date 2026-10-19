/* probatio_hospitium_corpus.c - hospitium v3 T3: corpora petitionum
 * per rivum. Multipartitum magnum (X MB) in filum temporarium cum
 * SHA-256 in itinere, piscina connexionis parva; corpus magnum
 * aliud in filum (corpus_via); 413 ante corpus; fila temporaria
 * deleta post petitionem nisi retenta; cliens abiens et sistere
 * molliter fila delent; curl -F ut oraculum differentiale.
 *
 * Cliens in eodem filo ac servus: mittere per partes cum gressibus
 * servi interpositis (fossa plena aliter obstrueret). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "filum.h"
#include "iter_directoria.h"
#include "moneta.h"
#include "processus.h"
#include "sigillum.h"
#include "tcp.h"
#include "hospitium.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define MAGNUM        (X * MXXIV * MXXIV)
#define APEX_MAXIMUS  (CCLVI * MXXIV)
#define LIMES         "LimesMagnusQ7"

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

/* mittere omnia, servo inter partes pumpato */
interior b32
_mittere (
      Hospitium* h,
    TcpConnexio* c,
    constans i8* octeti,
            s64  n)
{
    s64 missa   = ZEPHYRUM;
    i32 inanes  = ZEPHYRUM;

    dum (missa < n)
    {
        s64 pars = n - missa;
        s32 k;

        si (pars > LXIV * MXXIV)
        {
            pars = LXIV * MXXIV;
        }
        k = tcp_mittere(c, octeti + missa, (i32)pars);
        si (k < ZEPHYRUM)
        {
            redde FALSUM;
        }
        si (k == ZEPHYRUM)
        {
            si (++inanes > MM)
            {
                redde FALSUM;
            }
        }
        alioquin
        {
            inanes  = ZEPHYRUM;
            missa   += (s64)k;
        }
        _pumpare(h, I);
    }
    redde VERUM;
}

interior b32
_mittere_literis (
             Hospitium* h,
           TcpConnexio* c,
    constans character* s)
{
    redde _mittere(h, c, (constans i8*)s, (s64)strlen(s));
}

/* responsum totum legere pumpando; reddit longitudinem corporis, -I
 * si fractum; capita in 'capita' */
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

/* octeti praevisibiles (LCG) */
interior i8*
_octeti (
    Piscina* p,
        s64  n,
        i32  semen)
{
     i8* d = (i8*)piscina_allocare(p, (i64)n);
    i32  x = semen;
    s64  i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        x     = x * 1103515245U + 12345U;
        d[i]  = (i8)(x >> XVI);
    }
    redde d;
}

interior constans character*
_directorium_novum (
    Piscina* p)
{
    character  n[MONETA_NANOID_PLAGULAE_MENSURA];
    character* via = (character*)piscina_allocare(p, CXXVIII);

    (vacuum)moneta_nanoid_plagulae(n);
    sprintf(via, "/tmp/hospitium_corpus_%s", n);
    (vacuum)filum_directorium_creare_cum_parentibus(via);
    redde via;
}

interior s32
_numerus_filorum (
    constans character* via,
               Piscina* p)
{
     DirectoriumIterator* it = directorium_iterator_aperire(via, p);
    DirectoriumIntroitus* in;
                     s32  n = ZEPHYRUM;

    si (it == NIHIL)
    {
        redde -I;
    }
    dum ((in = directorium_iterator_proximum(it)) != NIHIL)
    {
        si (in->genus == INTROITUS_FILUM)
        {
            n++;
        }
    }
    directorium_iterator_claudere(it);
    redde n;
}

interior b32
_continet (
    constans character* a,
    constans character* b)
{
    redde strstr(a, b) != NIHIL;
}

interior b32
_aequalis (
                chorda  a,
    constans character* b)
{
    redde a.mensura == (i32)strlen(b)
        && memcmp(a.datum, b, (size_t)a.mensura) == ZEPHYRUM;
}

interior vacuum
_hex (
    constans i8* d,
            s64  n,
      character* hex)
{
    Sigillum s = sigillum_computare(d, (memoriae_index)n);

    sigillum_hex(&s, hex);
}

/* ---- tractator: status petitionis in piscinam sectionis ---- */

nomen structura {
               Piscina* p;
                   i32  vocationes;
                   i32  fila_n;
                   i64  mensura[IV];
             character  hex[IV][SIGILLUM_HEX_MENSURA];
    constans character* via[IV];
                chorda  titulus[IV];
                   i32  campi_n;
                chorda  campus[IV];
                chorda  valor[IV];
                   b32  corpus_in_filo;
    constans character* corpus_via;
                chorda  corpus;
                   i64  apex;
    constans character* retinere_ad;
                   b32  retentum;
} Status;

interior chorda
_copia (
      chorda  s,
     Piscina* p)
{
    chorda c;

    c.mensura  = s.mensura;
    c.datum    = (i8*)piscina_allocare(p, (i64)s.mensura + I);
    si (s.mensura > ZEPHYRUM)
    {
        memcpy(c.datum, s.datum, (size_t)s.mensura);
    }
    redde c;
}

interior constans character*
_literae (
    constans character* s,
               Piscina* p)
{
    character* c = (character*)piscina_allocare(p, (i64)strlen(s) + I);

    strcpy(c, s);
    redde c;
}

interior vacuum
_recipere (
    HospitiumColloquium* col)
{
                         Status* s  = (Status*)colloquium_datum(col);
        constans HospitiumFilum* fila;
       constans HospitiumCampus* campi;
    constans HttpPetitioServeri* pet = colloquium_petitio(col);
                            i32  i;

    s->vocationes++;
    s->fila_n = colloquium_fila(col, &fila);
    per (i = ZEPHYRUM; i < s->fila_n && i < IV; i++)
    {
        s->mensura[i]  = fila[i].mensura;
        strcpy(s->hex[i], fila[i].sigillum_hex);
        s->via[i]      = _literae(fila[i].via, s->p);
        s->titulus[i]  = _copia(fila[i].titulus_fili, s->p);
    }
    s->campi_n = colloquium_campi(col, &campi);
    per (i = ZEPHYRUM; i < s->campi_n && i < IV; i++)
    {
        s->campus[i]  = _copia(campi[i].campus, s->p);
        s->valor[i]   = _copia(campi[i].valor, s->p);
    }
    s->corpus_in_filo = pet->corpus_via != NIHIL;
    si (pet->corpus_via != NIHIL)
    {
        s->corpus_via  = _literae(pet->corpus_via, s->p);
        s->corpus      = filum_legere_totum(pet->corpus_via, s->p);
    }
    alioquin
    {
        s->corpus = _copia(pet->corpus, s->p);
    }
    si (s->retinere_ad != NIHIL && s->fila_n > I)
    {
        s->retentum = colloquium_filum_retinere(col, &fila[I],
            s->retinere_ad);
    }
    s->apex = piscina_summa_apex_usus(colloquium_piscina(col));
    colloquium_respondere(col, CC, "text/plain",
        chorda_ex_literis("bene", colloquium_piscina(col)));
}

interior vacuum
_parvum (
    HospitiumColloquium* col)
{
    colloquium_respondere(col, CC, "text/plain",
        chorda_ex_literis("parvum", colloquium_piscina(col)));
}

interior Hospitium*
_servum_creare (
          CredoContextus* c,
                     i32  petitio_maxima,
                     i32  memoria,
      constans character* directorium,
                     i32  connexiones,
                  Status* s)
{
    HospitiumConfiguratio  cfg;
                Hospitium* h;

    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.petitio_maxima             = petitio_maxima;
    cfg.corpus_in_memoria_maximum  = memoria;
    cfg.directorium_temporarium    = directorium;
    cfg.connexiones_maximae        = connexiones;
    h                              = hospitium_creare(c->piscina, &cfg);
    si (h != NIHIL)
    {
        memset(s, ZEPHYRUM, magnitudo(*s));
        s->p = c->piscina;
        (vacuum)hospitium_praebere(h, HTTP_POST, "/upload", _recipere,
            s);
        (vacuum)hospitium_praebere(h, HTTP_GET, "/parvum", _parvum,
            NIHIL);
    }
    redde h;
}

/* capita petitionis multipartitae cum Content-Length */
interior constans character*
_capita (
               Piscina* p,
    constans character* typus,
                   s64  longitudo)
{
    character* c = (character*)piscina_allocare(p, CCLVI);

    sprintf(c, "POST /upload HTTP/1.1\r\nHost: x\r\n"
        "Content-Type: %s\r\nContent-Length: %ld\r\n\r\n", typus,
        (longus)longitudo);
    redde c;
}

#define TYPUS_MP "multipart/form-data; boundary=" LIMES

hic_manens constans character* CAPUT_MAGNI =
    "--" LIMES "\r\n"
    "Content-Disposition: form-data; name=\"campus\"\r\n"
    "\r\n"
    "valor\r\n"
    "--" LIMES "\r\n"
    "Content-Disposition: form-data; name=\"filum\"; "
    "filename=\"magnum.bin\"\r\n"
    "Content-Type: application/octet-stream\r\n"
    "\r\n";
hic_manens constans character* CAUDA_MAGNI = "\r\n--" LIMES "--\r\n";

/* ---- sectiones ---- */

interior vacuum
_sectio_multipart_magnum (CredoContextus* c)
{
    constans character* dir = _directorium_novum(c->piscina);
                Status  s;
             Hospitium* h = _servum_creare(c, LXIV * MXXIV * MXXIV,
                            ZEPHYRUM, dir, ZEPHYRUM, &s);
                    i8* data = _octeti(c->piscina, MAGNUM, VII);
                   s64  longitudo = (s64)strlen(CAPUT_MAGNI) + MAGNUM
                       + (s64)strlen(CAUDA_MAGNI);
             character  hex[SIGILLUM_HEX_MENSURA];
             character  capita[MXXIV];
                    i8  corpus[XVI];
           TcpConnexio* a;

    CREDO_NECESSE_NON_NIHIL(h);
    a = _cliens(c->piscina, hospitium_portus(h));
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_VERUM(_mittere_literis(h, a, _capita(c->piscina, TYPUS_MP,
        longitudo)));
    CREDO_VERUM(_mittere_literis(h, a, CAPUT_MAGNI));
    CREDO_VERUM(_mittere(h, a, data, MAGNUM));
    CREDO_VERUM(_mittere_literis(h, a, CAUDA_MAGNI));
    CREDO_AEQUALIS_S32((s32)_legere_responsum(h, a, capita, MXXIV,
        corpus, XVI, CC), (s32)IV);
    CREDO_VERUM(_continet(capita, "200 OK"));

    _hex(data, MAGNUM, hex);
    CREDO_AEQUALIS_I32(s.vocationes, I);
    CREDO_AEQUALIS_I32(s.fila_n, I);
    CREDO_VERUM(s.mensura[ZEPHYRUM] == (i64)MAGNUM);
    CREDO_VERUM(strcmp(s.hex[ZEPHYRUM], hex) == ZEPHYRUM);
    CREDO_VERUM(_aequalis(s.titulus[ZEPHYRUM], "magnum.bin"));
    CREDO_AEQUALIS_I32(s.campi_n, I);
    CREDO_VERUM(_aequalis(s.campus[ZEPHYRUM], "campus"));
    CREDO_VERUM(_aequalis(s.valor[ZEPHYRUM], "valor"));
    CREDO_FALSUM(s.corpus_in_filo);
    CREDO_AEQUALIS_I32(s.corpus.mensura, ZEPHYRUM);
    CREDO_VERUM(s.apex < APEX_MAXIMUS);
    /* filum temporarium post petitionem deletum */
    CREDO_FALSUM(filum_existit(s.via[ZEPHYRUM]));
    CREDO_AEQUALIS_S32((s32)_numerus_filorum(dir, c->piscina),
        ZEPHYRUM);

    tcp_claudere(a);
    hospitium_destruere(h);
    (vacuum)filum_arborem_delere(dir);
}

interior vacuum
_sectio_corpus_in_filo (CredoContextus* c)
{
    constans character* dir = _directorium_novum(c->piscina);
                Status  s;
             Hospitium* h = _servum_creare(c, IV * MXXIV * MXXIV,
                            MXXIV * MXXIV, dir, ZEPHYRUM, &s);
                   s64  n     = II * MXXIV * MXXIV;
                    i8* data  = _octeti(c->piscina, n, XI);
             character  capita[MXXIV];
                    i8  corpus[XVI];
           TcpConnexio* a;

    CREDO_NECESSE_NON_NIHIL(h);
    a = _cliens(c->piscina, hospitium_portus(h));
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_VERUM(_mittere_literis(h, a, _capita(c->piscina,
        "application/octet-stream", n)));
    CREDO_VERUM(_mittere(h, a, data, n));
    CREDO_AEQUALIS_S32((s32)_legere_responsum(h, a, capita, MXXIV,
        corpus, XVI, CC), (s32)IV);
    CREDO_VERUM(s.corpus_in_filo);
    CREDO_VERUM(s.corpus.mensura == (i32)n
        && memcmp(s.corpus.datum, data, (size_t)n) == ZEPHYRUM);
    CREDO_FALSUM(filum_existit(s.corpus_via));
    CREDO_AEQUALIS_S32((s32)_numerus_filorum(dir, c->piscina),
        ZEPHYRUM);

    /* corpus parvum: in memoria, eadem connexio (keep-alive) */
    CREDO_VERUM(_mittere_literis(h, a, _capita(c->piscina,
        "application/octet-stream", C)));
    CREDO_VERUM(_mittere(h, a, data, C));
    CREDO_AEQUALIS_S32((s32)_legere_responsum(h, a, capita, MXXIV,
        corpus, XVI, CC), (s32)IV);
    CREDO_AEQUALIS_I32(s.vocationes, II);
    CREDO_FALSUM(s.corpus_in_filo);
    CREDO_VERUM(s.corpus.mensura == C
        && memcmp(s.corpus.datum, data, C) == ZEPHYRUM);

    tcp_claudere(a);
    hospitium_destruere(h);
    (vacuum)filum_arborem_delere(dir);
}

interior vacuum
_sectio_nimis_magnum (CredoContextus* c)
{
    constans character* dir = _directorium_novum(c->piscina);
                Status  s;
             Hospitium* h = _servum_creare(c, MXXIV * MXXIV, ZEPHYRUM,
                            dir, ZEPHYRUM, &s);
             character  capita[MXXIV];
                    i8  corpus[CCLVI];
           TcpConnexio* a;

    CREDO_NECESSE_NON_NIHIL(h);
    a = _cliens(c->piscina, hospitium_portus(h));
    CREDO_NECESSE_NON_NIHIL(a);
    /* capita SOLA - corpus numquam mittitur */
    CREDO_VERUM(_mittere_literis(h, a, "POST /upload HTTP/1.1\r\n"
        "Host: x\r\nContent-Type: application/octet-stream\r\n"
        "Expect: 100-continue\r\n"
        "Content-Length: 2097152\r\n\r\n"));
    CREDO_VERUM(_legere_responsum(h, a, capita, MXXIV, corpus, CCLVI,
        C) >= ZEPHYRUM);
    CREDO_VERUM(_continet(capita, "HTTP/1.1 413"));
    CREDO_FALSUM(_continet(capita, "100 Continue"));
    CREDO_AEQUALIS_I32(s.vocationes, ZEPHYRUM);

    tcp_claudere(a);
    hospitium_destruere(h);
    (vacuum)filum_arborem_delere(dir);
}

interior vacuum
_sectio_temporaria_deleta (CredoContextus* c)
{
    constans character* dir = _directorium_novum(c->piscina);
                Status  s;
             Hospitium* h = _servum_creare(c, ZEPHYRUM, ZEPHYRUM, dir,
                            ZEPHYRUM, &s);
    constans character* corpus_mp =
        "--" LIMES "\r\n"
        "Content-Disposition: form-data; name=\"a\"; "
        "filename=\"a.txt\"\r\n"
        "\r\n"
        "alpha\r\n"
        "--" LIMES "\r\n"
        "Content-Disposition: form-data; name=\"b\"; "
        "filename=\"b.txt\"\r\n"
        "\r\n"
        "beta\r\n"
        "--" LIMES "--\r\n";
             character* servatum = (character*)piscina_allocare(
                            c->piscina, CCLVI);
             character  capita[MXXIV];
                    i8  corpus[XVI];
           TcpConnexio* a;

    CREDO_NECESSE_NON_NIHIL(h);
    sprintf(servatum, "%s/servatum.txt", dir);
    s.retinere_ad  = servatum;
    a              = _cliens(c->piscina, hospitium_portus(h));
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_VERUM(_mittere_literis(h, a, _capita(c->piscina, TYPUS_MP,
        (s64)strlen(corpus_mp))));
    CREDO_VERUM(_mittere_literis(h, a, corpus_mp));
    CREDO_AEQUALIS_S32((s32)_legere_responsum(h, a, capita, MXXIV,
        corpus, XVI, CC), (s32)IV);
    CREDO_AEQUALIS_I32(s.fila_n, II);
    CREDO_VERUM(s.retentum);
    CREDO_FALSUM(filum_existit(s.via[ZEPHYRUM]));
    CREDO_FALSUM(filum_existit(s.via[I]));
    CREDO_VERUM(_aequalis(filum_legere_totum(servatum, c->piscina),
        "beta"));
    CREDO_AEQUALIS_S32((s32)_numerus_filorum(dir, c->piscina), I);

    tcp_claudere(a);
    hospitium_destruere(h);
    (vacuum)filum_arborem_delere(dir);
}

/* corpus dimidium missum: filum temporarium apparet */
interior TcpConnexio*
_dimidium_mittere (
    CredoContextus* c,
         Hospitium* h,
    constans character* dir)
{
    TcpConnexio* a     = _cliens(c->piscina, hospitium_portus(h));
             i8* data  = _octeti(c->piscina, C * MXXIV, III);
            i32  k;

    si (a == NIHIL)
    {
        redde NIHIL;
    }
    (vacuum)_mittere_literis(h, a, _capita(c->piscina, TYPUS_MP,
        (s64)strlen(CAPUT_MAGNI) + MXXIV * MXXIV
        + (s64)strlen(CAUDA_MAGNI)));
    (vacuum)_mittere_literis(h, a, CAPUT_MAGNI);
    (vacuum)_mittere(h, a, data, C * MXXIV);
    per (k = ZEPHYRUM; k < C; k++)
    {
        _pumpare(h, I);
        si (_numerus_filorum(dir, c->piscina) == I)
        {
            frange;
        }
    }
    redde a;
}

/* cliens abit mediis in corporibus: filum deletum, locus liber */
interior vacuum
_sectio_abruptum (CredoContextus* c)
{
    constans character* dir = _directorium_novum(c->piscina);
                Status  s;
             Hospitium* h = _servum_creare(c, IV * MXXIV * MXXIV,
                            ZEPHYRUM, dir, I, &s);
           TcpConnexio* a;
           TcpConnexio* b;
             character  capita[MXXIV];
                    i8  corpus[XVI];

    CREDO_NECESSE_NON_NIHIL(h);
    a = _dimidium_mittere(c, h, dir);
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_AEQUALIS_S32((s32)_numerus_filorum(dir, c->piscina), I);
    tcp_claudere(a);
    _pumpare(h, X);
    CREDO_AEQUALIS_S32((s32)_numerus_filorum(dir, c->piscina),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32(s.vocationes, ZEPHYRUM);

    /* connexiones_maximae I: locus liberatus servit iterum */
    b = _cliens(c->piscina, hospitium_portus(h));
    CREDO_NECESSE_NON_NIHIL(b);
    CREDO_VERUM(_mittere_literis(h, b,
        "GET /parvum HTTP/1.1\r\nHost: x\r\n\r\n"));
    CREDO_AEQUALIS_S32((s32)_legere_responsum(h, b, capita, MXXIV,
        corpus, XVI, C), (s32)VI);
    CREDO_VERUM(memcmp(corpus, "parvum", VI) == ZEPHYRUM);

    tcp_claudere(b);
    hospitium_destruere(h);
    (vacuum)filum_arborem_delere(dir);
}

/* sistere molliter dum corpus advenit et cliens tacet: post terminum
 * filum deletum, servus sistit */
interior vacuum
_sectio_molliter_corpus (CredoContextus* c)
{
    constans character* dir = _directorium_novum(c->piscina);
                Status  s;
             Hospitium* h = _servum_creare(c, IV * MXXIV * MXXIV,
                            ZEPHYRUM, dir, ZEPHYRUM, &s);
           TcpConnexio* a;
                   s64  initium;

    CREDO_NECESSE_NON_NIHIL(h);
    a = _dimidium_mittere(c, h, dir);
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_AEQUALIS_S32((s32)_numerus_filorum(dir, c->piscina), I);
    initium = _ms_nunc();
    hospitium_sistere_molliter(h, CCC);
    dum (!hospitium_sistit(h) && _ms_nunc() - initium < MM)
    {
        _pumpare(h, I);
    }
    CREDO_VERUM(hospitium_sistit(h));
    CREDO_VERUM(_ms_nunc() - initium >= CCL);
    CREDO_AEQUALIS_S32((s32)_numerus_filorum(dir, c->piscina),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32(s.vocationes, ZEPHYRUM);

    tcp_claudere(a);
    hospitium_destruere(h);
    (vacuum)filum_arborem_delere(dir);
}

/* curl -F: oraculum differentiale (campus et SHA-256 fili) */
interior vacuum
_sectio_cliens_alienus (CredoContextus* c)
{
    constans character* dir;
    constans character* fons_dir;
                Status  s;
             Hospitium* h;
                   s64  n = VI * MXXIV * MXXIV;
                    i8* data;
             character  hex[SIGILLUM_HEX_MENSURA];
             character* via_fontis;
             character* filum_arg;
             character* url;
    constans character* argumenta[IX];
             Processus* pr;
     ProcessusResultus  r;
                   s64  initium;

    si (!filum_existit("/usr/bin/curl"))
    {
        imprimere("  NOTA: /usr/bin/curl absens - sectio omissa\n");
        redde;
    }
    dir       = _directorium_novum(c->piscina);
    fons_dir  = _directorium_novum(c->piscina);
    h         = _servum_creare(c, XVI * MXXIV * MXXIV, ZEPHYRUM, dir,
                    ZEPHYRUM, &s);
    CREDO_NECESSE_NON_NIHIL(h);
    data = _octeti(c->piscina, n, XIII);
    _hex(data, n, hex);
    via_fontis  = (character*)piscina_allocare(c->piscina, CCLVI);
    filum_arg   = (character*)piscina_allocare(c->piscina, CCLVI);
    url         = (character*)piscina_allocare(c->piscina, CCLVI);
    sprintf(via_fontis, "%s/fons.bin", fons_dir);
    sprintf(filum_arg, "filum=@%s", via_fontis);
    sprintf(url, "http://127.0.0.1:%u/upload",
        (insignatus integer)hospitium_portus(h));
    CREDO_VERUM(filum_scribere(via_fontis, chorda_ex_buffer(data,
        (i32)n)));

    argumenta[ZEPHYRUM] = "/usr/bin/curl";
    argumenta[I] = "-s";
    argumenta[II] = "-F";
    argumenta[III] = "campus=valor";
    argumenta[IV] = "-F";
    argumenta[V] = filum_arg;
    argumenta[VI] = url;
    argumenta[VII] = NIHIL;
    pr = processus_incipere(argumenta, XX * M, c->piscina);
    CREDO_NECESSE_NON_NIHIL(pr);
    initium = _ms_nunc();
    dum (   processus_pulsare(pr) == PROCESSUS_CURRIT
         && _ms_nunc() - initium < XX * M)
    {
        _pumpare(h, I);
    }
    r = processus_metere(pr);
    CREDO_VERUM(r.successus);
    CREDO_AEQUALIS_I32(r.codex_exitus, ZEPHYRUM);
    CREDO_VERUM(_aequalis(r.effusio, "bene"));
    CREDO_AEQUALIS_I32(s.fila_n, I);
    CREDO_VERUM(s.mensura[ZEPHYRUM] == (i64)n);
    CREDO_VERUM(strcmp(s.hex[ZEPHYRUM], hex) == ZEPHYRUM);
    CREDO_VERUM(_aequalis(s.titulus[ZEPHYRUM], "fons.bin"));
    CREDO_AEQUALIS_I32(s.campi_n, I);
    CREDO_VERUM(_aequalis(s.campus[ZEPHYRUM], "campus"));
    CREDO_VERUM(_aequalis(s.valor[ZEPHYRUM], "valor"));
    CREDO_AEQUALIS_S32((s32)_numerus_filorum(dir, c->piscina),
        ZEPHYRUM);

    hospitium_destruere(h);
    (vacuum)filum_arborem_delere(dir);
    (vacuum)filum_arborem_delere(fons_dir);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "multipart_magnum", _sectio_multipart_magnum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "corpus_in_filo", _sectio_corpus_in_filo, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "nimis_magnum", _sectio_nimis_magnum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "temporaria_deleta", _sectio_temporaria_deleta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "abruptum", _sectio_abruptum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "molliter_corpus", _sectio_molliter_corpus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "curl", _sectio_cliens_alienus, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("hospitium_corpus", SECTIONES);
}
