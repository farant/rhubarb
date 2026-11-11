/* probatio_plastica.c - descriptio textualis in statuam (plastica.h)
 *
 * capsa: triangula, fines, normales extrorsum, signa, color, partes;
 * grex: locus et gyrus liberos movent; rotatio; invisibilis;
 * recusationes cum linea; determinata.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "stereometria.h"
#include "statua.h"
#include "plastica.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define NODI_MAXIMI LXIV

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
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("plastica", SECTIONES);
}
