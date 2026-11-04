/* probatio_gabc.c - lector gabc (include/gabc.h)
 *
 * Fixa in probationes/fixa/gabc/ (scripta manu, una res quaeque;
 * linea I commentarium '%' dicit quid figat). Plan:
 * project-specs/gabc-plan.md T2 (nucleus): caput, syllabae, textus
 * simplex, notae, neumae, claves, divisiones, tabula canonica; T3
 * formae, signa rhythmica, alterationes; T4 spatia, fractiones,
 * custodes, textus ornatus, signa opaca, vitia formae, totalitas; T5
 * figurae neumarum (G4: tabula FIGURAE; columna figura in tabula).
 *
 * GRAMMATICA TABULAE (gabc_tabulam_scribere), linea una per rem,
 * columnae tabulis separatae; textus effugitur (\t \n \r \\):
 *   caput    clavis  valor
 *   syllaba  index  verbum_incipit(0|1)  textus
 *   clavis   c|f  linea  bemolle(0|1)
 *   neuma    prima  numerus  figura|-
 *   nota     positio  gradus  alteratio  formae|-  mora  episema
 *            ictus  linea:columna
 *   divisio  genus  numerus  alta(0|1)  parenthesis(0|1)
 *   alteratio genus  positio  molle(0|1)  parenthesis(0|1)
 *   spatium  genus  factor|-
 *   fractio  iustificata(0|1)  custos(-1|0|1)
 *   custos   positio|-
 *   signum   textus  nota
 * post lineam syllabae, si syllaba ornata est (cursus plures, aut genus
 * non cantatus, aut stilus):
 *   textus   genus(cantatus|translatio|supra|verbatim)  stilus|-  textus
 * et si adsunt:
 *   vocalis  initium  finis
 *   signa    litterae
 * formae: littera una per bit, ordine bitorum:
 *   I v V s o O w W r R = q ~ < > - @
 * stilus: i b u c s t e p (italicus crassus sublineatus coloratus
 * capitella teletypus elisio protrusio); signa: n N e E p (nlba initium
 * finis, euouae initium finis, purga). Spatium "!" ante spatium aliud
 * (inseparabile Gregorii) = duo elementa: nullum, deinde spatium.
 * CAUTIO: formae '-' et littera initio debilis '-' idem scribuntur
 * (nota "-g" et "g" in tabula aequales); figura eas distinguit.
 */

#include "latina.h"
#include "credo.h"
#include "gabc.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "piscina.h"
#include "sors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior GabcCantus*
_legere_fixum (
         CredoContextus* c,
     constans character* titulus)
{
    character via[CCLVI];

    strcpy(via, "probationes/fixa/gabc/");
    strcat(via, titulus);
    redde gabc_legere(filum_legere_totum(via, c->piscina), c->piscina);
}

interior chorda
_tabula (
         CredoContextus* c,
    constans GabcCantus* cantus)
{
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina, MMM);

    si (!gabc_tabulam_scribere(cantus, a))
    {
        redde chorda_ex_literis("(tabula fracta)", c->piscina);
    }
    redde chorda_aedificator_finire(a);
}

interior b32
_aequat (
                 chorda  ch,
     constans character* literae)
{
    i32 n = (i32)strlen(literae);

    redde ch.mensura == n && (n == ZEPHYRUM
        || memcmp(ch.datum, literae, (size_t)n) == ZEPHYRUM);
}

/* textus cantatus syllabae (cursus omnes coniuncti) aequat? */
interior b32
_textus_aequat (
    constans GabcSyllaba* s,
      constans character* literae)
{
    character receptaculum[CCLVI];
          i32 n = ZEPHYRUM;
          i32 r;

    per (r = ZEPHYRUM; r < s->numerus_textuum; r++)
    {
        si (   s->textus[r].genus == GABC_TEXTUS_CANTATUS
            && n + (i32)s->textus[r].textus.mensura < CCLVI)
        {
            memcpy(receptaculum + n, s->textus[r].textus.datum,
                (size_t)s->textus[r].textus.mensura);
            n += (i32)s->textus[r].textus.mensura;
        }
    }
    redde n == (i32)strlen(literae)
        && memcmp(receptaculum, literae, (size_t)n) == ZEPHYRUM;
}

interior vacuum
_caput_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "caput.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_VERUM (gabc_successus(g));
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (g->numerus_capitum, (i32)VI);
    CREDO_VERUM (_aequat(g->titulus, "Salve"));
    CREDO_VERUM (_aequat(g->modus, "5"));
    CREDO_AEQUALIS_S32 (g->modus_numerus, (s32)V);
    CREDO_AEQUALIS_I32 (g->numerus_annotationum, (i32)II);
    CREDO_VERUM (_aequat(g->annotationes[ZEPHYRUM], "Ant."));
    CREDO_VERUM (_aequat(g->annotationes[I], "V"));
    CREDO_AEQUALIS_I32 (g->lineae, (i32)IV);
    CREDO_AEQUALIS_I32 (g->capita[ZEPHYRUM].linea, (i32)II);
    CREDO_AEQUALIS_I32 (g->capita[V].linea, (i32)VIII);
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\tSalve\n"
        "caput\tmode\t5\n"
        "caput\tannotation\tAnt.\n"
        "caput\tannotation\tV\n"
        "caput\tfoo\tbar\n"
        "caput\tcommentary\tunus\\nduo\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\ta\n"
        "neuma\t0\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t11:8\n");
}

interior vacuum
_syllabas_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "syllabae.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_NECESSE (g->numerus_syllabarum == VI);
    /* syllaba 0: clavis sola, sine textu */
    CREDO_VERUM (_textus_aequat(&g->syllabae[ZEPHYRUM], ""));
    CREDO_FALSUM (g->syllabae[ZEPHYRUM].verbum_incipit);
    CREDO_VERUM (_textus_aequat(&g->syllabae[I], "Sal"));
    CREDO_VERUM (_textus_aequat(&g->syllabae[II], "ve"));
    CREDO_VERUM (_textus_aequat(&g->syllabae[III], "Re"));
    CREDO_VERUM (_textus_aequat(&g->syllabae[IV], "gi"));
    CREDO_VERUM (_textus_aequat(&g->syllabae[V], "na"));
    CREDO_VERUM (g->syllabae[I].verbum_incipit);
    CREDO_FALSUM (g->syllabae[II].verbum_incipit);
    CREDO_VERUM (g->syllabae[III].verbum_incipit);
    CREDO_FALSUM (g->syllabae[IV].verbum_incipit);
    CREDO_FALSUM (g->syllabae[V].verbum_incipit);
    CREDO_AEQUALIS_I32 (g->syllabae[III].linea, (i32)IV);
    CREDO_AEQUALIS_I32 (g->syllabae[III].columna, (i32)XVIII);
    CREDO_AEQUALIS_S32 (g->syllabae[III].vocalis_initium, (s32)-I);
}

interior vacuum
_neumas_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "neumae.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (g->numerus_notarum, (i32)IX);
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\tneumae\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\ta\n"
        "neuma\t0\t2\tpes\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:8\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:9\n"
        "syllaba\t2\t1\tb\n"
        "neuma\t2\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:14\n"
        "spatium\tglyphae\t-\n"
        "neuma\t3\t1\tpunctum\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:16\n"
        "syllaba\t3\t1\tc\n"
        "neuma\t4\t2\tpes\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:21\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:22\n"
        "spatium\tneumaticum\t-\n"
        "neuma\t6\t2\tclivis\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:24\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:25\n"
        "syllaba\t4\t1\td\n"
        "neuma\t8\t1\tpunctum\n"
        "nota\tj\t0\t0\t-\t0\t0\t0\t4:30\n");
}

interior vacuum
_claves_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "claves.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    /* c4: do in j; f3: fa in h, do in e; c1: do in d */
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\tclaves\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\ta\n"
        "neuma\t0\t1\tpunctum\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:8\n"
        "syllaba\t2\t0\t\n"
        "clavis\tf\t3\t0\n"
        "syllaba\t3\t1\tb\n"
        "neuma\t1\t1\tpunctum\n"
        "nota\th\t3\t0\t-\t0\t0\t0\t4:18\n"
        "syllaba\t4\t0\t\n"
        "clavis\tc\t1\t0\n"
        "syllaba\t5\t1\tc\n"
        "neuma\t2\t1\tpunctum\n"
        "nota\td\t0\t0\t-\t0\t0\t0\t4:28\n");
}

interior vacuum
_divisiones_probare (
    CredoContextus* c)
{
           GabcCantus* g = _legere_fixum(c, "divisiones.gabc");
    ChordaAedificator* a;

    CREDO_NECESSE (g != NIHIL);
    CREDO_VERUM (gabc_successus(g));
    CREDO_NECESSE (g->numerus_vitiorum == I);
    CREDO_AEQUALIS_S32 ((s32)g->vitia[ZEPHYRUM].gravitas,
        (s32)GABC_MONITUM);
    CREDO_AEQUALIS_S32 ((s32)g->vitia[ZEPHYRUM].genus,
        (s32)GABC_DIVISIO_IN_NEUMA);
    a = chorda_aedificator_creare(c->piscina, CCLVI);
    CREDO_VERUM (gabc_vitia_scribere(g, "divisiones.gabc", a));
    CREDO_CHORDA_AEQUALIS_LITERIS (chorda_aedificator_finire(a),
        "divisiones.gabc:4:45: monitum: divisio intra notas"
        " coniunctas\n");
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\tdivisiones\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\ta\n"
        "neuma\t0\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:8\n"
        "syllaba\t2\t0\t\n"
        "divisio\tvirgula\t0\t0\t0\n"
        "syllaba\t3\t1\tb\n"
        "neuma\t1\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:17\n"
        "syllaba\t4\t0\t\n"
        "divisio\tminima\t0\t0\t0\n"
        "syllaba\t5\t1\tc\n"
        "neuma\t2\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:26\n"
        "syllaba\t6\t0\t\n"
        "divisio\tminor\t0\t0\t0\n"
        "syllaba\t7\t1\td\n"
        "neuma\t3\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:35\n"
        "syllaba\t8\t0\t\n"
        "divisio\tmaior\t0\t0\t0\n"
        "syllaba\t9\t1\te\n"
        "neuma\t4\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:44\n"
        "divisio\tminor\t0\t0\t0\n"
        "neuma\t5\t1\tpunctum\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:46\n"
        "syllaba\t10\t0\t\n"
        "divisio\tfinalis\t0\t0\t0\n"
        "syllaba\t11\t0\t\n"
        "divisio\tminimis\t0\t0\t0\n"
        "syllaba\t12\t0\t\n"
        "divisio\tmaior_punctata\t0\t0\t0\n"
        "syllaba\t13\t0\t\n"
        "divisio\tminima\t0\t1\t0\n"
        "syllaba\t14\t0\t\n"
        "divisio\tvirgula\t0\t0\t1\n"
        "syllaba\t15\t0\t\n"
        "divisio\tdominicana\t3\t0\t0\n");
}

/* Review Focus 3: textus sine notis et notae sine textu locum servant */
interior vacuum
_textum_sine_notis_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "textus_sine_notis.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_NECESSE (g->numerus_syllabarum == VI);
    CREDO_AEQUALIS_I32 (g->syllabae[III].numerus_elementorum, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\ttextus\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\tve\n"
        "neuma\t0\t2\tpes\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:9\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:10\n"
        "syllaba\t2\t0\t\n"
        "neuma\t2\t2\tclivis\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:14\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:15\n"
        "syllaba\t3\t1\t*\n"
        "syllaba\t4\t1\tre\n"
        "neuma\t4\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:25\n"
        "syllaba\t5\t0\t\n"
        "divisio\tfinalis\t0\t0\t0\n");
}

/* fons verus: syllabae CXLI et notae CXLV (numeratae extra lectorem,
 * per instrumenta crustae, T2 gradus I) */
interior vacuum
_salve_legere_probare (
    CredoContextus* c)
{
    GabcCantus* g = gabc_legere(filum_legere_totum(
        "musica/fontes/salve_2435.gabc", c->piscina), c->piscina);
           i32 k;
           i32 ictus;
           i32 mora;
           i32 episema;
           i32 quilisma;
           i32 e;
           i32 signa;
           i32 spatia;
           i32 vocales;

    CREDO_NECESSE (g != NIHIL);
    CREDO_VERUM (gabc_successus(g));
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (g->numerus_capitum, (i32)V);
    CREDO_VERUM (_aequat(g->titulus, "Salve Regina (simple tone)"));
    CREDO_AEQUALIS_S32 (g->modus_numerus, (s32)V);
    CREDO_AEQUALIS_I32 (g->numerus_syllabarum, (i32)CXLI);
    CREDO_AEQUALIS_I32 (g->numerus_notarum, (i32)CXLV);
    CREDO_VERUM (_textus_aequat(&g->syllabae[I], "SAl"));
    CREDO_VERUM (g->syllabae[I].verbum_incipit);
    /* T3: signa numerata per crustam (' 39, . 14, _ 8, w 2) */
    ictus = mora = episema = quilisma = ZEPHYRUM;
    per (k = ZEPHYRUM; k < g->numerus_notarum; k++)
    {
        ictus    += g->notae[k].ictus != ZEPHYRUM ? I : ZEPHYRUM;
        mora     += (i32)g->notae[k].mora;
        episema  += g->notae[k].episema != ZEPHYRUM ? I : ZEPHYRUM;
        quilisma += (g->notae[k].formae & GABC_FORMA_QUILISMA)
            != ZEPHYRUM
            ? I : ZEPHYRUM;
    }
    CREDO_AEQUALIS_I32 (ictus, (i32)XXXIX);
    CREDO_AEQUALIS_I32 (mora, (i32)XIV);
    CREDO_AEQUALIS_I32 (episema, (i32)VIII);
    CREDO_AEQUALIS_I32 (quilisma, (i32)II);
    /* T4: [ll:1] III + [oh:h] VI; ! II, / I, // I; {ia} (crusta) */
    signa = spatia = vocales = ZEPHYRUM;
    per (k = ZEPHYRUM; k < g->numerus_syllabarum; k++)
    {
        vocales += g->syllabae[k].vocalis_initium >= ZEPHYRUM
            ? I : ZEPHYRUM;
        per (e = ZEPHYRUM; e < g->syllabae[k].numerus_elementorum; e++)
        {
            signa += g->syllabae[k].elementa[e].genus
                == GABC_EL_SIGNUM_OPACUM ? I : ZEPHYRUM;
            spatia += g->syllabae[k].elementa[e].genus
                == GABC_EL_SPATIUM ? I : ZEPHYRUM;
        }
    }
    CREDO_AEQUALIS_I32 (signa, (i32)IX);
    CREDO_AEQUALIS_I32 (spatia, (i32)IV);
    CREDO_AEQUALIS_I32 (vocales, (i32)I);
}

/* T3: formae - littera una per bit (ordo in capite plagulae); vv sss
 * notas iterant; directio et signa r1-r5 in structura */
interior vacuum
_formas_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "formae.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_NECESSE (g->numerus_notarum == XXX);
    CREDO_AEQUALIS_S32 ((s32)g->notae[XVII].directio, (s32)-I);
    CREDO_AEQUALIS_S32 ((s32)g->notae[XVIII].directio, (s32)I);
    CREDO_AEQUALIS_S32 ((s32)g->notae[XIX].directio, (s32)I);
    CREDO_AEQUALIS_I32 ((i32)g->notae[XXV].signa,
        (i32)GABC_SIGNUM_ACCENTUS);
    CREDO_AEQUALIS_I32 ((i32)g->notae[XXVI].signa,
        (i32)GABC_SIGNUM_ACCENTUS_REVERSUS);
    CREDO_AEQUALIS_I32 ((i32)g->notae[XXVII].signa,
        (i32)GABC_SIGNUM_CIRCULUS);
    CREDO_AEQUALIS_I32 ((i32)g->notae[XXVIII].signa,
        (i32)GABC_SIGNUM_SEMICIRCULUS);
    CREDO_AEQUALIS_I32 ((i32)g->notae[XXIX].signa,
        (i32)GABC_SIGNUM_SEMICIRCULUS_REVERSUS);
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\tformae\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\ta\n"
        "neuma\t0\t1\tvirga\n"
        "nota\tg\t-3\t0\tv\t0\t0\t0\t4:8\n"
        "spatium\tglyphae\t-\n"
        "neuma\t1\t1\t-\n"
        "nota\tg\t-3\t0\tV\t0\t0\t0\t4:11\n"
        "spatium\tglyphae\t-\n"
        "neuma\t2\t1\tstropha\n"
        "nota\tg\t-3\t0\ts\t0\t0\t0\t4:14\n"
        "spatium\tglyphae\t-\n"
        "neuma\t3\t1\toriscus\n"
        "nota\tg\t-3\t0\to\t0\t0\t0\t4:17\n"
        "spatium\tglyphae\t-\n"
        "neuma\t4\t1\toriscus\n"
        "nota\tg\t-3\t0\tO\t0\t0\t0\t4:20\n"
        "spatium\tglyphae\t-\n"
        "neuma\t5\t1\t-\n"
        "nota\tg\t-3\t0\tw\t0\t0\t0\t4:23\n"
        "spatium\tglyphae\t-\n"
        "neuma\t6\t1\t-\n"
        "nota\tg\t-3\t0\tW\t0\t0\t0\t4:26\n"
        "spatium\tglyphae\t-\n"
        "neuma\t7\t1\tpunctum\n"
        "nota\tg\t-3\t0\tr\t0\t0\t0\t4:29\n"
        "spatium\tglyphae\t-\n"
        "neuma\t8\t1\t-\n"
        "nota\tg\t-3\t0\tR\t0\t0\t0\t4:32\n"
        "spatium\tglyphae\t-\n"
        "neuma\t9\t1\t-\n"
        "nota\tg\t-3\t0\trR\t0\t0\t0\t4:35\n"
        "spatium\tglyphae\t-\n"
        "neuma\t10\t1\t-\n"
        "nota\tg\t-3\t0\t=\t0\t0\t0\t4:39\n"
        "spatium\tglyphae\t-\n"
        "neuma\t11\t1\t-\n"
        "nota\tg\t-3\t0\tq\t0\t0\t0\t4:42\n"
        "spatium\tglyphae\t-\n"
        "neuma\t12\t1\tpunctum\n"
        "nota\tg\t-3\t0\t~\t0\t0\t0\t4:45\n"
        "spatium\tglyphae\t-\n"
        "neuma\t13\t1\tpunctum\n"
        "nota\tg\t-3\t0\t<\t0\t0\t0\t4:48\n"
        "spatium\tglyphae\t-\n"
        "neuma\t14\t1\tpunctum\n"
        "nota\tg\t-3\t0\t>\t0\t0\t0\t4:51\n"
        "spatium\tglyphae\t-\n"
        "neuma\t15\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:55\n"
        "spatium\tglyphae\t-\n"
        "neuma\t16\t1\tpunctum\n"
        "nota\tg\t-3\t0\t@\t0\t0\t0\t4:58\n"
        "spatium\tglyphae\t-\n"
        "neuma\t17\t1\tpunctum inclinatum\n"
        "nota\tg\t-3\t0\tI\t0\t0\t0\t4:60\n"
        "spatium\tglyphae\t-\n"
        "neuma\t18\t1\tpunctum inclinatum\n"
        "nota\tg\t-3\t0\tI\t0\t0\t0\t4:62\n"
        "spatium\tglyphae\t-\n"
        "neuma\t19\t1\toriscus\n"
        "nota\tg\t-3\t0\to\t0\t0\t0\t4:65\n"
        "syllaba\t2\t1\tb\n"
        "neuma\t20\t2\tbivirga\n"
        "nota\tg\t-3\t0\tv\t0\t0\t0\t5:3\n"
        "nota\tg\t-3\t0\tv\t0\t0\t0\t5:5\n"
        "spatium\tglyphae\t-\n"
        "neuma\t22\t3\ttristropha\n"
        "nota\tg\t-3\t0\ts\t0\t0\t0\t5:7\n"
        "nota\tg\t-3\t0\ts\t0\t0\t0\t5:9\n"
        "nota\tg\t-3\t0\ts\t0\t0\t0\t5:10\n"
        "spatium\tglyphae\t-\n"
        "neuma\t25\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:12\n"
        "spatium\tglyphae\t-\n"
        "neuma\t26\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:16\n"
        "spatium\tglyphae\t-\n"
        "neuma\t27\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:20\n"
        "spatium\tglyphae\t-\n"
        "neuma\t28\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:24\n"
        "spatium\tglyphae\t-\n"
        "neuma\t29\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:28\n");
}

/* T3: mora (. ..), episema (_ cum numeris), ictus (') et loci */
interior vacuum
_signa_rhythmica_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "signa_rhythmica.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_NECESSE (g->numerus_notarum == IX);
    CREDO_AEQUALIS_S32 ((s32)g->notae[ZEPHYRUM].mora_locus, (s32)-I);
    CREDO_AEQUALIS_S32 ((s32)g->notae[II].mora_locus, (s32)I);
    CREDO_AEQUALIS_S32 ((s32)g->notae[VI].ictus_locus, (s32)-I);
    CREDO_AEQUALIS_S32 ((s32)g->notae[VII].ictus_locus, (s32)I);
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\tsigna\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\ta\n"
        "neuma\t0\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t1\t0\t0\t4:8\n"
        "spatium\tglyphae\t-\n"
        "neuma\t1\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t2\t0\t0\t4:11\n"
        "spatium\tglyphae\t-\n"
        "neuma\t2\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t1\t0\t0\t4:15\n"
        "spatium\tglyphae\t-\n"
        "neuma\t3\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t1\t0\t4:19\n"
        "spatium\tglyphae\t-\n"
        "neuma\t4\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t3\t0\t4:22\n"
        "spatium\tglyphae\t-\n"
        "neuma\t5\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t13\t0\t4:26\n"
        "spatium\tglyphae\t-\n"
        "neuma\t6\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t1\t4:31\n"
        "spatium\tglyphae\t-\n"
        "neuma\t7\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t1\t4:34\n"
        "spatium\tglyphae\t-\n"
        "neuma\t8\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t1\t1\t1\t4:38\n");
}

/* Review Focus 1: alteratio valet usque ad finem verbi, divisionem,
 * signum aliud in eadem positione; positio sola (non octava); ficta
 * (r6-r8) notam solam; molle et parenthesis alterant */
interior vacuum
_alterationis_ambitum_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "alterationes.gabc");
           s32  sperata[XIV];
           i32  k;

    /* Sa(ixih) lu(i) | ta(i) | mi(ixi iyi) | re(ixib) | do(ixi) (,) (i)
     * | fa(gr6g) | la(iXi) | si(ix?i) */
    sperata[ZEPHYRUM] = -I;  sperata[I] = ZEPHYRUM;  sperata[II] = -I;
    sperata[III] = ZEPHYRUM; sperata[IV] = -I;       sperata[V] =
                                                         ZEPHYRUM;
    sperata[VI] = -I;        sperata[VII] = ZEPHYRUM; sperata[VIII] =
                                                          -I;
    sperata[IX] = ZEPHYRUM;  sperata[X] = -I;        sperata[XI] =
                                                         ZEPHYRUM;
    sperata[XII] = -I;       sperata[XIII] = -I;

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_NECESSE (g->numerus_notarum == XIV);
    per (k = ZEPHYRUM; k < XIV; k++)
    {
        CREDO_AEQUALIS_S32 (g->notae[k].alteratio, sperata[k]);
    }
    CREDO_AEQUALIS_I32 ((i32)g->notae[X].signa, (i32)GABC_SIGNUM_FICTA);
    CREDO_AEQUALIS_I32 ((i32)g->notae[XI].signa, ZEPHYRUM);
    /* la(iXi): signum molle; si(ix?i): parenthesis */
    CREDO_NECESSE (g->syllabae[X].numerus_elementorum == II);
    CREDO_AEQUALIS_S32 ((s32)g->syllabae[X].elementa[ZEPHYRUM].genus,
        (s32)GABC_EL_ALTERATIO);
    CREDO_VERUM (g->syllabae[X].elementa[ZEPHYRUM].molle);
    CREDO_NECESSE (g->syllabae[XI].numerus_elementorum == II);
    CREDO_VERUM (g->syllabae[XI].elementa[ZEPHYRUM].parenthesis);
    CREDO_FALSUM (g->syllabae[XI].elementa[ZEPHYRUM].molle);
}

/* Review Focus 2: clavis mutata sequentes solas resolvit (etiam intra
 * syllabam); cb3 si bemolle usque ad clavem proximam */
interior vacuum
_clavem_mutatam_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "claves_mutatae.gabc");
           s32  gradus[IX];
           s32  alteratio[IX];
           i32  k;

    /* a(g h | c3 | h g) (cb3) b(g n) c(gyg) (c4) d(g i) */
    gradus[ZEPHYRUM] = -III; gradus[I] = -II; gradus[II]  = ZEPHYRUM;
    gradus[III]                                           =
        -I;        gradus[IV] = -I; gradus[V] = VI;
    gradus[VI] =
        -I;         gradus[VII] = -III; gradus[VIII] = -I;
    per (k = ZEPHYRUM; k < IX; k++)
    {
        alteratio[k] = (k == IV || k == V) ? -I : ZEPHYRUM;
    }

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_NECESSE (g->numerus_notarum == IX);
    per (k = ZEPHYRUM; k < IX; k++)
    {
        CREDO_AEQUALIS_S32 (g->notae[k].gradus, gradus[k]);
        CREDO_AEQUALIS_S32 (g->notae[k].alteratio, alteratio[k]);
    }
    CREDO_NECESSE (g->syllabae[I].numerus_elementorum == III);
    CREDO_AEQUALIS_S32 ((s32)g->syllabae[I].elementa[I].genus,
        (s32)GABC_EL_CLAVIS);
    CREDO_VERUM (g->syllabae[II].elementa[ZEPHYRUM].clavis_bemolle);
}

/* T4: spatia omnia; "!" ante spatium = nullum + spatium (inseparabile
 * Gregorii), "!/0" "!/!" = spatium solum (Gregorius "!" ibi neglegit) */
interior vacuum
_spatia_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "spatia.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\tspatia\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\ta\n"
        "neuma\t0\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:8\n"
        "spatium\tglyphae\t-\n"
        "neuma\t1\t1\tpunctum\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:10\n"
        "spatium\tneumaticum\t-\n"
        "neuma\t2\t1\tpunctum\n"
        "nota\ti\t-1\t0\t-\t0\t0\t0\t4:12\n"
        "spatium\tmagnum\t-\n"
        "neuma\t3\t1\tpunctum\n"
        "nota\tj\t0\t0\t-\t0\t0\t0\t4:15\n"
        "spatium\tdimidium\t-\n"
        "neuma\t4\t1\tpunctum\n"
        "nota\tk\t1\t0\t-\t0\t0\t0\t4:18\n"
        "spatium\tinseparabile\t-\n"
        "neuma\t5\t1\tpunctum\n"
        "nota\tl\t2\t0\t-\t0\t0\t0\t4:21\n"
        "spatium\tnullum\t-\n"
        "neuma\t6\t1\tpunctum\n"
        "nota\tm\t3\t0\t-\t0\t0\t0\t4:23\n"
        "syllaba\t2\t1\tb\n"
        "neuma\t7\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:28\n"
        "spatium\tnullum\t-\n"
        "spatium\tneumaticum\t-\n"
        "neuma\t8\t1\tpunctum\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:31\n"
        "spatium\tnullum\t-\n"
        "spatium\tmagnum\t-\n"
        "neuma\t9\t1\tpunctum\n"
        "nota\ti\t-1\t0\t-\t0\t0\t0\t4:35\n"
        "spatium\tnullum\t-\n"
        "spatium\tglyphae\t-\n"
        "neuma\t10\t1\tpunctum\n"
        "nota\tj\t0\t0\t-\t0\t0\t0\t4:38\n"
        "spatium\tnullum\t-\n"
        "spatium\tfactor\t2\n"
        "neuma\t11\t1\tpunctum\n"
        "nota\tk\t1\t0\t-\t0\t0\t0\t4:44\n"
        "spatium\tfactor\t-0.5\n"
        "neuma\t12\t1\tpunctum\n"
        "nota\tl\t2\t0\t-\t0\t0\t0\t4:52\n"
        "spatium\tneumaticum\t-\n"
        "spatium\tfactor\t3\n"
        "neuma\t13\t1\tpunctum\n"
        "nota\tm\t3\t0\t-\t0\t0\t0\t4:58\n"
        "syllaba\t3\t1\tc\n"
        "neuma\t14\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:63\n"
        "spatium\tdimidium\t-\n"
        "neuma\t15\t1\tpunctum\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:67\n"
        "spatium\tinseparabile\t-\n"
        "neuma\t16\t1\tpunctum\n"
        "nota\ti\t-1\t0\t-\t0\t0\t0\t4:71\n");
}

/* T4: z Z cum + - (custos coactus aut vetitus), z0 custos automaticus */
interior vacuum
_fractiones_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "fractiones.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\tfractiones\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\ta\n"
        "neuma\t0\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:8\n"
        "fractio\t1\t0\n"
        "neuma\t1\t1\tpunctum\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:10\n"
        "fractio\t0\t0\n"
        "neuma\t2\t1\tpunctum\n"
        "nota\ti\t-1\t0\t-\t0\t0\t0\t4:12\n"
        "syllaba\t2\t1\tb\n"
        "fractio\t1\t1\n"
        "neuma\t3\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:19\n"
        "fractio\t1\t-1\n"
        "neuma\t4\t1\tpunctum\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:22\n"
        "fractio\t0\t1\n"
        "neuma\t5\t1\tpunctum\n"
        "nota\ti\t-1\t0\t-\t0\t0\t0\t4:25\n"
        "fractio\t0\t-1\n"
        "syllaba\t3\t1\tc\n"
        "neuma\t6\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:32\n"
        "custos\t-\n"
        "syllaba\t4\t1\td\n"
        "neuma\t7\t1\tpunctum\n"
        "nota\tj\t0\t0\t-\t0\t0\t0\t4:39\n");
}

/* Review Focus 2 (ex T3): custos ante clavem mutatam positionem scriptam
 * servat; notae post clavem nova clave resolvuntur */
interior vacuum
_custodem_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "custos.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_NECESSE (g->syllabae[II].numerus_elementorum == II);
    CREDO_AEQUALIS_S32 ((s32)g->syllabae[II].elementa[ZEPHYRUM].genus,
        (s32)GABC_EL_CUSTOS);
    CREDO_AEQUALIS_S32 (g->syllabae[II].elementa[ZEPHYRUM].positio,
        (s32)VIII);
    CREDO_NECESSE (g->numerus_notarum == III);
    CREDO_AEQUALIS_S32 (g->notae[II].gradus, (s32)I);
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\tcustos\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\ta\n"
        "neuma\t0\t2\tpes\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:8\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:9\n"
        "syllaba\t2\t0\t\n"
        "custos\ti\n"
        "fractio\t1\t0\n"
        "syllaba\t3\t0\t\n"
        "clavis\tc\t3\t0\n"
        "syllaba\t4\t1\tb\n"
        "neuma\t2\t1\tpunctum\n"
        "nota\ti\t1\t0\t-\t0\t0\t0\t4:25\n");
}

/* T4: stili (b i per syllabas manent, e non), sp, translatio, [/], {},
 * alt, v (crudum, '(' intra), nlba, eu, clear, $ effugium, ~, pr */
interior vacuum
_textum_ornatum_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "textus_ornatus.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_NECESSE (g->numerus_syllabarum == XVIII);
    CREDO_AEQUALIS_S32 (g->syllabae[VI].vocalis_initium, (s32)I);
    CREDO_AEQUALIS_S32 (g->syllabae[VI].vocalis_finis, (s32)III);
    CREDO_NECESSE (g->syllabae[IV].numerus_textuum == II);
    CREDO_AEQUALIS_S32 ((s32)g->syllabae[IV].textus[I].genus,
        (s32)GABC_TEXTUS_TRANSLATIO);
    CREDO_AEQUALIS_I32 (g->syllabae[XV].textus[ZEPHYRUM].stilus,
        (i32)GABC_STILUS_ITALICUS);
    CREDO_AEQUALIS_I32 (g->syllabae[XI].signa,
        (i32)(GABC_SYLLABA_EUOUAE_INITIUM | GABC_SYLLABA_EUOUAE_FINIS));
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\ttextus ornatus\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\tAlle\n"
        "textus\tcantatus\tb\tAl\n"
        "textus\tcantatus\tib\tle\n"
        "neuma\t0\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:25\n"
        "syllaba\t2\t1\t\xE2\x84\x9F.\n"
        "neuma\t1\t1\tpunctum\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:41\n"
        "syllaba\t3\t1\tclara\n"
        "textus\tcantatus\t-\tcla\n"
        "textus\tcantatus\te\tra\n"
        "neuma\t2\t2\tpes\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:57\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:58\n"
        "syllaba\t4\t1\tDo\n"
        "textus\tcantatus\t-\tDo\n"
        "textus\ttranslatio\t-\tLord\n"
        "neuma\t4\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:70\n"
        "syllaba\t5\t1\tmi\n"
        "textus\tcantatus\t-\tmi\n"
        "textus\ttranslatio\t-\t/\n"
        "neuma\t5\t1\tpunctum\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:79\n"
        "syllaba\t6\t1\tever\n"
        "vocalis\t1\t3\n"
        "neuma\t6\t1\tpunctum\n"
        "nota\te\t-5\t0\t-\t0\t0\t0\t4:89\n"
        "syllaba\t7\t1\tve\n"
        "textus\tsupra\t-\tPs.\n"
        "textus\tcantatus\t-\tve\n"
        "neuma\t7\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:18\n"
        "syllaba\t8\t1\tna\n"
        "textus\tverbatim\t-\t\\\\foo(x)\n"
        "textus\tcantatus\t-\tna\n"
        "neuma\t8\t1\tpunctum\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t5:38\n"
        "syllaba\t9\t1\tet\n"
        "signa\tn\n"
        "neuma\t9\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:50\n"
        "syllaba\t10\t1\tin\n"
        "signa\tN\n"
        "neuma\t10\t1\tpunctum\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t5:63\n"
        "syllaba\t11\t1\tE\n"
        "signa\teE\n"
        "neuma\t11\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:77\n"
        "syllaba\t12\t1\ta(b\n"
        "signa\tp\n"
        "neuma\t12\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:92\n"
        "syllaba\t13\t1\tx\xE2\x80\xBFy\n"
        "neuma\t13\t1\tpunctum\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t5:99\n"
        "syllaba\t14\t1\tab\n"
        "textus\tcantatus\ti\tab\n"
        "neuma\t14\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t6:7\n"
        "syllaba\t15\t1\tcd\n"
        "textus\tcantatus\ti\tcd\n"
        "neuma\t15\t1\tpunctum\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t6:17\n"
        "syllaba\t16\t1\t\xC7\xBD\xC5\x93\xE2\x9C\xB6\xE2\x80\xA0\xE2\x84\xA3\n"
        "neuma\t16\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t6:75\n"
        "syllaba\t17\t1\tfi:\n"
        "textus\tcantatus\t-\tfi\n"
        "textus\tcantatus\tp\t:\n"
        "neuma\t17\t1\tpunctum\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t6:86\n");
}

/* T4 (G5): signum quodque cum textu fontis exacto et nota cui adhaeret
 * (nota ultima neumae apertae, aliter -1); [ll:1] neumam non frangit */
/* T5: figurae neumarum (G4). Regulae Gregorii
 * (gabc-glyphs-determination.c): unisonus puncta separat, debilis
 * glypham incipit, liquescens claudit; ignota numquam falsa. */
interior constans character* FIGURAE[][II] = {
    { "g",       "punctum" },
    { "gv",      "virga" },
    { "G",       "punctum inclinatum" },
    { "gs",      "stropha" },
    { "go",      "oriscus" },
    { "gh",      "pes" },
    { "hg",      "clivis" },
    { "ghg",     "torculus" },
    { "gih",     "torculus" },
    { "hgh",     "porrectus" },
    { "ghi",     "scandicus" },
    { "hGF",     "climacus" },
    { "hvGFE",   "climacus" },
    { "gwh",     "pes quilismaticus" },
    { "gWh",     "pes quilismaticus" },
    { "gqh",     "pes quadratus" },
    { "gvv",     "bivirga" },
    { "gvgv",    "bivirga" },
    { "gvvv",    "trivirga" },
    { "gss",     "distropha" },
    { "gsgsgs",  "tristropha" },
    { "hghg",    "porrectus flexus" },
    { "gihi",    "torculus resupinus" },
    { "giHG",    "pes subbipunctis" },
    { "gh~",     "epiphonus" },
    { "hg~",     "cephalicus" },
    { "gh<",     "pes" },
    { "hg>",     "clivis" },
    { "ghg~",    "torculus" },
    { "-gh",     "pes" },
    { "-gig",    "torculus" },
    { "-gh~",    "epiphonus" },
    { "ghoi",    "salicus" },
    { "ghOi",    "salicus" },
    { "ggf",     "pressus" },
    { "ggof",    "pressus" },
    { "gr",      "punctum" },
    { "g@h",     "pes" },
    /* ignotae: Gregorius in glyphas plures dividit, aut nomen deest */
    { "gog",     "ignota" },
    { "gg",      "ignota" },
    { "hgf",     "ignota" },
    { "hgf~",    "ignota" },
    { "ghij",    "ignota" },
    { "-hgh",    "ignota" },
    { "-ghi",    "ignota" },
    { "g-h",     "ignota" },
    { "gh~i",    "ignota" },
    { "gV",      "ignota" },
    { "goh",     "ignota" },
    { "gwhg",    "ignota" },
    { "fgwh",    "ignota" },
    { "giHGF",   "ignota" },
    { "hG",      "ignota" },
    { "gvg",     "ignota" },
    { "gR",      "ignota" },
    { "-g",      "ignota" },
    { NIHIL, NIHIL }
};

interior vacuum
_figuras_probare (
    CredoContextus* c)
{
    character fons[LXIV];
    character exspectatum[LXIV];
    character inventum[LXIV];
          i32 k;

    per (k = ZEPHYRUM; FIGURAE[k][ZEPHYRUM] != NIHIL; k++)
    {
                  GabcCantus* g;
        constans GabcSyllaba* s;
          constans character* titulus = "(syllaba deest)";

        sprintf(fons, "%%%%\n(c4)a(%s)", FIGURAE[k][ZEPHYRUM]);
        g = gabc_legere(chorda_ex_literis(fons, c->piscina),
            c->piscina);
        CREDO_NECESSE (g != NIHIL);
        si (g->numerus_syllabarum > I)
        {
            s        = &g->syllabae[I];
            titulus  = "(non neuma una)";
            si (   s->numerus_elementorum      == I
                && s->elementa[ZEPHYRUM].genus == GABC_EL_NEUMA)
            {
                titulus = gabc_figurae_titulus(
                    gabc_figura(g, &s->elementa[ZEPHYRUM]));
            }
        }
        sprintf(exspectatum, "%s -> %s", FIGURAE[k][ZEPHYRUM],
            FIGURAE[k][I]);
        sprintf(inventum, "%s -> %s", FIGURAE[k][ZEPHYRUM], titulus);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            chorda_ex_literis(inventum, c->piscina), exspectatum);
        CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    }

    {   /* elementum non neuma (clavis) -> ignota */
        GabcCantus* g = gabc_legere(
            chorda_ex_literis("%%\n(c4)a(g)", c->piscina), c->piscina);

        CREDO_NECESSE (g->syllabae[ZEPHYRUM].elementa[ZEPHYRUM].genus
            == GABC_EL_CLAVIS);
        CREDO_AEQUALIS_I32 ((i32)gabc_figura(g,
            &g->syllabae[ZEPHYRUM].elementa[ZEPHYRUM]),
            (i32)GABC_FIGURA_IGNOTA);
        CREDO_NECESSE (strcmp(gabc_figurae_titulus(GABC_FIGURA_PRESSUS),
            "pressus") == ZEPHYRUM);
    }
}

interior vacuum
_signa_opaca_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "signa_opaca.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\tsigna opaca\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\ta\n"
        "neuma\t0\t2\tclivis\n"
        "nota\te\t-5\t0\t-\t0\t0\t0\t4:8\n"
        "nota\td\t-6\t0\t-\t0\t0\t0\t4:15\n"
        "signum\t[ll:1]\t0\n"
        "spatium\tglyphae\t-\n"
        "neuma\t2\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t1\t1\t4:17\n"
        "signum\t[oh:h]\t2\n"
        "syllaba\t2\t1\tb\n"
        "neuma\t3\t1\tpunctum\n"
        "nota\tf\t-4\t0\t-\t0\t0\t0\t4:30\n"
        "signum\t|vi\t3\n"
        "neuma\t4\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:35\n"
        "signum\t|pe\t4\n"
        "syllaba\t3\t1\tc\n"
        "signum\t[nocustos]\t-1\n"
        "neuma\t5\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:53\n"
        "syllaba\t4\t1\td\n"
        "signum\t@[\t-1\n"
        "neuma\t6\t2\tpes\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:60\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:61\n"
        "signum\t]\t7\n"
        "syllaba\t5\t1\te\n"
        "signum\t{\t-1\n"
        "neuma\t8\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:4\n"
        "signum\t}\t8\n"
        "syllaba\t6\t1\tf\n"
        "signum\t[[\t-1\n"
        "neuma\t9\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:12\n"
        "signum\t]]\t9\n"
        "syllaba\t7\t1\tg\n"
        "neuma\t10\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:19\n"
        "signum\t[alt:x$]y]\t10\n"
        "syllaba\t8\t1\th\n"
        "neuma\t11\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:34\n"
        "signum\t[cs:t]\t11\n"
        "syllaba\t9\t1\ti\n"
        "signum\t<nlba>\t-1\n"
        "neuma\t12\t1\tpunctum\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:51\n"
        "signum\t</nlba>\t12\n");
}

/* Review Focus 4: forma mala quaeque -> cantus et vitia locata; BOM et
 * CRLF nihil monent et tabulam geminae purae dant */
nomen structura {
     constans character* titulus;
     constans character* vitia;
                    s32  genera[X];   /* GabcVitiumGenus, -1 finit */
} VitiumSperatum;

hic_manens constans VitiumSperatum VITIA_FORMAE[] = {
    { "parenthesis_aperta.gabc",
          "parenthesis_aperta.gabc:4:12: vitium: '(' sine ')'\n",
        { (s32)GABC_PARENTHESIS_APERTA, -I } },
    { "stilus_apertus.gabc",
          "stilus_apertus.gabc:4:6: vitium: '<i>' sine '</i>'\n",
        { (s32)GABC_SIGNUM_APERTUM, -I } },
    { "uncinus_apertus.gabc",
          "uncinus_apertus.gabc:4:9: vitium: '[' sine ']'\n",
        { (s32)GABC_SIGNUM_APERTUM, -I } },
    { "sine_separatore.gabc",
          "sine_separatore.gabc:1:1: monitum: separator '%%' deest: totum corpus putatur\n",
        { (s32)GABC_SINE_CAPITE, -I } },
    { "separator_iteratus.gabc",
          "separator_iteratus.gabc:5:1: monitum: separator '%%' iteratus: commentarium putatur\n",
        { (s32)GABC_SEPARATOR_ITERATUS, -I } },
    { "nul.gabc",
          "nul.gabc:4:9: vitium: character in notis ignotus\n",
        { (s32)GABC_NOTA_IGNOTA, -I } },
    { "valor_apertus.gabc",
          "valor_apertus.gabc:3:13: vitium: valor capitis non clausus (';;' deest)\n",
        { (s32)GABC_CAPUT_MALUM, -I } },
    { "clavis_mala.gabc",
          "clavis_mala.gabc:4:2: vitium: linea clavis extra systema\n",
        { (s32)GABC_CLAVIS_MALA, -I } },
    { "alteratio_sine_positione.gabc",
          "alteratio_sine_positione.gabc:4:8: vitium: alteratio sine littera positionis\n",
        { (s32)GABC_ALTERATIO_SINE_POSITIONE, -I } },
    { "positio_extra.gabc",
          "positio_extra.gabc:4:9: monitum: positio supra systema\n"
          "positio_extra.gabc:4:14: monitum: positio supra systema\n"
          "positio_extra.gabc:4:20: monitum: positio supra systema\n",
        { (s32)GABC_POSITIO_EXTRA, (s32)GABC_POSITIO_EXTRA,
            (s32)GABC_POSITIO_EXTRA, -I } },
    { "non_utf8.gabc",
          "non_utf8.gabc:4:7: monitum: textus non UTF-8\n",
        { (s32)GABC_NON_UTF8, -I } },
    { "signa_ignota.gabc",
          "signa_ignota.gabc:4:6: monitum: signum textus ignotum: ut textus servatur\n"
          "signa_ignota.gabc:4:14: monitum: signum notae Gregorio ignotum: opacum servatur\n"
          "signa_ignota.gabc:4:22: monitum: '<sp>' ignotum: codex ut textus servatur\n",
        { (s32)GABC_SIGNUM_IGNOTUM, (s32)GABC_SIGNUM_IGNOTUM,
            (s32)GABC_SIGNUM_IGNOTUM, -I } },
    { "signa_textus_mala.gabc",
          "signa_textus_mala.gabc:4:7: monitum: '</i>' sine '<i>'\n"
          "signa_textus_mala.gabc:4:16: monitum: '}' sine '{'\n"
          "signa_textus_mala.gabc:4:22: vitium: '{' sine '}'\n"
          "signa_textus_mala.gabc:4:29: monitum: ']' sine '['\n"
          "signa_textus_mala.gabc:4:35: vitium: '<v>' sine '</v>'\n"
          "signa_textus_mala.gabc:4:43: vitium: '<sp>' sine '</sp>'\n"
          "signa_textus_mala.gabc:4:52: vitium: '[' sine ']'\n"
          "signa_textus_mala.gabc:4:58: vitium: '<e>' sine '</e>'\n"
          "signa_textus_mala.gabc:4:69: vitium: '/[' sine ']'\n",
        { (s32)GABC_SIGNUM_IGNOTUM, (s32)GABC_SIGNUM_IGNOTUM,
            (s32)GABC_SIGNUM_APERTUM, (s32)GABC_SIGNUM_IGNOTUM,
                (s32)GABC_SIGNUM_APERTUM, (s32)GABC_SIGNUM_APERTUM,
                    (s32)GABC_SIGNUM_APERTUM, (s32)GABC_SIGNUM_APERTUM,
                        (s32)GABC_SIGNUM_APERTUM, -I } },
    { "purum.gabc",
          "",
        { -I } },
    { "bom.gabc",
          "",
        { -I } },
    { "crlf.gabc",
          "",
        { -I } },
    { NIHIL, NIHIL, { -I } }
};

interior vacuum
_vitia_formae_probare (
    CredoContextus* c)
{
                  GabcCantus* g;
                  GabcCantus* purum;
           ChordaAedificator* a;
     constans VitiumSperatum* v;
                   character  via[CCLVI];
                      chorda  textus;
                         i32  k;

    per (v = VITIA_FORMAE; v->titulus != NIHIL; v++)
    {
        strcpy(via, "vitia_formae/");
        strcat(via, v->titulus);
        g = _legere_fixum(c, via);
        CREDO_NECESSE (g != NIHIL);
        a = chorda_aedificator_creare(c->piscina, MXXIV);
        CREDO_VERUM (gabc_vitia_scribere(g, v->titulus, a));
        CREDO_CHORDA_AEQUALIS_LITERIS (chorda_aedificator_finire(a),
            v->vitia);
        per (k = ZEPHYRUM; v->genera[k] >= ZEPHYRUM; k++)
        {
            CREDO_NECESSE (k < g->numerus_vitiorum);
            CREDO_AEQUALIS_S32 ((s32)g->vitia[k].genus, v->genera[k]);
        }
        CREDO_AEQUALIS_I32 (g->numerus_vitiorum, (i32)k);
    }

    /* lectio pergit post vitium */
    g = _legere_fixum(c, "vitia_formae/parenthesis_aperta.gabc");
    CREDO_AEQUALIS_I32 (g->numerus_notarum, (i32)II);
    g = _legere_fixum(c, "vitia_formae/nul.gabc");
    CREDO_AEQUALIS_I32 (g->numerus_syllabarum, (i32)III);
    CREDO_AEQUALIS_I32 (g->numerus_notarum, (i32)III);
    g = _legere_fixum(c, "vitia_formae/separator_iteratus.gabc");
    CREDO_NECESSE (g->numerus_syllabarum == III);
    CREDO_VERUM (_textus_aequat(&g->syllabae[II], "b"));
    g = _legere_fixum(c, "vitia_formae/signa_ignota.gabc");
    CREDO_NECESSE (g->numerus_syllabarum == III);
    CREDO_VERUM (_textus_aequat(&g->syllabae[I], "<foo>a"));
    CREDO_VERUM (_textus_aequat(&g->syllabae[II], "qq"));
    CREDO_NECESSE (g->syllabae[I].numerus_elementorum == II);
    CREDO_AEQUALIS_S32 ((s32)g->syllabae[I].elementa[I].genus,
        (s32)GABC_EL_SIGNUM_OPACUM);
    g = _legere_fixum(c, "vitia_formae/signa_textus_mala.gabc");
    CREDO_NECESSE (g->numerus_syllabarum == X);
    CREDO_AEQUALIS_S32 (g->syllabae[III].vocalis_initium, (s32)I);
    CREDO_AEQUALIS_S32 (g->syllabae[III].vocalis_finis, (s32)II);
    CREDO_VERUM (_textus_aequat(&g->syllabae[IV], "e]f"));
    CREDO_VERUM (_textus_aequat(&g->syllabae[VII], ""));
    CREDO_FALSUM (g->syllabae[VII].verbum_incipit);

    /* geminae: BOM et CRLF = purum; fixa vere differunt (ne
     * normalizatio linearum probationem tacite vacuet) */
    textus = filum_legere_totum(
        "probationes/fixa/gabc/vitia_formae/crlf.gabc", c->piscina);
    CREDO_VERUM (memchr(textus.datum, '\r', (size_t)textus.mensura)
        != NIHIL);
    textus = filum_legere_totum(
        "probationes/fixa/gabc/vitia_formae/bom.gabc", c->piscina);
    CREDO_NECESSE (textus.mensura > III);
    CREDO_AEQUALIS_I32 ((i32)textus.datum[ZEPHYRUM], (i32)0xEF);
    purum = _legere_fixum(c, "vitia_formae/purum.gabc");
    CREDO_VERUM (_textus_aequat(&purum->syllabae[I], "Sal ve"));
    g = _legere_fixum(c, "vitia_formae/bom.gabc");
    CREDO_CHORDA_AEQUALIS (_tabula(c, g), _tabula(c, purum));
    g = _legere_fixum(c, "vitia_formae/crlf.gabc");
    CREDO_CHORDA_AEQUALIS (_tabula(c, g), _tabula(c, purum));
}

/* G6: totalitas - fixum quodque truncatum ad octetum quemque et MM
 * mutationibus octeti unius (sors seminata: casus iterabiles); lectio,
 * tabula, vitia numquam NIHIL aut fracta. Casus in memoria propria
 * (malloc mensurae exactae) ut ASan lectionem ultra finem videat.
 * Filius (CREDO_NON_PENDET) omnes currit - ruina aut mora = fractura;
 * deinde parens eosdem iterum iudicat (cf. probatio_musicxml_totalitas). */
hic_manens constans character* FIXA_OMNIA[] = {
    "caput.gabc",
    "syllabae.gabc",
    "neumae.gabc",
    "claves.gabc",
    "divisiones.gabc",
    "textus_sine_notis.gabc",
    "formae.gabc",
    "signa_rhythmica.gabc",
    "alterationes.gabc",
    "claves_mutatae.gabc",
    "spatia.gabc",
    "fractiones.gabc",
    "custos.gabc",
    "textus_ornatus.gabc",
    "signa_opaca.gabc",
    "vitia_formae/parenthesis_aperta.gabc",
    "vitia_formae/stilus_apertus.gabc",
    "vitia_formae/uncinus_apertus.gabc",
    "vitia_formae/sine_separatore.gabc",
    "vitia_formae/separator_iteratus.gabc",
    "vitia_formae/nul.gabc",
    "vitia_formae/valor_apertus.gabc",
    "vitia_formae/clavis_mala.gabc",
    "vitia_formae/alteratio_sine_positione.gabc",
    "vitia_formae/positio_extra.gabc",
    "vitia_formae/non_utf8.gabc",
    "vitia_formae/signa_ignota.gabc",
    "vitia_formae/signa_textus_mala.gabc",
    "vitia_formae/purum.gabc",
    "vitia_formae/bom.gabc",
    "vitia_formae/crlf.gabc",
    "../../../musica/fontes/salve_2435.gabc",
    NIHIL
};

hic_manens constans character SIGNA_GABC[] =
    "()[]{}<>|$%;:,`^/!@+-zZ\n\r\t acgfjnpxyXY#vVsoOwWrR=q~'._0158";

nomen structura {
    i32 casus_numerus;
    i32 fracti;
} Summa;

interior vacuum
_casus (
           Summa* s,
         Piscina* opus,
     constans i8* fons,
             i32  n,
             s32  locus,
              i8  octetus)
{
            GabcCantus* g;
     ChordaAedificator* a;
                chorda  d;
                    i8* b = (i8*)malloc((size_t)(n > ZEPHYRUM ? n : I));

    s->casus_numerus++;
    si (b == NIHIL)
    {
        s->fracti++;
        redde;
    }
    si (n > ZEPHYRUM)
    {
        memcpy(b, fons, (size_t)n);
    }
    si (locus >= ZEPHYRUM)
    {
        b[locus] = octetus;
    }
    d.mensura  = n;
    d.datum    = b;
    g          = gabc_legere(d, opus);
    a          = chorda_aedificator_creare(opus, MXXIV);
    si (   g == NIHIL || a == NIHIL || !gabc_tabulam_scribere(g, a)
        || !gabc_vitia_scribere(g, "x", a))
    {
        s->fracti++;
    }
    free(b);
    piscina_vacare(opus);
}

interior Summa
_omnia (vacuum)
{
       Summa  s;
     Piscina* opus;
     Piscina* fontes;
        Sors  sors;
      chorda  textus;
   character  via[CCLVI];
         i32  f;
         i32  k;

    memset(&s, ZEPHYRUM, magnitudo(s));
    opus    = piscina_generare_dynamicum("gabc_totalitas",
                  (memoriae_index)(LXIV * MXXIV));
    fontes  = piscina_generare_dynamicum("gabc_totalitas_f",
                  (memoriae_index)(XVI * MXXIV));
    si (opus == NIHIL || fontes == NIHIL)
    {
        s.fracti++;
        redde s;
    }
    sors_seminare(&sors, (i64)MMXXVI, (i64)ZEPHYRUM);
    per (f = ZEPHYRUM; FIXA_OMNIA[f] != NIHIL; f++)
    {
        strcpy(via, "probationes/fixa/gabc/");
        strcat(via, FIXA_OMNIA[f]);
        textus = filum_legere_totum(via, fontes);
        si (textus.mensura == ZEPHYRUM)
        {
            s.fracti++;
            perge;
        }
        per (k = ZEPHYRUM; k <= textus.mensura; k++)
        {
            _casus(&s, opus, textus.datum, k, -I, ZEPHYRUM);
        }
        per (k = ZEPHYRUM; k < (i32)MM; k++)
        {
            _casus(&s, opus, textus.datum, textus.mensura,
                (s32)sors_intra(&sors, textus.mensura),
                sors_intra(&sors, II) == ZEPHYRUM
                    ? (i8)sors_intra(&sors, CCLVI)
                    : (i8)SIGNA_GABC[sors_intra(&sors,
                          (i32)magnitudo(SIGNA_GABC) - I)]);
        }
        piscina_vacare(fontes);
    }
    piscina_destruere(fontes);
    piscina_destruere(opus);
    redde s;
}

interior vacuum
_totalitatem_probare (
    CredoContextus* c)
{
    Summa s;

    (vacuum)c;
    /* filius: ruina aut mora (CXX s) = fractura */
    CREDO_NON_PENDET (_omnia(), CXX * M);
    s = _omnia();
    imprimere("  gabc totalitas: casus %u, fracti %u\n",
        (insignatus integer)s.casus_numerus,
        (insignatus integer)s.fracti);
    CREDO_MAIOR_I32 (s.casus_numerus, (i32)(L * M));
    CREDO_AEQUALIS_I32 (s.fracti, ZEPHYRUM);
}

interior constans CredoSectio SECTIONES[] = {
    { "caput: paria, annotationes, valor per lineas", _caput_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "syllabae: verbum_incipit", _syllabas_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "neumae: coniunctio et gradus sub c4", _neumas_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "claves: mutatio sequentes solas resolvit", _claves_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "divisiones: genera et monitum intra neumam",
        _divisiones_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "textus sine notis, notae sine textu",
        _textum_sine_notis_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "formae notarum, vv sss, directio, signa", _formas_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "signa rhythmica: mora, episema, ictus", _signa_rhythmica_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "alterationes: ambitus (verbum, divisio, signum)",
        _alterationis_ambitum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "claves mutatae et cb", _clavem_mutatam_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "spatia: genera, nullum ante spatium", _spatia_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "fractiones et custos automaticus", _fractiones_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "custos ante clavem mutatam", _custodem_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "textus ornatus: stili, sp, translatio, vocalis, signa",
        _textum_ornatum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "signa opaca cum textu fontis et nota", _signa_opaca_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "figurae neumarum (G4): tabula, ignotae", _figuras_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitia formae: loci, BOM et CRLF tacent", _vitia_formae_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "salve_2435.gabc: sine vitio", _salve_legere_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "totalitas: truncata et mutata, sine ruina", _totalitatem_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("gabc", SECTIONES, NIHIL, NIHIL);
}
