/* statua_glb.c - lector GLB in statuam (vide statua_glb.h)
 *
 * Ordo: continens (caput, pars JSON, pars BIN) -> JSON -> recusationes
 * summae (extensiones, pelles, plagulae externae) -> materiae et
 * texturae -> retia -> nodi -> scaena -> cycli -> statua_finire.
 * Omnis numerus ex JSON per _numerus transit (glTF '1' et '1.0'
 * promiscue scribit); omnis locus in BIN per _accessorem_solvere,
 * qui fines probat. Error primus vincit.
 *
 * Octeti parvo-fine leguntur ut scripti (machinae domus parvo-fine).
 */

#include "statua_glb.h"
#include "json.h"
#include "imago.h"
#include "stereometria.h"
#include <string.h>

#define GLB_MAGICA   0x46546C67u   /* "glTF" */
#define GLB_PARS_JSON 0x4E4F534Au  /* "JSON" */
#define GLB_PARS_BIN  0x004E4942u  /* "BIN\0" */

/* genera partium accessoris (glTF componentType) */
#define GENUS_S8    5120
#define GENUS_I8    5121
#define GENUS_S16   5122
#define GENUS_I16   5123
#define GENUS_I32   5125
#define GENUS_F32   5126

/* involutio texturae (glTF wrap) */
#define INVOLUTIO_MARGO     33071
#define INVOLUTIO_SPECULUM  33648

/* limites: plagula hostilis allocationem immanem ne cogat */
#define LIMES_NODORUM     0x10000
#define LIMES_VERTICUM    1000000
#define LIMES_INDICUM     3000000
#define LIMES_CLAVIUM     1000000
#define LIMES_IMAGINUM    CCLVI
#define LIMES_TEXTURAE    0x1000

nomen structura {
               Piscina* piscina;
    constans character* error;
             JsonValor* radix;
           constans i8* bin;
                   i32  bin_mensura;
                Statua* s;
                   s32* textura_imaginis;  /* per imaginem: index in
                                            * texturas statuae, -1 =
                                            * nondum decodificata */
                   i32 imaginum;
                   i32 clavium;           /* summa, contra limitem */
    /* iuxta statuam: nomina nodorum, momenta cyclorum */
            chorda* tituli;
    StatuaMomentum* momenta;
               i32  momentorum;
} Lector;

nomen structura {
    constans i8* datum;
            i32  numerus;
            i32  gradus;        /* octeti inter elementa */
            i32  genus;
            i32  partes;        /* I..IV */
            b32  normalizatus;
} Accessor;


/* ==================================================
 * Auxilia
 * ================================================== */

interior b32
_recusare (
                Lector* l,
    constans character* causa)
{
    si (!l->error)
    {
        l->error = causa;
    }
    redde FALSUM;
}

interior vacuum*
_locus (
            Lector* l,
    memoriae_index  mensura)
{
    vacuum* p = piscina_allocare(l->piscina,
        mensura ? mensura : I);

    memset(p, ZEPHYRUM, mensura);
    redde p;
}

interior b32
_finitus (
    f32 x)
{
    redde x == x && (x - x) == 0.0f;
}

interior i32
_u32_legere (
    constans i8* p)
{
    redde (i32)p[ZEPHYRUM] | ((i32)p[I] << VIII) | ((i32)p[II] << XVI)
        | ((i32)p[III] << XXIV);
}

/* tabulatum sub clave, aut NIHIL */
interior JsonValor*
_tabulatum (
             JsonValor* objectum,
    constans character* clavis)
{
    JsonValor* v = json_objectum_capere(objectum, clavis);

    redde v && json_est_tabulatum(v) ? v : NIHIL;
}

interior i32
_numerus_in (
    JsonValor* tabulatum)
{
    redde tabulatum ? json_tabulatum_numerus(tabulatum) : ZEPHYRUM;
}

/* numerus JSON, integer aut fluitans */
interior b32
_numerus (
    JsonValor* v,
          f64* exitus)
{
    si (v && json_est_integer(v))
    {
        *exitus = (f64)json_ad_integer(v);
        redde VERUM;
    }
    si (v && json_est_fluitans(v))
    {
        *exitus = json_ad_fluitans(v);
        redde VERUM;
    }
    redde FALSUM;
}

/* numerus integer in [0, limes) */
interior b32
_naturalis (
    JsonValor* v,
          i32  limes,
          i32* exitus)
{
    f64 x;

    si (   !_numerus(v, &x) || !(x >= 0.0) || !(x < (f64)limes)
        || (f64)(i32)x != x)
    {
        redde FALSUM;
    }
    *exitus = (i32)x;
    redde VERUM;
}

/* n numeri finiti sub clave in exitum; clavis absens: exitus manet,
 * VERUM. Forma mala: FALSUM */
interior b32
_numeros (
             JsonValor* objectum,
    constans character* clavis,
                   i32  n,
                   f32* exitus)
{
    JsonValor* v = json_objectum_capere(objectum, clavis);
          f64  x;
          f32  lecti[XVI];
          i32  k;

    si (!v)
    {
        redde VERUM;
    }
    si (!json_est_tabulatum(v) || json_tabulatum_numerus(v) != n)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (   !_numerus(json_tabulatum_obtinere(v, k), &x)
            || !_finitus((f32)x))
        {
            redde FALSUM;
        }
        lecti[k] = (f32)x;
    }
    memcpy(exitus, lecti, n * magnitudo(f32));
    redde VERUM;
}

interior b32
_chorda_est (
             JsonValor* objectum,
    constans character* clavis,
    constans character* valor)
{
    JsonValor* v = json_objectum_capere(objectum, clavis);

    redde v && json_est_chorda(v)
        && chorda_aequalis_literis(json_ad_chorda(v), valor);
}


/* ==================================================
 * Accessores
 * ================================================== */

interior i32
_generis_mensura (
    i32 genus)
{
    commutatio (genus)
    {
        casus GENUS_S8:
        casus GENUS_I8:
            redde I;
        casus GENUS_S16:
        casus GENUS_I16:
            redde II;
        casus GENUS_I32:
        casus GENUS_F32:
            redde IV;
        ordinarius:
            redde ZEPHYRUM;
    }
}

/* accessor per indicem JSON solutus, finibus contra BIN probatis */
interior b32
_accessorem_solvere (
       Lector* l,
    JsonValor* index,
     Accessor* a)
{
    JsonValor* accessores = _tabulatum(l->radix, "accessors");
    JsonValor* prospectus = _tabulatum(l->radix, "bufferViews");
    JsonValor* j;
    JsonValor* p;
          i32  n;
          i32  plagula    = ZEPHYRUM;
          i32  p_initium  = ZEPHYRUM;
          i32  p_mensura  = ZEPHYRUM;
          i32  a_initium  = ZEPHYRUM;
          i32  elementum;
          i64  finis;

    si (!_naturalis(index, _numerus_in(accessores), &n))
    {
        redde _recusare(l, "the file refers to data that is not "
            "there (accessor index out of range)");
    }
    j = json_tabulatum_obtinere(accessores, n);
    si (json_objectum_capere(j, "sparse"))
    {
        redde _recusare(l, "this model uses sparse data, which is "
            "not supported");
    }
    si (   !_naturalis(json_objectum_capere(j, "bufferView"),
               _numerus_in(prospectus), &n)
        || !_naturalis(json_objectum_capere(j, "componentType"),
               0x10000, &a->genus)
        || !_naturalis(json_objectum_capere(j, "count"), 0x7FFFFFFF,
               &a->numerus))
    {
        redde _recusare(l, "the file's data description is broken "
            "(accessor)");
    }
    si (   json_objectum_capere(j, "byteOffset")
        && !_naturalis(json_objectum_capere(j, "byteOffset"),
               0x7FFFFFFF, &a_initium))
    {
        redde _recusare(l, "the file's data description is broken "
            "(accessor offset)");
    }
    si (_chorda_est(j, "type", "SCALAR"))
    {
        a->partes = I;
    }
    alioquin si (_chorda_est(j, "type", "VEC2"))
    {
        a->partes = II;
    }
    alioquin si (_chorda_est(j, "type", "VEC3"))
    {
        a->partes = III;
    }
    alioquin si (_chorda_est(j, "type", "VEC4"))
    {
        a->partes = IV;
    }
    alioquin
    {
        redde _recusare(l, "the file uses a data shape that is not "
            "supported (accessor type)");
    }
    elementum = _generis_mensura(a->genus) * a->partes;
    si (elementum == ZEPHYRUM)
    {
        redde _recusare(l, "the file uses a number format that is "
            "not supported (accessor component type)");
    }
    a->normalizatus  = json_capere_boolean(j, "normalized", FALSUM);
    p                = json_tabulatum_obtinere(prospectus, n);
    a->gradus        = elementum;
    si (   (   json_objectum_capere(p, "buffer")
            && !_naturalis(json_objectum_capere(p, "buffer"), II,
                   &plagula))
        || plagula != ZEPHYRUM
        || (   json_objectum_capere(p, "byteOffset")
            && !_naturalis(json_objectum_capere(p, "byteOffset"),
                   0x7FFFFFFF, &p_initium))
        || !_naturalis(json_objectum_capere(p, "byteLength"),
               0x7FFFFFFF, &p_mensura)
        || (   json_objectum_capere(p, "byteStride")
            && !_naturalis(json_objectum_capere(p, "byteStride"),
                   0x100, &a->gradus)))
    {
        redde _recusare(l, "the file's data description is broken "
            "(buffer view)");
    }
    si (a->gradus == ZEPHYRUM)
    {
        a->gradus = elementum;
    }
    finis = (i64)a_initium;
    si (a->numerus > ZEPHYRUM)
    {
        finis += (i64)(a->numerus - I) * (i64)a->gradus
            + (i64)elementum;
    }
    si (   a->gradus < elementum
        || (i64)p_initium + (i64)p_mensura > (i64)l->bin_mensura
        || finis > (i64)p_mensura)
    {
        redde _recusare(l, "the file is damaged: its data runs past "
            "the end of the file");
    }
    a->datum = l->bin + p_initium + a_initium;
    redde VERUM;
}

/* pars ut integer non signatus (indices) */
interior i32
_integrum_legere (
    constans Accessor* a,
                  i32  elementum)
{
    constans i8* p = a->datum + (memoriae_index)elementum * a->gradus;

    commutatio (a->genus)
    {
        casus GENUS_I8:
            redde (i32)p[ZEPHYRUM];
        casus GENUS_I16:
            redde (i32)p[ZEPHYRUM] | ((i32)p[I] << VIII);
        ordinarius:
            redde _u32_legere(p);
    }
}

/* pars ut f32: fluitans, aut integer non signatus normalizatus */
interior f32
_partem_legere (
    constans Accessor* a,
                  i32  elementum,
                  i32  pars)
{
    constans i8* p = a->datum + (memoriae_index)elementum * a->gradus;
            f32  f;

    commutatio (a->genus)
    {
        casus GENUS_F32:
            memcpy(&f, p + pars * IV, IV);
            redde f;
        casus GENUS_I8:
            redde (f32)p[pars] / 255.0f;
        ordinarius:
            redde (f32)((i32)p[pars * II]
                | ((i32)p[pars * II + I] << VIII)) / 65535.0f;
    }
}


/* ==================================================
 * Materiae et texturae
 * ================================================== */

/* textura statuae pro imagine glTF (decodificata semel) */
interior b32
_texturam_imaginis (
    Lector* l,
       i32  imago,
       s32* exitus)
{
       JsonValor* imagines = _tabulatum(l->radix, "images");
       JsonValor* j = json_tabulatum_obtinere(imagines, imago);
       JsonValor* prospectus = _tabulatum(l->radix, "bufferViews");
       JsonValor* p;
    ImagoFructus  f;
             i32  n;
             i32  initium = ZEPHYRUM;
             i32  mensura = ZEPHYRUM;
             i32  plagula = ZEPHYRUM;
             i32  latitudo;
             i32  altitudo;

    si (l->textura_imaginis[imago] >= ZEPHYRUM)
    {
        *exitus = l->textura_imaginis[imago];
        redde VERUM;
    }
    si (!_naturalis(json_objectum_capere(j, "bufferView"),
        _numerus_in(prospectus), &n))
    {
        redde _recusare(l, "the file's texture image is missing");
    }
    p = json_tabulatum_obtinere(prospectus, n);
    si (   (   json_objectum_capere(p, "buffer")
            && !_naturalis(json_objectum_capere(p, "buffer"), II,
                   &plagula))
        || plagula != ZEPHYRUM
        || (   json_objectum_capere(p, "byteOffset")
            && !_naturalis(json_objectum_capere(p, "byteOffset"),
                   0x7FFFFFFF, &initium))
        || !_naturalis(json_objectum_capere(p, "byteLength"),
               0x7FFFFFFF, &mensura)
        || (i64)initium + (i64)mensura > (i64)l->bin_mensura)
    {
        redde _recusare(l, "the file is damaged: its texture image "
            "runs past the end of the file");
    }
    si (   !imago_mensuras_ex_memoria(l->bin + initium, mensura,
               &latitudo, &altitudo)
        || latitudo < I || altitudo < I)
    {
        redde _recusare(l, "the texture image could not be read "
            "(PNG and JPEG are supported)");
    }
    si (latitudo > LIMES_TEXTURAE || altitudo > LIMES_TEXTURAE)
    {
        redde _recusare(l, "the texture image is too large (the "
            "limit is 4096 pixels a side)");
    }
    f = imago_caricare_ex_memoria(l->bin + initium, mensura,
        l->piscina);
    si (   !f.successus || !f.imago.pixela || f.imago.latitudo < I
        || f.imago.altitudo < I)
    {
        redde _recusare(l, "the texture image could not be read "
            "(PNG and JPEG are supported)");
    }
    l->s->texturae[l->s->texturarum]  = f.imago;
    l->textura_imaginis[imago]        = (s32)l->s->texturarum;
    *exitus                           = (s32)l->s->texturarum;
    l->s->texturarum++;
    redde VERUM;
}

interior b32
_materiam_legere (
           Lector* l,
        JsonValor* j,
    StatuaMateria* m)
{
    JsonValor* basis = json_objectum_capere(j, "pbrMetallicRoughness");
    JsonValor* nexus = json_objectum_capere(basis, "baseColorTexture");
    JsonValor* texturae = _tabulatum(l->radix, "textures");
    JsonValor* moduli = _tabulatum(l->radix, "samplers");
    JsonValor* t;
    JsonValor* modulus;
          f64  x;
          f32  emissio[III];
          i32  n;
          i32  imago;
          i32  involutio;

    m->textura          = -I;
    m->color[ZEPHYRUM]  = 1.0f;
    m->color[I]         = 1.0f;
    m->color[II]        = 1.0f;
    m->color[III]       = 1.0f;
    m->limen_alpha      = -1.0f;
    emissio[ZEPHYRUM]   = 0.0f;
    emissio[I]          = 0.0f;
    emissio[II]         = 0.0f;
    si (   !_numeros(basis, "baseColorFactor", IV, m->color)
        || !_numeros(j, "emissiveFactor", III, emissio))
    {
        redde _recusare(l, "a material in the file is broken");
    }
    m->emissiva = emissio[ZEPHYRUM] > 0.0f || emissio[I] > 0.0f
        || emissio[II] > 0.0f;
    m->bilateralis = json_capere_boolean(j, "doubleSided", FALSUM);
    si (_chorda_est(j, "alphaMode", "MASK"))
    {
        m->limen_alpha = 0.5f;
        si (_numerus(json_objectum_capere(j, "alphaCutoff"), &x))
        {
            si (!_finitus((f32)x))
            {
                redde _recusare(l, "a material in the file is "
                    "broken");
            }
            m->limen_alpha = (f32)x < 0.0f ? 0.0f : (f32)x;
        }
    }
    alioquin si (_chorda_est(j, "alphaMode", "BLEND"))
    {
        m->limen_alpha = 0.5f;
    }
    si (!nexus)
    {
        redde VERUM;
    }
    si (!_naturalis(json_objectum_capere(nexus, "index"),
        _numerus_in(texturae), &n))
    {
        redde _recusare(l, "a material refers to a texture that is "
            "not in the file");
    }
    t = json_tabulatum_obtinere(texturae, n);
    /* glTF: sine modulo repetitio ordinaria est */
    m->repetit_s = VERUM;
    m->repetit_t = VERUM;
    si (json_objectum_capere(t, "sampler"))
    {
        si (!_naturalis(json_objectum_capere(t, "sampler"),
            _numerus_in(moduli), &n))
        {
            redde _recusare(l, "a texture refers to a sampler that "
                "is not in the file");
        }
        modulus = json_tabulatum_obtinere(moduli, n);
        si (_naturalis(json_objectum_capere(modulus, "wrapS"),
            0x10000, &involutio))
        {
            m->repetit_s   = involutio != INVOLUTIO_MARGO;
            m->speculat_s  = involutio == INVOLUTIO_SPECULUM;
        }
        si (_naturalis(json_objectum_capere(modulus, "wrapT"),
            0x10000, &involutio))
        {
            m->repetit_t   = involutio != INVOLUTIO_MARGO;
            m->speculat_t  = involutio == INVOLUTIO_SPECULUM;
        }
    }
    si (!json_objectum_capere(t, "source"))
    {
        redde VERUM;
    }
    si (!_naturalis(json_objectum_capere(t, "source"), l->imaginum,
        &imago))
    {
        redde _recusare(l, "a texture refers to an image that is "
            "not in the file");
    }
    redde _texturam_imaginis(l, imago, &m->textura);
}

interior b32
_materias_legere (
    Lector* l)
{
    JsonValor* materiae  = _tabulatum(l->radix, "materials");
       Statua* s         = l->s;
          i32  n;

    l->imaginum = _numerus_in(_tabulatum(l->radix, "images"));
    si (l->imaginum > LIMES_IMAGINUM)
    {
        redde _recusare(l, "the file has too many images");
    }
    l->textura_imaginis = (s32*)_locus(l,
        l->imaginum * magnitudo(s32));
    per (n = ZEPHYRUM; n < l->imaginum; n++)
    {
        l->textura_imaginis[n] = -I;
    }
    s->texturae    = (Imago*)_locus(l, l->imaginum * magnitudo(Imago));
    s->materiarum  = _numerus_in(materiae);
    si (s->materiarum > LIMES_NODORUM)
    {
        redde _recusare(l, "the file has too many materials");
    }
    s->materiae = (StatuaMateria*)_locus(l,
        s->materiarum * magnitudo(StatuaMateria));
    per (n = ZEPHYRUM; n < s->materiarum; n++)
    {
        si (!_materiam_legere(l, json_tabulatum_obtinere(materiae, n),
            &s->materiae[n]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Retia
 * ================================================== */

nomen structura {
    Accessor positiones;
    Accessor normales;
    Accessor coordinatae;
    Accessor indices;
         b32 habet_normales;
         b32 habet_coordinatas;
         b32 habet_indices;
         i32 indicum;          /* multiplum III */
         s32 materia;
} Primitivum;

interior b32
_primitivum_solvere (
        Lector* l,
     JsonValor* j,
    Primitivum* p)
{
    JsonValor* attributa  = json_objectum_capere(j, "attributes");
          i32  modus      = IV;
          i32  materia;

    memset(p, ZEPHYRUM, magnitudo(Primitivum));
    p->materia = -I;
    si (   json_objectum_capere(j, "mode")
        && (   !_naturalis(json_objectum_capere(j, "mode"), 0x100,
                   &modus)
            || modus != IV))
    {
        redde _recusare(l, "this model has points, lines or "
            "triangle strips; only plain triangles are supported");
    }
    si (_numerus_in(_tabulatum(j, "targets")) > ZEPHYRUM)
    {
        redde _recusare(l, "this model uses morph targets, which "
            "are not supported");
    }
    si (!json_objectum_capere(attributa, "POSITION"))
    {
        redde _recusare(l, "a mesh in the file has no vertex "
            "positions");
    }
    si (!_accessorem_solvere(l,
        json_objectum_capere(attributa, "POSITION"), &p->positiones))
    {
        redde FALSUM;
    }
    si (   p->positiones.genus  != GENUS_F32
        || p->positiones.partes != III)
    {
        redde _recusare(l, "a mesh stores its vertex positions in a "
            "format that is not supported");
    }
    si (json_objectum_capere(attributa, "NORMAL"))
    {
        si (!_accessorem_solvere(l,
            json_objectum_capere(attributa, "NORMAL"), &p->normales))
        {
            redde FALSUM;
        }
        p->habet_normales = p->normales.genus == GENUS_F32
            && p->normales.partes == III
            && p->normales.numerus == p->positiones.numerus;
    }
    si (json_objectum_capere(attributa, "TEXCOORD_0"))
    {
        si (!_accessorem_solvere(l,
            json_objectum_capere(attributa, "TEXCOORD_0"),
            &p->coordinatae))
        {
            redde FALSUM;
        }
        p->habet_coordinatas = p->coordinatae.partes == II
            && p->coordinatae.numerus == p->positiones.numerus
            && (   p->coordinatae.genus == GENUS_F32
                || (   p->coordinatae.normalizatus
                    && (   p->coordinatae.genus == GENUS_I8
                        || p->coordinatae.genus == GENUS_I16)));
    }
    p->indicum = p->positiones.numerus;
    si (json_objectum_capere(j, "indices"))
    {
        si (!_accessorem_solvere(l, json_objectum_capere(j, "indices"),
            &p->indices))
        {
            redde FALSUM;
        }
        si (   p->indices.partes != I
            || (   p->indices.genus != GENUS_I8
                && p->indices.genus != GENUS_I16
                && p->indices.genus != GENUS_I32))
        {
            redde _recusare(l, "a mesh stores its triangle indices "
                "in a format that is not supported");
        }
        p->habet_indices  = VERUM;
        p->indicum        = p->indices.numerus;
    }
    p->indicum -= p->indicum % III;
    si (json_objectum_capere(j, "material"))
    {
        si (!_naturalis(json_objectum_capere(j, "material"),
            l->s->materiarum, &materia))
        {
            redde _recusare(l, "a mesh refers to a material that is "
                "not in the file");
        }
        p->materia = (s32)materia;
    }
    redde VERUM;
}

/* normales absentes: summa normalium facierum per verticem */
interior vacuum
_normales_computare (
    Statua* s,
       i32  vertex_primus,
       i32  verticum,
       i32  index_primus,
       i32  indicum)
{
    StereometriaVector a;
    StereometriaVector b;
    StereometriaVector d;
    StereometriaVector n;
                   i32 k;
                   i32 j;
                   i32 v[III];

    per (k = ZEPHYRUM; k + II < indicum; k += III)
    {
        per (j = ZEPHYRUM; j < III; j++)
        {
            v[j] = s->indices[index_primus + k + j];
        }
        a = stereometria_vector(s->positiones[v[ZEPHYRUM] * III],
            s->positiones[v[ZEPHYRUM] * III + I],
            s->positiones[v[ZEPHYRUM] * III + II]);
        b = stereometria_vector(s->positiones[v[I] * III],
            s->positiones[v[I] * III + I],
            s->positiones[v[I] * III + II]);
        d = stereometria_vector(s->positiones[v[II] * III],
            s->positiones[v[II] * III + I],
            s->positiones[v[II] * III + II]);
        n = stereometria_crux(stereometria_subtrahere(b, a),
            stereometria_subtrahere(d, a));
        per (j = ZEPHYRUM; j < III; j++)
        {
            s->normales[v[j] * III]       += n.x;
            s->normales[v[j] * III + I]   += n.y;
            s->normales[v[j] * III + II]  += n.z;
        }
    }
    per (k = vertex_primus; k < vertex_primus + verticum; k++)
    {
        n = stereometria_normalizare(stereometria_vector(
            s->normales[k * III], s->normales[k * III + I],
            s->normales[k * III + II]));
        s->normales[k * III]       = n.x;
        s->normales[k * III + I]   = n.y;
        s->normales[k * III + II]  = n.z;
    }
}

/* primitivum in tabulas statuae copiare (loci iam allocati) */
interior b32
_primitivum_copiare (
                 Lector* l,
    constans Primitivum* p,
             StatuaPars* pars)
{
    Statua* s       = l->s;
       i32  basis   = s->verticum;
       i32  primus  = s->indicum;
       i32  n;
       i32  k;
       i32  index;
       f32  x;

    per (n = ZEPHYRUM; n < p->positiones.numerus; n++)
    {
        per (k = ZEPHYRUM; k < III; k++)
        {
            x = _partem_legere(&p->positiones, n, k);
            si (!_finitus(x))
            {
                redde _recusare(l, "the file is damaged: a vertex "
                    "position is not a number");
            }
            s->positiones[(basis + n) * III + k] = x;
            si (p->habet_normales)
            {
                x = _partem_legere(&p->normales, n, k);
                s->normales[(basis + n) * III + k] = _finitus(x) ? x
                                                                 : 0.0f;
            }
        }
        per (k = ZEPHYRUM; k < II && p->habet_coordinatas; k++)
        {
            x = _partem_legere(&p->coordinatae, n, k);
            s->coordinatae[(basis + n) * II + k] = _finitus(x) ? x
                                                               : 0.0f;
        }
    }
    per (n = ZEPHYRUM; n < p->indicum; n++)
    {
        index = p->habet_indices ? _integrum_legere(&p->indices, n) : n;
        si (index >= p->positiones.numerus)
        {
            redde _recusare(l, "the file is damaged: a triangle "
                "refers to a vertex that is not there");
        }
        s->indices[primus + n] = basis + index;
    }
    s->verticum         += p->positiones.numerus;
    s->indicum          += p->indicum;
    pars->index_primus  = primus;
    pars->indicum       = p->indicum;
    pars->materia       = p->materia;
    si (!p->habet_normales)
    {
        _normales_computare(s, basis, p->positiones.numerus, primus,
            p->indicum);
    }
    redde VERUM;
}

interior b32
_retia_legere (
    Lector* l)
{
     JsonValor* retia = _tabulatum(l->radix, "meshes");
     JsonValor* primitiva;
        Statua* s     = l->s;
    Primitivum  p;
           i32  n;
           i32  k;
           i32  partium   = ZEPHYRUM;
           i64  verticum  = ZEPHYRUM;
           i64  indicum   = ZEPHYRUM;

    s->retium = _numerus_in(retia);
    si (s->retium > LIMES_NODORUM)
    {
        redde _recusare(l, "the file has too many meshes");
    }
    /* transitus primus: summae (et omnis accessor probatus) */
    per (n = ZEPHYRUM; n < s->retium; n++)
    {
        primitiva = _tabulatum(json_tabulatum_obtinere(retia, n),
            "primitives");
        per (k = ZEPHYRUM; k < _numerus_in(primitiva); k++)
        {
            si (!_primitivum_solvere(l,
                json_tabulatum_obtinere(primitiva, k), &p))
            {
                redde FALSUM;
            }
            partium++;
            verticum  += (i64)p.positiones.numerus;
            indicum   += (i64)p.indicum;
            si (   verticum > LIMES_VERTICUM || indicum > LIMES_INDICUM
                || partium > LIMES_NODORUM)
            {
                redde _recusare(l, "this model is too large (the "
                    "limit is one million vertices and one million "
                    "triangles)");
            }
        }
    }
    s->retia        = (StatuaRete*)_locus(l,
        s->retium * magnitudo(StatuaRete));
    s->partes       = (StatuaPars*)_locus(l,
        partium * magnitudo(StatuaPars));
    s->positiones   = (f32*)_locus(l,
        (memoriae_index)verticum * III * magnitudo(f32));
    s->normales     = (f32*)_locus(l,
        (memoriae_index)verticum * III * magnitudo(f32));
    s->coordinatae  = (f32*)_locus(l,
        (memoriae_index)verticum * II * magnitudo(f32));
    s->indices      = (i32*)_locus(l,
        (memoriae_index)indicum * magnitudo(i32));
    /* transitus secundus: copia */
    per (n = ZEPHYRUM; n < s->retium; n++)
    {
        primitiva = _tabulatum(json_tabulatum_obtinere(retia, n),
            "primitives");
        s->retia[n].pars_prima = s->partium;
        per (k = ZEPHYRUM; k < _numerus_in(primitiva); k++)
        {
            si (   !_primitivum_solvere(l,
                       json_tabulatum_obtinere(primitiva, k), &p)
                || !_primitivum_copiare(l, &p,
                       &s->partes[s->partium]))
            {
                redde FALSUM;
            }
            s->partium++;
            s->retia[n].partium++;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Nodi et scaena
 * ================================================== */

/* quaternio iam longitudinis I est (intra 1e-6)? */
interior b32
_longitudo_una (
    constans f32* r)
{
    f32 quadratum = r[ZEPHYRUM] * r[ZEPHYRUM] + r[I] * r[I]
        + r[II] * r[II] + r[III] * r[III];

    redde quadratum >= 1.0f - 1.0e-6f && quadratum <= 1.0f + 1.0e-6f;
}

/* signa nodi ex extras.tags (tabulatum chordarum). Forma alia,
 * chordae ignotae, extras absens: 0 - numquam recusatio (extras
 * liberum est, plagula sine eo aeque valet). */
interior i32
_signa_legere (
    JsonValor* nodus)
{
    JsonValor* extra;
    JsonValor* notae;
    JsonValor* nota;
       chorda  verbum;
          i32  signa = ZEPHYRUM;
          i32  k;

    extra = json_objectum_capere(nodus, "extras");
    si (!extra || !json_est_objectum(extra))
    {
        redde ZEPHYRUM;
    }
    notae = _tabulatum(extra, "tags");
    per (k = ZEPHYRUM; k < _numerus_in(notae); k++)
    {
        nota = json_tabulatum_obtinere(notae, k);
        si (!nota || !json_est_chorda(nota))
        {
            perge;
        }
        verbum = json_ad_chorda(nota);
        si (chorda_aequalis_literis(verbum, "walkable"))
        {
            signa |= STATUA_SIGNUM_GRESSIBILE;
        }
        si (chorda_aequalis_literis(verbum, "collidable"))
        {
            signa |= STATUA_SIGNUM_SOLIDUM;
        }
        si (chorda_aequalis_literis(verbum, "water"))
        {
            signa |= STATUA_SIGNUM_AQUA;
        }
    }
    redde signa;
}

interior b32
_nodos_legere (
    Lector* l)
{
             JsonValor* nodi = _tabulatum(l->radix, "nodes");
             JsonValor* j;
             JsonValor* liberi;
                Statua* s    = l->s;
           StatuaNodus* nodus;
    StereometriaMatrix  m;
                   f32  t[III];
                   f32  r[IV];
                   f32  sc[III];
                   i32  n;
                   i32  k;
                   i32  index;

    s->nodorum = _numerus_in(nodi);
    si (s->nodorum > LIMES_NODORUM)
    {
        redde _recusare(l, "the file has too many nodes");
    }
    s->nodi = (StatuaNodus*)_locus(l,
        s->nodorum * magnitudo(StatuaNodus));
    l->tituli = (chorda*)_locus(l, s->nodorum * magnitudo(chorda));
    per (n = ZEPHYRUM; n < s->nodorum; n++)
    {
        j      = json_tabulatum_obtinere(nodi, n);
        nodus  = &s->nodi[n];
        /* nomen absens aut non chorda: vacuum (json_ad_chorda) */
        l->tituli[n]   = chorda_transcribere(json_ad_chorda(
            json_objectum_capere(j, "name")), l->piscina);
        nodus->parens  = -I;
        nodus->rete    = -I;
        nodus->signa   = _signa_legere(j);
        memset(t, ZEPHYRUM, magnitudo(t));
        memset(r, ZEPHYRUM, magnitudo(r));
        r[III]        = 1.0f;
        sc[ZEPHYRUM]  = 1.0f;
        sc[I]         = 1.0f;
        sc[II]        = 1.0f;
        si (   !_numeros(j, "translation", III, t)
            || !_numeros(j, "rotation", IV, r)
            || !_numeros(j, "scale", III, sc))
        {
            redde _recusare(l, "a node in the file has a broken "
                "position, rotation or scale");
        }
        nodus->translatio  = stereometria_vector(t[ZEPHYRUM], t[I],
            t[II]);
        nodus->rotatio.x = r[ZEPHYRUM];
        nodus->rotatio.y = r[I];
        nodus->rotatio.z = r[II];
        nodus->rotatio.w = r[III];
        /* rotatio iam unitaria intacta manet: normalizatio digitum
         * ultimum movere potest, et plagula a scriptore nostro
         * scripta eandem statuam reddere debet */
        si (!_longitudo_una(r))
        {
            nodus->rotatio = stereometria_quaternio_normalizare(
                nodus->rotatio);
        }
        nodus->scala       = stereometria_vector(sc[ZEPHYRUM], sc[I],
            sc[II]);
        si (json_objectum_capere(j, "matrix"))
        {
            m = stereometria_identitas();
            si (!_numeros(j, "matrix", XVI, m.e))
            {
                redde _recusare(l, "a node in the file has a broken "
                    "matrix");
            }
            stereometria_in_trs(m, &nodus->translatio,
                &nodus->rotatio, &nodus->scala);
        }
        si (json_objectum_capere(j, "mesh"))
        {
            si (!_naturalis(json_objectum_capere(j, "mesh"), s->retium,
                &index))
            {
                redde _recusare(l, "a node refers to a mesh that is "
                    "not in the file");
            }
            nodus->rete = (s32)index;
        }
    }
    per (n = ZEPHYRUM; n < s->nodorum; n++)
    {
        liberi = _tabulatum(json_tabulatum_obtinere(nodi, n),
            "children");
        per (k = ZEPHYRUM; k < _numerus_in(liberi); k++)
        {
            si (!_naturalis(json_tabulatum_obtinere(liberi, k),
                s->nodorum, &index))
            {
                redde _recusare(l, "a node refers to a child that "
                    "is not in the file");
            }
            si (s->nodi[index].parens >= ZEPHYRUM || index == n)
            {
                redde _recusare(l, "the file's node tree is broken "
                    "(a node has two parents)");
            }
            s->nodi[index].parens = (s32)n;
        }
    }
    redde VERUM;
}

/* nodi extra scaenam ordinariam retia sua amittunt */
interior b32
_scaenam_applicare (
    Lector* l)
{
    JsonValor* scaenae = _tabulatum(l->radix, "scenes");
    JsonValor* radices;
       Statua* s       = l->s;
          b32* in_scaena;
          i32  scaena  = ZEPHYRUM;
          i32  n;
          i32  k;
          i32  index;
          i32  gradus;

    si (_numerus_in(scaenae) == ZEPHYRUM)
    {
        redde VERUM;
    }
    si (   json_objectum_capere(l->radix, "scene")
        && !_naturalis(json_objectum_capere(l->radix, "scene"),
               _numerus_in(scaenae), &scaena))
    {
        redde _recusare(l, "the file's default scene is not in the "
            "file");
    }
    radices    = _tabulatum(json_tabulatum_obtinere(scaenae, scaena),
        "nodes");
    in_scaena  = (b32*)_locus(l, s->nodorum * magnitudo(b32));
    per (k = ZEPHYRUM; k < _numerus_in(radices); k++)
    {
        si (!_naturalis(json_tabulatum_obtinere(radices, k),
            s->nodorum, &index))
        {
            redde _recusare(l, "the file's scene refers to a node "
                "that is not in the file");
        }
        in_scaena[index] = VERUM;
    }
    per (n = ZEPHYRUM; n < s->nodorum; n++)
    {
        index   = n;
        gradus  = ZEPHYRUM;
        dum (s->nodi[index].parens >= ZEPHYRUM)
        {
            index = (i32)s->nodi[index].parens;
            si (++gradus > s->nodorum)
            {
                redde _recusare(l, "the file's node tree is broken "
                    "(it loops back on itself)");
            }
        }
        si (!in_scaena[index])
        {
            s->nodi[n].rete = -I;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Cycli
 * ================================================== */

/* canalis unus; *lectus FALSUM si canalis praeteritur (sine nodo,
 * proprietas ignota, sine clavibus) */
interior b32
_canalem_legere (
           Lector* l,
        JsonValor* j,
        JsonValor* moduli,
    StatuaCanalis* canalis,
              b32* lectus)
{
    JsonValor* scopus = json_objectum_capere(j, "target");
    JsonValor* modulus;
     Accessor  tempora;
     Accessor  valores;
          i32  n;
          i32  k;
          i32  latus;
          i32  index;

    *lectus = FALSUM;
    si (_chorda_est(scopus, "path", "weights"))
    {
        redde _recusare(l, "this model animates morph targets, "
            "which are not supported");
    }
    si (_chorda_est(scopus, "path", "translation"))
    {
        canalis->proprietas = STATUA_TRANSLATIO;
    }
    alioquin si (_chorda_est(scopus, "path", "rotation"))
    {
        canalis->proprietas = STATUA_ROTATIO;
    }
    alioquin si (_chorda_est(scopus, "path", "scale"))
    {
        canalis->proprietas = STATUA_SCALA;
    }
    alioquin
    {
        redde VERUM;
    }
    si (!json_objectum_capere(scopus, "node"))
    {
        redde VERUM;
    }
    si (   !_naturalis(json_objectum_capere(scopus, "node"),
               l->s->nodorum, &index)
        || !_naturalis(json_objectum_capere(j, "sampler"),
               _numerus_in(moduli), &n))
    {
        redde _recusare(l, "an animation refers to a node or "
            "sampler that is not in the file");
    }
    canalis->nodus  = (s32)index;
    modulus         = json_tabulatum_obtinere(moduli, n);
    si (_chorda_est(modulus, "interpolation", "STEP"))
    {
        canalis->gradatim = VERUM;
    }
    alioquin si (   json_objectum_capere(modulus, "interpolation")
                 && !_chorda_est(modulus, "interpolation", "LINEAR"))
    {
        redde _recusare(l, "this model uses spline (CUBICSPLINE) "
            "animation, which is not supported");
    }
    si (   !_accessorem_solvere(l,
               json_objectum_capere(modulus, "input"), &tempora)
        || !_accessorem_solvere(l,
               json_objectum_capere(modulus, "output"), &valores))
    {
        redde FALSUM;
    }
    latus = canalis->proprietas == STATUA_ROTATIO ? IV : III;
    si (   tempora.genus   != GENUS_F32 || tempora.partes != I
        || valores.genus   != GENUS_F32 || valores.partes != latus
        || valores.numerus != tempora.numerus)
    {
        redde _recusare(l, "an animation stores its keys in a "
            "format that is not supported");
    }
    si (tempora.numerus == ZEPHYRUM)
    {
        redde VERUM;
    }
    l->clavium += tempora.numerus;
    si (l->clavium > LIMES_CLAVIUM)
    {
        redde _recusare(l, "this model has too many animation keys");
    }
    canalis->clavium  = tempora.numerus;
    canalis->tempora  = (f32*)_locus(l,
        tempora.numerus * magnitudo(f32));
    canalis->valores  = (f32*)_locus(l,
        (memoriae_index)tempora.numerus * latus * magnitudo(f32));
    per (n = ZEPHYRUM; n < tempora.numerus; n++)
    {
        canalis->tempora[n] = _partem_legere(&tempora, n, ZEPHYRUM);
        si (!_finitus(canalis->tempora[n]))
        {
            redde _recusare(l, "the file is damaged: an animation "
                "key time is not a number");
        }
        per (k = ZEPHYRUM; k < latus; k++)
        {
            canalis->valores[n * latus + k] = _partem_legere(&valores,
                n, k);
            si (!_finitus(canalis->valores[n * latus + k]))
            {
                redde _recusare(l, "the file is damaged: an "
                    "animation key is not a number");
            }
        }
    }
    *lectus = VERUM;
    redde VERUM;
}

interior b32
_cyclos_legere (
    Lector* l)
{
       JsonValor* animationes = _tabulatum(l->radix, "animations");
       JsonValor* j;
       JsonValor* canales;
       JsonValor* titulus;
          Statua* s = l->s;
    StatuaCyclus* cyclus;
             b32  lectus;
             i32  n;
             i32  k;
             f32  finis;

    s->cyclorum = _numerus_in(animationes);
    si (s->cyclorum > LIMES_NODORUM)
    {
        redde _recusare(l, "the file has too many animations");
    }
    s->cycli = (StatuaCyclus*)_locus(l,
        s->cyclorum * magnitudo(StatuaCyclus));
    per (n = ZEPHYRUM; n < s->cyclorum; n++)
    {
        j        = json_tabulatum_obtinere(animationes, n);
        cyclus   = &s->cycli[n];
        canales  = _tabulatum(j, "channels");
        titulus  = json_objectum_capere(j, "name");
        cyclus->titulus = titulus && json_est_chorda(titulus)
            && json_ad_chorda(titulus).mensura > ZEPHYRUM
            ? chorda_transcribere(json_ad_chorda(titulus), l->piscina)
            : chorda_ex_literis("clip", l->piscina);
        si (_numerus_in(canales) > LIMES_NODORUM)
        {
            redde _recusare(l, "an animation has too many channels");
        }
        cyclus->canales = (StatuaCanalis*)_locus(l,
            _numerus_in(canales) * magnitudo(StatuaCanalis));
        per (k = ZEPHYRUM; k < _numerus_in(canales); k++)
        {
            memset(&cyclus->canales[cyclus->canalium], ZEPHYRUM,
                magnitudo(StatuaCanalis));
            si (!_canalem_legere(l, json_tabulatum_obtinere(canales, k),
                _tabulatum(j, "samplers"),
                &cyclus->canales[cyclus->canalium], &lectus))
            {
                redde FALSUM;
            }
            si (!lectus)
            {
                perge;
            }
            finis = cyclus->canales[cyclus->canalium].tempora[
                cyclus->canales[cyclus->canalium].clavium - I];
            si (finis > cyclus->duratio)
            {
                cyclus->duratio = finis;
            }
            cyclus->canalium++;
        }
    }
    redde VERUM;
}

/* momentum elementi extras.moments: nomen non vacuum et tempus
 * numericum non negativum; aliter FALSUM (praeteritur). Elementum
 * non objectum aut nomen non chorda: json nomen vacuum dat */
interior b32
_momentum_legere (
         JsonValor* j,
    StatuaMomentum* exitus)
{
    chorda titulus = json_ad_chorda(json_objectum_capere(j, "name"));
       f64 tempus;

    si (   titulus.mensura == ZEPHYRUM
        || !_numerus(json_objectum_capere(j, "time"), &tempus)
        || !(tempus >= 0.0) || !_finitus((f32)tempus))
    {
        redde FALSUM;
    }
    exitus->titulus  = titulus;
    exitus->tempus   = (f32)tempus;
    redde VERUM;
}

/* momenta cyclorum ex extras.moments animationum. Forma inopinata
 * praeteritur - numquam recusatio (extras liberum est) */
interior vacuum
_momenta_legere (
    Lector* l)
{
         JsonValor* animationes = _tabulatum(l->radix, "animations");
         JsonValor* notae;
    StatuaMomentum  m;
               i32  transitus;
               i32  numerus = ZEPHYRUM;
               i32  n;
               i32  k;

    /* transitus 0 numerat, transitus I implet */
    per (transitus = ZEPHYRUM; transitus < II; transitus++)
    {
        si (transitus)
        {
            si (!numerus)
            {
                redde;
            }
            l->momenta = (StatuaMomentum*)_locus(l,
                numerus * magnitudo(StatuaMomentum));
        }
        per (n = ZEPHYRUM; n < l->s->cyclorum; n++)
        {
            notae = _tabulatum(json_objectum_capere(
                json_tabulatum_obtinere(animationes, n), "extras"),
                "moments");
            per (k = ZEPHYRUM; k < _numerus_in(notae); k++)
            {
                si (!_momentum_legere(
                    json_tabulatum_obtinere(notae, k), &m))
                {
                    perge;
                }
                si (!transitus)
                {
                    numerus++;
                    perge;
                }
                m.cyclus   = n;
                m.titulus  = chorda_transcribere(m.titulus,
                    l->piscina);
                l->momenta[l->momentorum++] = m;
            }
        }
    }
}


/* ==================================================
 * Continens et summa
 * ================================================== */

/* caput et partes: JSON in *json, BIN in lectorem */
interior b32
_continens_legere (
    Lector* l,
    chorda  octeti,
    chorda* json)
{
    i32 summa;
    i32 mensura;
    i32 locus;

    si (   octeti.mensura < XX || !octeti.datum
        || _u32_legere(octeti.datum) != GLB_MAGICA)
    {
        redde _recusare(l, "this is not a GLB file");
    }
    si (_u32_legere(octeti.datum + IV) != II)
    {
        redde _recusare(l, "only glTF version 2 files are "
            "supported");
    }
    summa = _u32_legere(octeti.datum + VIII);
    si (summa > octeti.mensura || summa < XX)
    {
        redde _recusare(l, "the file is cut short (it is smaller "
            "than its header says)");
    }
    mensura = _u32_legere(octeti.datum + XII);
    si (   _u32_legere(octeti.datum + XVI) != GLB_PARS_JSON
        || mensura > summa - XX)
    {
        redde _recusare(l, "the file is damaged: its description "
            "part is missing or cut short");
    }
    json->datum     = octeti.datum + XX;
    json->mensura   = mensura;
    locus           = XX + mensura;
    l->bin          = NIHIL;
    l->bin_mensura  = ZEPHYRUM;
    si (summa - locus >= VIII)
    {
        mensura = _u32_legere(octeti.datum + locus);
        si (   _u32_legere(octeti.datum + locus + IV) != GLB_PARS_BIN
            || mensura > summa - locus - VIII)
        {
            redde _recusare(l, "the file is damaged: its data part "
                "is missing or cut short");
        }
        l->bin          = octeti.datum + locus + VIII;
        l->bin_mensura  = mensura;
    }
    redde VERUM;
}

interior b32
_summa_probare (
    Lector* l)
{
    JsonValor* plagulae = _tabulatum(l->radix, "buffers");
    JsonValor* imagines = _tabulatum(l->radix, "images");
          i32  n;

    si (!json_est_objectum(l->radix))
    {
        redde _recusare(l, "the file is damaged: its description "
            "is not valid");
    }
    si (_numerus_in(_tabulatum(l->radix, "extensionsRequired"))
        > ZEPHYRUM)
    {
        redde _recusare(l, "this model requires a glTF extension "
            "that is not supported");
    }
    si (_numerus_in(_tabulatum(l->radix, "skins")) > ZEPHYRUM)
    {
        redde _recusare(l, "this model uses skinning (a bending "
            "mesh), which is not supported yet");
    }
    si (   _numerus_in(plagulae) > I
        || (   _numerus_in(plagulae) == I
            && json_objectum_capere(
                   json_tabulatum_obtinere(plagulae, ZEPHYRUM),
                   "uri")))
    {
        redde _recusare(l, "this model keeps its data in separate "
            "files; only self-contained GLB files are supported");
    }
    per (n = ZEPHYRUM; n < _numerus_in(imagines); n++)
    {
        si (json_objectum_capere(json_tabulatum_obtinere(imagines, n),
            "uri"))
        {
            redde _recusare(l, "this model keeps its texture in a "
                "separate file; only self-contained GLB files are "
                "supported");
        }
    }
    redde VERUM;
}

/* estne aliquid pingendum? */
interior b32
_habet_figuram (
    constans Statua* s)
{
    i32 n;
    i32 k;

    per (n = ZEPHYRUM; n < s->nodorum; n++)
    {
        si (s->nodi[n].rete < ZEPHYRUM)
        {
            perge;
        }
        per (k = ZEPHYRUM; k < s->retia[s->nodi[n].rete].partium; k++)
        {
            si (s->partes[s->retia[s->nodi[n].rete].pars_prima + k]
                    .indicum > ZEPHYRUM)
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

interior b32
_legere (
    Lector* l,
    chorda  octeti)
{
    JsonResultus r;
          chorda json;

    si (!_continens_legere(l, octeti, &json))
    {
        redde FALSUM;
    }
    r = json_legere(json, l->piscina);
    si (!r.successus || !r.radix)
    {
        redde _recusare(l, "the file is damaged: its description "
            "is not valid");
    }
    l->radix  = r.radix;
    l->s      = (Statua*)_locus(l, magnitudo(Statua));
    si (   !_summa_probare(l) || !_materias_legere(l)
        || !_retia_legere(l) || !_nodos_legere(l)
        || !_scaenam_applicare(l) || !_cyclos_legere(l))
    {
        redde FALSUM;
    }
    si (!_habet_figuram(l->s))
    {
        redde _recusare(l, "this file has nothing to show (no "
            "meshes in its scene)");
    }
    si (!statua_finire(l->s, l->piscina))
    {
        redde _recusare(l, "the file is damaged: its node tree or "
            "animation keys are inconsistent");
    }
    _momenta_legere(l);
    redde VERUM;
}

StatuaFructus
statua_glb_legere (
     chorda  octeti,
    Piscina* piscina)
{
    StatuaFructus f;
           Lector l;

    memset(&f, ZEPHYRUM, magnitudo(f));
    memset(&l, ZEPHYRUM, magnitudo(l));
    si (!piscina)
    {
        redde f;
    }
    l.piscina = piscina;
    si (_legere(&l, octeti))
    {
        f.successus   = VERUM;
        f.statua      = l.s;
        f.tituli      = l.tituli;
        f.momenta     = l.momenta;
        f.momentorum  = l.momentorum;
        redde f;
    }
    f.error = chorda_ex_literis(l.error ? l.error
        : "the file could not be read", piscina);
    redde f;
}
