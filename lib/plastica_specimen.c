/* plastica_specimen.c - mensurae et pagina aspectuum (vide
 * plastica_specimen.h) */

#include "plastica_specimen.h"
#include "statua_pictura.h"
#include "statuae_quadrum.h"
#include "dithering.h"
#include <math.h>
#include <string.h>

/* margo circa sphaeram rei, pars lateris cellae (utrimque) */
#define SPECIMEN_MARGO        0.08f
#define ASPECTUUM             IV
/* latus craticulae vestigii maximum (cellae); res maior vestigium 0
 * dat */
#define VESTIGII_LATUS_MAXIMUM 4096
#define AREA_MINIMA           0.000001f
#define PARITAS_ALTITUDINIS   0.0001f
/* centrum cellae in ipso margine trianguli (diagonalis faciei
 * capsae per centra cellarum transit): intra habetur */
#define MARGO_PONDERIS   0.0001f
/* index palettae chartae sub dithering Atkinson: error nullus a
 * charta in rem diffunditur */
#define CHARTA_PALETTAE       V

nomen enumeratio {
    VESTIGIUM_NIHIL = ZEPHYRUM,
    VESTIGIUM_GRESSIBILE,
    VESTIGIUM_AQUA,
    VESTIGIUM_SOLIDUM
} VestigiiGenus;

nomen structura {
     i8* genera;
    f32* altitudines;
    s32  cella_x_prima;
    s32  cella_z_prima;
    s32  latitudo;
    s32  altitudo;
} Vestigium;

nomen structura {
    f32 gyrus;
    f32 inclinatio;
} Aspectus;

nomen structura {
     constans character* titulus;
                    f32  color[III];
} ColorPartis;

/* frons, latus, desuper, tribus quartis (anguli visus mundi) */
hic_manens constans Aspectus ASPECTUS[ASPECTUUM] = {
    { 0.0f, 0.0f }, { 90.0f, 0.0f }, { 0.0f, 90.0f },
    { PLASTICA_SPECIMEN_GYRUS, PLASTICA_SPECIMEN_INCLINATIO }
};

hic_manens constans ColorPartis COLORES[PLASTICA_SPECIMEN_COLORES] = {
    { "red",     { 0.86f, 0.16f, 0.16f } },
    { "orange",  { 0.94f, 0.55f, 0.08f } },
    { "yellow",  { 0.92f, 0.80f, 0.16f } },
    { "lime",    { 0.59f, 0.86f, 0.16f } },
    { "green",   { 0.12f, 0.59f, 0.24f } },
    { "teal",    { 0.08f, 0.59f, 0.55f } },
    { "cyan",    { 0.24f, 0.78f, 0.92f } },
    { "blue",    { 0.16f, 0.31f, 0.86f } },
    { "purple",  { 0.51f, 0.24f, 0.78f } },
    { "magenta", { 0.86f, 0.20f, 0.78f } },
    { "pink",    { 0.96f, 0.59f, 0.75f } },
    { "gray",    { 0.51f, 0.51f, 0.51f } }
};

constans character*
plastica_specimen_color (
    i32 pars)
{
    redde COLORES[pars % PLASTICA_SPECIMEN_COLORES].titulus;
}

/* tractus qui numquam pingitur: alpha materiae infra limen
 * excisionis (obex invisus plasticae) */
interior b32
_tractus_invisus (
        constans Statua* statua,
    constans StatuaPars* pars)
{
    constans StatuaMateria* materia;

    si (pars->materia < ZEPHYRUM)
    {
        redde FALSUM;
    }
    materia = &statua->materiae[pars->materia];
    redde materia->limen_alpha >= 0.0f
        && materia->color[III] < materia->limen_alpha;
}

interior StereometriaVector
_vertex (
       constans Statua* statua,
    StereometriaMatrix  matrix,
                   i32  index)
{
    constans f32* p = statua->positiones + (memoriae_index)index * III;

    redde stereometria_punctum_transformare(matrix,
        stereometria_vector(p[ZEPHYRUM], p[I], p[II]), NIHIL);
}

interior s32
_cella (
    f32 coordinata)
{
    redde (s32)floor((f64)(coordinata / PLASTICA_CELLA_VESTIGII));
}

interior VestigiiGenus
_genus_signorum (
    i32 signa)
{
    si (signa & STATUA_SIGNUM_GRESSIBILE)
    {
        redde VESTIGIUM_GRESSIBILE;
    }
    si (signa & STATUA_SIGNUM_AQUA)
    {
        redde VESTIGIUM_AQUA;
    }
    si (signa & STATUA_SIGNUM_SOLIDUM)
    {
        redde VESTIGIUM_SOLIDUM;
    }
    redde VESTIGIUM_NIHIL;
}

interior s32
_coercere (
    s32 valor,
    s32 minimum,
    s32 maximum)
{
    si (valor < minimum)
    {
        redde minimum;
    }
    redde valor > maximum ? maximum : valor;
}

/* triangulum desuper in cellas (regula mundus_solum) */
interior vacuum
_triangulum (
             Vestigium* v,
         VestigiiGenus  genus,
    StereometriaVector  a,
    StereometriaVector  b,
    StereometriaVector  d)
{
    memoriae_index locus;
               f32 area;
               f32 punctum_x;
               f32 punctum_z;
               f32 pondus_a;
               f32 pondus_b;
               f32 pondus_d;
               f32 y;
               f32 minimum;
               f32 maximum;
               s32 x_primum;
               s32 x_ultimum;
               s32 z_primum;
               s32 z_ultimum;
               s32 cella_x;
               s32 cella_z;

    area = (b.z - d.z) * (a.x - d.x) + (d.x - b.x) * (a.z - d.z);
    si (area < AREA_MINIMA && area > -AREA_MINIMA)
    {
        redde;
    }
    minimum = a.x < b.x ? a.x : b.x;
    minimum = minimum < d.x ? minimum : d.x;
    maximum = a.x > b.x ? a.x : b.x;
    maximum = maximum > d.x ? maximum : d.x;
    x_primum   = _coercere(_cella(minimum) - v->cella_x_prima,
        ZEPHYRUM, v->latitudo - I);
    x_ultimum  = _coercere(_cella(maximum) - v->cella_x_prima,
        ZEPHYRUM, v->latitudo - I);
    minimum = a.z < b.z ? a.z : b.z;
    minimum = minimum < d.z ? minimum : d.z;
    maximum = a.z > b.z ? a.z : b.z;
    maximum = maximum > d.z ? maximum : d.z;
    z_primum   = _coercere(_cella(minimum) - v->cella_z_prima,
        ZEPHYRUM, v->altitudo - I);
    z_ultimum  = _coercere(_cella(maximum) - v->cella_z_prima,
        ZEPHYRUM, v->altitudo - I);
    per (cella_z = z_primum; cella_z <= z_ultimum; cella_z++)
    {
        per (cella_x = x_primum; cella_x <= x_ultimum; cella_x++)
        {
            punctum_x = ((f32)(cella_x + v->cella_x_prima) + 0.5f)
                * PLASTICA_CELLA_VESTIGII;
            punctum_z = ((f32)(cella_z + v->cella_z_prima) + 0.5f)
                * PLASTICA_CELLA_VESTIGII;
            pondus_a = ((b.z - d.z) * (punctum_x - d.x)
                + (d.x - b.x) * (punctum_z - d.z)) / area;
            pondus_b = ((d.z - a.z) * (punctum_x - d.x)
                + (a.x - d.x) * (punctum_z - d.z)) / area;
            pondus_d = 1.0f - pondus_a - pondus_b;
            si (   pondus_a < -MARGO_PONDERIS
                || pondus_b < -MARGO_PONDERIS
                || pondus_d < -MARGO_PONDERIS)
            {
                perge;
            }
            y      = pondus_a * a.y + pondus_b * b.y + pondus_d * d.y;
            locus  = (memoriae_index)cella_z
                * (memoriae_index)v->latitudo
                + (memoriae_index)cella_x;
            si (   v->genera[locus] == VESTIGIUM_NIHIL
                || y > v->altitudines[locus] + PARITAS_ALTITUDINIS
                || (   y >= v->altitudines[locus]
                           - PARITAS_ALTITUDINIS
                    && genus != VESTIGIUM_GRESSIBILE
                    && v->genera[locus] == VESTIGIUM_GRESSIBILE))
            {
                v->genera[locus]       = (i8)genus;
                v->altitudines[locus]  = y;
            }
        }
    }
}

/* vestigium: cellae solidae et gressibiles in mensuras */
interior vacuum
_vestigium_metiri (
                constans Statua* statua,
    constans StereometriaMatrix* matrices,
               PlasticaMensurae* m,
                        Piscina* piscina)
{
    constans StatuaRete* rete;
    constans StatuaPars* pars;
              Vestigium  v;
          VestigiiGenus  genus;
         memoriae_index  cellae;
         memoriae_index  c;
                    i32  n;
                    i32  j;
                    i32  k;

    v.cella_x_prima  = _cella(m->minimum.x);
    v.cella_z_prima  = _cella(m->minimum.z);
    v.latitudo       = _cella(m->maximum.x) - v.cella_x_prima + I;
    v.altitudo       = _cella(m->maximum.z) - v.cella_z_prima + I;
    si (   v.latitudo < I || v.altitudo < I
        || v.latitudo > VESTIGII_LATUS_MAXIMUM
        || v.altitudo > VESTIGII_LATUS_MAXIMUM)
    {
        redde;
    }
    cellae         = (memoriae_index)v.latitudo
        * (memoriae_index)v.altitudo;
    v.genera       = (i8*)piscina_allocare(piscina, cellae);
    v.altitudines  = (f32*)piscina_allocare(piscina,
        cellae * magnitudo(f32));
    si (!v.genera || !v.altitudines)
    {
        redde;
    }
    memset(v.genera, ZEPHYRUM, cellae);
    per (n = ZEPHYRUM; n < statua->nodorum; n++)
    {
        genus = _genus_signorum(statua->nodi[n].signa);
        si (   genus == VESTIGIUM_NIHIL
            || statua->nodi[n].rete < ZEPHYRUM)
        {
            perge;
        }
        rete = &statua->retia[statua->nodi[n].rete];
        per (j = ZEPHYRUM; j < rete->partium; j++)
        {
            pars = &statua->partes[rete->pars_prima + j];
            per (k = ZEPHYRUM; k + II < pars->indicum; k += III)
            {
                _triangulum(&v, genus,
                    _vertex(statua, matrices[n],
                        statua->indices[pars->index_primus + k]),
                    _vertex(statua, matrices[n],
                        statua->indices[pars->index_primus + k + I]),
                    _vertex(statua, matrices[n],
                        statua->indices[pars->index_primus + k
                            + II]));
            }
        }
    }
    per (c = ZEPHYRUM; c < cellae; c++)
    {
        m->cellae_solidae += (i32)(v.genera[c] == VESTIGIUM_SOLIDUM);
        m->cellae_gressibiles +=
            (i32)(v.genera[c] == VESTIGIUM_GRESSIBILE);
    }
}

interior StereometriaMatrix*
_positura_quietis (
    constans Statua* statua,
            Piscina* piscina)
{
    StereometriaMatrix* matrices = (StereometriaMatrix*)
        piscina_allocare(piscina, (memoriae_index)(statua->nodorum + I)
            * magnitudo(StereometriaMatrix));

    si (matrices)
    {
        statua_positura(statua, -I, 0.0f, matrices);
    }
    redde matrices;
}

PlasticaMensurae
plastica_metiri (
    constans Statua* statua,
            Piscina* piscina)
{
       constans StatuaRete* rete;
       constans StatuaPars* pars;
        StereometriaMatrix* matrices;
        StereometriaVector  p;
          PlasticaMensurae  m;
                       b32  aliquid = FALSUM;
                       b32  invisa;
                       i32  n;
                       i32  j;
                       i32  k;

    memset(&m, ZEPHYRUM, magnitudo(m));
    si (!statua || !piscina)
    {
        redde m;
    }
    matrices = _positura_quietis(statua, piscina);
    si (!matrices)
    {
        redde m;
    }
    per (n = ZEPHYRUM; n < statua->nodorum; n++)
    {
        si (statua->nodi[n].rete < ZEPHYRUM)
        {
            perge;
        }
        rete    = &statua->retia[statua->nodi[n].rete];
        invisa  = rete->partium > ZEPHYRUM;
        m.partium++;
        per (j = ZEPHYRUM; j < rete->partium; j++)
        {
            pars            = &statua->partes[rete->pars_prima + j];
            m.triangulorum  += pars->indicum / III;
            invisa          = invisa && _tractus_invisus(statua, pars);
            per (k = ZEPHYRUM; k < pars->indicum; k++)
            {
                p = _vertex(statua, matrices[n],
                    statua->indices[pars->index_primus + k]);
                si (!aliquid)
                {
                    m.minimum  = p;
                    m.maximum  = p;
                    aliquid    = VERUM;
                }
                m.minimum.x = p.x < m.minimum.x ? p.x : m.minimum.x;
                m.minimum.y = p.y < m.minimum.y ? p.y : m.minimum.y;
                m.minimum.z = p.z < m.minimum.z ? p.z : m.minimum.z;
                m.maximum.x = p.x > m.maximum.x ? p.x : m.maximum.x;
                m.maximum.y = p.y > m.maximum.y ? p.y : m.maximum.y;
                m.maximum.z = p.z > m.maximum.z ? p.z : m.maximum.z;
            }
        }
        m.invisae += (i32)invisa;
    }
    si (aliquid)
    {
        _vestigium_metiri(statua, matrices, &m, piscina);
    }
    redde m;
}

/* exemplar statuae in quo quaeque pars materiam plani coloris sui
 * habet. Retia et tractus iterum scribuntur (rete inter nodos
 * commune partem cuique suam dat); vertices, indices et nodorum
 * ordo communes manent. Tractus invisus materiam suam servat.
 * FALSUM si piscina deficit */
interior b32
_exemplar_partium (
    constans Statua* statua,
             Statua* exemplar,
            Piscina* piscina)
{
    constans StatuaRete* rete;
          StatuaMateria* materiae;
            StatuaNodus* nodi;
             StatuaRete* retia;
             StatuaPars* partes;
                    i32  nodi_retis  = ZEPHYRUM;
                    i32  tractus     = ZEPHYRUM;
                    i32  cursor      = ZEPHYRUM;
                    i32  pars        = ZEPHYRUM;
                    i32  n;
                    i32  j;
                    i32  k;

    per (n = ZEPHYRUM; n < statua->nodorum; n++)
    {
        si (statua->nodi[n].rete >= ZEPHYRUM)
        {
            nodi_retis++;
            tractus += statua->retia[statua->nodi[n].rete].partium;
        }
    }
    /* materiae novae post veteres: XII unilaterales, XII bilaterales */
    materiae = (StatuaMateria*)piscina_allocare(piscina,
        (memoriae_index)(statua->materiarum
            + II * PLASTICA_SPECIMEN_COLORES)
            * magnitudo(StatuaMateria));
    nodi     = (StatuaNodus*)piscina_allocare(piscina,
        (memoriae_index)(statua->nodorum + I) * magnitudo(StatuaNodus));
    retia    = (StatuaRete*)piscina_allocare(piscina,
        (memoriae_index)(nodi_retis + I) * magnitudo(StatuaRete));
    partes   = (StatuaPars*)piscina_allocare(piscina,
        (memoriae_index)(tractus + I) * magnitudo(StatuaPars));
    si (!materiae || !nodi || !retia || !partes)
    {
        redde FALSUM;
    }
    si (statua->materiarum)
    {
        memcpy(materiae, statua->materiae,
            (memoriae_index)statua->materiarum
                * magnitudo(StatuaMateria));
    }
    per (k = ZEPHYRUM; k < II * PLASTICA_SPECIMEN_COLORES; k++)
    {
        StatuaMateria* materia =
            &materiae[statua->materiarum + k];
        constans ColorPartis* color =
            &COLORES[k % PLASTICA_SPECIMEN_COLORES];

        memset(materia, ZEPHYRUM, magnitudo(StatuaMateria));
        materia->textura          = -I;
        materia->color[ZEPHYRUM]  = color->color[ZEPHYRUM];
        materia->color[I]         = color->color[I];
        materia->color[II]        = color->color[II];
        materia->color[III]       = 1.0f;
        materia->limen_alpha      = -1.0f;
        materia->bilateralis      = k >= PLASTICA_SPECIMEN_COLORES;
    }
    memcpy(nodi, statua->nodi,
        (memoriae_index)statua->nodorum * magnitudo(StatuaNodus));
    per (n = ZEPHYRUM; n < statua->nodorum; n++)
    {
        si (statua->nodi[n].rete < ZEPHYRUM)
        {
            perge;
        }
        rete                    = &statua->retia[statua->nodi[n].rete];
        retia[pars].pars_prima  = cursor;
        retia[pars].partium     = rete->partium;
        per (j = ZEPHYRUM; j < rete->partium; j++)
        {
            constans StatuaPars* vetus =
                &statua->partes[rete->pars_prima + j];
                            b32 bilateralis = FALSUM;

            si (vetus->materia >= ZEPHYRUM)
            {
                bilateralis =
                    statua->materiae[vetus->materia].bilateralis;
            }
            partes[cursor] = *vetus;
            si (!_tractus_invisus(statua, vetus))
            {
                partes[cursor].materia = (s32)(statua->materiarum
                    + pars % PLASTICA_SPECIMEN_COLORES
                    + (bilateralis ? PLASTICA_SPECIMEN_COLORES
                                   : ZEPHYRUM));
            }
            cursor++;
        }
        nodi[n].rete = (s32)pars;
        pars++;
    }
    *exemplar           = *statua;
    exemplar->nodi      = nodi;
    exemplar->retia     = retia;
    exemplar->retium    = nodi_retis;
    exemplar->partes    = partes;
    exemplar->partium   = tractus;
    exemplar->materiae  = materiae;
    exemplar->materiarum  = statua->materiarum
        + II * PLASTICA_SPECIMEN_COLORES;
    redde VERUM;
}

/* pixela tecta cellae in paginam; cetera charta manent. indices
 * NIHIL: colores picturae ipsi; aliter colores palettae */
interior vacuum
_cellam_ponere (
                           Imago* pagina,
                             i32  columna,
                             i32  ordo,
    constans StatuaScaenaPictura* pictura,
                     constans i8* indices)
{
    i32 x;
    i32 y;

    per (y = ZEPHYRUM; y < PLASTICA_SPECIMEN_CELLA; y++)
    {
        per (x = ZEPHYRUM; x < PLASTICA_SPECIMEN_CELLA; x++)
        {
            memoriae_index fons = (memoriae_index)y
                * PLASTICA_SPECIMEN_CELLA + x;
            memoriae_index linea = (memoriae_index)ordo
                * PLASTICA_SPECIMEN_CELLA + y;
                       i8* p    = pagina->pixela
                           + (linea
                           * pagina->latitudo
                           + columna * PLASTICA_SPECIMEN_CELLA
                           + x) * IV;

            si (!pictura->tectum[fons])
            {
                perge;
            }
            si (indices)
            {
                dithering_indices_ad_rgba(indices + fons, p, I, I);
            }
            alioquin
            {
                memcpy(p, pictura->rgba + fons * IV, III);
            }
            p[III] = 255;
        }
    }
}

/* Atkinson cellae totius super chartam palettae (error a charta
 * nullus); FALSUM si deficit */
interior b32
_atkinson (
    constans StatuaScaenaPictura*  pictura,
                    constans b32*  activi,
                              i8** indices,
                         Piscina*  piscina)
{
    DitheringFructus f;
      memoriae_index pixela = (memoriae_index)pictura->latitudo
          * pictura->altitudo;
      memoriae_index  k;
                  i8* rgba   = (i8*)piscina_allocare(piscina,
                      pixela * IV);

    si (!rgba)
    {
        redde FALSUM;
    }
    memcpy(rgba, pictura->rgba, pixela * IV);
    per (k = ZEPHYRUM; k < pixela; k++)
    {
        si (!pictura->tectum[k])
        {
            memcpy(rgba + k * IV, AQUINAS_PALETTE[CHARTA_PALETTAE],
                III);
        }
    }
    f = dithering_atkinson_colorum(rgba, pictura->latitudo,
        pictura->altitudo, activi, piscina);
    *indices = f.indices;
    redde f.successus && f.indices;
}

b32
plastica_specimen_pingere (
    constans Statua* statua,
              Imago* exitus,
            Piscina* piscina)
{
    StatuaScaenaPictura  pictura;
      StatuaScaenaVisus  visus;
           StatuaLocata  locata;
                 Statua  exemplar;
     StereometriaMatrix* matrices;
         memoriae_index  pixela;
                    b32  activi[XVI];
                    f32  radius;
                    f32  x;
                    f32  y;
                    f32  profunditas;
                     i8* indices;
                     i8* atkinson;
                    i32  columna;

    si (   !statua || !exitus || !piscina
        || statua->verticum == ZEPHYRUM)
    {
        redde FALSUM;
    }
    matrices = _positura_quietis(statua, piscina);
    pixela   = (memoriae_index)PLASTICA_SPECIMEN_LATITUDO
        * PLASTICA_SPECIMEN_ALTITUDO;
    exitus->latitudo  = PLASTICA_SPECIMEN_LATITUDO;
    exitus->altitudo  = PLASTICA_SPECIMEN_ALTITUDO;
    exitus->pixela    = (i8*)piscina_allocare(piscina, pixela * IV);
    indices           = (i8*)piscina_allocare(piscina,
        (memoriae_index)PLASTICA_SPECIMEN_CELLA
            * PLASTICA_SPECIMEN_CELLA);
    si (   !matrices || !exitus->pixela || !indices
        || !_exemplar_partium(statua, &exemplar, piscina))
    {
        exitus->pixela = NIHIL;
        redde FALSUM;
    }
    memset(exitus->pixela, 0xFF, pixela * IV);
    dithering_praeparare_omnes(activi);
    radius = statua->radius > 0.000001f ? statua->radius : 1.0f;
    memset(&visus, ZEPHYRUM, magnitudo(visus));
    visus.illuminatum          = VERUM;
    visus.pixela_per_unitatem  = (f32)PLASTICA_SPECIMEN_CELLA
        * (1.0f - 2.0f * SPECIMEN_MARGO) / (2.0f * radius);
    locata.matrices  = matrices;
    locata.locatio   = stereometria_identitas();
    per (columna = ZEPHYRUM; columna < ASPECTUUM; columna++)
    {
        visus.gyrus       = ASPECTUS[columna].gyrus;
        visus.inclinatio  = ASPECTUS[columna].inclinatio;
        /* centrum sphaerae in mediam cellam */
        statua_scaena_proicere(&visus, statua->centrum, &x, &y,
            &profunditas);
        visus.origo_x = (s32)floor((f64)x)
            - (s32)(PLASTICA_SPECIMEN_CELLA / II);
        visus.origo_y = (s32)floor((f64)y)
            - (s32)(PLASTICA_SPECIMEN_CELLA / II);
        locata.statua = statua;
        si (!statua_scaenam_pingere(&locata, I, &visus,
            PLASTICA_SPECIMEN_CELLA, PLASTICA_SPECIMEN_CELLA, &pictura,
            piscina))
        {
            redde FALSUM;
        }
        _cellam_ponere(exitus, columna, ZEPHYRUM, &pictura, NIHIL);
        memset(indices, CHARTA_PALETTAE,
            (memoriae_index)PLASTICA_SPECIMEN_CELLA
                * PLASTICA_SPECIMEN_CELLA);
        si (!statuae_picturam_dithering(pictura.rgba, pictura.tectum,
            PLASTICA_SPECIMEN_CELLA, PLASTICA_SPECIMEN_CELLA, activi,
            indices, piscina))
        {
            redde FALSUM;
        }
        _cellam_ponere(exitus, columna, I, &pictura, indices);
        si (!_atkinson(&pictura, activi, &atkinson, piscina))
        {
            redde FALSUM;
        }
        _cellam_ponere(exitus, columna, II, &pictura, atkinson);
        locata.statua = &exemplar;
        si (!statua_scaenam_pingere(&locata, I, &visus,
            PLASTICA_SPECIMEN_CELLA, PLASTICA_SPECIMEN_CELLA, &pictura,
            piscina))
        {
            redde FALSUM;
        }
        _cellam_ponere(exitus, columna, III, &pictura, NIHIL);
    }
    redde VERUM;
}


/* ==================================================
 * Linea motus
 * ================================================== */

/* magnitudo mundi: persona II unitates alta (PLASTICA_MOTUS_ALTITUDO
 * pixela), non plus III lata, XL pixela per unitatem (mundus.h,
 * mundus_pictura.h; hic iterum scripta - plastica mundum non
 * includit) */
#define MOTUS_LATITUDO_MAXIMA  120.0f
#define MOTUS_LATERIS_GYRUS    90.0f
/* finis cycli: statua_positura tempus per durationem involvit */
#define MOTUS_FINIS            0.9999f
#define MOTUS_CINEREUM         CLX

hic_manens constans i8 MOTUS_SIGNUM[III] = { 230, 30, 30 };

/* vertices triangulorum omnium retium (cum iteratione) */
interior i32
_vertices_numerare (
    constans Statua* statua)
{
    i32 numerus = ZEPHYRUM;
    i32 n;
    i32 j;

    per (n = ZEPHYRUM; n < statua->nodorum; n++)
    {
        constans StatuaRete* rete;

        si (statua->nodi[n].rete < ZEPHYRUM)
        {
            perge;
        }
        rete = &statua->retia[statua->nodi[n].rete];
        per (j = ZEPHYRUM; j < rete->partium; j++)
        {
            numerus += statua->partes[rete->pars_prima + j].indicum;
        }
    }
    redde numerus;
}

/* eosdem in spatio statuae ponere, III per verticem; redde y
 * infimum */
interior f32
_vertices_ponere (
                constans Statua* statua,
    constans StereometriaMatrix* matrices,
                            f32* exitus)
{
    f32 infimum = 0.0f;
    i32 numerus = ZEPHYRUM;
    i32 n;
    i32 j;
    i32 k;

    per (n = ZEPHYRUM; n < statua->nodorum; n++)
    {
        constans StatuaRete* rete;

        si (statua->nodi[n].rete < ZEPHYRUM)
        {
            perge;
        }
        rete = &statua->retia[statua->nodi[n].rete];
        per (j = ZEPHYRUM; j < rete->partium; j++)
        {
            constans StatuaPars* pars =
                &statua->partes[rete->pars_prima + j];

            per (k = ZEPHYRUM; k < pars->indicum; k++)
            {
                      constans f32* p = statua->positiones
                          + statua->indices[pars->index_primus + k]
                              * III;
                StereometriaVector v =
                    stereometria_punctum_transformare(matrices[n],
                        stereometria_vector(p[ZEPHYRUM], p[I], p[II]),
                        NIHIL);

                exitus[numerus * III]       = v.x;
                exitus[numerus * III + I]   = v.y;
                exitus[numerus * III + II]  = v.z;
                si (!numerus || v.y < infimum)
                {
                    infimum = v.y;
                }
                numerus++;
            }
        }
    }
    redde infimum;
}

b32
plastica_metiri_lapsum (
    constans Statua* statua,
                i32  cyclus,
       constans i32* fines,
                i32  finium,
                f32  celeritas,
                f32* lapsus,
            Piscina* piscina)
{
    StereometriaMatrix* matrices;
    StereometriaVector* loca;      /* (EXEMPLA + I) x finium */
                   i32* infimi;    /* index finis infimi per instans */
                   f32  passus;
                   f32  summa = 0.0f;
                   i32  k;
                   i32  j;

    si (lapsus)
    {
        *lapsus = 0.0f;
    }
    si (   !statua || !fines || !lapsus || !piscina
        || finium == ZEPHYRUM || cyclus >= statua->cyclorum)
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < finium; j++)
    {
        si (fines[j] >= statua->nodorum)
        {
            redde FALSUM;
        }
    }
    matrices = (StereometriaMatrix*)piscina_allocare(piscina,
        (memoriae_index)(statua->nodorum + I)
            * magnitudo(StereometriaMatrix));
    loca     = (StereometriaVector*)piscina_allocare(piscina,
        (memoriae_index)(PLASTICA_MOTUS_EXEMPLA + I) * finium
            * magnitudo(StereometriaVector));
    infimi   = (i32*)piscina_allocare(piscina,
        (memoriae_index)(PLASTICA_MOTUS_EXEMPLA + I) * magnitudo(i32));
    si (!matrices || !loca || !infimi)
    {
        redde FALSUM;
    }
    /* instans ultimum ad durationem ipsam: statua_positura tempus
     * involvit, ergo positura prima */
    per (k = ZEPHYRUM; k <= PLASTICA_MOTUS_EXEMPLA; k++)
    {
        StereometriaVector* ordo = loca + (memoriae_index)k * finium;

        statua_positura(statua, (s32)cyclus,
            statua->cycli[cyclus].duratio
                * (f32)k / (f32)PLASTICA_MOTUS_EXEMPLA, matrices);
        infimi[k] = ZEPHYRUM;
        per (j = ZEPHYRUM; j < finium; j++)
        {
            ordo[j] = stereometria_punctum_transformare(
                matrices[fines[j]],
                stereometria_vector(0.0f, 0.0f, 0.0f), NIHIL);
            si (ordo[j].y < ordo[infimi[k]].y)
            {
                infimi[k] = j;
            }
        }
    }
    /* motus debitus per intervallum: retro */
    passus = celeritas * statua->cycli[cyclus].duratio
        / (f32)PLASTICA_MOTUS_EXEMPLA;
    per (k = ZEPHYRUM; k < PLASTICA_MOTUS_EXEMPLA; k++)
    {
        constans StereometriaVector* a =
            loca + (memoriae_index)k * finium + infimi[k];
        constans StereometriaVector* b =
            loca + (memoriae_index)(k + I) * finium + infimi[k];
                                f32 x = b->x - a->x;
                                f32 z = b->z - a->z + passus;

        si (infimi[k] == infimi[k + I])
        {
            summa += (f32)sqrt((f64)(x * x + z * z));
        }
    }
    *lapsus = summa;
    redde VERUM;
}

b32
plastica_metiri_motum (
          constans Statua* statua,
                      i32  cyclus,
    PlasticaMensuraeMotus* mensurae,
                  Piscina* piscina)
{
    StereometriaMatrix* matrices;
                   f32* prima;
                   f32* currens;
                   f32  duratio;
                   f32  infimum;
                   f32  hiatus = 0.0f;
                   i32  numerus;
                   i32  k;

    si (mensurae)
    {
        memset(mensurae, ZEPHYRUM, magnitudo(*mensurae));
    }
    si (   !statua || !mensurae || !piscina
        || cyclus >= statua->cyclorum || statua->verticum == ZEPHYRUM)
    {
        redde FALSUM;
    }
    numerus = _vertices_numerare(statua);
    si (numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    matrices = (StereometriaMatrix*)piscina_allocare(piscina,
        (memoriae_index)(statua->nodorum + I)
            * magnitudo(StereometriaMatrix));
    prima    = (f32*)piscina_allocare(piscina,
        (memoriae_index)numerus * III * magnitudo(f32));
    currens  = (f32*)piscina_allocare(piscina,
        (memoriae_index)numerus * III * magnitudo(f32));
    si (!matrices || !prima || !currens)
    {
        redde FALSUM;
    }
    duratio = statua->cycli[cyclus].duratio;
    per (k = ZEPHYRUM; k < PLASTICA_MOTUS_EXEMPLA; k++)
    {
        statua_positura(statua, (s32)cyclus,
            duratio * (f32)k / (f32)PLASTICA_MOTUS_EXEMPLA, matrices);
        infimum = _vertices_ponere(statua, matrices, currens);
        si (!k)
        {
            memcpy(prima, currens,
                (memoriae_index)numerus * III * magnitudo(f32));
            mensurae->infimum_minimum = infimum;
            mensurae->infimum_maximum = infimum;
        }
        si (infimum < mensurae->infimum_minimum)
        {
            mensurae->infimum_minimum = infimum;
        }
        si (infimum > mensurae->infimum_maximum)
        {
            mensurae->infimum_maximum = infimum;
        }
    }
    statua_positura(statua, (s32)cyclus, duratio * MOTUS_FINIS,
        matrices);
    (vacuum)_vertices_ponere(statua, matrices, currens);
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        f32 x = currens[k * III] - prima[k * III];
        f32 y = currens[k * III + I] - prima[k * III + I];
        f32 z = currens[k * III + II] - prima[k * III + II];
        f32 d = (f32)sqrt((f64)(x * x + y * y + z * z));

        si (d > hiatus)
        {
            hiatus = d;
        }
    }
    mensurae->hiatus_suturae = hiatus;
    redde VERUM;
}

/* pixelum cellae in imagine */
interior i8*
_pixelum_cellae (
    Imago* imago,
      i32  columna,
      i32  ordo,
      i32  x,
      i32  y)
{
    redde imago->pixela
        + ((memoriae_index)(ordo * PLASTICA_MOTUS_CELLA + y)
            * imago->latitudo
            + columna * PLASTICA_MOTUS_CELLA_LATA + x) * IV;
}

/* pixela tecta picturae in cellam; indices NIHIL = colores ipsi */
interior vacuum
_cellam_motus_ponere (
                           Imago* imago,
                             i32  columna,
                             i32  ordo,
    constans StatuaScaenaPictura* pictura,
                     constans i8* indices)
{
    i32 x;
    i32 y;

    per (y = ZEPHYRUM; y < PLASTICA_MOTUS_CELLA; y++)
    {
        per (x = ZEPHYRUM; x < PLASTICA_MOTUS_CELLA_LATA; x++)
        {
            memoriae_index fons = (memoriae_index)y
                * PLASTICA_MOTUS_CELLA_LATA + x;
                       i8* p    = _pixelum_cellae(imago, columna, ordo,
                           x, y);

            si (!pictura->tectum[fons])
            {
                perge;
            }
            si (indices)
            {
                dithering_indices_ad_rgba(indices + fons, p, I, I);
            }
            alioquin
            {
                memcpy(p, pictura->rgba + fons * IV, III);
            }
        }
    }
}

/* cardines articulorum: quadratum III x III rubrum, intra cellam */
interior vacuum
_cardines_signare (
                          Imago* imago,
                            i32  columna,
                            i32  ordo,
     constans StatuaScaenaVisus* visus,
    constans StereometriaMatrix* matrices,
                   constans i32* articuli,
                            i32  articulorum)
{
    i32 j;
    s32 dx;
    s32 dy;

    per (j = ZEPHYRUM; j < articulorum; j++)
    {
        StereometriaVector cardo = stereometria_punctum_transformare(
            matrices[articuli[j]],
            stereometria_vector(0.0f, 0.0f, 0.0f), NIHIL);
                       f32 x;
                       f32 y;
                       f32 profunditas;
                       s32 px;
                       s32 py;

        statua_scaena_proicere(visus, cardo, &x, &y, &profunditas);
        px = (s32)floor((f64)x) - visus->origo_x;
        py = (s32)floor((f64)y) - visus->origo_y;
        per (dy = -I; dy <= I; dy++)
        {
            per (dx = -I; dx <= I; dx++)
            {
                si (   px + dx < ZEPHYRUM || py + dy < ZEPHYRUM
                    || px + dx >= (s32)PLASTICA_MOTUS_CELLA_LATA
                    || py + dy >= (s32)PLASTICA_MOTUS_CELLA)
                {
                    perge;
                }
                memcpy(_pixelum_cellae(imago, columna, ordo,
                    (i32)(px + dx), (i32)(py + dy)), MOTUS_SIGNUM, III);
            }
        }
    }
}

b32
plastica_specimen_motus_pingere (
    constans Statua* statua,
       constans i32* articuli,
                i32  articulorum,
              Imago* exitus,
            Piscina* piscina)
{
    StatuaScaenaPictura  pictura;
      StatuaScaenaVisus  visus;
           StatuaLocata  locata;
       PlasticaMensurae  quies;
     StereometriaVector  medium;
     StereometriaMatrix* matrices;
         memoriae_index  pixela;
                    b32  activi[XVI];
                    f32  altitudo;
                    f32  latitudo;
                    f32  scala = (f32)PLASTICA_MOTUS_ALTITUDO;
                    f32  x;
                    f32  y;
                    f32  profunditas;
                     i8* indices;
                    i32  cyclus;
                    i32  latus;
                    i32  k;
                    i32  n;

    si (   !statua || !exitus || !piscina
        || statua->verticum == ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* sine cyclis imago nullius altitudinis est: allocatio nulla
     * infra FALSUM reddit */
    quies     = plastica_metiri(statua, piscina);
    altitudo  = quies.maximum.y - quies.minimum.y;
    latitudo  = quies.maximum.x - quies.minimum.x;
    si (quies.maximum.z - quies.minimum.z > latitudo)
    {
        latitudo = quies.maximum.z - quies.minimum.z;
    }
    /* scala: pixela per unitatem figurae */
    si (altitudo > 0.000001f)
    {
        scala = (f32)PLASTICA_MOTUS_ALTITUDO / altitudo;
    }
    si (latitudo * scala > MOTUS_LATITUDO_MAXIMA)
    {
        scala = MOTUS_LATITUDO_MAXIMA / latitudo;
    }
    medium = stereometria_vector(
        (quies.minimum.x + quies.maximum.x) * 0.5f, 0.0f,
        (quies.minimum.z + quies.maximum.z) * 0.5f);
    exitus->latitudo = PLASTICA_MOTUS_LATITUDO;
    exitus->altitudo = statua->cyclorum * II * PLASTICA_MOTUS_CELLA;
    pixela = (memoriae_index)exitus->latitudo * exitus->altitudo;
    exitus->pixela = (i8*)piscina_allocare(piscina, pixela * IV);
    indices           = (i8*)piscina_allocare(piscina,
        (memoriae_index)PLASTICA_MOTUS_CELLA_LATA
            * PLASTICA_MOTUS_CELLA);
    matrices          = (StereometriaMatrix*)piscina_allocare(piscina,
        (memoriae_index)(statua->nodorum + I)
            * magnitudo(StereometriaMatrix));
    si (!exitus->pixela || !indices || !matrices)
    {
        exitus->pixela = NIHIL;
        redde FALSUM;
    }
    memset(exitus->pixela, 0xFF, pixela * IV);
    dithering_praeparare_omnes(activi);
    memset(&visus, ZEPHYRUM, magnitudo(visus));
    visus.illuminatum          = VERUM;
    visus.pixela_per_unitatem  = scala;
    locata.statua              = statua;
    locata.matrices            = matrices;
    locata.locatio             = stereometria_identitas();
    per (cyclus = ZEPHYRUM; cyclus < statua->cyclorum; cyclus++)
    {
        per (latus = ZEPHYRUM; latus < II; latus++)
        {
            i32 ordo = cyclus * II + latus;

            visus.gyrus       = latus ? PLASTICA_SPECIMEN_GYRUS
                : MOTUS_LATERIS_GYRUS;
            visus.inclinatio  = latus ? PLASTICA_SPECIMEN_INCLINATIO
                : 0.0f;
            /* punctum humi sub medio figurae ad (medium, HUMUS) */
            statua_scaena_proicere(&visus, medium, &x, &y,
                &profunditas);
            visus.origo_x = (s32)floor((f64)x)
                - (s32)(PLASTICA_MOTUS_CELLA_LATA / II);
            visus.origo_y = (s32)floor((f64)y)
                - (s32)PLASTICA_MOTUS_HUMUS;
            per (k = ZEPHYRUM; k < PLASTICA_MOTUS_QUADRA; k++)
            {
                PiscinaNotatio nota = piscina_notare(piscina);
                           b32 bene;

                statua_positura(statua, (s32)cyclus,
                    statua->cycli[cyclus].duratio * (f32)k
                        / (f32)PLASTICA_MOTUS_QUADRA, matrices);
                bene = statua_scaenam_pingere(&locata, I, &visus,
                    PLASTICA_MOTUS_CELLA_LATA, PLASTICA_MOTUS_CELLA,
                    &pictura, piscina);
                si (bene && latus)
                {
                    memset(indices, CHARTA_PALETTAE,
                        (memoriae_index)PLASTICA_MOTUS_CELLA_LATA
                            * PLASTICA_MOTUS_CELLA);
                    bene = statuae_picturam_dithering(pictura.rgba,
                        pictura.tectum, PLASTICA_MOTUS_CELLA_LATA,
                        PLASTICA_MOTUS_CELLA, activi, indices,
                        piscina);
                }
                si (bene && latus)
                {
                    _cellam_motus_ponere(exitus, k, ordo, &pictura,
                        indices);
                }
                alioquin si (bene)
                {
                    /* linea humi sub figura, signa super eam */
                    per (n = ZEPHYRUM; n
                        < PLASTICA_MOTUS_CELLA_LATA; n++)
                    {
                        memset(_pixelum_cellae(exitus, k, ordo, n,
                            PLASTICA_MOTUS_HUMUS), MOTUS_CINEREUM,
                            III);
                    }
                    _cellam_motus_ponere(exitus, k, ordo, &pictura,
                        NIHIL);
                    _cardines_signare(exitus, k, ordo, &visus,
                        matrices, articuli, articulorum);
                }
                piscina_reficere(piscina, nota);
                si (!bene)
                {
                    exitus->pixela = NIHIL;
                    redde FALSUM;
                }
            }
        }
    }
    redde VERUM;
}
