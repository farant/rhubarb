/* lector.c - lector in latere vici (include/lector.h). Forma
 * sancti_visus: arbor dispositionis in cellulis (LITTERA x LINEA),
 * figura radicis omnia pingit, praecepta per membrum__praecepta,
 * paginae bibliothecae per membrum__album. */

#include "lector.h"
#include "membrum__album.h"
#include "membrum__praecepta.h"
#include "dispositio.h"
#include "fons.h"
#include "thema.h"
#include "motus.h"
#include "lector_paginae.h"
#include "codex.h"
#include "semita.h"

#include <stdio.h>
#include <string.h>

#define LECTOR_LATITUDO  CCC      /* superficies absens */
#define LECTOR_ALTITUDO  CCLVI
#define LECTOR_LITTERA   VI
#define LECTOR_LINEA     VIII
#define LECTOR_TITULI    III      /* lineae tituli */
#define LECTOR_CAPUT     II       /* lineae capitis (paginae) */
#define LECTOR_ORDO      III      /* lineae ordinis: titulus, auctor */
#define LECTOR_ORDO_CAPITIS II    /* lineae ordinis indicis capitum */

#define VISUS_BIBLIOTHECAE  I
#define VISUS_INDICIS       II
#define VISUS_PAGINAE       IV
#define VISUS_SENTENTIAE    VIII
#define VISUS_FINDENDI      XVI     /* sententia + cursor_findendi */
#define VISUS_FAVORUM       XXXII
/* = INTERVALLUM_LINEARUM lib/lector_paginae.c: passus linearum
 * sententiae = altitudo fontis + intervallum */
#define INTERVALLUM_SENTENTIAE  VIII
#define REQUIRIT_ELECTUM    I

hic_manens constans character actio_lectoris[] = "lector.agere";

/* figura: contextus privatus */
nomen structura {
       LectorMontatio* m;
     constans Organum* organum;
              Piscina* scheda;
                  s32  latitudo_montationis;
                  s32  altitudo_montationis;
    /* cache capitis currentis: liber, caput, area -> textus,
     * sententiae, paginae (arca vacatur cum mutantur) */
              Piscina* arca;
               chorda  liber_arcae;
                  s32  caput_arcae;     /* -1 = nullum */
                  s32  latitudo_arcae;
                  s32  altitudo_arcae;
                Codex* codex;
           CodexCaput* caput;
                  Xar* sententiae;      /* CodexSententia (emendatae) */
                  i32  amissae;         /* emendationes non repertae */
                  Xar* favores;         /* s32: offsets favorum */
                  i32  versio_arcae;    /* lector_bibliotheca_versio */
                  Xar* paginae;         /* LectorPagina */
    /* cache favorum libri currentis: liber, versio, latitudo ->
     * ordines (FavorOrdo); arca sua, capita sua */
              Piscina* arca_favorum;
               chorda  liber_favorum;
                  i32  versio_favorum;
                  s32  latitudo_favorum;
                  b32  favores_parati;
                  Xar* ordines_favorum;
} LectorFigurae;

/* ordo indicis favorum: cursus sententiarum consecutivarum quae omnes
 * favores sunt (unus si sola) */
nomen structura {
       s32  caput;
       i32  initium;   /* sententiae primae cursus */
    chorda  textus;    /* textus capitis (arca favorum) */
       Xar* lineae;    /* LectorLinea, FONS_PARVUS, x = y = 0 */
} FavorOrdo;


/* ==================================================
 * Status (insula)
 * ================================================== */

interior chorda
_campus (
    constans LectorMontatio* m,
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

nomen structura {
    constans character* constans* tituli;
    constans character* constans* valores;
                             i32 numerus;
} GrexCamporum;

interior vacuum
_campos_ponere (
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

/* campi actu uno, scriptore lectoris ("" aut NIHIL tollit) */
interior b32
_campos_scribere (
                LectorMontatio* m,
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
        actio_lectoris, m->piscina));
    ok = mutare_ramum(&m->ramus, _campos_ponere, &grex);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

interior s32
_superficies (
    constans LectorMontatio* m,
                        b32  latitudo)
{
    constans LectorFigurae* f;
                       s32  v;
                    chorda  a;
                       s32  n;

    f = (constans LectorFigurae*)m->figurae_ctx;
    v = !f ? ZEPHYRUM : latitudo ? f->latitudo_montationis
                                 : f->altitudo_montationis;
    si (v <= ZEPHYRUM)
    {
        v = latitudo ? LECTOR_LATITUDO : LECTOR_ALTITUDO;
    }
    a = _campus(m, latitudo ? "superficies_latitudo"
        : "superficies_altitudo");
    redde (a.mensura > ZEPHYRUM && chorda_ut_s32(a, &n) && n > ZEPHYRUM)
        ? n : v;
}

interior i32
_visus (
    constans LectorMontatio* m)
{
    chorda v;

    v = _campus(m, "visus");
    si (chorda_aequalis_literis(v, "pagina"))
    {
        redde VISUS_PAGINAE;
    }
    si (chorda_aequalis_literis(v, "sententia"))
    {
        redde VISUS_SENTENTIAE;
    }
    si (chorda_aequalis_literis(v, "index"))
    {
        redde VISUS_INDICIS;
    }
    si (chorda_aequalis_literis(v, "favores"))
    {
        redde VISUS_FAVORUM;
    }
    redde VISUS_BIBLIOTHECAE;
}

/* campus numerus (>= 0); -1 si abest aut malus */
interior s32
_campus_numerus (
    constans LectorMontatio* m,
         constans character* titulus)
{
    chorda a;
       s32 n;

    a = _campus(m, titulus);
    redde (a.mensura > ZEPHYRUM && chorda_ut_s32(a, &n)
        && n >= ZEPHYRUM)
        ? n : -I;
}

/* margines paginae ut liber (cellulae, intra marginem radicis): lateris
 * X, supra et infra VI; in latere angusto minuuntur ne columna textus
 * infra XL litteras cadat (lateris saltem I, supra et infra saltem I:
 * titulus currens et numerus paginae ibi stant) */
interior vacuum
_margines (
    constans LectorMontatio* m,
                        s32* lateris,
                        s32* supra)
{
    s32 columnae;
    s32 lineae;

    columnae  = _superficies(m, VERUM) / LECTOR_LITTERA - II;
    lineae    = _superficies(m, FALSUM) / LECTOR_LINEA - II;
    *lateris  = (columnae - XL) / II;
    *supra    = (lineae - XX) / II;
    si (*lateris > X)
    {
        *lateris = X;
    }
    si (*lateris < I)
    {
        *lateris = I;
    }
    si (*supra > VI)
    {
        *supra = VI;
    }
    si (*supra < I)
    {
        *supra = I;
    }
}

/* area textus paginae (px): superficies minus margo radicis et margines
 * libri - eadem in dispositione et pictura */
interior vacuum
_area (
    constans LectorMontatio* m,
                        s32* latitudo,
                        s32* altitudo)
{
    s32 lateris;
    s32 supra;

    _margines(m, &lateris, &supra);
    *latitudo  = (_superficies(m, VERUM) / LECTOR_LITTERA - II
        - II * lateris) * LECTOR_LITTERA;
    *altitudo  = (_superficies(m, FALSUM) / LECTOR_LINEA - II
        - II * supra) * LECTOR_LINEA;
    si (*latitudo < LECTOR_LITTERA)
    {
        *latitudo = LECTOR_LITTERA;
    }
    si (*altitudo < LECTOR_LINEA)
    {
        *altitudo = LECTOR_LINEA;
    }
}

/* liber campi "liber"; NIHIL si nullus aut non legibilis */
interior constans LectorLiber*
_liber_currens (
    constans LectorMontatio* m)
{
    redde lector_bibliotheca_invenire(m->bibliotheca, _campus(m,
        "liber"));
}

/* emendationes libri pro capite k (textus caput), per vestigium
 * repertae; non repertae non applicantur et *amissae augent */
interior CodexEmendatio*
_emendationes_capitis (
         LectorMontatio* m,
    constans CodexCaput* caput,
                 chorda  identitas,
                    s32  k,
                Piscina* p,
                    i32* numerus,
                    i32* amissae)
{
                         Xar* lista;
    constans LectorEmendatio* le;
              CodexEmendatio* e;
                         i32  i;
                         s32  r;

    *numerus = ZEPHYRUM;
    lista = lector_emendationes_legere(m->bibliotheca, identitas,
        p);
    si (!lista || xar_numerus(lista) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    e = (CodexEmendatio*)piscina_allocare(p, (memoriae_index)
        xar_numerus(lista) * magnitudo(CodexEmendatio));
    per (i = ZEPHYRUM; e && i < xar_numerus(lista); i++)
    {
        le = (constans LectorEmendatio*)xar_obtinere(lista, i);
        si (le->caput != (i32)k)
        {
            perge;
        }
        r = codex_reperire(caput, le->positio, le->vestigium);
        si (r < ZEPHYRUM)
        {
            (*amissae)++;
            perge;
        }
        e[*numerus].genus    = le->genus;
        e[*numerus].positio  = (i32)r;
        (*numerus)++;
    }
    redde e;
}

/* favores libri pro capite k: offsets per vestigium reperti (Xar de
 * s32); non reperti *amissae augent ut emendationes */
interior Xar*
_favores_capitis (
         LectorMontatio* m,
    constans CodexCaput* caput,
                 chorda  identitas,
                    s32  k,
                Piscina* p,
                    i32* amissae)
{
                       Xar* lista;
                       Xar* exitus;
    constans LectorPositio* q;
                       i32  i;
                       s32  r;

    exitus  = xar_creare(p, (i32)magnitudo(s32));
    lista   = lector_favores_legere(m->bibliotheca, identitas, p);
    per (i = ZEPHYRUM; exitus && lista && i < xar_numerus(lista); i++)
    {
        q = (constans LectorPositio*)xar_obtinere(lista, i);
        si (q->caput != (i32)k)
        {
            perge;
        }
        r = codex_reperire(caput, q->initium, q->vestigium);
        si (r < ZEPHYRUM)
        {
            (*amissae)++;
            perge;
        }
        *(s32*)xar_addere(exitus) = r;
    }
    redde exitus;
}

/* caput k libri identitas in arca (semel per liber, caput, aream);
 * FALSUM si liber aut caput non legitur */
interior b32
_caput_parare (
    LectorMontatio* m,
            chorda  identitas,
               s32  k)
{
           LectorFigurae* f;
    constans LectorLiber* l;
          CodexEmendatio* emendationes;
                  chorda  vitium;
                     s32  latitudo;
                     s32  altitudo;
                     i32  numerus;

    f = (LectorFigurae*)m->figurae_ctx;
    _area(m, &latitudo, &altitudo);
    si (   f->caput && f->caput_arcae == k
        && f->latitudo_arcae == latitudo
        && f->altitudo_arcae == altitudo
        && f->versio_arcae == lector_bibliotheca_versio(m->bibliotheca)
        && chorda_aequalis(f->liber_arcae, identitas))
    {
        redde VERUM;
    }
    piscina_vacare(f->arca);
    f->caput        = NIHIL;
    f->caput_arcae  = -I;
    l               = lector_bibliotheca_invenire(m->bibliotheca,
        identitas);
    f->codex        = l ? lector_bibliotheca_codex(m->bibliotheca, l,
        &vitium) : NIHIL;
    si (   !f->codex || k < ZEPHYRUM
        || (i32)k >= codex_numerus_capitum(f->codex))
    {
        redde FALSUM;
    }
    f->caput = codex_caput_legere(f->codex, (i32)k, f->arca, &vitium);
    si (!f->caput)
    {
        redde FALSUM;
    }
    f->amissae         = ZEPHYRUM;
    emendationes       = _emendationes_capitis(m, f->caput, identitas,
        k,
        f->arca, &numerus, &f->amissae);
    f->sententiae      = codex_sententiae(f->caput, emendationes,
        numerus,
        f->arca);
    f->paginae         = lector_paginare(f->caput, (i32)latitudo,
        (i32)altitudo, f->arca);
    f->favores         = _favores_capitis(m, f->caput, identitas, k,
        f->arca, &f->amissae);
    f->versio_arcae    = lector_bibliotheca_versio(m->bibliotheca);
    f->liber_arcae     = chorda_transcribere(identitas, f->arca);
    f->caput_arcae     = k;
    f->latitudo_arcae  = latitudo;
    f->altitudo_arcae  = altitudo;
    si (!f->sententiae || !f->paginae)
    {
        f->caput = NIHIL;
        redde FALSUM;
    }
    redde VERUM;
}

/* caput currens pane (campi liber, caput) in arca */
interior LectorFigurae*
_caput_currens (
    LectorMontatio* m)
{
    LectorFigurae* f;

    f = (LectorFigurae*)m->figurae_ctx;
    redde _caput_parare(m, _campus(m, "liber"), _campus_numerus(m,
        "caput")) ? f : NIHIL;
}


/* ==================================================
 * Positio et motus
 * ================================================== */

/* positio paginae k capitis in arca: sententia prima quae in pagina
 * incipit; nulla -> sententia quae initium paginae continet */
interior LectorPositio
_positio_paginae (
    constans LectorFigurae* f,
                       s32  k)
{
              LectorPositio  q;
    constans CodexSententia* t;
                        i32  a;
                        i32  b;
                        i32  i;

    q.caput              = (i32)f->caput_arcae;
    q.initium            = ZEPHYRUM;
    q.vestigium          = f->caput->textus;
    q.vestigium.mensura  = ZEPHYRUM;
    a = ((constans LectorPagina*)xar_obtinere(f->paginae, (i32)k))
        ->initium;
    b = (i32)k + I < xar_numerus(f->paginae) ? ((constans LectorPagina*)
        xar_obtinere(f->paginae, (i32)k + I))->initium
        : f->caput->textus.mensura;
    per (i = ZEPHYRUM; i < xar_numerus(f->sententiae); i++)
    {
        t = (constans CodexSententia*)xar_obtinere(f->sententiae, i);
        si (t->initium >= a && t->initium < b)
        {
            frange;   /* prima in pagina incipiens */
        }
        si (t->initium <= a && a < t->finis)
        {
            q.initium = t->initium;   /* continens (si nulla incipit) */
            q.vestigium = chorda_sectio(f->caput->textus, t->initium,
                t->finis);
        }
    }
    si (i < xar_numerus(f->sententiae))
    {
        q.initium    = t->initium;
        q.vestigium  = chorda_sectio(f->caput->textus, t->initium,
            t->finis);
    }
    redde q;
}

/* motus: caput et pagina scribuntur, positio libri quoque */
interior b32
_paginam_ponere (
    LectorMontatio* m,
               s32  caput,
               s32  pagina,
           Piscina* p)
{
         LectorFigurae* f;
         LectorPositio  q;
                chorda  liber;
    constans character* tituli[IV];
    constans character* valores[IV];
             character  numerus_capitis[XVI];
             character  numerus_paginae[XVI];

    f      = (LectorFigurae*)m->figurae_ctx;
    liber  = chorda_transcribere(_campus(m, "liber"), p);
    si (!_caput_parare(m, liber, caput))
    {
        redde VERUM;
    }
    si (pagina < ZEPHYRUM)
    {
        pagina = ZEPHYRUM;
    }
    si ((i32)pagina >= xar_numerus(f->paginae))
    {
        pagina = (s32)xar_numerus(f->paginae) - I;
    }
    sprintf(numerus_capitis, "%d", (integer)caput);
    sprintf(numerus_paginae, "%d", (integer)pagina);
    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "pagina";
    tituli[I]          = "caput";
    valores[I]         = numerus_capitis;
    tituli[II]         = "pagina";
    valores[II]        = numerus_paginae;
    tituli[III]        = "nota";
    valores[III]       = "";
    q                  = _positio_paginae(f, pagina);
    (vacuum)lector_positionem_scribere(m->bibliotheca, liber, &q);
    redde _campos_scribere(m, tituli, valores, IV);
}

/* liber ad positionem suam; nulla -> caput 0 (et positio scribitur);
 * amissa -> initium capitis cum nota (non scribitur) */
interior b32
_librum_aperire_identitate (
    LectorMontatio* m,
            chorda  identitas,
           Piscina* p)
{
         LectorFigurae* f;
         LectorPositio  q;
                   s32  r;
                   s32  pagina;
    constans character* tituli[VI];
    constans character* valores[VI];
             character  numerus_capitis[XVI];
             character  numerus_paginae[XVI];

    f                  = (LectorFigurae*)m->figurae_ctx;
    identitas          = chorda_transcribere(identitas, p);
    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "pagina";
    tituli[I]          = "liber";
    valores[I]         = chorda_ut_cstr(identitas, p);
    tituli[II]         = "electus_liber";
    valores[II]        = valores[I];
    si (!lector_positionem_legere(m->bibliotheca, identitas, &q, p))
    {
        redde _campos_scribere(m, tituli, valores, III)
            && _paginam_ponere(m, ZEPHYRUM, ZEPHYRUM, p);
    }
    si (q.caput >= codex_numerus_capitum(lector_bibliotheca_codex(
            m->bibliotheca, lector_bibliotheca_invenire(m->bibliotheca,
            identitas), NIHIL)))
    {
        q.caput = ZEPHYRUM;
    }
    si (!_caput_parare(m, identitas, (s32)q.caput))
    {
        redde VERUM;
    }
    r       = codex_reperire(f->caput, q.initium, q.vestigium);
    pagina  = r < ZEPHYRUM ? ZEPHYRUM : (s32)lector_pagina_positionis(
        f->paginae, (i32)r);
    sprintf(numerus_capitis, "%u", (insignatus integer)q.caput);
    sprintf(numerus_paginae, "%d", (integer)pagina);
    tituli[III]   = "caput";
    valores[III]  = numerus_capitis;
    tituli[IV]    = "pagina";
    valores[IV]   = numerus_paginae;
    tituli[V]     = "nota";
    valores[V]    = r < ZEPHYRUM ? "position not found" : "";
    redde _campos_scribere(m, tituli, valores, VI);
}

/* pagina proxima (gradus I) aut prior (-I), trans capita */
interior b32
_paginam_movere (
    LectorMontatio* m,
               s32  gradus,
           Piscina* p)
{
      LectorFigurae* f;
             chorda  liber;
                s32  caput;
                s32  pagina;
                s32  capita;

    f = _caput_currens(m);
    si (!f)
    {
        redde VERUM;
    }
    liber   = chorda_transcribere(_campus(m, "liber"), p);
    caput   = f->caput_arcae;
    pagina  = _campus_numerus(m, "pagina");
    capita  = (s32)codex_numerus_capitum(f->codex);
    si (pagina < ZEPHYRUM)
    {
        pagina = ZEPHYRUM;
    }
    si (gradus > ZEPHYRUM)
    {
        si ((i32)pagina + I < xar_numerus(f->paginae))
        {
            redde _paginam_ponere(m, caput, pagina + I, p);
        }
        redde caput + I < capita ? _paginam_ponere(m, caput + I,
            ZEPHYRUM, p) : VERUM;
    }
    si (pagina > ZEPHYRUM)
    {
        redde _paginam_ponere(m, caput, pagina - I, p);
    }
    si (caput <= ZEPHYRUM || !_caput_parare(m, liber, caput - I))
    {
        redde VERUM;
    }
    redde _paginam_ponere(m, caput - I, (s32)xar_numerus(f->paginae)
        - I, p);
}

/* index sententiae quae offset continet (aut proxima post eum; aut
 * ultima); -1 si caput sine sententiis */
interior s32
_sententiae_index (
    constans LectorFigurae* f,
                       s32  offset)
{
    i32 n;
    i32 i;

    n = xar_numerus(f->sententiae);
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (offset < (s32)((constans CodexSententia*)xar_obtinere(
                f->sententiae, i))->finis)
        {
            redde (s32)i;
        }
    }
    redde n > ZEPHYRUM ? (s32)n - I : -I;
}

/* motus in visu sententiae: sententia quae offset continet capitis
 * caput; campi et positio libri scribuntur */
interior b32
_sententiam_ponere (
    LectorMontatio* m,
               s32  caput,
               s32  offset,
           Piscina* p)
{
              LectorFigurae* f;
    constans CodexSententia* t;
              LectorPositio  q;
                     chorda  liber;
                        s32  k;
         constans character* tituli[IV];
         constans character* valores[IV];
                  character  numerus_capitis[XVI];
                  character  numerus_sententiae[XVI];

    f      = (LectorFigurae*)m->figurae_ctx;
    liber  = chorda_transcribere(_campus(m, "liber"), p);
    si (!_caput_parare(m, liber, caput))
    {
        redde VERUM;
    }
    k = _sententiae_index(f, offset < ZEPHYRUM ? ZEPHYRUM : offset);
    t = k >= ZEPHYRUM ? (constans CodexSententia*)xar_obtinere(
        f->sententiae, (i32)k) : NIHIL;
    sprintf(numerus_capitis, "%d", (integer)caput);
    sprintf(numerus_sententiae, "%u", t ? (insignatus integer)t->initium
        : (insignatus integer)ZEPHYRUM);
    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "sententia";
    tituli[I]          = "caput";
    valores[I]         = numerus_capitis;
    tituli[II]         = "sententia";
    valores[II]        = numerus_sententiae;
    tituli[III]        = "nota";
    valores[III]       = "";
    si (t)
    {
        q.caput    = (i32)caput;
        q.initium  = t->initium;
        q.vestigium  = chorda_sectio(f->caput->textus, t->initium,
            t->finis);
        (vacuum)lector_positionem_scribere(m->bibliotheca, liber, &q);
    }
    redde _campos_scribere(m, tituli, valores, IV);
}

/* sententia proxima (gradus I) aut prior (-I), trans capita (capita
 * sine sententiis praetereuntur) */
interior b32
_sententiam_movere (
    LectorMontatio* m,
               s32  gradus,
           Piscina* p)
{
     LectorFigurae* f;
            chorda  liber;
               s32  caput;
               s32  capita;
               s32  k;
               s32  c;
               s32  offset;

    f = _caput_currens(m);
    si (!f)
    {
        redde VERUM;
    }
    liber   = chorda_transcribere(_campus(m, "liber"), p);
    caput   = f->caput_arcae;
    capita  = (s32)codex_numerus_capitum(f->codex);
    offset  = _campus_numerus(m, "sententia");
    k = _sententiae_index(f, offset
        < ZEPHYRUM ? ZEPHYRUM : offset);
    si (   gradus > ZEPHYRUM && k >= ZEPHYRUM
        && (i32)k + I < xar_numerus(f->sententiae))
    {
        redde _sententiam_ponere(m, caput,
            (s32)((constans CodexSententia*)
            xar_obtinere(f->sententiae, (i32)k + I))->initium, p);
    }
    si (gradus < ZEPHYRUM && k > ZEPHYRUM)
    {
        redde _sententiam_ponere(m, caput,
            (s32)((constans CodexSententia*)
            xar_obtinere(f->sententiae, (i32)k - I))->initium, p);
    }
    per (c = caput + gradus; c >= ZEPHYRUM && c < capita; c += gradus)
    {
        si (   _caput_parare(m, liber, c)
            && xar_numerus(f->sententiae) > ZEPHYRUM)
        {
            redde _sententiam_ponere(m, c, gradus > ZEPHYRUM ? ZEPHYRUM
                : (s32)((constans CodexSententia*)xar_obtinere(
                f->sententiae, xar_numerus(f->sententiae)
                    - I))->initium,
                p);
        }
    }
    redde VERUM;
}

/* s in pagina: positio libri si in pagina iacet, aliter sententia prima
 * quae in pagina incipit */
interior b32
_sententiam_ex_pagina (
    LectorMontatio* m,
           Piscina* p)
{
    LectorFigurae* f;
    LectorPositio  q;
           chorda  liber;
              s32  pagina;
              s32  offset;
              s32  r;
              i32  a;
              i32  b;

    f = _caput_currens(m);
    si (!f)
    {
        redde VERUM;
    }
    liber   = chorda_transcribere(_campus(m, "liber"), p);
    pagina  = _campus_numerus(m, "pagina");
    si (pagina < ZEPHYRUM || (i32)pagina >= xar_numerus(f->paginae))
    {
        pagina = ZEPHYRUM;
    }
    a = ((constans LectorPagina*)xar_obtinere(f->paginae, (i32)pagina))
        ->initium;
    b = (i32)pagina + I < xar_numerus(f->paginae)
        ? ((constans LectorPagina*)xar_obtinere(f->paginae, (i32)pagina
        + I))->initium : f->caput->textus.mensura;
    offset = (s32)_positio_paginae(f, pagina).initium;
    si (   lector_positionem_legere(m->bibliotheca, liber, &q, p)
        && (s32)q.caput == f->caput_arcae)
    {
        r = codex_reperire(f->caput, q.initium, q.vestigium);
        si (r >= (s32)a && r < (s32)b)
        {
            offset = r;
        }
    }
    redde _sententiam_ponere(m, f->caput_arcae, offset, p);
}

/* s et Esc in sententia: pagina quae sententiam tenet */
interior b32
_paginam_ex_sententia (
    LectorMontatio* m)
{
         LectorFigurae* f;
                   s32  offset;
    constans character* tituli[II];
    constans character* valores[II];
             character  numerus[XVI];

    f = _caput_currens(m);
    si (!f)
    {
        redde VERUM;
    }
    offset = _campus_numerus(m, "sententia");
    sprintf(numerus, "%u", (insignatus integer)lector_pagina_positionis(
        f->paginae, offset < ZEPHYRUM ? ZEPHYRUM : (i32)offset));
    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "pagina";
    tituli[I]          = "pagina";
    valores[I]         = numerus;
    redde _campos_scribere(m, tituli, valores, II);
}

/* sententia favor est si favor ullus (offset repertus) intra eam
 * iacet - iuncta favor est si pars ulla fuit */
interior b32
_favor_est (
     constans LectorFigurae* f,
    constans CodexSententia* t)
{
    s32 r;
    i32 i;

    per (i = ZEPHYRUM; f->favores && i < xar_numerus(f->favores); i++)
    {
        r = *(constans s32*)xar_obtinere(f->favores, i);
        si (r >= (s32)t->initium && r < (s32)t->finis)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* sententia currens visus sententiae; NIHIL si nulla */
interior constans CodexSententia*
_sententia_currens (
    LectorMontatio* m,
               s32* k)
{
    LectorFigurae* f;
              s32  offset;

    f = _caput_currens(m);
    si (!f)
    {
        redde NIHIL;
    }
    offset = _campus_numerus(m, "sententia");
    *k = _sententiae_index(f, offset
        < ZEPHYRUM ? ZEPHYRUM : offset);
    redde *k >= ZEPHYRUM ? (constans CodexSententia*)xar_obtinere(
        f->sententiae, (i32)*k) : NIHIL;
}

/* *: favor sententiae currentis vertitur; iuncta: partes omnes
 * tolluntur */
interior b32
_favorem_vertere (
    LectorMontatio* m,
           Piscina* p)
{
               LectorFigurae* f;
     constans CodexSententia* t;
                         Xar* lista;
                         Xar* nova;
      constans LectorPositio* q;
               LectorPositio  novus;
                      chorda  liber;
                         s32  k;
                         s32  r;
                         i32  i;
                         b32  sublatus;

    t = _sententia_currens(m, &k);
    f = (LectorFigurae*)m->figurae_ctx;
    si (!t)
    {
        redde VERUM;
    }
    liber     = chorda_transcribere(_campus(m, "liber"), p);
    lista     = lector_favores_legere(m->bibliotheca, liber, p);
    nova      = xar_creare(p, (i32)magnitudo(LectorPositio));
    sublatus  = FALSUM;
    per (i = ZEPHYRUM; lista && nova && i < xar_numerus(lista); i++)
    {
        q = (constans LectorPositio*)xar_obtinere(lista, i);
        si ((s32)q->caput == f->caput_arcae)
        {
            r = codex_reperire(f->caput, q->initium, q->vestigium);
            si (r >= (s32)t->initium && r < (s32)t->finis)
            {
                sublatus = VERUM;
                perge;
            }
        }
        *(LectorPositio*)xar_addere(nova) = *q;
    }
    si (!nova)
    {
        redde VERUM;
    }
    si (!sublatus)
    {
        novus.caput    = (i32)f->caput_arcae;
        novus.initium  = t->initium;
        novus.vestigium  = chorda_transcribere(chorda_sectio(
            f->caput->textus, t->initium, t->finis), p);
        *(LectorPositio*)xar_addere(nova) = novus;
    }
    redde lector_favores_scribere(m->bibliotheca, liber, nova);
}

/* sententiae capitis currentis solis fissuris servatis (re-ancoratis)
 * applicatis - partes quas iuncturae coniungunt; NIHIL si memoria
 * deest */
interior Xar*
_partes_fissae (
    constans LectorFigurae* f,
                       Xar* lista,
                   Piscina* p)
{
    constans LectorEmendatio* e;
              CodexEmendatio* fissurae;
                         s32  r;
                         i32  i;
                         i32  n;

    fissurae = (CodexEmendatio*)piscina_allocare(p, (memoriae_index)
        (xar_numerus(lista) + I) * magnitudo(CodexEmendatio));
    si (!fissurae)
    {
        redde NIHIL;
    }
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(lista); i++)
    {
        e = (constans LectorEmendatio*)xar_obtinere(lista, i);
        si (   e->genus      != CODEX_FINDERE
            || (s32)e->caput != f->caput_arcae)
        {
            perge;
        }
        r = codex_reperire(f->caput, e->positio, e->vestigium);
        si (r >= ZEPHYRUM)
        {
            fissurae[n].genus    = CODEX_FINDERE;
            fissurae[n].positio  = (i32)r;
            n++;
        }
    }
    redde codex_sententiae(f->caput, fissurae, n, p);
}

/* J: sententia currens cum sequente in eadem clausula iungitur.
 * Inversa: limes per fissuram servatam factus -> fissura tollitur.
 * Aliter IUNGERE ad initium partis (solis fissuris) quae ante limitem
 * finit - catena iunctarum recte prolongatur */
interior b32
_iungere (
    LectorMontatio* m,
           Piscina* p)
{
               LectorFigurae* f;
     constans CodexSententia* t;
     constans CodexSententia* sequens;
    constans LectorEmendatio* e;
                         Xar* lista;
                         Xar* nova;
                         Xar* fissae;
             LectorEmendatio  iunctura;
                      chorda  liber;
                         s32  k;
                         s32  limes;
                         i32  i;
                         i32  positio;
                         b32  sublata;

    t = _sententia_currens(m, &k);
    f = (LectorFigurae*)m->figurae_ctx;
    si (!t || (i32)k + I >= xar_numerus(f->sententiae))
    {
        redde VERUM;
    }
    sequens = (constans CodexSententia*)xar_obtinere(f->sententiae,
        (i32)k + I);
    si (sequens->clausula != t->clausula)
    {
        redde VERUM;   /* iuncturae intra clausulam solae */
    }
    limes  = (s32)sequens->initium;
    liber  = chorda_transcribere(_campus(m, "liber"), p);
    lista  = lector_emendationes_legere(m->bibliotheca, liber, p);
    nova   = xar_creare(p, (i32)magnitudo(LectorEmendatio));
    si (!lista || !nova)
    {
        redde VERUM;
    }
    /* inversa: fissura ad limitem tollitur */
    sublata = FALSUM;
    per (i = ZEPHYRUM; i < xar_numerus(lista); i++)
    {
        e = (constans LectorEmendatio*)xar_obtinere(lista, i);
        si (   !sublata && e->genus == CODEX_FINDERE
            && (s32)e->caput == f->caput_arcae
            && codex_reperire(f->caput, e->positio, e->vestigium)
                == limes)
        {
            sublata = VERUM;
            perge;
        }
        *(LectorEmendatio*)xar_addere(nova) = *e;
    }
    si (sublata)
    {
        redde lector_emendationes_scribere(m->bibliotheca, liber, nova);
    }
    /* positio iuncturae: pars (solis fissuris) quae ante limitem
     * finit */
    positio  = t->initium;
    fissae   = _partes_fissae(f, lista, p);
    per (i = ZEPHYRUM; fissae && i + I < xar_numerus(fissae); i++)
    {
        si ((s32)((constans CodexSententia*)xar_obtinere(fissae, i + I))
                ->initium == limes)
        {
            positio = ((constans CodexSententia*)xar_obtinere(fissae,
                i))
                ->initium;
            frange;
        }
    }
    iunctura.genus    = CODEX_IUNGERE;
    iunctura.caput    = (i32)f->caput_arcae;
    iunctura.positio  = positio;
    iunctura.vestigium  = chorda_transcribere(chorda_sectio(
        f->caput->textus, positio, (i32)limes), p);
    *(LectorEmendatio*)xar_addere(nova) = iunctura;
    redde lector_emendationes_scribere(m->bibliotheca, liber, nova);
}


/* ==================================================
 * Modus findendi (i): cursor per verba sententiae currentis, Enter
 * fissuram pendentem vertit, Esc servat et claudit
 * ================================================== */

interior b32
_spatium_est (
    i8 c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/* verbum ad i incipit intra sententiam t */
interior b32
_verbi_initium_est (
                      chorda  textus,
     constans CodexSententia* t,
                         i32  i)
{
    redde i >= t->initium && i < t->finis
        && !_spatium_est(textus.datum[i])
        && (i == t->initium || _spatium_est(textus.datum[i - I]));
}

/* initium verbi secundi (primi si verbum unum): ante id non finditur */
interior s32
_verbum_secundum (
                      chorda  textus,
     constans CodexSententia* t)
{
    i32 i;

    per (i = t->initium + I; i < t->finis; i++)
    {
        si (_verbi_initium_est(textus, t, i))
        {
            redde (s32)i;
        }
    }
    redde (s32)t->initium;
}

/* cursor findendi intra sententiam t; -1 si abest aut extra eam
 * (latus alterum sententiam mutavit) = modus clausus */
interior s32
_cursor_findendi (
    constans LectorMontatio* m,
    constans CodexSententia* t)
{
    s32 c;

    c = _campus_numerus(m, "cursor_findendi");
    redde (t && c >= (s32)t->initium && c < (s32)t->finis) ? c : -I;
}

/* fissurae pendentes ("31,40") intra sententiam t (initium < r <
 * finis), ordine crescente, sine duplicatis; Xar de s32 */
interior Xar*
_fissurae_pendentes (
    constans LectorMontatio* m,
    constans CodexSententia* t,
                    Piscina* p)
{
    Xar* exitus;
 chorda  a;
    s32  r;
    s32* x;
    s32  y;
    i32  i;
    i32  j;
    b32  numerus;

    exitus   = xar_creare(p, (i32)magnitudo(s32));
    a        = _campus(m, "fissurae");
    r        = ZEPHYRUM;
    numerus  = FALSUM;
    per (i = ZEPHYRUM; exitus && t && i <= a.mensura; i++)
    {
        si (i < a.mensura && a.datum[i] >= '0' && a.datum[i] <= '9')
        {
            r        = r * X + (s32)(a.datum[i] - '0');
            numerus  = VERUM;
            perge;
        }
        si (numerus && r > (s32)t->initium && r < (s32)t->finis)
        {
            /* insertio ordinata; duplicata omittuntur */
            per (j = ZEPHYRUM; j < xar_numerus(exitus); j++)
            {
                si (*(constans s32*)xar_obtinere(exitus, j) >= r)
                {
                    frange;
                }
            }
            si (   j == xar_numerus(exitus)
                || *(constans s32*)xar_obtinere(exitus, j) != r)
            {
                *(s32*)xar_addere(exitus) = r;
                per (j = xar_numerus(exitus) - I; j > ZEPHYRUM; j--)
                {
                    x = (s32*)xar_obtinere(exitus, j - I);
                    si (*x <= r)
                    {
                        frange;
                    }
                    y                               = *x;
                    *x                              = r;
                    *(s32*)xar_obtinere(exitus, j)  = y;
                }
            }
        }
        r        = ZEPHYRUM;
        numerus  = FALSUM;
    }
    redde exitus;
}

/* i: modus findendi aperitur, cursor in verbo secundo */
interior b32
_findere_incipere (
    LectorMontatio* m)
{
     constans CodexSententia* t;
               LectorFigurae* f;
                         s32  k;
          constans character* tituli[II];
          constans character* valores[II];
                   character  numerus[XVI];

    t = _sententia_currens(m, &k);
    f = (LectorFigurae*)m->figurae_ctx;
    si (!t)
    {
        redde VERUM;
    }
    sprintf(numerus, "%d", (integer)_verbum_secundum(f->caput->textus,
        t));
    tituli[ZEPHYRUM]   = "cursor_findendi";
    valores[ZEPHYRUM]  = numerus;
    tituli[I]          = "fissurae";
    valores[I]         = "";
    redde _campos_scribere(m, tituli, valores, II);
}

/* w l (gradus I) / b h (-I): initium verbi proximi aut prioris; numquam
 * ante verbum secundum nec extra sententiam */
interior b32
_cursorem_movere (
    LectorMontatio* m,
               s32  gradus)
{
     constans CodexSententia* t;
               LectorFigurae* f;
                         s32  k;
                         s32  c;
                         s32  i;
                         s32  secundum;
          constans character* tituli[I];
          constans character* valores[I];
                   character  numerus[XVI];

    t = _sententia_currens(m, &k);
    f = (LectorFigurae*)m->figurae_ctx;
    si (!t)
    {
        redde VERUM;
    }
    secundum  = _verbum_secundum(f->caput->textus, t);
    c         = _cursor_findendi(m, t);
    si (c < secundum)
    {
        c = secundum;
    }
    per (i = c + gradus; i >= secundum
        && i < (s32)t->finis; i += gradus)
    {
        si (_verbi_initium_est(f->caput->textus, t, (i32)i))
        {
            c = i;
            frange;
        }
    }
    sprintf(numerus, "%d", (integer)c);
    tituli[ZEPHYRUM]   = "cursor_findendi";
    valores[ZEPHYRUM]  = numerus;
    redde _campos_scribere(m, tituli, valores, I);
}

/* Enter: fissura pendens ad cursorem vertitur (bis = nulla) */
interior b32
_fissuram_vertere (
    LectorMontatio* m,
           Piscina* p)
{
     constans CodexSententia* t;
                         Xar* pendentes;
                         s32  k;
                         s32  c;
                         s32  r;
                         i32  i;
                         b32  sublata;
                   character* textus;
                   character* scriptum;
          constans character* tituli[I];
          constans character* valores[I];

    t = _sententia_currens(m, &k);
    c = _cursor_findendi(m, t);
    si (c <= (s32)(t ? t->initium : ZEPHYRUM))
    {
        redde VERUM;   /* nullus cursor, aut verbum unicum */
    }
    pendentes  = _fissurae_pendentes(m, t, p);
    textus     = pendentes ? (character*)piscina_allocare(p,
        (memoriae_index)(xar_numerus(pendentes) + I) * XII + I) : NIHIL;
    si (!textus)
    {
        redde VERUM;
    }
    textus[ZEPHYRUM]  = '\0';
    scriptum          = textus;
    sublata           = FALSUM;
    per (i = ZEPHYRUM; i <= xar_numerus(pendentes); i++)
    {
        si (i < xar_numerus(pendentes))
        {
            r = *(constans s32*)xar_obtinere(pendentes, i);
            si (r == c)
            {
                sublata = VERUM;
                perge;
            }
        }
        alioquin si (!sublata)
        {
            r = c;
        }
        alioquin
        {
            frange;
        }
        sprintf(scriptum, "%s%d", scriptum == textus ? "" : ",",
            (integer)r);
        scriptum += strlen(scriptum);
    }
    tituli[ZEPHYRUM]   = "fissurae";
    valores[ZEPHYRUM]  = textus;
    redde _campos_scribere(m, tituli, valores, I);
}

/* fissura pendens r servatur in lista emendationum (nova lista
 * redditur). Inversa J: si r initium partis (solis fissuris) est et
 * pars prior iuncturam servatam habet, iunctura tollitur; si r initium
 * partis sine iunctura, nihil; aliter FINDERE ad r, vestigium = pars
 * ab r */
interior Xar*
_fissuram_servare (
    constans LectorFigurae* f,
                       Xar* lista,
                       s32  r,
                   Piscina* p)
{
     constans CodexSententia* s;
    constans LectorEmendatio* e;
                         Xar* fissae;
                         Xar* nova;
             LectorEmendatio  fissura;
                         s32  prior;
                         i32  finis;
                         i32  i;
                         b32  sublata;
                         b32  limes;

    fissae = _partes_fissae(f, lista, p);
    si (!fissae)
    {
        redde lista;
    }
    prior = -I;
    limes = FALSUM;
    finis = f->caput->textus.mensura;
    per (i = ZEPHYRUM; i < xar_numerus(fissae); i++)
    {
        s = (constans CodexSententia*)xar_obtinere(fissae, i);
        si ((s32)s->initium == r)
        {
            limes = VERUM;
            prior = i > ZEPHYRUM ? (s32)((constans CodexSententia*)
                xar_obtinere(fissae, i - I))->initium : -I;
            frange;
        }
        si ((s32)s->initium < r && r < (s32)s->finis)
        {
            finis = s->finis;
            frange;
        }
    }
    si (limes)
    {
        /* limes partium: iunctura partis prioris tollitur */
        nova     = xar_creare(p, (i32)magnitudo(LectorEmendatio));
        sublata  = FALSUM;
        per (i = ZEPHYRUM; nova && i < xar_numerus(lista); i++)
        {
            e = (constans LectorEmendatio*)xar_obtinere(lista, i);
            si (   !sublata && prior >= ZEPHYRUM
                && e->genus      == CODEX_IUNGERE
                && (s32)e->caput == f->caput_arcae
                && codex_reperire(f->caput, e->positio, e->vestigium)
                    == prior)
            {
                sublata = VERUM;
                perge;
            }
            *(LectorEmendatio*)xar_addere(nova) = *e;
        }
        redde nova ? nova : lista;
    }
    fissura.genus    = CODEX_FINDERE;
    fissura.caput    = (i32)f->caput_arcae;
    fissura.positio  = (i32)r;
    fissura.vestigium  = chorda_transcribere(chorda_sectio(
        f->caput->textus, (i32)r, finis), p);
    *(LectorEmendatio*)xar_addere(lista) = fissura;
    redde lista;
}

/* Esc: fissurae pendentes servantur (ordine), modus clauditur */
interior b32
_findere_perficere (
    LectorMontatio* m,
           Piscina* p)
{
     constans CodexSententia* t;
               LectorFigurae* f;
                         Xar* pendentes;
                         Xar* lista;
                      chorda  liber;
                         s32  k;
                         i32  i;
          constans character* tituli[II];
          constans character* valores[II];

    t          = _sententia_currens(m, &k);
    f          = (LectorFigurae*)m->figurae_ctx;
    pendentes  = t ? _fissurae_pendentes(m, t, p) : NIHIL;
    si (pendentes && xar_numerus(pendentes) > ZEPHYRUM)
    {
        liber = chorda_transcribere(_campus(m, "liber"), p);
        lista = lector_emendationes_legere(m->bibliotheca, liber, p);
        per (i = ZEPHYRUM; lista && i < xar_numerus(pendentes); i++)
        {
            lista = _fissuram_servare(f, lista, *(constans s32*)
                xar_obtinere(pendentes, i), p);
        }
        si (lista)
        {
            (vacuum)lector_emendationes_scribere(m->bibliotheca, liber,
                lista);
        }
    }
    tituli[ZEPHYRUM]   = "cursor_findendi";
    valores[ZEPHYRUM]  = "";
    tituli[I]          = "fissurae";
    valores[I]         = "";
    redde _campos_scribere(m, tituli, valores, II);
}


/* ==================================================
 * Bibliotheca: ordines
 * ================================================== */

interior i32
_numerus (
    constans LectorMontatio* m)
{
    redde lector_bibliotheca_numerus(m->bibliotheca);
}

/* clavis ordinis: identitas; non legibilis: via fili */
interior chorda
_clavis_libri (
    constans LectorLiber* l)
{
    redde l->identitas.mensura > ZEPHYRUM ? l->identitas : l->via;
}

/* index electi; -1 si bibliotheca vacua */
interior s32
_electus (
    constans LectorMontatio* m)
{
    constans LectorLiber* l;
                  chorda  e;
                     i32  i;

    si (_numerus(m) == ZEPHYRUM)
    {
        redde -I;
    }
    e = _campus(m, "electus_liber");
    per (i = ZEPHYRUM; e.mensura > ZEPHYRUM && i < _numerus(m); i++)
    {
        l = lector_bibliotheca_liber(m->bibliotheca, i);
        si (chorda_aequalis(_clavis_libri(l), e))
        {
            redde (s32)i;
        }
    }
    redde ZEPHYRUM;
}

/* capacitas in cellulis: altitudo minus margines, titulus, caput */
interior s32
_capacitas (
    constans LectorMontatio* m)
{
    redde _superficies(m, FALSUM) / LECTOR_LINEA - II - LECTOR_TITULI
        - LECTOR_CAPUT;
}

/* album: res = index + 1, id "r<res>" */
interior Album*
_album (
    constans LectorMontatio* m,
                    Piscina* p)
{
        Album* l;
          i32  i;
    character  id[XVI];

    l = album_creare(p, _capacitas(m));
    per (i = ZEPHYRUM; l && i < _numerus(m); i++)
    {
        sprintf(id, "r%d", (integer)(i + I));
        (vacuum)album_addere(l, id, ALBUM_RES, LECTOR_ORDO,
            (s32)(i + I));
    }
    redde l;
}

interior s32
_pagina (
    Album* l,
      s32  electus)
{
    s32 k;

    k = electus >= ZEPHYRUM ? album_pagina_rei(l, electus + I, ZEPHYRUM)
        : ZEPHYRUM;
    redde k < ZEPHYRUM ? ZEPHYRUM : k;
}

/* id ordinis "r<n>" -> n; 0 si non ordo */
interior s32
_id_ordinis (
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

/* codex libri currentis; NIHIL si nullus */
interior Codex*
_codex_currens (
    constans LectorMontatio* m)
{
    redde lector_bibliotheca_codex(m->bibliotheca, _liber_currens(m),
        NIHIL);
}

/* album capitum: res = caput + 1, id "r<res>" */
interior Album*
_capitum_album (
    constans LectorMontatio* m,
                    Piscina* p)
{
        Album* l;
        Codex* x;
          i32  i;
    character  id[XVI];

    l = album_creare(p, _capacitas(m));
    x = _codex_currens(m);
    per (i = ZEPHYRUM; l && x && i < codex_numerus_capitum(x); i++)
    {
        sprintf(id, "r%d", (integer)(i + I));
        (vacuum)album_addere(l, id, ALBUM_RES, LECTOR_ORDO_CAPITIS,
            (s32)(i + I));
    }
    redde l;
}

/* caput electum in indice: electus_caput, aliter caput currens */
interior s32
_electum_caput (
    constans LectorMontatio* m)
{
    Codex* x;
      s32  k;

    x = _codex_currens(m);
    si (!x || codex_numerus_capitum(x) == ZEPHYRUM)
    {
        redde -I;
    }
    k = _campus_numerus(m, "electus_caput");
    si (k < ZEPHYRUM || (i32)k >= codex_numerus_capitum(x))
    {
        k = _campus_numerus(m, "caput");
    }
    redde k >= ZEPHYRUM && (i32)k < codex_numerus_capitum(x) ? k
        : ZEPHYRUM;
}

/* latitudo textus ordinis (px): regio minus spatia ordinis */
interior s32
_latitudo_ordinis (
    constans LectorMontatio* m)
{
    redde (_superficies(m, VERUM) / LECTOR_LITTERA
        - II) * LECTOR_LITTERA
        - XVI;
}

/* cursus favorum capitis k in ordines: sententiae (emendationibus
 * applicatis) quae favorem continent; consecutivae (etiam trans
 * paragraphum) ordo unus */
interior vacuum
_favores_capitis_addere (
          LectorMontatio* m,
    constans LectorLiber* l,
          constans Codex* codex,
                     s32  k,
                     Xar* ordines,
                 Piscina* p)
{
                 CodexCaput* caput;
                        Xar* sententiae;
                        Xar* offsets;
             CodexEmendatio* emendationes;
    constans CodexSententia* t;
             CodexSententia  s;
                  FavorOrdo* o;
                     chorda  vitium;
                        s32  r;
                        s32  a;
                        i32  i;
                        i32  j;
                        i32  numerus;
                        i32  amissae;
                        b32  favor;

    caput = codex_caput_legere(codex, (i32)k, p, &vitium);
    si (!caput)
    {
        redde;
    }
    amissae       = ZEPHYRUM;
    emendationes  = _emendationes_capitis(m, caput, l->identitas, k, p,
        &numerus, &amissae);
    sententiae    = codex_sententiae(caput, emendationes, numerus, p);
    offsets       = _favores_capitis(m, caput, l->identitas, k, p,
        &amissae);
    a = -I;
    per (i = ZEPHYRUM; sententiae && offsets
        && i <= xar_numerus(sententiae); i++)
    {
        favor = FALSUM;
        t     = i < xar_numerus(sententiae) ? (constans CodexSententia*)
            xar_obtinere(sententiae, i) : NIHIL;
        per (j = ZEPHYRUM; t && !favor && j < xar_numerus(offsets); j++)
        {
            r      = *(constans s32*)xar_obtinere(offsets, j);
            favor  = r >= (s32)t->initium && r < (s32)t->finis;
        }
        si (favor && a < ZEPHYRUM)
        {
            a = (s32)i;
        }
        si (favor || a < ZEPHYRUM)
        {
            perge;
        }
        /* cursus [a, i - 1] finitur */
        t           = (constans CodexSententia*)xar_obtinere(sententiae,
            (i32)a);
        s.initium   = t->initium;
        s.clausula  = t->clausula;
        s.finis     =
            ((constans CodexSententia*)xar_obtinere(sententiae,
            i - I))->finis;
        o = (FavorOrdo*)xar_addere(ordines);
        si (!o)
        {
            redde;
        }
        o->caput    = k;
        o->initium  = s.initium;
        o->textus   = caput->textus;
        o->lineae   = lector_sententiam_involvere(caput, &s,
            FONS_PARVUS,
            (i32)_latitudo_ordinis(m), p);
        a = -I;
    }
}

/* ordines favorum libri currentis (cache per liber, versio,
 * latitudinem); NIHIL si liber nullus aut non legitur */
interior Xar*
_favores_parare (
    LectorMontatio* m)
{
             LectorFigurae* f;
      constans LectorLiber* l;
                     Codex* codex;
                       Xar* lista;
                       Xar* ordines;
    constans LectorPositio* q;
                    chorda  vitium;
                       i32  k;
                       i32  i;

    f = (LectorFigurae*)m->figurae_ctx;
    l = _liber_currens(m);
    si (!l)
    {
        redde NIHIL;
    }
    si (   f->favores_parati
        && f->versio_favorum
            == lector_bibliotheca_versio(m->bibliotheca)
        && f->latitudo_favorum == _latitudo_ordinis(m)
        && chorda_aequalis(f->liber_favorum, l->identitas))
    {
        redde f->ordines_favorum;
    }
    piscina_vacare(f->arca_favorum);
    f->favores_parati = FALSUM;
    codex = lector_bibliotheca_codex(m->bibliotheca, l, &vitium);
    ordines = xar_creare(f->arca_favorum, (i32)magnitudo(FavorOrdo));
    lista    = lector_favores_legere(m->bibliotheca, l->identitas,
        f->arca_favorum);
    si (!codex || !ordines || !lista)
    {
        redde NIHIL;
    }
    /* capita ordine libri; solum quae favorem servatum habent */
    per (k = ZEPHYRUM; k < codex_numerus_capitum(codex); k++)
    {
        per (i = ZEPHYRUM; i < xar_numerus(lista); i++)
        {
            q = (constans LectorPositio*)xar_obtinere(lista, i);
            si (q->caput == k)
            {
                frange;
            }
        }
        si (i < xar_numerus(lista))
        {
            _favores_capitis_addere(m, l, codex, (s32)k, ordines,
                f->arca_favorum);
        }
    }
    f->ordines_favorum   = ordines;
    f->liber_favorum     = chorda_transcribere(l->identitas,
        f->arca_favorum);
    f->versio_favorum    = lector_bibliotheca_versio(m->bibliotheca);
    f->latitudo_favorum  = _latitudo_ordinis(m);
    f->favores_parati    = VERUM;
    redde ordines;
}

/* album favorum: titulus gregis "g<caput + 1>" ante ordines capitis,
 * ordo "r<index + 1>" altitudine linearum + I */
interior Album*
_favorum_album (
    LectorMontatio* m,
           Piscina* p)
{
                 Album* l;
                   Xar* ordines;
    constans FavorOrdo* o;
                   s32  caput;
                   i32  i;
             character  id[XVI];

    l        = album_creare(p, _capacitas(m));
    ordines  = _favores_parare(m);
    caput    = -I;
    per (i = ZEPHYRUM; l && ordines && i < xar_numerus(ordines); i++)
    {
        o = (constans FavorOrdo*)xar_obtinere(ordines, i);
        si (o->caput != caput)
        {
            caput = o->caput;
            sprintf(id, "g%d", (integer)(caput + I));
            (vacuum)album_addere(l, id, ALBUM_TITULUS,
                LECTOR_ORDO_CAPITIS, ZEPHYRUM);
        }
        sprintf(id, "r%d", (integer)(i + I));
        (vacuum)album_addere(l, id, ALBUM_RES, (s32)(o->lineae
            ? xar_numerus(o->lineae) : ZEPHYRUM) + I, (s32)(i + I));
    }
    redde l;
}

/* ordo electus favorum (electus_favor coactus); -1 si nulli */
interior s32
_electus_favor (
    LectorMontatio* m)
{
    Xar* ordines;
    s32  k;

    ordines = _favores_parare(m);
    si (!ordines || xar_numerus(ordines) == ZEPHYRUM)
    {
        redde -I;
    }
    k = _campus_numerus(m, "electus_favor");
    si (k < ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde (i32)k < xar_numerus(ordines) ? k
        : (s32)xar_numerus(ordines) - I;
}


/* ==================================================
 * Praecepta
 * ================================================== */

nomen structura {
    LectorMontatio* m;
           Piscina* p;
             Album* l;
               s32  electus;
               i32  visus;
} ContextusPraecepti;

interior b32
_requisita (
        i32  requisita,
     vacuum* ctx)
{
    ContextusPraecepti* cp;

    cp = (ContextusPraecepti*)ctx;
    redde !(requisita & REQUIRIT_ELECTUM) || cp->electus >= ZEPHYRUM;
}

interior vacuum
_contextum_parare (
        LectorMontatio* m,
               Piscina* p,
    ContextusPraecepti* cp)
{
    cp->m      = m;
    cp->p      = p;
    cp->visus  = _visus(m);
    si (cp->visus == VISUS_INDICIS)
    {
        cp->l        = _capitum_album(m, p);
        cp->electus  = _electum_caput(m);
        redde;
    }
    si (cp->visus == VISUS_FAVORUM)
    {
        cp->l        = _favorum_album(m, p);
        cp->electus  = _electus_favor(m);
        redde;
    }
    cp->l        = _album(m, p);
    cp->electus  = _electus(m);
}

interior b32
_electum_scribere (
    ContextusPraecepti* cp,
                   s32  i)
{
    constans LectorLiber* l;
      constans character* tituli[I];
      constans character* valores[I];
               character  numerus[XVI];

    si (cp->visus == VISUS_INDICIS)
    {
        si (   i < ZEPHYRUM || !_codex_currens(cp->m)
            || (i32)i >= codex_numerus_capitum(_codex_currens(cp->m)))
        {
            redde VERUM;
        }
        sprintf(numerus, "%d", (integer)i);
        tituli[ZEPHYRUM]   = "electus_caput";
        valores[ZEPHYRUM]  = numerus;
        redde _campos_scribere(cp->m, tituli, valores, I);
    }
    si (cp->visus == VISUS_FAVORUM)
    {
        /* lustratio: solum electus, positio libri non movetur */
        si (   i < ZEPHYRUM || !_favores_parare(cp->m)
            || (i32)i >= xar_numerus(_favores_parare(cp->m)))
        {
            redde VERUM;
        }
        sprintf(numerus, "%d", (integer)i);
        tituli[ZEPHYRUM]   = "electus_favor";
        valores[ZEPHYRUM]  = numerus;
        redde _campos_scribere(cp->m, tituli, valores, I);
    }
    si (i < ZEPHYRUM || (i32)i >= _numerus(cp->m))
    {
        redde VERUM;
    }
    l                  = lector_bibliotheca_liber(cp->m->bibliotheca,
        (i32)i);
    tituli[ZEPHYRUM]   = "electus_liber";
    valores[ZEPHYRUM]  = chorda_ut_cstr(_clavis_libri(l), cp->p);
    redde _campos_scribere(cp->m, tituli, valores, I);
}

interior b32
fac_proximum (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _electum_scribere(cp, cp->electus + I);
}

interior b32
fac_priorem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _electum_scribere(cp, cp->electus - I);
}

interior b32
_paginam_vertere (
    ContextusPraecepti* cp,
                   s32  gradus)
{
    s32 k;

    k = _pagina(cp->l, cp->electus) + gradus;
    si (k < ZEPHYRUM || k >= album_paginae(cp->l))
    {
        redde VERUM;
    }
    redde _electum_scribere(cp, album_res_prima(cp->l, k) - I);
}

interior b32
fac_paginam_priorem (
    vacuum* ctx)
{
    redde _paginam_vertere((ContextusPraecepti*)ctx, -I);
}

interior b32
fac_paginam_proximam (
    vacuum* ctx)
{
    redde _paginam_vertere((ContextusPraecepti*)ctx, I);
}

/* librum i aperire; non legibilis: nihil (vitium in ordine pictum) */
interior b32
_librum_aperire (
    ContextusPraecepti* cp,
                   s32  i)
{
    constans LectorLiber* l;

    l = i >= ZEPHYRUM ? lector_bibliotheca_liber(cp->m->bibliotheca,
        (i32)i) : NIHIL;
    si (!l || l->vitium.mensura > ZEPHYRUM)
    {
        redde VERUM;
    }
    redde _librum_aperire_identitate(cp->m, l->identitas, cp->p);
}

interior b32
fac_aperire (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
    constans FavorOrdo* o;
                   s32  caput;
                   s32  initium;

    si (cp->visus == VISUS_INDICIS)
    {
        redde cp->electus < ZEPHYRUM || _paginam_ponere(cp->m,
            cp->electus, ZEPHYRUM, cp->p);
    }
    si (cp->visus == VISUS_FAVORUM)
    {
        /* Enter: ad sententiam primam cursus - motus, positio
         * scribitur */
        si (cp->electus < ZEPHYRUM)
        {
            redde VERUM;
        }
        o        = (constans FavorOrdo*)xar_obtinere(_favores_parare(
            cp->m), (i32)cp->electus);
        caput    = o->caput;
        initium  = (s32)o->initium;
        redde _sententiam_ponere(cp->m, caput, initium, cp->p);
    }
    redde _librum_aperire(cp, cp->electus);
}

/* f in pagina aut sententia: index favorum, visus prior servatur */
interior b32
fac_ad_favores (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
    constans character* tituli[III];
    constans character* valores[III];

    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "favores";
    tituli[I]          = "visus_prior";
    valores[I]         = cp->visus == VISUS_SENTENTIAE ? "sententia"
        : "pagina";
    tituli[II]   = "electus_favor";
    valores[II]  = "";
    redde _campos_scribere(cp->m, tituli, valores, III);
}

/* Esc in favoribus: unde venit (caput et pagina aut sententia intacta);
 * caput absens (latus restitutum in favoribus): liber ad positionem
 * suam */
interior b32
fac_favores_relinquere (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
    constans character* tituli[II];
    constans character* valores[II];

    si (_campus_numerus(cp->m, "caput") < ZEPHYRUM)
    {
        redde _librum_aperire_identitate(cp->m, chorda_transcribere(
            _campus(cp->m, "liber"), cp->p), cp->p);
    }
    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = chorda_aequalis_literis(_campus(cp->m,
        "visus_prior"), "sententia") ? "sententia" : "pagina";
    tituli[I]   = "visus_prior";
    valores[I]  = "";
    redde _campos_scribere(cp->m, tituli, valores, II);
}

/* index: Esc ad bibliothecam (liber e semita tollitur; electus
 * manet) */
interior b32
fac_ad_bibliothecam (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
    constans character* tituli[V];
    constans character* valores[V];

    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "bibliotheca";
    tituli[I]          = "liber";
    valores[I]         = "";
    tituli[II]         = "caput";
    valores[II]        = "";
    tituli[III]        = "pagina";
    valores[III]       = "";
    tituli[IV]         = "nota";
    valores[IV]        = "";
    redde _campos_scribere(cp->m, tituli, valores, V);
}

/* pagina: t et Esc ad indicem, caput currens electum */
interior b32
fac_ad_indicem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
    constans character* tituli[II];
    constans character* valores[II];

    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "index";
    tituli[I]          = "electus_caput";
    valores[I]         = chorda_ut_cstr(_campus(cp->m, "caput"), cp->p);
    redde _campos_scribere(cp->m, tituli, valores, II);
}

interior b32
fac_paginam_libri_priorem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _paginam_movere(cp->m, -I, cp->p);
}

interior b32
fac_paginam_libri_proximam (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _paginam_movere(cp->m, I, cp->p);
}

interior b32
fac_sententiam_ex_pagina (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _sententiam_ex_pagina(cp->m, cp->p);
}

interior b32
fac_paginam_ex_sententia (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _paginam_ex_sententia(cp->m);
}

interior b32
fac_sententiam_proximam (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _sententiam_movere(cp->m, I, cp->p);
}

interior b32
fac_sententiam_priorem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _sententiam_movere(cp->m, -I, cp->p);
}

interior b32
fac_iungere (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _iungere(cp->m, cp->p);
}

interior b32
fac_favorem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _favorem_vertere(cp->m, cp->p);
}

interior b32
fac_findere (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _findere_incipere(cp->m);
}

interior b32
fac_verbum_proximum (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _cursorem_movere(cp->m, I);
}

interior b32
fac_verbum_prius (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _cursorem_movere(cp->m, -I);
}

interior b32
fac_fissuram (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _fissuram_vertere(cp->m, cp->p);
}

interior b32
fac_findere_perficere (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _findere_perficere(cp->m, cp->p);
}

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "lector.proximus", "Next book", "j", VISUS_BIBLIOTHECAE, ZEPHYRUM,
        fac_proximum },
    { "lector.prior", "Previous book", "k", VISUS_BIBLIOTHECAE,
        ZEPHYRUM, fac_priorem },
    { "lector.pagina_prior", "Previous page", "[", VISUS_BIBLIOTHECAE,
        ZEPHYRUM, fac_paginam_priorem },
    { "lector.pagina_proxima", "Next page", "]", VISUS_BIBLIOTHECAE,
        ZEPHYRUM, fac_paginam_proximam },
    { "lector.aperire", "Read", "Enter", VISUS_BIBLIOTHECAE,
        REQUIRIT_ELECTUM, fac_aperire },
    { "lector.caput_proximum", "Next chapter", "j", VISUS_INDICIS,
        ZEPHYRUM, fac_proximum },
    { "lector.caput_prius", "Previous chapter", "k", VISUS_INDICIS,
        ZEPHYRUM, fac_priorem },
    { "lector.index_prior", "Previous page", "[", VISUS_INDICIS,
        ZEPHYRUM, fac_paginam_priorem },
    { "lector.index_proximus", "Next page", "]", VISUS_INDICIS,
        ZEPHYRUM, fac_paginam_proximam },
    { "lector.caput_aperire", "Read chapter", "Enter", VISUS_INDICIS,
        REQUIRIT_ELECTUM, fac_aperire },
    { "lector.ad_bibliothecam", "Library", "Esc", VISUS_INDICIS,
        ZEPHYRUM, fac_ad_bibliothecam },
    { "lector.pagina_libri_prior", "Previous page", "[", VISUS_PAGINAE,
        ZEPHYRUM, fac_paginam_libri_priorem },
    { "lector.pagina_libri_proxima", "Next page", "]", VISUS_PAGINAE,
        ZEPHYRUM, fac_paginam_libri_proximam },
    { "lector.index", "Contents", "t", VISUS_PAGINAE, ZEPHYRUM,
        fac_ad_indicem },
    { "lector.ad_indicem", "Contents", "Esc", VISUS_PAGINAE, ZEPHYRUM,
        fac_ad_indicem },
    { "lector.ad_sententiam", "Sentence view", "s", VISUS_PAGINAE,
        ZEPHYRUM, fac_sententiam_ex_pagina },
    { "lector.sententia_proxima", "Next sentence", "j",
        VISUS_SENTENTIAE,
        ZEPHYRUM, fac_sententiam_proximam },
    { "lector.sententia_prior", "Previous sentence", "k",
        VISUS_SENTENTIAE, ZEPHYRUM, fac_sententiam_priorem },
    { "lector.ad_paginam", "Page view", "s", VISUS_SENTENTIAE, ZEPHYRUM,
        fac_paginam_ex_sententia },
    { "lector.sententia_index", "Contents", "t", VISUS_SENTENTIAE,
        ZEPHYRUM, fac_ad_indicem },
    { "lector.sententia_redire", "Page view", "Esc", VISUS_SENTENTIAE,
        ZEPHYRUM, fac_paginam_ex_sententia },
    { "lector.iungere", "Join with next sentence", "J",
        VISUS_SENTENTIAE,
        ZEPHYRUM, fac_iungere },
    { "lector.favor", "Favourite", "*", VISUS_SENTENTIAE, ZEPHYRUM,
        fac_favorem },
    { "lector.findere", "Split sentence", "i", VISUS_SENTENTIAE,
        ZEPHYRUM, fac_findere },
    { "lector.favores", "Favourites", "f", VISUS_PAGINAE
        | VISUS_SENTENTIAE, ZEPHYRUM, fac_ad_favores },
    { "lector.favor_proximus", "Next favourite", "j", VISUS_FAVORUM,
        ZEPHYRUM, fac_proximum },
    { "lector.favor_prior", "Previous favourite", "k", VISUS_FAVORUM,
        ZEPHYRUM, fac_priorem },
    { "lector.favores_prior", "Previous page", "[", VISUS_FAVORUM,
        ZEPHYRUM, fac_paginam_priorem },
    { "lector.favores_proximus", "Next page", "]", VISUS_FAVORUM,
        ZEPHYRUM, fac_paginam_proximam },
    { "lector.favor_aperire", "Go to favourite", "Enter",
        VISUS_FAVORUM, REQUIRIT_ELECTUM, fac_aperire },
    { "lector.favores_relinquere", "Back", "Esc", VISUS_FAVORUM,
        ZEPHYRUM, fac_favores_relinquere },
    { "lector.verbum_proximum", "Next word", "w", VISUS_FINDENDI,
        ZEPHYRUM, fac_verbum_proximum },
    { "lector.verbum_dextrum", "Next word", "l", VISUS_FINDENDI,
        ZEPHYRUM, fac_verbum_proximum },
    { "lector.verbum_prius", "Previous word", "b", VISUS_FINDENDI,
        ZEPHYRUM, fac_verbum_prius },
    { "lector.verbum_sinistrum", "Previous word", "h", VISUS_FINDENDI,
        ZEPHYRUM, fac_verbum_prius },
    { "lector.fissura", "Split before word", "Enter", VISUS_FINDENDI,
        ZEPHYRUM, fac_fissuram },
    { "lector.findere_perficere", "Done splitting", "Esc",
        VISUS_FINDENDI, ZEPHYRUM, fac_findere_perficere },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

interior b32
agere (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
         LectorMontatio* m;
          LectorFigurae* privatum;
    constans Praeceptum* t;
     ContextusPraecepti  cp;
                    i32  visus;
                    s32  n;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)des;
    m         = (LectorMontatio*)ctx;
    privatum  = (LectorFigurae*)m->figurae_ctx;
    si (!privatum)
    {
        redde FALSUM;
    }
    piscina_vacare(privatum->scheda);
    visus = _visus(m);
    si (   visus                                 == VISUS_SENTENTIAE
        && _campus_numerus(m, "cursor_findendi") >= ZEPHYRUM)
    {
        visus = VISUS_FINDENDI;   /* claves sententiae quiescunt */
    }
    si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        t = praecepta_ad_clavem(TABULA_PRAECEPTORUM, visus, ev);
        si (!t)
        {
            redde FALSUM;
        }
        _contextum_parare(m, privatum->scheda, &cp);
        redde praecepta_status(t, visus, _requisita, &cp)
            == PRAECEPTUM_PRAESTO && t->fn(&cp);
    }
    si (   !nodus || visus == VISUS_PAGINAE || visus == VISUS_SENTENTIAE
        || visus                == VISUS_FINDENDI
        || ev->datum.mus.botton != MUS_SINISTER
        || (   ev->genus != EVENTUS_MUS_DEPRESSUS
            && ev->genus != EVENTUS_MUS_DUPLEX))
    {
        redde FALSUM;
    }
    _contextum_parare(m, privatum->scheda, &cp);
    n = nodus->partes
        == PARTES_ITEM ? _id_ordinis(nodus->id) : ZEPHYRUM;
    si (n > ZEPHYRUM && ev->genus == EVENTUS_MUS_DUPLEX)
    {
        cp.electus = n - I;
        redde fac_aperire(&cp);
    }
    si (n > ZEPHYRUM)
    {
        redde _electum_scribere(&cp, n - I);
    }
    si (   ev->genus == EVENTUS_MUS_DEPRESSUS
        && (   chorda_aequalis_literis(nodus->id, "pagina-prior")
            || chorda_aequalis_literis(nodus->id, "pagina-proxima")))
    {
        redde _paginam_vertere(&cp, chorda_aequalis_literis(nodus->id,
            "pagina-prior") ? -I : I);
    }
    redde FALSUM;
}


/* ==================================================
 * Figura
 * ================================================== */

/* praecidere ad columnas (runae, non octeti) */
interior chorda
_praecidere (
    chorda textus,
       s32 columnae)
{
    i32 i;
    s32 runae;

    runae = ZEPHYRUM;
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si ((textus.datum[i] & 0xC0) != 0x80)
        {
            si (runae == columnae)
            {
                textus.mensura = i;
                redde textus;
            }
            runae++;
        }
    }
    redde textus;
}

interior vacuum
_textum_pingere (
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

/* ordo bibliothecae: titulus; auctor (sinistrum) et progressus
 * (dextrum) obscuri infra; non legibilis: "unreadable: <vitium>".
 * Electus: obscura in colore fundi (ut nomenclator) */
interior vacuum
_ordinem_pingere (
          LectorMontatio* m,
                 Mandata* md,
      constans Componens* c,
    constans LectorLiber* l,
                     b32  electus)
{
       chorda t;
       chorda dextrum;
          s32 columnae;
          s32 progressus;
      Punctum p;
      Punctum q;
   ColorThema obscurus;
    character textus[XVI];

    obscurus = electus ? COLOR_BACKGROUND : COLOR_TEXT_DIM;
    si (electus)
    {
        figura_ordinem_electum_pingere(md, c->fines);
    }
    alioquin
    {
        p.x = c->fines.x;
        p.y = c->fines.y + c->fines.altitudo - I;
        q.x = c->fines.x + c->fines.latitudo - I;
        q.y = p.y;
        mandata_linea(md, p, q, I, figura_color_thematis(
            COLOR_SUPERFICIES));
    }
    columnae = (c->fines.latitudo - VIII) / LECTOR_LITTERA;
    _textum_pingere(md, c->fines.x + IV, c->fines.y + IV, _praecidere(
        l->titulus, columnae), FONS_PARVUS, l->vitium.mensura > ZEPHYRUM
        ? obscurus : COLOR_TEXT);
    si (l->vitium.mensura > ZEPHYRUM)
    {
        t = chorda_concatenare(chorda_ex_literis("unreadable: ",
            md->piscina), l->vitium, md->piscina);
        _textum_pingere(md, c->fines.x + IV, c->fines.y + IV
            + LECTOR_LINEA + II, _praecidere(t, columnae), FONS_PARVUS,
            obscurus);
        redde;
    }
    progressus = lector_progressus(m->bibliotheca, l);
    si (progressus < ZEPHYRUM)
    {
        sprintf(textus, "new");
    }
    alioquin
    {
        sprintf(textus, "%d%%", (integer)progressus);
    }
    dextrum = chorda_ex_literis(textus, md->piscina);
    _textum_pingere(md, c->fines.x + c->fines.latitudo - IV
        - (s32)dextrum.mensura * LECTOR_LITTERA, c->fines.y + IV
        + LECTOR_LINEA + II, dextrum, FONS_PARVUS, obscurus);
    _textum_pingere(md, c->fines.x + IV, c->fines.y + IV + LECTOR_LINEA
        + II, _praecidere(l->auctor, columnae - (s32)dextrum.mensura
        - I), FONS_PARVUS, obscurus);
}

interior vacuum
_paginas_pingere (
               Mandata* md,
    constans Componens* f,
                   s32  k,
                   s32  n)
{
    character textus[XXIV];

    si (chorda_aequalis_literis(f->id, "index_paginae") && n > I)
    {
        sprintf(textus, "%d/%d", (integer)(k + I), (integer)n);
        _textum_pingere(md, f->fines.x + (f->fines.latitudo
            - (s32)strlen(textus) * LECTOR_LITTERA) / II,
            f->fines.y + IV, chorda_ex_literis(textus, md->piscina),
            FONS_PARVUS, COLOR_TEXT_DIM);
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
}

interior vacuum
_bibliothecam_pingere (
        LectorMontatio* m,
    constans Componens* c,
               Mandata* md)
{
        Album* l;
    Componens* f;
          s32  e;
          s32  k;
          s32  n;
          s32  r;
          i32  i;
    character  textus[XXXII];

    l = _album(m, md->piscina);
    e = _electus(m);
    k = _pagina(l, e);
    n = album_paginae(l);
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        f = componens_liberum(c, i);
        r = f->partes == PARTES_ITEM ? _id_ordinis(f->id) : ZEPHYRUM;
        si (r > ZEPHYRUM)
        {
            _ordinem_pingere(m, md, f, lector_bibliotheca_liber(
                m->bibliotheca, (i32)(r - I)), r - I == e);
        }
        alioquin si (chorda_aequalis_literis(f->id, "titulus"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + II,
                chorda_ex_literis("Library", md->piscina), FONS_TITULI,
                COLOR_TEXT);
        }
        alioquin si (chorda_aequalis_literis(f->id, "caput"))
        {
            sprintf(textus, "%u books",
                (insignatus integer)_numerus(m));
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis(textus, md->piscina), FONS_PARVUS,
                COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "nihil"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis("No books in ~/.rhubarb/codices",
                md->piscina), FONS_PARVUS, COLOR_TEXT_DIM);
        }
        alioquin
        {
            _paginas_pingere(md, f, k, n);
        }
    }
}

/* titulus capitis k pro hominibus: prooemium "Beginning" */
interior chorda
_titulus_capitis (
    constans Codex* x,
               i32  k,
           Piscina* p)
{
    chorda t;

    t = codex_caput_titulus(x, k);
    redde t.mensura > ZEPHYRUM ? t : chorda_ex_literis("Beginning", p);
}

/* index: titulus libri, numerus capitum, ordines capitum (caput
 * currens punctum dextrum) */
interior vacuum
_indicem_pingere (
        LectorMontatio* m,
    constans Componens* c,
               Mandata* md)
{
    constans LectorLiber* l;
                   Codex* x;
                   Album* a;
               Componens* f;
                     s32  e;
                     s32  k;
                     s32  n;
                     s32  r;
                     s32  currens;
                     i32  i;
                 Punctum  p;
                 Punctum  q;
               character  textus[XXXII];

    l        = _liber_currens(m);
    x        = _codex_currens(m);
    a        = _capitum_album(m, md->piscina);
    e        = _electum_caput(m);
    k        = _pagina(a, e);
    n        = album_paginae(a);
    currens  = _campus_numerus(m, "caput");
    per (i = ZEPHYRUM; l && x
        && i < componens_numerus_liberorum(c); i++)
    {
        f = componens_liberum(c, i);
        r = f->partes == PARTES_ITEM ? _id_ordinis(f->id) : ZEPHYRUM;
        si (r > ZEPHYRUM)
        {
            si (r - I == e)
            {
                figura_ordinem_electum_pingere(md, f->fines);
            }
            alioquin
            {
                p.x = f->fines.x;
                p.y = f->fines.y + f->fines.altitudo - I;
                q.x = f->fines.x + f->fines.latitudo - I;
                q.y = p.y;
                mandata_linea(md, p, q, I, figura_color_thematis(
                    COLOR_SUPERFICIES));
            }
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                _praecidere(_titulus_capitis(x, (i32)(r - I),
                md->piscina), (f->fines.latitudo - XVI)
                / LECTOR_LITTERA), FONS_PARVUS, COLOR_TEXT);
            si (r - I == currens)
            {
                _textum_pingere(md, f->fines.x + f->fines.latitudo - IV
                    - LECTOR_LITTERA, f->fines.y + IV,
                    chorda_ex_literis(
                    "\302\267", md->piscina), FONS_PARVUS, r - I == e
                    ? COLOR_BACKGROUND : COLOR_TEXT_DIM);
            }
        }
        alioquin si (chorda_aequalis_literis(f->id, "titulus"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + II,
                _praecidere(l->titulus, (f->fines.latitudo - VIII)
                / fons_latitudo(FONS_TITULI)), FONS_TITULI, COLOR_TEXT);
        }
        alioquin si (chorda_aequalis_literis(f->id, "caput"))
        {
            sprintf(textus, "%u chapters", (insignatus integer)
                codex_numerus_capitum(x));
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis(textus, md->piscina), FONS_PARVUS,
                COLOR_TEXT_DIM);
        }
        alioquin
        {
            _paginas_pingere(md, f, k, n);
        }
    }
}

/* id tituli gregis "g<n>" -> n (caput + 1); 0 si non titulus */
interior s32
_id_gregis (
    chorda id)
{
    s32 n;

    si (id.mensura < II || id.datum[ZEPHYRUM] != 'g')
    {
        redde ZEPHYRUM;
    }
    id.datum++;
    id.mensura--;
    redde chorda_ut_s32(id, &n) && n > ZEPHYRUM ? n : ZEPHYRUM;
}

/* favores: titulus libri, "N favourites", greges per caput (titulus
 * capitis obscurus), ordines = cursus involuti (6x8); nulli: "No
 * favourites yet" */
interior vacuum
_favores_pingere (
        LectorMontatio* m,
    constans Componens* c,
               Mandata* md)
{
    constans LectorLiber* l;
                   Codex* x;
                   Album* a;
                     Xar* ordines;
      constans FavorOrdo* o;
    constans LectorLinea* linea;
               Componens* f;
                     s32  e;
                     s32  k;
                     s32  n;
                     s32  r;
                     s32  g;
                     i32  i;
                     i32  j;
                 Punctum  p;
                 Punctum  q;
               character  textus[XXXII];

    l        = _liber_currens(m);
    x        = _codex_currens(m);
    ordines  = _favores_parare(m);
    a        = _favorum_album(m, md->piscina);
    e        = _electus_favor(m);
    k        = _pagina(a, e);
    n        = album_paginae(a);
    per (i = ZEPHYRUM; l && x
        && i < componens_numerus_liberorum(c); i++)
    {
        f = componens_liberum(c, i);
        r = f->partes == PARTES_ITEM ? _id_ordinis(f->id) : ZEPHYRUM;
        g = _id_gregis(f->id);
        si (r > ZEPHYRUM && ordines && (i32)r <= xar_numerus(ordines))
        {
            si (r - I == e)
            {
                figura_ordinem_electum_pingere(md, f->fines);
            }
            alioquin
            {
                p.x = f->fines.x;
                p.y = f->fines.y + f->fines.altitudo - I;
                q.x = f->fines.x + f->fines.latitudo - I;
                q.y = p.y;
                mandata_linea(md, p, q, I, figura_color_thematis(
                    COLOR_SUPERFICIES));
            }
            o = (constans FavorOrdo*)xar_obtinere(ordines, (i32)(r
                - I));
            per (j = ZEPHYRUM; o->lineae && j < xar_numerus(o->lineae);
                j++)
            {
                si (IV + ((s32)j + I) * LECTOR_LINEA
                    > f->fines.altitudo)
                {
                    frange;   /* ordo pagina altior: praeciditur */
                }
                linea = (constans LectorLinea*)xar_obtinere(o->lineae,
                    j);
                _textum_pingere(md, f->fines.x + IV, f->fines.y + IV
                    + (s32)j * LECTOR_LINEA, chorda_sectio(o->textus,
                    linea->initium, linea->finis), FONS_PARVUS,
                    COLOR_TEXT);
            }
        }
        alioquin si (g > ZEPHYRUM)
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                _praecidere(_titulus_capitis(x, (i32)(g - I),
                md->piscina), (f->fines.latitudo - VIII)
                / LECTOR_LITTERA), FONS_PARVUS, COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "titulus"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + II,
                _praecidere(l->titulus, (f->fines.latitudo - VIII)
                / fons_latitudo(FONS_TITULI)), FONS_TITULI, COLOR_TEXT);
        }
        alioquin si (chorda_aequalis_literis(f->id, "caput"))
        {
            sprintf(textus, "%u favourite%s",
                (insignatus integer)(ordines
                ? xar_numerus(ordines) : ZEPHYRUM), ordines
                && xar_numerus(ordines) == I ? "" : "s");
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis(textus, md->piscina), FONS_PARVUS,
                COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "nihil"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis("No favourites yet", md->piscina),
                FONS_PARVUS, COLOR_TEXT_DIM);
        }
        alioquin
        {
            _paginas_pingere(md, f, k, n);
        }
    }
}

/* textus in medio componentis (horizontaliter et verticaliter),
 * praecisus ad latitudinem; runae numerantur, non octeti */
interior vacuum
_centrum_pingere (
               Mandata* md,
    constans Componens* c,
                chorda  textus,
            ColorThema  color)
{
    s32 columnae;
    s32 runae;
    i32 i;

    columnae  = c->fines.latitudo / LECTOR_LITTERA - II;
    textus    = _praecidere(textus, columnae);
    runae     = ZEPHYRUM;
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si ((textus.datum[i] & 0xC0) != 0x80)
        {
            runae++;
        }
    }
    _textum_pingere(md, c->fines.x + (c->fines.latitudo - runae
        * LECTOR_LITTERA) / II, c->fines.y + (c->fines.altitudo
        - LECTOR_LINEA) / II, textus, FONS_PARVUS, color);
}

/* runae inter octetos a et b textus */
interior s32
_runas_numerare (
    chorda textus,
       i32 a,
       i32 b)
{
    s32 n;
    i32 i;

    n = ZEPHYRUM;
    per (i = a; i < b && i < textus.mensura; i++)
    {
        si ((textus.datum[i] & 0xC0) != 0x80)
        {
            n++;
        }
    }
    redde n;
}

/* modus findendi: sententia t ad fissuras pendentes in partes divisa,
 * partes linea vacua separatae, in fonte sententiae totius (quem
 * lector_sententiam_disponere eligit); grex ut ille (centratus per
 * lineam longissimam, lineae ad sinistrum, verticaliter in medio). Grex
 * nimis altus: lineae quae capiunt, *praecisa */
interior Xar*
_partes_disponere (
        constans LectorFigurae* f,
       constans CodexSententia* t,
                  constans Xar* pendentes,
                           s32  latitudo,
                           s32  altitudo,
                           b32* praecisa,
                       Piscina* piscina)
{
                  Xar* tota;
                  Xar* pars;
                  Xar* exitus;
          LectorLinea* l;
       CodexSententia  s;
                  i32  fons;
                  s32  passus;
                  s32  lata;
                  s32  x0;
                  s32  y;
                  s32  y0;
                  i32  i;
                  i32  j;
                  i32  n;

    tota = lector_sententiam_disponere(f->caput, t, (i32)latitudo,
        (i32)altitudo, praecisa, piscina);
    si (!tota || xar_numerus(tota) == ZEPHYRUM)
    {
        redde tota;
    }
    fons = ((constans LectorLinea*)xar_obtinere(tota,
        ZEPHYRUM))->fons;
    passus      = fons_altitudo(fons) + INTERVALLUM_SENTENTIAE;
    exitus      = xar_creare(piscina, (i32)magnitudo(LectorLinea));
    n           = xar_numerus(pendentes);
    lata        = ZEPHYRUM;
    y           = ZEPHYRUM;
    s.initium   = t->initium;
    s.clausula  = t->clausula;
    per (i = ZEPHYRUM; exitus && i <= n; i++)
    {
        s.finis = i < n ? (i32)*(constans s32*)xar_obtinere(pendentes,
            i)
            : t->finis;
        /* spatia ante fissuram: lineae ad verbum ultimum finiunt */
        pars = lector_sententiam_involvere(f->caput, &s, fons,
            (i32)latitudo, piscina);
        per (j = ZEPHYRUM; pars && j < xar_numerus(pars); j++)
        {
            l = (LectorLinea*)xar_addere(exitus);
            si (!l)
            {
                redde NIHIL;
            }
            *l    = *(constans LectorLinea*)xar_obtinere(pars, j);
            si (_runas_numerare(f->caput->textus, l->initium, l->finis)
                > lata)
            {
                lata = _runas_numerare(f->caput->textus, l->initium,
                    l->finis);
            }
            l->y  = y;
            y     += passus;
        }
        y         += passus;   /* linea vacua inter partes */
        s.initium  = i < n ? (i32)*(constans s32*)xar_obtinere(
            pendentes, i) : t->finis;
    }
    si (!exitus)
    {
        redde NIHIL;
    }
    x0 = (latitudo - lata * fons_latitudo(fons)) / II;
    si (x0 < ZEPHYRUM)
    {
        x0 = ZEPHYRUM;
    }
    /* altitudo gregis: ultima linea + altitudo fontis */
    y          -= II * passus;
    y          += fons_altitudo(fons);
    y0         = (altitudo - y) / II;
    *praecisa  = y > altitudo;
    si (*praecisa)
    {
        y0 = ZEPHYRUM;
        per (n = ZEPHYRUM; n < xar_numerus(exitus); n++)
        {
            si (((constans LectorLinea*)xar_obtinere(exitus, n))->y
                + fons_altitudo(fons) > altitudo)
            {
                frange;
            }
        }
        (vacuum)xar_truncare(exitus, n > ZEPHYRUM ? n : I);
    }
    per (i = ZEPHYRUM; i < xar_numerus(exitus); i++)
    {
        l     = (LectorLinea*)xar_obtinere(exitus, i);
        l->x  = x0;
        l->y  += y0;
    }
    redde exitus;
}

/* L5: locus currens notas habet (hospes; nullus = nullae)? */
interior b32
_notae_loci (
    LectorMontatio* m,
           Piscina* piscina)
{
    redde m->notas_habet && m->notas_habet(m->notas_habet_ctx,
        lector_locus(m, piscina));
}

/* sententia: sola in medio areae (lector_sententiam_disponere);
 * titulus currens supra; infra "sentence k/n", emendationes amissae,
 * "..." si praecisa */
interior vacuum
_sententiam_pingere (
        LectorMontatio* m,
    constans Componens* c,
               Mandata* md)
{
              LectorFigurae* f;
    constans CodexSententia* t;
       constans LectorLinea* l;
                  Componens* x;
                        Xar* lineae;
                     chorda  nota;
                        s32  k;
                        s32  offset;
                        s32  lateris;
                        s32  supra;
                        s32  latitudo;
                        s32  altitudo;
                        b32  praecisa;
                        s32  cursor;
                        i32  finis;
                      Fines  linea;
                        i32  i;
                        i32  j;
                  character  textus[LXIV];

    f = _caput_currens(m);
    si (!f)
    {
        redde;
    }
    offset = _campus_numerus(m, "sententia");
    k = _sententiae_index(f, offset
        < ZEPHYRUM ? ZEPHYRUM : offset);
    t       = k >= ZEPHYRUM ? (constans CodexSententia*)xar_obtinere(
        f->sententiae, (i32)k) : NIHIL;
    _margines(m, &lateris, &supra);
    _area(m, &latitudo, &altitudo);
    praecisa  = FALSUM;
    cursor    = _cursor_findendi(m, t);
    lineae    = !t ? NIHIL : cursor >= ZEPHYRUM ? _partes_disponere(f,
        t,
        _fissurae_pendentes(m, t, md->piscina), latitudo, altitudo,
        &praecisa, md->piscina) : lector_sententiam_disponere(f->caput,
        t, (i32)latitudo, (i32)altitudo, &praecisa, md->piscina);
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        x = componens_liberum(c, i);
        si (chorda_aequalis_literis(x->id, "textus"))
        {
            per (j = ZEPHYRUM; lineae && j < xar_numerus(lineae); j++)
            {
                l = (constans LectorLinea*)xar_obtinere(lineae, j);
                _textum_pingere(md, x->fines.x
                    + lateris * LECTOR_LITTERA
                    + l->x, x->fines.y + l->y, chorda_sectio(
                    f->caput->textus, l->initium, l->finis), l->fons,
                    COLOR_TEXT);
                /* cursor findendi: verbum sublineatum */
                si (   cursor >= (s32)l->initium
                    && cursor < (s32)l->finis)
                {
                    finis = (i32)cursor;
                    dum (   finis < l->finis && !_spatium_est(
                            f->caput->textus.datum[finis]))
                    {
                        finis++;
                    }
                    linea.x         = x->fines.x + lateris
                        * LECTOR_LITTERA + l->x + _runas_numerare(
                        f->caput->textus, l->initium, (i32)cursor)
                        * fons_latitudo(l->fons);
                    linea.y         = x->fines.y + l->y + fons_altitudo(
                        l->fons);
                    linea.latitudo  = _runas_numerare(f->caput->textus,
                        (i32)cursor, finis) * fons_latitudo(l->fons);
                    linea.altitudo  = I;
                    mandata_rectangulum(md, linea,
                        figura_color_thematis(COLOR_TEXT), VERUM);
                }
            }
            /* favor: stella obscura ad sinistrum lineae primae */
            si (   t && lineae && xar_numerus(lineae) > ZEPHYRUM
                && _favor_est(f, t))
            {
                l = (constans LectorLinea*)xar_obtinere(lineae,
                    ZEPHYRUM);
                _textum_pingere(md, x->fines.x
                    + lateris * LECTOR_LITTERA
                    + (l->x >= II * LECTOR_LITTERA ? l->x - II
                    * LECTOR_LITTERA : ZEPHYRUM), x->fines.y + l->y,
                    chorda_ex_literis("*", md->piscina), FONS_PARVUS,
                    COLOR_TEXT_DIM);
            }
        }
        alioquin si (chorda_aequalis_literis(x->id, "caput_paginae"))
        {
            _centrum_pingere(md, x, _titulus_capitis(f->codex,
                (i32)f->caput_arcae, md->piscina), COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(x->id, "pes"))
        {
            nota = _campus(m, "nota");
            sprintf(textus, "sentence %d/%u", (integer)(k + I),
                (insignatus integer)xar_numerus(f->sententiae));
            si (f->amissae > ZEPHYRUM)
            {
                sprintf(textus + strlen(textus), " (%u mark%s lost)",
                    (insignatus integer)f->amissae, f->amissae == I ? ""
                    : "s");
            }
            si (praecisa)
            {
                strcat(textus, " ...");
            }
            si (cursor >= ZEPHYRUM)
            {
                strcat(textus, " - splitting");
            }
            si (_notae_loci(m, md->piscina))
            {
                strcat(textus, " \302\266");   /* pilcrow: notae */
            }
            _centrum_pingere(md, x, nota.mensura > ZEPHYRUM ? nota
                : chorda_ex_literis(textus, md->piscina),
                nota.mensura > ZEPHYRUM ? COLOR_ERROR : COLOR_TEXT_DIM);
        }
    }
}

/* pagina: lineae paginae currentis in fontibus suis intra margines;
 * titulus currens in margine supero, numerus paginae (aut nota) in
 * infero */
interior vacuum
_paginam_pingere (
        LectorMontatio* m,
    constans Componens* c,
               Mandata* md)
{
            LectorFigurae* f;
    constans LectorPagina* g;
     constans LectorLinea* l;
                Componens* x;
                   chorda  nota;
                      s32  k;
                      s32  lateris;
                      s32  supra;
                      i32  i;
                      i32  j;
                character  textus[XXIV];

    f = _caput_currens(m);
    si (!f)
    {
        redde;
    }
    k = _campus_numerus(m, "pagina");
    si (k < ZEPHYRUM || (i32)k >= xar_numerus(f->paginae))
    {
        k = ZEPHYRUM;
    }
    g = (constans LectorPagina*)xar_obtinere(f->paginae, (i32)k);
    _margines(m, &lateris, &supra);
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        x = componens_liberum(c, i);
        si (chorda_aequalis_literis(x->id, "textus"))
        {
            per (j = ZEPHYRUM; j < xar_numerus(g->lineae); j++)
            {
                l = (constans LectorLinea*)xar_obtinere(g->lineae, j);
                _textum_pingere(md, x->fines.x
                    + lateris * LECTOR_LITTERA
                    + l->x, x->fines.y + l->y, chorda_sectio(
                    f->caput->textus, l->initium, l->finis), l->fons,
                    COLOR_TEXT);
            }
        }
        alioquin si (chorda_aequalis_literis(x->id, "caput_paginae"))
        {
            _centrum_pingere(md, x, _titulus_capitis(f->codex,
                (i32)f->caput_arcae, md->piscina), COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(x->id, "pes"))
        {
            nota = _campus(m, "nota");
            sprintf(textus, "%d/%u", (integer)(k + I),
                (insignatus integer)xar_numerus(f->paginae));
            si (_notae_loci(m, md->piscina))
            {
                strcat(textus, " \302\266");   /* pilcrow: notae */
            }
            _centrum_pingere(md, x, nota.mensura > ZEPHYRUM ? nota
                : chorda_ex_literis(textus, md->piscina),
                nota.mensura > ZEPHYRUM ? COLOR_ERROR : COLOR_TEXT_DIM);
        }
    }
}

interior vacuum
figura_tabulae (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
    LectorFigurae* contextus;
            Fines  mensa;

    (vacuum)thema;
    contextus       = (LectorFigurae*)ctx;
    mensa.x         = ZEPHYRUM;
    mensa.y         = ZEPHYRUM;
    mensa.latitudo  = c->fines.latitudo;
    mensa.altitudo  = c->fines.altitudo;
    figura_mensam_pingere(md, mensa, chorda_aequalis_literis(c->titulus,
        "focatum"));
    mensa.x         = LECTOR_LITTERA;
    mensa.y         = LECTOR_LINEA;
    mensa.latitudo  = c->fines.latitudo - II * LECTOR_LITTERA;
    mensa.altitudo  = c->fines.altitudo - II * LECTOR_LINEA;
    mandata_rectangulum(md, mensa, figura_color_thematis(
        COLOR_BACKGROUND), VERUM);
    si (!contextus || !contextus->m)
    {
        redde;
    }
    si (_visus(contextus->m) == VISUS_PAGINAE)
    {
        _paginam_pingere(contextus->m, c, md);
    }
    alioquin si (_visus(contextus->m) == VISUS_SENTENTIAE)
    {
        _sententiam_pingere(contextus->m, c, md);
    }
    alioquin si (_visus(contextus->m) == VISUS_INDICIS)
    {
        _indicem_pingere(contextus->m, c, md);
    }
    alioquin si (_visus(contextus->m) == VISUS_FAVORUM)
    {
        _favores_pingere(contextus->m, c, md);
    }
    alioquin
    {
        _bibliothecam_pingere(contextus->m, c, md);
    }
}

interior b32
_figuras_registrare (
    LectorMontatio* m,
               i32  latitudo,
               i32  altitudo)
{
    LectorFigurae* contextus;

    contextus = (LectorFigurae*)piscina_allocare(m->piscina,
        magnitudo(LectorFigurae));
    si (!contextus)
    {
        redde FALSUM;
    }
    contextus->m        = m;
    contextus->organum  = NIHIL;
    contextus->scheda                = piscina_generare_dynamicum(
        "lector_scheda", LXIV * M);
    contextus->latitudo_montationis = (s32)latitudo;
    contextus->altitudo_montationis = (s32)altitudo;
    contextus->arca                  = piscina_generare_dynamicum(
        "lector_arca", CCLVI * M);
    contextus->arca_favorum          = piscina_generare_dynamicum(
        "lector_favores", LXIV * M);
    contextus->caput_arcae  = -I;
    contextus->caput        = NIHIL;
    m->figurae_ctx          = contextus;
    redde contextus->scheda && contextus->arca
        && contextus->arca_favorum
        && figura_registrare(m->figurae,
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
_locum_addere (
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

interior Componens*
_componentem_locare (
              Disponens* dp,
                    s32  parens,
     constans character* id,
                 Partes  partes,
        DispositioGenus  genus_x,
                    s32  valor_x,
        DispositioGenus  genus_y,
                    s32  lineae)
{
            Componens* c;
     LocusComponentis* l;
                  s32  index;

    index  = _locum_addere(dp, parens, DISPOSITIO_LINEA, genus_x,
        valor_x, genus_y, lineae);
    c      = componens_creare(dp->piscina, dp->intern, id, partes);
    si (!c || index < ZEPHYRUM)
    {
        redde NIHIL;
    }
    componens_ponere_actio(c, actio_lectoris);
    l = (LocusComponentis*)xar_addere(dp->loci);
    si (l)
    {
        l->index  = index;
        l->c      = c;
    }
    redde c;
}

/* caput: textus crescens et paginae (sagittae, index) */
interior vacuum
_lineam_paginarum_disponere (
              Disponens* dp,
                    s32  radix_d,
     constans character* id)
{
    s32 linea;

    linea = _locum_addere(dp, radix_d, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, LECTOR_CAPUT);
    (vacuum)_componentem_locare(dp, linea, id, PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, LECTOR_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "pagina-prior",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, DISPOSITIO_FIXA,
        LECTOR_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "index_paginae",
        PARTES_INDEX, DISPOSITIO_FIXA, VII, DISPOSITIO_FIXA,
        LECTOR_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "pagina-proxima",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, DISPOSITIO_FIXA,
        LECTOR_CAPUT);
}

interior vacuum
_bibliothecam_disponere (
    LectorMontatio* m,
         Disponens* dp,
               s32  radix_d,
           Piscina* piscina)
{
                  Album* l;
     constans AlbumOrdo* o;
                    s32  regio;
                    s32  a;
                    s32  b;
                    s32  t;
                    s32  i;

    (vacuum)_componentem_locare(dp, radix_d, "titulus", PARTES_TITULUS,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, LECTOR_TITULI);
    _lineam_paginarum_disponere(dp, radix_d, "caput");
    regio = _locum_addere(dp, radix_d, DISPOSITIO_COLUMNA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    l = _album(m, piscina);
    si (!l || album_numerus(l) == ZEPHYRUM)
    {
        (vacuum)_componentem_locare(dp, regio, "nihil", PARTES_INDEX,
            DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
            LECTOR_ORDO);
        redde;
    }
    album_pagina(l, _pagina(l, _electus(m)), &a, &b, &t);
    per (i = a; i < b; i++)
    {
        o = album_ordo(l, (i32)i);
        (vacuum)_componentem_locare(dp, regio, chorda_ut_cstr(o->id,
            piscina), PARTES_ITEM, DISPOSITIO_CRESCENS, ZEPHYRUM,
            DISPOSITIO_FIXA, o->altitudo);
    }
}

/* index capitum: titulus libri, linea paginarum, ordines */
interior vacuum
_indicem_disponere (
    LectorMontatio* m,
         Disponens* dp,
               s32  radix_d,
           Piscina* piscina)
{
                  Album* l;
     constans AlbumOrdo* o;
                    s32  regio;
                    s32  a;
                    s32  b;
                    s32  t;
                    s32  i;

    (vacuum)_componentem_locare(dp, radix_d, "titulus", PARTES_TITULUS,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, LECTOR_TITULI);
    _lineam_paginarum_disponere(dp, radix_d, "caput");
    regio = _locum_addere(dp, radix_d, DISPOSITIO_COLUMNA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    l = _capitum_album(m, piscina);
    si (!l || album_numerus(l) == ZEPHYRUM)
    {
        redde;
    }
    album_pagina(l, _pagina(l, _electum_caput(m)), &a, &b, &t);
    per (i = a; i < b; i++)
    {
        o = album_ordo(l, (i32)i);
        (vacuum)_componentem_locare(dp, regio, chorda_ut_cstr(o->id,
            piscina), PARTES_ITEM, DISPOSITIO_CRESCENS, ZEPHYRUM,
            DISPOSITIO_FIXA, o->altitudo);
    }
}

/* favores: titulus libri, linea paginarum, titulus gregis iteratus
 * (pagina in medio gregis), ordines et tituli paginae */
interior vacuum
_favores_disponere (
    LectorMontatio* m,
         Disponens* dp,
               s32  radix_d,
           Piscina* piscina)
{
                  Album* l;
     constans AlbumOrdo* o;
                    s32  regio;
                    s32  a;
                    s32  b;
                    s32  t;
                    s32  i;

    (vacuum)_componentem_locare(dp, radix_d, "titulus", PARTES_TITULUS,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, LECTOR_TITULI);
    _lineam_paginarum_disponere(dp, radix_d, "caput");
    regio = _locum_addere(dp, radix_d, DISPOSITIO_COLUMNA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    l = _favorum_album(m, piscina);
    si (!l || album_numerus(l) == ZEPHYRUM)
    {
        (vacuum)_componentem_locare(dp, regio, "nihil", PARTES_INDEX,
            DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
            LECTOR_ORDO);
        redde;
    }
    album_pagina(l, _pagina(l, _electus_favor(m)), &a, &b, &t);
    si (t >= ZEPHYRUM)
    {
        /* pagina in medio gregis: titulus eius iteratur */
        o = album_ordo(l, (i32)t);
        (vacuum)_componentem_locare(dp, regio, chorda_ut_cstr(o->id,
            piscina), PARTES_INDEX, DISPOSITIO_CRESCENS, ZEPHYRUM,
            DISPOSITIO_FIXA, o->altitudo);
    }
    per (i = a; i < b; i++)
    {
        o = album_ordo(l, (i32)i);
        (vacuum)_componentem_locare(dp, regio, chorda_ut_cstr(o->id,
            piscina), o->genus
                == ALBUM_RES ? PARTES_ITEM : PARTES_INDEX,
            DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
            o->altitudo);
    }
}

/* pagina: margo superus (titulus currens), area textus (cf. _area),
 * margo inferus (numerus paginae) */
interior vacuum
_paginam_disponere (
    LectorMontatio* m,
         Disponens* dp,
               s32  radix_d)
{
    s32 lateris;
    s32 supra;

    _margines(m, &lateris, &supra);
    (vacuum)_componentem_locare(dp, radix_d, "caput_paginae",
        PARTES_INDEX, DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
        supra);
    (vacuum)_componentem_locare(dp, radix_d, "textus", PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    (vacuum)_componentem_locare(dp, radix_d, "pes", PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, supra);
}

Componens*
lector_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
     LectorMontatio* m;
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
    m = (LectorMontatio*)ctx;
    si (!m)
    {
        redde NIHIL;
    }
    latitudo = _superficies(m, VERUM);
    altitudo = _superficies(m, FALSUM);
    radix     = componens_creare(piscina, intern, "lector",
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
    componens_ponere_actio(radix, actio_lectoris);
    si (motus_ramus_focatus(motus, m->ramus))
    {
        componens_ponere_titulum(radix, "focatum");
    }
    columnae    = latitudo / LECTOR_LITTERA;
    lineae      = altitudo / LECTOR_LINEA;
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
    si (_visus(m) == VISUS_PAGINAE || _visus(m) == VISUS_SENTENTIAE)
    {
        _paginam_disponere(m, &dp, radix_d);
    }
    alioquin si (_visus(m) == VISUS_INDICIS)
    {
        _indicem_disponere(m, &dp, radix_d, piscina);
    }
    alioquin si (_visus(m) == VISUS_FAVORUM)
    {
        _favores_disponere(m, &dp, radix_d, piscina);
    }
    alioquin
    {
        _bibliothecam_disponere(m, &dp, radix_d, piscina);
    }
    dispositio_computare(dp.d, columnae, lineae, NIHIL, NIHIL);
    per (i = ZEPHYRUM; i < (s32)xar_numerus(dp.loci); i++)
    {
        LocusComponentis* loc;

        loc = (LocusComponentis*)xar_obtinere(dp.loci, (i32)i);
        componens_ponere_fines(loc->c, dispositio_ad_pixela(
            dispositio_fines(dp.d, loc->index), columnae, lineae,
            latitudo, altitudo, LECTOR_LITTERA, LECTOR_LINEA));
        componens_addere_liberum(radix, loc->c);
    }
    redde radix;
}


/* ==================================================
 * Montatio
 * ================================================== */

b32
lector_montare (
         LectorMontatio* m,
                Piscina* piscina,
    InternamentumChorda* intern,
     InsulaRepositorium* repo,
     constans character* id,
      LectorBibliotheca* bibliotheca,
                    i32  latitudo,
                    i32  altitudo,
        StatusSystemata* systemata)
{
    Organum* organum;

    si (!m || !piscina || !intern || !repo)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(LectorMontatio));
    m->piscina      = piscina;
    m->intern       = intern;
    m->bibliotheca  = bibliotheca;
    m->ramus        = id ? insula_ramus(repo, "lector", id)
                         : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "lector", "lector");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"lector\"", id ? id : "lector", piscina,
               intern))
    {
        redde FALSUM;
    }
    m->actiones  = actio_registrum_creare(piscina, intern);
    m->figurae   = figura_registrum_creare(piscina);
    si (   !m->actiones || !m->figurae
        || !praecepta_verificare(TABULA_PRAECEPTORUM)
        || !actio_registrare(m->actiones, actio_lectoris, agere, m)
        || !_figuras_registrare(m, latitudo, altitudo))
    {
        redde FALSUM;
    }
    ((LectorFigurae*)m->figurae_ctx)->organum = organum;
    redde VERUM;
}


/* ==================================================
 * Semitae et locus
 * ================================================== */

/* SemitaOrganum lateris: manifestum suum; ctx = LectorMontatio* */
interior constans Organum*
_organa (
    vacuum* ctx,
    chorda  genus)
{
    LectorMontatio* m = (LectorMontatio*)ctx;

    si (   !m || !m->figurae_ctx
        || !chorda_aequalis_literis(genus, "lector"))
    {
        redde NIHIL;
    }
    redde ((LectorFigurae*)m->figurae_ctx)->organum;
}

b32
lector_argumentum_ponere (
         LectorMontatio* m,
     constans character* argumentum)
{
             StmlNodus* semita;
                chorda* liber;
                chorda  vitium;
                   b32  caput_nominatum;
    constans character* tituli[III];
    constans character* valores[III];

    si (!m || !m->figurae_ctx)
    {
        redde FALSUM;
    }
    /* URI aut cognomen manifesti ("" bibliotheca, identitas nuda) */
    semita = semita_ex_argumento(_organa, m, "lector", argumentum
        ? argumentum : "", m->piscina, m->intern, &vitium);
    si (!semita)
    {
        redde FALSUM;
    }
    /* liber ignotus: recusatur ante scripturam */
    liber = stml_attributum_capere(semita, "liber");
    si (   liber && liber->mensura > ZEPHYRUM
        && !lector_bibliotheca_invenire(m->bibliotheca, *liber))
    {
        redde FALSUM;
    }
    caput_nominatum = stml_attributum_capere(semita, "caput") != NIHIL;
    si (!semita_applicare(_organa, m, &m->ramus, semita, m->piscina,
            &vitium))
    {
        redde FALSUM;
    }
    /* semita sine capite (identitas nuda, liber idem): caput et pagina
     * lateris ligatione mutata non deleta sunt - tollere, ut positio
     * LIBRI vincat (non pagina vetus lateris, quae eam rescriberet) */
    si (!caput_nominatum)
    {
        tituli[ZEPHYRUM]   = "caput";
        valores[ZEPHYRUM]  = "";
        tituli[I]          = "pagina";
        valores[I]         = "";
        tituli[II]         = "sententia";
        valores[II]        = "";
        si (!_campos_scribere(m, tituli, valores, III))
        {
            redde FALSUM;
        }
    }
    redde lector_semitam_secuta(m);
}

b32
lector_semitam_secuta (
    vacuum* ctx)
{
    LectorMontatio* m;
     LectorFigurae* f;
               s32  caput;
               s32  pagina;

    m = (LectorMontatio*)ctx;
    f = m ? (LectorFigurae*)m->figurae_ctx : NIHIL;
    si (   !f || !_liber_currens(m)
        || (_visus(m) != VISUS_PAGINAE
            && _visus(m) != VISUS_SENTENTIAE))
    {
        redde VERUM;
    }
    piscina_vacare(f->scheda);
    caput   = _campus_numerus(m, "caput");
    pagina  = _campus_numerus(m, "pagina");
    si (_visus(m) == VISUS_SENTENTIAE)
    {
        si (caput >= ZEPHYRUM)
        {
            redde _sententiam_ponere(m, caput, _campus_numerus(m,
                "sententia"), f->scheda);
        }
        redde _librum_aperire_identitate(m, _campus(m, "liber"),
                   f->scheda)
            && _sententiam_ex_pagina(m, f->scheda);
    }
    /* caput nominatum = motus; non nominatum (restitutio, identitas
     * nuda) = positio libri - ultima mutatio vincit */
    si (caput >= ZEPHYRUM)
    {
        redde _paginam_ponere(m, caput, pagina >= ZEPHYRUM ? pagina
            : ZEPHYRUM, f->scheda);
    }
    redde _librum_aperire_identitate(m, _campus(m, "liber"), f->scheda);
}

chorda
lector_locus (
     vacuum* ctx,
    Piscina* piscina)
{
     LectorMontatio* m;
          StmlNodus* semita;
             chorda  vacua;

    m              = (LectorMontatio*)ctx;
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (!m || !piscina)
    {
        redde vacua;
    }
    semita = semita_legere(_organa, m, &m->ramus, "lector", piscina,
        m->intern);
    si (!semita)
    {
        redde vacua;
    }
    /* L5: locus per visum (comites, notae) - pagina = liber, caput,
     * pagina; sententia = liber, caput, sententia; ceteri liber solus:
     * campi status ex visu priore relicti locum non mutant */
    si (_visus(m) != VISUS_PAGINAE)
    {
        (vacuum)insula_attributum_tollere(semita, "pagina");
    }
    si (_visus(m) != VISUS_SENTENTIAE)
    {
        (vacuum)insula_attributum_tollere(semita, "sententia");
    }
    si (_visus(m) != VISUS_PAGINAE && _visus(m) != VISUS_SENTENTIAE)
    {
        (vacuum)insula_attributum_tollere(semita, "caput");
    }
    redde semita_ad_uri(_organa, m, semita, piscina);
}

chorda
lector_locus_titulus (
     vacuum* ctx,
    Piscina* piscina)
{
          LectorMontatio* m;
           LectorFigurae* f;
    constans LectorLiber* l;
                     s32  k;
               character  numeri[XXIV];

    m = (LectorMontatio*)ctx;
    l = m ? _liber_currens(m) : NIHIL;
    si (!l || _visus(m) == VISUS_BIBLIOTHECAE)
    {
        redde chorda_ex_literis("Library", piscina);
    }
    si (_visus(m) == VISUS_FAVORUM)
    {
        redde chorda_concatenare(l->titulus, chorda_ex_literis(
            " - Favourites", piscina), piscina);
    }
    f = _visus(m) == VISUS_PAGINAE || _visus(m) == VISUS_SENTENTIAE
        ? _caput_currens(m) : NIHIL;
    si (!f)
    {
        redde chorda_transcribere(l->titulus, piscina);
    }
    si (_visus(m) == VISUS_SENTENTIAE)
    {
        k = _campus_numerus(m, "sententia");
        sprintf(numeri, " - sentence %d/%u",
            (integer)(_sententiae_index(f,
            k < ZEPHYRUM ? ZEPHYRUM : k) + I),
            (insignatus integer)xar_numerus(f->sententiae));
    }
    alioquin
    {
        k = _campus_numerus(m, "pagina");
        si (k < ZEPHYRUM)
        {
            k = ZEPHYRUM;
        }
        sprintf(numeri, " - %d/%u", (integer)(k + I),
            (insignatus integer)xar_numerus(f->paginae));
    }
    redde chorda_concatenare(chorda_concatenare(l->titulus,
        chorda_concatenare(chorda_ex_literis(" - ", piscina),
        _titulus_capitis(f->codex, (i32)f->caput_arcae, piscina),
        piscina), piscina), chorda_ex_literis(numeri, piscina),
        piscina);
}
