/* mundus_solum.c - tabula gressus loci (vide mundus_solum.h)
 *
 * Structio: matrices quietis -> fines verticum nodorum signatorum ->
 * craticula -> quodque triangulum desuper in cellas (centrum cellae
 * intra triangulum in plano x z; altitudo per coordinatas
 * barycentricas) -> summum vincit.
 */

#include "mundus_solum.h"
#include "stereometria.h"
#include <math.h>
#include <string.h>

/* altitudines intra hoc pares habentur (aqua in solo iacens) */
#define PARITAS_ALTITUDINIS 0.0001f
#define AREA_MINIMA         0.000001f

nomen structura {
    f32 x;
    f32 y;
    f32 z;
} PunctumSoli;

interior MundusSoliGenus
_genus_signorum (
    i32 signa)
{
    si (signa & STATUA_SIGNUM_GRESSIBILE)
    {
        redde MUNDUS_SOLUM_GRESSIBILE;
    }
    si (signa & STATUA_SIGNUM_AQUA)
    {
        redde MUNDUS_SOLUM_AQUA;
    }
    si (signa & STATUA_SIGNUM_SOLIDUM)
    {
        redde MUNDUS_SOLUM_SOLIDUM;
    }
    redde MUNDUS_SOLUM_NIHIL;
}

interior PunctumSoli
_vertex (
       constans Statua* s,
    StereometriaMatrix  m,
                   i32  vertex)
{
        StereometriaVector v;
               PunctumSoli p;

    v = stereometria_punctum_transformare(m,
        stereometria_vector(s->positiones[vertex * III],
            s->positiones[vertex * III + I],
            s->positiones[vertex * III + II]), NIHIL);
    p.x = v.x;
    p.y = v.y;
    p.z = v.z;
    redde p;
}

/* cella in qua coordinata iacet; negativa si ante originem */
interior s32
_cella (
    f32 coordinata,
    f32 origo)
{
    redde (s32)floor((f64)((coordinata - origo)
        * (f32)MUNDUS_CELLAE_PER_UNITATEM));
}

/* centrum cellae in mundo */
interior f32
_centrum (
    s32 cella,
    f32 origo)
{
    redde origo + ((f32)cella + 0.5f) / (f32)MUNDUS_CELLAE_PER_UNITATEM;
}

interior b32
_intra (
    constans MundusSolum* s,
                     s32  cella_x,
                     s32  cella_z)
{
    redde cella_x >= ZEPHYRUM && cella_z >= ZEPHYRUM
        && cella_x < (s32)s->latitudo
        && cella_z < (s32)s->altitudo;
}

interior memoriae_index
_index (
    constans MundusSolum* s,
                     s32  cella_x,
                     s32  cella_z)
{
    redde (memoriae_index)cella_z * s->latitudo
        + (memoriae_index)cella_x;
}

interior b32
_gressibilis (
    constans MundusSolum* s,
                     s32  cella_x,
                     s32  cella_z)
{
    redde _intra(s, cella_x, cella_z)
        && s->genera[_index(s, cella_x, cella_z)]
            == MUNDUS_SOLUM_GRESSIBILE;
}

/* triangulum desuper in cellas */
interior vacuum
_triangulum (
            MundusSolum* s,
        MundusSoliGenus  genus,
            PunctumSoli  a,
            PunctumSoli  b,
            PunctumSoli  d)
{
    memoriae_index locus;
               f32 area;
               f32 punctum_x;
               f32 punctum_z;
               f32 l0;
               f32 l1;
               f32 l2;
               f32 y;
               f32 minimum;
               f32 maximum;
               s32 x_min;
               s32 x_max;
               s32 z_min;
               s32 z_max;
               s32 cella_x;
               s32 cella_z;

    area = (b.z - d.z) * (a.x - d.x) + (d.x - b.x) * (a.z - d.z);
    si (area < AREA_MINIMA && area > -AREA_MINIMA)
    {
        redde;
    }
    minimum  = a.x < b.x ? a.x : b.x;
    minimum  = minimum < d.x ? minimum : d.x;
    maximum  = a.x > b.x ? a.x : b.x;
    maximum  = maximum > d.x ? maximum : d.x;
    x_min    = _cella(minimum, s->origo_x);
    x_max    = _cella(maximum, s->origo_x);
    minimum  = a.z < b.z ? a.z : b.z;
    minimum  = minimum < d.z ? minimum : d.z;
    maximum  = a.z > b.z ? a.z : b.z;
    maximum  = maximum > d.z ? maximum : d.z;
    z_min    = _cella(minimum, s->origo_z);
    z_max    = _cella(maximum, s->origo_z);
    si (x_min < ZEPHYRUM)
    {
        x_min = ZEPHYRUM;
    }
    si (z_min < ZEPHYRUM)
    {
        z_min = ZEPHYRUM;
    }
    si (x_max > (s32)s->latitudo - I)
    {
        x_max = (s32)s->latitudo - I;
    }
    si (z_max > (s32)s->altitudo - I)
    {
        z_max = (s32)s->altitudo - I;
    }
    per (cella_z = z_min; cella_z <= z_max; cella_z++)
    {
        per (cella_x = x_min; cella_x <= x_max; cella_x++)
        {
            punctum_x = _centrum(cella_x, s->origo_x);
            punctum_z = _centrum(cella_z, s->origo_z);
            l0  = ((b.z - d.z) * (punctum_x - d.x) + (d.x
                - b.x) * (punctum_z - d.z))
                / area;
            l1  = ((d.z - a.z) * (punctum_x - d.x) + (a.x
                - d.x) * (punctum_z - d.z))
                / area;
            l2  = 1.0f - l0 - l1;
            si (l0 < 0.0f || l1 < 0.0f || l2 < 0.0f)
            {
                perge;
            }
            y      = l0 * a.y + l1 * b.y + l2 * d.y;
            locus  = _index(s, cella_x, cella_z);
            si (   s->genera[locus] == MUNDUS_SOLUM_NIHIL
                || y > s->altitudines[locus] + PARITAS_ALTITUDINIS
                || (   y >= s->altitudines[locus] - PARITAS_ALTITUDINIS
                    && genus != MUNDUS_SOLUM_GRESSIBILE
                    && s->genera[locus] == MUNDUS_SOLUM_GRESSIBILE))
            {
                s->genera[locus]       = (i8)genus;
                s->altitudines[locus]  = y;
            }
        }
    }
}

/* nodos signatos percurrere: gradus 0 fines colligit, gradus I
 * triangula in tabulam mittit */
interior b32
_percurrere (
                constans Statua* locus,
    constans StereometriaMatrix* matrices,
                    MundusSolum* s,
                            f32* fines)
{
        constans StatuaRete* rete;
        constans StatuaPars* pars;
            MundusSoliGenus  genus;
                PunctumSoli  v[III];
                        b32  aliquid = FALSUM;
                        i32  n;
                        i32  j;
                        i32  k;
                        i32  i;

    per (n = ZEPHYRUM; n < locus->nodorum; n++)
    {
        genus = _genus_signorum(locus->nodi[n].signa);
        si (   genus == MUNDUS_SOLUM_NIHIL
            || locus->nodi[n].rete < ZEPHYRUM)
        {
            perge;
        }
        rete = &locus->retia[locus->nodi[n].rete];
        per (j = ZEPHYRUM; j < rete->partium; j++)
        {
            pars = &locus->partes[rete->pars_prima + j];
            per (k = ZEPHYRUM; k + II < pars->indicum; k += III)
            {
                per (i = ZEPHYRUM; i < III; i++)
                {
                    v[i] = _vertex(locus, matrices[n],
                        locus->indices[pars->index_primus + k + i]);
                    si (fines)
                    {
                        si (!aliquid || v[i].x < fines[ZEPHYRUM])
                        {
                            fines[ZEPHYRUM] = v[i].x;
                        }
                        si (!aliquid || v[i].x > fines[I])
                        {
                            fines[I] = v[i].x;
                        }
                        si (!aliquid || v[i].z < fines[II])
                        {
                            fines[II] = v[i].z;
                        }
                        si (!aliquid || v[i].z > fines[III])
                        {
                            fines[III] = v[i].z;
                        }
                        aliquid = VERUM;
                    }
                }
                si (!fines)
                {
                    _triangulum(s, genus, v[ZEPHYRUM], v[I], v[II]);
                    aliquid = VERUM;
                }
            }
        }
    }
    redde aliquid;
}

MundusSolum*
mundus_solum_struere (
    constans Statua* locus,
            Piscina* piscina)
{
    StereometriaMatrix* matrices;
           MundusSolum* s;
        memoriae_index  cellae;
        memoriae_index  k;
                   f32  fines[IV];
                   f64  latitudo;
                   f64  altitudo;

    si (!locus || !piscina)
    {
        redde NIHIL;
    }
    s = (MundusSolum*)piscina_allocare(piscina, magnitudo(MundusSolum));
    si (!s)
    {
        redde NIHIL;
    }
    memset(s, ZEPHYRUM, magnitudo(MundusSolum));
    matrices = (StereometriaMatrix*)piscina_allocare(piscina,
        ((memoriae_index)locus->nodorum + I)
            * magnitudo(StereometriaMatrix));
    si (!matrices)
    {
        redde NIHIL;
    }
    statua_positura(locus, -I, 0.0f, matrices);
    memset(fines, ZEPHYRUM, magnitudo(fines));
    si (!_percurrere(locus, matrices, NIHIL, fines))
    {
        redde s;
    }
    s->origo_x = (f32)floor((f64)fines[ZEPHYRUM]);
    s->origo_z = (f32)floor((f64)fines[II]);
    latitudo    = ceil((f64)(fines[I] - s->origo_x)
        * MUNDUS_CELLAE_PER_UNITATEM);
    altitudo    = ceil((f64)(fines[III] - s->origo_z)
        * MUNDUS_CELLAE_PER_UNITATEM);
    si (   !(latitudo <= (f64)MUNDUS_SOLI_LATUS_MAXIMUM)
        || !(altitudo <= (f64)MUNDUS_SOLI_LATUS_MAXIMUM))
    {
        redde NIHIL;
    }
    s->latitudo  = latitudo < 1.0 ? I : (i32)latitudo;
    s->altitudo  = altitudo < 1.0 ? I : (i32)altitudo;
    cellae       = (memoriae_index)s->latitudo * s->altitudo;
    s->genera    = (i8*)piscina_allocare(piscina, cellae);
    s->altitudines = (f32*)piscina_allocare(piscina,
        cellae * magnitudo(f32));
    si (!s->genera || !s->altitudines)
    {
        redde NIHIL;
    }
    memset(s->genera, MUNDUS_SOLUM_NIHIL, cellae);
    per (k = ZEPHYRUM; k < cellae; k++)
    {
        s->altitudines[k] = 0.0f;
    }
    (vacuum)_percurrere(locus, matrices, s, NIHIL);
    redde s;
}

MundusSoliGenus
mundus_solum_genus (
    constans MundusSolum* s,
                     f32  x,
                     f32  z)
{
    s32 cella_x;
    s32 cella_z;

    si (!s || !s->genera)
    {
        redde MUNDUS_SOLUM_NIHIL;
    }
    cella_x = _cella(x, s->origo_x);
    cella_z = _cella(z, s->origo_z);
    si (!_intra(s, cella_x, cella_z))
    {
        redde MUNDUS_SOLUM_NIHIL;
    }
    redde (MundusSoliGenus)s->genera[_index(s, cella_x, cella_z)];
}

/* altitudo cellae vicinae si gressibilis et gradu ab 'basis'
 * attingitur; aliter basis ipsa */
interior f32
_altitudo_iuncta (
    constans MundusSolum* s,
                     s32  cella_x,
                     s32  cella_z,
                     f32  basis)
{
    f32 a;
    f32 differentia;

    si (!_gressibilis(s, cella_x, cella_z))
    {
        redde basis;
    }
    a            = s->altitudines[_index(s, cella_x, cella_z)];
    differentia  = a > basis ? a - basis : basis - a;
    redde differentia <= MUNDUS_ASCENSUS_MAXIMUS ? a : basis;
}

b32
mundus_solum_altitudo (
    constans MundusSolum* s,
                     f32  x,
                     f32  z,
                     f32* y)
{
    f32 basis;
    f32 reticuli_x;
    f32 reticuli_z;
    f32 pars_x;
    f32 pars_z;
    f32 a00;
    f32 a10;
    f32 a01;
    f32 a11;
    s32 cella_x;
    s32 cella_z;
    s32 x0;
    s32 z0;

    si (!s || !s->genera || !y)
    {
        redde FALSUM;
    }
    cella_x = _cella(x, s->origo_x);
    cella_z = _cella(z, s->origo_z);
    si (!_gressibilis(s, cella_x, cella_z))
    {
        redde FALSUM;
    }
    basis = s->altitudines[_index(s, cella_x, cella_z)];
    /* coordinatae in craticula centrorum */
    reticuli_x  = (x - s->origo_x) * (f32)MUNDUS_CELLAE_PER_UNITATEM
        - 0.5f;
    reticuli_z  = (z - s->origo_z) * (f32)MUNDUS_CELLAE_PER_UNITATEM
        - 0.5f;
    x0      = (s32)floor((f64)reticuli_x);
    z0      = (s32)floor((f64)reticuli_z);
    pars_x  = reticuli_x - (f32)x0;
    pars_z  = reticuli_z - (f32)z0;
    a00     = _altitudo_iuncta(s, x0, z0, basis);
    a10     = _altitudo_iuncta(s, x0 + I, z0, basis);
    a01     = _altitudo_iuncta(s, x0, z0 + I, basis);
    a11     = _altitudo_iuncta(s, x0 + I, z0 + I, basis);
    *y = (a00 * (1.0f - pars_x) + a10 * pars_x) * (1.0f - pars_z)
        + (a01 * (1.0f - pars_x) + a11 * pars_x) * pars_z;
    redde VERUM;
}

b32
mundus_solum_stare (
    constans MundusSolum* s,
                     f32  x,
                     f32  z,
                     f32  radius)
{
    f32 basis;
    f32 delta_x;
    f32 delta_z;
    f32 distantia;
    f32 differentia;
    s32 cella_x;
    s32 cella_z;
    s32 ambitus;
    s32 i;
    s32 j;

    si (!s || !s->genera || !(radius >= 0.0f))
    {
        redde FALSUM;
    }
    cella_x = _cella(x, s->origo_x);
    cella_z = _cella(z, s->origo_z);
    si (!_gressibilis(s, cella_x, cella_z))
    {
        redde FALSUM;
    }
    basis    = s->altitudines[_index(s, cella_x, cella_z)];
    ambitus  = (s32)ceil((f64)(radius
        * (f32)MUNDUS_CELLAE_PER_UNITATEM)) + I;
    per (j = cella_z - ambitus; j <= cella_z + ambitus; j++)
    {
        per (i = cella_x - ambitus; i <= cella_x + ambitus; i++)
        {
            delta_x = _centrum(i, s->origo_x) - x;
            delta_z = _centrum(j, s->origo_z) - z;
            distantia = (f32)sqrt((f64)(delta_x * delta_x
                + delta_z * delta_z));
            si (distantia > radius)
            {
                perge;
            }
            si (!_gressibilis(s, i, j))
            {
                redde FALSUM;
            }
            differentia = s->altitudines[_index(s, i, j)] - basis;
            si (differentia < 0.0f)
            {
                differentia = -differentia;
            }
            si (differentia > MUNDUS_ASCENSUS_MAXIMUS
                    + MUNDUS_CLIVUS_MAXIMUS * distantia)
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

b32
mundus_solum_licet (
    constans MundusSolum* s,
                     f32  x0,
                     f32  z0,
                     f32  x1,
                     f32  z1,
                     f32  radius)
{
    f32 differentia;
    f32 distantia;
    s32 cella_x;
    s32 cella_z;
    s32 delta_x;
    s32 delta_z;

    si (!s || !s->genera)
    {
        redde FALSUM;
    }
    cella_x = _cella(x0, s->origo_x);
    cella_z = _cella(z0, s->origo_z);
    delta_x = _cella(x1, s->origo_x);
    delta_z = _cella(z1, s->origo_z);
    si (   !_gressibilis(s, cella_x, cella_z)
        || !mundus_solum_stare(s, x1, z1, radius))
    {
        redde FALSUM;
    }
    differentia = s->altitudines[_index(s, delta_x, delta_z)]
        - s->altitudines[_index(s, cella_x, cella_z)];
    si (differentia < 0.0f)
    {
        differentia = -differentia;
    }
    distantia = (f32)sqrt((f64)((x1 - x0) * (x1 - x0)
        + (z1 - z0) * (z1 - z0)));
    redde differentia <= MUNDUS_ASCENSUS_MAXIMUS
        + MUNDUS_CLIVUS_MAXIMUS * distantia;
}

b32
mundus_solum_initium (
    constans MundusSolum* s,
                     f32  radius,
                     f32* x,
                     f32* z)
{
    f32 medium_x;
    f32 medium_z;
    f32 punctum_x;
    f32 punctum_z;
    f32 distantia;
    f32 optima    = 0.0f;
    b32 inventum  = FALSUM;
    s32 cella_x;
    s32 cella_z;

    si (!s || !s->genera || !x || !z)
    {
        redde FALSUM;
    }
    medium_x  = s->origo_x + (f32)s->latitudo * 0.5f
        / (f32)MUNDUS_CELLAE_PER_UNITATEM;
    medium_z  = s->origo_z + (f32)s->altitudo * 0.5f
        / (f32)MUNDUS_CELLAE_PER_UNITATEM;
    per (cella_z = ZEPHYRUM; cella_z < (s32)s->altitudo; cella_z++)
    {
        per (cella_x = ZEPHYRUM; cella_x < (s32)s->latitudo; cella_x++)
        {
            punctum_x = _centrum(cella_x, s->origo_x);
            punctum_z = _centrum(cella_z, s->origo_z);
            distantia = (punctum_x - medium_x) * (punctum_x - medium_x)
                + (punctum_z - medium_z) * (punctum_z - medium_z);
            si (   (!inventum || distantia < optima)
                && mundus_solum_stare(s, punctum_x, punctum_z, radius))
            {
                inventum  = VERUM;
                optima    = distantia;
                *x        = punctum_x;
                *z        = punctum_z;
            }
        }
    }
    redde inventum;
}
