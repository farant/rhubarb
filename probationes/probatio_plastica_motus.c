/* probatio_plastica_motus.c - figura cum motu (plastica_motus.h)
 *
 * cycli: canales soli articulorum tactorum, claves, tituli; positura:
 * rotatio, scala, motus per statua_positura ad claves et in medio,
 * 'ex', "quies"; circulus: sutura clausa, positura ultima tenta,
 * cyclus unius positurae; via: gradus scripti servantur (CLXX,
 * CCCXL), gyrus plenus per suturam; momenta; sine motu;
 * recusationes cum linea; fines numerorum; determinata.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "stereometria.h"
#include "statua.h"
#include "plastica.h"
#include "plastica_motus.h"
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
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("plastica_motus", SECTIONES);
}
