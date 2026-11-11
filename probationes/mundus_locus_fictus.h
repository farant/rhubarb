/*
 * mundus_locus_fictus.h - locus manu factus probationibus mundi
 *
 * Statua ex quadratis horizontalibus signatis (x dextrorsum, z
 * deorsum in tabula), nodus et rete per quadratum:
 *   solum       y 0   x [0, 8]   z [0, 16]   gressibile
 *   superius    y 4   x [8, 12]  z [0, 16]   gressibile
 *   clivus      y 0 -> 4 per x [4, 8], z [8, 12]   gressibilis
 *   scalae      VIII gradus dimidiae unitatis, x [4, 8], z [12, 16]
 *   aqua        y 0   x [1, 3]   z [1, 3]   (in solo iacens)
 *   saxum       y 1   x [5, 6]   z [1, 2]   solidum
 *   ornatus     y 2   x [1, 3]   z [5, 7]   sine signo
 *   paries      verticalis ad x 8, z [0, 8]  solidus
 *
 * Solum caput: functiones interiores, in probatione includenda post
 * credo.h. Memoria ex piscina sectionis.
 */

#ifndef MUNDUS_LOCUS_FICTUS_H
#define MUNDUS_LOCUS_FICTUS_H

#include "latina.h"
#include "piscina.h"
#include "stereometria.h"
#include "statua.h"
#include "credo.h"
#include <string.h>

#define LOCI_FICTI_QUADRATA_MAXIMA XXXII
#define LOCI_FICTI_GRESSIBILE \
    (STATUA_SIGNUM_GRESSIBILE | STATUA_SIGNUM_SOLIDUM)

nomen structura {
    f32 anguli[XII];   /* IV vertices (x y z) */
    i32 signa;
} LociFictiQuadratum;

interior vacuum*
loci_ficti_memoria (
    CredoContextus* c,
    memoriae_index  mensura)
{
    vacuum* p = piscina_allocare(c->piscina, mensura ? mensura : I);

    memset(p, ZEPHYRUM, mensura);
    redde p;
}

/* quadratum horizontale aut clivus secundum x: y0 ad x0, y1 ad x1 */
interior LociFictiQuadratum
loci_ficti_quadratum (
    f32 x0,
    f32 x1,
    f32 z0,
    f32 z1,
    f32 y0,
    f32 y1,
    i32 signa)
{
    LociFictiQuadratum q;

    q.anguli[ZEPHYRUM]  = x0;
    q.anguli[I]         = y0;
    q.anguli[II]        = z0;
    q.anguli[III]       = x1;
    q.anguli[IV]        = y1;
    q.anguli[V]         = z0;
    q.anguli[VI]        = x1;
    q.anguli[VII]       = y1;
    q.anguli[VIII]      = z1;
    q.anguli[IX]        = x0;
    q.anguli[X]         = y0;
    q.anguli[XI]        = z1;
    q.signa             = signa;
    redde q;
}

/* statua finita: nodus et rete per quadratum */
interior Statua*
loci_ficti_statua (
                 CredoContextus* c,
    constans LociFictiQuadratum* q,
                            i32  numerus)
{
    /* ordo verticum: facies sursum (+y) spectat - desuper visa
     * pingitur */
    hic_manens constans i32  INDICES[VI] = { 0, 2, 1, 0, 3, 2 };
                     Statua* s;
                        i32  k;
                        i32  v;

    s           = (Statua*)loci_ficti_memoria(c, magnitudo(Statua));
    s->nodorum  = numerus;
    s->nodi         = (StatuaNodus*)loci_ficti_memoria(c,
        (numerus + I) * magnitudo(StatuaNodus));
    s->retium       = numerus;
    s->retia        = (StatuaRete*)loci_ficti_memoria(c,
        (numerus + I) * magnitudo(StatuaRete));
    s->partium      = numerus;
    s->partes       = (StatuaPars*)loci_ficti_memoria(c,
        (numerus + I) * magnitudo(StatuaPars));
    s->verticum     = numerus * IV;
    s->positiones   = (f32*)loci_ficti_memoria(c,
        (numerus + I) * XII * magnitudo(f32));
    s->normales     = (f32*)loci_ficti_memoria(c,
        (numerus + I) * XII * magnitudo(f32));
    s->coordinatae  = (f32*)loci_ficti_memoria(c,
        (numerus + I) * VIII * magnitudo(f32));
    s->indicum      = numerus * VI;
    s->indices      = (i32*)loci_ficti_memoria(c,
        (numerus + I) * VI * magnitudo(i32));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        s->nodi[k].parens   = -I;
        s->nodi[k].rete     = (s32)k;
        s->nodi[k].signa    = q[k].signa;
        s->nodi[k].rotatio  = stereometria_quaternio_identitas();
        s->nodi[k].scala           = stereometria_vector(1.0f, 1.0f,
            1.0f);
        s->retia[k].pars_prima     = k;
        s->retia[k].partium        = I;
        s->partes[k].index_primus  = k * VI;
        s->partes[k].indicum       = VI;
        s->partes[k].materia       = -I;
        memcpy(s->positiones + k * XII, q[k].anguli,
            XII * magnitudo(f32));
        per (v = ZEPHYRUM; v < IV; v++)
        {
            s->normales[(k * IV + v) * III + I] = 1.0f;
        }
        per (v = ZEPHYRUM; v < VI; v++)
        {
            s->indices[k * VI + v] = k * IV + INDICES[v];
        }
    }
    CREDO_VERUM (statua_finire(s, c->piscina));
    redde s;
}

/* locus totus (vide caput) */
interior Statua*
loci_ficti_locus (
    CredoContextus* c)
{
    LociFictiQuadratum q[LOCI_FICTI_QUADRATA_MAXIMA];
                   i32 n = ZEPHYRUM;
                   i32 k;

    q[n++] = loci_ficti_quadratum(0.0f, 8.0f, 0.0f, 16.0f, 0.0f, 0.0f,
        LOCI_FICTI_GRESSIBILE);
    q[n++] = loci_ficti_quadratum(8.0f, 12.0f, 0.0f, 16.0f, 4.0f, 4.0f,
        LOCI_FICTI_GRESSIBILE);
    q[n++] = loci_ficti_quadratum(4.0f, 8.0f, 8.0f, 12.0f, 0.0f, 4.0f,
        LOCI_FICTI_GRESSIBILE);
    per (k = ZEPHYRUM; k < VIII; k++)
    {
        q[n++] = loci_ficti_quadratum(4.0f + 0.5f * (f32)k,
            4.5f + 0.5f * (f32)k, 12.0f, 16.0f, 0.5f * (f32)(k + I),
            0.5f * (f32)(k + I), LOCI_FICTI_GRESSIBILE);
    }
    q[n++] = loci_ficti_quadratum(1.0f, 3.0f, 1.0f, 3.0f, 0.0f, 0.0f,
        STATUA_SIGNUM_AQUA);
    q[n++] = loci_ficti_quadratum(5.0f, 6.0f, 1.0f, 2.0f, 1.0f, 1.0f,
        STATUA_SIGNUM_SOLIDUM);
    q[n++] = loci_ficti_quadratum(1.0f, 3.0f, 5.0f, 7.0f, 2.0f, 2.0f,
        ZEPHYRUM);
    /* paries verticalis: x 8 ubique, y 0 -> 4 secundum z */
    q[n] = loci_ficti_quadratum(8.0f, 8.0f, 0.0f, 8.0f, 0.0f, 0.0f,
        STATUA_SIGNUM_SOLIDUM);
    q[n].anguli[VII]  = 4.0f;
    q[n].anguli[X]    = 4.0f;
    n++;
    redde loci_ficti_statua(c, q, n);
}

#endif /* MUNDUS_LOCUS_FICTUS_H */
