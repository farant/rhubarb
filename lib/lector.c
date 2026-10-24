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
#define LECTOR_PES       II       /* lineae pedis paginae */

#define VISUS_BIBLIOTHECAE  I
#define VISUS_INDICIS       II
#define VISUS_PAGINAE       IV
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
                  Xar* sententiae;      /* CodexSententia */
                  Xar* paginae;         /* LectorPagina */
} LectorFigurae;


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
    si (chorda_aequalis_literis(v, "index"))
    {
        redde VISUS_INDICIS;
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

/* area textus paginae (px): superficies minus margines (cellula
 * utrimque), pes, et IV px intra - eadem in dispositione et pictura */
interior vacuum
_area (
    constans LectorMontatio* m,
                        s32* latitudo,
                        s32* altitudo)
{
    *latitudo  = (_superficies(m, VERUM) / LECTOR_LITTERA - II)
        * LECTOR_LITTERA - VIII;
    *altitudo  = (_superficies(m, FALSUM) / LECTOR_LINEA - II
        - LECTOR_PES) * LECTOR_LINEA - VIII;
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
                  chorda  vitium;
                     s32  latitudo;
                     s32  altitudo;

    f = (LectorFigurae*)m->figurae_ctx;
    _area(m, &latitudo, &altitudo);
    si (   f->caput && f->caput_arcae == k
        && f->latitudo_arcae == latitudo
        && f->altitudo_arcae == altitudo
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
    f->sententiae      = codex_sententiae(f->caput, NIHIL, ZEPHYRUM,
        f->arca);
    f->paginae         = lector_paginare(f->caput, (i32)latitudo,
        (i32)altitudo, f->arca);
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

    si (cp->visus == VISUS_INDICIS)
    {
        redde cp->electus < ZEPHYRUM || _paginam_ponere(cp->m,
            cp->electus, ZEPHYRUM, cp->p);
    }
    redde _librum_aperire(cp, cp->electus);
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
    si (   !nodus || visus == VISUS_PAGINAE
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
 * (dextrum) obscuri infra; non legibilis: "unreadable: <vitium>" */
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
    character textus[XVI];

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
        ? COLOR_TEXT_DIM : COLOR_TEXT);
    si (l->vitium.mensura > ZEPHYRUM)
    {
        t = chorda_concatenare(chorda_ex_literis("unreadable: ",
            md->piscina), l->vitium, md->piscina);
        _textum_pingere(md, c->fines.x + IV, c->fines.y + IV
            + LECTOR_LINEA + II, _praecidere(t, columnae), FONS_PARVUS,
            COLOR_TEXT_DIM);
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
        + LECTOR_LINEA + II, dextrum, FONS_PARVUS, COLOR_TEXT_DIM);
    _textum_pingere(md, c->fines.x + IV, c->fines.y + IV + LECTOR_LINEA
        + II, _praecidere(l->auctor, columnae - (s32)dextrum.mensura
        - I), FONS_PARVUS, COLOR_TEXT_DIM);
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
                    "\302\267", md->piscina), FONS_PARVUS,
                    COLOR_TEXT_DIM);
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

/* pagina: lineae paginae currentis in fontibus suis; pes: titulus
 * capitis (aut nota) et "k/n" */
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
                   chorda  sinistrum;
                   chorda  dextrum;
                      s32  k;
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
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        x = componens_liberum(c, i);
        si (chorda_aequalis_literis(x->id, "textus"))
        {
            per (j = ZEPHYRUM; j < xar_numerus(g->lineae); j++)
            {
                l = (constans LectorLinea*)xar_obtinere(g->lineae, j);
                _textum_pingere(md, x->fines.x + IV + l->x, x->fines.y
                    + IV + l->y, chorda_sectio(f->caput->textus,
                    l->initium, l->finis), l->fons, COLOR_TEXT);
            }
        }
        alioquin si (chorda_aequalis_literis(x->id, "pes"))
        {
            sprintf(textus, "%d/%u", (integer)(k + I),
                (insignatus integer)xar_numerus(f->paginae));
            dextrum = chorda_ex_literis(textus, md->piscina);
            _textum_pingere(md, x->fines.x + x->fines.latitudo - IV
                - (s32)dextrum.mensura * LECTOR_LITTERA, x->fines.y
                + IV,
                dextrum, FONS_PARVUS, COLOR_TEXT_DIM);
            nota       = _campus(m, "nota");
            sinistrum  = nota.mensura > ZEPHYRUM ? nota
                : _titulus_capitis(f->codex, (i32)f->caput_arcae,
                md->piscina);
            _textum_pingere(md, x->fines.x + IV, x->fines.y + IV,
                _praecidere(sinistrum, (x->fines.latitudo - VIII)
                / LECTOR_LITTERA - (s32)dextrum.mensura - I),
                FONS_PARVUS, nota.mensura > ZEPHYRUM ? COLOR_ERROR
                : COLOR_TEXT_DIM);
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
    alioquin si (_visus(contextus->m) == VISUS_INDICIS)
    {
        _indicem_pingere(contextus->m, c, md);
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
    contextus->caput_arcae  = -I;
    contextus->caput        = NIHIL;
    m->figurae_ctx          = contextus;
    redde contextus->scheda && contextus->arca
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

/* pagina: area textus (cf. _area) et pes */
interior vacuum
_paginam_disponere (
    Disponens* dp,
          s32  radix_d)
{
    (vacuum)_componentem_locare(dp, radix_d, "textus", PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    (vacuum)_componentem_locare(dp, radix_d, "pes", PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, LECTOR_PES);
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
    si (_visus(m) == VISUS_PAGINAE)
    {
        _paginam_disponere(&dp, radix_d);
    }
    alioquin si (_visus(m) == VISUS_INDICIS)
    {
        _indicem_disponere(m, &dp, radix_d, piscina);
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
    constans character* tituli[II];
    constans character* valores[II];

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
        si (!_campos_scribere(m, tituli, valores, II))
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
    si (!f || _visus(m) != VISUS_PAGINAE || !_liber_currens(m))
    {
        redde VERUM;
    }
    piscina_vacare(f->scheda);
    caput   = _campus_numerus(m, "caput");
    pagina  = _campus_numerus(m, "pagina");
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
    redde semita ? semita_ad_uri(_organa, m, semita, piscina) : vacua;
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
    f = _visus(m) == VISUS_PAGINAE ? _caput_currens(m) : NIHIL;
    si (!f)
    {
        redde chorda_transcribere(l->titulus, piscina);
    }
    k = _campus_numerus(m, "pagina");
    si (k < ZEPHYRUM)
    {
        k = ZEPHYRUM;
    }
    sprintf(numeri, " - %d/%u", (integer)(k + I),
        (insignatus integer)xar_numerus(f->paginae));
    redde chorda_concatenare(chorda_concatenare(l->titulus,
        chorda_concatenare(chorda_ex_literis(" - ", piscina),
        _titulus_capitis(f->codex, (i32)f->caput_arcae, piscina),
        piscina), piscina), chorda_ex_literis(numeri, piscina),
        piscina);
}
