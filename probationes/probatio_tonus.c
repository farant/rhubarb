/* probatio_tonus.c - Probationes toni (include/tonus.h)
 *
 * Linea monodica in clave modali: finalis, ambitus (authenticus /
 * plagalis), numerus toni, tenor (exspectatus aut alienus),
 * clausulae (fermata, pausa, ultima), linea scripta; non iudicatur:
 * clavis tonalis, pars vacua, polyphonia.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "carmen.h"
#include "tonus.h"
#include <stdio.h>
#include <string.h>

/* carmen unius partis 'c' cum clave et mutatione (fermatae) */
interior Carmen*
_carmen (
        CredoContextus* c,
    constans character* clavis,
    constans character* mutatio,
    constans character* textus)
{
         character* s;
           Carmina* liber;
    CarminisVitium  vitium;

    s = (character*)piscina_allocare(c->piscina, M * IV);
    sprintf(s, "<carmina><carmen titulus=\"t\" clavis=\"%s\">%s"
            "<pars titulus=\"c\" instrumentum=\"schola\"><frustum "
            "mensura=\"1\"><textus!>%s</textus></frustum></pars>"
            "<pars titulus=\"v\" instrumentum=\"schola\"/>"
            "</carmen></carmina>", clavis, mutatio, textus);
    liber = (Carmina*)piscina_allocare(c->piscina, magnitudo(Carmina));
    si (   !carmina_legere(chorda_ex_literis(s, c->piscina), c->piscina,
                        liber, &vitium) || liber->numerus < I)
    {
        imprimere("  carmen: %s\n", vitium.causa ? vitium.causa : "-");
        redde NIHIL;
    }
    redde &liber->carmina[ZEPHYRUM];
}

interior chorda
_linea (
     CredoContextus* c,
    constans Carmen* k,
     constans Tonus* t)
{
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina, CCLVI);
               chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (a == NIHIL || !tonus_scribere(k, t, a))
    {
        redde vacua;
    }
    redde chorda_aedificator_finire(a);
}

#define PRIMUS \
    "o4 d4 e4 f4 g4 | a4 a4 g4 a4 | b-4 a4 g4 f4 | g4 a2. | " \
    "a4 o5 c4 d4 c4 | o4 a4 g4 a4 o5 c4 | o4 b4 a4 g4 a4 | a1 | " \
    "f4 g4 a4 g4 | f4 e4 d4 e4 | f4 g4 f4 e4 | f1 | " \
    "d4 e4 f4 g4 | f4 e4 d4 c4 | d4 e4 f4 e4 | d1"

#define PRIMI_CORONAE \
    "<fermata mensura=\"4\" ictus=\"2\"/><fermata mensura=\"8\"/>" \
    "<fermata mensura=\"12\"/><fermata mensura=\"16\"/>"

/* tonus I (primus domus): finalis D, C4-D5 authenticus, tenor A,
 * clausulae sub fermatis A A F D; linea */
interior vacuum
_sectio_primus (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c, "d dorius", PRIMI_CORONAE, PRIMUS);
              Tonus  t;

    CREDO_NECESSE (k != NIHIL);
    CREDO_NECESSE (tonus_partis(k, ZEPHYRUM, &t));
    CREDO_AEQUALIS_S32 (t.numerus, I);
    CREDO_FALSUM (t.plagalis);
    CREDO_AEQUALIS_S32 (t.modus, II);
    CREDO_AEQUALIS_S32 (t.finalis, II);
    CREDO_VERUM (t.in_finali);
    CREDO_AEQUALIS_I32 (t.ima->nota, LX);
    CREDO_AEQUALIS_I32 (t.summa->nota, LXXIV);
    CREDO_AEQUALIS_S32 (t.tenor, IX);
    CREDO_AEQUALIS_S32 (t.tenor_exspectatus, IX);
    CREDO_NECESSE (t.numerus_clausularum == IV);
    CREDO_AEQUALIS_I32 (t.clausulae[ZEPHYRUM]->nota, LXIX);
    CREDO_AEQUALIS_I32 (t.clausulae[II]->nota, LXV);
    CREDO_AEQUALIS_I32 (t.clausulae[III]->nota, LXII);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &t),
        "I (dorius authenticus): finalis D, ambitus C4-D5, tenor A, "
        "clausulae A A F D");
}

/* tonus II: A3-F4 sub finali D4 (quarta infra: plagalis), tenor F;
 * clausula sola ultima */
interior vacuum
_sectio_plagalis (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c, "d dorius", "",
        "o3 a4 o4 c4 d4 f4 | e4 d4 c4 d4 | f2 e4 d4 | d1");
              Tonus t;

    CREDO_NECESSE (k != NIHIL);
    CREDO_NECESSE (tonus_partis(k, ZEPHYRUM, &t));
    CREDO_AEQUALIS_S32 (t.numerus, II);
    CREDO_VERUM (t.plagalis);
    CREDO_AEQUALIS_S32 (t.tenor, V);
    CREDO_AEQUALIS_S32 (t.tenor_exspectatus, V);
    CREDO_AEQUALIS_I32 (t.numerus_clausularum, I);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &t),
        "II (dorius plagalis): finalis D, ambitus A3-F4, tenor F, "
        "clausulae D");
}

/* tenor alienus (G pro A in tono I); numeri III, VIII, XI; pausa
 * clausulam facit; finis non in finali */
interior vacuum
_sectio_varia (
    CredoContextus* c)
{
    constans Carmen* k;
              Tonus  t;

    k = _carmen(c, "d dorius", "", "o4 d4 g2 g4 | g2 a4 g4 | f4 e4 d2");
    CREDO_NECESSE (k != NIHIL && tonus_partis(k, ZEPHYRUM, &t));
    CREDO_AEQUALIS_S32 (t.numerus, I);
    CREDO_AEQUALIS_S32 (t.tenor, VII);
    CREDO_AEQUALIS_S32 (t.tenor_exspectatus, IX);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, &t),
        "I (dorius authenticus): finalis D, ambitus D4-A4, tenor G "
        "(exspectatus A), clausulae D");
    k = _carmen(c, "e phrygius", "",
        "o4 e4 f4 g4 a4 | b4 o5 c4 o4 b4 a4 | g4 f4 e2");
    CREDO_NECESSE (k != NIHIL && tonus_partis(k, ZEPHYRUM, &t));
    CREDO_AEQUALIS_S32 (t.numerus, III);
    k = _carmen(c, "g mixolydius", "",
        "o4 d4 f4 g4 a4 | g4 f4 e4 f4 | g1");
    CREDO_NECESSE (k != NIHIL && tonus_partis(k, ZEPHYRUM, &t));
    CREDO_AEQUALIS_S32 (t.numerus, VIII);
    CREDO_VERUM (t.plagalis);
    k = _carmen(c, "c ionius", "", "o4 c4 d4 e4 g4 | o5 c4 o4 g4 c2");
    CREDO_NECESSE (k != NIHIL && tonus_partis(k, ZEPHYRUM, &t));
    CREDO_AEQUALIS_S32 (t.numerus, XI);
    k = _carmen(c, "d dorius", "",
        "o4 d4 e4 f4 r4 | g4 a4 g4 r4 | f4 e4 d2");
    CREDO_NECESSE (k != NIHIL && tonus_partis(k, ZEPHYRUM, &t));
    CREDO_NECESSE (t.numerus_clausularum == III);
    CREDO_AEQUALIS_I32 (t.clausulae[ZEPHYRUM]->nota, LXV);
    CREDO_AEQUALIS_I32 (t.clausulae[I]->nota, LXVII);
    k = _carmen(c, "d dorius", "", "o4 d4 e4 f4 g4 | a1");
    CREDO_NECESSE (k != NIHIL && tonus_partis(k, ZEPHYRUM, &t));
    CREDO_FALSUM (t.in_finali);
    /* ima tertia maior infra finalem (Bb3 sub D4): authenticus -
     * plagalis a quarta */
    k = _carmen(c, "d dorius", "", "o4 d4 o3 b-4 o4 d4 e4 | d1");
    CREDO_NECESSE (k != NIHIL && tonus_partis(k, ZEPHYRUM, &t));
    CREDO_AEQUALIS_S32 (t.numerus, I);
    /* tenor aequalis (G et A singulae quartae): exspectatus A */
    k = _carmen(c, "d dorius", "", "o4 d4 g4 a4 d4 | d1");
    CREDO_NECESSE (k != NIHIL && tonus_partis(k, ZEPHYRUM, &t));
    CREDO_AEQUALIS_S32 (t.tenor, IX);
    /* finalis numquam sonat: proxima infra imam (D4 sub E4) -
     * authenticus */
    k = _carmen(c, "d dorius", "", "o4 e4 f4 g4 a4 | g1");
    CREDO_NECESSE (k != NIHIL && tonus_partis(k, ZEPHYRUM, &t));
    CREDO_AEQUALIS_S32 (t.numerus, I);
    CREDO_FALSUM (t.plagalis);
    /* clavis ad notam ultimam: d dorius -> a aeolius, finis A4, ima D4
     * (quarta infra): X */
    k = _carmen(c, "d dorius",
        "<clavis mensura=\"2\" valor=\"a aeolius\"/>",
        "o4 d4 e4 f4 g4 | a4 b4 o5 c4 o4 b4 | a1");
    CREDO_NECESSE (k != NIHIL && tonus_partis(k, ZEPHYRUM, &t));
    CREDO_AEQUALIS_S32 (t.numerus, X);
    CREDO_VERUM (t.in_finali);
}

/* non iudicatur: clavis tonalis, pars vacua, polyphonia (notae
 * simul) */
interior vacuum
_sectio_nulla (
    CredoContextus* c)
{
     Carmen* k;
      Tonus  t;

    k = _carmen(c, "d minor", "", "o4 d4 e4 f4 g4 | a4 g4 f4 e4 | d1");
    CREDO_NECESSE (k != NIHIL);
    CREDO_FALSUM (tonus_partis(k, ZEPHYRUM, &t));
    k = _carmen(c, "d dorius", "", "o4 d4 e4 f4 g4 | d1");
    CREDO_NECESSE (k != NIHIL);
    CREDO_FALSUM (tonus_partis(k, I, &t));
    CREDO_VERUM (tonus_partis(k, ZEPHYRUM, &t));
    k->partes[ZEPHYRUM].frusta[ZEPHYRUM].notae[I].pulsus =
        k->partes[ZEPHYRUM].frusta[ZEPHYRUM].notae[ZEPHYRUM].pulsus;
    CREDO_FALSUM (tonus_partis(k, ZEPHYRUM, &t));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "primus", _sectio_primus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "plagalis", _sectio_plagalis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "varia", _sectio_varia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nulla", _sectio_nulla, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("tonus", SECTIONES);
}
