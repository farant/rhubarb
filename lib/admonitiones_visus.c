/* admonitiones_visus.c - centrum admonitionum et sequax libri praeconii
 * (include/admonitiones_visus.h; colloquia-plan T4a). Forma
 * nomenclatoris: arbor dispositionis in cellulis (LITTERA x LINEA),
 * figura radicis omnia pingit, praecepta per membrum__praecepta,
 * paginae per membrum__album. */

#include "admonitiones_visus.h"
#include "membrum__album.h"
#include "membrum__praecepta.h"
#include "colloquia.h"
#include "lectiones.h"
#include "dispositio.h"
#include "fons.h"
#include "thema.h"
#include "motus.h"

#include <stdio.h>
#include <string.h>

#define ADMONITIONES_LATITUDO  CCLXXX   /* superficies absens */
#define ADMONITIONES_ALTITUDO  CCLVI
#define ADMONITIONES_LITTERA   VI
#define ADMONITIONES_LINEA     VIII
#define ADMONITIONES_TITULI    III      /* lineae tituli */
#define ADMONITIONES_CAPUT     II       /* lineae capitis */
#define ADMONITIONES_ORDO      IV       /* lineae ordinis: titulus,
                                         * corpus */

hic_manens constans character actio_admonitionum[] =
    "admonitiones.agere";

/* figura: contextus privatus */
nomen structura {
    AdmonitionumMontatio* m;
                 Piscina* scheda;
                     s32  latitudo_montationis;
                     s32  altitudo_montationis;
} AdmonitionumFigurae;


/* ==================================================
 * Sequax
 * ================================================== */

nomen structura {
    chorda sessio;
    chorda titulus;
} TitulusSessionis;

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

/* titulus sessionis: memoria, aliter transcriptum (servatur), aliter
 * nomen directorii (non servatur: ai-title postea venire potest) */
interior chorda
titulum_sessionis (
            AdmonitionumSequax* s,
    constans PraeconiumEventus* e)
{
    TitulusSessionis* t;
              chorda  titulus;
                 i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(s->tituli); i++)
    {
        t = (TitulusSessionis*)xar_obtinere(s->tituli, i);
        si (chorda_aequalis(t->sessio, e->sessio))
        {
            redde t->titulus;
        }
    }
    titulus = chorda_ex_literis("", s->scheda);
    si (e->transcriptum.mensura > ZEPHYRUM)
    {
        titulus = colloquia_titulum_legere(chorda_ut_cstr(
            e->transcriptum, s->scheda), s->scheda);
    }
    si (titulus.mensura > ZEPHYRUM && e->sessio.mensura > ZEPHYRUM)
    {
        t = (TitulusSessionis*)xar_addere(s->tituli);
        si (t)
        {
            t->sessio   = chorda_transcribere(e->sessio, s->piscina);
            t->titulus  = chorda_transcribere(titulus, s->piscina);
            redde t->titulus;
        }
        redde titulus;
    }
    titulus = directorium_ultimum(e->via);
    redde titulus.mensura > ZEPHYRUM ? titulus : e->sessio;
}

/* eventus -> admonitiones; numerus additarum; *recentissima = id
 * recentissimae additae (in piscina indicis) */
interior i32
eventus_addere (
    AdmonitionumSequax* s,
                   Xar* eventus,
                chorda* recentissima)
{
    constans PraeconiumEventus* e;
                     Admonitio  x;
                        chorda  vacua;
                           s64  optimum;
                     character  id[CXXVIII];
                           i32  n;
                           i32  i;

    vacua         = chorda_ex_literis("", s->scheda);
    optimum       = -I;
    n             = ZEPHYRUM;
    id[ZEPHYRUM]  = '\0';
    per (i = ZEPHYRUM; eventus && i < xar_numerus(eventus); i++)
    {
        e = (constans PraeconiumEventus*)xar_obtinere(eventus, i);
        /* qualis? (sine titulo: transcriptum non legitur frustra) */
        si (!admonitio_ex_eventu(e, vacua, &x, s->scheda))
        {
            perge;
        }
        x.titulus = titulum_sessionis(s, e);
        si (!admonitiones_addere(s->index, &x))
        {
            perge;
        }
        n++;
        si (x.tempus > optimum && x.id.mensura < CXXVIII)
        {
            optimum = x.tempus;
            memcpy(id, x.id.datum, (size_t)x.id.mensura);
            id[x.id.mensura] = '\0';
        }
    }
    si (recentissima)
    {
        *recentissima = chorda_ex_literis("", s->scheda);
        per (i = ZEPHYRUM; n > ZEPHYRUM
                           && i < admonitiones_numerus(s->index); i++)
        {
            si (chorda_aequalis_literis(admonitio(s->index, i)->id, id))
            {
                *recentissima = admonitio(s->index, i)->id;
                frange;
            }
        }
    }
    redde n;
}

b32
admonitiones_sequacem_initiare (
    AdmonitionumSequax* s,
               Piscina* piscina,
          Admonitiones* index,
    constans character* via,
                   i32  octeti_initiales)
{
         FILE* f;
         long  mensura;
    character* copia;

    si (!s || !piscina || !index || !via)
    {
        redde FALSUM;
    }
    memset(s, ZEPHYRUM, magnitudo(AdmonitionumSequax));
    s->piscina  = piscina;
    copia       = (character*)piscina_allocare(piscina,
        (memoriae_index)strlen(via) + I);
    si (!copia)
    {
        redde FALSUM;
    }
    strcpy(copia, via);
    s->via     = copia;
    s->index   = index;
    s->tituli  = xar_creare(piscina, (i32)magnitudo(TitulusSessionis));
    s->scheda  = piscina_generare_dynamicum("admonitiones_sequax",
        LXIV * M);
    si (!s->index || !s->tituli || !s->scheda)
    {
        redde FALSUM;
    }
    /* positio ANTE caudam: quod post eam scribitur sequi legit (iterata
     * per id ignorantur) */
    f = lectiones_fopen(via, "rb");
    si (f)
    {
        (vacuum)fseek(f, 0L, SEEK_END);
        mensura     = ftell(f);
        s->positio  = mensura > 0L ? (s64)mensura : ZEPHYRUM;
        fclose(f);
    }
    (vacuum)eventus_addere(s, praeconium_caudam_legere(via,
        octeti_initiales, s->scheda), NIHIL);
    piscina_vacare(s->scheda);
    redde VERUM;
}

i32
admonitiones_sequi (
    AdmonitionumSequax* s,
                chorda* recentissima)
{
    si (!s || !s->scheda)
    {
        redde ZEPHYRUM;
    }
    piscina_vacare(s->scheda);
    redde eventus_addere(s, praeconium_legere_ab(s->via, &s->positio,
        s->scheda), recentissima);
}

i32
admonitiones_lectas_notare_ad (
    Admonitiones* a,
             s64  tempus)
{
    constans Admonitio* x;
                   i32  n;
                   i32  i;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < admonitiones_numerus(a); i++)
    {
        x = admonitio(a, i);
        si (   x->tempus <= tempus && !x->lecta
            && admonitionem_legere_notare(a, x->id))
        {
            n++;
        }
    }
    redde n;
}


/* ==================================================
 * Status (insula)
 * ================================================== */

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

/* duo campi actu uno in ramo dato (visus aut radix), scriptore
 * centri */
interior b32
campos_scribere (
       AdmonitionumMontatio* m,
       constans InsulaRamus* ramus,
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
        actio_admonitionum, m->piscina));
    ok = mutare_ramum(ramus, campos_ponere, &grex);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

/* superficies: attributum rami (vicus), aliter montationis, aliter
 * ordinaria */
interior s32
superficies (
    constans AdmonitionumMontatio* m,
                              b32  latitudo)
{
    constans AdmonitionumFigurae* f;
                             s32  v;

    f = (constans AdmonitionumFigurae*)m->figurae_ctx;
    v = !f ? ZEPHYRUM : latitudo ? f->latitudo_montationis
                                 : f->altitudo_montationis;
    si (v <= ZEPHYRUM)
    {
        v = latitudo ? ADMONITIONES_LATITUDO : ADMONITIONES_ALTITUDO;
    }
    redde attributum_s32(&m->ramus, latitudo ? "superficies_latitudo"
        : "superficies_altitudo", v);
}


/* ==================================================
 * Index (album)
 * ================================================== */

/* capacitas in cellulis: altitudo minus margines (II), titulus,
 * caput */
interior s32
capacitas (
    constans AdmonitionumMontatio* m)
{
    redde superficies(m, FALSUM) / ADMONITIONES_LINEA - II
        - ADMONITIONES_TITULI - ADMONITIONES_CAPUT;
}

/* ordines: res = index + 1, id "r<res>" */
interior Album*
ordines (
    constans AdmonitionumMontatio* m,
                          Piscina* p)
{
        Album* l;
          i32  i;
    character  id[XVI];

    l = album_creare(p, capacitas(m));
    per (i = ZEPHYRUM; l && i < admonitiones_numerus(m->index); i++)
    {
        sprintf(id, "r%d", (integer)(i + I));
        (vacuum)album_addere(l, id, ALBUM_RES, ADMONITIONES_ORDO,
            (s32)(i + I));
    }
    redde l;
}

interior s32
pagina_currens (
    constans AdmonitionumMontatio* m,
                            Album* l)
{
    redde album_pagina_coacta(l, attributum_s32(&m->ramus, "pagina",
        ZEPHYRUM));
}

/* index electae: campus electa si in indice, aliter prima paginae; -1
 * si nulla */
interior s32
electa (
    constans AdmonitionumMontatio* m,
                            Album* l)
{
     chorda* a;
        i32  i;

    a = insula_ramus_attributum(&m->ramus, "electa");
    per (i = ZEPHYRUM; a && i < admonitiones_numerus(m->index); i++)
    {
        si (chorda_aequalis(admonitio(m->index, i)->id, *a))
        {
            redde (s32)i;
        }
    }
    redde l ? album_res_prima(l, pagina_currens(m, l)) - I : -I;
}

/* electio: electa (id) et pagina eius */
interior b32
electionem_scribere (
     AdmonitionumMontatio* m,
                    Album* l,
                      s32  index,
                  Piscina* p)
{
    constans Admonitio* x;
                   s32  k;
             character  pagina[XVI];

    x = admonitio(m->index, (i32)index);
    si (!x)
    {
        redde FALSUM;
    }
    k = album_pagina_rei(l, index + I, pagina_currens(m, l));
    sprintf(pagina, "%d", (integer)(k < ZEPHYRUM ? ZEPHYRUM : k));
    redde campos_scribere(m, &m->ramus, "electa", chorda_ut_cstr(x->id,
        p), "pagina", k > ZEPHYRUM ? pagina : "");
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


/* ==================================================
 * Praecepta
 * ================================================== */

#define VISUS_INDICIS      I
#define REQUIRIT_ELECTAM   I

nomen structura {
    AdmonitionumMontatio* m;
                 Piscina* p;
                   Album* l;
                     s32  electa;
} ContextusPraecepti;

interior b32
requisita_admonitionum (
        i32  requisita,
     vacuum* ctx)
{
    ContextusPraecepti* cp;

    cp = (ContextusPraecepti*)ctx;
    redde !(requisita & REQUIRIT_ELECTAM) || cp->electa >= ZEPHYRUM;
}

interior b32
movere (
    ContextusPraecepti* cp,
                   s32  gradus)
{
    s32 i;

    i = cp->electa + gradus;
    si (i < ZEPHYRUM || i >= (s32)admonitiones_numerus(cp->m->index))
    {
        redde VERUM;   /* in fine: consumpta, nihil */
    }
    redde electionem_scribere(cp->m, cp->l, i, cp->p);
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

/* pagina k: res prima electa */
interior b32
paginam_vertere (
    ContextusPraecepti* cp,
                   s32  gradus)
{
    s32 k;

    k = pagina_currens(cp->m, cp->l) + gradus;
    si (k < ZEPHYRUM || k >= album_paginae(cp->l))
    {
        redde VERUM;
    }
    redde electionem_scribere(cp->m, cp->l, album_res_prima(cp->l, k)
        - I, cp->p);
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
fac_legere (
    vacuum* ctx)
{
    ContextusPraecepti* cp;

    cp = (ContextusPraecepti*)ctx;
    (vacuum)admonitionem_legere_notare(cp->m->index, admonitio(
        cp->m->index, (i32)cp->electa)->id);
    redde VERUM;
}

/* X: omnes lectae; campus radicis (persistens in vico) */
interior b32
fac_omnes_legere (
    vacuum* ctx)
{
      ContextusPraecepti* cp;
      constans Admonitio* x;
             InsulaRamus  radix;
               character  tempus[XXXII];

    cp  = (ContextusPraecepti*)ctx;
    x   = admonitio(cp->m->index, ZEPHYRUM);
    si (!x)
    {
        redde VERUM;
    }
    (vacuum)admonitiones_lectas_notare_ad(cp->m->index, x->tempus);
    sprintf(tempus, "%ld", (long)x->tempus);
    radix = insula_ramus_radix(cp->m->ramus.repo);
    redde campos_scribere(cp->m, &radix, "admonitiones_lectae_ad",
        tempus, NIHIL, NIHIL);
}

/* Enter: porta actionis, deinde lecta */
interior b32
fac_agere (
    vacuum* ctx)
{
    ContextusPraecepti* cp;
    constans Admonitio* x;

    cp  = (ContextusPraecepti*)ctx;
    x   = admonitio(cp->m->index, (i32)cp->electa);
    si (cp->m->agere)
    {
        (vacuum)cp->m->agere(cp->m->agere_ctx, x);
    }
    (vacuum)admonitionem_legere_notare(cp->m->index, x->id);
    redde VERUM;
}

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "admonitiones.proxima", "Next notification", "j", VISUS_INDICIS,
        ZEPHYRUM, fac_proximam },
    { "admonitiones.prior", "Previous notification", "k",
        VISUS_INDICIS, ZEPHYRUM, fac_priorem },
    { "admonitiones.pagina_prior", "Previous page", "[", VISUS_INDICIS,
        ZEPHYRUM, fac_paginam_priorem },
    { "admonitiones.pagina_proxima", "Next page", "]", VISUS_INDICIS,
        ZEPHYRUM, fac_paginam_proximam },
    { "admonitiones.legere", "Mark read", "x", VISUS_INDICIS,
        REQUIRIT_ELECTAM, fac_legere },
    { "admonitiones.omnes_legere", "Mark all read", "X", VISUS_INDICIS,
        ZEPHYRUM, fac_omnes_legere },
    { "admonitiones.agere", "Go to session", "Enter", VISUS_INDICIS,
        REQUIRIT_ELECTAM, fac_agere },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

interior vacuum
contextum_parare (
    AdmonitionumMontatio* m,
                 Piscina* p,
      ContextusPraecepti* cp)
{
    cp->m       = m;
    cp->p       = p;
    cp->l       = ordines(m, p);
    cp->electa  = electa(m, cp->l);
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
    AdmonitionumMontatio* m;
     AdmonitionumFigurae* privatum;
     constans Praeceptum* t;
      ContextusPraecepti  cp;
                     s32  n;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)des;
    m         = (AdmonitionumMontatio*)ctx;
    privatum  = (AdmonitionumFigurae*)m->figurae_ctx;
    si (!privatum)
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
        redde praecepta_status(t, VISUS_INDICIS, requisita_admonitionum,
            &cp) == PRAECEPTUM_PRAESTO && t->fn(&cp);
    }
    si (   !nodus || ev->datum.mus.botton != MUS_SINISTER
        || (   ev->genus != EVENTUS_MUS_DEPRESSUS
            && ev->genus != EVENTUS_MUS_DUPLEX))
    {
        redde FALSUM;
    }
    contextum_parare(m, privatum->scheda, &cp);
    n = nodus->partes == PARTES_ITEM ? id_ordinis(nodus->id) : ZEPHYRUM;
    si (n > ZEPHYRUM)
    {
        si (!electionem_scribere(m, cp.l, n - I, privatum->scheda))
        {
            redde FALSUM;
        }
        /* ictus duplex: actio */
        si (ev->genus == EVENTUS_MUS_DUPLEX)
        {
            cp.electa = n - I;
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

    n = latitudo / ADMONITIONES_LITTERA;
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

/* linea prima (corpus multilineum) */
interior chorda
linea_prima (
    chorda c)
{
    i32 i;

    per (i = ZEPHYRUM; i < c.mensura && c.datum[i] != '\n'; i++)
    {
    }
    c.mensura = i;
    redde c;
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

/* signum: octagonum VI x VI - plenum (ATTENTIO) aut vacuum (NUNTIUS) */
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

/* aetas: "now", "5m", "3h", "2d"; vacua sine porta temporis */
interior chorda
aetas (
    constans AdmonitionumMontatio* m,
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

/* ordo: electa fundo selectionis, ceteri linea infra; signum, titulus
 * (accentus si non lecta), aetas ad dextrum; corpus obscurum infra */
interior vacuum
ordinem_pingere (
     AdmonitionumMontatio* m,
                  Mandata* md,
       constans Componens* l,
                      s32  index,
                      s32  electa_index)
{
    constans Admonitio* x;
                chorda  t;
                chorda  a;
                   s32  spatium;
                   b32  electa_est;
               Punctum  p;
               Punctum  q;

    x = admonitio(m->index, (i32)index);
    si (!x)
    {
        redde;
    }
    electa_est = index == electa_index;
    si (electa_est)
    {
        figura_ordinem_electum_pingere(md, l->fines);
    }
    alioquin
    {
        p.x = l->fines.x;
        p.y = l->fines.y + l->fines.altitudo - I;
        q.x = l->fines.x + l->fines.latitudo - I;
        q.y = p.y;
        mandata_linea(md, p, q, I, figura_color_thematis(
            COLOR_SUPERFICIES));
    }
    signum_pingere(md, l->fines.x + IV, l->fines.y + V,
        x->gradus == ADMONITIO_ATTENTIO, x->gradus == ADMONITIO_ATTENTIO
        ? COLOR_WARNING : COLOR_TEXT_DIM);
    spatium  = l->fines.latitudo - XVI;
    a        = aetas(m, x->tempus, md->piscina);
    si (a.mensura > ZEPHYRUM)
    {
        textum_pingere(md, l->fines.x + l->fines.latitudo - VI
            - (s32)a.mensura * ADMONITIONES_LITTERA, l->fines.y + IV, a,
            ZEPHYRUM, electa_est ? COLOR_TEXT : COLOR_TEXT_DIM);
        spatium -= (s32)(a.mensura + II) * ADMONITIONES_LITTERA;
    }
    t = praecidere(x->titulus, spatium);
    textum_pingere(md, l->fines.x + XIV, l->fines.y + IV, t, ZEPHYRUM,
        x->lecta ? COLOR_TEXT : COLOR_ACCENT_PRIMARY);
    t = praecidere(linea_prima(x->corpus), l->fines.latitudo - XX);
    textum_pingere(md, l->fines.x + XIV, l->fines.y + IV
        + II * ADMONITIONES_LINEA - II, t, ZEPHYRUM, electa_est
        ? COLOR_TEXT : COLOR_TEXT_DIM);
}

interior vacuum
figura_tabulae (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
     AdmonitionumFigurae* contextus;
    AdmonitionumMontatio* m;
                   Album* l;
               Componens* f;
                   Fines  mensa;
                     s32  e;
                     s32  k;
                     s32  n;
                     s32  id;
                     i32  i;
               character  textus[LXIV];

    (vacuum)thema;
    contextus       = (AdmonitionumFigurae*)ctx;
    mensa.x         = ZEPHYRUM;
    mensa.y         = ZEPHYRUM;
    mensa.latitudo  = c->fines.latitudo;
    mensa.altitudo  = c->fines.altitudo;
    figura_mensam_pingere(md, mensa, chorda_aequalis_literis(c->titulus,
        "focatum"));
    mensa.x         = ADMONITIONES_LITTERA;
    mensa.y         = ADMONITIONES_LINEA;
    mensa.latitudo  = c->fines.latitudo - II * ADMONITIONES_LITTERA;
    mensa.altitudo  = c->fines.altitudo - II * ADMONITIONES_LINEA;
    mandata_rectangulum(md, mensa, figura_color_thematis(
        COLOR_BACKGROUND), VERUM);
    si (!contextus || !contextus->m || !contextus->m->index)
    {
        redde;
    }
    m = contextus->m;
    l = ordines(m, md->piscina);
    e = electa(m, l);
    k = pagina_currens(m, l);
    n = album_paginae(l);
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        f = componens_liberum(c, i);
        si (chorda_aequalis_literis(f->id, "titulus"))
        {
            textum_pingere(md, f->fines.x + IV, f->fines.y + II,
                chorda_ex_literis("Notifications", md->piscina),
                FONS_TITULI, COLOR_TEXT);
        }
        alioquin si (chorda_aequalis_literis(f->id, "caput"))
        {
            i32 u;
            i32 omnes;

            u      = admonitiones_non_lectae(m->index);
            omnes  = admonitiones_numerus(m->index);
            si (u > ZEPHYRUM)
            {
                sprintf(textus, "%d unread of %d", (integer)u,
                    (integer)omnes);
            }
            alioquin
            {
                sprintf(textus, "%d notification%s", (integer)omnes,
                    omnes == I ? "" : "s");
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
                    - (s32)strlen(textus) * ADMONITIONES_LITTERA) / II,
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
                chorda_ex_literis("No notifications", md->piscina),
                ZEPHYRUM, COLOR_TEXT_DIM);
        }
        alioquin si (f->partes == PARTES_ITEM)
        {
            id = id_ordinis(f->id);
            si (id > ZEPHYRUM)
            {
                ordinem_pingere(m, md, f, id - I, e);
            }
        }
    }
}

interior b32
figuras_registrare (
     AdmonitionumMontatio* m,
                      i32  latitudo,
                      i32  altitudo)
{
    AdmonitionumFigurae* contextus;

    contextus = (AdmonitionumFigurae*)piscina_allocare(m->piscina,
        magnitudo(AdmonitionumFigurae));
    si (!contextus)
    {
        redde FALSUM;
    }
    contextus->m                     = m;
    contextus->scheda                = piscina_generare_dynamicum(
        "admonitiones_scheda", XVI * M);
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

/* folium cum componente (actio centri) */
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
    componens_ponere_actio(c, actio_admonitionum);
    l = (LocusComponentis*)xar_addere(dp->loci);
    si (l)
    {
        l->index  = index;
        l->c      = c;
    }
    redde c;
}

/* titulus, caput (numerus, paginae), regio ordinum */
interior vacuum
indicem_disponere (
    AdmonitionumMontatio* m,
               Disponens* dp,
                     s32  radix_d,
                 Piscina* piscina)
{
                  Album* l;
     constans AlbumOrdo* o;
                    s32  caput;
                    s32  regio;
                    s32  a;
                    s32  b;
                    s32  t;
                    s32  i;

    (vacuum)componentem_locare(dp, radix_d, "titulus", PARTES_TITULUS,
        DISPOSITIO_CRESCENS, ZEPHYRUM, ADMONITIONES_TITULI);
    caput = locum_addere(dp, radix_d, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
        ADMONITIONES_CAPUT);
    (vacuum)componentem_locare(dp, caput, "caput", PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, ADMONITIONES_CAPUT);
    (vacuum)componentem_locare(dp, caput, "pagina-prior",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, ADMONITIONES_CAPUT);
    (vacuum)componentem_locare(dp, caput, "index_paginae",
        PARTES_INDEX, DISPOSITIO_FIXA, VII, ADMONITIONES_CAPUT);
    (vacuum)componentem_locare(dp, caput, "pagina-proxima",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, ADMONITIONES_CAPUT);
    regio = locum_addere(dp, radix_d, DISPOSITIO_COLUMNA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    l = ordines(m, piscina);
    si (!l || album_numerus(l) == ZEPHYRUM)
    {
        (vacuum)componentem_locare(dp, regio, "nihil", PARTES_INDEX,
            DISPOSITIO_CRESCENS, ZEPHYRUM, ADMONITIONES_ORDO);
        redde;
    }
    album_pagina(l, pagina_currens(m, l), &a, &b, &t);
    per (i = a; i < b; i++)
    {
        o = album_ordo(l, (i32)i);
        (vacuum)componentem_locare(dp, regio, chorda_ut_cstr(o->id,
            piscina), PARTES_ITEM, DISPOSITIO_CRESCENS, ZEPHYRUM,
            o->altitudo);
    }
}

Componens*
admonitiones_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
    AdmonitionumMontatio* m;
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
    m = (AdmonitionumMontatio*)ctx;
    si (!m)
    {
        redde NIHIL;
    }
    latitudo = superficies(m, VERUM);
    altitudo = superficies(m, FALSUM);
    radix     = componens_creare(piscina, intern, "admonitiones",
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
    componens_ponere_actio(radix, actio_admonitionum);
    si (motus_ramus_focatus(motus, m->ramus))
    {
        componens_ponere_titulum(radix, "focatum");
    }
    columnae    = latitudo / ADMONITIONES_LITTERA;
    lineae      = altitudo / ADMONITIONES_LINEA;
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
    indicem_disponere(m, &dp, radix_d, piscina);
    dispositio_computare(dp.d, columnae, lineae, NIHIL, NIHIL);
    per (i = ZEPHYRUM; i < (s32)xar_numerus(dp.loci); i++)
    {
        LocusComponentis* loc;

        loc = (LocusComponentis*)xar_obtinere(dp.loci, (i32)i);
        componens_ponere_fines(loc->c, dispositio_ad_pixela(
            dispositio_fines(dp.d, loc->index), columnae, lineae,
            latitudo, altitudo, ADMONITIONES_LITTERA,
            ADMONITIONES_LINEA));
        componens_addere_liberum(radix, loc->c);
    }
    redde radix;
}


/* ==================================================
 * Montatio
 * ================================================== */

b32
admonitiones_montare (
    AdmonitionumMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
            Admonitiones* index,
       AdmonitionisActio  agere_porta,
                  vacuum* agere_ctx,
                     i32  latitudo,
                     i32  altitudo,
         StatusSystemata* systemata)
{
    Organum* organum;

    si (!m || !piscina || !intern || !repo || !index)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(AdmonitionumMontatio));
    m->piscina    = piscina;
    m->intern     = intern;
    m->index      = index;
    m->agere      = agere_porta;
    m->agere_ctx  = agere_ctx;
    m->ramus      = id ? insula_ramus(repo, "admonitiones", id)
                       : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "admonitiones",
        "admonitiones");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"admonitiones\"", id ? id : "admonitiones",
               piscina, intern))
    {
        redde FALSUM;
    }
    m->actiones  = actio_registrum_creare(piscina, intern);
    m->figurae   = figura_registrum_creare(piscina);
    redde m->actiones && m->figurae
        && praecepta_verificare(TABULA_PRAECEPTORUM)
        && actio_registrare(m->actiones, actio_admonitionum, agere, m)
        && figuras_registrare(m, latitudo, altitudo);
}
