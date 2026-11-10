/* statuae.c - latus statuarum (vide include/statuae.h)
 *
 * Arbor: radix "statuae" (PARTES_TABULA, figura tota) cum liberis qui
 * areae ictus sunt - "sagitta_prior", "sagitta_proxima", "cyclus",
 * "modus" (bottones delectus), "lux", "duplex", "pictura" - et, dum
 * delectus apertus est, menu eius ULTIMUM (elenchus, PARTES_DIALOGUS).
 *
 * Quadrum: statuae_quadrum_pingere (charta opaca, Albus) cum clavis
 * eius
 * mutatur; clavis = sigillum statuae, cyclus, tempus cycli (ms), visus,
 * modus, mensura. Et pulsus et compositor quadrum curant, ut pictura
 * statum quem ostendit semper sequatur.
 */
#include "statuae.h"
#include "membrum__praecepta.h"
#include "membrum__delectus.h"
#include "statua_glb.h"
#include "statua_pictura.h"
#include "statuae_quadrum.h"
#include "statuae_repositorium.h"
#include "fons.h"
#include "thema.h"
#include "motus.h"
#include "destinatio.h"
#include "mandatum.h"
#include <stdio.h>
#include <string.h>

#define STATUAE_LATITUDO     CCCLX   /* superficies ordinaria */
#define STATUAE_ALTITUDO     CDL
#define ORDO_ALTITUDO        XII     /* ordo capitis */
#define LIMBUS               II
#define SAGITTA_LATITUDO     XII
#define LITTERA_LATITUDO     VI
#define LITTERA_ALTITUDO     VIII
#define LUX_LATITUDO         XXXII   /* "flat" */
#define DUPLEX_LATITUDO      XX      /* "2x" */
#define GYRUS_ORDINARIUS     CCC     /* decimae gradus */
#define INCLINATIO_ORDINARIA CL
#define INCLINATIO_MAXIMA    850
#define GYRUS_PLENUS         3600
#define VISUS_STATUARUM      I

#define DELECTUS_CYCLI "cyclus"
#define DELECTUS_MODI  "modus"

hic_manens constans character actio_statuarum[] = "statuae.agere";

/* modi quos latus offert, ordine menu; tituli ostensi */
hic_manens constans StatuaeModus MODI[IV] = {
    STATUAE_MODUS_ATKINSON, STATUAE_MODUS_BAYER,
    STATUAE_MODUS_EXEMPLARIA, STATUAE_MODUS_NULLUS
};
hic_manens constans character* constans MODORUM_NOMINA[IV] = {
    "Atkinson", "Bayer", "Exemplaria", "None"
};

/* dispositio lateris (spatium radicis) */
nomen structura {
    Fines prior;
    Fines proxima;
    Fines cyclus;
    Fines modus;
    Fines lux;
    Fines gemina;
    Fines pictura;
      s32 numerus_x;    /* textus "1/2" */
      s32 titulus_x;
      s32 titulus_latitudo;
      i32 ordines;      /* I aut II */
} Dispositio;


/* ==================================================
 * Status (ramus)
 * ================================================== */

interior s32
attributum_s32 (
    constans InsulaRamus* ramus,
      constans character* titulus,
                     s32  praestitutum)
{
    chorda* a = insula_ramus_attributum(ramus, titulus);
       s32  v;

    si (!a || !chorda_ut_s32(*a, &v))
    {
        redde praestitutum;
    }
    redde v;
}

interior b32
attributum_veritas (
    constans InsulaRamus* ramus,
      constans character* titulus)
{
    chorda* a = insula_ramus_attributum(ramus, titulus);

    redde a && chorda_aequalis_literis(*a, "verum");
}

nomen structura {
    constans character* titulus;
    constans character* valor;
} ParCampi;

nomen structura {
    ParCampi pares[III];
         i32 numerus;
} GrexCamporum;

interior vacuum
campos_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    GrexCamporum* grex = (GrexCamporum*)ctx;
             i32  k;

    per (k = ZEPHYRUM; k < grex->numerus; k++)
    {
        si (   !grex->pares[k].valor
            || !grex->pares[k].valor[ZEPHYRUM])
        {
            (vacuum)insula_attributum_tollere(radix,
                grex->pares[k].titulus);
            perge;
        }
        insula_attributum_ponere(radix, p, in, grex->pares[k].titulus,
            grex->pares[k].valor);
    }
}

/* campi scriptura una, scriptore lateris (vocatur etiam extra
 * actionem); valor vacuus campum tollit */
interior b32
gregem_scribere (
    StatuaeMontatio* m,
       GrexCamporum* grex)
{
    chorda prior = m->ramus.repo->scriptor;
       b32 ok;

    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        actio_statuarum, m->piscina));
    ok = mutare_ramum(&m->ramus, campos_ponere, grex);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

interior b32
campum_scribere (
       StatuaeMontatio* m,
    constans character* titulus,
    constans character* valor)
{
    GrexCamporum grex;

    grex.pares[ZEPHYRUM].titulus  = titulus;
    grex.pares[ZEPHYRUM].valor    = valor;
    grex.numerus                  = I;
    redde gregem_scribere(m, &grex);
}

/* gyrus in [0, 3600), inclinatio in [-850, 850] */
interior s32
gyrum_cogere (
    s32 v)
{
    v = v % GYRUS_PLENUS;
    redde v < ZEPHYRUM ? v + GYRUS_PLENUS : v;
}

interior s32
inclinationem_cogere (
    s32 v)
{
    si (v > INCLINATIO_MAXIMA)
    {
        redde INCLINATIO_MAXIMA;
    }
    redde v < -INCLINATIO_MAXIMA ? -INCLINATIO_MAXIMA : v;
}

interior s32
gyrus_currens (
    constans StatuaeMontatio* m)
{
    redde m->trahitur ? m->gyrus
        : gyrum_cogere(attributum_s32(&m->ramus, "gyrus",
              GYRUS_ORDINARIUS));
}

interior s32
inclinatio_currens (
    constans StatuaeMontatio* m)
{
    redde m->trahitur ? m->inclinatio
        : inclinationem_cogere(attributum_s32(&m->ramus, "inclinatio",
              INCLINATIO_ORDINARIA));
}

/* visus (gyrus et inclinatio) scriptura una */
interior b32
visum_scribere (
    StatuaeMontatio* m,
                s32  gyrus,
                s32  inclinatio)
{
    GrexCamporum grex;
       character g[XVI];
       character n[XVI];

    sprintf(g, "%d", (integer)gyrum_cogere(gyrus));
    sprintf(n, "%d", (integer)inclinationem_cogere(inclinatio));
    grex.pares[ZEPHYRUM].titulus  = "gyrus";
    grex.pares[ZEPHYRUM].valor    = g;
    grex.pares[I].titulus         = "inclinatio";
    grex.pares[I].valor           = n;
    grex.numerus                  = II;
    redde gregem_scribere(m, &grex);
}

/* locus modi in MODI (status ignotus aut absens = Atkinson) */
interior i32
modus_currens (
    constans StatuaeMontatio* m)
{
          chorda* a = insula_ramus_attributum(&m->ramus, "modus");
    StatuaeModus  modus;
             i32  k;

    si (a && statuae_modum_legere(*a, &modus))
    {
        per (k = ZEPHYRUM; k < IV; k++)
        {
            si (MODI[k] == modus)
            {
                redde k;
            }
        }
    }
    redde ZEPHYRUM;
}

interior s64
nunc (
    constans StatuaeMontatio* m)
{
    redde m->porta.nunc ? m->porta.nunc(m->porta.ctx) : ZEPHYRUM;
}


/* ==================================================
 * Index et statua visa
 * ================================================== */

interior i32
statuarum_numerus (
    constans StatuaeMontatio* m)
{
    redde m->index ? xar_numerus(m->index) : ZEPHYRUM;
}

interior StatuaeInscriptio*
inscriptio (
    constans StatuaeMontatio* m,
                         i32  k)
{
    redde (StatuaeInscriptio*)xar_obtinere(m->index, k);
}

/* locus statuae visae in indice: titulus status, aliter prima; -1 si
 * index vacuus */
interior s32
locus_statuae (
    constans StatuaeMontatio* m)
{
    chorda* a = insula_ramus_attributum(&m->ramus, "statua");
       i32  k;

    si (statuarum_numerus(m) == ZEPHYRUM)
    {
        redde -I;
    }
    per (k = ZEPHYRUM; a && k < statuarum_numerus(m); k++)
    {
        si (chorda_aequalis(inscriptio(m, k)->titulus, *a))
        {
            redde (s32)k;
        }
    }
    redde ZEPHYRUM;
}

interior vacuum
indicem_relegere (
    StatuaeMontatio* m)
{
    piscina_vacare(m->piscina_indicis);
    m->index      = m->volumen
        ? statuae_enumerare(m->volumen, m->piscina_indicis) : NIHIL;
    m->seq_visum  = m->volumen ? volumen_seq_ultimum(m->volumen)
                               : ZEPHYRUM;
}

/* statuam visam legere si alia (aut nulla) nunc videnda est; VERUM si
 * quid mutatum */
interior b32
statuam_curare (
    StatuaeMontatio* m)
{
    StatuaeInscriptio* x;
        StatuaFructus  f;
               chorda  octeti;
                  s32  locus     = locus_statuae(m);
                  b32  inventum  = FALSUM;

    si (locus < ZEPHYRUM)
    {
        si (   !m->statua && m->sigillum.mensura == ZEPHYRUM
            && m->causa.mensura == ZEPHYRUM)
        {
            redde FALSUM;
        }
        piscina_vacare(m->piscina_statuae);
        m->statua            = NIHIL;
        m->sigillum.mensura  = ZEPHYRUM;
        m->causa.mensura     = ZEPHYRUM;
        redde VERUM;
    }
    x = inscriptio(m, (i32)locus);
    si (   m->sigillum.mensura > ZEPHYRUM
        && chorda_aequalis(m->sigillum, x->sigillum_hex))
    {
        redde FALSUM;
    }
    piscina_vacare(m->piscina_statuae);
    m->statua    = NIHIL;
    m->sigillum  = chorda_transcribere(x->sigillum_hex,
        m->piscina_statuae);
    m->causa.mensura = ZEPHYRUM;
    octeti = volumen_massam_promere(m->volumen, x->sigillum_hex,
        m->piscina_statuae, &inventum);
    si (!inventum)
    {
        m->causa = chorda_ex_literis("the model's data is missing "
            "from the volume", m->piscina_statuae);
        redde VERUM;
    }
    f = statua_glb_legere(octeti, m->piscina_statuae);
    si (f.successus)
    {
        m->statua = f.statua;
    }
    alioquin
    {
        m->causa = f.error;
    }
    /* statua nova: cyclus ab initio */
    m->origo_ms    = m->hora_visa;
    m->quietum_ms  = ZEPHYRUM;
    redde VERUM;
}

/* index cycli currentis: titulus status, aliter primus; -1 si statua
 * nulla aut sine cyclis */
interior s32
cyclus_currens (
    constans StatuaeMontatio* m)
{
    chorda* a;
       i32  k;

    si (!m->statua || m->statua->cyclorum == ZEPHYRUM)
    {
        redde -I;
    }
    a = insula_ramus_attributum(&m->ramus, "cyclus");
    per (k = ZEPHYRUM; a && k < m->statua->cyclorum; k++)
    {
        si (chorda_aequalis(m->statua->cycli[k].titulus, *a))
        {
            redde (s32)k;
        }
    }
    redde ZEPHYRUM;
}

interior b32
quiescit (
    constans StatuaeMontatio* m)
{
    redde attributum_veritas(&m->ramus, "quiescit");
}

/* tempus cycli in ms, intra durationem involutum (integris, ne f32
 * post horas hebescat) */
interior s32
tempus_cycli_ms (
    constans StatuaeMontatio* m,
                         s32  cyclus)
{
    s64 t;
    s64 duratio;

    si (cyclus < ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    t = quiescit(m) ? (s64)m->quietum_ms : m->hora_visa - m->origo_ms;
    si (t < ZEPHYRUM)
    {
        t = ZEPHYRUM;
    }
    duratio = (s64)(m->statua->cycli[cyclus].duratio * 1000.0f + 0.5f);
    redde duratio > ZEPHYRUM ? (s32)(t % duratio) : ZEPHYRUM;
}


/* ==================================================
 * Dispositio
 * ================================================== */

interior s32
superficies (
    constans StatuaeMontatio* m,
                         b32  latitudo)
{
    s32 v = latitudo ? (s32)m->latitudo : (s32)m->altitudo;

    si (v <= ZEPHYRUM)
    {
        v = latitudo ? STATUAE_LATITUDO : STATUAE_ALTITUDO;
    }
    v = attributum_s32(&m->ramus, latitudo ? "superficies_latitudo"
        : "superficies_altitudo", v);
    redde v < ZEPHYRUM ? ZEPHYRUM : v;
}

/* textus bottonis cycli: titulus cycli currentis aut "no clips" */
interior chorda
cycli_textus (
    constans StatuaeMontatio* m,
                     Piscina* p)
{
    s32 cyclus = cyclus_currens(m);

    si (cyclus < ZEPHYRUM)
    {
        redde chorda_ex_literis("no clips", p);
    }
    redde m->statua->cycli[cyclus].titulus;
}

interior Fines
fines (
    s32 x,
    s32 y,
    s32 latitudo,
    s32 altitudo)
{
    Fines f;

    f.x         = x;
    f.y         = y;
    f.latitudo  = latitudo < ZEPHYRUM ? ZEPHYRUM : latitudo;
    f.altitudo  = altitudo < ZEPHYRUM ? ZEPHYRUM : altitudo;
    redde f;
}

/* numerus "k/n" ("0/0" si index vacuus) */
interior vacuum
numerum_formare (
    constans StatuaeMontatio* m,
                   character* alveus)
{
    sprintf(alveus, "%d/%u", (integer)(locus_statuae(m) + I),
        (insignatus)statuarum_numerus(m));
}

interior vacuum
disponere (
    constans StatuaeMontatio* m,
                     Piscina* p,
                  Dispositio* d)
{
    character numerus[XXXII];
          s32 latitudo = superficies(m, VERUM);
          s32 altitudo = superficies(m, FALSUM);
          s32 l_cycli;
          s32 l_modi;
          s32 sinistra;
          s32 dextra;
          s32 x;
          s32 y;

    numerum_formare(m, numerus);
    l_cycli  = (s32)delectus_bottonis_latitudo(cycli_textus(m, p));
    l_modi   = (s32)delectus_bottonis_latitudo(chorda_ex_literis(
        MODORUM_NOMINA[modus_currens(m)], p));
    d->prior      = fines(LIMBUS, ZEPHYRUM, SAGITTA_LATITUDO,
        ORDO_ALTITUDO);
    d->numerus_x  = LIMBUS + SAGITTA_LATITUDO + LIMBUS;
    x             = d->numerus_x
        + (s32)strlen(numerus) * LITTERA_LATITUDO + LIMBUS;
    d->proxima    = fines(x, ZEPHYRUM, SAGITTA_LATITUDO, ORDO_ALTITUDO);
    d->titulus_x  = x + SAGITTA_LATITUDO + LIMBUS * II;
    sinistra      = d->titulus_x
        + (s32)(m->statua || m->causa.mensura
              ? inscriptio(m, (i32)locus_statuae(m))->titulus.mensura
              : ZEPHYRUM) * LITTERA_LATITUDO + LIMBUS * II;
    dextra        = l_cycli + LIMBUS + l_modi + LIMBUS + LUX_LATITUDO
        + LIMBUS + DUPLEX_LATITUDO + LIMBUS;
    si (sinistra + dextra <= latitudo)
    {
        d->ordines           = I;
        x                    = latitudo - dextra;
        y                    = ZEPHYRUM;
        d->titulus_latitudo  = x - d->titulus_x - LIMBUS;
    }
    alioquin
    {
        d->ordines           = II;
        x                    = LIMBUS;
        y                    = ORDO_ALTITUDO;
        d->titulus_latitudo  = latitudo - d->titulus_x - LIMBUS;
    }
    d->cyclus   = fines(x, y, l_cycli, ORDO_ALTITUDO);
    x           += l_cycli + LIMBUS;
    d->modus    = fines(x, y, l_modi, ORDO_ALTITUDO);
    x           += l_modi + LIMBUS;
    d->lux      = fines(x, y, LUX_LATITUDO, ORDO_ALTITUDO);
    x           += LUX_LATITUDO + LIMBUS;
    d->gemina   = fines(x, y, DUPLEX_LATITUDO, ORDO_ALTITUDO);
    y           = (s32)d->ordines * ORDO_ALTITUDO;
    d->pictura  = fines(ZEPHYRUM, y, latitudo, altitudo - y);
    si (d->titulus_latitudo < ZEPHYRUM)
    {
        d->titulus_latitudo = ZEPHYRUM;
    }
}


/* ==================================================
 * Quadrum
 * ================================================== */

/* pixela duplicata (proximum), in piscina quadri */
interior vacuum
quadrum_duplicare (
    StatuaeMontatio* m)
{
    Imago  magna;
      i32  x;
      i32  y;
       i8* fons;
       i8* locus;

    magna.latitudo = m->quadrum.latitudo * II;
    magna.altitudo = m->quadrum.altitudo * II;
    magna.pixela    = (i8*)piscina_allocare(m->piscina_quadri,
        (memoriae_index)magna.latitudo * magna.altitudo * IV);
    per (y = ZEPHYRUM; y < magna.altitudo; y++)
    {
        per (x = ZEPHYRUM; x < magna.latitudo; x++)
        {
            fons   = m->quadrum.pixela
                + ((memoriae_index)(y / II) * m->quadrum.latitudo
                    + x / II) * IV;
            locus  = magna.pixela
                + ((memoriae_index)y * magna.latitudo + x) * IV;
            memcpy(locus, fons, IV);
        }
    }
    m->quadrum = magna;
}

/* quadrum reficere si clavis mutata; VERUM si mutatum */
interior b32
quadrum_curare (
    StatuaeMontatio* m,
            Piscina* p)
{
    StatuaeQuadrumPetitio petitio;
               Dispositio d;
                character clavis[CXXVIII];
                      s32 cyclus;
                      s32 tempus;
                      i32 latitudo;
                      i32 altitudo;
                      b32 geminatum;

    disponere(m, p, &d);
    geminatum  = attributum_veritas(&m->ramus, "duplex");
    latitudo   = (i32)d.pictura.latitudo;
    altitudo   = (i32)d.pictura.altitudo;
    si (geminatum)
    {
        latitudo /= II;
        altitudo /= II;
    }
    si (!m->statua || latitudo == ZEPHYRUM || altitudo == ZEPHYRUM)
    {
        si (!m->quadrum.pixela && m->quadri_clavis[ZEPHYRUM] == '\0')
        {
            redde FALSUM;
        }
        piscina_vacare(m->piscina_quadri);
        m->quadrum.pixela           = NIHIL;
        m->quadrum.latitudo         = ZEPHYRUM;
        m->quadrum.altitudo         = ZEPHYRUM;
        m->quadri_clavis[ZEPHYRUM]  = '\0';
        redde VERUM;
    }
    cyclus = cyclus_currens(m);
    tempus = tempus_cycli_ms(m, cyclus);
    memset(&petitio, ZEPHYRUM, magnitudo(petitio));
    petitio.cyclus            = cyclus;
    petitio.tempus            = (f32)tempus / 1000.0f;
    petitio.visus.gyrus       = (f32)gyrus_currens(m) / 10.0f;
    petitio.visus.inclinatio  = (f32)inclinatio_currens(m) / 10.0f;
    petitio.visus.illuminatum  = !attributum_veritas(&m->ramus,
        "planum");
    petitio.modus             = MODI[modus_currens(m)];
    petitio.latitudo          = latitudo;
    petitio.altitudo          = altitudo;
    petitio.charta_perlucida  = FALSUM;
    sprintf(clavis, "%.64s|%d|%d|%d|%d|%d|%d|%d|%ux%u",
        m->sigillum.mensura >= LXIV ? (constans character*)
        m->sigillum.datum : "", (integer)cyclus, (integer)tempus,
        (integer)gyrus_currens(m), (integer)inclinatio_currens(m),
        (integer)petitio.modus, (integer)petitio.visus.illuminatum,
        (integer)geminatum, (insignatus)latitudo, (insignatus)altitudo);
    si (strcmp(clavis, m->quadri_clavis) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    strcpy(m->quadri_clavis, clavis);
    piscina_vacare(m->piscina_quadri);
    si (!statuae_quadrum_pingere(m->statua, &petitio, &m->quadrum,
        m->piscina_quadri))
    {
        m->quadrum.pixela    = NIHIL;
        m->quadrum.latitudo  = ZEPHYRUM;
        m->quadrum.altitudo  = ZEPHYRUM;
        redde VERUM;
    }
    si (geminatum && m->quadrum.pixela)
    {
        quadrum_duplicare(m);
    }
    redde VERUM;
}

b32
statuae_pulsare (
    vacuum* ctx)
{
    StatuaeMontatio* m = (StatuaeMontatio*)ctx;
                s64  hora;
                b32  mutatum = FALSUM;

    si (!m)
    {
        redde FALSUM;
    }
    /* horologium SEMEL per pulsum legitur: compositor et eventa
     * tempus hoc vident, ne quadrum bis per pulsum fiat */
    hora          = nunc(m);
    m->hora_visa  = hora;
    si (   m->volumen
        && (   !m->inspectum
            || hora - m->inspectio_ms >= STATUAE_MORA_INSPECTIONIS))
    {
        m->inspectum     = VERUM;
        m->inspectio_ms  = hora;
        si (volumen_seq_ultimum(m->volumen) != m->seq_visum)
        {
            indicem_relegere(m);
            mutatum = VERUM;
        }
    }
    si (statuam_curare(m))
    {
        mutatum = VERUM;
    }
    si (quadrum_curare(m, m->piscina_indicis))
    {
        mutatum = VERUM;
    }
    redde mutatum;
}

Mora
statuae_mora (
    vacuum* ctx)
{
    StatuaeMontatio* m = (StatuaeMontatio*)ctx;

    si (!m)
    {
        redde -I;
    }
    si (m->trahitur || (cyclus_currens(m) >= ZEPHYRUM && !quiescit(m)))
    {
        redde STATUAE_MORA_MOTUS;
    }
    redde m->volumen ? STATUAE_MORA_INSPECTIONIS : -I;
}

constans Imago*
statuae_imago_fons (
    chorda  provenientia,
    vacuum* ctx)
{
    StatuaeMontatio* m = (StatuaeMontatio*)ctx;

    si (   !m || !m->quadrum.pixela
        || !chorda_aequalis(provenientia, m->quadri_titulus))
    {
        redde NIHIL;
    }
    redde &m->quadrum;
}

chorda
statuae_titulus (
     vacuum* ctx,
    Piscina* piscina)
{
    StatuaeMontatio* m = (StatuaeMontatio*)ctx;
                s32  locus;

    si (!m || (locus = locus_statuae(m)) < ZEPHYRUM)
    {
        redde chorda_ex_literis("", piscina);
    }
    redde chorda_transcribere(inscriptio(m, (i32)locus)->titulus,
        piscina);
}


/* ==================================================
 * Delectus: optiones
 * ================================================== */

/* m->menu ex delectu aperto (cyclus aut modus); FALSUM si nullus */
interior b32
optiones_parare (
    StatuaeMontatio* m)
{
    i32 k;
    i32 n;

    m->menu.res      = m->menu_res;
    m->menu.numerus  = ZEPHYRUM;
    m->menu.electa   = -I;
    memset(m->menu_res, ZEPHYRUM, magnitudo(m->menu_res));
    si (delectus_apertus(&m->ramus, DELECTUS_MODI))
    {
        per (k = ZEPHYRUM; k < IV; k++)
        {
            m->menu_res[k].titulus  = statuae_modi_titulus(MODI[k]);
            m->menu_res[k].textus   = MODORUM_NOMINA[k];
            m->menu_res[k].habilis  = VERUM;
        }
        m->menu.numerus = IV;
    }
    alioquin si (   delectus_apertus(&m->ramus, DELECTUS_CYCLI)
                 && m->statua)
    {
        n = m->statua->cyclorum > STATUAE_OPTIONES_MAXIMAE
            ? STATUAE_OPTIONES_MAXIMAE : m->statua->cyclorum;
        per (k = ZEPHYRUM; k < n; k++)
        {
            sprintf(m->menu_textus[k], "%.*s",
                (integer)(m->statua->cycli[k].titulus.mensura
                    < STATUAE_OPTIONIS_TEXTUS - I
                    ? m->statua->cycli[k].titulus.mensura
                    : STATUAE_OPTIONIS_TEXTUS - I),
                (constans character*)m->statua->cycli[k].titulus.datum);
            m->menu_res[k].titulus  = m->menu_textus[k];
            m->menu_res[k].textus   = m->menu_textus[k];
            m->menu_res[k].habilis  = VERUM;
        }
        m->menu.numerus = n;
    }
    si (m->menu.numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    m->menu.electa = delectus_electa(&m->ramus, m->menu.numerus);
    redde VERUM;
}


/* ==================================================
 * Praecepta
 * ================================================== */

interior b32
visum_movere (
    StatuaeMontatio* m,
                s32  gyrus,
                s32  inclinatio)
{
    redde visum_scribere(m, gyrus_currens(m) + gyrus,
        inclinatio_currens(m) + inclinatio);
}

interior b32
fac_gyrum_sinistrum (
    vacuum* ctx)
{
    redde visum_movere((StatuaeMontatio*)ctx, -STATUAE_GRADUS_CLAVIS,
        ZEPHYRUM);
}

interior b32
fac_gyrum_dextrum (
    vacuum* ctx)
{
    redde visum_movere((StatuaeMontatio*)ctx, STATUAE_GRADUS_CLAVIS,
        ZEPHYRUM);
}

interior b32
fac_inclinationem_infra (
    vacuum* ctx)
{
    redde visum_movere((StatuaeMontatio*)ctx, ZEPHYRUM,
        -STATUAE_GRADUS_CLAVIS);
}

interior b32
fac_inclinationem_supra (
    vacuum* ctx)
{
    redde visum_movere((StatuaeMontatio*)ctx, ZEPHYRUM,
        STATUAE_GRADUS_CLAVIS);
}

interior b32
fac_visum_ordinarium (
    vacuum* ctx)
{
    redde visum_scribere((StatuaeMontatio*)ctx, GYRUS_ORDINARIUS,
        INCLINATIO_ORDINARIA);
}

/* statua 'gradus' locis ab hac (circulo); visus ad ordinarium, cyclus
 * ad primum statuae novae (titulus ignotus ibi) */
interior b32
statuam_mutare (
    StatuaeMontatio* m,
                s32  gradus)
{
    GrexCamporum grex;
       character titulus[STATUAE_TITULUS_MAXIMUS + I];
          chorda t;
             s32 n = (s32)statuarum_numerus(m);
             s32 locus;

    si (n < II)
    {
        redde n > ZEPHYRUM;
    }
    locus  = (locus_statuae(m) + gradus + n) % n;
    t      = inscriptio(m, (i32)locus)->titulus;
    sprintf(titulus, "%.*s", (integer)(t.mensura
        <= STATUAE_TITULUS_MAXIMUS ? t.mensura
                                   : STATUAE_TITULUS_MAXIMUS),
        (constans character*)t.datum);
    grex.pares[ZEPHYRUM].titulus  = "statua";
    grex.pares[ZEPHYRUM].valor    = titulus;
    grex.pares[I].titulus         = "gyrus";
    grex.pares[I].valor           = "";
    grex.pares[II].titulus        = "inclinatio";
    grex.pares[II].valor          = "";
    grex.numerus                  = III;
    redde gregem_scribere(m, &grex);
}

interior b32
fac_statuam_priorem (
    vacuum* ctx)
{
    redde statuam_mutare((StatuaeMontatio*)ctx, -I);
}

interior b32
fac_statuam_proximam (
    vacuum* ctx)
{
    redde statuam_mutare((StatuaeMontatio*)ctx, I);
}

interior b32
fac_delectum_cycli (
    vacuum* ctx)
{
    StatuaeMontatio* m = (StatuaeMontatio*)ctx;

    si (cyclus_currens(m) < ZEPHYRUM)
    {
        redde VERUM;
    }
    redde delectus_aperire(&m->ramus, actio_statuarum, DELECTUS_CYCLI,
        cyclus_currens(m), m->piscina);
}

interior b32
fac_delectum_modi (
    vacuum* ctx)
{
    StatuaeMontatio* m = (StatuaeMontatio*)ctx;

    redde delectus_aperire(&m->ramus, actio_statuarum, DELECTUS_MODI,
        (s32)modus_currens(m), m->piscina);
}

interior b32
veritatem_commutare (
       StatuaeMontatio* m,
    constans character* titulus)
{
    redde campum_scribere(m, titulus,
        attributum_veritas(&m->ramus, titulus) ? "" : "verum");
}

interior b32
fac_lucem (
    vacuum* ctx)
{
    redde veritatem_commutare((StatuaeMontatio*)ctx, "planum");
}

interior b32
fac_duplicem (
    vacuum* ctx)
{
    redde veritatem_commutare((StatuaeMontatio*)ctx, "duplex");
}

/* quies: tempus cycli hic sistit; cursus: inde pergit */
interior b32
fac_quietem (
    vacuum* ctx)
{
    StatuaeMontatio* m = (StatuaeMontatio*)ctx;
                s64  t;

    si (quiescit(m))
    {
        m->origo_ms = m->hora_visa - (s64)m->quietum_ms;
        redde campum_scribere(m, "quiescit", "");
    }
    t              = m->hora_visa - m->origo_ms;
    m->quietum_ms  = t > ZEPHYRUM && t < 0x7FFFFFFF ? (s32)t : ZEPHYRUM;
    redde campum_scribere(m, "quiescit", "verum");
}

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "statuae.gyrus_sinister", "Turn left", "h", VISUS_STATUARUM,
        ZEPHYRUM, fac_gyrum_sinistrum },
    { "statuae.gyrus_dexter", "Turn right", "l", VISUS_STATUARUM,
        ZEPHYRUM, fac_gyrum_dextrum },
    { "statuae.inclinatio_infra", "Look from below", "j",
        VISUS_STATUARUM, ZEPHYRUM, fac_inclinationem_infra },
    { "statuae.inclinatio_supra", "Look from above", "k",
        VISUS_STATUARUM, ZEPHYRUM, fac_inclinationem_supra },
    { "statuae.visus", "Reset view", "r", VISUS_STATUARUM, ZEPHYRUM,
        fac_visum_ordinarium },
    { "statuae.prior", "Previous model", "[", VISUS_STATUARUM,
        ZEPHYRUM, fac_statuam_priorem },
    { "statuae.proxima", "Next model", "]", VISUS_STATUARUM, ZEPHYRUM,
        fac_statuam_proximam },
    { "statuae.cyclus", "Choose clip", "c", VISUS_STATUARUM, ZEPHYRUM,
        fac_delectum_cycli },
    { "statuae.modus", "Choose dither", "d", VISUS_STATUARUM, ZEPHYRUM,
        fac_delectum_modi },
    { "statuae.lux", "Lit / flat", "f", VISUS_STATUARUM, ZEPHYRUM,
        fac_lucem },
    { "statuae.geminatum", "Half size, doubled", "2", VISUS_STATUARUM,
        ZEPHYRUM, fac_duplicem },
    { "statuae.quies", "Pause / resume", " ", VISUS_STATUARUM,
        ZEPHYRUM, fac_quietem },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

interior b32
requisita_nulla (
       i32  requisita,
    vacuum* ctx)
{
    (vacuum)requisita;
    (vacuum)ctx;
    redde VERUM;
}


/* ==================================================
 * Actio
 * ================================================== */

/* optio delectus electa: campum suum scribit */
interior vacuum
optionem_eligere (
    StatuaeMontatio* m,
                b32  modi,
                s32  index)
{
    si (index < ZEPHYRUM || index >= (s32)m->menu.numerus)
    {
        redde;
    }
    si (modi)
    {
        (vacuum)campum_scribere(m, "modus",
            statuae_modi_titulus(MODI[index]));
        redde;
    }
    (vacuum)campum_scribere(m, "cyclus", m->menu_textus[index]);
    /* cyclus novus ab initio */
    m->origo_ms    = m->hora_visa;
    m->quietum_ms  = ZEPHYRUM;
}

interior vacuum
tractum_finire (
    StatuaeMontatio* m,
              Motus* motus)
{
    s32 gyrus       = m->gyrus;
    s32 inclinatio  = m->inclinatio;

    m->trahitur = FALSUM;
    si (motus)
    {
        motus_captura_tollere(motus);
    }
    (vacuum)visum_scribere(m, gyrus, inclinatio);
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
         StatuaeMontatio* m = (StatuaeMontatio*)ctx;
     constans Praeceptum* t;
          DelectusExitus  exitus;
                     b32  modi;
                     s32  electum = -I;

    (vacuum)repo;
    (vacuum)des;
    /* delectus apertus eventa prima sumit */
    si (delectus_apertus(&m->ramus, NIHIL))
    {
        modi = delectus_apertus(&m->ramus, DELECTUS_MODI);
        si (!optiones_parare(m))
        {
            (vacuum)delectus_claudere(&m->ramus, actio_statuarum,
                m->piscina);
        }
        alioquin
        {
            exitus = delectus_eventum_agere(&m->ramus, actio_statuarum,
                &m->menu, nodus, ev, &electum, m->piscina);
            si (exitus == DELECTUS_ELECTUM)
            {
                optionem_eligere(m, modi, electum);
            }
            si (exitus != DELECTUS_NIHIL)
            {
                redde VERUM;
            }
        }
    }
    si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        t = praecepta_ad_clavem(TABULA_PRAECEPTORUM, VISUS_STATUARUM,
            ev);
        redde t && praecepta_status(t, VISUS_STATUARUM,
            requisita_nulla, m) == PRAECEPTUM_PRAESTO && t->fn(m);
    }
    commutatio (ev->genus)
    {
        casus EVENTUS_MUS_DEPRESSUS:
            si (!nodus || ev->datum.mus.botton != MUS_SINISTER)
            {
                redde FALSUM;
            }
            si (chorda_aequalis_literis(nodus->id, "sagitta_prior"))
            {
                redde fac_statuam_priorem(m);
            }
            si (chorda_aequalis_literis(nodus->id, "sagitta_proxima"))
            {
                redde fac_statuam_proximam(m);
            }
            si (chorda_aequalis_literis(nodus->id, DELECTUS_CYCLI))
            {
                redde fac_delectum_cycli(m);
            }
            si (chorda_aequalis_literis(nodus->id, DELECTUS_MODI))
            {
                redde fac_delectum_modi(m);
            }
            si (chorda_aequalis_literis(nodus->id, "lux"))
            {
                redde fac_lucem(m);
            }
            si (chorda_aequalis_literis(nodus->id, "duplex"))
            {
                redde fac_duplicem(m);
            }
            si (!chorda_aequalis_literis(nodus->id, "pictura"))
            {
                redde FALSUM;
            }
            m->gyrus       = gyrus_currens(m);
            m->inclinatio  = inclinatio_currens(m);
            m->trahitur    = VERUM;
            m->tractus_x   = (s32)ev->datum.mus.x;
            m->tractus_y   = (s32)ev->datum.mus.y;
            si (motus)
            {
                motus_captura_ponere(motus, nodus->id);
            }
            redde VERUM;
        casus EVENTUS_MUS_MOTUS:
            si (!m->trahitur)
            {
                redde FALSUM;
            }
            m->gyrus = gyrum_cogere(m->gyrus
                + ((s32)ev->datum.mus.x - m->tractus_x)
                    * STATUAE_TRACTUS_DECIMAE);
            m->inclinatio = inclinationem_cogere(m->inclinatio
                + ((s32)ev->datum.mus.y - m->tractus_y)
                    * STATUAE_TRACTUS_DECIMAE);
            m->tractus_x  = (s32)ev->datum.mus.x;
            m->tractus_y  = (s32)ev->datum.mus.y;
            redde VERUM;
        casus EVENTUS_MUS_LIBERATUS:
            si (!m->trahitur)
            {
                redde FALSUM;
            }
            tractum_finire(m, motus);
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

b32
statuae_ictus_primus (
                vacuum* ctx,
    InsulaRepositorium* repo,
                 Motus* motus,
             Componens* arbor,
      constans Eventus* ev)
{
    StatuaeMontatio* m = (StatuaeMontatio*)ctx;
          Componens* pictura;
            Punctum  p;

    si (   !m || !arbor || !ev || ev->genus != EVENTUS_MUS_DEPRESSUS
        || ev->datum.mus.botton != MUS_SINISTER
        || delectus_apertus(&m->ramus, NIHIL))
    {
        redde FALSUM;
    }
    pictura = componens_invenire_in_spatio(arbor, m->ramus.via,
        chorda_ex_literis("pictura", m->piscina));
    si (!pictura)
    {
        redde FALSUM;
    }
    p.x  = (s32)ev->datum.mus.x;
    p.y  = (s32)ev->datum.mus.y;
    p    = destinatio_ad_locale(pictura, p);
    si (   p.x < ZEPHYRUM || p.y < ZEPHYRUM
        || p.x >= pictura->fines.latitudo
        || p.y >= pictura->fines.altitudo)
    {
        redde FALSUM;
    }
    redde agere(repo, motus, NIHIL, pictura, ev, m);
}


/* ==================================================
 * Figura
 * ================================================== */

interior vacuum
textum_pingere (
               Mandata* md,
                   s32  x,
                   s32  y,
                chorda  textus,
            ColorThema  color)
{
    mandata_textus(md, x, y, textus, FONS_PARVUS,
        figura_color_thematis(color));
}

/* textus in lineas latitudinis datae fractus (ad spatia; verbum
 * longius linea scinditur) */
interior vacuum
textum_fractum_pingere (
       Mandata* md,
           s32  x,
           s32  y,
           s32  latitudo,
        chorda  textus,
    ColorThema  color)
{
    chorda linea;
       i32 per_lineam = latitudo / LITTERA_LATITUDO > ZEPHYRUM
           ? (i32)(latitudo / LITTERA_LATITUDO) : I;
       i32 initium    = ZEPHYRUM;
       i32 finis;
       i32 k;

    dum (initium < textus.mensura)
    {
        finis = initium + per_lineam;
        si (finis >= textus.mensura)
        {
            finis = textus.mensura;
        }
        alioquin
        {
            per (k = finis; k > initium; k--)
            {
                si (textus.datum[k] == ' ')
                {
                    finis = k;
                    frange;
                }
            }
        }
        linea.datum    = textus.datum + initium;
        linea.mensura  = finis - initium;
        textum_pingere(md, x, y, linea, color);
        y        += LITTERA_ALTITUDO + II;
        initium  = finis;
        dum (initium < textus.mensura && textus.datum[initium] == ' ')
        {
            initium++;
        }
    }
}

interior vacuum
figura_statuarum (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
    StatuaeMontatio* m = (StatuaeMontatio*)ctx;
         Dispositio  d;
              Fines  f;
              Fines  imago;
             chorda  titulus;
          character  numerus[XXXII];
                s32  y_textus = (ORDO_ALTITUDO - LITTERA_ALTITUDO) / II;
                s32  locus;
                b32  plures;

    (vacuum)thema;
    f = fines(ZEPHYRUM, ZEPHYRUM, c->fines.latitudo, c->fines.altitudo);
    figura_mensam_pingere(md, f, chorda_aequalis_literis(c->titulus,
        "focatum"));
    si (!m)
    {
        redde;
    }
    disponere(m, md->piscina, &d);
    locus   = locus_statuae(m);
    plures  = statuarum_numerus(m) > I;
    /* caput: sagittae, numerus, titulus */
    figura_sagittam_pingere(md, d.prior, -I,
        plures ? ZEPHYRUM : FIGURA_OBSCURUM);
    numerum_formare(m, numerus);
    textum_pingere(md, d.numerus_x, y_textus,
        chorda_ex_literis(numerus, md->piscina), COLOR_TEXT_DIM);
    figura_sagittam_pingere(md, d.proxima, I,
        plures ? ZEPHYRUM : FIGURA_OBSCURUM);
    si (locus >= ZEPHYRUM)
    {
        titulus = inscriptio(m, (i32)locus)->titulus;
        si ((s32)titulus.mensura * LITTERA_LATITUDO
            > d.titulus_latitudo)
        {
            titulus.mensura = (i32)(d.titulus_latitudo
                / LITTERA_LATITUDO);
        }
        textum_pingere(md, d.titulus_x, y_textus, titulus, COLOR_TEXT);
    }
    /* delectus et commutatores */
    delectus_bottonem_pingere(md, d.cyclus, cycli_textus(m,
        md->piscina), delectus_apertus(&m->ramus, DELECTUS_CYCLI),
        cyclus_currens(m) >= ZEPHYRUM);
    delectus_bottonem_pingere(md, d.modus, chorda_ex_literis(
        MODORUM_NOMINA[modus_currens(m)], md->piscina),
        delectus_apertus(&m->ramus, DELECTUS_MODI), VERUM);
    figura_globulum_pingere(md, d.lux, chorda_ex_literis(
        attributum_veritas(&m->ramus, "planum") ? "flat" : "lit",
        md->piscina), ZEPHYRUM);
    figura_globulum_pingere(md, d.gemina, chorda_ex_literis("2x",
        md->piscina), attributum_veritas(&m->ramus, "duplex")
        ? FIGURA_ELECTUM : ZEPHYRUM);
    /* fundus planus sub pictura: indicium foci (exemplar mensae) in
     * capite manet, non circa statuam. Quadrum ipsum chartam opacam
     * fert (Albus) */
    mandata_rectangulum(md, d.pictura,
        figura_color_thematis(COLOR_BACKGROUND), VERUM);
    /* pictura, aut status vacuus / recusatus */
    si (m->quadrum.pixela)
    {
        imago = fines(
            d.pictura.x + ((s32)d.pictura.latitudo
                - (s32)m->quadrum.latitudo) / II,
            d.pictura.y + ((s32)d.pictura.altitudo
                - (s32)m->quadrum.altitudo) / II,
            (s32)m->quadrum.latitudo, (s32)m->quadrum.altitudo);
        mandata_imago(md, m->quadri_titulus, imago);
        redde;
    }
    si (locus < ZEPHYRUM)
    {
        textum_pingere(md, LIMBUS * IV, d.pictura.y + XVI,
            chorda_ex_literis("No models yet", md->piscina),
            COLOR_TEXT);
        textum_fractum_pingere(md, LIMBUS * IV, d.pictura.y + XXXII,
            (s32)d.pictura.latitudo - LIMBUS * VIII,
            chorda_ex_literis("statuae addere <file.glb> -volumen "
                "<volume>", md->piscina), COLOR_TEXT_DIM);
        redde;
    }
    si (m->causa.mensura > ZEPHYRUM)
    {
        textum_pingere(md, LIMBUS * IV, d.pictura.y + XVI,
            chorda_ex_literis("This model cannot be shown:",
                md->piscina), COLOR_TEXT);
        textum_fractum_pingere(md, LIMBUS * IV, d.pictura.y + XXXII,
            (s32)d.pictura.latitudo - LIMBUS * VIII, m->causa,
            COLOR_TEXT_DIM);
    }
}


/* ==================================================
 * Componere et montare
 * ================================================== */

interior vacuum
aream_addere (
              Componens* radix,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* id,
                  Fines  f)
{
    Componens* c = componens_creare(piscina, intern, id, PARTES_NULLUM);

    si (!c)
    {
        redde;
    }
    componens_ponere_fines(c, f);
    componens_ponere_actio(c, actio_statuarum);
    componens_addere_liberum(radix, c);
}

Componens*
statuae_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
    StatuaeMontatio* m = (StatuaeMontatio*)ctx;
          Componens* radix;
          Componens* liber;
         Dispositio  d;
              Fines  f;

    (vacuum)repo;
    si (!m)
    {
        redde NIHIL;
    }
    /* pictura statum hunc sequatur (eventus statum modo mutavit) */
    (vacuum)statuam_curare(m);
    (vacuum)quadrum_curare(m, piscina);
    radix = componens_creare(piscina, intern, "statuae", PARTES_TABULA);
    si (!radix)
    {
        redde NIHIL;
    }
    disponere(m, piscina, &d);
    f = fines(ZEPHYRUM, ZEPHYRUM, superficies(m, VERUM),
        superficies(m, FALSUM));
    componens_ponere_fines(radix, f);
    componens_ponere_focusabilis(radix, VERUM);
    componens_ponere_actio(radix, actio_statuarum);
    si (motus_ramus_focatus(motus, m->ramus))
    {
        componens_ponere_titulum(radix, "focatum");
    }
    aream_addere(radix, piscina, intern, "pictura", d.pictura);
    aream_addere(radix, piscina, intern, "sagitta_prior", d.prior);
    aream_addere(radix, piscina, intern, "sagitta_proxima", d.proxima);
    aream_addere(radix, piscina, intern, "lux", d.lux);
    aream_addere(radix, piscina, intern, "duplex", d.gemina);
    liber = delectus_bottonem_componere(DELECTUS_CYCLI, d.cyclus,
        actio_statuarum, piscina, intern);
    si (liber)
    {
        componens_addere_liberum(radix, liber);
    }
    liber = delectus_bottonem_componere(DELECTUS_MODI, d.modus,
        actio_statuarum, piscina, intern);
    si (liber)
    {
        componens_addere_liberum(radix, liber);
    }
    /* menu delectus aperti: filius ULTIMUS */
    si (delectus_apertus(&m->ramus, NIHIL) && optiones_parare(m))
    {
        liber = delectus_componere(&m->menu,
            delectus_apertus(&m->ramus, DELECTUS_MODI) ? d.modus
                                                       : d.cyclus,
            f, actio_statuarum, piscina, intern);
        si (liber)
        {
            componens_addere_liberum(radix, liber);
        }
    }
    redde radix;
}

vacuum
statuae_dimittere (
    StatuaeMontatio* m)
{
    si (!m)
    {
        redde;
    }
    si (m->piscina_indicis)
    {
        piscina_destruere(m->piscina_indicis);
    }
    si (m->piscina_statuae)
    {
        piscina_destruere(m->piscina_statuae);
    }
    si (m->piscina_quadri)
    {
        piscina_destruere(m->piscina_quadri);
    }
    m->piscina_indicis  = NIHIL;
    m->piscina_statuae  = NIHIL;
    m->piscina_quadri   = NIHIL;
    m->index            = NIHIL;
    m->statua           = NIHIL;
    m->quadrum.pixela   = NIHIL;
}

b32
statuae_montare (
          StatuaeMontatio* m,
                  Piscina* piscina,
      InternamentumChorda* intern,
                  Volumen* volumen,
       InsulaRepositorium* repo,
       constans character* id,
    constans StatuaePorta* porta,
                      i32  latitudo,
                      i32  altitudo,
          StatusSystemata* systemata)
{
      Organum* organum;
    character  titulus[CXXVIII];

    si (!m || !piscina || !intern || !repo)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(StatuaeMontatio));
    m->piscina   = piscina;
    m->intern    = intern;
    m->volumen   = volumen;
    m->latitudo  = latitudo;
    m->altitudo  = altitudo;
    si (porta)
    {
        m->porta = *porta;
    }
    m->ramus = id ? insula_ramus(repo, "statuae", id)
                  : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "statuae",
        "statuae");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"statuae\"", id ? id : "statuae", piscina,
               intern))
    {
        redde FALSUM;
    }
    m->piscina_indicis  = piscina_generare_dynamicum("statuae_index",
        0x10000);
    m->piscina_statuae  = piscina_generare_dynamicum("statuae_statua",
        0x100000);
    m->piscina_quadri   = piscina_generare_dynamicum("statuae_quadrum",
        0x400000);
    si (   !m->piscina_indicis || !m->piscina_statuae
        || !m->piscina_quadri)
    {
        redde FALSUM;
    }
    sprintf(titulus, "statuae:%.100s", id ? id : "statuae");
    m->quadri_titulus  = chorda_ex_literis(titulus, piscina);
    m->hora_visa       = nunc(m);
    m->origo_ms        = m->hora_visa;
    indicem_relegere(m);
    m->actiones  = actio_registrum_creare(piscina, intern);
    m->figurae   = figura_registrum_creare(piscina);
    redde m->actiones && m->figurae
        && praecepta_verificare(TABULA_PRAECEPTORUM)
        && actio_registrare(m->actiones, actio_statuarum, agere, m)
        && figura_registrare(m->figurae, PARTES_TABULA, ZEPHYRUM,
               figura_statuarum, m)
        && figura_registrare(m->figurae, PARTES_DIALOGUS, ZEPHYRUM,
               elenchus_figura, &m->menu);
}
