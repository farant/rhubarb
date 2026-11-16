/* probatio_plastica_motus.c - figura cum motu (plastica_motus.h)
 *
 * cycli: canales soli articulorum tactorum, claves, tituli; positura:
 * rotatio, scala, motus per statua_positura ad claves et in medio,
 * 'ex', "quies"; circulus: sutura clausa, positura ultima tenta,
 * cyclus unius positurae; via: gradus scripti servantur (CLXX,
 * CCCXL), gyrus plenus per suturam; momenta; sine motu;
 * recusationes cum linea; fines numerorum; determinata; speculum:
 * imago vera, par permutatum, imago imaginis; ossa: arbor composita,
 * motus radicis multiplicatus, cardo motus; substitutio positurae et
 * cycli; recusationes speculi et ossium cum plagula.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "stereometria.h"
#include "statua.h"
#include "plastica.h"
#include "plastica_motus.h"
#include "stml.h"
#include "internamentum.h"
#include "credo.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define NODI_MAXIMI  CCLVI

/* nodi: 0 radix, 1 corpus, 2 capsa, 3 bracchium, 4 manus, 5 caput,
 * 6 capsa */
#define PUPA_CORPUS \
    "<figura titulus=\"pupa\">\n" \
    "  <articulus titulus=\"corpus\" locus=\"0 1 0\">\n" \
    "    <capsa magnitudo=\"1 1 1\"/>\n" \
    "    <articulus titulus=\"bracchium\" locus=\"0.5 0.5 0\">\n" \
    "      <capsa titulus=\"manus\" magnitudo=\"1 0.2 0.2\"" \
    " locus=\"0.5 0 0\"/>\n" \
    "    </articulus>\n" \
    "    <articulus titulus=\"caput\" locus=\"0 1 0\">\n" \
    "      <capsa magnitudo=\"0.4 0.4 0.4\"/>\n" \
    "    </articulus>\n" \
    "  </articulus>\n"

hic_manens constans character* PUPA =
    PUPA_CORPUS
    "  <positura titulus=\"sursum\">\n"
    "    <art titulus=\"bracchium\" rotatio=\"0 0 90\"/>\n"
    "  </positura>\n"
    "  <positura titulus=\"alta\" ex=\"sursum\">\n"
    "    <art titulus=\"corpus\" motus=\"0 0.5 0\" scala=\"1 2 1\"/>\n"
    "  </positura>\n"
    "  <cyclus titulus=\"walk\" duratio=\"1\" circulus=\"verum\">\n"
    "    <clavis tempus=\"0\" positura=\"quies\"/>\n"
    "    <clavis tempus=\"0.5\" positura=\"sursum\"/>\n"
    "    <momentum titulus=\"ictus\" tempus=\"0.5\"/>\n"
    "  </cyclus>\n"
    "  <cyclus titulus=\"idle\" duratio=\"2\">\n"
    "    <clavis tempus=\"0\" positura=\"quies\"/>\n"
    "    <clavis tempus=\"1\" positura=\"alta\"/>\n"
    "  </cyclus>\n"
    "</figura>\n";

interior PlasticaFiguraFructus
_lege (
        CredoContextus* c,
    constans character* literae)
{
    redde plastica_figura_legere(
        chorda_ex_literis(literae, c->piscina), ZEPHYRUM, c->piscina);
}

/* punctum spatii nodi in spatio statuae, cyclo et tempore datis */
interior StereometriaVector
_punctum (
    constans Statua* s,
                s32  cyclus,
                f32  tempus,
                i32  nodus,
                f32  x,
                f32  y,
                f32  z)
{
    StereometriaMatrix m[NODI_MAXIMI];

    si (s->nodorum > NODI_MAXIMI)
    {
        redde stereometria_vector(-999.0f, -999.0f, -999.0f);
    }
    statua_positura(s, cyclus, tempus, m);
    redde stereometria_punctum_transformare(m[nodus],
        stereometria_vector(x, y, z), NIHIL);
}

#define PUNCTUM_EST(v, px, py, pz) \
    fac { \
        CREDO_F64_PROXIMUS ((f64)(v).x, (px), 1.0e-4); \
        CREDO_F64_PROXIMUS ((f64)(v).y, (py), 1.0e-4); \
        CREDO_F64_PROXIMUS ((f64)(v).z, (pz), 1.0e-4); \
    } dum (FALSUM)

/* canalis nodi et proprietatis in cyclo; NIHIL si nullus */
interior constans StatuaCanalis*
_canalis (
    constans StatuaCyclus* cyclus,
                      s32  nodus,
         StatuaProprietas  proprietas)
{
    i32 k;

    per (k = ZEPHYRUM; k < cyclus->canalium; k++)
    {
        si (   cyclus->canales[k].nodus      == nodus
            && cyclus->canales[k].proprietas == proprietas)
        {
            redde &cyclus->canales[k];
        }
    }
    redde NIHIL;
}

interior vacuum
_sectio_cycli (
    CredoContextus* c)
{
     PlasticaFiguraFructus  f = _lege(c, PUPA);
           constans Statua* s;
    constans StatuaCanalis* canalis;

    CREDO_NECESSE (f.successus);
    s = f.statua;
    CREDO_AEQUALIS_I32 (s->nodorum, VII);
    CREDO_AEQUALIS_I32 (s->cyclorum, II);
    CREDO_CHORDA_AEQUALIS_LITERIS (s->cycli[ZEPHYRUM].titulus, "walk");
    CREDO_CHORDA_AEQUALIS_LITERIS (s->cycli[I].titulus, "idle");
    CREDO_F64_PROXIMUS ((f64)s->cycli[ZEPHYRUM].duratio, 1.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)s->cycli[I].duratio, 2.0, 1.0e-6);
    /* tabulae plasticae transeunt */
    CREDO_AEQUALIS_I32 (f.articulorum, III);
    CREDO_NECESSE (f.articuli != NIHIL);
    CREDO_AEQUALIS_I32 (f.articuli[ZEPHYRUM], I);
    CREDO_AEQUALIS_I32 (f.articuli[I], III);
    CREDO_AEQUALIS_I32 (f.articuli[II], V);
    CREDO_CHORDA_AEQUALIS_LITERIS (f.partes[III], "bracchium");
    CREDO_CHORDA_AEQUALIS_LITERIS (f.partes[IV], "manus");
    /* walk: sola rotatio bracchii; claves 0, 0.5 et sutura ad 1 */
    CREDO_AEQUALIS_I32 (s->cycli[ZEPHYRUM].canalium, I);
    canalis = _canalis(&s->cycli[ZEPHYRUM], III, STATUA_ROTATIO);
    CREDO_NECESSE (canalis != NIHIL);
    CREDO_FALSUM (canalis->gradatim);
    CREDO_AEQUALIS_I32 (canalis->clavium, III);
    CREDO_F64_PROXIMUS ((f64)canalis->tempora[ZEPHYRUM], 0.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)canalis->tempora[I], 0.5, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)canalis->tempora[II], 1.0, 1.0e-6);
    /* quies: quaternio identitas; sursum: 90 circa z */
    CREDO_F64_PROXIMUS ((f64)canalis->valores[III], 1.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)canalis->valores[IV + II],
        0.70710678, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)canalis->valores[IV + III],
        0.70710678, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)canalis->valores[VIII + III], 1.0,
        1.0e-6);
    /* idle: rotatio bracchii (ex 'sursum'), scala et motus corporis;
     * claves 0, 1 et positura ultima tenta ad 2 */
    CREDO_AEQUALIS_I32 (s->cycli[I].canalium, III);
    canalis = _canalis(&s->cycli[I], III, STATUA_ROTATIO);
    CREDO_NECESSE (canalis != NIHIL);
    CREDO_AEQUALIS_I32 (canalis->clavium, III);
    CREDO_F64_PROXIMUS ((f64)canalis->tempora[II], 2.0, 1.0e-6);
    canalis = _canalis(&s->cycli[I], I, STATUA_SCALA);
    CREDO_NECESSE (canalis != NIHIL);
    CREDO_AEQUALIS_I32 (canalis->clavium, III);
    /* quies positura scalam non tangit: I in omni axe */
    CREDO_F64_PROXIMUS ((f64)canalis->valores[ZEPHYRUM], 1.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)canalis->valores[I], 1.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)canalis->valores[II], 1.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)canalis->valores[III + I], 2.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)canalis->valores[VI + I], 2.0, 1.0e-6);
    canalis = _canalis(&s->cycli[I], I, STATUA_TRANSLATIO);
    CREDO_NECESSE (canalis != NIHIL);
    CREDO_F64_PROXIMUS ((f64)canalis->valores[I], 1.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)canalis->valores[III + I], 1.5, 1.0e-6);
    /* caput intactum: canalis nullus in ullo cyclo; corpus in walk
     * intactum */
    CREDO_NIHIL (_canalis(&s->cycli[ZEPHYRUM], V, STATUA_ROTATIO));
    CREDO_NIHIL (_canalis(&s->cycli[I], V, STATUA_ROTATIO));
    CREDO_NIHIL (_canalis(&s->cycli[I], V, STATUA_SCALA));
    CREDO_NIHIL (_canalis(&s->cycli[ZEPHYRUM], I, STATUA_SCALA));
    CREDO_NIHIL (_canalis(&s->cycli[ZEPHYRUM], I, STATUA_TRANSLATIO));
    CREDO_NIHIL (_canalis(&s->cycli[I], III, STATUA_SCALA));
}

interior vacuum
_sectio_positura (
    CredoContextus* c)
{
    PlasticaFiguraFructus  f = _lege(c, PUPA);
          constans Statua* s;
       StereometriaVector  v;

    CREDO_NECESSE (f.successus);
    s = f.statua;
    /* quies: cardo bracchii ad (0.5 1.5 0), punctum 1 0 0 eius */
    v = _punctum(s, -I, 0.0f, III, 1.0f, 0.0f, 0.0f);
    PUNCTUM_EST (v, 1.5, 1.5, 0.0);
    /* walk: ad clavem 90 circa z (+x in +y), in medio 45 */
    v = _punctum(s, ZEPHYRUM, 0.0f, III, 1.0f, 0.0f, 0.0f);
    PUNCTUM_EST (v, 1.5, 1.5, 0.0);
    v = _punctum(s, ZEPHYRUM, 0.5f, III, 1.0f, 0.0f, 0.0f);
    PUNCTUM_EST (v, 0.5, 2.5, 0.0);
    v = _punctum(s, ZEPHYRUM, 0.25f, III, 1.0f, 0.0f, 0.0f);
    PUNCTUM_EST (v, 1.2071068, 2.2071068, 0.0);
    /* corpus in walk non movetur */
    v = _punctum(s, ZEPHYRUM, 0.5f, I, 0.0f, 1.0f, 0.0f);
    PUNCTUM_EST (v, 0.0, 2.0, 0.0);
    /* idle ad clavem 'alta': motus 0.5 sursum, scala y duplex */
    v = _punctum(s, I, 1.0f, I, 0.0f, 0.0f, 0.0f);
    PUNCTUM_EST (v, 0.0, 1.5, 0.0);
    v = _punctum(s, I, 1.0f, I, 0.0f, 1.0f, 0.0f);
    PUNCTUM_EST (v, 0.0, 3.5, 0.0);
    /* in medio: motus 0.25, scala 1.5 */
    v = _punctum(s, I, 0.5f, I, 0.0f, 1.0f, 0.0f);
    PUNCTUM_EST (v, 0.0, 2.75, 0.0);
    /* 'ex': alta rotationem bracchii ex 'sursum' tenet; scala
     * parentis cardinem et punctum fert */
    v = _punctum(s, I, 1.0f, III, 1.0f, 0.0f, 0.0f);
    PUNCTUM_EST (v, 0.5, 4.5, 0.0);

    /* scala numero uno; 'ex' valores articuli substituit, ceteros
     * servat; rotatio circa x, deinde y, deinde z */
    f = _lege(c,
        PUPA_CORPUS
        "<positura titulus=\"magna\">\n"
        "<art titulus=\"corpus\" scala=\"2\" motus=\"1 0 0.5\"/>\n"
        "</positura>\n"
        "<positura titulus=\"maior\" ex=\"magna\">\n"
        "<art titulus=\"corpus\" scala=\"1 3 1\"/>\n"
        "</positura>\n"
        "<positura titulus=\"versa\">\n"
        "<art titulus=\"bracchium\" rotatio=\"90 90 0\"/>\n"
        "</positura>\n"
        "<cyclus titulus=\"a\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"magna\"/>\n"
        "</cyclus>\n"
        "<cyclus titulus=\"b\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"maior\"/>\n"
        "</cyclus>\n"
        "<cyclus titulus=\"d\" duratio=\"1\" circulus=\"falsum\">\n"
        "<clavis tempus=\"0\" positura=\"versa\"/>\n"
        "</cyclus>\n"
        "<positura titulus=\"torta\">\n"
        "<art titulus=\"bracchium\" rotatio=\"0 90 90\"/>\n"
        "</positura>\n"
        "<cyclus titulus=\"e\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"torta\"/>\n"
        "</cyclus>\n"
        "</figura>");
    CREDO_NECESSE (f.successus);
    s = f.statua;
    v = _punctum(s, ZEPHYRUM, 0.0f, I, 1.0f, 1.0f, 1.0f);
    PUNCTUM_EST (v, 3.0, 3.0, 2.5);
    v = _punctum(s, I, 0.0f, I, 1.0f, 1.0f, 1.0f);
    PUNCTUM_EST (v, 2.0, 4.0, 1.5);
    /* 0 0 1 circa x 90 fit 0 -1 0; circa y 90 manet */
    v = _punctum(s, II, 0.0f, III, 0.0f, 0.0f, 1.0f);
    PUNCTUM_EST (v, 0.5, 0.5, 0.0);
    /* 0 1 0 circa x 90 fit 0 0 1; circa y 90 fit 1 0 0 */
    v = _punctum(s, II, 0.0f, III, 0.0f, 1.0f, 0.0f);
    PUNCTUM_EST (v, 1.5, 1.5, 0.0);
    /* y ante z: 1 0 0 circa y 90 fit 0 0 -1; circa z 90 manet */
    v = _punctum(s, III, 0.0f, III, 1.0f, 0.0f, 0.0f);
    PUNCTUM_EST (v, 0.5, 1.5, -1.0);

    /* rotatio ad quietem additur: articulus gyro 90 quietis */
    f = _lege(c,
        "<figura titulus=\"a\">\n"
        "<articulus titulus=\"rota\" gyrus=\"90\">\n"
        "<capsa magnitudo=\"1 1 1\"/>\n"
        "</articulus>\n"
        "<positura titulus=\"p\">\n"
        "<art titulus=\"rota\" rotatio=\"0 0 90\"/>\n"
        "</positura>\n"
        "<cyclus titulus=\"a\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"p\"/>\n"
        "</cyclus>\n"
        "</figura>");
    CREDO_NECESSE (f.successus);
    /* in spatio articuli 1 0 0 fit 0 1 0; gyrus quietis y servat */
    v = _punctum(f.statua, ZEPHYRUM, 0.0f, I, 1.0f, 0.0f, 0.0f);
    PUNCTUM_EST (v, 0.0, 1.0, 0.0);
    v = _punctum(f.statua, -I, 0.0f, I, 1.0f, 0.0f, 0.0f);
    PUNCTUM_EST (v, 0.0, 0.0, -1.0);
}

interior vacuum
_sectio_circulus (
    CredoContextus* c)
{
     PlasticaFiguraFructus  f = _lege(c, PUPA);
           constans Statua* s;
        StereometriaVector  v;
    constans StatuaCanalis* canalis;

    CREDO_NECESSE (f.successus);
    s = f.statua;
    /* circulus: post clavem ultimam ad primam redit */
    v = _punctum(s, ZEPHYRUM, 0.75f, III, 1.0f, 0.0f, 0.0f);
    PUNCTUM_EST (v, 1.2071068, 2.2071068, 0.0);
    v = _punctum(s, ZEPHYRUM, 0.999f, III, 1.0f, 0.0f, 0.0f);
    CREDO_F64_PROXIMUS ((f64)v.x, 1.5, 1.0e-2);
    /* non circulus: positura ultima tenetur usque ad durationem */
    v = _punctum(s, I, 1.5f, I, 0.0f, 1.0f, 0.0f);
    PUNCTUM_EST (v, 0.0, 3.5, 0.0);
    v = _punctum(s, I, 1.999f, I, 0.0f, 1.0f, 0.0f);
    PUNCTUM_EST (v, 0.0, 3.5, 0.0);

    /* cyclus unius positurae: stat; clavis ultima AD durationem
     * nihil addit */
    f = _lege(c,
        PUPA_CORPUS
        "<positura titulus=\"sursum\">\n"
        "<art titulus=\"bracchium\" rotatio=\"0 0 90\"/>\n"
        "</positura>\n"
        "<cyclus titulus=\"stat\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"sursum\"/>\n"
        "</cyclus>\n"
        "<cyclus titulus=\"gyrat\" duratio=\"1\" circulus=\"verum\">\n"
        "<clavis tempus=\"0\" positura=\"sursum\"/>\n"
        "</cyclus>\n"
        "<cyclus titulus=\"plenus\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"quies\"/>\n"
        "<clavis tempus=\"1\" positura=\"sursum\"/>\n"
        "</cyclus>\n"
        "</figura>");
    CREDO_NECESSE (f.successus);
    s = f.statua;
    CREDO_AEQUALIS_I32 (s->cyclorum, III);
    canalis = _canalis(&s->cycli[ZEPHYRUM], III, STATUA_ROTATIO);
    CREDO_NECESSE (canalis != NIHIL);
    CREDO_AEQUALIS_I32 (canalis->clavium, II);
    CREDO_F64_PROXIMUS ((f64)s->cycli[ZEPHYRUM].duratio, 1.0, 1.0e-6);
    v = _punctum(s, ZEPHYRUM, 0.7f, III, 1.0f, 0.0f, 0.0f);
    PUNCTUM_EST (v, 0.5, 2.5, 0.0);
    canalis = _canalis(&s->cycli[I], III, STATUA_ROTATIO);
    CREDO_NECESSE (canalis != NIHIL);
    CREDO_AEQUALIS_I32 (canalis->clavium, II);
    v = _punctum(s, I, 0.7f, III, 1.0f, 0.0f, 0.0f);
    PUNCTUM_EST (v, 0.5, 2.5, 0.0);
    canalis = _canalis(&s->cycli[II], III, STATUA_ROTATIO);
    CREDO_NECESSE (canalis != NIHIL);
    CREDO_AEQUALIS_I32 (canalis->clavium, II);
    CREDO_F64_PROXIMUS ((f64)canalis->tempora[I], 1.0, 1.0e-6);
}

#define ROTA_CORPUS \
    "<figura titulus=\"a\">\n" \
    "<articulus titulus=\"rota\">\n" \
    "<capsa magnitudo=\"1 1 1\"/>\n" \
    "</articulus>\n"

interior vacuum
_sectio_via (
    CredoContextus* c)
{
    /* gradus scripti servantur: 170 deinde 340 per 255 it, non
     * retro per 75 */
    PlasticaFiguraFructus f = _lege(c,
        ROTA_CORPUS
        "<positura titulus=\"a\">"
        "<art titulus=\"rota\" rotatio=\"0 0 170\"/></positura>\n"
        "<positura titulus=\"b\">"
        "<art titulus=\"rota\" rotatio=\"0 0 340\"/></positura>\n"
        "<cyclus titulus=\"a\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"quies\"/>\n"
        "<clavis tempus=\"0.25\" positura=\"a\"/>\n"
        "<clavis tempus=\"0.5\" positura=\"b\"/>\n"
        "</cyclus>\n"
        "</figura>");
       StereometriaVector v;

    CREDO_NECESSE (f.successus);
    v = _punctum(f.statua, ZEPHYRUM, 0.375f, I, 1.0f, 0.0f, 0.0f);
    PUNCTUM_EST (v, -0.2588190, -0.9659258, 0.0);
    v = _punctum(f.statua, ZEPHYRUM, 0.125f, I, 1.0f, 0.0f, 0.0f);
    PUNCTUM_EST (v, 0.0871557, 0.9961947, 0.0);
    /* 179 gradus inter claves licent */
    f = _lege(c,
        ROTA_CORPUS
        "<positura titulus=\"a\">"
        "<art titulus=\"rota\" rotatio=\"0 0 179\"/></positura>\n"
        "<cyclus titulus=\"a\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"quies\"/>\n"
        "<clavis tempus=\"0.5\" positura=\"a\"/>\n"
        "</cyclus>\n"
        "</figura>");
    CREDO_VERUM (f.successus);
    /* gyrus plenus: 0, 120, 240 et sutura modulo 360 ad 0 pergit
     * (300 ad tempus 0.75), non retro */
    f = _lege(c,
        ROTA_CORPUS
        "<positura titulus=\"a\">"
        "<art titulus=\"rota\" rotatio=\"0 0 120\"/></positura>\n"
        "<positura titulus=\"b\">"
        "<art titulus=\"rota\" rotatio=\"0 0 240\"/></positura>\n"
        "<cyclus titulus=\"a\" duratio=\"0.9\" circulus=\"verum\">\n"
        "<clavis tempus=\"0\" positura=\"quies\"/>\n"
        "<clavis tempus=\"0.3\" positura=\"a\"/>\n"
        "<clavis tempus=\"0.6\" positura=\"b\"/>\n"
        "</cyclus>\n"
        "</figura>");
    CREDO_NECESSE (f.successus);
    v = _punctum(f.statua, ZEPHYRUM, 0.75f, I, 1.0f, 0.0f, 0.0f);
    PUNCTUM_EST (v, 0.5, -0.8660254, 0.0);
    v = _punctum(f.statua, ZEPHYRUM, 0.45f, I, 1.0f, 0.0f, 0.0f);
    PUNCTUM_EST (v, -1.0, 0.0, 0.0);
}

interior vacuum
_sectio_momenta (
    CredoContextus* c)
{
    PlasticaFiguraFructus f = _lege(c, PUPA);

    CREDO_NECESSE (f.successus);
    CREDO_AEQUALIS_I32 (f.momentorum, I);
    CREDO_NECESSE (f.momenta != NIHIL);
    CREDO_AEQUALIS_I32 (f.momenta[ZEPHYRUM].cyclus, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (f.momenta[ZEPHYRUM].titulus,
        "ictus");
    CREDO_F64_PROXIMUS ((f64)f.momenta[ZEPHYRUM].tempus, 0.5, 1.0e-6);

    /* plura, ordine plagulae; fines 0 et duratio licent; titulus
     * idem in cyclo alio licet */
    f = _lege(c,
        ROTA_CORPUS
        "<cyclus titulus=\"a\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"quies\"/>\n"
        "<momentum titulus=\"initium\" tempus=\"0\"/>\n"
        "</cyclus>\n"
        "<cyclus titulus=\"b\" duratio=\"2\">\n"
        "<momentum titulus=\"initium\" tempus=\"0.25\"/>\n"
        "<clavis tempus=\"0\" positura=\"quies\"/>\n"
        "<momentum titulus=\"finis\" tempus=\"2\"/>\n"
        "</cyclus>\n"
        "</figura>");
    CREDO_NECESSE (f.successus);
    CREDO_AEQUALIS_I32 (f.momentorum, III);
    CREDO_AEQUALIS_I32 (f.momenta[ZEPHYRUM].cyclus, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (f.momenta[I].cyclus, I);
    CREDO_AEQUALIS_I32 (f.momenta[II].cyclus, I);
    CREDO_CHORDA_AEQUALIS_LITERIS (f.momenta[I].titulus, "initium");
    CREDO_CHORDA_AEQUALIS_LITERIS (f.momenta[II].titulus, "finis");
    CREDO_F64_PROXIMUS ((f64)f.momenta[I].tempus, 0.25, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)f.momenta[II].tempus, 2.0, 1.0e-6);
    /* cyclus sine canalibus (quies sola) durationem tenet */
    CREDO_AEQUALIS_I32 (f.statua->cyclorum, II);
    CREDO_AEQUALIS_I32 (f.statua->cycli[I].canalium, ZEPHYRUM);
}

interior vacuum
_sectio_sine_motu (
    CredoContextus* c)
{
    PlasticaFiguraFructus f = _lege(c,
        ROTA_CORPUS "</figura>");

    CREDO_NECESSE (f.successus);
    CREDO_AEQUALIS_I32 (f.statua->cyclorum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (f.momentorum, ZEPHYRUM);
    CREDO_NIHIL (f.momenta);
    CREDO_AEQUALIS_I32 (f.articulorum, I);
    /* positurae sine cyclo: nihil in statua */
    f = _lege(c,
        ROTA_CORPUS
        "<positura titulus=\"a\">"
        "<art titulus=\"rota\" rotatio=\"0 0 20\"/></positura>\n"
        "</figura>");
    CREDO_NECESSE (f.successus);
    CREDO_AEQUALIS_I32 (f.statua->cyclorum, ZEPHYRUM);
    /* figura sine articulis */
    f = _lege(c,
        "<figura titulus=\"a\"><capsa magnitudo=\"1 1 1\"/></figura>");
    CREDO_NECESSE (f.successus);
    CREDO_AEQUALIS_I32 (f.articulorum, ZEPHYRUM);
    /* semen transit */
    f = plastica_figura_legere(chorda_ex_literis(PUPA, c->piscina),
        VII, c->piscina);
    CREDO_VERUM (f.successus);
    f = plastica_figura_legere(chorda_ex_literis(PUPA, c->piscina),
        M * M + I, c->piscina);
    CREDO_FALSUM (f.successus);
    CREDO_NIHIL (f.statua);
}

nomen structura {
    constans character* textus;
                   i32  linea;
    constans character* fragmentum;
} Recusatio;

/* corpus lineas I-V tenet; sequentia a linea VI */
#define CAPUT \
    "<figura titulus=\"a\">\n" \
    "<articulus titulus=\"corpus\">\n" \
    "<capsa magnitudo=\"1 1 1\"/>\n" \
    "<articulus titulus=\"manus\"><capsa magnitudo=\"1 1 1\"/>" \
    "</articulus>\n" \
    "</articulus>\n"
#define CAUDA "</figura>"
#define QUIES_CLAVIS "<clavis tempus=\"0\" positura=\"quies\"/>\n"

interior vacuum
_sectio_recusationes (
    CredoContextus* c)
{
    hic_manens constans Recusatio R[] = {
        /* radix; recusationes plasticae transeunt */
        { "<res titulus=\"a\"><capsa magnitudo=\"1 1 1\"/></res>", I,
          "<figura>" },
        { "<figura titulus=\"a\">\n<pila/>\n</figura>", II, "pila" },
        { "<figura titulus=\"a\"><capsa", ZEPHYRUM, "STML" },
        /* art */
        { CAPUT "<positura titulus=\"p\">\n"
          "<art titulus=\"genu\" rotatio=\"0 0 1\"/>\n</positura>\n"
          CAUDA, VII, "unknown joint 'genu'" },
        { CAPUT "<positura titulus=\"p\">\n"
          "<art rotatio=\"0 0 1\"/>\n</positura>\n" CAUDA, VII,
          "titulus" },
        { CAPUT "<positura titulus=\"p\">\n"
          "<art titulus=\"manus\" rotatio=\"0 0 1\"/>\n"
          "<art titulus=\"manus\" scala=\"2\"/>\n</positura>\n" CAUDA,
          VIII, "twice" },
        { CAPUT "<positura titulus=\"p\">\n"
          "<art titulus=\"manus\"/>\n</positura>\n" CAUDA, VII,
          "at least one" },
        { CAPUT "<positura titulus=\"p\">\n"
          "<art titulus=\"manus\" motus=\"0 1 0\"/>\n</positura>\n"
          CAUDA, VII, "motus" },
        { CAPUT "<positura titulus=\"p\">\n"
          "<art titulus=\"corpus\" motus=\"0 1\"/>\n</positura>\n"
          CAUDA, VII, "motus" },
        { CAPUT "<positura titulus=\"p\">\n"
          "<art titulus=\"manus\" scala=\"0\"/>\n</positura>\n" CAUDA,
          VII, "scala" },
        { CAPUT "<positura titulus=\"p\">\n"
          "<art titulus=\"manus\" scala=\"1 -1 1\"/>\n</positura>\n"
          CAUDA, VII, "scala" },
        { CAPUT "<positura titulus=\"p\">\n"
          "<art titulus=\"manus\" scala=\"-1 1 1\"/>\n</positura>\n"
          CAUDA, VII, "scala" },
        { CAPUT "<positura titulus=\"p\">\n"
          "<art titulus=\"manus\" scala=\"1 1 -1\"/>\n</positura>\n"
          CAUDA, VII, "scala" },
        { CAPUT "<positura titulus=\"p\">\n"
          "<art titulus=\"manus\" rotatio=\"1 2 3 4\"/>\n"
          "</positura>\n" CAUDA, VII, "rotatio" },
        { CAPUT "<positura titulus=\"p\">\n"
          "<art titulus=\"manus\" rotatio=\"0 0 2000000\"/>\n"
          "</positura>\n" CAUDA, VII, "rotatio" },
        { CAPUT "<positura titulus=\"p\">\n"
          "<art titulus=\"manus\" rotatio=\"0 0 -2000000\"/>\n"
          "</positura>\n" CAUDA, VII, "rotatio" },
        { CAPUT "<positura titulus=\"p\">\n"
          "<art titulus=\"manus\" scala=\"1 1\"/>\n</positura>\n"
          CAUDA, VII, "scala" },
        { CAPUT "<positura titulus=\"p\">\n"
          "<art titulus=\"manus\" rotatio=\"1 2\"/>\n</positura>\n"
          CAUDA, VII, "rotatio" },
        { CAPUT "<positura titulus=\"p\">\n"
          "<art titulus=\"manus\" rotatio=\"1 nan 2\"/>\n"
          "</positura>\n" CAUDA, VII, "rotatio" },
        { CAPUT "<positura titulus=\"p\">\n"
          "<art titulus=\"manus\" color=\"#fff\"/>\n</positura>\n"
          CAUDA, VII, "color" },
        { CAPUT "<positura titulus=\"p\">\n<art titulus=\"manus\""
          " rotatio=\"0 0 1\"><capsa/></art>\n</positura>\n" CAUDA,
          VII, "<art>" },
        /* positura */
        { CAPUT "<positura>\n</positura>\n" CAUDA, VI, "titulus" },
        { CAPUT "<positura titulus=\"p\"/>\n"
          "<positura titulus=\"p\"/>\n" CAUDA, VII, "twice" },
        { CAPUT "<positura titulus=\"quies\"/>\n" CAUDA, VI,
          "built in" },
        { CAPUT "<positura titulus=\"p\" ex=\"nulla\"/>\n" CAUDA, VI,
          "'ex'" },
        { CAPUT "<positura titulus=\"p\" ex=\"q\"/>\n"
          "<positura titulus=\"q\"/>\n" CAUDA, VI, "'ex'" },
        { CAPUT "<positura titulus=\"p\" ex=\"p\"/>\n" CAUDA, VI,
          "'ex'" },
        { CAPUT "<positura titulus=\"p\" gyrus=\"1\"/>\n" CAUDA, VI,
          "gyrus" },
        { CAPUT "<positura titulus=\"p\">\n<os/>\n</positura>\n"
          CAUDA, VII, "holds only <art>, got <os>" },
        { CAPUT "<positura titulus=\"p\">\nverba\n</positura>\n"
          CAUDA, VI, "text" },
        /* cyclus */
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\"/>\n" CAUDA, VI,
          "no keys" },
        { CAPUT "<cyclus duratio=\"1\">\n" QUIES_CLAVIS "</cyclus>\n"
          CAUDA, VI, "titulus" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n" QUIES_CLAVIS
          "</cyclus>\n<cyclus titulus=\"c\" duratio=\"1\">\n"
          QUIES_CLAVIS "</cyclus>\n" CAUDA, IX, "twice" },
        { CAPUT "<cyclus titulus=\"c\">\n" QUIES_CLAVIS "</cyclus>\n"
          CAUDA, VI, "duratio" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"0\">\n" QUIES_CLAVIS
          "</cyclus>\n" CAUDA, VI, "duratio" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"-1\">\n" QUIES_CLAVIS
          "</cyclus>\n" CAUDA, VI, "duratio" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"60.5\">\n"
          QUIES_CLAVIS "</cyclus>\n" CAUDA, VI, "duratio" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"x\">\n" QUIES_CLAVIS
          "</cyclus>\n" CAUDA, VI, "duratio" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\""
          " circulus=\"fortasse\">\n" QUIES_CLAVIS "</cyclus>\n"
          CAUDA, VI, "circulus" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\" gyrus=\"1\">\n"
          QUIES_CLAVIS "</cyclus>\n" CAUDA, VI, "gyrus" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n" QUIES_CLAVIS
          "<art titulus=\"manus\"/>\n</cyclus>\n" CAUDA, VIII,
          "holds only" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n"
          "<clavis tempus=\"0\" positura=\"quies\"><capsa/>"
          "</clavis>\n</cyclus>\n" CAUDA, VII, "<clavis>" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n" QUIES_CLAVIS
          "<momentum titulus=\"m\" tempus=\"0.5\"><capsa/>"
          "</momentum>\n</cyclus>\n" CAUDA, VIII, "<momentum>" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n" QUIES_CLAVIS
          "<momentum titulus=\"m\" tempus=\"0.5\" vis=\"2\"/>\n"
          "</cyclus>\n" CAUDA, VIII, "vis" },
        /* claves */
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n"
          "<clavis tempus=\"0.1\" positura=\"quies\"/>\n</cyclus>\n"
          CAUDA, VII, "first key" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n" QUIES_CLAVIS
          "<clavis tempus=\"0.5\" positura=\"quies\"/>\n"
          "<clavis tempus=\"0.5\" positura=\"quies\"/>\n</cyclus>\n"
          CAUDA, IX, "increas" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n" QUIES_CLAVIS
          "<clavis tempus=\"0.5\" positura=\"quies\"/>\n"
          "<clavis tempus=\"0.4\" positura=\"quies\"/>\n</cyclus>\n"
          CAUDA, IX, "increas" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n" QUIES_CLAVIS
          "<clavis tempus=\"1.5\" positura=\"quies\"/>\n</cyclus>\n"
          CAUDA, VIII, "duratio" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\""
          " circulus=\"verum\">\n" QUIES_CLAVIS
          "<clavis tempus=\"1\" positura=\"quies\"/>\n</cyclus>\n"
          CAUDA, VIII, "before" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n"
          "<clavis tempus=\"0\" positura=\"nulla\"/>\n</cyclus>\n"
          CAUDA, VII, "unknown pose 'nulla'" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n"
          "<clavis tempus=\"0\"/>\n</cyclus>\n" CAUDA, VII,
          "positura" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n"
          "<clavis positura=\"quies\"/>\n</cyclus>\n" CAUDA, VII,
          "tempus" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n"
          "<clavis tempus=\"-1\" positura=\"quies\"/>\n</cyclus>\n"
          CAUDA, VII, "first key" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n"
          "<clavis tempus=\"0\" positura=\"quies\" lenitas=\"x\"/>\n"
          "</cyclus>\n" CAUDA, VII, "lenitas" },
        /* CLXXX gradus inter claves vicinas */
        { CAPUT "<positura titulus=\"p\">"
          "<art titulus=\"manus\" rotatio=\"0 0 180\"/></positura>\n"
          "<cyclus titulus=\"c\" duratio=\"1\">\n" QUIES_CLAVIS
          "<clavis tempus=\"0.5\" positura=\"p\"/>\n</cyclus>\n"
          CAUDA, IX, "add a key between" },
        { CAPUT "<positura titulus=\"p\">"
          "<art titulus=\"manus\" rotatio=\"0 0 -180\"/></positura>\n"
          "<cyclus titulus=\"c\" duratio=\"1\">\n" QUIES_CLAVIS
          "<clavis tempus=\"0.5\" positura=\"p\"/>\n</cyclus>\n"
          CAUDA, IX, "add a key between" },
        { CAPUT "<positura titulus=\"p\">"
          "<art titulus=\"manus\" rotatio=\"-90 0 0\"/></positura>\n"
          "<positura titulus=\"q\">"
          "<art titulus=\"manus\" rotatio=\"100 0 0\"/></positura>\n"
          "<cyclus titulus=\"c\" duratio=\"1\">\n"
          "<clavis tempus=\"0\" positura=\"p\"/>\n"
          "<clavis tempus=\"0.5\" positura=\"q\"/>\n</cyclus>\n"
          CAUDA, X, "'manus'" },
        /* sutura circuli: 180 modulo 360 ambigua */
        { CAPUT "<positura titulus=\"p\">"
          "<art titulus=\"manus\" rotatio=\"0 90 0\"/></positura>\n"
          "<positura titulus=\"q\">"
          "<art titulus=\"manus\" rotatio=\"0 180 0\"/></positura>\n"
          "<cyclus titulus=\"c\" duratio=\"1\" circulus=\"verum\">\n"
          QUIES_CLAVIS
          "<clavis tempus=\"0.3\" positura=\"p\"/>\n"
          "<clavis tempus=\"0.6\" positura=\"q\"/>\n</cyclus>\n"
          CAUDA, VIII, "add a key between" },
        /* momentum */
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n" QUIES_CLAVIS
          "<momentum titulus=\"m\" tempus=\"1.5\"/>\n</cyclus>\n"
          CAUDA, VIII, "tempus" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n" QUIES_CLAVIS
          "<momentum titulus=\"m\" tempus=\"-0.1\"/>\n</cyclus>\n"
          CAUDA, VIII, "tempus" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n" QUIES_CLAVIS
          "<momentum tempus=\"0.5\"/>\n</cyclus>\n" CAUDA, VIII,
          "titulus" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n" QUIES_CLAVIS
          "<momentum titulus=\"m\"/>\n</cyclus>\n" CAUDA, VIII,
          "tempus" },
        { CAPUT "<cyclus titulus=\"c\" duratio=\"1\">\n" QUIES_CLAVIS
          "<momentum titulus=\"m\" tempus=\"0.5\"/>\n"
          "<momentum titulus=\"m\" tempus=\"0.6\"/>\n</cyclus>\n"
          CAUDA, IX, "twice" },
        /* membrum nondum */
        { CAPUT "<membrum titulus=\"m\"/>\n" CAUDA, VI,
          "not built yet" },
        { NIHIL, ZEPHYRUM, NIHIL }
    };
    i32 k;

    per (k = ZEPHYRUM; R[k].textus; k++)
    {
        PlasticaFiguraFructus f = _lege(c, R[k].textus);

        si (f.successus || (R[k].linea && f.linea != R[k].linea))
        {
            printf("recusatio motus %u (linea %u): %s\n",
                (insignatus)k, (insignatus)f.linea, R[k].textus);
        }
        CREDO_FALSUM (f.successus);
        CREDO_NIHIL (f.statua);
        CREDO_NIHIL (f.partes);
        CREDO_NIHIL (f.articuli);
        CREDO_NIHIL (f.momenta);
        CREDO_AEQUALIS_I32 (f.articulorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (f.momentorum, ZEPHYRUM);
        CREDO_CHORDA_NON_VACUA (f.error);
        si (R[k].linea)
        {
            CREDO_AEQUALIS_I32 (f.linea, R[k].linea);
        }
        CREDO_FALSUM (chorda_continet(f.error,
            chorda_ex_literis("\n", c->piscina)));
        CREDO_CHORDA_CONTINET (f.error,
            chorda_ex_literis(R[k].fragmentum, c->piscina));
    }
}

/* figura cum numero positurarum, cyclorum, clavium et momentorum
 * dato (cyclus primus claves et momenta fert) */
interior PlasticaFiguraFructus
_numerosa (
    CredoContextus* c,
               i32  positurae,
               i32  cycli,
               i32  claves,
               i32  momenta)
{
    character* textus = (character*)piscina_allocare(c->piscina,
        (memoriae_index)(positurae + cycli + claves + momenta)
            * CXXVIII + DXII);
          i32 longitudo = ZEPHYRUM;
          i32 k;
          i32 j;

    longitudo += (i32)sprintf(textus + longitudo, "%s", CAPUT);
    per (k = ZEPHYRUM; k < positurae; k++)
    {
        longitudo += (i32)sprintf(textus + longitudo,
            "<positura titulus=\"p%u\"/>\n", (insignatus)k);
    }
    per (k = ZEPHYRUM; k < cycli; k++)
    {
        longitudo += (i32)sprintf(textus + longitudo,
            "<cyclus titulus=\"c%u\" duratio=\"10\">\n",
            (insignatus)k);
        per (j = ZEPHYRUM; j < (k ? I : claves); j++)
        {
            longitudo += (i32)sprintf(textus + longitudo,
                "<clavis tempus=\"%u.%u\" positura=\"quies\"/>\n",
                (insignatus)(j / X), (insignatus)(j % X));
        }
        per (j = ZEPHYRUM; j < (k ? ZEPHYRUM : momenta); j++)
        {
            longitudo += (i32)sprintf(textus + longitudo,
                "<momentum titulus=\"m%u\" tempus=\"1\"/>\n",
                (insignatus)j);
        }
        longitudo += (i32)sprintf(textus + longitudo, "</cyclus>\n");
    }
    longitudo += (i32)sprintf(textus + longitudo, "%s", CAUDA);
    redde plastica_figura_legere(
        chorda_ex_literis(textus, c->piscina), ZEPHYRUM, c->piscina);
}

interior vacuum
_sectio_fines (
    CredoContextus* c)
{
    PlasticaFiguraFructus f = _numerosa(c,
        PLASTICA_MOTUS_POSITURAE_MAXIMAE,
        PLASTICA_MOTUS_CYCLI_MAXIMI, PLASTICA_MOTUS_CLAVES_MAXIMAE,
        PLASTICA_MOTUS_MOMENTA_MAXIMA);

    CREDO_NECESSE (f.successus);
    CREDO_AEQUALIS_I32 (f.statua->cyclorum,
        PLASTICA_MOTUS_CYCLI_MAXIMI);
    CREDO_AEQUALIS_I32 (f.momentorum, PLASTICA_MOTUS_MOMENTA_MAXIMA);
    /* una plus cuiusque recusatur, linea elementi superflui */
    f = _numerosa(c, PLASTICA_MOTUS_POSITURAE_MAXIMAE + I, I, I,
        ZEPHYRUM);
    CREDO_FALSUM (f.successus);
    CREDO_AEQUALIS_I32 (f.linea, V + LXV);
    CREDO_CHORDA_CONTINET (f.error,
        chorda_ex_literis("64", c->piscina));
    f = _numerosa(c, ZEPHYRUM, PLASTICA_MOTUS_CYCLI_MAXIMI + I, I,
        ZEPHYRUM);
    CREDO_FALSUM (f.successus);
    CREDO_AEQUALIS_I32 (f.linea, V + XVI * III + I);
    CREDO_CHORDA_CONTINET (f.error,
        chorda_ex_literis("16", c->piscina));
    f = _numerosa(c, ZEPHYRUM, I, PLASTICA_MOTUS_CLAVES_MAXIMAE + I,
        ZEPHYRUM);
    CREDO_FALSUM (f.successus);
    CREDO_AEQUALIS_I32 (f.linea, V + I + LXV);
    CREDO_CHORDA_CONTINET (f.error,
        chorda_ex_literis("64", c->piscina));
    f = _numerosa(c, ZEPHYRUM, I, I,
        PLASTICA_MOTUS_MOMENTA_MAXIMA + I);
    CREDO_FALSUM (f.successus);
    CREDO_AEQUALIS_I32 (f.linea, V + I + I + IX);
    CREDO_CHORDA_CONTINET (f.error,
        chorda_ex_literis("8", c->piscina));
}

interior vacuum
_sectio_determinata (
    CredoContextus* c)
{
    PlasticaFiguraFructus a = _lege(c, PUPA);
    PlasticaFiguraFructus b = _lege(c, PUPA);
                      i32 k;
                      i32 j;

    CREDO_NECESSE (a.successus && b.successus);
    CREDO_NECESSE (a.statua != b.statua);
    CREDO_AEQUALIS_I32 (a.statua->cyclorum, b.statua->cyclorum);
    per (k = ZEPHYRUM; k < a.statua->cyclorum; k++)
    {
        constans StatuaCyclus* x = &a.statua->cycli[k];
        constans StatuaCyclus* y = &b.statua->cycli[k];

        CREDO_NECESSE (x->canalium == y->canalium);
        per (j = ZEPHYRUM; j < x->canalium; j++)
        {
            constans StatuaCanalis* p = &x->canales[j];
            constans StatuaCanalis* q = &y->canales[j];
                               i32  lata =
                                   p->proprietas
                                      == STATUA_ROTATIO ? IV : III;

            CREDO_AEQUALIS_S32 (p->nodus, q->nodus);
            CREDO_NECESSE (p->clavium == q->clavium);
            CREDO_NECESSE (p->proprietas == q->proprietas);
            CREDO_AEQUALIS_I32 ((i32)memcmp(p->tempora, q->tempora,
                p->clavium * magnitudo(f32)), ZEPHYRUM);
            CREDO_AEQUALIS_I32 ((i32)memcmp(p->valores, q->valores,
                p->clavium * lata * magnitudo(f32)), ZEPHYRUM);
        }
    }
    /* statua prior intacta: figura plastica sola cyclos non habet */
    CREDO_AEQUALIS_I32 (plastica_legere(
        chorda_ex_literis(PUPA, c->piscina), c->piscina)
            .statua->cyclorum, ZEPHYRUM);
}

/* nodi: 0 radix, 1 corpus, 2 capsa, 3 ala_sin, 4 capsa, 5 ala_dex,
 * 6 capsa, 7 caput, 8 capsa. Lineae I-XIII */
#define GEMINA_CORPUS \
    "<figura titulus=\"gemina\">\n" \
    "  <articulus titulus=\"corpus\" locus=\"0 1 0\">\n" \
    "    <capsa magnitudo=\"1 1 1\"/>\n" \
    "    <articulus titulus=\"ala_sin\" locus=\"0.5 0.5 0\"" \
    " gyrus=\"30\">\n" \
    "      <capsa magnitudo=\"1 0.2 0.2\" locus=\"0.5 0 0\"/>\n" \
    "    </articulus>\n" \
    "    <articulus titulus=\"ala_dex\" locus=\"-0.5 0.5 0\"" \
    " gyrus=\"-30\">\n" \
    "      <capsa magnitudo=\"1 0.2 0.2\" locus=\"-0.5 0 0\"/>\n" \
    "    </articulus>\n" \
    "    <articulus titulus=\"caput\" locus=\"0 1 0\">\n" \
    "      <capsa magnitudo=\"0.4 0.4 0.4\"/>\n" \
    "    </articulus>\n" \
    "  </articulus>\n"

#define CAPSA_I  "<capsa magnitudo=\"1 1 1\"/>"

#define CYCLUS_UNIUS(titulus, positura) \
    "<cyclus titulus=\"" titulus "\" duratio=\"1\">\n" \
    "<clavis tempus=\"0\" positura=\"" positura "\"/>\n" \
    "</cyclus>\n"

hic_manens constans character* GEMINA =
    GEMINA_CORPUS
    "<positura titulus=\"a\">\n"
    "<art titulus=\"ala_sin\" rotatio=\"10 20 30\" scala=\"1 2 1\"/>\n"
    "<art titulus=\"caput\" rotatio=\"5 15 25\"/>\n"
    "<art titulus=\"corpus\" motus=\"0.3 0.2 0.1\"/>\n"
    "</positura>\n"
    "<positura titulus=\"b\" speculum=\"a\"/>\n"
    "<positura titulus=\"c\" speculum=\"b\"/>\n"
    "<positura titulus=\"d\" speculum=\"a\">\n"
    "<art titulus=\"caput\" rotatio=\"0 0 0\"/>\n"
    "<art titulus=\"ala_sin\" scala=\"3\"/>\n"
    "</positura>\n"
    CYCLUS_UNIUS("ca", "a")
    CYCLUS_UNIUS("cb", "b")
    CYCLUS_UNIUS("cc", "c")
    CYCLUS_UNIUS("cd", "d")
    "</figura>\n";

/* canales cycli x in cyclo y iidem (nodus, proprietas, valores) */
interior b32
_cycli_pares (
    constans StatuaCyclus* x,
    constans StatuaCyclus* y)
{
    i32 k;
    i32 j;

    si (x->canalium != y->canalium)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < x->canalium; k++)
    {
        constans StatuaCanalis* p = &x->canales[k];
        constans StatuaCanalis* q =
            _canalis(y, p->nodus, p->proprietas);
                           i32 lata =
                               p->proprietas
                                  == STATUA_ROTATIO ? IV : III;

        si (!q || q->clavium != p->clavium)
        {
            redde FALSUM;
        }
        per (j = ZEPHYRUM; j < p->clavium * lata; j++)
        {
            si (fabs((f64)p->valores[j] - (f64)q->valores[j]) > 1.0e-6)
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

interior vacuum
_sectio_speculum (
    CredoContextus* c)
{
      PlasticaFiguraFructus  f = _lege(c, GEMINA);
            constans Statua* s;
     constans StatuaCanalis* canalis;
         StereometriaVector  p;
         StereometriaVector  q;

    si (!f.successus)
    {
        printf("speculum (linea %u): %.*s\n", (insignatus)f.linea,
            (integer)f.error.mensura,
            (constans character*)f.error.datum);
    }
    CREDO_NECESSE (f.successus);
    s = f.statua;
    CREDO_AEQUALIS_I32 (s->nodorum, IX);
    CREDO_NECESSE (s->cyclorum == IV);
    /* imago vera: punctum alae sinistrae in 'a' = imago puncti
     * alae dextrae in 'b' (quies gyrata, rotatio, scala, motus
     * corporis omnia insunt) */
    p = _punctum(s, ZEPHYRUM, 0.0f, III, 0.7f, 0.1f, 0.2f);
    q = _punctum(s, I, 0.0f, V, -0.7f, 0.1f, 0.2f);
    PUNCTUM_EST (q, -(f64)p.x, (f64)p.y, (f64)p.z);
    /* non casu: ala in 'a' a quiete distat */
    q = _punctum(s, -I, 0.0f, III, 0.7f, 0.1f, 0.2f);
    CREDO_VERUM (fabs((f64)p.y - (f64)q.y) > 0.05);
    CREDO_VERUM (fabs((f64)p.x - (f64)q.x) > 0.05);
    /* articulus sine pare in loco reflectitur */
    p = _punctum(s, ZEPHYRUM, 0.0f, VII, 0.1f, 0.2f, 0.3f);
    q = _punctum(s, I, 0.0f, VII, -0.1f, 0.2f, 0.3f);
    PUNCTUM_EST (q, -(f64)p.x, (f64)p.y, (f64)p.z);
    /* par permutatur: in 'b' ala sinistra quiescit, dextra movetur */
    CREDO_NIHIL (_canalis(&s->cycli[I], III, STATUA_ROTATIO));
    CREDO_NIHIL (_canalis(&s->cycli[I], III, STATUA_SCALA));
    CREDO_NON_NIHIL (_canalis(&s->cycli[I], V, STATUA_ROTATIO));
    CREDO_NIHIL (_canalis(&s->cycli[ZEPHYRUM], V, STATUA_ROTATIO));
    canalis = _canalis(&s->cycli[I], V, STATUA_SCALA);
    CREDO_NECESSE (canalis != NIHIL);
    CREDO_F64_PROXIMUS ((f64)canalis->valores[ZEPHYRUM], 1.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)canalis->valores[I], 2.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)canalis->valores[II], 1.0, 1.0e-6);
    /* motus: x negatur, y et z manent */
    canalis = _canalis(&s->cycli[I], I, STATUA_TRANSLATIO);
    CREDO_NECESSE (canalis != NIHIL);
    CREDO_F64_PROXIMUS ((f64)canalis->valores[ZEPHYRUM], -0.3, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)canalis->valores[I], 1.2, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)canalis->valores[II], 0.1, 1.0e-6);
    /* imago imaginis = origo */
    CREDO_VERUM (_cycli_pares(&s->cycli[ZEPHYRUM], &s->cycli[II]));
    CREDO_FALSUM (_cycli_pares(&s->cycli[ZEPHYRUM], &s->cycli[I]));
    /* 'd': <art> post imaginem substituit - caput ad quietem, ala
     * sinistra scalam novam accipit, dextra ut in 'b' */
    canalis = _canalis(&s->cycli[III], VII, STATUA_ROTATIO);
    CREDO_NECESSE (canalis != NIHIL);
    CREDO_F64_PROXIMUS ((f64)canalis->valores[III], 1.0, 1.0e-6);
    canalis = _canalis(&s->cycli[III], III, STATUA_SCALA);
    CREDO_NECESSE (canalis != NIHIL);
    CREDO_F64_PROXIMUS ((f64)canalis->valores[I], 3.0, 1.0e-6);
    p = _punctum(s, I, 0.0f, V, -0.7f, 0.1f, 0.2f);
    q = _punctum(s, III, 0.0f, V, -0.7f, 0.1f, 0.2f);
    PUNCTUM_EST (q, (f64)p.x, (f64)p.y, (f64)p.z);
    /* figura impar sine speculo licet */
    f = _lege(c,
        "<figura titulus=\"a\">\n"
        "<articulus titulus=\"ala_sin\" locus=\"1 0 0\">\n"
        "<capsa magnitudo=\"1 1 1\"/>\n</articulus>\n"
        "<positura titulus=\"p\">\n"
        "<art titulus=\"ala_sin\" rotatio=\"0 0 9\"/>\n</positura>\n"
        "</figura>\n");
    CREDO_VERUM (f.successus);
    /* "_sin" nudum par non est (praefixum vacuum): in loco */
    f = _lege(c,
        "<figura titulus=\"a\">\n"
        "<articulus titulus=\"_sin\" locus=\"1 0 0\">" CAPSA_I
        "</articulus>\n"
        "<articulus titulus=\"_dex\" locus=\"-1 0 0\">" CAPSA_I
        "</articulus>\n"
        "<positura titulus=\"p\">\n"
        "<art titulus=\"_sin\" rotatio=\"0 0 10\"/>\n</positura>\n"
        "<positura titulus=\"s\" speculum=\"p\"/>\n"
        CYCLUS_UNIUS("c", "s") "</figura>\n");
    CREDO_NECESSE (f.successus);
    CREDO_NON_NIHIL (_canalis(&f.statua->cycli[ZEPHYRUM], I,
        STATUA_ROTATIO));
    CREDO_NIHIL (_canalis(&f.statua->cycli[ZEPHYRUM], III,
        STATUA_ROTATIO));
    /* paria plura: tituli eiusdem longitudinis (bos / ala / pes) et
     * praefixi communis (ala / alae) non confunduntur; par sub pare
     * nidificatum; quies cum gyro et rotatione; articulus sine pare
     * extra medium. Nodi: 1 bos_dex, 3 alae_dex, 5 ala_dex,
     * 7 pes_dex, 9 ala_sin, 11 pes_sin, 13 alae_sin, 15 bos_sin,
     * 17 cauda */
    f = _lege(c,
        "<figura titulus=\"a\">\n"
        "<articulus titulus=\"bos_dex\" locus=\"-0.7 0.3 0\">"
        CAPSA_I "</articulus>\n"
        "<articulus titulus=\"alae_dex\" locus=\"-0.2 2 0\">"
        CAPSA_I "</articulus>\n"
        "<articulus titulus=\"ala_dex\" locus=\"-0.5 1 0\""
        " gyrus=\"-30\" rotatio=\"10 0 -20\">" CAPSA_I
        "<articulus titulus=\"pes_dex\" locus=\"-0.1 -0.5 0.2\">"
        CAPSA_I "</articulus></articulus>\n"
        "<articulus titulus=\"ala_sin\" locus=\"0.5 1 0\""
        " gyrus=\"30\" rotatio=\"10 0 20\">" CAPSA_I
        "<articulus titulus=\"pes_sin\" locus=\"0.1 -0.5 0.2\">"
        CAPSA_I "</articulus></articulus>\n"
        "<articulus titulus=\"alae_sin\" locus=\"0.2 2 0\">"
        CAPSA_I "</articulus>\n"
        "<articulus titulus=\"bos_sin\" locus=\"0.7 0.3 0\">"
        CAPSA_I "</articulus>\n"
        "<articulus titulus=\"cauda\" locus=\"0.3 0 -0.5\">"
        CAPSA_I "</articulus>\n"
        "<positura titulus=\"p\">\n"
        "<art titulus=\"pes_sin\" rotatio=\"0 0 15\"/>\n"
        "<art titulus=\"alae_sin\" rotatio=\"0 0 15\"/>\n"
        "<art titulus=\"bos_sin\" rotatio=\"0 0 15\"/>\n"
        "<art titulus=\"cauda\" rotatio=\"0 20 0\"/>\n</positura>\n"
        "<positura titulus=\"s\" speculum=\"p\"/>\n"
        CYCLUS_UNIUS("c", "s") "</figura>\n");
    si (!f.successus)
    {
        printf("paria (linea %u): %.*s\n", (insignatus)f.linea,
            (integer)f.error.mensura,
            (constans character*)f.error.datum);
    }
    CREDO_NECESSE (f.successus);
    s = f.statua;
    CREDO_NECESSE (s->nodorum == XIX);
    CREDO_AEQUALIS_I32 (s->cycli[ZEPHYRUM].canalium, IV);
    CREDO_NON_NIHIL (_canalis(&s->cycli[ZEPHYRUM], VII,
        STATUA_ROTATIO));
    CREDO_NON_NIHIL (_canalis(&s->cycli[ZEPHYRUM], III,
        STATUA_ROTATIO));
    CREDO_NON_NIHIL (_canalis(&s->cycli[ZEPHYRUM], I, STATUA_ROTATIO));
    canalis = _canalis(&s->cycli[ZEPHYRUM], XVII, STATUA_ROTATIO);
    CREDO_NECESSE (canalis != NIHIL);
    CREDO_F64_PROXIMUS ((f64)canalis->valores[I], -0.17364818, 1.0e-6);
}

/* figura cum alis; 'dex' = elementum alae dextrae totum; sequentia
 * a linea V */
#define ALAE(dex) \
    "<figura titulus=\"a\">\n" \
    "<articulus titulus=\"ala_sin\" locus=\"0.5 1 0\" gyrus=\"30\">" \
    "<capsa magnitudo=\"1 1 1\"/></articulus>\n" \
    dex "\n" \
    "<articulus titulus=\"caput\"><capsa magnitudo=\"1 1 1\"/>" \
    "</articulus>\n"
#define ALA_DEX(attributa) \
    "<articulus titulus=\"ala_dex\" " attributa ">" \
    "<capsa magnitudo=\"1 1 1\"/></articulus>"
#define ALA_RECTA  ALA_DEX("locus=\"-0.5 1 0\" gyrus=\"-30\"")
#define POSITURA_P \
    "<positura titulus=\"p\">\n" \
    "<art titulus=\"caput\" rotatio=\"0 0 9\"/>\n</positura>\n"
#define SPECULATA  "<positura titulus=\"s\" speculum=\"p\"/>\n"

interior vacuum
_recusationes_probare (
        CredoContextus* c,
    constans Recusatio* tabula,
                   i32  numerus)
{
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        PlasticaFiguraFructus f = _lege(c, tabula[k].textus);

        si (f.successus || f.linea != tabula[k].linea)
        {
            printf("recusatio %u (linea %u): %s\n", (insignatus)k,
                (insignatus)f.linea, tabula[k].textus);
        }
        CREDO_FALSUM (f.successus);
        CREDO_NIHIL (f.statua);
        CREDO_AEQUALIS_I32 (f.linea, tabula[k].linea);
        CREDO_CHORDA_VACUA (f.plagula);
        CREDO_CHORDA_CONTINET (f.error,
            chorda_ex_literis(tabula[k].fragmentum, c->piscina));
    }
}

interior vacuum
_sectio_recusationes_speculi (
    CredoContextus* c)
{
    hic_manens constans Recusatio R[] = {
        { ALAE(ALA_RECTA) "<positura titulus=\"s\""
          " speculum=\"nulla\"/>\n" CAUDA, V, "speculum" },
        /* posterius */
        { ALAE(ALA_RECTA) "<positura titulus=\"s\""
          " speculum=\"p\"/>\n" POSITURA_P CAUDA, V,
          "not written above" },
        { ALAE(ALA_RECTA) POSITURA_P
          "<positura titulus=\"s\" speculum=\"p\" ex=\"p\"/>\n" CAUDA,
          VIII, "together" },
        /* par deest */
        { ALAE("<grex/>") POSITURA_P SPECULATA CAUDA, VIII,
          "'ala_sin' has no partner 'ala_dex'" },
        { "<figura titulus=\"a\">\n" ALA_RECTA "\n"
          "<articulus titulus=\"caput\"/>\n" POSITURA_P
          SPECULATA CAUDA, VII, "'ala_dex' has no partner 'ala_sin'" },
        /* quies imago non est: locus (quisque axis), rotatio */
        { ALAE(ALA_DEX("locus=\"-0.4 1 0\" gyrus=\"-30\"")) POSITURA_P
          SPECULATA CAUDA, VIII, "mirror images at rest" },
        { ALAE(ALA_DEX("locus=\"-0.5 1.1 0\" gyrus=\"-30\""))
          POSITURA_P SPECULATA CAUDA, VIII, "mirror images at rest" },
        { ALAE(ALA_DEX("locus=\"-0.5 1 0.1\" gyrus=\"-30\""))
          POSITURA_P SPECULATA CAUDA, VIII, "mirror images at rest" },
        { ALAE(ALA_DEX("locus=\"-0.5 1 0\" gyrus=\"30\"")) POSITURA_P
          SPECULATA CAUDA, VIII, "mirror images at rest" },
        { ALAE(ALA_DEX("locus=\"-0.5 1 0\" gyrus=\"-30\""
          " rotatio=\"20 0 0\"")) POSITURA_P SPECULATA CAUDA, VIII,
          "mirror images at rest" },
        /* parens: ala dextra sub capite */
        { "<figura titulus=\"a\">\n"
          "<articulus titulus=\"ala_sin\" locus=\"0.5 1 0\">"
          "<capsa magnitudo=\"1 1 1\"/></articulus>\n"
          "<articulus titulus=\"caput\">"
          "<articulus titulus=\"ala_dex\" locus=\"-0.5 1 0\">"
          "<capsa magnitudo=\"1 1 1\"/></articulus></articulus>\n"
          POSITURA_P SPECULATA CAUDA, VII, "mirror images at rest" }
    };

    _recusationes_probare(c, R, (i32)(magnitudo(R) / magnitudo(R[0])));
}

/* ossa communia. Lineae I-XXI */
#define OSSA_ARTICULI \
    "<ossa titulus=\"duo\">\n" \
    "  <articulus titulus=\"pelvis\" locus=\"0 1 0\">\n" \
    "    <articulus titulus=\"crus_sin\" locus=\"0.2 0 0\"/>\n" \
    "    <articulus titulus=\"crus_dex\" locus=\"-0.2 0 0\"/>\n" \
    "  </articulus>\n"

hic_manens constans character* OSSA_DUO =
    OSSA_ARTICULI
    "  <positura titulus=\"gradus\">\n"
    "    <art titulus=\"pelvis\" motus=\"0.1 0.2 0.4\"/>\n"
    "    <art titulus=\"crus_sin\" rotatio=\"30 0 0\"/>\n"
    "  </positura>\n"
    "  <positura titulus=\"gradus_dex\" speculum=\"gradus\"/>\n"
    "  <cyclus titulus=\"walk\" duratio=\"1\" circulus=\"verum\">\n"
    "    <clavis tempus=\"0\" positura=\"quies\"/>\n"
    "    <clavis tempus=\"0.25\" positura=\"gradus\"/>\n"
    "    <clavis tempus=\"0.5\" positura=\"quies\"/>\n"
    "    <clavis tempus=\"0.75\" positura=\"gradus_dex\"/>\n"
    "    <momentum titulus=\"gressus\" tempus=\"0.25\"/>\n"
    "  </cyclus>\n"
    "  <cyclus titulus=\"idle\" duratio=\"2\">\n"
    "    <clavis tempus=\"0\" positura=\"quies\"/>\n"
    "  </cyclus>\n"
    "</ossa>\n";

#define CAPSA  "<capsa magnitudo=\"1 1 1\"/>"

/* nodi: 0 radix, 1 pelvis, 2 capsa, 3 crus_sin, 4 capsa, 5 crus_dex
 * (vacuus) */
hic_manens constans character* FIGURA_A =
    "<figura titulus=\"a\" ossa=\"duo\">\n"
    "<articulus titulus=\"pelvis\">" CAPSA "</articulus>\n"
    "<articulus titulus=\"crus_sin\">" CAPSA "</articulus>\n"
    "</figura>\n";

/* altior et latior; nodi ut A, et 6 capsa cruris dextri */
hic_manens constans character* FIGURA_B =
    "<figura titulus=\"b\" ossa=\"duo\">\n"
    "<articulus titulus=\"crus_dex\" locus=\"-0.3 0 0\">" CAPSA
    "</articulus>\n"
    "<articulus titulus=\"pelvis\" locus=\"0 2 0\">" CAPSA
    "</articulus>\n"
    "<articulus titulus=\"crus_sin\" locus=\"0.3 0 0\">" CAPSA
    "</articulus>\n"
    "</figura>\n";

nomen structura {
    constans character* titulus;
    constans character* textus;
                   i32  vocationes;
               Piscina* piscina;
} Armarium;

interior chorda
_fons (
    chorda  titulus,
    vacuum* contextus)
{
    Armarium* a = (Armarium*)contextus;
      chorda  vacua;

    memset(&vacua, ZEPHYRUM, magnitudo(vacua));
    a->vocationes++;
    si (!a->textus || !chorda_aequalis_literis(titulus, a->titulus))
    {
        redde vacua;
    }
    redde chorda_ex_literis(a->textus, a->piscina);
}

interior PlasticaFiguraFructus
_lege_cum (
        CredoContextus* c,
              Armarium* a,
    constans character* ossa,
    constans character* figura)
{
    PlasticaFiguraFructus f;

    a->titulus     = "duo";
    a->textus      = ossa;
    a->vocationes  = ZEPHYRUM;
    a->piscina     = c->piscina;
    f = plastica_figura_legere_cum_fonte(
        chorda_ex_literis(figura, c->piscina), ZEPHYRUM, _fons, a,
        c->piscina);
    redde f;
}

interior vacuum
_sectio_ossa (
    CredoContextus* c)
{
                  Armarium  a;
     PlasticaFiguraFructus  fa = _lege_cum(c, &a, OSSA_DUO, FIGURA_A);
     PlasticaFiguraFructus  fb;
     PlasticaFiguraFructus  fc;
           constans Statua* s;
           constans Statua* t;
    constans StatuaCanalis* p;
    constans StatuaCanalis* q;

    CREDO_NECESSE (fa.successus);
    CREDO_AEQUALIS_I32 (a.vocationes, I);
    CREDO_CHORDA_VACUA (fa.plagula);
    s = fa.statua;
    /* arbor ossium; solida figurae ante articulos liberos; articulus
     * non nominatus nodus vacuus */
    CREDO_NECESSE (s->nodorum == VI);
    CREDO_AEQUALIS_I32 (fa.articulorum, III);
    CREDO_AEQUALIS_I32 (fa.articuli[ZEPHYRUM], I);
    CREDO_AEQUALIS_I32 (fa.articuli[I], III);
    CREDO_AEQUALIS_I32 (fa.articuli[II], V);
    CREDO_CHORDA_AEQUALIS_LITERIS (fa.partes[I], "pelvis");
    CREDO_CHORDA_AEQUALIS_LITERIS (fa.partes[III], "crus_sin");
    CREDO_CHORDA_AEQUALIS_LITERIS (fa.partes[V], "crus_dex");
    CREDO_AEQUALIS_S32 (s->nodi[III].parens, I);
    CREDO_AEQUALIS_S32 (s->nodi[IV].parens, III);
    CREDO_AEQUALIS_S32 (s->nodi[V].parens, I);
    CREDO_AEQUALIS_S32 (s->nodi[II].rete, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (s->nodi[V].rete, -I);
    CREDO_F64_PROXIMUS ((f64)s->nodi[I].translatio.y, 1.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)s->nodi[III].translatio.x, 0.2, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)s->nodi[V].translatio.x, -0.2, 1.0e-6);
    /* cycli et momenta ossium */
    CREDO_NECESSE (s->cyclorum == II);
    CREDO_CHORDA_AEQUALIS_LITERIS (s->cycli[ZEPHYRUM].titulus, "walk");
    CREDO_CHORDA_AEQUALIS_LITERIS (s->cycli[I].titulus, "idle");
    CREDO_NECESSE (fa.momentorum == I);
    CREDO_CHORDA_AEQUALIS_LITERIS (fa.momenta[ZEPHYRUM].titulus,
        "gressus");
    CREDO_AEQUALIS_I32 (fa.momenta[ZEPHYRUM].cyclus, ZEPHYRUM);
    p = _canalis(&s->cycli[ZEPHYRUM], I, STATUA_TRANSLATIO);
    CREDO_NECESSE (p != NIHIL && p->clavium == V);
    CREDO_F64_PROXIMUS ((f64)p->valores[III], 0.1, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)p->valores[IV], 1.2, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)p->valores[V], 0.4, 1.0e-6);
    /* positura speculata ossium */
    CREDO_F64_PROXIMUS ((f64)p->valores[IX], -0.1, 1.0e-6);
    CREDO_NON_NIHIL (_canalis(&s->cycli[ZEPHYRUM], V, STATUA_ROTATIO));

    /* figura altera, proportiones aliae: rotationes eaedem, motus
     * radicis per altitudinem multiplicatus (II / I) */
    fb = _lege_cum(c, &a, OSSA_DUO, FIGURA_B);
    CREDO_NECESSE (fb.successus);
    t = fb.statua;
    CREDO_NECESSE (t->nodorum == VII);
    CREDO_CHORDA_AEQUALIS_LITERIS (fb.partes[V], "crus_dex");
    CREDO_AEQUALIS_S32 (t->nodi[VI].parens, V);
    CREDO_F64_PROXIMUS ((f64)t->nodi[I].translatio.y, 2.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)t->nodi[III].translatio.x, 0.3, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)t->nodi[V].translatio.x, -0.3, 1.0e-6);
    p = _canalis(&s->cycli[ZEPHYRUM], III, STATUA_ROTATIO);
    q = _canalis(&t->cycli[ZEPHYRUM], III, STATUA_ROTATIO);
    CREDO_NECESSE (p != NIHIL && q != NIHIL);
    CREDO_NECESSE (p->clavium == V && q->clavium == V);
    CREDO_AEQUALIS_I32 ((i32)memcmp(p->valores, q->valores,
        V * IV * magnitudo(f32)), ZEPHYRUM);
    CREDO_F64_PROXIMUS ((f64)p->valores[IV], 0.25881905, 1.0e-6);
    q = _canalis(&t->cycli[ZEPHYRUM], I, STATUA_TRANSLATIO);
    CREDO_NECESSE (q != NIHIL && q->clavium == V);
    CREDO_F64_PROXIMUS ((f64)q->valores[III], 0.2, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)q->valores[IV], 2.4, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)q->valores[V], 0.8, 1.0e-6);

    /* cardo unus motus nodum unum solum movet */
    fc = _lege_cum(c, &a, OSSA_DUO,
        "<figura titulus=\"c\" ossa=\"duo\">\n"
        "<articulus titulus=\"pelvis\" locus=\"0.5 1.5 0.25\">" CAPSA
        "</articulus>\n"
        "<articulus titulus=\"crus_sin\">" CAPSA "</articulus>\n"
        "</figura>\n");
    CREDO_NECESSE (fc.successus);
    t = fc.statua;
    CREDO_NECESSE (t->nodorum == VI);
    CREDO_F64_PROXIMUS ((f64)t->nodi[I].translatio.x, 0.5, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)t->nodi[I].translatio.y, 1.5, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)t->nodi[I].translatio.z, 0.25, 1.0e-6);
    CREDO_AEQUALIS_I32 ((i32)memcmp(&t->nodi[III].translatio,
        &s->nodi[III].translatio, magnitudo(s->nodi[III].translatio)),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 ((i32)memcmp(&t->nodi[V].translatio,
        &s->nodi[V].translatio, magnitudo(s->nodi[V].translatio)),
        ZEPHYRUM);

    /* elementa radicis figurae quae articuli non sunt ante arborem
     * ossium stant: nodus 1 solidum, 2 pelvis */
    fc = _lege_cum(c, &a, OSSA_DUO,
        "<figura titulus=\"c\" ossa=\"duo\">\n"
        "<articulus titulus=\"pelvis\">" CAPSA "</articulus>\n"
        "<capsa titulus=\"basis\" magnitudo=\"2 0.1 2\"/>\n"
        "</figura>\n");
    CREDO_NECESSE (fc.successus);
    CREDO_NECESSE (fc.statua->nodorum == VI);
    CREDO_CHORDA_AEQUALIS_LITERIS (fc.partes[I], "basis");
    CREDO_CHORDA_AEQUALIS_LITERIS (fc.partes[II], "pelvis");
    CREDO_AEQUALIS_I32 (fc.articuli[ZEPHYRUM], II);

    /* figura sine ossibus per fontem: fons non vocatur */
    a.vocationes = ZEPHYRUM;
    fc = plastica_figura_legere_cum_fonte(
        chorda_ex_literis(PUPA, c->piscina), ZEPHYRUM, _fons, &a,
        c->piscina);
    CREDO_VERUM (fc.successus);
    CREDO_AEQUALIS_I32 (a.vocationes, ZEPHYRUM);
    /* ossium altitudo 0: factor I */
    fc = _lege_cum(c, &a,
        "<ossa titulus=\"duo\">\n"
        "<articulus titulus=\"pelvis\" locus=\"0 0 0\"/>\n"
        "<positura titulus=\"p\">\n"
        "<art titulus=\"pelvis\" motus=\"0 0.5 0\"/>\n</positura>\n"
        CYCLUS_UNIUS("idle", "p") "</ossa>\n",
        "<figura titulus=\"c\" ossa=\"duo\">\n"
        "<articulus titulus=\"pelvis\" locus=\"0 3 0\">" CAPSA
        "</articulus>\n</figura>\n");
    CREDO_NECESSE (fc.successus);
    q = _canalis(&fc.statua->cycli[ZEPHYRUM], I, STATUA_TRANSLATIO);
    CREDO_NECESSE (q != NIHIL);
    CREDO_F64_PROXIMUS ((f64)q->valores[I], 3.5, 1.0e-6);
    /* titulus ossium cum cifra et '_' */
    a.titulus     = "os_2";
    a.textus      = OSSA_DUO;
    a.vocationes  = ZEPHYRUM;
    fc = plastica_figura_legere_cum_fonte(chorda_ex_literis(
        "<figura titulus=\"c\" ossa=\"os_2\">\n"
        "<articulus titulus=\"pelvis\">" CAPSA "</articulus>\n"
        "</figura>\n", c->piscina), ZEPHYRUM, _fons, &a, c->piscina);
    CREDO_VERUM (fc.successus);
    CREDO_AEQUALIS_I32 (a.vocationes, I);
}

interior vacuum
_sectio_substitutio (
    CredoContextus* c)
{
                  Armarium a;
     PlasticaFiguraFructus f = _lege_cum(c, &a, OSSA_DUO,
         "<figura titulus=\"d\" ossa=\"duo\">\n"
         "<articulus titulus=\"pelvis\" locus=\"0 2 0\">" CAPSA
         "</articulus>\n"
         "<positura titulus=\"gradus\" ex=\"gradus\">\n"
         "<art titulus=\"crus_sin\" rotatio=\"40 0 0\"/>\n"
         "</positura>\n"
         "<positura titulus=\"alta\" ex=\"gradus_dex\">\n"
         "<art titulus=\"pelvis\" motus=\"0 1 0\"/>\n"
         "</positura>\n"
         "<cyclus titulus=\"idle\" duratio=\"3\">\n"
         "<clavis tempus=\"0\" positura=\"alta\"/>\n"
         "</cyclus>\n"
         "<cyclus titulus=\"attack\" duratio=\"1\">\n"
         "<clavis tempus=\"0\" positura=\"quies\"/>\n"
         "<momentum titulus=\"ictus\" tempus=\"0.5\"/>\n"
         "</cyclus>\n"
         "</figura>\n");
           constans Statua* s;
    constans StatuaCanalis* p;

    CREDO_NECESSE (f.successus);
    s = f.statua;
    /* nodi: 0 radix, 1 pelvis, 2 capsa, 3 crus_sin, 4 crus_dex.
     * Cyclus figurae eiusdem tituli substituit, loco servato; novus
     * additur */
    CREDO_NECESSE (s->cyclorum == III);
    CREDO_CHORDA_AEQUALIS_LITERIS (s->cycli[ZEPHYRUM].titulus, "walk");
    CREDO_CHORDA_AEQUALIS_LITERIS (s->cycli[I].titulus, "idle");
    CREDO_CHORDA_AEQUALIS_LITERIS (s->cycli[II].titulus, "attack");
    CREDO_F64_PROXIMUS ((f64)s->cycli[I].duratio, 3.0, 1.0e-6);
    CREDO_NECESSE (f.momentorum == II);
    CREDO_CHORDA_AEQUALIS_LITERIS (f.momenta[ZEPHYRUM].titulus,
        "gressus");
    CREDO_AEQUALIS_I32 (f.momenta[ZEPHYRUM].cyclus, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (f.momenta[I].titulus, "ictus");
    CREDO_AEQUALIS_I32 (f.momenta[I].cyclus, II);
    /* cyclus ossium posituram substitutam accipit: XL gradus; motus
     * per 'ex' ex ossibus (multiplicatus) manet */
    p = _canalis(&s->cycli[ZEPHYRUM], III, STATUA_ROTATIO);
    CREDO_NECESSE (p != NIHIL && p->clavium == V);
    CREDO_F64_PROXIMUS ((f64)p->valores[IV], 0.34202014, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)p->valores[VII], 0.93969262, 1.0e-6);
    p = _canalis(&s->cycli[ZEPHYRUM], I, STATUA_TRANSLATIO);
    CREDO_NECESSE (p != NIHIL);
    CREDO_F64_PROXIMUS ((f64)p->valores[III], 0.2, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)p->valores[IV], 2.4, 1.0e-6);
    /* positura ossium ex priore derivata substitutionem non
     * sequitur: crus dextrum XXX gradus */
    p = _canalis(&s->cycli[ZEPHYRUM], IV, STATUA_ROTATIO);
    CREDO_NECESSE (p != NIHIL);
    CREDO_F64_PROXIMUS ((f64)p->valores[XII], 0.25881905, 1.0e-6);
    /* idle novus: 'alta' ex positura ossium; motus figurae ipsius
     * non multiplicatur; rotatio per 'ex' manet */
    p = _canalis(&s->cycli[I], I, STATUA_TRANSLATIO);
    CREDO_NECESSE (p != NIHIL);
    CREDO_F64_PROXIMUS ((f64)p->valores[ZEPHYRUM], 0.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)p->valores[I], 3.0, 1.0e-6);
    CREDO_F64_PROXIMUS ((f64)p->valores[II], 0.0, 1.0e-6);
    CREDO_NON_NIHIL (_canalis(&s->cycli[I], IV, STATUA_ROTATIO));
    CREDO_NIHIL (_canalis(&s->cycli[I], III, STATUA_ROTATIO));

    /* ad fines: ossa LXIV posituras et XVI cyclos tenent; figura
     * unum cuiusque substituit (numerus non crescit) */
    {
        character* ossa = (character*)piscina_allocare(c->piscina,
            (memoriae_index)(LXIV + XVI * III) * CXXVIII + DXII);
              i32 longitudo = ZEPHYRUM;
              i32 k;

        longitudo += (i32)sprintf(ossa + longitudo, "%s",
            OSSA_ARTICULI);
        per (k = ZEPHYRUM; k < PLASTICA_MOTUS_POSITURAE_MAXIMAE; k++)
        {
            longitudo += (i32)sprintf(ossa + longitudo,
                "<positura titulus=\"p%u\"/>\n", (insignatus)k);
        }
        per (k = ZEPHYRUM; k < PLASTICA_MOTUS_CYCLI_MAXIMI; k++)
        {
            longitudo += (i32)sprintf(ossa + longitudo,
                "<cyclus titulus=\"c%u\" duratio=\"1\">\n"
                "<clavis tempus=\"0\" positura=\"quies\"/>\n"
                "</cyclus>\n", (insignatus)k);
        }
        longitudo += (i32)sprintf(ossa + longitudo, "</ossa>\n");
        f = _lege_cum(c, &a, ossa,
            "<figura titulus=\"d\" ossa=\"duo\">\n"
            "<articulus titulus=\"pelvis\">" CAPSA "</articulus>\n"
            "<positura titulus=\"p0\"/>\n"
            "<cyclus titulus=\"c0\" duratio=\"5\">\n"
            "<clavis tempus=\"0\" positura=\"p0\"/>\n"
            "</cyclus>\n"
            "</figura>\n");
        CREDO_NECESSE (f.successus);
        CREDO_AEQUALIS_I32 (f.statua->cyclorum,
            PLASTICA_MOTUS_CYCLI_MAXIMI);
        CREDO_F64_PROXIMUS ((f64)f.statua->cycli[ZEPHYRUM].duratio,
            5.0, 1.0e-6);
        /* nova tamen ultra finem recusatur */
        f = _lege_cum(c, &a, ossa,
            "<figura titulus=\"d\" ossa=\"duo\">\n"
            "<articulus titulus=\"pelvis\">" CAPSA "</articulus>\n"
            "<positura titulus=\"nova\"/>\n"
            "</figura>\n");
        CREDO_FALSUM (f.successus);
        CREDO_AEQUALIS_I32 (f.linea, III);
        CREDO_CHORDA_VACUA (f.plagula);
    }
}

nomen structura {
    constans character* ossa;
    constans character* figura;
                   i32  linea;
    constans character* plagula;
    constans character* fragmentum;
} RecusatioOssium;

#define FIGURA_CAPUT  "<figura titulus=\"a\" ossa=\"duo\">\n"
#define PELVIS \
    "<articulus titulus=\"pelvis\">" CAPSA "</articulus>\n"
#define FIGURA_SIMPLEX  FIGURA_CAPUT PELVIS CAUDA
#define OSSA_CAUDA  "</ossa>\n"

interior vacuum
_sectio_recusationes_ossium (
    CredoContextus* c)
{
    hic_manens constans RecusatioOssium R[] = {
        /* ossa ipsa */
        { NIHIL, FIGURA_SIMPLEX, I, "", "unknown skeleton 'duo'" },
        { "", FIGURA_SIMPLEX, I, "", "unknown skeleton 'duo'" },
        { OSSA_ARTICULI OSSA_CAUDA,
          "<figura titulus=\"a\" ossa=\"../duo\">\n" PELVIS CAUDA, I,
          "", "a-z" },
        { OSSA_ARTICULI OSSA_CAUDA,
          "<figura titulus=\"a\" ossa=\"Duo\">\n" PELVIS CAUDA, I,
          "", "a-z" },
        { OSSA_ARTICULI OSSA_CAUDA,
          "<figura titulus=\"a\" ossa=\"du{o\">\n" PELVIS CAUDA, I,
          "", "a-z" },
        { "<ossa titulus=\"duo\"><articulus", FIGURA_SIMPLEX,
          ZEPHYRUM, "duo", "STML" },
        { "<figura titulus=\"duo\">\n" CAPSA "</figura>",
          FIGURA_SIMPLEX, I, "duo", "root element <ossa>" },
        { "<ossa titulus=\"duo\" ossa=\"alia\">\n"
          "<articulus titulus=\"pelvis\"/>\n" OSSA_CAUDA,
          FIGURA_SIMPLEX, I, "duo", "unknown attribute 'ossa'" },
        { "<ossa>\n<articulus titulus=\"pelvis\"/>\n" OSSA_CAUDA,
          FIGURA_SIMPLEX, I, "duo", "titulus" },
        /* solida et aliena in ossibus */
        { "<ossa titulus=\"duo\">\n"
          "<articulus titulus=\"pelvis\"/>\n" CAPSA "\n" OSSA_CAUDA,
          FIGURA_SIMPLEX, III, "duo", "solids" },
        { "<ossa titulus=\"duo\">\n"
          "<articulus titulus=\"pelvis\">\n" CAPSA "\n</articulus>\n"
          OSSA_CAUDA, FIGURA_SIMPLEX, III, "duo", "solids" },
        { "<ossa titulus=\"duo\">\n"
          "<articulus titulus=\"pelvis\"/>\n<membrum/>\n" OSSA_CAUDA,
          FIGURA_SIMPLEX, III, "duo", "not built yet" },
        /* articuli ossium */
        { "<ossa titulus=\"duo\">\n"
          "<articulus titulus=\"pelvis\"/>\n"
          "<articulus titulus=\"pelvis\"/>\n" OSSA_CAUDA,
          FIGURA_SIMPLEX, III, "duo", "twice" },
        { "<ossa titulus=\"duo\">\n"
          "<articulus titulus=\"pelvis\">\n<articulus/>\n"
          "</articulus>\n" OSSA_CAUDA,
          FIGURA_SIMPLEX, III, "duo", "titulus" },
        { "<ossa titulus=\"duo\">\n"
          "<articulus titulus=\"pelvis\" locus=\"0 x 0\"/>\n"
          OSSA_CAUDA, FIGURA_SIMPLEX, II, "duo", "locus" },
        { "<ossa titulus=\"duo\">\n"
          "<articulus titulus=\"pelvis\">\n"
          "<articulus titulus=\"genu\" color=\"#fff\"/>\n"
          "</articulus>\n" OSSA_CAUDA,
          FIGURA_SIMPLEX, III, "duo", "color" },
        /* motus in ossibus: linea plagulae ossium */
        { OSSA_ARTICULI "<positura titulus=\"p\">\n"
          "<art titulus=\"genu\" rotatio=\"0 0 1\"/>\n</positura>\n"
          OSSA_CAUDA, FIGURA_SIMPLEX, VII, "duo",
          "unknown joint 'genu'" },
        { OSSA_ARTICULI "<cyclus titulus=\"c\" duratio=\"1\">\n"
          "</cyclus>\n" OSSA_CAUDA, FIGURA_SIMPLEX, VI, "duo",
          "no keys" },
        /* tituli iterati in ossibus ipsis */
        { OSSA_ARTICULI "<positura titulus=\"p\"/>\n"
          "<positura titulus=\"p\"/>\n" OSSA_CAUDA, FIGURA_SIMPLEX,
          VII, "duo", "twice" },
        { OSSA_ARTICULI CYCLUS_UNIUS("c", "quies")
          CYCLUS_UNIUS("c", "quies") OSSA_CAUDA, FIGURA_SIMPLEX, IX,
          "duo", "twice" },
        /* figura substitutionem bis non facit */
        { OSSA_ARTICULI "<positura titulus=\"p\"/>\n" OSSA_CAUDA,
          FIGURA_CAPUT PELVIS "<positura titulus=\"p\"/>\n"
          "<positura titulus=\"p\"/>\n" CAUDA, IV, "", "twice" },
        { OSSA_ARTICULI CYCLUS_UNIUS("c", "quies") OSSA_CAUDA,
          FIGURA_CAPUT PELVIS CYCLUS_UNIUS("c", "quies")
          CYCLUS_UNIUS("c", "quies") CAUDA, VI, "", "twice" },
        /* figura */
        { OSSA_ARTICULI OSSA_CAUDA,
          FIGURA_CAPUT PELVIS "<articulus titulus=\"genu\">" CAPSA
          "</articulus>\n" CAUDA, III, "", "'genu'" },
        { OSSA_ARTICULI OSSA_CAUDA,
          FIGURA_CAPUT PELVIS PELVIS CAUDA, III, "", "twice" },
        { OSSA_ARTICULI OSSA_CAUDA,
          FIGURA_CAPUT "<articulus>" CAPSA "</articulus>\n" CAUDA,
          II, "", "titulus" },
        { OSSA_ARTICULI OSSA_CAUDA,
          FIGURA_CAPUT "<articulus titulus=\"pelvis\">\n"
          "<articulus titulus=\"crus_sin\">" CAPSA "</articulus>\n"
          "</articulus>\n" CAUDA, III, "", "nested" },
        { OSSA_ARTICULI OSSA_CAUDA,
          FIGURA_CAPUT "<articulus titulus=\"pelvis\" gyrus=\"10\">"
          CAPSA "</articulus>\n" CAUDA, II, "",
          "'gyrus' belongs to the skeleton" },
        { OSSA_ARTICULI OSSA_CAUDA,
          FIGURA_CAPUT "<articulus titulus=\"pelvis\""
          " rotatio=\"0 0 10\">" CAPSA "</articulus>\n" CAUDA, II,
          "", "'rotatio' belongs to the skeleton" },
        { OSSA_ARTICULI OSSA_CAUDA,
          FIGURA_CAPUT "<articulus titulus=\"pelvis\" locus=\"0 1\">"
          CAPSA "</articulus>\n" CAUDA, II, "", "'locus'" },
        { OSSA_ARTICULI OSSA_CAUDA,
          FIGURA_CAPUT "<articulus titulus=\"pelvis\" color=\"#fff\">"
          CAPSA "</articulus>\n" CAUDA, II, "", "color" },
        /* plastica figuram compositam recusat: linea figurae */
        { OSSA_ARTICULI OSSA_CAUDA,
          FIGURA_CAPUT "<articulus titulus=\"pelvis\">\n"
          "<capsa magnitudo=\"0 1 1\"/>\n</articulus>\n" CAUDA, III,
          "", "magnitudo" },
        { OSSA_ARTICULI OSSA_CAUDA,
          "<figura titulus=\"a\" ossa=\"duo\" obex=\"1\">\n" PELVIS
          CAUDA, I, "", "obex" },
        { OSSA_ARTICULI OSSA_CAUDA, FIGURA_CAPUT CAUDA, I, "",
          "solid" },
        /* motus in figura: linea figurae */
        { OSSA_ARTICULI OSSA_CAUDA,
          FIGURA_CAPUT PELVIS "<positura titulus=\"p\">\n"
          "<art titulus=\"genu\" rotatio=\"0 0 1\"/>\n</positura>\n"
          CAUDA, IV, "", "unknown joint 'genu'" },
        /* positura substituta viam cycli ossium frangit */
        { OSSA_ARTICULI "<positura titulus=\"p\">\n"
          "<art titulus=\"pelvis\" rotatio=\"0 0 10\"/>\n"
          "</positura>\n"
          "<cyclus titulus=\"c\" duratio=\"1\">\n"
          "<clavis tempus=\"0\" positura=\"quies\"/>\n"
          "<clavis tempus=\"0.5\" positura=\"p\"/>\n</cyclus>\n"
          OSSA_CAUDA,
          FIGURA_CAPUT PELVIS "<positura titulus=\"p\">\n"
          "<art titulus=\"pelvis\" rotatio=\"0 0 200\"/>\n"
          "</positura>\n" CAUDA, XI, "duo", "add a key between" },
        /* ... et viam cycli figurae ANTE substitutionem scripti */
        { OSSA_ARTICULI "<positura titulus=\"p\">\n"
          "<art titulus=\"pelvis\" rotatio=\"0 0 10\"/>\n"
          "</positura>\n" CYCLUS_UNIUS("z", "quies") OSSA_CAUDA,
          FIGURA_CAPUT PELVIS
          "<cyclus titulus=\"c\" duratio=\"1\">\n"
          "<clavis tempus=\"0\" positura=\"quies\"/>\n"
          "<clavis tempus=\"0.5\" positura=\"p\"/>\n</cyclus>\n"
          "<positura titulus=\"p\">\n"
          "<art titulus=\"pelvis\" rotatio=\"0 0 200\"/>\n"
          "</positura>\n" CAUDA, V, "", "add a key between" }
    };
     PlasticaFiguraFructus f;
                  Armarium a;
                       i32 k;

    per (k = ZEPHYRUM; k < (i32)(magnitudo(R) / magnitudo(R[0])); k++)
    {
        f = _lege_cum(c, &a, R[k].ossa, R[k].figura);
        si (f.successus || (R[k].linea && f.linea != R[k].linea))
        {
            printf("recusatio ossium %u (linea %u)\n", (insignatus)k,
                (insignatus)f.linea);
        }
        CREDO_FALSUM (f.successus);
        CREDO_NIHIL (f.statua);
        CREDO_NIHIL (f.partes);
        CREDO_NIHIL (f.momenta);
        si (R[k].linea)
        {
            CREDO_AEQUALIS_I32 (f.linea, R[k].linea);
        }
        CREDO_CHORDA_AEQUALIS_LITERIS (f.plagula, R[k].plagula);
        CREDO_FALSUM (chorda_continet(f.error,
            chorda_ex_literis("\n", c->piscina)));
        /* signum lineae ossium in nuntium non effluit */
        CREDO_FALSUM (chorda_continet(f.error,
            chorda_ex_literis("10737", c->piscina)));
        CREDO_CHORDA_CONTINET (f.error,
            chorda_ex_literis(R[k].fragmentum, c->piscina));
    }
    /* articuli ossium LXIV licent, LXV recusantur in plagula ossium */
    {
        character* ossa = (character*)piscina_allocare(c->piscina,
            (memoriae_index)LXVI * LXIV + DXII);
              i32 longitudo;
              i32 n;

        per (n = PLASTICA_ARTICULI_MAXIMI;
             n <= PLASTICA_ARTICULI_MAXIMI + I; n++)
        {
            longitudo = (i32)sprintf(ossa, "<ossa titulus=\"duo\">\n"
                "<articulus titulus=\"pelvis\"/>\n");
            per (k = I; k < n; k++)
            {
                longitudo += (i32)sprintf(ossa + longitudo,
                    "<articulus titulus=\"a%u\"/>\n", (insignatus)k);
            }
            (vacuum)sprintf(ossa + longitudo, "</ossa>\n");
            f = _lege_cum(c, &a, ossa, FIGURA_SIMPLEX);
            si (n == PLASTICA_ARTICULI_MAXIMI)
            {
                CREDO_VERUM (f.successus);
                CREDO_AEQUALIS_I32 (f.articulorum, LXIV);
                perge;
            }
            CREDO_FALSUM (f.successus);
            CREDO_AEQUALIS_I32 (f.linea, II + LXIV);
            CREDO_CHORDA_AEQUALIS_LITERIS (f.plagula, "duo");
            CREDO_CHORDA_CONTINET (f.error,
                chorda_ex_literis("64", c->piscina));
        }
    }
    /* arbor: radix nulla et semen extra fines recusantur */
    {
        StmlResultus r = stml_legere(
            chorda_ex_literis("<res titulus=\"a\">" CAPSA "</res>",
                c->piscina), c->piscina,
            internamentum_creare(c->piscina));
        PlasticaFructus p;

        CREDO_NECESSE (r.successus && r.elementum_radix != NIHIL);
        p = plastica_legere_arboris(NIHIL, ZEPHYRUM, c->piscina);
        CREDO_FALSUM (p.successus);
        CREDO_CHORDA_NON_VACUA (p.error);
        p = plastica_legere_arboris(r.elementum_radix, M * M + I,
            c->piscina);
        CREDO_FALSUM (p.successus);
        CREDO_CHORDA_CONTINET (p.error,
            chorda_ex_literis("seed", c->piscina));
        p = plastica_legere_arboris(r.elementum_radix, M * M,
            c->piscina);
        CREDO_VERUM (p.successus);
        CREDO_AEQUALIS_I32 (p.statua->nodorum, II);
    }
    /* sine fonte: figura cum ossibus recusatur */
    f = _lege(c, FIGURA_A);
    CREDO_FALSUM (f.successus);
    CREDO_AEQUALIS_I32 (f.linea, I);
    CREDO_CHORDA_VACUA (f.plagula);
    CREDO_CHORDA_CONTINET (f.error,
        chorda_ex_literis("need a source", c->piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "cycli", _sectio_cycli, NIHIL, NIHIL, NIHIL, NIHIL },
    { "positura", _sectio_positura, NIHIL, NIHIL, NIHIL, NIHIL },
    { "circulus", _sectio_circulus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "via", _sectio_via, NIHIL, NIHIL, NIHIL, NIHIL },
    { "momenta", _sectio_momenta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine_motu", _sectio_sine_motu, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusationes", _sectio_recusationes, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "fines", _sectio_fines, NIHIL, NIHIL, NIHIL, NIHIL },
    { "determinata", _sectio_determinata, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "speculum", _sectio_speculum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusationes_speculi", _sectio_recusationes_speculi, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "ossa", _sectio_ossa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "substitutio", _sectio_substitutio, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "recusationes_ossium", _sectio_recusationes_ossium, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("plastica_motus", SECTIONES);
}
