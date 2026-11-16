/*
 * plastica_motus.c - figura cum motu (plastica_motus.h)
 *
 * Tres gradus: plastica_legere corpus dat (nodi, articuli, quies);
 * plagula iterum legitur et positurae cyclique in tabulas colliguntur;
 * coctio eos in canales Statuae vertit. Statua nova valorem prioris
 * transcribit et tabulas eius mutuatur; cycli soli novi sunt.
 */
#include "plastica_motus.h"
#include "stml.h"
#include "internamentum.h"
#include "xar.h"
#include "stereometria.h"
#include "plastica_membrum.h"
#include <math.h>
#include <string.h>

#define ERRORIS_MENSURA  CCLVI
#define DURATIO_MAXIMA   60.0f
#define GRADUS_DIMIDII   180.0f
#define GRADUS_PLENI     360.0f
/* sutura: 180 modulo 360 ambigua est */
#define GRADUS_MARGO     1.0e-3f

#define TACTA_ROTATIO  I
#define TACTA_SCALA    II
#define TACTA_MOTUS    IV

/* lenitas spatii a clave ad sequentem; NULLA: non scripta */
#define LENITAS_NULLA      ZEPHYRUM
#define LENITAS_LINEARIS   I
#define LENITAS_LENIS      II
#define LENITAS_ACCELERAT  III
#define LENITAS_TARDAT     IV
#define LENITAS_TENET      V

/* instans sumptum clavi sequenti propius non ponitur */
#define INSTANTIS_MARGO  1.0e-4f
/* clavis mota in suturam ipsam cadit */
#define SUTURAE_MARGO    1.0e-5f


/* linea nodi ex plagula ossium in arbore composita (plastica eam ut
 * datam reddit) */
#define LINEA_OSSIUM     ((i32)0x40000000)
#define QUIES_MARGO      1.0e-4f
#define QUIES_ROTATIO    0.999999f
#define TITULI_MENSURA   LXIV


/* quid positura de articulo uno dicat */
nomen structura {
    i32 tacta;          /* TACTA_*; 0 = quies */
    f32 rotatio[III];   /* gradus, ad quietem additi */
    f32 scala[III];
    f32 motus[III];
} Artus;

/* meta membri in positura */
nomen structura {
    b32 adest;
    b32 inclinata;      /* finis orientationem quietis tenet */
    f32 locus[III];     /* cardo finis, in spatio figurae */
    f32 inclinatio;     /* gradus circa x */
    i32 linea;          /* metae; hereditatae: positurae */
} Meta;

nomen structura {
    chorda titulus;
       i32 radix;       /* indices articulorum */
       i32 medium;
       i32 finis;
       b32 flexus_ante;
       f32 prima;       /* a radice ad medium */
       f32 secunda;     /* a medio ad finem */
    /* membrum ossium: meta ossium a fine quiescente ossium ad finem
     * quiescentem figurae fertur, differentia per rationem
     * longitudinum multiplicata */
                   f32 ratio;
    StereometriaVector finis_ossium;
    StereometriaVector finis_figurae;
} Membrum;

nomen structura {
    chorda  titulus;
     Artus* artus;      /* articulorum */
      Meta* metae;      /* PLASTICA_MOTUS_MEMBRA_MAXIMA */
       b32  ossium;     /* ex plagula ossium: figura substituere
                         * potest */
} Positura;

nomen structura {
    i32 positura;
    f32 tempus;
    i32 linea;
    b32 sutura;         /* clavis addita quae circulum claudit */
    i32 lenitas;        /* LENITAS_*: spatium ad clavem sequentem */
} Clavis;

nomen structura {
    chorda titulus;
       f32 duratio;
       i32 linea;
       b32 ossium;
    Clavis claves[PLASTICA_MOTUS_CLAVES_MAXIMAE + I];
       i32 clavium;
    chorda momenta[PLASTICA_MOTUS_MOMENTA_MAXIMA];
       f32 tempora[PLASTICA_MOTUS_MOMENTA_MAXIMA];
       i32 momentorum;
       b32 circularis;
    /* mora articuli (secundae; 0 = nulla) et linea eius */
       f32 morae[PLASTICA_ARTICULI_MAXIMI];
       i32 lineae_morarum[PLASTICA_ARTICULI_MAXIMI];
} Cyclus;

nomen structura {
            Piscina* piscina;
    constans Statua* basis;
    constans chorda* partes;
       constans i32* articuli;
                i32  articulorum;
           Positura  positurae[PLASTICA_MOTUS_POSITURAE_MAXIMAE + I];
                i32  positurarum;   /* prima "quies" est */
             Cyclus* cycli;         /* PLASTICA_MOTUS_CYCLI_MAXIMI */
                i32  cyclorum;
                b32  fractum;
                i32  linea;
          character  error[ERRORIS_MENSURA];
             /* titulus ossium dum ossa leguntur; vacua in figura */
    chorda plagula;
    chorda plagula_erroris;
       b32 in_ossibus;
    /* figura posituram ossium substituit */
       b32 substitutum;
    /* motus positurae ossium per hunc multiplicatur */
       f32 factor[PLASTICA_ARTICULI_MAXIMI];

       i32 par[PLASTICA_ARTICULI_MAXIMI];
    /* articuli ossium, ordine arboris; et articulus figurae pro
     * quoque (NIHIL si non nominatus) */
    constans StmlNodus* ossa[PLASTICA_ARTICULI_MAXIMI];
    constans StmlNodus* solida[PLASTICA_ARTICULI_MAXIMI];
                   i32  ossium;
               Membrum  membra[PLASTICA_MOTUS_MEMBRA_MAXIMA];
                   i32  membrorum;
    /* ossa sola ut statua (quies ossium: factores, fines membrorum) */
    constans Statua* quies_ossium;
       constans i32* articuli_ossium;
} Motor;


/* ==================================================
 * Recusatio
 * ================================================== */

interior vacuum
_appone (
                 Motor* m,
                   i32* longitudo,
    constans character* fons,
                   i32  mensura)
{
    i32 k;

    per (k = ZEPHYRUM;
         k < mensura && *longitudo + I < ERRORIS_MENSURA; k++)
    {
        character littera = fons[k];

        si (littera == '\n' || littera == '\r' || littera == '\t')
        {
            littera = ' ';
        }
        m->error[(*longitudo)++] = littera;
    }
    m->error[*longitudo] = '\0';
}

/* nuntius = ante + medium + post; prima recusatio manet */
interior b32
_recusare (
                 Motor* m,
                   i32  linea,
    constans character* ante,
       constans chorda* medium,
    constans character* post)
{
    i32 longitudo = ZEPHYRUM;

    si (m->fractum)
    {
        redde FALSUM;
    }
    m->fractum          = VERUM;
    m->linea            = linea;
    m->plagula_erroris  = m->plagula;
    si (ante)
    {
        _appone(m, &longitudo, ante, (i32)strlen(ante));
    }
    si (medium && medium->datum)
    {
        _appone(m, &longitudo, (constans character*)medium->datum,
            medium->mensura > LXIV ? LXIV : medium->mensura);
    }
    si (post)
    {
        _appone(m, &longitudo, post, (i32)strlen(post));
    }
    redde FALSUM;
}

/* "<elementum> attribute 'titulus' quid" */
interior b32
_recusare_attributum (
                 Motor* m,
    constans StmlNodus* nodus,
    constans character* titulus,
    constans character* quid)
{
    i32 longitudo = ZEPHYRUM;

    si (m->fractum)
    {
        redde FALSUM;
    }
    m->fractum          = VERUM;
    m->linea            = nodus->linea;
    m->plagula_erroris  = m->plagula;
    _appone(m, &longitudo, "<", I);
    si (nodus->titulus && nodus->titulus->datum)
    {
        _appone(m, &longitudo,
            (constans character*)nodus->titulus->datum,
            nodus->titulus->mensura > LXIV ? LXIV
                : nodus->titulus->mensura);
    }
    _appone(m, &longitudo, "> attribute '", XIII);
    _appone(m, &longitudo, titulus, (i32)strlen(titulus));
    _appone(m, &longitudo, "' ", II);
    _appone(m, &longitudo, quid, (i32)strlen(quid));
    redde FALSUM;
}


/* ==================================================
 * Valores
 * ================================================== */

interior b32
_est (
       constans chorda* titulus,
    constans character* literae)
{
    redde titulus && chorda_aequalis_literis(*titulus, literae);
}

interior b32
_spatium (
    i8 littera)
{
    redde littera == ' ' || littera == '\t' || littera == '\n'
        || littera == '\r';
}

/* 'numerus' numeri finiti spatiis separati, nec plus nec minus */
interior b32
_numeri (
    constans chorda* valor,
                f32* exitus,
                i32  numerus)
{
    i32 k = ZEPHYRUM;
    i32 n = ZEPHYRUM;

    dum (VERUM)
    {
        i32 initium;
        f64 x;

        dum (k < valor->mensura && _spatium(valor->datum[k]))
        {
            k++;
        }
        si (k >= valor->mensura)
        {
            frange;
        }
        initium = k;
        dum (k < valor->mensura && !_spatium(valor->datum[k]))
        {
            k++;
        }
        si (   n >= numerus
            || !chorda_ut_f64(chorda_sectio(*valor, initium, k), &x)
            || !(x == x) || x > 1.0e6 || x < -1.0e6)
        {
            redde FALSUM;
        }
        exitus[n++] = (f32)x;
    }
    redde n == numerus;
}

interior b32
_attributa_licita (
                           Motor* m,
              constans StmlNodus* nodus,
    constans character* constans* licita)
{
    i32 numerus = nodus->attributa ? xar_numerus(nodus->attributa)
        : ZEPHYRUM;
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans StmlAttributum* a =
            (constans StmlAttributum*)xar_obtinere(nodus->attributa,
                k);
                            b32 notum = FALSUM;

        per (j = ZEPHYRUM; a && licita[j]; j++)
        {
            si (_est(a->titulus, licita[j]))
            {
                notum = VERUM;
                frange;
            }
        }
        si (!notum)
        {
            redde _recusare(m, nodus->linea, "unknown attribute '",
                a ? a->titulus : NIHIL, "'");
        }
    }
    redde VERUM;
}

/* liberum k-esimum quod elementum est; *finis VERUM post ultimum.
 * Commenta et spatia praetereuntur; textus alius recusatur */
interior constans StmlNodus*
_liberum (
                 Motor* m,
    constans StmlNodus* nodus,
                   i32* cursor,
                   b32* finis)
{
    i32 numerus = nodus->liberi ? xar_numerus(nodus->liberi)
        : ZEPHYRUM;

    *finis = FALSUM;
    dum (*cursor < numerus)
    {
                StmlNodus** locus =
                    (StmlNodus**)xar_obtinere(nodus->liberi, *cursor);
        constans StmlNodus* liber = locus ? *locus : NIHIL;
                       i32  k;

        (*cursor)++;
        si (!liber || liber->genus == STML_NODUS_COMMENTUM)
        {
            perge;
        }
        si (liber->genus == STML_NODUS_ELEMENTUM)
        {
            redde liber;
        }
        per (k = ZEPHYRUM; liber->valor && k < liber->valor->mensura;
             k++)
        {
            si (!_spatium(liber->valor->datum[k]))
            {
                (vacuum)_recusare(m, nodus->linea, "<",
                    nodus->titulus,
                    "> has unexpected text between its elements");
                redde NIHIL;
            }
        }
    }
    *finis = VERUM;
    redde NIHIL;
}

/* elementum liberos elementa non habet */
interior b32
_sine_liberis (
                 Motor* m,
    constans StmlNodus* nodus)
{
    i32 cursor = ZEPHYRUM;
    b32 finis;

    si (_liberum(m, nodus, &cursor, &finis))
    {
        redde _recusare(m, nodus->linea, "<", nodus->titulus,
            "> cannot hold other elements");
    }
    redde finis;
}

/* index articuli (in tabula articulorum) titulo; -1 si nullus */
interior s32
_articulus (
       constans Motor* m,
      constans chorda* titulus)
{
    i32 k;

    per (k = ZEPHYRUM; k < m->articulorum; k++)
    {
        si (chorda_aequalis(m->partes[m->articuli[k]], *titulus))
        {
            redde (s32)k;
        }
    }
    redde -I;
}

/* index positurae titulo; -1 si nulla */
interior s32
_positura (
       constans Motor* m,
      constans chorda* titulus)
{
    i32 k;

    per (k = ZEPHYRUM; k < m->positurarum; k++)
    {
        si (chorda_aequalis(m->positurae[k].titulus, *titulus))
        {
            redde (s32)k;
        }
    }
    redde -I;
}

interior StereometriaQuaternio
_circa_axem (
    f32 x,
    f32 y,
    f32 z,
    f32 gradus)
{
    StereometriaQuaternio q;
                      f32 dimidium =
                          gradus * STEREOMETRIA_PI / GRADUS_PLENI;
                      f32 sinus    = (f32)sin((f64)dimidium);

    q.x = x * sinus;
    q.y = y * sinus;
    q.z = z * sinus;
    q.w = (f32)cos((f64)dimidium);
    redde q;
}

/* a * b: b primum applicatur */
interior StereometriaQuaternio
_componere (
    StereometriaQuaternio a,
    StereometriaQuaternio b)
{
    StereometriaQuaternio q;

    q.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    q.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
    q.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
    q.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    redde q;
}

/* quies articuli, deinde gradus positurae in spatio eius: circa x,
 * deinde y, deinde z */
interior StereometriaQuaternio
_rotatio_articuli (
    StereometriaQuaternio  quies,
             constans f32* gradus)
{
    StereometriaQuaternio q =
        _circa_axem(1.0f, 0.0f, 0.0f, gradus[ZEPHYRUM]);

    q = _componere(_circa_axem(0.0f, 1.0f, 0.0f, gradus[I]), q);
    q = _componere(_circa_axem(0.0f, 0.0f, 1.0f, gradus[II]), q);
    redde stereometria_quaternio_normalizare(_componere(quies, q));
}

/* titulus in suffixo desinit et praefixum non vacuum habet */
interior b32
_desinit (
    constans chorda* titulus,
 constans character* suffixum)
{
    i32 mensura = (i32)strlen(suffixum);

    redde titulus->mensura > mensura
        && memcmp(titulus->datum + titulus->mensura - mensura,
               suffixum, mensura) == ZEPHYRUM;
}


/* ==================================================
 * Membra et metae
 * ================================================== */

interior StereometriaQuaternio
_coniugatus (
    StereometriaQuaternio q)
{
    q.x = -q.x;
    q.y = -q.y;
    q.z = -q.z;
    redde q;
}

/* vector quaternione versus */
interior StereometriaVector
_vertere (
    StereometriaQuaternio q,
       StereometriaVector v)
{
    StereometriaVector u = stereometria_vector(q.x, q.y, q.z);
    StereometriaVector t =
        stereometria_scalare(stereometria_crux(u, v), 2.0f);

    redde stereometria_addere(stereometria_addere(v,
        stereometria_scalare(t, q.w)), stereometria_crux(u, t));
}

/* index articuli qui nodus est; -1 si nodus articulus non est */
interior s32
_articulus_nodi (
    constans Motor* m,
               s32  nodus)
{
    i32 k;

    per (k = ZEPHYRUM; k < m->articulorum; k++)
    {
        si ((s32)m->articuli[k] == nodus)
        {
            redde (s32)k;
        }
    }
    redde -I;
}

/* translatio, rotatio, scala nodi in positura data */
interior vacuum
_transformatio (
          constans Motor* m,
          constans Artus* artus,
                     s32  nodus,
      StereometriaVector* translatio,
   StereometriaQuaternio* rotatio,
      StereometriaVector* scala)
{
    constans StatuaNodus* n = &m->basis->nodi[nodus];
                     s32  j = _articulus_nodi(m, nodus);

    *translatio  = n->translatio;
    *rotatio     = n->rotatio;
    *scala       = n->scala;
    si (j < ZEPHYRUM)
    {
        redde;
    }
    si (artus[j].tacta & TACTA_MOTUS)
    {
        translatio->x += artus[j].motus[ZEPHYRUM];
        translatio->y += artus[j].motus[I];
        translatio->z += artus[j].motus[II];
    }
    si (artus[j].tacta & TACTA_ROTATIO)
    {
        *rotatio = _rotatio_articuli(n->rotatio, artus[j].rotatio);
    }
    si (artus[j].tacta & TACTA_SCALA)
    {
        scala->x *= artus[j].scala[ZEPHYRUM];
        scala->y *= artus[j].scala[I];
        scala->z *= artus[j].scala[II];
    }
}

/* punctum spatii figurae in spatium nodi (positura data); nodus -1:
 * spatium figurae ipsum */
interior StereometriaVector
_in_spatium (
       constans Motor* m,
       constans Artus* artus,
                  s32  nodus,
   StereometriaVector  punctum)
{
       StereometriaVector t;
    StereometriaQuaternio r;
       StereometriaVector s;

    si (nodus < ZEPHYRUM)
    {
        redde punctum;
    }
    punctum = _in_spatium(m, artus, m->basis->nodi[nodus].parens,
        punctum);
    _transformatio(m, artus, nodus, &t, &r, &s);
    punctum = _vertere(_coniugatus(r),
        stereometria_subtrahere(punctum, t));
    punctum.x /= s.x;
    punctum.y /= s.y;
    punctum.z /= s.z;
    redde punctum;
}

/* rotatio nodi in spatio figurae (positura data); scalae neglectae */
interior StereometriaQuaternio
_rotatio_mundi (
    constans Motor* m,
    constans Artus* artus,
               s32  nodus)
{
       StereometriaVector t;
    StereometriaQuaternio r;
       StereometriaVector s;

    si (nodus < ZEPHYRUM)
    {
        redde stereometria_quaternio_identitas();
    }
    _transformatio(m, artus, nodus, &t, &r, &s);
    redde stereometria_quaternio_normalizare(_componere(
        _rotatio_mundi(m, artus, m->basis->nodi[nodus].parens), r));
}

/* quaternio in gradus x y z (rotatio = Rz * Ry * Rx, ut positura) */
interior vacuum
_gradus_ex_rotatione (
    StereometriaQuaternio  q,
                      f32* gradus)
{
    f64 sinus = -2.0 * ((f64)q.x * q.z - (f64)q.w * q.y);

    si (sinus > 1.0)
    {
        sinus = 1.0;
    }
    si (sinus < -1.0)
    {
        sinus = -1.0;
    }
    gradus[ZEPHYRUM] = (f32)(atan2(
        2.0 * ((f64)q.y * q.z + (f64)q.w * q.x),
        1.0 - 2.0 * ((f64)q.x * q.x + (f64)q.y * q.y))
            * (f64)GRADUS_DIMIDII / (f64)STEREOMETRIA_PI);
    gradus[I] = (f32)(asin(sinus) * (f64)GRADUS_DIMIDII
        / (f64)STEREOMETRIA_PI);
    gradus[II] = (f32)(atan2(
        2.0 * ((f64)q.x * q.y + (f64)q.w * q.z),
        1.0 - 2.0 * ((f64)q.y * q.y + (f64)q.z * q.z))
            * (f64)GRADUS_DIMIDII / (f64)STEREOMETRIA_PI);
}

/* nuntius = ante + primus + medio + alter + post */
interior b32
_recusare_duo (
                 Motor* m,
                   i32  linea,
    constans character* ante,
       constans chorda* primus,
    constans character* medio,
       constans chorda* alter,
    constans character* post)
{
    character cauda[II * TITULI_MENSURA + LXIV];
          i32 n = ZEPHYRUM;
          i32 mensura;

    mensura = (i32)strlen(medio);
    memcpy(cauda + n, medio, mensura);
    n += mensura;
    mensura = alter->mensura > TITULI_MENSURA ? TITULI_MENSURA
        : alter->mensura;
    memcpy(cauda + n, alter->datum, mensura);
    n        += mensura;
    mensura  = (i32)strlen(post);
    si (mensura > LX)
    {
        mensura = LX;
    }
    memcpy(cauda + n, post, mensura);
    cauda[n + mensura] = '\0';
    redde _recusare(m, linea, ante, primus, cauda);
}

/* index membri titulo; -1 si nullum */
interior s32
_membrum (
     constans Motor* m,
    constans chorda* titulus)
{
    i32 k;

    per (k = ZEPHYRUM; k < m->membrorum; k++)
    {
        si (chorda_aequalis(m->membra[k].titulus, *titulus))
        {
            redde (s32)k;
        }
    }
    redde -I;
}

/* cardo nodi in quiete, in spatio statuae */
interior b32
_cardo_quietis (
                 Motor* m,
       constans Statua* statua,
                   i32  nodus,
    StereometriaVector* exitus)
{
    StereometriaMatrix* matrices =
        (StereometriaMatrix*)piscina_allocare(m->piscina,
            (memoriae_index)(statua->nodorum + I)
                * magnitudo(StereometriaMatrix));

    si (!matrices)
    {
        redde FALSUM;
    }
    statua_positura(statua, -I, 0.0f, matrices);
    *exitus = stereometria_punctum_transformare(matrices[nodus],
        stereometria_vector(0.0f, 0.0f, 0.0f), NIHIL);
    redde VERUM;
}

/* elementum membrum: titulus, radix, medium, finis, flexus */
interior b32
_membrum_legere (
                 Motor* m,
    constans StmlNodus* nodus)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", "radix", "medium", "finis", "flexus", NIHIL
    };
    hic_manens constans character* constans OSSA[] = {
        "radix", "medium", "finis"
    };
         constans chorda* titulus =
             stml_attributum_capere(nodus, "titulus");
         constans chorda* flexus =
             stml_attributum_capere(nodus, "flexus");
         constans chorda* tituli[III];
    constans StatuaNodus* medius;
    constans StatuaNodus* ultimus;
                 Membrum* mb;
                     s32  ossa[III];
                     i32  k;
                     i32  j;
                     i32  n;

    si (   !_attributa_licita(m, nodus, LICITA)
        || !_sine_liberis(m, nodus))
    {
        redde FALSUM;
    }
    si (!titulus)
    {
        redde _recusare_attributum(m, nodus, "titulus",
            "is needed and cannot be empty");
    }
    si (_membrum(m, titulus) >= ZEPHYRUM)
    {
        redde _recusare(m, nodus->linea, "limb title '", titulus,
            "' is used twice");
    }
    si (m->membrorum >= PLASTICA_MOTUS_MEMBRA_MAXIMA)
    {
        redde _recusare(m, nodus->linea,
            "<membrum> is one limb too many (at most 16)", NIHIL,
            NIHIL);
    }
    per (k = ZEPHYRUM; k < III; k++)
    {
        tituli[k] = stml_attributum_capere(nodus, OSSA[k]);
        si (!tituli[k])
        {
            redde _recusare_attributum(m, nodus, OSSA[k],
                "is needed (a joint's title)");
        }
        ossa[k] = _articulus(m, tituli[k]);
        si (ossa[k] < ZEPHYRUM)
        {
            redde _recusare(m, nodus->linea,
                "<membrum> names an unknown joint '", tituli[k], "'");
        }
    }
    si (!flexus || (!_est(flexus, "ante") && !_est(flexus, "retro")))
    {
        redde _recusare_attributum(m, nodus, "flexus",
            "must be 'ante' (knee forward) or 'retro'");
    }
    medius   = &m->basis->nodi[m->articuli[ossa[I]]];
    ultimus  = &m->basis->nodi[m->articuli[ossa[II]]];
    si (medius->parens != (s32)m->articuli[ossa[ZEPHYRUM]])
    {
        redde _recusare_duo(m, nodus->linea, "<membrum>: joint '",
            tituli[I], "' is not a child of '", tituli[ZEPHYRUM], "'");
    }
    si (ultimus->parens != (s32)m->articuli[ossa[I]])
    {
        redde _recusare_duo(m, nodus->linea, "<membrum>: joint '",
            tituli[II], "' is not a child of '", tituli[I], "'");
    }
    per (k = ZEPHYRUM; k < m->membrorum; k++)
    {
        constans Membrum* prius = &m->membra[k];

        per (j = ZEPHYRUM; j < III; j++)
        {
            n = (i32)ossa[j];
            si (   n == prius->radix || n == prius->medium
                || n == prius->finis)
            {
                redde _recusare_duo(m, nodus->linea, "joint '",
                    tituli[j], "' is already in the limb '",
                    &prius->titulus, "'");
            }
        }
    }
    si (   fabs((f64)medius->translatio.x) > QUIES_MARGO
        || fabs((f64)medius->translatio.z) > QUIES_MARGO
        || !(medius->translatio.y < -QUIES_MARGO)
        || fabs((f64)ultimus->translatio.x) > QUIES_MARGO
        || fabs((f64)ultimus->translatio.z) > QUIES_MARGO
        || !(ultimus->translatio.y < -QUIES_MARGO)
        || fabs((f64)medius->rotatio.w) < QUIES_ROTATIO)
    {
        redde _recusare(m, nodus->linea, "limb '", titulus,
            "' must hang straight down at rest ('medium' and 'finis'"
            " below their parent, no rest turn on 'medium')");
    }
    mb               = &m->membra[m->membrorum++];
    mb->titulus      = *titulus;
    mb->radix        = (i32)ossa[ZEPHYRUM];
    mb->medium       = (i32)ossa[I];
    mb->finis        = (i32)ossa[II];
    mb->flexus_ante  = _est(flexus, "ante");
    mb->prima        = -medius->translatio.y;
    mb->secunda      = -ultimus->translatio.y;
    mb->ratio        = 1.0f;
    si (m->in_ossibus)
    {
        constans Statua* q = m->quies_ossium;
           constans i32* a = m->articuli_ossium;
                    f32  longitudo =
                        stereometria_longitudo(
                            q->nodi[a[ossa[I]]].translatio)
                        + stereometria_longitudo(
                            q->nodi[a[ossa[II]]].translatio);

        si (   !_cardo_quietis(m, q, m->articuli_ossium[ossa[II]],
                   &mb->finis_ossium)
            || !_cardo_quietis(m, m->basis, m->articuli[ossa[II]],
                   &mb->finis_figurae))
        {
            redde _recusare(m, ZEPHYRUM, "out of memory", NIHIL,
                NIHIL);
        }
        si (longitudo > QUIES_MARGO)
        {
            mb->ratio = (mb->prima + mb->secunda) / longitudo;
        }
    }
    redde VERUM;
}

/* membra sub radice legere (ante posituras) */
interior b32
_membra_legere (
                 Motor* m,
    constans StmlNodus* radix)
{
    constans StmlNodus* liber;
                   b32  finis;
                   i32  cursor = ZEPHYRUM;

    dum ((liber = _liberum(m, radix, &cursor, &finis)) != NIHIL)
    {
        si (   _est(liber->titulus, "membrum")
            && !_membrum_legere(m, liber))
        {
            redde FALSUM;
        }
    }
    redde finis;
}

/* elementum meta in positura: membrum, locus, inclinatio */
interior b32
_metam_legere (
                 Motor* m,
    constans StmlNodus* nodus,
                  Meta* metae,
                   b32* scriptae)
{
    hic_manens constans character* constans LICITA[] = {
        "membrum", "locus", "inclinatio", NIHIL
    };
    constans chorda* membrum =
        stml_attributum_capere(nodus, "membrum");
    constans chorda* locus = stml_attributum_capere(nodus, "locus");
    constans chorda* inclinatio =
        stml_attributum_capere(nodus, "inclinatio");
               Meta* mt;
                s32  k;

    si (   !_attributa_licita(m, nodus, LICITA)
        || !_sine_liberis(m, nodus))
    {
        redde FALSUM;
    }
    si (!membrum)
    {
        redde _recusare_attributum(m, nodus, "membrum",
            "is needed (a limb's title)");
    }
    k = _membrum(m, membrum);
    si (k < ZEPHYRUM)
    {
        redde _recusare(m, nodus->linea,
            "<meta> names an unknown limb '", membrum, "'");
    }
    si (scriptae[k])
    {
        redde _recusare(m, nodus->linea, "limb '", membrum,
            "' appears twice in this pose");
    }
    scriptae[k]  = VERUM;
    mt           = &metae[k];
    memset(mt, ZEPHYRUM, magnitudo(*mt));
    si (!locus || !_numeri(locus, mt->locus, III))
    {
        redde _recusare_attributum(m, nodus, "locus",
            "needs 3 numbers (x y z, where the limb's end goes)");
    }
    si (inclinatio)
    {
        si (!_numeri(inclinatio, &mt->inclinatio, I))
        {
            redde _recusare_attributum(m, nodus, "inclinatio",
                "needs 1 number (degrees)");
        }
        mt->inclinata = VERUM;
    }
    si (m->in_ossibus)
    {
        constans Membrum* mb = &m->membra[k];

        mt->locus[ZEPHYRUM] = mb->finis_figurae.x
            + (mt->locus[ZEPHYRUM] - mb->finis_ossium.x) * mb->ratio;
        mt->locus[I] = mb->finis_figurae.y
            + (mt->locus[I] - mb->finis_ossium.y) * mb->ratio;
        mt->locus[II] = mb->finis_figurae.z
            + (mt->locus[II] - mb->finis_ossium.z) * mb->ratio;
    }
    mt->adest = VERUM;
    mt->linea = nodus->linea;
    redde VERUM;
}

/* metae contra <art> positurae eiusdem: scripta cum <art> membri
 * recusatur; hereditata ab <art> tollitur. visi: I = <art> adest,
 * II = cum rotatione */
interior b32
_metas_conferre (
                 Motor* m,
    constans StmlNodus* nodus,
                  Meta* metae,
          constans b32* scriptae,
          constans b32* visi)
{
    i32 k;

    per (k = ZEPHYRUM; k < m->membrorum; k++)
    {
        constans Membrum* mb = &m->membra[k];
                    Meta* mt = &metae[k];

        si (!mt->adest)
        {
            perge;
        }
        si (!scriptae[k])
        {
            mt->linea = nodus->linea;
            si (visi[mb->radix] || visi[mb->medium])
            {
                mt->adest = FALSUM;
            }
            alioquin si (visi[mb->finis] & II)
            {
                mt->inclinata = FALSUM;
            }
            perge;
        }
        si (visi[mb->radix] || visi[mb->medium])
        {
            redde _recusare(m, mt->linea, "joint '",
                &m->partes[m->articuli[visi[mb->radix] ? mb->radix
                    : mb->medium]],
                "' has an <art> and its limb a <meta> in the same"
                " pose");
        }
        si (mt->inclinata && (visi[mb->finis] & II))
        {
            redde _recusare(m, mt->linea, "joint '",
                &m->partes[m->articuli[mb->finis]],
                "' has a turn and its limb a <meta> with"
                " 'inclinatio' in the same pose");
        }
    }
    redde VERUM;
}

/* metae positurae in rotationes: radix (x antrorsum, z ad latus),
 * medium (x), finis si inclinata. Post omnes <art> */
interior b32
_metas_solvere (
             Motor* m,
             Artus* artus,
     constans Meta* metae)
{
    i32 k;

    per (k = ZEPHYRUM; k < m->membrorum; k++)
    {
              constans Membrum* mb = &m->membra[k];
                 constans Meta* mt = &metae[k];
          constans StatuaNodus* radix;
        PlasticaMembrumSolutio  solutio;
            StereometriaVector  d;
                           f64  infra;

        si (!mt->adest)
        {
            perge;
        }
        radix = &m->basis->nodi[m->articuli[mb->radix]];
        /* meta in spatio quietis radicis, a cardine eius */
        d = _in_spatium(m, artus, radix->parens, stereometria_vector(
            mt->locus[ZEPHYRUM], mt->locus[I], mt->locus[II]));
        d = stereometria_subtrahere(d, radix->translatio);
        si (artus[mb->radix].tacta & TACTA_MOTUS)
        {
            d.x -= artus[mb->radix].motus[ZEPHYRUM];
            d.y -= artus[mb->radix].motus[I];
            d.z -= artus[mb->radix].motus[II];
        }
        d      = _vertere(_coniugatus(radix->rotatio), d);
        infra  = sqrt((f64)d.x * d.x + (f64)d.y * d.y);
        solutio = plastica_membrum_solvere(mb->prima, mb->secunda,
            d.z, (f32)infra, mb->flexus_ante);
        si (!solutio.successus)
        {
            redde _recusare(m, mt->linea, "limb '", &mb->titulus,
                "': the target is out of reach (too far, too near,"
                " or on the limb's root)");
        }
        artus[mb->radix].tacta              |= TACTA_ROTATIO;
        artus[mb->radix].rotatio[ZEPHYRUM]  = solutio.angulus_radicis;
        artus[mb->radix].rotatio[I]         = 0.0f;
        artus[mb->radix].rotatio[II]        = infra > 1.0e-6
            ? (f32)(atan2((f64)d.x, -(f64)d.y) * (f64)GRADUS_DIMIDII
                / (f64)STEREOMETRIA_PI)
            : 0.0f;
        artus[mb->medium].tacta              |= TACTA_ROTATIO;
        artus[mb->medium].rotatio[ZEPHYRUM]  = solutio.angulus_medii;
        artus[mb->medium].rotatio[I]         = 0.0f;
        artus[mb->medium].rotatio[II]        = 0.0f;
        si (mt->inclinata)
        {
            /* finis: orientatio quietis in spatio figurae, circa x
             * figurae inclinata */
                              s32 nodus = (s32)m->articuli[mb->finis];
            StereometriaQuaternio q     = _componere(
                _circa_axem(1.0f, 0.0f, 0.0f, mt->inclinatio),
                _rotatio_mundi(m, m->positurae[ZEPHYRUM].artus,
                    nodus));

            q = _componere(_coniugatus(_rotatio_mundi(m, artus,
                m->basis->nodi[nodus].parens)), q);
            q = _componere(
                _coniugatus(m->basis->nodi[nodus].rotatio), q);
            artus[mb->finis].tacta |= TACTA_ROTATIO;
            _gradus_ex_rotatione(
                stereometria_quaternio_normalizare(q),
                artus[mb->finis].rotatio);
        }
    }
    redde VERUM;
}

/* metae imaginis: in membrum par, x negato */
interior b32
_metas_speculare (
             Motor* m,
               i32  linea,
     constans Meta* fons,
              Meta* exitus)
{
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < m->membrorum; k++)
    {
        constans chorda* titulus   = &m->membra[k].titulus;
                    b32  sinister  = _desinit(titulus, "_sin");
                    i32  par       = k;

        si (!fons[k].adest)
        {
            perge;
        }
        si (sinister || _desinit(titulus, "_dex"))
        {
            i32 radix = titulus->mensura - IV;

            per (j = ZEPHYRUM; j < m->membrorum; j++)
            {
                constans chorda* alter = &m->membra[j].titulus;

                si (   alter->mensura == titulus->mensura
                    && _desinit(alter, sinister ? "_dex" : "_sin")
                    && memcmp(alter->datum, titulus->datum, radix)
                        == ZEPHYRUM)
                {
                    frange;
                }
            }
            si (j >= m->membrorum)
            {
                chorda praefixum = *titulus;

                praefixum.mensura = radix;
                redde _recusare_duo(m, linea, "limb '", titulus,
                    "' has no partner '", &praefixum,
                    sinister ? "_dex'" : "_sin'");
            }
            par = j;
        }
        exitus[par]                  = fons[k];
        exitus[par].locus[ZEPHYRUM]  = -fons[k].locus[ZEPHYRUM];
    }
    redde VERUM;
}


/* ==================================================
 * Positurae
 * ================================================== */

interior b32
_artum_legere (
                 Motor* m,
    constans StmlNodus* nodus,
                 Artus* artus,
                   b32* visi)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", "rotatio", "scala", "motus", NIHIL
    };
    constans chorda* titulus =
        stml_attributum_capere(nodus, "titulus");
    constans chorda* rotatio =
        stml_attributum_capere(nodus, "rotatio");
    constans chorda* scala = stml_attributum_capere(nodus, "scala");
    constans chorda* motus = stml_attributum_capere(nodus, "motus");
              Artus* a;
                s32  k;

    si (   !_attributa_licita(m, nodus, LICITA)
        || !_sine_liberis(m, nodus))
    {
        redde FALSUM;
    }
    si (!titulus)
    {
        redde _recusare_attributum(m, nodus, "titulus",
            "is needed (the joint's title)");
    }
    k = _articulus(m, titulus);
    si (k < ZEPHYRUM)
    {
        redde _recusare(m, nodus->linea,
            "<art> names an unknown joint '",
            titulus, "'");
    }
    si (visi[k])
    {
        redde _recusare(m, nodus->linea, "joint '", titulus,
            "' appears twice in this pose");
    }
    visi[k] = rotatio ? III : I;
    si (!rotatio && !scala && !motus)
    {
        redde _recusare(m, nodus->linea, "<art> needs at least one of"
            " 'rotatio', 'scala', 'motus'", NIHIL, NIHIL);
    }
    a = &artus[k];
    si (rotatio)
    {
        si (!_numeri(rotatio, a->rotatio, III))
        {
            redde _recusare_attributum(m, nodus, "rotatio",
                "needs 3 numbers (degrees about x y z)");
        }
        a->tacta |= TACTA_ROTATIO;
    }
    si (scala)
    {
        si (_numeri(scala, a->scala, I))
        {
            a->scala[I]   = a->scala[ZEPHYRUM];
            a->scala[II]  = a->scala[ZEPHYRUM];
        }
        alioquin si (!_numeri(scala, a->scala, III))
        {
            redde _recusare_attributum(m, nodus, "scala",
                "needs 1 or 3 numbers above 0");
        }
        si (   !(a->scala[ZEPHYRUM] > 0.0f) || !(a->scala[I] > 0.0f)
            || !(a->scala[II] > 0.0f))
        {
            redde _recusare_attributum(m, nodus, "scala",
                "needs 1 or 3 numbers above 0");
        }
        a->tacta |= TACTA_SCALA;
    }
    si (motus)
    {
        si (m->basis->nodi[m->articuli[k]].parens != ZEPHYRUM)
        {
            redde _recusare_attributum(m, nodus, "motus",
                "is only allowed on a joint directly under <figura>");
        }
        si (!_numeri(motus, a->motus, III))
        {
            redde _recusare_attributum(m, nodus, "motus",
                "needs 3 numbers (x y z)");
        }
        si (m->in_ossibus)
        {
            a->motus[ZEPHYRUM]  *= m->factor[k];
            a->motus[I]         *= m->factor[k];
            a->motus[II]        *= m->factor[k];
        }
        a->tacta |= TACTA_MOTUS;
    }
    redde VERUM;
}

/* quies paris imago est: locus trans x = 0, rotatio reflexa */
interior b32
_quies_imago (
    constans StatuaNodus* a,
    constans StatuaNodus* b)
{
    f32 punctum =
        a->rotatio.x * b->rotatio.x - a->rotatio.y * b->rotatio.y
        - a->rotatio.z * b->rotatio.z + a->rotatio.w * b->rotatio.w;

    redde fabs((f64)(a->translatio.x + b->translatio.x)) <= QUIES_MARGO
        && fabs((f64)(a->translatio.y - b->translatio.y))
            <= QUIES_MARGO
        && fabs((f64)(a->translatio.z - b->translatio.z))
            <= QUIES_MARGO
        && fabs((f64)punctum) >= QUIES_ROTATIO;
}

/* par cuiusque articuli ("X_sin" cum "X_dex"; ceteri sibi ipsi), et
 * quies paris imago */
interior b32
_paria_parare (
    Motor* m,
      i32  linea)
{
    character post[TITULI_MENSURA + XXXII];
          i32 j;
          i32 k;

    per (j = ZEPHYRUM; j < m->articulorum; j++)
    {
        constans chorda* titulus   = &m->partes[m->articuli[j]];
                    b32  sinister  = _desinit(titulus, "_sin");
                    i32  radix     = titulus->mensura - IV;

        m->par[j] = j;
        si (!sinister && !_desinit(titulus, "_dex"))
        {
            perge;
        }
        per (k = ZEPHYRUM; k < m->articulorum; k++)
        {
            constans chorda* alter = &m->partes[m->articuli[k]];

            si (   alter->mensura == titulus->mensura
                && _desinit(alter, sinister ? "_dex" : "_sin")
                && memcmp(alter->datum, titulus->datum, radix)
                    == ZEPHYRUM)
            {
                frange;
            }
        }
        si (k >= m->articulorum)
        {
            /* "' has no partner 'X_dex'" */
            i32 n = radix > TITULI_MENSURA ? TITULI_MENSURA : radix;

            memcpy(post, "' has no partner '", XVIII);
            memcpy(post + XVIII, titulus->datum, n);
            memcpy(post + XVIII + n, sinister ? "_dex'" : "_sin'",
                VI);
            redde _recusare(m, linea, "joint '", titulus, post);
        }
        m->par[j] = k;
    }
    per (j = ZEPHYRUM; j < m->articulorum; j++)
    {
        constans StatuaNodus* a = &m->basis->nodi[m->articuli[j]];
        constans StatuaNodus* b =
            &m->basis->nodi[m->articuli[m->par[j]]];
                         s32 parens = ZEPHYRUM;

        si (m->par[j] == j)
        {
            perge;
        }
        /* parens paris par parentis sit (radix sibi ipsa) */
        per (k = ZEPHYRUM; k < m->articulorum; k++)
        {
            si ((s32)m->articuli[k] == a->parens)
            {
                parens = (s32)m->articuli[m->par[k]];
            }
        }
        si (b->parens != parens || !_quies_imago(a, b))
        {
            redde _recusare(m, linea, "joint '",
                &m->partes[m->articuli[j]],
                "' and its partner are not mirror images at rest"
                " (locus, turn or parent)");
        }
    }
    redde VERUM;
}

/* imago positurae trans x = 0 in 'exitus' */
interior vacuum
_speculare (
     constans Motor* m,
     constans Artus* fons,
              Artus* exitus)
{
    i32 j;

    per (j = ZEPHYRUM; j < m->articulorum; j++)
    {
        Artus* a = &exitus[m->par[j]];

        *a                  = fons[j];
        a->rotatio[I]       = -a->rotatio[I];
        a->rotatio[II]      = -a->rotatio[II];
        a->motus[ZEPHYRUM]  = -a->motus[ZEPHYRUM];
    }
}

interior b32
_positura_legere (
                 Motor* m,
    constans StmlNodus* nodus)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", "ex", "speculum", NIHIL
    };
       constans chorda* titulus =
           stml_attributum_capere(nodus, "titulus");
       constans chorda* origo = stml_attributum_capere(nodus, "ex");
       constans chorda* speculum =
           stml_attributum_capere(nodus, "speculum");
    constans StmlNodus* liber;
              Positura* p;
                 Artus* artus;
                  Meta* metae;
                   b32  visi[PLASTICA_ARTICULI_MAXIMI];
                   b32  scriptae[PLASTICA_MOTUS_MEMBRA_MAXIMA];
                   b32  finis;
                   i32  cursor = ZEPHYRUM;
                   s32  locus;

    si (!_attributa_licita(m, nodus, LICITA))
    {
        redde FALSUM;
    }
    si (!titulus)
    {
        redde _recusare_attributum(m, nodus, "titulus",
            "is needed and cannot be empty");
    }
    si (_est(titulus, "quies"))
    {
        redde _recusare(m, nodus->linea, "the pose 'quies' is built in"
            " (the rest pose) and cannot be written", NIHIL, NIHIL);
    }
    /* figura posituram ossium eiusdem tituli substituit */
    locus = _positura(m, titulus);
    si (   locus >= ZEPHYRUM
        && (m->in_ossibus || !m->positurae[locus].ossium))
    {
        redde _recusare(m, nodus->linea, "pose title '", titulus,
            "' is used twice");
    }
    si (   locus < ZEPHYRUM
        && m->positurarum > PLASTICA_MOTUS_POSITURAE_MAXIMAE)
    {
        redde _recusare(m, nodus->linea,
            "<positura> is one pose too many (at most 64)", NIHIL,
            NIHIL);
    }
    artus = (Artus*)piscina_allocare(m->piscina,
        (m->articulorum ? m->articulorum : I) * magnitudo(Artus));
    metae = (Meta*)piscina_allocare(m->piscina,
        PLASTICA_MOTUS_MEMBRA_MAXIMA * magnitudo(Meta));
    si (!artus || !metae)
    {
        redde _recusare(m, ZEPHYRUM, "out of memory", NIHIL, NIHIL);
    }
    memset(artus, ZEPHYRUM,
        (m->articulorum ? m->articulorum : I) * magnitudo(Artus));
    memset(metae, ZEPHYRUM,
        PLASTICA_MOTUS_MEMBRA_MAXIMA * magnitudo(Meta));
    si (origo && speculum)
    {
        redde _recusare(m, nodus->linea, "<positura>: 'speculum' and"
            " 'ex' cannot be used together", NIHIL, NIHIL);
    }
    si (origo)
    {
        s32 k = _positura(m, origo);

        si (k < ZEPHYRUM)
        {
            redde _recusare_attributum(m, nodus, "ex",
                "names a pose that is not written above this one");
        }
        memcpy(artus, m->positurae[k].artus,
            m->articulorum * magnitudo(Artus));
        memcpy(metae, m->positurae[k].metae,
            PLASTICA_MOTUS_MEMBRA_MAXIMA * magnitudo(Meta));
    }
    si (speculum)
    {
        s32 k = _positura(m, speculum);

        si (k < ZEPHYRUM)
        {
            redde _recusare_attributum(m, nodus, "speculum",
                "names a pose that is not written above this one");
        }
        si (!_paria_parare(m, nodus->linea))
        {
            redde FALSUM;
        }
        _speculare(m, m->positurae[k].artus, artus);
        si (!_metas_speculare(m, nodus->linea,
            m->positurae[k].metae, metae))
        {
            redde FALSUM;
        }
    }
    memset(visi, ZEPHYRUM, magnitudo(visi));
    memset(scriptae, ZEPHYRUM, magnitudo(scriptae));
    dum ((liber = _liberum(m, nodus, &cursor, &finis)) != NIHIL)
    {
        si (_est(liber->titulus, "meta"))
        {
            si (!_metam_legere(m, liber, metae, scriptae))
            {
                redde FALSUM;
            }
            perge;
        }
        si (!_est(liber->titulus, "art"))
        {
            redde _recusare(m, liber->linea,
                "<positura> holds only <art> and <meta>, got <",
                liber->titulus, ">");
        }
        si (!_artum_legere(m, liber, artus, visi))
        {
            redde FALSUM;
        }
    }
    si (   !finis
        || !_metas_conferre(m, nodus, metae, scriptae, visi)
        || !_metas_solvere(m, artus, metae))
    {
        redde FALSUM;
    }
    si (locus < ZEPHYRUM)
    {
        locus = (s32)m->positurarum++;
    }
    alioquin
    {
        m->substitutum = VERUM;
    }
    p           = &m->positurae[locus];
    p->titulus  = *titulus;
    p->artus    = artus;
    p->metae    = metae;
    p->ossium   = m->in_ossibus;
    redde VERUM;
}


/* ==================================================
 * Cycli
 * ================================================== */

/* differentia graduum inter claves vicinas CLXXX attingit? Sutura
 * modulo CCCLX confertur */
interior b32
_nimis_vertit (
    f32 prior,
    f32 sequens,
    b32 sutura)
{
    f32 d = sequens - prior;

    si (sutura)
    {
        d -= GRADUS_PLENI
            * (f32)floor((f64)((d + GRADUS_DIMIDII) / GRADUS_PLENI));
        redde d < -GRADUS_DIMIDII + GRADUS_MARGO;
    }
    redde d >= GRADUS_DIMIDII || d <= -GRADUS_DIMIDII;
}

/* claves vicinae: nullus articulus in ullo axe CLXXX gradus vertit */
interior b32
_vias_probare (
               Motor* m,
     constans Cyclus* c)
{
    i32 k;
    i32 j;
    i32 a;

    per (k = I; k < c->clavium; k++)
    {
        constans Artus* prior =
            m->positurae[c->claves[k - I].positura].artus;
        constans Artus* sequens =
            m->positurae[c->claves[k].positura].artus;

        per (j = ZEPHYRUM; j < m->articulorum; j++)
        {
            per (a = ZEPHYRUM; a < III; a++)
            {
                si (_nimis_vertit(prior[j].rotatio[a],
                    sequens[j].rotatio[a], c->claves[k].sutura))
                {
                    redde _recusare(m, c->claves[k].linea,
                        "joint '", &m->partes[m->articuli[j]],
                        "' turns 180 degrees or more since the"
                        " previous key: add a key between");
                }
            }
        }
    }
    redde VERUM;
}

interior b32
_clavis_legere (
                 Motor* m,
    constans StmlNodus* nodus,
                Cyclus* c)
{
    hic_manens constans character* constans LICITA[] = {
        "tempus", "positura", "lenitas", NIHIL
    };
    hic_manens constans character* constans LENITATES[] = {
        "linearis", "lenis", "accelerat", "tardat", "tenet"
    };
    constans chorda* tempus = stml_attributum_capere(nodus, "tempus");
    constans chorda* positura =
        stml_attributum_capere(nodus, "positura");
    constans chorda* lenitas =
        stml_attributum_capere(nodus, "lenitas");
             Clavis* clavis;
                f32  t;
                s32  k;
                i32  modus = LENITAS_NULLA;


    si (   !_attributa_licita(m, nodus, LICITA)
        || !_sine_liberis(m, nodus))
    {
        redde FALSUM;
    }
    si (!tempus || !_numeri(tempus, &t, I))
    {
        redde _recusare_attributum(m, nodus, "tempus",
            "needs 1 number (seconds)");
    }
    si (!positura)
    {
        redde _recusare_attributum(m, nodus, "positura",
            "is needed (a pose title, or 'quies')");
    }
    k = _positura(m, positura);
    si (k < ZEPHYRUM)
    {
        redde _recusare(m, nodus->linea,
            "<clavis> names an unknown pose '", positura, "'");
    }
    si (c->clavium >= PLASTICA_MOTUS_CLAVES_MAXIMAE)
    {
        redde _recusare(m, nodus->linea,
            "<clavis> is one key too many (at most 64 in a cycle)",
            NIHIL, NIHIL);
    }
    si (c->clavium == ZEPHYRUM && t != 0.0f)
    {
        redde _recusare(m, nodus->linea,
            "the first key of a cycle must be at tempus 0", NIHIL,
            NIHIL);
    }
    si (c->clavium && !(t > c->claves[c->clavium - I].tempus))
    {
        redde _recusare(m, nodus->linea,
            "key times must be increasing", NIHIL, NIHIL);
    }
    si (t > c->duratio)
    {
        redde _recusare(m, nodus->linea,
            "<clavis> is later than the cycle's 'duratio'", NIHIL,
            NIHIL);
    }
    si (lenitas)
    {
        per (modus = ZEPHYRUM; modus < V; modus++)
        {
            si (_est(lenitas, LENITATES[modus]))
            {
                frange;
            }
        }
        si (modus >= V)
        {
            redde _recusare_attributum(m, nodus, "lenitas",
                "must be 'linearis', 'lenis', 'accelerat', 'tardat'"
                " or 'tenet'");
        }
        modus += LENITAS_LINEARIS;
    }
    clavis            = &c->claves[c->clavium++];
    clavis->positura  = (i32)k;
    clavis->tempus    = t;
    clavis->linea     = nodus->linea;
    clavis->sutura    = FALSUM;
    clavis->lenitas   = modus;
    redde VERUM;
}

interior b32
_momentum_legere (
                 Motor* m,
    constans StmlNodus* nodus,
                Cyclus* c)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", "tempus", NIHIL
    };
    constans chorda* titulus =
        stml_attributum_capere(nodus, "titulus");
    constans chorda* tempus = stml_attributum_capere(nodus, "tempus");
                f32  t;
                i32  k;

    si (   !_attributa_licita(m, nodus, LICITA)
        || !_sine_liberis(m, nodus))
    {
        redde FALSUM;
    }
    si (!titulus)
    {
        redde _recusare_attributum(m, nodus, "titulus",
            "is needed and cannot be empty");
    }
    si (   !tempus || !_numeri(tempus, &t, I) || t < 0.0f
        || t > c->duratio)
    {
        redde _recusare_attributum(m, nodus, "tempus",
            "needs 1 number from 0 to the cycle's 'duratio'");
    }
    per (k = ZEPHYRUM; k < c->momentorum; k++)
    {
        si (chorda_aequalis(c->momenta[k], *titulus))
        {
            redde _recusare(m, nodus->linea, "moment title '",
                titulus, "' is used twice in this cycle");
        }
    }
    si (c->momentorum >= PLASTICA_MOTUS_MOMENTA_MAXIMA)
    {
        redde _recusare(m, nodus->linea, "<momentum> is one moment"
            " too many (at most 8 in a cycle)", NIHIL, NIHIL);
    }
    c->momenta[c->momentorum] = *titulus;
    c->tempora[c->momentorum] = t;
    c->momentorum++;
    redde VERUM;
}

/* elementum mora: articulus tot secundis serius agit */
interior b32
_moram_legere (
                 Motor* m,
    constans StmlNodus* nodus,
                Cyclus* c)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", "tempus", NIHIL
    };
    constans chorda* titulus =
        stml_attributum_capere(nodus, "titulus");
    constans chorda* tempus = stml_attributum_capere(nodus, "tempus");
                f32  t;
                s32  j;

    si (   !_attributa_licita(m, nodus, LICITA)
        || !_sine_liberis(m, nodus))
    {
        redde FALSUM;
    }
    si (!titulus)
    {
        redde _recusare_attributum(m, nodus, "titulus",
            "is needed (a joint title)");
    }
    si (   !tempus || !_numeri(tempus, &t, I) || !(t > 0.0f)
        || !(t < c->duratio))
    {
        redde _recusare_attributum(m, nodus, "tempus",
            "needs 1 number above 0 and below the cycle's 'duratio'");
    }
    j = _articulus(m, titulus);
    si (j < ZEPHYRUM)
    {
        redde _recusare(m, nodus->linea,
            "<mora> names an unknown joint '", titulus, "'");
    }
    si (c->morae[j] > 0.0f)
    {
        redde _recusare(m, nodus->linea, "<mora> names joint '",
            titulus, "' twice in this cycle");
    }
    c->morae[j]           = t;
    c->lineae_morarum[j]  = nodus->linea;
    redde VERUM;
}

interior b32
_cyclus_legere (
                 Motor* m,
    constans StmlNodus* nodus)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", "duratio", "circulus", NIHIL
    };
       constans chorda* titulus =
           stml_attributum_capere(nodus, "titulus");
       constans chorda* duratio =
           stml_attributum_capere(nodus, "duratio");
       constans chorda* circulus =
           stml_attributum_capere(nodus, "circulus");
    constans StmlNodus* liber;
                Cyclus* c;
                Clavis* ultima;
                   b32  circularis = FALSUM;
                   b32  finis;
                   i32  cursor     = ZEPHYRUM;
                   i32  locus;
                   i32  j;
                   i32  k;


    si (!_attributa_licita(m, nodus, LICITA))
    {
        redde FALSUM;
    }
    si (!titulus)
    {
        redde _recusare_attributum(m, nodus, "titulus",
            "is needed and cannot be empty");
    }
    /* figura cyclum ossium eiusdem tituli substituit, loco servato */
    per (locus = ZEPHYRUM; locus < m->cyclorum; locus++)
    {
        si (chorda_aequalis(m->cycli[locus].titulus, *titulus))
        {
            si (m->in_ossibus || !m->cycli[locus].ossium)
            {
                redde _recusare(m, nodus->linea, "cycle title '",
                    titulus, "' is used twice");
            }
            frange;
        }
    }
    si (   locus       >= m->cyclorum
        && m->cyclorum >= PLASTICA_MOTUS_CYCLI_MAXIMI)
    {
        redde _recusare(m, nodus->linea,
            "<cyclus> is one cycle too many (at most 16)", NIHIL,
            NIHIL);
    }
    c = &m->cycli[locus];
    memset(c, ZEPHYRUM, magnitudo(*c));
    c->titulus  = *titulus;
    c->linea    = nodus->linea;
    c->ossium   = m->in_ossibus;
    si (   !duratio || !_numeri(duratio, &c->duratio, I)
        || !(c->duratio > 0.0f) || c->duratio > DURATIO_MAXIMA)
    {
        redde _recusare_attributum(m, nodus, "duratio",
            "needs 1 number above 0, at most 60 (seconds)");
    }
    si (circulus)
    {
        si (_est(circulus, "verum"))
        {
            circularis = VERUM;
        }
        alioquin si (!_est(circulus, "falsum"))
        {
            redde _recusare_attributum(m, nodus, "circulus",
                "must be 'verum' or 'falsum'");
        }
    }
    dum ((liber = _liberum(m, nodus, &cursor, &finis)) != NIHIL)
    {
        si (_est(liber->titulus, "clavis"))
        {
            si (!_clavis_legere(m, liber, c))
            {
                redde FALSUM;
            }
        }
        alioquin si (_est(liber->titulus, "momentum"))
        {
            si (!_momentum_legere(m, liber, c))
            {
                redde FALSUM;
            }
        }
        alioquin si (_est(liber->titulus, "mora"))
        {
            si (!_moram_legere(m, liber, c))
            {
                redde FALSUM;
            }
        }
        alioquin
        {
            redde _recusare(m, liber->linea, "<cyclus> holds only"
                " <clavis>, <momentum> and <mora>, got <",
                liber->titulus, ">");
        }
    }
    si (!finis)
    {
        redde FALSUM;
    }
    si (c->clavium == ZEPHYRUM)
    {
        redde _recusare(m, nodus->linea, "<cyclus> has no keys",
            NIHIL, NIHIL);
    }
    /* clavis ad durationem: circulus primam iterat, aliter ultima
     * tenetur */
    ultima         = &c->claves[c->clavium - I];
    c->circularis  = circularis;
    si (!circularis && ultima->lenitas != LENITAS_NULLA)
    {
        redde _recusare(m, ultima->linea, "'lenitas' on the last key"
            " of a cycle that does not loop: nothing follows this"
            " key", NIHIL, NIHIL);
    }
    /* mora articuli quem nulla positura cycli tangit */
    per (j = ZEPHYRUM; j < m->articulorum; j++)
    {
        i32 tacta = ZEPHYRUM;

        per (k = ZEPHYRUM; k < c->clavium; k++)
        {
            tacta |=
                m->positurae[c->claves[k].positura].artus[j].tacta;
        }
        si (c->morae[j] > 0.0f && !tacta)
        {
            redde _recusare(m, c->lineae_morarum[j],
                "<mora> names joint '", &m->partes[m->articuli[j]],
                "' is not moved in this cycle");
        }
    }
    si (circularis)
    {
        si (!(ultima->tempus < c->duratio))
        {
            redde _recusare(m, ultima->linea, "in a looping cycle the"
                " last key must come before 'duratio' (the first pose"
                " is repeated there)", NIHIL, NIHIL);
        }
        c->claves[c->clavium].positura =
            c->claves[ZEPHYRUM].positura;
    }
    alioquin
    {
        c->claves[c->clavium].positura = ultima->positura;
    }
    si (circularis || ultima->tempus < c->duratio)
    {
        c->claves[c->clavium].tempus  = c->duratio;
        c->claves[c->clavium].linea   = nodus->linea;
        c->claves[c->clavium].sutura  = circularis;

        c->clavium++;
    }
    si (!_vias_probare(m, c))
    {
        redde FALSUM;
    }
    si (locus >= m->cyclorum)
    {
        m->cyclorum++;
    }
    redde VERUM;
}


/* ==================================================
 * Coctio
 * ================================================== */

/* instans coctionis: clavis ipsa (pondus 0) aut punctum sumptum in
 * spatio a clave ad sequentem */
nomen structura {
    i32 clavis;
    f32 pondus;
    f32 tempus;
} Instans;

/* pondus lenitum: 0 .. 1 in 0 .. 1 */
interior f32
_lenire (
    i32 lenitas,
    f32 u)
{
    si (lenitas == LENITAS_LENIS)
    {
        redde u * u * (3.0f - 2.0f * u);
    }
    si (lenitas == LENITAS_ACCELERAT)
    {
        redde u * u;
    }
    si (lenitas == LENITAS_TARDAT)
    {
        redde 1.0f - (1.0f - u) * (1.0f - u);
    }
    redde u;
}

/* instantia cycli: claves omnes; spatium lenitum praeterea ad
 * PLASTICA_MOTUS_FREQUENTIA per secundam a clave sua; spatium
 * tentum clavem unam paulo ante sequentem */
interior b32
_instantia_parare (
              Motor*  m,
    constans Cyclus*  c,
            Instans** exitus,
                i32*  numerus)
{
    Instans* instantia = (Instans*)piscina_allocare(m->piscina,
        (memoriae_index)(c->clavium * III
            + (i32)(c->duratio * (f32)PLASTICA_MOTUS_FREQUENTIA) + IV)
            * magnitudo(Instans));
        i32 n = ZEPHYRUM;
        i32 k;
        i32 j;

    si (!instantia)
    {
        redde _recusare(m, ZEPHYRUM, "out of memory", NIHIL, NIHIL);
    }
    per (k = ZEPHYRUM; k < c->clavium; k++)
    {
        f32 initium   = c->claves[k].tempus;
        f32 spatium;
        i32 lenitas   = c->claves[k].lenitas;

        instantia[n].clavis = k;
        instantia[n].pondus = 0.0f;
        instantia[n].tempus = initium;
        n++;
        si (k + I >= c->clavium)
        {
            frange;
        }
        spatium = c->claves[k + I].tempus - initium;
        si (lenitas == LENITAS_TENET)
        {
            si (spatium > 2.0f * PLASTICA_MOTUS_TENET_MARGO)
            {
                instantia[n].clavis = k;
                instantia[n].pondus = 0.0f;
                instantia[n].tempus  =
                    c->claves[k + I].tempus
                        - PLASTICA_MOTUS_TENET_MARGO;
                n++;
            }
            perge;
        }
        si (lenitas == LENITAS_NULLA || lenitas == LENITAS_LINEARIS)
        {
            perge;
        }
        per (j = I; ; j++)
        {
            f32 t = (f32)j / (f32)PLASTICA_MOTUS_FREQUENTIA;

            si (!(t < spatium - INSTANTIS_MARGO))
            {
                frange;
            }
            instantia[n].clavis = k;
            instantia[n].pondus = _lenire(lenitas, t / spatium);
            instantia[n].tempus = initium + t;
            n++;
        }
    }
    *exitus   = instantia;
    *numerus  = n;
    redde VERUM;
}

/* valor articuli j in clave k: rotatio IV, aliter III */
interior vacuum
_valor_clavis (
     constans Motor* m,
    constans Cyclus* c,
                i32  j,
                i32  tactum,
                i32  k,
                f32* v)
{
    constans StatuaNodus* nodus  = &m->basis->nodi[m->articuli[j]];
          constans Artus* a      =
              &m->positurae[c->claves[k].positura].artus[j];

    si (tactum == TACTA_ROTATIO)
    {
        StereometriaQuaternio q =
            _rotatio_articuli(nodus->rotatio, a->rotatio);

        v[ZEPHYRUM]  = q.x;
        v[I]         = q.y;
        v[II]        = q.z;
        v[III]       = q.w;
    }
    alioquin si (tactum == TACTA_SCALA)
    {
        b32 tacta = (a->tacta & TACTA_SCALA) != ZEPHYRUM;

        v[ZEPHYRUM]  = nodus->scala.x
            * (tacta ? a->scala[ZEPHYRUM] : 1.0f);
        v[I]   = nodus->scala.y * (tacta ? a->scala[I] : 1.0f);
        v[II]  = nodus->scala.z * (tacta ? a->scala[II] : 1.0f);
    }
    alioquin
    {
        v[ZEPHYRUM]  = nodus->translatio.x + a->motus[ZEPHYRUM];
        v[I]         = nodus->translatio.y + a->motus[I];
        v[II]        = nodus->translatio.z + a->motus[II];
    }
}

/* inter valores duos: rotatio per sphaeram, aliter linealiter */
interior vacuum
_miscere (
             i32  tactum,
    constans f32* a,
    constans f32* b,
             f32  pondus,
             f32* exitus)
{
    si (tactum == TACTA_ROTATIO)
    {
        StereometriaQuaternio p;
        StereometriaQuaternio q;

        p.x               = a[ZEPHYRUM];
        p.y               = a[I];
        p.z               = a[II];
        p.w               = a[III];
        q.x               = b[ZEPHYRUM];
        q.y               = b[I];
        q.z               = b[II];
        q.w               = b[III];
        p                 = stereometria_quaternio_slerp(p, q, pondus);
        exitus[ZEPHYRUM]  = p.x;
        exitus[I]         = p.y;
        exitus[II]        = p.z;
        exitus[III]       = p.w;
    }
    alioquin
    {
        i32 k;

        per (k = ZEPHYRUM; k < III; k++)
        {
            exitus[k] = a[k] + (b[k] - a[k]) * pondus;
        }
    }
}

/* valor canalis cocti ad tempus (inter claves eius) */
interior vacuum
_aestimare (
    constans StatuaCanalis* canalis,
                       i32  tactum,
                       f32  tempus,
                       f32* exitus)
{
    i32 lata  = tactum == TACTA_ROTATIO ? IV : III;
    i32 k     = ZEPHYRUM;

    dum (   k + II < canalis->clavium
         && canalis->tempora[k + I] <= tempus)
    {
        k++;
    }
    _miscere(tactum, canalis->valores + k * lata,
        canalis->valores + (k + I) * lata,
        (tempus - canalis->tempora[k])
            / (canalis->tempora[k + I] - canalis->tempora[k]),
        exitus);
}

/* canalem tot secundis serius: claves eaedem motae. Circulus:
 * trans suturam, cum valore ad 0 et ad durationem qui 'mora' ante
 * suturam erat. Cyclus semel: positura prima exspectat, claves ultra
 * durationem omittuntur */
interior b32
_moram_applicare (
              Motor* m,
    constans Cyclus* c,
                i32  tactum,
                f32  mora,
      StatuaCanalis* canalis)
{
     i32  lata     = tactum == TACTA_ROTATIO ? IV : III;
     i32  veteres  = canalis->clavium;
     f32* tempora   = (f32*)piscina_allocare(m->piscina,
         (memoriae_index)(veteres + II) * magnitudo(f32));
     f32* valores   = (f32*)piscina_allocare(m->piscina,
         (memoriae_index)(veteres + II) * lata * magnitudo(f32));
     f32 finis     = c->duratio;
     f32 ultimus[IV];
     i32 n         = ZEPHYRUM;
     i32 k;
     i32 transitus;


    si (!tempora || !valores)
    {
        redde _recusare(m, ZEPHYRUM, "out of memory", NIHIL, NIHIL);
    }
    /* valor ad durationem; in circulo etiam ad 0 (sutura clausa) */
    _aestimare(canalis, tactum, finis - mora, ultimus);
    tempora[n] = 0.0f;
    memcpy(valores, c->circularis ? ultimus : canalis->valores,
        lata * magnitudo(f32));
    n++;
    /* transitus 0: claves trans suturam latae (circulus solus);
     * transitus I: ceterae. Clavis ultima ad durationem stat: mota
     * semper extra cadit */
    per (transitus = c->circularis ? ZEPHYRUM : I; transitus < II;
        transitus++)
    {
        per (k = ZEPHYRUM; k + I < veteres; k++)
        {
            f32 t = canalis->tempora[k] + mora
                - (transitus ? 0.0f : finis);

            si (t > SUTURAE_MARGO && t < finis - SUTURAE_MARGO)
            {
                tempora[n] = t;
                memcpy(valores + n * lata,
                    canalis->valores + k * lata,
                    lata * magnitudo(f32));
                n++;
            }
        }
    }
    tempora[n] = finis;
    memcpy(valores + n * lata, ultimus, lata * magnitudo(f32));
    n++;
    canalis->clavium = n;
    canalis->tempora = tempora;
    canalis->valores = valores;
    redde VERUM;
}

/* canalis unus: articulus j, proprietas, clavis ad quodque instans */
interior b32
_canalem_coquere (
                Motor* m,
      constans Cyclus* c,
     constans Instans* instantia,
                  i32  numerus,
                  i32  j,
                  i32  tactum,
        StatuaCanalis* canalis)
{
    i32 lata = tactum == TACTA_ROTATIO ? IV : III;
    i32 k;

    canalis->nodus       = (s32)m->articuli[j];
    canalis->proprietas  = tactum == TACTA_ROTATIO ? STATUA_ROTATIO
        : tactum == TACTA_SCALA ? STATUA_SCALA : STATUA_TRANSLATIO;
    canalis->gradatim  = FALSUM;
    canalis->clavium   = numerus;
    canalis->tempora     = (f32*)piscina_allocare(m->piscina,
        numerus * magnitudo(f32));
    canalis->valores     = (f32*)piscina_allocare(m->piscina,
        numerus * lata * magnitudo(f32));
    si (!canalis->tempora || !canalis->valores)
    {
        redde _recusare(m, ZEPHYRUM, "out of memory", NIHIL, NIHIL);
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        f32* v = canalis->valores + k * lata;

        canalis->tempora[k] = instantia[k].tempus;
        _valor_clavis(m, c, j, tactum, instantia[k].clavis, v);
        si (instantia[k].pondus > 0.0f)
        {
            f32 sequens[IV];

            _valor_clavis(m, c, j, tactum, instantia[k].clavis + I,
                sequens);
            _miscere(tactum, v, sequens, instantia[k].pondus, v);
        }
    }
    si (c->morae[j] > 0.0f)
    {
        redde _moram_applicare(m, c, tactum, c->morae[j], canalis);
    }
    redde VERUM;
}

/* cyclum in canales: articulus quem positura cycli tangit canalem
 * pro quaque proprietate tacta accipit */
interior b32
_cyclum_coquere (
               Motor* m,
     constans Cyclus* c,
        StatuaCyclus* exitus)
{
    hic_manens constans i32 TACTA[] = {
        TACTA_ROTATIO, TACTA_SCALA, TACTA_MOTUS
    };
    Instans* instantia;
        i32  numerus;
        i32  tacta[PLASTICA_ARTICULI_MAXIMI];
        i32  canalium  = ZEPHYRUM;
        i32  n         = ZEPHYRUM;
        i32  j;
        i32  k;
        i32  a;

    si (!_instantia_parare(m, c, &instantia, &numerus))
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < m->articulorum; j++)
    {
        tacta[j] = ZEPHYRUM;
        per (k = ZEPHYRUM; k < c->clavium; k++)
        {
            tacta[j] |=
                m->positurae[c->claves[k].positura].artus[j].tacta;
        }
        per (a = ZEPHYRUM; a < III; a++)
        {
            si (tacta[j] & TACTA[a])
            {
                canalium++;
            }
        }
    }
    exitus->titulus   = chorda_transcribere(c->titulus, m->piscina);
    exitus->duratio   = c->duratio;
    exitus->canalium  = canalium;
    exitus->canales   = (StatuaCanalis*)piscina_allocare(m->piscina,
        (canalium ? canalium : I) * magnitudo(StatuaCanalis));
    si (!exitus->canales)
    {
        redde _recusare(m, ZEPHYRUM, "out of memory", NIHIL, NIHIL);
    }
    per (j = ZEPHYRUM; j < m->articulorum; j++)
    {
        per (a = ZEPHYRUM; a < III; a++)
        {
            si (   (tacta[j] & TACTA[a])
                && !_canalem_coquere(m, c, instantia, numerus, j,
                TACTA[a],
                    &exitus->canales[n++]))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}


/* ==================================================
 * Publica
 * ================================================== */

interior PlasticaFiguraFructus
_recusatus (
    constans Motor* m,
           Piscina* piscina)
{
    PlasticaFiguraFructus f;

    memset(&f, ZEPHYRUM, magnitudo(f));
    f.error = chorda_ex_literis(m->error, piscina);
    f.linea = m->linea;
    si (m->plagula_erroris.mensura)
    {
        f.plagula = chorda_transcribere(m->plagula_erroris, piscina);
    }
    redde f;
}

/* elementa motus sub radice legere */
interior b32
_motum_legere (
                 Motor* m,
    constans StmlNodus* radix)
{
    constans StmlNodus* liber;
                   b32  finis;
                   i32  cursor = ZEPHYRUM;

    dum ((liber = _liberum(m, radix, &cursor, &finis)) != NIHIL)
    {
        si (_est(liber->titulus, "positura"))
        {
            si (!_positura_legere(m, liber))
            {
                redde FALSUM;
            }
        }
        alioquin si (_est(liber->titulus, "cyclus"))
        {
            si (!_cyclus_legere(m, liber))
            {
                redde FALSUM;
            }
        }

    }
    redde finis;
}


/* ==================================================
 * Ossa
 * ================================================== */

/* articulum ossium et liberos eius colligere: titulus, unicus, soli
 * articuli intus. Attributa cetera plastica iudicat */
interior b32
_os_probare (
                 Motor* m,
    constans StmlNodus* nodus)
{
       constans chorda* titulus =
           stml_attributum_capere(nodus, "titulus");
    constans StmlNodus* liber;
                   b32  finis;
                   i32  cursor = ZEPHYRUM;
                   i32  k;

    si (!titulus)
    {
        redde _recusare(m, nodus->linea,
            "<articulus> needs a non-empty attribute 'titulus'", NIHIL,
            NIHIL);
    }
    per (k = ZEPHYRUM; k < m->ossium; k++)
    {
        si (chorda_aequalis(
            *stml_attributum_capere(m->ossa[k], "titulus"), *titulus))
        {
            redde _recusare(m, nodus->linea, "joint title '", titulus,
                "' is used twice");
        }
    }
    si (m->ossium >= PLASTICA_ARTICULI_MAXIMI)
    {
        redde _recusare(m, nodus->linea,
            "<articulus> is one joint too many (at most 64)", NIHIL,
            NIHIL);
    }
    m->ossa[m->ossium++] = nodus;
    dum ((liber = _liberum(m, nodus, &cursor, &finis)) != NIHIL)
    {
        si (!_est(liber->titulus, "articulus"))
        {
            redde _recusare(m, liber->linea, "a skeleton joint holds"
                " only joints (solids belong to the figure), got <",
                liber->titulus, ">");
        }
        si (!_os_probare(m, liber))
        {
            redde FALSUM;
        }
    }
    redde finis;
}

/* plagula ossium: radix <ossa titulus>, articuli, positurae, cycli */
interior b32
_ossa_probare (
                 Motor* m,
    constans StmlNodus* radix)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", NIHIL
    };
    constans StmlNodus* liber;
                   b32  finis;
                   i32  cursor = ZEPHYRUM;

    si (!_est(radix->titulus, "ossa"))
    {
        redde _recusare(m, radix->linea,
            "a skeleton sheet needs the root element <ossa>, got <",
            radix->titulus, ">");
    }
    si (!_attributa_licita(m, radix, LICITA))
    {
        redde FALSUM;
    }
    si (!stml_attributum_capere(radix, "titulus"))
    {
        redde _recusare_attributum(m, radix, "titulus",
            "is needed and cannot be empty");
    }
    dum ((liber = _liberum(m, radix, &cursor, &finis)) != NIHIL)
    {
        si (_est(liber->titulus, "articulus"))
        {
            si (!_os_probare(m, liber))
            {
                redde FALSUM;
            }
        }
        alioquin si (   !_est(liber->titulus, "positura")
                     && !_est(liber->titulus, "cyclus")
                     && !_est(liber->titulus, "membrum"))
        {
            redde _recusare(m, liber->linea, "<ossa> holds only"
                " joints, poses and cycles (solids belong to the"
                " figure), got <", liber->titulus, ">");
        }
    }
    redde finis;
}

/* articuli plani figurae: quisque articulum ossium nominat */
interior b32
_figuram_probare (
                 Motor* m,
    constans StmlNodus* radix)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", "locus", NIHIL
    };
    constans StmlNodus* liber;
                   b32  finis;
                   i32  cursor = ZEPHYRUM;

    dum ((liber = _liberum(m, radix, &cursor, &finis)) != NIHIL)
    {
           constans chorda* titulus;
           constans chorda* locus;
        constans StmlNodus* intus;
                       f32  numeri[III];
                       b32  finis_intus;
                       i32  cursor_intus = ZEPHYRUM;
                       i32  k;

        si (!_est(liber->titulus, "articulus"))
        {
            perge;
        }
        si (stml_attributum_capere(liber, "gyrus"))
        {
            redde _recusare_attributum(m, liber, "gyrus", "belongs to"
                " the skeleton (a figure may only give 'locus')");
        }
        si (stml_attributum_capere(liber, "rotatio"))
        {
            redde _recusare_attributum(m, liber, "rotatio", "belongs"
                " to the skeleton (a figure may only give 'locus')");
        }
        si (!_attributa_licita(m, liber, LICITA))
        {
            redde FALSUM;
        }
        titulus  = stml_attributum_capere(liber, "titulus");
        locus    = stml_attributum_capere(liber, "locus");
        si (!titulus)
        {
            redde _recusare_attributum(m, liber, "titulus",
                "is needed (a joint of the skeleton)");
        }
        per (k = ZEPHYRUM; k < m->ossium; k++)
        {
            si (chorda_aequalis(
                *stml_attributum_capere(m->ossa[k], "titulus"),
                *titulus))
            {
                frange;
            }
        }
        si (k >= m->ossium)
        {
            redde _recusare(m, liber->linea,
                "<articulus> names a joint the skeleton lacks: '",
                titulus, "'");
        }
        si (m->solida[k])
        {
            redde _recusare(m, liber->linea, "joint '", titulus,
                "' appears twice in this figure");
        }
        si (locus && !_numeri(locus, numeri, III))
        {
            redde _recusare_attributum(m, liber, "locus",
                "needs 3 numbers (x y z)");
        }
        m->solida[k] = liber;
        dum ((intus = _liberum(m, liber, &cursor_intus, &finis_intus))
            != NIHIL)
        {
            si (_est(intus->titulus, "articulus"))
            {
                redde _recusare(m, intus->linea, "joints are not"
                    " nested in a figure with a skeleton (the"
                    " skeleton holds the tree)", NIHIL, NIHIL);
            }
        }
        si (!finis_intus)
        {
            redde FALSUM;
        }
    }
    redde finis;
}

/* attributa 'ex' in 'ad' transcribere, praeter unum (NIHIL: omnia) */
interior b32
_attributa_transcribere (
                  Motor* m,
    InternamentumChorda* intern,
              StmlNodus* ad,
     constans StmlNodus* ex,
     constans character* praeter)
{
    i32 numerus = ex->attributa ? xar_numerus(ex->attributa)
        : ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans StmlAttributum* a =
            (constans StmlAttributum*)xar_obtinere(ex->attributa, k);
                      character titulus[TITULI_MENSURA];
                         chorda valor;
                            i32 mensura;

        si (!a || !a->titulus || (praeter && _est(a->titulus, praeter)))
        {
            perge;
        }
        mensura = a->titulus->mensura < TITULI_MENSURA - I
            ? a->titulus->mensura : TITULI_MENSURA - I;
        memcpy(titulus, a->titulus->datum, mensura);
        titulus[mensura] = '\0';
        memset(&valor, ZEPHYRUM, magnitudo(valor));
        si (a->valor)
        {
            valor = *a->valor;
        }
        si (!stml_attributum_addere_chorda(ad, m->piscina, intern,
            titulus, valor))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* elementa libera 'ex' in 'ad' addere (nodi ipsi, non exemplaria).
 * 'radicis': articuli praetereuntur (ex ossibus veniunt); elementa
 * motus plastica ipsa praeterit */
interior b32
_elementa_addere (
             StmlNodus* ad,
    constans StmlNodus* ex,
                   b32  radicis)
{
    i32 numerus = ex->liberi ? xar_numerus(ex->liberi) : ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        StmlNodus** locus =
            (StmlNodus**)xar_obtinere(ex->liberi, k);
         StmlNodus* liber = locus ? *locus : NIHIL;

        si (!liber || liber->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (radicis && _est(liber->titulus, "articulus"))
        {
            perge;
        }
        si (!stml_liberum_addere(ad, liber))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* articulus ossium in arborem compositam: attributa ossium ('locus'
 * figurae si datus), solida figurae, deinde articuli liberi. *index
 * per arborem ossium procedit, ordine quo collecta sunt */
interior StmlNodus*
_os_componere (
                  Motor* m,
    InternamentumChorda* intern,
     constans StmlNodus* os,
                    i32* index)
{
    constans StmlNodus* figurae = m->solida[*index];
       constans chorda* locus   = figurae
           ? stml_attributum_capere(figurae, "locus") : NIHIL;
             StmlNodus* novus   =
                 stml_elementum_creare(m->piscina, intern, "articulus");
                   i32 numerus = os->liberi ? xar_numerus(os->liberi)
                       : ZEPHYRUM;
                   i32 k;

    (*index)++;
    si (!novus)
    {
        redde NIHIL;
    }
    novus->linea = os->linea | LINEA_OSSIUM;
    si (   !_attributa_transcribere(m, intern, novus, os,
               locus ? "locus" : NIHIL)
        || (locus && !stml_attributum_addere_chorda(novus, m->piscina,
               intern, "locus", *locus))
        || (figurae && !_elementa_addere(novus, figurae, FALSUM)))
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
         StmlNodus** sedes = (StmlNodus**)xar_obtinere(os->liberi, k);
         StmlNodus*  liber = sedes ? *sedes : NIHIL;
         StmlNodus*  filius;

        si (!liber || liber->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        filius = _os_componere(m, intern, liber, index);
        si (!filius || !stml_liberum_addere(novus, filius))
        {
            redde NIHIL;
        }
    }
    redde novus;
}

/* plastica arborem compositam recusavit: nuntius eius, linea sine
 * signo, plagula ossium si linea signum ferebat */
interior b32
_corpus_recusare (
                     Motor* m,
    constans PlasticaFructus* corpus,
           constans chorda* titulus)
{
    i32 longitudo = ZEPHYRUM;

    m->fractum  = VERUM;
    m->linea    = corpus->linea & ~LINEA_OSSIUM;
    si (corpus->linea & LINEA_OSSIUM)
    {
        m->plagula_erroris = chorda_transcribere(*titulus, m->piscina);
    }
    _appone(m, &longitudo, (constans character*)corpus->error.datum,
        corpus->error.mensura);
    redde FALSUM;
}

/* arbor ossium in radicem addere (solida figurae si iam collecta) */
interior b32
_ossa_addere (
                  Motor* m,
    InternamentumChorda* intern,
              StmlNodus* radix,
     constans StmlNodus* ossa)
{
    i32 numerus  = ossa->liberi ? xar_numerus(ossa->liberi) : ZEPHYRUM;
    i32 index    = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
         StmlNodus** sedes = (StmlNodus**)xar_obtinere(ossa->liberi, k);
         StmlNodus*  liber = sedes ? *sedes : NIHIL;
         StmlNodus*  os;

        si (   !liber || liber->genus != STML_NODUS_ELEMENTUM
            || !_est(liber->titulus, "articulus"))
        {
            perge;
        }
        os = _os_componere(m, intern, liber, &index);
        si (!os || !stml_liberum_addere(radix, os))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* ossa sola ut statua (cum solido uno ficto: plastica figuram sine
 * solidis recusat): quies ossium pro factoribus et finibus
 * membrorum. Ante articulos figurae collectos vocanda */
interior b32
_quietem_ossium_struere (
                  Motor* m,
    InternamentumChorda* intern,
     constans StmlNodus* ossa,
        constans chorda* titulus)
{
    PlasticaFructus  corpus;
          StmlNodus* radix =
              stml_elementum_creare(m->piscina, intern, "figura");
          StmlNodus* solidum =
              stml_elementum_creare(m->piscina, intern, "capsa");

    si (   !radix || !solidum
        || !stml_attributum_addere(radix, m->piscina, intern,
               "titulus", "ossa")
        || !stml_attributum_addere(solidum, m->piscina, intern,
               "magnitudo", "1 1 1")
        || !stml_liberum_addere(radix, solidum)
        || !_ossa_addere(m, intern, radix, ossa))
    {
        redde _recusare(m, ZEPHYRUM, "out of memory", NIHIL, NIHIL);
    }
    corpus = plastica_legere_arboris(radix, ZEPHYRUM, m->piscina);
    si (!corpus.successus)
    {
        redde _corpus_recusare(m, &corpus, titulus);
    }
    m->quies_ossium     = corpus.statua;
    m->articuli_ossium  = corpus.articuli;
    redde VERUM;
}

/* corpus figurae cum ossibus: plagulam ossium a fonte petere, utramque
 * probare, arborem componere, plasticae tradere. *ossa = radix ossium
 * (positurae et cycli eius postea leguntur) */
interior b32
_corpus_ex_ossibus (
                   Motor*  m,
               StmlNodus*  figura,
         constans chorda*  titulus,
            PlasticaFons   fons,
                  vacuum*  contextus,
                     i32   semen,
     InternamentumChorda*  intern,
         PlasticaFructus*  corpus,
      constans StmlNodus** ossa)
{
    StmlResultus  r;
       StmlNodus* radix;
          chorda  textus;
             i32  k;

    si (!fons)
    {
        redde _recusare(m, figura->linea, "<figura> names the"
            " skeleton '", titulus,
            "': skeleton sheets need a source");
    }
    per (k = ZEPHYRUM; k < titulus->mensura; k++)
    {
        i8 littera = titulus->datum[k];

        si (   !(littera >= 'a' && littera <= 'z')
            && !(littera >= '0' && littera <= '9') && littera != '_')
        {
            redde _recusare_attributum(m, figura, "ossa",
                "may only hold a-z, 0-9 and _");
        }
    }
    textus = fons(*titulus, contextus);
    si (!textus.mensura)
    {
        redde _recusare(m, figura->linea, "unknown skeleton '",
            titulus, "'");
    }
    m->plagula  = chorda_transcribere(*titulus, m->piscina);
    r           = stml_legere(textus, m->piscina, intern);
    si (!r.successus || !r.elementum_radix)
    {
        redde _recusare(m, r.successus ? ZEPHYRUM : r.linea_erroris,
            "the skeleton sheet is not valid STML", NIHIL, NIHIL);
    }
    *ossa = r.elementum_radix;
    si (   !_ossa_probare(m, *ossa)
        || !_quietem_ossium_struere(m, intern, *ossa, titulus))
    {
        redde FALSUM;
    }
    memset(&m->plagula, ZEPHYRUM, magnitudo(m->plagula));
    si (!_figuram_probare(m, figura))
    {
        redde FALSUM;
    }

    /* arbor composita: radix figurae (sine 'ossa'), elementa eius
     * quae articuli non sunt, deinde arbor ossium */
    radix = stml_elementum_creare(m->piscina, intern, "figura");
    si (   !radix
        || !_attributa_transcribere(m, intern, radix, figura, "ossa")
        || !_elementa_addere(radix, figura, VERUM))
    {
        redde _recusare(m, ZEPHYRUM, "out of memory", NIHIL, NIHIL);
    }
    radix->linea = figura->linea;
    si (!_ossa_addere(m, intern, radix, *ossa))
    {
        redde _recusare(m, ZEPHYRUM, "out of memory", NIHIL, NIHIL);
    }
    *corpus = plastica_legere_arboris(radix, semen, m->piscina);
    si (!corpus->successus)
    {
        redde _corpus_recusare(m, corpus, titulus);
    }
    redde VERUM;
}

/* factor motus cuiusque articuli: altitudo in figura per altitudinem
 * in ossibus (ossium 0: I). Solus articulus radicalis motum fert */
interior vacuum
_factores_parare (
    Motor* m)
{
    i32 k;

    per (k = ZEPHYRUM; k < m->articulorum; k++)
    {
        f32 altitudo = m->quies_ossium
            ->nodi[m->articuli_ossium[k]].translatio.y;

        m->factor[k] = 1.0f;
        si (altitudo != 0.0f)
        {
            m->factor[k] =
                m->basis->nodi[m->articuli[k]].translatio.y
                / altitudo;
        }
    }
}

PlasticaFiguraFructus
plastica_figura_legere (
     chorda  textus,
        i32  semen,
    Piscina* piscina)
{
    redde plastica_figura_legere_cum_fonte(textus, semen, NIHIL,
        NIHIL, piscina);
}

PlasticaFiguraFructus
plastica_figura_legere_cum_fonte (
          chorda  textus,
             i32  semen,
    PlasticaFons  fons,
          vacuum* contextus,
         Piscina* piscina)
{
    PlasticaFiguraFructus  f;
          PlasticaFructus  corpus;
                    Motor* m;
      InternamentumChorda* intern;
             StmlResultus  r;
       constans StmlNodus* ossa     = NIHIL;
          constans chorda* titulus  = NIHIL;
                   Statua* s;
                      i32  momentorum = ZEPHYRUM;
                      i32  k;
                      i32  j;
                      i32  n;

    memset(&f, ZEPHYRUM, magnitudo(f));
    m = (Motor*)piscina_allocare(piscina, magnitudo(Motor));
    si (!m)
    {
        f.error = chorda_ex_literis("out of memory", piscina);
        redde f;
    }
    memset(m, ZEPHYRUM, magnitudo(Motor));
    m->piscina  = piscina;
    intern      = internamentum_creare(piscina);
    r           = stml_legere(textus, piscina, intern);
    si (   r.successus && r.elementum_radix
        && _est(r.elementum_radix->titulus, "figura"))
    {
        titulus = stml_attributum_capere(r.elementum_radix, "ossa");
    }
    si (titulus)
    {
        si (!_corpus_ex_ossibus(m, r.elementum_radix, titulus, fons,
            contextus, semen, intern, &corpus, &ossa))
        {
            redde _recusatus(m, piscina);
        }
    }
    alioquin
    {
        corpus = plastica_legere_seminis(textus, semen, piscina);
        si (!corpus.successus)
        {
            f.error = corpus.error;
            f.linea = corpus.linea;
            redde f;
        }
        si (!r.successus || !r.elementum_radix)
        {
            (vacuum)_recusare(m, ZEPHYRUM,
                "the description is not valid STML", NIHIL, NIHIL);
            redde _recusatus(m, piscina);
        }
        si (!_est(r.elementum_radix->titulus, "figura"))
        {
            (vacuum)_recusare(m, r.elementum_radix->linea,
                "a figure needs the root element <figura>, got <",
                r.elementum_radix->titulus, ">");
            redde _recusatus(m, piscina);
        }
    }
    m->basis        = corpus.statua;
    m->partes       = corpus.partes;
    m->articuli     = corpus.articuli;
    m->articulorum  = corpus.articulorum;
    m->cycli        = (Cyclus*)piscina_allocare(piscina,
        PLASTICA_MOTUS_CYCLI_MAXIMI * magnitudo(Cyclus));
    /* positura "quies": nihil tangit */
    m->positurae[ZEPHYRUM].titulus =
        chorda_ex_literis("quies", piscina);
    m->positurae[ZEPHYRUM].artus = (Artus*)piscina_allocare(piscina,
        (m->articulorum ? m->articulorum : I) * magnitudo(Artus));
    m->positurae[ZEPHYRUM].metae = (Meta*)piscina_allocare(piscina,
        PLASTICA_MOTUS_MEMBRA_MAXIMA * magnitudo(Meta));
    s = (Statua*)piscina_allocare(piscina, magnitudo(Statua));
    si (   !m->cycli || !m->positurae[ZEPHYRUM].artus
        || !m->positurae[ZEPHYRUM].metae || !s)
    {
        (vacuum)_recusare(m, ZEPHYRUM, "out of memory", NIHIL, NIHIL);
        redde _recusatus(m, piscina);
    }
    memset(m->positurae[ZEPHYRUM].artus, ZEPHYRUM,
        (m->articulorum ? m->articulorum : I) * magnitudo(Artus));
    memset(m->positurae[ZEPHYRUM].metae, ZEPHYRUM,
        PLASTICA_MOTUS_MEMBRA_MAXIMA * magnitudo(Meta));
    m->positurarum = I;
    si (ossa)
    {
        /* membra et motus ossium primum, in plagula sua */
        _factores_parare(m);
        m->plagula     = chorda_transcribere(*titulus, piscina);
        m->in_ossibus  = VERUM;
        si (!_membra_legere(m, ossa) || !_motum_legere(m, ossa))
        {
            redde _recusatus(m, piscina);
        }
        m->in_ossibus = FALSUM;
        memset(&m->plagula, ZEPHYRUM, magnitudo(m->plagula));
    }
    si (   !_membra_legere(m, r.elementum_radix)
        || !_motum_legere(m, r.elementum_radix))
    {
        redde _recusatus(m, piscina);
    }
    /* positura substituta: viae cyclorum iterum probantur (cyclus
     * ante substitutionem lectus posituram veterem vidit), quisque
     * in plagula sua */
    per (k = ZEPHYRUM; m->substitutum && k < m->cyclorum; k++)
    {
        memset(&m->plagula, ZEPHYRUM, magnitudo(m->plagula));
        si (m->cycli[k].ossium)
        {
            m->plagula = chorda_transcribere(*titulus, piscina);
        }
        si (!_vias_probare(m, &m->cycli[k]))
        {
            redde _recusatus(m, piscina);
        }
    }
    memset(&m->plagula, ZEPHYRUM, magnitudo(m->plagula));

    /* statua nova: valor prioris cum cyclis; tabulae mutuatae */
    *s           = *corpus.statua;
    s->cyclorum  = m->cyclorum;
    s->cycli      = (StatuaCyclus*)piscina_allocare(piscina,
        (m->cyclorum ? m->cyclorum : I) * magnitudo(StatuaCyclus));
    si (!s->cycli)
    {
        (vacuum)_recusare(m, ZEPHYRUM, "out of memory", NIHIL, NIHIL);
        redde _recusatus(m, piscina);
    }
    per (k = ZEPHYRUM; k < m->cyclorum; k++)
    {
        si (!_cyclum_coquere(m, &m->cycli[k], &s->cycli[k]))
        {
            redde _recusatus(m, piscina);
        }
        momentorum += m->cycli[k].momentorum;
    }
    si (!statua_finire(s, piscina))
    {
        (vacuum)_recusare(m, ZEPHYRUM,
            "the model could not be finished", NIHIL, NIHIL);
        redde _recusatus(m, piscina);
    }
    si (momentorum)
    {
        f.momenta = (PlasticaMomentum*)piscina_allocare(piscina,
            momentorum * magnitudo(PlasticaMomentum));
        si (!f.momenta)
        {
            (vacuum)_recusare(m, ZEPHYRUM, "out of memory", NIHIL,
                NIHIL);
            redde _recusatus(m, piscina);
        }
        n = ZEPHYRUM;
        per (k = ZEPHYRUM; k < m->cyclorum; k++)
        {
            per (j = ZEPHYRUM; j < m->cycli[k].momentorum; j++)
            {
                f.momenta[n].cyclus   = k;
                f.momenta[n].titulus  = chorda_transcribere(
                    m->cycli[k].momenta[j], piscina);
                f.momenta[n].tempus   = m->cycli[k].tempora[j];
                n++;
            }
        }
    }
    si (m->membrorum)
    {
        f.fines = (i32*)piscina_allocare(piscina,
            m->membrorum * magnitudo(i32));
        si (!f.fines)
        {
            (vacuum)_recusare(m, ZEPHYRUM, "out of memory", NIHIL,
                NIHIL);
            redde _recusatus(m, piscina);
        }
        per (k = ZEPHYRUM; k < m->membrorum; k++)
        {
            f.fines[k] = m->articuli[m->membra[k].finis];
        }
    }
    f.finium       = m->membrorum;
    f.momentorum   = momentorum;
    f.successus    = VERUM;
    f.statua       = s;
    f.partes       = corpus.partes;
    f.articuli     = corpus.articuli;
    f.articulorum  = corpus.articulorum;
    redde f;
}
