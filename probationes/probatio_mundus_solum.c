/* probatio_mundus_solum.c - tabula gressus loci (mundus_solum.h)
 *
 * Locus manu factus ex quadratis horizontalibus signatis (x dextrorsum,
 * z deorsum in tabula):
 *   solum       y 0   x [0, 8]   z [0, 16]   gressibile
 *   superius    y 4   x [8, 12]  z [0, 16]   gressibile
 *   clivus      y 0 -> 4 per x [4, 8], z [8, 12]   gressibilis
 *   scalae      VIII gradus dimidiae unitatis, x [4, 8], z [12, 16]
 *   aqua        y 0   x [1, 3]   z [1, 3]   (in solo iacens)
 *   saxum       y 1   x [5, 6]   z [1, 2]   solidum
 *   ornatus     y 2   x [1, 3]   z [5, 7]   sine signo
 *   paries      verticalis ad x 8, z [0, 8]  solidus
 * genera, altitudo, licet, radius, initium, vacua; planum: locus verus
 * (probationes/fixa/statuae/planum.glb, apparatus scaenae Retro
 * Diffusion a Frano emptus) cum numeris fixis et ambulatione per
 * scalas.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "stereometria.h"
#include "statua.h"
#include "statua_glb.h"
#include "mundus_solum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VIA_PLANI "probationes/fixa/statuae/planum.glb"
#define QUADRATA_MAXIMA XXXII

nomen structura {
    f32 anguli[XII];   /* IV vertices (x y z) */
    i32 signa;
} Quadratum;

interior vacuum*
_locus (
    CredoContextus* c,
    memoriae_index  mensura)
{
    vacuum* p = piscina_allocare(c->piscina, mensura ? mensura : I);

    memset(p, ZEPHYRUM, mensura);
    redde p;
}

/* quadratum horizontale aut clivus secundum x: y0 ad x0, y1 ad x1 */
interior Quadratum
_quadratum (
    f32 x0,
    f32 x1,
    f32 z0,
    f32 z1,
    f32 y0,
    f32 y1,
    i32 signa)
{
    Quadratum q;

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

/* statua: nodus et rete per quadratum */
interior Statua*
_statuam_struere (
        CredoContextus* c,
    constans Quadratum* q,
                   i32  numerus)
{
    hic_manens constans i32  INDICES[VI] = { 0, 1, 2, 0, 2, 3 };
                     Statua* s = (Statua*)_locus(c, magnitudo(Statua));
                        i32  k;
                        i32  v;

    s->nodorum     = numerus;
    s->nodi        = (StatuaNodus*)_locus(c,
        (numerus + I) * magnitudo(StatuaNodus));
    s->retium      = numerus;
    s->retia       = (StatuaRete*)_locus(c,
        (numerus + I) * magnitudo(StatuaRete));
    s->partium     = numerus;
    s->partes      = (StatuaPars*)_locus(c,
        (numerus + I) * magnitudo(StatuaPars));
    s->verticum    = numerus * IV;
    s->positiones  = (f32*)_locus(c,
        (numerus + I) * XII * magnitudo(f32));
    s->normales    = (f32*)_locus(c,
        (numerus + I) * XII * magnitudo(f32));
    s->coordinatae = (f32*)_locus(c,
        (numerus + I) * VIII * magnitudo(f32));
    s->indicum     = numerus * VI;
    s->indices     = (i32*)_locus(c,
        (numerus + I) * VI * magnitudo(i32));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        s->nodi[k].parens = -I;
        s->nodi[k].rete = (s32)k;
        s->nodi[k].signa = q[k].signa;
        s->nodi[k].rotatio = stereometria_quaternio_identitas();
        s->nodi[k].scala = stereometria_vector(1.0f, 1.0f, 1.0f);
        s->retia[k].pars_prima = k;
        s->retia[k].partium = I;
        s->partes[k].index_primus = k * VI;
        s->partes[k].indicum = VI;
        s->partes[k].materia = -I;
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

#define GRESSIBILE (STATUA_SIGNUM_GRESSIBILE | STATUA_SIGNUM_SOLIDUM)

interior MundusSolum*
_locum_struere (
    CredoContextus* c)
{
      Quadratum  q[QUADRATA_MAXIMA];
            i32  n = ZEPHYRUM;
            i32  k;
    MundusSolum* s;

    q[n++] = _quadratum(0.0f, 8.0f, 0.0f, 16.0f, 0.0f, 0.0f,
        GRESSIBILE);
    q[n++] = _quadratum(8.0f, 12.0f, 0.0f, 16.0f, 4.0f, 4.0f,
        GRESSIBILE);
    q[n++] = _quadratum(4.0f, 8.0f, 8.0f, 12.0f, 0.0f, 4.0f,
        GRESSIBILE);
    per (k = ZEPHYRUM; k < VIII; k++)
    {
        q[n++] = _quadratum(4.0f + 0.5f * (f32)k, 4.5f + 0.5f * (f32)k,
            12.0f, 16.0f, 0.5f * (f32)(k + I), 0.5f * (f32)(k + I),
            GRESSIBILE);
    }
    q[n++] = _quadratum(1.0f, 3.0f, 1.0f, 3.0f, 0.0f, 0.0f,
        STATUA_SIGNUM_AQUA);
    q[n++] = _quadratum(5.0f, 6.0f, 1.0f, 2.0f, 1.0f, 1.0f,
        STATUA_SIGNUM_SOLIDUM);
    q[n++] = _quadratum(1.0f, 3.0f, 5.0f, 7.0f, 2.0f, 2.0f, ZEPHYRUM);
    /* paries verticalis: x 8 ubique, y 0 -> 4 secundum z */
    q[n] = _quadratum(8.0f, 8.0f, 0.0f, 8.0f, 0.0f, 0.0f,
        STATUA_SIGNUM_SOLIDUM);
    q[n].anguli[VII]  = 4.0f;
    q[n].anguli[X]    = 4.0f;
    n++;
    s = mundus_solum_struere(_statuam_struere(c, q, n), c->piscina);
    CREDO_NON_NIHIL (s);
    redde s;
}

interior vacuum
_sectio_genera (
    CredoContextus* c)
{
    MundusSolum* s = _locum_struere(c);

    CREDO_NECESSE (s != NIHIL);
    CREDO_AEQUALIS_I32 (s->latitudo, XII * MUNDUS_CELLAE_PER_UNITATEM);
    CREDO_AEQUALIS_I32 (s->altitudo, XVI * MUNDUS_CELLAE_PER_UNITATEM);
    CREDO_AEQUALIS_I32 (mundus_solum_genus(s, 2.0f, 2.0f),
        MUNDUS_SOLUM_AQUA);
    CREDO_AEQUALIS_I32 (mundus_solum_genus(s, 5.5f, 1.5f),
        MUNDUS_SOLUM_SOLIDUM);
    /* sub ornatu sine signo: solum */
    CREDO_AEQUALIS_I32 (mundus_solum_genus(s, 2.0f, 6.0f),
        MUNDUS_SOLUM_GRESSIBILE);
    CREDO_AEQUALIS_I32 (mundus_solum_genus(s, 10.0f, 2.0f),
        MUNDUS_SOLUM_GRESSIBILE);
    CREDO_AEQUALIS_I32 (mundus_solum_genus(s, 6.0f, 10.0f),
        MUNDUS_SOLUM_GRESSIBILE);
    /* paries verticalis nihil tegit */
    CREDO_AEQUALIS_I32 (mundus_solum_genus(s, 7.9f, 2.0f),
        MUNDUS_SOLUM_GRESSIBILE);
    CREDO_AEQUALIS_I32 (mundus_solum_genus(s, 8.1f, 2.0f),
        MUNDUS_SOLUM_GRESSIBILE);
    /* extra locum */
    CREDO_AEQUALIS_I32 (mundus_solum_genus(s, -0.1f, 2.0f),
        MUNDUS_SOLUM_NIHIL);
    CREDO_AEQUALIS_I32 (mundus_solum_genus(s, 2.0f, 16.1f),
        MUNDUS_SOLUM_NIHIL);
    CREDO_AEQUALIS_I32 (mundus_solum_genus(s, 1000.0f, -1000.0f),
        MUNDUS_SOLUM_NIHIL);
    CREDO_AEQUALIS_I32 (mundus_solum_genus(NIHIL, 2.0f, 2.0f),
        MUNDUS_SOLUM_NIHIL);
}

interior vacuum
_sectio_altitudo (
    CredoContextus* c)
{
    MundusSolum* s = _locum_struere(c);
            f32  y = -1.0f;

    CREDO_NECESSE (s != NIHIL);
    CREDO_VERUM (mundus_solum_altitudo(s, 2.0f, 6.0f, &y));
    CREDO_F32_PROXIMUS (y, 0.0f, 0.001f);
    CREDO_VERUM (mundus_solum_altitudo(s, 10.0f, 2.0f, &y));
    CREDO_F32_PROXIMUS (y, 4.0f, 0.001f);
    /* clivus medius */
    CREDO_VERUM (mundus_solum_altitudo(s, 6.0f, 10.0f, &y));
    CREDO_F32_PROXIMUS (y, 2.0f, 0.3f);
    /* scalae: gradus leniuntur (inter gradum quartum et quintum) */
    CREDO_VERUM (mundus_solum_altitudo(s, 6.0f, 14.0f, &y));
    CREDO_VERUM (y > 2.0f && y < 2.5f);
    /* margo rupis: nec infra descendit nec supra ascendit */
    CREDO_VERUM (mundus_solum_altitudo(s, 7.95f, 2.0f, &y));
    CREDO_F32_PROXIMUS (y, 0.0f, 0.001f);
    CREDO_VERUM (mundus_solum_altitudo(s, 8.05f, 2.0f, &y));
    CREDO_F32_PROXIMUS (y, 4.0f, 0.001f);
    /* non gressibile: FALSUM, y intactum */
    y = -7.0f;
    CREDO_FALSUM (mundus_solum_altitudo(s, 2.0f, 2.0f, &y));
    CREDO_FALSUM (mundus_solum_altitudo(s, 5.5f, 1.5f, &y));
    CREDO_FALSUM (mundus_solum_altitudo(s, -1.0f, 2.0f, &y));
    CREDO_F32_PROXIMUS (y, -7.0f, 0.001f);
    CREDO_FALSUM (mundus_solum_altitudo(s, 2.0f, 6.0f, NIHIL));
}

/* ab x0 ad x1 in linea z passibus octavae unitatis: omnes licent? */
interior b32
_iter_licet (
    constans MundusSolum* s,
                     f32  x0,
                     f32  x1,
                     f32  z,
                     f32  radius)
{
    f32 passus  = x1 > x0 ? 0.125f : -0.125f;
    f32 x       = x0;

    dum (   (passus > 0.0f && x + passus <= x1)
         || (passus < 0.0f && x + passus >= x1))
    {
        si (!mundus_solum_licet(s, x, z, x + passus, z, radius))
        {
            redde FALSUM;
        }
        x += passus;
    }
    redde VERUM;
}

interior vacuum
_sectio_licet (
    CredoContextus* c)
{
    MundusSolum* s = _locum_struere(c);

    CREDO_NECESSE (s != NIHIL);
    /* per solum */
    CREDO_VERUM (_iter_licet(s, 0.5f, 7.5f, 6.0f, 0.0f));
    /* trans parietem rupis: non, utroque versu */
    CREDO_FALSUM (mundus_solum_licet(s, 7.95f, 2.0f, 8.05f, 2.0f,
        0.0f));
    CREDO_FALSUM (mundus_solum_licet(s, 8.05f, 2.0f, 7.95f, 2.0f,
        0.0f));
    CREDO_FALSUM (_iter_licet(s, 6.0f, 10.0f, 2.0f, 0.0f));
    /* clivus sursum et deorsum */
    CREDO_VERUM (_iter_licet(s, 2.0f, 11.0f, 10.0f, 0.0f));
    CREDO_VERUM (_iter_licet(s, 11.0f, 2.0f, 10.0f, 0.0f));
    /* scalae sursum et deorsum, etiam cum corpore */
    CREDO_VERUM (_iter_licet(s, 2.0f, 11.0f, 14.0f, 0.0f));
    CREDO_VERUM (_iter_licet(s, 11.0f, 2.0f, 14.0f, 0.0f));
    CREDO_VERUM (_iter_licet(s, 2.0f, 11.0f, 14.0f, 0.5f));
    CREDO_VERUM (_iter_licet(s, 2.0f, 11.0f, 10.0f, 0.5f));
    /* in aquam, in saxum, extra locum */
    CREDO_FALSUM (mundus_solum_licet(s, 0.95f, 2.0f, 1.05f, 2.0f,
        0.0f));
    CREDO_FALSUM (mundus_solum_licet(s, 4.95f, 1.5f, 5.05f, 1.5f,
        0.0f));
    CREDO_FALSUM (mundus_solum_licet(s, 0.05f, 6.0f, -0.05f, 6.0f,
        0.0f));
    /* initium non gressibile */
    CREDO_FALSUM (mundus_solum_licet(s, 2.0f, 2.0f, 0.5f, 2.0f, 0.0f));
    /* a latere in clivum altum: saltus */
    CREDO_FALSUM (mundus_solum_licet(s, 7.0f, 7.95f, 7.0f, 8.05f,
        0.0f));
    CREDO_FALSUM (mundus_solum_licet(NIHIL, 0.5f, 6.0f, 0.6f, 6.0f,
        0.0f));
}

interior vacuum
_sectio_radius (
    CredoContextus* c)
{
    MundusSolum* s = _locum_struere(c);

    CREDO_NECESSE (s != NIHIL);
    /* iuxta saxum */
    CREDO_VERUM (mundus_solum_stare(s, 4.7f, 1.5f, 0.0f));
    CREDO_FALSUM (mundus_solum_stare(s, 4.7f, 1.5f, 0.5f));
    CREDO_VERUM (mundus_solum_stare(s, 4.2f, 1.5f, 0.5f));
    /* sub rupe et in margine eius */
    CREDO_VERUM (mundus_solum_stare(s, 7.7f, 2.0f, 0.0f));
    CREDO_FALSUM (mundus_solum_stare(s, 7.7f, 2.0f, 0.5f));
    CREDO_VERUM (mundus_solum_stare(s, 8.3f, 2.0f, 0.0f));
    CREDO_FALSUM (mundus_solum_stare(s, 8.3f, 2.0f, 0.5f));
    /* iuxta marginem loci et aquam */
    CREDO_FALSUM (mundus_solum_stare(s, 0.2f, 6.0f, 0.5f));
    CREDO_FALSUM (mundus_solum_stare(s, 3.3f, 2.0f, 0.5f));
    CREDO_VERUM (mundus_solum_stare(s, 3.7f, 2.0f, 0.5f));
    /* in scalis et in clivo medio */
    CREDO_VERUM (mundus_solum_stare(s, 6.0f, 14.0f, 0.5f));
    CREDO_VERUM (mundus_solum_stare(s, 6.0f, 10.0f, 0.5f));
    /* non gressibile, radius malus */
    CREDO_FALSUM (mundus_solum_stare(s, 2.0f, 2.0f, 0.0f));
    CREDO_FALSUM (mundus_solum_stare(s, 2.0f, 6.0f, -1.0f));
}

interior vacuum
_sectio_initium (
    CredoContextus* c)
{
    MundusSolum* s = _locum_struere(c);
      Quadratum  q[I];
            f32  x = -1.0f;
            f32  z = -1.0f;

    CREDO_NECESSE (s != NIHIL);
    CREDO_VERUM (mundus_solum_initium(s, 0.5f, &x, &z));
    CREDO_VERUM (mundus_solum_stare(s, x, z, 0.5f));
    /* medio (6, 8) proximum: intra duas unitates */
    CREDO_VERUM ((x - 6.0f) * (x - 6.0f) + (z - 8.0f) * (z - 8.0f)
        < 4.0f);
    /* locus sine cella gressibili */
    q[ZEPHYRUM] = _quadratum(0.0f, 4.0f, 0.0f, 4.0f, 0.0f, 0.0f,
        STATUA_SIGNUM_AQUA);
    s = mundus_solum_struere(_statuam_struere(c, q, I), c->piscina);
    CREDO_NECESSE (s != NIHIL);
    CREDO_FALSUM (mundus_solum_initium(s, 0.5f, &x, &z));
    /* corpus latius quam locus */
    q[ZEPHYRUM] = _quadratum(0.0f, 0.5f, 0.0f, 0.5f, 0.0f, 0.0f,
        GRESSIBILE);
    s = mundus_solum_struere(_statuam_struere(c, q, I), c->piscina);
    CREDO_NECESSE (s != NIHIL);
    CREDO_VERUM (mundus_solum_initium(s, 0.0f, &x, &z));
    CREDO_FALSUM (mundus_solum_initium(s, 0.5f, &x, &z));
}

interior vacuum
_sectio_vacua (
    CredoContextus* c)
{
      Quadratum  q[I];
    MundusSolum* s;
            f32  x = 0.0f;
            f32  z = 0.0f;

    /* statua sine signis: tabula vacua */
    q[ZEPHYRUM] = _quadratum(0.0f, 4.0f, 0.0f, 4.0f, 0.0f, 0.0f,
        ZEPHYRUM);
    s = mundus_solum_struere(_statuam_struere(c, q, I), c->piscina);
    CREDO_NECESSE (s != NIHIL);
    CREDO_AEQUALIS_I32 (s->latitudo, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (s->altitudo, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (mundus_solum_genus(s, 2.0f, 2.0f),
        MUNDUS_SOLUM_NIHIL);
    CREDO_FALSUM (mundus_solum_altitudo(s, 2.0f, 2.0f, &x));
    CREDO_FALSUM (mundus_solum_stare(s, 2.0f, 2.0f, 0.0f));
    CREDO_FALSUM (mundus_solum_licet(s, 2.0f, 2.0f, 2.1f, 2.0f, 0.0f));
    CREDO_FALSUM (mundus_solum_initium(s, 0.0f, &x, &z));
    /* statua sine nodis */
    s = mundus_solum_struere(_statuam_struere(c, q, ZEPHYRUM),
        c->piscina);
    CREDO_NECESSE (s != NIHIL);
    CREDO_AEQUALIS_I32 (s->latitudo, ZEPHYRUM);
    CREDO_NIHIL (mundus_solum_struere(NIHIL, c->piscina));
}

/* locus verus: numeri fixi tabulae et ambulatio per scalas */
#define PLANI_GRESSIBILES  60626
#define PLANI_AQUAE        3424
#define PLANI_SOLIDAE      1486
#define PLANI_ATTINGIBILES 42881

hic_manens constans s32 VICINI[VIII] = { 1, 0, -1, 0, 0, 1, 0, -1 };

interior vacuum
_sectio_planum (
    CredoContextus* c)
{
    StatuaFructus  f;
      MundusSolum* s;
               i8* visa;
              s32* acervus;
              i32  numeri[IV];
              i32  attingibiles  = ZEPHYRUM;
              i32  summae        = ZEPHYRUM;
              i32  mediae        = ZEPHYRUM;
              i32  n             = ZEPHYRUM;
              i32  k;
              s32  cella_x;
              s32  cella_z;
              s32  d;
              f32  x = 0.0f;
              f32  z = 0.0f;
              f32  punctum_x;
              f32  punctum_z;
              f32  cella;

    cella = 1.0f / (f32)MUNDUS_CELLAE_PER_UNITATEM;
    f = statua_glb_legere(filum_legere_totum(VIA_PLANI, c->piscina),
        c->piscina);
    CREDO_NECESSE (f.successus);
    s = mundus_solum_struere(f.statua, c->piscina);
    CREDO_NECESSE (s != NIHIL);
    CREDO_AEQUALIS_I32 (s->latitudo, LXIV * MUNDUS_CELLAE_PER_UNITATEM);
    CREDO_AEQUALIS_I32 (s->altitudo, LXIV * MUNDUS_CELLAE_PER_UNITATEM);
    memset(numeri, ZEPHYRUM, magnitudo(numeri));
    per (k = ZEPHYRUM; k < s->latitudo * s->altitudo; k++)
    {
        numeri[s->genera[k]]++;
    }
    /* ambulatio corporis radii dimidii ab initio per cellas vicinas:
     * quot cellae attinguntur, et attingitur stratum summum? */
    CREDO_NECESSE (mundus_solum_initium(s, 0.5f, &x, &z));
    visa     = (i8*)piscina_allocare(c->piscina,
        (memoriae_index)s->latitudo * s->altitudo);
    acervus  = (s32*)piscina_allocare(c->piscina,
        (memoriae_index)s->latitudo * s->altitudo * II
            * magnitudo(s32));
    memset(visa, ZEPHYRUM, (memoriae_index)s->latitudo * s->altitudo);
    cella_x = (s32)((x - s->origo_x) / cella);
    cella_z = (s32)((z - s->origo_z) / cella);
    visa[(i32)cella_z * s->latitudo + (i32)cella_x] = I;
    acervus[n++] = cella_x;
    acervus[n++] = cella_z;
    dum (n > ZEPHYRUM)
    {
        cella_z = acervus[--n];
        cella_x = acervus[--n];
        attingibiles++;
        punctum_x = s->origo_x + ((f32)cella_x + 0.5f) * cella;
        punctum_z = s->origo_z + ((f32)cella_z + 0.5f) * cella;
        si (s->altitudines[(i32)cella_z * s->latitudo + (i32)cella_x]
            > 7.0f)
        {
            summae++;
        }
        alioquin si (s->altitudines[(i32)cella_z * s->latitudo
                     + (i32)cella_x]
                     > 3.0f)
        {
            mediae++;
        }
        per (d = ZEPHYRUM; d < IV; d++)
        {
            s32 vicina_x = cella_x + VICINI[d * II];
            s32 vicina_z = cella_z + VICINI[d * II + I];

            si (   vicina_x < ZEPHYRUM || vicina_z < ZEPHYRUM
                || vicina_x >= (s32)s->latitudo
                || vicina_z >= (s32)s->altitudo
                || visa[(i32)vicina_z * s->latitudo + (i32)vicina_x]
                || !mundus_solum_licet(s, punctum_x, punctum_z,
                       punctum_x + (f32)VICINI[d * II] * cella,
                       punctum_z + (f32)VICINI[d * II + I] * cella,
                       0.5f))
            {
                perge;
            }
            visa[(i32)vicina_z * s->latitudo + (i32)vicina_x] = I;
            acervus[n++] = vicina_x;
            acervus[n++] = vicina_z;
        }
    }
    si (   numeri[MUNDUS_SOLUM_GRESSIBILE] != PLANI_GRESSIBILES
        || numeri[MUNDUS_SOLUM_AQUA]       != PLANI_AQUAE
        || numeri[MUNDUS_SOLUM_SOLIDUM]    != PLANI_SOLIDAE
        || attingibiles                    != PLANI_ATTINGIBILES)
    {
        printf("  planum: gressibiles %u aquae %u solidae %u nihil %u"
            " | attingibiles %u (mediae %u summae %u)"
            " ab (%.2f, %.2f)\n",
            numeri[MUNDUS_SOLUM_GRESSIBILE], numeri[MUNDUS_SOLUM_AQUA],
            numeri[MUNDUS_SOLUM_SOLIDUM], numeri[MUNDUS_SOLUM_NIHIL],
            attingibiles, mediae, summae, (f64)x, (f64)z);
    }
    CREDO_AEQUALIS_I32 (numeri[MUNDUS_SOLUM_GRESSIBILE],
        PLANI_GRESSIBILES);
    CREDO_AEQUALIS_I32 (numeri[MUNDUS_SOLUM_AQUA], PLANI_AQUAE);
    CREDO_AEQUALIS_I32 (numeri[MUNDUS_SOLUM_SOLIDUM], PLANI_SOLIDAE);
    CREDO_AEQUALIS_I32 (attingibiles, PLANI_ATTINGIBILES);
    /* scalae transeunt: stratum medium (y circa 4.5) ab initio (in
     * strato imo) attingitur. Stratum summum (y circa 8.5) in hoc loco
     * scalas non habet (tres scalae omnes y 0.5 -> 3.8 iungunt):
     * rupes eius numquam superantur. */
    CREDO_VERUM (mediae > M);
    CREDO_AEQUALIS_I32 (summae, ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "genera", _sectio_genera, NIHIL, NIHIL, NIHIL, NIHIL },
    { "altitudo", _sectio_altitudo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "licet", _sectio_licet, NIHIL, NIHIL, NIHIL, NIHIL },
    { "radius", _sectio_radius, NIHIL, NIHIL, NIHIL, NIHIL },
    { "initium", _sectio_initium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vacua", _sectio_vacua, NIHIL, NIHIL, NIHIL, NIHIL },
    { "planum", _sectio_planum, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("mundus_solum", SECTIONES);
}
