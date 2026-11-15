/*
 * plastica.c - descriptio textualis in statuam (plastica.h)
 *
 * Arbor STML bis ambulatur: gradus 0 probat et numerat (omnis
 * recusatio hic nascitur), gradus I tabulas statuae implet. Ambo
 * gradus geometriam totam computant (idem iter, idem numerus);
 * gradus I solus scribit.
 *
 * Omne solidum triangula per _triangulum emittit: vertices tres
 * proprii cum normali faciei (umbratio plana), speculo currenti
 * reflexi, triangulum sine area omissum.
 */

#include "plastica.h"
#include "plastica_textura.h"
#include "stml.h"
#include "internamentum.h"
#include "xar.h"
#include "stereometria.h"
#include <math.h>
#include <string.h>

#define ERRORIS_MENSURA    CCLVI
#define NODI_MAXIMI        (XVI * CCLVI)
#define TRIANGULA_MAXIMA   (CC * M)
#define PUNCTA_MAXIMA      LXIV
#define LATERA_MAXIMA      LXIV
#define SEGMENTA_MAXIMA    XXXII
#define NUMERUS_MAXIMUS    CCLVI
#define AREA_MINIMA        1.0e-10f
#define AREA_RELATIVA_MINIMA  1.0e-5f
#define TEXTURAE_MAXIMAE   LXIV
#define MANU_MAXIMAE       XXXII
#define COLORES_MANU       XVI
/* facies cuius normalis tam erecta est iacet: u = x, v = z */
#define FACIES_IACENS      1.0e-4f
/* altitudo obicis invisi (attributum 'obex' radicis): vestigium
 * solum tabulae gressus interest */
#define OBICIS_ALTITUDO    1.0f

/* clavis texturae statuae: solida cum clave eadem texturam unam
 * communicant. genus < PLASTICA_TEXTURA_GENERA = genita; aliter
 * manu scripta (genus - PLASTICA_TEXTURA_GENERA) */
nomen structura {
    i32 genus;
    i32 semen;
     i8 colores[IX];
} TexturaClavis;

nomen structura {
    chorda titulus;
     Imago imago;
} TexturaManu;

nomen structura {
      Piscina* piscina;
       Statua* statua;
       chorda* partes;
          i32  gradus;          /* 0 numerat, I implet */
          i32  nodorum;
          i32  solidorum;
          i32  verticum;
          i32  indicum;
          i32  index_solidi;    /* indicum ad initium solidi */
          i32  linea_solidi;
          i32  semen_additum;   /* omni semini rei additur */
          i32  semen_seriei;    /* exemplar seriei: semini additur */
          f32  magnitudo_seriei[III]; /* exemplar seriei: magnitudini
                                       * liberorum additur */
          f32 motus_seriei[II];      /* exemplar seriei: motui
                                       * texturae additur (texella) */
          f32 speculum_x;      /* 1 aut -1: reflexio currens */
          f32 speculum_z;
          b32 fractum;
          i32 linea;
    character error[ERRORIS_MENSURA];
    /* texturae: tabula in gradu 0 crescit, gradus I eam invenit */
    TexturaClavis claves[TEXTURAE_MAXIMAE];
            Imago texturae[TEXTURAE_MAXIMAE];
              i32 texturarum;
      TexturaManu manu[MANU_MAXIMAE];
              i32 manu_numerus;
    /* coordinatae solidi currentis: texella per unitatem divisa per
     * latus texturae (0 = sine textura); ancora = punctum faciei
     * cuius coordinatae (in unitatibus) datae sunt */
                   f32 texella_s;
                   f32 texella_t;
    /* motus texturae solidi currentis, in coordinatis (texella per
     * latus texturae) */
                   f32 motus_s;
                   f32 motus_t;
    StereometriaVector ancora;
                   f32 ancora_s;
                   f32 ancora_t;
    /* figura: radix <figura> est; claustrum = grex aut repetitor
     * intra quem sumus (articulum vetat), NIHIL = nullus; articuli
     * ordine plagulae cum elementis suis (titulus iteratus) */
    constans StmlNodus* radix;
                   b32  figura;
    constans character* claustrum;
                   i32  articulorum;
                   i32  articuli[PLASTICA_ARTICULI_MAXIMI];
    constans StmlNodus* fontes[PLASTICA_ARTICULI_MAXIMI];
} Plastes;


/* ==================================================
 * Recusatio
 * ================================================== */

interior vacuum
_appone (
               Plastes* p,
                   i32* longitudo,
    constans character* fons,
                   i32  mensura)
{
    i32 k;

    per (k = ZEPHYRUM;
         k < mensura && *longitudo + I < ERRORIS_MENSURA; k++)
    {
        character littera = fons[k];

        /* nuntius lineam novam numquam fert */
        si (littera == '\n' || littera == '\r' || littera == '\t')
        {
            littera = ' ';
        }
        p->error[(*longitudo)++] = littera;
    }
    p->error[*longitudo] = '\0';
}

interior vacuum
_appone_literas (
               Plastes* p,
                   i32* longitudo,
    constans character* fons)
{
    si (fons)
    {
        _appone(p, longitudo, fons, (i32)strlen(fons));
    }
}

interior vacuum
_appone_chordam (
            Plastes* p,
                i32* longitudo,
    constans chorda* fons)
{
    si (fons && fons->datum)
    {
        _appone(p, longitudo, (constans character*)fons->datum,
            fons->mensura > LXIV ? LXIV : fons->mensura);
    }
}

/* nuntius = ante + medium + post; prima recusatio manet */
interior b32
_recusare (
               Plastes* p,
                   i32  linea,
    constans character* ante,
       constans chorda* medium,
    constans character* post)
{
    i32 longitudo = ZEPHYRUM;

    si (p->fractum)
    {
        redde FALSUM;
    }
    p->fractum  = VERUM;
    p->linea    = linea;
    _appone_literas(p, &longitudo, ante);
    _appone_chordam(p, &longitudo, medium);
    _appone_literas(p, &longitudo, post);
    redde FALSUM;
}

/* "<elementum> attribute 'titulus' quid" */
interior b32
_recusare_attributum (
               Plastes* p,
    constans StmlNodus* nodus,
    constans character* titulus,
    constans character* quid)
{
    i32 longitudo = ZEPHYRUM;

    si (p->fractum)
    {
        redde FALSUM;
    }
    p->fractum  = VERUM;
    p->linea    = nodus->linea;
    _appone_literas(p, &longitudo, "<");
    _appone_chordam(p, &longitudo, nodus->titulus);
    _appone_literas(p, &longitudo, "> attribute '");
    _appone_literas(p, &longitudo, titulus);
    _appone_literas(p, &longitudo, "' ");
    _appone_literas(p, &longitudo, quid);
    redde FALSUM;
}

/* "<elementum> quid" */
interior b32
_recusare_elementum (
               Plastes* p,
    constans StmlNodus* nodus,
    constans character* quid)
{
    i32 longitudo = ZEPHYRUM;

    si (p->fractum)
    {
        redde FALSUM;
    }
    p->fractum  = VERUM;
    p->linea    = nodus->linea;
    _appone_literas(p, &longitudo, "<");
    _appone_chordam(p, &longitudo, nodus->titulus);
    _appone_literas(p, &longitudo, "> ");
    _appone_literas(p, &longitudo, quid);
    redde FALSUM;
}


/* ==================================================
 * Valores attributorum
 * ================================================== */

interior b32
_spatium (
    i8 littera)
{
    redde littera == ' ' || littera == '\t' || littera == '\n'
        || littera == '\r';
}

interior b32
_numerus_finitus (
    chorda  verbum,
       f32* exitus)
{
    f64 x;

    si (   !chorda_ut_f64(verbum, &x) || !(x == x) || x > 1.0e6
        || x < -1.0e6)
    {
        redde FALSUM;
    }
    *exitus = (f32)x;
    redde VERUM;
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
            || !_numerus_finitus(chorda_sectio(*valor, initium, k),
                &exitus[n]))
        {
            redde FALSUM;
        }
        n++;
    }
    redde n == numerus;
}

/* paria "a,b a,b ..." in a[] et b[]; *numerus paria lecta */
interior b32
_paria (
    constans chorda* valor,
                f32* a,
                f32* b,
                i32* numerus)
{
    i32 k = ZEPHYRUM;

    *numerus = ZEPHYRUM;
    dum (VERUM)
    {
        i32 initium;
        i32 comma;

        dum (k < valor->mensura && _spatium(valor->datum[k]))
        {
            k++;
        }
        si (k >= valor->mensura)
        {
            frange;
        }
        initium  = k;
        comma    = valor->mensura;
        dum (k < valor->mensura && !_spatium(valor->datum[k]))
        {
            si (valor->datum[k] == ',' && comma == valor->mensura)
            {
                comma = k;
            }
            k++;
        }
        si (   *numerus >= PUNCTA_MAXIMA || comma == valor->mensura
            || !_numerus_finitus(chorda_sectio(*valor, initium, comma),
                &a[*numerus])
            || !_numerus_finitus(chorda_sectio(*valor, comma + I, k),
                &b[*numerus]))
        {
            redde FALSUM;
        }
        (*numerus)++;
    }
    redde VERUM;
}

interior s32
_digitus_hex (
    i8 littera)
{
    si (littera >= '0' && littera <= '9')
    {
        redde (s32)(littera - '0');
    }
    si (littera >= 'a' && littera <= 'f')
    {
        redde (s32)(littera - 'a') + X;
    }
    si (littera >= 'A' && littera <= 'F')
    {
        redde (s32)(littera - 'A') + X;
    }
    redde -I;
}

/* "#rgb" aut "#rrggbb" in tres f32 (0 .. 1) */
interior b32
_color (
    constans chorda* valor,
                f32* exitus)
{
    i32 k;

    si (   valor->mensura == ZEPHYRUM || valor->datum[ZEPHYRUM] != '#'
        || (valor->mensura != IV && valor->mensura != VII))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < III; k++)
    {
        s32 a;
        s32 b;

        si (valor->mensura == IV)
        {
            a = _digitus_hex(valor->datum[I + k]);
            b = a;
        }
        alioquin
        {
            a = _digitus_hex(valor->datum[I + k * II]);
            b = _digitus_hex(valor->datum[II + k * II]);
        }
        si (a < ZEPHYRUM || b < ZEPHYRUM)
        {
            redde FALSUM;
        }
        exitus[k] = (f32)(a * XVI + b) / 255.0f;
    }
    redde VERUM;
}

interior i8
_octetus (
    f32 x)
{
    redde (i8)(x * 255.0f + 0.5f);
}

/* colores spatiis separati in octetos (III per colorem); *numerus
 * colores lecti; FALSUM si quis malus aut plures quam maximum */
interior b32
_colores_legere (
    constans chorda* valor,
                 i8* exitus,
                i32  maximum,
                i32* numerus)
{
    i32 k = ZEPHYRUM;

    *numerus = ZEPHYRUM;
    dum (VERUM)
    {
        chorda verbum;
           f32 color[III];
           i32 initium;
           i32 a;

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
        verbum = chorda_sectio(*valor, initium, k);
        si (*numerus >= maximum || !_color(&verbum, color))
        {
            redde FALSUM;
        }
        per (a = ZEPHYRUM; a < III; a++)
        {
            exitus[*numerus * III + a] = _octetus(color[a]);
        }
        (*numerus)++;
    }
    redde VERUM;
}

interior b32
_titulus_est (
       constans chorda* titulus,
    constans character* literae)
{
    redde titulus && chorda_aequalis_literis(*titulus, literae);
}

/* omne attributum nodi in indice 'licita' (NIHIL terminato) est? */
interior b32
_attributa_licita (
                         Plastes* p,
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
            si (_titulus_est(a->titulus, licita[j]))
            {
                notum = VERUM;
                frange;
            }
        }
        si (!notum)
        {
            redde _recusare(p, nodus->linea, "unknown attribute '",
                a ? a->titulus : NIHIL, "'");
        }
    }
    redde VERUM;
}

/* attributum numeri unius; absens: *exitus = praesumptus (aut
 * recusatur si requisitum) */
interior b32
_realis (
               Plastes* p,
    constans StmlNodus* nodus,
    constans character* titulus,
                   b32  requisitum,
                   f32  praesumptus,
                   f32* exitus)
{
    constans chorda* valor = stml_attributum_capere(nodus, titulus);

    *exitus = praesumptus;
    si (!valor)
    {
        redde requisitum
            ? _recusare_attributum(p, nodus, titulus, "is required")
            : VERUM;
    }
    si (!_numeri(valor, exitus, I))
    {
        redde _recusare_attributum(p, nodus, titulus,
            "needs 1 number");
    }
    redde VERUM;
}

/* attributum integrum intra [minimum, maximum] */
interior b32
_integer (
               Plastes* p,
    constans StmlNodus* nodus,
    constans character* titulus,
                   b32  requisitum,
                   i32  praesumptus,
                   i32  minimum,
                   i32  maximum,
    constans character* quid,
                   i32* exitus)
{
    constans chorda* valor = stml_attributum_capere(nodus, titulus);
                f32  x;

    *exitus = praesumptus;
    si (!valor)
    {
        redde requisitum
            ? _recusare_attributum(p, nodus, titulus, "is required")
            : VERUM;
    }
    si (   !_numeri(valor, &x, I) || x < 0.0f
        || x != (f32)floor((f64)x) || (i32)x < minimum
        || (i32)x > maximum)
    {
        redde _recusare_attributum(p, nodus, titulus, quid);
    }
    *exitus = (i32)x;
    redde VERUM;
}

/* attributum 'semen' (0 .. 1000000; absens 0) cum semine addito
 * (plastica_legere_seminis) */
interior b32
_semen (
               Plastes* p,
    constans StmlNodus* nodus,
                   i32* exitus)
{
    si (!_integer(p, nodus, "semen", FALSUM, ZEPHYRUM, ZEPHYRUM, M * M,
        "must be a whole number from 0 to 1000000", exitus))
    {
        redde FALSUM;
    }
    *exitus += p->semen_additum + p->semen_seriei;
    redde VERUM;
}


/* ==================================================
 * Fortuita ex semine (integer solus: eadem in omni machina)
 * ================================================== */

interior i32
_miscere (
    i32 x)
{
    x ^= x >> XVI;
    x *= 0x85EBCA6Bu;
    x ^= x >> XIII;
    x *= 0xC2B2AE35u;
    x ^= x >> XVI;
    redde x;
}

/* [0, 1) ex semine et duobus indicibus */
interior f32
_fortuna (
    i32 semen,
    i32 a,
    i32 b)
{
    i32 x = _miscere(semen * 0x9E3779B1u + 0x7F4A7C15u);

    x = _miscere(x ^ (a * 0x85EBCA77u + I));
    x = _miscere(x ^ (b * 0xC2B2AE3Du + II));
    redde (f32)(x >> VIII) / 16777216.0f;
}


/* ==================================================
 * Transformatio et nodi
 * ================================================== */

interior StereometriaQuaternio
_circa_axem (
    f32 x,
    f32 y,
    f32 z,
    f32 gradus)
{
    StereometriaQuaternio q;
                      f32 dimidium =
                          gradus * STEREOMETRIA_PI / 360.0f;
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

/* locus, gyrus, rotatio nodi: gyrus primus, deinde rotatio circa x,
 * y, z hoc ordine */
interior b32
_transformatio (
                  Plastes* p,
       constans StmlNodus* nodus,
       StereometriaVector* translatio,
    StereometriaQuaternio* rotatio)
{
    constans chorda* valor;
                f32  n[III];

    *translatio  = stereometria_vector(0.0f, 0.0f, 0.0f);
    *rotatio     = stereometria_quaternio_identitas();
    valor        = stml_attributum_capere(nodus, "locus");
    si (valor)
    {
        si (!_numeri(valor, n, III))
        {
            redde _recusare_attributum(p, nodus, "locus",
                "needs 3 numbers (x y z)");
        }
        *translatio = stereometria_vector(n[ZEPHYRUM], n[I], n[II]);
    }
    valor = stml_attributum_capere(nodus, "gyrus");
    si (valor)
    {
        si (!_numeri(valor, n, I))
        {
            redde _recusare_attributum(p, nodus, "gyrus",
                "needs 1 number (degrees)");
        }
        *rotatio = _circa_axem(0.0f, 1.0f, 0.0f, n[ZEPHYRUM]);
    }
    valor = stml_attributum_capere(nodus, "rotatio");
    si (valor)
    {
        si (!_numeri(valor, n, III))
        {
            redde _recusare_attributum(p, nodus, "rotatio",
                "needs 3 numbers (degrees about x y z)");
        }
        *rotatio = _componere(
            _circa_axem(1.0f, 0.0f, 0.0f, n[ZEPHYRUM]), *rotatio);
        *rotatio = _componere(_circa_axem(0.0f, 1.0f, 0.0f, n[I]),
            *rotatio);
        *rotatio = _componere(_circa_axem(0.0f, 0.0f, 1.0f, n[II]),
            *rotatio);
    }
    *rotatio = stereometria_quaternio_normalizare(*rotatio);
    redde VERUM;
}

/* nodum novum numerare aut implere. Speculum currens transformationem
 * reflectit: M T M (translatio reflexa, rotatio coniugata), ut
 * catena cum verticibus reflexis imaginem veram det sine scala
 * negativa. cum_titulo: titulus elementi in tabulam partium it. */
interior b32
_nodus_novus (
                  Plastes* p,
       constans StmlNodus* nodus,
                      b32  cum_titulo,
                      s32  parens,
       StereometriaVector  translatio,
    StereometriaQuaternio  rotatio,
                      f32  scala,
                      i32* index)
{
    i32 n = p->nodorum;

    si (n >= NODI_MAXIMI)
    {
        redde _recusare_elementum(p, nodus,
            "makes the model too large (more than 4096 parts)");
    }
    p->nodorum++;
    *index = n;
    si (p->gradus)
    {
        StatuaNodus* novus = &p->statua->nodi[n];

        translatio.x *= p->speculum_x;
        translatio.z *= p->speculum_z;
        si (p->speculum_x < 0.0f)
        {
            rotatio.y = -rotatio.y;
            rotatio.z = -rotatio.z;
        }
        si (p->speculum_z < 0.0f)
        {
            rotatio.x = -rotatio.x;
            rotatio.y = -rotatio.y;
        }
        novus->parens      = parens;
        novus->rete        = -I;
        novus->signa       = ZEPHYRUM;
        novus->translatio  = translatio;
        novus->rotatio     = rotatio;
        novus->scala       = stereometria_vector(scala, scala, scala);
        si (cum_titulo)
        {
            constans chorda* titulus =
                stml_attributum_capere(nodus, "titulus");

            si (titulus && titulus->mensura)
            {
                p->partes[n] =
                    chorda_transcribere(*titulus, p->piscina);
            }
        }
    }
    redde VERUM;
}


/* ==================================================
 * Solida: apparatus communis
 * ================================================== */

nomen structura {
    f32 color[IV];
    f32 limen_alpha;
    i32 signa;
    s32 textura;        /* index in texturas statuae; -1 = nulla */
    f32 texella_s;
    f32 texella_t;
    f32 motus_s;        /* motus texturae in coordinatis */
    f32 motus_t;
} Proprietates;

/* texturam clavis in tabula invenire aut addere (genitam hic
 * pingere) */
interior b32
_textura_statuae (
                   Plastes* p,
        constans StmlNodus* nodus,
    constans TexturaClavis* clavis,
                       s32* index)
{
    i32 k;

    per (k = ZEPHYRUM; k < p->texturarum; k++)
    {
        si (   p->claves[k].genus == clavis->genus
            && p->claves[k].semen == clavis->semen
            && memcmp(p->claves[k].colores, clavis->colores, IX)
                == ZEPHYRUM)
        {
            *index = (s32)k;
            redde VERUM;
        }
    }
    si (k >= TEXTURAE_MAXIMAE)
    {
        redde _recusare_elementum(p, nodus,
            "uses one texture too many (more than 64 different)");
    }
    si (clavis->genus < PLASTICA_TEXTURA_GENERA)
    {
        si (!plastica_textura_generare(
            chorda_ex_literis(plastica_textura_genus(clavis->genus),
                p->piscina), PLASTICA_TEXTURA_LATUS,
            PLASTICA_TEXTURA_LATUS, clavis->semen, clavis->colores,
            &p->texturae[k], p->piscina))
        {
            redde _recusare(p, nodus->linea, "out of memory", NIHIL,
                NIHIL);
        }
    }
    alioquin
    {
        p->texturae[k] =
            p->manu[clavis->genus - PLASTICA_TEXTURA_GENERA].imago;
    }
    p->claves[k]  = *clavis;
    p->texturarum++;
    *index = (s32)k;
    redde VERUM;
}

/* textura, semen, colores solidi (plastica_textura.h) */
interior b32
_proprietates_texturae (
               Plastes* p,
    constans StmlNodus* nodus,
          Proprietates* q)
{
    constans chorda* valor   =
        stml_attributum_capere(nodus, "textura");
    constans chorda* colores =
        stml_attributum_capere(nodus, "colores");
    constans chorda* motus;
                f32  texella[II];
      TexturaClavis  clavis;
                i32  numerus;
                i32  k;

    q->textura    = -I;
    q->texella_s  = 0.0f;
    q->texella_t  = 0.0f;
    q->motus_s    = 0.0f;
    q->motus_t    = 0.0f;
    motus         = stml_attributum_capere(nodus, "motus_texturae");
    si (!valor)
    {
        si (motus)
        {
            redde _recusare_attributum(p, nodus, "motus_texturae",
                "needs the attribute 'textura'");
        }
        redde colores
            ? _recusare_attributum(p, nodus, "colores",
                "needs the attribute 'textura'")
            : VERUM;
    }
    memset(&clavis, ZEPHYRUM, magnitudo(clavis));
    per (k = ZEPHYRUM; k < PLASTICA_TEXTURA_GENERA; k++)
    {
        si (_titulus_est(valor, plastica_textura_genus(k)))
        {
            frange;
        }
    }
    si (k < PLASTICA_TEXTURA_GENERA)
    {
        clavis.genus = k;
        si (!_semen(p, nodus, &clavis.semen))
        {
            redde FALSUM;
        }
        si (colores)
        {
            /* IV loci: color quartus deprehenditur */
            i8 lecti[XII];

            si (   !_colores_legere(colores, lecti, IV, &numerus)
                || numerus != III)
            {
                redde _recusare_attributum(p, nodus, "colores",
                    "needs 3 colours written '#rgb #rgb #rgb'"
                    " (dark, middle, light)");
            }
            memcpy(clavis.colores, lecti, IX);
        }
        alioquin
        {
            per (k = ZEPHYRUM; k < III; k++)
            {
                i32 medius = _octetus(q->color[k]);

                clavis.colores[k]        = (i8)(medius * III / IV);
                clavis.colores[III + k]  = (i8)medius;
                clavis.colores[VI + k]   =
                    (i8)(medius + (CCLV - medius) / IV);
            }
        }
    }
    alioquin
    {
        per (k = ZEPHYRUM; k < p->manu_numerus; k++)
        {
            si (chorda_aequalis(p->manu[k].titulus, *valor))
            {
                frange;
            }
        }
        si (k >= p->manu_numerus)
        {
            redde _recusare(p, nodus->linea, "unknown texture '",
                valor, "'");
        }
        si (colores)
        {
            redde _recusare_attributum(p, nodus, "colores",
                "cannot be given with a hand-written texture");
        }
        clavis.genus = PLASTICA_TEXTURA_GENERA + k;
    }
    si (!_textura_statuae(p, nodus, &clavis, &q->textura))
    {
        redde FALSUM;
    }
    q->texella_s = (f32)PLASTICA_TEXELLA_PER_UNITATEM
        / (f32)p->texturae[q->textura].latitudo;
    q->texella_t = (f32)PLASTICA_TEXELLA_PER_UNITATEM
        / (f32)p->texturae[q->textura].altitudo;
    texella[ZEPHYRUM]  = 0.0f;
    texella[I]         = 0.0f;
    si (motus && !_numeri(motus, texella, II))
    {
        redde _recusare_attributum(p, nodus, "motus_texturae",
            "needs 2 numbers (texels along and down)");
    }
    /* motus solidi et motus exemplaris seriei */
    q->motus_s = (texella[ZEPHYRUM] + p->motus_seriei[ZEPHYRUM])
        / (f32)p->texturae[q->textura].latitudo;
    q->motus_t = (texella[I] + p->motus_seriei[I])
        / (f32)p->texturae[q->textura].altitudo;
    /* textura colorem fert: factor albus (alpha manet) */
    q->color[ZEPHYRUM]  = 1.0f;
    q->color[I]         = 1.0f;
    q->color[II]        = 1.0f;
    redde VERUM;
}

/* color, signum, invisibilis, textura solidi */
interior b32
_proprietates (
               Plastes* p,
    constans StmlNodus* nodus,
          Proprietates* q)
{
    constans chorda* valor;

    q->color[ZEPHYRUM]  = 1.0f;
    q->color[I]         = 1.0f;
    q->color[II]        = 1.0f;
    q->color[III]       = 1.0f;
    q->limen_alpha      = -1.0f;
    q->signa            = ZEPHYRUM;
    valor               = stml_attributum_capere(nodus, "color");
    si (valor && !_color(valor, q->color))
    {
        redde _recusare_attributum(p, nodus, "color",
            "must be #rgb or #rrggbb");
    }
    valor = stml_attributum_capere(nodus, "signum");
    si (valor)
    {
        si (_titulus_est(valor, "solidum"))
        {
            q->signa = STATUA_SIGNUM_SOLIDUM;
        }
        alioquin si (_titulus_est(valor, "gressibile"))
        {
            q->signa = STATUA_SIGNUM_GRESSIBILE;
        }
        alioquin
        {
            redde _recusare_attributum(p, nodus, "signum",
                "must be 'solidum' or 'gressibile'");
        }
    }
    valor = stml_attributum_capere(nodus, "invisibilis");
    si (valor)
    {
        si (!_titulus_est(valor, "verum"))
        {
            redde _recusare_attributum(p, nodus, "invisibilis",
                "must be 'verum' or absent");
        }
        q->color[III]   = 0.0f;
        q->limen_alpha  = 0.5f;
    }
    redde _proprietates_texturae(p, nodus, q);
}

/* nodus liberum elementum habet? (solidum liberos non fert) */
interior b32
_habet_elementa (
    constans StmlNodus* nodus)
{
    i32 numerus = nodus->liberi ? xar_numerus(nodus->liberi)
        : ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        StmlNodus** locus =
            (StmlNodus**)xar_obtinere(nodus->liberi, k);

        si (   locus && *locus
            && (*locus)->genus == STML_NODUS_ELEMENTUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* nodum n solidum facere: rete, partem et materiam ex
 * proprietatibus aperire; triangula deinde per _triangulum */
interior vacuum
_solidum_inire (
                  Plastes* p,
                      i32  linea,
                      i32  n,
    constans Proprietates* q)
{
    p->index_solidi  = p->indicum;
    p->linea_solidi  = linea;
    p->texella_s     = q->texella_s;
    p->texella_t     = q->texella_t;
    p->motus_s       = q->motus_s;
    p->motus_t       = q->motus_t;
    p->ancora        = stereometria_vector(0.0f, 0.0f, 0.0f);
    p->ancora_s      = 0.0f;
    p->ancora_t      = 0.0f;
    si (p->gradus)
    {
               Statua* s = p->statua;
                  i32  k = p->solidorum;
        StatuaMateria* materia;

        s->nodi[n].rete            = (s32)k;
        s->nodi[n].signa           = q->signa;
        s->retia[k].pars_prima     = k;
        s->retia[k].partium        = I;
        s->partes[k].index_primus  = p->indicum;
        s->partes[k].materia       = (s32)k;
        materia                    = &s->materiae[k];
        memset(materia, ZEPHYRUM, magnitudo(*materia));
        materia->textura      = q->textura;
        memcpy(materia->color, q->color, magnitudo(materia->color));
        materia->limen_alpha  = q->limen_alpha;
        materia->repetit_s    = VERUM;
        materia->repetit_t    = VERUM;
    }
}

/* attributa communia solidi probare, nodum, rete, partem et
 * materiam aperire; triangula deinde per _triangulum */
interior b32
_solidum_aperire (
                         Plastes* p,
              constans StmlNodus* nodus,
                             s32  parens,
    constans character* constans* licita)
{
       StereometriaVector translatio;
    StereometriaQuaternio rotatio;
             Proprietates q;
                      i32 n;

    si (!p->gradus)
    {
        si (!_attributa_licita(p, nodus, licita))
        {
            redde FALSUM;
        }
        si (_habet_elementa(nodus))
        {
            redde _recusare_elementum(p, nodus,
                "is a solid and cannot have children");
        }
    }
    si (   !_transformatio(p, nodus, &translatio, &rotatio)
        || !_proprietates(p, nodus, &q)
        || !_nodus_novus(p, nodus, VERUM, parens, translatio, rotatio,
            1.0f, &n))
    {
        redde FALSUM;
    }
    _solidum_inire(p, nodus->linea, n, &q);
    redde VERUM;
}

interior b32
_solidum_claudere (
               Plastes* p,
    constans StmlNodus* nodus)
{
    si (p->fractum)
    {
        redde FALSUM;
    }
    si (p->indicum == p->index_solidi)
    {
        redde _recusare_elementum(p, nodus, "has no surface");
    }
    si (p->gradus)
    {
        p->statua->partes[p->solidorum].indicum =
            p->indicum - p->index_solidi;
    }
    p->solidorum++;
    redde VERUM;
}

/* facies quae sequuntur coordinatas ab hoc puncto numerant: punctum
 * coordinatas (s, t) in unitatibus accipit. Sic superficies curva
 * ambitum continuum tenet (facies iacentes ancoram neglegunt) */
interior vacuum
_ancoram_ponere (
               Plastes* p,
    StereometriaVector  punctum,
                   f32  s,
                   f32  t)
{
    p->ancora    = punctum;
    p->ancora_s  = s;
    p->ancora_t  = t;
}

/* coordinatae texturae trianguli (ante reflexionem: imago speculi
 * texturam reflexam fert). Texella quadrata: s secundum latus
 * horizontale faciei dextrorsum extra visae, t deorsum secundum
 * clivum (t = 0 ordo summus); facies iacens: s = x, t = z */
interior vacuum
_coordinatae (
      constans Plastes* p,
    StereometriaVector  a,
    StereometriaVector  b,
    StereometriaVector  c,
                   f32  st[III][II])
{
    StereometriaVector puncta[III];
    StereometriaVector n = stereometria_crux(
        stereometria_subtrahere(b, a), stereometria_subtrahere(c, a));
                   f32 longitudo  = stereometria_longitudo(n);
                   f32 planum     =
                       (f32)sqrt((f64)(n.x * n.x + n.z * n.z));
                   i32 k;

    memset(st, ZEPHYRUM, III * II * magnitudo(f32));
    si (p->texella_s == 0.0f || !(longitudo > 0.0f))
    {
        redde;
    }
    puncta[ZEPHYRUM]  = a;
    puncta[I]         = b;
    puncta[II]        = c;
    per (k = ZEPHYRUM; k < III; k++)
    {
        f32 s;
        f32 t;

        si (planum <= FACIES_IACENS * longitudo)
        {
            s = puncta[k].x;
            t = puncta[k].z;
        }
        alioquin
        {
            StereometriaVector d =
                stereometria_subtrahere(puncta[k], p->ancora);
            /* dextrorsum: sursum x normalis; sursum per faciem:
             * normalis x dextrorsum */
                           f32 dexter_x = n.z / planum;
                           f32 dexter_z = -n.x / planum;

            s  = p->ancora_s + d.x * dexter_x + d.z * dexter_z;
            t  = p->ancora_t
                - (d.x * n.y * dexter_z + d.y * planum
                    - d.z * n.y * dexter_x)
                / longitudo;
        }
        st[k][ZEPHYRUM]  = s * p->texella_s + p->motus_s;
        st[k][I]         = t * p->texella_t + p->motus_t;
    }
}

/* triangulum unum, extrorsum visum contra horologium (a b c) */
interior vacuum
_triangulum (
               Plastes* p,
    StereometriaVector  a,
    StereometriaVector  b,
    StereometriaVector  c)
{
    StereometriaVector normalis;
    StereometriaVector latus_primum;
    StereometriaVector latus_alterum;
    StereometriaVector anguli[III];
                   f32 st[III][II];
                   f32 longitudo;
                   i32 ordo[III];
                   i32 k;

    _coordinatae(p, a, b, c, st);
    ordo[ZEPHYRUM]    = ZEPHYRUM;
    a.x               *= p->speculum_x;
    a.z               *= p->speculum_z;
    b.x               *= p->speculum_x;
    b.z               *= p->speculum_z;
    c.x               *= p->speculum_x;
    c.z               *= p->speculum_z;
    anguli[ZEPHYRUM]  = a;
    /* reflexio una ordinem invertit; duae eum restituunt */
    si (p->speculum_x * p->speculum_z < 0.0f)
    {
        anguli[I]   = c;
        anguli[II]  = b;
        ordo[I]     = II;
        ordo[II]    = I;
    }
    alioquin
    {
        anguli[I]   = b;
        anguli[II]  = c;
        ordo[I]     = I;
        ordo[II]    = II;
    }
    latus_primum  =
        stereometria_subtrahere(anguli[I], anguli[ZEPHYRUM]);
    latus_alterum =
        stereometria_subtrahere(anguli[II], anguli[ZEPHYRUM]);
    normalis   = stereometria_crux(latus_primum, latus_alterum);
    longitudo  = stereometria_longitudo(normalis);
    /* triangulum sine area omittitur. Limen RELATIVUM ad latera:
     * compilator a*b - c*d in fma contrahit, unde crux laterum
     * identicorum non exacte 0 est sed residuum rotundationis */
    si (   p->fractum
        || !(longitudo > AREA_RELATIVA_MINIMA
            * stereometria_longitudo(latus_primum)
            * stereometria_longitudo(latus_alterum)))
    {
        redde;
    }
    si (p->indicum / III >= TRIANGULA_MAXIMA)
    {
        (vacuum)_recusare(p, p->linea_solidi,
            "the model is too large (more than 200000 triangles)",
            NIHIL, NIHIL);
        redde;
    }
    si (p->gradus)
    {
        Statua* s = p->statua;

        normalis = stereometria_scalare(normalis, 1.0f / longitudo);
        per (k = ZEPHYRUM; k < III; k++)
        {
            i32 v = p->verticum + k;

            s->positiones[v * III]       = anguli[k].x;
            s->positiones[v * III + I]   = anguli[k].y;
            s->positiones[v * III + II]  = anguli[k].z;
            s->normales[v * III]         = normalis.x;
            s->normales[v * III + I]     = normalis.y;
            s->normales[v * III + II]    = normalis.z;
            s->coordinatae[v * II]       = st[ordo[k]][ZEPHYRUM];
            s->coordinatae[v * II + I]   = st[ordo[k]][I];
            s->indices[p->indicum + k]   = v;
        }
    }
    p->verticum  += III;
    p->indicum   += III;
}

/* quadrum (a b c d) extrorsum contra horologium */
interior vacuum
_quadrum (
               Plastes* p,
    StereometriaVector  a,
    StereometriaVector  b,
    StereometriaVector  c,
    StereometriaVector  d)
{
    _triangulum(p, a, b, c);
    _triangulum(p, a, c, d);
}

/* punctum in circulo: angulus k / latera, +x versus +z */
interior StereometriaVector
_in_circulo (
    f32 radius,
    f32 y,
    i32 k,
    i32 latera)
{
    f32 angulus = 2.0f * STEREOMETRIA_PI * (f32)(k % latera)
        / (f32)latera;

    redde stereometria_vector(radius * (f32)cos((f64)angulus), y,
        radius * (f32)sin((f64)angulus));
}

/* magnitudo="x y z" positiva */
interior b32
_magnitudo (
               Plastes* p,
    constans StmlNodus* nodus,
                   f32* exitus)
{
    constans chorda* valor =
        stml_attributum_capere(nodus, "magnitudo");
                i32 a;

    si (   !valor || !_numeri(valor, exitus, III)
        || !(exitus[ZEPHYRUM] > 0.0f) || !(exitus[I] > 0.0f)
        || !(exitus[II] > 0.0f))
    {
        redde _recusare_attributum(p, nodus, "magnitudo",
            "needs 3 positive numbers (x y z)");
    }
    /* exemplar seriei cum magnitudo_gressus */
    per (a = ZEPHYRUM; a < III; a++)
    {
        exitus[a] += p->magnitudo_seriei[a];
        si (!(exitus[a] > 0.0f))
        {
            redde _recusare_attributum(p, nodus, "magnitudo",
                "is not positive in this copy of the series"
                " (magnitudo_gressus)");
        }
    }
    redde VERUM;
}


/* ==================================================
 * Solida
 * ================================================== */

#define COMMUNIA_SOLIDI \
    "titulus", "locus", "gyrus", "rotatio", "color", "signum", \
    "invisibilis", "textura", "semen", "colores", "motus_texturae"

/* facies capsae: normalis et duo axes tangentes (u x v = normalis),
 * ut anguli (-u -v) (+u -v) (+u +v) (-u +v) extrorsum visi contra
 * horologium eant */
hic_manens constans s32 CAPSAE_FACIES[VI][IX] = {
    {  1,  0,  0,   0, 1, 0,   0, 0, 1 },
    { -1,  0,  0,   0, 0, 1,   0, 1, 0 },
    {  0,  1,  0,   0, 0, 1,   1, 0, 0 },
    {  0, -1,  0,   1, 0, 0,   0, 0, 1 },
    {  0,  0,  1,   1, 0, 0,   0, 1, 0 },
    {  0,  0, -1,   0, 1, 0,   1, 0, 0 }
};

/* sex facies capsae: dimidiae latitudines, basis in humo */
interior vacuum
_capsam_emittere (
         Plastes* p,
    constans f32* dimidia)
{
    hic_manens constans s32 ANGULI[IV][II] = {
        { -1, -1 }, { 1, -1 }, { 1, 1 }, { -1, 1 }
    };
    StereometriaVector anguli[IV];
                   f32 positio[III];
                   i32 f;
                   i32 a;
                   i32 k;

    per (f = ZEPHYRUM; f < VI; f++)
    {
        constans s32* facies = CAPSAE_FACIES[f];

        per (k = ZEPHYRUM; k < IV; k++)
        {
            per (a = ZEPHYRUM; a < III; a++)
            {
                positio[a] = dimidia[a]
                    * (f32)(facies[a]
                        + ANGULI[k][ZEPHYRUM] * facies[III + a]
                        + ANGULI[k][I] * facies[VI + a]);
            }
            /* basis in humo: centrum ad dimidiam altitudinem */
            anguli[k] = stereometria_vector(positio[ZEPHYRUM],
                positio[I] + dimidia[I], positio[II]);
        }
        _quadrum(p, anguli[ZEPHYRUM], anguli[I], anguli[II],
            anguli[III]);
    }
}

interior b32
_capsa (
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens)
{
    hic_manens constans character* constans LICITA[] = {
        COMMUNIA_SOLIDI, "magnitudo", NIHIL
    };
    f32 dimidia[III];
    i32 a;

    si (   !_solidum_aperire(p, nodus, parens, LICITA)
        || !_magnitudo(p, nodus, dimidia))
    {
        redde FALSUM;
    }
    per (a = ZEPHYRUM; a < III; a++)
    {
        dimidia[a] *= 0.5f;
    }
    _capsam_emittere(p, dimidia);
    redde _solidum_claudere(p, nodus);
}

interior b32
_cylindrus (
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens)
{
    hic_manens constans character* constans LICITA[] = {
        COMMUNIA_SOLIDI, "radius", "radius_summus", "altitudo",
        "latera", NIHIL
    };
    StereometriaVector imum;
    StereometriaVector summum;
                   f32 radius;
                   f32 radius_summus;
                   f32 altitudo;
                   f32 latus;
                   i32 latera;
                   i32 k;

    si (   !_solidum_aperire(p, nodus, parens, LICITA)
        || !_realis(p, nodus, "radius", VERUM, 0.0f, &radius)
        || !_realis(p, nodus, "altitudo", VERUM, 0.0f, &altitudo))
    {
        redde FALSUM;
    }
    si (!(radius > 0.0f))
    {
        redde _recusare_attributum(p, nodus, "radius",
            "must be positive");
    }
    altitudo += p->magnitudo_seriei[I];
    si (!(altitudo > 0.0f))
    {
        redde _recusare_attributum(p, nodus, "altitudo",
            "must be positive (in a series: in every copy)");
    }
    si (   !_realis(p, nodus, "radius_summus", FALSUM, radius,
            &radius_summus)
        || !_integer(p, nodus, "latera", FALSUM, VIII, III,
            LATERA_MAXIMA, "must be a whole number from 3 to 64",
            &latera))
    {
        redde FALSUM;
    }
    si (radius_summus < 0.0f)
    {
        redde _recusare_attributum(p, nodus, "radius_summus",
            "must not be negative");
    }
    imum    = stereometria_vector(0.0f, 0.0f, 0.0f);
    summum  = stereometria_vector(0.0f, altitudo, 0.0f);
    latus   = 2.0f * radius
        * (f32)sin((f64)(STEREOMETRIA_PI / (f32)latera));
    per (k = ZEPHYRUM; k < latera; k++)
    {
        StereometriaVector b0 = _in_circulo(radius, 0.0f, k, latera);
        StereometriaVector b1 =
            _in_circulo(radius, 0.0f, k + I, latera);
        StereometriaVector t0 =
            _in_circulo(radius_summus, altitudo, k, latera);
        StereometriaVector t1 =
            _in_circulo(radius_summus, altitudo, k + I, latera);

        /* ambitus: s ab angulo 0 retrorsum currit (dextrorsum
         * extra visum est contra angulum crescentem) */
        _ancoram_ponere(p, b0, -(f32)k * latus, 0.0f);
        _quadrum(p, b0, t0, t1, b1);
        _triangulum(p, imum, b0, b1);
        _triangulum(p, summum, t1, t0);
    }
    redde _solidum_claudere(p, nodus);
}

/* vertex sphaerae: zona j (0 = polus imus), segmentum k; asperitas
 * radium variat, idem valor omni triangulo qui verticem tangit */
interior StereometriaVector
_vertex_sphaerae (
    constans f32* dimidia,
             i32  j,
             i32  k,
             i32  zonae,
             i32  segmenta,
             f32  asperitas,
             i32  semen)
{
    f32 sinus;
    f32 cosinus;
    f32 angulus;
    f32 factor;

    si (j == ZEPHYRUM || j == zonae)
    {
        sinus    = 0.0f;
        cosinus  = j == ZEPHYRUM ? 1.0f : -1.0f;
        k        = ZEPHYRUM;
    }
    alioquin
    {
        f32 phi = STEREOMETRIA_PI * (f32)j / (f32)zonae;

        sinus    = (f32)sin((f64)phi);
        cosinus  = (f32)cos((f64)phi);
        k        = k % segmenta;
    }
    angulus  = 2.0f * STEREOMETRIA_PI * (f32)k / (f32)segmenta;
    factor   = 1.0f + asperitas * (_fortuna(semen, j, k) - 0.5f);
    redde stereometria_vector(
        dimidia[ZEPHYRUM] * factor * sinus * (f32)cos((f64)angulus),
        dimidia[I] - dimidia[I] * factor * cosinus,
        dimidia[II] * factor * sinus * (f32)sin((f64)angulus));
}

interior b32
_sphaera (
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens)
{
    hic_manens constans character* constans LICITA[] = {
        COMMUNIA_SOLIDI, "radius", "magnitudo", "segmenta",
        "asperitas", NIHIL
    };
    f32 dimidia[III];
    f32 radius;
    f32 asperitas;
    f32 passus;
    i32 segmenta;
    i32 semen;
    i32 zonae;
    i32 j;
    i32 k;

    si (!_solidum_aperire(p, nodus, parens, LICITA))
    {
        redde FALSUM;
    }
    si (stml_attributum_capere(nodus, "magnitudo"))
    {
        si (stml_attributum_capere(nodus, "radius"))
        {
            redde _recusare_attributum(p, nodus, "radius",
                "cannot be given together with 'magnitudo'");
        }
        si (!_magnitudo(p, nodus, dimidia))
        {
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < III; k++)
        {
            dimidia[k] *= 0.5f;
        }
    }
    alioquin
    {
        si (!_realis(p, nodus, "radius", VERUM, 0.0f, &radius))
        {
            redde FALSUM;
        }
        si (!(radius > 0.0f))
        {
            redde _recusare_attributum(p, nodus, "radius",
                "must be positive");
        }
        dimidia[ZEPHYRUM]  = radius;
        dimidia[I]         = radius;
        dimidia[II]        = radius;
    }
    si (   !_integer(p, nodus, "segmenta", FALSUM, VI, III,
            SEGMENTA_MAXIMA, "must be a whole number from 3 to 32",
            &segmenta)
        || !_realis(p, nodus, "asperitas", FALSUM, 0.0f, &asperitas)
        || !_semen(p, nodus, &semen))
    {
        redde FALSUM;
    }
    si (asperitas < 0.0f || asperitas > 1.0f)
    {
        redde _recusare_attributum(p, nodus, "asperitas",
            "must be between 0 and 1");
    }
    zonae   = segmenta / II < II ? II : segmenta / II;
    passus  = STEREOMETRIA_PI * dimidia[I] / (f32)zonae;
    per (j = ZEPHYRUM; j < zonae; j++)
    {
        per (k = ZEPHYRUM; k < segmenta; k++)
        {
            StereometriaVector a = _vertex_sphaerae(dimidia, j, k,
                zonae, segmenta, asperitas, semen);
            StereometriaVector b = _vertex_sphaerae(dimidia, j + I, k,
                zonae, segmenta, asperitas, semen);
            StereometriaVector c = _vertex_sphaerae(dimidia, j + I,
                k + I, zonae, segmenta, asperitas, semen);
            StereometriaVector d = _vertex_sphaerae(dimidia, j, k + I,
                zonae, segmenta, asperitas, semen);
                           f32 infra = stereometria_longitudo(
                               stereometria_subtrahere(d, a));
                           f32 supra = stereometria_longitudo(
                               stereometria_subtrahere(c, b));

            /* ancora in anulo latiore (polus latitudinem non
             * habet); t secundum meridianum */
            si (supra > infra)
            {
                _ancoram_ponere(p, b, -(f32)k * supra,
                    -(f32)(j + I) * passus);
            }
            alioquin
            {
                _ancoram_ponere(p, a, -(f32)k * infra,
                    -(f32)j * passus);
            }
            _quadrum(p, a, b, c, d);
        }
    }
    redde _solidum_claudere(p, nodus);
}

interior b32
_cuneus (
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens)
{
    hic_manens constans character* constans LICITA[] = {
        COMMUNIA_SOLIDI, "magnitudo", NIHIL
    };
    StereometriaVector a;
    StereometriaVector b;
    StereometriaVector c;
    StereometriaVector d;
    StereometriaVector e;
    StereometriaVector f;
                   f32 m[III];
                   f32 dimidium_x;
                   f32 dimidium_z;

    si (   !_solidum_aperire(p, nodus, parens, LICITA)
        || !_magnitudo(p, nodus, m))
    {
        redde FALSUM;
    }
    dimidium_x  = m[ZEPHYRUM] * 0.5f;
    dimidium_z  = m[II] * 0.5f;
    a           = stereometria_vector(-dimidium_x, 0.0f, -dimidium_z);
    b           = stereometria_vector(dimidium_x, 0.0f, -dimidium_z);
    c           = stereometria_vector(dimidium_x, 0.0f, dimidium_z);
    d           = stereometria_vector(-dimidium_x, 0.0f, dimidium_z);
    e           = stereometria_vector(-dimidium_x, m[I], -dimidium_z);
    f           = stereometria_vector(-dimidium_x, m[I], dimidium_z);
    _quadrum(p, a, b, c, d);      /* basis */
    _quadrum(p, a, d, f, e);      /* tergum ad -x */
    _quadrum(p, e, f, c, b);      /* clivus */
    _triangulum(p, a, e, b);      /* latus -z */
    _triangulum(p, d, c, f);      /* latus +z */
    redde _solidum_claudere(p, nodus);
}

/* (b - a) x (c - a) in plano (x, z) */
interior f32
_crux_plana (
    constans f32* x,
    constans f32* z,
             i32  a,
             i32  b,
             i32  c)
{
    redde (x[b] - x[a]) * (z[c] - z[a])
        - (z[b] - z[a]) * (x[c] - x[a]);
}

/* segmenta (a b) et (c d) se proprie secant? */
interior b32
_secant (
    constans f32* x,
    constans f32* z,
             i32  a,
             i32  b,
             i32  c,
             i32  d)
{
    f32 p = _crux_plana(x, z, a, b, c);
    f32 q = _crux_plana(x, z, a, b, d);
    f32 r = _crux_plana(x, z, c, d, a);
    f32 s = _crux_plana(x, z, c, d, b);

    redde ((p > 0.0f && q < 0.0f) || (p < 0.0f && q > 0.0f))
        && ((r > 0.0f && s < 0.0f) || (r < 0.0f && s > 0.0f));
}

interior b32
_extrusio (
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens)
{
    hic_manens constans character* constans LICITA[] = {
        COMMUNIA_SOLIDI, "puncta", "altitudo", NIHIL
    };
    constans chorda* valor;
                f32  x[PUNCTA_MAXIMA];
                f32  z[PUNCTA_MAXIMA];
                i32  ordo[PUNCTA_MAXIMA];
                f32  altitudo;
                f32  area    = 0.0f;
                f32  cursus  = 0.0f;
                i32  numerus;
                i32  reliqua;
                i32  conatus;
                i32  k;
                i32  j;

    si (!_solidum_aperire(p, nodus, parens, LICITA))
    {
        redde FALSUM;
    }
    valor = stml_attributum_capere(nodus, "puncta");
    si (!valor || !_paria(valor, x, z, &numerus) || numerus < III)
    {
        redde _recusare_attributum(p, nodus, "puncta",
            "needs 3 to 64 points written 'x,z x,z ...'");
    }
    si (!_realis(p, nodus, "altitudo", VERUM, 0.0f, &altitudo))
    {
        redde FALSUM;
    }
    altitudo += p->magnitudo_seriei[I];
    si (!(altitudo > 0.0f))
    {
        redde _recusare_attributum(p, nodus, "altitudo",
            "must be positive (in a series: in every copy)");
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        i32 proximus = (k + I) % numerus;

        area += x[k] * z[proximus] - x[proximus] * z[k];
        per (j = k + I; j < numerus; j++)
        {
            si (x[k] == x[j] && z[k] == z[j])
            {
                redde _recusare_attributum(p, nodus, "puncta",
                    "repeats a point");
            }
            si (   j != proximus && (j + I) % numerus != k
                && _secant(x, z, k, proximus, j, (j + I) % numerus))
            {
                redde _recusare_attributum(p, nodus, "puncta",
                    "describes an outline that crosses itself");
            }
        }
    }
    si (!(area > AREA_MINIMA) && !(area < -AREA_MINIMA))
    {
        redde _recusare_attributum(p, nodus, "puncta",
            "describes an outline with no area");
    }
    /* ordo: desuper visum contra horologium (area hic negativa) */
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        ordo[k] = area > 0.0f ? numerus - I - k : k;
    }
    /* parietes */
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        i32 a = ordo[k];
        i32 b = ordo[(k + I) % numerus];

        /* ambitus: s secundum lineamentum crescit */
        _ancoram_ponere(p, stereometria_vector(x[a], 0.0f, z[a]),
            cursus, 0.0f);
        cursus += (f32)sqrt((f64)((x[b] - x[a]) * (x[b] - x[a])
            + (z[b] - z[a]) * (z[b] - z[a])));
        _quadrum(p, stereometria_vector(x[a], 0.0f, z[a]),
            stereometria_vector(x[b], 0.0f, z[b]),
            stereometria_vector(x[b], altitudo, z[b]),
            stereometria_vector(x[a], altitudo, z[a]));
    }
    /* opercula: aures abscinduntur */
    reliqua  = numerus;
    conatus  = ZEPHYRUM;
    k        = ZEPHYRUM;
    dum (reliqua >= III && conatus < reliqua)
    {
        i32 a      = ordo[(k + reliqua - I) % reliqua];
        i32 b      = ordo[k % reliqua];
        i32 c      = ordo[(k + I) % reliqua];
        f32 crux   = _crux_plana(x, z, a, b, c);
        b32 auris  = crux < 0.0f;

        per (j = ZEPHYRUM; auris && j < reliqua; j++)
        {
            i32 q = ordo[j];

            si (   q                          != a && q != b && q != c
                && _crux_plana(x, z, a, b, q) <= 0.0f
                && _crux_plana(x, z, b, c, q) <= 0.0f
                && _crux_plana(x, z, c, a, q) <= 0.0f)
            {
                auris = FALSUM;
            }
        }
        si (auris || crux == 0.0f)
        {
            si (auris)
            {
                _triangulum(p,
                    stereometria_vector(x[a], altitudo, z[a]),
                    stereometria_vector(x[b], altitudo, z[b]),
                    stereometria_vector(x[c], altitudo, z[c]));
                _triangulum(p, stereometria_vector(x[a], 0.0f, z[a]),
                    stereometria_vector(x[c], 0.0f, z[c]),
                    stereometria_vector(x[b], 0.0f, z[b]));
            }
            /* verticem b tollere */
            per (j = k % reliqua; j + I < reliqua; j++)
            {
                ordo[j] = ordo[j + I];
            }
            reliqua--;
            conatus = ZEPHYRUM;
        }
        alioquin
        {
            k = (k + I) % reliqua;
            conatus++;
        }
    }
    si (reliqua >= III)
    {
        redde _recusare_attributum(p, nodus, "puncta",
            "describes an outline that could not be filled");
    }
    redde _solidum_claudere(p, nodus);
}

interior b32
_tornus (
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens)
{
    hic_manens constans character* constans LICITA[] = {
        COMMUNIA_SOLIDI, "puncta", "latera", NIHIL
    };
    constans chorda* valor;
                f32  r[PUNCTA_MAXIMA];
                f32  y[PUNCTA_MAXIMA];
                f32  cursus = 0.0f;
                i32  numerus;
                i32  latera;
                i32  j;
                i32  k;

    si (!_solidum_aperire(p, nodus, parens, LICITA))
    {
        redde FALSUM;
    }
    valor = stml_attributum_capere(nodus, "puncta");
    si (!valor || !_paria(valor, r, y, &numerus) || numerus < II)
    {
        redde _recusare_attributum(p, nodus, "puncta",
            "needs 2 to 64 points written 'r,y r,y ...'");
    }
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        si (r[j] < 0.0f)
        {
            redde _recusare_attributum(p, nodus, "puncta",
                "has a negative radius");
        }
        si (j + I < numerus && r[j] == r[j + I] && y[j] == y[j + I])
        {
            redde _recusare_attributum(p, nodus, "puncta",
                "repeats a point");
        }
    }
    si (!_integer(p, nodus, "latera", FALSUM, VIII, III,
            LATERA_MAXIMA, "must be a whole number from 3 to 64",
            &latera))
    {
        redde FALSUM;
    }
    /* superficies ad dextram itineris: ordo idem ac lateris
     * cylindri, qui sursum eundo extrorsum spectat */
    per (j = ZEPHYRUM; j + I < numerus; j++)
    {
        /* t secundum profilum currit (sursum eundo decrescit);
         * ancora in anulo latiore; s retrorsum in pariete
         * exteriore (sursum), prorsum in interiore (deorsum) */
        b32 sursum = y[j + I] > y[j];
        i32 anulus = r[j + I] > r[j] ? j + I : j;
        f32 longitudo  = (f32)sqrt((f64)((r[j + I] - r[j])
            * (r[j + I] - r[j])
            + (y[j + I] - y[j]) * (y[j + I] - y[j])));
        f32 sequens    = sursum ? cursus - longitudo
            : cursus + longitudo;
        f32 latus      = 2.0f * r[anulus]
            * (f32)sin((f64)(STEREOMETRIA_PI / (f32)latera));

        per (k = ZEPHYRUM; k < latera; k++)
        {
            _ancoram_ponere(p,
                _in_circulo(r[anulus], y[anulus], k, latera),
                sursum ? -(f32)k * latus : (f32)k * latus,
                anulus == j ? cursus : sequens);
            _quadrum(p, _in_circulo(r[j], y[j], k, latera),
                _in_circulo(r[j + I], y[j + I], k, latera),
                _in_circulo(r[j + I], y[j + I], k + I, latera),
                _in_circulo(r[j], y[j], k + I, latera));
        }
        cursus = sequens;
    }
    redde _solidum_claudere(p, nodus);
}


/* ==================================================
 * Greges et repetitores
 * ================================================== */

#define COMMUNIA_GREGIS  "titulus", "locus", "gyrus", "rotatio"

interior b32
_liberi (
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens);

/* nodus gregis cum transformatione sua */
interior b32
_grex_aperire (
                         Plastes* p,
              constans StmlNodus* nodus,
                             s32  parens,
    constans character* constans* licita,
                             i32* index)
{
       StereometriaVector translatio;
    StereometriaQuaternio rotatio;

    redde (p->gradus || _attributa_licita(p, nodus, licita))
        && _transformatio(p, nodus, &translatio, &rotatio)
        && _nodus_novus(p, nodus, VERUM, parens, translatio, rotatio,
            1.0f, index);
}

interior b32
_grex (
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens)
{
    hic_manens constans character* constans LICITA[] = {
        COMMUNIA_GREGIS, NIHIL
    };
    i32 n;

    redde _grex_aperire(p, nodus, parens, LICITA, &n)
        && _liberi(p, nodus, (s32)n);
}

/* "joint title 't' is used twice (first on line n)" */
interior b32
_recusare_iteratum (
               Plastes* p,
    constans StmlNodus* nodus,
       constans chorda* titulus,
                   i32  linea_prima)
{
    character digiti[XVI];
          i32 longitudo  = ZEPHYRUM;
          i32 k          = ZEPHYRUM;

    si (p->fractum)
    {
        redde FALSUM;
    }
    p->fractum  = VERUM;
    p->linea    = nodus->linea;
    _appone_literas(p, &longitudo, "joint title '");
    _appone_chordam(p, &longitudo, titulus);
    _appone_literas(p, &longitudo, "' is used twice (first on line ");
    fac
    {
        digiti[k++] = (character)('0' + (character)(linea_prima % X));
        linea_prima /= X;
    } dum (linea_prima && k < XVI);
    dum (k)
    {
        _appone(p, &longitudo, &digiti[--k], I);
    }
    _appone_literas(p, &longitudo, ")");
    redde FALSUM;
}

/* articulus: grex nominatus, nodus unus, cardo in origine sua
 * (plastica-motus-spec IV.1). Solum sub <figura> ipsa aut sub alio
 * articulo */
interior b32
_articulus (
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens)
{
    hic_manens constans character* constans LICITA[] = {
        COMMUNIA_GREGIS, NIHIL
    };
    constans chorda* titulus =
        stml_attributum_capere(nodus, "titulus");
                i32 k;
                i32 n;

    si (!p->figura)
    {
        redde _recusare_elementum(p, nodus,
            "is only allowed in <figura>");
    }
    si (p->claustrum)
    {
        i32 longitudo = ZEPHYRUM;

        si (!p->fractum)
        {
            p->fractum  = VERUM;
            p->linea    = nodus->linea;
            _appone_literas(p, &longitudo,
                "a joint (<articulus>) cannot be inside <");
            _appone_literas(p, &longitudo, p->claustrum);
            _appone_literas(p, &longitudo, ">");
        }
        redde FALSUM;
    }
    /* attributum vacuum STML ut absens reddit */
    si (!titulus)
    {
        redde _recusare_elementum(p, nodus,
            "needs a non-empty attribute 'titulus'");
    }
    per (k = ZEPHYRUM; k < p->articulorum; k++)
    {
        constans chorda* prior =
            stml_attributum_capere(p->fontes[k], "titulus");

        si (prior && chorda_aequalis(*prior, *titulus))
        {
            redde _recusare_iteratum(p, nodus, titulus,
                p->fontes[k]->linea);
        }
    }
    si (p->articulorum >= PLASTICA_ARTICULI_MAXIMI)
    {
        redde _recusare_elementum(p, nodus,
            "is one joint too many (at most 64)");
    }
    si (!_grex_aperire(p, nodus, parens, LICITA, &n))
    {
        redde FALSUM;
    }
    p->articuli[p->articulorum]  = n;
    p->fontes[p->articulorum]    = nodus;
    p->articulorum++;
    redde _liberi(p, nodus, (s32)n);
}

interior b32
_speculum (
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens)
{
    hic_manens constans character* constans LICITA[] = {
        COMMUNIA_GREGIS, "axis", NIHIL
    };
    constans chorda* axis = stml_attributum_capere(nodus, "axis");
                f32* reflexio;
                b32  bene;
                i32  n;

    si (!_grex_aperire(p, nodus, parens, LICITA, &n))
    {
        redde FALSUM;
    }
    si (_titulus_est(axis, "x"))
    {
        reflexio = &p->speculum_x;
    }
    alioquin si (_titulus_est(axis, "z"))
    {
        reflexio = &p->speculum_z;
    }
    alioquin
    {
        redde _recusare_attributum(p, nodus, "axis",
            "must be 'x' or 'z'");
    }
    si (!_liberi(p, nodus, (s32)n))
    {
        redde FALSUM;
    }
    *reflexio  = -*reflexio;
    bene       = _liberi(p, nodus, (s32)n);
    *reflexio  = -*reflexio;
    redde bene;
}

interior b32
_series (
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens)
{
    hic_manens constans character* constans LICITA[] = {
        COMMUNIA_GREGIS, "numerus", "gressus", "gyrus_gressus",
        "magnitudo_gressus", "semen_gressus",
        "motus_texturae_gressus", NIHIL
    };
    constans chorda* valor;
                f32  gressus[III];
                f32  crementum[III];
                f32  magnitudo_prior[III];
                f32  motus_gressus[II];
                f32  motus_prior[II];
                f32  gyrus;
                b32  bene = VERUM;
                i32  semen_gressus;
                i32  semen_prius;
                i32  numerus;
                i32  n;
                i32  k;

    si (   !_grex_aperire(p, nodus, parens, LICITA, &n)
        || !_integer(p, nodus, "numerus", VERUM, I, I,
            NUMERUS_MAXIMUS, "must be a whole number from 1 to 256",
            &numerus)
        || !_realis(p, nodus, "gyrus_gressus", FALSUM, 0.0f, &gyrus))
    {
        redde FALSUM;
    }
    crementum[ZEPHYRUM]  = 0.0f;
    crementum[I]         = 0.0f;
    crementum[II]        = 0.0f;
    gressus[ZEPHYRUM]    = 0.0f;
    gressus[I]           = 0.0f;
    gressus[II]          = 0.0f;
    valor                = stml_attributum_capere(nodus, "gressus");
    si (valor && !_numeri(valor, gressus, III))
    {
        redde _recusare_attributum(p, nodus, "gressus",
            "needs 3 numbers (x y z)");
    }
    valor = stml_attributum_capere(nodus, "magnitudo_gressus");
    si (valor && !_numeri(valor, crementum, III))
    {
        redde _recusare_attributum(p, nodus, "magnitudo_gressus",
            "needs 3 numbers (x y z)");
    }
    motus_gressus[ZEPHYRUM] = 0.0f;
    motus_gressus[I] = 0.0f;
    valor = stml_attributum_capere(nodus, "motus_texturae_gressus");
    si (valor && !_numeri(valor, motus_gressus, II))
    {
        redde _recusare_attributum(p, nodus, "motus_texturae_gressus",
            "needs 2 numbers (texels along and down)");
    }
    memcpy(motus_prior, p->motus_seriei, magnitudo(motus_prior));
    si (!_integer(p, nodus, "semen_gressus", FALSUM, ZEPHYRUM,
        ZEPHYRUM, M * M, "must be a whole number from 0 to 1000000",
        &semen_gressus))
    {
        redde FALSUM;
    }
    /* gressus seriei exterioris manet: interior ei addit */
    semen_prius = p->semen_seriei;
    memcpy(magnitudo_prior, p->magnitudo_seriei,
        magnitudo(magnitudo_prior));
    per (k = ZEPHYRUM; bene && k < numerus; k++)
    {
        i32 exemplar;
        i32 a;

        p->semen_seriei = semen_prius + k * semen_gressus;
        p->motus_seriei[ZEPHYRUM] = motus_prior[ZEPHYRUM]
            + motus_gressus[ZEPHYRUM] * (f32)k;
        p->motus_seriei[I] = motus_prior[I]
            + motus_gressus[I] * (f32)k;
        per (a = ZEPHYRUM; a < III; a++)
        {
            p->magnitudo_seriei[a] = magnitudo_prior[a]
                + crementum[a] * (f32)k;
        }
        bene = _nodus_novus(p, nodus, FALSUM, (s32)n,
                stereometria_vector(gressus[ZEPHYRUM] * (f32)k,
                    gressus[I] * (f32)k, gressus[II] * (f32)k),
                _circa_axem(0.0f, 1.0f, 0.0f, gyrus * (f32)k), 1.0f,
                &exemplar)
            && _liberi(p, nodus, (s32)exemplar);
    }
    p->semen_seriei = semen_prius;
    memcpy(p->magnitudo_seriei, magnitudo_prior,
        magnitudo(magnitudo_prior));
    memcpy(p->motus_seriei, motus_prior, magnitudo(motus_prior));
    redde bene;
}

interior b32
_sparsio (
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens)
{
    hic_manens constans character* constans LICITA[] = {
        COMMUNIA_GREGIS, "numerus", "semen", "area", "gyrus_varius",
        "magnitudo_varia", NIHIL
    };
    constans chorda* valor;
                f32  area[II];
                f32  varia;
                b32  gyrus_varius = FALSUM;
                i32  numerus;
                i32  semen;
                i32  n;
                i32  k;

    si (   !_grex_aperire(p, nodus, parens, LICITA, &n)
        || !_integer(p, nodus, "numerus", VERUM, I, I,
            NUMERUS_MAXIMUS, "must be a whole number from 1 to 256",
            &numerus)
        || !_semen(p, nodus, &semen)
        || !_realis(p, nodus, "magnitudo_varia", FALSUM, 0.0f,
            &varia))
    {
        redde FALSUM;
    }
    si (varia < 0.0f || varia >= 1.0f)
    {
        redde _recusare_attributum(p, nodus, "magnitudo_varia",
            "must be at least 0 and less than 1");
    }
    valor = stml_attributum_capere(nodus, "area");
    si (   !valor || !_numeri(valor, area, II)
        || area[ZEPHYRUM] < 0.0f || area[I] < 0.0f)
    {
        redde _recusare_attributum(p, nodus, "area",
            "needs 2 numbers that are not negative (x z)");
    }
    valor = stml_attributum_capere(nodus, "gyrus_varius");
    si (valor)
    {
        si (!_titulus_est(valor, "verum"))
        {
            redde _recusare_attributum(p, nodus, "gyrus_varius",
                "must be 'verum' or absent");
        }
        gyrus_varius = VERUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        /* quattuor sortes per exemplar, semper omnes: exemplar k
         * idem manet cum attributa alia mutantur */
        f32 x      = (_fortuna(semen, k, ZEPHYRUM) - 0.5f)
            * area[ZEPHYRUM];
        f32 z      = (_fortuna(semen, k, I) - 0.5f) * area[I];
        f32 gyrus  = _fortuna(semen, k, II) * 360.0f;
        f32 scala  = 1.0f
            + varia * (_fortuna(semen, k, III) * 2.0f - 1.0f);
        i32 exemplar;

        si (   !_nodus_novus(p, nodus, FALSUM, (s32)n,
                stereometria_vector(x, 0.0f, z),
                _circa_axem(0.0f, 1.0f, 0.0f,
                    gyrus_varius ? gyrus : 0.0f), scala, &exemplar)
            || !_liberi(p, nodus, (s32)exemplar))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Textura manu scripta
 * ================================================== */

/* elementum textura (titulus, latitudo, colores; litterae intus):
 * liber radicis; in gradu 0 in tabulam 'manu' it, nodum nullum facit */
interior b32
_textura_manu (
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", "latitudo", "colores", NIHIL
    };
    constans chorda* titulus;
    constans chorda* valor;
        TexturaManu* nova;
                 i8  colores[COLORES_MANU * III];
                 i8  signa[PLASTICA_TEXTURA_MANU_MAXIMA
                     * PLASTICA_TEXTURA_MANU_MAXIMA];
                i32 colorum;
                i32 latitudo;
                i32 numerus = ZEPHYRUM;
                i32 liberi  = nodus->liberi
                    ? xar_numerus(nodus->liberi) : ZEPHYRUM;
                i32 k;
                i32 j;

    si (p->gradus)
    {
        redde VERUM;
    }
    si (parens != ZEPHYRUM)
    {
        redde _recusare_elementum(p, nodus,
            "must be a direct child of <res>");
    }
    si (!_attributa_licita(p, nodus, LICITA))
    {
        redde FALSUM;
    }
    titulus = stml_attributum_capere(nodus, "titulus");
    si (!titulus || titulus->mensura == ZEPHYRUM)
    {
        redde _recusare_attributum(p, nodus, "titulus",
            "is required");
    }
    per (k = ZEPHYRUM; k < PLASTICA_TEXTURA_GENERA; k++)
    {
        si (_titulus_est(titulus, plastica_textura_genus(k)))
        {
            redde _recusare(p, nodus->linea, "a hand-written texture"
                " cannot take the name of the generated texture '",
                titulus, "'");
        }
    }
    per (k = ZEPHYRUM; k < p->manu_numerus; k++)
    {
        si (chorda_aequalis(p->manu[k].titulus, *titulus))
        {
            redde _recusare(p, nodus->linea, "texture '", titulus,
                "' is defined twice");
        }
    }
    si (p->manu_numerus >= MANU_MAXIMAE)
    {
        redde _recusare_elementum(p, nodus,
            "is one hand-written texture too many (more than 32)");
    }
    si (!_integer(p, nodus, "latitudo", VERUM, I, I,
        PLASTICA_TEXTURA_MANU_MAXIMA,
        "must be a whole number from 1 to 16", &latitudo))
    {
        redde FALSUM;
    }
    valor = stml_attributum_capere(nodus, "colores");
    si (   !valor
        || !_colores_legere(valor, colores, COLORES_MANU, &colorum)
        || colorum == ZEPHYRUM)
    {
        redde _recusare_attributum(p, nodus, "colores",
            "needs 1 to 16 colours written '#rgb #rgb ...'");
    }
    si (_habet_elementa(nodus))
    {
        redde _recusare_elementum(p, nodus,
            "holds letters only and cannot have children");
    }
    per (k = ZEPHYRUM; k < liberi; k++)
    {
        StmlNodus** locus =
            (StmlNodus**)xar_obtinere(nodus->liberi, k);

        si (   !locus || !*locus
            || (*locus)->genus != STML_NODUS_TEXTUS
            || !(*locus)->valor)
        {
            perge;
        }
        valor = (*locus)->valor;
        per (j = ZEPHYRUM; j < valor->mensura; j++)
        {
            i8 littera = valor->datum[j];

            si (_spatium(littera))
            {
                perge;
            }
            si (littera < 'a' || littera >= 'a' + colorum)
            {
                chorda una = chorda_sectio(*valor, j, j + I);

                redde _recusare(p, nodus->linea,
                    "<textura> uses the letter '", &una,
                    "' which has no colour");
            }
            si (numerus >= PLASTICA_TEXTURA_MANU_MAXIMA
                * PLASTICA_TEXTURA_MANU_MAXIMA)
            {
                redde _recusare_elementum(p, nodus,
                    "is larger than 16 x 16");
            }
            signa[numerus++] = (i8)(littera - 'a');
        }
    }
    si (numerus == ZEPHYRUM || numerus % latitudo)
    {
        redde _recusare_elementum(p, nodus,
            "needs whole rows of 'latitudo' letters");
    }
    si (numerus / latitudo > PLASTICA_TEXTURA_MANU_MAXIMA)
    {
        redde _recusare_elementum(p, nodus, "is larger than 16 x 16");
    }
    nova                  = &p->manu[p->manu_numerus];
    nova->titulus         = chorda_transcribere(*titulus, p->piscina);
    nova->imago.latitudo  = latitudo;
    nova->imago.altitudo  = numerus / latitudo;
    nova->imago.pixela    = (i8*)piscina_allocare(p->piscina,
        (memoriae_index)numerus * IV);
    si (!nova->imago.pixela)
    {
        redde _recusare(p, nodus->linea, "out of memory", NIHIL,
            NIHIL);
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        memcpy(nova->imago.pixela + k * IV, colores + signa[k] * III,
            III);
        nova->imago.pixela[k * IV + III] = CCLV;
    }
    p->manu_numerus++;
    redde VERUM;
}


/* ==================================================
 * Ambulatio
 * ================================================== */

nomen b32 (*Factor)(
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens);

nomen structura {
    constans character* titulus;
                Factor  factor;
                   b32  claudit;   /* intra hoc articulus vetatur */
} Elementum;

hic_manens constans Elementum ELEMENTA[] = {
    { "capsa", _capsa, FALSUM },
    { "cylindrus", _cylindrus, FALSUM },
    { "sphaera", _sphaera, FALSUM },
    { "cuneus", _cuneus, FALSUM },
    { "extrusio", _extrusio, FALSUM },
    { "tornus", _tornus, FALSUM },
    { "grex", _grex, VERUM },
    { "speculum", _speculum, VERUM },
    { "series", _series, VERUM },
    { "sparsio", _sparsio, VERUM },
    { "textura", _textura_manu, FALSUM },
    { "articulus", _articulus, FALSUM },
    { NIHIL, NIHIL, FALSUM }
};

interior b32
_textus_vacuus (
    constans chorda* valor)
{
    i32 k;

    si (!valor)
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < valor->mensura; k++)
    {
        si (!_spatium(valor->datum[k]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_liberi (
               Plastes* p,
    constans StmlNodus* nodus,
                   s32  parens)
{
    i32 numerus = nodus->liberi ? xar_numerus(nodus->liberi)
        : ZEPHYRUM;
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
                StmlNodus** locus =
                    (StmlNodus**)xar_obtinere(nodus->liberi, k);
        constans StmlNodus* liber = locus ? *locus : NIHIL;

        si (!liber || liber->genus == STML_NODUS_COMMENTUM)
        {
            perge;
        }
        si (liber->genus == STML_NODUS_TEXTUS)
        {
            si (!_textus_vacuus(liber->valor))
            {
                redde _recusare(p, liber->linea,
                    "unexpected text between elements", NIHIL,
                    NIHIL);
            }
            perge;
        }
        si (liber->genus != STML_NODUS_ELEMENTUM)
        {
            redde _recusare(p, liber->linea,
                "only elements and comments are allowed here", NIHIL,
                NIHIL);
        }
        /* elementa motus, liberi <figura> ipsius: plastica_motus
         * ea legit */
        si (   p->figura && nodus == p->radix
            && (   _titulus_est(liber->titulus, "positura")
                || _titulus_est(liber->titulus, "cyclus")
                || _titulus_est(liber->titulus, "membrum")))
        {
            perge;
        }
        per (j = ZEPHYRUM; ELEMENTA[j].titulus; j++)
        {
            si (_titulus_est(liber->titulus, ELEMENTA[j].titulus))
            {
                frange;
            }
        }
        si (!ELEMENTA[j].titulus)
        {
            redde _recusare(p, liber->linea, "unknown element <",
                liber->titulus, ">");
        }
        {
            constans character* prius = p->claustrum;
                           b32  bene;

            si (ELEMENTA[j].claudit)
            {
                p->claustrum = ELEMENTA[j].titulus;
            }
            bene          = ELEMENTA[j].factor(p, liber, parens);
            p->claustrum  = prius;
            si (!bene)
            {
                redde FALSUM;
            }
        }
    }
    redde !p->fractum;
}

interior b32
_radix (
               Plastes* p,
    constans StmlNodus* radix)
{
    hic_manens constans character* constans LICITA[] = {
        "titulus", "obex", NIHIL
    };
    hic_manens constans character* constans LICITA_FIGURAE[] = {
        "titulus", NIHIL
    };
    constans chorda* obex = stml_attributum_capere(radix, "obex");
                f32  latera[II];
                i32  n;

    p->radix        = radix;
    p->figura       = _titulus_est(radix->titulus, "figura");
    p->articulorum  = ZEPHYRUM;
    p->nodorum      = ZEPHYRUM;
    p->solidorum    = ZEPHYRUM;
    p->verticum     = ZEPHYRUM;
    p->indicum      = ZEPHYRUM;
    p->speculum_x   = 1.0f;
    p->speculum_z   = 1.0f;
    si (!p->gradus)
    {
        constans chorda* titulus;

        si (!p->figura && !_titulus_est(radix->titulus, "res"))
        {
            redde _recusare(p, radix->linea,
                "the root element must be <res> or <figura>, got <",
                radix->titulus, ">");
        }
        si (!_attributa_licita(p, radix,
            p->figura ? LICITA_FIGURAE : LICITA))
        {
            redde FALSUM;
        }
        titulus = stml_attributum_capere(radix, "titulus");
        si (!titulus || titulus->mensura == ZEPHYRUM)
        {
            redde _recusare_elementum(p, radix,
                "needs a non-empty attribute 'titulus'");
        }
    }
    si (   !_nodus_novus(p, radix, VERUM, -I,
            stereometria_vector(0.0f, 0.0f, 0.0f),
            stereometria_quaternio_identitas(), 1.0f, &n)
        || !_liberi(p, radix, (s32)n))
    {
        redde FALSUM;
    }
    si (!p->gradus && p->solidorum == ZEPHYRUM)
    {
        redde _recusare_elementum(p, radix, "has no solids");
    }
    /* obex invisus: pars ultima, sub radice */
    si (obex)
    {
        Proprietates q;
                 f32 dimidia[III];
                 i32 m;

        si (_numeri(obex, latera, I))
        {
            latera[I] = latera[ZEPHYRUM];
        }
        alioquin si (!_numeri(obex, latera, II))
        {
            redde _recusare(p, radix->linea, "<res> attribute 'obex'"
                " needs 1 or 2 positive numbers (width, or width and"
                " depth)", NIHIL, NIHIL);
        }
        si (!(latera[ZEPHYRUM] > 0.0f) || !(latera[I] > 0.0f))
        {
            redde _recusare(p, radix->linea, "<res> attribute 'obex'"
                " needs 1 or 2 positive numbers (width, or width and"
                " depth)", NIHIL, NIHIL);
        }
        memset(&q, ZEPHYRUM, magnitudo(q));
        q.color[ZEPHYRUM]  = 1.0f;
        q.color[I]         = 1.0f;
        q.color[II]        = 1.0f;
        q.color[III]       = 0.0f;
        q.limen_alpha      = 0.5f;
        q.signa            = STATUA_SIGNUM_SOLIDUM;
        q.textura          = -I;
        si (!_nodus_novus(p, radix, FALSUM, (s32)n,
            stereometria_vector(0.0f, 0.0f, 0.0f),
            stereometria_quaternio_identitas(), 1.0f, &m))
        {
            redde FALSUM;
        }
        si (p->gradus)
        {
            p->partes[m] = chorda_ex_literis("obex", p->piscina);
        }
        dimidia[ZEPHYRUM]  = latera[ZEPHYRUM] * 0.5f;
        dimidia[I]         = OBICIS_ALTITUDO * 0.5f;
        dimidia[II]        = latera[I] * 0.5f;
        _solidum_inire(p, radix->linea, m, &q);
        _capsam_emittere(p, dimidia);
        si (!_solidum_claudere(p, radix))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Publica
 * ================================================== */

interior vacuum*
_locus (
           Piscina* piscina,
    memoriae_index  mensura)
{
    vacuum* p = piscina_allocare(piscina, mensura ? mensura : I);

    si (p)
    {
        memset(p, ZEPHYRUM, mensura ? mensura : I);
    }
    redde p;
}

interior PlasticaFructus
_fructus_recusatus (
    constans Plastes* p,
             Piscina* piscina)
{
    PlasticaFructus f;

    memset(&f, ZEPHYRUM, magnitudo(f));
    f.error = chorda_ex_literis(p->error, piscina);
    f.linea = p->linea;
    redde f;
}

PlasticaFructus
plastica_legere (
     chorda  textus,
    Piscina* piscina)
{
    redde plastica_legere_seminis(textus, ZEPHYRUM, piscina);
}

PlasticaFructus
plastica_legere_seminis (
     chorda  textus,
        i32  semen,
    Piscina* piscina)
{
    PlasticaFructus  f;
            Plastes  p;
       StmlResultus  r;
             Statua* s;

    memset(&f, ZEPHYRUM, magnitudo(f));
    memset(&p, ZEPHYRUM, magnitudo(p));
    p.piscina        = piscina;
    p.semen_additum  = semen;
    si (semen > M * M)
    {
        (vacuum)_recusare(&p, ZEPHYRUM,
            "the added seed must be from 0 to 1000000", NIHIL, NIHIL);
        redde _fructus_recusatus(&p, piscina);
    }
    r = stml_legere(textus, piscina, internamentum_creare(piscina));
    si (!r.successus || !r.elementum_radix)
    {
        chorda causa = r.error;

        (vacuum)_recusare(&p,
            r.successus ? ZEPHYRUM : r.linea_erroris,
            "the description is not valid STML: ",
            causa.mensura ? &causa : NIHIL,
            causa.mensura ? NIHIL : "no root element");
        redde _fructus_recusatus(&p, piscina);
    }
    p.gradus = ZEPHYRUM;
    si (!_radix(&p, r.elementum_radix))
    {
        redde _fructus_recusatus(&p, piscina);
    }

    s = (Statua*)_locus(piscina, magnitudo(Statua));
    si (s)
    {
        s->nodorum      = p.nodorum;
        s->nodi         = (StatuaNodus*)_locus(piscina,
            p.nodorum * magnitudo(StatuaNodus));
        s->retium       = p.solidorum;
        s->retia        = (StatuaRete*)_locus(piscina,
            p.solidorum * magnitudo(StatuaRete));
        s->partium      = p.solidorum;
        s->partes       = (StatuaPars*)_locus(piscina,
            p.solidorum * magnitudo(StatuaPars));
        s->materiarum   = p.solidorum;
        s->materiae     = (StatuaMateria*)_locus(piscina,
            p.solidorum * magnitudo(StatuaMateria));
        s->verticum     = p.verticum;
        s->positiones   = (f32*)_locus(piscina,
            p.verticum * III * magnitudo(f32));
        s->normales     = (f32*)_locus(piscina,
            p.verticum * III * magnitudo(f32));
        s->coordinatae  = (f32*)_locus(piscina,
            p.verticum * II * magnitudo(f32));
        s->indicum      = p.indicum;
        s->indices      = (i32*)_locus(piscina,
            p.indicum * magnitudo(i32));
        p.partes        = (chorda*)_locus(piscina,
            p.nodorum * magnitudo(chorda));
        s->texturarum   = p.texturarum;
        s->texturae     = (Imago*)_locus(piscina,
            p.texturarum * magnitudo(Imago));
        si (s->texturae)
        {
            memcpy(s->texturae, p.texturae,
                p.texturarum * magnitudo(Imago));
        }
    }
    si (   !s || !s->nodi || !s->texturae || !s->retia || !s->partes
        || !s->materiae
        || !s->positiones || !s->normales || !s->coordinatae
        || !s->indices || !p.partes)
    {
        (vacuum)_recusare(&p, ZEPHYRUM, "out of memory", NIHIL,
            NIHIL);
        redde _fructus_recusatus(&p, piscina);
    }
    p.statua = s;
    p.gradus = I;
    si (!_radix(&p, r.elementum_radix) || !statua_finire(s, piscina))
    {
        (vacuum)_recusare(&p, ZEPHYRUM,
            "the model could not be finished", NIHIL, NIHIL);
        redde _fructus_recusatus(&p, piscina);
    }
    si (p.articulorum)
    {
        f.articuli = (i32*)_locus(piscina,
            p.articulorum * magnitudo(i32));
        si (!f.articuli)
        {
            (vacuum)_recusare(&p, ZEPHYRUM, "out of memory", NIHIL,
                NIHIL);
            redde _fructus_recusatus(&p, piscina);
        }
        memcpy(f.articuli, p.articuli,
            p.articulorum * magnitudo(i32));
    }
    f.articulorum  = p.articulorum;
    f.successus    = VERUM;
    f.statua       = s;
    f.partes       = p.partes;
    redde f;
}
