/* mundus_pictura.c - humus mundi (vide mundus_pictura.h)
 *
 * Regio (i, j) pixela mundi [i * R, (i + 1) * R) x [j * R, (j + 1) *
 * R) tegit. Pingitur cum MARGINE circum (Atkinson errorem dextrorsum
 * et deorsum fundit: sine margine ora sinistra et summa regionis
 * errorem vicinae non acciperet et sutura appareret); margo abicitur.
 */

#include "mundus_pictura.h"
#include "dithering.h"
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
       MundusHumus* h;
    memoriae_index  mensura;
               f32  fines[IV];
               s32  i_max;
               s32  j_max;

    si (!locus || !activi || !piscina)
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
    fructus = dithering_atkinson_colorum(pictura.rgba, latus, latus,
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
