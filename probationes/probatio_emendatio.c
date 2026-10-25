/* probatio_emendatio.c - Probationes emendationis (include/emendatio.h)
 *
 * Liber TOTUS octetim comparatur: exspectatus = liber originalis cum
 * substitutione una (spatia, attributa, commenta, carmina cetera
 * intacta). Octavae: '<' / '>' circumdantur et iterum emendatae se
 * tollunt (liber ad originalem redit).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "chorda_aedificator.h"
#include "carmen.h"
#include "emendatio.h"
#include <stdio.h>
#include <string.h>

hic_manens constans character LIBER[] =
    "<!-- liber probationis -->\n"
    "<carmina>\n"
    "  <carmen titulus=\"s\" tempus=\"72\" metrum=\"3/4\""
    " clavis=\"d minor\">\n"
    "    <pars titulus=\"bassus\" instrumentum=\"triangulum\""
    " lucrum=\"0.65\">\n"
    "      <frustum mensura=\"1\"><textus!>o3 d2. | o2 b-2. | g2 a4"
    "</textus></frustum>\n"
    "      <frustum mensura=\"5\"><textus!>o3 d4 e4 f4</textus>"
    "</frustum>\n"
    "    </pars>\n"
    "  </carmen>\n"
    "  <carmen titulus=\"t\" metrum=\"6/8\">\n"
    "    <pars titulus=\"bassus\" instrumentum=\"aether\">\n"
    "      <frustum mensura=\"1\"><textus!>o4 c4. d8 e8 f8 | g2. |"
    " @punctum c4</textus></frustum>\n"
    "    </pars>\n"
    "    <pars titulus=\"vox\" instrumentum=\"aether\">\n"
    "      <frustum mensura=\"1\"><textus>o4 c2.</textus></frustum>\n"
    "    </pars>\n"
    "  </carmen>\n"
    "</carmina>\n";

/* liber cum substitutione PRIMA 'vetus' -> 'novum' (vetus unicum in
 * libro esse debet - probatio ipsa id custodit) */
interior chorda
_exspectatus (
          CredoContextus* c,
                  chorda  liber,
      constans character* vetus,
      constans character* novum)
{
       i32 lv = (i32)strlen(vetus);
       i32 ln = (i32)strlen(novum);
       i32 i;
       i32 inventa  = ZEPHYRUM;
       i32 ubi      = ZEPHYRUM;
    chorda r;

    per (i = ZEPHYRUM; i + lv <= liber.mensura; i++)
    {
        si (memcmp(liber.datum + i, vetus, (size_t)lv) == ZEPHYRUM)
        {
            si (inventa == ZEPHYRUM)
            {
                ubi = i;
            }
            inventa++;
        }
    }
    r.mensura  = ZEPHYRUM;
    r.datum    = NIHIL;
    si (inventa != I)
    {
        imprimere("  '%s' in libro %u vicibus\n", vetus, inventa);
        redde r;
    }
    r.mensura = liber.mensura - lv + ln;
    r.datum   = (i8*)piscina_allocare(c->piscina,
        (memoriae_index)r.mensura);
    memcpy(r.datum, liber.datum, (size_t)ubi);
    memcpy(r.datum + ubi, novum, (size_t)ln);
    memcpy(r.datum + ubi + ln, liber.datum + ubi + lv,
           (size_t)(liber.mensura - ubi - lv));
    redde r;
}

interior EmendatioLocus
_locus (
    i32 mensura,
    i32 tempus,
    s64 pulsus)
{
    EmendatioLocus l;

    l.mensura  = mensura;
    l.tempus   = tempus;
    l.pulsus   = pulsus;
    redde l;
}

/* ponere; FALSUM -> causa imprimitur */
interior b32
_ponere (
                CredoContextus* c,
                        chorda  liber,
            constans character* carmen,
            constans character* pars,
       constans EmendatioLocus  locus,
            constans character* nota,
                        chorda* novus,
              EmendatioMutatio* mutatio,
                CarminisVitium* vitium)
{
    b32 r;

    memset(novus, ZEPHYRUM, magnitudo(*novus));
    memset(mutatio, ZEPHYRUM, magnitudo(*mutatio));
    memset(vitium, ZEPHYRUM, magnitudo(*vitium));
    r = emendatio_notam_ponere(liber,
        chorda_ex_literis(carmen, c->piscina),
        chorda_ex_literis(pars, c->piscina), &locus,
        chorda_ex_literis(nota, c->piscina), c->piscina, novus, mutatio,
        vitium);
    si (!r)
    {
        imprimere("  %s %s %u:%u+%d %s: %s (frustum %d, offset %u)\n",
                  carmen, pars, locus.mensura, locus.tempus,
                  (integer)locus.pulsus, nota,
                  vitium->causa ? vitium->causa : "-",
                  (integer)vitium->frustum, vitium->offset);
    }
    redde r;
}

interior b32
_locus_est (
         CredoContextus* c,
     constans character* textus,
                    i32  mensura,
                    i32  tempus,
                    s64  pulsus)
{
    EmendatioLocus l;

    si (   emendatio_locum_legere(chorda_ex_literis(textus, c->piscina),
                                  &l)
        && l.mensura == mensura && l.tempus == tempus
        && l.pulsus  == pulsus)
    {
        redde VERUM;
    }
    imprimere("  locus '%s'\n", textus);
    redde FALSUM;
}

interior vacuum
_sectio_locus (
    CredoContextus* c)
{
    hic_manens constans character* MALI[] = {
        "", "3", "3:", ":2", "0:1", "3:0", "3:2+", "3:2+x", "a:b",
        "3:2 ", " 3:2", "3:2+-5", "3:-2", "3:2:1", "3+2", "3:2+480+1"
    };
    EmendatioLocus l;
               i32 i;

    CREDO_VERUM (_locus_est(c, "3:2", III, II, ZEPHYRUM));
    CREDO_VERUM (_locus_est(c, "1:1", I, I, ZEPHYRUM));
    CREDO_VERUM (_locus_est(c, "3:2+480", III, II, CDLXXX));
    CREDO_VERUM (_locus_est(c, "12:4+0", XII, IV, ZEPHYRUM));
    per (i = ZEPHYRUM; i < (i32)(magnitudo(MALI) / magnitudo(MALI[0]));
         i++)
    {
        si (emendatio_locum_legere(chorda_ex_literis(MALI[i],
                                                     c->piscina), &l))
        {
            imprimere("  malus acceptus: '%s'\n", MALI[i]);
            CREDO_CULPA ("locus malus acceptus");
        }
    }
}

/* octava eadem, inferior, superior; frustum secundum; liber totus */
interior vacuum
_sectio_ponere (
    CredoContextus* c)
{
              chorda liber = chorda_ex_literis(LIBER, c->piscina);
              chorda novus;
    EmendatioMutatio m;
      CarminisVitium v;

    /* octava inferior: o3 d2. -> <g2.> */
    CREDO_VERUM (_ponere(c, liber, "s", "bassus", _locus(I, I,
        ZEPHYRUM),
                         "G2", &novus, &m, &v));
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber,
        "o3 d2. | o2", "o3 <g2.> | o2"));
    CREDO_AEQUALIS_S32 (m.frustum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (m.offset, III);
    CREDO_CHORDA_AEQUALIS_LITERIS (m.vetus, "d2.");
    CREDO_CHORDA_AEQUALIS_LITERIS (m.novum, "<g2.>");
    /* octava superior, mensura 3 tempus 3 (pulsus MMMMMMMDCLXXX) */
    CREDO_VERUM (_ponere(c, liber, "s", "bassus", _locus(III, III,
                         ZEPHYRUM), "C#3", &novus, &m, &v));
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber, "g2 a4",
        "g2 >c#4<"));
    CREDO_CHORDA_AEQUALIS_LITERIS (m.novum, ">c#4<");
    /* octava eadem: alteratio sola mutata */
    CREDO_VERUM (_ponere(c, liber, "s", "bassus", _locus(II, I,
                         ZEPHYRUM), "B2", &novus, &m, &v));
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber, "b-2.",
        "b2."));
    CREDO_CHORDA_AEQUALIS_LITERIS (m.vetus, "b-2.");
    CREDO_CHORDA_AEQUALIS_LITERIS (m.novum, "b2.");
    /* bemollis scriptus ut '-' */
    CREDO_VERUM (_ponere(c, liber, "s", "bassus", _locus(III, I,
                         ZEPHYRUM), "Ab2", &novus, &m, &v));
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber, "g2 a4",
        "a-2 a4"));
    /* frustum secundum (mensura 5): offset in textu SUO */
    CREDO_VERUM (_ponere(c, liber, "s", "bassus", _locus(V, II,
                         ZEPHYRUM), "E4", &novus, &m, &v));
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber, "d4 e4 f4",
        "d4 >e4< f4"));
    CREDO_AEQUALIS_S32 (m.frustum, I);
    CREDO_AEQUALIS_I32 (m.offset, VI);
}

/* 6/8: tempus = tres octavae; pulsus additi intra tempus; carmen
 * secundum, pars homonyma carminis primi intacta */
interior vacuum
_sectio_composita (
    CredoContextus* c)
{
              chorda liber = chorda_ex_literis(LIBER, c->piscina);
              chorda novus;
    EmendatioMutatio m;
      CarminisVitium v;

    CREDO_VERUM (_ponere(c, liber, "t", "bassus", _locus(I, II,
                         ZEPHYRUM), "E4", &novus, &m, &v));
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber,
        "c4. d8 e8", "c4. e8 e8"));
    CREDO_VERUM (_ponere(c, liber, "t", "bassus", _locus(I, II,
                         CDLXXX), "G4", &novus, &m, &v));
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber,
        "d8 e8 f8", "d8 g8 f8"));
    CREDO_VERUM (_ponere(c, liber, "t", "bassus", _locus(II, I,
                         ZEPHYRUM), "A3", &novus, &m, &v));
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber, "| g2. |",
        "| <a2.> |"));
}

/* iterum emendata: '<' '>' adiacentes se tollunt; liber ad originalem
 * redit octetim; eadem directio cumulatur */
interior vacuum
_sectio_iterum (
    CredoContextus* c)
{
              chorda liber = chorda_ex_literis(LIBER, c->piscina);
              chorda primus;
              chorda secundus;
    EmendatioMutatio m;
      CarminisVitium v;

    CREDO_NECESSE (_ponere(c, liber, "s", "bassus", _locus(I, I,
                           ZEPHYRUM), "G2", &primus, &m, &v));
    CREDO_VERUM (_ponere(c, primus, "s", "bassus", _locus(I, I,
                         ZEPHYRUM), "D3", &secundus, &m, &v));
    CREDO_CHORDA_AEQUALIS (secundus, liber);
    CREDO_CHORDA_AEQUALIS_LITERIS (m.vetus, "<g2.>");
    CREDO_CHORDA_AEQUALIS_LITERIS (m.novum, "d2.");
    CREDO_AEQUALIS_I32 (m.offset, III);
    CREDO_VERUM (_ponere(c, primus, "s", "bassus", _locus(I, I,
                         ZEPHYRUM), "G1", &secundus, &m, &v));
    CREDO_CHORDA_AEQUALIS (secundus, _exspectatus(c, liber,
        "o3 d2. | o2", "o3 <<g2.>> | o2"));
    /* altera via: superior deinde redit */
    CREDO_NECESSE (_ponere(c, liber, "s", "bassus", _locus(III, III,
                           ZEPHYRUM), "C#3", &primus, &m, &v));
    CREDO_VERUM (_ponere(c, primus, "s", "bassus", _locus(III, III,
                         ZEPHYRUM), "A2", &secundus, &m, &v));
    CREDO_CHORDA_AEQUALIS (secundus, liber);
}

/* vitia: liber novus nullus (*novus intactus), causa et locus */
interior b32
_recusat (
                CredoContextus* c,
            constans character* carmen,
            constans character* pars,
       constans EmendatioLocus  locus,
            constans character* nota,
                CarminisVitium* v)
{
              chorda liber = chorda_ex_literis(LIBER, c->piscina);
              chorda novus;
    EmendatioMutatio m;

    novus = chorda_ex_literis("intactus", c->piscina);
    memset(v, ZEPHYRUM, magnitudo(*v));
    si (emendatio_notam_ponere(liber,
            chorda_ex_literis(carmen, c->piscina),
            chorda_ex_literis(pars, c->piscina), &locus,
            chorda_ex_literis(nota, c->piscina), c->piscina, &novus,
            &m, v))
    {
        imprimere("  acceptum: %s %s %s\n", carmen, pars, nota);
        redde FALSUM;
    }
    si (   v->causa == NIHIL || novus.mensura != VIII
        || memcmp(novus.datum, "intactus", VIII) != ZEPHYRUM)
    {
        imprimere("  sine causa aut novus mutatus: %s %s %s\n", carmen,
                  pars, nota);
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
_sectio_vitia (
    CredoContextus* c)
{
      CarminisVitium v;
              chorda novus;
    EmendatioMutatio m;
      EmendatioLocus primus = _locus(I, I, ZEPHYRUM);

    CREDO_VERUM (_recusat(c, "x", "bassus", _locus(I, I, ZEPHYRUM),
                          "G2", &v));
    CREDO_VERUM (_recusat(c, "s", "melodia", _locus(I, I, ZEPHYRUM),
                          "G2", &v));
    CREDO_CHORDA_AEQUALIS_LITERIS (v.pars, "melodia");
    /* 3/4: tempus IV nullum; pulsus additi >= tempus */
    CREDO_VERUM (_recusat(c, "s", "bassus", _locus(I, IV, ZEPHYRUM),
                          "G2", &v));
    CREDO_VERUM (_recusat(c, "s", "bassus", _locus(I, I, CMLX), "G2",
                          &v));
    CREDO_VERUM (_recusat(c, "t", "bassus", _locus(I, III, ZEPHYRUM),
                          "G2", &v));
    /* pulsus additi ultra tempus in notam alteram caderent (3:3 = a4):
     * recusatur, non 'tempus proximum' */
    CREDO_VERUM (_recusat(c, "s", "bassus", _locus(III, I, MCMXX), "G2",
                          &v));
    /* intra notam sonantem: frustum et offset notae quae sonat */
    CREDO_VERUM (_recusat(c, "s", "bassus", _locus(I, II, ZEPHYRUM),
                          "G2", &v));
    CREDO_AEQUALIS_S32 (v.frustum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (v.offset, III);
    /* silentium inter frusta (mensura 4) */
    CREDO_VERUM (_recusat(c, "s", "bassus", _locus(IV, I, ZEPHYRUM),
                          "G2", &v));
    /* sonitus '@' */
    CREDO_VERUM (_recusat(c, "t", "bassus", _locus(III, I, ZEPHYRUM),
                          "G2", &v));
    /* nota nova mala; alteratio duplex (MML unam scribit); extra
     * octavas MML 0..8 (liber novus iudicio cadit) */
    CREDO_VERUM (_recusat(c, "s", "bassus", _locus(I, I, ZEPHYRUM),
                          "H2", &v));
    CREDO_VERUM (_recusat(c, "s", "bassus", _locus(I, I, ZEPHYRUM),
                          "C##3", &v));
    CREDO_LITERAE_AEQUALES (v.causa,
        "MML alterationem unam scribit (# aut b)");
    CREDO_VERUM (_recusat(c, "s", "bassus", _locus(I, I, ZEPHYRUM),
                          "C-1", &v));
    CREDO_LITERAE_AEQUALES (v.causa, "octava extra MML (0..8)");
    CREDO_VERUM (_recusat(c, "s", "bassus", _locus(I, I, ZEPHYRUM),
                          "G9", &v));
    /* textus non crudus ('<' in MML tagum aperiret) */
    CREDO_VERUM (_recusat(c, "t", "vox", _locus(I, I, ZEPHYRUM), "D4",
                          &v));
    /* liber malus */
    CREDO_FALSUM (emendatio_notam_ponere(
        chorda_ex_literis("<carmina><carmen", c->piscina),
        chorda_ex_literis("s", c->piscina),
        chorda_ex_literis("bassus", c->piscina), &primus,
        chorda_ex_literis("G2", c->piscina), c->piscina, &novus, &m,
        &v));
}


/* ==================================================
 * Mensurae (E2)
 * ================================================== */

/* bassus: mensurae 1-4 in frusto 0 (3: 'l8' et '>' mutat; 4: finis
 * textus), 5 silentium, 6-7 in frusto 1. sine: lineae nullae. medium:
 * frustum intra mensuram 1 incipit. tympanum: '@' in mensura 1,
 * '|' ultima */
hic_manens constans character LIBER_M[] =
    "<carmina>\n"
    "  <carmen titulus=\"m\" metrum=\"3/4\">\n"
    "    <pars titulus=\"bassus\" instrumentum=\"triangulum\">\n"
    "      <frustum mensura=\"1\"><textus!>o3 d2. | o2 b-2. |"
    " l8 g4 a b >c d | <a2.</textus></frustum>\n"
    "      <frustum mensura=\"6\"><textus!>r2. | c2.</textus>"
    "</frustum>\n"
    "    </pars>\n"
    "    <pars titulus=\"sine\" instrumentum=\"aether\">\n"
    "      <frustum mensura=\"1\"><textus!>o4 c2. d2. e2.</textus>"
    "</frustum>\n"
    "    </pars>\n"
    "    <pars titulus=\"medium\" instrumentum=\"aether\">\n"
    "      <frustum pulsus=\"960\"><textus!>c4 c4 | c2.</textus>"
    "</frustum>\n"
    "    </pars>\n"
    "    <pars titulus=\"tympanum\" instrumentum=\"aether\">\n"
    "      <frustum mensura=\"1\"><textus!>@punctum c2. | c2. |"
    "</textus></frustum>\n"
    "    </pars>\n"
    "    <pars titulus=\"duplex\" instrumentum=\"aether\">\n"
    "      <frustum mensura=\"1\"><textus!>o3 d2. | | o2 a2. | |"
    " c2.</textus></frustum>\n"
    "    </pars>\n"
    "    <pars titulus=\"brevis\" instrumentum=\"aether\">\n"
    "      <frustum mensura=\"1\"><textus!>c2. | c4</textus>"
    "</frustum>\n"
    "    </pars>\n"
    "    <pars titulus=\"punctata\" instrumentum=\"aether\">\n"
    "      <frustum mensura=\"1\"><textus!>o4 l4. c c | d d</textus>"
    "</frustum>\n"
    "    </pars>\n"
    "  </carmen>\n"
    "</carmina>\n";

interior b32
_inventa_est (
         CredoContextus* c,
        constans Carmen* k,
     constans character* pars,
                    i32  mensura,
                    s32  frustum,
     constans character* textus,
                    s32  octava,
                    i32  ordinaria,
     constans character* praeparatum)
{
    EmendatioMensura m;
      CarminisVitium v;

    memset(&m, ZEPHYRUM, magnitudo(m));
    si (   emendatio_mensuram_invenire(k,
               chorda_ex_literis(pars, c->piscina), mensura, &m, &v)
        && m.frustum                    == frustum
        && chorda_aequalis_literis(m.textus, textus)
        && m.finis - m.initium          == m.textus.mensura
        && m.status.octava              == octava
        && m.status.ordinaria           == ordinaria
        && m.status.praeparatum.mensura == (i32)strlen(praeparatum)
        && (praeparatum[0] == '\0'
            || chorda_aequalis_literis(m.status.praeparatum,
                                       praeparatum)))
    {
        redde VERUM;
    }
    imprimere("  %s %u: frustum %d '%.*s' o%d l%u\n", pars, mensura,
              (integer)m.frustum, (integer)m.textus.mensura,
              m.textus.datum ? (constans character*)m.textus.datum : "",
              (integer)m.status.octava, m.status.ordinaria);
    redde FALSUM;
}

interior vacuum
_sectio_invenire (
    CredoContextus* c)
{
             Carmina  liber;
      CarminisVitium  v;
    EmendatioMensura  m;
     constans Carmen* k;

    CREDO_NECESSE (carmina_legere(chorda_ex_literis(LIBER_M,
                                                    c->piscina),
                                  c->piscina, &liber, &v));
    k = &liber.carmina[ZEPHYRUM];
    CREDO_VERUM (_inventa_est(c, k, "bassus", I, ZEPHYRUM, "o3 d2.", IV,
                              CMLX, ""));
    CREDO_VERUM (_inventa_est(c, k, "bassus", II, ZEPHYRUM, "o2 b-2.",
                              III, CMLX, ""));
    CREDO_VERUM (_inventa_est(c, k, "bassus", III, ZEPHYRUM,
                              "l8 g4 a b >c d", II, CMLX, ""));
    CREDO_VERUM (_inventa_est(c, k, "bassus", IV, ZEPHYRUM, "<a2.", III,
                              CDLXXX, ""));
    CREDO_VERUM (_inventa_est(c, k, "bassus", VI, I, "r2.", IV, CMLX,
                              ""));
    CREDO_VERUM (_inventa_est(c, k, "bassus", VII, I, "c2.", IV, CMLX,
                              ""));
    CREDO_VERUM (_inventa_est(c, k, "medium", II, ZEPHYRUM, "c2.", IV,
                              CMLX, ""));
    CREDO_VERUM (_inventa_est(c, k, "tympanum", II, ZEPHYRUM, "c2.", IV,
                              CMLX, "punctum"));
    /* lineae duplices: initium post ULTIMAM, finis ad PRIMAM */
    CREDO_VERUM (_inventa_est(c, k, "duplex", II, ZEPHYRUM, "o2 a2.",
                              III, CMLX, ""));
    CREDO_VERUM (_inventa_est(c, k, "duplex", III, ZEPHYRUM, "c2.", II,
                              CMLX, ""));
    /* sedes in textu frusti: initium post '| ', finis ante ' |' */
    CREDO_NECESSE (emendatio_mensuram_invenire(k,
        chorda_ex_literis("bassus", c->piscina), II, &m, &v));
    CREDO_AEQUALIS_I32 (m.initium, IX);
    CREDO_AEQUALIS_I32 (m.finis, XVI);
    /* silentium inter frusta; ultra carmen; mensura 0 */
    CREDO_FALSUM (emendatio_mensuram_invenire(k,
        chorda_ex_literis("bassus", c->piscina), V, &m, &v));
    CREDO_FALSUM (emendatio_mensuram_invenire(k,
        chorda_ex_literis("bassus", c->piscina), VIII, &m, &v));
    CREDO_FALSUM (emendatio_mensuram_invenire(k,
        chorda_ex_literis("bassus", c->piscina), ZEPHYRUM, &m, &v));
    /* lineae non scriptae (initium et finis) */
    CREDO_FALSUM (emendatio_mensuram_invenire(k,
        chorda_ex_literis("sine", c->piscina), I, &m, &v));
    CREDO_FALSUM (emendatio_mensuram_invenire(k,
        chorda_ex_literis("sine", c->piscina), II, &m, &v));
    /* frustum intra mensuram incipit; frustum intra mensuram desinit */
    CREDO_FALSUM (emendatio_mensuram_invenire(k,
        chorda_ex_literis("medium", c->piscina), I, &m, &v));
    CREDO_LITERAE_AEQUALES (v.causa, "frustum intra mensuram incipit");
    CREDO_FALSUM (emendatio_mensuram_invenire(k,
        chorda_ex_literis("brevis", c->piscina), II, &m, &v));
    CREDO_LITERAE_AEQUALES (v.causa, "mensura non intra frustum unum");
    CREDO_FALSUM (emendatio_mensuram_invenire(k,
        chorda_ex_literis("melodia", c->piscina), I, &m, &v));
    CREDO_CHORDA_AEQUALIS_LITERIS (v.pars, "melodia");
}

/* substituere; FALSUM -> causa imprimitur */
interior b32
_substituere (
         CredoContextus* c,
                 chorda  liber,
     constans character* pars,
                    i32  mensura,
     constans character* textus,
                 chorda* novus,
       EmendatioMutatio* mutatio,
         CarminisVitium* vitium)
{
    b32 r;

    memset(novus, ZEPHYRUM, magnitudo(*novus));
    memset(mutatio, ZEPHYRUM, magnitudo(*mutatio));
    memset(vitium, ZEPHYRUM, magnitudo(*vitium));
    r = emendatio_mensuram_substituere(liber,
        chorda_ex_literis("m", c->piscina),
        chorda_ex_literis(pars, c->piscina), mensura,
        chorda_ex_literis(textus, c->piscina), c->piscina, novus,
        mutatio, vitium);
    si (!r)
    {
        imprimere("  %s %u '%s': %s (frustum %d, offset %u)\n", pars,
                  mensura, textus, vitium->causa ? vitium->causa : "-",
                  (integer)vitium->frustum, vitium->offset);
    }
    redde r;
}

/* status restitutus: octava, 'l', 'v', '@'; nihil ubi frustum desinit;
 * spatia orae servata; textus novus sine spatiis orae */
interior vacuum
_sectio_substituere (
    CredoContextus* c)
{
              chorda liber = chorda_ex_literis(LIBER_M, c->piscina);
              chorda novus;
    EmendatioMutatio m;
      CarminisVitium v;

    /* octava: initium o3 (ex mensura 1), finis vetus o2 */
    CREDO_VERUM (_substituere(c, liber, "bassus", II, "o3 d2.", &novus,
                              &m, &v));
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber,
        "| o2 b-2. |", "| o3 d2. o2 |"));
    CREDO_AEQUALIS_S32 (m.frustum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (m.offset, IX);
    CREDO_CHORDA_AEQUALIS_LITERIS (m.vetus, "o2 b-2.");
    CREDO_CHORDA_AEQUALIS_LITERIS (m.novum, "o3 d2. o2");
    /* status idem: nihil additum; spatia textus novi abscissa */
    CREDO_VERUM (_substituere(c, liber, "bassus", I, "  o3 f2 e4 ",
                              &novus, &m, &v));
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber,
        "o3 d2. | o2", "o3 f2 e4 | o2"));
    /* 'l' et octava: finis vetus o3 l8 */
    CREDO_VERUM (_substituere(c, liber, "bassus", III, "g2.", &novus,
                              &m, &v));
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber,
        "| l8 g4 a b >c d |", "| g2. o3 l8 |"));
    /* 'v' (et octava) */
    CREDO_VERUM (_substituere(c, liber, "bassus", II, "v50 g2.",
                              &novus, &m, &v));
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber,
        "| o2 b-2. |", "| v50 g2. o2 v100 |"));
    /* frustum desinit: nihil restituitur */
    CREDO_VERUM (_substituere(c, liber, "bassus", IV, "f2.", &novus,
                              &m, &v));
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber,
        "| <a2.</textus>", "| f2.</textus>"));
    /* pausa -> nota, frustum secundum */
    CREDO_VERUM (_substituere(c, liber, "bassus", VI, "g2.", &novus,
                              &m, &v));
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber,
        ">r2. | c2.<", ">g2. | c2.<"));
    CREDO_AEQUALIS_S32 (m.frustum, I);
    /* '@' vetus restitutum */
    CREDO_VERUM (_substituere(c, liber, "tympanum", I, "c2.", &novus,
                              &m, &v));
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber,
        ">@punctum c2. |", ">c2. @punctum |"));
    /* 'l' punctata restituta */
    CREDO_VERUM (_substituere(c, liber, "punctata", I, "o4 l4 c2.",
                              &novus, &m, &v));
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber,
        ">o4 l4. c c |", ">o4 l4 c2. l4. |"));
}

/* bis: mensura substituta deinde reddita -> liber originalis */
interior vacuum
_sectio_mensura_iterum (
    CredoContextus* c)
{
              chorda liber = chorda_ex_literis(LIBER_M, c->piscina);
              chorda primus;
              chorda secundus;
    EmendatioMutatio m;
      CarminisVitium v;

    CREDO_NECESSE (_substituere(c, liber, "bassus", II, "o3 d2.",
                                &primus, &m, &v));
    CREDO_VERUM (_substituere(c, primus, "bassus", II, "o2 b-2.",
                              &secundus, &m, &v));
    CREDO_CHORDA_AEQUALIS (secundus, liber);
    CREDO_NECESSE (_substituere(c, liber, "bassus", III, "g2.",
                                &primus, &m, &v));
    CREDO_VERUM (_substituere(c, primus, "bassus", III,
                              "l8 g4 a b >c d", &secundus, &m, &v));
    CREDO_CHORDA_AEQUALIS (secundus, liber);
}

interior b32
_substituere_recusat (
         CredoContextus* c,
     constans character* pars,
                    i32  mensura,
     constans character* textus,
         CarminisVitium* v)
{
              chorda liber = chorda_ex_literis(LIBER_M, c->piscina);
              chorda novus = chorda_ex_literis("intactus", c->piscina);
    EmendatioMutatio m;

    memset(v, ZEPHYRUM, magnitudo(*v));
    si (emendatio_mensuram_substituere(liber,
            chorda_ex_literis("m", c->piscina),
            chorda_ex_literis(pars, c->piscina), mensura,
            chorda_ex_literis(textus, c->piscina), c->piscina, &novus,
            &m, v))
    {
        imprimere("  acceptum: %s %u '%s'\n", pars, mensura, textus);
        redde FALSUM;
    }
    si (   v->causa == NIHIL
        || !chorda_aequalis_literis(novus, "intactus"))
    {
        imprimere("  sine causa aut novus mutatus: %s %u\n", pars,
                  mensura);
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
_sectio_mensura_vitia (
    CredoContextus* c)
{
    CarminisVitium v;

    /* longitudo brevior, longior */
    CREDO_VERUM (_substituere_recusat(c, "bassus", II, "o3 d2", &v));
    CREDO_LITERAE_AEQUALES (v.causa,
        "longitudo textus novi mensuram non aequat");
    CREDO_VERUM (_substituere_recusat(c, "bassus", II, "d2. d4", &v));
    CREDO_LITERAE_AEQUALES (v.causa,
        "longitudo textus novi mensuram non aequat");
    /* '|' in textu novo */
    CREDO_VERUM (_substituere_recusat(c, "bassus", II, "d4 | d2", &v));
    CREDO_LITERAE_AEQUALES (v.causa, "textus novus sine '|'");
    /* '@' novum ubi finis vetus nullum habet */
    CREDO_VERUM (_substituere_recusat(c, "bassus", II, "@punctum c2.",
                                      &v));
    CREDO_LITERAE_AEQUALES (v.causa,
        "'@' novum notas sequentes mutaret (MML '@' revocare nequit)");
    /* MML mala; 't' in frusto (liber novus iudicio cadit) */
    CREDO_VERUM (_substituere_recusat(c, "bassus", II, "x2.", &v));
    CREDO_VERUM (_substituere_recusat(c, "bassus", II, "t90 d2.", &v));
    /* invenire: silentium, lineae non scriptae, carmen ignotum */
    CREDO_VERUM (_substituere_recusat(c, "bassus", V, "d2.", &v));
    CREDO_VERUM (_substituere_recusat(c, "sine", II, "d2.", &v));
    CREDO_VERUM (_substituere_recusat(c, "nulla", II, "d2.", &v));
}


/* ==================================================
 * Copia et transpositio (palette #4)
 * ================================================== */

/* G maior, deinde c maior a mensura 3; mensura 6 pausa desinit,
 * mensura 7 notam trans lineam fert; tympanum '@' */
hic_manens constans character LIBER_S[] =
    "<carmina><carmen titulus=\"s\" clavis=\"g maior\">"
    "<clavis mensura=\"3\" valor=\"c maior\"/>"
    "<pars titulus=\"melodia\" instrumentum=\"aether\"><frustum "
    "mensura=\"1\"><textus!>o4 f#4 g4 a4 b4 | c4 d8 e8 r4 >c#4 | c1 | "
    "v50 c4 d4 v100 e4 f4 | a4 b4 >c4 d4 | c2 r2 | c2. d2</textus>"
    "</frustum>"
    "</pars>"
    "<pars titulus=\"tympanum\" instrumentum=\"aether\"><frustum "
    "mensura=\"1\"><textus!>@punctum c1</textus></frustum></pars>"
    "</carmen></carmina>";

/* textus scriptoris; vacua si FALSUM (causa in *v) */
interior chorda
_scriptum (
              CredoContextus* c,
          constans character* pars,
                         i32  mensura,
       EmendatioTranspositio  modus,
                         s32  intervallum,
                         i32  meta,
              CarminisVitium* v)
{
              Carmina  liber;
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina, CCLVI);
               chorda  vacua;
      constans Carmen* k;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    memset(v, ZEPHYRUM, magnitudo(*v));
    si (   a == NIHIL
        || !carmina_legere(chorda_ex_literis(LIBER_S, c->piscina),
                           c->piscina, &liber, v))
    {
        redde vacua;
    }
    k = &liber.carmina[ZEPHYRUM];
    si (!emendatio_mensuram_scribere(k, chorda_ex_literis(pars,
            c->piscina), mensura, modus, intervallum,
            carmen_pulsus_mensurae(k, meta), a, v))
    {
        redde vacua;
    }
    redde chorda_aedificator_finire(a);
}

interior vacuum
_sectio_scriptor (
    CredoContextus* c)
{
    CarminisVitium v;

    /* idem: longitudines expressae, 'oN' initio et mutata */
    CREDO_CHORDA_AEQUALIS_LITERIS (_scriptum(c, "melodia", I,
        EMENDATIO_SEMITONIA, ZEPHYRUM, I, &v), "o4 f#4 g4 a4 b4");
    CREDO_CHORDA_AEQUALIS_LITERIS (_scriptum(c, "melodia", II,
        EMENDATIO_SEMITONIA, ZEPHYRUM, II, &v),
        "o4 c4 d8 e8 r4 o5 c#4");
    /* diatonice in clave eadem; alteratio ad signaturam servata */
    CREDO_CHORDA_AEQUALIS_LITERIS (_scriptum(c, "melodia", I,
        EMENDATIO_DIATONICE, II, I, &v), "o4 a4 b4 o5 c4 d4");
    CREDO_CHORDA_AEQUALIS_LITERIS (_scriptum(c, "melodia", II,
        EMENDATIO_DIATONICE, I, II, &v), "o4 d4 e8 f#8 r4 o5 d#4");
    /* ex G maior in c maior: gradus servatur (F# -> B, VII manet
     * VII) */
    CREDO_CHORDA_AEQUALIS_LITERIS (_scriptum(c, "melodia", I,
        EMENDATIO_DIATONICE, ZEPHYRUM, III, &v), "o4 b4 o5 c4 d4 e4");
    /* lucrum; octava transiens */
    CREDO_CHORDA_AEQUALIS_LITERIS (_scriptum(c, "melodia", IV,
        EMENDATIO_SEMITONIA, ZEPHYRUM, IV, &v),
        "o5 v50 c4 d4 v100 e4 f4");
    CREDO_CHORDA_AEQUALIS_LITERIS (_scriptum(c, "melodia", V,
        EMENDATIO_DIATONICE, II, V, &v), "o6 c4 d4 e4 f4");
    /* semitonia: orthographia ex clave metae (c maior: diesis) */
    CREDO_CHORDA_AEQUALIS_LITERIS (_scriptum(c, "melodia", III,
        EMENDATIO_SEMITONIA, III, III, &v), "o5 d#1");
    /* recusationes */
    /* pausa in fine mensurae */
    CREDO_CHORDA_AEQUALIS_LITERIS (_scriptum(c, "melodia", VI,
        EMENDATIO_SEMITONIA, ZEPHYRUM, VI, &v), "o6 c2 r2");
    CREDO_CHORDA_VACUA (_scriptum(c, "melodia", VII,
        EMENDATIO_SEMITONIA,
        ZEPHYRUM, VII, &v));
    CREDO_LITERAE_AEQUALES (v.causa, "nota trans lineam mensurae");
    CREDO_CHORDA_VACUA (_scriptum(c, "tympanum", I, EMENDATIO_SEMITONIA,
        ZEPHYRUM, I, &v));
    CREDO_LITERAE_AEQUALES (v.causa, "sonitus '@' non transponitur");
    CREDO_CHORDA_VACUA (_scriptum(c, "melodia", V, EMENDATIO_DIATONICE,
        XXI, V, &v));
    CREDO_LITERAE_AEQUALES (v.causa, "octava extra MML (0..8)");
    /* C# +3 in G maior: F## */
    CREDO_CHORDA_VACUA (_scriptum(c, "melodia", II, EMENDATIO_DIATONICE,
        III, II, &v));
    CREDO_LITERAE_AEQUALES (v.causa,
        "MML alterationem unam scribit (# aut b)");
}

hic_manens constans character LIBER_C[] =
    "<carmina><carmen titulus=\"c\">"
    "<pars titulus=\"melodia\" instrumentum=\"aether\"><frustum "
    "mensura=\"1\"><textus!>o4 c4 d4 e4 f4 | g1 | a1 | b1 | >c1 | d1 | "
    "e1 | f1</textus></frustum></pars>"
    "<pars titulus=\"bassus\" instrumentum=\"aether\"><frustum "
    "mensura=\"1\"><textus!>o3 c1 | c1 | c1 | c1 | c1 | c1 | c1 | c1"
    "</textus></frustum></pars>"
    "<pars titulus=\"tympanum\" instrumentum=\"aether\"><frustum "
    "mensura=\"1\"><textus!>@punctum c1 | c1</textus></frustum></pars>"
    "</carmen></carmina>";

/* copia; FALSUM -> causa imprimitur */
interior b32
_copiare (
              CredoContextus* c,
                      chorda  liber,
          constans character* fons,
                         i32  prima,
                         i32  ultima,
          constans character* meta,
                         i32  k,
       EmendatioTranspositio  modus,
                         s32  intervallum,
                         i32  repetitiones,
                      chorda* novus,
                         i32* mutatae,
              CarminisVitium* v)
{
    EmendatioCopia x;
               b32 r;

    x.pars_fons     = chorda_ex_literis(fons, c->piscina);
    x.prima         = prima;
    x.ultima        = ultima;
    x.pars_meta     = chorda_ex_literis(meta, c->piscina);
    x.meta          = k;
    x.modus         = modus;
    x.intervallum   = intervallum;
    x.repetitiones  = repetitiones;
    memset(v, ZEPHYRUM, magnitudo(*v));
    *novus = chorda_ex_literis("intactus", c->piscina);
    r = emendatio_mensuras_copiare(liber, chorda_ex_literis("c",
            c->piscina), &x, c->piscina, novus, mutatae, v);
    si (!r)
    {
        imprimere("  copia %s %u-%u -> %s %u: %s\n", fons, prima,
            ultima,
                  meta, k, v->causa ? v->causa : "-");
    }
    redde r;
}

interior vacuum
_sectio_copia (
    CredoContextus* c)
{
            chorda liber = chorda_ex_literis(LIBER_C, c->piscina);
            chorda novus;
               i32 n;
    CarminisVitium v;

    /* copia simplex +2 in mensuram 3 */
    CREDO_VERUM (_copiare(c, liber, "melodia", I, I, "melodia", III,
        EMENDATIO_DIATONICE, II, I, &novus, &n, &v));
    CREDO_AEQUALIS_I32 (n, I);
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber, "| a1 |",
        "| o4 e4 f4 g4 a4 |"));
    /* sequentia: 1 -> 2, gradu infra, ter */
    CREDO_VERUM (_copiare(c, liber, "melodia", I, I, "melodia", II,
        EMENDATIO_DIATONICE, -I, III, &novus, &n, &v));
    CREDO_AEQUALIS_I32 (n, III);
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber,
        "| g1 | a1 | b1 |",
        "| o3 b4 o4 c4 d4 e4 | o3 a4 b4 o4 c4 d4 | "
        "o3 g4 a4 b4 o4 c4 |"));
    /* in partem aliam, octava infra (-7 gradus) */
    CREDO_VERUM (_copiare(c, liber, "melodia", I, I, "bassus", II,
        EMENDATIO_DIATONICE, -VII, I, &novus, &n, &v));
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber,
        "o3 c1 | c1 | c1", "o3 c1 | o3 c4 d4 e4 f4 | c1"));
    /* pars metae vacua = pars fontis */
    CREDO_VERUM (_copiare(c, liber, "melodia", I, I, "", III,
        EMENDATIO_DIATONICE, II, I, &novus, &n, &v));
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber, "| a1 |",
        "| o4 e4 f4 g4 a4 |"));
    /* meta fontem tegit: fons ex libro originali */
    CREDO_VERUM (_copiare(c, liber, "melodia", I, II, "melodia", II,
        EMENDATIO_SEMITONIA, ZEPHYRUM, I, &novus, &n, &v));
    CREDO_AEQUALIS_I32 (n, II);
    CREDO_CHORDA_AEQUALIS (novus, _exspectatus(c, liber,
        "| g1 | a1 |", "| o4 c4 d4 e4 f4 | o4 g1 |"));
    /* recusationes: fons extra carmen, meta extra frusta, meta '@' */
    CREDO_FALSUM (_copiare(c, liber, "melodia", IX, IX, "melodia", I,
        EMENDATIO_SEMITONIA, ZEPHYRUM, I, &novus, &n, &v));
    CREDO_CHORDA_AEQUALIS_LITERIS (novus, "intactus");
    CREDO_FALSUM (_copiare(c, liber, "melodia", I, I, "melodia", IX,
        EMENDATIO_SEMITONIA, ZEPHYRUM, I, &novus, &n, &v));
    CREDO_FALSUM (_copiare(c, liber, "melodia", I, I, "tympanum", II,
        EMENDATIO_SEMITONIA, ZEPHYRUM, I, &novus, &n, &v));
    CREDO_LITERAE_AEQUALES (v.causa, "meta in statu '@' (sonitus)");
    CREDO_CHORDA_AEQUALIS_LITERIS (novus, "intactus");
}

#define LIBER_REPETITUS \
    "<carmina><carmen titulus=\"r\">" \
    "<sectio titulus=\"a\" mensura=\"1\" usque=\"3\"/>" \
    "<repetitio sectio=\"a\" mensura=\"3\" semitonia=\"2\"/>" \
    "<pars titulus=\"m\" instrumentum=\"aether\">" \
    "<frustum mensura=\"1\"><textus!>o4 c1 | d1</textus></frustum>" \
    "<frustum mensura=\"5\"><textus!>e1</textus></frustum>" \
    "</pars></carmen></carmina>"

/* repetitio: emendatio in iteratis recusatur fontem nominans; frustum
 * scriptum POST iteratum (index exemplaris II, STML I) recte mutatur */
interior vacuum
_sectio_repetitio (
    CredoContextus* c)
{
               chorda liber = chorda_ex_literis(LIBER_REPETITUS,
                                                 c->piscina);
               chorda novus;
     EmendatioMutatio mutatio;
       CarminisVitium vitium;
       EmendatioCopia x;
                  i32 mutatae;

    CREDO_FALSUM (_ponere(c, liber, "r", "m", _locus(III, I, ZEPHYRUM),
                          "G4", &novus, &mutatio, &vitium));
    CREDO_VERUM (   vitium.causa != NIHIL
                 && strstr(vitium.causa, "repetitio") != NIHIL);
    CREDO_NECESSE (_ponere(c, liber, "r", "m", _locus(V, I, ZEPHYRUM),
                           "F4", &novus, &mutatio, &vitium));
    CREDO_VERUM (chorda_aequalis(novus, _exspectatus(c, liber,
        ">e1</textus>", ">f1</textus>")));
    memset(&vitium, ZEPHYRUM, magnitudo(vitium));
    CREDO_FALSUM (emendatio_mensuram_substituere(liber,
        chorda_ex_literis("r", c->piscina),
        chorda_ex_literis("m", c->piscina), IV,
        chorda_ex_literis("e1", c->piscina), c->piscina, &novus,
        &mutatio, &vitium));
    CREDO_VERUM (   vitium.causa != NIHIL
                 && strstr(vitium.causa, "repetitio") != NIHIL);
    memset(&x, ZEPHYRUM, magnitudo(x));
    x.pars_fons     = chorda_ex_literis("m", c->piscina);
    x.prima         = III;
    x.ultima        = III;
    x.pars_meta     = chorda_ex_literis("m", c->piscina);
    x.meta          = V;
    x.modus         = EMENDATIO_SEMITONIA;
    x.intervallum   = ZEPHYRUM;
    x.repetitiones  = I;
    memset(&vitium, ZEPHYRUM, magnitudo(vitium));
    CREDO_FALSUM (emendatio_mensuras_copiare(liber,
        chorda_ex_literis("r", c->piscina), &x, c->piscina, &novus,
        &mutatae, &vitium));
    CREDO_VERUM (   vitium.causa != NIHIL
                 && strstr(vitium.causa, "repetitio") != NIHIL);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "locus", _sectio_locus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ponere", _sectio_ponere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "composita", _sectio_composita, NIHIL, NIHIL, NIHIL, NIHIL },
    { "iterum", _sectio_iterum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitia", _sectio_vitia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "invenire", _sectio_invenire, NIHIL, NIHIL, NIHIL, NIHIL },
    { "substituere", _sectio_substituere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mensura_iterum", _sectio_mensura_iterum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "mensura_vitia", _sectio_mensura_vitia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "scriptor", _sectio_scriptor, NIHIL, NIHIL, NIHIL, NIHIL },
    { "copia", _sectio_copia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "repetitio", _sectio_repetitio, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("emendatio", SECTIONES);
}
