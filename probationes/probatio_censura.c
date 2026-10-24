/* probatio_censura.c - Probationes censurae (include/censura.h)
 *
 * Regula quaeque: casus quem invenire debet ET casus propinquus quem
 * non debet (tritonus in tempore debili, quintae contrariae, G# in A
 * minor, '|' recta in metro mutato). Textus inventi exactus.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "carmen.h"
#include "censura.h"
#include <stdio.h>
#include <string.h>

#define CAPACITAS XVI

interior Carmen*
_carmen (
        CredoContextus* c,
    constans character* stml)
{
           Carmina* liber;
    CarminisVitium  vitium;

    liber = (Carmina*)piscina_allocare(c->piscina, magnitudo(Carmina));
    si (!carmina_legere(chorda_ex_literis(stml, c->piscina), c->piscina,
                        liber, &vitium) || liber->numerus < I)
    {
        imprimere("  carmen: %s (offset %u)\n",
                  vitium.causa ? vitium.causa : "-", vitium.offset);
        redde NIHIL;
    }
    redde &liber->carmina[ZEPHYRUM];
}

/* forma cum regula sola */
interior CensuraForma
_sola (
    CensuraRegula regula)
{
    CensuraForma f;

    censura_ordinaria(&f);
    memset(f.activae, ZEPHYRUM, magnitudo(f.activae));
    f.activae[regula] = VERUM;
    redde f;
}

interior chorda
_linea (
              CredoContextus* c,
             constans Carmen* k,
    constans CensuraInventum* inv)
{
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina,
        CXXVIII);
               chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (a == NIHIL || !censura_inventum_scribere(k, inv, a))
    {
        redde vacua;
    }
    redde chorda_aedificator_finire(a);
}

interior b32
_ambitus_angustus (
     vacuum* ctx,
     chorda  instrumentum,
        i32* infima,
        i32* summa)
{
    (vacuum)ctx;
    (vacuum)instrumentum;
    *infima  = LX;
    *summa   = LXII;
    redde VERUM;
}


    /* ==================================================
     * ORDINARIA: activae, tituli, ambitus
     * ================================================== */

interior vacuum
_sectio_ordinaria (
    CredoContextus* c)
{
     CensuraForma f;
    CensuraRegula r;
    CensuraRegula inventa;
              i32 infima;
              i32 summa;

    censura_ordinaria(&f);
    CREDO_VERUM (f.activae[CENSURA_LINEAE]);
    CREDO_VERUM (f.activae[CENSURA_CLAVIS]);
    CREDO_VERUM (f.activae[CENSURA_AMBITUS]);
    CREDO_VERUM (f.activae[CENSURA_DISSONANTIA]);
    CREDO_FALSUM (f.activae[CENSURA_PARALLELAE]);
    CREDO_FALSUM (f.activae[CENSURA_SALTUS]);
    CREDO_FALSUM (f.activae[CENSURA_TERTIA]);
    CREDO_AEQUALIS_I32 (f.saltus_maximus, ZEPHYRUM);
    CREDO_VERUM (f.ambitus == NIHIL);
    per (r = CENSURA_LINEAE; r < CENSURA_REGULAE;
         r = (CensuraRegula)(r + I))
    {
        CREDO_NECESSE (censura_titulus(r) != NIHIL);
        CREDO_VERUM (censura_regulam_invenire(
            chorda_ex_literis(censura_titulus(r), c->piscina),
            &inventa));
        CREDO_VERUM (inventa == r);
    }
    CREDO_CHORDA_AEQUALIS_LITERIS (chorda_ex_literis(
        censura_titulus(CENSURA_DISSONANTIA), c->piscina),
        "dissonantia");
    CREDO_VERUM (censura_titulus(CENSURA_REGULAE) == NIHIL);
    CREDO_FALSUM (censura_regulam_invenire(
        chorda_ex_literis("nulla", c->piscina), &inventa));
    CREDO_VERUM (censura_ambitus_ordinarius(NIHIL,
        chorda_ex_literis("quadrans", c->piscina), &infima, &summa));
    CREDO_AEQUALIS_I32 (infima, XLVIII);
    CREDO_AEQUALIS_I32 (summa, XCVI);
    CREDO_VERUM (censura_ambitus_ordinarius(NIHIL,
        chorda_ex_literis("triangulum", c->piscina), &infima, &summa));
    CREDO_AEQUALIS_I32 (infima, XXIV);
    CREDO_AEQUALIS_I32 (summa, LXXII);
    CREDO_VERUM (censura_ambitus_ordinarius(NIHIL,
        chorda_ex_literis("chorda", c->piscina), &infima, &summa));
    CREDO_AEQUALIS_I32 (infima, XXVIII);
    CREDO_AEQUALIS_I32 (summa, LXXXVIII);
    CREDO_VERUM (censura_ambitus_ordinarius(NIHIL,
        chorda_ex_literis("campana", c->piscina), &infima, &summa));
    CREDO_AEQUALIS_I32 (infima, XLVIII);
    CREDO_FALSUM (censura_ambitus_ordinarius(NIHIL,
        chorda_ex_literis("meum", c->piscina), &infima, &summa));
}


    /* ==================================================
     * LINEAE: '|' extra lineam; recta in metro mutato (3/4)
     * ================================================== */

interior vacuum
_sectio_lineae (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"l\">"
        "<metrum mensura=\"2\" valor=\"3/4\"/>"
        "<pars titulus=\"m\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\">"
        "<textus!>o4 c d e f | g a b | c2 | c</textus></frustum></pars>"
        "<pars titulus=\"n\" instrumentum=\"aether\"><frustum "
        "mensura=\"2\">"
        "<textus!>| c2. | c</textus></frustum></pars>"
        "</carmen></carmina>");
    CensuraInventum inv[CAPACITAS];

    CREDO_NECESSE (k != NIHIL);
    /* ordinaria: solum '|' post c2 (mensura 3 + dimidia); 'n' frustum
     * a mensura 2: '|' eius ad pulsum frusti + initium frusti */
    CREDO_NECESSE (censura_iudicare(k, NIHIL, inv, CAPACITAS) == I);
    CREDO_VERUM (inv[ZEPHYRUM].regula == CENSURA_LINEAE);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].mensura, III);
    CREDO_AEQUALIS_S64 (inv[ZEPHYRUM].pulsus, (s64)MCMXX);
    CREDO_AEQUALIS_S32 (inv[ZEPHYRUM].frustum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].offset, XXIV);
    CREDO_VERUM (inv[ZEPHYRUM].nota == NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[ZEPHYRUM]),
        "mensura 3 +1920 m: '|' non in linea mensurae "
        "(frustum 0, offset 24)\n");
}


    /* ==================================================
     * CLAVIS: minor VI et VII elevati licent; clavis mutata; percussio
     * ================================================== */

interior vacuum
_sectio_clavis (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"k\" clavis=\"a minor\">"
        "<clavis mensura=\"2\" valor=\"c maior\"/>"
        "<pars titulus=\"m\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\">"
        "<textus!>l8 o4 a b >c d e f# g g# | g#2 f2 | c#2 @punctum c#2"
        "</textus></frustum></pars></carmen></carmina>");
    CensuraInventum inv[CAPACITAS];

    CREDO_NECESSE (k != NIHIL);
    CREDO_NECESSE (censura_iudicare(k, NIHIL, inv, CAPACITAS) == II);
    CREDO_VERUM (inv[ZEPHYRUM].regula == CENSURA_CLAVIS);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].mensura, II);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].nota->nota, LXXX);
    CREDO_AEQUALIS_I32 (inv[I].mensura, III);
    CREDO_AEQUALIS_I32 (inv[I].nota->nota, LXXIII);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[ZEPHYRUM]),
        "mensura 2 m: G#5 extra clavem c maior (frustum 0, offset "
        "27)\n");
}


    /* ==================================================
     * AMBITUS: ordinarius, instrumentum ignotum, ambitus proprius
     * ================================================== */

interior vacuum
_sectio_ambitus (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"a\">"
        "<pars titulus=\"q\" instrumentum=\"quadrans\"><frustum "
        "mensura=\"1\">"
        "<textus!>o2 c o4 c r4 o5 c</textus></frustum></pars>"
        "<pars titulus=\"u\" instrumentum=\"meum\"><frustum "
        "mensura=\"1\">"
        "<textus!>o0 c r2.</textus></frustum></pars>"
        "</carmen></carmina>");
    CensuraInventum inv[CAPACITAS];
       CensuraForma f;

    CREDO_NECESSE (k != NIHIL);
    CREDO_NECESSE (censura_iudicare(k, NIHIL, inv, CAPACITAS) == I);
    CREDO_VERUM (inv[ZEPHYRUM].regula == CENSURA_AMBITUS);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].nota->nota, XXXVI);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[ZEPHYRUM]),
        "mensura 1 q: C2 extra ambitum quadrans (frustum 0, offset "
        "3)\n");
    /* ambitus proprius: etiam instrumentum 'meum' iudicatur */
    f          = _sola(CENSURA_AMBITUS);
    f.ambitus  = _ambitus_angustus;
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == III);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].nota->nota, XXXVI);
    CREDO_AEQUALIS_I32 (inv[I].nota->nota, XII);
    CREDO_CHORDA_AEQUALIS_LITERIS (inv[I].pars, "u");
    /* supra ambitum quoque (C5 > LXII) */
    CREDO_AEQUALIS_I32 (inv[II].nota->nota, LXXII);
}


    /* ==================================================
     * DISSONANTIA: tempora fortia (4/4, 6/8), tritonus debilis non
     * ================================================== */

interior vacuum
_sectio_dissonantia (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"d\" clavis=\"g maior\">"
        "<pars titulus=\"melodia\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o5 d4 f#4 b4 c4</textus>"
        "</frustum></pars>"
        "<pars titulus=\"bassus\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o3 c1</textus></frustum>"
        "</pars>"
        "</carmen></carmina>");
    constans Carmen* sex = _carmen(c,
        "<carmina><carmen titulus=\"s\" metrum=\"6/8\">"
        "<pars titulus=\"melodia\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o5 d4. d4.</textus></frustum>"
        "</pars><pars titulus=\"bassus\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o3 c2.</textus></frustum>"
        "</pars>"
        "</carmen></carmina>");
    constans Carmen* duodecim = _carmen(c,
        "<carmina><carmen titulus=\"x\" metrum=\"12/8\">"
        "<pars titulus=\"melodia\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o5 d4. d4. d4. d4.</textus>"
        "</frustum></pars>"
        "<pars titulus=\"bassus\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o3 c1.</textus></frustum>"
        "</pars></carmen></carmina>");
    constans Carmen* tres = _carmen(c,
        "<carmina><carmen titulus=\"t\" metrum=\"3/4\">"
        "<pars titulus=\"melodia\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o5 g4 d4 d4</textus>"
        "</frustum></pars>"
        "<pars titulus=\"bassus\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o3 c2.</textus></frustum>"
        "</pars></carmen></carmina>");
    CensuraInventum inv[CAPACITAS];
       CensuraForma f = _sola(CENSURA_DISSONANTIA);

    CREDO_NECESSE (k != NIHIL && sex != NIHIL && duodecim != NIHIL);
    /* dissonantia ORD. (N2): D5 et B5 saltibus sine explicatione */
    CREDO_AEQUALIS_I32 (censura_iudicare(k, NIHIL, inv, CAPACITAS), II);
    /* G maior (F# in clave: ordinaria tacet); D5/C3 ad initium, B5/C3
     * ad medium; F#5 (tritonus) debilis */
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == II);
    CREDO_AEQUALIS_S64 (inv[ZEPHYRUM].pulsus, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].nota->nota, LXXIV);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].nota_altera->nota, XLVIII);
    CREDO_CHORDA_AEQUALIS_LITERIS (inv[ZEPHYRUM].pars_altera, "bassus");
    CREDO_AEQUALIS_S64 (inv[I].pulsus, (s64)MCMXX);
    CREDO_AEQUALIS_I32 (inv[I].nota->nota, LXXXIII);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[ZEPHYRUM]),
        "mensura 1 melodia/bassus: D5 contra C3, secunda - nota sine "
        "explicatione (frustum 0, offset 3)\n");
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[I]),
        "mensura 1 +1920 melodia/bassus: B5 contra C3, septima - nota "
        "sine explicatione (frustum 0, offset 10)\n");
    /* 6/8: tempora fortia ad 0 et tertiam octavam */
    CREDO_NECESSE (censura_iudicare(sex, &f, inv, CAPACITAS) == II);
    CREDO_AEQUALIS_S64 (inv[I].pulsus, (s64)MCDXL);
    /* 12/8: quattuor tempora fortia (0, 3, 6, 9 octavae) */
    CREDO_NECESSE (censura_iudicare(duodecim, &f, inv, CAPACITAS)
        == IV);
    CREDO_AEQUALIS_S64 (inv[III].pulsus, (s64)III * MCDXL);
    /* 3/4 simplex: initium solum forte - D5/C3 in temporibus II et III
     * (debilibus) non, G5/C3 (quinta) in initio */
    CREDO_NECESSE (tres != NIHIL);
    CREDO_AEQUALIS_I32 (censura_iudicare(tres, &f, inv, CAPACITAS),
        ZEPHYRUM);
}


    /* ==================================================
     * PARALLELAE: quintae, octavae; motus contrarius non
     * ================================================== */

interior vacuum
_sectio_parallelae (
    CredoContextus* c)
{
    Carmen* k = (Carmen*)(vacuum*)_carmen(c,
        "<carmina><carmen titulus=\"p\">"
        "<pars titulus=\"melodia\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o5 c d e r4 | c d r2 | "
        "g c r2 | o4 g >c <a >d</textus></frustum></pars>"
        "<pars titulus=\"bassus\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o4 f g c r4 | o3 c d r2 | "
        "o4 c f r2 | o4 f r4 g r4</textus></frustum></pars>"
        "</carmen></carmina>");
    CensuraInventum inv[CAPACITAS];
       CensuraForma f = _sola(CENSURA_PARALLELAE);

    CarmenNota* n;

    CREDO_NECESSE (k != NIHIL);
    /* mensura 4: acervi [G4 C5] et [A4 D5] (editio, non MML) - summae
     * C5/F4 et D5/G4 quintae parallelae; imae (G4/F4, A4/G4) non */
    n               = k->partes[ZEPHYRUM].frusta[ZEPHYRUM].notae;
    n[VIII].pulsus  = n[VII].pulsus;
    n[X].pulsus     = n[IX].pulsus;
    CREDO_AEQUALIS_I32 (censura_iudicare(k, NIHIL, inv, CAPACITAS),
        ZEPHYRUM);
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == III);
    CREDO_AEQUALIS_I32 (inv[II].mensura, IV);
    CREDO_AEQUALIS_S64 (inv[II].pulsus, (s64)MCMXX);
    CREDO_AEQUALIS_I32 (inv[II].nota->nota, LXXIV);
    CREDO_AEQUALIS_I32 (inv[II].nota_altera->nota, LXVII);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].mensura, I);
    CREDO_AEQUALIS_S64 (inv[ZEPHYRUM].pulsus, (s64)CMLX);
    CREDO_AEQUALIS_I32 (inv[I].mensura, II);
    CREDO_AEQUALIS_S64 (inv[I].pulsus, (s64)CMLX);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[ZEPHYRUM]),
        "mensura 1 +960 melodia/bassus: quintae parallelae "
        "C5/F4 -> D5/G4 (frustum 0, offset 5)\n");
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[I]),
        "mensura 2 +960 melodia/bassus: octavae parallelae "
        "C5/C3 -> D5/D3 (frustum 0, offset 16)\n");
}


/* tertia: D3 A4 D5 ad initium (sic), F4 ad medium (non); D A D in
 * tempore debili (non); A2 G4 E5 (sic), A2 G4 C#5 (non: A7); notae
 * sustentae ad medium (sic); duae voces sine tertia (non); infimae
 * aequales: pars prior; acordum sustentum semel */
interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"t\" clavis=\"d minor\">"
        "<pars titulus=\"melodia\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o5 d1 | d1 | e2 c#2 | f2 d2 |"
        " r1 | d1</textus></frustum></pars>"
        "<pars titulus=\"media\" instrumentum=\"quadrans\">"
        "<frustum mensura=\"1\"><textus!>o4 a2 f2 | f4 a4 f2 | g2 g2 |"
        " a1 | a1 | o3 d1</textus></frustum></pars>"
        "<pars titulus=\"bassus\" instrumentum=\"triangulum\">"
        "<frustum mensura=\"1\"><textus!>o3 d1 | d1 | o2 a1 | o3 d1 |"
        " d1 | d1</textus></frustum></pars>"
        "</carmen></carmina>");
    CensuraInventum inv[CAPACITAS];
       CensuraForma f = _sola(CENSURA_TERTIA);

    CREDO_NECESSE (k != NIHIL);
    /* tertia non ORD.: ordinaria nullum inventum 'tertia' reddit
     * (dissonantia ORD. alia invenire potest) */
    {
        i32 n = censura_iudicare(k, NIHIL, inv, CAPACITAS);
        i32 q;

        per (q = ZEPHYRUM; q < n && q < CAPACITAS; q++)
        {
            CREDO_VERUM (inv[q].regula != CENSURA_TERTIA);
        }
    }
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == IV);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].mensura, I);
    CREDO_AEQUALIS_S64 (inv[ZEPHYRUM].pulsus, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (inv[ZEPHYRUM].pars, "bassus");
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].nota->nota, L);
    CREDO_VERUM (inv[ZEPHYRUM].nota_altera == NIHIL);
    CREDO_AEQUALIS_I32 (inv[I].mensura, III);
    CREDO_AEQUALIS_S64 (inv[I].pulsus, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (inv[II].mensura, IV);
    CREDO_AEQUALIS_S64 (inv[II].pulsus, (s64)MCMXX);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[ZEPHYRUM]),
        "mensura 1 bassus: voces 3 sine tertia: D3 A4 D5 "
        "(frustum 0, offset 3)\n");
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[I]),
        "mensura 3 bassus: voces 3 sine tertia: A2 G4 E5 "
        "(frustum 0, offset 16)\n");
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[II]),
        "mensura 4 +1920 bassus: voces 3 sine tertia: D3 A4 D5 "
        "(frustum 0, offset 24)\n");
    /* infimae aequales (D3 media, D3 bassus): pars prior nominatur;
     * acordum sustentum ad medium mensurae 6 non iteratur */
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[III]),
        "mensura 6 media: voces 3 sine tertia: D3 D3 D5 "
        "(frustum 0, offset 43)\n");
}


    /* ==================================================
     * ORDO ET CAPACITAS: inventa ordine temporis; numerus totus
     * ================================================== */

interior vacuum
_sectio_capacitas (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"c\">"
        "<pars titulus=\"q\" instrumentum=\"quadrans\"><frustum "
        "mensura=\"1\">"
        "<textus!>o2 c o4 c# r2 | l8 c# d# f# g# a# r4.</textus>"
        "</frustum></pars></carmen></carmina>");
    CensuraInventum inv[CAPACITAS];

    CREDO_NECESSE (k != NIHIL);
    /* ambitus (C2, +0) ante clavem (C#4, +960) - ordo temporis, non
     * regularum */
    CREDO_NECESSE (censura_iudicare(k, NIHIL, inv, CAPACITAS) == VII);
    CREDO_VERUM (inv[ZEPHYRUM].regula == CENSURA_AMBITUS);
    CREDO_VERUM (inv[I].regula == CENSURA_CLAVIS);
    CREDO_AEQUALIS_S64 (inv[I].pulsus, (s64)CMLX);
    CREDO_AEQUALIS_I32 (inv[II].mensura, II);
    CREDO_AEQUALIS_S64 (inv[VI].pulsus, (s64)MCMXX);
    /* capacitas II: numerus totus, II servata */
    memset(inv, ZEPHYRUM, magnitudo(inv));
    CREDO_AEQUALIS_I32 (censura_iudicare(k, NIHIL, inv, II), VII);
    CREDO_VERUM (inv[ZEPHYRUM].nota != NIHIL);
    CREDO_VERUM (inv[I].nota != NIHIL);
    CREDO_VERUM (inv[II].nota == NIHIL);
}


    /* ==================================================
     * LIBER: Korobeiniki ordinaria sine inventis
     * ================================================== */

interior vacuum
_sectio_liber (
    CredoContextus* c)
{
            Carmina  liber;
     CarminisVitium  vitium;
    constans Carmen* k;
    CensuraInventum  inv[CAPACITAS];
       CensuraForma  f;

    CREDO_NECESSE (carmina_ordinaria(c->piscina, &liber, &vitium));
    k = carmina_invenire(&liber, chorda_ex_literis("korobeiniki",
        c->piscina));
    CREDO_NECESSE (k != NIHIL);
    CREDO_AEQUALIS_I32 (censura_iudicare(k, NIHIL, inv, CAPACITAS),
        ZEPHYRUM);
    /* septimae E7 (tritonus G#/D) explicatae, transitus tacent (N2;
     * ante V inventa) */
    f = _sola(CENSURA_DISSONANTIA);
    CREDO_AEQUALIS_I32 (censura_iudicare(k, &f, inv, CAPACITAS),
        ZEPHYRUM);
    /* bassus octavis alternantibus: VII saltus veri (ad XIX) - causa
     * cur 'saltus' non ORD. */
    f = _sola(CENSURA_SALTUS);
    CREDO_AEQUALIS_I32 (censura_iudicare(k, &f, inv, CAPACITAS), VII);
    /* stella: ordinaria munda - septima A7 non resoluta (mensura XIV)
     * in tempore III (debili) iacet, dissonantia solum fortia iudicat;
     * appoggiatura/transitus mensurae VII explicati */
    k = carmina_invenire(&liber, chorda_ex_literis("stella",
        c->piscina));
    CREDO_NECESSE (k != NIHIL);
    CREDO_AEQUALIS_I32 (censura_iudicare(k, NIHIL, inv, CAPACITAS),
        ZEPHYRUM);
}


    /* ==================================================
     * SALTUS: ultra octavam (etiam trans pausam); octava ipsa non;
     * percussio non; limes proprius
     * ================================================== */

interior vacuum
_sectio_saltus (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"s\">"
        "<pars titulus=\"m\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 c >c d r4 | o4 e >>c @punctum "
        "o2 c r4</textus></frustum></pars></carmen></carmina>");
    Carmen* acervus = (Carmen*)(vacuum*)_carmen(c,
        "<carmina><carmen titulus=\"a\">"
        "<pars titulus=\"m\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 c >>c r2</textus></frustum></pars>"
        "</carmen></carmina>");
    CensuraInventum inv[CAPACITAS];
       CensuraForma f = _sola(CENSURA_SALTUS);

    CREDO_NECESSE (k != NIHIL && acervus != NIHIL);
    /* C4 -> C6 saltus; idem acervus [C4 C6] (editio) - non saltus */
    CREDO_AEQUALIS_I32 (censura_iudicare(acervus, &f, inv, CAPACITAS),
        I);
    acervus->partes[ZEPHYRUM].frusta[ZEPHYRUM].notae[I].pulsus
        = ZEPHYRUM;
    CREDO_AEQUALIS_I32 (censura_iudicare(acervus, &f, inv, CAPACITAS),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (censura_iudicare(k, NIHIL, inv, CAPACITAS),
        ZEPHYRUM);
    /* C4 -> C5 (XII) licet; D5 -> (pausa) E4 = X; E4 -> C6 = XX;
     * percussio (C2) non iudicatur */
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == I);
    CREDO_VERUM (inv[ZEPHYRUM].regula == CENSURA_SALTUS);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].mensura, II);
    CREDO_AEQUALIS_S64 (inv[ZEPHYRUM].pulsus, (s64)CMLX);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].nota->nota, LXXXIV);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].nota_prior->nota, LXIV);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[ZEPHYRUM]),
        "mensura 2 +960 m: saltus E4 -> C6, 20 semitonia "
        "(frustum 0, offset 22)\n");
    /* limes proprius: XX licet, XIX non */
    f.saltus_maximus = XX;
    CREDO_AEQUALIS_I32 (censura_iudicare(k, &f, inv, CAPACITAS),
        ZEPHYRUM);
    f.saltus_maximus = XIX;
    CREDO_AEQUALIS_I32 (censura_iudicare(k, &f, inv, CAPACITAS), I);
}

/* dissonantia per explicationem: transitus, suspensio, septima
 * resolutae tacent; non resolutae causam nominant */
/* carmen trium partium (bassus, media, melodia) */
interior constans Carmen*
_tres_partes (
        CredoContextus* c,
    constans character* bassus,
    constans character* media,
    constans character* melodia)
{
    character* s = (character*)piscina_allocare(c->piscina, M);

    sprintf(s, "<carmina><carmen titulus=\"e\">"
            "<pars titulus=\"bassus\" instrumentum=\"aether\"><frustum "
            "mensura=\"1\"><textus!>%s</textus></frustum></pars>"
            "<pars titulus=\"media\" instrumentum=\"aether\"><frustum "
            "mensura=\"1\"><textus!>%s</textus></frustum></pars>"
            "<pars titulus=\"melodia\" instrumentum=\"aether\">"
            "<frustum mensura=\"1\"><textus!>%s</textus></frustum>"
            "</pars></carmen></carmina>", bassus, media, melodia);
    redde _carmen(c, s);
}

/* inventa dissonantiae solius; XCIX si carmen non legitur */
interior i32
_dissonantiae (
        CredoContextus* c,
    constans character* bassus,
    constans character* media,
    constans character* melodia,
       CensuraInventum* inv)
{
    constans Carmen* k = _tres_partes(c, bassus, media, melodia);
       CensuraForma  f = _sola(CENSURA_DISSONANTIA);

    si (k == NIHIL)
    {
        redde XCIX;
    }
    redde censura_iudicare(k, &f, inv, CAPACITAS);
}

interior vacuum
_sectio_explicata (
    CredoContextus* c)
{
    CensuraInventum inv[CAPACITAS];

    /* D5 transitus in tempore III (forte) contra C3: tacet */
    CREDO_AEQUALIS_I32 (_dissonantiae(c, "o3 c1", "o4 c1",
        "o5 f4 e4 d4 c4", inv), ZEPHYRUM);
    /* C5 suspensio super G (contra D4) gradu resoluta: tacet */
    CREDO_AEQUALIS_I32 (_dissonantiae(c, "o3 f2 g2", "o4 a2 d2",
        "o5 c2. <b4", inv), ZEPHYRUM);
    /* non resoluta (G4 saltu): C5 contra B4 */
    CREDO_NECESSE (_dissonantiae(c, "o3 f2 g2", "o4 a2 b2",
        "o5 c2. <g4", inv) == I);
    CREDO_LITERAE_AEQUALES (inv[ZEPHYRUM].causa,
        "suspensio non resoluta");
    /* G7 -> C: F4 septima gradu descendit - tacet (etiam tritonus
     * B4/F4) */
    CREDO_AEQUALIS_I32 (_dissonantiae(c, "o2 g1 | o3 c1", "o4 f1 | e1",
        "o4 b2 >d2 | c1", inv), ZEPHYRUM);
    /* F4 -> G4: septima non resoluta - F4/G2 (tempora I et III) et
     * B4/F4 (I; in III melodia D5) */
    CREDO_NECESSE (_dissonantiae(c, "o2 g1 | o3 c1", "o4 f1 | g1",
        "o4 b2 >d2 | c1", inv) == III);
    CREDO_LITERAE_AEQUALES (inv[ZEPHYRUM].causa,
        "septima non resoluta");
    /* intervallum ab altitudine absoluta: pars inferior prima -
     * septima, non 'secunda' (G2 sub F4) */
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, _tres_partes(c,
        "o2 g1 | o3 c1", "o4 f1 | g1", "o4 b2 >d2 | c1"),
        &inv[ZEPHYRUM]),
        "mensura 1 bassus/media: G2 contra F4, septima - septima non "
        "resoluta (frustum 0, offset 3)\n");
    /* G#dim: tritonus acordi sui tacet */
    CREDO_AEQUALIS_I32 (_dissonantiae(c, "o2 g#1", "o4 b1", "o5 d1",
        inv), ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "ordinaria", _sectio_ordinaria, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lineae", _sectio_lineae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "clavis", _sectio_clavis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ambitus", _sectio_ambitus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "dissonantia", _sectio_dissonantia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "parallelae", _sectio_parallelae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "saltus", _sectio_saltus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tertia", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "explicata", _sectio_explicata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "capacitas", _sectio_capacitas, NIHIL, NIHIL, NIHIL, NIHIL },
    { "liber", _sectio_liber, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("censura", SECTIONES);
}
