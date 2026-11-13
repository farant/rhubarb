/* probatio_plastica.c - descriptio textualis in statuam (plastica.h)
 *
 * capsa: triangula, fines, normales extrorsum, signa, color, partes;
 * grex: locus et gyrus liberos movent; rotatio; invisibilis;
 * recusationes cum linea; determinata. T2: cylindrus, sphaera
 * (asperitas sine rimis), cuneus, extrusio (concava), tornus (paries
 * interior), speculum (imago vera), series, sparsio, recusationes
 * solidorum et repetitorum.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "stereometria.h"
#include "statua.h"
#include "plastica.h"
#include "credo.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define NODI_MAXIMI DXII

nomen structura {
    f32 minimum[III];
    f32 maximum[III];
    i32 triangulorum;
} Fines;

interior PlasticaFructus
_lege (
        CredoContextus* c,
    constans character* literae)
{
    redde plastica_legere(chorda_ex_literis(literae, c->piscina),
        c->piscina);
}

/* fines mundi omnium retium et numerus triangulorum */
interior Fines
_fines (
    constans Statua* s)
{
    StereometriaMatrix m[NODI_MAXIMI];
                 Fines f;
                   i32 n;
                   i32 j;
                   i32 k;
                   i32 a;

    per (a = ZEPHYRUM; a < III; a++)
    {
        f.minimum[a] = 1.0e9f;
        f.maximum[a] = -1.0e9f;
    }
    f.triangulorum = ZEPHYRUM;
    si (s->nodorum > NODI_MAXIMI)
    {
        redde f;
    }
    statua_positura(s, -I, 0.0f, m);
    per (n = ZEPHYRUM; n < s->nodorum; n++)
    {
        constans StatuaRete* rete;

        si (s->nodi[n].rete < ZEPHYRUM)
        {
            perge;
        }
        rete = &s->retia[s->nodi[n].rete];
        per (j = ZEPHYRUM; j < rete->partium; j++)
        {
            constans StatuaPars* pars =
                &s->partes[rete->pars_prima + j];

            f.triangulorum += pars->indicum / III;
            per (k = ZEPHYRUM; k < pars->indicum; k++)
            {
                constans f32* p = s->positiones
                    + s->indices[pars->index_primus + k] * III;
                StereometriaVector v =
                    stereometria_punctum_transformare(m[n],
                        stereometria_vector(p[ZEPHYRUM], p[I], p[II]),
                        NIHIL);
                              f32 w[III];

                w[ZEPHYRUM]  = v.x;
                w[I]         = v.y;
                w[II]        = v.z;
                per (a = ZEPHYRUM; a < III; a++)
                {
                    si (w[a] < f.minimum[a])
                    {
                        f.minimum[a] = w[a];
                    }
                    si (w[a] > f.maximum[a])
                    {
                        f.maximum[a] = w[a];
                    }
                }
            }
        }
    }
    redde f;
}

#define FINES_SUNT(f, x0, x1, y0, y1, z0, z1) \
    fac { \
        CREDO_F64_PROXIMUS ((f64)(f).minimum[ZEPHYRUM], (x0), 1.0e-4); \
        CREDO_F64_PROXIMUS ((f64)(f).maximum[ZEPHYRUM], (x1), 1.0e-4); \
        CREDO_F64_PROXIMUS ((f64)(f).minimum[I], (y0), 1.0e-4); \
        CREDO_F64_PROXIMUS ((f64)(f).maximum[I], (y1), 1.0e-4); \
        CREDO_F64_PROXIMUS ((f64)(f).minimum[II], (z0), 1.0e-4); \
        CREDO_F64_PROXIMUS ((f64)(f).maximum[II], (z1), 1.0e-4); \
    } dum (FALSUM)

/* nodus primus cum reti; -1 si nullus */
interior s32
_nodus_retis (
    constans Statua* s)
{
    i32 n;

    per (n = ZEPHYRUM; n < s->nodorum; n++)
    {
        si (s->nodi[n].rete >= ZEPHYRUM)
        {
            redde (s32)n;
        }
    }
    redde -I;
}

interior vacuum
_sectio_capsa (
    CredoContextus* c)
{
     PlasticaFructus f = _lege(c,
         "<res titulus=\"well\">\n"
         "  <capsa titulus=\"murus\" magnitudo=\"2 1 4\"\n"
         "         color=\"#804020\" signum=\"solidum\"/>\n"
         "</res>\n");
           constans Statua* s;
       constans StatuaPars* pars;
    constans StatuaMateria* materia;
                     Fines  fines;
                       s32  n;
                       i32  t;
                       i32  k;

    CREDO_VERUM (f.successus);
    CREDO_NON_NIHIL (f.statua);
    CREDO_CHORDA_VACUA (f.error);
    CREDO_AEQUALIS_I32 (f.linea, ZEPHYRUM);
    si (!f.statua)
    {
        redde;
    }
    s = f.statua;
    /* radix <res> et solidum */
    CREDO_AEQUALIS_I32 (s->nodorum, II);
    CREDO_AEQUALIS_S32 (s->nodi[ZEPHYRUM].parens, -I);
    CREDO_AEQUALIS_S32 (s->nodi[ZEPHYRUM].rete, -I);
    n = _nodus_retis(s);
    CREDO_AEQUALIS_S32 (n, I);
    CREDO_AEQUALIS_S32 (s->nodi[I].parens, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (s->nodi[I].signa, STATUA_SIGNUM_SOLIDUM);
    CREDO_NON_NIHIL (f.partes);
    CREDO_CHORDA_AEQUALIS_LITERIS (f.partes[ZEPHYRUM], "well");
    CREDO_CHORDA_AEQUALIS_LITERIS (f.partes[I], "murus");

    fines = _fines(s);
    CREDO_AEQUALIS_I32 (fines.triangulorum, XII);
    /* basis in humo: y 0 -> 1; x et z circa centrum */
    FINES_SUNT (fines, -1.0, 1.0, 0.0, 1.0, -2.0, 2.0);

    /* normales: axis unus, longitudo I, extrorsum; ordo verticum
     * cum normali congruit */
    pars = &s->partes[s->retia[s->nodi[I].rete].pars_prima];
    CREDO_AEQUALIS_I32 (pars->indicum, XXXVI);
    per (t = ZEPHYRUM; t < pars->indicum / III; t++)
    {
        StereometriaVector p[III];
        StereometriaVector normalis;
        StereometriaVector facies;
        StereometriaVector centrum =
            stereometria_vector(0.0f, 0.5f, 0.0f);

        per (k = ZEPHYRUM; k < III; k++)
        {
            i32 v = s->indices[pars->index_primus + t * III + k];

            p[k] = stereometria_vector(s->positiones[v * III],
                s->positiones[v * III + I],
                s->positiones[v * III + II]);
            normalis = stereometria_vector(s->normales[v * III],
                s->normales[v * III + I], s->normales[v * III + II]);
            CREDO_F64_PROXIMUS (
                (f64)stereometria_longitudo(normalis), 1.0, 1.0e-5);
            CREDO_F64_PROXIMUS ((f64)(normalis.x * normalis.x
                * normalis.y * normalis.y + normalis.y * normalis.y
                * normalis.z * normalis.z + normalis.x * normalis.x
                * normalis.z * normalis.z), 0.0, 1.0e-6);
            CREDO_VERUM (stereometria_productum(normalis,
                stereometria_subtrahere(p[k], centrum)) > 0.0f);
        }
        facies = stereometria_crux(
            stereometria_subtrahere(p[I], p[ZEPHYRUM]),
            stereometria_subtrahere(p[II], p[ZEPHYRUM]));
        CREDO_VERUM (stereometria_productum(facies, normalis) > 0.0f);
    }

    /* materia: color planus, opaca */
    CREDO_VERUM (pars->materia >= ZEPHYRUM);
    si (pars->materia < ZEPHYRUM)
    {
        redde;
    }
    materia = &s->materiae[pars->materia];
    CREDO_AEQUALIS_S32 (materia->textura, -I);
    CREDO_F64_PROXIMUS ((f64)materia->color[ZEPHYRUM],
        128.0 / 255.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)materia->color[I], 64.0 / 255.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)materia->color[II], 32.0 / 255.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)materia->color[III], 1.0, 1.0e-6);
    CREDO_VERUM (materia->limen_alpha < 0.0f);
}

interior vacuum
_sectio_ordinaria (
    CredoContextus* c)
{
    /* sine colore: albus; sine signo: nullum; sine titulo: vacuus;
     * color brevis #rgb */
    PlasticaFructus f = _lege(c,
        "<res titulus=\"a\">\n"
        "  <capsa magnitudo=\"1 1 1\"/>\n"
        "  <capsa magnitudo=\"1 1 1\" color=\"#f08\"\n"
        "         signum=\"gressibile\" locus=\"3 2 1\"/>\n"
        "</res>\n");
    constans Statua* s;
              Fines  fines;

    CREDO_VERUM (f.successus);
    si (!f.statua)
    {
        redde;
    }
    s = f.statua;
    CREDO_AEQUALIS_I32 (s->nodorum, III);
    CREDO_AEQUALIS_I32 (s->nodi[I].signa, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (s->nodi[II].signa, STATUA_SIGNUM_GRESSIBILE);
    CREDO_CHORDA_VACUA (f.partes[I]);
    CREDO_AEQUALIS_I32 (s->materiarum, II);
    CREDO_F64_PROXIMUS ((f64)s->materiae[ZEPHYRUM].color[ZEPHYRUM],
        1.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)s->materiae[ZEPHYRUM].color[II], 1.0,
        1.0e-6);
    CREDO_F64_PROXIMUS ((f64)s->materiae[I].color[ZEPHYRUM], 1.0,
        1.0e-6);
    CREDO_F64_PROXIMUS ((f64)s->materiae[I].color[I], 0.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)s->materiae[I].color[II],
        136.0 / 255.0, 1.0e-6);
    fines = _fines(s);
    CREDO_AEQUALIS_I32 (fines.triangulorum, XXIV);
    /* capsa altera: basis ad (3, 2, 1) */
    FINES_SUNT (fines, -0.5, 3.5, 0.0, 3.0, -0.5, 1.5);
}

interior vacuum
_sectio_grex (
    CredoContextus* c)
{
    /* grex liberum movet et vertit: gyrus 90 fert +x in -z, ergo
     * capsa 4 x 1 x 2 fit 2 x 1 x 4 circa (5, 0, 7) */
    PlasticaFructus f = _lege(c,
        "<res titulus=\"a\">\n"
        "  <grex titulus=\"g\" locus=\"5 0 7\" gyrus=\"90\">\n"
        "    <capsa magnitudo=\"4 1 2\"/>\n"
        "  </grex>\n"
        "</res>\n");
              Fines fines;

    CREDO_VERUM (f.successus);
    si (!f.statua)
    {
        redde;
    }
    CREDO_AEQUALIS_I32 (f.statua->nodorum, III);
    CREDO_AEQUALIS_S32 (f.statua->nodi[I].rete, -I);
    CREDO_AEQUALIS_S32 (f.statua->nodi[II].parens, I);
    CREDO_CHORDA_AEQUALIS_LITERIS (f.partes[I], "g");
    fines = _fines(f.statua);
    FINES_SUNT (fines, 4.0, 6.0, 0.0, 1.0, 5.0, 9.0);

    /* greges nidificati: translationes adduntur, locus liberi in
     * spatio gregis versi (1 0 0 fit 0 0 -1) */
    f = _lege(c,
        "<res titulus=\"a\">\n"
        "  <grex locus=\"10 1 10\">\n"
        "    <grex gyrus=\"90\">\n"
        "      <capsa magnitudo=\"1 1 1\" locus=\"1 0 0\"/>\n"
        "    </grex>\n"
        "  </grex>\n"
        "</res>\n");
    CREDO_VERUM (f.successus);
    si (!f.statua)
    {
        redde;
    }
    fines = _fines(f.statua);
    FINES_SUNT (fines, 9.5, 10.5, 1.0, 2.0, 8.5, 9.5);

    /* gyrus solidi ipsius circa basim suam */
    f = _lege(c,
        "<res titulus=\"a\">\n"
        "  <capsa magnitudo=\"4 1 2\" locus=\"5 0 7\" gyrus=\"90\"/>\n"
        "</res>\n");
    CREDO_VERUM (f.successus);
    si (!f.statua)
    {
        redde;
    }
    fines = _fines(f.statua);
    FINES_SUNT (fines, 4.0, 6.0, 0.0, 1.0, 5.0, 9.0);
}

interior vacuum
_sectio_rotatio (
    CredoContextus* c)
{
    /* rotatio x 90: +y in +z; columna 1 x 2 x 1 iacet secundum z */
    PlasticaFructus f = _lege(c,
        "<res titulus=\"a\">\n"
        "  <capsa magnitudo=\"1 2 1\" rotatio=\"90 0 0\"/>\n"
        "</res>\n");
              Fines fines;

    CREDO_VERUM (f.successus);
    si (!f.statua)
    {
        redde;
    }
    fines = _fines(f.statua);
    FINES_SUNT (fines, -0.5, 0.5, -0.5, 0.5, 0.0, 2.0);

    /* rotatio z 90: +x in +y; trabs 2 x 1 x 1 surgit, basis eius
     * (y 0 -> 1) fit x -1 -> 0 */
    f = _lege(c,
        "<res titulus=\"a\">\n"
        "  <capsa magnitudo=\"2 1 1\" rotatio=\"0 0 90\"/>\n"
        "</res>\n");
    CREDO_VERUM (f.successus);
    si (!f.statua)
    {
        redde;
    }
    fines = _fines(f.statua);
    FINES_SUNT (fines, -1.0, 0.0, -1.0, 1.0, -0.5, 0.5);

    /* gyrus primus, deinde rotatio: gyrus 90 capsam 4 x 1 x 2 in
     * 2 x 1 x 4 vertit, rotatio x 90 deinde +y in +z fert */
    f = _lege(c,
        "<res titulus=\"a\">\n"
        "  <capsa magnitudo=\"4 1 2\" gyrus=\"90\"\n"
        "         rotatio=\"90 0 0\"/>\n"
        "</res>\n");
    CREDO_VERUM (f.successus);
    si (!f.statua)
    {
        redde;
    }
    fines = _fines(f.statua);
    FINES_SUNT (fines, -1.0, 1.0, -2.0, 2.0, 0.0, 1.0);
}

interior vacuum
_sectio_invisibilis (
    CredoContextus* c)
{
    PlasticaFructus f = _lege(c,
        "<res titulus=\"a\">\n"
        "  <capsa magnitudo=\"1 1 1\"/>\n"
        "  <capsa magnitudo=\"1 2 1\" signum=\"solidum\"\n"
        "         invisibilis=\"verum\"/>\n"
        "</res>\n");
           constans Statua* s;
    constans StatuaMateria* materia;

    CREDO_VERUM (f.successus);
    si (!f.statua)
    {
        redde;
    }
    s = f.statua;
    CREDO_AEQUALIS_I32 (s->nodi[II].signa, STATUA_SIGNUM_SOLIDUM);
    materia = &s->materiae[s->partes[
        s->retia[s->nodi[II].rete].pars_prima].materia];
    CREDO_F64_PROXIMUS ((f64)materia->color[III], 0.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)materia->limen_alpha, 0.5, 1.0e-6);
    materia = &s->materiae[s->partes[
        s->retia[s->nodi[I].rete].pars_prima].materia];
    CREDO_F64_PROXIMUS ((f64)materia->color[III], 1.0, 1.0e-6);
    CREDO_VERUM (materia->limen_alpha < 0.0f);
}

nomen structura {
    constans character* textus;
                   i32  linea;      /* 0 = non probatur */
    constans character* fragmentum; /* in errore; NIHIL = non
                                     * probatur */
} Recusatio;

interior vacuum
_sectio_recusationes (
    CredoContextus* c)
{
    hic_manens constans Recusatio R[] = {
        /* STML fractum */
        { "<res titulus=\"a\"><capsa", ZEPHYRUM, NIHIL },
        { "", ZEPHYRUM, NIHIL },
        { "nihil hic", ZEPHYRUM, NIHIL },
        /* radix */
        { "<grex/>", I, "res" },
        { "<res>\n<capsa magnitudo=\"1 1 1\"/>\n</res>", I,
          "titulus" },
        { "<res titulus=\"\">\n<capsa magnitudo=\"1 1 1\"/>\n</res>",
          I, "titulus" },
        /* elementum et attributum ignotum */
        { "<res titulus=\"a\">\n  <pila radius=\"1\"/>\n</res>", II,
          "pila" },
        { "<res titulus=\"a\">\n\n\n"
          "  <capsa magnitudo=\"1 1 1\" colour=\"#fff\"/>\n</res>",
          IV, "colour" },
        { "<res titulus=\"a\" gyrus=\"3\">\n"
          "<capsa magnitudo=\"1 1 1\"/>\n</res>", I, "gyrus" },
        { "<res titulus=\"a\">\n<grex magnitudo=\"1 1 1\">\n"
          "<capsa magnitudo=\"1 1 1\"/>\n</grex>\n</res>", II,
          "magnitudo" },
        /* magnitudo */
        { "<res titulus=\"a\">\n<capsa/>\n</res>", II, "magnitudo" },
        { "<res titulus=\"a\">\n<capsa magnitudo=\"1 a 1\"/>\n</res>",
          II, "magnitudo" },
        { "<res titulus=\"a\">\n<capsa magnitudo=\"1 1\"/>\n</res>",
          II, "magnitudo" },
        { "<res titulus=\"a\">\n"
          "<capsa magnitudo=\"1 1 1 1\"/>\n</res>", II, "magnitudo" },
        { "<res titulus=\"a\">\n<capsa magnitudo=\"0 1 1\"/>\n</res>",
          II, "magnitudo" },
        { "<res titulus=\"a\">\n<capsa magnitudo=\"1 -1 1\"/>\n</res>",
          II, "magnitudo" },
        { "<res titulus=\"a\">\n"
          "<capsa magnitudo=\"1 nan 1\"/>\n</res>", II, "magnitudo" },
        { "<res titulus=\"a\">\n"
          "<capsa magnitudo=\"1 inf 1\"/>\n</res>", II, "magnitudo" },
        { "<res titulus=\"a\">\n<capsa magnitudo=\"\"/>\n</res>", II,
          "magnitudo" },
        /* cetera attributa */
        { "<res titulus=\"a\">\n"
          "<capsa magnitudo=\"1 1 1\" locus=\"1 2\"/>\n</res>", II,
          "locus" },
        { "<res titulus=\"a\">\n"
          "<capsa magnitudo=\"1 1 1\" gyrus=\"x\"/>\n</res>", II,
          "gyrus" },
        { "<res titulus=\"a\">\n"
          "<capsa magnitudo=\"1 1 1\" rotatio=\"1\"/>\n</res>", II,
          "rotatio" },
        { "<res titulus=\"a\">\n"
          "<capsa magnitudo=\"1 1 1\" signum=\"durum\"/>\n</res>", II,
          "signum" },
        { "<res titulus=\"a\">\n"
          "<capsa magnitudo=\"1 1 1\" color=\"#12\"/>\n</res>", II,
          "color" },
        { "<res titulus=\"a\">\n"
          "<capsa magnitudo=\"1 1 1\" color=\"804020\"/>\n</res>", II,
          "color" },
        { "<res titulus=\"a\">\n"
          "<capsa magnitudo=\"1 1 1\" color=\"#80402g\"/>\n</res>",
          II, "color" },
        { "<res titulus=\"a\">\n<capsa magnitudo=\"1 1 1\"\n"
          "invisibilis=\"fortasse\"/>\n</res>", II, "invisibilis" },
        /* textus inter elementa */
        { "<res titulus=\"a\">\nverba\n"
          "<capsa magnitudo=\"1 1 1\"/>\n</res>", ZEPHYRUM, "text" },
        /* solidum liberos non habet */
        { "<res titulus=\"a\">\n<capsa magnitudo=\"1 1 1\">\n"
          "<capsa magnitudo=\"1 1 1\"/>\n</capsa>\n</res>", II,
          "capsa" },
        /* res sine solidis */
        { "<res titulus=\"a\"></res>", I, "solid" },
        { "<res titulus=\"a\">\n<grex>\n<grex/>\n</grex>\n</res>", I,
          "solid" }
    };
    i32 k;

    per (k = ZEPHYRUM; k < magnitudo(R) / magnitudo(R[ZEPHYRUM]);
         k++)
    {
        PlasticaFructus f = _lege(c, R[k].textus);

        si (   f.successus || f.statua || f.partes
            || f.error.mensura == ZEPHYRUM)
        {
            printf("recusatio %u non recusata: %s\n", (insignatus)k,
                R[k].textus);
        }
        CREDO_FALSUM (f.successus);
        CREDO_NIHIL (f.statua);
        CREDO_NIHIL (f.partes);
        CREDO_CHORDA_NON_VACUA (f.error);
        CREDO_FALSUM (chorda_continet(f.error,
            chorda_ex_literis("\n", c->piscina)));
        si (R[k].linea)
        {
            si (f.linea != R[k].linea)
            {
                printf("recusatio %u linea %u: %s\n", (insignatus)k,
                    (insignatus)f.linea, R[k].textus);
            }
            CREDO_AEQUALIS_I32 (f.linea, R[k].linea);
        }
        si (R[k].fragmentum)
        {
            CREDO_CHORDA_CONTINET (f.error,
                chorda_ex_literis(R[k].fragmentum, c->piscina));
        }
    }
}

interior vacuum
_sectio_tolerata (
    CredoContextus* c)
{
    /* commenta et spatia inter elementa tolerantur; numeri cum
     * spatiis pluribus */
    PlasticaFructus f = _lege(c,
        "<!-- puteus -->\n"
        "<res titulus=\"a\">\n"
        "  <!-- murus -->\n"
        "  <capsa magnitudo=\" 1   2.5  1 \" locus=\"-1 0 .5\"/>\n"
        "</res>\n");
              Fines fines;

    CREDO_VERUM (f.successus);
    si (!f.statua)
    {
        redde;
    }
    fines = _fines(f.statua);
    FINES_SUNT (fines, -1.5, -0.5, 0.0, 2.5, 0.0, 1.0);
}

interior vacuum
_sectio_determinata (
    CredoContextus* c)
{
    constans character* T =
        "<res titulus=\"a\">\n"
        "  <grex locus=\"1 2 3\" gyrus=\"33\">\n"
        "    <capsa magnitudo=\"1 2 3\" color=\"#123456\"/>\n"
        "    <capsa magnitudo=\"3 2 1\" rotatio=\"10 20 30\"/>\n"
        "  </grex>\n"
        "</res>\n";
       PlasticaFructus a = _lege(c, T);
       PlasticaFructus b = _lege(c, T);

    CREDO_VERUM (a.successus && b.successus);
    si (!a.statua || !b.statua)
    {
        redde;
    }
    CREDO_AEQUALIS_I32 (a.statua->nodorum, b.statua->nodorum);
    CREDO_AEQUALIS_I32 (a.statua->verticum, b.statua->verticum);
    CREDO_AEQUALIS_I32 (a.statua->indicum, b.statua->indicum);
    CREDO_AEQUALIS_S32 (memcmp(a.statua->positiones,
        b.statua->positiones,
        a.statua->verticum * III * magnitudo(f32)), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (memcmp(a.statua->normales, b.statua->normales,
        a.statua->verticum * III * magnitudo(f32)), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (memcmp(a.statua->indices, b.statua->indices,
        a.statua->indicum * magnitudo(i32)), ZEPHYRUM);
    CREDO_AEQUALIS_S32 (memcmp(a.statua->nodi, b.statua->nodi,
        a.statua->nodorum * magnitudo(StatuaNodus)), ZEPHYRUM);
    /* sphaera quietis a statua_finire computata */
    CREDO_VERUM (a.statua->radius > 0.0f);
}


/* ==================================================
 * T2: cetera solida et repetitores
 * ================================================== */

#define TRIANGULA_MAXIMA (IV * M)

nomen structura {
    StereometriaVector p[III];
    StereometriaVector normalis;   /* condita, in mundo */
                   i32 nodus;
} Triangulum;

/* triangula omnia in spatio mundi; redde numerum (0 si nimia) */
interior i32
_triangula (
    constans Statua* s,
         Triangulum* t)
{
    StereometriaMatrix m[NODI_MAXIMI];
                   i32 numerus = ZEPHYRUM;
                   i32 n;
                   i32 j;
                   i32 k;
                   i32 a;

    si (s->nodorum > NODI_MAXIMI || s->indicum / III > TRIANGULA_MAXIMA)
    {
        redde ZEPHYRUM;
    }
    statua_positura(s, -I, 0.0f, m);
    per (n = ZEPHYRUM; n < s->nodorum; n++)
    {
        constans StatuaRete* rete;

        si (s->nodi[n].rete < ZEPHYRUM)
        {
            perge;
        }
        rete = &s->retia[s->nodi[n].rete];
        per (j = ZEPHYRUM; j < rete->partium; j++)
        {
            constans StatuaPars* pars =
                &s->partes[rete->pars_prima + j];

            per (k = ZEPHYRUM; k < pars->indicum / III; k++)
            {
                Triangulum* novum = &t[numerus++];

                per (a = ZEPHYRUM; a < III; a++)
                {
                    i32 v = s->indices[pars->index_primus + k * III
                        + a];

                    novum->p[a] = stereometria_punctum_transformare(
                        m[n], stereometria_vector(
                            s->positiones[v * III],
                            s->positiones[v * III + I],
                            s->positiones[v * III + II]), NIHIL);
                    novum->normalis = stereometria_normalizare(
                        stereometria_directionem_transformare(m[n],
                            stereometria_vector(s->normales[v * III],
                                s->normales[v * III + I],
                                s->normales[v * III + II])));
                }
                novum->nodus = n;
            }
        }
    }
    redde numerus;
}

interior StereometriaVector
_medium (
    constans Triangulum* t)
{
    redde stereometria_scalare(stereometria_addere(t->p[ZEPHYRUM],
        stereometria_addere(t->p[I], t->p[II])), 1.0f / 3.0f);
}

/* normalis geometrica (ex ordine verticum), longitudo I */
interior StereometriaVector
_facies (
    constans Triangulum* t)
{
    redde stereometria_normalizare(stereometria_crux(
        stereometria_subtrahere(t->p[I], t->p[ZEPHYRUM]),
        stereometria_subtrahere(t->p[II], t->p[ZEPHYRUM])));
}

/* triangula quorum normalis condita cum ordine verticum non
 * congruit */
interior i32
_discordia (
    constans Triangulum* t,
                    i32  numerus)
{
    i32 mala = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (stereometria_productum(_facies(&t[k]), t[k].normalis)
            < 0.999f)
        {
            mala++;
        }
    }
    redde mala;
}

/* triangula quae a puncto interiori NON avertuntur (solidum
 * convexum: omnia averti debent) */
interior i32
_male_versa (
    constans Triangulum* t,
                    i32  numerus,
     StereometriaVector  intus)
{
    i32 mala = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (stereometria_productum(_facies(&t[k]),
                stereometria_subtrahere(_medium(&t[k]), intus))
            <= 0.0f)
        {
            mala++;
        }
    }
    redde mala;
}

/* positiones distinctae (tolerantia 1e-5) */
interior i32
_distinctae (
    constans Triangulum* t,
                    i32  numerus)
{
    i32 distinctae = ZEPHYRUM;
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < numerus * III; k++)
    {
        StereometriaVector a     = t[k / III].p[k % III];
                       b32 nova  = VERUM;

        per (j = ZEPHYRUM; j < k && nova; j++)
        {
            si (stereometria_longitudo(stereometria_subtrahere(a,
                    t[j / III].p[j % III])) < 1.0e-5f)
            {
                nova = FALSUM;
            }
        }
        si (nova)
        {
            distinctae++;
        }
    }
    redde distinctae;
}

interior Triangulum*
_tabula (
    CredoContextus* c)
{
    redde (Triangulum*)piscina_allocare(c->piscina,
        TRIANGULA_MAXIMA * magnitudo(Triangulum));
}

#define LEGE_BENE(f, textus) \
    fac { \
        (f) = _lege(c, textus); \
        si (!(f).successus) \
        { \
            printf("recusata (linea %u): %.*s\n", \
                (insignatus)(f).linea, (integer)(f).error.mensura, \
                (constans character*)(f).error.datum); \
        } \
        CREDO_VERUM ((f).successus); \
        si (!(f).statua) \
        { \
            redde; \
        } \
    } dum (FALSUM)

interior vacuum
_sectio_cylindrus (
    CredoContextus* c)
{
    PlasticaFructus  f;
         Triangulum* t = _tabula(c);
              Fines  fines;
                i32  n;
                i32  k;

    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <cylindrus radius=\"1\" altitudo=\"2\" latera=\"6\"/>\n"
        "</res>\n");
    n      = _triangula(f.statua, t);
    fines  = _fines(f.statua);
    CREDO_AEQUALIS_I32 (n, XXIV);
    FINES_SUNT (fines, -1.0, 1.0, 0.0, 2.0, -0.8660254, 0.8660254);
    CREDO_AEQUALIS_I32 (_discordia(t, n), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_male_versa(t, n,
        stereometria_vector(0.0f, 1.0f, 0.0f)), ZEPHYRUM);

    /* conus: latera et basis; apex solus in summo */
    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <cylindrus radius=\"1\" radius_summus=\"0\" altitudo=\"2\"\n"
        "             latera=\"6\"/>\n"
        "</res>\n");
    n = _triangula(f.statua, t);
    CREDO_AEQUALIS_I32 (n, XII);
    CREDO_AEQUALIS_I32 (_discordia(t, n), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_male_versa(t, n,
        stereometria_vector(0.0f, 0.5f, 0.0f)), ZEPHYRUM);
    per (k = ZEPHYRUM; k < n * III; k++)
    {
        StereometriaVector v = t[k / III].p[k % III];

        si (v.y > 1.0f)
        {
            CREDO_F64_PROXIMUS ((f64)v.x, 0.0, 1.0e-5);
            CREDO_F64_PROXIMUS ((f64)v.z, 0.0, 1.0e-5);
        }
    }

    /* latera ordinaria VIII; truncus cum radio summo minore */
    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <cylindrus radius=\"2\" radius_summus=\"1\"\n"
        "             altitudo=\"1\"/>\n"
        "</res>\n");
    n = _triangula(f.statua, t);
    CREDO_AEQUALIS_I32 (n, XXXII);
    CREDO_AEQUALIS_I32 (_male_versa(t, n,
        stereometria_vector(0.0f, 0.5f, 0.0f)), ZEPHYRUM);
    per (k = ZEPHYRUM; k < n * III; k++)
    {
        StereometriaVector v = t[k / III].p[k % III];
                       f32 r = (f32)sqrt((f64)(v.x * v.x + v.z * v.z));

        si (v.y > 0.5f && r > 0.1f)
        {
            CREDO_F64_PROXIMUS ((f64)r, 1.0, 1.0e-5);
        }
    }
}

interior vacuum
_sectio_sphaera (
    CredoContextus* c)
{
     PlasticaFructus  f;
     PlasticaFructus  g;
          Triangulum* t        = _tabula(c);
          Triangulum* u        = _tabula(c);
  StereometriaVector  centrum  = stereometria_vector(0.0f, 1.0f, 0.0f);
               Fines  fines;
                 i32  n;
                 i32  m;
                 i32  k;
                 b32  differt = FALSUM;

    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <sphaera radius=\"1\"/>\n"
        "</res>\n");
    n = _triangula(f.statua, t);
    /* VI segmenta, III zonae: 6 + 12 + 6 */
    CREDO_AEQUALIS_I32 (n, XXIV);
    CREDO_AEQUALIS_I32 (_discordia(t, n), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_male_versa(t, n, centrum), ZEPHYRUM);
    /* II poli + II anuli x VI */
    CREDO_AEQUALIS_I32 (_distinctae(t, n), XIV);
    per (k = ZEPHYRUM; k < n * III; k++)
    {
        CREDO_F64_PROXIMUS ((f64)stereometria_longitudo(
            stereometria_subtrahere(t[k / III].p[k % III], centrum)),
            1.0, 1.0e-5);
    }
    fines = _fines(f.statua);
    CREDO_F64_PROXIMUS ((f64)fines.minimum[I], 0.0, 1.0e-5);
    CREDO_F64_PROXIMUS ((f64)fines.maximum[I], 2.0, 1.0e-5);

    /* ellipsoides per magnitudinem */
    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <sphaera magnitudo=\"4 2 2\" segmenta=\"8\"/>\n"
        "</res>\n");
    fines = _fines(f.statua);
    FINES_SUNT (fines, -2.0, 2.0, 0.0, 2.0, -1.0, 1.0);

    /* asperitas: semen idem res eadem, aliud alia; rimae nullae
     * (positiones distinctae eaedem numero); radii intra fines */
    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <sphaera radius=\"1\" asperitas=\"0.4\" semen=\"3\"/>\n"
        "</res>\n");
    LEGE_BENE (g,
        "<res titulus=\"a\">\n"
        "  <sphaera radius=\"1\" asperitas=\"0.4\" semen=\"3\"/>\n"
        "</res>\n");
    n = _triangula(f.statua, t);
    m = _triangula(g.statua, u);
    CREDO_AEQUALIS_I32 (n, XXIV);
    CREDO_AEQUALIS_I32 (m, n);
    CREDO_AEQUALIS_S32 (memcmp(f.statua->positiones,
        g.statua->positiones,
        f.statua->verticum * III * magnitudo(f32)), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_distinctae(t, n), XIV);
    CREDO_AEQUALIS_I32 (_discordia(t, n), ZEPHYRUM);
    per (k = ZEPHYRUM; k < n * III; k++)
    {
        f32 r = stereometria_longitudo(stereometria_subtrahere(
            t[k / III].p[k % III], centrum));

        CREDO_VERUM (r > 0.79f && r < 1.21f);
        si (r < 0.999f || r > 1.001f)
        {
            differt = VERUM;
        }
    }
    CREDO_VERUM (differt);
    LEGE_BENE (g,
        "<res titulus=\"a\">\n"
        "  <sphaera radius=\"1\" asperitas=\"0.4\" semen=\"4\"/>\n"
        "</res>\n");
    CREDO_INAEQUALITAS_S32 (memcmp(f.statua->positiones,
        g.statua->positiones,
        f.statua->verticum * III * magnitudo(f32)), ZEPHYRUM);
}

interior vacuum
_sectio_cuneus (
    CredoContextus* c)
{
    PlasticaFructus  f;
         Triangulum* t = _tabula(c);
              Fines  fines;
                i32  n;
                i32  k;

    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <cuneus magnitudo=\"2 1 4\"/>\n"
        "</res>\n");
    n      = _triangula(f.statua, t);
    fines  = _fines(f.statua);
    CREDO_AEQUALIS_I32 (n, VIII);
    FINES_SUNT (fines, -1.0, 1.0, 0.0, 1.0, -2.0, 2.0);
    CREDO_AEQUALIS_I32 (_discordia(t, n), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_male_versa(t, n,
        stereometria_vector(-0.4f, 0.3f, 0.0f)), ZEPHYRUM);
    /* altitudo plena ad marginem -x solum */
    per (k = ZEPHYRUM; k < n * III; k++)
    {
        StereometriaVector v = t[k / III].p[k % III];

        si (v.y > 0.5f)
        {
            CREDO_F64_PROXIMUS ((f64)v.x, -1.0, 1.0e-5);
        }
    }
}

/* summa arearum triangulorum quorum normalis sursum spectat et
 * omnes vertices ad altitudinem y sunt */
interior f32
_area_operculi (
    constans Triangulum* t,
                    i32  numerus,
                    f32  y)
{
    f32 area = 0.0f;
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (   t[k].p[ZEPHYRUM].y > y - 1.0e-5f
            && t[k].p[I].y > y - 1.0e-5f && t[k].p[II].y > y - 1.0e-5f
            && _facies(&t[k]).y > 0.999f)
        {
            area += 0.5f * stereometria_longitudo(stereometria_crux(
                stereometria_subtrahere(t[k].p[I], t[k].p[ZEPHYRUM]),
                stereometria_subtrahere(t[k].p[II],
                    t[k].p[ZEPHYRUM])));
        }
    }
    redde area;
}

interior vacuum
_sectio_extrusio (
    CredoContextus* c)
{
    PlasticaFructus  f;
         Triangulum* t = _tabula(c);
              Fines  fines;
                i32  n;
                i32  k;
                i32  deorsum = ZEPHYRUM;

    /* quadratum = capsa 2 x 1 x 4, ordine utroque */
    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <extrusio puncta=\"-1,-2 1,-2 1,2 -1,2\" altitudo=\"1\"/>\n"
        "</res>\n");
    n      = _triangula(f.statua, t);
    fines  = _fines(f.statua);
    CREDO_AEQUALIS_I32 (n, XII);
    FINES_SUNT (fines, -1.0, 1.0, 0.0, 1.0, -2.0, 2.0);
    CREDO_AEQUALIS_I32 (_discordia(t, n), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_male_versa(t, n,
        stereometria_vector(0.0f, 0.5f, 0.0f)), ZEPHYRUM);
    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <extrusio puncta=\"-1,2 1,2 1,-2 -1,-2\" altitudo=\"1\"/>\n"
        "</res>\n");
    n = _triangula(f.statua, t);
    CREDO_AEQUALIS_I32 (n, XII);
    CREDO_AEQUALIS_I32 (_male_versa(t, n,
        stereometria_vector(0.0f, 0.5f, 0.0f)), ZEPHYRUM);

    /* forma L concava (area III): opercula IV + IV, parietes XII;
     * operculum summum totam aream tegit et sursum spectat */
    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <extrusio puncta=\"0,0 2,0 2,1 1,1 1,2 0,2\"\n"
        "            altitudo=\"3\"/>\n"
        "</res>\n");
    n = _triangula(f.statua, t);
    CREDO_AEQUALIS_I32 (n, XX);
    CREDO_AEQUALIS_I32 (_discordia(t, n), ZEPHYRUM);
    CREDO_F64_PROXIMUS ((f64)_area_operculi(t, n, 3.0f), 3.0, 1.0e-5);
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (_facies(&t[k]).y < -0.999f)
        {
            deorsum++;
            CREDO_F64_PROXIMUS ((f64)t[k].p[ZEPHYRUM].y, 0.0, 1.0e-6);
        }
    }
    CREDO_AEQUALIS_I32 (deorsum, IV);
    /* paries anguli concavi (x = 1, z 1 -> 2) ad +x spectat */
    per (k = ZEPHYRUM; k < n; k++)
    {
        StereometriaVector medium = _medium(&t[k]);

        si (   medium.x > 0.999f && medium.x < 1.001f
            && medium.z > 1.0f)
        {
            CREDO_VERUM (_facies(&t[k]).x > 0.999f);
        }
    }

    /* punctum collineare in latere toleratur */
    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <extrusio puncta=\"0,0 1,0 2,0 2,2 0,2\" altitudo=\"1\"/>\n"
        "</res>\n");
    n = _triangula(f.statua, t);
    CREDO_F64_PROXIMUS ((f64)_area_operculi(t, n, 1.0f), 4.0, 1.0e-5);
}

interior vacuum
_sectio_tornus (
    CredoContextus* c)
{
     PlasticaFructus  f;
          Triangulum* t = _tabula(c);
               Fines  fines;
                 i32  n;
                 i32  k;
                 i32  interiora = ZEPHYRUM;

    /* tubus apertus = latera cylindri */
    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <tornus puncta=\"1,0 1,2\" latera=\"6\"/>\n"
        "</res>\n");
    n      = _triangula(f.statua, t);
    fines  = _fines(f.statua);
    CREDO_AEQUALIS_I32 (n, XII);
    FINES_SUNT (fines, -1.0, 1.0, 0.0, 2.0, -0.8660254, 0.8660254);
    CREDO_AEQUALIS_I32 (_discordia(t, n), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_male_versa(t, n,
        stereometria_vector(0.0f, 1.0f, 0.0f)), ZEPHYRUM);

    /* profilum ab axe ad axem: clausum, ut cylindrus */
    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <tornus puncta=\"0,0 1,0 1,2 0,2\" latera=\"6\"/>\n"
        "</res>\n");
    n = _triangula(f.statua, t);
    CREDO_AEQUALIS_I32 (n, XXIV);
    CREDO_AEQUALIS_I32 (_discordia(t, n), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_male_versa(t, n,
        stereometria_vector(0.0f, 1.0f, 0.0f)), ZEPHYRUM);

    /* situla: sursum extra, deorsum intra - paries interior ad
     * axem spectat, fundus interior sursum */
    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <tornus puncta=\"0,0 1,0 1,2 0.8,2 0.8,0.2 0,0.2\"\n"
        "          latera=\"8\"/>\n"
        "</res>\n");
    n = _triangula(f.statua, t);
    /* VIII + XVI + XVI + XVI + VIII */
    CREDO_AEQUALIS_I32 (n, LXIV);
    CREDO_AEQUALIS_I32 (_discordia(t, n), ZEPHYRUM);
    per (k = ZEPHYRUM; k < n; k++)
    {
        StereometriaVector medium = _medium(&t[k]);
        StereometriaVector facies = _facies(&t[k]);
                       f32 r       = (f32)sqrt((f64)(medium.x
                           * medium.x + medium.z * medium.z));

        si (medium.y > 0.3f && medium.y < 1.9f && r < 0.85f)
        {
            interiora++;
            CREDO_VERUM (facies.x * medium.x + facies.z * medium.z
                < 0.0f);
        }
        si (medium.y > 0.19f && medium.y < 0.21f)
        {
            CREDO_VERUM (facies.y > 0.999f);
        }
        si (medium.y < 0.01f)
        {
            CREDO_VERUM (facies.y < -0.999f);
        }
    }
    CREDO_AEQUALIS_I32 (interiora, XVI);
}

interior vacuum
_sectio_speculum (
    CredoContextus* c)
{
     PlasticaFructus  f;
          Triangulum* t = _tabula(c);
               Fines  fines;
                 f64  summa_x        = 0.0;
                 f64  summa_obliqua  = 0.0;
                 f64  summa_erecta   = 0.0;
                 i32  n;
                 i32  k;

    /* capsa et imago eius trans x = 0 */
    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <speculum axis=\"x\">\n"
        "    <capsa magnitudo=\"1 1 1\" locus=\"2 0 1\"/>\n"
        "  </speculum>\n"
        "</res>\n");
    n      = _triangula(f.statua, t);
    fines  = _fines(f.statua);
    CREDO_AEQUALIS_I32 (n, XXIV);
    FINES_SUNT (fines, -2.5, 2.5, 0.0, 1.0, 0.5, 1.5);
    CREDO_AEQUALIS_I32 (_discordia(t, n), ZEPHYRUM);
    per (k = ZEPHYRUM; k < n; k++)
    {
        StereometriaVector medium = _medium(&t[k]);

        /* quaeque facies a centro capsae SUAE avertitur */
        CREDO_VERUM (stereometria_productum(_facies(&t[k]),
            stereometria_subtrahere(medium, stereometria_vector(
                medium.x > 0.0f ? 2.0f : -2.0f, 0.5f, 1.0f))) > 0.0f);
    }

    /* cum gyro et rotatione: imago vera - momenta imparia in x
     * evanescunt, normales cum ordine congruunt */
    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <speculum axis=\"x\" locus=\"0 1 0\">\n"
        "    <grex locus=\"3 0 1\" gyrus=\"30\">\n"
        "      <cuneus magnitudo=\"4 1 2\" rotatio=\"10 0 20\"/>\n"
        "    </grex>\n"
        "  </speculum>\n"
        "</res>\n");
    n      = _triangula(f.statua, t);
    fines  = _fines(f.statua);
    CREDO_AEQUALIS_I32 (n, XVI);
    CREDO_AEQUALIS_I32 (_discordia(t, n), ZEPHYRUM);
    CREDO_F64_PROXIMUS ((f64)fines.minimum[ZEPHYRUM],
        -(f64)fines.maximum[ZEPHYRUM], 1.0e-4);
    per (k = ZEPHYRUM; k < n * III; k++)
    {
        StereometriaVector v = t[k / III].p[k % III];

        summa_x        += (f64)v.x;
        summa_obliqua  += (f64)(v.x * v.z);
        summa_erecta   += (f64)(v.x * v.y);
    }
    CREDO_F64_PROXIMUS (summa_x, 0.0, 1.0e-3);
    CREDO_F64_PROXIMUS (summa_obliqua, 0.0, 1.0e-3);
    CREDO_F64_PROXIMUS (summa_erecta, 0.0, 1.0e-3);
    /* imago non est copia translata: cuneus obliquus */
    CREDO_VERUM (fines.maximum[II] - fines.minimum[II] > 1.0f);

    /* axis z; specula nidificata: IV exemplaria */
    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <speculum axis=\"z\">\n"
        "    <speculum axis=\"x\">\n"
        "      <cuneus magnitudo=\"1 1 1\" locus=\"2 0 3\"\n"
        "              gyrus=\"20\"/>\n"
        "    </speculum>\n"
        "  </speculum>\n"
        "</res>\n");
    n      = _triangula(f.statua, t);
    fines  = _fines(f.statua);
    CREDO_AEQUALIS_I32 (n, XXXII);
    CREDO_AEQUALIS_I32 (_discordia(t, n), ZEPHYRUM);
    CREDO_F64_PROXIMUS ((f64)fines.minimum[ZEPHYRUM],
        -(f64)fines.maximum[ZEPHYRUM], 1.0e-4);
    CREDO_F64_PROXIMUS ((f64)fines.minimum[II],
        -(f64)fines.maximum[II], 1.0e-4);
    CREDO_VERUM (fines.maximum[II] > 3.0f);
    per (k = ZEPHYRUM; k < n; k++)
    {
        StereometriaVector medium = _medium(&t[k]);

        CREDO_VERUM (stereometria_productum(_facies(&t[k]),
            stereometria_subtrahere(medium, stereometria_vector(
                medium.x > 0.0f ? 2.0f : -2.0f, 0.3f,
                medium.z > 0.0f ? 3.0f : -3.0f))) > 0.0f);
    }
}

interior vacuum
_sectio_series (
    CredoContextus* c)
{
    PlasticaFructus  f;
         Triangulum* t = _tabula(c);
              Fines  fines;
                i32  n;

    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <series numerus=\"4\" gressus=\"2 0.5 0\">\n"
        "    <capsa magnitudo=\"1 1 1\"/>\n"
        "  </series>\n"
        "</res>\n");
    n      = _triangula(f.statua, t);
    fines  = _fines(f.statua);
    CREDO_AEQUALIS_I32 (n, XLVIII);
    FINES_SUNT (fines, -0.5, 6.5, 0.0, 2.5, -0.5, 0.5);
    /* radix, series, IV exemplaria, IV capsae */
    CREDO_AEQUALIS_I32 (f.statua->nodorum, X);

    /* gyrus_gressus: exemplaria circa axem */
    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <series numerus=\"4\" gyrus_gressus=\"90\">\n"
        "    <capsa magnitudo=\"1 1 1\" locus=\"2 0 0\"/>\n"
        "  </series>\n"
        "</res>\n");
    fines = _fines(f.statua);
    FINES_SUNT (fines, -2.5, 2.5, 0.0, 1.0, -2.5, 2.5);
    n = _triangula(f.statua, t);
    CREDO_AEQUALIS_I32 (_distinctae(t, n), XXXII);

    /* numerus I: exemplar unum, immotum */
    LEGE_BENE (f,
        "<res titulus=\"a\">\n"
        "  <series numerus=\"1\" gressus=\"9 9 9\">\n"
        "    <capsa magnitudo=\"1 1 1\"/>\n"
        "  </series>\n"
        "</res>\n");
    fines = _fines(f.statua);
    FINES_SUNT (fines, -0.5, 0.5, 0.0, 1.0, -0.5, 0.5);
}

#define SPARSIO(semen, cetera) \
    "<res titulus=\"a\">\n" \
    "  <sparsio numerus=\"10\" semen=\"" semen "\" area=\"8 4\"" \
    cetera ">\n" \
    "    <capsa magnitudo=\"0.2 0.2 0.2\"/>\n" \
    "  </sparsio>\n" \
    "</res>\n"

interior vacuum
_sectio_sparsio (
    CredoContextus* c)
{
    PlasticaFructus  f;
    PlasticaFructus  g;
         Triangulum* t = _tabula(c);
                i32  n;
                i32  k;
                b32  scalae_differunt      = FALSUM;
                b32  rotationes_differunt  = FALSUM;

    LEGE_BENE (f, SPARSIO ("5", ""));
    LEGE_BENE (g, SPARSIO ("5", ""));
    /* radix, sparsio, X exemplaria, X capsae */
    CREDO_AEQUALIS_I32 (f.statua->nodorum, XXII);
    CREDO_AEQUALIS_S32 (memcmp(f.statua->nodi, g.statua->nodi,
        f.statua->nodorum * magnitudo(StatuaNodus)), ZEPHYRUM);
    n = _triangula(f.statua, t);
    CREDO_AEQUALIS_I32 (n, CXX);
    /* X loci distincti, omnes intra aream; sine variis: scala I,
     * rotatio nulla */
    CREDO_AEQUALIS_I32 (_distinctae(t, n), LXXX);
    per (k = II; k < f.statua->nodorum; k += II)
    {
        constans StatuaNodus* nodus = &f.statua->nodi[k];

        CREDO_AEQUALIS_S32 (nodus->parens, I);
        CREDO_VERUM (nodus->translatio.x >= -4.0f
            && nodus->translatio.x <= 4.0f);
        CREDO_VERUM (nodus->translatio.z >= -2.0f
            && nodus->translatio.z <= 2.0f);
        CREDO_F64_PROXIMUS ((f64)nodus->translatio.y, 0.0, 1.0e-6);
        CREDO_F64_PROXIMUS ((f64)nodus->scala.x, 1.0, 1.0e-6);
        CREDO_F64_PROXIMUS ((f64)nodus->rotatio.w, 1.0, 1.0e-6);
    }
    /* area tota adhibetur, non angulus unus */
    CREDO_VERUM (f.statua->radius > 2.0f);

    LEGE_BENE (g, SPARSIO ("6", ""));
    CREDO_INAEQUALITAS_S32 (memcmp(f.statua->nodi, g.statua->nodi,
        f.statua->nodorum * magnitudo(StatuaNodus)), ZEPHYRUM);

    /* varia: loci iidem ac sine variis (sortes fixae per
     * exemplar), scalae intra fines et diversae, rotationes
     * diversae */
    LEGE_BENE (g, SPARSIO ("5",
        " gyrus_varius=\"verum\" magnitudo_varia=\"0.5\""));
    per (k = II; k < g.statua->nodorum; k += II)
    {
        constans StatuaNodus* nodus = &g.statua->nodi[k];

        CREDO_F64_PROXIMUS ((f64)nodus->translatio.x,
            (f64)f.statua->nodi[k].translatio.x, 1.0e-6);
        CREDO_F64_PROXIMUS ((f64)nodus->translatio.z,
            (f64)f.statua->nodi[k].translatio.z, 1.0e-6);
        CREDO_VERUM (nodus->scala.x >= 0.5f && nodus->scala.x <= 1.5f);
        CREDO_F64_PROXIMUS ((f64)nodus->scala.x, (f64)nodus->scala.y,
            1.0e-6);
        CREDO_F64_PROXIMUS ((f64)nodus->scala.x, (f64)nodus->scala.z,
            1.0e-6);
        si (nodus->scala.x != g.statua->nodi[II].scala.x)
        {
            scalae_differunt = VERUM;
        }
        si (nodus->rotatio.w != g.statua->nodi[II].rotatio.w)
        {
            rotationes_differunt = VERUM;
        }
    }
    CREDO_VERUM (scalae_differunt);
    CREDO_VERUM (rotationes_differunt);
}

interior vacuum
_sectio_recusationes_solidorum (
    CredoContextus* c)
{
#define RES(interiora) \
    "<res titulus=\"a\">\n" interiora "\n</res>"
    hic_manens constans Recusatio R[] = {
        /* cylindrus */
        { RES ("<cylindrus altitudo=\"1\"/>"), II, "radius" },
        { RES ("<cylindrus radius=\"1\"/>"), II, "altitudo" },
        { RES ("<cylindrus radius=\"0\" altitudo=\"1\"/>"), II,
          "radius" },
        { RES ("<cylindrus radius=\"1\" altitudo=\"-1\"/>"), II,
          "altitudo" },
        { RES ("<cylindrus radius=\"1\" altitudo=\"1\" "
               "radius_summus=\"-1\"/>"), II, "radius_summus" },
        { RES ("<cylindrus radius=\"1\" altitudo=\"1\" "
               "latera=\"2\"/>"), II, "latera" },
        { RES ("<cylindrus radius=\"1\" altitudo=\"1\" "
               "latera=\"65\"/>"), II, "latera" },
        { RES ("<cylindrus radius=\"1\" altitudo=\"1\" "
               "latera=\"6.5\"/>"), II, "latera" },
        { RES ("<cylindrus radius=\"1\" altitudo=\"1\" "
               "magnitudo=\"1 1 1\"/>"), II, "magnitudo" },
        /* sphaera */
        { RES ("<sphaera/>"), II, "radius" },
        { RES ("<sphaera radius=\"1\" magnitudo=\"1 1 1\"/>"), II,
          "magnitudo" },
        { RES ("<sphaera radius=\"-1\"/>"), II, "radius" },
        { RES ("<sphaera radius=\"1\" segmenta=\"2\"/>"), II,
          "segmenta" },
        { RES ("<sphaera radius=\"1\" segmenta=\"33\"/>"), II,
          "segmenta" },
        { RES ("<sphaera radius=\"1\" asperitas=\"2\"/>"), II,
          "asperitas" },
        { RES ("<sphaera radius=\"1\" semen=\"-1\"/>"), II, "semen" },
        { RES ("<sphaera radius=\"1\" semen=\"1.5\"/>"), II,
          "semen" },
        /* cuneus */
        { RES ("<cuneus/>"), II, "magnitudo" },
        { RES ("<cuneus magnitudo=\"1 0 1\"/>"), II, "magnitudo" },
        /* extrusio */
        { RES ("<extrusio altitudo=\"1\"/>"), II, "puncta" },
        { RES ("<extrusio puncta=\"0,0 1,0\" altitudo=\"1\"/>"), II,
          "puncta" },
        { RES ("<extrusio puncta=\"0,0 1,0 1\" altitudo=\"1\"/>"),
          II, "puncta" },
        { RES ("<extrusio puncta=\"0,0 1,0 a,1\" altitudo=\"1\"/>"),
          II, "puncta" },
        { RES ("<extrusio puncta=\"0,0 1,0 1,1,1\" "
               "altitudo=\"1\"/>"), II, "puncta" },
        { RES ("<extrusio puncta=\"0,0 1,1 1,0 0,1\" "
               "altitudo=\"1\"/>"), II, "crosses" },
        { RES ("<extrusio puncta=\"0,0 1,0 1,1 0,0\" "
               "altitudo=\"1\"/>"), II, "repeats" },
        { RES ("<extrusio puncta=\"0,0 1,0 2,0\" altitudo=\"1\"/>"),
          II, "no area" },
        { RES ("<extrusio puncta=\"0,0 1,0 1,1\"/>"), II,
          "altitudo" },
        { RES ("<extrusio puncta=\"0,0 1,0 1,1\" altitudo=\"0\"/>"),
          II, "altitudo" },
        /* tornus */
        { RES ("<tornus/>"), II, "puncta" },
        { RES ("<tornus puncta=\"1,0\"/>"), II, "puncta" },
        { RES ("<tornus puncta=\"1,0 -1,2\"/>"), II, "negative" },
        { RES ("<tornus puncta=\"1,0 1,0 1,2\"/>"), II, "repeats" },
        { RES ("<tornus puncta=\"1,0 1,2\" latera=\"2\"/>"), II,
          "latera" },
        /* profilum in axe iacens: superficies nulla */
        { RES ("<tornus puncta=\"0,0 0,2\"/>"), II, "surface" },
        /* speculum */
        { RES ("<speculum>\n<capsa magnitudo=\"1 1 1\"/>\n"
               "</speculum>"), II, "axis" },
        { RES ("<speculum axis=\"y\">\n<capsa magnitudo=\"1 1 1\"/>\n"
               "</speculum>"), II, "axis" },
        /* series */
        { RES ("<series>\n<capsa magnitudo=\"1 1 1\"/>\n</series>"),
          II, "numerus" },
        { RES ("<series numerus=\"0\">\n<capsa magnitudo=\"1 1 1\"/>\n"
               "</series>"), II, "numerus" },
        { RES ("<series numerus=\"257\">\n"
               "<capsa magnitudo=\"1 1 1\"/>\n</series>"), II,
          "numerus" },
        { RES ("<series numerus=\"2\" gressus=\"1 1\">\n"
               "<capsa magnitudo=\"1 1 1\"/>\n</series>"), II,
          "gressus" },
        { RES ("<series numerus=\"2\" gyrus_gressus=\"x\">\n"
               "<capsa magnitudo=\"1 1 1\"/>\n</series>"), II,
          "gyrus_gressus" },
        /* error in libero seriei lineam liberi fert */
        { RES ("<series numerus=\"2\">\n"
               "<capsa magnitudo=\"1 1\"/>\n</series>"), III,
          "magnitudo" },
        /* sparsio */
        { RES ("<sparsio numerus=\"0\" area=\"1 1\">\n"
               "<capsa magnitudo=\"1 1 1\"/>\n</sparsio>"), II,
          "numerus" },
        { RES ("<sparsio numerus=\"2\">\n"
               "<capsa magnitudo=\"1 1 1\"/>\n</sparsio>"), II,
          "area" },
        { RES ("<sparsio numerus=\"2\" area=\"1 -1\">\n"
               "<capsa magnitudo=\"1 1 1\"/>\n</sparsio>"), II,
          "area" },
        { RES ("<sparsio numerus=\"2\" area=\"1 1\" "
               "magnitudo_varia=\"1\">\n"
               "<capsa magnitudo=\"1 1 1\"/>\n</sparsio>"), II,
          "magnitudo_varia" },
        { RES ("<sparsio numerus=\"2\" area=\"1 1\" "
               "gyrus_varius=\"falsum\">\n"
               "<capsa magnitudo=\"1 1 1\"/>\n</sparsio>"), II,
          "gyrus_varius" },
        /* repetitores nidificati: nimis multi nodi */
        { RES ("<series numerus=\"100\">\n<series numerus=\"100\">\n"
               "<capsa magnitudo=\"1 1 1\"/>\n</series>\n</series>"),
          ZEPHYRUM, "too large" },
        /* grex repetitoris sine solidis */
        { RES ("<series numerus=\"3\"></series>"), I, "solid" }
    };
#undef RES
    i32 k;

    per (k = ZEPHYRUM; k < magnitudo(R) / magnitudo(R[ZEPHYRUM]);
         k++)
    {
        PlasticaFructus f = _lege(c, R[k].textus);

        si (   f.successus || (R[k].linea && f.linea != R[k].linea)
            || (R[k].fragmentum && !chorda_continet(f.error,
                chorda_ex_literis(R[k].fragmentum, c->piscina))))
        {
            printf("recusatio solidorum %u (linea %u, '%.*s'): %s\n",
                (insignatus)k, (insignatus)f.linea,
                (integer)f.error.mensura,
                (constans character*)f.error.datum, R[k].textus);
        }
        CREDO_FALSUM (f.successus);
        CREDO_NIHIL (f.statua);
        CREDO_NIHIL (f.partes);
        CREDO_CHORDA_NON_VACUA (f.error);
        si (R[k].linea)
        {
            CREDO_AEQUALIS_I32 (f.linea, R[k].linea);
        }
        CREDO_CHORDA_CONTINET (f.error,
            chorda_ex_literis(R[k].fragmentum, c->piscina));
    }
}

/* plastica_legere_seminis: semen omni semini rei additum (sphaera,
 * textura, sparsio). Semen 0 = plastica_legere; semen III idem dat
 * ac descriptio cum seminibus III maioribus scripta */
interior b32
_statuae_eaedem (
    constans Statua* a,
    constans Statua* b)
{
    i32 k;

    si (   a->nodorum    != b->nodorum || a->verticum != b->verticum
        || a->indicum    != b->indicum
        || a->texturarum != b->texturarum
        || memcmp(a->nodi, b->nodi,
               a->nodorum * magnitudo(StatuaNodus)) != ZEPHYRUM
        || memcmp(a->positiones, b->positiones,
               a->verticum * III * magnitudo(f32)) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < a->texturarum; k++)
    {
        si (   a->texturae[k].latitudo != b->texturae[k].latitudo
            || a->texturae[k].altitudo != b->texturae[k].altitudo
            || memcmp(a->texturae[k].pixela, b->texturae[k].pixela,
                   a->texturae[k].latitudo * a->texturae[k].altitudo
                       * IV) != ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_sectio_semen_additum (
    CredoContextus* c)
{
    constans character* scripta =
        "<res titulus=\"a\">"
        "<sphaera radius=\"1\" asperitas=\"0.5\" semen=\"2\""
        " textura=\"saxum\" color=\"#888\"/>"
        "<sphaera radius=\"1\" asperitas=\"0.5\" locus=\"3 0 0\"/>"
        "<sparsio numerus=\"3\" semen=\"4\" area=\"4 4\""
        " gyrus_varius=\"verum\">"
        "<capsa magnitudo=\"0.2 0.2 0.2\"/>"
        "</sparsio>"
        "</res>";
    constans character* aucta =
        "<res titulus=\"a\">"
        "<sphaera radius=\"1\" asperitas=\"0.5\" semen=\"5\""
        " textura=\"saxum\" color=\"#888\"/>"
        "<sphaera radius=\"1\" asperitas=\"0.5\" locus=\"3 0 0\""
        " semen=\"3\"/>"
        "<sparsio numerus=\"3\" semen=\"7\" area=\"4 4\""
        " gyrus_varius=\"verum\">"
        "<capsa magnitudo=\"0.2 0.2 0.2\"/>"
        "</sparsio>"
        "</res>";
    PlasticaFructus nuda = _lege(c, scripta);
    PlasticaFructus nulla = plastica_legere_seminis(
        chorda_ex_literis(scripta, c->piscina), ZEPHYRUM, c->piscina);
    PlasticaFructus tria = plastica_legere_seminis(
        chorda_ex_literis(scripta, c->piscina), III, c->piscina);
    PlasticaFructus manu = _lege(c, aucta);
    PlasticaFructus nimia;

    CREDO_NECESSE (nuda.successus && nulla.successus);
    CREDO_NECESSE (tria.successus && manu.successus);
    CREDO_VERUM (_statuae_eaedem(nuda.statua, nulla.statua));
    CREDO_VERUM (_statuae_eaedem(tria.statua, manu.statua));
    CREDO_FALSUM (_statuae_eaedem(nuda.statua, tria.statua));
    /* partes (tituli) non mutantur */
    CREDO_VERUM (chorda_aequalis(tria.partes[ZEPHYRUM],
        nuda.partes[ZEPHYRUM]));
    /* semen extra fines: recusatur */
    nimia = plastica_legere_seminis(
        chorda_ex_literis(scripta, c->piscina), M * M + I,
        c->piscina);
    CREDO_FALSUM (nimia.successus);
    CREDO_VERUM (nimia.statua == NIHIL);
    CREDO_VERUM (nimia.error.mensura > ZEPHYRUM);
    CREDO_AEQUALIS_I32 (nimia.linea, ZEPHYRUM);
    /* descriptio recusata cum semine: linea ut semper */
    nimia = plastica_legere_seminis(chorda_ex_literis(
        "<res titulus=\"a\">\n<globus/>\n</res>", c->piscina), III,
        c->piscina);
    CREDO_FALSUM (nimia.successus);
    CREDO_AEQUALIS_I32 (nimia.linea, II);
}

/* fines verticum retis nodi n in spatio nodi (sine transformatione) */
interior Fines
_fines_nodi (
    constans Statua* s,
                i32  n)
{
    constans StatuaRete* rete = &s->retia[s->nodi[n].rete];
    constans StatuaPars* pars = &s->partes[rete->pars_prima];
                  Fines  f;
                    i32  k;
                    i32  a;

    per (a = ZEPHYRUM; a < III; a++)
    {
        f.minimum[a] = 1.0e9f;
        f.maximum[a] = -1.0e9f;
    }
    f.triangulorum = pars->indicum / III;
    per (k = ZEPHYRUM; k < pars->indicum; k++)
    {
        constans f32* v = s->positiones
            + s->indices[pars->index_primus + k] * III;

        per (a = ZEPHYRUM; a < III; a++)
        {
            si (v[a] < f.minimum[a])
            {
                f.minimum[a] = v[a];
            }
            si (v[a] > f.maximum[a])
            {
                f.maximum[a] = v[a];
            }
        }
    }
    redde f;
}

/* nodi cum reti, ordine: index k-esimi */
interior i32
_solidum_k (
    constans Statua* s,
                i32  k)
{
    i32 n;

    per (n = ZEPHYRUM; n < s->nodorum; n++)
    {
        si (s->nodi[n].rete >= ZEPHYRUM && k-- == ZEPHYRUM)
        {
            redde n;
        }
    }
    redde ZEPHYRUM;
}

/* series: magnitudo_gressus et semen_gressus */
interior vacuum
_sectio_series_gressus (
    CredoContextus* c)
{
    PlasticaFructus f = _lege(c,
        "<res titulus=\"scalae\">"
        "<series numerus=\"4\" gressus=\"0.5 0 0\""
        " magnitudo_gressus=\"0 0.5 0.25\">"
        "<capsa magnitudo=\"0.5 0.5 3\"/>"
        "<cylindrus radius=\"0.1\" altitudo=\"1\"/>"
        "</series>"
        "<capsa magnitudo=\"1 1 1\"/>"
        "</res>");
    PlasticaFructus g;
    PlasticaFructus sola;
              Fines fines;
                i32 k;

    CREDO_NECESSE (f.successus);
    /* exemplar k: capsa alta 0.5 (k + 1), lata 3 + 0.25 k; cylindrus
     * altus 1 + 0.5 k */
    per (k = ZEPHYRUM; k < IV; k++)
    {
        fines = _fines_nodi(f.statua, _solidum_k(f.statua, k * II));
        CREDO_F64_PROXIMUS ((f64)fines.maximum[I],
            0.5 * (f64)(k + I), 1.0e-5);
        CREDO_F64_PROXIMUS ((f64)fines.maximum[II],
            1.5 + 0.125 * (f64)k, 1.0e-5);
        CREDO_F64_PROXIMUS ((f64)fines.maximum[ZEPHYRUM], 0.25,
            1.0e-5);
        fines = _fines_nodi(f.statua,
            _solidum_k(f.statua, k * II + I));
        CREDO_F64_PROXIMUS ((f64)fines.maximum[I],
            1.0 + 0.5 * (f64)k, 1.0e-5);
        CREDO_F64_PROXIMUS ((f64)fines.maximum[ZEPHYRUM], 0.1,
            1.0e-5);
    }
    /* post seriem magnitudo propria redit */
    fines = _fines_nodi(f.statua, _solidum_k(f.statua, VIII));
    CREDO_F64_PROXIMUS ((f64)fines.maximum[I], 1.0, 1.0e-5);
    CREDO_F64_PROXIMUS ((f64)fines.maximum[II], 0.5, 1.0e-5);
    /* series nidificatae: gressus adduntur */
    g = _lege(c,
        "<res titulus=\"a\">"
        "<series numerus=\"2\" magnitudo_gressus=\"0 1 0\">"
        "<series numerus=\"2\" magnitudo_gressus=\"0 0.25 0\">"
        "<capsa magnitudo=\"1 1 1\"/>"
        "</series>"
        "</series>"
        "</res>");
    CREDO_NECESSE (g.successus);
    fines = _fines_nodi(g.statua, _solidum_k(g.statua, ZEPHYRUM));
    CREDO_F64_PROXIMUS ((f64)fines.maximum[I], 1.0, 1.0e-5);
    fines = _fines_nodi(g.statua, _solidum_k(g.statua, I));
    CREDO_F64_PROXIMUS ((f64)fines.maximum[I], 1.25, 1.0e-5);
    fines = _fines_nodi(g.statua, _solidum_k(g.statua, II));
    CREDO_F64_PROXIMUS ((f64)fines.maximum[I], 2.0, 1.0e-5);
    fines = _fines_nodi(g.statua, _solidum_k(g.statua, III));
    CREDO_F64_PROXIMUS ((f64)fines.maximum[I], 2.25, 1.0e-5);
    /* gressus negativus ad magnitudinem non positivam: recusatur */
    g = _lege(c,
        "<res titulus=\"a\">\n"
        "<series numerus=\"3\" magnitudo_gressus=\"0 -0.5 0\">\n"
        "<capsa magnitudo=\"1 1 1\"/>\n"
        "</series>\n</res>");
    CREDO_FALSUM (g.successus);
    CREDO_AEQUALIS_I32 (g.linea, III);
    g = _lege(c,
        "<res titulus=\"a\">\n"
        "<series numerus=\"3\" magnitudo_gressus=\"0 1\">\n"
        "<capsa magnitudo=\"1 1 1\"/>\n"
        "</series>\n</res>");
    CREDO_FALSUM (g.successus);
    CREDO_AEQUALIS_I32 (g.linea, II);
    /* semen_gressus: exemplar k = sphaera sola cum semine k * VII */
    g = _lege(c,
        "<res titulus=\"a\">"
        "<series numerus=\"3\" gressus=\"3 0 0\" semen_gressus=\"7\">"
        "<sphaera radius=\"1\" asperitas=\"0.5\" semen=\"2\"/>"
        "</series>"
        "</res>");
    CREDO_NECESSE (g.successus);
    sola = _lege(c,
        "<res titulus=\"a\">"
        "<sphaera radius=\"1\" asperitas=\"0.5\" semen=\"2\"/>"
        "<sphaera radius=\"1\" asperitas=\"0.5\" semen=\"9\"/>"
        "<sphaera radius=\"1\" asperitas=\"0.5\" semen=\"16\"/>"
        "</res>");
    CREDO_NECESSE (sola.successus);
    CREDO_AEQUALIS_I32 (g.statua->verticum, sola.statua->verticum);
    CREDO_VERUM (memcmp(g.statua->positiones, sola.statua->positiones,
        g.statua->verticum * III * magnitudo(f32)) == ZEPHYRUM);
    /* sine semen_gressus exemplaria eadem */
    g = _lege(c,
        "<res titulus=\"a\">"
        "<series numerus=\"2\" gressus=\"3 0 0\">"
        "<sphaera radius=\"1\" asperitas=\"0.5\" semen=\"2\"/>"
        "</series>"
        "</res>");
    CREDO_NECESSE (g.successus);
    CREDO_VERUM (memcmp(g.statua->positiones,
        g.statua->positiones + g.statua->verticum / II * III,
        g.statua->verticum / II * III * magnitudo(f32)) == ZEPHYRUM);
    g = _lege(c,
        "<res titulus=\"a\">\n"
        "<series numerus=\"2\" semen_gressus=\"-1\">\n"
        "<capsa magnitudo=\"1 1 1\"/>\n"
        "</series>\n</res>");
    CREDO_FALSUM (g.successus);
    CREDO_AEQUALIS_I32 (g.linea, II);
}

/* obex: attributum radicis obicem invisum solidum addit */
interior vacuum
_sectio_obex (
    CredoContextus* c)
{
    PlasticaFructus f = _lege(c,
        "<res titulus=\"palus\" obex=\"0.5\">"
        "<cylindrus titulus=\"hasta\" radius=\"0.1\" altitudo=\"2\"/>"
        "</res>");
              Fines fines;
                i32 n;

    CREDO_NECESSE (f.successus);
    /* radix, hasta, obex (ultimus) */
    CREDO_AEQUALIS_I32 (f.statua->nodorum, III);
    n = II;
    CREDO_AEQUALIS_S32 (f.statua->nodi[n].parens, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (f.statua->nodi[n].signa, STATUA_SIGNUM_SOLIDUM);
    CREDO_AEQUALIS_I32 (f.statua->nodi[I].signa, ZEPHYRUM);
    CREDO_VERUM (chorda_aequalis_literis(f.partes[n], "obex"));
    CREDO_VERUM (chorda_aequalis_literis(f.partes[I], "hasta"));
    fines = _fines_nodi(f.statua, n);
    CREDO_AEQUALIS_I32 (fines.triangulorum, XII);
    FINES_SUNT (fines, -0.25, 0.25, 0.0, 1.0, -0.25, 0.25);
    /* invisus: alpha 0, limen 0.5 */
    CREDO_F64_PROXIMUS ((f64)f.statua->materiae[I].color[III], 0.0,
        1.0e-9);
    CREDO_F64_PROXIMUS ((f64)f.statua->materiae[I].limen_alpha, 0.5,
        1.0e-9);
    CREDO_AEQUALIS_S32 (f.statua->materiae[I].textura, -I);
    /* duo numeri: x et z */
    f = _lege(c,
        "<res titulus=\"saepes\" obex=\"2 0.5\">"
        "<capsa magnitudo=\"2 0.1 0.1\" locus=\"0 0.5 0\"/>"
        "</res>");
    CREDO_NECESSE (f.successus);
    fines = _fines_nodi(f.statua, II);
    FINES_SUNT (fines, -1.0, 1.0, 0.0, 1.0, -0.25, 0.25);
    /* sine obice nihil additur */
    f = _lege(c,
        "<res titulus=\"a\"><capsa magnitudo=\"1 1 1\"/></res>");
    CREDO_NECESSE (f.successus);
    CREDO_AEQUALIS_I32 (f.statua->nodorum, II);
    /* valores mali */
    f = _lege(c, "<res titulus=\"a\" obex=\"0\">"
        "<capsa magnitudo=\"1 1 1\"/></res>");
    CREDO_FALSUM (f.successus);
    CREDO_AEQUALIS_I32 (f.linea, I);
    f = _lege(c, "<res titulus=\"a\" obex=\"1 -1\">"
        "<capsa magnitudo=\"1 1 1\"/></res>");
    CREDO_FALSUM (f.successus);
    f = _lege(c, "<res titulus=\"a\" obex=\"1 2 3\">"
        "<capsa magnitudo=\"1 1 1\"/></res>");
    CREDO_FALSUM (f.successus);
    f = _lege(c, "<res titulus=\"a\" obex=\"latus\">"
        "<capsa magnitudo=\"1 1 1\"/></res>");
    CREDO_FALSUM (f.successus);
    /* obex solus rem non facit */
    f = _lege(c, "<res titulus=\"a\" obex=\"1\"></res>");
    CREDO_FALSUM (f.successus);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "capsa", _sectio_capsa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ordinaria", _sectio_ordinaria, NIHIL, NIHIL, NIHIL, NIHIL },
    { "grex", _sectio_grex, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rotatio", _sectio_rotatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "invisibilis", _sectio_invisibilis, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "recusationes", _sectio_recusationes, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "tolerata", _sectio_tolerata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "determinata", _sectio_determinata, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "cylindrus", _sectio_cylindrus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sphaera", _sectio_sphaera, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cuneus", _sectio_cuneus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extrusio", _sectio_extrusio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tornus", _sectio_tornus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "speculum", _sectio_speculum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "series", _sectio_series, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sparsio", _sectio_sparsio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusationes_solidorum", _sectio_recusationes_solidorum,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "semen_additum", _sectio_semen_additum, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "series_gressus", _sectio_series_gressus, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "obex", _sectio_obex, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("plastica", SECTIONES);
}
