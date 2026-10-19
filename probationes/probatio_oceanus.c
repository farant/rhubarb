/* probatio_oceanus.c - cliens DNS DigitalOcean (hospitium v3 T10)
 * contra API FICTAM: hospitium in processu filio (fork), JSON ad
 * formam documentorum DO (domain_records, links.pages.next,
 * domain_record, 401 {id, message}). Paginae (II per paginam),
 * creare/mutare/delere, assecurare (IDEM sine scriptione - fictus
 * numerat), clavis (modus 0600, praefixum dop_v1_), 401 sine clave in
 * errore aut stderr, pagina proxima extra basim recusata, effusiones
 * siccae scriptorum contra fixa. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "filum.h"
#include "json.h"
#include "moneta.h"
#include "processus.h"
#include "xar.h"
#include "hospitium.h"
#include "oceanus.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define CLAVIS_FICTA \
    "dop_v1_fictum0123456789abcdef0123456789abcdef0123456789abcdef01"
#define REGISTRA_MAXIMA XXXII

/* ---- API ficta (status in filio vivit) ---- */

nomen structura {
          i64 id;
    character genus[XVI];
    character titulus[LXIV];
    character data[LXIV];
          i32 ttl;
          b32 vivum;
} Scheda;

nomen structura {
       Scheda schedae[REGISTRA_MAXIMA];
          i32 numerus;
          i64 id_proximum;
          i32 scriptiones;
    character basis[LXIV];
          b32 proxima_aliena;   /* links.pages.next ad hospitem alium */
} Fictus;

interior b32
_auctor (
    HospitiumColloquium* col)
{
    chorda a = colloquium_caput(col, "Authorization");

    redde chorda_aequalis_literis(a, "Bearer " CLAVIS_FICTA);
}

interior vacuum
_non_auctor (
    HospitiumColloquium* col)
{
    colloquium_respondere(col, CDI, "application/json",
        chorda_ex_literis(
        "{\"id\":\"unauthorized\",\"message\":\"Unable to authenticate "
        "you.\"}", colloquium_piscina(col)));
}

interior vacuum
_scheda_json (
    ChordaAedificator* a,
               Scheda* s)
{
    character n[CXXVIII];

    sprintf(n, "{\"id\":%ld,\"type\":\"", (longus)s->id);
    chorda_aedificator_appendere_literis(a, n);
    chorda_aedificator_appendere_literis(a, s->genus);
    chorda_aedificator_appendere_literis(a, "\",\"name\":\"");
    chorda_aedificator_appendere_literis(a, s->titulus);
    chorda_aedificator_appendere_literis(a, "\",\"data\":\"");
    chorda_aedificator_appendere_literis(a, s->data);
    sprintf(n, "\",\"priority\":null,\"port\":null,\"ttl\":%u,"
        "\"weight\":null,\"flags\":null,\"tag\":null}",
        (insignatus integer)s->ttl);
    chorda_aedificator_appendere_literis(a, n);
}

interior vacuum
_registra (
    HospitiumColloquium* col)
{
               Fictus* f       = (Fictus*)colloquium_datum(col);
               chorda  pg      = colloquium_quaestio(col, "page");
                  i32  pagina  = I;
                  i32  k;
                  i32  visa     = ZEPHYRUM;
                  i32  scripta  = ZEPHYRUM;
                  b32  plura    = FALSUM;
    ChordaAedificator* a;

    si (!_auctor(col))
    {
        _non_auctor(col);
        redde;
    }
    si (pg.datum && pg.mensura > ZEPHYRUM)
    {
        pagina = (i32)atoi(chorda_ut_cstr(pg, colloquium_piscina(col)));
    }
    a = chorda_aedificator_creare(colloquium_piscina(col), MXXIV);
    chorda_aedificator_appendere_literis(a, "{\"domain_records\":[");
    per (k = ZEPHYRUM; k < f->numerus; k++)
    {
        si (!f->schedae[k].vivum)
        {
            perge;
        }
        visa++;
        si (visa <= (pagina - I) * II)
        {
            perge;
        }
        si (scripta == II)
        {
            plura = VERUM;
            frange;
        }
        si (scripta > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a, ",");
        }
        _scheda_json(a, &f->schedae[k]);
        scripta++;
    }
    chorda_aedificator_appendere_literis(a, "],\"links\":{");
    si (plura)
    {
        character n[CCLVI];

        sprintf(n, "\"pages\":{\"next\":\"%s/v2/domains/golden.earth/"
            "records?page=%u&per_page=2\"}", f->proxima_aliena
            ? "http://malum.example" : f->basis,
            (insignatus integer)(pagina + I));
        chorda_aedificator_appendere_literis(a, n);
    }
    chorda_aedificator_appendere_literis(a,
        "},\"meta\":{\"total\":0}}");
    colloquium_respondere(col, CC, "application/json",
        chorda_aedificator_finire(a));
}

interior vacuum
_ex_corpore (
    HospitiumColloquium* col,
                 Scheda* s)
{
    JsonResultus r = json_legere(colloquium_petitio(col)->corpus,
                         colloquium_piscina(col));
          chorda v = chorda_ex_literis("", colloquium_piscina(col));

    si (!r.successus)
    {
        redde;
    }
    v = json_capere_chorda(r.radix, "type", v);
    sprintf(s->genus, "%.*s", (int)v.mensura,
        (constans character*)v.datum);
    v = json_capere_chorda(r.radix, "name", v);
    sprintf(s->titulus, "%.*s", (int)v.mensura,
        (constans character*)v.datum);
    v = json_capere_chorda(r.radix, "data", v);
    sprintf(s->data, "%.*s", (int)v.mensura,
        (constans character*)v.datum);
    s->ttl = (i32)json_capere_integer(r.radix, "ttl", MDCCC);
}

interior vacuum
_creare (
    HospitiumColloquium* col)
{
               Fictus* f = (Fictus*)colloquium_datum(col);
               Scheda* s;
    ChordaAedificator* a;

    si (!_auctor(col))
    {
        _non_auctor(col);
        redde;
    }
    s = &f->schedae[f->numerus++];
    memset(s, ZEPHYRUM, magnitudo(*s));
    s->id     = f->id_proximum++;
    s->vivum  = VERUM;
    _ex_corpore(col, s);
    f->scriptiones++;
    a = chorda_aedificator_creare(colloquium_piscina(col), CCLVI);
    chorda_aedificator_appendere_literis(a, "{\"domain_record\":");
    _scheda_json(a, s);
    chorda_aedificator_appendere_literis(a, "}");
    colloquium_respondere(col, CCI, "application/json",
        chorda_aedificator_finire(a));
}

interior Scheda*
_scheda (
    HospitiumColloquium* col)
{
     Fictus* f = (Fictus*)colloquium_datum(col);
     chorda  id = colloquium_param(col, "id");
        i64  n = (i64)atol(chorda_ut_cstr(id, colloquium_piscina(col)));
        i32  k;

    per (k = ZEPHYRUM; k < f->numerus; k++)
    {
        si (f->schedae[k].vivum && f->schedae[k].id == n)
        {
            redde &f->schedae[k];
        }
    }
    redde NIHIL;
}

interior vacuum
_mutare (
    HospitiumColloquium* col)
{
               Fictus* f = (Fictus*)colloquium_datum(col);
               Scheda* s;
    ChordaAedificator* a;

    si (!_auctor(col))
    {
        _non_auctor(col);
        redde;
    }
    s = _scheda(col);
    si (!s)
    {
        colloquium_respondere(col, CDIV, "application/json",
            chorda_ex_literis("{\"id\":\"not_found\",\"message\":"
            "\"The resource you were accessing could not be found.\"}",
            colloquium_piscina(col)));
        redde;
    }
    _ex_corpore(col, s);
    f->scriptiones++;
    a = chorda_aedificator_creare(colloquium_piscina(col), CCLVI);
    chorda_aedificator_appendere_literis(a, "{\"domain_record\":");
    _scheda_json(a, s);
    chorda_aedificator_appendere_literis(a, "}");
    colloquium_respondere(col, CC, "application/json",
        chorda_aedificator_finire(a));
}

interior vacuum
_delere (
    HospitiumColloquium* col)
{
    Fictus* f = (Fictus*)colloquium_datum(col);
    Scheda* s;

    si (!_auctor(col))
    {
        _non_auctor(col);
        redde;
    }
    s = _scheda(col);
    si (!s)
    {
        colloquium_respondere(col, CDIV, NIHIL, chorda_ex_literis("",
            colloquium_piscina(col)));
        redde;
    }
    s->vivum = FALSUM;
    f->scriptiones++;
    colloquium_respondere(col, CCIV, NIHIL, chorda_ex_literis("",
        colloquium_piscina(col)));
}

interior vacuum
_contator (
    HospitiumColloquium* col)
{
       Fictus* f = (Fictus*)colloquium_datum(col);
    character  n[LXIV];

    sprintf(n, "%u", (insignatus integer)f->scriptiones);
    colloquium_respondere(col, CC, "text/plain", chorda_ex_literis(n,
        colloquium_piscina(col)));
}

nomen structura {
          s32 pid;
    character basis[LXIV];
} Servus;

interior vacuum
_seminare (
                Fictus* f,
    constans character* genus,
    constans character* titulus,
    constans character* data)
{
    Scheda* s = &f->schedae[f->numerus++];

    memset(s, ZEPHYRUM, magnitudo(*s));
    s->id     = f->id_proximum++;
    s->ttl    = MDCCC;
    s->vivum  = VERUM;
    strcpy(s->genus, genus);
    strcpy(s->titulus, titulus);
    strcpy(s->data, data);
}

/* hospitium fictum in filio; f a vocante seminatus */
interior b32
_fictum_incipere (
    CredoContextus* c,
            Fictus* f,
            Servus* s)
{
    HospitiumConfiguratio  cfg;
                Hospitium* h;
                    pid_t  pid;

    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    h = hospitium_creare(c->piscina, &cfg);
    si (!h)
    {
        redde FALSUM;
    }
    sprintf(f->basis, "http://127.0.0.1:%u",
        (insignatus integer)hospitium_portus(h));
    strcpy(s->basis, f->basis);
    (vacuum)hospitium_praebere(h, HTTP_GET,
        "/v2/domains/:dominium/records", _registra, f);
    (vacuum)hospitium_praebere(h, HTTP_POST,
        "/v2/domains/:dominium/records", _creare, f);
    (vacuum)hospitium_praebere(h, HTTP_PUT,
        "/v2/domains/:dominium/records/:id", _mutare, f);
    (vacuum)hospitium_praebere(h, HTTP_DELETE,
        "/v2/domains/:dominium/records/:id", _delere, f);
    (vacuum)hospitium_praebere(h, HTTP_GET, "/_contator", _contator, f);
    pid = fork();
    si (pid == ZEPHYRUM)
    {
        hospitium_currere(h);
        _exit(ZEPHYRUM);
    }
    hospitium_destruere(h);   /* exemplar patris; filius suum tenet */
    s->pid = (s32)pid;
    redde pid > ZEPHYRUM;
}

interior vacuum
_fictum_sistere (
    Servus* s)
{
    integer status;

    kill((pid_t)s->pid, SIGKILL);
    (vacuum)waitpid((pid_t)s->pid, &status, ZEPHYRUM);
}

interior constans character*
_clavis_scribere (
               Piscina* p,
    constans character* contentum,
               integer  modus)
{
     character  n[MONETA_NANOID_PLAGULAE_MENSURA];
     character* via = (character*)piscina_allocare(p, CXXVIII);

    (vacuum)moneta_nanoid_plagulae(n);
    sprintf(via, "/tmp/oceanus_clavis_%s", n);
    (vacuum)filum_scribere_literis(via, contentum);
    (vacuum)filum_modum_ponere(via, modus);
    redde via;
}

interior i32
_scripta_numerare (
     Servus* s,
    Piscina* p)
{
       character  url[CXXVIII];
     HttpPetitio* q;
    HttpResultus  r;

    sprintf(url, "%s/_contator", s->basis);
    q = http_petitio_creare(p, HTTP_GET, url);
    r = http_exsequi(q, p);
    si (!r.successus)
    {
        redde ~(i32)ZEPHYRUM;
    }
    redde (i32)atoi(chorda_ut_cstr(r.responsum->corpus, p));
}

/* ---- sectiones ---- */

interior vacuum
_sectio_clavis (CredoContextus* c)
{
    constans character* bona = _clavis_scribere(c->piscina,
                               CLAVIS_FICTA "\n", 0600);
    constans character* lata = _clavis_scribere(c->piscina,
                               CLAVIS_FICTA "\n", 0644);
    constans character* sine = _clavis_scribere(c->piscina,
                               "fictum0123456789\n", 0600);
             Oceanus* o;

    o = oceanus_creare(bona, "http://127.0.0.1:1", c->piscina);
    CREDO_NECESSE_NON_NIHIL(o);
    oceanus_claudere(o);
    CREDO_NIHIL(oceanus_creare(lata, "http://127.0.0.1:1", c->piscina));
    CREDO_NIHIL(oceanus_creare(sine, "http://127.0.0.1:1", c->piscina));
    CREDO_NIHIL(oceanus_creare("/tmp/oceanus_clavis_absens",
        "http://127.0.0.1:1", c->piscina));
    (vacuum)filum_delere(bona);
    (vacuum)filum_delere(lata);
    (vacuum)filum_delere(sine);
}

interior vacuum
_sectio_registra (CredoContextus* c)
{
                Fictus  f;
                Servus  s;
               Oceanus* o;
                   Xar* x;
    constans character* clavis = _clavis_scribere(c->piscina,
                               CLAVIS_FICTA, 0600);
    OceanusRegistrum* r;

    memset(&f, ZEPHYRUM, magnitudo(f));
    f.id_proximum = 3352890;
    _seminare(&f, "A", "@", "1.2.3.4");
    _seminare(&f, "A", "www", "1.2.3.5");
    _seminare(&f, "CNAME", "blog", "golden.earth.");
    _seminare(&f, "MX", "@", "mail.golden.earth.");
    _seminare(&f, "A", "smaragda", "165.227.210.190");
    CREDO_NECESSE(_fictum_incipere(c, &f, &s));
    o = oceanus_creare(clavis, s.basis, c->piscina);
    CREDO_NECESSE_NON_NIHIL(o);
    x = xar_creare(c->piscina, (i32)magnitudo(OceanusRegistrum));
    CREDO_VERUM(oceanus_registra(o, "golden.earth", x));
    CREDO_AEQUALIS_I32(xar_numerus(x), V);
    r = (OceanusRegistrum*)xar_obtinere(x, IV);
    CREDO_VERUM(r->id == 3352894);
    CREDO_VERUM(chorda_aequalis_literis(r->titulus, "smaragda"));
    CREDO_VERUM(chorda_aequalis_literis(r->data, "165.227.210.190"));
    CREDO_AEQUALIS_I32(r->ttl, MDCCC);
    oceanus_claudere(o);
    _fictum_sistere(&s);
    (vacuum)filum_delere(clavis);
}

interior vacuum
_sectio_cyclus (CredoContextus* c)
{
                Fictus  f;
                Servus  s;
               Oceanus* o;
      OceanusRegistrum  r;
                   i64  id = ZEPHYRUM;
                   Xar* x;
    constans character* clavis = _clavis_scribere(c->piscina,
                               CLAVIS_FICTA, 0600);

    memset(&f, ZEPHYRUM, magnitudo(f));
    f.id_proximum = C;
    CREDO_NECESSE(_fictum_incipere(c, &f, &s));
    o = oceanus_creare(clavis, s.basis, c->piscina);
    CREDO_NECESSE_NON_NIHIL(o);
    memset(&r, ZEPHYRUM, magnitudo(r));
    r.genus    = chorda_ex_literis("A", c->piscina);
    r.titulus  = chorda_ex_literis("probatio", c->piscina);
    r.data     = chorda_ex_literis("10.0.0.1", c->piscina);
    r.ttl      = MMMDC;
    CREDO_VERUM(oceanus_registrum_creare(o, "golden.earth", &r, &id));
    CREDO_VERUM(id == C);
    r.data = chorda_ex_literis("10.0.0.2", c->piscina);
    CREDO_VERUM(oceanus_registrum_mutare(o, "golden.earth", id, &r));
    x = xar_creare(c->piscina, (i32)magnitudo(OceanusRegistrum));
    CREDO_VERUM(oceanus_registra(o, "golden.earth", x));
    CREDO_AEQUALIS_I32(xar_numerus(x), I);
    r = *(OceanusRegistrum*)xar_obtinere(x, ZEPHYRUM);
    CREDO_VERUM(chorda_aequalis_literis(r.data, "10.0.0.2"));
    CREDO_VERUM(oceanus_registrum_delere(o, "golden.earth", id));
    x = xar_creare(c->piscina, (i32)magnitudo(OceanusRegistrum));
    CREDO_VERUM(oceanus_registra(o, "golden.earth", x));
    CREDO_AEQUALIS_I32(xar_numerus(x), ZEPHYRUM);
    CREDO_FALSUM(oceanus_registrum_delere(o, "golden.earth", id));
    CREDO_AEQUALIS_I32(_scripta_numerare(&s, c->piscina), III);
    oceanus_claudere(o);
    _fictum_sistere(&s);
    (vacuum)filum_delere(clavis);
}

interior vacuum
_sectio_assecurare (CredoContextus* c)
{
                Fictus  f;
                Servus  s;
               Oceanus* o;
    constans character* clavis = _clavis_scribere(c->piscina,
                               CLAVIS_FICTA, 0600);

    memset(&f, ZEPHYRUM, magnitudo(f));
    f.id_proximum = M;
    _seminare(&f, "A", "www", "1.2.3.5");
    _seminare(&f, "A", "duplex_a", "1.1.1.1");
    _seminare(&f, "A", "duplex_a", "2.2.2.2");
    CREDO_NECESSE(_fictum_incipere(c, &f, &s));
    o = oceanus_creare(clavis, s.basis, c->piscina);
    CREDO_NECESSE_NON_NIHIL(o);
    CREDO_AEQUALIS_I32((i32)oceanus_assecurare(o, "golden.earth", "A",
        "exemplar", "165.227.210.190", MMMDC), (i32)OCEANUS_CREATUM);
    CREDO_AEQUALIS_I32(_scripta_numerare(&s, c->piscina), I);
    CREDO_AEQUALIS_I32((i32)oceanus_assecurare(o, "golden.earth", "A",
        "exemplar", "165.227.210.190", MMMDC), (i32)OCEANUS_IDEM);
    /* IDEM: nulla scriptio */
    CREDO_AEQUALIS_I32(_scripta_numerare(&s, c->piscina), I);
    CREDO_AEQUALIS_I32((i32)oceanus_assecurare(o, "golden.earth", "A",
        "exemplar", "165.227.210.191", MMMDC), (i32)OCEANUS_MUTATUM);
    CREDO_AEQUALIS_I32(_scripta_numerare(&s, c->piscina), II);
    CREDO_AEQUALIS_I32((i32)oceanus_assecurare(o, "golden.earth", "A",
        "duplex_a", "3.3.3.3", MMMDC), (i32)OCEANUS_ERROR);
    CREDO_VERUM(strstr(oceanus_error(o), "plura") != NIHIL);
    CREDO_AEQUALIS_I32(_scripta_numerare(&s, c->piscina), II);
    oceanus_claudere(o);
    _fictum_sistere(&s);
    (vacuum)filum_delere(clavis);
}

/* 401: error nominat, clavis NUSQUAM (error, stderr) */
interior vacuum
_sectio_auctoritas (CredoContextus* c)
{
                Fictus  f;
                Servus  s;
               Oceanus* o;
                   Xar* x;
    constans character* falsa =
        "dop_v1_falsaclavis9999999999999999999999999999999999999999999";
    constans character* clavis = _clavis_scribere(c->piscina, falsa,
        0600);
               integer fistula[II];
               integer servatum;
             character acta[MMXLVIII];
               ssize_t n;

    memset(&f, ZEPHYRUM, magnitudo(f));
    CREDO_NECESSE(_fictum_incipere(c, &f, &s));
    fflush(stderr);
    servatum = dup(II);
    CREDO_VERUM(pipe(fistula) == ZEPHYRUM);
    dup2(fistula[I], II);

    o = oceanus_creare(clavis, s.basis, c->piscina);
    CREDO_NECESSE_NON_NIHIL(o);
    x = xar_creare(c->piscina, (i32)magnitudo(OceanusRegistrum));
    CREDO_FALSUM(oceanus_registra(o, "golden.earth", x));
    CREDO_AEQUALIS_I32((i32)oceanus_assecurare(o, "golden.earth", "A",
        "exemplar", "1.2.3.4", MMMDC), (i32)OCEANUS_ERROR);
    fprintf(stderr, "error: %s\n", oceanus_error(o));

    fflush(stderr);
    dup2(servatum, II);
    close(servatum);
    close(fistula[I]);
    n = read(fistula[ZEPHYRUM], acta, magnitudo(acta) - I);
    close(fistula[ZEPHYRUM]);
    acta[n > ZEPHYRUM ? n : ZEPHYRUM] = '\0';
    CREDO_VERUM(strstr(oceanus_error(o), "401") != NIHIL);
    CREDO_VERUM(strstr(oceanus_error(o), "authenticate") != NIHIL);
    CREDO_VERUM(strstr(oceanus_error(o), "falsaclavis") == NIHIL);
    CREDO_VERUM(strstr(acta, "falsaclavis") == NIHIL);
    CREDO_VERUM(strstr(acta, "401") != NIHIL);
    oceanus_claudere(o);
    _fictum_sistere(&s);
    (vacuum)filum_delere(clavis);
}

/* links.pages.next ad hospitem alium: non sequitur (clavis ibi
 * mitteretur) */
interior vacuum
_sectio_pagina_aliena (CredoContextus* c)
{
                Fictus  f;
                Servus  s;
               Oceanus* o;
                   Xar* x;
    constans character* clavis = _clavis_scribere(c->piscina,
                               CLAVIS_FICTA, 0600);

    memset(&f, ZEPHYRUM, magnitudo(f));
    f.proxima_aliena = VERUM;
    _seminare(&f, "A", "a", "1.1.1.1");
    _seminare(&f, "A", "b", "1.1.1.2");
    _seminare(&f, "A", "c", "1.1.1.3");
    CREDO_NECESSE(_fictum_incipere(c, &f, &s));
    o = oceanus_creare(clavis, s.basis, c->piscina);
    CREDO_NECESSE_NON_NIHIL(o);
    x = xar_creare(c->piscina, (i32)magnitudo(OceanusRegistrum));
    CREDO_FALSUM(oceanus_registra(o, "golden.earth", x));
    CREDO_VERUM(strstr(oceanus_error(o), "extra basim") != NIHIL);
    oceanus_claudere(o);
    _fictum_sistere(&s);
    (vacuum)filum_delere(clavis);
}

interior b32
_siccum_congruit (
               Piscina* p,
    constans character* constans* argumenta,
    constans character* fixum)
{
    ProcessusResultus r    = processus_exsequi(argumenta, XXX * M, p);
               chorda exp  = filum_legere_totum(fixum, p);

    redde r.successus && r.codex_exitus == ZEPHYRUM
        && chorda_aequalis(r.effusio, exp);
}

interior vacuum
_sectio_scripta_sicca (CredoContextus* c)
{
    constans character* dim[VI];
    constans character* prae[IX];

    dim[ZEPHYRUM]  = "/bin/bash";
    dim[I]         = "tools/dimittere.sh";
    dim[II]        = "-siccum";
    dim[III]       = "exemplar";
    dim[IV]        = "smaragda";
    dim[V]         = NIHIL;
    CREDO_VERUM(_siccum_congruit(c->piscina, dim,
        "probationes/fixa/oceanus/dimittere_siccum.txt"));
    prae[ZEPHYRUM]  = "/bin/bash";
    prae[I]         = "tools/praeparare_hospitem.sh";
    prae[II]        = "-siccum";
    prae[III]       = "exemplar";
    prae[IV]        = "smaragda";
    prae[V]         = "exemplar.golden.earth";
    prae[VI]        = "165.227.210.190";
    prae[VII]       = NIHIL;
    CREDO_VERUM(_siccum_congruit(c->piscina, prae,
        "probationes/fixa/oceanus/praeparare_siccum.txt"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "clavis", _sectio_clavis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "registra", _sectio_registra, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cyclus", _sectio_cyclus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "assecurare", _sectio_assecurare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "auctoritas", _sectio_auctoritas, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pagina_aliena", _sectio_pagina_aliena, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "scripta_sicca", _sectio_scripta_sicca, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("oceanus", SECTIONES);
}
