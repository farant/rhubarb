/* colloquia_visus.c - visus sessionum et fons communis
 * (include/colloquia_visus.h; colloquia-plan T5). Forma centri
 * admonitionum: dispositio in cellulis, figura radicis omnia pingit,
 * praecepta per membrum__praecepta, greges et paginae per
 * membrum__album. */

#include "colloquia_visus.h"
#include "praeconium.h"
#include "membrum__album.h"
#include "membrum__praecepta.h"
#include "lectiones.h"
#include "dispositio.h"
#include "fons.h"
#include "thema.h"
#include "motus.h"

#include <stdio.h>
#include <string.h>

#define COLLOQUIA_LATITUDO   CCLXXX   /* superficies absens */
#define COLLOQUIA_ALTITUDO   CCLVI
#define COLLOQUIA_LITTERA    VI
#define COLLOQUIA_LINEA      VIII
#define COLLOQUIA_TITULI     III      /* lineae tituli */
#define COLLOQUIA_CAPUT      II       /* lineae capitis */
#define COLLOQUIA_GREX       II       /* lineae tituli gregis */
#define COLLOQUIA_ORDO       IV       /* lineae ordinis */
#define COLLOQUIA_PANELLI_MS ((s64)V * (s64)M)

hic_manens constans character actio_colloquiorum[] = "colloquia.agere";


/* ==================================================
 * Fons
 * ================================================== */

/* panelli unius servitoris (socket sessionum); panelli NIHIL =
 * servitor non currit (omnes eius sessiones clausae) */
nomen structura {
    chorda  servitor;
       Xar* panelli;
} PanelliMinistri;

/* titulus per sessionem: inventus servatur; vacuus (nondum ai-title)
 * iterum quaeritur cum sessio novum eventum habet */
nomen structura {
    chorda sessio;
    chorda titulus;
       s64 tempus_conatus;
} TitulusSessionis;

b32
colloquia_visus_fontem_initiare (
      ColloquiorumFons* f,
               Piscina* piscina,
    constans character* via,
                   i32  octeti_initiales,
  constans Multiplexor* m)
{
         FILE* plagula;
         long  mensura;
    character* copia;

    si (!f || !piscina || !via)
    {
        redde FALSUM;
    }
    memset(f, ZEPHYRUM, magnitudo(ColloquiorumFons));
    f->piscina      = piscina;
    f->multiplexor  = m;
    copia = (character*)piscina_allocare(piscina, (memoriae_index)
        strlen(via) + I);
    si (!copia)
    {
        redde FALSUM;
    }
    strcpy(copia, via);
    f->via     = copia;
    f->status  = colloquia_status_creare(piscina);
    f->tituli  = xar_creare(piscina,
        (i32)magnitudo(TitulusSessionis));
    f->scheda  = piscina_generare_dynamicum("colloquia_fons",
        LXIV * M);
    f->panellorum  = piscina_generare_dynamicum("colloquia_panelli",
        XVI * M);
    si (!f->status || !f->tituli || !f->scheda || !f->panellorum)
    {
        redde FALSUM;
    }
    /* positio ANTE caudam (ut sequax admonitionum) */
    plagula = lectiones_fopen(via, "rb");
    si (plagula)
    {
        (vacuum)fseek(plagula, 0L, SEEK_END);
        mensura     = ftell(plagula);
        f->positio  = mensura > 0L ? (s64)mensura : ZEPHYRUM;
        fclose(plagula);
    }
    (vacuum)colloquia_status_applicare(f->status,
        praeconium_caudam_legere(via, octeti_initiales, f->scheda));
    piscina_vacare(f->scheda);
    redde VERUM;
}

/* FNV-1a servitorum et idorum panellorum (mutatio vivorum) */
interior i64
fnv_addere (
       i64 h,
    chorda c)
{
    i32 j;

    per (j = ZEPHYRUM; j < c.mensura; j++)
    {
        h ^= (i64)c.datum[j];
        h *= (i64)CDIII;
    }
    h ^= (i64)'|';
    h *= (i64)CDIII;
    redde h;
}

interior i64
signum_panellorum (
    Xar* ministri)
{
        constans PanelliMinistri* ps;
    constans MultiplexorPanellus* p;
                             i64  h;
                             i32  i;
                             i32  j;

    h = (i64)CCLXI * (i64)M;   /* semen quodvis */
    per (i = ZEPHYRUM; ministri && i < xar_numerus(ministri); i++)
    {
        ps  = (constans PanelliMinistri*)xar_obtinere(ministri, i);
        h   = fnv_addere(h, ps->servitor);
        per (j = ZEPHYRUM; ps->panelli && j < xar_numerus(ps->panelli);
             j++)
        {
            p = (constans MultiplexorPanellus*)xar_obtinere(ps->panelli,
                j);
            h = fnv_addere(h, p->id);
        }
    }
    redde h;
}

interior PanelliMinistri*
ministrum_invenire (
       Xar* ministri,
    chorda  servitor)
{
      PanelliMinistri* ps;
                  i32  i;

    per (i = ZEPHYRUM; ministri && i < xar_numerus(ministri); i++)
    {
        ps = (PanelliMinistri*)xar_obtinere(ministri, i);
        si (chorda_aequalis(ps->servitor, servitor))
        {
            redde ps;
        }
    }
    redde NIHIL;
}

/* panelli per servitorem quemque quem sessiones nominant (pane ids
 * per servitorem: %1 in servitore alio alius est) */
interior Xar*
panellos_legere (
    ColloquiorumFons* f)
{
                    Xar* ministri;
                    Xar* l;
    constans Colloquium* c;
        PanelliMinistri* ps;
            Multiplexor  mx;
                    i32  i;

    ministri = xar_creare(f->panellorum,
        (i32)magnitudo(PanelliMinistri));
    l = colloquia_status_legere(f->status, f->scheda);
    per (i = ZEPHYRUM; ministri && l && i < xar_numerus(l); i++)
    {
        c = (constans Colloquium*)xar_obtinere(l, i);
        si (   c->servitor.mensura == ZEPHYRUM
            || ministrum_invenire(ministri, c->servitor))
        {
            perge;
        }
        ps = (PanelliMinistri*)xar_addere(ministri);
        si (!ps)
        {
            frange;
        }
        ps->servitor  = chorda_transcribere(c->servitor, f->panellorum);
        mx            = *f->multiplexor;
        mx.socket     = chorda_ut_cstr(ps->servitor, f->panellorum);
        ps->panelli   = multiplexor_panelli(&mx, f->panellorum);
    }
    redde ministri;
}

b32
colloquia_visus_fontem_pulsare (
    ColloquiorumFons* f,
                 s64  nunc)
{
    Xar* novi;
    i64  signum;
    b32  mutatum;

    si (!f || !f->scheda)
    {
        redde FALSUM;
    }
    mutatum = FALSUM;
    piscina_vacare(f->scheda);
    novi = praeconium_legere_ab(f->via, &f->positio, f->scheda);
    si (novi && xar_numerus(novi) > ZEPHYRUM)
    {
        (vacuum)colloquia_status_applicare(f->status, novi);
        mutatum = VERUM;
    }
    si (   f->multiplexor
        && (!f->panelli
        || nunc - f->panelli_ad >= COLLOQUIA_PANELLI_MS))
    {
        piscina_vacare(f->panellorum);
        f->panelli     = panellos_legere(f);
        f->panelli_ad  = nunc;
        signum         = signum_panellorum(f->panelli);
        si (signum != f->signum_panellorum)
        {
            f->signum_panellorum  = signum;
            mutatum               = VERUM;
        }
    }
    redde mutatum;
}

/* nomen directorii ultimum ("/w/alpha" -> "alpha") */
interior chorda
directorium_ultimum (
    chorda via)
{
    i32 i;

    dum (via.mensura > ZEPHYRUM && via.datum[via.mensura - I] == '/')
    {
        via.mensura--;
    }
    per (i = via.mensura; i > ZEPHYRUM && via.datum[i - I] != '/'; i--)
    {
    }
    via.datum    += i;
    via.mensura  -= i;
    redde via;
}

interior chorda
titulum_sessionis (
          ColloquiorumFons* f,
       constans Colloquium* c)
{
    TitulusSessionis* t;
              chorda  titulus;
                 i32  i;

    t = NIHIL;
    per (i = ZEPHYRUM; i < xar_numerus(f->tituli); i++)
    {
        t = (TitulusSessionis*)xar_obtinere(f->tituli, i);
        si (chorda_aequalis(t->sessio, c->sessio))
        {
            frange;
        }
        t = NIHIL;
    }
    si (!t)
    {
        t = (TitulusSessionis*)xar_addere(f->tituli);
        si (!t)
        {
            redde directorium_ultimum(c->via);
        }
        t->sessio = chorda_transcribere(c->sessio, f->piscina);
        t->titulus.datum = NIHIL;
        t->titulus.mensura = ZEPHYRUM;
        t->tempus_conatus = -I;
    }
    si (   t->titulus.mensura == ZEPHYRUM
        && c->tempus > t->tempus_conatus
        && c->transcriptum.mensura > ZEPHYRUM)
    {
        t->tempus_conatus  = c->tempus;
        titulus            = colloquia_titulum_legere(chorda_ut_cstr(
            c->transcriptum, f->scheda), f->scheda);
        si (titulus.mensura > ZEPHYRUM)
        {
            t->titulus = chorda_transcribere(titulus, f->piscina);
        }
    }
    si (t->titulus.mensura > ZEPHYRUM)
    {
        redde t->titulus;
    }
    titulus = directorium_ultimum(c->via);
    redde titulus.mensura > ZEPHYRUM ? titulus : c->sessio;
}

Xar*
colloquia_visus_fontis (
    ColloquiorumFons* f,
             Piscina* piscina)
{
           Xar* l;
    Colloquium* c;
           i32  i;

    si (!f || !f->status)
    {
        redde NIHIL;
    }
    l = colloquia_status_legere(f->status, piscina);
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        c           = (Colloquium*)xar_obtinere(l, i);
        c->titulus  = titulum_sessionis(f, c);
    }
    /* vivi: panellus in servitore SUO; sine servitore non iudicatur */
    per (i = ZEPHYRUM; l && f->panelli && i < xar_numerus(l); i++)
    {
        constans PanelliMinistri* ps;

        c   = (Colloquium*)xar_obtinere(l, i);
        ps  = ministrum_invenire(f->panelli, c->servitor);
        si (!ps)
        {
            perge;
        }
        c->panellus_vivit = ps->panelli
            && multiplexor_panellum_invenire(
            ps->panelli, c->panellus) != NIHIL;
        si (!c->panellus_vivit)
        {
            c->status = COLLOQUIUM_CLAUSUM;
        }
    }
    redde l;
}


/* ==================================================
 * Status (insula)
 * ================================================== */

/* figura: contextus privatus */
nomen structura {
    ColloquiorumMontatio* m;
                 Piscina* scheda;
                     s32  latitudo_montationis;
                     s32  altitudo_montationis;
} ColloquiorumFigurae;

interior s32
attributum_s32 (
    constans InsulaRamus* ramus,
      constans character* titulus,
                     s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = insula_ramus_attributum(ramus, titulus);
    si (a && chorda_ut_s32(*a, &v) && v >= ZEPHYRUM)
    {
        redde v;
    }
    redde praestitutum;
}

interior b32
clausae_ostensae (
    constans ColloquiorumMontatio* m)
{
    chorda* a;

    a = insula_ramus_attributum(&m->ramus, "clausa");
    redde a && chorda_aequalis_literis(*a, "verum");
}

nomen structura {
    constans character* titulus;
    constans character* valor;
} ParCampi;

interior vacuum
campum_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    ParCampi* par;

    par = (ParCampi*)ctx;
    si (!par->valor || !par->valor[ZEPHYRUM])
    {
        (vacuum)insula_attributum_tollere(radix, par->titulus);
        redde;
    }
    insula_attributum_ponere(radix, p, in, par->titulus, par->valor);
}

nomen structura {
    ParCampi* pares;
         i32  numerus;
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
        campum_ponere(radix, p, in, (vacuum*)(grex->pares + i));
    }
}

/* duo campi actu uno, scriptore visus */
interior b32
campos_scribere (
    ColloquiorumMontatio* m,
      constans character* titulus_primus,
      constans character* valor_primus,
      constans character* titulus_secundus,
      constans character* valor_secundus)
{
        ParCampi pares[II];
    GrexCamporum grex;
          chorda prior;
             b32 ok;

    pares[ZEPHYRUM].titulus  = titulus_primus;
    pares[ZEPHYRUM].valor    = valor_primus;
    pares[I].titulus         = titulus_secundus;
    pares[I].valor           = valor_secundus;
    grex.pares               = pares;
    grex.numerus             = titulus_secundus ? II : I;
    prior                    = m->ramus.repo->scriptor;
    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        actio_colloquiorum, m->piscina));
    ok = mutare_ramum(&m->ramus, campos_ponere, &grex);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

interior s32
superficies (
    constans ColloquiorumMontatio* m,
                              b32  latitudo)
{
    constans ColloquiorumFigurae* f;
                             s32  v;

    f = (constans ColloquiorumFigurae*)m->figurae_ctx;
    v = !f ? ZEPHYRUM : latitudo ? f->latitudo_montationis
                                 : f->altitudo_montationis;
    si (v <= ZEPHYRUM)
    {
        v = latitudo ? COLLOQUIA_LATITUDO : COLLOQUIA_ALTITUDO;
    }
    redde attributum_s32(&m->ramus, latitudo ? "superficies_latitudo"
        : "superficies_altitudo", v);
}


/* ==================================================
 * Index (album per greges)
 * ================================================== */

/* greges ordine: te eget, laborat, perfecit, otiosum (et ignotum),
 * clausum */
hic_manens constans StatusColloquii ordo_gregum[V] = {
    COLLOQUIUM_EXSPECTAT, COLLOQUIUM_LABORAT, COLLOQUIUM_PERFECIT,
    COLLOQUIUM_OTIOSUM, COLLOQUIUM_CLAUSUM
};

hic_manens constans character* tituli_gregum[V] = {
    "Needs you", "Working", "Done", "Idle", "Closed"
};

interior StatusColloquii
grex_statuis (
    StatusColloquii s)
{
    redde s == COLLOQUIUM_IGNOTUM ? COLLOQUIUM_OTIOSUM : s;
}

interior s32
capacitas (
    constans ColloquiorumMontatio* m)
{
    redde superficies(m, FALSUM) / COLLOQUIA_LINEA - II
        - COLLOQUIA_TITULI - COLLOQUIA_CAPUT;
}

/* ordines: tituli gregum "g<k>", res "r<n>" (n = index + 1 in l) */
interior Album*
ordines (
    constans ColloquiorumMontatio* m,
                              Xar* l,
                          Piscina* p)
{
     Album* a;
       i32  g;
       i32  i;
       b32  titulus;
 character  id[XVI];

    a = album_creare(p, capacitas(m));
    per (g = ZEPHYRUM; a && g < V; g++)
    {
        si (   ordo_gregum[g] == COLLOQUIUM_CLAUSUM
            && !clausae_ostensae(m))
        {
            perge;
        }
        titulus = FALSUM;
        per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
        {
            si (grex_statuis(((constans Colloquium*)xar_obtinere(l,
                i))->status) != ordo_gregum[g])
            {
                perge;
            }
            si (!titulus)
            {
                sprintf(id, "g%d", (integer)g);
                (vacuum)album_addere(a, id, ALBUM_TITULUS,
                    COLLOQUIA_GREX, ZEPHYRUM);
                titulus = VERUM;
            }
            sprintf(id, "r%d", (integer)(i + I));
            (vacuum)album_addere(a, id, ALBUM_RES, COLLOQUIA_ORDO,
                (s32)(i + I));
        }
    }
    redde a;
}

interior s32
pagina_currens (
    constans ColloquiorumMontatio* m,
                            Album* a)
{
    redde album_pagina_coacta(a, attributum_s32(&m->ramus, "pagina",
        ZEPHYRUM));
}

/* res electa (index + 1 in l): campus electa si in albo, aliter prima
 * paginae; 0 si nulla */
interior s32
electa (
    constans ColloquiorumMontatio* m,
                              Xar* l,
                            Album* a)
{
     chorda* e;
        i32  i;
        s32  n;

    e = insula_ramus_attributum(&m->ramus, "electa");
    per (i = ZEPHYRUM; e && l && i < xar_numerus(l); i++)
    {
        si (chorda_aequalis(((constans Colloquium*)xar_obtinere(l,
            i))->sessio, *e))
        {
            n = (s32)(i + I);
            si (album_pagina_rei(a, n, ZEPHYRUM) >= ZEPHYRUM)
            {
                redde n;
            }
        }
    }
    redde album_res_prima(a, pagina_currens(m, a));
}

interior b32
electionem_scribere (
    ColloquiorumMontatio* m,
                     Xar* l,
                   Album* a,
                     s32  res,
                 Piscina* p)
{
    constans Colloquium* c;
                    s32  k;
              character  pagina[XVI];

    si (res <= ZEPHYRUM || !l || res > (s32)xar_numerus(l))
    {
        redde FALSUM;
    }
    c = (constans Colloquium*)xar_obtinere(l, (i32)(res - I));
    k = album_pagina_rei(a, res, pagina_currens(m, a));
    sprintf(pagina, "%d", (integer)(k < ZEPHYRUM ? ZEPHYRUM : k));
    redde campos_scribere(m, "electa", chorda_ut_cstr(c->sessio, p),
        "pagina", k > ZEPHYRUM ? pagina : "");
}

/* id "<littera><n>" -> n; 0 si alienum */
interior s32
numerus_id (
                chorda id,
    constans character littera)
{
    s32 n;

    si (id.mensura < II || id.datum[ZEPHYRUM] != (i8)littera)
    {
        redde ZEPHYRUM;
    }
    id.datum++;
    id.mensura--;
    redde chorda_ut_s32(id, &n) && n > ZEPHYRUM ? n : ZEPHYRUM;
}


/* ==================================================
 * Praecepta
 * ================================================== */

#define VISUS_INDICIS      I
#define REQUIRIT_ELECTAM   I

nomen structura {
    ColloquiorumMontatio* m;
                 Piscina* p;
                     Xar* l;
                   Album* a;
                     s32  electa;
} ContextusPraecepti;

interior b32
requisita_colloquiorum (
        i32  requisita,
     vacuum* ctx)
{
    ContextusPraecepti* cp;

    cp = (ContextusPraecepti*)ctx;
    redde !(requisita & REQUIRIT_ELECTAM) || cp->electa > ZEPHYRUM;
}

/* gradus +1 / -1 inter res albi (tituli gregum praetereuntur) */
interior b32
movere (
    ContextusPraecepti* cp,
                   s32  gradus)
{
    constans AlbumOrdo* o;
                   s32  i;
                   s32  n;
                   s32  index;

    n      = (s32)album_numerus(cp->a);
    index  = -I;
    per (i = ZEPHYRUM; i < n; i++)
    {
        o = album_ordo(cp->a, (i32)i);
        si (o->genus == ALBUM_RES && o->res == cp->electa)
        {
            index = i;
            frange;
        }
    }
    per (i = index + gradus; i >= ZEPHYRUM && i < n; i += gradus)
    {
        o = album_ordo(cp->a, (i32)i);
        si (o->genus == ALBUM_RES)
        {
            redde electionem_scribere(cp->m, cp->l, cp->a, o->res,
                cp->p);
        }
    }
    redde VERUM;
}

interior b32
fac_proximam (
    vacuum* ctx)
{
    redde movere((ContextusPraecepti*)ctx, I);
}

interior b32
fac_priorem (
    vacuum* ctx)
{
    redde movere((ContextusPraecepti*)ctx, -I);
}

interior b32
paginam_vertere (
    ContextusPraecepti* cp,
                   s32  gradus)
{
    s32 k;

    k = pagina_currens(cp->m, cp->a) + gradus;
    si (k < ZEPHYRUM || k >= album_paginae(cp->a))
    {
        redde VERUM;
    }
    redde electionem_scribere(cp->m, cp->l, cp->a,
        album_res_prima(cp->a,
        k), cp->p);
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

interior b32
fac_clausas (
    vacuum* ctx)
{
    ContextusPraecepti* cp;

    cp = (ContextusPraecepti*)ctx;
    redde campos_scribere(cp->m, "clausa", clausae_ostensae(cp->m)
        ? "falsum" : "verum", "pagina", "");
}

interior b32
fac_agere (
    vacuum* ctx)
{
    ContextusPraecepti* cp;

    cp = (ContextusPraecepti*)ctx;
    si (cp->m->agere)
    {
        (vacuum)cp->m->agere(cp->m->agere_ctx,
            (constans Colloquium*)xar_obtinere(cp->l,
            (i32)(cp->electa - I)));
    }
    redde VERUM;
}

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "colloquia.proxima", "Next session", "j", VISUS_INDICIS,
        ZEPHYRUM, fac_proximam },
    { "colloquia.prior", "Previous session", "k", VISUS_INDICIS,
        ZEPHYRUM, fac_priorem },
    { "colloquia.pagina_prior", "Previous page", "[", VISUS_INDICIS,
        ZEPHYRUM, fac_paginam_priorem },
    { "colloquia.pagina_proxima", "Next page", "]", VISUS_INDICIS,
        ZEPHYRUM, fac_paginam_proximam },
    { "colloquia.clausae", "Show / hide closed", "c", VISUS_INDICIS,
        ZEPHYRUM, fac_clausas },
    { "colloquia.agere", "Go to session", "Enter", VISUS_INDICIS,
        REQUIRIT_ELECTAM, fac_agere },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

interior vacuum
contextum_parare (
    ColloquiorumMontatio* m,
                 Piscina* p,
      ContextusPraecepti* cp)
{
    cp->m       = m;
    cp->p       = p;
    cp->l       = colloquia_visus_fontis(m->fons, p);
    cp->a       = ordines(m, cp->l, p);
    cp->electa  = cp->a ? electa(m, cp->l, cp->a) : ZEPHYRUM;
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
    ColloquiorumMontatio* m;
     ColloquiorumFigurae* privatum;
     constans Praeceptum* t;
      ContextusPraecepti  cp;
                     s32  n;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)des;
    m         = (ColloquiorumMontatio*)ctx;
    privatum  = (ColloquiorumFigurae*)m->figurae_ctx;
    si (!privatum || !m->fons)
    {
        redde FALSUM;
    }
    piscina_vacare(privatum->scheda);
    si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        t = praecepta_ad_clavem(TABULA_PRAECEPTORUM, VISUS_INDICIS, ev);
        si (!t)
        {
            redde FALSUM;
        }
        contextum_parare(m, privatum->scheda, &cp);
        redde cp.a && praecepta_status(t, VISUS_INDICIS,
            requisita_colloquiorum, &cp) == PRAECEPTUM_PRAESTO
            && t->fn(&cp);
    }
    si (   !nodus || ev->datum.mus.botton != MUS_SINISTER
        || (   ev->genus != EVENTUS_MUS_DEPRESSUS
            && ev->genus != EVENTUS_MUS_DUPLEX))
    {
        redde FALSUM;
    }
    contextum_parare(m, privatum->scheda, &cp);
    si (!cp.a)
    {
        redde FALSUM;
    }
    n = nodus->partes == PARTES_ITEM ? numerus_id(nodus->id, 'r')
                                     : ZEPHYRUM;
    si (n > ZEPHYRUM)
    {
        si (!electionem_scribere(m, cp.l, cp.a, n, privatum->scheda))
        {
            redde FALSUM;
        }
        si (ev->genus == EVENTUS_MUS_DUPLEX)
        {
            cp.electa = n;
            redde fac_agere(&cp);
        }
        redde VERUM;
    }
    si (   ev->genus == EVENTUS_MUS_DEPRESSUS
        && (   chorda_aequalis_literis(nodus->id, "pagina-prior")
            || chorda_aequalis_literis(nodus->id, "pagina-proxima")))
    {
        redde paginam_vertere(&cp, chorda_aequalis_literis(nodus->id,
            "pagina-prior") ? -I : I);
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

    n = latitudo / COLLOQUIA_LITTERA;
    si (n < ZEPHYRUM)
    {
        n = ZEPHYRUM;
    }
    si (textus.mensura > (i32)n)
    {
        textus.mensura = (i32)n;
    }
    redde textus;
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

/* signum: octagonum VI x VI (ut admonitiones) */
interior vacuum
signum_pingere (
       Mandata* md,
           s32  x,
           s32  y,
           b32  plenum,
    ColorThema  color)
{
    Punctum p[VIII];

    p[ZEPHYRUM].x  = x + II;
    p[ZEPHYRUM].y  = y;
    p[I].x         = x + IV;
    p[I].y         = y;
    p[II].x        = x + VI;
    p[II].y        = y + II;
    p[III].x       = x + VI;
    p[III].y       = y + IV;
    p[IV].x        = x + IV;
    p[IV].y        = y + VI;
    p[V].x         = x + II;
    p[V].y         = y + VI;
    p[VI].x        = x;
    p[VI].y        = y + IV;
    p[VII].x       = x;
    p[VII].y       = y + II;
    mandata_polygonum(md, p, VIII, figura_color_thematis(color),
        plenum);
}

interior chorda
aetas (
    constans ColloquiorumMontatio* m,
                              s64  tempus,
                          Piscina* p)
{
          s64 s;
    character textus[XXXII];

    si (!m->nunc)
    {
        redde chorda_ex_literis("", p);
    }
    s = (m->nunc(m->nunc_ctx) - tempus) / (s64)M;
    si (s < (s64)LX)
    {
        redde chorda_ex_literis("now", p);
    }
    si (s < (s64)MMMDC)
    {
        sprintf(textus, "%ldm", (long)(s / (s64)LX));
    }
    alioquin si (s < (s64)LXXXVI * (s64)M + (s64)CD)
    {
        sprintf(textus, "%ldh", (long)(s / (s64)MMMDC));
    }
    alioquin
    {
        sprintf(textus, "%ldd", (long)(s / ((s64)LXXXVI * (s64)M
            + (s64)CD)));
    }
    redde chorda_ex_literis(textus, p);
}

/* ordo sessionis: electa fundo selectionis; signum coloris status,
 * titulus, aetas ad dextrum; linea status infra */
interior vacuum
ordinem_pingere (
     ColloquiorumMontatio* m,
                  Mandata* md,
       constans Componens* f,
      constans Colloquium* c,
                      b32  electa_est)
{
       chorda a;
          s32 spatium;
      Punctum p;
      Punctum q;
   ColorThema color;
          b32 plenum;

    si (electa_est)
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
    plenum  = VERUM;
    color   = COLOR_TEXT_DIM;
    si (c->status == COLLOQUIUM_EXSPECTAT)
    {
        color = COLOR_WARNING;
    }
    alioquin si (c->status == COLLOQUIUM_LABORAT)
    {
        /* accentus = successus (folium) in themate: textus plenus */
        color = COLOR_TEXT;
    }
    alioquin si (c->status == COLLOQUIUM_PERFECIT)
    {
        color = COLOR_SUCCESS;
    }
    alioquin
    {
        plenum = FALSUM;
    }
    signum_pingere(md, f->fines.x + IV, f->fines.y + V, plenum, color);
    spatium  = f->fines.latitudo - XVI;
    a        = aetas(m, c->tempus, md->piscina);
    si (a.mensura > ZEPHYRUM)
    {
        textum_pingere(md, f->fines.x + f->fines.latitudo - VI
            - (s32)a.mensura * COLLOQUIA_LITTERA, f->fines.y + IV, a,
            ZEPHYRUM, electa_est ? COLOR_TEXT : COLOR_TEXT_DIM);
        spatium -= (s32)(a.mensura + II) * COLLOQUIA_LITTERA;
    }
    textum_pingere(md, f->fines.x + XIV, f->fines.y + IV, praecidere(
        c->titulus, spatium), ZEPHYRUM, COLOR_TEXT);
    textum_pingere(md, f->fines.x + XIV, f->fines.y + IV + II
        * COLLOQUIA_LINEA - II, praecidere(c->linea, f->fines.latitudo
        - XX), ZEPHYRUM, electa_est ? COLOR_TEXT : COLOR_TEXT_DIM);
}

interior vacuum
figura_tabulae (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
    ColloquiorumFigurae* contextus;
   ColloquiorumMontatio* m;
                    Xar* l;
                  Album* a;
              Componens* f;
                  Fines  mensa;
                    s32  e;
                    s32  k;
                    s32  n;
                    s32  r;
                    i32  i;
                    i32  activae;
                    i32  clausae;
              character  textus[LXIV];

    (vacuum)thema;
    contextus       = (ColloquiorumFigurae*)ctx;
    mensa.x         = ZEPHYRUM;
    mensa.y         = ZEPHYRUM;
    mensa.latitudo  = c->fines.latitudo;
    mensa.altitudo  = c->fines.altitudo;
    figura_mensam_pingere(md, mensa, chorda_aequalis_literis(c->titulus,
        "focatum"));
    mensa.x         = COLLOQUIA_LITTERA;
    mensa.y         = COLLOQUIA_LINEA;
    mensa.latitudo  = c->fines.latitudo - II * COLLOQUIA_LITTERA;
    mensa.altitudo  = c->fines.altitudo - II * COLLOQUIA_LINEA;
    mandata_rectangulum(md, mensa, figura_color_thematis(
        COLOR_BACKGROUND), VERUM);
    si (!contextus || !contextus->m || !contextus->m->fons)
    {
        redde;
    }
    m = contextus->m;
    l = colloquia_visus_fontis(m->fons, md->piscina);
    a = ordines(m, l, md->piscina);
    si (!a)
    {
        redde;
    }
    e = electa(m, l, a);
    k = pagina_currens(m, a);
    n = album_paginae(a);
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        f = componens_liberum(c, i);
        si (chorda_aequalis_literis(f->id, "titulus"))
        {
            textum_pingere(md, f->fines.x + IV, f->fines.y + II,
                chorda_ex_literis("Sessions", md->piscina), FONS_TITULI,
                COLOR_TEXT);
        }
        alioquin si (chorda_aequalis_literis(f->id, "caput"))
        {
            i32 j;

            activae = ZEPHYRUM;
            clausae = ZEPHYRUM;
            per (j = ZEPHYRUM; l && j < xar_numerus(l); j++)
            {
                si (((constans Colloquium*)xar_obtinere(l, j))->status
                    == COLLOQUIUM_CLAUSUM)
                {
                    clausae++;
                }
                alioquin
                {
                    activae++;
                }
            }
            si (clausae > ZEPHYRUM && !clausae_ostensae(m))
            {
                sprintf(textus, "%d active, %d closed (c)",
                    (integer)activae, (integer)clausae);
            }
            alioquin
            {
                sprintf(textus, "%d active", (integer)activae);
            }
            textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis(textus, md->piscina), ZEPHYRUM,
                COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "index_paginae"))
        {
            si (n > I)
            {
                sprintf(textus, "%d/%d", (integer)(k + I), (integer)n);
                textum_pingere(md, f->fines.x + (f->fines.latitudo
                    - (s32)strlen(textus) * COLLOQUIA_LITTERA) / II,
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
        alioquin si (chorda_aequalis_literis(f->id, "nihil"))
        {
            textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis("No sessions", md->piscina), ZEPHYRUM,
                COLOR_TEXT_DIM);
        }
        alioquin si (   f->partes == PARTES_INDEX && f->id.mensura >= II
                     && f->id.datum[ZEPHYRUM] == 'g')
        {
            /* "g<k>" aut "g<k>-c" (iteratus): k cifra una */
            r = (s32)(f->id.datum[I] - '0');
            si (r >= ZEPHYRUM && r < V)
            {
                figura_fasciam_pingere(md, f->fines);
                figura_textum_audacem(md, f->fines.x + VI, f->fines.y
                    + IV, chorda_transcribere(chorda_ex_literis(
                    tituli_gregum[r], md->piscina), md->piscina),
                    figura_color_thematis(COLOR_TEXT));
            }
        }
        alioquin si (f->partes == PARTES_ITEM)
        {
            r = numerus_id(f->id, 'r');
            si (r > ZEPHYRUM && l && r <= (s32)xar_numerus(l))
            {
                ordinem_pingere(m, md, f, (constans Colloquium*)
                    xar_obtinere(l, (i32)(r - I)), r == e);
            }
        }
    }
}

interior b32
figuras_registrare (
     ColloquiorumMontatio* m,
                      i32  latitudo,
                      i32  altitudo)
{
    ColloquiorumFigurae* contextus;

    contextus = (ColloquiorumFigurae*)piscina_allocare(m->piscina,
        magnitudo(ColloquiorumFigurae));
    si (!contextus)
    {
        redde FALSUM;
    }
    contextus->m                     = m;
    contextus->scheda                = piscina_generare_dynamicum(
        "colloquia_scheda", XVI * M);
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
    componens_ponere_actio(c, actio_colloquiorum);
    l = (LocusComponentis*)xar_addere(dp->loci);
    si (l)
    {
        l->index  = index;
        l->c      = c;
    }
    redde c;
}

interior vacuum
indicem_disponere (
    ColloquiorumMontatio* m,
               Disponens* dp,
                     s32  radix_d,
                 Piscina* piscina)
{
                  Album* a;
                    Xar* l;
     constans AlbumOrdo* o;
                    s32  caput;
                    s32  regio;
                    s32  x;
                    s32  y;
                    s32  t;
                    s32  i;

    (vacuum)componentem_locare(dp, radix_d, "titulus", PARTES_TITULUS,
        DISPOSITIO_CRESCENS, ZEPHYRUM, COLLOQUIA_TITULI);
    caput = locum_addere(dp, radix_d, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
        COLLOQUIA_CAPUT);
    (vacuum)componentem_locare(dp, caput, "caput", PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, COLLOQUIA_CAPUT);
    (vacuum)componentem_locare(dp, caput, "pagina-prior",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, COLLOQUIA_CAPUT);
    (vacuum)componentem_locare(dp, caput, "index_paginae",
        PARTES_INDEX, DISPOSITIO_FIXA, VII, COLLOQUIA_CAPUT);
    (vacuum)componentem_locare(dp, caput, "pagina-proxima",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, COLLOQUIA_CAPUT);
    regio = locum_addere(dp, radix_d, DISPOSITIO_COLUMNA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    l = colloquia_visus_fontis(m->fons, piscina);
    a = ordines(m, l, piscina);
    si (!a || album_numerus(a) == ZEPHYRUM)
    {
        (vacuum)componentem_locare(dp, regio, "nihil", PARTES_INDEX,
            DISPOSITIO_CRESCENS, ZEPHYRUM, COLLOQUIA_ORDO);
        redde;
    }
    album_pagina(a, pagina_currens(m, a), &x, &y, &t);
    /* pagina in medio gregis: titulus iteratus ("<id>-c") */
    si (t >= ZEPHYRUM)
    {
        o = album_ordo(a, (i32)t);
        (vacuum)componentem_locare(dp, regio, chorda_ut_cstr(
            chorda_concatenare(o->id, chorda_ex_literis("-c", piscina),
            piscina), piscina), PARTES_INDEX, DISPOSITIO_CRESCENS,
            ZEPHYRUM, o->altitudo);
    }
    per (i = x; i < y; i++)
    {
        o = album_ordo(a, (i32)i);
        (vacuum)componentem_locare(dp, regio, chorda_ut_cstr(o->id,
            piscina), o->genus
                == ALBUM_RES ? PARTES_ITEM : PARTES_INDEX,
            DISPOSITIO_CRESCENS, ZEPHYRUM, o->altitudo);
    }
}

Componens*
colloquia_visus_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
    ColloquiorumMontatio* m;
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
    m = (ColloquiorumMontatio*)ctx;
    si (!m)
    {
        redde NIHIL;
    }
    latitudo = superficies(m, VERUM);
    altitudo = superficies(m, FALSUM);
    radix     = componens_creare(piscina, intern, "colloquia",
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
    componens_ponere_actio(radix, actio_colloquiorum);
    si (motus_ramus_focatus(motus, m->ramus))
    {
        componens_ponere_titulum(radix, "focatum");
    }
    columnae    = latitudo / COLLOQUIA_LITTERA;
    lineae      = altitudo / COLLOQUIA_LINEA;
    dp.d        = dispositio_creare(piscina);
    dp.loci     = xar_creare(piscina, (i32)magnitudo(LocusComponentis));
    dp.piscina  = piscina;
    dp.intern   = intern;
    si (!dp.d || !dp.loci || !m->fons)
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
    indicem_disponere(m, &dp, radix_d, piscina);
    dispositio_computare(dp.d, columnae, lineae, NIHIL, NIHIL);
    per (i = ZEPHYRUM; i < (s32)xar_numerus(dp.loci); i++)
    {
        LocusComponentis* loc;

        loc = (LocusComponentis*)xar_obtinere(dp.loci, (i32)i);
        componens_ponere_fines(loc->c, dispositio_ad_pixela(
            dispositio_fines(dp.d, loc->index), columnae, lineae,
            latitudo, altitudo, COLLOQUIA_LITTERA, COLLOQUIA_LINEA));
        componens_addere_liberum(radix, loc->c);
    }
    redde radix;
}


/* ==================================================
 * Montatio
 * ================================================== */

b32
colloquia_visus_reficere (
    ColloquiorumMontatio* m)
{
    si (!m || !m->fons)
    {
        redde FALSUM;
    }
    redde colloquia_visus_fontem_pulsare(m->fons, m->nunc
        ? m->nunc(m->nunc_ctx) : ZEPHYRUM);
}

b32
colloquia_visus_montare (
    ColloquiorumMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
        ColloquiorumFons* fons,
          ColloquiiActio  agere_porta,
                  vacuum* agere_ctx,
                     i32  latitudo,
                     i32  altitudo,
         StatusSystemata* systemata)
{
    Organum* organum;

    si (!m || !piscina || !intern || !repo)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(ColloquiorumMontatio));
    m->piscina    = piscina;
    m->intern     = intern;
    m->fons       = fons;
    m->agere      = agere_porta;
    m->agere_ctx  = agere_ctx;
    m->ramus      = id ? insula_ramus(repo, "colloquia", id)
                       : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "colloquia",
        "colloquia");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"colloquia\"", id ? id : "colloquia", piscina,
               intern))
    {
        redde FALSUM;
    }
    m->actiones  = actio_registrum_creare(piscina, intern);
    m->figurae   = figura_registrum_creare(piscina);
    redde m->actiones && m->figurae
        && praecepta_verificare(TABULA_PRAECEPTORUM)
        && actio_registrare(m->actiones, actio_colloquiorum, agere, m)
        && figuras_registrare(m, latitudo, altitudo);
}
