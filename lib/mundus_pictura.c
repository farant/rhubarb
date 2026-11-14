/* mundus_pictura.c - humus mundi (vide mundus_pictura.h)
 *
 * Regio (i, j) pixela mundi [i * R, (i + 1) * R) x [j * R, (j + 1) *
 * R) tegit. Pingitur cum MARGINE circum (Atkinson errorem dextrorsum
 * et deorsum fundit: sine margine ora sinistra et summa regionis
 * errorem vicinae non acciperet et sutura appareret); margo abicitur.
 */

#include "mundus_pictura.h"
#include "dithering.h"
#include "dithering_exemplaria_velox.h"
#include "statuae_quadrum.h"
#include "stereometria.h"
#include <math.h>
#include <string.h>

/* margo circa regionem dum pingitur et redigitur (pixela) */
#define REGIONIS_MARGO   XVI
/* index internus: pixelum regionis quod locus non tegit */
#define INDEX_EXTRA      0xFF
/* tabula regionum maxima (per latus) */
#define REGIONES_MAXIMAE CCLVI

nomen structura {
     i8* indices;       /* R * R; NIHIL = nondum facta */
    f32* profunditas;   /* R * R */
} Regio;

structura MundusHumus {
        constans Statua* locus;
     StereometriaMatrix* matrices;      /* quies */
                Piscina* piscina;
                    b32  activi[XVI];
        MundusHumiModus  modus;
    /* regiones quas locus tangit: [i_min, i_min + latitudo) etc. */
                    s32  i_min;
                    s32  j_min;
                    i32  latitudo;      /* regiones; 0 = locus vacuus */
                    i32  altitudo;
                  Regio* regiones;
                    i32  factae;
};

StatuaScaenaVisus
mundus_visus (
    s32 origo_x,
    s32 origo_y)
{
    StatuaScaenaVisus v;

    v.gyrus                = MUNDUS_GYRUS;
    v.inclinatio           = MUNDUS_INCLINATIO;
    v.pixela_per_unitatem  = MUNDUS_PIXELA_PER_UNITATEM;
    v.origo_x              = origo_x;
    v.origo_y              = origo_y;
    v.illuminatum          = VERUM;
    redde v;
}

/* divisio deorsum rotundata (dividens positivus) */
interior s32
_regio_pixeli (
    s32 pixelum)
{
    redde pixelum >= ZEPHYRUM ? pixelum / (s32)MUNDUS_REGIO
        : -((-pixelum + (s32)MUNDUS_REGIO - I) / (s32)MUNDUS_REGIO);
}

/* fines loci in pixelis mundi; FALSUM si locus sine verticibus */
interior b32
_fines_loci (
    constans MundusHumus* h,
                     f32* fines)
{
         constans Statua* s      = h->locus;
       StatuaScaenaVisus  visus  = mundus_visus(ZEPHYRUM, ZEPHYRUM);
     constans StatuaRete* rete;
     constans StatuaPars* pars;
      StereometriaVector  punctum;
                     f32  x            = 0.0f;
                     f32  y            = 0.0f;
                     f32  profunditas  = 0.0f;
                     b32  aliquid      = FALSUM;
                     i32  n;
                     i32  j;
                     i32  k;
                     i32  vertex;

    per (n = ZEPHYRUM; n < s->nodorum; n++)
    {
        si (s->nodi[n].rete < ZEPHYRUM)
        {
            perge;
        }
        rete = &s->retia[s->nodi[n].rete];
        per (j = ZEPHYRUM; j < rete->partium; j++)
        {
            pars = &s->partes[rete->pars_prima + j];
            per (k = ZEPHYRUM; k < pars->indicum; k++)
            {
                vertex   = s->indices[pars->index_primus + k];
                punctum  = stereometria_punctum_transformare(
                    h->matrices[n],
                    stereometria_vector(s->positiones[vertex * III],
                        s->positiones[vertex * III + I],
                        s->positiones[vertex * III + II]), NIHIL);
                statua_scaena_proicere(&visus, punctum, &x, &y,
                    &profunditas);
                si (!aliquid || x < fines[ZEPHYRUM])
                {
                    fines[ZEPHYRUM] = x;
                }
                si (!aliquid || x > fines[I])
                {
                    fines[I] = x;
                }
                si (!aliquid || y < fines[II])
                {
                    fines[II] = y;
                }
                si (!aliquid || y > fines[III])
                {
                    fines[III] = y;
                }
                aliquid = VERUM;
            }
        }
    }
    redde aliquid;
}

MundusHumus*
mundus_humus_creare (
    constans Statua* locus,
       constans b32* activi,
            Piscina* piscina)
{
    redde mundus_humus_creare_modi(locus, activi, MUNDUS_HUMUS_ATKINSON,
        piscina);
}

MundusHumus*
mundus_humus_creare_modi (
    constans Statua* locus,
       constans b32* activi,
    MundusHumiModus  modus,
            Piscina* piscina)
{
       MundusHumus* h;
    memoriae_index  mensura;
               f32  fines[IV];
               s32  i_max;
               s32  j_max;

    si (   !locus || !activi || !piscina || (s32)modus < ZEPHYRUM
        || modus >= MUNDUS_HUMI_MODORUM)
    {
        redde NIHIL;
    }
    h = (MundusHumus*)piscina_allocare(piscina, magnitudo(MundusHumus));
    si (!h)
    {
        redde NIHIL;
    }
    memset(h, ZEPHYRUM, magnitudo(MundusHumus));
    h->locus    = locus;
    h->piscina  = piscina;
    h->modus    = modus;
    memcpy(h->activi, activi, magnitudo(h->activi));
    h->matrices = (StereometriaMatrix*)piscina_allocare(piscina,
        ((memoriae_index)locus->nodorum + I)
            * magnitudo(StereometriaMatrix));
    si (!h->matrices)
    {
        redde NIHIL;
    }
    statua_positura(locus, -I, 0.0f, h->matrices);
    memset(fines, ZEPHYRUM, magnitudo(fines));
    si (!_fines_loci(h, fines))
    {
        redde h;
    }
    h->i_min  = _regio_pixeli((s32)floor((f64)fines[ZEPHYRUM]));
    i_max     = _regio_pixeli((s32)floor((f64)fines[I]));
    h->j_min  = _regio_pixeli((s32)floor((f64)fines[II]));
    j_max     = _regio_pixeli((s32)floor((f64)fines[III]));
    si (   i_max - h->i_min >= (s32)REGIONES_MAXIMAE
        || j_max - h->j_min >= (s32)REGIONES_MAXIMAE)
    {
        redde NIHIL;
    }
    h->latitudo = (i32)(i_max - h->i_min + I);
    h->altitudo = (i32)(j_max - h->j_min + I);
    mensura      = (memoriae_index)h->latitudo * h->altitudo
        * magnitudo(Regio);
    h->regiones  = (Regio*)piscina_allocare(piscina, mensura);
    si (!h->regiones)
    {
        redde NIHIL;
    }
    memset(h->regiones, ZEPHYRUM, mensura);
    redde h;
}

/* regionem (i, j) pingere, redigere, servare */
interior b32
_regionem_facere (
    MundusHumus* h,
          Regio* r,
            s32  i,
            s32  j)
{
           StatuaLocata locata;
      StatuaScaenaVisus visus;
    StatuaScaenaPictura pictura;
       DitheringFructus fructus;
         PiscinaNotatio nota;
         memoriae_index fons;
         memoriae_index meta;
                    i32 latus = MUNDUS_REGIO + II * REGIONIS_MARGO;
                    i32 x;
                    i32 y;

    r->indices      = (i8*)piscina_allocare(h->piscina,
        (memoriae_index)MUNDUS_REGIO * MUNDUS_REGIO);
    r->profunditas  = (f32*)piscina_allocare(h->piscina,
        (memoriae_index)MUNDUS_REGIO * MUNDUS_REGIO * magnitudo(f32));
    si (!r->indices || !r->profunditas)
    {
        r->indices = NIHIL;
        redde FALSUM;
    }
    nota             = piscina_notare(h->piscina);
    locata.statua    = h->locus;
    locata.matrices  = h->matrices;
    locata.locatio   = stereometria_identitas();
    visus = mundus_visus(i * (s32)MUNDUS_REGIO - (s32)REGIONIS_MARGO,
        j * (s32)MUNDUS_REGIO - (s32)REGIONIS_MARGO);
    si (!statua_scaenam_pingere(&locata, I, &visus, latus, latus,
            &pictura, h->piscina))
    {
        piscina_reficere(h->piscina, nota);
        r->indices = NIHIL;
        redde FALSUM;
    }
    fructus = h->modus == MUNDUS_HUMUS_EXEMPLARIA
        ? dithering_exemplaria_velox_colorum(pictura.rgba, latus, latus,
              h->activi, NIHIL, NIHIL, h->piscina)
        : dithering_atkinson_colorum(pictura.rgba, latus, latus,
              h->activi, h->piscina);
    si (!fructus.successus || !fructus.indices)
    {
        piscina_reficere(h->piscina, nota);
        r->indices = NIHIL;
        redde FALSUM;
    }
    per (y = ZEPHYRUM; y < MUNDUS_REGIO; y++)
    {
        per (x = ZEPHYRUM; x < MUNDUS_REGIO; x++)
        {
            fons  = (memoriae_index)(y + REGIONIS_MARGO) * latus
                + (memoriae_index)(x + REGIONIS_MARGO);
            meta  = (memoriae_index)y * MUNDUS_REGIO
                + (memoriae_index)x;
            r->indices[meta] = pictura.tectum[fons]
                ? fructus.indices[fons] : (i8)INDEX_EXTRA;
            r->profunditas[meta] = pictura.tectum[fons]
                ? pictura.profunditas[fons] : MUNDUS_PROFUNDITAS_VACUA;
        }
    }
    piscina_reficere(h->piscina, nota);
    h->factae++;
    redde VERUM;
}

b32
mundus_humus_copiare (
    MundusHumus* h,
            s32  origo_x,
            s32  origo_y,
            i32  latitudo,
            i32  altitudo,
             i8  fundus,
             i8* indices,
            f32* profunditas)
{
             Regio* r;
    memoriae_index  pixela;
    memoriae_index  k;
    memoriae_index  fons;
    memoriae_index  meta;
               s32  i;
               s32  j;
               s32  i_primus;
               s32  i_ultimus;
               s32  j_primus;
               s32  j_ultimus;
               s32  x0;
               s32  x1;
               s32  y0;
               s32  y1;
               s32  x;
               s32  y;
                i8  index;

    si (!h || !indices)
    {
        redde FALSUM;
    }
    si (latitudo == ZEPHYRUM || altitudo == ZEPHYRUM)
    {
        redde VERUM;
    }
    pixela = (memoriae_index)latitudo * altitudo;
    memset(indices, fundus, pixela);
    per (k = ZEPHYRUM; profunditas && k < pixela; k++)
    {
        profunditas[k] = MUNDUS_PROFUNDITAS_VACUA;
    }
    si (   h->latitudo == ZEPHYRUM
        || latitudo > 0x3FFFFFFF || altitudo > 0x3FFFFFFF
        || origo_x > 0x3FFFFFFF || origo_x < -0x3FFFFFFF
        || origo_y > 0x3FFFFFFF || origo_y < -0x3FFFFFFF)
    {
        redde VERUM;
    }
    i_primus   = _regio_pixeli(origo_x);
    i_ultimus  = _regio_pixeli(origo_x + (s32)latitudo - I);
    j_primus   = _regio_pixeli(origo_y);
    j_ultimus  = _regio_pixeli(origo_y + (s32)altitudo - I);
    si (i_primus < h->i_min)
    {
        i_primus = h->i_min;
    }
    si (j_primus < h->j_min)
    {
        j_primus = h->j_min;
    }
    si (i_ultimus > h->i_min + (s32)h->latitudo - I)
    {
        i_ultimus = h->i_min + (s32)h->latitudo - I;
    }
    si (j_ultimus > h->j_min + (s32)h->altitudo - I)
    {
        j_ultimus = h->j_min + (s32)h->altitudo - I;
    }
    per (j = j_primus; j <= j_ultimus; j++)
    {
        per (i = i_primus; i <= i_ultimus; i++)
        {
            r = &h->regiones[(memoriae_index)(j
                - h->j_min) * h->latitudo
                + (memoriae_index)(i - h->i_min)];
            si (!r->indices && !_regionem_facere(h, r, i, j))
            {
                redde FALSUM;
            }
            /* pars regionis intra rectangulum, in pixelis mundi */
            x0 = i * (s32)MUNDUS_REGIO;
            x1 = x0 + (s32)MUNDUS_REGIO;
            y0 = j * (s32)MUNDUS_REGIO;
            y1 = y0 + (s32)MUNDUS_REGIO;
            si (x0 < origo_x)
            {
                x0 = origo_x;
            }
            si (y0 < origo_y)
            {
                y0 = origo_y;
            }
            si (x1 > origo_x + (s32)latitudo)
            {
                x1 = origo_x + (s32)latitudo;
            }
            si (y1 > origo_y + (s32)altitudo)
            {
                y1 = origo_y + (s32)altitudo;
            }
            per (y = y0; y < y1; y++)
            {
                per (x = x0; x < x1; x++)
                {
                    fons = (memoriae_index)(y - j * (s32)MUNDUS_REGIO)
                        * MUNDUS_REGIO
                        + (memoriae_index)(x - i * (s32)MUNDUS_REGIO);
                    index = r->indices[fons];
                    si (index == (i8)INDEX_EXTRA)
                    {
                        perge;
                    }
                    meta = (memoriae_index)(y - origo_y) * latitudo
                        + (memoriae_index)(x - origo_x);
                    indices[meta] = index;
                    si (profunditas)
                    {
                        profunditas[meta] = r->profunditas[fons];
                    }
                }
            }
        }
    }
    redde VERUM;
}

i32
mundus_humus_regiones (
    constans MundusHumus* h)
{
    redde h ? h->factae : ZEPHYRUM;
}


/* ==================================================
 * Corpora et quadrum
 * ================================================== */

/* titulus fragmentum continet? (litterae ut scriptae) */
interior b32
_titulus_continet (
                chorda  titulus,
    constans character* fragmentum)
{
    i32 longitudo = (i32)strlen(fragmentum);
    i32 k;

    per (k = ZEPHYRUM; k + longitudo <= titulus.mensura; k++)
    {
        si (memcmp(titulus.datum + k, fragmentum, longitudo)
            == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

MundusForma
mundus_forma (
    constans Statua* statua,
            Piscina* piscina)
{
    constans StatuaRete* rete;
    constans StatuaPars* pars;
            MundusForma  f;
     StereometriaMatrix* matrices = NIHIL;
     StereometriaVector  p;
                    f32  minima[III];
                    f32  maxima[III];
                    f32  altitudo;
                    f32  latitudo;
                    f32  coordinata;
                    b32  aliquid = FALSUM;
                    i32  n;
                    i32  j;
                    i32  k;
                    i32  i;
                    i32  vertex;

    memset(&f, ZEPHYRUM, magnitudo(f));
    f.statua            = statua;
    f.cyclus_ambulandi  = -I;
    f.cyclus_quietis    = -I;
    f.scala             = 1.0f;
    si (!statua)
    {
        redde f;
    }
    per (k = ZEPHYRUM; k < statua->cyclorum; k++)
    {
        si (   f.cyclus_ambulandi < ZEPHYRUM
            && _titulus_continet(statua->cycli[k].titulus, "walk"))
        {
            f.cyclus_ambulandi = (s32)k;
        }
        si (   f.cyclus_quietis < ZEPHYRUM
            && _titulus_continet(statua->cycli[k].titulus, "idle"))
        {
            f.cyclus_quietis = (s32)k;
        }
    }
    si (f.cyclus_ambulandi < ZEPHYRUM && statua->cyclorum > ZEPHYRUM)
    {
        f.cyclus_ambulandi = ZEPHYRUM;
    }
    /* capsa positurae quietis (sine piscina: per sphaeram
     * ambientem) */
    si (piscina)
    {
        matrices = (StereometriaMatrix*)piscina_allocare(piscina,
            ((memoriae_index)statua->nodorum + I)
                * magnitudo(StereometriaMatrix));
    }
    si (matrices)
    {
        statua_positura(statua, -I, 0.0f, matrices);
    }
    per (n = ZEPHYRUM; matrices && n < statua->nodorum; n++)
    {
        si (statua->nodi[n].rete < ZEPHYRUM)
        {
            perge;
        }
        rete = &statua->retia[statua->nodi[n].rete];
        per (j = ZEPHYRUM; j < rete->partium; j++)
        {
            pars = &statua->partes[rete->pars_prima + j];
            per (k = ZEPHYRUM; k < pars->indicum; k++)
            {
                vertex  = statua->indices[pars->index_primus + k] * III;
                p       = stereometria_punctum_transformare(matrices[n],
                    stereometria_vector(statua->positiones[vertex],
                        statua->positiones[vertex + I],
                        statua->positiones[vertex + II]), NIHIL);
                per (i = ZEPHYRUM; i < III; i++)
                {
                    coordinata = i == ZEPHYRUM ? p.x
                        : (i == I ? p.y : p.z);
                    si (!aliquid || coordinata < minima[i])
                    {
                        minima[i] = coordinata;
                    }
                    si (!aliquid || coordinata > maxima[i])
                    {
                        maxima[i] = coordinata;
                    }
                }
                aliquid = VERUM;
            }
        }
    }
    si (!aliquid)
    {
        si (statua->radius > 0.000001f)
        {
            /* sine capsa: sphaera ambiens */
            f.scala     = MUNDUS_CORPORIS_ALTITUDO
                / (2.0f * statua->radius);
            f.imum      = statua->centrum.y - statua->radius;
            f.medium_x  = statua->centrum.x;
            f.medium_z  = statua->centrum.z;
        }
        redde f;
    }
    altitudo = maxima[I] - minima[I];
    latitudo = maxima[ZEPHYRUM] - minima[ZEPHYRUM];
    si (maxima[II] - minima[II] > latitudo)
    {
        latitudo = maxima[II] - minima[II];
    }
    si (altitudo > 0.000001f)
    {
        f.scala = MUNDUS_CORPORIS_ALTITUDO / altitudo;
    }
    si (latitudo * f.scala > MUNDUS_CORPORIS_LATITUDO_MAXIMA)
    {
        f.scala = MUNDUS_CORPORIS_LATITUDO_MAXIMA / latitudo;
    }
    f.imum      = minima[I];
    f.medium_x  = (minima[ZEPHYRUM] + maxima[ZEPHYRUM]) * 0.5f;
    f.medium_z  = (minima[II] + maxima[II]) * 0.5f;
    redde f;
}

vacuum
mundus_pixelum_puncti (
    StereometriaVector  punctum,
                   s32* x,
                   s32* y)
{
    StatuaScaenaVisus visus        = mundus_visus(ZEPHYRUM, ZEPHYRUM);
                  f32 fractum_x    = 0.0f;
                  f32 fractum_y    = 0.0f;
                  f32 profunditas  = 0.0f;

    statua_scaena_proicere(&visus, punctum, &fractum_x, &fractum_y,
        &profunditas);
    *x = (s32)floor((f64)fractum_x);
    *y = (s32)floor((f64)fractum_y);
}

/* medium corporis in mundo (dimidia altitudo supra pedes) */
interior StereometriaVector
_medium_corporis (
    constans MundusCorpus* c)
{
    redde stereometria_vector(c->x,
        c->y + MUNDUS_CORPORIS_ALTITUDO * 0.5f, c->z);
}

b32
mundus_quadri_origo (
    constans Mundus* mundus,
                i32  sequens,
                i32  latitudo,
                i32  altitudo,
                s32* origo_x,
                s32* origo_y)
{
    s32 x;
    s32 y;

    si (   !mundus || !mundus->corpora || !origo_x || !origo_y
        || sequens >= mundus->corporum
        || latitudo > 0x3FFFFFFF || altitudo > 0x3FFFFFFF)
    {
        redde FALSUM;
    }
    mundus_pixelum_puncti(_medium_corporis(&mundus->corpora[sequens]),
        &x, &y);
    *origo_x = x - (s32)(latitudo / II);
    *origo_y = y - (s32)(altitudo / II);
    redde VERUM;
}

/* ad multiplum reticuli deorsum (signatum) */
interior s32
_ad_reticulum (
    s32 valor)
{
    s32 gradus = (s32)STATUAE_FINIUM_GRADUS;

    redde valor >= ZEPHYRUM ? valor / gradus * gradus
        : -((-valor + gradus - I) / gradus * gradus);
}

/* corpus pictum in capsa sua (gradus primus quadri) */
nomen structura {
    constans i8* rgba;     /* NIHIL = corpus non in visu */
            s32  x0;       /* capsa in pixelis mundi */
            s32  y0;
            i32  latitudo;
            i32  altitudo;
} CorpusPictum;

/* gradus primus: corpus in capsam suam pingere; ubi nihil propius
 * est, profunditatem quadri et DOMINUM pixeli (signum) scribere */
interior b32
_corpus_pingere (
    constans MundusCorpus* c,
     constans MundusForma* forma,
                      i32  signum,
                      s32  origo_x,
                      s32  origo_y,
                      i32  latitudo,
                      i32  altitudo,
                      i32* domini,
                      f32* profunditas,
             CorpusPictum* pictum,
                  Piscina* piscina)
{
       constans Statua* statua = forma->statua;
          StatuaLocata  locata;
     StatuaScaenaVisus  visus;
   StatuaScaenaPictura  pictura;
    StereometriaMatrix* matrices;
 StereometriaQuaternio  rotatio;
        memoriae_index  k;
        memoriae_index  locus;
                   f32  dimidium;
                   s32  medium_x;
                   s32  medium_y;
                   s32  radius;
                   s32  x0;
                   s32  y0;
                   s32  x1;
                   s32  y1;
                   s32  x;
                   s32  y;
                   i32  lat;
                   i32  alt;

    /* capsa: sphaera ambiens scalata circa medium corporis, ad
     * reticulum pixelorum MUNDI (dithering corporis cum visu labente
     * non natat), intra quadrum */
    mundus_pixelum_puncti(_medium_corporis(c), &medium_x, &medium_y);
    radius = (s32)ceil((f64)(statua->radius * forma->scala
        * MUNDUS_PIXELA_PER_UNITATEM)) + (s32)STATUAE_FINIUM_MARGO;
    si (radius > (s32)MUNDUS_CAPSA_MAXIMA / II)
    {
        radius = (s32)MUNDUS_CAPSA_MAXIMA / II;
    }
    x0 = _ad_reticulum(medium_x - radius);
    y0 = _ad_reticulum(medium_y - radius);
    x1 = _ad_reticulum(medium_x + radius + (s32)STATUAE_FINIUM_GRADUS);
    y1 = _ad_reticulum(medium_y + radius + (s32)STATUAE_FINIUM_GRADUS);
    si (x0 < origo_x)
    {
        x0 = origo_x;
    }
    si (y0 < origo_y)
    {
        y0 = origo_y;
    }
    si (x1 > origo_x + (s32)latitudo)
    {
        x1 = origo_x + (s32)latitudo;
    }
    si (y1 > origo_y + (s32)altitudo)
    {
        y1 = origo_y + (s32)altitudo;
    }
    si (x1 <= x0 || y1 <= y0)
    {
        redde VERUM;
    }
    lat = (i32)(x1 - x0);
    alt = (i32)(y1 - y0);
    matrices = (StereometriaMatrix*)piscina_allocare(piscina,
        ((memoriae_index)statua->nodorum + I)
            * magnitudo(StereometriaMatrix));
    si (!matrices)
    {
        redde FALSUM;
    }
    statua_positura(statua,
        c->movetur ? forma->cyclus_ambulandi : forma->cyclus_quietis,
        c->movetur ? c->tempus_motus : 0.0f, matrices);
    /* locatio: pedes et medium statuae ad locum corporis, scalata,
     * facie versa (0 = +z, crescit versus +x: rotatio circa y) */
    dimidium         = c->facies * STEREOMETRIA_PI / 360.0f;
    rotatio.x        = 0.0f;
    rotatio.y        = (f32)sin((f64)dimidium);
    rotatio.z        = 0.0f;
    rotatio.w        = (f32)cos((f64)dimidium);
    locata.statua    = statua;
    locata.matrices  = matrices;
    locata.locatio   = stereometria_multiplicare(
        stereometria_ex_trs(stereometria_vector(c->x, c->y, c->z),
            rotatio,
            stereometria_vector(forma->scala, forma->scala,
                forma->scala)),
        stereometria_translatio(stereometria_vector(-forma->medium_x,
            -forma->imum, -forma->medium_z)));
    visus = mundus_visus(x0, y0);
    si (!statua_scaenam_pingere(&locata, I, &visus, lat, alt, &pictura,
            piscina))
    {
        redde FALSUM;
    }
    per (y = ZEPHYRUM; y < (s32)alt; y++)
    {
        per (x = ZEPHYRUM; x < (s32)lat; x++)
        {
            k      = (memoriae_index)y * lat + (memoriae_index)x;
            locus  = (memoriae_index)(y0 - origo_y + y) * latitudo
                + (memoriae_index)(x0 - origo_x + x);
            si (   pictura.tectum[k]
                && pictura.profunditas[k] < profunditas[locus])
            {
                profunditas[locus]  = pictura.profunditas[k];
                domini[locus]       = signum;
            }
        }
    }
    pictum->rgba      = pictura.rgba;
    pictum->x0        = x0;
    pictum->y0        = y0;
    pictum->latitudo  = lat;
    pictum->altitudo  = alt;
    redde VERUM;
}

/* gradus alter: pixela quorum corpus dominus est (post OMNIA corpora
 * picta - ordo corporum nihil mutat) exemplariis redigere et in
 * quadrum ponere */
interior b32
_corpus_redigere (
    constans CorpusPictum* pictum,
                      i32  signum,
             constans b32* activi,
                      s32  origo_x,
                      s32  origo_y,
                      i32  latitudo,
             constans i32* domini,
                       i8* indices,
                  Piscina* piscina)
{
    memoriae_index pixela = (memoriae_index)pictum->latitudo
        * pictum->altitudo;
    memoriae_index  k;
    memoriae_index  locus;
                i8* larva;
                i8* redacta;
               s32  x;
               s32  y;
               b32  aliquid = FALSUM;

    larva    = (i8*)piscina_allocare(piscina, pixela);
    redacta  = (i8*)piscina_allocare(piscina, pixela);
    si (!larva || !redacta)
    {
        redde FALSUM;
    }
    per (y = ZEPHYRUM; y < (s32)pictum->altitudo; y++)
    {
        per (x = ZEPHYRUM; x < (s32)pictum->latitudo; x++)
        {
            k      = (memoriae_index)y * pictum->latitudo
                + (memoriae_index)x;
            locus  = (memoriae_index)(pictum->y0 - origo_y + y)
                * latitudo
                + (memoriae_index)(pictum->x0 - origo_x + x);
            larva[k]  = (i8)(domini[locus] == signum);
            aliquid   = aliquid || larva[k];
        }
    }
    si (!aliquid)
    {
        redde VERUM;
    }
    si (!statuae_picturam_dithering(pictum->rgba, larva,
            pictum->latitudo, pictum->altitudo, activi, redacta,
            piscina))
    {
        redde FALSUM;
    }
    per (y = ZEPHYRUM; y < (s32)pictum->altitudo; y++)
    {
        per (x = ZEPHYRUM; x < (s32)pictum->latitudo; x++)
        {
            k = (memoriae_index)y * pictum->latitudo
                + (memoriae_index)x;
            si (larva[k])
            {
                locus = (memoriae_index)(pictum->y0 - origo_y + y)
                    * latitudo
                    + (memoriae_index)(pictum->x0 - origo_x + x);
                indices[locus] = redacta[k];
            }
        }
    }
    redde VERUM;
}

b32
mundus_quadrum_pingere (
             MundusHumus* humus,
         constans Mundus* mundus,
    constans MundusForma* formae,
                     i32  formarum,
                     i32  sequens,
            constans b32* activi,
                      i8  fundus,
                     i32  latitudo,
                     i32  altitudo,
                      i8* indices,
                 Piscina* piscina)
{
    constans MundusCorpus* c;
             CorpusPictum* picta;
                      f32* profunditas;
                      i32* domini;
           memoriae_index  pixela;
                      s32  origo_x;
                      s32  origo_y;
                      i32  k;

    si (   !humus || !mundus || !activi || !indices || !piscina
        || (formarum && !formae)
        || !mundus_quadri_origo(mundus, sequens, latitudo, altitudo,
               &origo_x, &origo_y))
    {
        redde FALSUM;
    }
    si (latitudo == ZEPHYRUM || altitudo == ZEPHYRUM)
    {
        redde VERUM;
    }
    pixela       = (memoriae_index)latitudo * altitudo;
    profunditas  = (f32*)piscina_allocare(piscina,
        pixela * magnitudo(f32));
    domini       = (i32*)piscina_allocare(piscina,
        pixela * magnitudo(i32));
    picta        = (CorpusPictum*)piscina_allocare(piscina,
        ((memoriae_index)mundus->corporum + I)
            * magnitudo(CorpusPictum));
    si (   !profunditas || !domini || !picta
        || !mundus_humus_copiare(humus, origo_x, origo_y, latitudo,
               altitudo, fundus, indices, profunditas))
    {
        redde FALSUM;
    }
    /* dominus 0 = humus; k + I = corpus k */
    memset(domini, ZEPHYRUM, pixela * magnitudo(i32));
    memset(picta, ZEPHYRUM, ((memoriae_index)mundus->corporum + I)
        * magnitudo(CorpusPictum));
    per (k = ZEPHYRUM; k < mundus->corporum; k++)
    {
        c = &mundus->corpora[k];
        si (   c->forma < ZEPHYRUM || (i32)c->forma >= formarum
            || !formae[c->forma].statua)
        {
            perge;
        }
        si (!_corpus_pingere(c, &formae[c->forma], k + I, origo_x,
                origo_y, latitudo, altitudo, domini, profunditas,
                &picta[k], piscina))
        {
            redde FALSUM;
        }
    }
    per (k = ZEPHYRUM; k < mundus->corporum; k++)
    {
        si (   picta[k].rgba
            && !_corpus_redigere(&picta[k], k + I, activi, origo_x,
                   origo_y, latitudo, domini, indices, piscina))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}
