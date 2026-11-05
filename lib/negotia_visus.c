/* negotia_visus.c - negotia in latere vici (include/negotia_visus.h;
 * negotia-plan T3). Forma centri admonitionum: arbor dispositionis in
 * cellulis (LITTERA x LINEA), figura radicis omnia pingit, praecepta
 * per membrum__praecepta, paginae per membrum__album; semitae ut
 * sancti_visus (argumentum -> semita -> ramus). */

#include "negotia_visus.h"
#include "membrum__album.h"
#include "membrum__praecepta.h"
#include "dispositio.h"
#include "fons.h"
#include "thema.h"
#include "motus.h"
#include "semita.h"
#include "paginarium.h"
#include "chorda_aedificator.h"
#include "fasti.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#define NEGOTIA_LATITUDO     CCLXXX   /* superficies absens */
#define NEGOTIA_ALTITUDO     CCLVI
#define NEGOTIA_LITTERA      VI
#define NEGOTIA_LINEA        VIII
#define NEGOTIA_TITULI       III      /* lineae tituli */
#define NEGOTIA_CAPUT        II       /* lineae capitis */
#define NEGOTIA_ORDO         III      /* indicis: titulus, summarium */
#define NEGOTIA_ORDO_FAC     II       /* facultatis: status, titulus */
#define NEGOTIA_GLOBULUS     L        /* latitudo globuli status (VII
                                       * litterae + margines) */

#define VISUS_INDICIS        I
#define VISUS_NEGOTII        II
#define VISUS_FACULTATIS     IV
#define NEGOTIA_SPATIUM      X        /* pixela per lineam textus */
#define REQUIRIT_ELECTAM     I

hic_manens constans character actio_negotiorum[] = "negotia.agere";

/* figura: contextus privatus; organum manifesti (semitae) */
nomen structura {
      NegotiaMontatio* m;
     constans Organum* organum;
              Piscina* scheda;
                  s32  latitudo_montationis;
                  s32  altitudo_montationis;
} NegotiaFigurae;


/* ==================================================
 * Status (insula)
 * ================================================== */

interior chorda
campus (
    constans NegotiaMontatio* m,
          constans character* titulus)
{
    chorda* a;
    chorda  vacua;

    a = insula_ramus_attributum(&m->ramus, titulus);
    si (a)
    {
        redde *a;
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

interior s32
campus_s32 (
    constans NegotiaMontatio* m,
          constans character* titulus,
                         s32  praestitutum)
{
    chorda a;
       s32 v;

    a = campus(m, titulus);
    redde (a.mensura > ZEPHYRUM && chorda_ut_s32(a, &v)
        && v >= ZEPHYRUM) ? v : praestitutum;
}

nomen structura {
    constans character* constans* tituli;
    constans character* constans* valores;
                             i32 numerus;
} GrexCamporum;

interior vacuum
campos_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    GrexCamporum* grex;
             i32  i;

    grex = (GrexCamporum*)ctx;
    per (i = ZEPHYRUM; i < grex->numerus; i++)
    {
        si (!grex->valores[i] || !grex->valores[i][ZEPHYRUM])
        {
            (vacuum)insula_attributum_tollere(radix, grex->tituli[i]);
        }
        alioquin
        {
            insula_attributum_ponere(radix, p, in, grex->tituli[i],
                grex->valores[i]);
        }
    }
}

/* campi actu uno, scriptore lateris ("" aut NIHIL tollit) */
interior b32
campos_scribere (
                NegotiaMontatio* m,
    constans character* constans* tituli,
    constans character* constans* valores,
                             i32  numerus)
{
    GrexCamporum grex;
          chorda prior;
             b32 ok;

    grex.tituli   = tituli;
    grex.valores  = valores;
    grex.numerus  = numerus;
    prior         = m->ramus.repo->scriptor;
    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        actio_negotiorum, m->piscina));
    ok = mutare_ramum(&m->ramus, campos_ponere, &grex);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

interior b32
campum_scribere (
       NegotiaMontatio* m,
    constans character* titulus,
    constans character* valor)
{
    constans character* t[I];
    constans character* v[I];

    t[ZEPHYRUM] = titulus;
    v[ZEPHYRUM] = valor;
    redde campos_scribere(m, t, v, I);
}

/* superficies: attributum rami (vicus), aliter montationis, aliter
 * ordinaria */
interior s32
superficies (
    constans NegotiaMontatio* m,
                         b32  latitudo)
{
    constans NegotiaFigurae* f;
                        s32  v;

    f = (constans NegotiaFigurae*)m->figurae_ctx;
    v = !f ? ZEPHYRUM : latitudo ? f->latitudo_montationis
                                 : f->altitudo_montationis;
    si (v <= ZEPHYRUM)
    {
        v = latitudo ? NEGOTIA_LATITUDO : NEGOTIA_ALTITUDO;
    }
    redde campus_s32(m, latitudo ? "superficies_latitudo"
        : "superficies_altitudo", v);
}

/* negotium apertum (visus negotium, id exactum); NIHIL in indice */
interior constans Negotium*
apertum (
    constans NegotiaMontatio* m)
{
    constans Negotium* x;
               chorda  id;

    si (   !chorda_aequalis_literis(campus(m, "visus"), "negotium")
        && !chorda_aequalis_literis(campus(m, "visus"), "facultas"))
    {
        redde NIHIL;
    }
    id = campus(m, "negotium");
    x  = id.mensura > ZEPHYRUM ? negotia_negotium_invenire(m->negotia,
        id,
        NIHIL) : NIHIL;
    redde (x && chorda_aequalis(x->id, id)) ? x : NIHIL;
}

/* facultas aperta (visus facultas, id exactum, negotii x); NIHIL
 * aliter - visus tunc ut negotium */
interior constans NegotiiFacultas*
facultas_aperta (
    constans NegotiaMontatio* m,
           constans Negotium* x)
{
    constans NegotiiFacultas* f;
                      chorda  id;

    si (!x || !chorda_aequalis_literis(campus(m, "visus"), "facultas"))
    {
        redde NIHIL;
    }
    id = campus(m, "facultas");
    f  = id.mensura > ZEPHYRUM ? negotia_facultatem_invenire(m->negotia,
        id, NIHIL) : NIHIL;
    redde (   f && chorda_aequalis(f->id, id)
           && chorda_aequalis(f->negotium, x->id)) ? f : NIHIL;
}


/* ==================================================
 * Ordines (album): index = negotia ordine activitatis; negotium =
 * facultates ordine creationis. Id ordinis "r<k>" (k = index + 1).
 * ================================================== */

nomen structura {
           constans Negotium* negotium;   /* NIHIL = index */
                         Xar* facultates; /* NegotiiFacultas (copiae) */
                       Album* l;
    constans NegotiiFacultas* facultas; /* visus facultas; aliter
                                         * NIHIL */
} Ordines;

interior i32
numerus_rerum (
    constans NegotiaMontatio* m,
            constans Ordines* o)
{
    si (o->negotium)
    {
        redde o->facultates ? xar_numerus(o->facultates) : ZEPHYRUM;
    }
    redde negotia_numerus(m->negotia);
}

/* id rei k (negotii aut facultatis); vacua extra ambitum */
interior chorda
id_rei (
    constans NegotiaMontatio* m,
            constans Ordines* o,
                         s32  k)
{
    chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (k < ZEPHYRUM || (i32)k >= numerus_rerum(m, o))
    {
        redde vacua;
    }
    si (o->negotium)
    {
        redde ((constans NegotiiFacultas*)xar_obtinere(o->facultates,
            (i32)k))->id;
    }
    redde negotia_index(m->negotia, (i32)k)->id;
}

/* capacitas in cellulis: altitudo minus margines (II), titulus,
 * caput */
interior s32
capacitas (
    constans NegotiaMontatio* m)
{
    redde superficies(m, FALSUM) / NEGOTIA_LINEA - II - NEGOTIA_TITULI
        - NEGOTIA_CAPUT;
}

interior vacuum
ordines_parare (
    constans NegotiaMontatio* m,
                     Piscina* p,
                     Ordines* o)
{
          i32 n;
          i32 i;
    character id[XVI];

    o->negotium    = apertum(m);
    o->facultates  = o->negotium ? negotia_facultates(m->negotia,
        o->negotium->id, p) : NIHIL;
    o->facultas  = facultas_aperta(m, o->negotium);
    o->l         = album_creare(p, capacitas(m));
    n            = numerus_rerum(m, o);
    per (i = ZEPHYRUM; o->l && i < n; i++)
    {
        sprintf(id, "r%u", (insignatus integer)(i + I));
        (vacuum)album_addere(o->l, id, ALBUM_RES, o->negotium
            ? NEGOTIA_ORDO_FAC : NEGOTIA_ORDO, (s32)(i + I));
    }
}

interior s32
pagina_currens (
    constans NegotiaMontatio* m,
                       Album* l)
{
    redde album_pagina_coacta(l, campus_s32(m, "pagina", ZEPHYRUM));
}

/* index electae: campus electa si in ordinibus, aliter prima paginae;
 * -1 si nulla */
interior s32
electa (
    constans NegotiaMontatio* m,
            constans Ordines* o)
{
    chorda a;
       i32 n;
       i32 i;

    a = campus(m, "electa");
    n = numerus_rerum(m, o);
    per (i = ZEPHYRUM; a.mensura > ZEPHYRUM && i < n; i++)
    {
        si (chorda_aequalis(id_rei(m, o, (s32)i), a))
        {
            redde (s32)i;
        }
    }
    redde o->l && n > ZEPHYRUM
        ? album_res_prima(o->l, pagina_currens(m, o->l)) - I : -I;
}

/* id ordinis "r<n>" -> n (index + 1); 0 si non ordo */
interior s32
id_ordinis (
    chorda id)
{
    s32 n;

    si (id.mensura < II || id.datum[ZEPHYRUM] != 'r')
    {
        redde ZEPHYRUM;
    }
    id.datum++;
    id.mensura--;
    redde chorda_ut_s32(id, &n) && n > ZEPHYRUM ? n : ZEPHYRUM;
}

/* textus ad n characteres (UTF-8: octeti continuationis non
 * numerantur, numquam character dimidius) */
interior chorda
runas_praecidere (
    chorda textus,
       i32 n)
{
    i32 i;
    i32 runae = ZEPHYRUM;

    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si ((textus.datum[i] & 0xC0) != 0x80)
        {
            si (runae == n)
            {
                textus.mensura = i;
                frange;
            }
            runae++;
        }
    }
    redde textus;
}


/* ==================================================
 * Textus facultatis: corpus involutum, deinde "History" et revisiones
 * (recentissima prima) - series una linearum per paginas secta
 * ================================================== */

#define LINEA_CORPUS     ZEPHYRUM
#define LINEA_VACUA      I
#define LINEA_TITULUS    II
#define LINEA_HISTORIA   III

nomen structura {
    chorda textus;
       i32 genus;   /* LINEA_* */
} LineaFacultatis;

/* characteres per lineam: superficies minus margines mensae et XVI */
interior i32
columnae_textus (
    constans NegotiaMontatio* m)
{
    s32 c;

    c = (superficies(m, VERUM) - II * NEGOTIA_LITTERA - XVI)
        / NEGOTIA_LITTERA;
    redde c > X ? (i32)c : X;
}

/* lineae per paginam (regio ordinum, NEGOTIA_SPATIUM pixela) */
interior i32
lineae_paginae (
    constans NegotiaMontatio* m)
{
    s32 n;

    n = capacitas(m) * NEGOTIA_LINEA / NEGOTIA_SPATIUM;
    redde n > ZEPHYRUM ? (i32)n : I;
}

interior vacuum
lineam_addere (
        Xar* lineae,
     chorda  textus,
        i32  genus)
{
    LineaFacultatis* l = (LineaFacultatis*)xar_addere(lineae);

    si (l)
    {
        l->textus  = textus;
        l->genus   = genus;
    }
}

/* "2026-11-04 23:38 · shipped · <linea prima>" (ad columnas) */
/* momentum actus (ISO UTC, cum fractione) -> "2026-11-04 18:38" in
 * hora LOCALI; non parsatum: "YYYY-MM-DD HH:MM" ex UTC (vel textus
 * crudus si brevis) */
interior chorda
momentum_locale (
     chorda  momentum,
    Piscina* p)
{
       DiesHora dh;
         time_t t;
    structura tm* tm_info;
      character textus[XXXII];
         chorda iso;

    si (momentum.mensura < XIX)
    {
        redde momentum;
    }
    /* fasti_ex_iso fractiones recusat: XIX characteres + 'Z' */
    sprintf(textus, "%.19sZ", (constans character*)momentum.datum);
    iso = chorda_ex_literis(textus, p);
    si (fasti_ex_iso(iso, &dh))
    {
        t        = (time_t)fasti_ad_unix(dh);
        tm_info  = localtime(&t);
        si (tm_info)
        {
            sprintf(textus, "%04d-%02d-%02d %02d:%02d",
                (integer)(tm_info->tm_year + MCM),
                (integer)(tm_info->tm_mon + I),
                (integer)tm_info->tm_mday,
                (integer)tm_info->tm_hour, (integer)tm_info->tm_min);
            redde chorda_ex_literis(textus, p);
        }
    }
    sprintf(textus, "%.10s %.5s", (constans character*)momentum.datum,
        (constans character*)momentum.datum + XI);
    redde chorda_ex_literis(textus, p);
}

/* "2026-11-04 18:38 · shipped · <linea prima>" (ad columnas) */
interior chorda
linea_revisionis (
    constans NegotiiFacultas* r,
                         i32  columnae,
                     Piscina* p)
{
    ChordaAedificator* a;
               chorda  prima;
                  i32  i;

    a = chorda_aedificator_creare(p, LXIV);
    (vacuum)chorda_aedificator_appendere_chorda(a, momentum_locale(
        r->momentum, p));
    (vacuum)chorda_aedificator_appendere_literis(a, " \302\267 ");
    (vacuum)chorda_aedificator_appendere_literis(a,
        negotia_status_titulus(r->status));
    prima = r->corpus;
    per (i = ZEPHYRUM; i < prima.mensura && prima.datum[i] != '\n'; i++)
    {
    }
    prima.mensura = i;
    si (prima.mensura > ZEPHYRUM)
    {
        (vacuum)chorda_aedificator_appendere_literis(a, " \302\267 ");
        (vacuum)chorda_aedificator_appendere_chorda(a, prima);
    }
    redde runas_praecidere(chorda_aedificator_spectare(a), columnae);
}

/* Xar de LineaFacultatis */
interior Xar*
lineae_facultatis (
       constans NegotiaMontatio* m,
       constans NegotiiFacultas* f,
                        Piscina* p)
{
                   Xar* lineae;
                   Xar* revisiones;
      PaginariumConfig  config;
    PaginariumResultus  res;
                   i32  columnae;
                   i32  k;
                   i32  j;
                   s32  r;

    lineae    = xar_creare(p, (i32)magnitudo(LineaFacultatis));
    columnae  = columnae_textus(m);
    si (!lineae)
    {
        redde NIHIL;
    }
    si (f->corpus.mensura > ZEPHYRUM)
    {
        config           = paginarium_config_defectus();
        config.latitudo  = columnae;
        config.altitudo  = M * M;   /* pagina una: lineae solae */
        res              = paginarium_paginare(f->corpus, config, p);
        per (k = ZEPHYRUM; k < res.numerus_paginarum; k++)
        {
            PaginariumPagina* pg = paginarium_pagina_obtinere(&res, k);

            per (j = ZEPHYRUM; pg
                 && j < paginarium_pagina_numerus_linearum(pg); j++)
            {
                PaginariumLinea* ln = paginarium_linea_obtinere(pg, j);

                lineam_addere(lineae, ln->est_vacua ? chorda_ex_literis(
                    "", p) : paginarium_linea_reddere(f->corpus, ln, p),
                    ln->est_vacua ? LINEA_VACUA : LINEA_CORPUS);
            }
        }
        lineam_addere(lineae, chorda_ex_literis("", p), LINEA_VACUA);
    }
    lineam_addere(lineae, chorda_ex_literis("History", p),
        LINEA_TITULUS);
    revisiones = negotia_revisiones(m->negotia, f->id, p);
    per (r = revisiones ? (s32)xar_numerus(revisiones) - I : -I;
         r >= ZEPHYRUM; r--)
    {
        lineam_addere(lineae,
            linea_revisionis((constans NegotiiFacultas*)
            xar_obtinere(revisiones, (i32)r), columnae, p),
            LINEA_HISTORIA);
    }
    redde lineae;
}

interior s32
paginae_textus (
    constans NegotiaMontatio* m,
                constans Xar* lineae)
{
    i32 n;
    i32 per_paginam;

    n            = lineae ? xar_numerus(lineae) : ZEPHYRUM;
    per_paginam  = lineae_paginae(m);
    redde n == ZEPHYRUM ? I : (s32)((n + per_paginam - I)
        / per_paginam);
}

interior s32
pagina_textus_currens (
    constans NegotiaMontatio* m,
                         s32  paginae)
{
    s32 k;

    k = campus_s32(m, "pagina_textus", ZEPHYRUM);
    redde k >= paginae ? paginae - I : k;
}


/* ==================================================
 * Praecepta
 * ================================================== */

nomen structura {
    NegotiaMontatio* m;
            Piscina* p;
            Ordines  o;
                s32  electa;
} ContextusPraecepti;

interior vacuum
contextum_parare (
       NegotiaMontatio* m,
               Piscina* p,
    ContextusPraecepti* cp)
{
    cp->m = m;
    cp->p = p;
    ordines_parare(m, p, &cp->o);
    cp->electa = electa(m, &cp->o);
}

interior i32
visus_currens (
    constans ContextusPraecepti* cp)
{
    redde cp->o.facultas ? VISUS_FACULTATIS
         : cp->o.negotium ? VISUS_NEGOTII : VISUS_INDICIS;
}

interior b32
requisita_negotiorum (
        i32  requisita,
     vacuum* ctx)
{
    ContextusPraecepti* cp;

    cp = (ContextusPraecepti*)ctx;
    redde !(requisita & REQUIRIT_ELECTAM) || cp->electa >= ZEPHYRUM;
}

/* electio: electa (id) et pagina eius */
interior b32
electionem_scribere (
    ContextusPraecepti* cp,
                   s32  index)
{
                chorda  id;
                   s32  k;
             character  pagina[XVI];
    constans character* tituli[II];
    constans character* valores[II];

    id = id_rei(cp->m, &cp->o, index);
    si (id.mensura == ZEPHYRUM)
    {
        redde VERUM;   /* in fine: consumpta, nihil */
    }
    k = album_pagina_rei(cp->o.l, index + I, pagina_currens(cp->m,
        cp->o.l));
    sprintf(pagina, "%d", (integer)(k < ZEPHYRUM ? ZEPHYRUM : k));
    tituli[ZEPHYRUM]   = "electa";
    valores[ZEPHYRUM]  = chorda_ut_cstr(id, cp->p);
    tituli[I]          = "pagina";
    valores[I]         = k > ZEPHYRUM ? pagina : "";
    redde campos_scribere(cp->m, tituli, valores, II);
}

interior b32
fac_proximam (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde electionem_scribere(cp, cp->electa + I);
}

interior b32
fac_priorem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde electionem_scribere(cp, cp->electa - I);
}

/* pagina k: res prima electa */
interior b32
paginam_vertere (
    ContextusPraecepti* cp,
                   s32  gradus)
{
    s32 k;

    k = pagina_currens(cp->m, cp->o.l) + gradus;
    si (k < ZEPHYRUM || k >= album_paginae(cp->o.l))
    {
        redde VERUM;
    }
    redde electionem_scribere(cp, album_res_prima(cp->o.l, k) - I);
}

interior b32
fac_paginam_priorem (
    vacuum* ctx)
{
    redde paginam_vertere((ContextusPraecepti*)ctx, -I);
}

interior b32
fac_paginam_proximam (
    vacuum* ctx)
{
    redde paginam_vertere((ContextusPraecepti*)ctx, I);
}

/* rem index i aperire. Negotium: porta hospitis (latus novum), aliter
 * visus in loco (origo index, historia impelle). Facultas: visus in
 * loco */
interior b32
aperire (
    ContextusPraecepti* cp,
                   s32  i)
{
                chorda  id;
    constans character* tituli[V];
    constans character* valores[V];

    id = id_rei(cp->m, &cp->o, i);
    si (id.mensura == ZEPHYRUM || cp->o.facultas)
    {
        redde VERUM;
    }
    si (!electionem_scribere(cp, i))
    {
        redde FALSUM;
    }
    si (cp->o.negotium)
    {
        /* facultas: visus in loco (historia impelle), electa manet */
        tituli[ZEPHYRUM]   = "visus";
        valores[ZEPHYRUM]  = "facultas";
        tituli[I]          = "facultas";
        valores[I]         = chorda_ut_cstr(id, cp->p);
        tituli[II]         = "pagina_textus";
        valores[II]        = "";
        tituli[III]        = "super";
        valores[III]       = "";
        redde campos_scribere(cp->m, tituli, valores, IV);
    }
    si (cp->m->portae && cp->m->portae->negotium_aperire)
    {
        redde cp->m->portae->negotium_aperire(cp->m->portae->ctx, id);
    }
    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "negotium";
    tituli[I]          = "negotium";
    valores[I]         = chorda_ut_cstr(id, cp->p);
    tituli[II]         = "origo";
    valores[II]        = "index";
    tituli[III]        = "electa";
    valores[III]       = "";
    tituli[IV]         = "pagina";
    valores[IV]        = "";
    redde campos_scribere(cp->m, tituli, valores, V);
}

interior b32
fac_aperire (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde aperire(cp, cp->electa);
}

/* Esc in indice: hospes (vicus) latus tollit; hic non consumitur */
interior b32
fac_redire (
    vacuum* ctx)
{
    (vacuum)ctx;
    redde FALSUM;
}

/* Esc in negotio: ad indicem SOLUM si in loco apertum (origo index;
 * electa = negotium); ab argumento non consumitur - latus prius per
 * vicum (Franus 2026-10-22: Esc non est "retro" universale) */
interior b32
fac_ad_indicem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
    ContextusPraecepti  novus;
                chorda  id;
                   i32  i;
    constans character* tituli[V];
    constans character* valores[V];

    si (!chorda_aequalis_literis(campus(cp->m, "origo"), "index"))
    {
        redde FALSUM;
    }
    id                 = cp->o.negotium->id;
    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "index";
    tituli[I]          = "negotium";
    valores[I]         = "";
    tituli[II]         = "origo";
    valores[II]        = "";
    tituli[III]        = "electa";
    valores[III]       = "";
    tituli[IV]         = "pagina";
    valores[IV]        = "";
    si (!campos_scribere(cp->m, tituli, valores, V))
    {
        redde FALSUM;
    }
    contextum_parare(cp->m, cp->p, &novus);
    per (i = ZEPHYRUM; i < negotia_numerus(cp->m->negotia); i++)
    {
        si (chorda_aequalis(negotia_index(cp->m->negotia, i)->id, id))
        {
            redde electionem_scribere(&novus, (s32)i);
        }
    }
    redde VERUM;
}

/* Esc in facultate: ad facultates, electa = facultas haec (pagina
 * eius) */
interior b32
fac_ad_facultates (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
                   i32  i;
    constans character* tituli[IV];
    constans character* valores[IV];

    per (i = ZEPHYRUM; i < numerus_rerum(cp->m, &cp->o); i++)
    {
        si (chorda_aequalis(id_rei(cp->m, &cp->o, (s32)i),
            cp->o.facultas->id))
        {
            (vacuum)electionem_scribere(cp, (s32)i);
            frange;
        }
    }
    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "negotium";
    tituli[I]          = "facultas";
    valores[I]         = "";
    tituli[II]         = "pagina_textus";
    valores[II]        = "";
    tituli[III]        = "super";
    valores[III]       = "";
    redde campos_scribere(cp->m, tituli, valores, IV);
}

/* visus facultatis: [ ] et sagittae - paginae textus */
interior b32
paginam_textus_vertere (
    ContextusPraecepti* cp,
                   s32  gradus)
{
          s32 paginae;
          s32 k;
    character numerus[XVI];

    paginae = paginae_textus(cp->m, lineae_facultatis(cp->m,
        cp->o.facultas, cp->p));
    k = pagina_textus_currens(cp->m, paginae) + gradus;
    si (k < ZEPHYRUM || k >= paginae)
    {
        redde VERUM;
    }
    sprintf(numerus, "%d", (integer)k);
    redde campum_scribere(cp->m, "pagina_textus", k > ZEPHYRUM ? numerus
        : "");
}

interior b32
fac_textum_priorem (
    vacuum* ctx)
{
    redde paginam_textus_vertere((ContextusPraecepti*)ctx, -I);
}

interior b32
fac_textum_proximum (
    vacuum* ctx)
{
    redde paginam_textus_vertere((ContextusPraecepti*)ctx, I);
}

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "negotia.proximum", "Next project", "j", VISUS_INDICIS, ZEPHYRUM,
        fac_proximam },
    { "negotia.prius", "Previous project", "k", VISUS_INDICIS, ZEPHYRUM,
        fac_priorem },
    { "negotia.pagina_prior", "Previous page", "[", VISUS_INDICIS
        | VISUS_NEGOTII, ZEPHYRUM, fac_paginam_priorem },
    { "negotia.pagina_proxima", "Next page", "]", VISUS_INDICIS
        | VISUS_NEGOTII, ZEPHYRUM, fac_paginam_proximam },
    { "negotia.aperire", "Open project", "Enter", VISUS_INDICIS,
        REQUIRIT_ELECTAM, fac_aperire },
    { "negotia.redire", "Back", "Esc", VISUS_INDICIS, ZEPHYRUM,
        fac_redire },
    { "negotia.facultas_proxima", "Next feature", "j", VISUS_NEGOTII,
        ZEPHYRUM, fac_proximam },
    { "negotia.facultas_prior", "Previous feature", "k", VISUS_NEGOTII,
        ZEPHYRUM, fac_priorem },
    { "negotia.ad_indicem", "Back to list", "Esc", VISUS_NEGOTII,
        ZEPHYRUM, fac_ad_indicem },
    { "negotia.facultas_aperire", "Open feature", "Enter",
        VISUS_NEGOTII, REQUIRIT_ELECTAM, fac_aperire },
    { "negotia.textus_prior", "Previous page", "[", VISUS_FACULTATIS,
        ZEPHYRUM, fac_textum_priorem },
    { "negotia.textus_proximus", "Next page", "]", VISUS_FACULTATIS,
        ZEPHYRUM, fac_textum_proximum },
    { "negotia.ad_facultates", "Back to features", "Esc",
        VISUS_FACULTATIS, ZEPHYRUM, fac_ad_facultates },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

/* mus super ordinem: campus super (scribitur solum si mutatur); exitus
 * delet solum si eius ordinis est. VERUM = consumptum (aliter radix
 * motum accipit) */
interior b32
super_aptare (
       NegotiaMontatio* m,
             Componens* nodus,
      constans Eventus* ev,
               Piscina* p)
{
    chorda a;

    si (   !nodus || nodus->partes != PARTES_ITEM
        || id_ordinis(nodus->id) <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    a = campus(m, "super");
    si (ev->genus == EVENTUS_MUS_EXIIT)
    {
        si (chorda_aequalis(a, nodus->id))
        {
            (vacuum)campum_scribere(m, "super", "");
        }
        redde VERUM;
    }
    si (!chorda_aequalis(a, nodus->id))
    {
        (vacuum)campum_scribere(m, "super", chorda_ut_cstr(nodus->id,
            p));
    }
    redde VERUM;
}

interior b32
agere (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
         NegotiaMontatio* m;
          NegotiaFigurae* privatum;
     constans Praeceptum* t;
      ContextusPraecepti  cp;
                     s32  n;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)des;
    m         = (NegotiaMontatio*)ctx;
    privatum  = (NegotiaFigurae*)m->figurae_ctx;
    si (!privatum)
    {
        redde FALSUM;
    }
    piscina_vacare(privatum->scheda);
    si (   ev->genus == EVENTUS_MUS_MOTUS
        || ev->genus == EVENTUS_MUS_INTRAVIT
        || ev->genus == EVENTUS_MUS_EXIIT)
    {
        redde super_aptare(m, nodus, ev, privatum->scheda);
    }
    contextum_parare(m, privatum->scheda, &cp);
    si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        t = praecepta_ad_clavem(TABULA_PRAECEPTORUM, visus_currens(&cp),
            ev);
        redde t && praecepta_status(t, visus_currens(&cp),
            requisita_negotiorum, &cp) == PRAECEPTUM_PRAESTO
            && t->fn(&cp);
    }
    /* ictus UNUS (mus primum); duplex non aperit iterum */
    si (   !nodus || ev->genus != EVENTUS_MUS_DEPRESSUS
        || ev->datum.mus.botton != MUS_SINISTER)
    {
        redde FALSUM;
    }
    n = nodus->partes == PARTES_ITEM ? id_ordinis(nodus->id) : ZEPHYRUM;
    si (n > ZEPHYRUM)
    {
        redde aperire(&cp, n - I);
    }
    si (   chorda_aequalis_literis(nodus->id, "pagina-prior")
        || chorda_aequalis_literis(nodus->id, "pagina-proxima"))
    {
        n = chorda_aequalis_literis(nodus->id, "pagina-prior") ? -I : I;
        redde cp.o.facultas ? paginam_textus_vertere(&cp, n)
                            : paginam_vertere(&cp, n);
    }
    redde FALSUM;
}


/* ==================================================
 * Figura
 * ================================================== */

interior chorda
praecidere (
    chorda textus,
       s32 latitudo)
{
    s32 n;

    n = latitudo / NEGOTIA_LITTERA;
    redde runas_praecidere(textus, n < ZEPHYRUM ? ZEPHYRUM : (i32)n);
}

interior vacuum
textum_pingere (
       Mandata* md,
           s32  x,
           s32  y,
        chorda  textus,
           i32  fons,
    ColorThema  color)
{
    mandata_textus(md, x, y, chorda_transcribere(textus, md->piscina),
        fons, figura_color_thematis(color));
}

/* fundus ordinis: electus vincit super; ceteri linea infra */
interior vacuum
fundum_ordinis_pingere (
               Mandata* md,
    constans Componens* l,
                   b32  electus,
                   b32  super)
{
    Punctum p;
    Punctum q;

    si (electus)
    {
        figura_ordinem_electum_pingere(md, l->fines);
        redde;
    }
    si (super)
    {
        figura_ordinem_super_pingere(md, l->fines);
    }
    p.x = l->fines.x;
    p.y = l->fines.y + l->fines.altitudo - I;
    q.x = l->fines.x + l->fines.latitudo - I;
    q.y = p.y;
    mandata_linea(md, p, q, I,
        figura_color_thematis(COLOR_SUPERFICIES));
}

/* ordo indicis: titulus, summarium infra (obscurum; in ordine electo
 * colore textus - obscurum in fundo selectionis evanescit) */
interior vacuum
negotium_pingere (
       NegotiaMontatio* m,
               Mandata* md,
    constans Componens* l,
     constans Negotium* x,
                   b32  electus)
{
    textum_pingere(md, l->fines.x + VIII, l->fines.y + IV, praecidere(
        x->titulus, l->fines.latitudo - XVI), ZEPHYRUM, COLOR_TEXT);
    textum_pingere(md, l->fines.x + VIII, l->fines.y + IV
        + NEGOTIA_LINEA + II, praecidere(negotia_summarium(m->negotia,
        x->id, md->piscina), l->fines.latitudo - XVI), ZEPHYRUM,
        electus ? COLOR_TEXT : COLOR_TEXT_DIM);
}

/* ordo facultatis: globulus status (shipped plenus, partial vacuus,
 * planned obscurus), titulus */
interior vacuum
facultatem_pingere (
                     Mandata* md,
          constans Componens* l,
    constans NegotiiFacultas* f)
{
    Fines g;
      i32 status;

    status      = f->status == NEGOTIA_PERFECTA ? FIGURA_ELECTUM
                : f->status == NEGOTIA_PROPOSITA ? FIGURA_OBSCURUM
                : ZEPHYRUM;
    g.x         = l->fines.x + VI;
    g.y         = l->fines.y + II;
    g.latitudo  = NEGOTIA_GLOBULUS;
    g.altitudo  = l->fines.altitudo - IV;
    figura_globulum_pingere(md, g, chorda_ex_literis(
        negotia_status_titulus(f->status), md->piscina), status);
    textum_pingere(md, g.x + g.latitudo + VI, l->fines.y + IV,
        praecidere(f->titulus, l->fines.latitudo - g.latitudo - XVIII),
        ZEPHYRUM, COLOR_TEXT);
}

interior vacuum
caput_pingere (
           NegotiaMontatio* m,
                   Mandata* md,
        constans Componens* f,
          constans Ordines* o)
{
       chorda t;
          i32 n;
    character textus[XXXII];

    si (o->facultas)
    {
         Fines  g;
           Xar* rs;
           i32  status;

        status      = o->facultas->status == NEGOTIA_PERFECTA
                    ? FIGURA_ELECTUM : o->facultas->status
                    == NEGOTIA_PROPOSITA ? FIGURA_OBSCURUM : ZEPHYRUM;
        g.x         = f->fines.x + IV;
        g.y         = f->fines.y + I;
        g.latitudo  = NEGOTIA_GLOBULUS;
        g.altitudo  = f->fines.altitudo - II;
        figura_globulum_pingere(md, g, chorda_ex_literis(
            negotia_status_titulus(o->facultas->status), md->piscina),
            status);
        rs = negotia_revisiones(m->negotia, o->facultas->id,
            md->piscina);
        n = rs ? xar_numerus(rs) : ZEPHYRUM;
        sprintf(textus, "%u revision%s", (insignatus integer)n, n == I
            ? "" : "s");
        textum_pingere(md, g.x + g.latitudo + VI, f->fines.y + IV,
            chorda_ex_literis(textus, md->piscina), ZEPHYRUM,
            COLOR_TEXT_DIM);
        redde;
    }
    si (o->negotium)
    {
        t = negotia_summarium(m->negotia, o->negotium->id, md->piscina);
    }
    alioquin
    {
        n = negotia_numerus(m->negotia);
        sprintf(textus, "%u project%s", (insignatus integer)n, n == I
            ? "" : "s");
        t = chorda_ex_literis(textus, md->piscina);
    }
    textum_pingere(md, f->fines.x + IV, f->fines.y + IV, praecidere(t,
        f->fines.latitudo - VIII), ZEPHYRUM, COLOR_TEXT_DIM);
}

/* pagina k linearum facultatis: corpus colore textus, titulus
 * historiae audax, revisiones obscurae; numquam infra componentem */
interior vacuum
textum_facultatis_pingere (
        NegotiaMontatio* m,
                Mandata* md,
     constans Componens* f,
           constans Xar* lineae,
                    s32  k)
{
    constans LineaFacultatis* l;
                         i32  per_paginam;
                         i32  i;
                         s32  y;

    per_paginam  = lineae_paginae(m);
    y            = f->fines.y + II;
    per (i = (i32)k * per_paginam; lineae && i < xar_numerus(lineae)
         && i < ((i32)k + I) * per_paginam; i++)
    {
        si (y + NEGOTIA_LINEA > f->fines.y + f->fines.altitudo)
        {
            frange;
        }
        l = (constans LineaFacultatis*)xar_obtinere(lineae, i);
        si (l->genus == LINEA_TITULUS)
        {
            figura_textum_audacem(md, f->fines.x + IV, y,
                chorda_transcribere(l->textus, md->piscina),
                figura_color_thematis(COLOR_TEXT));
        }
        alioquin si (l->genus != LINEA_VACUA)
        {
            textum_pingere(md, f->fines.x + IV, y, praecidere(l->textus,
                f->fines.latitudo - VIII), ZEPHYRUM, l->genus
                == LINEA_HISTORIA ? COLOR_TEXT_DIM : COLOR_TEXT);
        }
        y += NEGOTIA_SPATIUM;
    }
}

interior vacuum
figura_tabulae (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
     NegotiaFigurae* contextus;
    NegotiaMontatio* m;
          Componens* f;
            Ordines  o;
              Fines  mensa;
             chorda  super;
                Xar* lineae;
                s32  e;
                s32  k;
                s32  n;
                s32  id;
                i32  i;
          character  textus[XXXII];

    (vacuum)thema;
    contextus       = (NegotiaFigurae*)ctx;
    mensa.x         = ZEPHYRUM;
    mensa.y         = ZEPHYRUM;
    mensa.latitudo  = c->fines.latitudo;
    mensa.altitudo  = c->fines.altitudo;
    figura_mensam_pingere(md, mensa, chorda_aequalis_literis(c->titulus,
        "focatum"));
    mensa.x         = NEGOTIA_LITTERA;
    mensa.y         = NEGOTIA_LINEA;
    mensa.latitudo  = c->fines.latitudo - II * NEGOTIA_LITTERA;
    mensa.altitudo  = c->fines.altitudo - II * NEGOTIA_LINEA;
    mandata_rectangulum(md, mensa, figura_color_thematis(
        COLOR_BACKGROUND), VERUM);
    si (!contextus || !contextus->m || !contextus->m->negotia)
    {
        redde;
    }
    m      = contextus->m;
    ordines_parare(m, md->piscina, &o);
    e       = electa(m, &o);
    k       = pagina_currens(m, o.l);
    n       = album_paginae(o.l);
    super   = campus(m, "super");
    lineae  = NIHIL;
    si (o.facultas)
    {
        lineae  = lineae_facultatis(m, o.facultas, md->piscina);
        n       = paginae_textus(m, lineae);
        k       = pagina_textus_currens(m, n);
    }
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        f = componens_liberum(c, i);
        si (chorda_aequalis_literis(f->id, "titulus"))
        {
            textum_pingere(md, f->fines.x + IV, f->fines.y + II,
                o.facultas ? praecidere(o.facultas->titulus,
                f->fines.latitudo - VIII) : o.negotium ? praecidere(
                o.negotium->titulus, f->fines.latitudo - VIII)
                : chorda_ex_literis("Projects", md->piscina),
                FONS_TITULI,
                COLOR_TEXT);
        }
        alioquin si (chorda_aequalis_literis(f->id, "caput"))
        {
            caput_pingere(m, md, f, &o);
        }
        alioquin si (chorda_aequalis_literis(f->id, "index_paginae"))
        {
            si (n > I)
            {
                sprintf(textus, "%d/%d", (integer)(k + I), (integer)n);
                textum_pingere(md, f->fines.x + (f->fines.latitudo
                    - (s32)strlen(textus) * NEGOTIA_LITTERA) / II,
                    f->fines.y + IV, chorda_ex_literis(textus,
                    md->piscina), ZEPHYRUM, COLOR_TEXT_DIM);
            }
        }
        alioquin si (chorda_aequalis_literis(f->id, "pagina-prior"))
        {
            figura_sagittam_pingere(md, f->fines, -I, k <= ZEPHYRUM
                ? FIGURA_OBSCURUM : ZEPHYRUM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "pagina-proxima"))
        {
            figura_sagittam_pingere(md, f->fines, I, k >= n - I
                ? FIGURA_OBSCURUM : ZEPHYRUM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "textus"))
        {
            textum_facultatis_pingere(m, md, f, lineae, k);
        }
        alioquin si (chorda_aequalis_literis(f->id, "nihil"))
        {
            textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis(o.negotium
                ? "No features yet - add one with `negotia facultas`"
                : "No projects yet - create one with `negotia creare`",
                md->piscina), ZEPHYRUM, COLOR_TEXT_DIM);
        }
        alioquin si (f->partes == PARTES_ITEM)
        {
            id = id_ordinis(f->id);
            si (id <= ZEPHYRUM || (i32)id > numerus_rerum(m, &o))
            {
                perge;
            }
            fundum_ordinis_pingere(md, f, id - I == e, chorda_aequalis(
                super, f->id));
            si (o.negotium)
            {
                facultatem_pingere(md, f, (constans NegotiiFacultas*)
                    xar_obtinere(o.facultates, (i32)(id - I)));
            }
            alioquin
            {
                negotium_pingere(m, md, f, negotia_index(m->negotia,
                    (i32)(id - I)), id - I == e);
            }
        }
    }
}

interior b32
figuras_registrare (
    NegotiaMontatio* m,
                i32  latitudo,
                i32  altitudo)
{
    NegotiaFigurae* contextus;

    contextus = (NegotiaFigurae*)piscina_allocare(m->piscina,
        magnitudo(NegotiaFigurae));
    si (!contextus)
    {
        redde FALSUM;
    }
    memset(contextus, ZEPHYRUM, magnitudo(NegotiaFigurae));
    contextus->m                     = m;
    contextus->scheda                = piscina_generare_dynamicum(
        "negotia_scheda", XVI * M);
    contextus->latitudo_montationis  = (s32)latitudo;
    contextus->altitudo_montationis  = (s32)altitudo;
    m->figurae_ctx                   = contextus;
    redde contextus->scheda && figura_registrare(m->figurae,
        PARTES_TABULA, ZEPHYRUM, figura_tabulae, contextus);
}


/* ==================================================
 * Componere (dispositio in cellulis)
 * ================================================== */

nomen structura {
           s32  index;
     Componens* c;
} LocusComponentis;

nomen structura {
             Dispositio* d;
                    Xar* loci;
                Piscina* piscina;
    InternamentumChorda* intern;
} Disponens;

interior s32
locum_addere (
              Disponens* dp,
                    s32  parens,
     DispositioDirectio  directio,
        DispositioGenus  genus_x,
                    s32  valor_x,
        DispositioGenus  genus_y,
                    s32  valor_y)
{
    DispositioForma forma;

    dispositio_formam_initiare(&forma);
    forma.directio        = directio;
    forma.latitudo.genus  = genus_x;
    forma.latitudo.valor  = valor_x;
    forma.altitudo.genus  = genus_y;
    forma.altitudo.valor  = valor_y;
    redde dispositio_addere(dp->d, parens, &forma);
}

/* folium cum componente (actio lateris) */
interior Componens*
componentem_locare (
              Disponens* dp,
                    s32  parens,
     constans character* id,
                 Partes  partes,
        DispositioGenus  genus_x,
                    s32  valor_x,
                    s32  lineae)
{
            Componens* c;
     LocusComponentis* l;
                  s32  index;

    index = locum_addere(dp, parens, DISPOSITIO_LINEA, genus_x, valor_x,
        DISPOSITIO_FIXA, lineae);
    c     = componens_creare(dp->piscina, dp->intern, id, partes);
    si (!c || index < ZEPHYRUM)
    {
        redde NIHIL;
    }
    componens_ponere_actio(c, actio_negotiorum);
    l = (LocusComponentis*)xar_addere(dp->loci);
    si (l)
    {
        l->index  = index;
        l->c      = c;
    }
    redde c;
}

/* titulus, caput (summarium, paginae), regio ordinum aut textus
 * facultatis */
interior vacuum
disponere (
    NegotiaMontatio* m,
          Disponens* dp,
                s32  radix_d,
            Piscina* piscina)
{
               Ordines  o;
    constans AlbumOrdo* x;
                   s32  caput;
                   s32  regio;
                   s32  a;
                   s32  b;
                   s32  t;
                   s32  i;

    (vacuum)componentem_locare(dp, radix_d, "titulus", PARTES_TITULUS,
        DISPOSITIO_CRESCENS, ZEPHYRUM, NEGOTIA_TITULI);
    caput = locum_addere(dp, radix_d, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, NEGOTIA_CAPUT);
    (vacuum)componentem_locare(dp, caput, "caput", PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, NEGOTIA_CAPUT);
    (vacuum)componentem_locare(dp, caput, "pagina-prior",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, NEGOTIA_CAPUT);
    (vacuum)componentem_locare(dp, caput, "index_paginae",
        PARTES_INDEX, DISPOSITIO_FIXA, VII, NEGOTIA_CAPUT);
    (vacuum)componentem_locare(dp, caput, "pagina-proxima",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, NEGOTIA_CAPUT);
    regio = locum_addere(dp, radix_d, DISPOSITIO_COLUMNA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    ordines_parare(m, piscina, &o);
    si (o.facultas)
    {
        (vacuum)componentem_locare(dp, regio, "textus", PARTES_INDEX,
            DISPOSITIO_CRESCENS, ZEPHYRUM, capacitas(m));
        redde;
    }
    si (!o.l || album_numerus(o.l) == ZEPHYRUM)
    {
        (vacuum)componentem_locare(dp, regio, "nihil", PARTES_INDEX,
            DISPOSITIO_CRESCENS, ZEPHYRUM, NEGOTIA_ORDO);
        redde;
    }
    album_pagina(o.l, pagina_currens(m, o.l), &a, &b, &t);
    per (i = a; i < b; i++)
    {
        x = album_ordo(o.l, (i32)i);
        (vacuum)componentem_locare(dp, regio, chorda_ut_cstr(x->id,
            piscina), PARTES_ITEM, DISPOSITIO_CRESCENS, ZEPHYRUM,
            x->altitudo);
    }
}

Componens*
negotia_visus_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
    NegotiaMontatio* m;
          Componens* radix;
          Disponens  dp;
    DispositioForma  forma;
                s32  latitudo;
                s32  altitudo;
                s32  columnae;
                s32  lineae;
                s32  radix_d;
                s32  i;
              Fines  f;

    (vacuum)repo;
    m = (NegotiaMontatio*)ctx;
    si (!m)
    {
        redde NIHIL;
    }
    latitudo = superficies(m, VERUM);
    altitudo = superficies(m, FALSUM);
    radix     = componens_creare(piscina, intern, "negotia",
        PARTES_TABULA);
    si (!radix)
    {
        redde NIHIL;
    }
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = latitudo;
    f.altitudo  = altitudo;
    componens_ponere_fines(radix, f);
    componens_ponere_focusabilis(radix, VERUM);
    componens_ponere_actio(radix, actio_negotiorum);
    si (motus_ramus_focatus(motus, m->ramus))
    {
        componens_ponere_titulum(radix, "focatum");
    }
    columnae    = latitudo / NEGOTIA_LITTERA;
    lineae      = altitudo / NEGOTIA_LINEA;
    dp.d        = dispositio_creare(piscina);
    dp.loci     = xar_creare(piscina, (i32)magnitudo(LocusComponentis));
    dp.piscina  = piscina;
    dp.intern   = intern;
    si (!dp.d || !dp.loci)
    {
        redde radix;
    }
    dispositio_formam_initiare(&forma);
    forma.directio           = DISPOSITIO_COLUMNA;
    forma.latitudo.genus     = DISPOSITIO_CRESCENS;
    forma.altitudo.genus     = DISPOSITIO_CRESCENS;
    forma.spatium_sinistrum  = I;
    forma.spatium_dextrum    = I;
    forma.spatium_superum    = I;
    forma.spatium_inferum    = I;
    radix_d                  = dispositio_addere(dp.d, -I, &forma);
    disponere(m, &dp, radix_d, piscina);
    dispositio_computare(dp.d, columnae, lineae, NIHIL, NIHIL);
    per (i = ZEPHYRUM; i < (s32)xar_numerus(dp.loci); i++)
    {
        LocusComponentis* loc;

        loc = (LocusComponentis*)xar_obtinere(dp.loci, (i32)i);
        componens_ponere_fines(loc->c, dispositio_ad_pixela(
            dispositio_fines(dp.d, loc->index), columnae, lineae,
            latitudo, altitudo, NEGOTIA_LITTERA, NEGOTIA_LINEA));
        componens_addere_liberum(radix, loc->c);
    }
    redde radix;
}


/* ==================================================
 * Montatio, argumentum, pulsus
 * ================================================== */

b32
negotia_visus_montare (
                NegotiaMontatio* m,
                        Piscina* piscina,
            InternamentumChorda* intern,
             InsulaRepositorium* repo,
             constans character* id,
                        Negotia* negotia,
    constans NegotiaVisusPortae* portae,
                            i32  latitudo,
                            i32  altitudo,
                StatusSystemata* systemata)
{
    Organum* organum;

    si (!m || !piscina || !intern || !repo || !negotia)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(NegotiaMontatio));
    m->piscina      = piscina;
    m->intern       = intern;
    m->negotia      = negotia;
    m->portae       = portae;
    m->versio_visa  = negotia_versio(negotia);
    m->ramus        = id ? insula_ramus(repo, "negotia", id)
                         : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "negotia",
        "negotia");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"negotia\"", id ? id : "negotia", piscina,
               intern))
    {
        redde FALSUM;
    }
    m->actiones  = actio_registrum_creare(piscina, intern);
    m->figurae   = figura_registrum_creare(piscina);
    si (   !m->actiones || !m->figurae
        || !praecepta_verificare(TABULA_PRAECEPTORUM)
        || !actio_registrare(m->actiones, actio_negotiorum, agere, m)
        || !figuras_registrare(m, latitudo, altitudo))
    {
        redde FALSUM;
    }
    ((NegotiaFigurae*)m->figurae_ctx)->organum = organum;
    redde VERUM;
}

/* SemitaOrganum lateris: manifestum suum (liberi nulli); ctx =
 * NegotiaMontatio* */
interior constans Organum*
organa (
    vacuum* ctx,
    chorda  genus)
{
    NegotiaMontatio* m = (NegotiaMontatio*)ctx;

    si (   !m || !m->figurae_ctx
        || !chorda_aequalis_literis(genus, "negotia"))
    {
        redde NIHIL;
    }
    redde ((NegotiaFigurae*)m->figurae_ctx)->organum;
}

b32
negotia_visus_argumentum_ponere (
       NegotiaMontatio* m,
    constans character* argumentum)
{
             StmlNodus* semita;
                chorda* id;
                chorda  vitium;
     constans Negotium* x;
    constans character* arg;
    constans character* tituli[IV];
    constans character* valores[IV];

    si (!m || !m->negotia)
    {
        redde FALSUM;
    }
    arg = argumentum ? argumentum : "";
    /* id nudum aut suffixum unicum -> id plenum (cognomen typatum) */
    si (arg[ZEPHYRUM] && !strchr(arg, '/'))
    {
        x = negotia_negotium_invenire(m->negotia, chorda_ex_literis(arg,
            m->piscina), NIHIL);
        si (!x)
        {
            redde FALSUM;
        }
        arg = chorda_ut_cstr(x->id, m->piscina);
    }
    semita = semita_ex_argumento(organa, m, "negotia", arg, m->piscina,
        m->intern, &vitium);
    si (!semita)
    {
        redde FALSUM;
    }
    /* negotium ignotum: recusatur ante scripturam (URI: id exactum) */
    id = stml_attributum_capere(semita, "negotium");
    si (id && id->mensura > ZEPHYRUM)
    {
        x = negotia_negotium_invenire(m->negotia, *id, NIHIL);
        si (!x || !chorda_aequalis(x->id, *id))
        {
            redde FALSUM;
        }
    }
    /* facultas ignota aut negotii alieni: recusatur */
    {
                          chorda* fid;
        constans NegotiiFacultas* f;

        fid = stml_attributum_capere(semita, "facultas");
        si (fid && fid->mensura > ZEPHYRUM)
        {
            f = negotia_facultatem_invenire(m->negotia, *fid, NIHIL);
            si (   !f || !chorda_aequalis(f->id, *fid) || !id
                || !chorda_aequalis(f->negotium, *id))
            {
                redde FALSUM;
            }
        }
    }
    si (!semita_applicare(organa, m, &m->ramus, semita, m->piscina,
            &vitium))
    {
        redde FALSUM;
    }
    /* sequela lateris (non semita): origo argumentum, electio nova */
    tituli[ZEPHYRUM]   = "origo";
    valores[ZEPHYRUM]  = "";
    tituli[I]          = "electa";
    valores[I]         = "";
    tituli[II]         = "pagina";
    valores[II]        = "";
    tituli[III]        = "super";
    valores[III]       = "";
    redde campos_scribere(m, tituli, valores, IV);
}

b32
negotia_visus_reficere (
    NegotiaMontatio* m)
{
    s64 v;

    si (!m || !m->negotia)
    {
        redde FALSUM;
    }
    (vacuum)negotia_renovare(m->negotia);
    v = negotia_versio(m->negotia);
    si (v == m->versio_visa)
    {
        redde FALSUM;
    }
    m->versio_visa = v;
    redde VERUM;
}

s64
negotia_visus_mora (
    constans NegotiaMontatio* m)
{
    (vacuum)m;
    redde (s64)M;
}

chorda
negotia_visus_locus_titulus (
     vacuum* ctx,
    Piscina* piscina)
{
             NegotiaMontatio* m;
           constans Negotium* x;
    constans NegotiiFacultas* f;
           ChordaAedificator* a;

    m = (NegotiaMontatio*)ctx;
    x = m ? apertum(m) : NIHIL;
    f = x ? facultas_aperta(m, x) : NIHIL;
    si (f)
    {
        /* "<negotium> · <facultas>" (comites: locus facultatis) */
        a = chorda_aedificator_creare(piscina, LXIV);
        (vacuum)chorda_aedificator_appendere_chorda(a, x->titulus);
        (vacuum)chorda_aedificator_appendere_literis(a, " \302\267 ");
        (vacuum)chorda_aedificator_appendere_chorda(a, f->titulus);
        redde chorda_aedificator_spectare(a);
    }
    redde x ? chorda_transcribere(x->titulus, piscina)
            : chorda_ex_literis("Projects", piscina);
}
