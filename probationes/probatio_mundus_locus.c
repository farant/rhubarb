/* probatio_mundus_locus.c - dispositio et locus instructus
 * (mundus_locus.h)
 *
 * legere: campi, lineae, recusationes. struere in loco ficto: capsa
 * in solo plano ad altitudinem soli stat et cellas suas claudit,
 * ceterae intactae; gradus signatus suggestum attingibile facit;
 * coma sine signo solum non claudit; persona post capsam altam
 * occultatur, ante eam apparet; loca mala et palus tenuis cum linea
 * recusantur. Constantes paginae plasticae cum mundo congruunt.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "thema.h"
#include "dithering.h"
#include "stereometria.h"
#include "statua.h"
#include "statua_pictura.h"
#include "statua_glb.h"
#include "plastica.h"
#include "plastica_specimen.h"
#include "mundus_solum.h"
#include "mundus.h"
#include "mundus_pictura.h"
#include "mundus_locus.h"
#include "credo.h"
#include "mundus_locus_fictus.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define VIA_FUNGI    "probationes/fixa/statuae/fungus.glb"
#define QUADRI_LATUS CCXL
#define FUNDUS       0x55

hic_manens constans character* RES_ARCA =
    "<res titulus=\"arca\">"
    "<capsa magnitudo=\"1 1 1\" signum=\"solidum\"/>"
    "</res>";

hic_manens constans character* RES_GRADUS =
    "<res titulus=\"gradus\">"
    "<capsa magnitudo=\"1 0.75 1\""
    " signum=\"gressibile\"/>"
    "</res>";

hic_manens constans character* RES_ARBOR =
    "<res titulus=\"arbor\">"
    "<cylindrus radius=\"0.15\" altitudo=\"2\""
    " color=\"#642\"/>"
    "<capsa magnitudo=\"0.5 2 0.5\""
    " invisibilis=\"verum\" signum=\"solidum\"/>"
    "<sphaera magnitudo=\"3 2 3\" locus=\"0 2 0\""
    " color=\"#262\"/>"
    "</res>";

hic_manens constans character* RES_PALUS =
    "<res titulus=\"palus\">"
    "<capsa magnitudo=\"0.1 2 0.1\" signum=\"solidum\"/>"
    "</res>";

hic_manens constans character* RES_PALUS_II =
    "<res titulus=\"palus\">"
    "<capsa magnitudo=\"0.1 2 0.1\"/>"
    "<capsa magnitudo=\"0.5 2 0.5\""
    " invisibilis=\"verum\" signum=\"solidum\"/>"
    "</res>";

hic_manens constans character* RES_FLOS =
    "<res titulus=\"flos\">"
    "<capsa magnitudo=\"0.1 0.3 0.1\" color=\"#f0f\"/>"
    "</res>";

hic_manens constans character* RES_TURRIS =
    "<res titulus=\"turris\">"
    "<capsa magnitudo=\"3 5 3\" color=\"#777\""
    " signum=\"solidum\"/>"
    "</res>";

hic_manens constans character* RES_TRABS =
    "<res titulus=\"trabs\">"
    "<capsa magnitudo=\"2 1 0.5\" signum=\"solidum\"/>"
    "</res>";

interior Statua*
_res (
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

interior MundusLocusRes
_positio (
    f32 x,
    f32 z,
    f32 gyrus,
    i32 linea)
{
    MundusLocusRes r;

    memset(&r, ZEPHYRUM, magnitudo(r));
    r.x      = x;
    r.z      = z;
    r.gyrus  = gyrus;
    r.linea  = linea;
    redde r;
}

/* rem unam struere */
interior MundusLocusInstructus
_strue (
     CredoContextus* c,
    constans Statua* basis,
    constans Statua* res,
                f32  x,
                f32  z,
                i32  linea)
{
     MundusLocusRes  positio = _positio(x, z, 0.0f, linea);
    constans Statua* statuae[I];

    statuae[ZEPHYRUM] = res;
    redde mundus_locus_struere(basis, &positio, statuae, I,
        c->piscina);
}

#define RECUSATUR(fructus, linea_sperata, fragmentum) \
    fac { \
        CREDO_FALSUM ((fructus).successus); \
        CREDO_AEQUALIS_I32 ((fructus).linea, (linea_sperata)); \
        si (!chorda_continet((fructus).error, \
            chorda_ex_literis((fragmentum), c->piscina))) \
        { \
            printf("nuntius: %.*s\n", \
                (integer)(fructus).error.mensura, \
                (constans character*)(fructus).error.datum); \
        } \
        CREDO_VERUM (chorda_continet((fructus).error, \
            chorda_ex_literis((fragmentum), c->piscina))); \
    } dum (FALSUM)

interior MundusLocusFructus
_lege (
        CredoContextus* c,
    constans character* literae)
{
    redde mundus_locus_legere(chorda_ex_literis(literae, c->piscina),
        c->piscina);
}

/* cellae in quibus tabulae duae genere differunt; *extra = quot
 * earum extra rectangulum [x0, x1] x [z0, z1] iacent */
interior i32
_cellae_mutatae (
    constans MundusSolum* a,
    constans MundusSolum* b,
                     f32  x0,
                     f32  x1,
                     f32  z0,
                     f32  z1,
                     i32* extra)
{
    i32 mutatae = ZEPHYRUM;
    f32 x;
    f32 z;

    *extra = ZEPHYRUM;
    per (z = -1.875f; z < 20.0f; z += 0.25f)
    {
        per (x = -1.875f; x < 20.0f; x += 0.25f)
        {
            si (mundus_solum_genus(a, x, z)
                == mundus_solum_genus(b, x, z))
            {
                perge;
            }
            mutatae++;
            *extra += (i32)(x < x0 || x > x1 || z < z0 || z > z1);
        }
    }
    redde mutatae;
}


/* ==================================================
 * Legere
 * ================================================== */

interior vacuum
_sectio_legere (
    CredoContextus* c)
{
    MundusLocusFructus f = _lege(c,
        "<locus titulus=\"clearing\"\n"
        "       basis=\"probationes/fixa/statuae/planum.glb\">\n"
        "  <!-- puteus in medio -->\n"
        "  <res fons=\"plastica/res/puteus.stml\" locus=\"30 34\""
        " gyrus=\"15\"/>\n"
        "\n"
        "  <res fons=\"plastica/res/saepes.stml\" locus=\"-2.5 7\"/>\n"
        "</locus>\n");

    CREDO_NECESSE (f.successus);
    CREDO_VERUM (chorda_aequalis_literis(f.titulus, "clearing"));
    CREDO_VERUM (chorda_aequalis_literis(f.basis,
        "probationes/fixa/statuae/planum.glb"));
    CREDO_AEQUALIS_I32 (f.numerus, II);
    CREDO_VERUM (chorda_aequalis_literis(f.res[ZEPHYRUM].fons,
        "plastica/res/puteus.stml"));
    CREDO_F64_PROXIMUS ((f64)f.res[ZEPHYRUM].x, 30.0, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)f.res[ZEPHYRUM].z, 34.0, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)f.res[ZEPHYRUM].gyrus, 15.0, 1.0e-9);
    CREDO_AEQUALIS_I32 (f.res[ZEPHYRUM].linea, IV);
    CREDO_VERUM (chorda_aequalis_literis(f.res[I].fons,
        "plastica/res/saepes.stml"));
    CREDO_F64_PROXIMUS ((f64)f.res[I].x, -2.5, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)f.res[I].z, 7.0, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)f.res[I].gyrus, 0.0, 1.0e-9);
    CREDO_AEQUALIS_I32 (f.res[I].linea, VI);
    CREDO_AEQUALIS_I32 (f.error.mensura, ZEPHYRUM);
    /* dispositio sine rebus licet */
    f = _lege(c, "<locus titulus=\"a\" basis=\"b.glb\"/>");
    CREDO_NECESSE (f.successus);
    CREDO_AEQUALIS_I32 (f.numerus, ZEPHYRUM);
}

interior vacuum
_sectio_legere_recusat (
    CredoContextus* c)
{
    MundusLocusFructus f;

    f = _lege(c, "non stml <");
    CREDO_FALSUM (f.successus);
    CREDO_VERUM (f.res == NIHIL);
    CREDO_VERUM (f.error.mensura > ZEPHYRUM);
    f = _lege(c, "<res titulus=\"a\"/>");
    RECUSATUR (f, I, "<locus>");
    f = _lege(c, "<locus basis=\"b.glb\"/>");
    RECUSATUR (f, I, "'titulus'");
    f = _lege(c, "<locus titulus=\" \" basis=\"b.glb\"/>");
    RECUSATUR (f, I, "'titulus'");
    f = _lege(c, "<locus titulus=\"a\"/>");
    RECUSATUR (f, I, "'basis'");
    f = _lege(c,
        "<locus titulus=\"a\" basis=\"b.glb\" color=\"#fff\"/>");
    RECUSATUR (f, I, "'color'");
    f = _lege(c, "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "<res locus=\"1 2\"/>\n</locus>");
    RECUSATUR (f, II, "'fons'");
    f = _lege(c, "<locus titulus=\"a\" basis=\"b.glb\">\n\n"
        "<res fons=\"x.stml\"/>\n</locus>");
    RECUSATUR (f, III, "'locus'");
    f = _lege(c, "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "<res fons=\"x.stml\" locus=\"1 2 3\"/>\n</locus>");
    RECUSATUR (f, II, "'locus'");
    f = _lege(c, "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "<res fons=\"x.stml\" locus=\"1 duo\"/>\n</locus>");
    RECUSATUR (f, II, "'locus'");
    f = _lege(c, "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "<res fons=\"x.stml\" locus=\"1 2\" gyrus=\"multum\"/>\n"
        "</locus>");
    RECUSATUR (f, II, "'gyrus'");
    f = _lege(c, "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "<res fons=\"x.stml\" locus=\"1 2\" altitudo=\"3\"/>\n"
        "</locus>");
    RECUSATUR (f, II, "'altitudo'");
    f = _lege(c, "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "<res fons=\"x.stml\" locus=\"1 2\"/>\n"
        "<capsa magnitudo=\"1 1 1\"/>\n</locus>");
    RECUSATUR (f, III, "capsa");
    f = _lege(c, "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "<res fons=\"x.stml\" locus=\"1 2\"><res fons=\"y.stml\""
        " locus=\"1 2\"/></res>\n</locus>");
    RECUSATUR (f, II, "children");
    f = _lege(c, "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "verba\n</locus>");
    CREDO_FALSUM (f.successus);
    CREDO_VERUM (chorda_continet(f.error,
        chorda_ex_literis("text", c->piscina)));
}


/* ==================================================
 * Struere
 * ================================================== */

/* capsa in solo plano: altitudo soli, cellae clausae, ceterae
 * intactae */
interior vacuum
_sectio_capsa (
    CredoContextus* c)
{
                   Statua* basis = loci_ficti_locus(c);
                   Statua* capsa = _res(c, RES_ARCA);
              MundusSolum* nudum = mundus_solum_struere(basis,
                  c->piscina);
              MundusSolum* instructum;
    MundusLocusInstructus  r;
                      i32  extra;

    r = _strue(c, basis, capsa, 2.0f, 10.0f, VII);
    si (!r.successus)
    {
        printf("recusatus: %.*s\n", (integer)r.error.mensura,
            (constans character*)r.error.datum);
    }
    CREDO_NECESSE (r.successus);
    CREDO_NECESSE (r.statua != NIHIL);
    CREDO_AEQUALIS_I32 (r.linea, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (r.statua->nodorum, basis->nodorum + I
        + capsa->nodorum);
    /* radix rei: locus et altitudo soli (0) */
    CREDO_F64_PROXIMUS (
        (f64)r.statua->nodi[basis->nodorum].translatio.x, 2.0, 1.0e-6);
    CREDO_F64_PROXIMUS (
        (f64)r.statua->nodi[basis->nodorum].translatio.y, 0.0, 1.0e-6);
    CREDO_F64_PROXIMUS (
        (f64)r.statua->nodi[basis->nodorum].translatio.z, 10.0,
        1.0e-6);
    instructum = mundus_solum_struere(r.statua, c->piscina);
    CREDO_NECESSE (nudum != NIHIL && instructum != NIHIL);
    /* capsa I x I: XVI cellae, omnes intra vestigium */
    CREDO_AEQUALIS_I32 (_cellae_mutatae(nudum, instructum, 1.5f, 2.5f,
        9.5f, 10.5f, &extra), XVI);
    CREDO_AEQUALIS_I32 (extra, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (mundus_solum_genus(instructum, 2.1f, 10.1f),
        MUNDUS_SOLUM_SOLIDUM);
    CREDO_AEQUALIS_I32 (mundus_solum_genus(nudum, 2.1f, 10.1f),
        MUNDUS_SOLUM_GRESSIBILE);
    CREDO_AEQUALIS_I32 (mundus_solum_genus(instructum, 2.7f, 10.1f),
        MUNDUS_SOLUM_GRESSIBILE);
    /* in suggestu superiore: altitudo IV */
    r = _strue(c, basis, capsa, 10.0f, 4.0f, I);
    CREDO_NECESSE (r.successus);
    CREDO_F64_PROXIMUS (
        (f64)r.statua->nodi[basis->nodorum].translatio.y, 4.0, 1.0e-4);
    /* in clivo: altitudo soli sub puncto (x 6 -> y 2) */
    r = _strue(c, basis, capsa, 6.0f, 10.0f, I);
    CREDO_NECESSE (r.successus);
    CREDO_F64_PROXIMUS (
        (f64)r.statua->nodi[basis->nodorum].translatio.y, 2.0, 0.2);
    /* gyrus: trabs II x 0.5 gyro XC secundum z iacet */
    {
         MundusLocusRes  positio = _positio(2.0f, 10.0f, 90.0f, I);
        constans Statua* statuae[I];

        statuae[ZEPHYRUM] = _res(c, RES_TRABS);
        r = mundus_locus_struere(basis, &positio, statuae, I,
            c->piscina);
        CREDO_NECESSE (r.successus);
        instructum = mundus_solum_struere(r.statua, c->piscina);
        CREDO_NECESSE (instructum != NIHIL);
        CREDO_AEQUALIS_I32 (_cellae_mutatae(nudum, instructum, 1.75f,
            2.25f, 9.0f, 11.0f, &extra), XVI);
        CREDO_AEQUALIS_I32 (extra, ZEPHYRUM);
    }
    /* nullae res: locus nudus */
    r = mundus_locus_struere(basis, NIHIL, NIHIL, ZEPHYRUM, c->piscina);
    CREDO_NECESSE (r.successus);
    CREDO_AEQUALIS_I32 (r.statua->nodorum, basis->nodorum);
    CREDO_AEQUALIS_I32 (r.statua->verticum, basis->verticum);
}

/* gradus signatus suggestum altiorem attingibile facit */
interior vacuum
_sectio_gradus (
    CredoContextus* c)
{
       LociFictiQuadratum  q[II];
                   Statua* basis;
                   Statua* gradus = _res(c, RES_GRADUS);
              MundusSolum* nudum;
              MundusSolum* instructum;
    MundusLocusInstructus  r;
                      b32  nudum_patet  = VERUM;
                      b32  patet        = VERUM;
                      f32  x;

    q[ZEPHYRUM]  = loci_ficti_quadratum(0.0f, 4.0f, 0.0f, 4.0f, 0.0f,
        0.0f, LOCI_FICTI_GRESSIBILE);
    q[I]         = loci_ficti_quadratum(4.0f, 8.0f, 0.0f, 4.0f, 1.5f,
        1.5f, LOCI_FICTI_GRESSIBILE);
    basis  = loci_ficti_statua(c, q, II);
    nudum  = mundus_solum_struere(basis, c->piscina);
    r      = _strue(c, basis, gradus, 3.5f, 2.0f, III);
    si (!r.successus)
    {
        printf("recusatus: %.*s\n", (integer)r.error.mensura,
            (constans character*)r.error.datum);
    }
    CREDO_NECESSE (r.successus);
    instructum = mundus_solum_struere(r.statua, c->piscina);
    CREDO_NECESSE (nudum != NIHIL && instructum != NIHIL);
    /* iter ab x 2.625 ad x 4.625 secundum z 2.125, cella post
     * cellam */
    per (x = 2.625f; x < 4.5f; x += 0.25f)
    {
        nudum_patet = nudum_patet && mundus_solum_licet(nudum, x,
            2.125f, x + 0.25f, 2.125f, 0.1f);
        patet       = patet && mundus_solum_licet(instructum, x,
            2.125f, x + 0.25f, 2.125f, 0.1f);
    }
    CREDO_FALSUM (nudum_patet);
    CREDO_VERUM (patet);
    /* gradus gressibilis est, non solidus */
    CREDO_AEQUALIS_I32 (mundus_solum_genus(instructum, 3.5f, 2.0f),
        MUNDUS_SOLUM_GRESSIBILE);
}

/* coma sine signo solum non claudit; truncus cum obice claudit
 * (spec AUDIENDA 3) */
interior vacuum
_sectio_coma (
    CredoContextus* c)
{
                   Statua* basis = loci_ficti_locus(c);
                   Statua* arbor = _res(c, RES_ARBOR);
              MundusSolum* nudum = mundus_solum_struere(basis,
                  c->piscina);
              MundusSolum* instructum;
    MundusLocusInstructus  r;
                      i32  extra;

    r = _strue(c, basis, arbor, 2.0f, 10.0f, I);
    CREDO_NECESSE (r.successus);
    instructum = mundus_solum_struere(r.statua, c->piscina);
    CREDO_NECESSE (nudum != NIHIL && instructum != NIHIL);
    /* obex dimidiae unitatis: IV cellae; coma III lata nihil */
    CREDO_AEQUALIS_I32 (_cellae_mutatae(nudum, instructum, 1.75f,
        2.25f, 9.75f, 10.25f, &extra), IV);
    CREDO_AEQUALIS_I32 (extra, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (mundus_solum_genus(instructum, 3.0f, 10.0f),
        MUNDUS_SOLUM_GRESSIBILE);
}

interior vacuum
_sectio_recusationes (
    CredoContextus* c)
{
                   Statua* basis            = loci_ficti_locus(c);
                   Statua* capsa            = _res(c, RES_ARCA);
                   Statua* palus            = _res(c, RES_PALUS);
                   Statua* palus_cum_obice  = _res(c, RES_PALUS_II);
                   Statua* ornatus          = _res(c, RES_FLOS);
           MundusLocusRes  positiones[III];
          constans Statua* statuae[III];
    MundusLocusInstructus  r;

    /* extra locum, in aqua, in saxo (non gressibile) */
    r = _strue(c, basis, capsa, -3.0f, -3.0f, XI);
    RECUSATUR (r, XI, "outside");
    CREDO_VERUM (r.statua == NIHIL);
    r = _strue(c, basis, capsa, 2.0f, 2.0f, XII);
    RECUSATUR (r, XII, "water");
    r = _strue(c, basis, capsa, 5.5f, 1.5f, XIII);
    RECUSATUR (r, XIII, "not walkable");
    /* palus tenuis solidus nullam cellam claudit; cum obice licet;
     * res sine signo nihil claudere debet */
    r = _strue(c, basis, palus, 2.0f, 10.0f, XIV);
    RECUSATUR (r, XIV, "blocks no cell");
    r = _strue(c, basis, palus_cum_obice, 2.0f, 10.0f, XV);
    CREDO_VERUM (r.successus);
    r = _strue(c, basis, ornatus, 2.0f, 10.0f, XVI);
    CREDO_VERUM (r.successus);
    /* statua rei NIHIL */
    r = _strue(c, basis, NIHIL, 2.0f, 10.0f, XVII);
    RECUSATUR (r, XVII, "not compiled");
    /* prima culpata vincit; bonae ante eam nihil servant */
    positiones[ZEPHYRUM]  = _positio(2.0f, 10.0f, 0.0f, XXI);
    positiones[I]         = _positio(2.0f, 2.0f, 0.0f, XXII);
    positiones[II]        = _positio(-9.0f, 0.0f, 0.0f, XXIII);
    statuae[ZEPHYRUM]     = capsa;
    statuae[I]            = capsa;
    statuae[II]           = capsa;
    r = mundus_locus_struere(basis, positiones, statuae, III,
        c->piscina);
    RECUSATUR (r, XXII, "water");
    CREDO_VERUM (r.statua == NIHIL);
    /* argumenta NIHIL */
    r = mundus_locus_struere(NIHIL, positiones, statuae, I,
        c->piscina);
    CREDO_FALSUM (r.successus);
    CREDO_AEQUALIS_I32 (r.linea, ZEPHYRUM);
    r = mundus_locus_struere(basis, positiones, statuae, I, NIHIL);
    CREDO_FALSUM (r.successus);
    r = mundus_locus_struere(basis, NIHIL, statuae, I, c->piscina);
    CREDO_FALSUM (r.successus);
    r = mundus_locus_struere(basis, positiones, NIHIL, I, c->piscina);
    CREDO_FALSUM (r.successus);
}

/* pixela quibus quadrum cum corpore ab humo sola differt */
interior i32
_pixela_corporis (
     CredoContextus* c,
             Statua* locus,
    constans Statua* persona,
                f32  x,
                f32  z)
{
     MundusHumus* humus;
          Mundus  mundus;
    MundusCorpus  corpus;
     MundusForma  forma;
             b32  activi[XVI];
              i8* quadrum;
              i8* nudum;
             s32  origo_x = ZEPHYRUM;
             s32  origo_y = ZEPHYRUM;
             i32  numerus = ZEPHYRUM;
             i32  k;

    dithering_praeparare_omnes(activi);
    memset(&mundus, ZEPHYRUM, magnitudo(mundus));
    memset(&corpus, ZEPHYRUM, magnitudo(corpus));
    humus = mundus_humus_creare(locus, activi, c->piscina);
    mundus.solum = mundus_solum_struere(locus, c->piscina);
    mundus.corpora = &corpus;
    mundus.corporum = I;
    forma = mundus_forma(persona, c->piscina);
    corpus.radius = MUNDUS_CORPORIS_RADIUS;
    quadrum = (i8*)loci_ficti_memoria(c, QUADRI_LATUS * QUADRI_LATUS);
    nudum = (i8*)loci_ficti_memoria(c, QUADRI_LATUS * QUADRI_LATUS);
    CREDO_NECESSE (humus != NIHIL && mundus.solum != NIHIL);
    CREDO_NECESSE (mundus_corpus_ponere(&mundus, &corpus, x, z));
    CREDO_NECESSE (mundus_quadrum_pingere(humus, &mundus, &forma, I,
        ZEPHYRUM, activi, FUNDUS, QUADRI_LATUS, QUADRI_LATUS, quadrum,
        c->piscina));
    CREDO_NECESSE (mundus_quadri_origo(&mundus, ZEPHYRUM, QUADRI_LATUS,
        QUADRI_LATUS, &origo_x, &origo_y));
    CREDO_NECESSE (mundus_humus_copiare(humus, origo_x, origo_y,
        QUADRI_LATUS, QUADRI_LATUS, FUNDUS, nudum, NIHIL));
    per (k = ZEPHYRUM; k < QUADRI_LATUS * QUADRI_LATUS; k++)
    {
        numerus += (i32)(quadrum[k] != nudum[k]);
    }
    redde numerus;
}

/* persona post capsam altam occultatur, ante eam apparet (spec
 * AUDIENDA 4) */
interior vacuum
_sectio_occultatio (
    CredoContextus* c)
{
       LociFictiQuadratum  q;
                   Statua* basis;
                   Statua* turris = _res(c, RES_TURRIS);
            StatuaFructus  fungus = statua_glb_legere(
                filum_legere_totum(VIA_FUNGI, c->piscina), c->piscina);
        StatuaScaenaVisus visus = mundus_visus(ZEPHYRUM, ZEPHYRUM);
    MundusLocusInstructus r;
                      f32 px;
                      f32 py;
                      f32 profunditas_a = 0.0f;
                      f32 profunditas_b = 0.0f;
                      i32 pixela_a;
                      i32 pixela_b;
                      i32 post;
                      i32 ante;

    thema_initiare();
    CREDO_NECESSE (fungus.successus);
    q = loci_ficti_quadratum(0.0f, 16.0f, 0.0f, 16.0f, 0.0f, 0.0f,
        LOCI_FICTI_GRESSIBILE);
    basis  = loci_ficti_statua(c, &q, I);
    r      = _strue(c, basis, turris, 8.0f, 8.0f, I);
    CREDO_NECESSE (r.successus);
    /* puncta duo trans capsam in diagonali visus: uter remotior sit
     * profunditas dicit */
    statua_scaena_proicere(&visus,
        stereometria_vector(5.6f, 0.0f, 10.4f), &px, &py,
        &profunditas_a);
    statua_scaena_proicere(&visus,
        stereometria_vector(10.4f, 0.0f, 5.6f), &px, &py,
        &profunditas_b);
    CREDO_VERUM (fabs((f64)(profunditas_a - profunditas_b)) > 1.0);
    pixela_a = _pixela_corporis(c, r.statua, fungus.statua, 5.6f,
        10.4f);
    pixela_b = _pixela_corporis(c, r.statua, fungus.statua, 10.4f,
        5.6f);
    post = profunditas_a > profunditas_b ? pixela_a : pixela_b;
    ante = profunditas_a > profunditas_b ? pixela_b : pixela_a;
    printf("  persona ante capsam %u pixela, post eam %u\n",
        (insignatus)ante, (insignatus)post);
    CREDO_VERUM (ante > D);
    CREDO_VERUM (post * X < ante);
    /* in loco nudo utrobique apparet */
    CREDO_VERUM (_pixela_corporis(c, basis, fungus.statua, 5.6f,
        10.4f) > D);
    CREDO_VERUM (_pixela_corporis(c, basis, fungus.statua, 10.4f,
        5.6f) > D);
}

/* locus verus instructus (apps/mundus/loci/clearing.stml cum rebus
 * plastica/res): numeri FIXI semel visi et in mappa oculis probati
 * (plastica-plan T10). Dispositio aut res mutata hos numeros mutat:
 * consulto renoventur, mappa iterum inspecta. Locus nudus: 60626
 * gressibiles, 42881 attingibiles, stratum summum numquam attingitur
 * (probatio_mundus_solum). */
#define VIA_DISPOSITIONIS     "apps/mundus/loci/clearing.stml"
#define NUDI_GRESSIBILES      60626
#define NUDI_ATTINGIBILES     42881
#define PRATI_RES          XLII
#define PRATI_GRESSIBILES  60043
#define PRATI_ATTINGIBILES 46683
#define PRATI_SUMMAE       5405

hic_manens constans s32 VICINI[VIII] = { 1, 0, -1, 0, 0, 1, 0, -1 };

interior vacuum
_sectio_pratum (
    CredoContextus* c)
{
       MundusLocusFructus d = mundus_locus_legere(
           filum_legere_totum(VIA_DISPOSITIONIS, c->piscina),
           c->piscina);
    MundusLocusInstructus   r;
            StatuaFructus   basis;
          constans Statua** statuae;
              MundusSolum*  s;
                       i8*  visa;
                      s32*  acervus;
                      i32   gressibiles   = ZEPHYRUM;
                      i32   attingibiles  = ZEPHYRUM;
                      i32   summae        = ZEPHYRUM;
                      i32   n             = ZEPHYRUM;
                      i32   k;
                      i32   j;
                      s32   cella_x;
                      s32   cella_z;
                      s32   directio;
                      f32   x = 0.0f;
                      f32   z = 0.0f;
                      f32   cella = 1.0f
                          / (f32)MUNDUS_CELLAE_PER_UNITATEM;

    si (!d.successus)
    {
        printf("dispositio recusata (linea %u): %.*s\n",
            (insignatus)d.linea, (integer)d.error.mensura,
            (constans character*)d.error.datum);
    }
    CREDO_NECESSE (d.successus);
    CREDO_VERUM (chorda_aequalis_literis(d.titulus, "clearing"));
    CREDO_AEQUALIS_I32 (d.numerus, PRATI_RES);
    basis = statua_glb_legere(filum_legere_totum(
        chorda_ut_cstr(d.basis, c->piscina), c->piscina), c->piscina);
    CREDO_NECESSE (basis.successus);
    statuae = (constans Statua**)piscina_allocare(c->piscina,
        (d.numerus + I) * magnitudo(Statua*));
    CREDO_NECESSE (statuae != NIHIL);
    /* res quaeque semel componitur (fons idem, index idem) */
    per (k = ZEPHYRUM; k < d.numerus; k++)
    {
        per (j = ZEPHYRUM; j < k; j++)
        {
            si (chorda_aequalis(d.res[j].fons, d.res[k].fons))
            {
                frange;
            }
        }
        si (j < k)
        {
            statuae[k] = statuae[j];
            perge;
        }
        statuae[k] = _res(c, chorda_ut_cstr(filum_legere_totum(
            chorda_ut_cstr(d.res[k].fons, c->piscina), c->piscina),
            c->piscina));
    }
    r = mundus_locus_struere(basis.statua, d.res, statuae, d.numerus,
        c->piscina);
    si (!r.successus)
    {
        printf("locus recusatus (linea %u): %.*s\n",
            (insignatus)r.linea, (integer)r.error.mensura,
            (constans character*)r.error.datum);
    }
    CREDO_NECESSE (r.successus);
    s = mundus_solum_struere(r.statua, c->piscina);
    CREDO_NECESSE (s != NIHIL);
    per (k = ZEPHYRUM; k < s->latitudo * s->altitudo; k++)
    {
        gressibiles += (i32)(s->genera[k] == MUNDUS_SOLUM_GRESSIBILE);
    }
    /* ambulatio corporis radii dimidii ab initio per cellas vicinas
     * (ut probatio_mundus_solum) */
    CREDO_NECESSE (mundus_solum_initium(s, 0.5f, &x, &z));
    visa     = (i8*)loci_ficti_memoria(c,
        (memoriae_index)s->latitudo * s->altitudo);
    acervus  = (s32*)loci_ficti_memoria(c,
        (memoriae_index)s->latitudo * s->altitudo * II
            * magnitudo(s32));
    cella_x = (s32)((x - s->origo_x) / cella);
    cella_z = (s32)((z - s->origo_z) / cella);
    visa[(i32)cella_z * s->latitudo + (i32)cella_x] = I;
    acervus[n++] = cella_x;
    acervus[n++] = cella_z;
    dum (n > ZEPHYRUM)
    {
        f32 punctum_x;
        f32 punctum_z;

        cella_z = acervus[--n];
        cella_x = acervus[--n];
        attingibiles++;
        punctum_x = s->origo_x + ((f32)cella_x + 0.5f) * cella;
        punctum_z = s->origo_z + ((f32)cella_z + 0.5f) * cella;
        summae += (i32)(s->altitudines[(i32)cella_z * s->latitudo
            + (i32)cella_x] > 7.0f);
        per (directio = ZEPHYRUM; directio < IV; directio++)
        {
            s32 vicina_x = cella_x + VICINI[directio * II];
            s32 vicina_z = cella_z + VICINI[directio * II + I];

            si (   vicina_x < ZEPHYRUM || vicina_z < ZEPHYRUM
                || vicina_x >= (s32)s->latitudo
                || vicina_z >= (s32)s->altitudo
                || visa[(i32)vicina_z * s->latitudo + (i32)vicina_x]
                || !mundus_solum_licet(s, punctum_x, punctum_z,
                       punctum_x + (f32)VICINI[directio * II] * cella,
                       punctum_z
                           + (f32)VICINI[directio * II + I] * cella,
                       0.5f))
            {
                perge;
            }
            visa[(i32)vicina_z * s->latitudo + (i32)vicina_x] = I;
            acervus[n++] = vicina_x;
            acervus[n++] = vicina_z;
        }
    }
    si (   gressibiles  != PRATI_GRESSIBILES
        || attingibiles != PRATI_ATTINGIBILES
        || summae       != PRATI_SUMMAE)
    {
        printf("  pratum: gressibiles %u attingibiles %u summae %u\n",
            (insignatus)gressibiles, (insignatus)attingibiles,
            (insignatus)summae);
    }
    CREDO_AEQUALIS_I32 (gressibiles, PRATI_GRESSIBILES);
    CREDO_AEQUALIS_I32 (attingibiles, PRATI_ATTINGIBILES);
    CREDO_AEQUALIS_I32 (summae, PRATI_SUMMAE);
    /* res cellas claudunt, numquam addunt extra scalas: gressibiles
     * minus quam nudae; scalae stratum summum aperiunt, ergo
     * attingibiles PLURES quam in loco nudo */
    CREDO_VERUM (gressibiles < NUDI_GRESSIBILES);
    CREDO_VERUM (NUDI_GRESSIBILES - gressibiles < M);
    CREDO_VERUM (summae > M);
    CREDO_VERUM (attingibiles > NUDI_ATTINGIBILES);
    /* sine strato summo pauciores attinguntur quam in loco nudo:
     * res nihil praeter scalas aperiunt */
    CREDO_VERUM (attingibiles - summae < NUDI_ATTINGIBILES);
}

/* pagina plastica eundem visum et eandem cellam habet quam mundus */
interior vacuum
_sectio_constantes (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_F64_PROXIMUS ((f64)PLASTICA_CELLA_VESTIGII,
        1.0 / (f64)MUNDUS_CELLAE_PER_UNITATEM, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)PLASTICA_SPECIMEN_GYRUS,
        (f64)MUNDUS_GYRUS, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)PLASTICA_SPECIMEN_INCLINATIO,
        (f64)MUNDUS_INCLINATIO, 1.0e-9);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "legere", _sectio_legere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "legere_recusat", _sectio_legere_recusat, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "capsa", _sectio_capsa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "gradus", _sectio_gradus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "coma", _sectio_coma, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusationes", _sectio_recusationes, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "occultatio", _sectio_occultatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pratum", _sectio_pratum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "constantes", _sectio_constantes, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("mundus_locus", SECTIONES);
}
