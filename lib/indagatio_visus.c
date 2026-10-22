/* indagatio_visus.c - INDAGATIO in latere vici (S3a;
 * include/indagatio_visus.h). Forma bibliothecae sanctorum: arbor
 * dispositionis in cellulis (LITTERA x LINEA), figura radicis omnia
 * pingit, praecepta per membrum__praecepta, conspectus paginatus per
 * membrum__album (tituli gregum iterantur), quaestio per
 * membrum__prompta.
 *
 * Fructus servatur per (verba, genus, pagina, capacitas) et versionem
 * indicis: componere, pingere et agere eundem legunt sine quaestione
 * iterata. Ordines visus = lista plana ex fructu (grex, inventum,
 * "+ k more"); electus = index inter eligibiles. */

#include "indagatio_visus.h"
#include "membrum__album.h"
#include "membrum__praecepta.h"
#include "membrum__prompta.h"
#include "chorda_aedificator.h"
#include "dispositio.h"
#include "fons.h"
#include "thema.h"
#include "motus.h"
#include "semita.h"

#include <stdio.h>
#include <string.h>

#define INDAGATIO_LATITUDO  CCC     /* superficies absens */
#define INDAGATIO_ALTITUDO  CCLVI
#define INDAGATIO_LITTERA   VI
#define INDAGATIO_LINEA     VIII
#define INDAGATIO_TITULI    III     /* lineae tituli */
#define INDAGATIO_CAPUT     II      /* lineae capitis */
#define INDAGATIO_GREX      II      /* lineae tituli gregis */
#define INDAGATIO_INVENTUM  III     /* lineae inventi */
#define INDAGATIO_PLURA     II      /* lineae "+ k more" */
#define INDAGATIO_PRIMA     III     /* inventa gregis in conspectu */

#define VISUS_CONSPECTUS    I
#define VISUS_GREGIS        II
#define REQUIRIT_ELECTUM    I

hic_manens constans character actio_indagationis[] = "indagatio.agere";

/* figura: contextus privatus; fructus servatus */
nomen structura {
     IndagatioMontatio* m;
      constans Organum* organum;
               Piscina* scheda;
                   s32  latitudo_montationis;
                   s32  altitudo_montationis;
               Piscina* memoria;    /* fructus servatus et clavis */
                chorda  clavis;
                   s64  versio;
      IndagatioFructus* fructus;
} IndagatioFigurae;

nomen enumeratio {
    ORDO_GREX = I,
    ORDO_INVENTUM,
    ORDO_PLURA
} GenusOrdinis;

/* ordo visus (lista plana ex fructu) */
nomen structura {
                    GenusOrdinis  genus;
          constans IndagatioGrex* grex;
      constans IndagatioInventum* inventum;   /* ORDO_INVENTUM */
                             s32  electio;    /* inter eligibiles; -1
                                              * grex */
} OrdoVisus;


/* ==================================================
 * Status (insula)
 * ================================================== */

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior chorda
_campus (
    constans IndagatioMontatio* m,
            constans character* titulus)
{
    chorda* a;

    a = insula_ramus_attributum(&m->ramus, titulus);
    redde a ? *a : _vacua();
}

interior s32
_campus_s32 (
    constans IndagatioMontatio* m,
            constans character* titulus)
{
    chorda a;
       s32 v;

    a = _campus(m, titulus);
    redde (a.mensura > ZEPHYRUM && chorda_ut_s32(a, &v)
        && v >= ZEPHYRUM)
        ? v : ZEPHYRUM;
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

/* campi actu uno, scriptore lateris ("" aut NIHIL tollit) */
interior b32
_campos_scribere (
             IndagatioMontatio* m,
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
        actio_indagationis, m->piscina));
    ok = mutare_ramum(&m->ramus, _campos_ponere, &grex);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

interior b32
_campum_scribere (
      IndagatioMontatio* m,
     constans character* titulus,
     constans character* valor)
{
    constans character* t[I];
    constans character* v[I];

    t[ZEPHYRUM] = titulus;
    v[ZEPHYRUM] = valor;
    redde _campos_scribere(m, t, v, I);
}

interior s32
_superficies (
     constans IndagatioMontatio* m,
                            b32  latitudo)
{
    constans IndagatioFigurae* f;
                          s32  v;
                       chorda  a;
                          s32  n;

    f = (constans IndagatioFigurae*)m->figurae_ctx;
    v = !f ? ZEPHYRUM : latitudo ? f->latitudo_montationis
                                 : f->altitudo_montationis;
    si (v <= ZEPHYRUM)
    {
        v = latitudo ? INDAGATIO_LATITUDO : INDAGATIO_ALTITUDO;
    }
    a = _campus(m, latitudo ? "superficies_latitudo"
        : "superficies_altitudo");
    redde (a.mensura > ZEPHYRUM && chorda_ut_s32(a, &n) && n > ZEPHYRUM)
        ? n : v;
}

/* verba currentia: prompta aperta = textus vivus, aliter q; textus in
 * exitus[PROMPTA_MAXIMA] */
interior chorda
_verba (
    constans IndagatioMontatio* m,
                     character* exitus)
{
    chorda q;

    si (prompta_aperta(&m->ramus, NIHIL))
    {
        q.datum    = (i8*)exitus;
        q.mensura  = prompta_textus(&m->ramus, exitus);
        redde q;
    }
    redde _campus(m, "q");
}

interior b32
_visus_gregis (
    constans IndagatioMontatio* m)
{
    redde _campus(m, "genus").mensura > ZEPHYRUM;
}

/* lineae ordinum: altitudo minus margines, titulus, caput */
interior s32
_lineae_regionis (
    constans IndagatioMontatio* m)
{
    redde _superficies(m, FALSUM) / INDAGATIO_LINEA - II
        - INDAGATIO_TITULI - INDAGATIO_CAPUT;
}

/* inventa per paginam gregis (I saltem) */
interior s32
_capacitas_gregis (
    constans IndagatioMontatio* m)
{
    s32 n;

    n = _lineae_regionis(m) / INDAGATIO_INVENTUM;
    redde n < I ? I : n;
}


/* ==================================================
 * Fructus et ordines
 * ================================================== */

/* fructus currens (servatus si clavis et versio eaedem) */
interior IndagatioFructus*
_fructus (
    IndagatioMontatio* m)
{
     IndagatioFigurae* f;
     IndagatioPetitio  petitio;
    ChordaAedificator* a;
               chorda  clavis;
            character  vivus[PROMPTA_MAXIMA];
            character  numerus[XXXII];
                  s64  versio;

    f = (IndagatioFigurae*)m->figurae_ctx;
    si (!f || !m->index)
    {
        redde NIHIL;
    }
    memset(&petitio, ZEPHYRUM, magnitudo(petitio));
    petitio.verba       = _verba(m, vivus);
    petitio.conspectus  = _campus(m, "genus");
    petitio.ordo        = INDAGATIO_ORDO_OPTIMUS;
    si (petitio.conspectus.mensura > ZEPHYRUM)
    {
        petitio.pagina     = (i32)_campus_s32(m, "pagina");
        petitio.capacitas  = (i32)_capacitas_gregis(m);
    }
    alioquin
    {
        petitio.capacitas = INDAGATIO_PRIMA;
    }
    a = chorda_aedificator_creare(f->scheda, (memoriae_index)CXXVIII);
    chorda_aedificator_appendere_chorda(a, petitio.verba);
    chorda_aedificator_appendere_character(a, '\x1f');
    chorda_aedificator_appendere_chorda(a, petitio.conspectus);
    sprintf(numerus, "\x1f%u\x1f%u", (insignatus integer)petitio.pagina,
        (insignatus integer)petitio.capacitas);
    chorda_aedificator_appendere_literis(a, numerus);
    clavis = chorda_aedificator_finire(a);
    (vacuum)indagatio_renovare(m->index);
    versio = indagatio_mensura(m->index).versio;
    si (   f->fructus && versio == f->versio
        && chorda_aequalis(clavis, f->clavis))
    {
        redde f->fructus;
    }
    piscina_vacare(f->memoria);
    f->fructus  = indagatio_quaerere(m->index, &petitio, f->memoria);
    f->clavis   = chorda_transcribere(clavis, f->memoria);
    f->versio   = f->fructus ? f->fructus->versio : versio;
    redde f->fructus;
}

interior i32
_summa (
    constans IndagatioFructus* fr)
{
    i32 n;
    i32 i;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; fr && fr->greges && i < xar_numerus(fr->greges);
         i++)
    {
        n += ((constans IndagatioGrex*)xar_obtinere(fr->greges,
            i))->numerus;
    }
    redde n;
}

/* ordines visus: conspectus (tituli, inventa, plura) aut grex
 * (inventa sola); *eligibiles numerus */
interior Xar*
_ordines (
    constans IndagatioMontatio* m,
     constans IndagatioFructus* fr,
                       Piscina* p,
                           s32* eligibiles)
{
    Xar* x;
    b32  grex_solus;
    s32  n;
    i32  i;
    i32  j;

    x           = xar_creare(p, (i32)magnitudo(OrdoVisus));
    grex_solus  = _visus_gregis(m);
    n           = ZEPHYRUM;
    per (i = ZEPHYRUM; x && fr && fr->greges
        && i < xar_numerus(fr->greges); i++)
    {
        constans IndagatioGrex* g =
            (constans IndagatioGrex*)xar_obtinere(fr->greges, i);
        OrdoVisus* o;

        si (!grex_solus)
        {
            o            = (OrdoVisus*)xar_addere(x);
            o->genus     = ORDO_GREX;
            o->grex      = g;
            o->inventum  = NIHIL;
            o->electio   = -I;
        }
        per (j = ZEPHYRUM; g->inventa && j < xar_numerus(g->inventa);
             j++)
        {
            o         = (OrdoVisus*)xar_addere(x);
            o->genus  = ORDO_INVENTUM;
            o->grex   = g;
            o->inventum  = (constans IndagatioInventum*)xar_obtinere(
                g->inventa, j);
            o->electio   = n++;
        }
        si (   !grex_solus && g->inventa
            && g->numerus > xar_numerus(g->inventa))
        {
            o            = (OrdoVisus*)xar_addere(x);
            o->genus     = ORDO_PLURA;
            o->grex      = g;
            o->inventum  = NIHIL;
            o->electio   = n++;
        }
    }
    *eligibiles = n;
    redde x;
}

interior constans OrdoVisus*
_ordo_electionis (
    Xar* ordines,
    s32  electio)
{
    i32 i;

    per (i = ZEPHYRUM; ordines && i < xar_numerus(ordines); i++)
    {
        constans OrdoVisus* o =
            (constans OrdoVisus*)xar_obtinere(ordines, i);

        si (o->electio == electio && electio >= ZEPHYRUM)
        {
            redde o;
        }
    }
    redde NIHIL;
}

interior s32
_altitudo_ordinis (
    constans OrdoVisus* o)
{
    redde o->genus == ORDO_GREX ? INDAGATIO_GREX
        : o->genus == ORDO_PLURA ? INDAGATIO_PLURA : INDAGATIO_INVENTUM;
}

/* album conspectus: id "g<i>" titulus (index ordinis + 1), "r<n>" res
 * (electio + 1) */
interior Album*
_album (
    constans IndagatioMontatio* m,
                           Xar* ordines,
                       Piscina* p)
{
     Album* l;
       i32  i;
 character  id[XVI];

    l = album_creare(p, _lineae_regionis(m));
    per (i = ZEPHYRUM; l && ordines && i < xar_numerus(ordines); i++)
    {
        constans OrdoVisus* o =
            (constans OrdoVisus*)xar_obtinere(ordines, i);

        si (o->genus == ORDO_GREX)
        {
            sprintf(id, "g%d", (integer)(i + I));
            (vacuum)album_addere(l, id, ALBUM_TITULUS,
                _altitudo_ordinis(o), ZEPHYRUM);
        }
        alioquin
        {
            sprintf(id, "r%d", (integer)(o->electio + I));
            (vacuum)album_addere(l, id, ALBUM_RES,
                _altitudo_ordinis(o), o->electio + I);
        }
    }
    redde l;
}

/* id "r<n>" -> n; "g<n>" -> -n; 0 aliter */
interior s32
_id_ordinis (
    chorda id)
{
          s32 n;
    character c;

    si (id.mensura < II)
    {
        redde ZEPHYRUM;
    }
    c = (character)id.datum[ZEPHYRUM];
    si (c != 'r' && c != 'g')
    {
        redde ZEPHYRUM;
    }
    id.datum++;
    id.mensura--;
    si (!chorda_ut_s32(id, &n) || n <= ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde c == 'r' ? n : -n;
}

/* paginae gregis (I saltem) */
interior s32
_paginae_gregis (
     constans IndagatioMontatio* m,
      constans IndagatioFructus* fr)
{
    s32 n;
    s32 k;

    n = (s32)_summa(fr);
    k = _capacitas_gregis(m);
    redde n <= ZEPHYRUM ? I : (n + k - I) / k;
}


/* ==================================================
 * Praecepta
 * ================================================== */

nomen structura {
    IndagatioMontatio* m;
              Piscina* p;
     IndagatioFructus* fructus;
                  Xar* ordines;
                Album* l;          /* conspectus solum */
                  s32  eligibiles;
                  s32  electus;    /* coactus; -1 si nullus */
                  b32  grex;       /* visus gregis */
} ContextusPraecepti;

interior vacuum
_contextum_parare (
     IndagatioMontatio* m,
               Piscina* p,
    ContextusPraecepti* cp)
{
    s32 e;

    cp->m        = m;
    cp->p        = p;
    cp->fructus  = _fructus(m);
    cp->ordines  = _ordines(m, cp->fructus, p, &cp->eligibiles);
    cp->grex     = _visus_gregis(m);
    cp->l        = cp->grex ? NIHIL : _album(m, cp->ordines, p);
    e            = _campus_s32(m, "electus");
    si (e >= cp->eligibiles)
    {
        e = cp->eligibiles - I;
    }
    cp->electus = cp->eligibiles > ZEPHYRUM ? e : -I;
}

interior b32
_requisita (
        i32  requisita,
     vacuum* ctx)
{
    ContextusPraecepti* cp;

    cp = (ContextusPraecepti*)ctx;
    redde !(requisita & REQUIRIT_ELECTUM) || cp->electus >= ZEPHYRUM;
}

interior b32
_electum_scribere (
    IndagatioMontatio* m,
                  s32  e)
{
    character numerus[XVI];

    sprintf(numerus, "%d", (integer)e);
    redde _campum_scribere(m, "electus", e > ZEPHYRUM ? numerus : "");
}

/* pagina gregis et electus actu uno */
interior b32
_paginam_scribere (
    IndagatioMontatio* m,
                  s32  pagina,
                  s32  electus)
{
     constans character* tituli[II];
     constans character* valores[II];
              character  p[XVI];
              character  e[XVI];

    sprintf(p, "%d", (integer)pagina);
    sprintf(e, "%d", (integer)electus);
    tituli[ZEPHYRUM]   = "pagina";
    valores[ZEPHYRUM]  = pagina > ZEPHYRUM ? p : "";
    tituli[I]          = "electus";
    valores[I]         = electus > ZEPHYRUM ? e : "";
    redde _campos_scribere(m, tituli, valores, II);
}

/* grex datus: genus ponitur, pagina et electus tolluntur */
interior b32
_gregem_aperire (
    IndagatioMontatio* m,
               chorda  conspectus,
              Piscina* p)
{
    constans character* tituli[III];
    constans character* valores[III];

    tituli[ZEPHYRUM]   = "genus";
    valores[ZEPHYRUM]  = chorda_ut_cstr(conspectus, p);
    tituli[I]          = "pagina";
    valores[I]         = "";
    tituli[II]         = "electus";
    valores[II]        = "";
    redde _campos_scribere(m, tituli, valores, III);
}

interior b32
fac_proximum (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
                   s32  pagina;

    si (cp->electus + I < cp->eligibiles)
    {
        redde _electum_scribere(cp->m, cp->electus + I);
    }
    /* grex: trans finem paginae ad proximam */
    pagina = _campus_s32(cp->m, "pagina");
    si (cp->grex && pagina + I < _paginae_gregis(cp->m, cp->fructus))
    {
        redde _paginam_scribere(cp->m, pagina + I, ZEPHYRUM);
    }
    redde VERUM;
}

interior b32
fac_priorem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
                   s32  pagina;

    si (cp->electus > ZEPHYRUM)
    {
        redde _electum_scribere(cp->m, cp->electus - I);
    }
    pagina = _campus_s32(cp->m, "pagina");
    si (cp->grex && pagina > ZEPHYRUM)
    {
        redde _paginam_scribere(cp->m, pagina - I,
            _capacitas_gregis(cp->m) - I);
    }
    redde VERUM;
}

interior b32
_paginam_vertere (
    ContextusPraecepti* cp,
                   s32  gradus)
{
    s32 k;

    si (cp->grex)
    {
        k = _campus_s32(cp->m, "pagina") + gradus;
        si (k < ZEPHYRUM || k >= _paginae_gregis(cp->m, cp->fructus))
        {
            redde VERUM;
        }
        redde _paginam_scribere(cp->m, k, ZEPHYRUM);
    }
    si (!cp->l || cp->electus < ZEPHYRUM)
    {
        redde VERUM;
    }
    k = album_pagina_rei(cp->l, cp->electus + I, ZEPHYRUM) + gradus;
    si (k < ZEPHYRUM || k >= album_paginae(cp->l))
    {
        redde VERUM;
    }
    redde _electum_scribere(cp->m, album_res_prima(cp->l, k) - I);
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

/* ordo electus: inventum per portam, "+ k more" gregem aperit */
interior b32
_aperire (
    ContextusPraecepti* cp,
                   s32  electio)
{
    constans OrdoVisus* o;

    o = _ordo_electionis(cp->ordines, electio);
    si (!o)
    {
        redde VERUM;
    }
    si (o->genus == ORDO_PLURA)
    {
        redde _gregem_aperire(cp->m, o->grex->conspectus, cp->p);
    }
    si (cp->m->aperire && o->inventum)
    {
        (vacuum)cp->m->aperire(cp->m->aperire_ctx, o->inventum->semita);
    }
    redde VERUM;
}

interior b32
fac_aperire (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _aperire(cp, cp->electus);
}

interior b32
fac_gregem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
    constans OrdoVisus* o;

    o = _ordo_electionis(cp->ordines, cp->electus);
    redde o ? _gregem_aperire(cp->m, o->grex->conspectus, cp->p)
        : VERUM;
}

interior b32
fac_quaestionem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde prompta_aperire(&cp->m->ramus, actio_indagationis, "quaestio",
        _campus(cp->m, "q"), cp->p);
}

/* Esc: grex -> conspectus -> q vacua -> non consumitur (vicus) */
interior b32
fac_redire (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
    constans character* tituli[III];
    constans character* valores[III];

    si (cp->grex)
    {
        tituli[ZEPHYRUM]   = "genus";
        valores[ZEPHYRUM]  = "";
        tituli[I]          = "pagina";
        valores[I]         = "";
        tituli[II]         = "electus";
        valores[II]        = "";
        redde _campos_scribere(cp->m, tituli, valores, III);
    }
    si (_campus(cp->m, "q").mensura > ZEPHYRUM)
    {
        tituli[ZEPHYRUM]   = "q";
        valores[ZEPHYRUM]  = "";
        tituli[I]          = "electus";
        valores[I]         = "";
        redde _campos_scribere(cp->m, tituli, valores, II);
    }
    redde FALSUM;
}

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "indagatio.proximum", "Next result", "j",
        VISUS_CONSPECTUS | VISUS_GREGIS, ZEPHYRUM, fac_proximum },
    { "indagatio.prius", "Previous result", "k",
        VISUS_CONSPECTUS | VISUS_GREGIS, ZEPHYRUM, fac_priorem },
    { "indagatio.deorsum", "Next result", "Down",
        VISUS_CONSPECTUS | VISUS_GREGIS, ZEPHYRUM, fac_proximum },
    { "indagatio.sursum", "Previous result", "Up",
        VISUS_CONSPECTUS | VISUS_GREGIS, ZEPHYRUM, fac_priorem },
    { "indagatio.pagina_prior", "Previous page", "[",
        VISUS_CONSPECTUS | VISUS_GREGIS, ZEPHYRUM,
            fac_paginam_priorem },
    { "indagatio.pagina_proxima", "Next page", "]",
        VISUS_CONSPECTUS | VISUS_GREGIS, ZEPHYRUM,
        fac_paginam_proximam },
    { "indagatio.aperire", "Open", "Enter",
        VISUS_CONSPECTUS | VISUS_GREGIS, REQUIRIT_ELECTUM,
            fac_aperire },
    { "indagatio.grex", "Show all of this kind", "\t", VISUS_CONSPECTUS,
        REQUIRIT_ELECTUM, fac_gregem },
    { "indagatio.quaestio", "Search", "/",
        VISUS_CONSPECTUS | VISUS_GREGIS, ZEPHYRUM, fac_quaestionem },
    { "indagatio.redire", "Back", "Esc",
        VISUS_CONSPECTUS | VISUS_GREGIS, ZEPHYRUM, fac_redire },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

/* Reditus promptae: q committitur (quota duplex omittitur), pagina et
 * electus tolluntur, genus manet, prompta clauditur */
interior b32
_promptam_committere (
    IndagatioMontatio* m,
               chorda  textus,
              Piscina* p)
{
    constans character* tituli[III];
    constans character* valores[III];
             character* q;
                   i32  i;
                   i32  n;

    q = (character*)piscina_allocare(p, (memoriae_index)textus.mensura
        + I);
    si (!q)
    {
        redde FALSUM;
    }
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si (textus.datum[i] != '"')
        {
            q[n++] = (character)textus.datum[i];
        }
    }
    q[n] = '\0';
    (vacuum)prompta_claudere(&m->ramus, actio_indagationis, p);
    tituli[ZEPHYRUM]   = "q";
    valores[ZEPHYRUM]  = q;
    tituli[I]          = "pagina";
    valores[I]         = "";
    tituli[II]         = "electus";
    valores[II]        = "";
    redde _campos_scribere(m, tituli, valores, III);
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
       IndagatioMontatio* m;
        IndagatioFigurae* privatum;
     constans Praeceptum* t;
      ContextusPraecepti  cp;
                     i32  visus;
                     s32  n;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)des;
    m         = (IndagatioMontatio*)ctx;
    privatum  = (IndagatioFigurae*)m->figurae_ctx;
    si (!privatum)
    {
        redde FALSUM;
    }
    piscina_vacare(privatum->scheda);
    /* prompta aperta claves omnes accipit */
    si (prompta_aperta(&m->ramus, NIHIL))
    {
               chorda textus;
        PromptaExitus exitus;

        exitus = prompta_clavem_agere(&m->ramus, actio_indagationis, ev,
            &textus, privatum->scheda);
        si (exitus == PROMPTA_REDITUS)
        {
            redde _promptam_committere(m, textus, privatum->scheda);
        }
        si (exitus != PROMPTA_NIHIL)
        {
            redde VERUM;
        }
    }
    visus = _visus_gregis(m) ? VISUS_GREGIS : VISUS_CONSPECTUS;
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
    si (   !nodus || ev->datum.mus.botton != MUS_SINISTER
        || (   ev->genus != EVENTUS_MUS_DEPRESSUS
            && ev->genus != EVENTUS_MUS_DUPLEX))
    {
        redde FALSUM;
    }
    _contextum_parare(m, privatum->scheda, &cp);
    n = nodus->partes == PARTES_ITEM ? _id_ordinis(nodus->id)
        : ZEPHYRUM;
    si (n > ZEPHYRUM)
    {
        redde ev->genus == EVENTUS_MUS_DUPLEX ? _aperire(&cp, n - I)
            : _electum_scribere(m, n - I);
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

/* runae textus (octeti continuationis non numerantur) */
interior s32
_runae (
    chorda textus)
{
    s32 n;
    i32 i;

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

/* praecidere ad columnas (runae, non octeti) */
interior chorda
_praecidere (
    chorda textus,
       s32 columnae)
{
    i32 i;
    s32 runae;

    si (columnae <= ZEPHYRUM)
    {
        textus.mensura = ZEPHYRUM;
        redde textus;
    }
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

/* linea excerpti: partes signatae fundo selectionis (in ordine
 * electo fundo inverso); '\n' -> ' ' */
interior vacuum
_excerptum_pingere (
               Mandata* md,
                   s32  x,
                   s32  y,
                   s32  columnae,
                   Xar* partes,
                   b32  electus)
{
    s32 initium;
    i32 i;
    i32 j;

    initium = x;
    per (i = ZEPHYRUM; partes && i < xar_numerus(partes)
        && columnae > ZEPHYRUM; i++)
    {
        constans IndagatioPars* pars =
            (constans IndagatioPars*)xar_obtinere(partes, i);
        chorda t;
           s32 n;

        t = chorda_transcribere(pars->textus, md->piscina);
        per (j = ZEPHYRUM; j < t.mensura; j++)
        {
            si (t.datum[j] == '\n' || t.datum[j] == '\t')
            {
                t.datum[j] = ' ';
            }
        }
        /* spatia initialia lineae (corpus a lineis vacuis incipere
         * potest) */
        dum (   x                 == initium && t.mensura > ZEPHYRUM
             && t.datum[ZEPHYRUM] == ' ')
        {
            t.datum++;
            t.mensura--;
        }
        t = _praecidere(t, columnae);
        n = _runae(t);
        si (n == ZEPHYRUM)
        {
            perge;
        }
        si (pars->congruit)
        {
            Fines f;

            f.x         = x;
            f.y         = y - I;
            f.latitudo  = n * INDAGATIO_LITTERA;
            f.altitudo  = INDAGATIO_LINEA + II;
            mandata_rectangulum(md, f, figura_color_thematis(electus
                ? COLOR_BACKGROUND : COLOR_SELECTION), VERUM);
        }
        _textum_pingere(md, x, y, t, ZEPHYRUM, pars->congruit
            || electus ? COLOR_TEXT : COLOR_TEXT_DIM);
        x         += n * INDAGATIO_LITTERA;
        columnae  -= n;
    }
}

interior vacuum
_ordinem_pingere (
               Mandata* md,
    constans Componens* c,
    constans OrdoVisus* o,
                   b32  electus)
{
       chorda t;
       chorda ctx;
          s32 columnae;
          s32 spatium;
      Punctum p;
      Punctum q;
    character textus[LXIV];

    columnae = (c->fines.latitudo - VIII) / INDAGATIO_LITTERA;
    si (o->genus == ORDO_GREX)
    {
        figura_fasciam_pingere(md, c->fines);
        sprintf(textus, "%u", (insignatus integer)o->grex->numerus);
        _textum_pingere(md, c->fines.x + c->fines.latitudo - IV
            - (s32)strlen(textus) * INDAGATIO_LITTERA, c->fines.y + IV,
            chorda_ex_literis(textus, md->piscina), ZEPHYRUM,
            COLOR_TEXT_DIM);
        figura_textum_audacem(md, c->fines.x + IV, c->fines.y + IV,
            _praecidere(o->grex->genus, columnae - (s32)strlen(textus)
            - I), figura_color_thematis(COLOR_TEXT));
        redde;
    }
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
    si (o->genus == ORDO_PLURA)
    {
        sprintf(textus, "+ %u more", (insignatus integer)(
            o->grex->numerus - xar_numerus(o->grex->inventa)));
        _textum_pingere(md, c->fines.x + IV, c->fines.y + IV,
            chorda_ex_literis(textus, md->piscina), ZEPHYRUM, electus
            ? COLOR_TEXT : COLOR_TEXT_DIM);
        redde;
    }
    /* titulus sinister (VIII columnae saltem contextui relictae),
     * contextus dexter in reliquo, "..." si praecisus */
    t        = _praecidere(o->inventum->titulus, columnae - VIII);
    spatium  = columnae - _runae(t) - II;
    ctx      = o->inventum->contextus;
    si (_runae(ctx) > spatium)
    {
        ctx = chorda_concatenare(_praecidere(ctx, spatium - I),
            chorda_ex_literis("\342\200\246", md->piscina),
            md->piscina);
    }
    si (ctx.mensura > ZEPHYRUM && spatium > I)
    {
        _textum_pingere(md, c->fines.x + c->fines.latitudo - IV
            - _runae(ctx) * INDAGATIO_LITTERA, c->fines.y + IV, ctx,
            ZEPHYRUM, electus ? COLOR_TEXT : COLOR_TEXT_DIM);
    }
    _textum_pingere(md, c->fines.x + IV, c->fines.y + IV, t, ZEPHYRUM,
        COLOR_TEXT);
    _excerptum_pingere(md, c->fines.x + IV, c->fines.y + IV
        + INDAGATIO_LINEA + II, columnae, o->inventum->excerptum,
        electus);
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
            - (s32)strlen(textus) * INDAGATIO_LITTERA) / II,
            f->fines.y + IV, chorda_ex_literis(textus, md->piscina),
            ZEPHYRUM, COLOR_TEXT_DIM);
    }
    alioquin si (   chorda_aequalis_literis(f->id, "pagina-prior")
                 && n > I)
    {
        figura_sagittam_pingere(md, f->fines, -I, k <= ZEPHYRUM
            ? FIGURA_OBSCURUM : ZEPHYRUM);
    }
    alioquin si (   chorda_aequalis_literis(f->id, "pagina-proxima")
                 && n > I)
    {
        figura_sagittam_pingere(md, f->fines, I, k >= n - I
            ? FIGURA_OBSCURUM : ZEPHYRUM);
    }
}

/* titulus gregis currentis (genus) ex fructu */
interior chorda
_titulus_gregis (
    constans IndagatioFructus* fr)
{
    redde fr && fr->greges && xar_numerus(fr->greges) > ZEPHYRUM
        ? ((constans IndagatioGrex*)xar_obtinere(fr->greges,
        ZEPHYRUM))->genus : _vacua();
}

interior vacuum
_caput_pingere (
     IndagatioMontatio* m,
    ContextusPraecepti* cp,
    constans Componens* f,
               Mandata* md)
{
       chorda q;
       chorda g;
          i32 n;
    character vivus[PROMPTA_MAXIMA];
    character textus[CXCII];

    si (prompta_aperta(&m->ramus, NIHIL))
    {
        prompta_pingere(md, f->fines, "search:", &m->ramus);
        redde;
    }
    q = _verba(m, vivus);
    n = _summa(cp->fructus);
    si (q.mensura == ZEPHYRUM)
    {
        sprintf(textus, "Press / to search");
    }
    alioquin si (n == ZEPHYRUM)
    {
        sprintf(textus, "No results for \"%.*s\"",
            (integer)(q.mensura > XL ? XL : q.mensura),
            (constans character*)q.datum);
    }
    alioquin si (cp->grex)
    {
        g = _titulus_gregis(cp->fructus);
        sprintf(textus, "%.*s \302\267 \"%.*s\" \302\267 %u result%s",
            (integer)(g.mensura > XX ? XX : g.mensura),
            (constans character*)g.datum,
            (integer)(q.mensura > XL ? XL : q.mensura),
            (constans character*)q.datum, (insignatus integer)n,
            n == I ? "" : "s");
    }
    alioquin
    {
        sprintf(textus, "\"%.*s\" \302\267 %u result%s",
            (integer)(q.mensura > XL ? XL : q.mensura),
            (constans character*)q.datum, (insignatus integer)n,
            n == I ? "" : "s");
    }
    _textum_pingere(md, f->fines.x + IV, f->fines.y + IV, _praecidere(
        chorda_ex_literis(textus, md->piscina), (f->fines.latitudo
        - VIII) / INDAGATIO_LITTERA), ZEPHYRUM, COLOR_TEXT_DIM);
}

interior vacuum
_indicem_pingere (
     IndagatioMontatio* m,
    constans Componens* c,
               Mandata* md)
{
    ContextusPraecepti  cp;
             Componens* f;
                   s32  k;
                   s32  n;
                   s32  id;
                   i32  i;

    _contextum_parare(m, md->piscina, &cp);
    si (cp.grex)
    {
        k = _campus_s32(m, "pagina");
        n = _paginae_gregis(m, cp.fructus);
    }
    alioquin
    {
        k = cp.l && cp.electus >= ZEPHYRUM ? album_pagina_rei(cp.l,
            cp.electus + I, ZEPHYRUM) : ZEPHYRUM;
        k = k < ZEPHYRUM ? ZEPHYRUM : k;
        n = cp.l ? album_paginae(cp.l) : I;
    }
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        f = componens_liberum(c, i);
        si (chorda_aequalis_literis(f->id, "titulus"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + II,
                chorda_ex_literis("Search", md->piscina), FONS_TITULI,
                COLOR_TEXT);
        }
        alioquin si (chorda_aequalis_literis(f->id, "caput"))
        {
            _caput_pingere(m, &cp, f, md);
        }
        alioquin si (f->partes == PARTES_ITEM)
        {
            id = _id_ordinis(f->id);
            si (id > ZEPHYRUM && _ordo_electionis(cp.ordines, id - I))
            {
                _ordinem_pingere(md, f, _ordo_electionis(cp.ordines,
                    id - I), id - I == cp.electus);
            }
            alioquin si (   id < ZEPHYRUM && cp.ordines
                         && -id <= (s32)xar_numerus(cp.ordines))
            {
                _ordinem_pingere(md, f, (constans OrdoVisus*)
                    xar_obtinere(cp.ordines, (i32)(-id - I)), FALSUM);
            }
        }
        alioquin
        {
            _paginas_pingere(md, f, k, n);
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
    IndagatioFigurae* contextus;
               Fines  mensa;

    (vacuum)thema;
    contextus       = (IndagatioFigurae*)ctx;
    mensa.x         = ZEPHYRUM;
    mensa.y         = ZEPHYRUM;
    mensa.latitudo  = c->fines.latitudo;
    mensa.altitudo  = c->fines.altitudo;
    figura_mensam_pingere(md, mensa, chorda_aequalis_literis(c->titulus,
        "focatum"));
    mensa.x         = INDAGATIO_LITTERA;
    mensa.y         = INDAGATIO_LINEA;
    mensa.latitudo  = c->fines.latitudo - II * INDAGATIO_LITTERA;
    mensa.altitudo  = c->fines.altitudo - II * INDAGATIO_LINEA;
    mandata_rectangulum(md, mensa, figura_color_thematis(
        COLOR_BACKGROUND), VERUM);
    si (contextus && contextus->m)
    {
        _indicem_pingere(contextus->m, c, md);
    }
}

interior b32
_figuras_registrare (
    IndagatioMontatio* m,
                  i32  latitudo,
                  i32  altitudo)
{
    IndagatioFigurae* contextus;

    contextus = (IndagatioFigurae*)piscina_allocare(m->piscina,
        magnitudo(IndagatioFigurae));
    si (!contextus)
    {
        redde FALSUM;
    }
    memset(contextus, ZEPHYRUM, magnitudo(IndagatioFigurae));
    contextus->m                     = m;
    contextus->scheda                = piscina_generare_dynamicum(
        "indagatio_scheda", LXIV * M);
    contextus->memoria               = piscina_generare_dynamicum(
        "indagatio_fructus", LXIV * M);
    contextus->latitudo_montationis  = (s32)latitudo;
    contextus->altitudo_montationis  = (s32)altitudo;
    m->figurae_ctx                   = contextus;
    redde contextus->scheda && contextus->memoria
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

    index = _locum_addere(dp, parens, DISPOSITIO_LINEA, genus_x,
        valor_x, genus_y, lineae);
    c     = componens_creare(dp->piscina, dp->intern, id, partes);
    si (!c || index < ZEPHYRUM)
    {
        redde NIHIL;
    }
    componens_ponere_actio(c, actio_indagationis);
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
_caput_disponere (
    Disponens* dp,
          s32  radix_d)
{
    s32 linea;

    linea = _locum_addere(dp, radix_d, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
        INDAGATIO_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "caput", PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
        INDAGATIO_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "pagina-prior",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, DISPOSITIO_FIXA,
        INDAGATIO_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "index_paginae",
        PARTES_INDEX, DISPOSITIO_FIXA, VII, DISPOSITIO_FIXA,
        INDAGATIO_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "pagina-proxima",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, DISPOSITIO_FIXA,
        INDAGATIO_CAPUT);
}

interior vacuum
_ordinem_locare (
              Disponens* dp,
                    s32  regio,
     constans OrdoVisus* o,
                    s32  index,
                Piscina* piscina)
{
    character id[XVI];

    si (o->genus == ORDO_GREX)
    {
        sprintf(id, "g%d", (integer)(index + I));
    }
    alioquin
    {
        sprintf(id, "r%d", (integer)(o->electio + I));
    }
    (vacuum)piscina;
    (vacuum)_componentem_locare(dp, regio, id, PARTES_ITEM,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
        _altitudo_ordinis(o));
}

interior vacuum
_indicem_disponere (
    IndagatioMontatio* m,
            Disponens* dp,
                  s32  radix_d,
              Piscina* piscina)
{
    ContextusPraecepti cp;
                   s32 regio;
                   s32 a;
                   s32 b;
                   s32 t;
                   s32 k;
                   s32 i;

    (vacuum)_componentem_locare(dp, radix_d, "titulus", PARTES_TITULUS,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
        INDAGATIO_TITULI);
    _caput_disponere(dp, radix_d);
    regio = _locum_addere(dp, radix_d, DISPOSITIO_COLUMNA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    _contextum_parare(m, piscina, &cp);
    si (!cp.ordines || xar_numerus(cp.ordines) == ZEPHYRUM)
    {
        redde;
    }
    si (cp.grex)
    {
        per (i = ZEPHYRUM; i < (s32)xar_numerus(cp.ordines); i++)
        {
            _ordinem_locare(dp, regio,
                (constans OrdoVisus*)xar_obtinere(
                cp.ordines, (i32)i), i, piscina);
        }
        redde;
    }
    /* conspectus: pagina electi; titulus gregis iteratus supra */
    k = cp.electus >= ZEPHYRUM ? album_pagina_rei(cp.l, cp.electus + I,
        ZEPHYRUM) : ZEPHYRUM;
    album_pagina(cp.l, k < ZEPHYRUM ? ZEPHYRUM : k, &a, &b, &t);
    si (t >= ZEPHYRUM)
    {
        _ordinem_locare(dp, regio, (constans OrdoVisus*)xar_obtinere(
            cp.ordines, (i32)t), t, piscina);
    }
    per (i = a; i < b; i++)
    {
        _ordinem_locare(dp, regio, (constans OrdoVisus*)xar_obtinere(
            cp.ordines, (i32)i), i, piscina);
    }
}

Componens*
indagatio_visus_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
    IndagatioMontatio* m;
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
    m = (IndagatioMontatio*)ctx;
    si (!m)
    {
        redde NIHIL;
    }
    latitudo = _superficies(m, VERUM);
    altitudo = _superficies(m, FALSUM);
    radix     = componens_creare(piscina, intern, "indagatio",
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
    componens_ponere_actio(radix, actio_indagationis);
    si (motus_ramus_focatus(motus, m->ramus))
    {
        componens_ponere_titulum(radix, "focatum");
    }
    columnae    = latitudo / INDAGATIO_LITTERA;
    lineae      = altitudo / INDAGATIO_LINEA;
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
    _indicem_disponere(m, &dp, radix_d, piscina);
    dispositio_computare(dp.d, columnae, lineae, NIHIL, NIHIL);
    per (i = ZEPHYRUM; i < (s32)xar_numerus(dp.loci); i++)
    {
        LocusComponentis* loc;

        loc = (LocusComponentis*)xar_obtinere(dp.loci, (i32)i);
        componens_ponere_fines(loc->c, dispositio_ad_pixela(
            dispositio_fines(dp.d, loc->index), columnae, lineae,
            latitudo, altitudo, INDAGATIO_LITTERA, INDAGATIO_LINEA));
        componens_addere_liberum(radix, loc->c);
    }
    redde radix;
}


/* ==================================================
 * Montatio
 * ================================================== */

b32
indagatio_visus_montare (
       IndagatioMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
               Indagatio* index,
        IndagatioAperire  aperire,
                  vacuum* aperire_ctx,
                     i32  latitudo,
                     i32  altitudo,
         StatusSystemata* systemata)
{
    Organum* organum;

    si (!m || !piscina || !intern || !repo)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(IndagatioMontatio));
    m->piscina      = piscina;
    m->intern       = intern;
    m->index        = index;
    m->aperire      = aperire;
    m->aperire_ctx  = aperire_ctx;
    m->ramus        = id ? insula_ramus(repo, "indagatio", id)
                         : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "indagatio",
        "indagatio");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"indagatio\"", id ? id : "indagatio", piscina,
               intern))
    {
        redde FALSUM;
    }
    m->actiones  = actio_registrum_creare(piscina, intern);
    m->figurae   = figura_registrum_creare(piscina);
    si (   !m->actiones || !m->figurae
        || !praecepta_verificare(TABULA_PRAECEPTORUM)
        || !actio_registrare(m->actiones, actio_indagationis, agere, m)
        || !_figuras_registrare(m, latitudo, altitudo))
    {
        redde FALSUM;
    }
    ((IndagatioFigurae*)m->figurae_ctx)->organum = organum;
    redde VERUM;
}

/* SemitaOrganum lateris: manifestum suum; ctx = IndagatioMontatio* */
interior constans Organum*
_organa (
     vacuum* ctx,
     chorda  genus)
{
    IndagatioMontatio* m = (IndagatioMontatio*)ctx;

    si (   !m || !m->figurae_ctx
        || !chorda_aequalis_literis(genus, "indagatio"))
    {
        redde NIHIL;
    }
    redde ((IndagatioFigurae*)m->figurae_ctx)->organum;
}

/* semita tota: q, genus, pagina absentes in semita tolluntur (nexus
 * quaestionem totam reddit, non partem); electus semper; prompta
 * aperta clauditur */
b32
indagatio_visus_argumentum_ponere (
       IndagatioMontatio* m,
      constans character* argumentum)
{
              StmlNodus* semita;
                 chorda  vitium;
     constans character* tituli[IV];
     constans character* valores[IV];
     constans character* campi[III];
                    i32  n;
                    i32  i;

    si (!m)
    {
        redde FALSUM;
    }
    semita = semita_ex_argumento(_organa, m, "indagatio", argumentum
        ? argumentum : "", m->piscina, m->intern, &vitium);
    si (   !semita
        || !semita_applicare(_organa, m, &m->ramus, semita, m->piscina,
               &vitium))
    {
        redde FALSUM;
    }
    /* semita aperta fructus ostendit: prompta clauditur */
    si (prompta_aperta(&m->ramus, NIHIL))
    {
        (vacuum)prompta_claudere(&m->ramus, actio_indagationis,
            m->piscina);
    }
    campi[ZEPHYRUM]  = "q";
    campi[I]         = "genus";
    campi[II]        = "pagina";
    n                = ZEPHYRUM;
    per (i = ZEPHYRUM; i < III; i++)
    {
        si (!stml_attributum_capere(semita, campi[i]))
        {
            tituli[n]   = campi[i];
            valores[n]  = "";
            n++;
        }
    }
    tituli[n]   = "electus";
    valores[n]  = "";
    n++;
    redde _campos_scribere(m, tituli, valores, n);
}

b32
indagatio_visus_quaestionem_aperire (
    IndagatioMontatio* m)
{
    si (!m)
    {
        redde FALSUM;
    }
    redde prompta_aperire(&m->ramus, actio_indagationis, "quaestio",
        _campus(m, "q"), m->piscina);
}
