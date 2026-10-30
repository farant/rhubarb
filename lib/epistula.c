/* epistula.c - API domus epistularum: provisores resend et fictus
 * (communio-plan-3 T2) */

#include "postulata_posix.h"
#include "epistula.h"
#include "http.h"
#include "http_vocatio.h"
#include "json.h"
#include "xar.h"

#include <string.h>
#include <stdio.h>

#define CLAVIS_MAXIMA      CCLVI
#define BASIS_MAXIMA       CCLVI
#define CAUSA_MAXIMA       CXXVIII
#define TEMPUS_MISSIONIS   ((i32)XXX * (i32)M)
#define RESEND_BASIS       "https://api.resend.com"

nomen structura Missio Missio;

/* provisor: nuntium in missione incipit; finis per _finire */
nomen vacuum (*EpistulaMittens)(Missio* m,
    constans EpistulaNuntius* nuntius);

nomen structura {
     constans character* titulus;
        EpistulaMittens  mittens;
} ProvisorEpistulae;

structura Missio {
    structura Epistularius* epistularius;
                       b32  viva;
                   Piscina* piscina;
               HttpVocatio* vocatio;
            ReactorTimerId  hora;
                       b32  hora_viva;
             EpistulaFinis  finis;
                    vacuum* ctx;
                       b32  successus;
                 character  causa[CAUSA_MAXIMA];
};

structura Epistularius {
                       Piscina* piscina;
                       Reactor* reactor;
    constans ProvisorEpistulae* provisor;
                     character  clavis[CLAVIS_MAXIMA];
                           i32  clavis_mensura;
                     character  basis[BASIS_MAXIMA];
                           Xar* fictae;        /* EpistulaNuntius */
                           b32  claudens;
                        Missio  missiones[EPISTULAE_MISSIONES];
};


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
_copia (
      chorda  c,
     Piscina* piscina)
{
    chorda r;

    r.datum    = NIHIL;
    r.mensura  = ZEPHYRUM;
    si (c.mensura == ZEPHYRUM || !c.datum)
    {
        redde r;
    }
    r.datum = (i8*)piscina_allocare(piscina, (memoriae_index)c.mensura);
    si (r.datum)
    {
        memcpy(r.datum, c.datum, (size_t)c.mensura);
        r.mensura = c.mensura;
    }
    redde r;
}

/* finis vocatur, locus liberatur (piscina missionis deletur) */
interior vacuum
_finire (
    Missio* m)
{
     EpistulaFinis  finis;
            vacuum* ctx;
               b32  successus;
         character  causa[CAUSA_MAXIMA];
            chorda  c;

    finis      = m->finis;
    ctx        = m->ctx;
    successus  = m->successus;
    memcpy(causa, m->causa, magnitudo(causa));
    si (m->piscina)
    {
        piscina_destruere(m->piscina);
        m->piscina = NIHIL;
    }
    m->vocatio  = NIHIL;
    m->viva     = FALSUM;
    c.datum     = (i8*)causa;
    c.mensura   = (i32)strlen(causa);
    si (finis)
    {
        finis(ctx, successus, c);
    }
}

interior vacuum
_hora_finis (
    vacuum* datum)
{
    Missio* m = (Missio*)datum;

    m->hora_viva = FALSUM;
    _finire(m);
}

/* exitus notatur, finis in gyro proximo (numquam intra mittere) */
interior vacuum
_complere (
                Missio* m,
                   b32  successus,
    constans character* causa)
{
    m->successus = successus;
    sprintf(m->causa, "%.120s", causa);
    m->hora = reactor_timer_adicere(m->epistularius->reactor, I, FALSUM,
        _hora_finis, m);
    m->hora_viva = m->hora != (ReactorTimerId)-I;
    si (!m->hora_viva)
    {
        _finire(m);
    }
}

chorda
epistula_resend_corpus (
    constans EpistulaNuntius* nuntius,
                     Piscina* piscina)
{
     JsonValor* o;
     JsonValor* ad;
        chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    o              = json_objectum_creare(piscina);
    ad             = json_tabulatum_creare(piscina);
    si (!nuntius || !o || !ad)
    {
        redde vacua;
    }
    json_objectum_ponere(o, "from", json_chorda_creare(piscina,
        nuntius->a));
    json_tabulatum_addere(ad, json_chorda_creare(piscina, nuntius->ad));
    json_objectum_ponere(o, "to", ad);
    json_objectum_ponere(o, "subject", json_chorda_creare(piscina,
        nuntius->titulus));
    json_objectum_ponere(o, "text", json_chorda_creare(piscina,
        nuntius->textus));
    si (nuntius->html.mensura > ZEPHYRUM)
    {
        json_objectum_ponere(o, "html", json_chorda_creare(piscina,
            nuntius->html));
    }
    redde json_scribere(o, piscina);
}


/* ==================================================
 * Provisores
 * ================================================== */

interior vacuum
_fictus_mittens (
                      Missio* m,
    constans EpistulaNuntius* n)
{
       Epistularius* e;
    EpistulaNuntius* c;

    e = m->epistularius;
    c = (EpistulaNuntius*)xar_addere(e->fictae);
    si (!c)
    {
        _complere(m, FALSUM, "fictus: memoria");
        redde;
    }
    c->a        = _copia(n->a, e->piscina);
    c->ad       = _copia(n->ad, e->piscina);
    c->titulus  = _copia(n->titulus, e->piscina);
    c->textus   = _copia(n->textus, e->piscina);
    c->html     = _copia(n->html, e->piscina);
    c->clavis_idempotentiae  = _copia(n->clavis_idempotentiae,
        e->piscina);
    _complere(m, VERUM, "");
}

interior vacuum
_hora_responsum (
    vacuum* datum)
{
          Missio* m;
     HttpVocatio* v;
    HttpResultus  res;
       character  causa[CAUSA_MAXIMA];

    m             = (Missio*)datum;
    m->hora_viva  = FALSUM;
    v             = m->vocatio;
    m->vocatio    = NIHIL;
    res           = http_vocatio_resultus(v);
    si (   res.successus && res.responsum && res.responsum->status >= CC
        && res.responsum->status < CCC)
    {
        m->successus        = VERUM;
        m->causa[ZEPHYRUM]  = '\0';
    }
    alioquin
    {
        si (res.successus && res.responsum)
        {
            sprintf(causa, "resend: HTTP %d",
                (integer)res.responsum->status);
        }
        alioquin
        {
            sprintf(causa, "resend: rete");
        }
        m->successus = FALSUM;
        memcpy(m->causa, causa, magnitudo(causa));
    }
    _finire(m);
}

interior vacuum
_vocatio_finis (
         vacuum* datum,
    HttpVocatio* vocatio)
{
    Missio* m = (Missio*)datum;

    (vacuum)vocatio;
    si (m->epistularius->claudens)
    {
        redde;
    }
    /* responsum in gyro proximo: piscina vocationis vivere debet */
    m->hora = reactor_timer_adicere(m->epistularius->reactor, I, FALSUM,
        _hora_responsum, m);
    m->hora_viva = m->hora != (ReactorTimerId)-I;
}

interior vacuum
_resend_mittens (
                  Missio* m,
    constans EpistulaNuntius* n)
{
      Epistularius* e;
       HttpPetitio* pet;
         character  url[BASIS_MAXIMA + XVI];
         character  auctoritas[CLAVIS_MAXIMA + XVI];
         character* idem;

    e = m->epistularius;
    sprintf(url, "%s/emails", e->basis[ZEPHYRUM] ? e->basis
                                                 : RESEND_BASIS);
    pet = http_petitio_creare(m->piscina, HTTP_POST, url);
    si (!pet)
    {
        _complere(m, FALSUM, "resend: petitio");
        redde;
    }
    sprintf(auctoritas, "Bearer %.*s", (integer)e->clavis_mensura,
        e->clavis);
    http_petitio_caput_addere(pet, "Authorization", auctoritas);
    memset(auctoritas, ZEPHYRUM, magnitudo(auctoritas));
    http_petitio_caput_addere(pet, "Content-Type", "application/json");
    si (n->clavis_idempotentiae.mensura > ZEPHYRUM)
    {
        idem = chorda_ut_cstr(n->clavis_idempotentiae, m->piscina);
        http_petitio_caput_addere(pet, "Idempotency-Key", idem);
    }
    http_petitio_corpus_ponere_chorda(pet, epistula_resend_corpus(n,
        m->piscina));
    http_petitio_tempus_ponere(pet, (MoraAngusta)TEMPUS_MISSIONIS);
    m->vocatio = http_vocare(e->reactor, pet, NIHIL, NIHIL,
        _vocatio_finis,
        m, NIHIL, m->piscina);
    si (!m->vocatio)
    {
        _complere(m, FALSUM, "resend: rete");
    }
}

hic_manens constans ProvisorEpistulae PROVISORES[] = {
    { "resend", _resend_mittens },
    { "fictus", _fictus_mittens },
    { NIHIL,    NIHIL }
};


/* ==================================================
 * Interfacies
 * ================================================== */

Epistularius*
epistularium_aperire (
               Piscina* piscina,
               Reactor* reactor,
    constans character* provisor,
                chorda  clavis,
    constans character* basis)
{
     Epistularius* e;
              i32  i;

    si (   !piscina || !reactor || !provisor
        || clavis.mensura >= CLAVIS_MAXIMA
        || (basis && strlen(basis) >= BASIS_MAXIMA))
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; PROVISORES[i].titulus; i++)
    {
        si (strcmp(PROVISORES[i].titulus, provisor) == ZEPHYRUM)
        {
            frange;
        }
    }
    si (!PROVISORES[i].titulus)
    {
        fprintf(stderr, "epistula: provisor ignotus '%s' - noti:",
            provisor);
        per (i = ZEPHYRUM; PROVISORES[i].titulus; i++)
        {
            fprintf(stderr, " %s", PROVISORES[i].titulus);
        }
        fprintf(stderr, "\n");
        redde NIHIL;
    }
    si (   PROVISORES[i].mittens == _resend_mittens
        && clavis.mensura        == ZEPHYRUM)
    {
        fprintf(stderr, "epistula: resend sine clave\n");
        redde NIHIL;
    }
    e = (Epistularius*)piscina_allocare(piscina,
        magnitudo(Epistularius));
    si (!e)
    {
        redde NIHIL;
    }
    memset(e, ZEPHYRUM, magnitudo(*e));
    e->piscina   = piscina;
    e->reactor   = reactor;
    e->provisor  = &PROVISORES[i];
    e->fictae    = xar_creare(piscina, (i32)magnitudo(EpistulaNuntius));
    si (clavis.mensura > ZEPHYRUM)
    {
        memcpy(e->clavis, clavis.datum, (size_t)clavis.mensura);
        e->clavis_mensura = clavis.mensura;
    }
    si (basis)
    {
        strcpy(e->basis, basis);
    }
    per (i = ZEPHYRUM; i < EPISTULAE_MISSIONES; i++)
    {
        e->missiones[i].epistularius = e;
    }
    redde e->fictae ? e : NIHIL;
}

b32
epistulam_mittere (
                Epistularius* epistularius,
    constans EpistulaNuntius* nuntius,
               EpistulaFinis  finis,
                      vacuum* ctx)
{
    Missio* m;
       i32  i;

    si (   !epistularius || !nuntius || epistularius->claudens
        || nuntius->a.mensura       == ZEPHYRUM
        || nuntius->ad.mensura      == ZEPHYRUM
        || nuntius->titulus.mensura == ZEPHYRUM
        || nuntius->textus.mensura  == ZEPHYRUM)
    {
        redde FALSUM;
    }
    m = NIHIL;
    per (i = ZEPHYRUM; i < EPISTULAE_MISSIONES && !m; i++)
    {
        si (!epistularius->missiones[i].viva)
        {
            m = &epistularius->missiones[i];
        }
    }
    si (!m)
    {
        redde FALSUM;
    }
    m->piscina = piscina_generare_dynamicum("epistula",
        (memoriae_index)XVI * (memoriae_index)MXXIV);
    si (!m->piscina)
    {
        redde FALSUM;
    }
    m->viva             = VERUM;
    m->vocatio          = NIHIL;
    m->hora_viva        = FALSUM;
    m->finis            = finis;
    m->ctx              = ctx;
    m->successus        = FALSUM;
    m->causa[ZEPHYRUM]  = '\0';
    epistularius->provisor->mittens(m, nuntius);
    redde VERUM;
}

vacuum
epistularium_claudere (
    Epistularius* epistularius)
{
    i32 i;

    si (!epistularius)
    {
        redde;
    }
    epistularius->claudens = VERUM;
    per (i = ZEPHYRUM; i < EPISTULAE_MISSIONES; i++)
    {
        Missio* m = &epistularius->missiones[i];

        si (!m->viva)
        {
            perge;
        }
        si (m->hora_viva)
        {
            (vacuum)reactor_timer_cancellare(epistularius->reactor,
                m->hora);
            m->hora_viva = FALSUM;
        }
        si (m->vocatio)
        {
            HttpVocatio* v = m->vocatio;

            m->vocatio = NIHIL;
            http_vocationem_abrumpere(v);
        }
        si (m->piscina)
        {
            piscina_destruere(m->piscina);
            m->piscina = NIHIL;
        }
        m->viva = FALSUM;
    }
    memset(epistularius->clavis, ZEPHYRUM,
        magnitudo(epistularius->clavis));
    epistularius->clavis_mensura = ZEPHYRUM;
}

i32
epistularius_fictus_numerus (
    constans Epistularius* epistularius)
{
    redde epistularius ? xar_numerus(epistularius->fictae) : ZEPHYRUM;
}

b32
epistularius_fictus_nuntius (
    constans Epistularius* epistularius,
                      i32  index,
          EpistulaNuntius* exitus)
{
    si (   !epistularius || !exitus
        || index >= xar_numerus(epistularius->fictae))
    {
        redde FALSUM;
    }
    *exitus = *(EpistulaNuntius*)xar_obtinere(epistularius->fictae,
        index);
    redde VERUM;
}
