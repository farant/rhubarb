/* probatio_statua_iunctio.c - iunctio statuarum (statua_iunctio.h)
 *
 * Numeri summantur; res addita locum et gyrum suum habet (sensu
 * gyri plastici); basis intacta; signa manent; statua eadem pluries
 * addita semel tenetur; texturae pares semel; cycli basis manent,
 * addendarum omittuntur; iter per scriptorem GLB; recusationes.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "imago_typus.h"
#include "stereometria.h"
#include "statua.h"
#include "statua_glb.h"
#include "statua_glb_scriptor.h"
#include "statua_iunctio.h"
#include "plastica.h"
#include "credo.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define VIA_FUNGI "probationes/fixa/statuae/fungus.glb"

hic_manens constans character* BASIS =
    "<res titulus=\"locus\">"
    "<capsa titulus=\"humus\" magnitudo=\"20 1 20\" locus=\"0 -1 0\""
    " textura=\"saxum\" color=\"#888\" signum=\"gressibile\"/>"
    "<capsa titulus=\"murus\" magnitudo=\"1 2 1\" locus=\"-8 0 0\""
    " color=\"#555\" signum=\"solidum\"/>"
    "</res>";

/* capsa extra originem: gyrus sensum suum ostendit */
hic_manens constans character* ARCA =
    "<res titulus=\"arca\">"
    "<capsa titulus=\"arca\" magnitudo=\"1 1 1\" locus=\"1 0 0\""
    " textura=\"tabulae\" color=\"#8c6840\" signum=\"solidum\"/>"
    "</res>";

hic_manens constans character* PUTEUS =
    "<res titulus=\"puteus\">"
    "<grex titulus=\"murus\">"
    "<capsa titulus=\"saxum\" magnitudo=\"2 1 2\" textura=\"saxum\""
    " color=\"#888\" signum=\"solidum\"/>"
    "</grex>"
    "<cylindrus titulus=\"palus\" radius=\"0.1\" altitudo=\"2\""
    " locus=\"0 1 0\" color=\"#6b4a2b\"/>"
    "</res>";

nomen structura {
    f32 minimum[III];
    f32 maximum[III];
} Fines;

interior Statua*
_statua (
        CredoContextus* c,
    constans character* literae)
{
    PlasticaFructus f = plastica_legere(
        chorda_ex_literis(literae, c->piscina), c->piscina);

    si (!f.successus)
    {
        printf("descriptio recusata (linea %u): %.*s\n",
            (insignatus)f.linea, (integer)f.error.mensura,
            (constans character*)f.error.datum);
    }
    CREDO_NECESSE (f.successus);
    redde f.statua;
}

interior StatuaIungenda
_iungenda (
    constans Statua* statua,
                f32  x,
                f32  y,
                f32  z,
                f32  gyrus)
{
    StatuaIungenda i;

    i.statua  = statua;
    i.locus   = stereometria_vector(x, y, z);
    i.gyrus   = gyrus;
    i.scala   = 1.0f;
    redde i;
}

/* fines mundi retium nodorum [primus, finis) */
interior Fines
_fines (
     CredoContextus* c,
    constans Statua* s,
                i32  primus,
                i32  finis)
{
    StereometriaMatrix* m = (StereometriaMatrix*)piscina_allocare(
        c->piscina, (s->nodorum + I) * magnitudo(StereometriaMatrix));
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
    si (!m)
    {
        redde f;
    }
    statua_positura(s, -I, 0.0f, m);
    per (n = primus; n < finis; n++)
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

/* tabulae basis initium tabularum iunctae sunt? */
interior b32
_basis_intacta (
    constans Statua* basis,
    constans Statua* s)
{
    i32 n;

    si (   s->nodorum < basis->nodorum
        || s->retium < basis->retium
        || s->partium < basis->partium
        || s->verticum < basis->verticum
        || s->indicum < basis->indicum
        || s->materiarum < basis->materiarum
        || s->texturarum < basis->texturarum
        || s->cyclorum != basis->cyclorum)
    {
        redde FALSUM;
    }
    si (   memcmp(s->nodi, basis->nodi,
               basis->nodorum * magnitudo(StatuaNodus)) != ZEPHYRUM
        || memcmp(s->retia, basis->retia,
               basis->retium * magnitudo(StatuaRete)) != ZEPHYRUM
        || memcmp(s->partes, basis->partes,
               basis->partium * magnitudo(StatuaPars)) != ZEPHYRUM
        || memcmp(s->positiones, basis->positiones,
               basis->verticum * III * magnitudo(f32)) != ZEPHYRUM
        || memcmp(s->normales, basis->normales,
               basis->verticum * III * magnitudo(f32)) != ZEPHYRUM
        || memcmp(s->coordinatae, basis->coordinatae,
               basis->verticum * II * magnitudo(f32)) != ZEPHYRUM
        || memcmp(s->indices, basis->indices,
               basis->indicum * magnitudo(i32)) != ZEPHYRUM
        || memcmp(s->materiae, basis->materiae,
               basis->materiarum * magnitudo(StatuaMateria))
               != ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (n = ZEPHYRUM; n < basis->texturarum; n++)
    {
        si (   s->texturae[n].latitudo != basis->texturae[n].latitudo
            || s->texturae[n].altitudo != basis->texturae[n].altitudo
            || memcmp(s->texturae[n].pixela, basis->texturae[n].pixela,
                   basis->texturae[n].latitudo
                       * basis->texturae[n].altitudo * IV) != ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    per (n = ZEPHYRUM; n < basis->cyclorum; n++)
    {
        si (   !chorda_aequalis(s->cycli[n].titulus,
                   basis->cycli[n].titulus)
            || s->cycli[n].canalium != basis->cycli[n].canalium
            || s->cycli[n].duratio  != basis->cycli[n].duratio)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_sectio_numeri (
    CredoContextus* c)
{
            Statua* basis   = _statua(c, BASIS);
            Statua* arca    = _statua(c, ARCA);
            Statua* puteus  = _statua(c, PUTEUS);
    StatuaIungenda  addendae[II];
            Statua* s;

    addendae[ZEPHYRUM] = _iungenda(arca, 5.0f, 0.0f, 7.0f, 90.0f);
    addendae[I] = _iungenda(puteus, -3.0f, 0.0f, 2.0f, 0.0f);
    s = statua_coniungere(basis, addendae, II, c->piscina);
    CREDO_NECESSE (s != NIHIL);
    CREDO_VERUM (s != basis);
    CREDO_AEQUALIS_I32 (s->nodorum, basis->nodorum + I + arca->nodorum
        + I + puteus->nodorum);
    CREDO_AEQUALIS_I32 (s->retium, basis->retium + arca->retium
        + puteus->retium);
    CREDO_AEQUALIS_I32 (s->partium, basis->partium + arca->partium
        + puteus->partium);
    CREDO_AEQUALIS_I32 (s->verticum, basis->verticum + arca->verticum
        + puteus->verticum);
    CREDO_AEQUALIS_I32 (s->indicum, basis->indicum + arca->indicum
        + puteus->indicum);
    CREDO_AEQUALIS_I32 (s->materiarum, basis->materiarum
        + arca->materiarum + puteus->materiarum);
    /* finita: ordo et sphaera */
    CREDO_VERUM (s->ordo != NIHIL);
    CREDO_VERUM (s->radius > 10.0f);
    CREDO_VERUM (_basis_intacta(basis, s));
    /* statuae datae non mutatae */
    CREDO_AEQUALIS_I32 (basis->nodorum, III);
    CREDO_AEQUALIS_I32 (arca->nodorum, II);
}

/* locus et gyrus: idem ac grex plasticus cum locus et gyrus */
interior vacuum
_sectio_locus (
    CredoContextus* c)
{
            Statua* basis  = _statua(c, BASIS);
            Statua* arca   = _statua(c, ARCA);
            Statua* grex  = _statua(c,
                "<res titulus=\"a\">"
                "<grex locus=\"5 0 7\" gyrus=\"90\">"
                "<capsa magnitudo=\"1 1 1\" locus=\"1 0 0\"/>"
                "</grex>"
                "</res>");
    StatuaIungenda  addenda;
            Statua* s;
             Fines  f;
             Fines  g;
               i32  radix;

    addenda  = _iungenda(arca, 5.0f, 0.0f, 7.0f, 90.0f);
    s        = statua_coniungere(basis, &addenda, I, c->piscina);
    CREDO_NECESSE (s != NIHIL);
    radix = basis->nodorum;
    /* nodus radicalis novus */
    CREDO_AEQUALIS_S32 (s->nodi[radix].parens, -I);
    CREDO_AEQUALIS_S32 (s->nodi[radix].rete, -I);
    CREDO_AEQUALIS_I32 (s->nodi[radix].signa, ZEPHYRUM);
    CREDO_F64_PROXIMUS ((f64)s->nodi[radix].translatio.x, 5.0, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)s->nodi[radix].translatio.z, 7.0, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)s->nodi[radix].scala.x, 1.0, 1.0e-9);
    /* radix addendae sub eo pendet */
    CREDO_AEQUALIS_S32 (s->nodi[radix + I].parens, (s32)radix);
    CREDO_AEQUALIS_S32 (s->nodi[radix + II].parens, (s32)radix + I);
    /* capsa ad (1 0 0) gyro XC ad (0 0 -1) it: centrum (5, 0.5, 6) */
    f = _fines(c, s, radix, s->nodorum);
    FINES_SUNT (f, 4.5, 5.5, 0.0, 1.0, 5.5, 6.5);
    g = _fines(c, grex, ZEPHYRUM, grex->nodorum);
    FINES_SUNT (g, 4.5, 5.5, 0.0, 1.0, 5.5, 6.5);
    /* basis ubi erat */
    f = _fines(c, s, ZEPHYRUM, basis->nodorum);
    g = _fines(c, basis, ZEPHYRUM, basis->nodorum);
    CREDO_VERUM (memcmp(&f, &g, magnitudo(f)) == ZEPHYRUM);
    /* altitudo loci: res in y = 3 */
    addenda  = _iungenda(arca, 0.0f, 3.0f, 0.0f, 0.0f);
    s        = statua_coniungere(basis, &addenda, I, c->piscina);
    CREDO_NECESSE (s != NIHIL);
    f = _fines(c, s, basis->nodorum, s->nodorum);
    FINES_SUNT (f, 0.5, 1.5, 3.0, 4.0, -0.5, 0.5);
}

interior vacuum
_sectio_vacua (
    CredoContextus* c)
{
    Statua* basis = _statua(c, BASIS);
    Statua* s     = statua_coniungere(basis, NIHIL, ZEPHYRUM,
        c->piscina);

    CREDO_NECESSE (s != NIHIL);
    CREDO_VERUM (s != basis);
    CREDO_AEQUALIS_I32 (s->nodorum, basis->nodorum);
    CREDO_AEQUALIS_I32 (s->retium, basis->retium);
    CREDO_AEQUALIS_I32 (s->partium, basis->partium);
    CREDO_AEQUALIS_I32 (s->verticum, basis->verticum);
    CREDO_AEQUALIS_I32 (s->indicum, basis->indicum);
    CREDO_AEQUALIS_I32 (s->materiarum, basis->materiarum);
    CREDO_AEQUALIS_I32 (s->texturarum, basis->texturarum);
    CREDO_VERUM (_basis_intacta(basis, s));
    CREDO_F64_PROXIMUS ((f64)s->radius, (f64)basis->radius, 1.0e-6);
    /* tabulae propriae, non mutuatae */
    CREDO_VERUM (s->nodi != basis->nodi);
    CREDO_VERUM (s->positiones != basis->positiones);
}

interior vacuum
_sectio_signa (
    CredoContextus* c)
{
            Statua* basis   = _statua(c, BASIS);
            Statua* puteus  = _statua(c, PUTEUS);
    StatuaIungenda  addenda = _iungenda(puteus, 1.0f, 0.0f, 1.0f,
        45.0f);
            Statua* s = statua_coniungere(basis, &addenda, I,
                c->piscina);
               i32 radix;
               i32 n;

    CREDO_NECESSE (s != NIHIL);
    radix = basis->nodorum + I;
    per (n = ZEPHYRUM; n < puteus->nodorum; n++)
    {
        CREDO_AEQUALIS_I32 (s->nodi[radix + n].signa,
            puteus->nodi[n].signa);
    }
    /* saxum solidum, palus sine signo */
    CREDO_AEQUALIS_I32 (s->nodi[radix + II].signa,
        STATUA_SIGNUM_SOLIDUM);
    CREDO_AEQUALIS_I32 (s->nodi[radix + III].signa, ZEPHYRUM);
    /* signa basis */
    CREDO_AEQUALIS_I32 (s->nodi[I].signa, STATUA_SIGNUM_GRESSIBILE);
    CREDO_AEQUALIS_I32 (s->nodi[II].signa, STATUA_SIGNUM_SOLIDUM);
    /* materia et rete nodi additi ad tabulas motas spectant */
    CREDO_AEQUALIS_S32 (s->nodi[radix + II].rete,
        puteus->nodi[II].rete + (s32)basis->retium);
    CREDO_AEQUALIS_S32 (
        s->partes[s->retia[s->nodi[radix + III].rete].pars_prima]
            .materia, I + (s32)basis->materiarum);
    CREDO_F64_PROXIMUS ((f64)s->materiae[basis->materiarum + I]
        .color[ZEPHYRUM], (f64)0x6b / 255.0, 1.0e-6);
}

/* statua eadem ter: retia, vertices, materiae semel; nodi ter */
interior vacuum
_sectio_semel (
    CredoContextus* c)
{
            Statua* basis   = _statua(c, BASIS);
            Statua* arca    = _statua(c, ARCA);
            Statua* puteus  = _statua(c, PUTEUS);
    StatuaIungenda  addendae[IV];
            Statua* s;
             Fines  f;
               i32  radix;

    addendae[ZEPHYRUM] = _iungenda(arca, 2.0f, 0.0f, 0.0f, 0.0f);
    addendae[I] = _iungenda(puteus, 0.0f, 0.0f, 5.0f, 0.0f);
    addendae[II] = _iungenda(arca, 4.0f, 0.0f, 0.0f, 0.0f);
    addendae[III] = _iungenda(arca, 6.0f, 0.0f, 0.0f, 180.0f);
    s = statua_coniungere(basis, addendae, IV, c->piscina);
    CREDO_NECESSE (s != NIHIL);
    CREDO_AEQUALIS_I32 (s->nodorum, basis->nodorum
        + III * (I + arca->nodorum) + I + puteus->nodorum);
    CREDO_AEQUALIS_I32 (s->retium, basis->retium + arca->retium
        + puteus->retium);
    CREDO_AEQUALIS_I32 (s->verticum, basis->verticum + arca->verticum
        + puteus->verticum);
    CREDO_AEQUALIS_I32 (s->indicum, basis->indicum + arca->indicum
        + puteus->indicum);
    CREDO_AEQUALIS_I32 (s->materiarum, basis->materiarum
        + arca->materiarum + puteus->materiarum);
    /* exemplaria tria rete idem spectant */
    radix = basis->nodorum;
    CREDO_AEQUALIS_S32 (s->nodi[radix + II].rete,
        (s32)basis->retium);
    radix += I + arca->nodorum + I + puteus->nodorum;
    CREDO_AEQUALIS_S32 (s->nodi[radix + II].rete,
        (s32)basis->retium);
    f = _fines(c, s, radix, radix + I + arca->nodorum);
    FINES_SUNT (f, 4.5, 5.5, 0.0, 1.0, -0.5, 0.5);
    radix += I + arca->nodorum;
    CREDO_AEQUALIS_S32 (s->nodi[radix + II].rete,
        (s32)basis->retium);
    /* gyro CLXXX capsa ad (-1 0 0): centrum (5, 0.5, 0) */
    f = _fines(c, s, radix, s->nodorum);
    FINES_SUNT (f, 4.5, 5.5, 0.0, 1.0, -0.5, 0.5);
}

/* scala: res tota circa originem suam crescit; scala non positiva
 * aut non finita recusatur */
interior vacuum
_sectio_scala (
    CredoContextus* c)
{
            Statua* basis    = _statua(c, BASIS);
            Statua* arca     = _statua(c, ARCA);
    StatuaIungenda  addenda  = _iungenda(arca, 5.0f, 0.0f, 7.0f, 0.0f);
            Statua* s;
             Fines  f;
               i32  radix = basis->nodorum;

    /* capsa I x I x I ad (1 0 0); scala II: centrum (5 + 2, 1, 7),
     * latus II */
    addenda.scala  = 2.0f;
    s              = statua_coniungere(basis, &addenda, I, c->piscina);
    CREDO_NECESSE (s != NIHIL);
    CREDO_F64_PROXIMUS ((f64)s->nodi[radix].scala.x, 2.0, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)s->nodi[radix].scala.y, 2.0, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)s->nodi[radix].scala.z, 2.0, 1.0e-9);
    f = _fines(c, s, radix, s->nodorum);
    FINES_SUNT (f, 6.0, 8.0, 0.0, 2.0, 6.0, 8.0);
    /* scala dimidia cum gyro XC: capsa ad (0 0 -0.5) */
    addenda        = _iungenda(arca, 5.0f, 0.0f, 7.0f, 90.0f);
    addenda.scala  = 0.5f;
    s              = statua_coniungere(basis, &addenda, I, c->piscina);
    CREDO_NECESSE (s != NIHIL);
    f = _fines(c, s, radix, s->nodorum);
    FINES_SUNT (f, 4.75, 5.25, 0.0, 0.5, 6.25, 6.75);
    /* basis non scalatur */
    f = _fines(c, s, ZEPHYRUM, radix);
    CREDO_F64_PROXIMUS ((f64)f.maximum[ZEPHYRUM], 10.0, 1.0e-4);
    /* scala mala */
    addenda.scala = 0.0f;
    CREDO_VERUM (statua_coniungere(basis, &addenda, I, c->piscina)
        == NIHIL);
    addenda.scala = -1.0f;
    CREDO_VERUM (statua_coniungere(basis, &addenda, I, c->piscina)
        == NIHIL);
    addenda.scala = (f32)HUGE_VAL;
    CREDO_VERUM (statua_coniungere(basis, &addenda, I, c->piscina)
        == NIHIL);
}

/* texturae pixelis pares semel tenentur */
interior vacuum
_sectio_texturae (
    CredoContextus* c)
{
            Statua* basis   = _statua(c, BASIS);
            Statua* arca    = _statua(c, ARCA);
            Statua* puteus  = _statua(c, PUTEUS);
            Statua* aliud   = _statua(c,
                "<res titulus=\"a\">"
                "<capsa magnitudo=\"1 1 1\" textura=\"saxum\""
                " color=\"#888\""
                " semen=\"9\"/>"
                "</res>");
    StatuaIungenda  addendae[III];
            Statua* s;

    CREDO_AEQUALIS_I32 (basis->texturarum, I);
    CREDO_AEQUALIS_I32 (puteus->texturarum, I);
    /* puteus texturam basis ipsam habet (saxum #888 semen 0) */
    addendae[ZEPHYRUM] = _iungenda(puteus, 0.0f, 0.0f, 0.0f, 0.0f);
    addendae[I] = _iungenda(arca, 3.0f, 0.0f, 0.0f, 0.0f);
    addendae[II] = _iungenda(aliud, 6.0f, 0.0f, 0.0f, 0.0f);
    s = statua_coniungere(basis, addendae, III, c->piscina);
    CREDO_NECESSE (s != NIHIL);
    /* saxum (commune), tabulae, saxum seminis alterius */
    CREDO_AEQUALIS_I32 (s->texturarum, III);
    CREDO_AEQUALIS_S32 (s->materiae[basis->materiarum].textura,
        ZEPHYRUM);
    CREDO_AEQUALIS_S32 (s->materiae[basis->materiarum + I].textura,
        -I);
    CREDO_AEQUALIS_S32 (s->materiae[basis->materiarum
        + puteus->materiarum].textura, I);
    CREDO_AEQUALIS_S32 (s->materiae[basis->materiarum
        + puteus->materiarum + arca->materiarum].textura, II);
    CREDO_VERUM (memcmp(s->texturae[I].pixela,
        arca->texturae[ZEPHYRUM].pixela, XXXII * XXXII * IV)
        == ZEPHYRUM);
    CREDO_VERUM (memcmp(s->texturae[II].pixela,
        aliud->texturae[ZEPHYRUM].pixela, XXXII * XXXII * IV)
        == ZEPHYRUM);
}

/* statua addita cum texturis pluribus: quaeque materia texturam SUAM
 * servat (pixela eadem), etiam cum indices moventur */
interior vacuum
_sectio_texturae_plures (
    CredoContextus* c)
{
            Statua* basis = _statua(c, BASIS);
            Statua* duae  = _statua(c,
                "<res titulus=\"a\">"
                "<capsa magnitudo=\"1 1 1\" textura=\"lateres\""
                " color=\"#9c4a38\"/>"
                "<capsa magnitudo=\"1 1 1\" locus=\"2 0 0\""
                " textura=\"stramen\" color=\"#b08c44\"/>"
                "<capsa magnitudo=\"1 1 1\" locus=\"4 0 0\""
                " textura=\"saxum\" color=\"#888\"/>"
                "</res>");
    StatuaIungenda  addenda = _iungenda(duae, 0.0f, 0.0f, 5.0f, 0.0f);
            Statua* s = statua_coniungere(basis, &addenda, I,
                c->piscina);
               i32 k;

    CREDO_NECESSE (s != NIHIL);
    CREDO_AEQUALIS_I32 (duae->texturarum, III);
    /* saxum basis commune; lateres et stramen nova */
    CREDO_AEQUALIS_I32 (s->texturarum, III);
    per (k = ZEPHYRUM; k < III; k++)
    {
        s32 nova   = s->materiae[basis->materiarum + k].textura;
        s32 vetus  = duae->materiae[k].textura;

        CREDO_NECESSE (nova >= ZEPHYRUM && vetus >= ZEPHYRUM);
        CREDO_VERUM (memcmp(s->texturae[nova].pixela,
            duae->texturae[vetus].pixela, XXXII * XXXII * IV)
            == ZEPHYRUM);
    }
    CREDO_AEQUALIS_S32 (s->materiae[basis->materiarum].textura, I);
    CREDO_AEQUALIS_S32 (s->materiae[basis->materiarum + I].textura,
        II);
    CREDO_AEQUALIS_S32 (s->materiae[basis->materiarum + II].textura,
        ZEPHYRUM);
}

/* cycli basis manent; cycli addendae omittuntur */
interior vacuum
_sectio_cycli (
    CredoContextus* c)
{
    StatuaFructus lectio = statua_glb_legere(
        filum_legere_totum(VIA_FUNGI, c->piscina), c->piscina);
           Statua* fungus;
           Statua* arca   = _statua(c, ARCA);
           Statua* basis  = _statua(c, BASIS);
   StatuaIungenda  addenda;
           Statua* s;

    CREDO_NECESSE (lectio.successus);
    fungus = lectio.statua;
    CREDO_VERUM (fungus->cyclorum > ZEPHYRUM);
    addenda  = _iungenda(arca, 3.0f, 0.0f, 0.0f, 0.0f);
    s        = statua_coniungere(fungus, &addenda, I, c->piscina);
    CREDO_NECESSE (s != NIHIL);
    CREDO_AEQUALIS_I32 (s->cyclorum, fungus->cyclorum);
    CREDO_VERUM (_basis_intacta(fungus, s));
    CREDO_AEQUALIS_S32 (s->cycli[ZEPHYRUM].canales[ZEPHYRUM].nodus,
        fungus->cycli[ZEPHYRUM].canales[ZEPHYRUM].nodus);
    addenda  = _iungenda(fungus, 3.0f, 0.0f, 0.0f, 0.0f);
    s        = statua_coniungere(basis, &addenda, I, c->piscina);
    CREDO_NECESSE (s != NIHIL);
    CREDO_AEQUALIS_I32 (s->cyclorum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (s->nodorum, basis->nodorum + I
        + fungus->nodorum);
}

/* statua iuncta per scriptorem et lectorem GLB it */
interior vacuum
_sectio_glb (
    CredoContextus* c)
{
            Statua* basis   = _statua(c, BASIS);
            Statua* arca    = _statua(c, ARCA);
            Statua* puteus  = _statua(c, PUTEUS);
    StatuaIungenda  addendae[III];
            Statua* s;
            chorda  g;
     StatuaFructus  lectio;
             Fines  f;
             Fines  h;

    addendae[ZEPHYRUM] = _iungenda(arca, 5.0f, 0.0f, 7.0f, 90.0f);
    addendae[I] = _iungenda(puteus, -3.0f, 0.0f, 2.0f, 30.0f);
    addendae[II] = _iungenda(arca, 2.0f, 0.0f, -4.0f, 0.0f);
    s = statua_coniungere(basis, addendae, III, c->piscina);
    CREDO_NECESSE (s != NIHIL);
    g = statua_glb_scribere(s, NIHIL, c->piscina);
    CREDO_NECESSE (g.datum != NIHIL);
    lectio = statua_glb_legere(g, c->piscina);
    CREDO_NECESSE (lectio.successus);
    CREDO_AEQUALIS_I32 (lectio.statua->nodorum, s->nodorum);
    CREDO_AEQUALIS_I32 (lectio.statua->retium, s->retium);
    CREDO_AEQUALIS_I32 (lectio.statua->verticum, s->verticum);
    CREDO_AEQUALIS_I32 (lectio.statua->indicum, s->indicum);
    CREDO_AEQUALIS_I32 (lectio.statua->texturarum, s->texturarum);
    f = _fines(c, s, ZEPHYRUM, s->nodorum);
    h = _fines(c, lectio.statua, ZEPHYRUM, lectio.statua->nodorum);
    CREDO_VERUM (memcmp(&f, &h, magnitudo(f)) == ZEPHYRUM);
}

interior vacuum
_sectio_nihil (
    CredoContextus* c)
{
            Statua* basis  = _statua(c, BASIS);
            Statua* arca   = _statua(c, ARCA);
    StatuaIungenda  addendae[II];

    addendae[ZEPHYRUM]  = _iungenda(arca, 1.0f, 0.0f, 0.0f, 0.0f);
    addendae[I]         = _iungenda(arca, 2.0f, 0.0f, 0.0f, 0.0f);
    CREDO_VERUM (statua_coniungere(NIHIL, addendae, II, c->piscina)
        == NIHIL);
    CREDO_VERUM (statua_coniungere(basis, addendae, II, NIHIL)
        == NIHIL);
    CREDO_VERUM (statua_coniungere(basis, NIHIL, II, c->piscina)
        == NIHIL);
    addendae[I].statua = NIHIL;
    CREDO_VERUM (statua_coniungere(basis, addendae, II, c->piscina)
        == NIHIL);
    addendae[I]          = _iungenda(arca, 2.0f, 0.0f, 0.0f, 0.0f);
    addendae[I].locus.z  = (f32)HUGE_VAL;
    CREDO_VERUM (statua_coniungere(basis, addendae, II, c->piscina)
        == NIHIL);
    addendae[I]        = _iungenda(arca, 2.0f, 0.0f, 0.0f, 0.0f);
    addendae[I].gyrus  = (f32)HUGE_VAL;
    CREDO_VERUM (statua_coniungere(basis, addendae, II, c->piscina)
        == NIHIL);
    /* integra adhuc iunguntur */
    addendae[I] = _iungenda(arca, 2.0f, 0.0f, 0.0f, 0.0f);
    CREDO_VERUM (statua_coniungere(basis, addendae, II, c->piscina)
        != NIHIL);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "numeri", _sectio_numeri, NIHIL, NIHIL, NIHIL, NIHIL },
    { "locus", _sectio_locus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vacua", _sectio_vacua, NIHIL, NIHIL, NIHIL, NIHIL },
    { "signa", _sectio_signa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "scala", _sectio_scala, NIHIL, NIHIL, NIHIL, NIHIL },
    { "semel", _sectio_semel, NIHIL, NIHIL, NIHIL, NIHIL },
    { "texturae", _sectio_texturae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "texturae_plures", _sectio_texturae_plures, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "cycli", _sectio_cycli, NIHIL, NIHIL, NIHIL, NIHIL },
    { "glb", _sectio_glb, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nihil", _sectio_nihil, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("statua_iunctio", SECTIONES);
}
