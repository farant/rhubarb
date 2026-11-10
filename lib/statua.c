/* statua.c - statua et positura eius (vide statua.h) */

#include "statua.h"
#include <math.h>
#include <string.h>


/* ==================================================
 * Claves
 * ================================================== */

/* valor canalis ad tempus (iam involutum): ante primam prima, post
 * ultimam ultima; inter claves mixtio aut (gradatim) clavis prior.
 * exitus: III valores, rotatio IV */
interior vacuum
_canalem_sumere (
    constans StatuaCanalis* k,
                       f32  tempus,
                       f32* exitus)
{
    StereometriaQuaternio a;
    StereometriaQuaternio b;
                      i32 latus;
                      i32 prior;
                      i32 n;
                      f32 spatium;
                      f32 t;

    latus = k->proprietas == STATUA_ROTATIO ? IV : III;
    prior = ZEPHYRUM;
    dum (prior + I < k->clavium && k->tempora[prior + I] <= tempus)
    {
        prior++;
    }
    si (   prior + I >= k->clavium || k->gradatim
        || tempus    <= k->tempora[prior])
    {
        per (n = ZEPHYRUM; n < latus; n++)
        {
            exitus[n] = k->valores[prior * latus + n];
        }
        redde;
    }
    spatium  = k->tempora[prior + I] - k->tempora[prior];
    t        = spatium > 0.0f ? (tempus - k->tempora[prior]) / spatium
                              : 0.0f;
    si (k->proprietas != STATUA_ROTATIO)
    {
        per (n = ZEPHYRUM; n < III; n++)
        {
            exitus[n] = k->valores[prior * III + n]
                + (k->valores[(prior + I) * III + n]
                    - k->valores[prior * III + n]) * t;
        }
        redde;
    }
    a.x               = k->valores[prior * IV];
    a.y               = k->valores[prior * IV + I];
    a.z               = k->valores[prior * IV + II];
    a.w               = k->valores[prior * IV + III];
    b.x               = k->valores[(prior + I) * IV];
    b.y               = k->valores[(prior + I) * IV + I];
    b.z               = k->valores[(prior + I) * IV + II];
    b.w               = k->valores[(prior + I) * IV + III];
    a                 = stereometria_quaternio_slerp(a, b, t);
    exitus[ZEPHYRUM]  = a.x;
    exitus[I]         = a.y;
    exitus[II]        = a.z;
    exitus[III]       = a.w;
}

/* matrix localis nodi: quies, canalibus cycli superscripta */
interior StereometriaMatrix
_matrix_localis (
           constans Statua* s,
     constans StatuaCyclus* cyclus,
                       i32  nodus,
                       f32  tempus)
{
       StereometriaVector translatio  = s->nodi[nodus].translatio;
    StereometriaQuaternio rotatio     = s->nodi[nodus].rotatio;
       StereometriaVector scala       = s->nodi[nodus].scala;
                      f32 v[IV];
                      i32 n;

    per (n = ZEPHYRUM; cyclus && n < cyclus->canalium; n++)
    {
        si (cyclus->canales[n].nodus != (s32)nodus)
        {
            perge;
        }
        _canalem_sumere(&cyclus->canales[n], tempus, v);
        commutatio (cyclus->canales[n].proprietas)
        {
            casus STATUA_TRANSLATIO:
                translatio = stereometria_vector(v[ZEPHYRUM], v[I],
                    v[II]);
                frange;
            casus STATUA_SCALA:
                scala = stereometria_vector(v[ZEPHYRUM], v[I], v[II]);
                frange;
            ordinarius:
                rotatio.x  = v[ZEPHYRUM];
                rotatio.y  = v[I];
                rotatio.z  = v[II];
                rotatio.w  = v[III];
                rotatio    = stereometria_quaternio_normalizare(
                    rotatio);
                frange;
        }
    }
    redde stereometria_ex_trs(translatio, rotatio, scala);
}

/* positura ad tempus iam involutum; cyclus NIHIL = quies */
interior vacuum
_positura (
           constans Statua* s,
     constans StatuaCyclus* cyclus,
                       f32  tempus,
        StereometriaMatrix* matrices)
{
    StereometriaMatrix localis;
                   i32 k;
                   i32 nodus;

    per (k = ZEPHYRUM; k < s->nodorum; k++)
    {
        nodus    = s->ordo[k];
        localis  = _matrix_localis(s, cyclus, nodus, tempus);
        si (s->nodi[nodus].parens < ZEPHYRUM)
        {
            matrices[nodus] = localis;
        }
        alioquin
        {
            matrices[nodus] = stereometria_multiplicare(
                matrices[s->nodi[nodus].parens], localis);
        }
    }
}

vacuum
statua_positura (
       constans Statua* statua,
                   s32  cyclus,
                   f32  tempus,
    StereometriaMatrix* matrices)
{
    constans StatuaCyclus* c = NIHIL;

    si (!statua || !matrices)
    {
        redde;
    }
    si (cyclus >= ZEPHYRUM && cyclus < (s32)statua->cyclorum)
    {
        c = &statua->cycli[cyclus];
        si (c->duratio > 0.0f)
        {
            tempus = tempus - (f32)floor((f64)(tempus / c->duratio))
                * c->duratio;
            /* rotundatio: numquam ad durationem ipsam */
            si (tempus >= c->duratio || tempus < 0.0f)
            {
                tempus = 0.0f;
            }
        }
        alioquin
        {
            tempus = 0.0f;
        }
    }
    _positura(statua, c, tempus, matrices);
}


/* ==================================================
 * Finire: probatio, ordo, sphaera
 * ================================================== */

/* [primus, primus + numerus) intra [0, summa)? (sine superfluxu) */
interior b32
_intra (
    i32 primus,
    i32 numerus,
    i32 summa)
{
    redde primus <= summa && numerus <= summa - primus;
}

interior b32
_canalis_sanus (
    constans StatuaCanalis* k,
                       i32  nodorum)
{
    i32 n;

    si (   k->nodus < ZEPHYRUM || k->nodus >= (s32)nodorum
        || k->clavium < I || !k->tempora || !k->valores
        || (   k->proprietas != STATUA_TRANSLATIO
            && k->proprietas != STATUA_ROTATIO
            && k->proprietas != STATUA_SCALA))
    {
        redde FALSUM;
    }
    per (n = ZEPHYRUM; n < k->clavium; n++)
    {
        /* comparatio negata NaN quoque recusat */
        si (!(k->tempora[n] >= 0.0f))
        {
            redde FALSUM;
        }
        si (n > ZEPHYRUM && !(k->tempora[n] >= k->tempora[n - I]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_sana (
    constans Statua* s)
{
    i32 n;
    i32 k;

    si (   (s->nodorum && !s->nodi) || (s->retium && !s->retia)
        || (s->partium && !s->partes) || (s->indicum && !s->indices)
        || (s->materiarum && !s->materiae)
        || (s->texturarum && !s->texturae)
        || (s->cyclorum && !s->cycli)
        || (s->verticum
            && (!s->positiones || !s->normales || !s->coordinatae)))
    {
        redde FALSUM;
    }
    per (n = ZEPHYRUM; n < s->nodorum; n++)
    {
        si (   s->nodi[n].parens < -I
            || s->nodi[n].parens >= (s32)s->nodorum
            || s->nodi[n].rete < -I
            || s->nodi[n].rete   >= (s32)s->retium)
        {
            redde FALSUM;
        }
    }
    per (n = ZEPHYRUM; n < s->retium; n++)
    {
        si (!_intra(s->retia[n].pars_prima, s->retia[n].partium,
            s->partium))
        {
            redde FALSUM;
        }
    }
    per (n = ZEPHYRUM; n < s->partium; n++)
    {
        si (   !_intra(s->partes[n].index_primus, s->partes[n].indicum,
                   s->indicum)
            || s->partes[n].indicum % III != ZEPHYRUM
            || s->partes[n].materia < -I
            || s->partes[n].materia       >= (s32)s->materiarum)
        {
            redde FALSUM;
        }
    }
    per (n = ZEPHYRUM; n < s->indicum; n++)
    {
        si (s->indices[n] >= s->verticum)
        {
            redde FALSUM;
        }
    }
    per (n = ZEPHYRUM; n < s->materiarum; n++)
    {
        si (   s->materiae[n].textura < -I
            || s->materiae[n].textura >= (s32)s->texturarum)
        {
            redde FALSUM;
        }
    }
    per (n = ZEPHYRUM; n < s->texturarum; n++)
    {
        si (   !s->texturae[n].pixela || s->texturae[n].latitudo < I
            || s->texturae[n].altitudo < I)
        {
            redde FALSUM;
        }
    }
    per (n = ZEPHYRUM; n < s->cyclorum; n++)
    {
        si (   (s->cycli[n].canalium && !s->cycli[n].canales)
            || !(s->cycli[n].duratio >= 0.0f))
        {
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < s->cycli[n].canalium; k++)
        {
            si (!_canalis_sanus(&s->cycli[n].canales[k], s->nodorum))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

/* ordo: transitus repetiti, nodus ponitur cum parens positus est;
 * transitus sine progressu = circulus */
interior b32
_ordinem_computare (
     Statua* s,
    Piscina* piscina)
{
    b32* positus;
    i32  positi = ZEPHYRUM;
    i32  ante;
    i32  n;

    si (s->nodorum == ZEPHYRUM)
    {
        s->ordo = NIHIL;
        redde VERUM;
    }
    s->ordo  = (i32*)piscina_allocare(piscina,
        s->nodorum * magnitudo(i32));
    positus  = (b32*)piscina_allocare(piscina,
        s->nodorum * magnitudo(b32));
    si (!s->ordo || !positus)
    {
        redde FALSUM;
    }
    memset(positus, ZEPHYRUM, s->nodorum * magnitudo(b32));
    dum (positi < s->nodorum)
    {
        ante = positi;
        per (n = ZEPHYRUM; n < s->nodorum; n++)
        {
            si (   !positus[n]
                && (   s->nodi[n].parens < ZEPHYRUM
                    || positus[s->nodi[n].parens]))
            {
                positus[n]         = VERUM;
                s->ordo[positi++]  = n;
            }
        }
        si (positi == ante)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* vertices nodorum cum retibus, in positura data, visitati: modus 0
 * capsam (minimum, maximum) extendit, modus I distantiam maximam a
 * centro */
interior vacuum
_vertices_metiri (
                constans Statua* s,
    constans StereometriaMatrix* matrices,
                            b32  radium,
             StereometriaVector* minimum,
             StereometriaVector* maximum,
                            b32* coeptum,
                            f32* radius)
{
    constans StatuaRete* rete;
    constans StatuaPars* pars;
     StereometriaVector  p;
                    i32  n;
                    i32  k;
                    i32  j;
                    i32  v;
                    f32  d;

    per (n = ZEPHYRUM; n < s->nodorum; n++)
    {
        si (s->nodi[n].rete < ZEPHYRUM)
        {
            perge;
        }
        rete = &s->retia[s->nodi[n].rete];
        per (k = ZEPHYRUM; k < rete->partium; k++)
        {
            pars = &s->partes[rete->pars_prima + k];
            per (j = ZEPHYRUM; j < pars->indicum; j++)
            {
                v = s->indices[pars->index_primus + j];
                p = stereometria_punctum_transformare(matrices[n],
                    stereometria_vector(s->positiones[v * III],
                        s->positiones[v * III + I],
                        s->positiones[v * III + II]), NIHIL);
                si (radium)
                {
                    d = stereometria_longitudo(
                        stereometria_subtrahere(p, s->centrum));
                    si (d > *radius)
                    {
                        *radius = d;
                    }
                    perge;
                }
                si (!*coeptum)
                {
                    *minimum = p;
                    *maximum = p;
                    *coeptum = VERUM;
                    perge;
                }
                si (p.x < minimum->x)
                {
                    minimum->x = p.x;
                }
                si (p.y < minimum->y)
                {
                    minimum->y = p.y;
                }
                si (p.z < minimum->z)
                {
                    minimum->z = p.z;
                }
                si (p.x > maximum->x)
                {
                    maximum->x = p.x;
                }
                si (p.y > maximum->y)
                {
                    maximum->y = p.y;
                }
                si (p.z > maximum->z)
                {
                    maximum->z = p.z;
                }
            }
        }
    }
}

/* sphaera positurae quietis: transitus primus capsam verticum,
 * secundus distantiam maximam a centro capsae. Cycli NON intrant:
 * pars quae in cyclo longe volat (saxum iactum) sphaeram, et ergo
 * mensuram statuae in pictura, ne regat */
interior b32
_sphaeram_computare (
     Statua* s,
    Piscina* piscina)
{
    StereometriaMatrix* matrices;
    StereometriaVector  minimum;
    StereometriaVector  maximum;
                   b32  coeptum = FALSUM;
                   b32  radium;

    s->centrum  = stereometria_vector(0.0f, 0.0f, 0.0f);
    s->radius   = 0.0f;
    minimum     = s->centrum;
    maximum     = s->centrum;
    si (s->nodorum == ZEPHYRUM)
    {
        redde VERUM;
    }
    matrices = (StereometriaMatrix*)piscina_allocare(piscina,
        s->nodorum * magnitudo(StereometriaMatrix));
    si (!matrices)
    {
        redde FALSUM;
    }
    _positura(s, NIHIL, 0.0f, matrices);
    per (radium = FALSUM; radium <= VERUM; radium++)
    {
        _vertices_metiri(s, matrices, radium, &minimum, &maximum,
            &coeptum, &s->radius);
        si (!radium)
        {
            s->centrum = stereometria_scalare(
                stereometria_addere(minimum, maximum), 0.5f);
        }
    }
    redde VERUM;
}

b32
statua_finire (
     Statua* statua,
    Piscina* piscina)
{
    redde statua && piscina && _sana(statua)
        && _ordinem_computare(statua, piscina)
        && _sphaeram_computare(statua, piscina);
}
