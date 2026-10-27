/* navigator.c - navigator in latere vici (include/navigator.h). Forma
 * lectoris (lib/lector.c): arbor dispositionis in cellulis (LITTERA x
 * LINEA), figura radicis omnia pingit, praecepta per
 * membrum__praecepta, index per membrum__album (titulus gregis =
 * hospes). Articulus in arca: HTML versionis -> adaptator situs ->
 * navigator_paginae -> lector_paginare, per (articulus, versio,
 * area). */

#include "navigator.h"
#include "membrum__album.h"
#include "membrum__praecepta.h"
#include "dispositio.h"
#include "fons.h"
#include "thema.h"
#include "motus.h"
#include "lector_paginae.h"
#include "navigator_paginae.h"
#include "vicipaedia.h"
#include "semita.h"

#include <stdio.h>
#include <string.h>

#define NAV_LATITUDO  CCC      /* superficies absens */
#define NAV_ALTITUDO  CCLVI
#define NAV_LITTERA   VI
#define NAV_LINEA     VIII
#define NAV_TITULI    III      /* lineae tituli */
#define NAV_CAPUT     II       /* lineae capitis (paginae indicis) */
#define NAV_ORDO      II       /* lineae ordinis et tituli gregis */

#define VISUS_INDICIS  I
#define VISUS_PAGINAE  II
#define REQUIRIT_ELECTUM  I

hic_manens constans character actio_navigatoris[] = "navigator.agere";

/* figura: contextus privatus */
nomen structura {
    NavigatorMontatio* m;
     constans Organum* organum;
              Piscina* scheda;
                  s32  latitudo_montationis;
                  s32  altitudo_montationis;
    /* cache articuli currentis: (articulus, versio, area) -> textus,
     * paginae (arca vacatur cum mutantur) */
              Piscina* arca;
               chorda  articulus_arcae;
               chorda  versio_arcae;
                  s32  latitudo_arcae;
                  s32  altitudo_arcae;
      NavigatorTextus* textus;
                  Xar* paginae;          /* LectorPagina: articulus,
                                          * deinde index nexuum */
                  i32 paginae_articuli; /* priores ex caput.textus */
               chorda titulus;          /* titulus versionis */
               chorda hospes;
} NavigatorFigurae;


/* ==================================================
 * Status (insula)
 * ================================================== */

interior chorda
_vacua (vacuum)
{
    chorda v;

    v.datum    = NIHIL;
    v.mensura  = ZEPHYRUM;
    redde v;
}

interior chorda
_campus (
    constans NavigatorMontatio* m,
            constans character* titulus)
{
    chorda* a;

    a = insula_ramus_attributum(&m->ramus, titulus);
    redde a ? *a : _vacua();
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

/* campi actu uno, scriptore navigatoris ("" aut NIHIL tollit) */
interior b32
_campos_scribere (
             NavigatorMontatio* m,
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
        actio_navigatoris, m->piscina));
    ok = mutare_ramum(&m->ramus, _campos_ponere, &grex);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

interior s32
_superficies (
    constans NavigatorMontatio* m,
                           b32  latitudo)
{
    constans NavigatorFigurae* f;
                          s32  v;
                       chorda  a;
                          s32  n;

    f = (constans NavigatorFigurae*)m->figurae_ctx;
    v = !f ? ZEPHYRUM : latitudo ? f->latitudo_montationis
                                 : f->altitudo_montationis;
    si (v <= ZEPHYRUM)
    {
        v = latitudo ? NAV_LATITUDO : NAV_ALTITUDO;
    }
    a = _campus(m, latitudo ? "superficies_latitudo"
        : "superficies_altitudo");
    redde (a.mensura > ZEPHYRUM && chorda_ut_s32(a, &n) && n > ZEPHYRUM)
        ? n : v;
}

interior i32
_visus (
    constans NavigatorMontatio* m)
{
    redde chorda_aequalis_literis(_campus(m, "visus"), "pagina")
        ? VISUS_PAGINAE : VISUS_INDICIS;
}

/* campus numerus (>= 0); -1 si abest aut malus */
interior s32
_campus_numerus (
    constans NavigatorMontatio* m,
            constans character* titulus)
{
    chorda a;
       s32 n;

    a = _campus(m, titulus);
    redde (a.mensura > ZEPHYRUM && chorda_ut_s32(a, &n)
        && n >= ZEPHYRUM) ? n : -I;
}

/* margines paginae ut lector (cellulae): lateris X, supra et infra VI;
 * in latere angusto minuuntur (lateris et supra saltem I) */
interior vacuum
_margines (
    constans NavigatorMontatio* m,
                           s32* lateris,
                           s32* supra)
{
    s32 columnae;
    s32 lineae;

    columnae  = _superficies(m, VERUM) / NAV_LITTERA - II;
    lineae    = _superficies(m, FALSUM) / NAV_LINEA - II;
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

/* area textus paginae (px) - eadem in dispositione et pictura */
interior vacuum
_area (
    constans NavigatorMontatio* m,
                           s32* latitudo,
                           s32* altitudo)
{
    s32 lateris;
    s32 supra;

    _margines(m, &lateris, &supra);
    *latitudo  = (_superficies(m, VERUM) / NAV_LITTERA - II
        - II * lateris) * NAV_LITTERA;
    *altitudo  = (_superficies(m, FALSUM) / NAV_LINEA - II
        - II * supra) * NAV_LINEA;
    si (*latitudo < NAV_LITTERA)
    {
        *latitudo = NAV_LITTERA;
    }
    si (*altitudo < NAV_LINEA)
    {
        *altitudo = NAV_LINEA;
    }
}


/* ==================================================
 * Articuli
 * ================================================== */

interior constans NavigatorArticulus*
_articulus_per_id (
    constans NavigatorMontatio* m,
                        chorda  identitas)
{
    i32 i;

    per (i = ZEPHYRUM; identitas.mensura > ZEPHYRUM
        && i < navigator_articulorum_numerus(m->bibliotheca); i++)
    {
        constans NavigatorArticulus* a = navigator_articulus(
            m->bibliotheca, i);

        si (chorda_aequalis(a->identitas, identitas))
        {
            redde a;
        }
    }
    redde NIHIL;
}

/* versio per identitatem; vacua = ultima; NIHIL si ignota */
interior constans NavigatorVersio*
_versio (
    constans NavigatorArticulus* a,
                         chorda  identitas)
{
    i32 i;
    i32 n;

    n = a ? xar_numerus(a->versiones) : ZEPHYRUM;
    si (n == ZEPHYRUM)
    {
        redde NIHIL;
    }
    si (identitas.mensura == ZEPHYRUM)
    {
        redde (constans NavigatorVersio*)xar_obtinere(a->versiones,
            n - I);
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans NavigatorVersio* v = (constans NavigatorVersio*)
            xar_obtinere(a->versiones, i);

        si (chorda_aequalis(v->identitas, identitas))
        {
            redde v;
        }
    }
    redde NIHIL;
}

/* chorda super litteras constantes (sine copia) */
interior chorda
_litterae (
    constans character* s)
{
    chorda c;

    c.datum    = (i8*)(vacuum*)(memoriae_index)s;
    c.mensura  = (i32)strlen(s);
    redde c;
}

/* hospes URL ("https://en.wikipedia.org/wiki/X": "en.wikipedia.org") */
interior chorda
_hospes (
    chorda url)
{
    s32 a;
    s32 b;

    a = chorda_invenire_index(url, _litterae("://"));
    si (a < ZEPHYRUM)
    {
        redde url;
    }
    url  = chorda_sectio(url, (i32)a + III, url.mensura);
    b    = chorda_invenire_index(url, _litterae("/"));
    redde b < ZEPHYRUM ? url : chorda_sectio(url, ZEPHYRUM, (i32)b);
}

/* titulus articuli: versionis ultimae; vacuus -> URL */
interior chorda
_titulus_articuli (
    constans NavigatorArticulus* a)
{
    constans NavigatorVersio* v = _versio(a, _vacua());

    redde v && v->titulus.mensura > ZEPHYRUM ? v->titulus : a->url;
}

/* adaptator situs (v1: Vicipaedia sola) */
interior Articulus*
_extrahere (
                         chorda  situs,
                         chorda  html,
    constans NavigatorArticulus* a,
                        Piscina* p)
{
    si (chorda_aequalis_literis(situs, "vicipaedia"))
    {
        redde vicipaedia_articulus(html, chorda_ut_cstr(a->url, p), p);
    }
    redde NIHIL;
}

/* articulus currens paginatus (campi articulus, versio) in arca */
interior NavigatorFigurae*
_paginas_parare (
    NavigatorMontatio* m)
{
               NavigatorFigurae* f;
    constans NavigatorArticulus* a;
       constans NavigatorVersio* v;
                         chorda  html;
                      Articulus* articulus;
                            s32  latitudo;
                            s32  altitudo;

    f = (NavigatorFigurae*)m->figurae_ctx;
    a = _articulus_per_id(m, _campus(m, "articulus"));
    v = _versio(a, _campus(m, "versio"));
    si (!f || !a || !v)
    {
        redde NIHIL;
    }
    _area(m, &latitudo, &altitudo);
    si (   f->textus && f->latitudo_arcae == latitudo
        && f->altitudo_arcae == altitudo
        && chorda_aequalis(f->articulus_arcae, a->identitas)
        && chorda_aequalis(f->versio_arcae, v->identitas))
    {
        redde f;
    }
    piscina_vacare(f->arca);
    f->textus   = NIHIL;
    f->paginae  = NIHIL;
    html        = navigator_versionis_html(m->bibliotheca, v, f->arca);
    articulus = html.mensura > ZEPHYRUM ? _extrahere(a->situs, html, a,
        f->arca) : NIHIL;
    f->textus = articulus ? navigator_textum_facere(articulus, f->arca)
        : NIHIL;
    f->paginae = f->textus ? lector_paginare(&f->textus->caput,
        (i32)latitudo, (i32)altitudo, f->arca) : NIHIL;
    si (!f->paginae || xar_numerus(f->paginae) == ZEPHYRUM)
    {
        f->textus = NIHIL;
        redde NIHIL;
    }
    /* index nexuum (N3.1b): paginae novae post articulum */
    f->paginae_articuli = xar_numerus(f->paginae);
    si (f->textus->nexus.textus.mensura > ZEPHYRUM)
    {
        Xar* nexus = lector_paginare(&f->textus->nexus, (i32)latitudo,
            (i32)altitudo, f->arca);
        i32 i;

        per (i = ZEPHYRUM; nexus && i < xar_numerus(nexus); i++)
        {
            LectorPagina* g = (LectorPagina*)xar_addere(f->paginae);

            si (g)
            {
                *g = *(LectorPagina*)xar_obtinere(nexus, i);
            }
        }
    }
    f->articulus_arcae  = chorda_transcribere(a->identitas, f->arca);
    f->versio_arcae     = chorda_transcribere(v->identitas, f->arca);
    f->titulus         = chorda_transcribere(v->titulus.mensura
        > ZEPHYRUM ? v->titulus : a->url, f->arca);
    f->hospes          = chorda_transcribere(_hospes(a->url), f->arca);
    f->latitudo_arcae  = latitudo;
    f->altitudo_arcae  = altitudo;
    redde f;
}

/* pagina currens coacta ad [0, n-1] */
interior s32
_pagina_currens (
      constans NavigatorMontatio* m,
       constans NavigatorFigurae* f)
{
    s32 k;

    k = _campus_numerus(m, "pagina");
    si (k < ZEPHYRUM)
    {
        k = ZEPHYRUM;
    }
    si ((i32)k >= xar_numerus(f->paginae))
    {
        k = (s32)xar_numerus(f->paginae) - I;
    }
    redde k;
}


/* ==================================================
 * Index (ordo, album, electus)
 * ================================================== */

/* a ante b: hospes, deinde titulus (sine casu ASCII) */
interior b32
_praecedit (
    constans NavigatorArticulus* a,
    constans NavigatorArticulus* b)
{
    chorda x;
    chorda y;
       i32 i;
       s32 d;

    x = _hospes(a->url);
    y = _hospes(b->url);
    d = chorda_comparare(x, y);
    si (d != ZEPHYRUM)
    {
        redde d < ZEPHYRUM;
    }
    x = _titulus_articuli(a);
    y = _titulus_articuli(b);
    per (i = ZEPHYRUM; i < x.mensura && i < y.mensura; i++)
    {
        character p = (character)x.datum[i];
        character q = (character)y.datum[i];

        si (p >= 'A' && p <= 'Z')
        {
            p = (character)(p - 'A' + 'a');
        }
        si (q >= 'A' && q <= 'Z')
        {
            q = (character)(q - 'A' + 'a');
        }
        si (p != q)
        {
            redde (insignatus character)p < (insignatus character)q;
        }
    }
    redde x.mensura < y.mensura;
}

/* indices bibliothecae ordine indicis (Xar de i32) */
interior Xar*
_ordo (
    constans NavigatorMontatio* m,
                       Piscina* p)
{
    Xar* o;
    i32  n;
    i32  i;
    i32  j;

    n = navigator_articulorum_numerus(m->bibliotheca);
    o = xar_creare(p, (i32)magnitudo(i32));
    per (i = ZEPHYRUM; o && i < n; i++)
    {
        i32* e = (i32*)xar_addere(o);

        si (!e)
        {
            redde o;
        }
        *e = i;
        /* insertio: elementum novum retro movetur */
        per (j = i; j > ZEPHYRUM; j--)
        {
            i32* prior  = (i32*)xar_obtinere(o, j - I);
            i32* hic    = (i32*)xar_obtinere(o, j);
            i32  t;

            si (!_praecedit(navigator_articulus(m->bibliotheca, *hic),
                navigator_articulus(m->bibliotheca, *prior)))
            {
                frange;
            }
            t       = *prior;
            *prior  = *hic;
            *hic    = t;
        }
    }
    redde o;
}

interior i32
_index_ordinis (
    Xar* ordo,
    s32  positio)
{
    redde *(i32*)xar_obtinere(ordo, (i32)positio);
}

/* capacitas in cellulis: altitudo minus margines, titulus, caput */
interior s32
_capacitas (
    constans NavigatorMontatio* m)
{
    redde _superficies(m, FALSUM) / NAV_LINEA - II - NAV_TITULI
        - NAV_CAPUT;
}

/* album: grex per hospitem ("g<positio primi>"), res = index + 1
 * ("r<res>") */
interior Album*
_album (
    constans NavigatorMontatio* m,
                           Xar* ordo,
                       Piscina* p)
{
     Album* l;
    chorda  prior = _vacua();
       i32  k;
 character  id[XVI];

    l = album_creare(p, _capacitas(m));
    per (k = ZEPHYRUM; l && k < xar_numerus(ordo); k++)
    {
           i32 i = _index_ordinis(ordo, (s32)k);
        chorda h = _hospes(navigator_articulus(m->bibliotheca, i)->url);

        si (k == ZEPHYRUM || !chorda_aequalis(h, prior))
        {
            sprintf(id, "g%d", (integer)k);
            (vacuum)album_addere(l, id, ALBUM_TITULUS, NAV_ORDO,
                ZEPHYRUM);
            prior = h;
        }
        sprintf(id, "r%d", (integer)(i + I));
        (vacuum)album_addere(l, id, ALBUM_RES, NAV_ORDO, (s32)(i + I));
    }
    redde l;
}

/* positio electi in ordine; -1 si bibliotheca vacua */
interior s32
_electus (
    constans NavigatorMontatio* m,
                           Xar* ordo)
{
    chorda e;
       i32 k;

    si (xar_numerus(ordo) == ZEPHYRUM)
    {
        redde -I;
    }
    e = _campus(m, "electus_articulus");
    per (k = ZEPHYRUM; e.mensura > ZEPHYRUM && k < xar_numerus(ordo);
         k++)
    {
        si (chorda_aequalis(navigator_articulus(m->bibliotheca,
            _index_ordinis(ordo, (s32)k))->identitas, e))
        {
            redde (s32)k;
        }
    }
    redde ZEPHYRUM;
}

/* positio in ordine indicis bibliothecae i; -1 si abest */
interior s32
_positio_indicis (
    Xar* ordo,
    i32  index)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(ordo); k++)
    {
        si (_index_ordinis(ordo, (s32)k) == index)
        {
            redde (s32)k;
        }
    }
    redde -I;
}

interior s32
_pagina_indicis (
    Album* l,
      Xar* ordo,
      s32  electus)
{
    s32 k;

    k = electus >= ZEPHYRUM ? album_pagina_rei(l, (s32)_index_ordinis(
        ordo, electus) + I, ZEPHYRUM) : ZEPHYRUM;
    redde k < ZEPHYRUM ? ZEPHYRUM : k;
}

/* id "r<n>" aut "g<n>" -> n (r: >0; g: >=0); -1 si aliud */
interior s32
_id_numerus (
         chorda id,
      character praefixum)
{
    s32 n;

    si (id.mensura < II || id.datum[ZEPHYRUM] != (i8)praefixum)
    {
        redde -I;
    }
    id.datum++;
    id.mensura--;
    redde chorda_ut_s32(id, &n) && n >= ZEPHYRUM ? n : -I;
}


/* ==================================================
 * Motus
 * ================================================== */

interior b32
_articulum_aperire (
    NavigatorMontatio* m,
               chorda  identitas,
              Piscina* p)
{
    constans character* tituli[VI];
    constans character* valores[VI];

    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "pagina";
    tituli[I]          = "articulus";
    valores[I]         = chorda_ut_cstr(identitas, p);
    /* versio ultima FIGITUR: locus (comes) versionis suae est, etiam
     * cum versio nova postea advenit (spec V.11, V.16) */
    tituli[II]         = "versio";
    valores[II]        = _versio(_articulus_per_id(m, identitas),
        _vacua()) ? chorda_ut_cstr(_versio(_articulus_per_id(m,
        identitas), _vacua())->identitas, p) : "";
    tituli[III]   = "pagina";
    valores[III]  = "0";
    tituli[IV]    = "electus_articulus";
    valores[IV]   = valores[I];
    tituli[V]     = "nota";
    valores[V]    = "";
    redde _campos_scribere(m, tituli, valores, VI);
}

/* pagina articuli proxima (gradus I) aut prior (-I), coacta */
interior b32
_paginam_movere (
    NavigatorMontatio* m,
                  s32  gradus)
{
        NavigatorFigurae* f;
                     s32  k;
      constans character* tituli[I];
      constans character* valores[I];
               character  numerus[XVI];

    f = _paginas_parare(m);
    si (!f)
    {
        redde VERUM;
    }
    k = _pagina_currens(m, f) + gradus;
    si (k < ZEPHYRUM || (i32)k >= xar_numerus(f->paginae))
    {
        redde VERUM;
    }
    sprintf(numerus, "%d", (integer)k);
    tituli[ZEPHYRUM]   = "pagina";
    valores[ZEPHYRUM]  = numerus;
    redde _campos_scribere(m, tituli, valores, I);
}


/* ==================================================
 * Praecepta
 * ================================================== */

nomen structura {
    NavigatorMontatio* m;
              Piscina* p;
                Album* l;
                  Xar* ordo;
                  s32  electus;     /* positio in ordine */
                  i32  visus;
} ContextusPraecepti;

interior b32
_requisita (
        i32  requisita,
     vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde !(requisita & REQUIRIT_ELECTUM) || cp->electus >= ZEPHYRUM;
}

interior vacuum
_contextum_parare (
     NavigatorMontatio* m,
               Piscina* p,
    ContextusPraecepti* cp)
{
    cp->m        = m;
    cp->p        = p;
    cp->visus    = _visus(m);
    cp->ordo     = _ordo(m, p);
    cp->l        = _album(m, cp->ordo, p);
    cp->electus  = _electus(m, cp->ordo);
}

interior b32
_electum_scribere (
    ContextusPraecepti* cp,
                   s32  positio)
{
    constans character* tituli[I];
    constans character* valores[I];

    si (positio < ZEPHYRUM || (i32)positio >= xar_numerus(cp->ordo))
    {
        redde VERUM;
    }
    tituli[ZEPHYRUM]   = "electus_articulus";
    valores[ZEPHYRUM]  = chorda_ut_cstr(navigator_articulus(
        cp->m->bibliotheca, _index_ordinis(cp->ordo,
        positio))->identitas,
        cp->p);
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

/* paginae indicis: electus = res prima paginae */
interior b32
_indicem_vertere (
    ContextusPraecepti* cp,
                   s32  gradus)
{
    s32 k;
    s32 res;

    k = _pagina_indicis(cp->l, cp->ordo, cp->electus) + gradus;
    si (k < ZEPHYRUM || k >= album_paginae(cp->l))
    {
        redde VERUM;
    }
    res = album_res_prima(cp->l, k);
    redde res > ZEPHYRUM ? _electum_scribere(cp, _positio_indicis(
        cp->ordo, (i32)(res - I))) : VERUM;
}

interior b32
fac_indicem_priorem (
    vacuum* ctx)
{
    redde _indicem_vertere((ContextusPraecepti*)ctx, -I);
}

interior b32
fac_indicem_proximum (
    vacuum* ctx)
{
    redde _indicem_vertere((ContextusPraecepti*)ctx, I);
}

interior b32
fac_aperire (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    si (cp->electus < ZEPHYRUM)
    {
        redde VERUM;
    }
    redde _articulum_aperire(cp->m, navigator_articulus(
        cp->m->bibliotheca, _index_ordinis(cp->ordo, cp->electus))
        ->identitas, cp->p);
}

interior b32
fac_paginam_proximam (
    vacuum* ctx)
{
    redde _paginam_movere(((ContextusPraecepti*)ctx)->m, I);
}

interior b32
fac_paginam_priorem (
    vacuum* ctx)
{
    redde _paginam_movere(((ContextusPraecepti*)ctx)->m, -I);
}

/* Esc in pagina: ad indicem (articulus e semita tollitur; electus
 * manet) */
interior b32
fac_ad_indicem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
    constans character* tituli[V];
    constans character* valores[V];

    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "index";
    tituli[I]          = "articulus";
    valores[I]         = "";
    tituli[II]         = "versio";
    valores[II]        = "";
    tituli[III]        = "pagina";
    valores[III]       = "";
    tituli[IV]         = "nota";
    valores[IV]        = "";
    redde _campos_scribere(cp->m, tituli, valores, V);
}

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "navigator.proximus", "Next article", "j", VISUS_INDICIS,
        ZEPHYRUM, fac_proximum },
    { "navigator.prior", "Previous article", "k", VISUS_INDICIS,
        ZEPHYRUM, fac_priorem },
    { "navigator.index_prior", "Previous page", "[", VISUS_INDICIS,
        ZEPHYRUM, fac_indicem_priorem },
    { "navigator.index_proximus", "Next page", "]", VISUS_INDICIS,
        ZEPHYRUM, fac_indicem_proximum },
    { "navigator.aperire", "Read", "Enter", VISUS_INDICIS,
        REQUIRIT_ELECTUM, fac_aperire },
    { "navigator.pagina_proxima", "Next page", "j", VISUS_PAGINAE,
        ZEPHYRUM, fac_paginam_proximam },
    { "navigator.pagina_prior", "Previous page", "k", VISUS_PAGINAE,
        ZEPHYRUM, fac_paginam_priorem },
    { "navigator.ad_indicem", "Saved articles", "Esc", VISUS_PAGINAE,
        ZEPHYRUM, fac_ad_indicem },
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
      NavigatorMontatio* m;
       NavigatorFigurae* privatum;
    constans Praeceptum* t;
     ContextusPraecepti  cp;
                    s32  n;
                    s32  gradus;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)des;
    m         = (NavigatorMontatio*)ctx;
    privatum  = (NavigatorFigurae*)m->figurae_ctx;
    si (!privatum)
    {
        redde FALSUM;
    }
    piscina_vacare(privatum->scheda);
    si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        t = praecepta_ad_clavem(TABULA_PRAECEPTORUM, _visus(m), ev);
        si (!t)
        {
            redde FALSUM;
        }
        _contextum_parare(m, privatum->scheda, &cp);
        redde praecepta_status(t, cp.visus, _requisita, &cp)
            == PRAECEPTUM_PRAESTO && t->fn(&cp);
    }
    si (   !nodus || ev->datum.mus.botton != MUS_SINISTER
        || (   ev->genus != EVENTUS_MUS_DEPRESSUS
            && ev->genus != EVENTUS_MUS_DUPLEX))
    {
        redde FALSUM;
    }
    gradus = chorda_aequalis_literis(nodus->id, "pagina-prior") ? -I
        : chorda_aequalis_literis(nodus->id, "pagina-proxima") ? I
        : ZEPHYRUM;
    si (_visus(m) == VISUS_PAGINAE)
    {
        redde gradus != ZEPHYRUM && ev->genus == EVENTUS_MUS_DEPRESSUS
            && _paginam_movere(m, gradus);
    }
    _contextum_parare(m, privatum->scheda, &cp);
    n = nodus->partes == PARTES_ITEM ? _id_numerus(nodus->id, 'r') : -I;
    si (n > ZEPHYRUM)
    {
        cp.electus = _positio_indicis(cp.ordo, (i32)(n - I));
        si (ev->genus == EVENTUS_MUS_DUPLEX)
        {
            redde fac_aperire(&cp);
        }
        redde _electum_scribere(&cp, cp.electus);
    }
    si (gradus != ZEPHYRUM && ev->genus == EVENTUS_MUS_DEPRESSUS)
    {
        redde _indicem_vertere(&cp, gradus);
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

interior s32
_runae (
    chorda textus)
{
    i32 i;
    s32 n;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si ((textus.datum[i] & 0xC0) != 0x80)
        {
            n++;
        }
    }
    redde n;
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

interior vacuum
_centrum_pingere (
               Mandata* md,
    constans Componens* c,
                chorda  textus,
            ColorThema  color)
{
    s32 columnae;

    columnae  = c->fines.latitudo / NAV_LITTERA - II;
    textus    = _praecidere(textus, columnae);
    _textum_pingere(md, c->fines.x + (c->fines.latitudo - _runae(textus)
        * NAV_LITTERA) / II, c->fines.y + (c->fines.altitudo
        - NAV_LINEA) / II, textus, FONS_PARVUS, color);
}

/* ordo indicis: titulus sinister, "N versions" dexter obscurus;
 * electus ut lector */
interior vacuum
_ordinem_pingere (
                        Mandata* md,
             constans Componens* c,
    constans NavigatorArticulus* a,
                            b32  electus)
{
       chorda dextrum;
          s32 columnae;
      Punctum p;
      Punctum q;
   ColorThema obscurus;
    character textus[XXIV];
          i32 n;

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
    n = xar_numerus(a->versiones);
    sprintf(textus, n == I ? "1 version" : "%u versions",
        (insignatus integer)n);
    dextrum  = chorda_ex_literis(textus, md->piscina);
    columnae = (c->fines.latitudo - VIII) / NAV_LITTERA
        - (s32)dextrum.mensura - II;
    _textum_pingere(md, c->fines.x + IV, c->fines.y + IV, _praecidere(
        _titulus_articuli(a), columnae), FONS_PARVUS, electus
        ? COLOR_BACKGROUND : COLOR_TEXT);
    _textum_pingere(md, c->fines.x + c->fines.latitudo - IV
        - (s32)dextrum.mensura * NAV_LITTERA, c->fines.y + IV, dextrum,
        FONS_PARVUS, obscurus);
}

/* sagittae et index paginarum (index; pes paginae) */
interior vacuum
_sagittam_pingere (
               Mandata* md,
    constans Componens* f,
                   s32  k,
                   s32  n)
{
    si (chorda_aequalis_literis(f->id, "pagina-prior"))
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
_indicem_pingere (
     NavigatorMontatio* m,
    constans Componens* c,
               Mandata* md)
{
        Album* l;
          Xar* ordo;
    Componens* f;
          s32  e;
          s32  k;
          s32  n;
          s32  r;
          i32  i;
    character  textus[XXXII];

    ordo  = _ordo(m, md->piscina);
    l     = _album(m, ordo, md->piscina);
    e     = _electus(m, ordo);
    k     = _pagina_indicis(l, ordo, e);
    n     = album_paginae(l);
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        f = componens_liberum(c, i);
        r = f->partes == PARTES_ITEM ? _id_numerus(f->id, 'r') : -I;
        si (r > ZEPHYRUM)
        {
            _ordinem_pingere(md, f, navigator_articulus(m->bibliotheca,
                (i32)(r - I)), e >= ZEPHYRUM && _index_ordinis(ordo, e)
                == (i32)(r - I));
        }
        alioquin si (_id_numerus(f->id, 'g') >= ZEPHYRUM)
        {
            r = _id_numerus(f->id, 'g');
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                _hospes(navigator_articulus(m->bibliotheca,
                _index_ordinis(ordo, r))->url), FONS_PARVUS,
                COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "titulus"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + II,
                chorda_ex_literis("Saved articles", md->piscina),
                FONS_TITULI, COLOR_TEXT);
        }
        alioquin si (chorda_aequalis_literis(f->id, "caput"))
        {
            sprintf(textus, xar_numerus(ordo) == I ? "1 article"
                : "%u articles", (insignatus integer)xar_numerus(ordo));
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis(textus, md->piscina), FONS_PARVUS,
                COLOR_TEXT_DIM);
        }
        alioquin si (   chorda_aequalis_literis(f->id, "index_paginae")
                     && n > I)
        {
            sprintf(textus, "%d/%d", (integer)(k + I), (integer)n);
            _centrum_pingere(md, f, chorda_ex_literis(textus,
                md->piscina), COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "nihil"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis("No saved articles", md->piscina),
                FONS_PARVUS, COLOR_TEXT_DIM);
        }
        alioquin
        {
            _sagittam_pingere(md, f, k, n);
        }
    }
}

/* L5 (ut lector): locus currens notas habet (hospes)? */
interior b32
_notae_loci (
    NavigatorMontatio* m,
              Piscina* piscina)
{
    redde m->notas_habet && m->notas_habet(m->notas_habet_ctx,
        navigator_locus(m, piscina));
}

/* pagina: lineae articuli; pes "titulus - hospes - k/n" inter sagittas
 * (nota: nuntius loco eius) */
interior vacuum
_paginam_pingere (
     NavigatorMontatio* m,
    constans Componens* c,
               Mandata* md)
{
          NavigatorFigurae* f;
     constans LectorPagina* g;
      constans LectorLinea* l;
                 Componens* x;
                    chorda  nota;
                    chorda  pes;
                       s32  k;
                       s32  n;
                       s32  lateris;
                       s32  supra;
                       i32  i;
                       i32  j;
                 character  numeri[XXIV];

    f = _paginas_parare(m);
    si (!f)
    {
        redde;
    }
    k = _pagina_currens(m, f);
    n = (s32)xar_numerus(f->paginae);
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
                _textum_pingere(md, x->fines.x + lateris * NAV_LITTERA
                    + l->x, x->fines.y + l->y, chorda_sectio(
                    (i32)k < f->paginae_articuli
                    ? f->textus->caput.textus : f->textus->nexus.textus,
                    l->initium, l->finis), l->fons, COLOR_TEXT);
            }
        }
        alioquin si (chorda_aequalis_literis(x->id, "pes"))
        {
            nota = _campus(m, "nota");
            sprintf(numeri, " - %d/%d", (integer)(k + I), (integer)n);
            pes = chorda_concatenare(chorda_concatenare(f->titulus,
                chorda_concatenare(chorda_ex_literis(" - ",
                md->piscina),
                f->hospes, md->piscina), md->piscina),
                chorda_ex_literis(
                numeri, md->piscina), md->piscina);
            si (_notae_loci(m, md->piscina))
            {
                pes = chorda_concatenare(pes, chorda_ex_literis(
                    " \302\266", md->piscina), md->piscina);
            }
            _centrum_pingere(md, x, nota.mensura
                > ZEPHYRUM ? nota : pes,
                nota.mensura > ZEPHYRUM ? COLOR_ERROR : COLOR_TEXT_DIM);
        }
        alioquin
        {
            _sagittam_pingere(md, x, k, n);
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
    NavigatorFigurae* contextus;
               Fines  mensa;

    (vacuum)thema;
    contextus       = (NavigatorFigurae*)ctx;
    mensa.x         = ZEPHYRUM;
    mensa.y         = ZEPHYRUM;
    mensa.latitudo  = c->fines.latitudo;
    mensa.altitudo  = c->fines.altitudo;
    figura_mensam_pingere(md, mensa, chorda_aequalis_literis(c->titulus,
        "focatum"));
    mensa.x         = NAV_LITTERA;
    mensa.y         = NAV_LINEA;
    mensa.latitudo  = c->fines.latitudo - II * NAV_LITTERA;
    mensa.altitudo  = c->fines.altitudo - II * NAV_LINEA;
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
    alioquin
    {
        _indicem_pingere(contextus->m, c, md);
    }
}

interior b32
_figuras_registrare (
    NavigatorMontatio* m,
                  i32  latitudo,
                  i32  altitudo)
{
    NavigatorFigurae* contextus;

    contextus = (NavigatorFigurae*)piscina_allocare(m->piscina,
        magnitudo(NavigatorFigurae));
    si (!contextus)
    {
        redde FALSUM;
    }
    memset(contextus, ZEPHYRUM, magnitudo(NavigatorFigurae));
    contextus->m                     = m;
    contextus->scheda                = piscina_generare_dynamicum(
        "navigator_scheda", LXIV * M);
    contextus->arca                  = piscina_generare_dynamicum(
        "navigator_arca", CCLVI * M);
    contextus->latitudo_montationis  = (s32)latitudo;
    contextus->altitudo_montationis  = (s32)altitudo;
    m->figurae_ctx                   = contextus;
    redde contextus->scheda && contextus->arca
        && figura_registrare(m->figurae, PARTES_TABULA, ZEPHYRUM,
        figura_tabulae, contextus);
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
    componens_ponere_actio(c, actio_navigatoris);
    l = (LocusComponentis*)xar_addere(dp->loci);
    si (l)
    {
        l->index  = index;
        l->c      = c;
    }
    redde c;
}

/* linea: [sagitta] medium crescens [sagitta] */
interior vacuum
_lineam_sagittarum_disponere (
              Disponens* dp,
                    s32  radix_d,
     constans character* medium,
                    s32  lineae)
{
    s32 linea;

    linea = _locum_addere(dp, radix_d, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, lineae);
    (vacuum)_componentem_locare(dp, linea, "pagina-prior",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, DISPOSITIO_FIXA, lineae);
    (vacuum)_componentem_locare(dp, linea, medium, PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, lineae);
    (vacuum)_componentem_locare(dp, linea, "pagina-proxima",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, DISPOSITIO_FIXA, lineae);
}

/* index: titulus, caput (numerus, sagittae, index paginarum), regio
 * ordinum cum titulo gregis iterato */
interior vacuum
_indicem_disponere (
    NavigatorMontatio* m,
            Disponens* dp,
                  s32  radix_d,
              Piscina* piscina)
{
                  Album* l;
                    Xar* ordo;
     constans AlbumOrdo* o;
                    s32  linea;
                    s32  regio;
                    s32  a;
                    s32  b;
                    s32  t;
                    s32  i;

    (vacuum)_componentem_locare(dp, radix_d, "titulus", PARTES_TITULUS,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, NAV_TITULI);
    linea = _locum_addere(dp, radix_d, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, NAV_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "caput", PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, NAV_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "pagina-prior",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, DISPOSITIO_FIXA,
        NAV_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "index_paginae",
        PARTES_INDEX,
        DISPOSITIO_FIXA, VII, DISPOSITIO_FIXA, NAV_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "pagina-proxima",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, DISPOSITIO_FIXA,
        NAV_CAPUT);
    regio = _locum_addere(dp, radix_d, DISPOSITIO_COLUMNA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    ordo  = _ordo(m, piscina);
    l     = _album(m, ordo, piscina);
    si (!l || xar_numerus(ordo) == ZEPHYRUM)
    {
        (vacuum)_componentem_locare(dp, regio, "nihil", PARTES_INDEX,
            DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, NAV_ORDO);
        redde;
    }
    album_pagina(l, _pagina_indicis(l, ordo, _electus(m, ordo)), &a, &b,
        &t);
    si (t >= ZEPHYRUM)
    {
        /* pagina in medio gregis: hospes iteratur */
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

/* pagina: margo superus, area textus (cf. _area), pes cum sagittis */
interior vacuum
_paginam_disponere (
    NavigatorMontatio* m,
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
    _lineam_sagittarum_disponere(dp, radix_d, "pes", supra);
}

Componens*
navigator_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
    NavigatorMontatio* m;
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
    m = (NavigatorMontatio*)ctx;
    si (!m)
    {
        redde NIHIL;
    }
    latitudo = _superficies(m, VERUM);
    altitudo = _superficies(m, FALSUM);
    radix    = componens_creare(piscina, intern, "navigator",
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
    componens_ponere_actio(radix, actio_navigatoris);
    si (motus_ramus_focatus(motus, m->ramus))
    {
        componens_ponere_titulum(radix, "focatum");
    }
    columnae    = latitudo / NAV_LITTERA;
    lineae      = altitudo / NAV_LINEA;
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
        _paginam_disponere(m, &dp, radix_d);
    }
    alioquin
    {
        _indicem_disponere(m, &dp, radix_d, piscina);
    }
    dispositio_computare(dp.d, columnae, lineae, NIHIL, NIHIL);
    per (i = ZEPHYRUM; i < (s32)xar_numerus(dp.loci); i++)
    {
        LocusComponentis* loc;

        loc = (LocusComponentis*)xar_obtinere(dp.loci, (i32)i);
        componens_ponere_fines(loc->c, dispositio_ad_pixela(
            dispositio_fines(dp.d, loc->index), columnae, lineae,
            latitudo, altitudo, NAV_LITTERA, NAV_LINEA));
        componens_addere_liberum(radix, loc->c);
    }
    redde radix;
}


/* ==================================================
 * Montatio
 * ================================================== */

b32
navigator_montare (
       NavigatorMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
    NavigatorBibliotheca* bibliotheca,
                     i32  latitudo,
                     i32  altitudo,
         StatusSystemata* systemata)
{
    Organum* organum;

    si (!m || !piscina || !intern || !repo || !bibliotheca)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(NavigatorMontatio));
    m->piscina      = piscina;
    m->intern       = intern;
    m->bibliotheca  = bibliotheca;
    m->ramus        = id ? insula_ramus(repo, "navigator", id)
                         : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "navigator",
        "navigator");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"navigator\"", id ? id : "navigator", piscina,
               intern))
    {
        redde FALSUM;
    }
    m->actiones  = actio_registrum_creare(piscina, intern);
    m->figurae   = figura_registrum_creare(piscina);
    si (   !m->actiones || !m->figurae
        || !praecepta_verificare(TABULA_PRAECEPTORUM)
        || !actio_registrare(m->actiones, actio_navigatoris, agere, m)
        || !_figuras_registrare(m, latitudo, altitudo))
    {
        redde FALSUM;
    }
    ((NavigatorFigurae*)m->figurae_ctx)->organum = organum;
    redde VERUM;
}


/* ==================================================
 * Semitae et locus
 * ================================================== */

/* SemitaOrganum lateris: manifestum suum; ctx = NavigatorMontatio* */
interior constans Organum*
_organa (
    vacuum* ctx,
    chorda  genus)
{
    NavigatorMontatio* m = (NavigatorMontatio*)ctx;

    si (   !m || !m->figurae_ctx
        || !chorda_aequalis_literis(genus, "navigator"))
    {
        redde NIHIL;
    }
    redde ((NavigatorFigurae*)m->figurae_ctx)->organum;
}

b32
navigator_argumentum_ponere (
      NavigatorMontatio* m,
     constans character* argumentum)
{
     StmlNodus* semita;
        chorda* articulus;
        chorda* versio;
        chorda  vitium;

    si (!m || !m->figurae_ctx)
    {
        redde FALSUM;
    }
    /* URI aut cognomen manifesti ("" index, identitas nuda) */
    semita = semita_ex_argumento(_organa, m, "navigator", argumentum
        ? argumentum : "", m->piscina, m->intern, &vitium);
    si (!semita)
    {
        redde FALSUM;
    }
    /* articulus aut versio ignota: recusatur ante scripturam */
    articulus  = stml_attributum_capere(semita, "articulus");
    versio     = stml_attributum_capere(semita, "versio");
    si (   articulus && articulus->mensura > ZEPHYRUM
        && (!_articulus_per_id(m, *articulus)
            || (versio && versio->mensura > ZEPHYRUM
                && !_versio(_articulus_per_id(m, *articulus),
                *versio))))
    {
        redde FALSUM;
    }
    si (!semita_applicare(_organa, m, &m->ramus, semita, m->piscina,
            &vitium))
    {
        redde FALSUM;
    }
    redde navigator_semitam_secuta(m);
}

b32
navigator_semitam_secuta (
    vacuum* ctx)
{
     NavigatorMontatio* m;
      NavigatorFigurae* f;
                   s32  k;
    constans character* tituli[V];
    constans character* valores[V];
             character  numerus[XVI];

    m = (NavigatorMontatio*)ctx;
    si (!m || !m->figurae_ctx || _visus(m) != VISUS_PAGINAE)
    {
        redde VERUM;
    }
    f = _paginas_parare(m);
    si (!f)
    {
        /* articulus amissus (restitutio): ad indicem cum nota */
        tituli[ZEPHYRUM]   = "visus";
        valores[ZEPHYRUM]  = "index";
        tituli[I]          = "articulus";
        valores[I]         = "";
        tituli[II]         = "versio";
        valores[II]        = "";
        tituli[III]        = "pagina";
        valores[III]       = "";
        tituli[IV]         = "nota";
        valores[IV]        = "article not found";
        redde _campos_scribere(m, tituli, valores, V);
    }
    /* pagina coacta; versio absens figitur (versio in arca = ultima) */
    k = _pagina_currens(m, f);
    si (   k == _campus_numerus(m, "pagina")
        && _campus(m, "versio").mensura > ZEPHYRUM)
    {
        redde VERUM;
    }
    sprintf(numerus, "%d", (integer)k);
    tituli[ZEPHYRUM]   = "pagina";
    valores[ZEPHYRUM]  = numerus;
    tituli[I]          = "versio";
    valores[I]         = chorda_ut_cstr(f->versio_arcae, f->scheda);
    redde _campos_scribere(m, tituli, valores, II);
}

chorda
navigator_locus (
     vacuum* ctx,
    Piscina* piscina)
{
     NavigatorMontatio* m;
             StmlNodus* semita;

    m = (NavigatorMontatio*)ctx;
    si (!m || !piscina || !m->figurae_ctx)
    {
        redde _vacua();
    }
    semita = semita_legere(_organa, m, &m->ramus, "navigator", piscina,
        m->intern);
    si (!semita)
    {
        redde _vacua();
    }
    /* index: articulus, versio, pagina iam absunt - omnis scriptor qui
     * ad indicem ducit eos tollit (fac_ad_indicem, semita indicis) */
    redde semita_ad_uri(_organa, m, semita, piscina);
}

chorda
navigator_locus_titulus (
     vacuum* ctx,
    Piscina* piscina)
{
     NavigatorMontatio* m;
      NavigatorFigurae* f;
             character  numeri[XXIV];

    m = (NavigatorMontatio*)ctx;
    f = m && _visus(m) == VISUS_PAGINAE ? _paginas_parare(m) : NIHIL;
    si (!f)
    {
        redde chorda_ex_literis("Saved articles", piscina);
    }
    sprintf(numeri, " - %d/%u", (integer)(_pagina_currens(m, f) + I),
        (insignatus integer)xar_numerus(f->paginae));
    redde chorda_concatenare(chorda_concatenare(f->titulus,
        chorda_concatenare(chorda_ex_literis(" - ", piscina), f->hospes,
        piscina), piscina), chorda_ex_literis(numeri, piscina),
        piscina);
}
