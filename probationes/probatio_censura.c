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
    CREDO_VERUM (f.activae[CENSURA_ACORDA]);
    CREDO_FALSUM (f.activae[CENSURA_EVITANDA]);
    CREDO_FALSUM (f.activae[CENSURA_OCCULTAE]);
    CREDO_FALSUM (f.activae[CENSURA_DECUSSATIO]);
    CREDO_FALSUM (f.activae[CENSURA_SPATIUM]);
    CREDO_FALSUM (f.intentiones_liberae);
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
    CREDO_VERUM (censura_ambitus_ordinarius(NIHIL,
        chorda_ex_literis("organum", c->piscina), &infima, &summa));
    CREDO_AEQUALIS_I32 (infima, XXXVI);
    CREDO_AEQUALIS_I32 (summa, XCVI);
    CREDO_VERUM (censura_ambitus_ordinarius(NIHIL,
        chorda_ex_literis("marimba", c->piscina), &infima, &summa));
    CREDO_AEQUALIS_I32 (infima, XLV);
    CREDO_AEQUALIS_I32 (summa, XCVI);
    CREDO_VERUM (censura_ambitus_ordinarius(NIHIL,
        chorda_ex_literis("glockenspiel", c->piscina), &infima,
        &summa));
    CREDO_AEQUALIS_I32 (infima, LXXIX);
    CREDO_AEQUALIS_I32 (summa, CVIII);
    CREDO_VERUM (censura_ambitus_ordinarius(NIHIL,
        chorda_ex_literis("schola", c->piscina), &infima, &summa));
    CREDO_AEQUALIS_I32 (infima, XLVIII);
    CREDO_AEQUALIS_I32 (summa, LXXIV);
    CREDO_VERUM (censura_ambitus_ordinarius(NIHIL,
        chorda_ex_literis("fistula", c->piscina), &infima, &summa));
    CREDO_AEQUALIS_I32 (infima, LX);
    CREDO_AEQUALIS_I32 (summa, XCVI);
    CREDO_VERUM (censura_ambitus_ordinarius(NIHIL,
        chorda_ex_literis("nubes", c->piscina), &infima, &summa));
    CREDO_AEQUALIS_I32 (infima, XXXVI);
    CREDO_AEQUALIS_I32 (summa, LXXXIV);
    CREDO_FALSUM (censura_ambitus_ordinarius(NIHIL,
        chorda_ex_literis("meum", c->piscina), &infima, &summa));
    /* ambitus instrumenti libri: B3 extra 60..72, C4 intra;
     * instrumentum
     * libri sine ambitu nec in tabula: tacet */
    {
        constans Carmen* k = _carmen(c,
            "<carmina><instrumentum titulus=\"angustum\" "
            "basis=\"aether\""
            " ambitus=\"60..72\"/><instrumentum titulus=\"liberum\"/>"
            "<carmen titulus=\"am\"><pars titulus=\"p\" "
            "instrumentum=\"angustum\"><frustum mensura=\"1\"><textus!>"
            "o3 b4 >c4 c2</textus></frustum></pars><pars titulus=\"q\" "
            "instrumentum=\"liberum\"><frustum mensura=\"1\"><textus!>"
            "o1 c1</textus></frustum></pars><pars titulus=\"r\" "
            "instrumentum=\"aether\"><frustum mensura=\"1\"><textus!>"
            "o1 c1</textus></frustum></pars></carmen></carmina>");
        CensuraInventum inv[CAPACITAS];
           CensuraForma f = _sola(CENSURA_AMBITUS);

        /* 'aether' (praeparatum) non redefinitum: tabula (36..96) - C1
         * extra */
        CREDO_NECESSE (k != NIHIL);
        CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == II);
        CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].nota->nota, LIX);
    }
    /* instrumentarium ordinarium: campanae_tubulares 60..77 - C4
     * intra, G5 extra */
    {
        constans Carmen* k = _carmen(c,
            "<carmina><carmen titulus=\"ct\"><pars titulus=\"p\" "
            "instrumentum=\"campanae_tubulares\">"
            "<frustum mensura=\"1\">"
            "<textus!>o4 c2 o5 g2</textus></frustum></pars></carmen>"
            "</carmina>");
        CensuraInventum inv[CAPACITAS];
           CensuraForma f = _sola(CENSURA_AMBITUS);

        CREDO_NECESSE (k != NIHIL);
        CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == I);
        CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].nota->nota, LXXIX);
    }
    /* praeparatum redefinitum SINE ambitu: tabula manet (non 0..127) */
    {
        constans Carmen* k = _carmen(c,
            "<carmina><instrumentum titulus=\"aether\" "
            "basis=\"quadrans\""
            "/><carmen titulus=\"rd\"><pars titulus=\"p\" "
            "instrumentum=\"aether\"><frustum mensura=\"1\"><textus!>"
            "o1 c1</textus></frustum></pars></carmen></carmina>");
        CensuraInventum inv[CAPACITAS];
           CensuraForma f = _sola(CENSURA_AMBITUS);

        CREDO_NECESSE (k != NIHIL);
        CREDO_AEQUALIS_I32 (censura_iudicare(k, &f, inv, CAPACITAS), I);
    }
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


/* clavis caerulea: toni caerulei (b3 b5 b7 tonicae) in clave - F:
 * Ab, B, Eb; F# extra, nomen clavis cum modo */
interior vacuum
_sectio_caerulea (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"b\" clavis=\"f maior caerulea\">"
        "<pars titulus=\"m\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\">"
        "<textus!>o4 f8 a-8 b8 c8 e-8 f#8 f4</textus></frustum></pars>"
        "</carmen></carmina>");
    constans Carmen* m = _carmen(c,
        "<carmina><carmen titulus=\"n\" clavis=\"a minor caerulea\">"
        "<pars titulus=\"m\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\">"
        "<textus!>o4 a4 d#4 b-4 a4</textus></frustum></pars>"
        "</carmen></carmina>");
       CensuraForma f = _sola(CENSURA_CLAVIS);
    CensuraInventum inv[CAPACITAS];

    CREDO_NECESSE (k != NIHIL && m != NIHIL);
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == I);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[ZEPHYRUM]),
        "mensura 1 +2400 m: F#4 extra clavem f maior caerulea (frustum "
        "0, offset 20)\n");
    /* minor: b5 (D#) caerulea a tonica A; Bb (b7 maioris relativae C)
     * non */
    CREDO_NECESSE (censura_iudicare(m, &f, inv, CAPACITAS) == I);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].nota->nota, LXX);
}


/* ornamentum chromaticum: nota extra clavem quae transitus, vicinus
 * aut appoggiatura resoluta est et semitonio exit licet; gradu toto
 * exiens aut sine forma nominatur */
interior constans Carmen*
_melodia_super_c (
        CredoContextus* c,
    constans character* melodia)
{
    character* s = (character*)piscina_allocare(c->piscina, M);

    sprintf(s, "<carmina><carmen titulus=\"o\" clavis=\"c maior\">"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 c1</textus></frustum></pars>"
        "<pars titulus=\"t\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 e1</textus></frustum></pars>"
        "<pars titulus=\"m\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>%s</textus></frustum></pars>"
        "</carmen></carmina>", melodia);
    redde _carmen(c, s);
}

interior vacuum
_sectio_chromatica (
    CredoContextus* c)
{
       CensuraForma f = _sola(CENSURA_CLAVIS);
    CensuraInventum inv[CAPACITAS];

    /* transitus C C# D, vicinus G F# G, appoggiatura C D# E: licent */
    CREDO_AEQUALIS_I32 (censura_iudicare(_melodia_super_c(c,
        "o5 c4 c#4 d4 e4"), &f, inv, CAPACITAS), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (censura_iudicare(_melodia_super_c(c,
        "o5 g4 f#4 g4 e4"), &f, inv, CAPACITAS), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (censura_iudicare(_melodia_super_c(c,
        "o5 c4 d#4 e4 g4"), &f, inv, CAPACITAS), ZEPHYRUM);
    /* vicinus E F# E: gradu toto exit - nominatur */
    CREDO_NECESSE (censura_iudicare(_melodia_super_c(c,
        "o5 e4 f#4 e4 g4"), &f, inv, CAPACITAS) == I);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].nota->nota, LXXVIII);
    /* transitus A Bb C: gradu toto exit - nominatur */
    CREDO_NECESSE (censura_iudicare(_melodia_super_c(c,
        "o5 a4 b-4 >c4 <g4"), &f, inv, CAPACITAS) == I);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].nota->nota, LXXXII);
    /* sine forma (saltu intrat et exit) - nominatur */
    CREDO_NECESSE (censura_iudicare(_melodia_super_c(c,
        "o5 g4 >c#4 <e4 g4"), &f, inv, CAPACITAS) == I);
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


/* tertia: D3 A4 D5 ad initium, F4 ad medium - fenestra Dm (non); D A
 * D in tempore debili (non); A2 G4 E5 ante C#5 - fenestra A7 (non);
 * notae sustentae ad medium in fenestra Dm (non); duae voces sine
 * tertia (non); D3 D3 D5 in fenestra (D) (sic): infimae aequales =
 * pars prior; acordum sustentum semel */
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
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == I);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].mensura, VI);
    CREDO_AEQUALIS_S64 (inv[ZEPHYRUM].pulsus, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (inv[ZEPHYRUM].pars, "media");
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].nota->nota, L);
    CREDO_VERUM (inv[ZEPHYRUM].nota_altera == NIHIL);
    /* infimae aequales (D3 media, D3 bassus): pars prior nominatur;
     * acordum sustentum ad medium mensurae 6 non iteratur */
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[ZEPHYRUM]),
        "mensura 6 media: voces 3 sine tertia: D3 D3 D5 "
        "(frustum 0, offset 43)\n");
}

/* tertia per fenestram: acordum fractum (arpeggium) E3 B3 E3 G3
 * super E2 B3 (fenestra Em) non; mensura 2 sola E et B (fenestra
 * E(5), tertia demum in fenestra proxima) sic, ad initium et ad
 * medium (arpeggium iteratur); mensura 3 G ad medium (fenestra Em)
 * non */
interior vacuum
_sectio_fractum (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"a\" clavis=\"e minor\">"
        "<pars titulus=\"bassus\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o2 e1 | e1 | e1</textus></frustum>"
        "</pars><pars titulus=\"pad\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 b1 | b1 | b1</textus></frustum>"
        "</pars><pars titulus=\"arp\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 e8 b8 e8 g8 e8 b8 e8 g8 | e8 b8 e8 "
        "b8 e8 b8 e8 b8 | e2 g2</textus></frustum></pars>"
        "</carmen></carmina>");
    CensuraInventum inv[CAPACITAS];
       CensuraForma f = _sola(CENSURA_TERTIA);

    CREDO_NECESSE (k != NIHIL);
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == II);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[ZEPHYRUM]),
        "mensura 2 bassus: voces 3 sine tertia: E2 E3 B3 "
        "(frustum 0, offset 8)\n");
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[I]),
        "mensura 2 +1920 bassus: voces 3 sine tertia: E2 E3 B3 "
        "(frustum 0, offset 8)\n");
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

/* tertia Picardiae: F# acordi finalis D maioris in D minor licita (nec
 * medio: D -> Gm = V/iv, F# extra clavem nominatur) */
interior vacuum
_sectio_picardia (
    CredoContextus* c)
{
    constans Carmen* finalis = _carmen(c,
        "<carmina><carmen titulus=\"p\" clavis=\"d minor\">"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 d1 | g1 | d1</textus></frustum>"
        "</pars><pars titulus=\"m\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 f1 | b-1 | f#1</textus></frustum>"
        "</pars><pars titulus=\"s\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 a1 | o5 d1 | o4 a1</textus>"
        "</frustum></pars></carmen></carmina>");
    constans Carmen* medio = _carmen(c,
        "<carmina><carmen titulus=\"p\" clavis=\"d minor\">"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 d1 | g1 | a1 | d1</textus></frustum>"
        "</pars><pars titulus=\"m\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 f#1 | b-1 | c#1 | f1</textus>"
        "</frustum></pars><pars titulus=\"s\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o4 a1 | o5 d1 | o4 e1 | a1"
        "</textus></frustum></pars></carmen></carmina>");
    CensuraInventum inv[CAPACITAS];
       CensuraForma f = _sola(CENSURA_CLAVIS);

    CREDO_NECESSE (finalis != NIHIL && medio != NIHIL);
    CREDO_AEQUALIS_I32 (censura_iudicare(finalis, &f, inv, CAPACITAS),
                        ZEPHYRUM);
    CREDO_NECESSE (censura_iudicare(medio, &f, inv, CAPACITAS) == I);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].nota->nota, LXVI);
}

/* harmonia declarata: clavis tonos acordi declarati admittit; acorda
 * discordiam nominat (nota sine forma extra acordum, tertia deest) */
interior vacuum
_sectio_acorda (
    CredoContextus* c)
{
    constans Carmen* k;
    CensuraInventum  inv[CAPACITAS];
       CensuraForma  f;
                i32  n;

    /* D7 declaratum in F: F# licet; C# (extra D7) non */
    k = _carmen(c, "<carmina><carmen titulus=\"k\" clavis=\"f maior\">"
        "<acorda mensura=\"1\" textus=\"F | D7 | Gm\"/>"
        "<pars titulus=\"m\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 a1 | f#2 c#2 | g1</textus>"
        "</frustum></pars></carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    f = _sola(CENSURA_CLAVIS);
    n = censura_iudicare(k, &f, inv, CAPACITAS);
    CREDO_NECESSE (n == I);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].nota->nota, LXI);
    /* 'r' declaratum nihil admittit: F# extra clavem */
    k = _carmen(c, "<carmina><carmen titulus=\"r\" clavis=\"f maior\">"
        "<acorda mensura=\"1\" textus=\"F | r | Gm\"/>"
        "<pars titulus=\"m\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 a1 | f#1 | g1</textus>"
        "</frustum></pars></carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    CREDO_AEQUALIS_I32 (censura_iudicare(k, &f, inv, CAPACITAS), I);
    /* Bm declaratum, A tenta sine forma: discordia */
    k = _carmen(c, "<carmina><carmen titulus=\"b\" clavis=\"d maior\">"
        "<acorda mensura=\"1\" textus=\"Bm\"/>"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 b1</textus></frustum></pars>"
        "<pars titulus=\"t\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 d1</textus></frustum></pars>"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 f#1</textus></frustum></pars>"
        "<pars titulus=\"s\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o5 a1</textus></frustum></pars>"
        "</carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    f = _sola(CENSURA_ACORDA);
    n = censura_iudicare(k, &f, inv, CAPACITAS);
    CREDO_NECESSE (n == I);
    CREDO_VERUM (inv[ZEPHYRUM].regula == CENSURA_ACORDA);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[ZEPHYRUM]),
        "mensura 1 s: Bm declaratum, sonat Bm7 (A5) (frustum 0, offset "
        "3)\n");
    /* C7 declaratum, E nusquam: tertia deest */
    k = _carmen(c, "<carmina><carmen titulus=\"t\">"
        "<acorda mensura=\"1\" textus=\"C7\"/>"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 c1</textus></frustum></pars>"
        "<pars titulus=\"t\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 g1</textus></frustum></pars>"
        "<pars titulus=\"s\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 b-1</textus></frustum></pars>"
        "</carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    n = censura_iudicare(k, &f, inv, CAPACITAS);
    CREDO_NECESSE (n == I);
    CREDO_CHORDA_CONTINET (_linea(c, k, &inv[ZEPHYRUM]),
        chorda_ex_literis("C7 declaratum, sonat ", c->piscina));
    CREDO_CHORDA_CONTINET (_linea(c, k, &inv[ZEPHYRUM]),
        chorda_ex_literis("(tertia deest)", c->piscina));
    /* C declaratum, C nusquam: radix deest */
    k = _carmen(c, "<carmina><carmen titulus=\"q\">"
        "<acorda mensura=\"1\" textus=\"C\"/>"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 e1</textus></frustum></pars>"
        "<pars titulus=\"s\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 g1</textus></frustum></pars>"
        "</carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == I);
    CREDO_CHORDA_CONTINET (_linea(c, k, &inv[ZEPHYRUM]),
        chorda_ex_literis("(radix deest)", c->piscina));
    /* duae alienae tentae: ambae nominantur */
    k = _carmen(c, "<carmina><carmen titulus=\"u\">"
        "<acorda mensura=\"1\" textus=\"C\"/>"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 c1</textus></frustum></pars>"
        "<pars titulus=\"t\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 e1</textus></frustum></pars>"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 a1</textus></frustum></pars>"
        "<pars titulus=\"s\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o5 d1</textus></frustum></pars>"
        "</carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == I);
    CREDO_CHORDA_CONTINET (_linea(c, k, &inv[ZEPHYRUM]),
        chorda_ex_literis("(A4 D5)", c->piscina));
    /* nota transiens super C declaratum: concordia */
    k = _carmen(c, "<carmina><carmen titulus=\"p\">"
        "<acorda mensura=\"1\" textus=\"C\"/>"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 c1</textus></frustum></pars>"
        "<pars titulus=\"t\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 g1</textus></frustum></pars>"
        "<pars titulus=\"s\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o5 c4 d4 e2</textus></frustum></pars>"
        "</carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    CREDO_AEQUALIS_I32 (censura_iudicare(k, &f, inv, CAPACITAS),
                        ZEPHYRUM);
}

/* stili: regulae ("+a,-b" mutat, "a,b" solae), catena basium (basis
 * prior), <censura regulae> carminis; septimae_liberae */
interior vacuum
_sectio_stili (
    CredoContextus* c)
{
       CensuraForma  f;
             chorda  ignota;
    constans Carmen* k;
    CensuraInventum  inv[CAPACITAS];

    censura_ordinaria(&f);
    CREDO_VERUM (censura_regulas_legere(chorda_ex_literis(
        "+parallelae,-ambitus", c->piscina), &f, &ignota));
    CREDO_VERUM (f.activae[CENSURA_PARALLELAE]);
    CREDO_FALSUM (f.activae[CENSURA_AMBITUS]);
    CREDO_VERUM (f.activae[CENSURA_CLAVIS]);
    CREDO_VERUM (censura_regulas_legere(chorda_ex_literis(
        "lineae,clavis", c->piscina), &f, &ignota));
    CREDO_VERUM (f.activae[CENSURA_LINEAE]
        && f.activae[CENSURA_CLAVIS]);
    CREDO_FALSUM (f.activae[CENSURA_DISSONANTIA]);
    CREDO_FALSUM (f.activae[CENSURA_PARALLELAE]);
    CREDO_FALSUM (censura_regulas_legere(chorda_ex_literis(
        "+tertia,-nulla", c->piscina), &f, &ignota));
    CREDO_CHORDA_AEQUALIS_LITERIS (ignota, "nulla");
    /* catena: popularis (+tertia, liberae) -> jazz (-tertia,
     * +parallelae) -> censura carminis (-lineae) */
    k = _carmen(c,
        "<carmina><stylus titulus=\"popularis\" septimae=\"liberae\" "
        "regulae=\"+tertia\"/>"
        "<stylus titulus=\"jazz\" basis=\"popularis\" "
        "regulae=\"-tertia,+parallelae\"/>"
        "<carmen titulus=\"a\" stylus=\"jazz\">"
        "<censura regulae=\"-lineae\"/></carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    CREDO_VERUM (censura_carminis_forma(k, &f, &ignota));
    CREDO_VERUM (f.septimae_liberae);
    CREDO_FALSUM (f.activae[CENSURA_TERTIA]);
    CREDO_VERUM (f.activae[CENSURA_PARALLELAE]);
    CREDO_FALSUM (f.activae[CENSURA_LINEAE]);
    CREDO_VERUM (f.activae[CENSURA_CLAVIS]);
    /* sine stylo: ordinaria */
    k = _carmen(c,
        "<carmina><carmen titulus=\"b\"></carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    CREDO_VERUM (censura_carminis_forma(k, &f, &ignota));
    CREDO_FALSUM (f.septimae_liberae);
    CREDO_VERUM (f.activae[CENSURA_LINEAE]);
    /* regula ignota in stylo */
    k = _carmen(c,
        "<carmina><stylus titulus=\"x\" regulae=\"+nulla\"/>"
        "<carmen titulus=\"d\" stylus=\"x\"></carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    CREDO_FALSUM (censura_carminis_forma(k, &f, &ignota));
    CREDO_CHORDA_AEQUALIS_LITERIS (ignota, "nulla");
    /* septimae liberae: septima Cmaj7 non resoluta tacet; nota aliena
     * sine forma (F#) manet */
    k = _carmen(c, "<carmina><carmen titulus=\"s\">"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 c1 | f1</textus></frustum></pars>"
        "<pars titulus=\"t\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 e1 | a1</textus></frustum></pars>"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 g1 | f1</textus></frustum></pars>"
        "<pars titulus=\"s\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 b1 | o5 c1</textus></frustum></pars>"
        "</carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    f = _sola(CENSURA_DISSONANTIA);
    CREDO_VERUM (censura_iudicare(k, &f, inv, CAPACITAS) > ZEPHYRUM);
    f.septimae_liberae = VERUM;
    CREDO_AEQUALIS_I32 (censura_iudicare(k, &f, inv, CAPACITAS),
                        ZEPHYRUM);
    k = _carmen(c, "<carmina><carmen titulus=\"g\">"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 c1</textus></frustum></pars>"
        "<pars titulus=\"t\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 e1</textus></frustum></pars>"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 g1</textus></frustum></pars>"
        "<pars titulus=\"s\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 f#1</textus></frustum></pars>"
        "</carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    CREDO_VERUM (censura_iudicare(k, &f, inv, CAPACITAS) > ZEPHYRUM);
}

/* stylus iazzicus: tensiones licitae acordi (13 super G7) tacent;
 * evitanda: 11 tenta super Fmaj7 nominatur, transiens non; tertia
 * super C7sus4 */
interior vacuum
_sectio_intentiones (
    CredoContextus* c)
{
       CensuraForma  f;
             chorda  ignota;
    constans Carmen* k;
    CensuraInventum  inv[CAPACITAS];

    k = _carmen(c,
        "<carmina><stylus titulus=\"popularis\" septimae=\"liberae\"/>"
        "<stylus titulus=\"jazz\" basis=\"popularis\" "
        "intentiones=\"liberae\" regulae=\"+evitanda\"/>"
        "<carmen titulus=\"a\" stylus=\"jazz\"></carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    CREDO_VERUM (censura_carminis_forma(k, &f, &ignota));
    CREDO_VERUM (f.intentiones_liberae && f.septimae_liberae);
    CREDO_VERUM (f.activae[CENSURA_EVITANDA]);
    /* G7 declaratum cum 13 (E5) tenta */
    k = _carmen(c, "<carmina><carmen titulus=\"g\">"
        "<acorda mensura=\"1\" textus=\"G7\"/>"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o2 g1</textus></frustum></pars>"
        "<pars titulus=\"t\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 b1</textus></frustum></pars>"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 f1</textus></frustum></pars>"
        "<pars titulus=\"s\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o5 e1</textus></frustum></pars>"
        "</carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    censura_ordinaria(&f);
    memset(f.activae, ZEPHYRUM, magnitudo(f.activae));
    f.activae[CENSURA_DISSONANTIA]  = VERUM;
    f.activae[CENSURA_ACORDA]       = VERUM;
    f.septimae_liberae              = VERUM;
    CREDO_VERUM (censura_iudicare(k, &f, inv, CAPACITAS) > ZEPHYRUM);
    f.intentiones_liberae = VERUM;
    CREDO_AEQUALIS_I32 (censura_iudicare(k, &f, inv, CAPACITAS),
                        ZEPHYRUM);
    /* Fmaj7 cum Bb tenta: evitanda */
    k = _carmen(c, "<carmina><carmen titulus=\"f\">"
        "<acorda mensura=\"1\" textus=\"Fmaj7\"/>"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o2 f1</textus></frustum></pars>"
        "<pars titulus=\"t\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 a1</textus></frustum></pars>"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 e1</textus></frustum></pars>"
        "<pars titulus=\"s\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 b-1</textus></frustum></pars>"
        "</carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    f                      = _sola(CENSURA_EVITANDA);
    f.intentiones_liberae  = VERUM;
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == I);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[ZEPHYRUM]),
        "mensura 1 s: Fmaj7: nota evitanda Bb4 (11) (frustum 0, offset "
        "3)\n");
    /* Bb transiens: licet */
    k = _carmen(c, "<carmina><carmen titulus=\"p\">"
        "<acorda mensura=\"1\" textus=\"Fmaj7\"/>"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o2 f1</textus></frustum></pars>"
        "<pars titulus=\"t\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 a1</textus></frustum></pars>"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 e1</textus></frustum></pars>"
        "<pars titulus=\"s\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 a4 b-4 o5 c2</textus></frustum>"
        "</pars>"
        "</carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    CREDO_AEQUALIS_I32 (censura_iudicare(k, &f, inv, CAPACITAS),
                        ZEPHYRUM);
    /* C7sus4 cum E tenta: tertia evitanda */
    k = _carmen(c, "<carmina><carmen titulus=\"s\">"
        "<acorda mensura=\"1\" textus=\"C7sus4\"/>"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 c1</textus></frustum></pars>"
        "<pars titulus=\"t\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 b-1</textus></frustum></pars>"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 f1</textus></frustum></pars>"
        "<pars titulus=\"s\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 e1</textus></frustum></pars>"
        "</carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == I);
    CREDO_CHORDA_CONTINET (_linea(c, k, &inv[ZEPHYRUM]),
        chorda_ex_literis("nota evitanda E4 (3)", c->piscina));
}

/* intentiones liberae quacumque figura: nona suspensa super G7 non
 * resolvenda; undecima (evitanda) suspensa resolvenda manet; clavis
 * intentionem acordi DECLARATI admittit (F7: Ab, #9), non sine
 * acordis */
interior vacuum
_sectio_intentiones_figurae (
    CredoContextus* c)
{
       CensuraForma  f;
    constans Carmen* k;
    CensuraInventum  inv[CAPACITAS];

    /* A3 ex Dm7 in G7 tenta: nona suspensa */
    k = _carmen(c, "<carmina><carmen titulus=\"n\">"
        "<acorda mensura=\"1\" textus=\"Dm7:2 G7:2\"/>"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o2 d2 g2</textus></frustum></pars>"
        "<pars titulus=\"t\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 a1</textus></frustum></pars>"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 f1</textus></frustum></pars>"
        "<pars titulus=\"s\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o5 c2 o4 b2</textus></frustum></pars>"
        "</carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    f                   = _sola(CENSURA_DISSONANTIA);
    f.septimae_liberae  = VERUM;
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == II);
    CREDO_CHORDA_CONTINET (_linea(c, k, &inv[ZEPHYRUM]),
        chorda_ex_literis("suspensio non resoluta", c->piscina));
    f.intentiones_liberae = VERUM;
    CREDO_AEQUALIS_I32 (censura_iudicare(k, &f, inv, CAPACITAS),
                        ZEPHYRUM);
    /* C5 ex C in G7 tenta: undecima, evitanda - resolvenda */
    k = _carmen(c, "<carmina><carmen titulus=\"q\">"
        "<acorda mensura=\"1\" textus=\"C:2 G7:2\"/>"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o2 c2 g2</textus></frustum></pars>"
        "<pars titulus=\"t\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 g2 b2</textus></frustum></pars>"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 e2 f2</textus></frustum></pars>"
        "<pars titulus=\"s\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o5 c1</textus></frustum></pars>"
        "</carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == I);
    CREDO_CHORDA_CONTINET (_linea(c, k, &inv[ZEPHYRUM]),
        chorda_ex_literis("suspensio non resoluta", c->piscina));
    /* F7 declaratum in F, Ab tenta (#9): clavis */
    k = _carmen(c, "<carmina><carmen titulus=\"k\" clavis=\"f maior\">"
        "<acorda mensura=\"1\" textus=\"F7\"/>"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o2 f1</textus></frustum></pars>"
        "<pars titulus=\"t\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 a1</textus></frustum></pars>"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 e-1</textus></frustum></pars>"
        "<pars titulus=\"s\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 a-1</textus></frustum></pars>"
        "</carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    f = _sola(CENSURA_CLAVIS);
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == I);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[ZEPHYRUM]),
        "mensura 1 s: Ab4 extra clavem f maior (frustum 0, offset "
        "3)\n");
    f.intentiones_liberae = VERUM;
    CREDO_AEQUALIS_I32 (censura_iudicare(k, &f, inv, CAPACITAS),
                        ZEPHYRUM);
    /* sine acordis: intentio nulla declarata - Eb et Ab extra clavem */
    k = _carmen(c, "<carmina><carmen titulus=\"u\" clavis=\"f maior\">"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o2 f1</textus></frustum></pars>"
        "<pars titulus=\"t\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 a1</textus></frustum></pars>"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 e-1</textus></frustum></pars>"
        "<pars titulus=\"s\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 a-1</textus></frustum></pars>"
        "</carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    CREDO_AEQUALIS_I32 (censura_iudicare(k, &f, inv, CAPACITAS), II);
}

/* occultae: inter voces extremas (superius, bassus; percussio ultima
 * non est vox): octavae saltu (m1), gradu licet (m2), contrariae (m3),
 * quintae saltu deorsum (m4), parallelae non occultae (m5), gradu
 * toto licet (m6: A4 B4 super C3 E3) */
interior vacuum
_sectio_occultae (
    CredoContextus* c)
{
    Carmen* k = (Carmen*)(vacuum*)_carmen(c,
        "<carmina><carmen titulus=\"o\">"
        "<pars titulus=\"superius\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>"
        "o4 b4 o5 e4 r2 | o4 b4 o5 c4 r2 | "
        "o5 c4 g4 r2 | o5 e4 o4 a4 r2 | o5 d4 e4 r2 | o4 a4 b4 r2"
        "</textus></frustum>"
        "</pars>"
        "<pars titulus=\"bassus\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>"
        "o2 g4 o3 e4 r2 | o2 g4 o3 c4 r2 | "
        "o3 e4 c4 r2 | o3 g4 d4 r2 | o3 g4 a4 r2 | c4 e4 r2"
        "</textus></frustum>"
        "</pars>"
        "<pars titulus=\"tympanum\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>@tympanum_grave o2 c4 c4 c4 c4"
        "</textus></frustum></pars>"
        "</carmen></carmina>");
    CensuraInventum inv[CAPACITAS];
       CensuraForma f = _sola(CENSURA_OCCULTAE);

    CREDO_NECESSE (k != NIHIL);
    CREDO_AEQUALIS_I32 (censura_iudicare(k, NIHIL, inv, CAPACITAS),
        ZEPHYRUM);
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == II);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].mensura, I);
    CREDO_AEQUALIS_S64 (inv[ZEPHYRUM].pulsus, (s64)CMLX);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].nota->nota, LXXVI);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].nota_altera->nota, LII);
    CREDO_AEQUALIS_I32 (inv[I].mensura, IV);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[ZEPHYRUM]),
        "mensura 1 +960 superius/bassus: octavae occultae "
        "B4/G2 -> E5/E3 (frustum 0, offset 9)\n");
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[I]),
        "mensura 4 +960 superius/bassus: quintae occultae "
        "E5/G3 -> A4/D3 (frustum 0, offset 57)\n");
    /* voces duae: spatium nullum (vox ima cum proxima libera) */
    f = _sola(CENSURA_SPATIUM);
    CREDO_AEQUALIS_I32 (censura_iudicare(k, &f, inv, CAPACITAS),
        ZEPHYRUM);
}

/* occultae solum inter extremas: m1 altus saltu ad octavam cum basso
 * (G4 -> C5 super G3 -> C4), superius tenet - nihil; m2 superius et
 * bassus (B4 -> E5, G2 -> E3), altus inter tenet - extremae */
interior vacuum
_sectio_occultae_mediae (
    CredoContextus* c)
{
    Carmen* k = (Carmen*)(vacuum*)_carmen(c,
        "<carmina><carmen titulus=\"o\">"
        "<pars titulus=\"superius\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o5 e2 e2 | o4 b4 o5 e4 r2"
        "</textus></frustum>"
        "</pars>"
        "<pars titulus=\"altus\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o4 g2 o5 c2 | o4 g2 r2"
        "</textus></frustum>"
        "</pars>"
        "<pars titulus=\"bassus\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o3 g2 o4 c2 | o2 g4 o3 e4 r2"
        "</textus></frustum></pars>"
        "</carmen></carmina>");
    CensuraInventum inv[CAPACITAS];
       CensuraForma f = _sola(CENSURA_OCCULTAE);

    CREDO_NECESSE (k != NIHIL);
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == I);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].mensura, II);
    CREDO_CHORDA_AEQUALIS_LITERIS (inv[ZEPHYRUM].pars_altera,
        "bassus");
}

/* decussatio: voces proximae (percussio et pars vacua inter eas non
 * sunt voces); m1 altus B4 super G4 bis - semel; m2 unisonus non; m3
 * altus acervus [E4 B4] sub A4: summa inferioris; m4 superius acervus
 * [F4 C5] super A4: infima superioris */
interior vacuum
_sectio_decussatio (
    CredoContextus* c)
{
    Carmen* k = (Carmen*)(vacuum*)_carmen(c,
        "<carmina><carmen titulus=\"d\">"
        "<pars titulus=\"superius\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o4 g4 g4 g4 r4 | g4 r2. | "
        "a4 r2. | f4 o5 c4 r2</textus></frustum></pars>"
        "<pars titulus=\"tympanum\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>@tympanum_grave o2 c4 c4 c4 c4"
        "</textus></frustum></pars>"
        "<pars titulus=\"vacua\" instrumentum=\"aether\"/>"
        "<pars titulus=\"altus\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o4 e4 b4 b4 r4 | g4 r2. | "
        "e4 b4 r2 | a4 r2.</textus></frustum></pars>"
        "<pars titulus=\"bassus\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o3 c4 c4 c4 r4 | c4 r2. | "
        "c4 r2. | c4 r2.</textus></frustum></pars>"
        "</carmen></carmina>");
    CensuraInventum  inv[CAPACITAS];
       CensuraForma  f = _sola(CENSURA_DECUSSATIO);
         CarmenNota* n;

    CREDO_NECESSE (k != NIHIL);
    /* acerva (editio, non MML): altus m3 [E4 B4], superius m4 [F4
     * C5] */
    n             = k->partes[III].frusta[ZEPHYRUM].notae;
    n[V].pulsus   = n[IV].pulsus;
    n             = k->partes[ZEPHYRUM].frusta[ZEPHYRUM].notae;
    n[VI].pulsus  = n[V].pulsus;
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == III);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].mensura, I);
    CREDO_AEQUALIS_S64 (inv[ZEPHYRUM].pulsus, (s64)CMLX);
    CREDO_AEQUALIS_I32 (inv[I].mensura, III);
    CREDO_AEQUALIS_I32 (inv[I].nota->nota, LXIX);
    CREDO_AEQUALIS_I32 (inv[I].nota_altera->nota, LXXI);
    CREDO_AEQUALIS_I32 (inv[II].mensura, IV);
    CREDO_AEQUALIS_I32 (inv[II].nota->nota, LXV);
    CREDO_AEQUALIS_I32 (inv[II].nota_altera->nota, LXIX);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[ZEPHYRUM]),
        "mensura 1 +960 superius/altus: decussatio G4 infra B4 "
        "(frustum 0, offset 6)\n");
}

/* spatium: voces superiores proximae > XII; vox ima libera. m1 C6
 * C5 C4 C2 (XII, non); m2 D6 / C5 bis - semel; m3 altus E5 / tenor
 * B3 (XVII) */
interior vacuum
_sectio_spatium (
    CredoContextus* c)
{
    Carmen* k = (Carmen*)(vacuum*)_carmen(c,
        "<carmina><carmen titulus=\"s\">"
        "<pars titulus=\"superius\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o6 c1 | d2 d2 | o5 g1"
        "</textus></frustum></pars>"
        "<pars titulus=\"altus\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o5 c1 | c2 c2 | e1"
        "</textus></frustum></pars>"
        "<pars titulus=\"tenor\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o4 c1 | c1 | o3 b1"
        "</textus></frustum></pars>"
        "<pars titulus=\"bassus\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o2 c1 | c1 | c1"
        "</textus></frustum></pars>"
        "</carmen></carmina>");
    CensuraInventum inv[CAPACITAS];
       CensuraForma f = _sola(CENSURA_SPATIUM);

    CREDO_NECESSE (k != NIHIL);
    CREDO_NECESSE (censura_iudicare(k, &f, inv, CAPACITAS) == II);
    CREDO_AEQUALIS_I32 (inv[ZEPHYRUM].mensura, II);
    CREDO_AEQUALIS_S64 (inv[ZEPHYRUM].pulsus, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (inv[I].mensura, III);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[ZEPHYRUM]),
        "mensura 2 superius/altus: spatium D6 / C5, 14 semitonia "
        "(frustum 0, offset 8)\n");
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &inv[I]),
        "mensura 3 altus/tenor: spatium E5 / B3, 17 semitonia "
        "(frustum 0, offset 16)\n");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "ordinaria", _sectio_ordinaria, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lineae", _sectio_lineae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "clavis", _sectio_clavis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "caerulea", _sectio_caerulea, NIHIL, NIHIL, NIHIL, NIHIL },
    { "chromatica", _sectio_chromatica, NIHIL, NIHIL, NIHIL, NIHIL },
    { "picardia", _sectio_picardia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ambitus", _sectio_ambitus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "dissonantia", _sectio_dissonantia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "parallelae", _sectio_parallelae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "saltus", _sectio_saltus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "occultae", _sectio_occultae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "occultae mediae", _sectio_occultae_mediae, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "decussatio", _sectio_decussatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "spatium", _sectio_spatium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tertia", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "acorda", _sectio_acorda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "stili", _sectio_stili, NIHIL, NIHIL, NIHIL, NIHIL },
    { "iazz", _sectio_intentiones, NIHIL, NIHIL, NIHIL, NIHIL },
    { "iazz figurae", _sectio_intentiones_figurae, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "fractum", _sectio_fractum, NIHIL, NIHIL, NIHIL, NIHIL },
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
