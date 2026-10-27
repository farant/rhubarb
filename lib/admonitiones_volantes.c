/* admonitiones_volantes.c - admonitiones volantes (toasts;
 * include/admonitiones_volantes.h; colloquia-plan T4b). Visibiles ex
 * indice et tempore; dispositio in cellulis (LITTERA x LINEA), figura
 * radicis omnia pingit (ut nomenclator). */

#include "admonitiones_volantes.h"
#include "dispositio.h"
#include "fons.h"
#include "thema.h"

#include <stdio.h>
#include <string.h>

#define VOLANTES_LATITUDO   CDLXXX   /* superficies absens */
#define VOLANTES_ALTITUDO   CDLXXX
#define VOLANTES_LITTERA    VI
#define VOLANTES_LINEA      VIII
#define VOLANTES_MAXIMUM    III      /* simul visibiles */
#define VOLANTES_SUPERUM    II       /* lineae supra (linea tabularum
                                      * vici et intervallum) */
#define VOLANTES_CORPUS     XLV      /* cellulae corporis */
#define VOLANTES_CLAUSURA   III      /* cellulae 'x' */
#define VOLANTES_ALTITUDO_TABELLAE IV   /* lineae: titulus, corpus */
#define VOLANTES_NUNTIUS_MS  ((s64)VIII * (s64)M)
#define VOLANTES_ATTENTIO_MS ((s64)XXX * (s64)M)

hic_manens constans character actio_volantium[] = "volantes.agere";

/* privatum: scheda figurae et actionis; visibiles ultimae (pulsus) */
nomen structura {
    VolantiumMontatio* m;
              Piscina* scheda;
                  s32  latitudo_montationis;
                  s32  altitudo_montationis;
            character  ultimae[CCCLXXXIV];   /* ids iuncti */
} VolantiumPrivatum;


/* ==================================================
 * Visibiles
 * ================================================== */

/* id in campo dimissae (verbum integrum)? */
interior b32
dimissa (
    constans VolantiumMontatio* m,
                        chorda  id)
{
    chorda* a;
       i32  i;
       i32  j;

    a = insula_ramus_attributum(&m->ramus, "dimissae");
    si (!a)
    {
        redde FALSUM;
    }
    i = ZEPHYRUM;
    dum (i < a->mensura)
    {
        per (j = i; j < a->mensura && a->datum[j] != ' '; j++)
        {
        }
        si (   j - i == id.mensura
            && memcmp(a->datum + i, id.datum, (size_t)id.mensura)
               == ZEPHYRUM)
        {
            redde VERUM;
        }
        i = j + I;
    }
    redde FALSUM;
}

/* volare potest (dimissione omissa): non lecta, aetate minor limite
 * gradus sui */
interior b32
volabilis (
    constans Admonitio* x,
                   s64  nunc)
{
    redde !x->lecta && nunc - x->tempus < (x->gradus
        == ADMONITIO_ATTENTIO ? VOLANTES_ATTENTIO_MS
        : VOLANTES_NUNTIUS_MS);
}

/* volabilis proxima indicis ab *i (NIHIL si nulla); *i in ea manet */
interior constans Admonitio*
proxima_volabilis (
    constans VolantiumMontatio* m,
         constans Admonitiones* ix,
                           i32* i,
                           s64  nunc)
{
    constans Admonitio* x;

    dum (ix && *i < admonitiones_numerus(ix))
    {
        x = admonitio(ix, *i);
        si (volabilis(x, nunc) && !dimissa(m, x->id))
        {
            redde x;
        }
        (*i)++;
    }
    redde NIHIL;
}

/* visibiles (recentissima primum) in v[] - indicis communis et
 * transientium per tempus mixtae; numerus */
interior i32
visibiles (
    constans VolantiumMontatio*  m,
            constans Admonitio** v)
{
    constans Admonitio* a;
    constans Admonitio* b;
                   s64  nunc;
                   i32  n;
                   i32  i;
                   i32  j;

    si (!m->nunc || !m->index)
    {
        redde ZEPHYRUM;
    }
    nunc  = m->nunc(m->nunc_ctx);
    n     = ZEPHYRUM;
    i     = ZEPHYRUM;
    j     = ZEPHYRUM;
    dum (n < VOLANTES_MAXIMUM)
    {
        a = proxima_volabilis(m, m->index, &i, nunc);
        b = proxima_volabilis(m, m->transientes, &j, nunc);
        si (!a && !b)
        {
            frange;
        }
        si (a && (!b || a->tempus >= b->tempus))
        {
            v[n++] = a;
            i++;
        }
        alioquin
        {
            v[n++] = b;
            j++;
        }
    }
    redde n;
}

b32
volantes_pulsare (
    VolantiumMontatio* m)
{
     VolantiumPrivatum* p;
             character  iuncti[CCCLXXXIV];
    constans Admonitio* v[VOLANTES_MAXIMUM];
                   i32  n;
                   i32  k;
                   i32  l;
    constans Admonitio* x;

    p = m ? (VolantiumPrivatum*)m->privatum : NIHIL;
    si (!p)
    {
        redde FALSUM;
    }
    n                 = visibiles(m, v);
    l                 = ZEPHYRUM;
    iuncti[ZEPHYRUM]  = '\0';
    per (k = ZEPHYRUM; k < n; k++)
    {
        x = v[k];
        si (l + x->id.mensura + II < (i32)magnitudo(iuncti))
        {
            memcpy(iuncti + l, x->id.datum, (size_t)x->id.mensura);
            l            += x->id.mensura;
            iuncti[l++]  = ' ';
            iuncti[l]    = '\0';
        }
    }
    si (strcmp(iuncti, p->ultimae) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    strcpy(p->ultimae, iuncti);
    redde VERUM;
}

s64
volantes_mora (
    constans VolantiumMontatio* m)
{
    constans Admonitio* v[VOLANTES_MAXIMUM];
                   s64  nunc;
                   s64  reliquum;
                   s64  mora;
                   i32  n;
                   i32  k;

    si (!m || !m->privatum || !m->nunc)
    {
        redde -I;
    }
    n     = visibiles(m, v);
    nunc  = m->nunc(m->nunc_ctx);
    mora  = -I;
    per (k = ZEPHYRUM; k < n; k++)
    {
        reliquum = v[k]->tempus + (v[k]->gradus == ADMONITIO_ATTENTIO
            ? VOLANTES_ATTENTIO_MS : VOLANTES_NUNTIUS_MS) - nunc;
        si (reliquum < ZEPHYRUM)
        {
            reliquum = ZEPHYRUM;
        }
        si (mora < ZEPHYRUM || reliquum < mora)
        {
            mora = reliquum;
        }
    }
    redde mora;
}


/* ==================================================
 * Actio
 * ================================================== */

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

/* dimittere: id primum, deinde dimissae quae adhuc volare possent */
interior b32
dimittere (
    VolantiumMontatio* m,
               chorda  id,
              Piscina* p)
{
    constans Admonitio* x;
              ParCampi  par;
                chorda  valor;
                chorda  spatium;
                chorda  prior;
                   s64  nunc;
                   i32  i;
                   b32  ok;

    nunc     = m->nunc ? m->nunc(m->nunc_ctx) : ZEPHYRUM;
    valor    = chorda_transcribere(id, p);
    spatium  = chorda_ex_literis(" ", p);
    per (i = ZEPHYRUM; i < admonitiones_numerus(m->index)
                       + admonitiones_numerus(m->transientes); i++)
    {
        x = i < admonitiones_numerus(m->index)
            ? admonitio(m->index, i)
            : admonitio(m->transientes, i - admonitiones_numerus(
                  m->index));
        si (   !chorda_aequalis(x->id, id) && volabilis(x, nunc)
            && dimissa(m, x->id))
        {
            valor = chorda_concatenare(chorda_concatenare(valor,
                spatium,
                p), x->id, p);
        }
    }
    par.titulus  = "dimissae";
    par.valor    = chorda_ut_cstr(valor, p);
    prior        = m->ramus.repo->scriptor;
    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        actio_volantium, p));
    ok = mutare_ramum(&m->ramus, campum_ponere, &par);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

/* id componentis "v<k>" aut "x<k>" -> k (ab I); 0 si alienum */
interior i32
numerus_tabellae (
                chorda id,
    constans character littera)
{
    s32 k;

    si (id.mensura < II || id.datum[ZEPHYRUM] != (i8)littera)
    {
        redde ZEPHYRUM;
    }
    id.datum++;
    id.mensura--;
    redde chorda_ut_s32(id, &k) && k > ZEPHYRUM ? (i32)k : ZEPHYRUM;
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
     VolantiumMontatio* m;
     VolantiumPrivatum* p;
    constans Admonitio* x;
    constans Admonitio* v[VOLANTES_MAXIMUM];
                   i32  n;
                   i32  k;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)des;
    m = (VolantiumMontatio*)ctx;
    p = (VolantiumPrivatum*)m->privatum;
    si (   !p || !nodus || ev->genus != EVENTUS_MUS_DEPRESSUS
        || ev->datum.mus.botton != MUS_SINISTER)
    {
        redde FALSUM;
    }
    piscina_vacare(p->scheda);
    n = visibiles(m, v);
    k = numerus_tabellae(nodus->id, 'x');
    si (k > ZEPHYRUM && k <= n)
    {
        redde dimittere(m, v[k - I]->id, p->scheda);
    }
    k = numerus_tabellae(nodus->id, 'v');
    si (k > ZEPHYRUM && k <= n)
    {
        x = v[k - I];
        /* nuntius hospitis: legitur, porta nulla */
        si (   m->transientes
            && admonitionem_legere_notare(m->transientes, x->id))
        {
            redde VERUM;
        }
        si (m->agere)
        {
            (vacuum)m->agere(m->agere_ctx, x);
        }
        (vacuum)admonitionem_legere_notare(m->index, x->id);
        redde VERUM;
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

    n = latitudo / VOLANTES_LITTERA;
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
      ColorThema  color)
{
    mandata_textus(md, x, y, chorda_transcribere(textus, md->piscina),
        ZEPHYRUM, figura_color_thematis(color));
}

/* signum: octagonum VI x VI (ut centrum) */
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

/* aetas: "now", "5s"... (volantes iuvenes: secundae) */
interior chorda
aetas (
             s64  ms,
         Piscina* p)
{
    character textus[XVI];

    si (ms < (s64)M)
    {
        redde chorda_ex_literis("now", p);
    }
    sprintf(textus, "%lds", (long)(ms / (s64)M));
    redde chorda_ex_literis(textus, p);
}

/* tabella: umbra, fundus, margo (ut menu); signum, titulus (accentus),
 * aetas; corpus obscurum; 'x' in clausura */
interior vacuum
tabellam_pingere (
     VolantiumMontatio* m,
               Mandata* md,
    constans Componens* corpus,
    constans Componens* clausura,
    constans Admonitio* x)
{
     Fines t;
    chorda a;
       s32 spatium;

    t           = corpus->fines;
    t.latitudo  = clausura->fines.x + clausura->fines.latitudo - t.x;
    figura_umbram_pingere(md, t);
    mandata_rectangulum(md, t, figura_color_thematis(COLOR_BACKGROUND),
        VERUM);
    mandata_rectangulum(md, t, figura_color_thematis(COLOR_TEXT),
        FALSUM);
    signum_pingere(md, t.x + V, t.y + V, x->gradus
        == ADMONITIO_ATTENTIO,
        x->gradus
            == ADMONITIO_ATTENTIO ? COLOR_WARNING : COLOR_TEXT_DIM);
    spatium = corpus->fines.latitudo - XVIII;
    si (m->nunc)
    {
        a = aetas(m->nunc(m->nunc_ctx) - x->tempus, md->piscina);
        textum_pingere(md, corpus->fines.x + corpus->fines.latitudo
            - (s32)a.mensura * VOLANTES_LITTERA - II, t.y + IV, a,
            COLOR_TEXT_DIM);
        spatium -= (s32)(a.mensura + I) * VOLANTES_LITTERA;
    }
    textum_pingere(md, t.x + XV, t.y + IV, praecidere(x->titulus,
        spatium), COLOR_ACCENT_PRIMARY);
    textum_pingere(md, t.x + XV, t.y + IV + II * VOLANTES_LINEA - II,
        praecidere(linea_prima(x->corpus), corpus->fines.latitudo
        - XVIII), COLOR_TEXT_DIM);
    textum_pingere(md, clausura->fines.x + (clausura->fines.latitudo
        - VOLANTES_LITTERA) / II, t.y + IV, chorda_ex_literis("x",
        md->piscina), COLOR_TEXT_DIM);
}

interior vacuum
figura_strati (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
     VolantiumPrivatum* p;
             Componens* f;
             Componens* corpora[VOLANTES_MAXIMUM];
             Componens* clausurae[VOLANTES_MAXIMUM];
    constans Admonitio* v[VOLANTES_MAXIMUM];
                   i32  n;
                   i32  k;
                   i32  i;

    (vacuum)thema;
    p = (VolantiumPrivatum*)ctx;
    si (!p || !p->m)
    {
        redde;
    }
    memset(corpora, ZEPHYRUM, magnitudo(corpora));
    memset(clausurae, ZEPHYRUM, magnitudo(clausurae));
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        f = componens_liberum(c, i);
        k = numerus_tabellae(f->id, 'v');
        si (k > ZEPHYRUM && k <= VOLANTES_MAXIMUM)
        {
            corpora[k - I] = f;
        }
        k = numerus_tabellae(f->id, 'x');
        si (k > ZEPHYRUM && k <= VOLANTES_MAXIMUM)
        {
            clausurae[k - I] = f;
        }
    }
    n = visibiles(p->m, v);
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (corpora[k] && clausurae[k])
        {
            tabellam_pingere(p->m, md, corpora[k], clausurae[k],
                v[k]);
        }
    }
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

interior vacuum
componentem_locare (
              Disponens* dp,
                    s32  parens,
     constans character* id,
                 Partes  partes,
                    s32  columnae)
{
            Componens* c;
     LocusComponentis* l;
                  s32  index;

    index = locum_addere(dp, parens, DISPOSITIO_LINEA, DISPOSITIO_FIXA,
        columnae, DISPOSITIO_FIXA, VOLANTES_ALTITUDO_TABELLAE);
    c     = componens_creare(dp->piscina, dp->intern, id, partes);
    si (!c || index < ZEPHYRUM)
    {
        redde;
    }
    componens_ponere_actio(c, actio_volantium);
    l = (LocusComponentis*)xar_addere(dp->loci);
    si (l)
    {
        l->index  = index;
        l->c      = c;
    }
}

interior s32
superficies (
    constans VolantiumMontatio* m,
                           b32  latitudo)
{
     constans VolantiumPrivatum* p;
                         chorda* a;
                            s32  v;

    p = (constans VolantiumPrivatum*)m->privatum;
    v = !p ? ZEPHYRUM : latitudo ? p->latitudo_montationis
                                 : p->altitudo_montationis;
    si (v <= ZEPHYRUM)
    {
        v = latitudo ? VOLANTES_LATITUDO : VOLANTES_ALTITUDO;
    }
    a = insula_ramus_attributum(&m->ramus, latitudo
        ? "superficies_latitudo" : "superficies_altitudo");
    si (a)
    {
        s32 n;

        si (chorda_ut_s32(*a, &n) && n > ZEPHYRUM)
        {
            v = n;
        }
    }
    redde v;
}

/* radix 0 x 0 (ictus extra tabellas ad latera transeunt); columna:
 * spatium superum, deinde tabellae ad dextrum (corpus + clausura,
 * cellula marginis) intervallo lineae unius */
Componens*
volantes_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
     VolantiumMontatio* m;
             Componens* radix;
             Disponens  dp;
       DispositioForma  forma;
                 Fines  f;
                   s32  columnae;
                   s32  lineae;
                   s32  radix_d;
                   s32  ordo;
    constans Admonitio* v[VOLANTES_MAXIMUM];
                   i32  n;
                   i32  k;
             character  id[VIII];

    (vacuum)repo;
    (vacuum)motus;
    m = (VolantiumMontatio*)ctx;
    si (!m)
    {
        redde NIHIL;
    }
    radix = componens_creare(piscina, intern, "volantes",
        PARTES_TABULA);
    si (!radix)
    {
        redde NIHIL;
    }
    memset(&f, ZEPHYRUM, magnitudo(f));
    componens_ponere_fines(radix, f);
    componens_ponere_actio(radix, actio_volantium);
    n = visibiles(m, v);
    si (n == ZEPHYRUM)
    {
        redde radix;
    }
    columnae    = superficies(m, VERUM) / VOLANTES_LITTERA;
    lineae      = superficies(m, FALSUM) / VOLANTES_LINEA;
    dp.d        = dispositio_creare(piscina);
    dp.loci     = xar_creare(piscina, (i32)magnitudo(LocusComponentis));
    dp.piscina  = piscina;
    dp.intern   = intern;
    si (!dp.d || !dp.loci)
    {
        redde radix;
    }
    dispositio_formam_initiare(&forma);
    forma.directio        = DISPOSITIO_COLUMNA;
    forma.latitudo.genus  = DISPOSITIO_CRESCENS;
    forma.altitudo.genus  = DISPOSITIO_CRESCENS;
    radix_d               = dispositio_addere(dp.d, -I, &forma);
    (vacuum)locum_addere(&dp, radix_d, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
        VOLANTES_SUPERUM);
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (k > ZEPHYRUM)
        {
            (vacuum)locum_addere(&dp, radix_d, DISPOSITIO_LINEA,
                DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, I);
        }
        ordo = locum_addere(&dp, radix_d, DISPOSITIO_LINEA,
            DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
            VOLANTES_ALTITUDO_TABELLAE);
        (vacuum)locum_addere(&dp, ordo, DISPOSITIO_LINEA,
            DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
            VOLANTES_ALTITUDO_TABELLAE);
        sprintf(id, "v%d", (integer)(k + I));
        componentem_locare(&dp, ordo, id, PARTES_ITEM, VOLANTES_CORPUS);
        sprintf(id, "x%d", (integer)(k + I));
        componentem_locare(&dp, ordo, id, PARTES_BOTTONE,
            VOLANTES_CLAUSURA);
        (vacuum)locum_addere(&dp, ordo, DISPOSITIO_LINEA,
            DISPOSITIO_FIXA,
            I, DISPOSITIO_FIXA, VOLANTES_ALTITUDO_TABELLAE);
    }
    dispositio_computare(dp.d, columnae, lineae, NIHIL, NIHIL);
    per (k = ZEPHYRUM; k < (i32)xar_numerus(dp.loci); k++)
    {
        LocusComponentis* loc;

        loc = (LocusComponentis*)xar_obtinere(dp.loci, k);
        componens_ponere_fines(loc->c, dispositio_ad_pixela(
            dispositio_fines(dp.d, loc->index), columnae, lineae,
            columnae * VOLANTES_LITTERA, lineae * VOLANTES_LINEA,
            VOLANTES_LITTERA, VOLANTES_LINEA));
        componens_addere_liberum(radix, loc->c);
    }
    redde radix;
}


/* ==================================================
 * Montatio
 * ================================================== */

b32
volantes_montare (
       VolantiumMontatio* m,
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
    VolantiumPrivatum* p;

    si (!m || !piscina || !intern || !repo || !index)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(VolantiumMontatio));
    m->piscina    = piscina;
    m->intern     = intern;
    m->index      = index;
    m->agere      = agere_porta;
    m->agere_ctx  = agere_ctx;
    m->ramus      = id ? insula_ramus(repo, "volantes", id)
                       : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "volantes",
        "volantes");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata, NIHIL,
               id ? id : "volantes", piscina, intern))
    {
        redde FALSUM;
    }
    p = (VolantiumPrivatum*)piscina_allocare(piscina,
        magnitudo(VolantiumPrivatum));
    si (!p)
    {
        redde FALSUM;
    }
    memset(p, ZEPHYRUM, magnitudo(VolantiumPrivatum));
    p->m                     = m;
    p->scheda                = piscina_generare_dynamicum(
        "volantes_scheda", VIII * M);
    p->latitudo_montationis  = (s32)latitudo;
    p->altitudo_montationis  = (s32)altitudo;
    m->privatum              = p;
    m->actiones              = actio_registrum_creare(piscina, intern);
    m->figurae               = figura_registrum_creare(piscina);
    redde p->scheda && m->actiones && m->figurae
        && actio_registrare(m->actiones, actio_volantium, agere, m)
        && figura_registrare(m->figurae, PARTES_TABULA, ZEPHYRUM,
               figura_strati, p);
}
